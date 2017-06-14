#include <xx_uv.h>
#include <xx_helpers.h>

// todo: 模拟玩家上下文, 对于 UVServerPeer 会在断开时析构的特性做出处理. 初步考虑用双向指针与上下文类( 不会随着断线就删的 )建立关联, 在析构时从上下文清除并同步状态

struct MyUVServerPeer : xx::UVServerPeer
{
	MyUVServerPeer(xx::UVListener* listener) : xx::UVServerPeer(listener) 
	{
		if (auto rtv = SetNoDelay(true)) throw rtv;
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
		return mempool().Create<MyUVServerPeer>(this);
	}
};

struct MyTimer : xx::UVTimer
{
	MyTimer(xx::UV* uv) : xx::UVTimer(uv, 0, 100) {}
	inline virtual void OnFire() override
	{
		std::cout << ".";
	}
};

int main()
{
	std::cout << "Server" << std::endl;
	xx::MemPool mp;
	xx::UV_v uv(mp);
	auto listener = uv->CreateListener<MyUVListener>(12345);
	assert(listener);
	auto timer = uv->CreateTimer<MyTimer>();
	assert(timer);
	uv->Run();
	return 0;
}






















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
