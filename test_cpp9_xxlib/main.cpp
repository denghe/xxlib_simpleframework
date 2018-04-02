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

	// 用于解析收到的指令
	xx::String cmd;

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

	// 用于解析收到的指令
	xx::String cmd;

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


	// 用于发送时拼接指令或公用于其他会用到 string 的地儿
	xx::String cmd;

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
	, cmd(loop.mempool)
{

	// 先根据 服务配置创建所有相应的连接实例
	auto c = mempool->Create<ServiceClient>(loop);
	c->SetAddress("127.0.0.1", 12345);	// Service1
	c->Connect();
	serviceClientGuests.Add(c);

	c = mempool->Create<ServiceClient>(loop);
	c->SetAddress("127.0.0.1", 12346);	// Service2
	c->Connect();
	serviceClientGuests.Add(c);

	// 批量绑连接实例的事件
	for (auto& c : serviceClientGuests)
	{
		// this 生命周期可确保死在 c 之后
		// 取 c 的 Ref 传入 lambda, 以便探测当回调发生时 c 已 release 的情况
		c->OnConnect = [this, c](int status)
		{
			// 没连上( 判断 status 非0也行 )
			if (!c->alive()) return;

			// 连上后发起地址请求
			cmd.Assign("get routing address");

			c->SendRequest(cmd, [this, c](uint32_t serial, xx::BBuffer* bb)
			{
				// 超时
				if (!bb) return;

				// c 已断开
				if (!c->alive()) return;

				// 试着反序列化填充地址. 失败则断开并退出( 理论上讲不应该出现 )
				if (auto r = bb->ReadRoot(c->addr))
				{
					c->Disconnect();
					return;
				}

				// 将 c 从 list 移至 map
				serviceClientGuests.Remove(c);
				auto r = serviceClientMappings.Add(&c->addr, c);
				assert(r.success);
			});
		};

		c->OnDisconnect = [this, c]
		{
			// 如果 c 位于 map ( addr 有值即位于 map ), 要从 map 移除并放入 guests 并清 addr
			if (c->addr.dataLen)
			{
				serviceClientMappings.Remove(&c->addr);
				serviceClientGuests.Add(c);
				c->addr.Clear();
			}
		};
	}

	clientListener.Bind("0.0.0.0", 12344);
	clientListener.Listen();

	// Accept 的时候创建 key 并映射之, 创建时要将 key 存起来( 理论上讲直接用 指针+版本号 当 key 是 ok 的. 不过跨语言不方便 )
	clientListener.OnCreatePeer = [this]()
	{
		// 创建
		auto p = mempool->Create<ClientPeer>(clientListener);

		// 填充 addr( 自增id )
		p->addr = clientPeerId++;

		// 对于客户端首次通信来说, 可能并不知道应该找哪个服务通信, 应该约定一种表示 "未知" "默认" 的地址来发起内容, 以便路由转发给相应的处理服务
		// 另一种解决方案为用不带地址的版本收发, 但这会导致路由服务处理函数多写一份, 先不考虑. 姑且先用 addrLen 为 1, 内容为 \0 来约定.
		// 这样的地址, 将导致与 Service1 服务通信.
		p->OnReceiveRouting = [this, p](xx::BBuffer& bb, size_t pkgOffset, size_t pkgLen, size_t addrOffset, size_t addrLen)
		{
			// 读出 addr, 在 map 中定位到 c, 构造数据后发出( 如果无法定位到 c, 或 c 连接异常, 则将 p 踢掉 )

			// 先判断是否为默认地址. 如果是就填充映射地址
			if (addrLen == 1 && !bb[addrOffset])
			{
				cmd.Assign("Service1");
			}
			// 否则将地址复制到 String 结构以方便后续查字典
			else
			{
				cmd.Assign(bb.buf + addrOffset, addrLen);
			}

			// 得到查找结果下标
			auto idx = serviceClientMappings.Find(&cmd);

			// 如果有定位到, 则继续操作
			if (idx != -1)
			{
				// 取出 c( 位于这个容器的 c 一定是 alive 的 )
				auto c = serviceClientMappings.ValueAt(idx);

				// 开始填充 c->bbSend, 构造转发包. 除了数据区, 这里需要完全解析包结构. 
				// 需要保留原有 Request Response 特性, 算出新的 addrLen. 
				// 如果因为附加地址导致数据超过 64k, 还要用大包标记来发

				// 下面先直接实现拼数据逻辑, 测试验证通过之后, 移到 c 的成员函数中去. 比如叫 SendRouting
				// 其接口大概长相应该是传入 OnReceiveRouting 的事件参数

				// 引用到 bbSend
				auto& sBB = c->bbSend;

				// 清空以备用
				sBB.Clear();

				auto typeId = (uint8_t)bb[pkgOffset];


				// 将包头的 type, datalen 部分 复制到目标
				sBB.AddRange(bb.buf + pkgOffset, addrOffset - pkgOffset);

				// 记录当前数据长, 准备在写入发送者 addr 后算 addrLen
				auto dataLen_bak = sBB.dataLen;
				sBB.Write(p->addr);

				// 算出 addr 区长度
				auto sAddrLen = sBB.dataLen - dataLen_bak;

				// 修正头部的掩码
				sBB[0] = sBB[0] & 0xF | ((sAddrLen - 1) << 4);

				// todo

				// 发送 bbSend
				//c->Send();
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
		if (!c->alive()) c->Connect();
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
		// 非转发包( 默认向某一个接受非转发包的服务转发, 例如登录服务 )
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

