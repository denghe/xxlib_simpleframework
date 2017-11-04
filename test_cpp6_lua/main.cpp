#pragma execution_character_set("utf-8")
#include "std_cout_helper.h"
#include <thread>
#include <chrono>
using namespace std::chrono_literals;

#ifdef _WIN32
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

inline static void SockInit() noexcept
{
#ifdef _WIN32
	WSADATA d;
	WSAStartup(MAKEWORD(2, 2), &d);
#else
	signal(SIGPIPE, SIG_IGN);	// 防止 write 一个已断开 socket 时引发该信号量
#endif
}

inline static int SockGetErrorNo() noexcept
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
	return socket(AF_INET, SOCK_STREAM, 0);
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
	int rtv = 0;
	do
	{
		fd_set wset, eset;
		FD_ZERO(&wset);
		FD_ZERO(&eset);
		FD_SET(s, &wset);
		FD_SET(s, &eset);
		rtv = ::select(int(s + 1), 0, &wset, &eset, sec < 0 ? nullptr : &tv);    // 阻塞 或 超时
	} while (rtv == -1 && SockGetErrorNo() == EINTR); // interupt by signal
	return rtv;
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
	if (r > 0) return FD_ISSET(s, &rset) ? r : -1;
	return r;
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
	int r;
	do {
		r = ::connect(s, (sockaddr *)&addr, sizeof(addr));
	} while (r == -1 && SockGetErrorNo() == EINTR);
	return r;
}

inline static std::pair<int, int> SockSend(Socket_t const& s, char const* const& buf, int len)
{
	std::pair<int, int> rtv(0, 0);
	if (len < 1)
	{
		rtv.first = -1;
		return rtv;
	}
	while (len)
	{              // 一直发，直到 发完 或 出错
		int r = ::send(s, buf, len, 0);
		if (r == -1)
		{
			int e = SockGetErrorNo();
			if (e == EAGAIN && e == EWOULDBLOCK) return rtv;
			rtv.first = e;
			return rtv;
		}
		len -= r;
		rtv.second += r;
	}
	return rtv;
}


// todo: 将 run() 改造成靠 update() 更新的状态机. 

int run()
{
	std::this_thread::sleep_for(1s);

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
		r = SockGetErrorNo();
		if (r != EINPROGRESS && r != EWOULDBLOCK)
		{
			SockClose(s);
			return -3;
		}

		r = SockWaitWritable(s, 1);	// wait 1s	// todo: 这个改造为循环检测
		if (r <= 0)
		{
			SockClose(s);
			return -4;
		}
		else
		{
			SockLen_t len = sizeof(r);
			if (SockGetSockOpt(s, SOL_SOCKET, SO_ERROR, &r, &len) == -1 || r)
			{
				SockClose(s);
				return -5;
			}
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

int main()
{
	CoutLine(run());
	return 0;
}
