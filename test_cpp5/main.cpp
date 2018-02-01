#include "xx_uv.h"

/*
	todo: 网关服务


	协议设计: 网关应该具备自己的通信协议结构以及转发上下文
	网关根据所读到的配置文件, 去连接其他服务( 也就是说每个服务都要开个 Listener 供多个网关连接 )
	在配置文件中, 需要为每个服务统一分配 id 号( 或理解为地址 )

	发向网关的包结构如下:
	class C2P
	{
		uint serviceId;
		BBuffer data;
	}

	网关根据 tarAddr, 从映射表中定位到相应 tcp 长连接, 发送下列结构的包:
	class P2S
	{
		Guid sender;
		BBuffer data;
	}
	如果 tarAddr 无效, 则断开 client? ( 暂行方案 )
	
	服务器首次收到后, 应该根据 sender 创建虚拟连接, 需要保存下列结构:
	class VC
	{
		Peer proxy;
		Guid sender;
	}
	如果发现该结构已存在( 应该存在一个 Dict<Guid, VC> 映射 ), 则定位之
	应该从设计上避免出现一个 client 连接多个 proxy 的情况. ( 二期工程, 考虑 proxy 总控用于处理 guid 单点 )

	当数据需要延迟回发时, 通过 Guid 到 VC 的映射来定位连接信息, 进而将数据发向相应的 proxy.
	发向 代理 的数据如下:

	class S2P
	{
		Guid sender;
		BBuffer data;
	}

	代理 根据 Guid 来定位具体的 client udp 连接, 进而执行回发 data 的操作. 
	发向 client 的数据如下:
	class P2C
	{
		uint serviceId;		// 从收包连接上下文中取
		BBuffer data;
	}

	客户端在收到数据之后, 产生的收包回调中, 也需要将 serviceId 体现出来, 或是进一步的定位到相应的服务虚拟连接, 做进一步的转发

	如果不纠结收发数据量, 设 serviceId 为 Guid, 则能极大减少 代理 服务的 memcpy 行为, 对于收到的包, 直接将 serviceId 的内容改为 sender 就能转发给 service.
	并且, 所有收发相关结构, 都是相同数据结构. 经简化后, 初始可以是固定的 16 字节, 后面跟包内容, 这样似乎简单明快.
	也就是说, 不再需要上面的类结构, BBuffer 在构造数据时, 前 16 字节就用来放地址信息.
*/


void f1()
{
	// proxy server
	xx::MemPool mp;
	xx::UvLoop loop(&mp);
	auto listener = loop.CreateTcpListener();
	loop.InitTimeouter();
	uint64_t counter = 0;
	listener->OnAccept = [&loop, &counter](xx::UvTcpPeer* peer)
	{
		std::cout << "peer: ip = " << peer->ip() << " Accepted." << std::endl;
		peer->BindTimeouter(loop.timeouter);
		peer->TimeoutReset();
		peer->OnTimeout = [peer]()
		{
			std::cout << "peer: ip = " << peer->ip() << " Timeouted." << std::endl;
			peer->Release();
		};
		peer->OnDispose = [peer]()
		{
			std::cout << "peer: ip = " << peer->ip() << " Disposed." << std::endl;
		};
		peer->OnReceivePackage = [peer, &counter](xx::BBuffer& bb)
		{
			xx::Object_p o;
			if (int r = bb.ReadPackage(o))
			{
				std::cout << "listener: bb.ReadPackage(o) fail. r = " << r << std::endl;
			}
			else
			{
				++counter;
				peer->TimeoutReset();
				//std::cout << "listener: recv pkg" << std::endl;
				peer->Send(o);
			}
		};
	};
	listener->Bind("0.0.0.0", 12345);
	listener->Listen();

	auto timer = loop.CreateTimer(1000, 1000, [&counter]()
	{
		std::cout << counter << std::endl;
	});

	std::cout << "listener: loop.Run();" << std::endl;
	loop.Run();
}
int main()
{
	f1();
	return 0;
}
