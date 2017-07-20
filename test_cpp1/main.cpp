#include "main.h"
#include <thread>
#include <chrono>

/*************************************************************************************/

int main()
{
	PKG::AllTypesRegister();

	xx::MemPool mp;
	xx::UV_v uv(mp);
	auto c1 = mp.CreateMPtr<MyClient>(uv, "a", "a");	// 自持有, 自杀
	auto c2 = mp.CreateMPtr<MyClient>(uv, "b", "b");	// 自持有, 自杀
	uv->Run();
	std::cout << "main: press any key to continue ..." << std::endl;
	std::cin.get();
	return 0;
}


/*************************************************************************************/

inline MyConnector::MyConnector(xx::UV* uv, MyClient* owner)
	: xx::UVClientPeer(uv)
	, owner(owner)
	, recvMsgs(mempool())
{}
inline MyConnector::~MyConnector()
{
	Cout(owner->pkgJoin->username, " MyConnector: ~MyConnector()\n");
}
inline void MyConnector::OnReceivePackage(xx::BBuffer & bb)
{
	// 试将收到的包解出来
	if (bb.ReadPackages(*recvPkgs) != 1)
	{
		Disconnect(true);
		return;
	}

	// 将数据转移并追加到持续收到的数据队列中
	for (uint32_t i = 0; i < recvPkgs->dataLen; ++i)
	{
		recvMsgs->Push(recvPkgs->At(i));
	}
	recvPkgs->Clear();
}
inline void MyConnector::OnConnect()
{
	Cout(owner->pkgJoin->username, " MyConnector: ", (state == xx::UVPeerStates::Connected
		? "state == xx::UVPeerStates::Connected\n"
		: "state != xx::UVPeerStates::Connected\n"));
	owner->connecting = false;
}


/*************************************************************************************/

inline MyTimer::MyTimer(xx::UV* uv, MyClient* owner)
	: xx::UVTimer(uv)
	, owner(owner)
{
	Start(0, 1);
}
inline void MyTimer::OnFire()
{
	if (owner->Update()) owner->Release();
}


/*************************************************************************************/

inline MyClient::MyClient(xx::UV* uv, char const* un, char const* pw)
	: uv(uv)
{
	conn = uv->CreateClientPeer<MyConnector>(this);
	timer = uv->CreateTimer<MyTimer>(this);

	// 预构造待发数据包
	mempool().CreateTo(pkgJoin);
	mempool().CreateTo(pkgJoin->username);
	mempool().CreateTo(pkgJoin->password);
	pkgJoin->username->Assign(un);
	pkgJoin->password->Assign(pw);

	mempool().CreateTo(pkgMessage);
	mempool().CreateTo(pkgMessage->text);

	Cout("MyClient::MyClient(uv, ", un, ", ", pw, ")\n");
}

inline MyClient::~MyClient()
{
	Cout(pkgJoin->username, " MyClient:~MyClient()\n");
	auto& mp = mempool();
	mp.SafeRelease(timer);
	mp.SafeRelease(conn);

	mp.SafeRelease(users);

	mp.SafeRelease(pkgJoin);
	mp.SafeRelease(pkgMessage);
}

