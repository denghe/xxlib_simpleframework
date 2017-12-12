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

	XXUVLIB_API uv_loop_t* xxuv_alloc_uv_loop_t(void* ud) noexcept;
	XXUVLIB_API uv_tcp_t* xxuv_alloc_uv_tcp_t(void* ud) noexcept;
	XXUVLIB_API sockaddr_in* xxuv_alloc_sockaddr_in(void* ud) noexcept;
	XXUVLIB_API uv_timer_t* xxuv_alloc_uv_timer_t(void* ud) noexcept;
	XXUVLIB_API uv_async_t* xxuv_alloc_uv_async_t(void* ud) noexcept;
	XXUVLIB_API uv_signal_t* xxuv_alloc_uv_signal_t(void* ud) noexcept;

	XXUVLIB_API void xxuv_free(void* p) noexcept;
	XXUVLIB_API void* xxuv_get_ud(void* p) noexcept;
	XXUVLIB_API void* xxuv_get_ud_from_uv_connect_t(uv_connect_t* req) noexcept;
	XXUVLIB_API void* xxuv_get_buf(uv_buf_t* p) noexcept;


	XXUVLIB_API const char* xxuv_strerror(int n) noexcept;
	XXUVLIB_API const char* xxuv_err_name(int n) noexcept;


	XXUVLIB_API void xxuv_close(uv_handle_t* handle, uv_close_cb close_cb) noexcept;
	XXUVLIB_API void xxuv_close_(uv_handle_t* handle) noexcept;

	XXUVLIB_API int xxuv_loop_init(uv_loop_t* loop) noexcept;
	XXUVLIB_API int xxuv_run(uv_loop_t* loop, uv_run_mode mode) noexcept;
	XXUVLIB_API void xxuv_stop(uv_loop_t* loop) noexcept;
	XXUVLIB_API int xxuv_loop_close(uv_loop_t* loop) noexcept;
	XXUVLIB_API int xxuv_loop_alive(uv_loop_t* loop) noexcept;

	XXUVLIB_API int xxuv_ip4_addr(const char* ip, int port, sockaddr_in* addr) noexcept;

	XXUVLIB_API int xxuv_tcp_init(uv_loop_t* loop, uv_tcp_t* tcp) noexcept;
	XXUVLIB_API int xxuv_tcp_bind(uv_tcp_t* tcp, const struct sockaddr* addr, unsigned int flags) noexcept;
	XXUVLIB_API int xxuv_tcp_bind_(uv_tcp_t* tcp, const struct sockaddr* addr) noexcept;
	XXUVLIB_API int xxuv_listen(uv_stream_t* listener, int backlog, uv_connection_cb cb) noexcept;
	XXUVLIB_API int xxuv_accept(uv_stream_t* listener, uv_stream_t* peer) noexcept;
	XXUVLIB_API int xxuv_read_start(uv_stream_t* stream, uv_alloc_cb alloc_cb, uv_read_cb read_cb) noexcept;
	XXUVLIB_API int xxuv_read_start_(uv_stream_t* stream, uv_read_cb read_cb) noexcept;
	XXUVLIB_API int xxuv_write(uv_write_t* req, uv_stream_t* stream, const uv_buf_t bufs[], unsigned int nbufs, uv_write_cb cb) noexcept;
	XXUVLIB_API int xxuv_write_(uv_stream_t* stream, char* buf, unsigned int len) noexcept;
	XXUVLIB_API int xxuv_fill_client_ip(uv_tcp_t* stream, char* buf, int buf_len, int* data_len) noexcept;
	XXUVLIB_API int xxuv_tcp_connect(uv_connect_t* req, uv_tcp_t* handle, const struct sockaddr* addr, uv_connect_cb cb) noexcept;
	XXUVLIB_API int xxuv_tcp_connect_(uv_tcp_t* handle, const struct sockaddr* addr, uv_connect_cb cb) noexcept;


	XXUVLIB_API int xxuv_timer_init(uv_loop_t* loop, uv_timer_t* timer_req) noexcept;
	XXUVLIB_API int xxuv_timer_start(uv_timer_t* timer_req, uv_timer_cb cb, unsigned long long timeoutMS, unsigned long long repeatIntervalMS) noexcept;
	XXUVLIB_API void xxuv_timer_set_repeat(uv_timer_t* timer_req, unsigned long long repeatIntervalMS) noexcept;
	XXUVLIB_API int xxuv_timer_again(uv_timer_t* timer_req) noexcept;
	XXUVLIB_API int xxuv_timer_stop(uv_timer_t* timer_req) noexcept;


	XXUVLIB_API int xxuv_async_init(uv_loop_t* loop, uv_async_t* async_req, uv_async_cb cb) noexcept;
	XXUVLIB_API int xxuv_async_send(uv_async_t* async_req) noexcept;


	XXUVLIB_API int xxuv_signal_init(uv_loop_t* loop, uv_signal_t* signal) noexcept;
	XXUVLIB_API int xxuv_signal_start(uv_signal_t* signal, uv_signal_cb cb) noexcept;
	XXUVLIB_API void xxuv_walk(uv_loop_t* loop, uv_walk_cb cb, void* arg) noexcept;


	// todo
	XXUVLIB_API int xxuv_is_readable(const uv_stream_t* stream) noexcept;
	XXUVLIB_API int xxuv_is_writable(const uv_stream_t* stream) noexcept;
	XXUVLIB_API size_t xxuv_stream_get_write_queue_size(const uv_stream_t* stream) noexcept;
	XXUVLIB_API int xxuv_try_write(uv_stream_t* stream, const uv_buf_t bufs[], unsigned int nbufs) noexcept;
	XXUVLIB_API int xxuv_try_write_(uv_stream_t* stream, char* buf, unsigned int len) noexcept;

#ifdef __cplusplus
}
#endif
