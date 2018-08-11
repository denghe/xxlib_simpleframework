// todo: noexcept 狂加一波

#include "xx_uv.h"
#include <iostream>

inline void Host()
{
	xx::MemPool mp;
	xx::UvLoop loop(&mp);
	auto listener = loop.CreateTcpListener();
	listener->OnCreatePeer = [listener]
	{
		auto peer = listener->mempool->Create<xx::UvHttpPeer>(*listener);
		//peer->rawData.MPCreate(peer->mempool);
		peer->OnMessageComplete = [peer]
		{
			//std::cout << "host rawData = " << peer->rawData << std::endl;

			xx::String s(peer->mempool);
			s.AppendFormat("url: {0}\r\n", peer->url);
			for (auto& kv : peer->headers)
			{
				s.AppendFormat("key: {0}, value: {1}\r\n", kv.key, kv.value);
			}
			s.Append("body: ", peer->body);
			peer->SendHttpResponse(s.buf, s.dataLen);
			return 0;
		};
		peer->OnError = [peer](uint32_t errorNumber, char const* errorMessage)
		{
			//std::cout << errorMessage << std::endl;
		};
		return peer;
	};
	listener->Bind("0.0.0.0", 10080);
	listener->Listen();
	std::cout << "Host...\r\n";
	loop.Run();
}

inline void Test()
{
	xx::MemPool mp;
	xx::UvLoop loop(&mp);
	auto client = mp.Create<xx::UvHttpClient>(loop);
	client->rawData.MPCreate(&mp);
	client->OnConnect = [client](auto status)
	{
		if (status)
		{
			std::cout << "connecte failed. status = " << status << std::endl;
			return;
		}
		xx::String txt(client->mempool);
		txt = "GET /test HTTP/1.1\r\n"
			"User-Agent: curl/7.18.0 (i486-pc-linux-gnu) libcurl/7.18.0 OpenSSL/0.9.8g zlib/1.2.3.3 libidn/1.1\r\n"
			"Host: 0.0.0.0=5000\r\n"
			"Accept: */*\r\n"
			"Transfer-Encoding: chunked\r\n"
			"\r\n"
			"a\r\n1234567890\r\n"
			"1e\r\n123456789012345678901234567890\r\n"
			"0\r\n\r\n";
		client->SendBytes(txt.buf, (int)txt.dataLen);
	};
	client->OnMessageComplete = [client]
	{
		//std::cout << "client rawData = " << client->rawData << std::endl;
		std::cout << "client body = " << client->body << std::endl;
		return 0;
	};
	client->SetAddress("127.0.0.1", 10080);
	client->Connect();
	std::cout << "Test...\r\n";
	loop.Run();
}

int main()
{
	std::thread t1([] { Host(); });
	t1.detach();

	std::thread t2([] { Test(); });
	t2.detach();

	std::cin.get();
	return 0;
}


//buf = "POST /xxxx HTTP/1.1\r\n"
//        "Transfer-Encoding: chunked\r\n"
//        "\r\n"
//        "1e\r\nall your base are belong to us\r\n"
//        "1e\r\nall your base are belong to us\r\n"
//        "0\r\n"
//	"\r\n";

//buf = "GET /test HTTP/1.1\r\n"
//	"User-Agent: curl/7.18.0 (i486-pc-linux-gnu) libcurl/7.18.0 OpenSSL/0.9.8g zlib/1.2.3.3 libidn/1.1\r\n"
//	"Host: 0.0.0.0=5000\r\n"
//	"Accept: */*\r\n"
//"\r\n";


//buf = "HTTP/1.1 200 OK\r\n"
//	//"Content-Type: text/plain\r\n"
//	"Content-Length: 12\r\n"
//	"\r\n"
//	"hello world\n";







//#include "xx.h"
//int main()
//{
//	xx::MemPool mp_, *mp = &mp_;
//	auto s = mp->Str("asdf");
//	auto refS = s.MakeRef();
//	xx::Dict<xx::String_p, xx::String_r> ss(mp);
//	ss.Add(s, refS);
//	return 0;
//}


//#define _CRT_SECURE_NO_WARNINGS
//
//
//#include "xx_uv.h"
//#include "../pkg/RPC_class.h"
//// 超时值容器
//struct TimeoutData : public xx::UvTimeouterBase
//{
//	TimeoutData(xx::MemPool* const& mp) : xx::UvTimeouterBase(mp) {}
//	int keyIndex = -1;
//	xx::Object_p value;		// todo: 已序列化结果缓存?
//};
//
//void f1()
//{
//	xx::MemPool mp;
//	xx::UvLoop uvLoop(&mp);
//	xx::UvTimeoutManager uvTM(uvLoop, 1000, 21, 20);
//	xx::Dict<xx::String_p, TimeoutData*> cache(&mp);
//	// 模拟放入一个值
//	{
//		// 创建一个超时值容器
//		auto d = mp.MPCreate<TimeoutData>();
//
//		// 设超时回调: 从字典移除后自杀
//		d->OnTimeout = [d, &cache]
//		{
//			cache.RemoveAt(d->keyIndex);
//			std::cout << "timeout. release: " << d->value << std::endl;
//			d->Release();
//		};
//		d->timeouter = &uvTM;
//		uvTM.Add(d);			// 加入超时管理
//
//		// 模拟填值
//		d->value = mp.MPCreatePtr<xx::String>("asdfqwerzxcv");
//
//		// 放入字典, 存下标
//		auto r = cache.Add(mp.MPCreatePtr<xx::String>("a"), d);
//		d->keyIndex = r.index;
//	}
//	// 模拟读取一个值
//	{
//		auto key = mp.MPCreatePtr<xx::String>("a");
//		auto idx = cache.Find(key);
//		if (idx != -1)
//		{
//			auto d = cache.ValueAt(idx);
//			uvTM.AddOrUpdate(d);			// 刷新超时
//
//			std::cout << d->value << std::endl;
//		}
//	}
//
//	//xx::UvTcpListener uvListener(uvLoop);
//	//uvListener.Bind("0.0.0.0", 12345);
//
//	uvLoop.Run();
//}
//
//void f2()
//{
//
//}
//
//int main()
//{
//	f1();
//
//	return 0;
//}
