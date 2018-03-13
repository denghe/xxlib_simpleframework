#pragma execution_character_set("utf-8")

#include "xxbuf.h"
#include "xxuvlib.h"
#include <stdio.h>	// printf
#ifndef _WIN32
#include <uuid.h>
#endif
#include <deque>
#include <array>

// 基于 libuv udp + kcp 的 udp 通信模块, 帧 update 状态机用法( 同 NBSocket )
struct XxUdpSocket
{
	// 这部分只是为了兼容 tcp 的调用代码尽量不改做的适配
	enum class States
	{
		Disconnected,								// 初始状态 / 出错 / 成功断开后
		Connecting,									// 执行 Connect 之后
		Connected,									// 握手并进入可收发状态之后
		Disconnecting,								// 执行 Disconnect( 延迟断开时长 ) 之后
	};

	uv_loop_t*						uv = nullptr;
	uv_udp_t*						udp = nullptr;
	ikcpcb*							kcp = nullptr;
	Guid							guid;
	int								interval = 33;	// 在 connect 时可以改. 默认对应 30 fps 的间隔时间

	sockaddr_in6					addr;			// ipv4/6 公用结构( ipv4 时硬转为 sockaddr_in )
	int								ticks = 0;		// 当前状态持续 ticks 计数 ( Disconnecting 时例外, 该值为负, 当变到 0 时, 执行 Close )
	States							state = States::Disconnected;

	int								readLen = 0;	// 接收缓冲区已存在的数据长度
	std::array<char, 131075>		readBuf;		// 接收缓冲区( 至少能含 1 个 64k 完整包 + 1 段上次处理剩下的数据 即 64k * 2 + 1.5 个包头 3 字节 )

	std::deque<XxBuf>				recvBufs;		// 收到的"包"数据将堆积在此

	XxUdpSocket()
	{
		// todo: 创建独立 uv 结构( 先不做 )
		uv = uv_default_loop();		
		addr.sin6_family = 0;						// 用这个来做是否有设置过 addr 的标记
	}

	~XxUdpSocket()
	{
		Close(0);
		// todo: 析构 uv( 先不做 )
	}

	// 保留一个内存头存 userdata
	inline static void* Alloc(size_t size, void* ud)
	{
		auto p = (void**)malloc(size + sizeof(void*));
		p[0] = ud;
		return &p[1];
	}

	inline static void* Alloc(size_t size)
	{
		return (void**)malloc(size + sizeof(void*)) + 1;
	}

	inline static void Free(void* p) noexcept
	{
		free((void**)p - 1);
	}

	// 关闭 libuv handle
	inline static void Close(uv_handle_t* h)
	{
		if (uv_is_closing(h)) return;
		uv_close(h, [](uv_handle_t* h)
		{
			Free(h);
		});
	}

	inline static void AllocCB(uv_handle_t* h, size_t suggested_size, uv_buf_t* buf)
	{
		buf->base = (char*)Alloc(suggested_size);
		buf->len = decltype(buf->len)(suggested_size);
	}

	inline void OnReceiveImpl(char const* bufPtr, int len, void* addr)
	{
		if (len < 36) return;
		if (int r = ikcp_input(kcp, bufPtr, len)) throw r;
	}

	inline static void OnRecvCBImpl(void* uvudp, ptrdiff_t nread, void* buf_t, void* addr, uint32_t flags)
	{
		auto client = *((XxUdpSocket**)uvudp - 1);
		auto bufPtr = ((uv_buf_t*)buf_t)->base;
		int len = (int)nread;
		if (len > 0)
		{
			client->OnReceiveImpl(bufPtr, len, addr);
		}
		Free(bufPtr);
		//if (len < 0) return;
	}

	// 设置目标地址( v4 ip串, 不支持域名 )( 针对苹果机, 多一步使用 getaddrinfo 将 ip 转为 sockaddr_in/6 结构体的过程 )
	inline void SetAddress(char const* const& ip, uint16_t const& port)
	{
		xxuv_ip4_addr(ip, port, (sockaddr*)&addr);
	}

