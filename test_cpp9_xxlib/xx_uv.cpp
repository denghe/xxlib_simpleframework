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


// 分配内存时保留一个头空间并填充
inline static void* Alloc(size_t size, void* ud)
{
	auto p = (void**)malloc(size + sizeof(void*));
	p[0] = ud;
	return &p[1];
}

// 容忍浪费, 简化 Free 流程
inline static void* Alloc(size_t size)
{
	return (void**)malloc(size + sizeof(void*)) + 1;
}

// 还原真实指针, 释放
inline static void Free(void* p) noexcept
{
	free((void**)p - 1);
}

inline static void Close(uv_handle_t* p) noexcept
{
	if (uv_is_closing(p)) return;
	uv_close(p, [](uv_handle_t* h)
	{
		Free(h);
	});
}

int xx::uv_write_(xx::MemPool* mp, void* stream, char* inBuf, unsigned int offset, unsigned int len) noexcept
{
	struct write_req_t
	{
		xx::MemPool* mp;
		uv_write_t req;
		uv_buf_t buf;
	};
	auto req = (write_req_t*)mp->Alloc(sizeof(write_req_t));
	req->mp = mp;
	auto buf = (char*)mp->Alloc(len);
	memcpy(buf, inBuf + offset, len);
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
	//, clients(&mp)
	//, timers(&mp)
	//, asyncs(&mp)
	//, rpcMgr(&mp)
{
	ptr = Alloc(sizeof(uv_loop_t), this);
	if (!ptr) throw - 1;
	if (int r = uv_loop_init((uv_loop_t*)ptr))
	{
		Free(ptr);
		throw r;
	}
}

inline xx::UvLoop::~UvLoop()
{
	assert(ptr);
	//listeners.ForEachRevert([](auto& o) { o.Release(); });
	//clients.ForEachRevert([](auto& o) { o.Release(); });
	//timers.ForEachRevert([](auto& o) { o.Release(); });
	//asyncs.ForEachRevert([](auto& o) { o.Release(); });

	if (uv_loop_close((uv_loop_t*)ptr))
	{
		uv_run((uv_loop_t*)ptr, UV_RUN_DEFAULT);
		uv_loop_close((uv_loop_t*)ptr);
	}
	Free(ptr);
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

inline bool xx::UvLoop::alive()
{
	return uv_loop_alive((uv_loop_t*)ptr) != 0;
}








inline xx::UvTcpListener::UvTcpListener(UvLoop& loop)
	: loop(loop)
{
	if (int r = uv_tcp_init((uv_loop_t*)loop.ptr, (uv_tcp_t*)ptr)) throw r;
	index_at_container = loop.listeners.dataLen;
	loop.listeners.Add(this);
}

inline xx::UvTcpListener::~UvTcpListener()
{
	//peers.ForEachRevert([](auto& o) { o.Release(); });
	Close((uv_handle_t*)ptr);
	ptr = nullptr;
	Free(addrPtr);
	addrPtr = nullptr;
}

inline void xx::UvTcpListener::OnAcceptCB(void* server, int status)
{
	if (status != 0) return;
	auto listener = container_of(server, UvTcpListener, ptr);
	//Ptr<UvTcpPeer> peer;
	//try
	//{
	//	if (listener->OnCreatePeer) peer = listener->OnCreatePeer();
	//	else mempool->CreateTo(peer, listener);
	//}
	//catch
	//{
	//	return;
	//}
	//if (listener->OnAccept) listener->OnAccept(peer);
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









inline void xx::UvTimerBase::TimerClear()
{
	timerPrev = nullptr;
	timerNext = nullptr;
	timerIndex = -1;
}

inline void xx::UvTimerBase::TimeoutReset(int interval)
{
	if (!timerManager) throw - 1;
	//timerManager->AddOrUpdate(this, interval);
}

inline void xx::UvTimerBase::TimerStop()
{
	if (!timerManager) throw - 1;
	//if (timering) timerManager->Remove(this);
}

inline void xx::UvTimerBase::BindTo(UvTimeouter* tm)
{
	if (timerManager) throw - 1;
	timerManager = tm;
}

inline void xx::UvTimerBase::UnbindTimerManager()
{
	if (timering()) timerManager->Remove(this);
	//timerManager->Release();
}

inline bool xx::UvTimerBase::timering()
{
	return timerManager && (timerIndex != -1 || timerPrev);
}












inline xx::UvTimer::UvTimer(UvLoop & loop, uint64_t timeoutMS, uint64_t repeatIntervalMS, std::function<void()>&& OnFire)
	: loop(loop)
{
	//: this(loop, timeoutMS, repeatIntervalMS)
	//this.OnFire = OnFire;

	ptr = Alloc(sizeof(uv_timer_t), this);
	if (!ptr) throw - 1;

	if (int r = uv_timer_init((uv_loop_t*)loop.ptr, (uv_timer_t*)ptr))
	{
		Free(ptr);
		throw r;
	}

	if (int r = uv_timer_start((uv_timer_t*)ptr, (uv_timer_cb)OnTimerCBImpl, timeoutMS, repeatIntervalMS))
	{
		Close((uv_handle_t*)ptr);
		ptr = nullptr;
		throw r;
	}

	//index_at_container = loop.timers.dataLen;
	//loop.timers.Add(this);
}

inline xx::UvTimer::~UvTimer()
{
	Close((uv_handle_t*)ptr);
	ptr = nullptr;
	//loop.timers.SwapRemoveAt(index_at_container);
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









inline xx::UvTimeouter::UvTimeouter(UvLoop& loop, uint64_t intervalMS, int wheelLen, int defaultInterval)
	: timer(loop, 0, intervalMS, [this] { Process(); })
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

	// 环形定位到 timers 下标
	interval += cursor;
	if (interval >= timerssLen) interval -= timerssLen;

	// 填充 链表信息
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










inline xx::UvTcpBase::UvTcpBase(UvLoop& loop)
	: loop(loop)
	, bbRecv(&loop.mp)
	, bbSend(&loop.mp)
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
				//loop.rpcMgr->Callback(serial, bbRecv);
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
	if (int r = uv_write_(&loop.mp, ptr, inBuf, 0, len)) throw r;
}

inline void xx::UvTcpBase::SendBytes(BBuffer const & bb)
{
	if (!ptr || !bb.dataLen) throw - 1;
	if (int r = uv_write_(&loop.mp, ptr, bb.buf, 0, bb.dataLen)) throw r;
}

inline xx::UvTcpPeer::UvTcpPeer(UvTcpListener & listener)
	: UvTcpBase(listener.loop)
	, listener(listener)
{
	ipBuf.fill(0);

	ptr = Alloc(sizeof(uv_tcp_t), this);
	if (!ptr) throw - 1;

	if (int r = uv_tcp_init((uv_loop_t*)loop.ptr, (uv_tcp_t*)ptr))
	{
		Free(ptr);
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

	addrPtr = Alloc(sizeof(sockaddr_in));
	if (!addrPtr)
	{
		Close((uv_handle_t*)ptr);
		ptr = nullptr;
		throw - 1;
	}

	//index_at_container = listener.peers.dataLen;
	//listener.peers.Add(this);
}

inline xx::UvTcpPeer::~UvTcpPeer()
{
	DisconnectImpl();
}

inline void xx::UvTcpPeer::DisconnectImpl()
{
	//UvInterop.xxuv_close_(ptr);
	//ptr = IntPtr.Zero;
	//this.Free(ref addrPtr);
	//this.Unhandle(ref handle);

	//UnbindTimerManager();
	//OnTimeout = null;

	//bbSend = null;
	//bbRecv = null;
	//listener.peers.SwapRemoveAt(index_at_container);
	//listener = null;
	//loop = null;
	//disposed = true;
}

inline char* xx::UvTcpPeer::ip()
{
	if (!ptr) throw - 1;
	if (ipBuf[0]) return ipBuf.data();
	int len = 0;
	if (int r = uv_fill_client_ip((uv_tcp_t*)ptr, ipBuf.data(), ipBuf.size(), &len)) throw r;
	ipBuf[len] = 0;
	return ipBuf.data();
}
