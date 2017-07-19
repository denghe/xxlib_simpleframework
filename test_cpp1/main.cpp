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
