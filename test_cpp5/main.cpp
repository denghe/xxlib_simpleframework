// todo: 用 sg 语法改进 uv 库
// todo: 所有回调理论上讲都应该用版本检查来探测回调后是否 this 变野
// todo: 所有 c api 回调实现如果有 call 用户回调, 都应该 try
// todo: cpp 版 uv 库不再用 try
// todo: 一些 catch ... 要改成 Release Disconnect 之类?
// todo: 一些 void 返回值要改成 int
// todo: serial 考虑改成 int
// todo: 将回调的 try 用宏包起来做开关

// todo: noexcept 狂加一波


#include "xx_uv.h"
#include <iostream>

inline void Host()
{
	xx::MemPool mp;
	xx::Dict<char const*, std::function<void(xx::UvHttpPeer*)>> pathHandlers(&mp);

	pathHandlers["test"] = [](xx::UvHttpPeer* peer)
	{
		peer->s.Append("{ \"method\":\"", peer->method
			, "\", \"path\":\"", peer->path
			, "\", \"queries\":", peer->queries
			, "\", \"headers\":", peer->headers
			, "\", \"body\":\"", peer->body, "\""
		);
		peer->SendHttpResponse();
	};
	pathHandlers["add_1"] = [](xx::UvHttpPeer* peer)
	{
		if (peer->queries.dataLen == 1 && strcmp(peer->queries[0].first, "value") == 0)
		{
			int value = 0;
			xx::FromString(value, peer->queries[0].second);
			++value;
			peer->s.Append(value);
		}
		else
		{
			peer->s.Append("invalid argument.");
		}
		peer->SendHttpResponse();
	};

	xx::UvLoop loop(&mp);
	auto listener = loop.CreateTcpListener();
	listener->OnCreatePeer = [&pathHandlers, listener]
	{
		auto peer = listener->mempool->Create<xx::UvHttpPeer>(*listener);
		peer->rawData.MPCreate(peer->mempool);
		peer->OnMessageComplete = [&pathHandlers, peer]
		{
			peer->ParseUrl();
			auto idx = pathHandlers.Find(peer->path);
			if (idx != -1)
			{
				// 根据 url 之 path 路由处理函数
				pathHandlers.ValueAt(idx)(peer);
			}
			else
			{
				peer->Release();
			}
			return 0;
		};
		peer->OnError = [peer](uint32_t errorNumber, char const* errorMessage)
		{
			std::cout << errorMessage << std::endl;
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

	xx::String txt(&mp);
	int value = 0;
	xx::Stopwatch sw;

	auto client = mp.Create<xx::UvHttpClient>(loop);
	client->OnConnect = [&](auto status)
	{
		if (status)
		{
			std::cout << "connecte failed. status = " << status << std::endl;
			return;
		}
		txt.Clear();
		txt.Append("GET /add_1?value=", value, " HTTP/1.1\r\n""\r\n");
		client->SendBytes(txt.buf, (int)txt.dataLen);
	};
	client->OnMessageComplete = [&]
	{
		xx::FromString(value, client->body.c_str());
		if (value == 100000)
		{
			std::cout << "elapsed ms = " << sw() << ", value = " << value << std::endl;
		}
		else
		{
			txt.Clear();
			txt.Append("GET /add_1?value=", value, " HTTP/1.1\r\n""\r\n");
			client->SendBytes(txt.buf, (int)txt.dataLen);
		}
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
