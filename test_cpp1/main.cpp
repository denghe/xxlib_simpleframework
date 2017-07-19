#include "xx_uv.h"
#include "xx_helpers.h"
#include <thread>
#include <chrono>

struct MyAsync : xx::UVAsync
{
	MyAsync(xx::UV* uv) : xx::UVAsync(uv) {}
	inline virtual void OnFire() override
	{
		std::cout << "." << std::flush;
	}
};

struct MyConnector : xx::UVClientPeer
{
	virtual void OnReceivePackage(xx::BBuffer & bb) override
	{
	}
	virtual void OnConnect() override
	{
	}
};

struct MyTimer : xx::UVTimer
{
	inline MyTimer(xx::UV* uv) : xx::UVTimer(uv)
	{
		Start(0, 1);
	}
	virtual void OnFire() override
	{
		if (Update()) Release();
	}

	int lineNumber = 0;
	int Update()
	{
		XX_CORO_BEGIN();
		XX_CORO_(1);
		XX_CORO_(2);
		XX_CORO_END();
	}
};

int main()
{
	xx::MemPool mp;
	xx::UV_v uv(mp);
	auto myAsync = uv->CreateAsync<MyAsync>();

	std::thread t([=]
	{
		while (true)
		{
			std::this_thread::sleep_for(std::chrono::milliseconds(1));
			myAsync->Fire();
		}
	});
	uv->Run();
	return 0;
}
