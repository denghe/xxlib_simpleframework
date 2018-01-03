#include "xx_uv.h"
#include <uv.h>

#ifdef min
#undef min
#endif
#ifdef max
#undef max
#endif


#ifndef _countof
template<typename T, size_t N>
size_t _countof(T const (&arr)[N])
{
	return N;
}
#endif


#ifndef _offsetof
#define _offsetof(s,m) ((size_t)&reinterpret_cast<char const volatile&>((((s*)0)->m)))
#endif


#ifndef container_of
#define container_of(ptr, type, member) \
  ((type *) ((char *) (ptr) - _offsetof(type, member)))
#endif


inline static void* Alloc(xx::MemPool* mp, size_t size, void* ud)
{
	auto p = (void**)mp->Alloc(size + sizeof(void*));
	p[0] = ud;
	return &p[1];
}

inline static void* Alloc(xx::MemPool* mp, size_t size)
{
	return (void**)mp->Alloc(size + sizeof(void*)) + 1;
}

inline static void Free(xx::MemPool* mp, void* p) noexcept
{
	mp->Free((void**)p - 1);
}

inline static void Close(uv_handle_t* p) noexcept
{
	if (uv_is_closing(p)) return;
	uv_close(p, [](uv_handle_t* h)
	{
		Free((xx::MemPool*)h->loop->data, h);
	});
}

inline static int uv_write_(void* stream, char* inBuf, uint32_t len) noexcept
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

int uv_fill_client_ip(uv_tcp_t* stream, char* buf, int buf_len, int* data_len) noexcept
{
	sockaddr_in saddr;
	int len = sizeof(saddr);
	int r = 0;
	if (r = uv_tcp_getpeername(stream, (sockaddr*)&saddr, &len)) return r;
	if (r = uv_inet_ntop(AF_INET, &saddr.sin_addr, buf, buf_len)) return r;
	*data_len = (int)strlen(buf);
	*data_len += sprintf_s(buf + *data_len, buf_len - *data_len, ":%d", ntohs(saddr.sin_port));
	return r;
}







inline xx::UvLoop::UvLoop(uint64_t rpcIntervalMS, int rpcDefaultInterval)
	: listeners(&mp)
	, clients(&mp)
	, timers(&mp)
	, asyncs(&mp)
	, rpcMgr(&mp, *this, rpcIntervalMS, rpcDefaultInterval)
{
	ptr = Alloc(&mp, sizeof(uv_loop_t));
	if (!ptr) throw - 1;
	if (int r = uv_loop_init((uv_loop_t*)ptr))
	{
		Free(&mp, ptr);
		ptr = nullptr;
		throw r;
	}
	((uv_loop_t*)ptr)->data = &mp;
}

inline xx::UvLoop::~UvLoop()
{
	assert(ptr);
	listeners.ForEachRevert([&mp = mp](auto& o) { mp.Release(o); });
	clients.ForEachRevert([&mp = mp](auto& o) { mp.Release(o); });
	timers.ForEachRevert([&mp = mp](auto& o) { mp.Release(o); });
	asyncs.ForEachRevert([&mp = mp](auto& o) { mp.Release(o); });

	if (uv_loop_close((uv_loop_t*)ptr))
	{
		uv_run((uv_loop_t*)ptr, UV_RUN_DEFAULT);
		uv_loop_close((uv_loop_t*)ptr);
	}
	Free(&mp, ptr);
	ptr = nullptr;
}

inline void xx::UvLoop::Run(int mode)
{
	if (int r = uv_run((uv_loop_t*)ptr, (uv_run_mode)mode)) throw r;
}

inline void xx::UvLoop::Stop()
{
	uv_stop((uv_loop_t*)ptr);
}

inline bool xx::UvLoop::alive() const
{
	return uv_loop_alive((uv_loop_t*)ptr) != 0;
}


xx::UvTcpListener* xx::UvLoop::CreateTcpListener()
{
	return mp.CreateNativePointer<UvTcpListener>(*this);
}
xx::UvTcpClient* xx::UvLoop::CreateClient()
{
	return mp.CreateNativePointer<UvTcpClient>(*this);
}
xx::UvTimer* xx::UvLoop::CreateTimer(uint64_t timeoutMS, uint64_t repeatIntervalMS, std::function<void()>&& OnFire)
{
	return mp.CreateNativePointer<UvTimer>(*this, timeoutMS, repeatIntervalMS, std::move(OnFire));
}
xx::UvAsync* xx::UvLoop::CreateAsync()
{
	return mp.CreateNativePointer<UvAsync>(*this);
}






