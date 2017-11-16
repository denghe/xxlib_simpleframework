#pragma execution_character_set("utf-8")

#include "lua_nbsocket.h"
#include "std_cout_helper.h"
#include <thread>
#include <chrono>
using namespace std::chrono_literals;

void DumpState(NBSocket::States s, int ticks)
{
	static char const* ss[] =
	{
		"Disconnected",
		"Connecting",
		"Connected",
		"Disconnecting"
	};
	CoutLine("state = ", ss[(int)s], " ticks = ", ticks);
}

int main()
{
	//CoutLine(NBSocket::Run());
	Lua_MemPool mp;
	NBSocket::SockInit();
	NBSocket nbs(&mp);

	auto s = nbs.GetState();
	DumpState(s, nbs.GetTicks());

	nbs.SetAddress("127.0.0.1", 12345);
	nbs.Connect();

	DumpState(s = nbs.GetState(), nbs.GetTicks());
	while (true)
	{
		s = nbs.GetState();
		switch (s)
		{
		case NBSocket::States::Disconnected:
		{
			goto LabEnd;
		}
		case NBSocket::States::Connecting:
		{
			if (nbs.GetTicks() > 10)	// 1 秒连不上就算超时吧
			{
				nbs.Disconnect();
			}
			break;
		}
		case NBSocket::States::Connected:
		{
			// 刚连上时发包
			if (nbs.GetTicks() == 0) nbs.Send("\x4\x0""abcd""\x4\x0""efgh", 12);

			// dump 收到的包
			while (!nbs.recvBufs.empty())
			{
				auto& front = nbs.recvBufs.front();
				CoutLine("recv pkg. len = ", front.dataLen);
				nbs.recvBufs.pop_front();
			}

			if (nbs.ticks > 10) nbs.Disconnect(2);	// 存活 10 帧后, 延迟 2 帧杀掉
			break;
		}
		case NBSocket::States::Disconnecting:
			break;

		default:
			break;
		}
		DumpState(s = nbs.GetState(), nbs.GetTicks());
		std::this_thread::sleep_for(100ms);
		nbs.Update();
	}
LabEnd:
	DumpState(s = nbs.GetState(), nbs.GetTicks());
	CoutLine("press any key to continue...");
	std::cin.get();
	return 0;
}
