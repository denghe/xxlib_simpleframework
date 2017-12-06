#include "xxuvlib.h"
#include <cstdlib>

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

#endif

static void* Alloc(size_t size)
{
	return std::malloc(size);
}

template<typename T>
static T* Alloc()
{
	return (T*)Alloc(sizeof(T));
}

XXUVLIB_API void xxuv_free(void* p) noexcept
{
	std::free(p);
}


XXUVLIB_API const char* xxuv_strerror(int n) noexcept
{
	return uv_strerror(n);
}
XXUVLIB_API const char* xxuv_err_name(int n) noexcept
{
	return uv_err_name(n);
}


XXUVLIB_API void xxuv_set_data(uv_handle_t* handle, void* data) noexcept
{
	handle->data = data;
}
XXUVLIB_API void* xxuv_get_data(uv_handle_t* handle) noexcept
{
	return handle->data;
}


XXUVLIB_API void xxuv_close(uv_handle_t* handle, uv_close_cb close_cb) noexcept
{
	uv_close(handle, close_cb);
}

XXUVLIB_API void xxuv_close_(uv_handle_t* handle) noexcept
{
	uv_close(handle, [](uv_handle_t* handle)
	{
		xxuv_free(handle);
	});
}

XXUVLIB_API uv_loop_t* xxuv_alloc_uv_loop_t() noexcept
{
	return ::Alloc<uv_loop_t>();
}

XXUVLIB_API uv_tcp_t* xxuv_alloc_uv_tcp_t() noexcept
{
	return ::Alloc<uv_tcp_t>();
}

XXUVLIB_API sockaddr_in* xxuv_alloc_sockaddr_in() noexcept
{
	return ::Alloc<sockaddr_in>();
}


XXUVLIB_API int xxuv_loop_init(uv_loop_t* p) noexcept
{
	return uv_loop_init(p);
}

XXUVLIB_API int xxuv_loop_close(uv_loop_t* p) noexcept
{
	return uv_loop_close(p);
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
	return uv_read_start(stream, [](uv_handle_t*, size_t suggested_size, uv_buf_t* buf)
	{
		buf->base = (char*)Alloc(suggested_size);
		buf->len = decltype(buf->len)(suggested_size);
	}
	, read_cb);
}

XXUVLIB_API int xxuv_write(uv_write_t* req, uv_stream_t* stream, const uv_buf_t bufs[], unsigned int nbufs, uv_write_cb cb) noexcept
{
	return uv_write(req, stream, bufs, nbufs, cb);
}

XXUVLIB_API int xxuv_write_(uv_stream_t* stream, char* inBuf, unsigned int len) noexcept
{
	struct write_req_t
	{
		uv_write_t req;
		uv_buf_t buf;
	};
	auto req = Alloc<write_req_t>();
	auto buf = (char*)Alloc(len);
	memcpy(buf, inBuf, len);
	req->buf = uv_buf_init(buf, (uint32_t)len);
	return uv_write((uv_write_t*)req, stream, &req->buf, 1, [](uv_write_t *req, int status)
	{
		if (status) fprintf(stderr, "Write error: %s\n", uv_strerror(status));
		write_req_t *wr = (write_req_t*)req;
		xxuv_free(wr->buf.base);
		xxuv_free(wr);
	});
}


XXUVLIB_API int xxuv_run(uv_loop_t* loop, uv_run_mode mode) noexcept
{
	return uv_run(loop, mode);
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
