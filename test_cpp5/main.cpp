#include "xx_uv.h"
void f1()			// rpc echo server
{
	xx::MemPool mp;
	xx::UvLoop loop(&mp);
	loop.InitRpcManager();
	auto listener = loop.CreateTcpListener();
	listener->Bind("0.0.0.0", 12345);
	listener->Listen();
	uint64_t counter = 0;
	listener->OnAccept = [&loop, &counter](xx::UvTcpPeer* peer)
	{
		peer->OnReceiveRequest = [peer, &counter](uint32_t serial, xx::BBuffer& bb)
		{
			xx::Object_p o;
			if (int r = bb.ReadRoot(o))			// 解不出来
			{
				peer->Release();
				return;
			}
			auto& pkg = *(xx::String_p*)&o;
			if (pkg->Equals("asdf"))	++counter;
			peer->SendResponse(serial, o);
		};
	};
	auto timer = loop.CreateTimer(1000, 1000, [&loop, &counter]()
	{
		std::cout << "server counter = " << counter << std::endl;
	});
	loop.Run();
}
void f2()			// test client
{
	std::this_thread::sleep_for(std::chrono::milliseconds(100));
	xx::MemPool mp;
	xx::UvLoop loop(&mp);
	loop.InitRpcManager(10);
	uint64_t counter = 0;
	auto client = loop.CreateTcpClient();
	client->SetAddress("127.0.0.1", 12345);
	client->Connect();
	auto timer = loop.CreateTimer(1000, 1000, [&]()
	{
		std::cout << "client counter = " << counter << std::endl;
	});
	auto timer2 = loop.CreateTimer(10, 1, [&]()
	{
		for (int i = 0; i < 600; ++i)
		{
			xx::String_p pkg;
			mp.MPCreateTo(pkg);
			pkg->Assign("asdf");
			client->SendRequest(pkg, [&](uint32_t serial, xx::BBuffer* bb)
			{
				if (!bb) return;	// 超时
				xx::Object_p o;
				if (int r = bb->ReadRoot(o)) return;	// 解不出来
				auto& pkg = *(xx::String_p*)&o;
				if (pkg->Equals("asdf")) ++counter;
			});
		}
	});
	loop.Run();
}
int main()
{
	xx::MemPool::RegisterInternal();
	std::thread t(f2);
	t.detach();
	f1();
	return 0;
}
