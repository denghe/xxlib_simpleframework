#include <xx_uv.h>
#include <xx_helpers.h>
#ifdef _WIN32
#include <windows.h>	// Sleep
#endif


// todo: 在 timer 回调中书写状态机以控制 ClientPeer 的生命周期. 理论上讲 ClientPeer 不会因为断线而析构, 故可直接派生写成上下文容器

struct MyClientPeer : xx::UVClientPeer
{
	xx::Stopwatch sw;
	uint64_t v = 0;
	MyClientPeer(xx::UV* uv) : xx::UVClientPeer(uv) {}
	inline virtual void OnConnect() override
	{
		assert(connected);
		auto rtv = SetNoDelay(true);
		assert(!rtv);
		auto bb = GetSendBB();
		bb->WritePackage(v);
		Send(bb);
	}
	inline virtual void OnReceivePackage(xx::BBuffer& bb) override
	{
		uint64_t recv_v = 0;
		if (bb.Read(recv_v) || v + 1 != recv_v)
		{
			Disconnect();
			return;
		}
		v += 2;
		auto sendBB = GetSendBB();
		sendBB->WritePackage(v);
		Send(sendBB);
	}
	inline virtual void OnDisconnect() override
	{
		std::cout << "OnDisconnect status = " << lastStatus << std::endl;
		Release();	// 可以在断开后释放
	}
	~MyClientPeer()
	{
		std::cout << "v = " << v << ", sw = " << sw() << std::endl;
	}
};

struct MyTimer : xx::UVTimer
{
	MyTimer(xx::UV* uv) : xx::UVTimer(uv)
	{
		auto rtv = Start(0, 100);
		assert(!rtv);
	}
	inline virtual void OnFire() override
	{
		std::cout << ".";
	}
};

int main()
{
	std::cout << "Client" << std::endl;
	Sleep(1);
	{
		xx::MemPool mp;
		xx::UV_v uv(mp);
		auto c = uv->CreateClientPeer<MyClientPeer>();
		assert(c);
		auto timer = uv->CreateTimer<MyTimer>();
		assert(timer);
		auto rtv = c->SetAddress("127.0.0.1", 12345);
		assert(!rtv);
		rtv = c->SetNoDelay(true);
		assert(!rtv);
		rtv = c->Connect();
		assert(!rtv);
		uv->Run();
	}
	system("pause");
}

