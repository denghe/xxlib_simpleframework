#include "main.h"
#include <thread>
#include <chrono>

/*************************************************************************************/

inline MyAsync::MyAsync(xx::UV* uv, MyClient* owner) : xx::UVAsync(uv) {}
inline void MyAsync::OnFire()
{
	std::cout << "." << std::flush;
}


/*************************************************************************************/

inline MyConnector::MyConnector(xx::UV* uv, MyClient* owner) : xx::UVClientPeer(uv) {}
inline void MyConnector::OnReceivePackage(xx::BBuffer & bb)
{
}
inline void MyConnector::OnConnect()
{
}


/*************************************************************************************/

inline MyTimer::MyTimer(xx::UV* uv, MyClient* owner) : xx::UVTimer(uv)
{
	Start(0, 1);
}
inline void MyTimer::OnFire()
{
	if (Update()) Release();
}

inline int MyTimer::Update()
{
	XX_CORO_BEGIN();
	XX_CORO_(1);
	XX_CORO_(2);
	XX_CORO_END();
	return -1;
}


/*************************************************************************************/

inline MyClient::MyClient(xx::UV* uv)
	: uv(uv)
	, conn(mempool(), this)
	, timer(mempool(), this)
	, dispacher(mempool(), this)
{
}

/*************************************************************************************/

int main()
{
	xx::MemPool mp;
	xx::UV_v uv(mp);
	MyClient c(uv);
	uv->Run();
	return 0;
}

//auto myAsync = uv->CreateAsync<MyAsync>();
//std::thread t([=]
//{
//	while (true)
//	{
//		std::this_thread::sleep_for(std::chrono::milliseconds(1));
//		myAsync->Fire();
//	}
//});
