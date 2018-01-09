#pragma execution_character_set("utf-8")

#ifdef _WIN32
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#include <WinSock2.h>								// 含 windows.h
#pragma comment(lib, "WS2_32") 
#undef min											// 免得干扰 std::min
#undef max											// 免得干扰 std::max
static_assert(SOCKET_ERROR == -1 && INVALID_SOCKET == -1);
typedef SOCKET      Socket_t;
typedef int         SockLen_t;
#else
#include <unistd.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <errno.h>
#include <fcntl.h>
typedef int         Socket_t;
typedef socklen_t   SockLen_t;
#endif

#include "xxbuf.h"
#include <deque>
#include <array>

// 非阻塞, 帧 update 状态机用法的 tcp client
struct XxNBSocket
{
	enum class States
	{
		Disconnected,								// 初始状态 / 出错 / 成功断开后
		Connecting,									// 执行 Connect 之后
		Connected,									// 握手并进入可收发状态之后
		Disconnecting,								// 执行 Disconnect( 延迟断开时长 ) 之后
	};

	Socket_t					sock = -1;
	States						state = States::Disconnected;
	int							ticks = 0;			// 当前状态持续 ticks 计数 ( Disconnecting 时例外, 该值为负, 当变到 0 时, 执行 Close )
	sockaddr_in					addr;

	std::deque<XxBuf>			sendBufs;			// 未及时发走的数据将堆积在此
	std::deque<XxBuf>			recvBufs;			// 收到的"包"数据将堆积在此

	int							readLen = 0;		// 接收缓冲区已存在的数据长度
	std::array<char, 131075>	readBuf;			// 接收缓冲区( 至少能含 1 个 64k 完整包 + 1 段上次处理剩下的数据 即 64k * 2 + 1.5 个包头 3 字节 )

	XxNBSocket()
	{
		addr.sin_port = 0;							// 用这个来做是否有设置过 addr 的标记
	}

	~XxNBSocket()
	{
		if (sock != -1) Close(0);
	}

	// 设置目标地址
	inline void SetAddress(char const* const& ip, uint16_t const& port)
	{
		SockSetAddress(addr, ip, port);
	}

