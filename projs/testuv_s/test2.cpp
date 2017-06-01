//#include <uv.h>
//#include <assert.h>
//#include <memory>
//#include <iostream>
//
//// todo: 直接用 uv_default_loop, 用模板来切换静态函数中的 userdata 之类型
//
//typedef struct {
//	uv_write_t req;
//	uv_buf_t buf;
//} write_req_t;
//
//struct UVH
//{
//	static int server_closed;
//
//	static void on_connection(uv_stream_t* server, int status)
//	{
//		// todo: 从 server 拿池 从池拿内存
//		uv_stream_t* stream;
//		int r;
//
//		if (status != 0) {
//			fprintf(stderr, "Connect error %s\n", uv_err_name(status));
//		}
//		assert(status == 0);
//
//		stream = (uv_stream_t*)malloc(sizeof(uv_tcp_t));
//		assert(stream != NULL);
//		r = uv_tcp_init(uv_default_loop(), (uv_tcp_t*)stream);
//		assert(r == 0);
//
//		/* associate server with stream */
//		stream->data = server;
//
//		r = uv_accept(server, stream);
//		assert(r == 0);
//
//		r = uv_read_start(stream, echo_alloc, after_read);
//		assert(r == 0);
//	}
//
//	static void echo_alloc(uv_handle_t* handle,
//		size_t suggested_size,
//		uv_buf_t* buf) {
//		buf->base = (char*)malloc(suggested_size);
//		buf->len = suggested_size;
//	}
//
//
//	static void after_read(uv_stream_t* handle,
//		ssize_t nread,
//		const uv_buf_t* buf) {
//		int i;
//		write_req_t *wr;
//		uv_shutdown_t* sreq;
//
//		if (nread < 0) {
//			/* Error or EOF */
//			assert(nread == UV_EOF);
//
//			free(buf->base);
//			sreq = (uv_shutdown_t*)malloc(sizeof* sreq);
//			assert(0 == uv_shutdown(sreq, handle, after_shutdown));
//			return;
//		}
//
//		if (nread == 0) {
//			/* Everything OK, but nothing read. */
//			free(buf->base);
//			return;
//		}
//
//		/*
//		* Scan for the letter Q which signals that we should quit the server.
//		* If we get QS it means close the stream.
//		*/
//		if (!server_closed) {
//			for (i = 0; i < nread; i++) {
//				if (buf->base[i] == 'Q') {
//					if (i + 1 < nread && buf->base[i + 1] == 'S') {
//						free(buf->base);
//						uv_close((uv_handle_t*)handle, on_close);
//						return;
//					}
//					else {
//						uv_close(server, on_server_close);
//						server_closed = 1;
//					}
//				}
//			}
//		}
//
//		wr = (write_req_t*)malloc(sizeof *wr);
//		assert(wr != NULL);
//		wr->buf = uv_buf_init(buf->base, nread);
//
//		if (uv_write(&wr->req, handle, &wr->buf, 1, after_write)) {
//			throw ("uv_write failed");
//		}
//	}
//
//	static void after_write(uv_write_t* req, int status) {
//		write_req_t* wr;
//
//		/* Free the read/write buffer and the request */
//		wr = (write_req_t*)req;
//		free(wr->buf.base);
//		free(wr);
//
//		if (status == 0)
//			return;
//
//		fprintf(stderr,
//			"uv_write error: %s - %s\n",
//			uv_err_name(status),
//			uv_strerror(status));
//	}
//
//	static void after_shutdown(uv_shutdown_t* req, int status) {
//		uv_close((uv_handle_t*)req->handle, on_close);
//		free(req);
//	}
//
//
//	static void on_close(uv_handle_t* peer) {
//		free(peer);
//	}
//
//	static void on_server_close(uv_handle_t* handle) {
//		//assert(handle == server);
//	}
//};
//
//template<typename T>
//struct UV
//{
//	uv_loop_t* loop;
//	uv_tcp_t tcpServer;
//
//	UV()
//	{
//		loop = uv_default_loop();
//	}
//
//	// todo: 理论上讲可以创建多个 listener 以 host 多个 port. 每个 host 需要 bind 一个处理函数.
//
//	inline int Listen(int port)
//	{
//		struct sockaddr_in addr;
//		int r;
//
//		assert(0 == uv_ip4_addr("0.0.0.0", port, &addr));
//
//		r = uv_tcp_init(loop, &tcpServer);
//		if (r) {
//			/* TODO: Error codes */
//			fprintf(stderr, "Socket creation error\n");
//			return 1;
//		}
//
//		r = uv_tcp_bind(&tcpServer, (const struct sockaddr*) &addr, 0);
//		if (r) {
//			/* TODO: Error codes */
//			fprintf(stderr, "Bind error\n");
//			return 1;
//		}
//
//		r = uv_listen((uv_stream_t*)&tcpServer, SOMAXCONN, T::on_connection);
//		if (r) {
//			/* TODO: Error codes */
//			fprintf(stderr, "Listen error %s\n", uv_err_name(r));
//			return 1;
//		}
//
//		return 0;
//	}
//};
//
//int main()
//{
//	return 0;
//}
