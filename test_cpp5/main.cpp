// todo: noexcept 狂加一波

#include "http_parser.h"

#include <string>
#include <iostream>

#include "xx_uv.h"

namespace xx
{
	class UvHttpPeer : public UvTcpPeer
	{
		UvHttpPeer(UvTcpListener& listener) : UvTcpPeer(listener) {}
		virtual void ReceiveImpl(char const* const& bufPtr, int const& len) override
		{
			// todo
		}
	};
}

int main()
{
	http_parser parser;
	http_parser_settings parser_settings;
	http_parser_init(&parser, HTTP_BOTH);
	parser_settings.on_message_begin = [](http_parser* parser)->int
	{
		std::cout << "message begin." << std::endl;
		return 0;
	};
	parser_settings.on_url = [](http_parser*, const char *at, size_t length)->int
	{
		std::string s(at, length);
		std::cout << "url: " << s << std::endl;
		return 0;
	};
	parser_settings.on_status = [](http_parser*, const char *at, size_t length)->int
	{
		std::string s(at, length);
		std::cout << "status: " << s << std::endl;
		return 0;
	};
	parser_settings.on_header_field = [](http_parser*, const char *at, size_t length)->int
	{
		std::string s(at, length);
		std::cout << "key: " << s << std::endl;
		return 0;
	};
	parser_settings.on_header_value = [](http_parser*, const char *at, size_t length)->int
	{
		std::string s(at, length);
		std::cout << "value: " << s << std::endl;
		return 0;
	};
	parser_settings.on_headers_complete = [](http_parser* parser)->int
	{
		std::cout << "headers complete." << std::endl;
		return 0;
	};
	parser_settings.on_body = [](http_parser*, const char *at, size_t length)->int
	{
		std::string s(at, length);
		std::cout << "body: " << s << std::endl;
		return 0;
	};
	parser_settings.on_message_complete = [](http_parser* parser)->int
	{
		std::cout << "message complete." << std::endl;
		return 0;
	};
	parser_settings.on_chunk_header = [](http_parser* parser)->int
	{
		std::cout << "on_chunk_header. content_length = " << parser->content_length << std::endl;
		return 0;
	};
	parser_settings.on_chunk_complete = [](http_parser* parser)->int
	{
		std::cout << "on_chunk_complete." << std::endl;
		return 0;
	};



	std::string buf;

	//buf = "POST /xxxx HTTP/1.1\r\n"
 //        "Transfer-Encoding: chunked\r\n"
 //        "\r\n"
 //        "1e\r\nall your base are belong to us\r\n"
 //        "1e\r\nall your base are belong to us\r\n"
 //        "0\r\n"
	//	"\r\n";

	//buf = "GET /test HTTP/1.1\r\n"
	////	"User-Agent: curl/7.18.0 (i486-pc-linux-gnu) libcurl/7.18.0 OpenSSL/0.9.8g zlib/1.2.3.3 libidn/1.1\r\n"
	////	"Host: 0.0.0.0=5000\r\n"
	////	"Accept: */*\r\n"
	//	"\r\n";


	//buf = "HTTP/1.1 200 OK\r\n"
	//	//"Content-Type: text/plain\r\n"
	//	"Content-Length: 12\r\n"
	//	"\r\n"
	//	"hello world\n";

	buf = "POST /xxxx HTTP/";
	auto siz = http_parser_execute(&parser, &parser_settings, buf.data(), buf.size());
	std::cout << "parse siz = " << siz << std::endl;
	std::cout << "http_errno = " << parser.http_errno << ", msg = " << http_errno_description((http_errno)parser.http_errno) << std::endl;

	return 0;
}

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
