#include "xx_uv.h"

// 模拟两个服务, 一个路由, 一个客户端
// 客户端通过路由发数据到服务1, 服务1 返给它 服务2 的地址, 
// 客户端再通过路由向 服务2 发数据, 收到服务2 的回应后, 流程结束.

/*******************************************************************/
// decls
/*******************************************************************/

// 服务器1
class Service1 : public xx::Object
{
public:
	xx::UvLoop& loop;
	xx::UvTcpListener routerListener;	// 接受路由的连接
	Service1(xx::UvLoop& loop);
	~Service1();
};


// 服务器2
class Service2 : public xx::Object
{
public:
	xx::UvLoop& loop;
	xx::UvTcpListener routerListener;	// 接受路由的连接
	Service2(xx::UvLoop& loop);
	~Service2();
};


// 路由器
class Router : public xx::Object
{
public:
	xx::UvLoop& loop;
	xx::UvTcpListener clientListener;				// 接受客户端的连接
	xx::List<xx::UvTcpClient_p> serviceClients;		// 连所有内部服务器
	Router(xx::UvLoop& loop);
	~Router();
};


// 客户端
class Client : public xx::Object
{
public:
	xx::UvLoop& loop;
	xx::UvTcpClient routerClient;		// 连路由
	xx::UvTimer timer;					// 帧逻辑驱动模拟
	void Update();
	Client(xx::UvLoop& loop);
	~Client();
};





/*******************************************************************/
// impls
/*******************************************************************/


inline Service1::Service1(xx::UvLoop& loop)
	: xx::Object(loop.mempool)
	, loop(loop)
	, routerListener(loop)
{
	routerListener.Bind("0.0.0.0", 12345);
	routerListener.Listen();
	routerListener.OnAccept = [this](xx::UvTcpPeer *p)
	{
		// 通过设置路由地址的方式告知 peer 数据接收处理下发进阶事件
		p->routingAddress = "Service1";

		p->OnReceiveRequest = [this, p](uint32_t serial, xx::BBuffer& bb)
		{
			// 路由过来的包, 带返回地址
			if (p->recvingAddress.dataLen)
			{
				// 这时候可以 p + 该地址 作为连接标识 记录到相应的玩家上下文

				// 先简单的直发回去
				p->SendRoutingResponse(p->recvingAddress, serial, xx::String(mempool, "Service2"));
			}
			// 非转发包
			else
			{
				// 先随便定义一个路由找服务要 addr 的指令, 简单判断
				if (bb.dataLen == 1 && bb[0] == 'a')
				{
					// 将自己的地址发回去
					p->SendResponse(serial, p->routingAddress);
				}
				// ...
			}
		};
	};
}
inline Service1::~Service1()
{
}



inline Service2::Service2(xx::UvLoop& loop)
	: xx::Object(loop.mempool)
	, loop(loop)
	, routerListener(loop)
{
	routerListener.Bind("0.0.0.0", 12346);
	routerListener.Listen();
	routerListener.OnAccept = [this](xx::UvTcpPeer *p)
	{
		// 通过设置路由地址的方式告知 peer 数据接收处理下发进阶事件
		p->routingAddress = "Service2";

		p->OnReceiveRequest = [this, p](uint32_t serial, xx::BBuffer& bb)
		{
			// 路由过来的包, 带返回地址
			if (p->recvingAddress.dataLen)
			{
				// 这时候可以 p + 该地址 作为连接标识 记录到相应的玩家上下文

				// 先简单的直发回去
				p->SendRoutingResponse(p->recvingAddress, serial, xx::String(mempool, "ok"));
			}
			// 非转发包
			else
			{
				// 先随便定义一个路由找服务要 addr 的指令, 简单判断
				if (bb.dataLen == 1 && bb[0] == 'a')
				{
					// 将自己的地址发回去
					p->SendResponse(serial, p->routingAddress);
				}
				// ...
			}
		};
	};
}
inline Service2::~Service2()
{
}



inline Router::Router(xx::UvLoop& loop)
	: xx::Object(loop.mempool)
	, loop(loop)
	, clientListener(loop)
	, serviceClients(loop.mempool)
{
	//serviceClients........

	clientListener.Bind("0.0.0.0", 12344);
	clientListener.Listen();
	clientListener.OnAccept = [this](xx::UvTcpPeer *p)
	{
		//p->OnReceiveRouting = [this, p](xx::BBuffer& bb, size_t pkgOffset, size_t pkgLen, size_t addrOffset, size_t addrLen)
		//{
		//};
	};

}
inline Router::~Router()
{
}



inline Client::Client(xx::UvLoop& loop)
	: xx::Object(loop.mempool)
	, loop(loop)
	, routerClient(loop)
	, timer(loop, 0, 33, [this] { Update(); })
{
	routerClient.SetAddress("127.0.0.1", 12344);
	routerClient.Connect();
	routerClient.routingAddress = "client";
	routerClient.OnReceiveRequest = [this](uint32_t serial, xx::BBuffer& bb)
	{
		// 路由过来的包, 带返回地址
		if (routerClient.recvingAddress.dataLen)
		{
		}
		// 非转发包
		else
		{
		}
	};
}
inline void Client::Update()
{
	// todo
}
inline Client::~Client()
{
}





/*******************************************************************/
// entry
/*******************************************************************/

int main()
{
	xx::MemPool::RegisterInternal();
	xx::MemPool mp;
	xx::UvLoop loop(&mp);
	Service1 s1(loop);
	Service2 s2(loop);
	Router r(loop);
	Client c(loop);
	loop.Run();
	return 0;
}

