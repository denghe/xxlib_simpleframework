#include "xxmempool.h"
#include "xxuvlib.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#ifdef _WIN32

BOOL APIENTRY DllMain(HMODULE hModule,
	DWORD  ul_reason_for_call,
	LPVOID lpReserved
)
{
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
	case DLL_THREAD_ATTACH:
	case DLL_THREAD_DETACH:
	case DLL_PROCESS_DETACH:
		break;
	}
	return TRUE;
}
#else
#define sprintf_s snprintf
#endif


// 分配内存时保留一个头空间并填充
static void* Alloc(size_t size, void* ud)
{
	auto p = (void**)malloc(size + sizeof(void*));
	p[0] = ud;
	return &p[1];
}

// 容忍浪费, 简化 Free 流程
static void* Alloc(size_t size)
{
	return (void**)malloc(size + sizeof(void*)) + 1;
}

// 还原真实指针, 释放
static void Free(void* p) noexcept
{
	free((void**)p - 1);
}

static void AllocCB(uv_handle_t* h, size_t suggested_size, uv_buf_t* buf)
{
	buf->base = (char*)((XxMemPool*)h->loop->data)->Alloc(suggested_size);
	buf->len = decltype(buf->len)(suggested_size);
}




XXUVLIB_API uv_loop_t* xxuv_alloc_uv_loop_t(void* ud) noexcept
{
	return (uv_loop_t*)Alloc(sizeof(uv_loop_t), ud);
}

XXUVLIB_API uv_tcp_t* xxuv_alloc_uv_tcp_t(void* ud) noexcept
{
	return (uv_tcp_t*)Alloc(sizeof(uv_tcp_t), ud);
}

XXUVLIB_API uv_udp_t* xxuv_alloc_uv_udp_t(void* ud) noexcept
{
	return (uv_udp_t*)Alloc(sizeof(uv_udp_t), ud);
}


XXUVLIB_API sockaddr_in* xxuv_alloc_sockaddr_in(void* ud) noexcept
{
	return (sockaddr_in*)Alloc(sizeof(sockaddr_in), ud);
}

XXUVLIB_API uv_timer_t* xxuv_alloc_uv_timer_t(void* ud) noexcept
{
	return (uv_timer_t*)Alloc(sizeof(uv_timer_t), ud);
}

XXUVLIB_API uv_async_t* xxuv_alloc_uv_async_t(void* ud) noexcept
{
	return (uv_async_t*)Alloc(sizeof(uv_async_t), ud);
}

XXUVLIB_API uv_signal_t* xxuv_alloc_uv_signal_t(void* ud) noexcept
{
	return (uv_signal_t*)Alloc(sizeof(uv_signal_t), ud);
}





XXUVLIB_API void xxuv_free(void* p) noexcept
{
	if (p) Free(p);
}
XXUVLIB_API void xxuv_pool_free(uv_loop_t* loop, void* p) noexcept
{
	if (p) ((XxMemPool*)loop->data)->Free(p);
}

XXUVLIB_API void* xxuv_get_ud(void* p) noexcept
{
	return *((void**)p - 1);
}

XXUVLIB_API void* xxuv_get_ud_from_uv_connect_t(uv_connect_t* req) noexcept
{
	return xxuv_get_ud(req->handle);
}

XXUVLIB_API void* xxuv_get_buf(uv_buf_t* p) noexcept
{
	return p->base;
}





XXUVLIB_API const char* xxuv_strerror(int n) noexcept
{
	return uv_strerror(n);
}
XXUVLIB_API const char* xxuv_err_name(int n) noexcept
{
	return uv_err_name(n);
}







XXUVLIB_API void xxuv_close(uv_handle_t* handle, uv_close_cb close_cb) noexcept
{
	uv_close(handle, close_cb);
}

XXUVLIB_API void xxuv_close_(uv_handle_t* handle) noexcept
{
#ifndef NDEBUG
	if (uv_is_closing(handle)) return;
#endif
	uv_close(handle, [](uv_handle_t* handle)
	{
		Free(handle);
	});
}





XXUVLIB_API int xxuv_loop_init(uv_loop_t* p) noexcept
{
	auto r = uv_loop_init(p);
	if (!r)
	{
		p->data = new XxMemPool();
	}
	return r;
}

