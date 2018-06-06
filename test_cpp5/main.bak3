#include "xx_uv.h"

// 简单的实现一个记录动态 IP 的功能( 利用路由器定时的用 http 方式请求端口的方式 )
// 创建两个监听器, 一个用于路由器连接, 一个用于看 ip



// 主循环体, 正好拿来存 ip
class MyLoop : public xx::UvLoop
{
public:
	typedef xx::UvLoop BaseType;
	MyLoop(xx::MemPool* mp);
	xx::String lastIp;
};

// 存 ip 的 listener
class Listener_SaveIP : public xx::UvTcpListener
{
public:
	typedef xx::UvTcpListener BaseType;
	Listener_SaveIP(MyLoop* loop);
};

// 存 ip 的 listener 创建的 peer
class Peer_SaveIP : public xx::UvTcpPeer
{
public:
	typedef xx::UvTcpPeer BaseType;
	Peer_SaveIP(Listener_SaveIP* listener) : BaseType(*listener) {}
	inline virtual void ReceiveImpl(char const* bufPtr, int len) override;
};

// 取 ip 的 listener
class Listener_ShowIP : public xx::UvTcpListener
{
public:
	typedef xx::UvTcpListener BaseType;
	Listener_ShowIP(MyLoop* loop);
};

// 取 ip 的 listener 创建的 peer
class Peer_ShowIP : public xx::UvTcpPeer
{
public:
	typedef xx::UvTcpPeer BaseType;
	Peer_ShowIP(Listener_ShowIP* listener) : BaseType(*listener) {}
	inline virtual void ReceiveImpl(char const* bufPtr, int len) override;
};






inline MyLoop::MyLoop(xx::MemPool* mp)
	: BaseType(mp)
	, lastIp(mp)
{
}

inline Listener_SaveIP::Listener_SaveIP(MyLoop* loop)
	: BaseType(*loop)
{
	this->OnCreatePeer = [this]
	{
		auto p = mempool->Create<Peer_SaveIP>(this);
		p->BindTimeouter();
		p->TimeoutReset();
		p->OnTimeout = [p] { p->Release(); };
		return p;
	};
}

inline Listener_ShowIP::Listener_ShowIP(MyLoop* loop)
	: BaseType(*loop)
{
	this->OnCreatePeer = [this]
	{
		auto p = mempool->Create<Peer_ShowIP>(this);
		p->BindTimeouter();
		p->TimeoutReset();
		p->OnTimeout = [p] { p->Release(); };
		return p;
	};
}

inline void Peer_SaveIP::ReceiveImpl(char const* bufPtr, int len)
{
	if (!len)
	{
		Release();
		return;
	}
	if (*bufPtr == 'G')	// 记录 ip( http 请求头为 GET XXXX )
	{
		auto ml = (MyLoop*)&this->listener.loop;
		ml->lastIp = ip();
		Release();
	}
	else
	{
		Release();
	}
}

inline void Peer_ShowIP::ReceiveImpl(char const* bufPtr, int len)
{
	if (!len)
	{
		Release();
		return;
	}
	if (*bufPtr == 'G')	// 返回 ip ( 来自浏览器的请求? )
	{
		auto ml = (MyLoop*)&this->listener.loop;
		SendBytes(ml->lastIp.c_str(), (int)ml->lastIp.dataLen + 1);
	}
	else
	{
		Release();
	}
}



int main(int numArgs, char *args[])
{
	// 从参数读端口并建立监听
	int port_save = 0, port_show = 0;
	if (numArgs != 3 || !(port_save = std::atoi(args[1])) || !(port_show = std::atoi(args[2])))
	{
		std::cout << "args must be a valid tcp ports: save port, show port." << std::endl;
		std::cin.get();
		return -1;
	}
	xx::MemPool mp;
	MyLoop loop(&mp);
	loop.InitTimeouter();
	Listener_SaveIP listener_saveip(&loop);
	Listener_ShowIP listener_showip(&loop);
	try
	{
		listener_saveip.Bind("0.0.0.0", port_save);
		listener_showip.Bind("0.0.0.0", port_show);
	}
	catch (int e)
	{
		std::cout << "listen error. e = " << e << std::endl;
		std::cin.get();
		return -1;
	}

	std::cout << "listen to port:" << port_save << ", " << port_show << std::endl;

	listener_saveip.Listen();
	listener_showip.Listen();
	loop.Run();
	return 0;
}