inline xx::UvTcpListener::UvTcpListener(MemPool* mp, UvLoop& loop)
	: Object(mp)
	, loop(loop)
	, peers(&loop.mp)
{
	if (int r = uv_tcp_init((uv_loop_t*)loop.ptr, (uv_tcp_t*)ptr)) throw r;
	index_at_container = loop.listeners.dataLen;
	loop.listeners.Add(this);
}

inline xx::UvTcpListener::~UvTcpListener()
{
	peers.ForEachRevert([&mp = loop.mp](auto& o) { mp.Release(o); });
	Close((uv_handle_t*)ptr);
	ptr = nullptr;
	Free(&loop.mp, addrPtr);
	addrPtr = nullptr;
}

inline void xx::UvTcpListener::OnAcceptCB(void* server, int status)
{
	if (status != 0) return;
	auto listener = *((UvTcpListener**)server - 1);
	UvTcpPeer* peer = nullptr;
	try
	{
		if (listener->OnCreatePeer) peer = listener->OnCreatePeer();
		else listener->loop.mp.CreateTo(peer, *listener);
	}
	catch (...)
	{
		return;
	}
	if (peer && listener->OnAccept) listener->OnAccept(peer);
}

inline void xx::UvTcpListener::Listen(int backlog)
{
	if (int r = uv_listen((uv_stream_t*)ptr, backlog, (uv_connection_cb)OnAcceptCB)) throw r;
}

inline void xx::UvTcpListener::Bind(char const * const & ipv4, int port)
{
	if (int r = uv_ip4_addr(ipv4, port, (sockaddr_in*)addrPtr)) throw r;
	if (int r = uv_tcp_bind((uv_tcp_t*)ptr, (sockaddr*)addrPtr, 0)) throw r;
}







inline xx::UvTimerBase::UvTimerBase(MemPool* mp)
	: Object(mp)
{

}

inline void xx::UvTimerBase::TimerClear()
{
	timerPrev = nullptr;
	timerNext = nullptr;
	timerIndex = -1;
}

inline void xx::UvTimerBase::TimeoutReset(int interval)
{
	if (!timerManager) throw - 1;
	timerManager->AddOrUpdate(this, interval);
}

inline void xx::UvTimerBase::TimerStop()
{
	if (!timerManager) throw - 1;
	if (timering()) timerManager->Remove(this);
}

inline void xx::UvTimerBase::BindTo(UvTimeouter* tm)
{
	if (timerManager) throw - 1;
	timerManager = tm;
}

inline void xx::UvTimerBase::UnbindTimerManager()
{
	if (timering()) timerManager->Remove(this);
	timerManager = nullptr;
}

inline bool xx::UvTimerBase::timering()
{
	return timerManager && (timerIndex != -1 || timerPrev);
}












inline xx::UvTimer::UvTimer(MemPool* mp, UvLoop & loop, uint64_t timeoutMS, uint64_t repeatIntervalMS, std::function<void()>&& OnFire)
	: Object(mp)
	, OnFire(std::move(OnFire))
	, loop(loop)
{
	ptr = Alloc(&loop.mp, sizeof(uv_timer_t), this);
	if (!ptr) throw - 1;

	if (int r = uv_timer_init((uv_loop_t*)loop.ptr, (uv_timer_t*)ptr))
	{
		Free(&loop.mp, ptr);
		throw r;
	}

	if (int r = uv_timer_start((uv_timer_t*)ptr, (uv_timer_cb)OnTimerCBImpl, timeoutMS, repeatIntervalMS))
	{
		Close((uv_handle_t*)ptr);
		ptr = nullptr;
		throw r;
	}

	index_at_container = loop.timers.dataLen;
	loop.timers.Add(this);
}

inline xx::UvTimer::~UvTimer()
{
	Close((uv_handle_t*)ptr);
	ptr = nullptr;
	loop.timers.SwapRemoveAt(index_at_container);
}

inline void xx::UvTimer::OnTimerCBImpl(void * handle)
{
	auto timer = *((UvTimer**)handle - 1);
	timer->OnFire();
}

inline void xx::UvTimer::SetRepeat(uint64_t repeatIntervalMS)
{
	if (!ptr) throw - 1;
	uv_timer_set_repeat((uv_timer_t*)ptr, repeatIntervalMS);
}

inline void xx::UvTimer::Again()
{
	if (!ptr) throw - 1;
	if (int r = uv_timer_again((uv_timer_t*)ptr)) throw r;
}