	// 连接目标地址服务器. 可设阻塞时长. 
	// 返回负数 表示出错. 0 表示没发生错误 但也没连上. 1 表示连接成功
	inline int Connect(int const& sec = 0, int const& usec = 0)
	{
		if (state != States::Disconnected) return -2;// wrong state
		if (!addr.sin_port) return -3;				 // not set address ?
		if (sock != -1) return -4;					 // socket is created

		sock = SockCreate();
		if (sock == -1) return -5;					 // socket create fail

		int r = SockSetNonBlock(sock);
		if (r) return Close(-6);					 // set non block fail

		r = SockConnect(sock, addr);
		if (r == -1)
		{
			r = SockGetErrNo();
			if (r != EINPROGRESS && r != EWOULDBLOCK) return Close(-7);	// connect fail

			r = SockWaitReadOrWritable(sock, false, sec, usec);
			if (r < 0) return Close(-8);			// wait writable fail
			else if (r == 0)
			{
				state = States::Connecting;
				ticks = 0;
				return 0;							// timeout( not error )
			}
		}
		state = States::Connected;
		ticks = 0;
		return 1;									// success
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
			int r = SockWaitReadOrWritable(sock, false, sec, usec);
			if (r < 0) return Close(-8);			// wait writable fail
			else if (r == 0) return 0;				// timeout( not error )
			else
			{
				state = States::Connected;
				ticks = 0;
				return 0;
			}
		}
		case States::Connected:
		{
			// 判断 socket 是否处于 待接收数据 状态
			int r = SockWaitReadOrWritable(sock, true, sec, usec);
			if (r < 0) return Close(r);
			else if (r > 0)
			{
			LabRetry:
				r = ::recv(sock, readBuf.data() + readLen, (int)readBuf.size() - readLen, 0);
				if (r < 0)
				{
					r = SockGetErrNo();
					if (r == EINTR) goto LabRetry;
					if (r != EAGAIN && r != EWOULDBLOCK) return Close(-1);
				}
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

			// 判断是否有待发数据, 持续发. 有数据收则收到队列. 
			if (!sendBufs.empty())
			{
				auto& front = sendBufs.front();
				auto len = front.dataLen - front.offset;
				auto r = SockSend(sock, front.buf + front.offset, len);
				if (r.first) return Close(-2);
				if (r.second < (int)len)
				{
					front.offset += r.second;
				}
				else
				{
					sendBufs.pop_front();
				}
			}
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
	}


	// 返回负数表示出错. 0 表示成功放入待发送队列或无数据可发. > 0 表示已立刻发送成功的长度. 
	// 如果有剩下部分, 会放入待发送队列, 在下次 Update 时继续发
	inline int Send(char const* const& buf, int const& dataLen)
	{
		if (dataLen <= 0) return -1;

		// 判断当前是否存在待发数据. 如果有就追加到后面 并返回 0
		if (!sendBufs.empty())
		{
			sendBufs.emplace_back(buf, dataLen);
			return 0;
		}

		// 直接发. 如果出错则关闭 socket, 改 state 并返回错误码.
		auto r = SockSend(sock, buf, dataLen);
		if (r.first) return Close(-2);

		// 将没发完的数据追加到待发
		if (dataLen > r.second)
		{
			sendBufs.emplace_back(buf + r.second, dataLen - r.second);
		}
		return r.second;
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
		SockClose(sock);
		sock = -1;
		state = States::Disconnected;
		ticks = 0;
		readLen = 0;
		sendBufs.clear();
		recvBufs.clear();
		return e;
	}

public:

	/**********************************************************************************************************/
	// 一些静态工具函数
	/**********************************************************************************************************/

	inline static void SockInit() noexcept
	{
#ifdef _WIN32
		WSADATA d;
		WSAStartup(MAKEWORD(2, 2), &d);
#else
		signal(SIGPIPE, SIG_IGN);	// 防止 write 一个已断开 socket 时引发该信号量
#endif
	}

	inline static int SockGetErrNo() noexcept
	{
#ifdef _WIN32
		int e = WSAGetLastError();
		return e == WSAEWOULDBLOCK ? EWOULDBLOCK : e;
#else
		return errno;
#endif
	}

	inline static Socket_t SockCreate()
	{
		return ::socket(AF_INET, SOCK_STREAM, 0);
	}

	inline static int SockClose(Socket_t const& s) noexcept
	{
#ifdef _WIN32
		return ::closesocket(s);
#else
		return ::close(s);
#endif
	}

	inline static int SockSetNonBlock(Socket_t const& s) noexcept
	{
#ifdef _WIN32
		u_long v = -1;
		return ::ioctlsocket(s, FIONBIO, &v);
#else
		int flag = ::fcntl(s, F_GETFL, 0);
		if ((flag & O_NONBLOCK) == O_NONBLOCK) return 0; // already nonblock
		return ::fcntl(s, F_SETFL, flag | O_NONBLOCK);
#endif
	}

	inline static int SockWaitReadOrWritable(Socket_t const& s, bool isRead = true, int const& sec = -1, int const& usec = 0) noexcept
	{
		timeval tv;
		if (sec >= 0)
		{
			tv.tv_sec = sec;
			tv.tv_usec = usec;
		}
		fd_set rwset, eset;
		int r;
	LabRetry:
		FD_ZERO(&rwset);
		FD_ZERO(&eset);
		FD_SET(s, &rwset);
		FD_SET(s, &eset);
		if (isRead)
		{
			r = ::select(int(s + 1), &rwset, nullptr, &eset, sec < 0 ? nullptr : &tv);
		}
		else
		{
			r = ::select(int(s + 1), nullptr, &rwset, &eset, sec < 0 ? nullptr : &tv);
		}
		if (r == -1 && SockGetErrNo() == EINTR) goto LabRetry;
		if (r <= 0) return r;									// 超时 或 出错
		return FD_ISSET(s, &rwset) ? r : -1;					// 因为只 select 1 个 socket. 不在 rwset 即在 eset 即出错
	}

	inline static void SockSetAddress(sockaddr_in& addr, char const* const& ip, uint16_t const& port)
	{
		memset(addr.sin_zero, 0, sizeof(addr.sin_zero));
		addr.sin_family = AF_INET;
		addr.sin_addr.s_addr = inet_addr(ip);
		addr.sin_port = htons(port);
	}

	inline static int SockConnect(Socket_t const& s, sockaddr_in const& addr)
	{
	LabRetry:
		int r = ::connect(s, (sockaddr *)&addr, sizeof(addr));
		if (r == -1 && SockGetErrNo() == EINTR) goto LabRetry;
		return r;
	}

	inline static std::pair<int, int> SockSend(Socket_t const& s, char const* const& buf, int len)
	{
		std::pair<int, int> rtv(0, 0);		// errno, sent len
		assert(buf && len > 0);
		int left = len;
		while (left)
		{
			int r = ::send(s, buf + len - left, left, 0);
			if (r == -1)
			{
				int e = SockGetErrNo();
				if (e == EAGAIN && e == EWOULDBLOCK) break;
				rtv.first = -1;
				break;
			}
			left -= r;
		}
		rtv.second = len - left;
		return rtv;
	}
};
