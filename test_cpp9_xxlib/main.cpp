#include "xx_uv.h"

void f1()
{
	// echo server
	xx::UvLoop loop;
	loop.InitTimeouter();
	loop.InitKcpFlushInterval(10);
	auto listener = loop.CreateUdpListener();
	listener->Bind("0.0.0.0", 12345);
	listener->Listen();
	uint64_t counter = 0;
	listener->OnAccept = [&loop, &counter](xx::UvUdpPeer* peer)
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

	auto timer = loop.CreateTimer(1000, 1000, [&counter]()
	{
		std::cout << counter << std::endl;
	});

	std::cout << "listener: loop.Run();" << std::endl;
	loop.Run();
}
void f2()
{
	// test client
	std::this_thread::sleep_for(std::chrono::milliseconds(100));
	xx::UvLoop loop;
	loop.InitKcpFlushInterval(10);
	xx::BBuffer_p pkg;
	uint64_t counter = 0;
	auto client = loop.CreateUdpClient();
	client->OnReceivePackage = [&](xx::BBuffer& bb)
	{
		//std::cout << "client: recv pkg" << std::endl;
		++counter;
		client->Send(pkg);
	};
	client->SetAddress("127.0.0.1", 12345);
	client->Connect(xx::Guid());
	client->Send(pkg);

	auto timer = loop.CreateTimer(1000, 1000, [&]() 
	{
		std::cout << counter << std::endl;
	});

	std::cout << "client: loop.Run();" << std::endl;
	loop.Run();
}
int main()
{
	std::thread t(f2);
	t.detach();
	f1();
	return 0;
}
