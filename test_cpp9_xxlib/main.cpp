#include <uv.h>
#include <iostream>
#include <thread>
#include "ikcp.h"	// todo
#include "xx.h"

namespace thread1
{
	uv_loop_t loop;
	uv_udp_t udp;
	sockaddr_in addr;
	sockaddr_in clientAddr;
	uv_timer_t timer;
	uint64_t recvCounter = 0;
	uv_udp_send_t req;
	ikcpcb *kcp;// = ikcp_create(conv, user);
	void f1()
	{
		kcp = ikcp_create(1, &clientAddr);
		ikcp_setoutput(kcp, [](const char *buf, int len, struct IKCPCB *kcp, void *user)
		{
			++recvCounter;
			//uv_buf_t uvbuf;
			//uvbuf.base = (char*)buf;
			//uvbuf.len = len;
			//if (int r = uv_udp_send(&req, &udp, &uvbuf, 1, (sockaddr*)&user, [](uv_udp_send_t *req, int status)
			//{
			//	if (status)
			//	{
			//		fprintf(stderr, "Send error %s\n", uv_strerror(status));
			//		return;
			//	}
			//})) throw r;
			return 0;	// unused
		});

		IUINT32 current = clock();
		IUINT32 slap = current + 20;
		IUINT32 index = 0;
		IUINT32 next = 0;
		IINT64 sumrtt = 0;
		int count = 0;
		int maxrtt = 0;

		// 配置窗口大小：平均延迟200ms，每20ms发送一个包，
		// 而考虑到丢包重发，设置最大收发窗口为128
		ikcp_wndsize(kcp, 128, 128);

		if (int r = uv_loop_init(&loop)) throw r;
		if (int r = uv_udp_init(&loop, &udp)) throw r;
		if (int r = uv_ip4_addr("0.0.0.0", 12345, &addr)) throw r;
		if (int r = uv_udp_bind(&udp, (sockaddr*)&addr, UV_UDP_REUSEADDR)) throw r;
		if (int r = uv_udp_recv_start(&udp, [](uv_handle_t *handle, size_t suggested_size, uv_buf_t *buf)
		{
			buf->base = (char*)malloc(suggested_size);
			buf->len = (decltype(buf->len))suggested_size;

		}, [](uv_udp_t *req, ssize_t nread, const uv_buf_t *buf, const sockaddr *addr, unsigned flags)
		{
			if (nread < 0) {
				fprintf(stderr, "Read error %s\n", uv_err_name((int)nread));
				uv_close((uv_handle_t*)req, NULL);
				free(buf->base);
				return;
			}

			//memcpy(&clientAddr, addr, sizeof(clientAddr));

			// kcp 输入
			ikcp_input(kcp, buf->base, nread);

			//std::cout << "nread = " << nread << std::endl;
			free(buf->base);
			//uv_udp_recv_stop(req);
			++recvCounter;

		})) throw r;

		if (int r = uv_timer_init(&loop, &timer)) throw r;
		uv_timer_start(&timer, [](uv_timer_t* handle)
		{
			ikcp_update(kcp, 1);
		}, 1, 1);

		uv_run(&loop, UV_RUN_DEFAULT);
	}
}

namespace thread2
{
	uv_loop_t loop;
	sockaddr_in addr;
	uv_udp_t udp;
	uv_buf_t buf;
	uv_udp_send_t req;
	uv_timer_t timer;
	ikcpcb *kcp;// = ikcp_create(conv, user);
	void SendCB(uv_udp_send_t *req, int status)
	{
		if (status)
		{
			fprintf(stderr, "Send error %s\n", uv_strerror(status));
			return;
		}
	}
	void f2()
	{
		kcp = ikcp_create(1, nullptr);
		ikcp_setoutput(kcp, [](const char *buf, int len, struct IKCPCB *kcp, void *user)
		{
			uv_buf_t uvbuf;
			uvbuf.base = (char*)buf;
			uvbuf.len = len;
			if (int r = uv_udp_send(&req, &udp, &uvbuf, 1, (sockaddr*)&addr, SendCB)) throw r;
			return 0;
		});

		IUINT32 current = clock();
		IUINT32 slap = current + 20;
		IUINT32 index = 0;
		IUINT32 next = 0;
		IINT64 sumrtt = 0;
		int count = 0;
		int maxrtt = 0;

		// 配置窗口大小：平均延迟200ms，每20ms发送一个包，
		// 而考虑到丢包重发，设置最大收发窗口为128
		ikcp_wndsize(kcp, 128, 128);

		std::this_thread::sleep_for(std::chrono::milliseconds(100));
		if (int r = uv_loop_init(&loop)) throw r;
		if (int r = uv_udp_init(&loop, &udp)) throw r;
		if (int r = uv_ip4_addr("127.0.0.1", 12345, &addr)) throw r;
		buf.len = 1000;
		buf.base = (char*)malloc(buf.len);
		SendCB(&req, 0);

		if (int r = uv_timer_init(&loop, &timer)) throw r;
		uv_timer_start(&timer, [](uv_timer_t* handle)
		{
			std::cout << "sendCounter = " << sendCounter << std::endl;
		}, 1000, 1000);

		uv_run(&loop, UV_RUN_DEFAULT);
	}
}

#include <thread>
int main()
{
	std::thread t(thread2::f2);
	t.detach();
	thread1::f1();
	return 0;
}
