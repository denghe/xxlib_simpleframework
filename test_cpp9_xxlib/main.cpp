#include "xx_uv.h"

// 模拟两个服务, 一个路由, 一个客户端
// 客户端通过路由发数据到服务1, 服务1 返给它 服务2 的地址, 
// 客户端再通过路由向 服务2 发数据, 收到服务2 的回应后, 流程结束.

/*******************************************************************/
// decls
/*******************************************************************/


/*************************/
// 服务器1
class Service1 : public xx::Object
{
public:
	xx::UvLoop& loop;

	// 接受路由的连接
	xx::UvTcpListener routerListener;

	Service1(xx::UvLoop& loop);
	~Service1();
};


/*************************/
// 服务器2

class Service2 : public xx::Object
{
public:
	xx::UvLoop& loop;

	// 接受路由的连接
	xx::UvTcpListener routerListener;

	Service2(xx::UvLoop& loop);
	~Service2();
};


/*************************/
// 路由器


// 服务连接( 创建一次, 反复使用 )
class ServiceClient : public xx::UvTcpClient
{
public:
	ServiceClient(xx::UvLoop& loop);
	xx::String addr;				// 连上服务器后从服务器收到的地址信息
};

// 客户对端( 可能因断线而 release )
class ClientPeer : public xx::UvTcpPeer
{
public:
	ClientPeer(xx::UvTcpListener& listener);
	uint64_t addr;					// 接受客户端连接后为其生成一个唯一连接id
};
using ClientPeer_r = xx::Ref<ClientPeer>;

class Router : public xx::Object
{
public:
	xx::UvLoop& loop;

	// 客户对端创建后填充用自增id( 先用再++ ), 用于写 addr 做 key 以定位客户对端
	uint64_t clientPeerId = 0;

	// 接受客户端的连接
	xx::UvTcpListener clientListener;

	// 所有玩家对端连接上下文, 自增id 做 key
	xx::Dict<uint64_t, ClientPeer*> clientPeerMappings;



	// 拿来做 ServiceClients 断线自动重连
	xx::UvTimer timer;

	// 连所有内部服务. 在连上或收到其 addr 之前为匿名状态, 存在这里
	xx::List<ServiceClient*> serviceClientGuests;

	// 所有内部服务连接上下文, 地址做 key
	xx::Dict<xx::String*, ServiceClient*> serviceClientMappings;


	// 公用 String
	xx::String str;

	void Update();
	Router(xx::UvLoop& loop);
	~Router();
};


/*************************/
// 客户端
class Client : public xx::Object
{
public:
	xx::UvLoop& loop;

	// 连路由
	xx::UvTcpClient routerClient;

	// 帧逻辑驱动模拟
	xx::UvTimer timer;

	// 帧逻辑
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
			//std::cout << "p->OnReceiveRequest = [this, p](uint32_t serial, xx::BBuffer& bb)" << std::endl;

			// 路由过来的包, 带返回地址
			if (p->senderAddress.dataLen)
			{
				// 这时候可以 p + 该地址 作为连接标识 记录到相应的玩家上下文

				// 先简单的直发回去
				p->SendRoutingResponse(p->senderAddress.buf, p->senderAddress.dataLen, serial, xx::String(mempool, "Service2"));
			}
			// 非转发包
			else
			{
				// 未知指令
				p->Release();
			}
		};

		// 在连接成功后先行下发一个只含地址的特殊包
		p->SendRoutingAddress(p->routingAddress.buf, p->routingAddress.dataLen);
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
			//std::cout << "p->OnReceiveRequest = [this, p](uint32_t serial, xx::BBuffer& bb)" << std::endl;

			// 路由过来的包, 带返回地址
			if (p->senderAddress.dataLen)
			{
				// 这时候可以 p + 该地址 作为连接标识 记录到相应的玩家上下文

				// 先简单的直发回去
				p->SendRoutingResponse(p->senderAddress.buf, p->senderAddress.dataLen, serial, xx::String(mempool, "ok"));
			}
			// 非转发包
			else
			{
				// 未知指令
				p->Release();
			}
		};

		// 在连接成功后先行下发一个只含地址的特殊包
		p->SendRoutingAddress(p->routingAddress.buf, p->routingAddress.dataLen);
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

