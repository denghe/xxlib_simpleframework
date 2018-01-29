#include "xx_uv.h"

bool running = true;

void f1()
{
	// echo server
	xx::MemPool mp;
	xx::UvLoop loop(&mp);
	loop.InitTimeouter();
	loop.InitKcpFlushInterval(10);
	auto listener = loop.CreateUdpListener();
	listener->Bind("0.0.0.0", 12345);
	listener->Listen();
	uint64_t counter = 0;
	listener->OnCreatePeer = [listener](auto& g)
	{
		return listener->CreatePeer(g, 1024, 1024);
	};
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
				// todo: 队列深度检测. 如果超深, T 掉
				if (peer->GetSendQueueSize() > 100)
				{
					std::cout << "listener: peer->GetSendQueueSize() > 100" << std::endl;
					peer->Release();
					return;
				}

				++counter;
				peer->TimeoutReset();
				//std::cout << "listener: recv pkg" << std::endl;
				peer->Send(o);
			}
		};
	};

	auto timer = loop.CreateTimer(1000, 1000, [&loop, &counter]()
	{
		std::cout << counter << std::endl;
		if (!running) loop.Stop();
	});

	std::cout << "listener: loop.Run();" << std::endl;
	loop.Run();
}
void f2()
{
	// test client
	std::this_thread::sleep_for(std::chrono::milliseconds(100));
	xx::MemPool mp;
	xx::UvLoop loop(&mp);
	loop.InitKcpFlushInterval(10);
	xx::BBuffer_p pkg;
	mp.CreateTo(pkg);
	uint64_t pkgid = 0;
	uint64_t recvpkgid = 0;
	uint64_t counter = 0;
	auto client = loop.CreateUdpClient();
	client->OnReceivePackage = [&](xx::BBuffer& bb)
	{
		//std::cout << "client: recv pkg" << std::endl;
		xx::BBuffer_p recvpkg;
		if (int r = bb.ReadPackage(recvpkg))
		{
			std::cout << "client: bb.ReadPackage(o) fail. r = " << r << std::endl;
		}
		else
		{
			uint64_t id;
			if (r = recvpkg->Read(id))
			{
				std::cout << "client: recvpkg.Read(id) fail. r = " << r << std::endl;
			}
			else
			{
				if (id != recvpkgid)
				{
					std::cout << "client: OnReceivePackage lost package. id = " << id << ", recvpkgid = " << recvpkgid << std::endl;
				}
				else
				{
					recvpkgid++;
				}
			}
		}
		++counter;
	};
	client->SetAddress("127.0.0.1", 12345);
	client->Connect(xx::Guid(), 1024, 1024);

	auto timer = loop.CreateTimer(1000, 1000, [&]()
	{
		std::cout << counter << std::endl;
		if (!running) loop.Stop();
	});

	auto timer2 = loop.CreateTimer(1, 1, [&]()
	{
		//while (client->GetSendQueueSize() < 512)
		//{
			pkg->Clear();
			pkg->Write(pkgid++);
			client->Send(pkg);
		//}
	});

	std::cout << "client: loop.Run();" << std::endl;
	loop.Run();
}
int main()
{
	xx::MemPool::Register<xx::BBuffer, xx::Object>();

	// ctrl + break 事件处理. ctrl + c, 直接 x 拦截不能
	SetConsoleCtrlHandler([](DWORD CtrlType)->BOOL
	{
		running = false;
		return TRUE;
	}, 1);

	std::thread t(f2);
	t.detach();
	f1();
	return 0;
}
