// test_clr.cpp : main project file.

#include "stdafx.h"

using namespace System;

#include <xx_uv.h>
#include <xx_helpers.h>

struct MyUVServerPeer : xx::UVServerPeer
{
	MyUVServerPeer(xx::UVListener* listener) : xx::UVServerPeer(listener)
	{
		//auto rtv = SetNoDelay(true);
		//assert(rtv);
		std::cout << "\nAccepted client ip = " << GetPeerName().C_str() << std::endl;
	}

	inline virtual void OnDisconnect() override
	{
		std::cout << "\nDisconnected client ip = " << tmpStr->C_str() << std::endl;
		Release();		// 释放资源
	}
	inline virtual void OnReceive() override
	{
		// 覆盖为 echo 模式, 废掉 OnReceivePackage
		auto sendBB = GetSendBB();
		sendBB->WriteBuf(this->bbReceive);
		Send(sendBB);
	}

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
		auto peer = mempool().Create<MyUVServerPeer>(this);
		//auto rtv = peer->SetNoDelay(true);
		//assert(rtv);
		return peer;
	}
};

struct MyTimer : xx::UVTimer
{
	MyTimer(xx::UV* uv) : xx::UVTimer(uv)
	{
		auto rtv = Start(0, 1000);
		assert(!rtv);
	}
	inline virtual void OnFire() override
	{
		std::cout << ".";
	}
};

public ref class MyUVWrapper
{
	xx::MemPool* mp;
	xx::UV* uv;
public:
	MyUVWrapper()
	{
		mp = new xx::MemPool();
		uv = mp->Create<xx::UV>();
		auto listener = uv->CreateListener<MyUVListener>(12345);
		auto timer = uv->CreateTimer<MyTimer>();
	}
	~MyUVWrapper()
	{
		mp->Release(uv);
		delete mp;
	}
	void Run()
	{
		uv->Run();
	}
};

int main(array<System::String ^> ^args)
{
	Console::WriteLine(L"CLR Echo Server");
	auto uv = gcnew MyUVWrapper();
	uv->Run();
	return 0;
}
