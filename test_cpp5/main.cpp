#include "xx_uv.h"

struct Listener;

struct Peer : xx::UVServerPeer
{
	Peer(Listener* listener);
	//virtual void OnReceive() override;
	virtual void OnReceivePackage(xx::BBuffer& bb) override;
	virtual void OnDisconnect() override;
};
struct Listener : xx::UVListener
{
	Listener(xx::UV* uv, int port, int backlog);
	virtual xx::UVServerPeer* OnCreatePeer() override;
};


inline xx::UVServerPeer* Listener::OnCreatePeer()
{
	return mempool().Create<Peer>(this);
}
inline Listener::Listener(xx::UV* uv, int port, int backlog)
	: xx::UVListener(uv, port, backlog)
{
}


inline Peer::Peer(Listener* listener)
	: xx::UVServerPeer(listener)
{
	Cout(GetPeerName(), " connected!\n");
}
//inline void Peer::OnReceive()
//{
//	Cout(tmpStr, " recv = ", bbReceive, "\n");
//}
inline void Peer::OnReceivePackage(xx::BBuffer& bb)
{
	// echo
	auto sendBB = GetSendBB();
	sendBB->WritePackageLength((uint16_t)bb.dataLen);
	sendBB->WriteBuf(bb);
	Send(sendBB);
	Cout(tmpStr, " recvPkg = ", bb, "\n");
}
inline void Peer::OnDisconnect()
{
	Cout(tmpStr, " disconnected!\n");
}



int main()
{
	xx::MemPool mp;
	xx::UV_v uv(mp);
	int port = 12345;
	xx::Ptr<Listener> listener = uv->CreateListener<Listener>(port, 128);
	if (!listener) return -1;
	mp.Cout("listener running... port = ", port, "\n");
	uv->Run();
	return 0;
}