XXUVLIB_API int xxuv_run(uv_loop_t* loop, uv_run_mode mode) noexcept
{
	return uv_run(loop, mode);
}

XXUVLIB_API void xxuv_stop(uv_loop_t* loop) noexcept
{
	uv_stop(loop);
}

XXUVLIB_API int xxuv_loop_close(uv_loop_t* p) noexcept
{
	auto r = uv_loop_close(p);
	if (!r && p->data)
	{
		delete (XxMemPool*)p->data;
		p->data = nullptr;
	}
	return r;
}

XXUVLIB_API int xxuv_loop_alive(uv_loop_t* p) noexcept
{
	return uv_loop_alive(p);
}





XXUVLIB_API int xxuv_ip4_addr(const char* ip, int port, sockaddr_in* addr) noexcept
{
	return uv_ip4_addr(ip, port, addr);
}



XXUVLIB_API int xxuv_tcp_init(uv_loop_t* loop, uv_tcp_t* tcp) noexcept
{
	return uv_tcp_init(loop, tcp);
}

XXUVLIB_API int xxuv_tcp_bind(uv_tcp_t* tcp, const sockaddr* addr, unsigned int flags) noexcept
{
	return uv_tcp_bind(tcp, addr, flags);
}
XXUVLIB_API int xxuv_tcp_bind_(uv_tcp_t* tcp, const sockaddr* addr) noexcept
{
	return uv_tcp_bind(tcp, addr, 0);
}


XXUVLIB_API int xxuv_listen(uv_stream_t* listener, int backlog, uv_connection_cb cb) noexcept
{
	return uv_listen(listener, backlog, cb);
}

XXUVLIB_API int xxuv_accept(uv_stream_t* listener, uv_stream_t* peer) noexcept
{
	return uv_accept(listener, peer);
}

XXUVLIB_API int xxuv_read_start(uv_stream_t* stream, uv_alloc_cb alloc_cb, uv_read_cb read_cb) noexcept
{
	return uv_read_start(stream, alloc_cb, read_cb);
}
XXUVLIB_API int xxuv_read_start_(uv_stream_t* stream, uv_read_cb read_cb) noexcept
{
	return uv_read_start(stream, AllocCB, read_cb);
}

XXUVLIB_API int xxuv_write(uv_write_t* req, uv_stream_t* stream, const uv_buf_t bufs[], unsigned int nbufs, uv_write_cb cb) noexcept
{
	return uv_write(req, stream, bufs, nbufs, cb);
}

XXUVLIB_API int xxuv_write_(uv_stream_t* stream, char const* inBuf, unsigned int offset, unsigned int len) noexcept
{
	struct uv_write_t_ex
	{
		XxMemPool* mp;
		uv_write_t req;
		uv_buf_t buf;
	};
	auto mp = (XxMemPool*)stream->loop->data;
	auto req = mp->Alloc<uv_write_t_ex>();
	req->mp = mp;
	auto buf = (char*)mp->Alloc(len);
	memcpy(buf, inBuf + offset, len);
	req->buf = uv_buf_init(buf, (uint32_t)len);
	return uv_write((uv_write_t*)req, stream, &req->buf, 1, [](uv_write_t *req, int status)
	{
		//if (status) fprintf(stderr, "Write error: %s\n", uv_strerror(status));
		auto *wr = (uv_write_t_ex*)req;
		wr->mp->Free(wr->buf.base);
		wr->mp->Free(wr);
	});
}


XXUVLIB_API int xxuv_fill_client_ip(uv_tcp_t* stream, char* buf, int buf_len, int* data_len) noexcept
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


XXUVLIB_API int xxuv_tcp_connect(uv_connect_t* req, uv_tcp_t* handle, const sockaddr* addr, uv_connect_cb cb) noexcept
{
	return uv_tcp_connect(req, handle, addr, cb);
}

XXUVLIB_API int xxuv_tcp_connect_(uv_tcp_t* handle, const struct sockaddr* addr, uv_connect_cb cb) noexcept
{
	auto req = (uv_connect_t*)Alloc(sizeof(uv_connect_t));
	auto r = uv_tcp_connect(req, handle, addr, cb);
	if (r) Free(req);
	return r;
}





