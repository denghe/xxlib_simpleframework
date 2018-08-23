#define _CRT_SECURE_NO_WARNINGS
#include <uv.h>
#include "http_parser.h"
#include "ikcp.h"
#include "xx_uv.h"

static void* Alloc(xx::MemPool* const& mp, size_t const& size, void* const& ud = nullptr) noexcept
{
	auto p = (void**)mp->Alloc(size + sizeof(void*));
	if (!p) return nullptr;
	p[0] = ud;
	return &p[1];
}

static void Free(xx::MemPool* const& mp, void* const& p) noexcept
{
	mp->Free((void**)p - 1);
}

static void Close(uv_handle_t* const& p) noexcept
{
	if (uv_is_closing(p)) return;
	uv_close(p, [](uv_handle_t* h)
	{
		Free((xx::MemPool*)h->loop->data, h);
	});
}

static void AllocCB(uv_handle_t* h, size_t suggested_size, uv_buf_t* buf) noexcept
{
	buf->base = (char*)((xx::MemPool*)h->loop->data)->Alloc(suggested_size);
	buf->len = decltype(buf->len)(suggested_size);
}

static int TcpWrite(void* stream, char* inBuf, uint32_t len) noexcept
{
	struct write_req_t
	{
		xx::MemPool* mp;
		uv_write_t req;
		uv_buf_t buf;
	};
	auto mp = (xx::MemPool*)((uv_stream_t*)stream)->loop->data;
	auto req = (write_req_t*)mp->Alloc(sizeof(write_req_t));
	req->mp = mp;
	auto buf = (char*)mp->Alloc(len);
	memcpy(buf, inBuf, len);
	req->buf = uv_buf_init(buf, (uint32_t)len);
	return uv_write((uv_write_t*)req, (uv_stream_t*)stream, &req->buf, 1, [](uv_write_t *req, int status)
	{
		//if (status) fprintf(stderr, "Write error: %s\n", uv_strerror(status));
		auto *wr = (write_req_t*)req;
		wr->mp->Free(wr->buf.base);
		wr->mp->Free(wr);
	});
}

static int FillIP(uv_tcp_t* stream, char* buf, size_t bufLen) noexcept
{
	sockaddr_in saddr;
	int len = sizeof(saddr);
	int r = 0;
	if ((r = uv_tcp_getpeername(stream, (sockaddr*)&saddr, &len))) return r;
	if ((r = uv_ip4_name(&saddr, buf, (int)bufLen))) return r;
	auto dataLen = strlen(buf);
	return 0;
}

static int FillIP(sockaddr_in* addr, char* buf, size_t bufLen) noexcept
{
	int r = 0;
	if ((r = uv_ip4_name(addr, buf, (int)bufLen))) return r;
	auto dataLen = strlen(buf);
	sprintf(buf + dataLen, ":%d", ntohs(addr->sin_port));
	return 0;
}


xx::UvLoop::UvLoop(MemPool* const& mp)
	: Object(mp)
	, tcpListeners(mp)
	, tcpClients(mp)
	, udpListeners(mp)
	, udpClients(mp)
	, timers(mp)
	, asyncs(mp)
{
	ptr = Alloc(mp, sizeof(uv_loop_t), this);
	if (!ptr) throw - 1;
	xx::ScopeGuard sg_ptr([&] { Free(mempool, ptr); ptr = nullptr; });

	if (int r = uv_loop_init((uv_loop_t*)ptr)) throw r;

	((uv_loop_t*)ptr)->data = mp;

	sg_ptr.Cancel();
}

xx::UvLoop::~UvLoop()
{
	assert(ptr);
	udpListeners.ForEachRevert([mp = this->mempool](auto& o) { mp->Release(o); });
	udpClients.ForEachRevert([&mp = this->mempool](auto& o) { mp->Release(o); });
	tcpListeners.ForEachRevert([&mp = this->mempool](auto& o) { mp->Release(o); });
	tcpClients.ForEachRevert([&mp = this->mempool](auto& o) { mp->Release(o); });
	mempool->Release(udpTimer); udpTimer = nullptr;
	mempool->Release(timeoutManager); timeoutManager = nullptr;
	mempool->Release(rpcMgr);  rpcMgr = nullptr;
	timers.ForEachRevert([&mp = this->mempool](auto& o) { mp->Release(o); });
	asyncs.ForEachRevert([&mp = this->mempool](auto& o) { mp->Release(o); });

	if (uv_loop_close((uv_loop_t*)ptr))
	{
		uv_run((uv_loop_t*)ptr, UV_RUN_DEFAULT);
		uv_loop_close((uv_loop_t*)ptr);
	}
	Free(mempool, ptr);
	ptr = nullptr;
}

int xx::UvLoop::InitTimeoutManager(uint64_t const& intervalMS, int const& wheelLen, int const& defaultInterval) noexcept
{
	assert(!timeoutManager);
	return mempool->CreateTo(timeoutManager, *this, intervalMS, wheelLen, defaultInterval) ? 0 : -1;
}

int xx::UvLoop::InitRpcManager(uint64_t const& rpcIntervalMS, int const& rpcDefaultInterval) noexcept
{
	assert(!rpcMgr);
	return mempool->CreateTo(rpcMgr, *this, rpcIntervalMS, rpcDefaultInterval) ? 0 : -1;
}

int xx::UvLoop::InitKcpFlushInterval(uint32_t const& interval) noexcept
{
	assert(!udpTimer);
	kcpInterval = interval;
	return mempool->CreateTo(udpTimer, *this, 0, interval, [this]
	{
		auto vn = udpTimer->memHeader().versionNumber;
		udpTicks += kcpInterval;
		for (int i = (int)udpListeners.dataLen - 1; i >= 0; --i)
		{
			auto& L = udpListeners[i];
			for (auto& kv : L->peers)
			{
				kv.value->Update(udpTicks);
				if (udpTimer->IsReleased(vn)) return 0;
			}
		}
		for (int i = (int)udpClients.dataLen - 1; i >= 0; --i)
		{
			udpClients[i]->Update(udpTicks);
			if (udpTimer->IsReleased(vn)) return 0;
		}
	}) ? 0 : -1;
}

int xx::UvLoop::Run(UvRunMode const& mode) noexcept
{
	return uv_run((uv_loop_t*)ptr, (uv_run_mode)mode);
}

void xx::UvLoop::Stop() noexcept
{
	uv_stop((uv_loop_t*)ptr);
}

bool xx::UvLoop::Alive() const noexcept
{
	return uv_loop_alive((uv_loop_t*)ptr) != 0;
}

xx::UvTcpListener* xx::UvLoop::CreateTcpListener() noexcept
{
	return mempool->Create<UvTcpListener>(*this);
}
xx::UvTcpClient* xx::UvLoop::CreateTcpClient() noexcept
{
	return mempool->Create<UvTcpClient>(*this);
}

xx::UvUdpListener* xx::UvLoop::CreateUdpListener() noexcept
{
	return mempool->Create<UvUdpListener>(*this);
}
xx::UvUdpClient* xx::UvLoop::CreateUdpClient() noexcept
{
	return mempool->Create<UvUdpClient>(*this);
}

xx::UvTimer* xx::UvLoop::CreateTimer(uint64_t const& timeoutMS, uint64_t const& repeatIntervalMS, std::function<void()>&& OnFire) noexcept
{
	return mempool->Create<UvTimer>(*this, timeoutMS, repeatIntervalMS, std::move(OnFire));
}

xx::UvAsync* xx::UvLoop::CreateAsync() noexcept
{
	return mempool->Create<UvAsync>(*this);
}









xx::UvListenerBase::UvListenerBase(UvLoop& loop)
	: Object(loop.mempool)
	, loop(loop)
{
}








