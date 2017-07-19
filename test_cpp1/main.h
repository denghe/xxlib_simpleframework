#pragma once
#include "xx_uv.h"
#include "xx_helpers.h"

struct MyClient;

struct MyAsync : xx::UVAsync
{
	MyClient* owner;
	MyAsync(xx::UV* uv, MyClient* owner);
	virtual void OnFire() override;
};


struct MyConnector : xx::UVClientPeer
{
	MyClient* owner;
	MyConnector(xx::UV* uv, MyClient* owner);
	virtual void OnReceivePackage(xx::BBuffer & bb) override;
	virtual void OnConnect() override;
};


struct MyTimer : xx::UVTimer
{
	MyClient* owner;
	MyTimer(xx::UV* uv, MyClient* owner);
	virtual void OnFire() override;

	int lineNumber = 0;
	int Update();
};


struct MyClient : xx::MPObject
{
	xx::UV* uv;
	xx::MemHeaderBox<MyConnector> conn;
	xx::MemHeaderBox<MyTimer> timer;
	xx::MemHeaderBox<MyAsync> dispacher;
	MyClient(xx::UV* uv);
};
