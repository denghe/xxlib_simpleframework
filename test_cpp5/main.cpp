#include "xx_uv.h"

//void f1()			// rpc echo server
//{
//	xx::MemPool mp;
//	xx::UvLoop loop(&mp);
//	loop.InitRpcManager();
//	auto listener = loop.CreateTcpListener();
//	listener->Bind("0.0.0.0", 12345);
//	listener->Listen();
//	xx::UvAsync dispacher(loop);
//
//	// 模拟一个线程池
//	Concurrency::concurrent_queue<Task*> tasks;
//	std::thread t([&]
//	{
//		while (true)
//		{
//			Task* task = nullptr;
//			while (tasks.try_pop(task))
//			{
//				dispacher.Dispatch([task] 
//				{
//					if (task->peer)
//					{
//						xx::String_p str;
//						task->peer->mempool->MPCreateTo(str, task->str);
//						task->peer->SendResponse(task->serial, str);
//					}
//					delete task;
//				});
//			}
//			Sleep(1);
//		}
//	});
//	t.detach();
//
//	uint64_t counter = 0;
//	listener->OnAccept = [&loop, &counter, &tasks](xx::UvTcpPeer* peer)
//	{
//		peer->OnReceiveRequest = [peer, &counter, &tasks](uint32_t serial, xx::BBuffer& bb)
//		{
//			xx::Object_p o;
//			if (int r = bb.ReadRoot(o))			// 解不出来
//			{
//				peer->Release();
//				return;
//			}
//			auto& pkg = *(xx::String_p*)&o;
//			if (pkg->Equals("asdf"))	++counter;
//			
//			tasks.push(new Task{ peer, serial, pkg->c_str() });
//			//peer->SendResponse(serial, o);
//		};
//	};
//	auto timer = loop.CreateTimer(1000, 1000, [&loop, &counter]()
//	{
//		std::cout << "server counter = " << counter << std::endl;
//	});
//	loop.Run();
//}

class MyListener : public xx::UvTcpListener
{
public:
	xx::String lastIp;
};
class MyPeer : public xx::UvTcpPeer
{
public:
	inline virtual void ReceiveImpl(char const* bufPtr, int len) override
	{
		((MyListener*)&this->listener)->lastIp = ip();
		Release();
	}
};


int main(int arg, char *arv[])
{
	// 从参数读端口并建立监听


	return 0;
}