inline void xx::UvTimer::Stop()
{
	if (!ptr) throw - 1;
	if (int r = uv_timer_stop((uv_timer_t*)ptr)) throw r;
}









inline xx::UvTimeouter::UvTimeouter(MemPool* mp, UvLoop& loop, uint64_t intervalMS, int wheelLen, int defaultInterval)
	: Object(mp)
	, timer(mp, loop, 0, intervalMS, [this] { Process(); })
	, timerss(&loop.mp)
{
	timerss.Resize(wheelLen);
	this->defaultInterval = defaultInterval;
}

inline void xx::UvTimeouter::Process()
{
	auto t = timerss[cursor];
	while (t)
	{
		t->OnTimeout();
		auto nt = t->timerNext;
		t->TimerClear();
		t = nt;
	};
	timerss[cursor] = nullptr;
	cursor++;
	if (cursor == timerss.dataLen) cursor = 0;
}

inline void xx::UvTimeouter::Clear()
{
	for (int i = 0; i < timerss.dataLen; ++i)
	{
		auto t = timerss[i];
		while (t)
		{
			auto nt = t->timerNext;
			t->TimerClear();
			t = nt;
		};
		timerss[i] = nullptr;
	}
	cursor = 0;
}

inline void xx::UvTimeouter::Add(UvTimerBase * t, int interval)
{
	if (t->timering()) throw - 1;
	auto timerssLen = (int)timerss.dataLen;
	if (!t || (interval < 0 && interval >= timerssLen)) throw - 2;
	if (interval == 0) interval = defaultInterval;

	interval += cursor;
	if (interval >= timerssLen) interval -= timerssLen;

	t->timerPrev = nullptr;
	t->timerIndex = interval;
	t->timerNext = timerss[interval];
	if (t->timerNext)
	{
		t->timerNext->timerPrev = t;
	}
	timerss[interval] = t;
}

inline void xx::UvTimeouter::Remove(UvTimerBase * t)
{
	if (!t->timering()) throw - 1;
	if (t->timerNext) t->timerNext->timerPrev = t->timerPrev;
	if (t->timerPrev) t->timerPrev->timerNext = t->timerNext;
	else timerss[t->timerIndex] = t->timerNext;
	t->TimerClear();
}

inline void xx::UvTimeouter::AddOrUpdate(UvTimerBase * t, int interval)
{
	if (t->timering()) Remove(t);
	Add(t, interval);
}










inline xx::UvTcpBase::UvTcpBase(MemPool* mp, UvLoop& loop)
	: UvTimerBase(mp)
	, loop(loop)
	, bbRecv(mp)
	, bbSend(mp)
{
}

inline void xx::UvTcpBase::OnReadCBImpl(void * stream, ptrdiff_t nread, const void * buf_t)
{
	auto tcp = *((UvTcpBase**)stream - 1);
	auto mp = &tcp->loop.mp;
	auto bufPtr = ((uv_buf_t*)buf_t)->base;
	int len = (int)nread;
	if (len > 0)
	{
		tcp->OnReceiveImpl(bufPtr, len);
	}
	mp->Free(bufPtr);
	if (len < 0)
	{
		tcp->DisconnectImpl();
	}
}

inline void xx::UvTcpBase::OnReceiveImpl(char const * bufPtr, int len)
{
	bbRecv.WriteBuf(bufPtr, len);

	auto buf = (uint8_t*)bbRecv.buf;
	auto dataLen = bbRecv.dataLen;
	size_t offset = 0;
	while (offset + 3 <= dataLen)
	{
		auto typeId = buf[offset];
		auto pkgLen = buf[offset + 1] + (buf[offset + 2] << 8);
		if (pkgLen == 0)
		{
			DisconnectImpl();
			return;
		}
		if (offset + 3 + pkgLen > dataLen) break;
		offset += 3;

		bbRecv.offset = offset;
		if (typeId == 0)
		{
			if (OnReceivePackage) OnReceivePackage(bbRecv);
			if (!ptr) return;
		}
		else
		{
			uint32_t serial = 0;
			if (bbRecv.Read(serial))
			{
				DisconnectImpl();
				return;
			}
			if (typeId == 1)
			{
				if (OnReceiveRequest) OnReceiveRequest(serial, bbRecv);
				if (!ptr) return;
			}
			else if (typeId == 2)
			{
				loop.rpcMgr.Callback(serial, &bbRecv);
				if (!ptr) return;
			}
		}
		offset += pkgLen;
	}
	if (offset < dataLen)
	{
		memmove(buf, buf + offset, dataLen - offset);
	}
	bbRecv.dataLen -= offset;
}