	// 并非连接目标地址服务器. 只是初始化 socket kcp 和参数些. udp 不需要连接.
	// 返回非 0 表示出错.
	inline int Connect(int sndwnd = 128, int rcvwnd = 128
		, int nodelay = 1, int interval = 33, int resend = 2, int nc = 1, int minrto = 100)
	{
		if (state != States::Disconnected) return -2;	// wrong state
		if (!addr.sin6_family) return -3;				// not set address ?
		if (udp) return -4;								// not Disconnect ?

		// renew Guid
#ifdef _WIN32
		CoCreateGuid((GUID*)&guid);
#else
		uuid_generate(reinterpret_cast<unsigned char *>(&guid));
#endif

		udp = (uv_udp_t*)Alloc(sizeof(uv_udp_t), this);
		if (!udp) return -5;

		if (int r = uv_udp_init(uv, udp))
		{
			Free(udp);
			udp = nullptr;
			return r;
		}

		if (int r = uv_udp_recv_start(udp, AllocCB, (uv_udp_recv_cb)OnRecvCBImpl))
		{
			Close((uv_handle_t*)udp);
			udp = nullptr;
			return r;
		}

		kcp = ikcp_create(&guid, this, nullptr);
		if (!kcp)
		{
			Close((uv_handle_t*)udp);
			udp = nullptr;
			return -6;
		}

		ikcp_wndsize(kcp, sndwnd, rcvwnd);
		ikcp_nodelay(kcp, nodelay, interval, resend, nc);
		this->interval = interval;

		ikcp_setoutput(kcp, [](const char *inBuf, int len, ikcpcb *kcp)->int
		{
			auto self = (XxUdpSocket*)kcp->user;
			struct uv_udp_send_t_ex
			{
				uv_udp_send_t req;
				uv_buf_t buf;
			};
			auto req = (uv_udp_send_t_ex*)Alloc(sizeof(uv_udp_send_t_ex));
			auto buf = (char*)Alloc(len);
			memcpy(buf, inBuf, len);
			req->buf = uv_buf_init(buf, (uint32_t)len);
			return uv_udp_send((uv_udp_send_t*)req, self->udp, &req->buf, 1, (sockaddr*)&self->addr, [](uv_udp_send_t* req, int status)
			{
				//if (status) fprintf(stderr, "Write error: %s\n", uv_strerror(status));
				auto *wr = (uv_udp_send_t_ex*)req;
				Free(wr->buf.base);
				Free(wr);
			});

		});

		state = States::Connected;
		ticks = 0;
		return 1;										// success
	}

	// 断开连接
	inline void Disconnect(int delayTicks = 0)
	{
		if (state == States::Disconnected) return;
		if (!delayTicks)
		{
			Close(0);
		}
		else
		{
			state = States::Disconnecting;
			ticks = -delayTicks;
		}
	}

	// 帧更新函数
	// 返回负数表示出错. 0 表示没发生错误
	inline int Update(int const& sec = 0, int const& usec = 0)
	{
		++ticks;
		uv_run(uv, uv_run_mode::UV_RUN_NOWAIT);
		switch (state)
		{
		case States::Connecting:
		{
			return -1;								// udp 不存在这个状态
		}
		case States::Disconnecting:
		{
			if (!ticks) Close(0);
		}
		}
		ikcp_update(kcp, interval * ticks);
		while (true)
		{
			int r = ikcp_recv(kcp, readBuf.data() + readLen, (int)readBuf.size() - readLen);
			if (r < 0) break;
			else if (r == 0) return Close(-2);
			else // r > 0
			{
				readLen += r;

				auto buf = (uint8_t*)readBuf.data();			// 无符号方便位操作读出头
				int offset = 0;									// 游标

				while (readLen - offset >= 3)					// 确保 3字节 包头长度
				{
					// 读出长度信息( 首字节 pkg type id 用不到, 直接跳过 )
					auto dataLen = buf[offset + 1] + (buf[offset + 2] << 8);
					if (!dataLen) return Close(-3);				// 异常: 读不到长度, 关闭连接
					dataLen += 3;								// 将包头的长度纳入
					if (offset + dataLen > readLen) break;		// 确保数据长

																// 将数据弄到 recvBufs( 含包头, 以便上层代码继续解析 )
					recvBufs.emplace_back((char*)buf + offset, dataLen);
					offset += dataLen;
				}

				// 剩余数据移到首部后退出
				readLen -= offset;
				if (readLen)
				{
					memmove(buf, buf + offset, readLen);
				}
			}
		}
		return 0;
	}


	// 返回非 0 表示出错
	inline int Send(char const* const& buf, int const& dataLen)
	{
		if (dataLen <= 0) return -1;
		if (state != States::Connected) return -1;

		return ikcp_send(kcp, buf, dataLen);
	}

	// Send 之 BB 版
	inline int Send(XxBuf const* const& bb)
	{
		return Send(bb->buf, bb->dataLen);
	}

private:

	// 关闭连接, 清状态 及 收发包, 返回传入的错误以简化一些代码书写
	inline int Close(int e)
	{
		if (state == States::Disconnected) return e;
		state = States::Disconnected;
		ticks = 0;
		Close((uv_handle_t*)udp);
		udp = nullptr;
		ikcp_release(kcp);
		kcp = nullptr;
		return e;
	}
};