XXUVLIB_API int xxuv_udp_init(uv_loop_t* loop, uv_udp_t* udp) noexcept
{
	return uv_udp_init(loop, udp);
}

XXUVLIB_API int xxuv_udp_bind(uv_udp_t* udp, const struct sockaddr* addr, unsigned int flags) noexcept
{
	return uv_udp_bind(udp, addr, flags);
}

XXUVLIB_API int xxuv_udp_bind_(uv_udp_t* udp, const struct sockaddr* addr) noexcept
{
	return uv_udp_bind(udp, addr, UV_UDP_REUSEADDR);
}

XXUVLIB_API int xxuv_udp_recv_start(uv_udp_t* handle, uv_alloc_cb alloc_cb, uv_udp_recv_cb recv_cb) noexcept
{
	return uv_udp_recv_start(handle, alloc_cb, recv_cb);
}

XXUVLIB_API int xxuv_udp_recv_start_(uv_udp_t* handle, uv_udp_recv_cb recv_cb) noexcept
{
	return uv_udp_recv_start(handle, AllocCB, recv_cb);
}

XXUVLIB_API int xxuv_udp_recv_stop(uv_udp_t* handle) noexcept
{
	return uv_udp_recv_stop(handle);
}

XXUVLIB_API int xxuv_udp_send(uv_udp_send_t* req, uv_udp_t* handle, const uv_buf_t bufs[], unsigned int nbufs, const struct sockaddr* addr, uv_udp_send_cb send_cb) noexcept
{
	return uv_udp_send(req, handle, bufs, nbufs, addr, send_cb);
}

XXUVLIB_API int xxuv_udp_send_(uv_udp_t* handle, char const* inBuf, unsigned int offset, unsigned int len, const struct sockaddr* addr) noexcept
{
	//uv_udp_get_send_queue_size
		
	struct uv_udp_send_t_ex
	{
		XxMemPool* mp;
		uv_udp_send_t req;
		uv_buf_t buf;
	};
	auto mp = (XxMemPool*)handle->loop->data;
	auto req = mp->Alloc<uv_udp_send_t_ex>();
	req->mp = mp;
	auto buf = (char*)mp->Alloc(len);
	memcpy(buf, inBuf + offset, len);
	req->buf = uv_buf_init(buf, (uint32_t)len);
	return uv_udp_send((uv_udp_send_t*)req, handle, &req->buf, 1, addr, [](uv_udp_send_t* req, int status)
	{
		//if (status) fprintf(stderr, "Write error: %s\n", uv_strerror(status));
		auto *wr = (uv_udp_send_t_ex*)req;
		wr->mp->Free(wr->buf.base);
		wr->mp->Free(wr);
	});
}
XXUVLIB_API size_t xxuv_udp_get_send_queue_size(const uv_udp_t* udp) noexcept
{
	return uv_udp_get_send_queue_size(udp);
}

XXUVLIB_API void xxuv_addr_copy(sockaddr_in* from, sockaddr_in* to) noexcept
{
	memcpy(to, from, sizeof(sockaddr_in));
}

XXUVLIB_API int xxuv_fill_ip(sockaddr_in* addr, char* buf, int buf_len, int* data_len) noexcept
{
	if (int r = uv_ip4_name(addr, buf, buf_len)) throw r;
	*data_len = (int)strlen(buf);
	*data_len += sprintf_s(buf + *data_len, buf_len - *data_len, ":%d", ntohs(addr->sin_port));
	return 0;
}






XXUVLIB_API int xxuv_timer_init(uv_loop_t* loop, uv_timer_t* timer_req) noexcept
{
	return uv_timer_init(loop, timer_req);
}
XXUVLIB_API int xxuv_timer_start(uv_timer_t* timer_req, uv_timer_cb cb, unsigned long long timeoutMS, unsigned long long repeatIntervalMS) noexcept
{
	return uv_timer_start(timer_req, cb, timeoutMS, repeatIntervalMS);
}
XXUVLIB_API void xxuv_timer_set_repeat(uv_timer_t* timer_req, unsigned long long repeatIntervalMS) noexcept
{
	uv_timer_set_repeat(timer_req, repeatIntervalMS);
}
XXUVLIB_API int xxuv_timer_again(uv_timer_t* timer_req) noexcept
{
	return uv_timer_again(timer_req);
}
XXUVLIB_API int xxuv_timer_stop(uv_timer_t* timer_req) noexcept
{
	return uv_timer_stop(timer_req);
}





