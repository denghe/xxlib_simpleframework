#include <iostream>
#include <chrono>
#include <thread>
#include "../testuv_s/xx_uv.h"

struct MyClientPeer : xx::UVClientPeer
{
	MyClientPeer(xx::UV* uv) : xx::UVClientPeer(uv) {}
	inline virtual void OnConnect() override
	{
		std::cout << "OnConnect status = " << lastStatus << std::endl;
		if (connected)
		{
			auto bb = GetSendBB();
			bb->WritePackage("abcd");
			bb->WritePackage("abcd");
			auto rtv = Send(bb);
			if (rtv)
			{
				std::cout << "Send Error. rtv = " << rtv << std::endl;
			}
		}
	}
	inline virtual void OnReceivePackage(xx::BBuffer const & bb) override
	{
		std::cout << "OnReceivePackage bb dataLen = " << bb.dataLen << std::endl;
	}
	inline virtual void OnDisconnect() override
	{
		std::cout << "OnDisconnect status = " << lastStatus << std::endl;
	}
};

int main()
{
	std::this_thread::sleep_for(std::chrono::milliseconds(1000));
	xx::MemPool mp;
	xx::UV_v uv(mp);
	auto c = uv->CreateClient<MyClientPeer>();
	assert(c);
	c->SetAddress("127.0.0.1", 12345);
	auto rtv = c->Connect();
	assert(!rtv);
	uv->Run();
}














//#include <iostream>
//#include <chrono>
//#include <thread>
//#include <uv.h>
//#include <xx_mempool.h>
//#include <xx_bbqueue.h>
//
//using namespace std;
//int main()
//{
//    this_thread::sleep_for(chrono::milliseconds(100));
//
//    auto loop = uv_default_loop();
//
//    uv_tcp_t socket;
//    uv_tcp_init(loop, &socket);
//
//    sockaddr_in addr;
//    uv_ip4_addr("127.0.0.1", 12345, &addr);
//
//    uv_connect_t conn;
//    uv_tcp_connect(&conn, &socket, (sockaddr*)&addr, [](uv_connect_t* conn, int status)
//    {
//        if (status < 0)
//        {
//            cout << "uv_tcp_connect error " << uv_strerror(status) << endl;
//            return;
//        }
//
//		// Ëæ±ãÐ´µÄ. ÓÐÐ¹Â¶
//		auto& mp = *new xx::MemPool();
//		xx::BBQueue_v bbq(mp);
//		xx::List_v<uv_buf_t> bufs(mp);
//		auto bb = bbq->PopLastBB();
//		bb->WritePackage("abcd");
//		bb->WritePackage("abcd");
//		bbq->Push(bb);
//
//		//uv_read_start(conn->handle, [](uv_handle_t* handle, size_t suggested_size, uv_buf_t* buf)
//		//{
//		//	cout << "suggested_size = " << suggested_size << endl;
//		//	buf->base = (char*)malloc(suggested_size);
//		//	buf->len = (ULONG)suggested_size;
//		//}, [](uv_stream_t* client, ssize_t nread, const uv_buf_t* buf)
//		//{
//		//	if (nread < 0)
//		//	{
//		//		if (nread != UV_EOF)
//		//		{
//		//			cout << "Read error " << uv_err_name((int)nread) << endl;
//		//		}
//		//	}
//		//	else if (nread > 0)
//		//	{
//		//		cout << "nread = " << nread << " content = " << buf->base << endl;
//		//		// todo: dump buf
//		//	}
//		//	uv_close((uv_handle_t*)client, nullptr);
//		//	// todo: close conn ?
//		//});
//
//		while (bbq->PopTo(*bufs, 10))
//		{
//			std::cin.get();
//
//			auto write_req = (uv_write_t *)mp.Alloc(sizeof(uv_write_t));
//			write_req->data = &mp;
//			uv_write(write_req, conn->handle, bufs->buf, bufs->dataLen, [](uv_write_t *write_req, int status)
//			{
//				if (status < 0)
//				{
//					cout << "uv_write error " << uv_strerror(status) << endl;
//					return;
//				}
//
//				//free(write_req);
//				((xx::MemPool*)write_req->data)->Free(write_req);
//			});
//		}
//    });
//
//    uv_run(loop, UV_RUN_DEFAULT);
//    return 0;
//}