xx::UvTcpListener::UvTcpListener(UvLoop& loop)
	: UvListenerBase(loop)
	, peers(loop.mempool)
{
	ptr = Alloc(mempool, sizeof(uv_tcp_t), this);
	if (!ptr) throw - 1;
	xx::ScopeGuard sg_ptr([&] { Free(mempool, ptr); ptr = nullptr; });

	if (int r = uv_tcp_init((uv_loop_t*)loop.ptr, (uv_tcp_t*)ptr)) throw r;
	xx::ScopeGuard sg_ptr_init([&] { Close((uv_handle_t*)ptr); });

	addrPtr = Alloc(mempool, sizeof(sockaddr_in));
	if (!addrPtr) throw - 2;

	index_at_container = loop.tcpListeners.dataLen;
	loop.tcpListeners.Add(this);

	sg_ptr_init.Cancel();
	sg_ptr.Cancel();
}

xx::UvTcpListener::~UvTcpListener()
{
	assert(ptr);

	peers.ForEachRevert([mp = mempool](auto& o) { mp->Release(o); });
	Close((uv_handle_t*)ptr);
	ptr = nullptr;
	Free(mempool, addrPtr);
	addrPtr = nullptr;
}

void xx::UvTcpListener::OnAcceptCB(void* server, int status) noexcept
{
	if (status != 0) return;
	auto listener = *((UvTcpListener**)server - 1);
	UvTcpPeer* peer = nullptr;
	if (listener->OnCreatePeer)
	{
		auto vn = listener->memHeader().versionNumber;
		peer = listener->OnCreatePeer();
		if(listener->IsReleased(vn)) return;
	}
	else
	{
		listener->loop.mempool->CreateTo(peer, *listener);
	}
	if (peer && listener->OnAccept)
	{
		listener->OnAccept(peer);
	}
}

int xx::UvTcpListener::Bind(char const* const& ipv4, int const& port) noexcept
{
	if (int r = uv_ip4_addr(ipv4, port, (sockaddr_in*)addrPtr)) return r;
	if (int r = uv_tcp_bind((uv_tcp_t*)ptr, (sockaddr*)addrPtr, 0)) return r;
	return 0;
}

int xx::UvTcpListener::Listen(int const& backlog) noexcept
{
	return uv_listen((uv_stream_t*)ptr, backlog, (uv_connection_cb)OnAcceptCB);
}










xx::UvTimeouterBase::UvTimeouterBase(MemPool* const& mp)
	: Object(mp)
{
}

xx::UvTimeouterBase::~UvTimeouterBase()
{
	UnbindTimeoutManager();
	OnTimeout = nullptr;
}

void xx::UvTimeouterBase::TimeouterClear() noexcept
{
	timeouterPrev = nullptr;
	timeouterNext = nullptr;
	timeouterIndex = -1;
}

void xx::UvTimeouterBase::TimeoutReset(int const& interval) noexcept
{
	assert(timeoutManager);
	timeoutManager->AddOrUpdate(this, interval);
}

void xx::UvTimeouterBase::TimeouterStop() noexcept
{
	assert(timeoutManager);
	if (Timeouting()) timeoutManager->Remove(this);
}

void xx::UvTimeouterBase::BindTimeoutManager(UvTimeoutManager* const& t) noexcept
{
	assert(!timeoutManager);
	timeoutManager = t;
}

void xx::UvTimeouterBase::UnbindTimeoutManager() noexcept
{
	if (Timeouting()) timeoutManager->Remove(this);
	timeoutManager = nullptr;
}

bool xx::UvTimeouterBase::Timeouting() noexcept
{
	return timeoutManager && (timeouterIndex != -1 || timeouterPrev);
}








xx::UvTcpUdpBase::UvTcpUdpBase(UvLoop& loop)
	: UvTimeouterBase(loop.mempool)
	, routingAddress(loop.mempool)
	, senderAddress(loop.mempool)
	, loop(loop)
	, bbRecv(loop.mempool)
	, bbSend(loop.mempool)
{
}

void xx::UvTcpUdpBase::BindTimeoutManager(UvTimeoutManager* const& t) noexcept
{
	assert(!timeoutManager);
	timeoutManager = t ? t : loop.timeoutManager;
}

// 包头 = 1字节掩码 + 数据长(2/4字节) + 地址(转发) + 流水号(RPC)
// 1字节掩码:
// ......XX :   00: 一般数据包      01: RPC请求包       10: RPC回应包
// .....X.. :   0: 2字节数据长       1: 4字节数据长
// ....X... :   0: 包头中不带地址    1: 带地址(长度由 XXXX.... 部分决定, 值需要+1)
void xx::UvTcpUdpBase::ReceiveImpl(char const* const& bufPtr, int const& len) noexcept
{
	// 检测用户事件代码执行过后收包行为是否还该继续( 如果只是 client disconnect 则用 bbRecv.dataLen == 0 来检测 )
	auto vn = memHeader().versionNumber;

	bbRecv.WriteBuf(bufPtr, len);					// 追加收到的数据到接收缓冲区

	auto buf = (uint8_t*)bbRecv.buf;				// 方便使用
	size_t offset = 0;								// 独立于 bbRecv 以避免受到其负面影响
	while (offset + 3 <= bbRecv.dataLen)			// 确保 3字节 包头长度
	{
		auto typeId = buf[offset];					// 读出头
		int pkgType = 0;							// 备用

		auto dataLen = (size_t)(buf[offset + 1] + (buf[offset + 2] << 8));
		int headerLen = 3;
		if ((typeId & 0x4) > 0)                     // 大包确保 5字节 包头长度
		{
			if (offset + 5 > bbRecv.dataLen) break;
			headerLen = 5;
			dataLen += (buf[offset + 3] << 16) + (buf[offset + 4] << 24);   // 修正为大包包长
		}


		if (dataLen <= 0)                           // 数据异常
		{
			DisconnectImpl();
			return;
		}
		if (offset + headerLen + dataLen > bbRecv.dataLen) break;   // 确保数据长
		auto pkgOffset = offset;
		offset += headerLen;

		size_t addrLen = 0;
		if ((typeId & 8) > 0)                       // 转发类数据包
		{
			addrLen = ((size_t)typeId >> 4) + 1;
			auto addrOffset = offset;
			auto pkgLen = offset + dataLen - pkgOffset;

			// 进一步判断是否有设置地址信息( 如果 routingAddress 非空, 表示当前连接为数据接收方, 否则就是路由 )
			if (routingAddress.dataLen)
			{
				// 存返件地址之后 跳到正常包逻辑代码继续处理
				senderAddress.Assign((char*)buf + addrOffset, addrLen);
				goto LabAfterAddress;
			}

			bbRecv.offset = offset - headerLen;
			if (OnReceiveRouting)
			{
				OnReceiveRouting(bbRecv, pkgLen, addrOffset, addrLen);
			}
			if (IsReleased(vn) || !bbRecv.dataLen) return;
			if (Disconnected())
			{
				bbRecv.Clear();
				return;
			}

			// 路由服务直接全权处理, 不再继续抛出进阶事件, 故跳之
			goto LabEnd;
		}
		// 非转发包不含返回地址, 清空以便于在事件函数中判断来源( goto LabAfterAddress 的会跳过这步 )
		senderAddress.Clear();


	LabAfterAddress:
		bbRecv.offset = offset + addrLen;
		pkgType = typeId & 3;
		if (pkgType == 0)
		{
			if (OnReceivePackage)
			{
				OnReceivePackage(bbRecv);
			}
			if (IsReleased(vn) || !bbRecv.dataLen) return;
			if (Disconnected())
			{
				bbRecv.Clear();
				return;
			}
		}
		else
		{
			uint32_t serial = 0;
			if (bbRecv.Read(serial))
			{
				DisconnectImpl();
				return;
			}
			if (pkgType == 1)
			{
				if (OnReceiveRequest)
				{
					OnReceiveRequest(serial, bbRecv);
				}
				if (IsReleased(vn) || !bbRecv.dataLen) return;
				if (Disconnected())
				{
					bbRecv.Clear();
					return;
				}
			}
			else if (pkgType == 2)
			{
				loop.rpcMgr->Callback(serial, &bbRecv);
				if (IsReleased(vn) || !bbRecv.dataLen) return;
				if (Disconnected())
				{
					bbRecv.Clear();
					return;
				}
			}
		}

	LabEnd:
		offset += dataLen;
	}
	if (offset < bbRecv.dataLen)
	{
		memmove(buf, buf + offset, bbRecv.dataLen - offset);
	}
	bbRecv.dataLen -= offset;
	assert(bbRecv.dataLen <= len);
}

