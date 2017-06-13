#include "xx_uv.h"
#include "xx_helpers.h"

struct MyUVServerPeer : xx::UVServerPeer
{
	MyUVServerPeer(xx::UVListener* listener) : xx::UVServerPeer(listener) {}
	inline virtual void OnReceivePackage(xx::BBuffer const& bb) override
	{
		std::cout << "OnReceivePackage bb content = ";
		xx::Dump(bb);

		// 实现 echo 效果
		auto sendBB = GetSendBB();
		sendBB->WritePackageLength(bb.dataLen);
		sendBB->WriteBuf(bb);
		Send(sendBB);

		std::cout << "sendBB content = ";
		xx::Dump(sendBB);
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

int main()
{
	xx::MemPool mp;
	xx::UV_v uv(mp);
	auto rtv = uv->CreateListener<MyUVListener>(12345);
	assert(rtv);
	uv->Run();
	return 0;
}


























//inline virtual void OnReceive() override
//{
//	std::cout << "OnReceive" << std::endl;
//	Dump(bbReceive);
//	this->xx::UVServerPeer::OnReceive();
//	Dump(bbReceiveLeft);
//}
