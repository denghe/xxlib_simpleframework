#include <uv.h>
#include <iostream>
#include <thread>
#include "ikcp.h"	// todo
#include "xx.h"


//void SendCB(uv_udp_send_t *req, int status)
//{
//	if (status)
//	{
//		fprintf(stderr, "Send error %s\n", uv_strerror(status));
//		return;
//	}
//}
//
//namespace thread1
//{
//	uv_loop_t loop;
//	uv_udp_t udp;
//	sockaddr_in addr;
//	sockaddr_in clientAddr;
//	uv_timer_t timer;
//	uv_udp_send_t req;
//	ikcpcb *kcp;// = ikcp_create(conv, user);
//	xx::MemPool mp;
//	xx::BBuffer_p bb;
//
//	void f1()
//	{
//		kcp = ikcp_create(1, &clientAddr);
//		ikcp_setoutput(kcp, [](const char *buf, int len, struct IKCPCB *kcp, void *user)
//		{
//			uv_buf_t uvbuf;
//			uvbuf.base = (char*)buf;
//			uvbuf.len = len;
//			if (int r = uv_udp_send(&req, &udp, &uvbuf, 1, (sockaddr*)&user, SendCB)) throw r;
//			return 0;
//		});
//
//		ikcp_wndsize(kcp, 128, 128);
//		ikcp_nodelay(kcp, 1, 10, 2, 1);
//
//		// todo: BBuffer 处应该进一步限制包长度在 65000 内, 留点白以作为包头进一步发挥的空间, 以及兼容 udp 65507 字节限制
//		mp.CreateTo(bb, 65536 - sizeof(xx::MemHeader));
//
//		if (int r = uv_loop_init(&loop)) throw r;
//		if (int r = uv_udp_init(&loop, &udp)) throw r;
//		if (int r = uv_ip4_addr("0.0.0.0", 12345, &addr)) throw r;
//		if (int r = uv_udp_bind(&udp, (sockaddr*)&addr, UV_UDP_REUSEADDR)) throw r;
//		if (int r = uv_udp_recv_start(&udp, [](uv_handle_t *handle, size_t suggested_size, uv_buf_t *buf)
//		{
//			buf->base = (char*)malloc(suggested_size);
//			buf->len = (decltype(buf->len))suggested_size;
//
//		}, [](uv_udp_t *req, ssize_t nread, const uv_buf_t *buf, const sockaddr *addr, unsigned flags)
//		{
//			if (nread < 0) {
//				fprintf(stderr, "Read error %s\n", uv_err_name((int)nread));
//				uv_close((uv_handle_t*)req, NULL);
//				free(buf->base);
//				return;
//			}
//
//			//memcpy(&clientAddr, addr, sizeof(clientAddr));
//
//			// kcp 输入
//			ikcp_input(kcp, buf->base, nread);
//			while (int len = ikcp_recv(kcp, bb->buf, bb->bufLen))
//			{
//				std::cout << len << std::endl;
//			}
//
//
//			//std::cout << "nread = " << nread << std::endl;
//			free(buf->base);
//			//uv_udp_recv_stop(req);
//
//		})) throw r;
//
//		if (int r = uv_timer_init(&loop, &timer)) throw r;
//		uv_timer_start(&timer, [](uv_timer_t* handle)
//		{
//			ikcp_update(kcp, 1);
//		}, 1, 1);
//
//		uv_run(&loop, UV_RUN_DEFAULT);
//	}
//}
//
//namespace thread2
//{
//	uv_loop_t loop;
//	sockaddr_in addr;
//	uv_udp_t udp;
//	uv_buf_t buf;
//	uv_udp_send_t req;
//	uv_timer_t timer;
//	ikcpcb *kcp;// = ikcp_create(conv, user);
//
//	void f2()
//	{
//		kcp = ikcp_create(1, nullptr);
//		ikcp_setoutput(kcp, [](const char *buf, int len, struct IKCPCB *kcp, void *user)
//		{
//			uv_buf_t uvbuf;
//			uvbuf.base = (char*)buf;
//			uvbuf.len = len;
//			if (int r = uv_udp_send(&req, &udp, &uvbuf, 1, (sockaddr*)&addr, SendCB)) throw r;
//			return 0;
//		});
//
//		ikcp_wndsize(kcp, 128, 128);
//		ikcp_nodelay(kcp, 1, 10, 2, 1);
//
//		std::this_thread::sleep_for(std::chrono::milliseconds(100));
//		if (int r = uv_loop_init(&loop)) throw r;
//		if (int r = uv_udp_init(&loop, &udp)) throw r;
//		if (int r = uv_ip4_addr("127.0.0.1", 12345, &addr)) throw r;
//		if (int r = uv_timer_init(&loop, &timer)) throw r;
//		uv_timer_start(&timer, [](uv_timer_t* handle)
//		{
//			ikcp_send(kcp, "a", 1);
//			ikcp_flush(kcp);
//			
//		}, 1000, 1000);
//
//		uv_run(&loop, UV_RUN_DEFAULT);
//	}
//}
//
//#include <thread>
//int main()
//{
//	std::thread t(thread2::f2);
//	t.detach();
//	thread1::f1();
//	return 0;
//}


