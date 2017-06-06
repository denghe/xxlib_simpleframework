#include "xx_uv.h"

void Dump(xx::MPObject const* o)
{
	if (!o) return;
	xx::String_v str(o->mempool());
	o->ToString(*str);
	std::cout << str->C_str() << std::endl;
}

struct MyUVServerPeer : xx::UVServerPeer
{
	MyUVServerPeer(xx::UVListener* listener) : xx::UVServerPeer(listener) {}
	virtual void OnReceive() override
	{
		std::cout << "OnReceive" << std::endl;
		Dump(bbReceive);
		this->xx::UVServerPeer::OnReceive();
		Dump(bbReceiveLeft);
	}
	virtual void OnReceivePackage(xx::BBuffer const& bb) override
	{
		Dump(&bb);
	}
};

struct MyUVListener : xx::UVListener
{
	MyUVListener(xx::UV* uv, int port, int backlog) : xx::UVListener(uv, port, backlog) {}
	virtual xx::UVServerPeer* OnCreatePeer() override
	{
		return mempool().Create<MyUVServerPeer>(this);
	}
};

int main()
{
	{
		xx::MemPool mp;
		xx::UV_v uv(mp);
		uv->CreateListener<MyUVListener>(12345);
		uv->Run();
	}
	return 0;
}
