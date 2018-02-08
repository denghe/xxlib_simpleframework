#pragma once
#include <uv.h>
#include "ikcp.h"

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
	XXUVLIB_API uv_udp_t* xxuv_alloc_uv_udp_t(void* ud) noexcept;
	XXUVLIB_API sockaddr* xxuv_alloc_sockaddr_in(void* ud) noexcept;
	XXUVLIB_API uv_timer_t* xxuv_alloc_uv_timer_t(void* ud) noexcept;
	XXUVLIB_API uv_async_t* xxuv_alloc_uv_async_t(void* ud) noexcept;
	XXUVLIB_API uv_signal_t* xxuv_alloc_uv_signal_t(void* ud) noexcept;

	XXUVLIB_API void xxuv_free(void* p) noexcept;
	XXUVLIB_API void xxuv_pool_free(uv_loop_t* loop, void* p) noexcept;

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

	XXUVLIB_API int xxuv_ip4_addr(const char* ipv4, int port, sockaddr* addr) noexcept;
	XXUVLIB_API int xxuv_ip6_addr(const char* ipv6, int port, sockaddr_in6* addr) noexcept;

	XXUVLIB_API int xxuv_tcp_init(uv_loop_t* loop, uv_tcp_t* tcp) noexcept;
	XXUVLIB_API int xxuv_tcp_bind(uv_tcp_t* tcp, const struct sockaddr* addr, unsigned int flags) noexcept;
	XXUVLIB_API int xxuv_tcp_bind_(uv_tcp_t* tcp, const struct sockaddr* addr) noexcept;
	XXUVLIB_API int xxuv_listen(uv_stream_t* listener, int backlog, uv_connection_cb cb) noexcept;
	XXUVLIB_API int xxuv_accept(uv_stream_t* listener, uv_stream_t* peer) noexcept;
	XXUVLIB_API int xxuv_read_start(uv_stream_t* stream, uv_alloc_cb alloc_cb, uv_read_cb read_cb) noexcept;
	XXUVLIB_API int xxuv_read_start_(uv_stream_t* stream, uv_read_cb read_cb) noexcept;
	XXUVLIB_API int xxuv_write(uv_write_t* req, uv_stream_t* stream, const uv_buf_t bufs[], unsigned int nbufs, uv_write_cb cb) noexcept;
	XXUVLIB_API int xxuv_write_(uv_stream_t* stream, char const* buf, unsigned int offset, unsigned int len) noexcept;
	XXUVLIB_API int xxuv_fill_client_ip(uv_tcp_t* stream, char* buf, int buf_len, int* data_len) noexcept;
	XXUVLIB_API int xxuv_tcp_connect(uv_connect_t* req, uv_tcp_t* stream, const struct sockaddr* addr, uv_connect_cb cb) noexcept;
	XXUVLIB_API int xxuv_tcp_connect_(uv_tcp_t* stream, const struct sockaddr* addr, uv_connect_cb cb) noexcept;
	XXUVLIB_API int xxuv_is_readable(const uv_stream_t* stream) noexcept;
	XXUVLIB_API int xxuv_is_writable(const uv_stream_t* stream) noexcept;
	XXUVLIB_API size_t xxuv_stream_get_write_queue_size(const uv_stream_t* stream) noexcept;
	XXUVLIB_API int xxuv_try_write(uv_stream_t* stream, const uv_buf_t bufs[], unsigned int nbufs) noexcept;
	XXUVLIB_API int xxuv_try_write_(uv_stream_t* stream, char* buf, unsigned int len) noexcept;


	XXUVLIB_API int xxuv_udp_init(uv_loop_t* loop, uv_udp_t* udp) noexcept;
	XXUVLIB_API int xxuv_udp_bind(uv_udp_t* udp, const struct sockaddr* addr, unsigned int flags) noexcept;
	XXUVLIB_API int xxuv_udp_bind_(uv_udp_t* udp, const struct sockaddr* addr) noexcept;
	XXUVLIB_API int xxuv_udp_recv_start(uv_udp_t* udp, uv_alloc_cb alloc_cb, uv_udp_recv_cb recv_cb) noexcept;
	XXUVLIB_API int xxuv_udp_recv_start_(uv_udp_t* udp, uv_udp_recv_cb recv_cb) noexcept;
	XXUVLIB_API int xxuv_udp_recv_stop(uv_udp_t* udp) noexcept;
	XXUVLIB_API int xxuv_udp_send(uv_udp_send_t* req, uv_udp_t* handle, const uv_buf_t bufs[], unsigned int nbufs, const struct sockaddr* addr, uv_udp_send_cb send_cb) noexcept;
	XXUVLIB_API int xxuv_udp_send_(uv_udp_t* handle, char const* buf, unsigned int offset, unsigned int len, const struct sockaddr* addr) noexcept;
	XXUVLIB_API size_t xxuv_udp_get_send_queue_size(const uv_udp_t* udp) noexcept;
	XXUVLIB_API void xxuv_addr_copy(sockaddr* from, sockaddr* to) noexcept;
	XXUVLIB_API int xxuv_fill_ip(sockaddr* addr, char* buf, int buf_len, int* data_len) noexcept;


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




	// kcp 相关
	XXUVLIB_API ikcpcb* xx_ikcp_create(Guid const* conv, void *ud, uv_loop_t* loop) noexcept;
	XXUVLIB_API void xx_ikcp_release(ikcpcb *kcp) noexcept;
	XXUVLIB_API void xx_ikcp_setoutput(ikcpcb *kcp, int(*output)(const char *buf, int len, ikcpcb *kcp)) noexcept;
	XXUVLIB_API int xx_ikcp_wndsize(ikcpcb *kcp, int sndwnd, int rcvwnd) noexcept;
	XXUVLIB_API int xx_ikcp_nodelay(ikcpcb *kcp, int nodelay, int interval, int resend, int nc) noexcept;
	XXUVLIB_API void* xx_ikcp_get_ud(ikcpcb* kcp) noexcept;
	XXUVLIB_API int xx_ikcp_input(ikcpcb* kcp, const char *data, int size) noexcept;
	XXUVLIB_API int xx_ikcp_send(ikcpcb* kcp, const char *buffer, int offset, int len) noexcept;
	XXUVLIB_API void xx_ikcp_update(ikcpcb* kcp, uint32_t current) noexcept;
	XXUVLIB_API uint32_t xx_ikcp_check(ikcpcb* kcp, uint32_t current) noexcept;
	XXUVLIB_API int xx_ikcp_recv(ikcpcb* kcp, char* outBuf, int bufLen) noexcept;


#ifdef __cplusplus
}
#endif