inline void xx::UvTcpBase::SendBytes(char * inBuf, int len)
{
	if (!ptr || !inBuf || !len) throw - 1;
	if (int r = uv_write_(ptr, inBuf, len)) throw r;
}

inline void xx::UvTcpBase::SendBytes(BBuffer const & bb)
{
	if (!ptr || !bb.dataLen) throw - 1;
	if (int r = uv_write_(ptr, bb.buf, (int)bb.dataLen)) throw r;
}











inline bool xx::UvTcpPeer::alive() const
{
	return ptr;
}

inline xx::UvTcpPeer::UvTcpPeer(MemPool* mp, UvTcpListener & listener)
	: UvTcpBase(mp, listener.loop)
	, listener(listener)
{
	ipBuf.fill(0);

	ptr = Alloc(&loop.mp, sizeof(uv_tcp_t), this);
	if (!ptr) throw - 1;

	if (int r = uv_tcp_init((uv_loop_t*)loop.ptr, (uv_tcp_t*)ptr))
	{
		Free(&loop.mp, ptr);
		ptr = nullptr;
		throw r;
	}

	if (int r = uv_accept((uv_stream_t*)listener.ptr, (uv_stream_t*)ptr))
	{
		Close((uv_handle_t*)ptr);
		ptr = nullptr;
		throw r;
	}

	if (int r = uv_read_start((uv_stream_t*)ptr, [](uv_handle_t* h, size_t suggested_size, uv_buf_t* buf)
	{
		buf->base = (char*)((MemPool*)h->loop->data)->Alloc(suggested_size);
		buf->len = decltype(buf->len)(suggested_size);
	}, (uv_read_cb)OnReadCBImpl))
	{
		Close((uv_handle_t*)ptr);
		ptr = nullptr;
		throw r;
	}

	addrPtr = Alloc(&loop.mp, sizeof(sockaddr_in));
	if (!addrPtr)
	{
		Close((uv_handle_t*)ptr);
		ptr = nullptr;
		throw - 1;
	}

	index_at_container = listener.peers.dataLen;
	listener.peers.Add(this);
}

inline xx::UvTcpPeer::~UvTcpPeer()
{
	Dispose();
}

inline void xx::UvTcpPeer::DisconnectImpl()
{
	Dispose();
}

inline void xx::UvTcpPeer::Dispose()
{
	if (!ptr) return;
	Close((uv_handle_t*)ptr);
	ptr = nullptr;
	Free(&loop.mp, addrPtr);
	addrPtr = nullptr;
	UnbindTimerManager();
	listener.peers.SwapRemoveAt(index_at_container);
}

inline char* xx::UvTcpPeer::ip()
{
	if (!ptr) throw - 1;
	if (ipBuf[0]) return ipBuf.data();
	int len = 0;
	if (int r = uv_fill_client_ip((uv_tcp_t*)ptr, ipBuf.data(), (int)ipBuf.size(), &len)) throw r;
	ipBuf[len] = 0;
	return ipBuf.data();
}











inline bool xx::UvTcpClient::alive() const
{
	return ptr && state == UvTcpStates::Connected;
}

inline xx::UvTcpClient::UvTcpClient(MemPool* mp, UvLoop& loop)
	: UvTcpBase(mp, loop)
{
	addrPtr = Alloc(&loop.mp, sizeof(sockaddr_in));
	if (!addrPtr) throw - 1;

	index_at_container = loop.clients.dataLen;
	loop.clients.Add(this);
}

inline void xx::UvTcpClient::SetAddress(char const* const& ipv4, int port)
{
	if (!ptr) throw - 1;
	if (int r = uv_ip4_addr(ipv4, port, (sockaddr_in*)addrPtr))throw r;
}

inline void xx::UvTcpClient::OnConnectCBImpl(void* req, int status)
{
	auto client = *((UvTcpClient**)req - 1);
	Free(&client->loop.mp, req);
	if (status < 0)
	{
		client->Disconnect();
	}
	else
	{
		client->state = UvTcpStates::Connected;
		uv_read_start((uv_stream_t*)client->ptr, [](uv_handle_t* h, size_t suggested_size, uv_buf_t* buf)
		{
			buf->base = (char*)((MemPool*)h->loop->data)->Alloc(suggested_size);
			buf->len = decltype(buf->len)(suggested_size);
		}
		, (uv_read_cb)OnReadCBImpl);
	}
	if (client->OnConnect) client->OnConnect(status);
}