inline ClientPeer::ClientPeer(xx::UvTcpListener& L)
	: xx::UvTcpPeer(L)
{
}


inline Router::Router(xx::UvLoop& loop)
	: xx::Object(loop.mempool)
	, loop(loop)
	, clientListener(loop)
	, clientPeerMappings(loop.mempool)
	, timer(loop, 0, 1000, [this] { Update(); })
	, serviceClientGuests(loop.mempool)
	, serviceClientMappings(loop.mempool)
	, str(loop.mempool)
{

	// 先根据 服务配置创建所有相应的连接实例
	auto sc = mempool->Create<ServiceClient>(loop);
	sc->SetAddress("127.0.0.1", 12345);	// Service1
	sc->Connect();
	serviceClientGuests.Add(sc);

	sc = mempool->Create<ServiceClient>(loop);
	sc->SetAddress("127.0.0.1", 12346);	// Service2
	sc->Connect();
	serviceClientGuests.Add(sc);

	// 批量绑连接实例的事件
	for (auto& sc : serviceClientGuests)
	{
		sc->OnDisconnect = [this, sc]
		{
			// 如果 sc 位于 map ( addr 有值即位于 map ), 要从 map 移除并放入 guests 并清 addr
			if (sc->addr.dataLen)
			{
				serviceClientMappings.Remove(&sc->addr);
				serviceClientGuests.Add(sc);
				sc->addr.Clear();
			}
		};

		// 当 sc 连接成功时, Service 应该发地址包过来. 
		sc->OnReceivePackage = [this, sc](xx::BBuffer& bb)
		{
			// 填充地址
			sc->addr.Assign(bb.buf + bb.offset, sc->GetRoutingAddressLength(bb));

			// 将 sc 从 list 移至 map
			serviceClientGuests.Remove(sc);
			auto r = serviceClientMappings.Add(&sc->addr, sc);
			assert(r.success);
		};

		sc->OnReceiveRouting = [this, sc](xx::BBuffer& bb, size_t pkgLen, size_t addrOffset, size_t addrLen)
		{
			//std::cout << "sc->OnReceiveRouting = [this, sc](xx::BBuffer& bb, size_t pkgLen, size_t addrOffset, size_t addrLen)" << std::endl;

			// 还原出 客户端连接 查找用 key
			uint64_t addr;
			assert(addrLen == sizeof(addr));
			memcpy(&addr, bb.buf + addrOffset, sizeof(addr));

			// 得到查找结果下标
			auto idx = clientPeerMappings.Find(addr);

			// 如果有定位到, 则继续操作
			if (idx != -1)
			{
				// 取出 cp( 位于这个容器的 cp 一定是 alive 的 )
				auto cp = clientPeerMappings.ValueAt(idx);

				// 篡改地址部分为 实际发信服务 addr 并转发
				cp->SendRoutingByRouter(bb, pkgLen, addrOffset, addrLen, sc->addr.buf, sc->addr.bufLen);
			}
			// 否则表示 客户端早已断开 不必理会
		};
	}

	clientListener.Bind("0.0.0.0", 12344);
	clientListener.Listen();

	// Accept 的时候创建 key 并映射之, 创建时要将 key 存起来( 理论上讲直接用 指针+版本号 当 key 是 ok 的. 不过跨语言不方便 )
	clientListener.OnCreatePeer = [this]()
	{
		//std::cout << "clientListener.OnCreatePeer = [this]()" << std::endl;
		// 创建
		auto p = mempool->Create<ClientPeer>(clientListener);

		// 填充 addr( 自增id )
		p->addr = clientPeerId++;

		// 对于客户端首次通信来说, 可能并不知道应该找哪个服务通信, 应该约定一种表示 "未知" "默认" 的地址来发起内容, 以便路由转发给相应的处理服务
		// 另一种解决方案为用不带地址的版本收发, 但这会导致路由服务处理函数多写一份, 先不考虑. 姑且先用 addrLen 为 1, 内容为 \0 来约定.
		// 这样的地址, 将导致与 Service1 服务通信.
		p->OnReceiveRouting = [this, p](xx::BBuffer& bb, size_t pkgLen, size_t addrOffset, size_t addrLen)
		{
			//std::cout << "p->OnReceiveRouting = [this, p](xx::BBuffer& bb, size_t pkgLen, size_t addrOffset, size_t addrLen)" << std::endl;
			// 读出 addr, 在 map 中定位到 c, 构造数据后发出( 如果无法定位到 c, 或 c 连接异常, 则将 p 踢掉 )

			// 先判断是否为默认地址. 如果是就填充映射地址
			if (addrLen == 1 && !bb[addrOffset])
			{
				str.Assign("Service1");
			}
			// 否则将地址复制到 String 结构以方便后续查字典
			else
			{
				str.Assign(bb.buf + addrOffset, addrLen);
			}

			// 得到查找结果下标
			auto idx = serviceClientMappings.Find(&str);

			// 如果有定位到, 则继续操作
			if (idx != -1)
			{
				// 取出 c( 位于这个容器的 c 一定是 alive 的 )
				auto c = serviceClientMappings.ValueAt(idx);
				assert(c->alive());

				// 篡改地址部分为客户对端的 key 并转发
				c->SendRoutingByRouter(bb, pkgLen, addrOffset, addrLen, (char*)&p->addr, sizeof(p->addr));
			}
			// 否则将 p 回收掉
			else
			{
				p->Release();
			}
		};

		// 在 p 的销毁事件中, 将自己从 map 中移除
		p->OnDispose = [this, p]()
		{
			clientPeerMappings.Remove(p->addr);
		};

		// 将 p 放入 map
		auto r = clientPeerMappings.Add(p->addr, p);
		assert(r.success);

		return p;
	};
}

