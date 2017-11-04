#pragma execution_character_set("utf-8")
#pragma once

#include <memory>
#ifdef _WIN32
#include <WinSock2.h>
#pragma comment(lib, "WS2_32") 
#undef min
#undef max
#endif

struct Lua_Socket
{

#ifdef _WIN32
	static_assert(SOCKET_ERROR == -1 && INVALID_SOCKET == -1);
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

	inline int GetSockOpt(int level, int optname, void* optval, SockLen_t* optlen)
	{
#ifdef _WIN32
		return ::getsockopt(s, level, optname, (char*)optval, optlen);
#else
		return ::getsockopt(s, level, optname, optval, optlen);
#endif
	}

	inline int CloseSocket()
	{
#ifdef _WIN32
		return ::closesocket(s);
#else
		return ::close(s);
#endif
	}

	inline int CloseSocket(int r)
	{
		CloseSocket();
		s = -1;
		return r;
	}

	inline int SetNonBlock()
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


	inline void OnStateChanged()
	{
		// todo: call registerd lua func
	}

	// 设置 Connect 的地址. 
	// 只能在状态为 Disconnected 时使用
	// 返回 0 成功, 负数各种失败
	inline int SetAddress_(uint32_t ip, uint16_t port)
	{
		if (state != States::Disconnected) return -1;
		addr.sin_addr.s_addr = ip;
		addr.sin_port = htons(port);
		return 0;
	}

	// 开始连接目标址.
	// 只能在状态为 Disconnected 时使用
	// 返回 0 成功, 非 0 各种失败
	inline int Connect_(uint32_t ip, uint16_t port, int timeoutMS)
	{
		if (state != States::Disconnected) return 1;	// wrong state
		if (!addr.sin_port) return 2;					// not set address ?
		if (s != -1) return 3;							// socket is opened

		s = socket(AF_INET, SOCK_STREAM, 0);			// create socket
		if (s == -1) return 4;							// create fail

		int r = SetNonBlock();
		if (r) return CloseSocket(5);					// set non block fail

		do {
			r = ::connect(s, (sockaddr *)&addr, sizeof(addr));
		} while (r == -1 && GetErrorNumber() == EINTR);

		if (r == -1)
		{
			r = GetErrorNumber();
			if (r != EINPROGRESS && r != EWOULDBLOCK) return CloseSocket(6);	// connect fail

			r = WaitForWritable(s, timeoutMS);
			if (r <= 0) return CloseSocket(7);			// wait writable fail
			else
			{
				SockLen_t len = sizeof(r);
				if (GetSockOpt(SOL_SOCKET, SO_ERROR, &r, &len) == -1) return CloseSocket(9);	// get sock opt fail
				if (r) return CloseSocket(10);
			}
		}
		return r;
	}

	inline int Receive(int timeoutMS)
	{
		int n = WaitForReadable(s, timeoutMS);
		if (n > 0)
		{
			do
			{
				//n = ::recv(s, buf, bufLen, 0);
				//if (n == -1)
				//{
				//	n = GetErrorNumber();
				//	if (n == EINTR) continue;
				//	if (n != EAGAIN && n != EWOULDBLOCK)
				//	{
				//		CloseSocket(s);
				//	}
				//	return -1;
				//}
				//else if (n == 0)
				//{
				//	CloseSocket(s);
				//	return -1;
				//}
				//else if (n > 0)
				//{
				//	//CCLOG("\trecv data: %u", n);
				//	//_readBuf.woffset(_readBuf.woffset() + n);
				//	//onReceived(_readBuf);
				//	//return 0;
				//}
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



	enum class States
	{
		Disconnected,			// 初始状态, 可以发起 Connect
		Connecting,
		Connected,
		Disconnecting,
		Closed
	};

	// todo: 允许注册 OnStateChanged OnReceivePackage lua 函数
	// todo: GetState 函数
	// todo: SetAddress 函数
	// todo: Connect 函数( 只允许 State 位于 Disconnected 状态才能执行 )

	Socket_t s = -1;									// current socket
	sockaddr_in addr;                                   // ipv4 address
	States state = States::Disconnected;				// current state

	Lua_BBuffer bbReceive;					// 常规接收容器. 每收够一个包就发起一个回调
	Lua_BBuffer bbReceiveLeft;				// 积攒 OnReceive 处理时剩下的数据
	Lua_BBuffer bbReceivePackage;			// for OnReceivePackage 传参, 引用 bbReceive 或 bbReceiveLeft 的内存

	Lua_Socket(lua_State* L)
		: bbReceive(L)
		, bbReceiveLeft(L)
		, bbReceivePackage(L)
	{
		memset(addr.sin_zero, 0, sizeof(addr.sin_zero));
		addr.sin_family = AF_INET;
		addr.sin_addr.s_addr = 0;
		addr.sin_port = 0;
	}
};
