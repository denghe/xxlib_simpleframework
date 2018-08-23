// todo: noexcept 狂加一波
// todo: 用 sg 语法改进各种库
// todo: xx_uv 从 c# 那边复制备注


#include "xx_uv.h"
#include <iostream>

inline void Host()
{
	xx::MemPool mp;

	// 根据 url 之 path 路由处理函数
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
		peer->OnReceiveHttp = [&pathHandlers, peer]
		{
			peer->ParseUrl();
			auto idx = pathHandlers.Find(peer->path);
			if (idx != -1)
			{
				pathHandlers.ValueAt(idx)(peer);
			}
			else
			{
				peer->Release();
			}
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
	client->OnReceiveHttp = [&]
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
	std::thread t2([] { Test(); });
	std::cin.get();
	return 0;
}