int xx::UvTcpUdpBase::SendBytes(BBuffer& bb) noexcept
{
	return SendBytes(bb.buf, (int)bb.dataLen);
}

int xx::UvTcpUdpBase::SendRoutingAddress(char const* const& buf, size_t const& len) noexcept
{
	assert(len <= 16);
	bbSend.Clear();
	bbSend.Reserve(3 + len);
	bbSend.buf[0] = 0;
	bbSend.buf[1] = (uint8_t)len;
	bbSend.buf[2] = (uint8_t)(len >> 8);
	memcpy(bbSend.buf + 3, buf, len);
	return SendBytes(bbSend.buf, (int)(3 + len));
}

size_t xx::UvTcpUdpBase::GetRoutingAddressLength(BBuffer& bb) noexcept
{
	auto p = (uint8_t*)(bb.buf + bb.offset - 2);
	return p[0] + (p[1] << 8);
}

int xx::UvTcpUdpBase::SendRoutingByRouter(xx::BBuffer& bb, size_t const& pkgLen, size_t const& addrOffset, size_t const& addrLen, char const* const& senderAddr, size_t const& senderAddrLen) noexcept
{
	// 防止误用
	assert(bb[bb.offset] & 8);

	// 如果发送方地址长与当前包地址相同, 走捷径, 直接替换地址部分即发送. 不需要再次拼接
	if (senderAddrLen == addrLen)
	{
		memcpy(bb.buf + addrOffset, senderAddr, addrLen);
		return SendBytes(bb.buf + bb.offset, (int)pkgLen);
	}

	// 否则老实拼接一次( 当然, 理论上讲调用 pair<char*, size_t>[] 的多 buf 发送版本也可以不必拼接, 这里为简化设计, 先用唯一 buf 版的函数来实现
	bbSend.Clear();
	auto newPkgLen = pkgLen - addrLen + senderAddrLen;		// 新包总长
	bbSend.Reserve(newPkgLen);

	// 用 p1 p2 定位到两个 包 的包头部位
	auto p1 = bbSend.buf;
	auto p2 = bb.buf + bb.offset;

	auto isBig = (p2[0] & 4) != 0;							// 是否大包
	size_t addr_serial_data_len = 0;						// 地址 + 流水 + 数据 的长度
	if (!isBig)
	{
		// 2字节长度
		addr_serial_data_len = (size_t)(p2[1] + (p2[2] << 8));
	}
	else
	{
		// 4字节长度
		addr_serial_data_len = (size_t)(p2[1] + (p2[2] << 8)) + (p2[3] << 16) + (p2[4] << 24);
	}
	size_t serial_data_len = addr_serial_data_len - addrLen;// 流水 + 数据 的长度

	size_t new_addr_serial_data_len = serial_data_len + senderAddrLen;	// 新包的 地址 + 流水 + 数据 的长度

																		// 如果新包加上新地址超过 2字节能表达的长度, 转为大包
	if (new_addr_serial_data_len > std::numeric_limits<uint16_t>::max())
	{
		isBig = true;
	}

	// 开始填充新包
	if (!isBig)												// 接着写长度
	{
		// 2字节长度
		p1[0] = p2[0] & 0b11111011;
		p1[1] = (uint8_t)new_addr_serial_data_len;
		p1[2] = (uint8_t)(new_addr_serial_data_len >> 8);
		memcpy(p1 + 3, senderAddr, senderAddrLen);
		memcpy(p1 + 3 + senderAddrLen, p2 + 3 + addrLen, serial_data_len);
	}
	else
	{
		// 4字节长度
		p1[0] = p2[0] | 0b00000100;
		p1[1] = (uint8_t)new_addr_serial_data_len;
		p1[2] = (uint8_t)(new_addr_serial_data_len >> 8);
		p1[3] = (uint8_t)(new_addr_serial_data_len >> 16);
		p1[4] = (uint8_t)(new_addr_serial_data_len >> 24);
		memcpy(p1 + 5, senderAddr, senderAddrLen);
		memcpy(p1 + 5 + senderAddrLen, p2 + 5 + addrLen, serial_data_len);
	}

	return SendBytes(p1, (int)newPkgLen);
}

void xx::UvTcpUdpBase::RpcTraceCallback() noexcept
{
	if (rpcSerials)
	{
		for (auto& serial : *rpcSerials)
		{
			loop.rpcMgr->Callback(serial, nullptr);
		}
		assert(rpcSerials->Empty());
	}
}

void xx::UvTcpUdpBase::DelayRelease(int const& interval) noexcept
{
	OnReceivePackage = nullptr;
	OnReceiveRequest = nullptr;
	OnReceiveRouting = nullptr;
	OnDispose = nullptr;
	if (!timeoutManager)
	{
		BindTimeoutManager();
	}
	OnTimeout = [this] { Release(); };
	TimeoutReset(interval);
}





xx::UvTcpBase::UvTcpBase(UvLoop& loop)
	: UvTcpUdpBase(loop)
{
}

void xx::UvTcpBase::OnReadCBImpl(void* stream, ptrdiff_t nread, void const* buf_t) noexcept
{
	auto tcp = *((UvTcpBase**)stream - 1);
	auto mp = tcp->loop.mempool;
	auto bufPtr = ((uv_buf_t*)buf_t)->base;
	int len = (int)nread;
	if (len > 0)
	{
		auto vn = tcp->memHeader().versionNumber;
		tcp->ReceiveImpl(bufPtr, len);
		if (tcp->IsReleased(vn))
		{
			tcp = nullptr;
		}
	}
	mp->Free(bufPtr);
	if (tcp && len < 0)
	{
		tcp->DisconnectImpl();
	}
}

int xx::UvTcpBase::SendBytes(char const* const& inBuf, int const& len) noexcept
{
	assert(addrPtr && inBuf && len);
	return TcpWrite(ptr, (char*)inBuf, len);
}

size_t xx::UvTcpBase::GetSendQueueSize() noexcept
{
	assert(addrPtr);
	return uv_stream_get_write_queue_size((uv_stream_t*)ptr);
}









xx::UvTcpPeer::UvTcpPeer(UvTcpListener & listener)
	: UvTcpBase(listener.loop)
	, listener(listener)
{
	ipBuf.fill(0);

	ptr = Alloc(loop.mempool, sizeof(uv_tcp_t), this);
	if (!ptr) throw - 1;
	xx::ScopeGuard sg_ptr([&] { Free(mempool, ptr); ptr = nullptr; });

	if (int r = uv_tcp_init((uv_loop_t*)loop.ptr, (uv_tcp_t*)ptr)) throw r;
	xx::ScopeGuard sg_ptr_init([&] { Close((uv_handle_t*)ptr); });

	if (int r = uv_accept((uv_stream_t*)listener.ptr, (uv_stream_t*)ptr)) throw r;
	if (int r = uv_read_start((uv_stream_t*)ptr, AllocCB, (uv_read_cb)OnReadCBImpl)) throw r;

	addrPtr = Alloc(loop.mempool, sizeof(sockaddr_in));
	if (!addrPtr) throw - 2;

	index_at_container = listener.peers.dataLen;
	listener.peers.Add(this);

	sg_ptr_init.Cancel();
	sg_ptr.Cancel();
}

xx::UvTcpPeer::~UvTcpPeer()
{
	assert(addrPtr);
	RpcTraceCallback();
	if (OnDispose)
	{
		OnDispose();
	}
	Close((uv_handle_t*)ptr);
	ptr = nullptr;
	Free(loop.mempool, addrPtr);
	addrPtr = nullptr;
	listener.peers[listener.peers.dataLen - 1]->index_at_container = index_at_container;
	listener.peers.SwapRemoveAt(index_at_container);
}

void xx::UvTcpPeer::DisconnectImpl() noexcept
{
	Release();
}

bool xx::UvTcpPeer::Disconnected() noexcept
{
	return false;
}

