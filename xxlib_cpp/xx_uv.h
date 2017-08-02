#pragma once
#include <uv.h>
#include "xx_mempool.h"
#include "xx_bbqueue.h"
#include <assert.h>
#include <memory>
#include <functional>

namespace xx
{
	struct UV;
	struct UVListener;
	struct UVPeer;
	struct UVServerPeer;
	struct UVClientPeer;
	struct UVTimer;
	struct UVAsync;

	struct UV : MPObject											// 该类可能只能创建 1 份实例
	{
		List_v<UVListener*> listeners;
		List_v<UVClientPeer*> clientPeers;
		List_v<UVTimer*> timers;
		List_v<UVAsync*> asyncs;

		UV();
		~UV();
		int EnableIdle();
		void DisableIdle();
		void Run();
		void Stop();
		virtual void OnIdle();
		template<typename ListenerType, typename ...Args>
		ListenerType* CreateListener(int port, int backlog, Args &&... args);
		template<typename ClientPeerType, typename ...Args>
		ClientPeerType* CreateClientPeer(Args &&... args);
		template<typename TimerType, typename ...Args>
		TimerType* CreateTimer(Args &&... args);
		template<typename AsyncType, typename ...Args>
		AsyncType* CreateAsync(Args &&... args);

		// uv's
		uv_loop_t loop;
		uv_idle_t idler;
		static void IdleCB(uv_idle_t* handle);
	};

	struct UVListener : MPObject									// 当前为 ipv4, ip 为 0.0.0.0
	{
		UV* uv;
		uint32_t uv_listeners_index;
		List_v<UVServerPeer*> peers;
		virtual UVServerPeer* OnCreatePeer() = 0;					// 重写以提供创建具体 peer 类型的函数
		UVListener(UV* uv, int port, int backlog);
		~UVListener();

		// uv's
		uv_tcp_t tcpServer;
		static void OnConnect(uv_stream_t* server, int status);
	};

	enum class UVPeerStates
	{
		Disconnected,
		Connecting,
		Connected,
		Disconnecting,
		Closed
	};

	struct UVPeer : MPObject										// 一些基础数据结构
	{
		UVPeer();
		~UVPeer();

		UV* uv;
		BBuffer_v bbReceive;										// for ReadCB & OnReceive
		BBuffer_v bbReceiveLeft;									// 积攒 OnReceive 处理时剩下的数据
		BBuffer_v bbReceivePackage;									// for OnReceivePackage 传参, 引用 bbReceive 或 bbReceiveLeft 的内存

		BBQueue_v sendBufs;											// 待发送数据队列. 所有 Send 操作都是将数据压入这里, 再取适当长度的一段来发送
		List_v<uv_buf_t> writeBufs;									// 复用的 uv 写操作 多段数据参数

		bool sending = false;										// 发送操作标记. 当前设计中只同时发一段数据, 成功回调时才继续发下一段
		UVPeerStates state;											// 连接状态( server peer 初始为 Connected, client peer 为 Disconnected )

		virtual void OnReceive();									// 默认实现为读取包( 2 byte长度 + 数据 ), 并于凑齐完整包后 call OnReceivePackage
		virtual void OnReceivePackage(BBuffer& bb) = 0;				// OnReceive 凑齐一个包时将产生该调用
		virtual void OnDisconnect() = 0;							// 断开事件

		BBuffer* GetSendBB(int const& capacity = 0);				// 获取或创建一个发送用的 BBuffer( 里面可能已经有部分数据 ), 不要自己持有, 填完传给 Send( 不管是否断开 )
		int Send(BBuffer* const& bb);								// 将数据"移入"待发送队列, 可能立即发送, 立即返回是否成功( 0 表示成功 )( 失败原因可能是待发数据过多 )
		virtual int Disconnect(bool const& immediately = true);		// 断开( 接着会 Release ). immediately 为否就走 shutdown 模式( 延迟杀, 能尽可能确保数据发出去 )

