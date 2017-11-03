#pragma execution_character_set("utf-8")
#pragma once

#ifdef _WIN32
#include <WinSock2.h>
#undef min
#undef max
#endif

struct Lua_Socket
{

#ifdef _WIN32
	static_assert(SOCKET_ERROR == -1);
	typedef SOCKET      Socket_t;
	typedef int         SockLen_t;
#else
	typedef int         Socket_t;
	typedef socklen_t   SockLen_t;
#endif

	inline static void Init()
	{
#ifdef _WIN32
		WSADATA d;
		WSAStartup(MAKEWORD(2, 2), &d);
#else
		signal(SIGPIPE, SIG_IGN);	// 防止 write 一个已断开 socket 时引发该信号量
#endif

		// todo: register to lua
	}

	// 下面都是些内部函数. 得改成成员函数. 加上状态管理

	inline static int GetErrorNumber()
	{
#ifdef _WIN32
		int e = WSAGetLastError();
		return e == WSAEWOULDBLOCK ? EWOULDBLOCK : e;
#else
		return errno;
#endif
	}

	inline static int GetSockOpt(Socket_t s, int level, int optname, void* optval, SockLen_t* optlen)
	{
#ifdef _WIN32
		return ::getsockopt(s, level, optname, (char*)optval, optlen);
#else
		return ::getsockopt(s, level, optname, optval, optlen);
#endif
	}

	inline static int CloseSocket(Socket_t s)
	{
#ifdef _WIN32
		return ::closesocket(s);
#else
		return ::close(s);
#endif
	}

	inline static int SetNonBlock(Socket_t s)
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

	inline static int WaitForWritable(Socket_t s, int timeoutMS)
	{
		timeval tv;
		if (timeoutMS > 0)
		{
			tv.tv_sec = timeoutMS / 1000;
			tv.tv_usec = (timeoutMS % 1000) * 1000;
		}
		int rtv = 0;
		do
		{
			fd_set wset, eset;
			FD_ZERO(&wset);
			FD_ZERO(&eset);
			FD_SET(s, &wset);
			FD_SET(s, &eset);
			rtv = ::select(int(s + 1), 0, &wset, &eset, timeoutMS < 0 ? 0 : &tv);    // 阻塞 或 超时
		} while (rtv == -1 && GetErrorNumber() == EINTR); // interupt by signal
		return rtv;
	}

	inline static int WaitForReadable(Socket_t s, int timeoutMS)
	{
		static_assert(SOCKET_ERROR == -1);
		timeval tv;
		if (timeoutMS > 0)
		{
			tv.tv_sec = timeoutMS / 1000;
			tv.tv_usec = (timeoutMS % 1000) * 1000;
		}
		fd_set rset;
		FD_ZERO(&rset);
		FD_SET(s, &rset);
		int r = ::select(int(s + 1), &rset, 0, 0, timeoutMS < 0 ? 0 : &tv);
		if (r > 0) return FD_ISSET(s, &rset) ? r : -1;
		return r;
	}

	// 成功返回 socket. 失败返回 -1.
	inline static Socket_t Connect(uint32_t ip, uint16_t port, int timeoutMS)
	{
		Socket_t s = socket(AF_INET, SOCK_STREAM, 0);      // create socket

		sockaddr_in addr;                                   // create addr ipv4
		addr.sin_family = AF_INET;
		addr.sin_port = htons(port);
		addr.sin_addr.s_addr = ip;
		memset(addr.sin_zero, 0, sizeof(addr.sin_zero));

		int r;
		do
		{
			SetNonBlock(s);
			do
			{
				r = ::connect(s, (sockaddr *)&addr, sizeof(addr));
			} while (r == -1 && GetErrorNumber() == EINTR);

			if (r == -1)
			{
				r = GetErrorNumber();
				if (r != EINPROGRESS && r != EWOULDBLOCK)
				{
					break;
				}

				r = WaitForWritable(s, timeoutMS);
				if (r <= 0)
				{
					r = -1;
				}
				else
				{
					SockLen_t len = sizeof(SockLen_t);
					if (GetSockOpt(s, SOL_SOCKET, SO_ERROR, &r, &len) == -1 || r != 0)
					{
						r = -1;
					}
				}
			}
		} while (0);

		if (r == -1)
		{
			CloseSocket(s);
			return -1;
		}
		return s;
	}

	inline static int Receive(Socket_t s, char* buf, int bufLen, int timeoutMS)
	{
		int n = WaitForReadable(s, timeoutMS);
		if (n > 0)
		{
			do
			{
				n = ::recv(s, buf, bufLen, 0);
				if (n == -1)
				{
					n = GetErrorNumber();
					if (n == EINTR) continue;
					if (n != EAGAIN && n != EWOULDBLOCK)
					{
						CloseSocket(s);
					}
					return -1;
				}
				else if (n == 0)
				{
					CloseSocket(s);
					return -1;
				}
				else if (n > 0)
				{
					//CCLOG("\trecv data: %u", n);
					//_readBuf.woffset(_readBuf.woffset() + n);
					//onReceived(_readBuf);
					//return 0;
				}
				//else
				//{
				//	e = NetUtils::getSockErrNo();
				//	CCLOG("SOCKET ERROR: %d", e);

				//	if (e != EAGAIN &&
				//		e != EWOULDBLOCK)
				//	{
				//		close();
				//		return -1;
				//	}
				//}
				break;
			} while (true);

		}
	}
};