const char* xx::UvTcpPeer::Ip() noexcept
{
	assert(ptr);
	if (ipBuf[0]) return ipBuf.data();
	if (FillIP((uv_tcp_t*)ptr, ipBuf.data(), (int)ipBuf.size())) return nullptr;
	return ipBuf.data();
}









xx::UvTcpClient::UvTcpClient(UvLoop& loop)
	: UvTcpBase(loop)
{
	addrPtr = Alloc(loop.mempool, sizeof(sockaddr_in));
	if (!addrPtr) throw - 1;

	index_at_container = loop.tcpClients.dataLen;
	loop.tcpClients.Add(this);
}

xx::UvTcpClient::~UvTcpClient()
{
	Disconnect();
	if (OnDispose)
	{
		OnDispose();
	}
	Free(loop.mempool, addrPtr);
	addrPtr = nullptr;
	loop.tcpClients[loop.tcpClients.dataLen - 1]->index_at_container = index_at_container;
	loop.tcpClients.SwapRemoveAt(index_at_container);
}

bool xx::UvTcpClient::Alive() const noexcept
{
	return state == UvTcpStates::Connected;
}

int xx::UvTcpClient::SetAddress(char const* const& ipv4, int const& port) noexcept
{
	assert(addrPtr);
	return uv_ip4_addr(ipv4, port, (sockaddr_in*)addrPtr);
}

void xx::UvTcpClient::OnConnectCBImpl(void* req, int status) noexcept
{
	auto client = *((UvTcpClient**)req - 1);
	Free(client->loop.mempool, req);
	if (!client) return;
	if (status < 0)
	{
		auto vn = client->memHeader().versionNumber;
		client->Disconnect();
		if (client->IsReleased(vn)) return;
	}
	else
	{
		client->state = UvTcpStates::Connected;
		uv_read_start((uv_stream_t*)client->ptr, AllocCB, (uv_read_cb)OnReadCBImpl);
	}
	if (client->OnConnect)
	{
		client->OnConnect(status);
	}
}

int xx::UvTcpClient::Connect() noexcept
{
	assert(addrPtr && !ptr);
	if (state != UvTcpStates::Disconnected) return -1;

	ptr = Alloc(loop.mempool, sizeof(uv_tcp_t), this);
	if (!ptr) return -2;
	xx::ScopeGuard sg_ptr([&] { Free(mempool, ptr); ptr = nullptr; });

	if (int r = uv_tcp_init((uv_loop_t*)loop.ptr, (uv_tcp_t*)ptr)) return r;
	xx::ScopeGuard sg_ptr_init([&] { Close((uv_handle_t*)ptr); });

	auto req = (uv_connect_t*)Alloc(loop.mempool, sizeof(uv_connect_t), this);
	xx::ScopeGuard sg_req([&] { Free(mempool, req); req = nullptr; });

	if (int r = uv_tcp_connect((uv_connect_t*)req, (uv_tcp_t*)ptr, (sockaddr*)addrPtr, (uv_connect_cb)OnConnectCBImpl)) return r;

	state = UvTcpStates::Connecting;

	sg_req.Cancel();
	sg_ptr_init.Cancel();
	sg_ptr.Cancel();
	return 0;
}

int xx::UvTcpClient::ConnectEx(char const* const& ipv4, int const& port) noexcept
{
	Disconnect();
	int r = 0;
	if ((r = SetAddress(ipv4, port))) return r;
	return Connect();
}

void xx::UvTcpClient::Disconnect() noexcept
{
	if (!addrPtr) return;
	if (state == UvTcpStates::Disconnected) return;
	state = UvTcpStates::Disconnected;
	RpcTraceCallback();					// 有可能再次触发 Disconnect
	if (OnDisconnect)
	{
		auto vn = memHeader().versionNumber;
		OnDisconnect();
		if (IsReleased(vn)) return;
	}
	Close((uv_handle_t*)ptr);
	ptr = nullptr;
	bbSend.Clear();
	bbRecv.Clear();
}

void xx::UvTcpClient::DisconnectImpl() noexcept
{
	Disconnect();
}

bool xx::UvTcpClient::Disconnected() noexcept
{
	return ptr == nullptr;
}


// todo: 简单的变野判断去掉, 没用






xx::UvTimer::UvTimer(UvLoop& loop, uint64_t const& timeoutMS, uint64_t const& repeatIntervalMS, std::function<void()>&& OnFire)
	: Object(loop.mempool)
	, OnFire(std::move(OnFire))
	, loop(loop)
{
	ptr = Alloc(loop.mempool, sizeof(uv_timer_t), this);
	if (!ptr) throw - 1;
	xx::ScopeGuard sg_ptr([&] { Free(mempool, ptr); ptr = nullptr; });

	if (int r = uv_timer_init((uv_loop_t*)loop.ptr, (uv_timer_t*)ptr)) throw r;
	xx::ScopeGuard sg_ptr_init([&] { Close((uv_handle_t*)ptr); });

	if (int r = uv_timer_start((uv_timer_t*)ptr, (uv_timer_cb)OnTimerCBImpl, timeoutMS, repeatIntervalMS)) throw r;

	index_at_container = loop.timers.dataLen;
	loop.timers.Add(this);

	sg_ptr_init.Cancel();
	sg_ptr.Cancel();
}

xx::UvTimer::~UvTimer()
{
	assert(ptr);
	Close((uv_handle_t*)ptr);
	ptr = nullptr;
	loop.timers[loop.timers.dataLen - 1]->index_at_container = index_at_container;
	loop.timers.SwapRemoveAt(index_at_container);
}

void xx::UvTimer::OnTimerCBImpl(void* handle) noexcept
{
	auto timer = *((UvTimer**)handle - 1);
	if (timer->OnFire)
	{
		timer->OnFire();
	}
}

void xx::UvTimer::SetRepeat(uint64_t const& repeatIntervalMS) noexcept
{
	assert(ptr);
	uv_timer_set_repeat((uv_timer_t*)ptr, repeatIntervalMS);
}

int xx::UvTimer::Again() noexcept
{
	assert(ptr);
	return uv_timer_again((uv_timer_t*)ptr);
}

int xx::UvTimer::Stop() noexcept
{
	assert(ptr);
	return uv_timer_stop((uv_timer_t*)ptr);
}









xx::UvTimeoutManager::UvTimeoutManager(UvLoop& loop, uint64_t const& intervalMS, int const& wheelLen, int const& defaultInterval)
	: Object(loop.mempool)
	, timeouterss(loop.mempool)
{
	mempool->CreateTo(timer, loop, 0, intervalMS, [this] { Process(); });
	timeouterss.Resize(wheelLen);
	this->defaultInterval = defaultInterval;
}

xx::UvTimeoutManager::~UvTimeoutManager()
{
	mempool->Release(timer); timer = nullptr;
}

void xx::UvTimeoutManager::Process() noexcept
{
	auto t = timeouterss[cursor];
	while (t)
	{
		auto nt = t->timeouterNext;
		auto vn = t->memHeader().versionNumber;
		if (t->OnTimeout)
		{
			t->OnTimeout();
		}
		if (!t->IsReleased(vn))
		{
			t->TimeouterClear();
		}
		t = nt;
	};
	timeouterss[cursor] = nullptr;
	cursor++;
	if (cursor == timeouterss.dataLen) cursor = 0;
}

void xx::UvTimeoutManager::Clear() noexcept
{
	for (size_t i = 0; i < timeouterss.dataLen; ++i)
	{
		auto t = timeouterss[i];
		while (t)
		{
			auto nt = t->timeouterNext;
			t->TimeouterClear();
			t = nt;
		};
		timeouterss[i] = nullptr;
	}
	cursor = 0;
}

void xx::UvTimeoutManager::Add(UvTimeouterBase* const& t, int interval) noexcept
{
	assert(!t->Timeouting());
	auto timerssLen = (int)timeouterss.dataLen;
	assert(!(!t || (interval < 0 && interval >= timerssLen)));
	if (interval == 0) interval = defaultInterval;

	interval += cursor;
	if (interval >= timerssLen) interval -= timerssLen;

	t->timeouterPrev = nullptr;
	t->timeouterIndex = interval;
	t->timeouterNext = timeouterss[interval];
	if (t->timeouterNext)
	{
		t->timeouterNext->timeouterPrev = t;
	}
	timeouterss[interval] = t;
}

