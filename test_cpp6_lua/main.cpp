#pragma execution_character_set("utf-8")
#include "lua_mempool.h"

#include "std_cout_helper.h"
#include <thread>
#include <chrono>
using namespace std::chrono_literals;

#ifdef _WIN32
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#include <WinSock2.h>
#pragma comment(lib, "WS2_32") 
#undef min
#undef max
static_assert(SOCKET_ERROR == -1 && INVALID_SOCKET == -1);
typedef SOCKET      Socket_t;
typedef int         SockLen_t;
#else
typedef int         Socket_t;
typedef socklen_t   SockLen_t;
#endif


enum class NBSocketStates
{
	Disconnected,				// 初始状态 / 出错 / 成功断开后
	Connecting,					// 执行 Connect 之后
	Connected,					// 握手并进入可收发状态之后
	Disconnecting,				// 执行 Disconnect( 延迟断开时长 ) 之后
};

struct NBSocket
{
	// todo: 提供 SetAddress, Connect, GetState, Send, GetRecv, Disconnect, Clear, Update, GetTicks

	// 结构说明 + 变长数据区. 因为只是用于 client 发送, 此处并不试图省掉秒种内存复制. 
	struct Buf
	{
		uint32_t    dataLen;
		uint32_t	offset;						// 已 读 | 发送 长度
		Buf*		next;						// 指向下一段待发送数据

		inline char* GetData()					// 指向数据区
		{
			return (char*)(this + 1);
		}
	};

	Lua_MemPool*	mempool;					// 指向内存池
	Socket_t		sock = -1;
	NBSocketStates	state = NBSocketStates::Disconnected;
	int				ticks = 0;					// 当前状态持续 ticks 计数 ( Disconnecting 时例外, 该值为负, 当变到 0 时, 执行 Close )
	sockaddr_in		addr;
	Buf*			curr = nullptr;				// 正在发送的数据块
	Buf*			last = nullptr;				// 指向尾数据块

	NBSocket()
	{
		addr.sin_port = 0;	// 用这个来做是否有设置过 addr 的标记
	}

	~NBSocket()
	{
		// todo: close socket? 回收 curr last?
	}

	// 设置目标地址
	inline void SetAddress(char const* const& ip, uint16_t const& port)
	{
		SockSetAddress(addr, ip, port);
	}

	// 返回负数 表示出错. 0 表示没发生错误 但也没连上. 1 表示连接成功
	inline int Connect(int const& sec = 0, int const& usec = 0)
	{
		if (state != NBSocketStates::Disconnected) return -2;	// wrong state
		if (!addr.sin_port) return -3;							// not set address ?
		if (sock != -1) return -4;								// socket is created

		sock = SockCreate();
		if (sock == -1) return -5;								// socket create fail

		int r = SockSetNonBlock(sock);
		if (r) return E(-6);									// set non block fail

		r = SockConnect(sock, addr);
		if (r == -1)
		{
			r = SockGetErrNo();
			if (r != EINPROGRESS && r != EWOULDBLOCK) return E(-7);	// connect fail

			r = SockWaitWritable(sock, sec, usec);
			if (r < 0) return E(-8);							// wait writable fail
			else if (r == 0)
			{
				state = NBSocketStates::Connecting;
				return 0;										// timeout( not error )
			}
		}
		state = NBSocketStates::Connected;
		return 1;												// success
	}

	// 断开连接
	inline void Disconnect(int delayTicks = 0)
	{
		if (state == NBSocketStates::Disconnected) return;
		if (!delayTicks)
		{
			E(0);
		}
		else
		{
			state = NBSocketStates::Disconnecting;
			ticks = -delayTicks;
		}
	}

	// 返回负数表示出错. 0 表示没发生错误
	inline int Update(int const& sec = 0, int const& usec = 0)
	{
		++ticks;
		switch (state)
		{
		case NBSocketStates::Disconnected:
		{
			return 0;
		}
		case NBSocketStates::Connecting:
		{
			int r = SockWaitWritable(sock, sec, usec);
			if (r < 0) return E(-8);			// wait writable fail
			else if (r == 0) return 0;			// timeout( not error )
			else
			{
				state = NBSocketStates::Connected;
				return 0;
			}
		}
		case NBSocketStates::Connected:
		{
			// todo: 判断是否有待发数据, 持续发. 有数据收则收到队列. 
			if (curr)
			{
				auto buf = curr->GetData() + curr->offset;
				auto len = curr->dataLen - curr->offset;
				auto r = SockSend(sock, buf, len);
				if (r.first) return E(-2);
				if (r.second < len)
				{
					curr->offset += r.second;
				}
				else // todo
				{
					curr = curr->next;
				}
			}

			return 0;
		}
		case NBSocketStates::Disconnecting:
		{
			if (!ticks) E(0);
			return 0;
		}
		default:
			return -1;							// unknown state
		}
	}

	inline NBSocketStates const& GetState() const noexcept
	{
		return state;
	}

	inline int const& GetTicks() const noexcept
	{
		return ticks;
	}

	// 返回负数表示出错. 0 表示成功放入待发送链表或无数据可发. > 0 表示已立刻发送成功的长度. 
	// 如果有剩下部分, 会放入待发送链表, 在下次 Update 时继续发
	inline int Send(char const* const& buf, int dataLen, int const& offset)
	{
		if (dataLen < 0 || offset < 0 || offset > dataLen) return -1;
		if (offset == dataLen) return 0;

		// 算从哪发, 发多长
		auto p = buf + offset;
		auto len = dataLen - offset;

		// 判断当前是否存在待发数据. 如果有就追加到后面
		if (last)
		{
			PushBuf(p, len);
			return 0;
		}

		// 直接发. 如果出错则关闭 socket, 改 state 并返回错误码.
		auto r = SockSend(sock, p, len);
		if (r.first) return E(-2);

		// 将没发完的数据追加到待发
		if (len < r.second)
		{
			PushBuf(p + r.second, len - r.second);
		}
		return r.second;
	}

