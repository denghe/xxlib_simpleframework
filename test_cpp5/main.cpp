#include "xx_uv.h"

//void f1()
//{
//	// echo server
//	xx::MemPool mp;
//	xx::UvLoop loop(&mp);
//	auto listener = loop.CreateTcpListener();
//	loop.InitTimeouter();
//	uint64_t counter = 0;
//	listener->OnAccept = [&loop, &counter](xx::UvTcpPeer* peer)
//	{
//		std::cout << "peer: ip = " << peer->ip() << " Accepted." << std::endl;
//		peer->BindTimeouter(loop.timeouter);
//		peer->TimeoutReset();
//		peer->OnTimeout = [peer]()
//		{
//			std::cout << "peer: ip = " << peer->ip() << " Timeouted." << std::endl;
//			peer->Release();
//		};
//		peer->OnDispose = [peer]()
//		{
//			std::cout << "peer: ip = " << peer->ip() << " Disposed." << std::endl;
//		};
//		peer->OnReceivePackage = [peer, &counter](xx::BBuffer& bb)
//		{
//			xx::Object_p o;
//			if (int r = bb.ReadPackage(o))
//			{
//				std::cout << "listener: bb.ReadPackage(o) fail. r = " << r << std::endl;
//			}
//			else
//			{
//				++counter;
//				peer->TimeoutReset();
//				//std::cout << "listener: recv pkg" << std::endl;
//				peer->Send(o);
//			}
//		};
//	};
//	listener->Bind("0.0.0.0", 12345);
//	listener->Listen();
//
//	auto timer = loop.CreateTimer(1000, 1000, [&counter]()
//	{
//		std::cout << counter << std::endl;
//	});
//
//	std::cout << "listener: loop.Run();" << std::endl;
//	loop.Run();
//}
void f2()
{
	// test client
	std::this_thread::sleep_for(std::chrono::milliseconds(100));
	xx::MemPool mp;
	xx::UvLoop loop(&mp);
	xx::BBuffer_p pkg;
	uint64_t counter = 0;
	auto client = loop.CreateTcpClient();
	client->OnConnect = [&](int status)
	{
		std::cout << "client: OnConnect status = " << status << std::endl;
		if (client->alive())
		{
			client->Send(pkg);
		}
	};
	client->OnReceivePackage = [&](xx::BBuffer& bb)
	{
		//std::cout << "client: recv pkg" << std::endl;
		++counter;
		client->Send(pkg);
	};
	client->SetAddress("192.168.1.250", 12345);
	client->Connect();

	auto timer = loop.CreateTimer(1000, 1000, [&]()
	{
		std::cout << counter << std::endl;
	});

	std::cout << "client: loop.Run();" << std::endl;
	loop.Run();
}
int main()
{
	//std::thread t(f2);
	//t.detach();
	f2();
	return 0;
}