// todo: 现约定每个 udp 包前 16 字节为 Guid 用于上下文路由. 
// 客户端自己生成 Guid, 直接与 server 做 kcp 通信
// server 以 Guid 为 key 建字典, value 存放连接模拟上下文, 注册 timeouter 以便超时清理

class Kcp
{
public:
	ikcpcb * kcp;
	uint32_t ticks = 0, next = 0, interval;
	std::function<void(char const* buf, int len)> OnSend;
	std::function<void(char const* buf, int len)> OnRecv;
	char buf[65100];

	explicit Kcp(xx::Guid const& conv, uint32_t interval = 10, xx::MemPool* mp = nullptr)
		: interval(interval)
	{
		if (mp)
		{
			ikcp_allocator([](auto a, auto s)
			{
				return ((xx::MemPool*)a)->Alloc(s);
			}, [](auto a, auto p)
			{
				((xx::MemPool*)a)->Free(p);
			});
		}
		kcp = ikcp_create(conv, this, mp);
		if (!kcp) throw - 1;

		ikcp_wndsize(kcp, 128, 128);
		ikcp_nodelay(kcp, 1, interval, 2, 1);
		ikcp_setoutput(kcp, [](const char *buf, int len, IKCPCB *kcp)
		{
			auto self = (Kcp*)kcp->user;
			self->OnSend(buf, len);
			return 0;
		});
	}
	~Kcp()
	{
		assert(kcp);
		ikcp_release(kcp);
		kcp = nullptr;
	}
	void Input(const char *data, int size)
	{
		next = ticks;
		if (int r = ikcp_input(kcp, data, size)) throw r;
	}
	void Send(const char *data, int size)
	{
		next = ticks;
		if (int r = ikcp_send(kcp, data, size)) throw r;
	}
	void Update()
	{
		if (next <= ticks)
		{
			ikcp_update(kcp, ticks);
			next = ikcp_check(kcp, ticks);

			int len = ikcp_recv(kcp, buf, _countof(buf));
			if (len > 0)
			{
				OnRecv(buf, len);
			}
		}
		ticks += interval;
	}
};

xx::MemPool mp;
xx::Guid guid;
Kcp kcp1(guid, 10, &mp);
Kcp kcp2(guid, 10, &mp);
int main()
{
	kcp1.OnSend = [&](auto buf, auto len)
	{
		kcp2.Input(buf, len);
	};
	kcp2.OnSend = [&](auto buf, auto len)
	{
		kcp1.Input(buf, len);
	};
	kcp2.OnRecv = [](auto buf, auto len)
	{
		//std::cout << len << " " << buf << std::endl;
	};

	uv_loop_t loop;
	uv_timer_t timer1, timer2;
	if (int r = uv_loop_init(&loop)) throw r;
	if (int r = uv_timer_init(&loop, &timer1)) throw r;
	if (int r = uv_timer_init(&loop, &timer2)) throw r;

	uv_timer_start(&timer1, [](uv_timer_t* handle)
	{
		kcp1.Send("a", 2);
		//ikcp_flush(kcp);
	}, 10, 10);

	uv_timer_start(&timer2, [](uv_timer_t* handle)
	{
		for (int i = 0; i < 1000; ++i)
		{
			kcp1.Update();
			kcp2.Update();
		}
	}, 0, 10);

	uv_run(&loop, UV_RUN_DEFAULT);
	return 0;
}
