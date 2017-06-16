#include "xx_uv.h"
#include "xx_helpers.h"

struct MyUVServerPeer : xx::UVServerPeer
{
	MyUVServerPeer(xx::UVListener* listener) : xx::UVServerPeer(listener)
	{
		std::cout << "\nAccepted client ip = " << GetPeerName().C_str() << std::endl;
	}
	~MyUVServerPeer()
	{
		std::cout << "\nDisconnected client ip = " << tmpStr->C_str() << std::endl;
	}

	inline virtual void OnReceivePackage(xx::BBuffer& bb) override
	{
		//// 一定会收到一个 uint64_t 的值, 加 1 并返回
		//uint64_t v = 0;
		//if (bb.Read(v))
		//{
		//	Disconnect();
		//	return;
		//}
		//auto sendBB = GetSendBB();
		//sendBB->WritePackage(++v);
		//Send(sendBB);
	}
};

struct MyUVListener : xx::UVListener
{
	MyUVListener(xx::UV* uv, int port, int backlog) : xx::UVListener(uv, port, backlog) {}
	inline virtual xx::UVServerPeer* OnCreatePeer() override
	{
		auto peer = mempool().Create<MyUVServerPeer>(this);
		return peer;
	}
};

struct MyTimer : xx::UVTimer
{
	MyTimer(xx::UV* uv) : xx::UVTimer(uv)
	{
		auto rtv = Start(0, 1);
		assert(!rtv);
	}
	inline virtual void OnFire() override
	{
		std::cout << "." << std::flush;

	}
};

int main()
{
	xx::MemPool mp;
	xx::UV_v uv(mp);
	auto listener = uv->CreateListener<MyUVListener>(12345);
	assert(listener);
	auto timer = uv->CreateTimer<MyTimer>();
	assert(timer);
	uv->Run();
	return 0;
}

