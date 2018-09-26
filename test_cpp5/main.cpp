#pragma execution_character_set("utf-8")
//#include "../pkg/PKG_class.h"

// todo: Unique, Weak 用起来?
#include "xx_uv.h"

int main1()
{
	xx::MemPool mp;
	xx::UvLoop uv(&mp);
	uv.CreateTimer(100, 100, [] {std::cout << "."; });
	uv.DelayExecute([&]
	{
		uv.Stop();
	}, 1000);
	uv.Run();
	return 0;
}

int main()
{
	main1();
	return 0;
}











//
//int main2()
//{
//	xx::MemPool mp;
//	xx::UvLoop uv(&mp);
//	auto c = uv.CreateTcpClient();
//	c->OnConnect = [c](int state)
//	{
//		std::cout << "OnConnect state = " << state << std::endl;
//		if (!state)
//		{
//			c->Disconnect();
//		}
//	};
//	c->OnDisconnect = []
//	{
//		std::cout << "OnDisconnect" << std::endl;
//	};
//	auto t = uv.CreateTimer(100, 0, [&]
//	{
//		int r = uv.GetIPList("www.baidu.com", [&](xx::List<xx::String>* ips)	//8F4637D7-2936-4751-A6D6-8933B7D5FEE7
//		{
//			if (ips)
//			{
//				std::cout << ips->At(0) << std::endl;
//				c->ConnectEx(ips->At(0).c_str(), 80);
//				//c->ConnectEx(ips->At(0).c_str(), 80, 100);
//				//c->Disconnect();
//				//for (auto& ip : *ips)
//				//{
//				//	std::cout << ip << std::endl;
//				//}
//			}
//			else
//			{
//				std::cout << "ips is null" << std::endl;
//			}
//		}, 100);
//		std::cout << "r = " << r << std::endl;
//	});
//	return uv.Run();
//}











//#include "uv.h"
//
//// todo: 封 DNS 解析到 xxuv
//
//void on_resolved(uv_getaddrinfo_t *resolver, int status, struct addrinfo *res)
//{
//	if (status < 0) 
//	{
//		fprintf(stderr, "getaddrinfo callback error\n");
//		return;
//	}
//
//	auto ai = res;
//	do
//	{
//		char addr[33] = { '\0' };
//		uv_ip4_name((struct sockaddr_in*) ai->ai_addr, addr, 32);
//		fprintf(stderr, "%s\n", addr);
//		ai = ai->ai_next;
//	} while (ai);
//
//	uv_freeaddrinfo(res);
//}
//
//int main(int argc, char const* const* argv)
//{
//	auto loop = uv_default_loop();
//
//	//struct addrinfo hints;
//	//hints.ai_family = PF_INET;
//	//hints.ai_socktype = SOCK_STREAM;
//	//hints.ai_protocol = 0;// IPPROTO_TCP;
//	//hints.ai_flags = 0;
//
//	uv_getaddrinfo_t resolver;
//	int r = uv_getaddrinfo(loop, &resolver, on_resolved, "www.baidu.com", nullptr, nullptr);//&hints);
//
//	if (r)
//	{
//		fprintf(stderr, "getaddrinfo call error\n");
//		return 1;
//	}
//	return uv_run(loop, UV_RUN_DEFAULT);
//}
