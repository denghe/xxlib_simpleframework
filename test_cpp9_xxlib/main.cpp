#include "xx_uv.h"

void f1()
{
	// echo server
	xx::UvLoop loop;
	auto listener = loop.CreateTcpListener();
	auto timeouter = loop.CreateTimeouter();
	listener->OnAccept = [loop = &loop, timeouter](xx::UvTcpPeer* peer)
	{
		std::cout << "peer: ip = " << peer->ip() << " Accepted." << std::endl;
		peer->BindTo(timeouter);
		peer->TimeoutReset();
		peer->OnTimeout = [=]() 
		{
			std::cout << "peer: ip = " << peer->ip() << " Timeouted." << std::endl;
			peer->Release();
		};
		peer->OnDispose = [=]()
		{
			std::cout << "peer: ip = " << peer->ip() << " Disposed." << std::endl;
		};
		peer->OnReceivePackage = [=](xx::BBuffer& bb)
		{
			//std::cout << "listener: recv bb " << bb.dataLen << std::endl;
			//for (int i = 0; i < bb.dataLen; ++i) std::cout << (int)bb[i];
			//std::cout << std::endl;
			xx::Object_p o;
			if (int r = bb.ReadPackage(o))
			{
				std::cout << "listener: bb.ReadPackage(o) fail. r = " << r << std::endl;
			}
			else
			{
				peer->TimeoutReset();
				//std::cout << "listener: recv pkg" << std::endl;
				peer->Send(o);
			}
		};
	};
	listener->Bind("0.0.0.0", 12345);
	listener->Listen();
	std::cout << "listener: loop.Run();" << std::endl;
	loop.Run();
}
//void f2()
//{
//	// test client
//	std::this_thread::sleep_for(std::chrono::milliseconds(100));
//	xx::UvLoop loop;
//	xx::BBuffer_p pkg;
//	uint64_t counter = 0;
//	auto client = loop.CreateClient();
//	client->OnConnect = [&](int status)
//	{
//		std::cout << "client: OnConnect status = " << status << std::endl;
//		if (client->alive())
//		{
//			client->Send(pkg);
//		}
//	};
//	client->OnReceivePackage = [&](xx::BBuffer& bb)
//	{
//		//std::cout << "client: recv pkg" << std::endl;
//		++counter;
//		client->Send(pkg);
//	};
//	client->SetAddress("127.0.0.1", 12345);
//	client->Connect();
//
//	auto timer = loop.CreateTimer(1000, 1000, [&]() 
//	{
//		std::cout << counter << std::endl;
//	});
//
//	std::cout << "client: loop.Run();" << std::endl;
//	loop.Run();
//}
int main()
{
	//std::thread t(f2);
	//t.detach();
	f1();
	return 0;
}
