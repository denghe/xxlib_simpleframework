#pragma once
#include <uv.h>

#ifdef _WIN32
	#include <SDKDDKVer.h>
	#define WIN32_LEAN_AND_MEAN
	#include <windows.h>
	#ifdef xxuvlib_EXPORTS
	#define XXUVLIB_API __declspec(dllexport)
	#else
	#define XXUVLIB_API __declspec(dllimport)
	#endif
#else
	#define XXUVLIB_API
#endif

#ifdef __cplusplus
extern "C" {
#endif

	XXUVLIB_API uv_loop_t* xxuv_alloc_uv_loop_t() noexcept;
	XXUVLIB_API uv_tcp_t* xxuv_alloc_uv_tcp_t() noexcept;
	XXUVLIB_API sockaddr_in* xxuv_alloc_sockaddr_in() noexcept;
	XXUVLIB_API void xxuv_free(void* p) noexcept;


	XXUVLIB_API const char* xxuv_strerror(int n) noexcept;
	XXUVLIB_API const char* xxuv_err_name(int n) noexcept;


	XXUVLIB_API void xxuv_close(uv_handle_t* handle, uv_close_cb close_cb) noexcept;
	XXUVLIB_API void xxuv_close_(uv_handle_t* handle) noexcept;

	XXUVLIB_API int xxuv_loop_init(uv_loop_t* loop) noexcept;
	XXUVLIB_API int xxuv_run(uv_loop_t* loop, uv_run_mode mode) noexcept;
	XXUVLIB_API int xxuv_loop_close(uv_loop_t* loop) noexcept;

	XXUVLIB_API int xxuv_ip4_addr(const char* ip, int port, sockaddr_in* addr) noexcept;

	XXUVLIB_API int xxuv_tcp_init(uv_loop_t* loop, uv_tcp_t* tcp) noexcept;
	XXUVLIB_API int xxuv_tcp_bind(uv_tcp_t* tcp, const sockaddr* addr, unsigned int flags) noexcept;
	XXUVLIB_API int xxuv_tcp_bind_(uv_tcp_t* tcp, const sockaddr* addr) noexcept;
	XXUVLIB_API int xxuv_listen(uv_stream_t* tcp, int backlog, uv_connection_cb cb) noexcept;
	XXUVLIB_API int xxuv_accept(uv_stream_t* server, uv_stream_t* client) noexcept;
	XXUVLIB_API int xxuv_read_start(uv_stream_t* client, uv_alloc_cb alloc_cb, uv_read_cb read_cb) noexcept;
	XXUVLIB_API int xxuv_read_start_(uv_stream_t* client, uv_read_cb read_cb) noexcept;
	XXUVLIB_API int xxuv_write(uv_write_t* req, uv_stream_t* stream, const uv_buf_t bufs[], unsigned int nbufs, uv_write_cb cb);
	XXUVLIB_API int xxuv_write_(uv_stream_t* stream, char* buf, unsigned int len);

	// todo
	XXUVLIB_API int xxuv_is_readable(const uv_stream_t* handle);
	XXUVLIB_API int xxuv_is_writable(const uv_stream_t* handle);
	XXUVLIB_API size_t xxuv_stream_get_write_queue_size(const uv_stream_t* stream);
	XXUVLIB_API int xxuv_try_write(uv_stream_t* handle, const uv_buf_t bufs[], unsigned int nbufs);

#ifdef __cplusplus
}
#endif