void xx::UvTimeoutManager::Remove(UvTimeouterBase* const& t) noexcept
{
	assert(t->Timeouting());
	if (t->timeouterNext) t->timeouterNext->timeouterPrev = t->timeouterPrev;
	if (t->timeouterPrev) t->timeouterPrev->timeouterNext = t->timeouterNext;
	else timeouterss[t->timeouterIndex] = t->timeouterNext;
	t->TimeouterClear();
}

void xx::UvTimeoutManager::AddOrUpdate(UvTimeouterBase* const& t, int const& interval) noexcept
{
	if (t->Timeouting()) Remove(t);
	Add(t, interval);
}











xx::UvAsync::UvAsync(UvLoop& loop)
	: Object(loop.mempool)
	, loop(loop)
	, actions(loop.mempool)
{
	ptr = Alloc(loop.mempool, sizeof(uv_async_t), this);
	if (!ptr) throw - 1;
	xx::ScopeGuard sg_ptr([&] { Free(mempool, ptr); ptr = nullptr; });

	if (int r = uv_async_init((uv_loop_t*)loop.ptr, (uv_async_t*)ptr, (uv_async_cb)OnAsyncCBImpl)) throw r;
	xx::ScopeGuard sg_ptr_init([&] { Close((uv_handle_t*)ptr); });

	OnFire = std::bind(&UvAsync::OnFireImpl, this);

	index_at_container = loop.asyncs.dataLen;
	loop.asyncs.Add(this);

	sg_ptr_init.Cancel();
	sg_ptr.Cancel();
}

xx::UvAsync::~UvAsync()
{
	assert(ptr);
	if (OnDispose)
	{
		OnDispose();
	}

	Close((uv_handle_t*)ptr);
	ptr = nullptr;

	loop.asyncs[loop.asyncs.dataLen - 1]->index_at_container = index_at_container;
	loop.asyncs.SwapRemoveAt(index_at_container);
}

void xx::UvAsync::OnAsyncCBImpl(void* handle) noexcept
{
	auto self = *((UvAsync**)handle - 1);
	if (self->OnFire)
	{
		self->OnFire();
	}
}

int xx::UvAsync::Dispatch(std::function<void()>&& a) noexcept
{
	assert(ptr);
	{
		std::scoped_lock<std::mutex> g(mtx);
		actions.Push(std::move(a));
	}
	return uv_async_send((uv_async_t*)ptr);
}

void xx::UvAsync::OnFireImpl() noexcept
{
	std::function<void()> a;
	while (true)
	{
		{
			std::scoped_lock<std::mutex> g(mtx);
			if (!actions.TryPop(a)) break;
		}
		a();
	}
}











xx::UvRpcManager::UvRpcManager(UvLoop& loop, uint64_t const& intervalMS, int const& defaultInterval)
	: Object(loop.mempool)
	, timer(nullptr)
	, mapping(loop.mempool)
	, serials(loop.mempool)
	, defaultInterval(defaultInterval)
	, serial(0)
	, ticks(0)
{
	if (defaultInterval <= 0) throw - 1;
	timer = loop.CreateTimer(0, intervalMS, [this] { Process(); });
}

xx::UvRpcManager::~UvRpcManager()
{
	if (timer)
	{
		timer->Release();
		timer = nullptr;
	}
}

void xx::UvRpcManager::Process() noexcept
{
	++ticks;
	if (serials.Empty()) return;
	while (!serials.Empty() && serials.Top().first <= ticks)
	{
		auto idx = mapping.Find(serials.Top().second);
		if (idx != -1)
		{
			auto a = std::move(mapping.ValueAt(idx));
			mapping.RemoveAt(idx);
			a(serial, nullptr);
		}
		serials.Pop();
	}
}

uint32_t xx::UvRpcManager::Register(std::function<void(uint32_t, BBuffer*)>&& cb, int interval) noexcept
{
	if (interval == 0) interval = defaultInterval;
	++serial;
	auto r = mapping.Add(serial, std::move(cb), true);
	serials.Push(std::make_pair(ticks + interval, serial));
	return serial;
}

void xx::UvRpcManager::Unregister(uint32_t const& serial) noexcept
{
	mapping.Remove(serial);
}

void xx::UvRpcManager::Callback(uint32_t const& serial, BBuffer* const& bb) noexcept
{
	int idx = mapping.Find(serial);
	if (idx == -1) return;
	auto a = std::move(mapping.ValueAt(idx));
	mapping.RemoveAt(idx);
	if (a) a(serial, bb);
}

size_t xx::UvRpcManager::Count() noexcept
{
	return serials.Count();
}










xx::UvContextBase::UvContextBase(MemPool* const& mp)
	: Object(mp)
{
}

xx::UvContextBase::~UvContextBase()
{
	KickPeer();
}

bool xx::UvContextBase::PeerAlive() noexcept
{
	return peer && !peer->Disconnected();
}

bool xx::UvContextBase::BindPeer(UvTcpUdpBase* const& p) noexcept
{
	if (peer) return false;
	p->OnReceiveRequest = std::bind(&UvContextBase::OnPeerReceiveRequest, this, std::placeholders::_1, std::placeholders::_2);
	p->OnReceivePackage = std::bind(&UvContextBase::OnPeerReceivePackage, this, std::placeholders::_1);
	p->OnDispose = std::bind(&UvContextBase::OnPeerDisconnect, this);
	peer = p;
	return true;
}

void xx::UvContextBase::KickPeer(bool const& immediately) noexcept
{
	if (peer)
	{
		peer->OnDispose = nullptr;              // 防止产生 OnDispose 调用
		peer->OnReceivePackage = nullptr;       // 清空收发包回调
		peer->OnReceiveRequest = nullptr;

		if (immediately)
		{
			peer->Release();					// 立即断开
		}
		peer = nullptr;
	}
}

void xx::UvContextBase::OnPeerReceiveRequest(uint32_t serial, BBuffer& bb) noexcept
{
	Ptr<Object> o;
	if (int r = bb.ReadRoot(o))
	{
		KickPeer();
		return;
	}
	HandleRequest(serial, o);
}

void xx::UvContextBase::OnPeerReceivePackage(BBuffer& bb) noexcept
{
	Ptr<Object> o;
	if (int r = bb.ReadRoot(o))
	{
		KickPeer();
		return;
	}
	HandlePackage(o);
}

void xx::UvContextBase::OnPeerDisconnect() noexcept
{
	KickPeer(false);
	HandleDisconnect();
}









xx::UvUdpListener::UvUdpListener(UvLoop& loop)
	: UvListenerBase(loop)
	, peers(loop.mempool)
{
	ptr = Alloc(mempool, sizeof(uv_udp_t), this);
	if (!ptr) throw - 1;
	xx::ScopeGuard sg_ptr([&] { Free(mempool, ptr); ptr = nullptr; });

	if (int r = uv_udp_init((uv_loop_t*)loop.ptr, (uv_udp_t*)ptr)) throw r;
	xx::ScopeGuard sg_ptr_init([&] { Close((uv_handle_t*)ptr); });

	addrPtr = Alloc(mempool, sizeof(sockaddr_in));
	if (!addrPtr) throw - 2;

	index_at_container = loop.udpListeners.dataLen;
	loop.udpListeners.Add(this);

	sg_ptr_init.Cancel();
	sg_ptr.Cancel();
}

xx::UvUdpListener::~UvUdpListener()
{
	assert(ptr);
	if (OnDispose)
	{
		OnDispose();
	}
	for (auto& kv : peers)
	{
		kv.value->Release();
	}
	peers.Clear();
	Close((uv_handle_t*)ptr);
	ptr = nullptr;
	Free(mempool, addrPtr);
	addrPtr = nullptr;

	loop.udpListeners[loop.udpListeners.dataLen - 1]->index_at_container = index_at_container;
	loop.udpListeners.SwapRemoveAt(index_at_container);
	index_at_container = -1;
}

