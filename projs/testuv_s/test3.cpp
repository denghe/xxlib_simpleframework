#include "xx_uv.h"
#include "xx_helpers.h"

// todo: 模拟玩家上下文, 对于 UVServerPeer 会在断开时析构的特性做出处理. 初步考虑用双向指针与上下文类( 不会随着断线就删的 )建立关联, 在析构时从上下文清除并同步状态

struct MyUVServerPeer : xx::UVServerPeer
{
	MyUVServerPeer(xx::UVListener* listener) : xx::UVServerPeer(listener)
	{
		//auto rtv = SetNoDelay(true);
		//assert(rtv);
		std::cout << "\nAccepted client ip = " << GetPeerName().C_str() << std::endl;
	}

	inline virtual void OnDisconnect() override
	{
		std::cout << "\nDisconnected client ip = " << tmpStr->C_str() << std::endl;
		Release();		// 释放资源
	}

	inline virtual void OnReceive() override
	{
		// 覆盖为 echo 模式, 废掉 OnReceivePackage
		auto sendBB = GetSendBB();
		sendBB->WriteBuf(this->bbReceive);
		Send(sendBB);
	}

	inline virtual void OnReceivePackage(xx::BBuffer& bb) override
	{
		// 一定会收到一个 uint64_t 的值, 加 1 并返回
		uint64_t v = 0;
		if (bb.Read(v))
		{
			Disconnect();
			return;
		}
		auto sendBB = GetSendBB();
		sendBB->WritePackage(++v);
		Send(sendBB);
	}
};

struct MyUVListener : xx::UVListener
{
	MyUVListener(xx::UV* uv, int port, int backlog) : xx::UVListener(uv, port, backlog) {}
	inline virtual xx::UVServerPeer* OnCreatePeer() override
	{
		auto peer = mempool().Create<MyUVServerPeer>(this);
		//auto rtv = peer->SetNoDelay(true);
		//assert(rtv);
		return peer;
	}
};

struct MyTimer : xx::UVTimer
{
	MyTimer(xx::UV* uv) : xx::UVTimer(uv)
	{
		auto rtv = Start(0, 1000);
		assert(!rtv);
	}
	inline virtual void OnFire() override
	{
		std::cout << "." << std::flush;
		
	}
};

int main()
{
	//std::cout << "+1 Server" << std::endl;
	std::cout << "Echo Server" << std::endl;
	xx::MemPool mp;
	xx::UV_v uv(mp);
	auto listener = uv->CreateListener<MyUVListener>(12345);
	assert(listener);
	auto timer = uv->CreateTimer<MyTimer>();
	assert(timer);
	uv->Run();
	return 0;
}

















//for (auto& listener : *uv->listeners)
//{
//	for (auto& p : *listener->peers)
//	{
//		auto sendBB = p->GetSendBB();
//		sendBB->WritePackage(1);
//		p->Send(sendBB);
//	}
//}

//xx::BBQueue_v bbq(mp);
//xx::List_v<uv_buf_t> bufs(mp);
//for (int i = 0; i < 99999; ++i)
//{
//	Sleep(1);
//	auto bb = bbq->PopLastBB();
//	bb->WritePackage(1);
//	bbq->Push(bb);
//	bbq->PopTo(*bufs, 4096);
//}


//#include <time.h>
//struct MyUV : xx::UV
//{
//	clock_t clockBegin;
//	uint64_t counter = 0;
//	
//	MyUV() : xx::UV()
//	{
//		EnableIdle();
//		clockBegin = clock();
//	}
//	virtual void OnIdle() override
//	{
//		++counter;
//		if (counter % 65536 == 0)
//		{
//			std::cout << "counter = " << counter << ", elapsedMS = " << (clock() - clockBegin) << std::endl;
//		}
//	}
//};




//inline virtual void OnReceive() override
//{
//	std::cout << "OnReceive" << std::endl;
//	Dump(bbReceive);
//	this->xx::UVServerPeer::OnReceive();
//	Dump(bbReceiveLeft);
//}


//std::cout << "OnReceivePackage bb content = ";
//xx::Dump(bb);

//// 实现 echo 效果
//auto sendBB = GetSendBB();
//sendBB->WritePackageLength(bb.dataLen);
//sendBB->WriteBuf(bb);
//Send(sendBB);

//std::cout << "sendBB content = ";
//xx::Dump(sendBB);