	inline void PushBuf(char const* const& buf, int const& len)
	{
		auto b = (Buf*)mempool->Alloc(sizeof(Buf) + len);
		b->dataLen = len;
		b->offset = 0;
		b->next = nullptr;
		memcpy(b + 1, buf, len);

		if (last)
		{
			assert(curr);
			last->next = b;
		}
		else
		{
			assert(!curr);
			curr = b;
		}
	}

private:

	// 简化同时关连接改状态的代码
	inline int E(int e)
	{
		SockClose(sock);
		sock = -1;
		state = NBSocketStates::Disconnected;
		ticks = 0;
		// todo: 清各种 buf
		return e;
	}

public:

	// todo: 将 run() 改造成靠 update() 更新的状态机. 

	inline static int Run()
	{
		std::this_thread::sleep_for(200ms);

		SockInit();

		auto s = SockCreate();
		if (s == -1) return -1;

		int r = SockSetNonBlock(s);
		if (r)
		{
			SockClose(s);
			return -2;
		}

		sockaddr_in addr;
		SockSetAddress(addr, "127.0.0.1", 12345);

		r = SockConnect(s, addr);
		if (r == -1)
		{
			r = SockGetErrNo();
			if (r != EINPROGRESS && r != EWOULDBLOCK)
			{
				SockClose(s);
				return -3;
			}

			do
			{
				r = SockWaitWritable(s, 0);
				std::this_thread::sleep_for(1ms);
			} while (!r);

			if (r < 0)
			{
				SockClose(s);
				return -4;
			}
		}

		auto buf = "asdf";
		int bufLen = 4, left = 4;
		while (left)
		{
			auto rtv = SockSend(s, buf + bufLen - left, left);
			if (rtv.first)
			{
				SockClose(s);
				return -6;
			}
			left -= rtv.second;
		}

		SockClose(s);
		return 0;
	}



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

	inline static int SockGetSockOpt(Socket_t const& s, int const& level, int const& optname, void* const& optval, SockLen_t* const& optlen) noexcept
	{
#ifdef _WIN32
		return ::getsockopt(s, level, optname, (char*)optval, optlen);
#else
		return ::getsockopt(s, level, optname, optval, optlen);
#endif
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

	inline static int SockWaitWritable(Socket_t const& s, int const& sec = -1, int const& usec = 0) noexcept
	{
		timeval tv;
		if (sec >= 0)
		{
			tv.tv_sec = sec;
			tv.tv_usec = usec;
		}
		fd_set wset, eset;
	LabRetry:
		FD_ZERO(&wset);
		FD_ZERO(&eset);
		FD_SET(s, &wset);
		FD_SET(s, &eset);
		int r = ::select(int(s + 1), 0, &wset, &eset, sec < 0 ? nullptr : &tv);
		if (r == -1 && SockGetErrNo() == EINTR) goto LabRetry;

		if (r == 0) return 0;			// 超时
		else if (r == -1) return -1;
		else
		{
			SockLen_t len = sizeof(r);
			if (SockGetSockOpt(s, SOL_SOCKET, SO_ERROR, &r, &len) == -1 || r) return -1;
			return 1;
		}
	}

	inline static int SockWaitReadable(Socket_t const& s, int const& sec = -1, int const& usec = 0) noexcept
	{
		timeval tv;
		if (sec >= 0)
		{
			tv.tv_sec = sec;
			tv.tv_usec = usec;
		}
		fd_set rset;
		FD_ZERO(&rset);
		FD_SET(s, &rset);
		int r = ::select(int(s + 1), &rset, 0, 0, sec < 0 ? nullptr : &tv);
		if (r > 0) return FD_ISSET(s, &rset) ? 0 : -1;
		return -1;
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


void DumpState(NBSocketStates s)
{
	static char const* ss[] =
	{
		"Disconnected",
		"Connecting",
		"Connected",
		"Disconnecting"
	};
	CoutLine("state = ", ss[(int)s]);
}

int main()
{
	//CoutLine(NBSocket::Run());

	NBSocket::SockInit();
	NBSocket nbs;

	auto s = nbs.GetState();
	DumpState(s);

	nbs.SetAddress("127.0.0.1", 12345);
	nbs.Connect();

	DumpState(s = nbs.GetState());
	while (true)
	{
		s = nbs.GetState();
		switch (s)
		{
		case NBSocketStates::Disconnected:
		{
			goto LabEnd;
		}
		case NBSocketStates::Connecting:
		{
			if (nbs.GetTicks() > 10)	// 1 秒连不上就算超时吧
			{
				nbs.Disconnect();
			}
			break;
		}
		case NBSocketStates::Connected:
		{
			// todo: Send
			nbs.Disconnect(2);		// 延迟 2 帧杀掉
			break;
		}
		case NBSocketStates::Disconnecting:
			break;

		default:
			break;
		}
		DumpState(s = nbs.GetState());
		std::this_thread::sleep_for(100ms);
		nbs.Update();
	}
LabEnd:
	DumpState(s = nbs.GetState());
	CoutLine("press any key to continue...");
	std::cin.get();
	return 0;
}
