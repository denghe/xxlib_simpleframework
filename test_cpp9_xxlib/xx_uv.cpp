//#include <uv.h>
//#ifdef min
//#undef min
//#endif
//#ifdef max
//#undef max
//#endif
//#include "xx_uv.h"
//
//#ifndef _countof
//template<typename T, size_t N>
//size_t _countof(T const (&arr)[N])
//{
//	return N;
//}
//#endif
//
//
//#ifndef _offsetof
//#define _offsetof(s,m) ((size_t)&reinterpret_cast<char const volatile&>((((s*)0)->m)))
//#endif
//
//
//#ifndef container_of
//#define container_of(ptr, type, member) \
//  ((type *) ((char *) (ptr) - _offsetof(type, member)))
//#endif
//
//
//static void* Alloc(xx::MemPool* mp, size_t size, void* ud)
//{
//	auto p = (void**)mp->Alloc(size + sizeof(void*));
//	p[0] = ud;
//	return &p[1];
//}
//
//static void* Alloc(xx::MemPool* mp, size_t size)
//{
//	return (void**)mp->Alloc(size + sizeof(void*)) + 1;
//}
//
//static void Free(xx::MemPool* mp, void* p) noexcept
//{
//	mp->Free((void**)p - 1);
//}
//
//static void Close(uv_handle_t* p) noexcept
//{
//	if (uv_is_closing(p)) return;
//	uv_close(p, [](uv_handle_t* h)
//	{
//		Free((xx::MemPool*)h->loop->data, h);
//	});
//}
//
//static int uv_write_(void* stream, char* inBuf, uint32_t len) noexcept
//{
//	struct write_req_t
//	{
//		xx::MemPool* mp;
//		uv_write_t req;
//		uv_buf_t buf;
//	};
//	auto mp = (xx::MemPool*)((uv_stream_t*)stream)->loop->data;
//	auto req = (write_req_t*)mp->Alloc(sizeof(write_req_t));
//	req->mp = mp;
//	auto buf = (char*)mp->Alloc(len);
//	memcpy(buf, inBuf, len);
//	req->buf = uv_buf_init(buf, (uint32_t)len);
//	return uv_write((uv_write_t*)req, (uv_stream_t*)stream, &req->buf, 1, [](uv_write_t *req, int status)
//	{
//		//if (status) fprintf(stderr, "Write error: %s\n", uv_strerror(status));
//		auto *wr = (write_req_t*)req;
//		wr->mp->Free(wr->buf.base);
//		wr->mp->Free(wr);
//	});
//}
//
//int uv_fill_client_ip(uv_tcp_t* stream, char* buf, int buf_len, int* data_len) noexcept
//{
//	sockaddr_in saddr;
//	int len = sizeof(saddr);
//	int r = 0;
//	if (r = uv_tcp_getpeername(stream, (sockaddr*)&saddr, &len)) return r;
//	if (r = uv_inet_ntop(AF_INET, &saddr.sin_addr, buf, buf_len)) return r;
//	*data_len = (int)strlen(buf);
//	*data_len += sprintf_s(buf + *data_len, buf_len - *data_len, ":%d", ntohs(saddr.sin_port));
//	return r;
//}
//
//
//
//
//
//
//
//xx::UvLoop::UvLoop(uint64_t rpcIntervalMS, int rpcDefaultInterval)
//	: tcpListeners(&mp)
//	, tcpClients(&mp)
//	, timers(&mp)
//	, asyncs(&mp)
//{
//	ptr = Alloc(&mp, sizeof(uv_loop_t), this);
//	if (!ptr) throw - 1;
//	if (int r = uv_loop_init((uv_loop_t*)ptr))
//	{
//		Free(&mp, ptr);
//		ptr = nullptr;
//		throw r;
//	}
//	((uv_loop_t*)ptr)->data = &mp;
//
//	mp.CreateTo(rpcMgr, *this, rpcIntervalMS, rpcDefaultInterval);
//}
//
//xx::UvLoop::~UvLoop()
//{
//	assert(ptr);
//	mp.Release(rpcMgr);
//	rpcMgr = nullptr;
//	tcpListeners.ForEachRevert([&mp = mp](auto& o) { mp.Release(o); });
//	tcpClients.ForEachRevert([&mp = mp](auto& o) { mp.Release(o); });
//	timers.ForEachRevert([&mp = mp](auto& o) { mp.Release(o); });
//	asyncs.ForEachRevert([&mp = mp](auto& o) { mp.Release(o); });
//
//	if (uv_loop_close((uv_loop_t*)ptr))
//	{
//		uv_run((uv_loop_t*)ptr, UV_RUN_DEFAULT);
//		uv_loop_close((uv_loop_t*)ptr);
//	}
//	Free(&mp, ptr);
//	ptr = nullptr;
//}
//
//void xx::UvLoop::Run(UvRunMode mode)
//{
//	if (int r = uv_run((uv_loop_t*)ptr, (uv_run_mode)mode)) throw r;
//}
//
//void xx::UvLoop::Stop()
//{
//	uv_stop((uv_loop_t*)ptr);
//}
//
//bool xx::UvLoop::alive() const
//{
//	return uv_loop_alive((uv_loop_t*)ptr) != 0;
//}
//
//xx::UvTcpListener* xx::UvLoop::CreateTcpListener()
//{
//	return mp.CreateNativePointer<UvTcpListener>(*this);
//}
//xx::UvTcpClient* xx::UvLoop::CreateClient()
//{
//	return mp.CreateNativePointer<UvTcpClient>(*this);
//}
//xx::UvTimer* xx::UvLoop::CreateTimer(uint64_t timeoutMS, uint64_t repeatIntervalMS, std::function<void()>&& OnFire)
//{
//	return mp.CreateNativePointer<UvTimer>(*this, timeoutMS, repeatIntervalMS, std::move(OnFire));
//}
//xx::UvAsync* xx::UvLoop::CreateAsync()
//{
//	return mp.CreateNativePointer<UvAsync>(*this);
//}
//xx::UvTimeouter* xx::UvLoop::CreateTimeouter(uint64_t intervalMS, int wheelLen, int defaultInterval)
//{
//	return mp.CreateNativePointer<UvTimeouter>(*this, intervalMS, wheelLen, defaultInterval);
//}
//
//
//
//
//
//
//
//
//xx::UvTcpListener::UvTcpListener(MemPool* mp, UvLoop& loop)
//	: Object(mp)
//	, loop(loop)
//	, peers(&loop.mp)
//{
//	ptr = Alloc(mp, sizeof(uv_tcp_t), this);
//	if (!ptr) throw - 1;
//
//	if (int r = uv_tcp_init((uv_loop_t*)loop.ptr, (uv_tcp_t*)ptr))
//	{
//		Free(mp, ptr);
//		ptr = nullptr;
//	}
//
//	addrPtr = Alloc(mp, sizeof(sockaddr_in));
//	if (!addrPtr)
//	{
//		Close((uv_handle_t*)ptr);
//		ptr = nullptr;
//		throw - 1;
//	}
//
//	index_at_container = loop.tcpListeners.dataLen;
//	loop.tcpListeners.Add(this);
//}
//
//xx::UvTcpListener::~UvTcpListener()
//{
//	assert(ptr);
//	peers.ForEachRevert([&mp = loop.mp](auto& o) { mp.Release(o); });
//	Close((uv_handle_t*)ptr);
//	ptr = nullptr;
//	Free(&loop.mp, addrPtr);
//	addrPtr = nullptr;
//}
//
//void xx::UvTcpListener::OnAcceptCB(void* server, int status)
//{
//	if (status != 0) return;
//	auto listener = *((UvTcpListener**)server - 1);
//	UvTcpPeer* peer = nullptr;
//	try
//	{
//		if (listener->OnCreatePeer) peer = listener->OnCreatePeer();
//		else listener->loop.mp.CreateTo(peer, *listener);
//	}
//	catch (...)
//	{
//		return;
//	}
//	if (peer && listener->OnAccept) listener->OnAccept(peer);
//}
//
//void xx::UvTcpListener::Listen(int backlog)
//{
//	if (int r = uv_listen((uv_stream_t*)ptr, backlog, (uv_connection_cb)OnAcceptCB)) throw r;
//}
//
//void xx::UvTcpListener::Bind(char const * const & ipv4, int port)
//{
//	if (int r = uv_ip4_addr(ipv4, port, (sockaddr_in*)addrPtr)) throw r;
//	if (int r = uv_tcp_bind((uv_tcp_t*)ptr, (sockaddr*)addrPtr, 0)) throw r;
//}
//
//
//
//
//
//
//
//
//
//xx::UvTimerBase::UvTimerBase(MemPool* mp)
//	: Object(mp)
//{
//
//}
//
//void xx::UvTimerBase::TimerClear()
//{
//	timerPrev = nullptr;
//	timerNext = nullptr;
//	timerIndex = -1;
//}
//
//void xx::UvTimerBase::TimeoutReset(int interval)
//{
//	if (!timerManager) throw - 1;
//	timerManager->AddOrUpdate(this, interval);
//}
//
//void xx::UvTimerBase::TimerStop()
//{
//	if (!timerManager) throw - 1;
//	if (timering()) timerManager->Remove(this);
//}
//
//void xx::UvTimerBase::BindTo(UvTimeouter* tm)
//{
//	if (timerManager) throw - 1;
//	timerManager = tm;
//}
//
//void xx::UvTimerBase::UnbindTimerManager()
//{
//	if (timering()) timerManager->Remove(this);
//	timerManager = nullptr;
//}
//
//bool xx::UvTimerBase::timering()
//{
//	return timerManager && (timerIndex != -1 || timerPrev);
//}
//
//
//
//
//
//
//
//
//
//
//
//
//xx::UvTimer::UvTimer(MemPool* mp, UvLoop & loop, uint64_t timeoutMS, uint64_t repeatIntervalMS, std::function<void()>&& OnFire)
//	: Object(mp)
//	, OnFire(std::move(OnFire))
//	, loop(loop)
//{
//	ptr = Alloc(&loop.mp, sizeof(uv_timer_t), this);
//	if (!ptr) throw - 1;
//
//	if (int r = uv_timer_init((uv_loop_t*)loop.ptr, (uv_timer_t*)ptr))
//	{
//		Free(&loop.mp, ptr);
//		throw r;
//	}
//
//	if (int r = uv_timer_start((uv_timer_t*)ptr, (uv_timer_cb)OnTimerCBImpl, timeoutMS, repeatIntervalMS))
//	{
//		Close((uv_handle_t*)ptr);
//		ptr = nullptr;
//		throw r;
//	}
//
//	index_at_container = loop.timers.dataLen;
//	loop.timers.Add(this);
//}
//
//xx::UvTimer::~UvTimer()
//{
//	assert(ptr);
//	Close((uv_handle_t*)ptr);
//	ptr = nullptr;
//	loop.timers.SwapRemoveAt(index_at_container);
//}
//
//void xx::UvTimer::OnTimerCBImpl(void * handle)
//{
//	auto timer = *((UvTimer**)handle - 1);
//	timer->OnFire();
//}
//
//void xx::UvTimer::SetRepeat(uint64_t repeatIntervalMS)
//{
//	if (!ptr) throw - 1;
//	uv_timer_set_repeat((uv_timer_t*)ptr, repeatIntervalMS);
//}
//
//void xx::UvTimer::Again()
//{
//	if (!ptr) throw - 1;
//	if (int r = uv_timer_again((uv_timer_t*)ptr)) throw r;
//}
//
//void xx::UvTimer::Stop()
//{
//	if (!ptr) throw - 1;
//	if (int r = uv_timer_stop((uv_timer_t*)ptr)) throw r;
//}
//
//
//
//
//
//
//
//
//
//xx::UvTimeouter::UvTimeouter(MemPool* mp, UvLoop& loop, uint64_t intervalMS, int wheelLen, int defaultInterval)
//	: Object(mp)
//	, timer(mp, loop, 0, intervalMS, [this] { Process(); })
//	, timerss(&loop.mp)
//{
//	timerss.Resize(wheelLen);
//	this->defaultInterval = defaultInterval;
//}
//
//void xx::UvTimeouter::Process()
//{
//	auto t = timerss[cursor];
//	while (t)
//	{
//		t->OnTimeout();
//		auto nt = t->timerNext;
//		t->TimerClear();
//		t = nt;
//	};
//	timerss[cursor] = nullptr;
//	cursor++;
//	if (cursor == timerss.dataLen) cursor = 0;
//}
//
//void xx::UvTimeouter::Clear()
//{
//	for (int i = 0; i < timerss.dataLen; ++i)
//	{
//		auto t = timerss[i];
//		while (t)
//		{
//			auto nt = t->timerNext;
//			t->TimerClear();
//			t = nt;
//		};
//		timerss[i] = nullptr;
//	}
//	cursor = 0;
//}
//
//void xx::UvTimeouter::Add(UvTimerBase * t, int interval)
//{
//	if (t->timering()) throw - 1;
//	auto timerssLen = (int)timerss.dataLen;
//	if (!t || (interval < 0 && interval >= timerssLen)) throw - 2;
//	if (interval == 0) interval = defaultInterval;
//
//	interval += cursor;
//	if (interval >= timerssLen) interval -= timerssLen;
//
//	t->timerPrev = nullptr;
//	t->timerIndex = interval;
//	t->timerNext = timerss[interval];
//	if (t->timerNext)
//	{
//		t->timerNext->timerPrev = t;
//	}
//	timerss[interval] = t;
//}
//
//void xx::UvTimeouter::Remove(UvTimerBase * t)
//{
//	if (!t->timering()) throw - 1;
//	if (t->timerNext) t->timerNext->timerPrev = t->timerPrev;
//	if (t->timerPrev) t->timerPrev->timerNext = t->timerNext;
//	else timerss[t->timerIndex] = t->timerNext;
//	t->TimerClear();
//}
//
//void xx::UvTimeouter::AddOrUpdate(UvTimerBase * t, int interval)
//{
//	if (t->timering()) Remove(t);
//	Add(t, interval);
//}
//
//
//
//
//
//
//
//
//
//
//xx::UvTcpBase::UvTcpBase(MemPool* mp, UvLoop& loop)
//	: UvTimerBase(mp)
//	, loop(loop)
//	, bbRecv(mp)
//	, bbSend(mp)
//	, self(this)
//{
//}
//
//void xx::UvTcpBase::OnReadCBImpl(void * stream, ptrdiff_t nread, const void * buf_t)
//{
//	auto tcp = *((UvTcpBase**)stream - 1);
//	auto mp = &tcp->loop.mp;
//	auto bufPtr = ((uv_buf_t*)buf_t)->base;
//	int len = (int)nread;
//	if (len > 0)
//	{
//		tcp->OnReceiveImpl(bufPtr, len);
//	}
//	mp->Free(bufPtr);
//	if (len < 0)
//	{
//		tcp->DisconnectImpl();
//	}
//}
//
//void xx::UvTcpBase::OnReceiveImpl(char const * bufPtr, int len)
//{
//	bbRecv.WriteBuf(bufPtr, len);
//
//	auto buf = (uint8_t*)bbRecv.buf;
//	auto dataLen = bbRecv.dataLen;
//	size_t offset = 0;
//	while (offset + 3 <= dataLen)
//	{
//		auto typeId = buf[offset];
//		auto pkgLen = buf[offset + 1] + (buf[offset + 2] << 8);
//		if (pkgLen == 0)
//		{
//			DisconnectImpl();
//			return;
//		}
//		if (offset + 3 + pkgLen > dataLen) break;
//		offset += 3;
//
//		bbRecv.offset = offset;
//		if (typeId == 0)
//		{
//			if (OnReceivePackage) OnReceivePackage(bbRecv);
//			if (!self) return;
//			if (!ptr)
//			{
//				bbRecv.Clear();
//				return;
//			}
//		}
//		else
//		{
//			uint32_t serial = 0;
//			if (bbRecv.Read(serial))
//			{
//				DisconnectImpl();
//				return;
//			}
//			if (typeId == 1)
//			{
//				if (OnReceiveRequest) OnReceiveRequest(serial, bbRecv);
//				if (!self) return;
//				if (!ptr)
//				{
//					bbRecv.Clear();
//					return;
//				}
//			}
//			else if (typeId == 2)
//			{
//				loop.rpcMgr->Callback(serial, &bbRecv);
//				if (!self) return;
//				if (!ptr)
//				{
//					bbRecv.Clear();
//					return;
//				}
//			}
//		}
//		offset += pkgLen;
//	}
//	if (offset < dataLen)
//	{
//		memmove(buf, buf + offset, dataLen - offset);
//	}
//	bbRecv.dataLen -= offset;
//}
//
//void xx::UvTcpBase::SendBytes(char * inBuf, int len)
//{
//	if (!addrPtr || !inBuf || !len) throw - 1;
//	if (int r = uv_write_(ptr, inBuf, len)) throw r;
//}
//
//void xx::UvTcpBase::SendBytes(BBuffer const & bb)
//{
//	if (!addrPtr || !bb.dataLen) throw - 1;
//	if (int r = uv_write_(ptr, bb.buf, (int)bb.dataLen)) throw r;
//}
//
//size_t xx::UvTcpBase::GetSendQueueSize()
//{
//	if (!addrPtr) throw - 1;
//	return uv_stream_get_write_queue_size((uv_stream_t*)ptr);
//}
//
//
//
//
//
//
//
//
//bool xx::UvTcpPeer::alive() const
//{
//	return ptr;
//}
//
//xx::UvTcpPeer::UvTcpPeer(MemPool* mp, UvTcpListener & listener)
//	: UvTcpBase(mp, listener.loop)
//	, listener(listener)
//{
//	ipBuf.fill(0);
//
//	ptr = Alloc(&loop.mp, sizeof(uv_tcp_t), this);
//	if (!ptr) throw - 1;
//
//	if (int r = uv_tcp_init((uv_loop_t*)loop.ptr, (uv_tcp_t*)ptr))
//	{
//		Free(&loop.mp, ptr);
//		ptr = nullptr;
//		throw r;
//	}
//
//	if (int r = uv_accept((uv_stream_t*)listener.ptr, (uv_stream_t*)ptr))
//	{
//		Close((uv_handle_t*)ptr);
//		ptr = nullptr;
//		throw r;
//	}
//
//	if (int r = uv_read_start((uv_stream_t*)ptr, [](uv_handle_t* h, size_t suggested_size, uv_buf_t* buf)
//	{
//		buf->base = (char*)((MemPool*)h->loop->data)->Alloc(suggested_size);
//		buf->len = decltype(buf->len)(suggested_size);
//	}, (uv_read_cb)OnReadCBImpl))
//	{
//		Close((uv_handle_t*)ptr);
//		ptr = nullptr;
//		throw r;
//	}
//
//	addrPtr = Alloc(&loop.mp, sizeof(sockaddr_in));
//	if (!addrPtr)
//	{
//		Close((uv_handle_t*)ptr);
//		ptr = nullptr;
//		throw - 1;
//	}
//
//	index_at_container = listener.peers.dataLen;
//	listener.peers.Add(this);
//}
//
//xx::UvTcpPeer::~UvTcpPeer()
//{
//	assert(addrPtr);
//	Dispose();
//}
//
//void xx::UvTcpPeer::DisconnectImpl()
//{
//	Dispose();
//}
//
//void xx::UvTcpPeer::Dispose()
//{
//	if (!ptr) return;
//	if (OnDispose) OnDispose();
//	Close((uv_handle_t*)ptr);
//	ptr = nullptr;
//	Free(&loop.mp, addrPtr);
//	addrPtr = nullptr;
//	UnbindTimerManager();
//	listener.peers.SwapRemoveAt(index_at_container);
//}
//
//char* xx::UvTcpPeer::ip()
//{
//	if (!ptr) throw - 1;
//	if (ipBuf[0]) return ipBuf.data();
//	int len = 0;
//	if (int r = uv_fill_client_ip((uv_tcp_t*)ptr, ipBuf.data(), (int)ipBuf.size(), &len)) throw r;
//	ipBuf[len] = 0;
//	return ipBuf.data();
//}
//
//
//
//
//
//
//
//
//
//
//
//bool xx::UvTcpClient::alive() const
//{
//	return addrPtr && state == UvTcpStates::Connected;
//}
//
//xx::UvTcpClient::UvTcpClient(MemPool* mp, UvLoop& loop)
//	: UvTcpBase(mp, loop)
//{
//	addrPtr = Alloc(&loop.mp, sizeof(sockaddr_in));
//	if (!addrPtr) throw - 1;
//
//	index_at_container = loop.tcpClients.dataLen;
//	loop.tcpClients.Add(this);
//}
//
//void xx::UvTcpClient::SetAddress(char const* const& ipv4, int port)
//{
//	if (!addrPtr) throw - 1;
//	if (int r = uv_ip4_addr(ipv4, port, (sockaddr_in*)addrPtr))throw r;
//}
//
//void xx::UvTcpClient::OnConnectCBImpl(void* req, int status)
//{
//	auto client = *((UvTcpClient**)req - 1);
//	Free(&client->loop.mp, req);
//	if (status < 0)
//	{
//		client->Disconnect();
//	}
//	else
//	{
//		client->state = UvTcpStates::Connected;
//		uv_read_start((uv_stream_t*)client->ptr, [](uv_handle_t* h, size_t suggested_size, uv_buf_t* buf)
//		{
//			buf->base = (char*)((MemPool*)h->loop->data)->Alloc(suggested_size);
//			buf->len = decltype(buf->len)(suggested_size);
//		}
//		, (uv_read_cb)OnReadCBImpl);
//	}
//	if (client->OnConnect) client->OnConnect(status);
//}
//
//void xx::UvTcpClient::Connect()
//{
//	if (!addrPtr || ptr) throw - 1;
//	if (state != UvTcpStates::Disconnected) throw - 2;
//
//	ptr = Alloc(&loop.mp, sizeof(uv_tcp_t), this);
//	if (!ptr) throw - 3;
//
//	if (int r = uv_tcp_init((uv_loop_t*)loop.ptr, (uv_tcp_t*)ptr))
//	{
//		Free(&loop.mp, ptr);
//		ptr = nullptr;
//		throw r;
//	}
//
//	auto req = (uv_connect_t*)Alloc(&loop.mp, sizeof(uv_connect_t), this);
//	if (int r = uv_tcp_connect((uv_connect_t*)req, (uv_tcp_t*)ptr, (sockaddr*)addrPtr, (uv_connect_cb)OnConnectCBImpl))
//	{
//		Free(&loop.mp, req);
//		Close((uv_handle_t*)ptr);
//		ptr = nullptr;
//		throw r;
//	}
//
//	state = UvTcpStates::Connecting;
//}
//
//void xx::UvTcpClient::Disconnect()
//{
//	if (!addrPtr) throw - 1;
//	if (state == UvTcpStates::Disconnected) return;
//	Close((uv_handle_t*)ptr);
//	ptr = nullptr;
//	state = UvTcpStates::Disconnected;
//}
//
//void xx::UvTcpClient::DisconnectImpl()
//{
//	Disconnect();
//	if (OnDisconnect) OnDisconnect();
//}
//
//void xx::UvTcpClient::Dispose()
//{
//	if (!addrPtr) return;
//	state = UvTcpStates::Disconnected;
//	if (ptr != nullptr)
//	{
//		Close((uv_handle_t*)ptr);
//		ptr = nullptr;
//	}
//	Free(&loop.mp, addrPtr);
//	addrPtr = nullptr;
//
//	UnbindTimerManager();
//	loop.tcpClients.SwapRemoveAt(index_at_container);
//}
//
//
//
//
//
//
//
//
//
//
//
//xx::UvAsync::UvAsync(MemPool* mp, UvLoop & loop)
//	: Object(mp)
//	, loop(loop)
//	, actions(mp)
//{
//	ptr = Alloc(&loop.mp, sizeof(uv_async_t), this);
//	if (!ptr) throw - 1;
//
//	if (int r = uv_async_init((uv_loop_t*)loop.ptr, (uv_async_t*)ptr, (uv_async_cb)OnAsyncCBImpl))
//	{
//		Free(&loop.mp, ptr);
//		ptr = nullptr;
//		throw r;
//	}
//
//	index_at_container = loop.asyncs.dataLen;
//	loop.asyncs.Add(this);
//}
//
//void xx::UvAsync::OnAsyncCBImpl(void * handle)
//{
//	auto self = *((UvAsync**)handle - 1);
//	self->OnFire();
//}
//
//void xx::UvAsync::Dispatch(std::function<void()>&& a)
//{
//	if (!ptr) throw - 1;
//	{
//		std::scoped_lock<std::mutex> g(mtx);
//		actions.Push(std::move(a));
//	}
//	if (int r = uv_async_send((uv_async_t*)ptr)) throw r;
//}
//
//void xx::UvAsync::OnFireImpl()
//{
//	std::function<void()> a;
//	while (true)
//	{
//		{
//			std::scoped_lock<std::mutex> g(mtx);
//			if (!actions.TryPop(a)) break;
//		}
//		a();
//	}
//}
//
//void xx::UvAsync::Dispose()
//{
//	if (!ptr) return;
//	if (OnDispose) OnDispose();
//
//	Close((uv_handle_t*)ptr);
//	ptr = nullptr;
//
//	loop.asyncs.SwapRemoveAt(index_at_container);
//}
//
//xx::UvAsync::~UvAsync()
//{
//	assert(ptr);
//	Dispose();
//}
//
//
//
//
//
//
//
//
//
//
//size_t xx::UvRpcManager::Count()
//{
//	return serials.Count();
//}
//
//xx::UvRpcManager::UvRpcManager(MemPool* mp, UvLoop& loop, uint64_t intervalMS, int defaultInterval)
//	: Object(mp)
//	, timer(nullptr)
//	, mapping(mp)
//	, serials(mp)
//	, defaultInterval(defaultInterval)
//	, serial(0)
//	, ticks(0)
//{
//	if (defaultInterval <= 0) throw - 1;
//	timer = loop.CreateTimer(0, intervalMS, [this] { Process(); });
//}
//
//xx::UvRpcManager::~UvRpcManager()
//{
//	if (timer)
//	{
//		timer->Release();
//		timer = nullptr;
//	}
//}
//
//void xx::UvRpcManager::Process()
//{
//	++ticks;
//	if (serials.Empty()) return;
//	while (!serials.Empty() && serials.Top().first <= ticks)
//	{
//		auto idx = mapping.Find(serials.Top().second);
//		if (idx != -1)
//		{
//			auto a = std::move(mapping.ValueAt(idx));
//			mapping.RemoveAt(idx);
//			a(serial, nullptr);
//		}
//		serials.Pop();
//	}
//}
//
//uint32_t xx::UvRpcManager::Register(std::function<void(uint32_t, BBuffer*)>&& cb, int interval)
//{
//	if (interval == 0) interval = defaultInterval;
//	++serial;
//	auto r = mapping.Add(serial, std::move(cb), true);
//	serials.Push(std::make_pair(ticks + interval, serial));
//	return serial;
//}
//
//void xx::UvRpcManager::Unregister(uint32_t serial)
//{
//	mapping.Remove(serial);
//}
//
//void xx::UvRpcManager::Callback(uint32_t serial, BBuffer* bb)
//{
//	int idx = mapping.Find(serial);
//	if (idx == -1) return;
//	auto a = std::move(mapping.ValueAt(idx));
//	mapping.RemoveAt(idx);
//	if (a) a(serial, bb);
//}
