#pragma execution_character_set("utf-8")
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
	// todo: 提供 SetAddress, Connect, GetState, Send, GetRecv, Disconnect, Clear, Update, GetElapsedMS

	Socket_t sock = -1;
	NBSocketStates state = NBSocketStates::Disconnected;
	sockaddr_in addr;

	NBSocket()
	{
		addr.sin_port = 0;	// 用这个来做是否有设置过 addr 的标记
	}
	inline void SetAddress(char const* const& ip, uint16_t const& port)
	{
		SockSetAddress(addr, ip, port);
	}

	// 简化同时关连接改状态的代码
	inline int E(int e)
	{
		SockClose(sock);
		sock = -1;
		state = NBSocketStates::Disconnected;
		return e;
	}

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

	inline int Update(int const& sec = 0, int const& usec = 0)
	{
		switch (state)
		{
		case NBSocketStates::Disconnected:
		{
			return 0;
		}
		case NBSocketStates::Connecting:
		{
			int r = SockWaitWritable(sock, sec, usec);
			if (r < 0) return E(-8);							// wait writable fail
			else if (r == 0) return 0;							// timeout( not error )
			else
			{
				state = NBSocketStates::Connected;
				return 0;
			}
		}
		case NBSocketStates::Connected:
		{
			// todo: 判断是否有待发数据, 持续发. 有数据收则收到队列. 
			return 0;
		}
		case NBSocketStates::Disconnecting:
		{
			// todo: 状态检查 / 超时后close
			return 0;
		}
		default:
			return -1;
		}
	}

	inline NBSocketStates const& GetState() const noexcept
	{
		return state;
	}


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
		std::pair<int, int> rtv(0, 0);		// err, sent len
		if (len < 1)
		{
			rtv.first = -1;
			return rtv;
		}
		while (len)							// 一直发，直到 发完 或 出错
		{
			int r = ::send(s, buf, len, 0);
			if (r == -1)
			{
				int e = SockGetErrNo();
				if (e == EAGAIN && e == EWOULDBLOCK) return rtv;
				rtv.first = e;
				return rtv;
			}
			len -= r;
			rtv.second += r;
		}
		return rtv;
	}
};



int main()
{
	CoutLine(NBSocket::Run());
	std::cin.get();
	return 0;
}