inline void Router::Update()
{
	// 断线自动重连
	for (auto& c : serviceClientGuests)
	{
		if (c->state == xx::UvTcpStates::Disconnected)
		{
			c->Connect();
		}
	}
}
inline Router::~Router()
{
}



inline Client::Client(xx::UvLoop& loop)
	: xx::Object(loop.mempool)
	, loop(loop)
	, routerClient(loop)
	, timer(loop, 1000, 1000, [this] { Update(); })
{
	routerClient.SetAddress("127.0.0.1", 12344);
	routerClient.routingAddress = "client";	// 这个只是令包事件分发器检测到这是非路由端
}
inline void Client::Update()
{
	if (routerClient.state == xx::UvTcpStates::Disconnected)
	{
		routerClient.Connect();
		//std::cout << "routerClient.Connect();" << std::endl;
	}
	else if (routerClient.alive())
	{
		//std::cout << "routerClient.SendRoutingRequest" << std::endl;
		routerClient.SendRoutingRequest("Service1", 8, xx::String(mempool, "hi"), [this](uint32_t serial, xx::BBuffer* bb)
		{
			// RPC超时
			if (!bb) return;

			xx::String str(mempool);
			bb->ToString(str);
			//std::cout << "recv " << routerClient.senderAddress << "'s response: " << str << std::endl;

			// 试解包( 应该是收到地址 Service2 )
			if (int r = bb->ReadPackage(str))
			{
				// 解包失败, 忽略
				return;
			}

			// 根据收到的 服务地址, 继续发包( 前提是没断开 )
			if (routerClient.alive())
			{
				routerClient.SendRoutingRequest(str.buf, str.dataLen, xx::String(mempool, "hi!!!"), [this](uint32_t serial, xx::BBuffer* bb)
				{
					// RPC超时
					if (!bb) return;

					xx::String str2(mempool);
					bb->ToString(str2);
					std::cout << "recv " << routerClient.senderAddress << "'s response: " << str2 << std::endl;
				});
			}
		});
	}
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
	loop.InitRpcManager();
	Service1 s1(loop);
	Service2 s2(loop);
	Router r(loop);
	Client c(loop);
	loop.Run();
	return 0;
}

