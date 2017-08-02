#pragma once
#include "xx_uv.h"
#include "xx_helpers.h"
#include "pkg\PKG_class.h"

struct MyClient;

struct MyConnector : xx::UVClientPeer
{
	MyClient* owner;
	MyConnector(xx::UV* uv, MyClient* owner);
	~MyConnector();
	virtual void OnReceivePackage(xx::BBuffer & bb) override;
	virtual void OnConnect() override;
	virtual void OnDisconnect() override;

	// 将所有收到的数据移进这里来放着以免 AI 来不及处理冲掉
	xx::Queue_v<xx::MPObject*> recvMsgs;
};


struct MyTimer : xx::UVTimer
{
	MyClient* owner;
	MyTimer(xx::UV* uv, MyClient* owner);
	virtual void OnFire() override;
};


struct MyClient : xx::MPObject
{
	xx::UV* uv;
	MyConnector* conn;
	MyTimer* timer;
	MyClient(xx::UV* uv, char const* un, char const* pw);
	~MyClient();

	// 预创建方便发包的容器
	PKG::Client_Server::Join* pkgJoin;
	PKG::Client_Server::Message* pkgMessage;

	// 运行时上下文
	xx::MPtr<PKG::UserInfo> self;
	xx::List<PKG::UserInfo*>* users = nullptr;

	int lineNumber = 0;					    // stackless 协程行号
	int Update();

	int64_t lastMS = 0;						// 用于各种超时判断
	bool connecting = false;				// 产生 OnConnect 回调的标志
};
