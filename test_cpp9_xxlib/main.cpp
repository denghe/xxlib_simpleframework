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
	xx::UvTcpListener routerListener;				// 接受路由的连接
	xx::String cmd;									// 用于解析收到的指令
	Service1(xx::UvLoop& loop);
	~Service1();
};


// 服务器2
class Service2 : public xx::Object
{
public:
	xx::UvLoop& loop;
	xx::UvTcpListener routerListener;				// 接受路由的连接
	xx::String cmd;									// 用于解析收到的指令
	Service2(xx::UvLoop& loop);
	~Service2();
};


// 路由器
class ServiceClient : public xx::UvTcpClient
{
public:
	ServiceClient(xx::UvLoop& loop);
	xx::String addr;				// 连上服务器后从服务器收到的地址信息
};
using ServiceClient_p = xx::Ptr<ServiceClient>;
class Router : public xx::Object
{
public:
	xx::UvLoop& loop;

	// 接受客户端的连接
	xx::UvTcpListener clientListener;

	// 拿来做断线自动重连
	xx::UvTimer timer;

	// 连所有内部服务器. 在连上或收到其 addr 之前为匿名状态, 存在这里
	xx::List<ServiceClient_p> serviceClientGuests;

	// 所有内部服务器连接上下文, 地址做 key
	xx::Dict<xx::String*, ServiceClient_p> serviceClientMappings;

	// 用于发送时拼接指令
	xx::String cmd;

	void Update();
	Router(xx::UvLoop& loop);
	~Router();
};


// 客户端
class Client : public xx::Object
{
public:
	xx::UvLoop& loop;
	xx::UvTcpClient routerClient;					// 连路由
	xx::UvTimer timer;								// 帧逻辑驱动模拟
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
	, cmd(loop.mempool)
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
			if (p->senderAddress.dataLen)
			{
				// 这时候可以 p + 该地址 作为连接标识 记录到相应的玩家上下文

				// 先简单的直发回去
				p->SendRoutingResponse(p->senderAddress, serial, xx::String(mempool, "Service2"));
			}
			// 非转发包
			else
			{
				// 反序列化出指令
				if (auto r = bb.ReadRoot(cmd))
				{
					// 解析失败
					p->Release();
					return;
				}
				// 先随便定义一个路由找服务要 addr 的指令, 简单判断
				if (cmd.Equals("get routing address"))
				{
					// 将自己的地址发回去
					p->SendResponse(serial, p->routingAddress);
					return;
				}
				// 未知指令
				p->Release();
				return;
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
	, cmd(loop.mempool)
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
			if (p->senderAddress.dataLen)
			{
				// 这时候可以 p + 该地址 作为连接标识 记录到相应的玩家上下文

				// 先简单的直发回去
				p->SendRoutingResponse(p->senderAddress, serial, xx::String(mempool, "ok"));
			}
			// 非转发包
			else
			{
				// 反序列化出指令
				if (auto r = bb.ReadRoot(cmd))
				{
					// 解析失败
					p->Release();
					return;
				}
				// 先随便定义一个路由找服务要 addr 的指令, 简单判断
				if (cmd.Equals("get routing address"))
				{
					// 将自己的地址发回去
					p->SendResponse(serial, p->routingAddress);
					return;
				}
				// 未知指令
				p->Release();
				return;
			}
		};
	};
}
inline Service2::~Service2()
{
}




inline ServiceClient::ServiceClient(xx::UvLoop& loop)
	: xx::UvTcpClient(loop)
	, addr(loop.mempool)
{
}

inline Router::Router(xx::UvLoop& loop)
	: xx::Object(loop.mempool)
	, loop(loop)
	, clientListener(loop)
	, timer(loop, 0, 1000, [this] { Update(); })
	, serviceClientGuests(loop.mempool)
	, serviceClientMappings(loop.mempool)
	, cmd(loop.mempool)
{

	auto c = mempool->CreatePtr<ServiceClient>(loop);
	c->SetAddress("127.0.0.1", 12345);
	c->Connect();
	serviceClientGuests.Emplace(std::move(c));

	c = mempool->CreatePtr<ServiceClient>(loop);
	c->SetAddress("127.0.0.1", 12346);
	c->Connect();
	serviceClientGuests.Emplace(std::move(c));

	for (auto& c : serviceClientGuests)
	{
		// 引用传递 this, c 到 lambda 备用
		c->OnConnect = [&](int status)
		{
			// 没连上
			if (!status) return;	

			// 连上后发起地址请求
			cmd.Assign("get routing address");

			// 取 c 的 Ref 传入 lambda, 以便探测当回调发生时 c 已 release 的情况
			c->SendRequest(cmd, [this, cRef = c.MakeRef()](uint32_t serial, xx::BBuffer* bb)
			{
				// 超时
				if (!bb) return;

				// c 已释放
				auto c = cRef.Lock();
				if (!c) return;

				// c 已断开
				if (!c->alive) return;

				// 试着反序列化填充地址. 失败则断开并退出( 理论上讲不应该出现 )
				if (auto r = bb->ReadRoot(c->addr))
				{
					c->Disconnect();
					return;
				}

				// todo: 将 c 从 list 移至 map
			});
		};

		c->OnDisconnect = [this] 
		{
			// todo: 如果 c 位于 map ( addr 有值即位于 map ), 要从 map 移除并放入 guests 并清 addr
		};
	}

	clientListener.Bind("0.0.0.0", 12344);
	clientListener.Listen();
	clientListener.OnAccept = [this](xx::UvTcpPeer *p)
	{
		p->OnReceiveRouting = [this, p](xx::BBuffer& bb, size_t pkgOffset, size_t pkgLen, size_t addrOffset, size_t addrLen)
		{
			// todo: 读出 addr, 在 map 中定位到 c, 修改 bb 中 addr 部分后用 c 发送
		};
	};
}

inline void Router::Update()
{
	// 断线自动重连
	for (auto& c : serviceClientGuests)
	{
		if (!c->alive) c->Connect();
	}
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
		if (routerClient.senderAddress.dataLen)
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

