#include "xx_uv.h"

struct MyUVServerPeer : xx::UVServerPeer
{
	MyUVServerPeer(xx::UVListener* listener) : xx::UVServerPeer(listener) {}
	virtual void OnReceive(char* buf, ssize_t len) override
	{
		// todo
	}
};

struct MyUVListener : xx::UVListener
{
	MyUVListener(xx::UV* uv, int backlog, int port) : xx::UVListener(uv, backlog, port) {}
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
		uv->CreateListener<MyUVListener>(128, 12345);
		uv->Run();
	}
	return 0;
}