void xx::UvUdpListener::OnRecvCBImpl(void* uvudp, ptrdiff_t nread, void* buf_t, void* addr, uint32_t flags) noexcept
{
	auto listener = *((UvUdpListener**)uvudp - 1);
	auto mp = listener->mempool;
	auto bufPtr = ((uv_buf_t*)buf_t)->base;
	int len = (int)nread;
	if (len > 0)
	{
		listener->OnReceiveImpl(bufPtr, len, addr);	// 这里不需要 check listener 的死活
	}
	mp->Free(bufPtr);
	//if (len < 0) return;
}

void xx::UvUdpListener::OnReceiveImpl(char const* const& bufPtr, int const& len, void* const& addr) noexcept
{
	// 所有消息长度至少都有 36 字节长( Guid conv 的 kcp 头 )
	if (len < 36) return;

	// 前 16 字节转为 Guid
	Guid g(false);
	g.Fill(bufPtr);

	// 取出版本号备用
	auto vn = memHeader().versionNumber;

	// 去字典中找. 没有就新建.
	int idx = peers.Find(g);
	UvUdpPeer* p = nullptr;
	if (idx < 0)
	{
		if (!OnCreatePeer) mempool->CreateTo(p, *this, g);
		else
		{
			p = OnCreatePeer(g);
			if (IsReleased(vn) || !p) return;
		}
		peers.Add(g, p);
	}
	else
	{
		p = peers.ValueAt(idx);
	}

	// 无脑更新 peer 的目标 ip 地址
	memcpy(p->addrPtr, addr, sizeof(sockaddr_in));

	if (idx < 0)
	{
		if (OnAccept)
		{
			OnAccept(p);
			if (IsReleased(vn)) return;
		}
	}

	// 转发到 peer 的 kcp
	if (p->Input(bufPtr, len))
	{
		p->Release();
	}
}

int xx::UvUdpListener::Bind(char const* const& ipv4, int const& port) noexcept
{
	if (int r = uv_ip4_addr(ipv4, port, (sockaddr_in*)addrPtr)) return r;
	if (int r = uv_udp_bind((uv_udp_t*)ptr, (sockaddr*)addrPtr, UV_UDP_REUSEADDR)) return r;
	return 0;
}

int xx::UvUdpListener::Listen() noexcept
{
	return uv_udp_recv_start((uv_udp_t*)ptr, AllocCB, (uv_udp_recv_cb)OnRecvCBImpl);
}

int xx::UvUdpListener::StopListen() noexcept
{
	return uv_udp_recv_stop((uv_udp_t*)ptr);
}

xx::UvUdpPeer* xx::UvUdpListener::CreatePeer(Guid const& g
	, int const& sndwnd, int const& rcvwnd
	, int const& nodelay/*, int const& interval*/, int const& resend, int const& nc, int const& minrto) noexcept
{
	return mempool->Create<xx::UvUdpPeer>(*this, g, sndwnd, rcvwnd, nodelay/*, interval*/, resend, nc, minrto);
}








xx::UvUdpBase::UvUdpBase(UvLoop& loop)
	: UvTcpUdpBase(loop)
{
}

typedef int(*KcpOutputCB)(const char *buf, int len, ikcpcb *kcp);







xx::UvUdpPeer::UvUdpPeer(UvUdpListener& listener
	, Guid const& g
	, int const& sndwnd, int const& rcvwnd
	, int const& nodelay/*, int const& interval*/, int const& resend, int const& nc, int const& minrto)
	: UvUdpBase(listener.loop)
	, listener(listener)
{
	if (!loop.kcpInterval) throw - 1;

	ipBuf.fill(0);

	ptr = ikcp_create(&g, this, mempool);
	if (!ptr) throw - 2;
	xx::ScopeGuard sg_ptr([&] { ikcp_release((ikcpcb*)ptr); ptr = nullptr; });

	int r = 0;
	if ((r = ikcp_wndsize((ikcpcb*)ptr, sndwnd, rcvwnd))
		|| (r = ikcp_nodelay((ikcpcb*)ptr, nodelay, loop.kcpInterval, resend, nc))) throw r;

	((ikcpcb*)ptr)->rx_minrto = minrto;
	ikcp_setoutput((ikcpcb*)ptr, (KcpOutputCB)OutputImpl);

	addrPtr = Alloc(mempool, sizeof(sockaddr_in));
	if (!addrPtr) throw - 3;

	index_at_container = listener.peers.Add(g, this).index;

	sg_ptr.Cancel();
}

xx::UvUdpPeer::~UvUdpPeer()
{
	RpcTraceCallback();
	if (OnDispose)
	{
		OnDispose();
	}
	Free(mempool, addrPtr);
	ikcp_release((ikcpcb*)ptr);
	ptr = nullptr;
	listener.peers.RemoveAt((int)index_at_container);
	index_at_container = -1;
}

int xx::UvUdpPeer::OutputImpl(char const* inBuf, int len, void* kcpPtr) noexcept
{
	auto peer = (UvUdpPeer*)((ikcpcb*)kcpPtr)->user;

	struct uv_udp_send_t_ex
	{
		MemPool* mp;
		uv_udp_send_t req;
		uv_buf_t buf;
	};
	auto mp = peer->mempool;
	auto req = (uv_udp_send_t_ex*)mp->Alloc(sizeof(uv_udp_send_t_ex));
	req->mp = mp;
	auto buf = (char*)mp->Alloc(len);
	memcpy(buf, inBuf, len);
	req->buf = uv_buf_init(buf, (uint32_t)len);
	return uv_udp_send((uv_udp_send_t*)req, (uv_udp_t*)peer->listener.ptr, &req->buf, 1, (sockaddr*)peer->addrPtr, [](uv_udp_send_t* req, int status)
	{
		//if (status) fprintf(stderr, "Write error: %s\n", uv_strerror(status));
		auto *wr = (uv_udp_send_t_ex*)req;
		wr->mp->Free(wr->buf.base);
		wr->mp->Free(wr);
	});
}


void xx::UvUdpPeer::Update(uint32_t const& current) noexcept
{
	if (nextUpdateTicks > current) return;
	ikcp_update((ikcpcb*)ptr, current);
	nextUpdateTicks = ikcp_check((ikcpcb*)ptr, current);

	auto vn = memHeader().versionNumber;
	do
	{
		int len = ikcp_recv((ikcpcb*)ptr, (char*)&loop.udpRecvBuf, (int)loop.udpRecvBuf.size());
		if (len <= 0) break;
		ReceiveImpl((char*)&loop.udpRecvBuf, len);
	} while (!IsReleased(vn));
}


int xx::UvUdpPeer::Input(char const* const& data, int const& len) noexcept
{
	return ikcp_input((ikcpcb*)ptr, data, len);
}

int xx::UvUdpPeer::SendBytes(char const* const& inBuf, int const& len) noexcept
{
	assert(addrPtr && inBuf && len);
	return ikcp_send((ikcpcb*)ptr, inBuf, len);
}

void xx::UvUdpPeer::DisconnectImpl() noexcept
{
	Release();
}

size_t xx::UvUdpPeer::GetSendQueueSize() noexcept
{
	//return uv_udp_get_send_queue_size((uv_udp_t*)listener.ptr);
	return ikcp_waitsnd((ikcpcb*)ptr);
}

bool xx::UvUdpPeer::Disconnected() noexcept
{
	return false;
}

char* xx::UvUdpPeer::Ip() noexcept
{
	assert(ptr);
	if (ipBuf[0]) return ipBuf.data();
	if (FillIP((sockaddr_in*)addrPtr, ipBuf.data(), ipBuf.size())) return nullptr;
	return ipBuf.data();
}









xx::UvUdpClient::UvUdpClient(UvLoop& loop)
	: UvUdpBase(loop)
{
	addrPtr = Alloc(mempool, sizeof(sockaddr_in));
	if (!addrPtr) throw - 1;

	index_at_container = loop.udpClients.dataLen;
	loop.udpClients.Add(this);
}

