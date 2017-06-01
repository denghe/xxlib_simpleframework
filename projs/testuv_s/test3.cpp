#include <uv.h>
#include <xx_mempool.h>
#include <assert.h>
#include <memory>
#include <iostream>

struct UV;
struct UVListener;
struct UVServerPeer;
struct UVClient;



struct UV : xx::MPObject							// 该类只能创建 1 份实例
{
	uv_loop_t* loop;
	xx::List_v<UVListener*> listeners;
	xx::List_v<UVClient*> clients;
	UV();
	~UV();
	UVListener* CreateListener(int backlog, int port);
	UVClient* CreateClient();
};

struct UVListener : xx::MPObject					// 当前为 ipv4, ip 为 0.0.0.0. 未来考虑 v4 v6 同时双开
{
	UV* uv;
	uint32_t uv_listeners_index;
	xx::List_v<UVServerPeer*> peers;
	xx::List_v<UVServerPeer*> disconnectingPeers;	// 注册 timer 并丢到这里以延迟杀掉
	virtual UVServerPeer* OnAccept() = 0;
	// todo: virtual OnAccept 用来创建具体的 server peer 类型
	UVListener(UV* uv, int backlog, int port);
	~UVListener();
};

struct UVServerPeer : xx::MPObject					// 当前为 ipv4, 未来考虑 v4 v6 同时支持
{
	UV* uv;
	UVListener* listener;
	uint32_t listener_peers_index;
	UVServerPeer(UVListener* listener);
	~UVServerPeer();
	// Send, virtual OnReceive, virtual OnDisconnect
	bool Disconnect(/* timeout */);	// 注册 timer 并丢到 listener->disconnectingPeers 以延迟杀掉
};

struct UVClient : xx::MPObject
{
	UV* uv;
	uint32_t uv_clients_index;
	UVClient(UV* uv);
	~UVClient();
	bool SetAddress(/* addr */);
	bool Connect();
	bool Disconnect(/* timeout */);
	// Send, virtual OnReceive, virtual OnDisconnect
};

using UV_v = xx::MemHeaderBox<UV>;





inline UV::UV()
	: listeners(mempool())
	, clients(mempool())
{
	loop = uv_default_loop();
}
inline UV::~UV()
{
	// todo release container items
}
inline UVListener* UV::CreateListener(int backlog, int port)
{
	auto o = mempool().Create<UVListener>(this, backlog, port);
	if (o) listeners->Add(o);
	return o;
}
inline UVClient* UV::CreateClient()
{
	auto o = mempool().Create<UVClient>(this);
	if (o) clients->Add(o);
	return o;
}


inline UVListener::UVListener(UV* uv, int backlog, int port)
	: uv(uv)
	, uv_listeners_index(uv->listeners->dataLen)
	, peers(mempool())
	, disconnectingPeers(mempool())
{
	// todo: init, throw err
}
inline UVListener::~UVListener()
{
	// todo release
	XX_LIST_SWAP_REMOVE(uv->listeners, this, uv_listeners_index);
}


inline UVServerPeer::UVServerPeer(UVListener* listener)
	: uv(listener->uv)
	, listener(listener)
	, listener_peers_index(listener->peers->dataLen)
{
	// todo: init, throw err
}
inline UVServerPeer::~UVServerPeer()
{
	// todo release
	XX_LIST_SWAP_REMOVE(listener->peers, this, listener_peers_index);
}


inline UVClient::UVClient(UV* uv)
{

}
inline UVClient::~UVClient()
{

}
bool UVClient::SetAddress(/*, addr */)
{
	return false;
}
bool UVClient::Connect()
{

}




int main()
{
	{
		xx::MemPool mp;
		UV_v uv(mp);
	}
	return 0;
}