XXUVLIB_API int xxuv_async_init(uv_loop_t* loop, uv_async_t* async_req, uv_async_cb cb) noexcept
{
	return uv_async_init(loop, async_req, cb);
}
XXUVLIB_API int xxuv_async_send(uv_async_t* async_req) noexcept
{
	return uv_async_send(async_req);
}





XXUVLIB_API int xxuv_signal_init(uv_loop_t* loop, uv_signal_t* signal) noexcept
{
	return uv_signal_init(loop, signal);
}
XXUVLIB_API int xxuv_signal_start(uv_signal_t* signal, uv_signal_cb cb) noexcept
{
	return uv_signal_start(signal, cb, SIGINT);
}
XXUVLIB_API void xxuv_walk(uv_loop_t* loop, uv_walk_cb cb, void* arg) noexcept
{
	uv_walk(loop, cb, arg);
}







XXUVLIB_API int xxuv_is_readable(const uv_stream_t* stream) noexcept
{
	return uv_is_readable(stream);
}
XXUVLIB_API int xxuv_is_writable(const uv_stream_t* stream) noexcept
{
	return uv_is_writable(stream);
}
XXUVLIB_API size_t xxuv_stream_get_write_queue_size(const uv_stream_t* stream) noexcept
{
	return uv_stream_get_write_queue_size(stream);
}
XXUVLIB_API int xxuv_try_write(uv_stream_t* stream, const uv_buf_t bufs[], unsigned int nbufs) noexcept
{
	return uv_try_write(stream, bufs, nbufs);
}
XXUVLIB_API int xxuv_try_write_(uv_stream_t* stream, char* buf, unsigned int len) noexcept
{
	auto bufs = uv_buf_init(buf, len);
	return uv_try_write(stream, &bufs, 1);
}










XXUVLIB_API ikcpcb* xx_ikcp_create(Guid const* conv, void *user, uv_loop_t* loop) noexcept
{
	if (loop)
	{
		ikcp_allocator([](auto a, auto s)
		{
			return ((XxMemPool*)a)->Alloc(s);
		}, [](auto a, auto p)
		{
			((XxMemPool*)a)->Free(p);
		});
		return ikcp_create(conv, user, loop->data);
	}
	return ikcp_create(conv, user, nullptr);
}

XXUVLIB_API void xx_ikcp_release(ikcpcb *kcp) noexcept
{
	ikcp_release(kcp);
}

XXUVLIB_API void xx_ikcp_setoutput(ikcpcb *kcp, int(*output)(const char *buf, int len, ikcpcb *kcp)) noexcept
{
	ikcp_setoutput(kcp, output);
}

XXUVLIB_API int xx_ikcp_wndsize(ikcpcb *kcp, int sndwnd, int rcvwnd) noexcept
{
	return ikcp_wndsize(kcp, sndwnd, rcvwnd);
}

XXUVLIB_API int xx_ikcp_nodelay(ikcpcb *kcp, int nodelay, int interval, int resend, int nc) noexcept
{
	return ikcp_nodelay(kcp, nodelay, interval, resend, nc);
}

XXUVLIB_API void* xx_ikcp_get_ud(ikcpcb* kcp) noexcept
{
	return kcp->user;
}

XXUVLIB_API int xx_ikcp_input(ikcpcb* kcp, const char *data, int size) noexcept
{
	return ikcp_input(kcp, data, size);
}

XXUVLIB_API int xx_ikcp_send(ikcpcb* kcp, const char *buffer, int offset, int len) noexcept
{
	return ikcp_send(kcp, buffer + offset, len);
}

XXUVLIB_API void xx_ikcp_update(ikcpcb* kcp, uint32_t current) noexcept
{
	ikcp_update(kcp, current);
}

XXUVLIB_API uint32_t xx_ikcp_check(ikcpcb* kcp, uint32_t current) noexcept
{
	return ikcp_check(kcp, current);
}

XXUVLIB_API int xx_ikcp_recv(ikcpcb* kcp, char* outBuf, int bufLen) noexcept
{
	return ikcp_recv(kcp, outBuf, bufLen);
}

