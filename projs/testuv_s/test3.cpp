#include "xx_uv.h"

struct MyUVServerPeer : xx::UVServerPeer
{
	MyUVServerPeer(xx::UVListener* listener) : xx::UVServerPeer(listener) {}
	virtual void OnReceive(char* buf, ssize_t len) override
	{
		for (ssize_t i = 0; i < len; ++i)
		{
			std::cout << buf[i];
		}
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
		uv->CreateListener<MyUVListener>(12346);
		uv->Run();
	}
	return 0;
}
