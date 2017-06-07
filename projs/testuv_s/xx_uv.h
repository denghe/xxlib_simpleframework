#pragma once
#include <uv.h>
#include <xx_mempool.h>
#include <xx_bbqueue.h>
#include <assert.h>
#include <memory>
#include <iostream>

namespace xx
{
	struct UV;
	struct UVListener;
	struct UVServerPeer;
	struct UVClient;

	struct UV : xx::MPObject									// 该类只能创建 1 份实例
	{
		xx::List_v<UVListener*> listeners;
		xx::List_v<UVClient*> clients;
		UV();
		~UV();
		void Run();
		template<typename ListenerType> ListenerType* CreateListener(int port, int backlog = SOMAXCONN);
		template<typename ClientType> ClientType* CreateClient();
		// uv's
		uv_loop_t* loop;
	};

	struct UVListener : xx::MPObject							// 当前为 ipv4, ip 为 0.0.0.0
	{
		UV* uv;
		uint32_t uv_listeners_index;
		xx::List_v<UVServerPeer*> peers;
		virtual UVServerPeer* OnCreatePeer() = 0;				// 重写以提供创建具体 peer 类型的函数
		UVListener(UV* uv, int port, int backlog);
		~UVListener();
		// uv's
		uv_tcp_t tcpServer;
		static void OnConnect(uv_stream_t* server, int status);
	};

	struct UVServerPeer : xx::MPObject							// 当前为 ipv4, 未来考虑 v4 v6 同时支持
	{
		UV* uv;
		UVListener* listener;
		uint32_t listener_peers_index;

		UVServerPeer(UVListener* listener);
		~UVServerPeer();

		BBuffer_v bbReceive;									// for ReadCB & OnReceive
		BBuffer_v bbReceiveLeft;								// 积攒 OnReceive 处理时剩下的数据
		virtual void OnReceive();								// 默认实现为读取包( 2 byte长度 + 数据 ), 并于凑齐完整包后 call OnReceivePackage

		BBuffer_v bbReceivePackage;								// for OnReceivePackage 传参, 引用 bbReceive 或 bbReceiveLeft 的内存
		virtual void OnReceivePackage(BBuffer const& bb) = 0;	// OnReceive 凑齐一个包时将产生该调用

		// Send, Disconnect, virtual OnDisconnect
		uv_tcp_t stream;
		uv_shutdown_t sreq;

		uv_write_t writer;
		bool writing = false;									// 写操作标记. 当前设计中只允许同时写一段数据, 待到写成功回调时才继续写下一段
		xx::BBQueue_v sendBufs;									// 待发送数据队列. 所有 Send 操作都是将数据压入这里, 再取适当长度的一段来发送
		xx::List_v<uv_buf_t> writeBufs;							// 复用的 uv 写操作 多段数据参数
		void Write();
		void Kill();

		static void AllocCB(uv_handle_t* handle, size_t suggested_size, uv_buf_t* buf);
		static void CloseCB(uv_handle_t* stream);
		static void ReadCB(uv_stream_t* handle, ssize_t nread, const uv_buf_t* buf);
		static void ShutdownCB(uv_shutdown_t* req, int status);
		static void WriteCB(uv_write_t *req, int status);
	};

	struct UVClient : xx::MPObject
	{
		UV* uv;
		uint32_t uv_clients_index;
		UVClient(UV* uv);
		~UVClient();
		bool SetAddress(/* addr */);
		bool Connect();
		// bool Disconnect(/* timeout */);
		// Send, virtual OnReceive, virtual OnDisconnect
	};

	using UV_v = xx::MemHeaderBox<UV>;
}

#include "xx_uv.hpp"
