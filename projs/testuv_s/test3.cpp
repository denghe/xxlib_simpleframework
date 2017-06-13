#include <xx_uv.h>
#include <xx_helpers.h>

struct MyUVServerPeer : xx::UVServerPeer
{
	MyUVServerPeer(xx::UVListener* listener) : xx::UVServerPeer(listener) {}
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

int main()
{
	std::cout << "Server" << std::endl;
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


//std::cout << "OnReceivePackage bb content = ";
//xx::Dump(bb);

//// 实现 echo 效果
//auto sendBB = GetSendBB();
//sendBB->WritePackageLength(bb.dataLen);
//sendBB->WriteBuf(bb);
//Send(sendBB);

//std::cout << "sendBB content = ";
//xx::Dump(sendBB);
