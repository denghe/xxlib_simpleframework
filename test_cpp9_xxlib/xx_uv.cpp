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


inline xx::UvLoop::UvLoop()
	: listeners(&mp)
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
	listeners.ForEachRevert([](auto& o) { o.Release(); });
	// todo: more dispose

	if (uv_loop_close((uv_loop_t*)ptr))
	{
		uv_run((uv_loop_t*)ptr, UV_RUN_DEFAULT);
		uv_loop_close((uv_loop_t*)ptr);
	}
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





inline xx::UvTcpListener::UvTcpListener(UvLoop * loop)
	: Object(&loop->mp)
	, loop(loop)
{
	if (int r = uv_tcp_init((uv_loop_t*)loop->ptr, (uv_tcp_t*)ptr)) throw r;
	index_at_container = loop->listeners.dataLen;
	loop->listeners.Add(this);
}

inline xx::UvTcpListener::~UvTcpListener()
{
	//peers.ForEachReverse(o = > o.Dispose());

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