xx::UvUdpClient::~UvUdpClient()
{
	RpcTraceCallback();
	if (OnDispose)
	{
		OnDispose();
	}
	Disconnect();
	loop.udpClients.RemoveAt(index_at_container);
	index_at_container = (size_t)-1;
}

int xx::UvUdpClient::Connect(xx::Guid const& g
	, int const& sndwnd, int const& rcvwnd
	, int const& nodelay/*, int const& interval*/, int const& resend, int const& nc, int const& minrto) noexcept
{
	if (!loop.kcpInterval) return -1;

	if (ptr) return -2;

	ptr = Alloc(mempool, sizeof(uv_udp_t), this);
	if (!ptr) return -3;
	xx::ScopeGuard sg_ptr([&] { Free(mempool, ptr); ptr = nullptr; });

	int r = 0;
	if ((r = uv_udp_init((uv_loop_t*)loop.ptr, (uv_udp_t*)ptr))) return r;
	xx::ScopeGuard sg_ptr_init([&] { Close((uv_handle_t*)ptr); });

	if ((r = uv_udp_recv_start((uv_udp_t*)ptr, AllocCB, (uv_udp_recv_cb)OnRecvCBImpl))) return r;

	this->guid = g;
	kcpPtr = ikcp_create(&g, this, mempool);
	if (!kcpPtr) return -4;
	xx::ScopeGuard sg_kcpPtr([&] { ikcp_release((ikcpcb*)kcpPtr); kcpPtr = nullptr; });


	if ((r = ikcp_wndsize((ikcpcb*)kcpPtr, sndwnd, rcvwnd))
		|| (r = ikcp_nodelay((ikcpcb*)kcpPtr, nodelay, loop.kcpInterval, resend, nc))) return -5;

	((ikcpcb*)ptr)->rx_minrto = minrto;
	ikcp_setoutput((ikcpcb*)kcpPtr, (KcpOutputCB)OutputImpl);

	sg_kcpPtr.Cancel();
	sg_ptr_init.Cancel();
	sg_ptr.Cancel();
	return 0;
}

void xx::UvUdpClient::OnRecvCBImpl(void* uvudp, ptrdiff_t nread, void* buf_t, void* addr, uint32_t flags) noexcept
{
	auto client = *((UvUdpClient**)uvudp - 1);
	auto mp = client->mempool;
	auto bufPtr = ((uv_buf_t*)buf_t)->base;
	int len = (int)nread;
	if (len > 0)
	{
		client->OnReceiveImpl(bufPtr, len, addr);
	}
	mp->Free(bufPtr);
	//if (len < 0) return;
}

void xx::UvUdpClient::OnReceiveImpl(char const* const& bufPtr, int const& len, void* const& addr) noexcept
{
	if (len < 36) return;
	if (ikcp_input((ikcpcb*)kcpPtr, bufPtr, len))
	{
		Disconnect();
	}
}

int xx::UvUdpClient::OutputImpl(char const* inBuf, int len, void* kcpPtr) noexcept
{
	auto client = (UvUdpClient*)((ikcpcb*)kcpPtr)->user;

	struct uv_udp_send_t_ex
	{
		MemPool* mp;
		uv_udp_send_t req;
		uv_buf_t buf;
	};
	auto mp = client->mempool;
	auto req = (uv_udp_send_t_ex*)mp->Alloc(sizeof(uv_udp_send_t_ex));
	req->mp = mp;
	auto buf = (char*)mp->Alloc(len);
	memcpy(buf, inBuf, len);
	req->buf = uv_buf_init(buf, (uint32_t)len);
	return uv_udp_send((uv_udp_send_t*)req, (uv_udp_t*)client->ptr, &req->buf, 1, (sockaddr*)client->addrPtr, [](uv_udp_send_t* req, int status)
	{
		//if (status) fprintf(stderr, "Write error: %s\n", uv_strerror(status));
		auto *wr = (uv_udp_send_t_ex*)req;
		wr->mp->Free(wr->buf.base);
		wr->mp->Free(wr);
	});
}

void xx::UvUdpClient::Update(uint32_t const& current) noexcept
{
	if (nextUpdateTicks > current) return;
	ikcp_update((ikcpcb*)kcpPtr, current);
	nextUpdateTicks = ikcp_check((ikcpcb*)kcpPtr, current);

	auto vn = memHeader().versionNumber;
	do
	{
		int len = ikcp_recv((ikcpcb*)kcpPtr, (char*)&loop.udpRecvBuf, (int)loop.udpRecvBuf.size());
		if (len <= 0) break;
		ReceiveImpl((char*)&loop.udpRecvBuf, len);
	} while (!IsReleased(vn));
}

void xx::UvUdpClient::Disconnect() noexcept
{
	if (!ptr) return;

	Close((uv_handle_t*)ptr);
	ptr = nullptr;

	ikcp_release((ikcpcb*)kcpPtr);
	kcpPtr = nullptr;
}

int xx::UvUdpClient::SetAddress(char const* const& ipv4, int const& port) noexcept
{
	return uv_ip4_addr(ipv4, port, (sockaddr_in*)addrPtr);
}

int xx::UvUdpClient::SendBytes(char const* const& inBuf, int const& len) noexcept
{
	assert(addrPtr && inBuf && len);
	return ikcp_send((ikcpcb*)kcpPtr, inBuf, len);
}

void xx::UvUdpClient::DisconnectImpl() noexcept
{
	Disconnect();
}

bool xx::UvUdpClient::Disconnected() noexcept
{
	return kcpPtr != nullptr;
}

size_t xx::UvUdpClient::GetSendQueueSize() noexcept
{
	assert(kcpPtr);
	return ikcp_waitsnd((ikcpcb*)kcpPtr);
}







xx::UvHttpPeer::UvHttpPeer(UvTcpListener& listener)
	: UvTcpPeer(listener)
	, method(listener.mempool)
	, headers(listener.mempool)
	, body(listener.mempool)
	, url(listener.mempool)
	, urlDecoded(listener.mempool)
	, queries(listener.mempool)
	, status(loop.mempool)
	, lastKey(listener.mempool)
	, s(listener.mempool)
{
	OnReceiveHttp = [] {};

	parser = (http_parser*)mempool->Alloc(sizeof(http_parser));
	parser_settings = (http_parser_settings*)mempool->Alloc(sizeof(http_parser_settings));

	parser->data = this;
	http_parser_init(parser, HTTP_REQUEST);

	parser_settings->on_message_begin = [](http_parser* parser)
	{
		auto self = (UvHttpPeer*)parser->data;
		self->method.Clear();
		self->headers.Clear();
		self->keepAlive = false;
		self->body.Clear();
		self->url.Clear();
		self->urlDecoded.Clear();
		self->status.Clear();
		self->lastKey.Clear();
		self->lastValue = nullptr;
		self->s.Clear();
		return 0;
	};
	parser_settings->on_url = [](http_parser* parser, const char *buf, size_t length)
	{
		((UvHttpPeer*)parser->data)->url.AddRange(buf, length);
		return 0;
	};
	parser_settings->on_status = [](http_parser* parser, const char *buf, size_t length)
	{
		((UvHttpPeer*)parser->data)->status.AddRange(buf, length);
		return 0;
	};
	parser_settings->on_header_field = [](http_parser* parser, const char *buf, size_t length)
	{
		auto self = (UvHttpPeer*)parser->data;
		if (self->lastValue)
		{
			self->lastValue = nullptr;
		}
		self->lastKey.AddRange(buf, length);
		return 0;
	};
	parser_settings->on_header_value = [](http_parser* parser, const char *buf, size_t length)
	{
		auto self = (UvHttpPeer*)parser->data;
		if (!self->lastValue)
		{
			auto r = self->headers.Add(std::move(self->lastKey), xx::String(self->mempool));
			self->lastValue = &self->headers.ValueAt(r.index);
		}
		self->lastValue->AddRange(buf, length);
		return 0;
	};
	parser_settings->on_headers_complete = [](http_parser* parser)
	{
		auto self = (UvHttpPeer*)parser->data;
		self->lastValue = nullptr;
		self->method = http_method_str((http_method)parser->method);
		self->keepAlive = http_should_keep_alive(parser);
		return 0;
	};
	parser_settings->on_body = [](http_parser* parser, const char *buf, size_t length)
	{
		((UvHttpPeer*)parser->data)->body.AddRange(buf, length);
		return 0;
	};
	parser_settings->on_message_complete = [](http_parser* parser)
	{
		auto self = (UvHttpPeer*)parser->data;
		auto vn = self->memHeader().versionNumber;
		self->OnReceiveHttp();
		if (self->IsReleased(vn)) return -1;
		if (self->rawData)
		{
			self->rawData->Clear();
		}
		return 0;
	};
	parser_settings->on_chunk_header = [](http_parser* parser) { return 0; };
	parser_settings->on_chunk_complete = [](http_parser* parser) { return 0; };
}