		int SetNoDelay(bool const& enable);							// 开关 tcp 延迟发送以积攒数据的功能
		int SetKeepAlive(bool const& enable, uint32_t const& delay);// 设置 tcp 保持活跃的时长

		String_v tmpStr;
		String& GetPeerName();

		int Send();													// 内部函数, 开始发送 sendBufs 里的东西
		void Clear();												// 内部函数, 于断开之后清理收发相关缓存

		// 方便使用的一些扩展( 当前并不直接映射到 C# )
		List_v<MPObject*> recvPkgs;									// 可于 OnReceivePackage 时用 bb.ReadPackages(*recvPkgs) 来填充它. 须用 bb.ReleasePackages 释放.
		void ReleaseRecvPkgs();										// 主动回收 recvPkgs 的数据
	protected:
		template<typename T>
		int SendCore(T const& pkg);
		template<typename T, typename ...TS>
		int SendCore(T const& pkg, TS const& ... pkgs);
		template<typename T>
		void SendCombineCore(BBuffer& bb, T const& pkg);
		template<typename T, typename ...TS>
		void SendCombineCore(BBuffer& bb, T const& pkg, TS const& ... pkgs);
	public:
		template<typename ...TS>
		int SendPackages(TS const& ... pkgs);						// 语法糖, 等同于写多行的 Send 针对每个参数. 会发出 pkgs 个数个 [head] + [data]
		template<typename ...TS>
		int SendCombine(TS const& ... pkgs);						// 会在物理上将多个包合并成 1 个 [head] + [data] 中的 [data] 发出

		// uv's
		uv_tcp_t stream;
		uv_shutdown_t sreq;
		uv_write_t writer;

		static void AllocCB(uv_handle_t* handle, size_t suggested_size, uv_buf_t* buf);
		static void CloseCB(uv_handle_t* stream);
		static void ReadCB(uv_stream_t* handle, ssize_t nread, const uv_buf_t* buf);
		static void ShutdownCB(uv_shutdown_t* req, int status);
		static void SendCB(uv_write_t *req, int status);
	};

	struct UVServerPeer : UVPeer									// 当前为 ipv4, 未来考虑 v4 v6 同时支持
	{
		UVListener* listener;
		uint32_t listener_peers_index;

		UVServerPeer(UVListener* listener);
		~UVServerPeer();
	};

	struct UVClientPeer : UVPeer
	{
		uint32_t uv_clientPeers_index;
		int lastStatus = 0;											// 最后状态( 连接, 断开 )

		UVClientPeer(UV* uv);
		~UVClientPeer();											// 如果析构时 connected 为 true 则表示已断线

		int SetAddress(char const* ip, int port);
		int Connect();
		virtual void OnConnect() = 0;								// lastStatus 非 0 或 connected 为 false 表示没连上

		// uv's
		sockaddr_in tarAddr;
		uv_connect_t conn;
		static void ConnectCB(uv_connect_t* conn, int status);
	};

	struct UVTimer : MPObject
	{
		UV* uv;
		uint32_t uv_timers_index;

		UVTimer(UV* uv);
		~UVTimer();

		virtual void OnFire() = 0;
		int Start(uint64_t const& timeoutMS, uint64_t const& repeatIntervalMS);
		void SetRepeat(uint64_t const& repeatIntervalMS);
		int Again();
		int Stop();

		// uv's
		uv_timer_t timer_req;
		static void TimerCB(uv_timer_t* handle);
	};

	struct UVAsync : MPObject
	{
		UV* uv;
		uint32_t uv_asyncs_index;

		UVAsync(UV* uv);
		~UVAsync();

		void Fire();
		virtual void OnFire() = 0;

		// uv's
		uv_async_t async_req;
		static void AsyncCB(uv_async_t* handle);
	};

	using UV_v = MemHeaderBox<UV>;

	template<>
	struct BufMaker<uv_buf_t, void>
	{
		static uv_buf_t Make(char* buf, uint32_t len)
		{
			uv_buf_t rtv;
			rtv.base = buf;
			rtv.len = len;
			return rtv;
		}
	};

}

#include "xx_uv.hpp"