inline void xx::UvTcpClient::Connect()
{
	if (!ptr) throw - 1;
	if (state != UvTcpStates::Disconnected) throw - 2;

	ptr = Alloc(&loop.mp, sizeof(uv_tcp_t), this);
	if (!ptr) throw - 3;

	if (int r = uv_tcp_init((uv_loop_t*)loop.ptr, (uv_tcp_t*)ptr))
	{
		Free(&loop.mp, ptr);
		ptr = nullptr;
		throw r;
	}

	auto req = (uv_connect_t*)Alloc(&loop.mp, sizeof(uv_connect_t));
	if (int r = uv_tcp_connect((uv_connect_t*)req, (uv_tcp_t*)ptr, (sockaddr*)addrPtr, (uv_connect_cb)OnConnectCBImpl))
	{
		Free(&loop.mp, req);
		Close((uv_handle_t*)ptr);
		ptr = nullptr;
		throw r;
	}

	state = UvTcpStates::Connecting;
}

inline void xx::UvTcpClient::Disconnect()
{
	if (!ptr) throw - 1;
	if (state == UvTcpStates::Disconnected) return;
	Close((uv_handle_t*)ptr);
	ptr = nullptr;
	state = UvTcpStates::Disconnected;
}

inline void xx::UvTcpClient::DisconnectImpl()
{
	Disconnect();
	if (OnDisconnect) OnDisconnect();
}

inline void xx::UvTcpClient::Dispose()
{
	if (!ptr) return;
	state = UvTcpStates::Disconnected;
	if (ptr != nullptr)
	{
		Close((uv_handle_t*)ptr);
		ptr = nullptr;
	}
	Free(&loop.mp, addrPtr);
	addrPtr = nullptr;

	UnbindTimerManager();
	loop.clients.SwapRemoveAt(index_at_container);
}











inline xx::UvAsync::UvAsync(MemPool* mp, UvLoop & loop)
	: Object(mp)
	, loop(loop)
	, actions(mp)
{
	ptr = Alloc(&loop.mp, sizeof(uv_async_t), this);
	if (!ptr) throw - 1;

	if (int r = uv_async_init((uv_loop_t*)loop.ptr, (uv_async_t*)ptr, (uv_async_cb)OnAsyncCBImpl))
	{
		Free(&loop.mp, ptr);
		ptr = nullptr;
		throw r;
	}

	index_at_container = loop.asyncs.dataLen;
	loop.asyncs.Add(this);
}

inline void xx::UvAsync::OnAsyncCBImpl(void * handle)
{
	auto self = *((UvAsync**)handle - 1);
	self->OnFire();
}

inline void xx::UvAsync::Dispatch(std::function<void()>&& a)
{
	if (!ptr) throw - 1;
	{
		std::scoped_lock<std::mutex> g(mtx);
		actions.Push(std::move(a));
	}
	if (int r = uv_async_send((uv_async_t*)ptr)) throw r;
}

inline void xx::UvAsync::OnFireImpl()
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

inline void xx::UvAsync::Dispose()
{
	if (!ptr) return;
	if (OnDispose) OnDispose();

	Close((uv_handle_t*)ptr);
	ptr = nullptr;

	loop.asyncs.SwapRemoveAt(index_at_container);
}

inline xx::UvAsync::~UvAsync()
{
	Dispose();
}










inline size_t xx::UvRpcManager::Count()
{
	return serials.Count();
}

inline xx::UvRpcManager::UvRpcManager(MemPool* mp, UvLoop& loop, uint64_t intervalMS, int defaultInterval)
	: Object(mp)
	, timer(mp, loop, 0, intervalMS, [this] { Process(); })
	, mapping(mp)
	, serials(mp)
	, defaultInterval(defaultInterval)
	, serial(0)
	, ticks(0)
{
	if (defaultInterval <= 0) throw - 1;
}

inline void xx::UvRpcManager::Process()
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

inline uint32_t xx::UvRpcManager::Register(std::function<void(uint32_t, BBuffer*)>&& cb, int interval)
{
	if (interval == 0) interval = defaultInterval;
	++serial;
	auto r = mapping.Add(serial, std::move(cb), true);
	serials.Push(std::make_pair(ticks + interval, serial));
	return serial;
}

inline void xx::UvRpcManager::Unregister(uint32_t serial)
{
	mapping.Remove(serial);
}

inline void xx::UvRpcManager::Callback(uint32_t serial, BBuffer* bb)
{
	int idx = mapping.Find(serial);
	if (idx == -1) return;
	auto a = std::move(mapping.ValueAt(idx));
	mapping.RemoveAt(idx);
	if (a) a(serial, bb);
}