xx::UvHttpPeer::~UvHttpPeer()
{
	mempool->Free(parser_settings);
	mempool->Free(parser);
}

void xx::UvHttpPeer::ReceiveImpl(char const* const& bufPtr, int const& len) noexcept
{
	if (rawData)
	{
		rawData->AddRange(bufPtr, len);	// 如果粘包, 尾部可能会多点东西出来, 当前不方便去除
	}
	auto vn = memHeader().versionNumber;
	auto parsed = http_parser_execute(parser, parser_settings, bufPtr, len);
	if (IsReleased(vn)) return;
	if (parsed < len)
	{
		if (OnError)
		{
			OnError(parser->http_errno, http_errno_description((http_errno)parser->http_errno));
			if (IsReleased(vn)) return;
		}
		Release();
	}
}

void xx::UvHttpPeer::SendHttpResponse(char const* const& bufPtr, size_t const& len) noexcept
{
	// prepare
	bbSend.Clear();

	// write prefix
	bbSend.WriteBuf(responsePartialHeader_text.data(), responsePartialHeader_text.size());

	// write len\r\n\r\n
	lastKey.Clear();
	lastKey.Append(len);
	bbSend.WriteBuf(lastKey.buf, lastKey.dataLen);
	bbSend.WriteBuf("\r\n\r\n", 4);

	// write body
	bbSend.WriteBuf(bufPtr, len);

	// send data
	SendBytes(bbSend.buf, (int)bbSend.dataLen);
}

void xx::UvHttpPeer::SendHttpResponse() noexcept
{
	SendHttpResponse(s.buf, s.dataLen);
}

inline char* FindAndTerminate(char* s, char const& c) noexcept
{
	s = strchr(s, c);
	if (!s) return nullptr;
	*s = '\0';
	return s + 1;
}


inline uint8_t FromHex(uint8_t const& c) noexcept
{
	if (c >= 'A' && c <= 'Z') return c - 'A' + 10;
	else if (c >= 'a' && c <= 'z') return c - 'a' + 10;
	else if (c >= '0' && c <= '9') return c - '0';
	else return 0;
}

inline void UrlDecode(xx::String& src, xx::String& dst) noexcept
{
	for (size_t i = 0; i < src.dataLen; i++)
	{
		if (src[i] == '+') dst.Append(' ');
		else if (src[i] == '%')
		{
			auto high = FromHex(src[++i]);
			auto low = FromHex(src[++i]);
			dst.Append((char)(uint8_t)(high * 16 + low));
		}
		else dst.Append(src[i]);
	}
}

void xx::UvHttpPeer::ParseUrl() noexcept
{
	urlDecoded.Reserve(url.dataLen);
	UrlDecode(url, urlDecoded);
	auto u = urlDecoded.c_str();

	// 从后往前扫
	fragment = FindAndTerminate(u, '#');
	auto q = FindAndTerminate(u, '?');
	path = FindAndTerminate(u, '/');

	queries.Clear();
	if (!q || '\0' == *q) return;
	queries.Reserve(16);
	int i = 0;
	queries.dataLen = i + 1;
	queries[i++].first = q;
	while (q = strchr(q, '&'))
	{
		queries.Reserve(i + 1);
		queries.dataLen = i + 1;
		*q = '\0';
		queries[i].first = ++q;
		queries[i].second = nullptr;

		if (i && (queries[i - 1].second = strchr(queries[i - 1].first, '=')))
		{
			*(queries[i - 1].second)++ = '\0';
		}
		i++;
	}
	if (queries[i - 1].second = strchr(queries[i - 1].first, '='))
	{
		*(queries[i - 1].second)++ = '\0';
	}
}






xx::UvHttpClient::UvHttpClient(UvLoop& loop)
	: UvTcpClient(loop)
	, headers(loop.mempool)
	, body(loop.mempool)
	, url(loop.mempool)
	, status(loop.mempool)
	, lastKey(loop.mempool)
{
	OnReceiveHttp = [] {};

	parser = (http_parser*)mempool->Alloc(sizeof(http_parser));
	parser_settings = (http_parser_settings*)mempool->Alloc(sizeof(http_parser_settings));

	parser->data = this;
	http_parser_init(parser, HTTP_RESPONSE);	// 这里是 response.

	parser_settings->on_message_begin = [](http_parser* parser)
	{
		auto self = (UvHttpClient*)parser->data;
		self->headers.Clear();
		self->keepAlive = false;
		self->body.Clear();
		self->url.Clear();
		self->status.Clear();
		self->lastKey.Clear();
		self->lastValue = nullptr;
		return 0;
	};
	parser_settings->on_url = [](http_parser* parser, const char *buf, size_t length)
	{
		((UvHttpClient*)parser->data)->url.AddRange(buf, length);
		return 0;
	};
	parser_settings->on_status = [](http_parser* parser, const char *buf, size_t length)
	{
		((UvHttpClient*)parser->data)->status.AddRange(buf, length);
		return 0;
	};
	parser_settings->on_header_field = [](http_parser* parser, const char *buf, size_t length)
	{
		auto self = (UvHttpClient*)parser->data;
		if (self->lastValue)
		{
			self->lastValue = nullptr;
		}
		self->lastKey.AddRange(buf, length);
		return 0;
	};
	parser_settings->on_header_value = [](http_parser* parser, const char *buf, size_t length)
	{
		auto self = (UvHttpClient*)parser->data;
		if (!self->lastValue)
		{
			auto r = self->headers.Add(std::move(self->lastKey), xx::String(self->mempool));
			self->lastValue = &self->headers.ValueAt(r.index);
		}
		self->lastValue->AddRange(buf, length);
		return 0;
	};
	parser_settings->on_headers_complete = [](http_parser* parser)
	{
		auto self = (UvHttpClient*)parser->data;
		self->lastValue = nullptr;
		self->keepAlive = http_should_keep_alive(parser);
		return 0;
	};
	parser_settings->on_body = [](http_parser* parser, const char *buf, size_t length)
	{
		((UvHttpClient*)parser->data)->body.AddRange(buf, length);
		return 0;
	};
	parser_settings->on_message_complete = [](http_parser* parser)
	{
		auto self = (UvHttpClient*)parser->data;
		auto vn = self->memHeader().versionNumber;
		self->OnReceiveHttp();
		if (self->IsReleased(vn)) return -1;
		if (self->rawData)
		{
			self->rawData->Clear();
		}
		return 0;
	};
	parser_settings->on_chunk_header = [](http_parser* parser) { return 0; };
	parser_settings->on_chunk_complete = [](http_parser* parser) { return 0; };
}

xx::UvHttpClient::~UvHttpClient()
{
	mempool->Free(parser_settings);
	mempool->Free(parser);
}

void xx::UvHttpClient::ReceiveImpl(char const* const& bufPtr, int const& len) noexcept
{
	if (rawData)
	{
		rawData->AddRange(bufPtr, len);	// 如果粘包, 尾部可能会多点东西出来, 当前不方便去除
	}
	auto vn = memHeader().versionNumber;
	auto parsed = http_parser_execute(parser, parser_settings, bufPtr, len);
	if (IsReleased(vn)) return;
	if (parsed < len)
	{
		if (OnError)
		{
			OnError(parser->http_errno, http_errno_description((http_errno)parser->http_errno));
			if (IsReleased(vn)) return;
		}
		Release();
	}
}
