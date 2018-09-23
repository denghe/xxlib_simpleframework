#pragma execution_character_set("utf-8")
//#include "../pkg/PKG_class.h"

// todo: 为 UvTcpClient 加 带超时的 Connect 函数. 同时 GetIPList 要映射到 Lua

#include "xx_uv.h"
int main()
{
	xx::MemPool mp;
	xx::UvLoop uv(&mp);
	auto t = uv.CreateTimer(0, 1000, [&]
	{
		int r = uv.GetIPList("www.baidu.com", [](xx::List<xx::String>* ips)
		{
			if (ips)
			{
				for (auto& ip : *ips)
				{
					std::cout << ip << std::endl;
				}
			}
		});
		std::cout << r << std::endl;
	});
	return uv.Run();
}

















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
