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

	uv_loop_t*					uv = nullptr;
	uv_udp_t*					udp = nullptr;
	ikcpcb*						kcp = nullptr;
	Guid guid;

	sockaddr_in6				addr;				// ipv4/6 公用结构( ipv4 时硬转为 sockaddr_in )
	int							ticks = 0;			// 当前状态持续 ticks 计数 ( Disconnecting 时例外, 该值为负, 当变到 0 时, 执行 Close )
	States						state = States::Disconnected;

	std::deque<XxBuf>			sendBufs;			// 未及时发走的数据将堆积在此
	std::deque<XxBuf>			recvBufs;			// 收到的"包"数据将堆积在此

	int							readLen = 0;		// 接收缓冲区已存在的数据长度
	std::array<char, 131075>	readBuf;			// 接收缓冲区( 至少能含 1 个 64k 完整包 + 1 段上次处理剩下的数据 即 64k * 2 + 1.5 个包头 3 字节 )

	XxUdpSocket()
	{
		uv = uv_default_loop();
		addr.sin6_family = 0;						// 用这个来做是否有设置过 addr 的标记
	}

	~XxUdpSocket()
	{
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
		, int nodelay = 1, int interval = 10, int resend = 2, int nc = 1, int minrto = 100)
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
		
		// todo: kcp_create, ...
		udp = (uv_udp_t*)Alloc(sizeof(uv_udp_t), this);
		if (udp == nullptr) return -5;

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
			throw r;
		}

		// todo: kcp_create

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
		switch (state)
		{
		case States::Disconnected:
		{
			return 0;
		}
		case States::Connecting:
		{
			return -1;								// invalid state
		}
		case States::Connected:
		{
			// todo: call uv run nowait

		//	// 判断 socket 是否处于 待接收数据 状态
		//	int r = SockWaitReadOrWritable(sock, true, sec, usec);
		//	if (r < 0) return Close(r);
		//	else if (r > 0)
		//	{
		//	LabRetry:
		//		r = ::recv(sock, readBuf.data() + readLen, (int)readBuf.size() - readLen, 0);
		//		if (r < 0)
		//		{
		//			r = SockGetErrNo();
		//			if (r == EINTR) goto LabRetry;
		//			if (r != EAGAIN && r != EWOULDBLOCK) return Close(-1);
		//		}
		//		else if (r == 0) return Close(-2);
		//		else // r > 0
		//		{
		//			readLen += r;

		//			auto buf = (uint8_t*)readBuf.data();			// 无符号方便位操作读出头
		//			int offset = 0;									// 游标

		//			while (readLen - offset >= 3)					// 确保 3字节 包头长度
		//			{
		//				// 读出长度信息( 首字节 pkg type id 用不到, 直接跳过 )
		//				auto dataLen = buf[offset + 1] + (buf[offset + 2] << 8);
		//				if (!dataLen) return Close(-3);				// 异常: 读不到长度, 关闭连接
		//				dataLen += 3;								// 将包头的长度纳入
		//				if (offset + dataLen > readLen) break;		// 确保数据长

		//				// 将数据弄到 recvBufs( 含包头, 以便上层代码继续解析 )
		//				recvBufs.emplace_back((char*)buf + offset, dataLen);
		//				offset += dataLen;
		//			}

		//			// 剩余数据移到首部后退出
		//			readLen -= offset;
		//			if (readLen)
		//			{
		//				memmove(buf, buf + offset, readLen);
		//			}
		//		}
		//	}

		//	// 判断是否有待发数据, 持续发. 有数据收则收到队列. 
		//	if (!sendBufs.empty())
		//	{
		//		auto& front = sendBufs.front();
		//		auto len = front.dataLen - front.offset;
		//		auto r = SockSend(sock, front.buf + front.offset, len);
		//		if (r.first) return Close(-2);
		//		if (r.second < (int)len)
		//		{
		//			front.offset += r.second;
		//		}
		//		else
		//		{
		//			sendBufs.pop_front();
		//		}
		//	}
			return 0;
		}
		case States::Disconnecting:
		{
			if (!ticks) Close(0);
			return 0;
		}
		default:
			return -1;								// unknown state
		}

		return 0;
	}


	// 返回负数表示出错. 0 表示成功放入待发送队列或无数据可发. > 0 表示已立刻发送成功的长度. 
	// 如果有剩下部分, 会放入待发送队列, 在下次 Update 时继续发
	inline int Send(char const* const& buf, int const& dataLen)
	{
		if (dataLen <= 0) return -1;

		//// 判断当前是否存在待发数据. 如果有就追加到后面 并返回 0
		//if (!sendBufs.empty())
		//{
		//	sendBufs.emplace_back(buf, dataLen);
		//	return 0;
		//}

		//// 直接发. 如果出错则关闭 socket, 改 state 并返回错误码.
		//auto r = SockSend(sock, buf, dataLen);
		//if (r.first) return Close(-2);

		//// 将没发完的数据追加到待发
		//if (dataLen > r.second)
		//{
		//	sendBufs.emplace_back(buf + r.second, dataLen - r.second);
		//}
		//return r.second;

		return 0;
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
		// todo
		//SockClose(sock);
		//sock = -1;

		state = States::Disconnected;
		ticks = 0;
		readLen = 0;
		sendBufs.clear();	// todo: 可能不再需要这两个容器
		recvBufs.clear();
		return e;
	}
};
