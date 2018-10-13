#pragma execution_character_set("utf-8")
//#include "../pkg/PKG_class.h"

// todo: Unique, Weak 用起来?

#include "xx_uv.h"

xx::MemPool mp;

template<typename...Args>
void Cout(Args const&...args)
{
	xx::String s(&mp);
	s.Append(args...);
	std::cout << s;
}

int main1()
{
	xx::UvLoop uv(&mp);
	uv.CreateTimer(0, 100, [] { Cout("."); });
	uv.CreateTimer(0, 100, [] { Cout("a"); });
	uv.DelayExecute([&]
	{
		uv.Stop();
	}, 1000);
	uv.Run();
	return 0;
}

struct Foo
{
	int v1, v2;
};

int main2()
{
	xx::DictEx<Foo, decltype(Foo::v1), decltype(Foo::v2)> de(&mp);

	{
		Foo f{ 1, 2 };

		auto r = de.Add(f, f.v1, f.v2);
		Cout(r.success, ", ", r.index, '\n');

		r = de.Add(f, f.v1, f.v2);
		Cout(r.success, ", ", r.index, '\n');
	}

	{
		Foo f{ 2, 3 };

		auto r = de.Add(f, f.v1, f.v2);
		Cout(r.success, ", ", r.index, '\n');

		r = de.Add(f, f.v1, f.v2);
		Cout(r.success, ", ", r.index, '\n');
	}

	de.Add(Foo{}, 3, 4);
	de.Add(Foo{}, 4, 5);
	de.Add(Foo{}, 5, 6);
	de.Add(Foo{}, 6, 7);

	Cout("find 0/3, 1/4 : ", de.Find<0>(3), " ", de.Find<1>(4), "\n");

	de.Remove<1>(4);
	assert(de.Find<0>(3) == -1);
	int idx = de.Find<0>(4);
	de.RemoveAt(idx);

	for (decltype(auto) iv : de)
	{
		Cout("k0: ", de.KeyAt<0>(iv.index), ", k1: ", de.KeyAt<1>(iv.index), ", foo = ", iv.Value().v1, ", ", iv.Value().v2, "\n");
	}

	return 0;
}

int main()
{
	main2();
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
//				//for (decltype(auto) ip : *ips)
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