inline int MyClient::Update()
{
	auto currMS = xx::GetCurrentMS();
	XX_CORO_BEGIN();
	{
		conn->SetAddress("127.0.0.1", 12345);
		connecting = true;
		if (auto rtv = conn->Connect())
		{
			Cout(pkgJoin->username, " MyClient: conn->Connect() error! rtv = ", rtv, '\n');
			return -1;
		}

		lastMS = currMS;		// 重置计时器为下一环节超时判断作准备
	}
	XX_CORO_(1);
	{
		if (!connecting)
		{
			XX_CORO_GOTO(2);
		}
		if (currMS - lastMS < 5000 && conn->state != xx::UVPeerStates::Connected)
		{
			XX_CORO_YIELDTO(1);
		}
		else
		{
			Cout(pkgJoin->username, " MyClient: connect to server timeout!\n");
			return -1;
		}
	}
	XX_CORO_(2);
	{
		if (conn->state != xx::UVPeerStates::Connected)
		{
			Cout(pkgJoin->username, " MyClient: can't connect to server!\n");
			return -1;
		}
	}
	XX_CORO_(3);
	{
		Cout(pkgJoin->username, " MyClient: connected!\n");

		// un & pw 在构造函数中填过了
		//pkgJoin->username->Assign("a");
		//pkgJoin->password->Assign("a");

		if (auto rtv = conn->SendPackages(pkgJoin))
		{
			Cout(pkgJoin->username, " MyClient: conn->SendPackages(pkgJoin) error! rtv = ", rtv, '\n');
			return -1;
		}

		lastMS = currMS;		// 重置计时器为下一环节超时判断作准备
	}
	XX_CORO_(4);
	{
		if (conn->state != xx::UVPeerStates::Connected)
		{
			Cout(pkgJoin->username, " MyClient: wait conn Recv disconnected!\n");
			return -1;
		}
		else if (!conn->recvMsgs->Empty())
		{
			XX_CORO_GOTO(5);
		}
		if (currMS - lastMS < 5000)
		{
			XX_CORO_YIELDTO(4);
		}
		else
		{
			Cout(pkgJoin->username, " MyClient: wait recv timeout!\n");
			return -1;
		}
	}
	XX_CORO_(5);
	{
		assert(!conn->recvMsgs->Empty());

		// 取出第 1 个包处理. 必然是 JoinSuccess 或 JoinFail
		auto o_ = conn->recvMsgs->Top();
		xx::ScopeGuard sg_o_killer([&] { o_->Release(); });		// 跳出这层大扩号就删
		conn->recvMsgs->Pop();

		auto& typeId = o_->typeId();
		switch (typeId)
		{
		case xx::TypeId_v<PKG::Server_Client::JoinSuccess>:
		{
			auto o = (PKG::Server_Client::JoinSuccess*)o_;

			// 转移包成员继续用
			self = o->self;			// 转为 MPtr
			users = o->users;		// 作为持有容器, 所有包引用计数都为1, 刚好
			o->self = nullptr;		// 避免析构到移走的数据
			o->users = nullptr;		// 避免析构到移走的数据

			// 显示点收到的内容
			Cout(pkgJoin->username, " MyClient: recv msg == PKG::Server_Client::JoinSuccess!\n"
				"users->dataLen = ", users->dataLen, "\n"
				"self->id = ", self->id, '\n');

			lastMS = currMS;		// 重置计时器为下一环节超时判断作准备
			XX_CORO_GOTO(6);
		}
		case xx::TypeId_v<PKG::Server_Client::JoinFail>:
		{
			auto o = (PKG::Server_Client::JoinFail*)o_;
			Cout(pkgJoin->username, " MyClient: recv msg == PKG::Server_Client::JoinFail!\n    reason = ", o->reason, '\n');
			return -1;
		}
		default:
		{
			Cout(pkgJoin->username, " MyClient: recv unhandled msg!!! typeId = ", typeId, '\n');
			return -1;
		}
		}
	}
	XX_CORO_(6);
	{
		if (conn->state != xx::UVPeerStates::Connected)
		{
			Cout(pkgJoin->username, " MyClient: wait conn Recv disconnected! ( joined )\n");
			return -1;
		}

		// 继续处理后续收到的包
		xx::MPObject* o_;
		if (conn->recvMsgs->TryPop(o_))
		{
			xx::ScopeGuard sg_o_killer([&] { o_->Release(); });		// 跳出这层大扩号就删
			auto& typeId = o_->typeId();
			switch (typeId)
			{
			case xx::TypeId_v<PKG::Server_Client::PushJoin>:
			{
				auto o = (PKG::Server_Client::PushJoin*)o_;
				Cout(pkgJoin->username, " MyClient: recv msg == PKG::Server_Client::PushJoin! id = ", o->id, '\n');
				break;
			}
			case xx::TypeId_v<PKG::Server_Client::PushLogout>:
			{
				auto o = (PKG::Server_Client::PushLogout*)o_;
				Cout(pkgJoin->username, " MyClient: recv msg == PKG::Server_Client::PushLogout! id & reason = ", o->id, ' ', o->reason, '\n');
				break;
			}
			case xx::TypeId_v<PKG::Server_Client::PushMessage>:
			{
				auto o = (PKG::Server_Client::PushMessage*)o_;
				Cout(pkgJoin->username, " MyClient: recv msg == PKG::Server_Client::PushMessage! id & text = ", o->id, ' ', o->text, '\n');
				break;
			}
			default:
			{
				Cout(pkgJoin->username, " MyClient: recv unhandled msg! typeId = ", typeId, '\n');
				return -1;
			}
			}
		}

		// todo: 通过另外一个线程, 从 Console 接收字符以产生 PKG::Client_Server::Message 消息

		// 每秒钟发一次当前 ms
		if (currMS - lastMS > 1000)
		{
			lastMS = currMS;

			// 先模拟一下键盘输入
			pkgMessage->text->Clear();
			pkgMessage->text->Append("currMS = ", currMS);

			// 发 Message
			if (auto rtv = conn->SendPackages(pkgMessage))
			{
				Cout(pkgJoin->username, " MyClient: conn->SendPackages(pkgMessage) error! rtv = ", rtv, '\n');
				return -1;
			}
		}

		// 继续本状态循环
		XX_CORO_YIELDTO(6);
	}
	XX_CORO_END();
	return -1;
}

