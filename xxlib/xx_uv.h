#pragma once
#include "xx.h"
#include <mutex>

namespace xx
{
	class UvLoop;
	class UvListenerBase;
	class UvTcpListener;
	class UvTcpUdpBase;
	class UvTcpBase;
	class UvTcpPeer;
	class UvTcpClient;
	class UvTimer;
	class UvTimeouterBase;
	class UvTimeouter;
	class UvAsync;
	class UvRpcManager;
	class UvTimeouter;
	class UvContextBase;
	class UvUdpListener;
	class UvUdpBase;
	class UvUdpPeer;
	class UvUdpClient;

	enum class UvTcpStates
	{
		Disconnected,
		Connecting,
		Connected,
		Disconnecting,
	};

	enum class UvRunMode
	{
		Default,
		Once,
		NoWait
	};

	class UvLoop : public Object
	{
	public:
		void* ptr = nullptr;
		List<UvTcpListener*> tcpListeners;
		List<UvTcpClient*> tcpClients;
		List<UvUdpListener*> udpListeners;
		List<UvUdpClient*> udpClients;
		List<UvTimer*> timers;
		List<UvAsync*> asyncs;
		UvTimeouter* timeouter = nullptr;
		UvRpcManager* rpcMgr = nullptr;
		UvTimer* udpTimer = nullptr;
		uint32_t udpTicks = 0;
		std::array<char,65536> udpRecvBuf;
		uint32_t kcpInterval = 0;

		explicit UvLoop(MemPool* mp);
		~UvLoop();

		void InitTimeouter(uint64_t intervalMS = 1000, int wheelLen = 6, int defaultInterval = 5);
		void InitRpcManager(uint64_t rpcIntervalMS = 1000, int rpcDefaultInterval = 5);
		void InitKcpFlushInterval(uint32_t interval = 10);

		void Run(UvRunMode mode = UvRunMode::Default);
		void Stop();
		bool alive() const;

		UvTcpListener* CreateTcpListener();
		UvTcpClient* CreateTcpClient();
		UvUdpListener* CreateUdpListener();
		UvUdpClient* CreateUdpClient();
		UvTimer* CreateTimer(uint64_t timeoutMS, uint64_t repeatIntervalMS, std::function<void()>&& OnFire = nullptr);
		UvAsync* CreateAsync();
	};

	class UvListenerBase : public Object
	{
	public:
		std::function<void()> OnDispose;

		UvLoop& loop;
		size_t index_at_container = -1;

		void* ptr = nullptr;
		void* addrPtr = nullptr;

		UvListenerBase(UvLoop& loop);
	};

	class UvTcpListener : public UvListenerBase
	{
	public:
		std::function<UvTcpPeer*()> OnCreatePeer;
		std::function<void(UvTcpPeer*)> OnAccept;
		List<UvTcpPeer*> peers;

		UvTcpListener(UvLoop& loop);
		~UvTcpListener();

		static void OnAcceptCB(void* server, int status);
		void Bind(char const* ipv4, int port);
		void Listen(int backlog = 128);
	};

	class UvTimeouterBase : public Object
	{
	public:
		UvTimeouterBase(MemPool* mp);
		~UvTimeouterBase();
		UvTimeouter* timerManager = nullptr;
		UvTimeouterBase* timerPrev = nullptr;
		UvTimeouterBase* timerNext = nullptr;
		int timerIndex = -1;
		std::function<void()> OnTimeout;

		void TimerClear();
		void TimeoutReset(int interval = 0);
		void TimerStop();
		void BindTimeouter(UvTimeouter* tm);
		void UnbindTimerManager();
		bool timering();
	};

	class UvTcpUdpBase : public UvTimeouterBase
	{
	public:
		std::function<void(BBuffer&)> OnReceivePackage;

		// uint32_t: 流水号
		std::function<void(uint32_t, BBuffer&)> OnReceiveRequest;



		// 由于路由服务需要保持 routingAddress 为空, 在收到 Routing 包时, 可抛出 OnReceiveRouting, 以便进一步用相应的 client 转发
		// 如果 routingAddress 非空, 在收到 Routing 包时, 可抛出 OnReceivePackage, OnReceiveRequest 或 触发 RPC 回调.
		// 不同的是, senderAddress 将被填充返回地址. 故可根据它来判断 接收事件 属于何种数据来源.
		xx::String routingAddress;
		xx::String senderAddress;

		// (xx::BBuffer& bb, size_t pkgOffset, size_t pkgLen, size_t addrOffset, size_t addrLen)
		// 4个 size_t 代表 包起始offset, 含包头的总包长, 地址起始偏移, 地址长度( 方便替换地址并 memcpy )
		// BBuffer 的 offset 停在地址后方( 流水号或数据起始位置 )
		std::function<void(BBuffer&, size_t, size_t, size_t, size_t)> OnReceiveRouting;


		std::function<void()> OnDispose;


		UvLoop& loop;
		size_t index_at_container = -1;

		void* ptr = nullptr;
		void* addrPtr = nullptr;

		BBuffer bbRecv;
		BBuffer bbSend;

		UvTcpUdpBase(UvLoop& loop);

		virtual void DisconnectImpl() = 0;
		virtual bool Disconnected() = 0;
		virtual size_t GetSendQueueSize() = 0;
		virtual void SendBytes(char const* inBuf, int len = 0) = 0;

		virtual void ReceiveImpl(char const* bufPtr, int len);

		void SendBytes(BBuffer& bb);

		template<typename T>
		void Send(T const& pkg);

		template<typename T>
		uint32_t SendRequest(T const& pkg, std::function<void(uint32_t, BBuffer*)>&& cb, int interval = 0);

		template<typename T>
		void SendResponse(uint32_t serial, T const& pkg);

		// 转发的相关重载, Send 配套函数
		template<typename T>
		void SendRouting(xx::String& rAddr, T const& pkg);

		template<typename T>
		uint32_t SendRoutingRequest(xx::String& rAddr, T const& pkg, std::function<void(uint32_t, BBuffer*)>&& cb, int interval = 0);

		template<typename T>
		void SendRoutingResponse(xx::String& rAddr, uint32_t serial, T const& pkg);


		// 上面 6 个的 Big 版

		template<typename T>
		void SendBig(T const& pkg);

		template<typename T>
		uint32_t SendBigRequest(T const& pkg, std::function<void(uint32_t, BBuffer*)> cb, int interval = 0);

		template<typename T>
		void SendBigResponse(uint32_t serial, T const& pkg);

		// 转发的相关重载, Send 配套函数
		template<typename T>
		void SendBigRouting(xx::String& rAddr, T const& pkg);

		template<typename T>
		uint32_t SendBigRoutingRequest(xx::String& rAddr, T const& pkg, std::function<void(uint32_t, BBuffer*)> cb, int interval = 0);

		template<typename T>
		void SendBigRoutingResponse(xx::String& rAddr, uint32_t serial, T const& pkg);
	};

	class UvTcpBase : public UvTcpUdpBase
	{
	public:
		UvTcpBase(UvLoop& loop);

		size_t GetSendQueueSize() override;
		void SendBytes(char const* inBuf, int len = 0) override;

		static void OnReadCBImpl(void* stream, ptrdiff_t nread, const void* buf_t);
	};

	class UvTcpPeer : public UvTcpBase
	{
	public:
		// bool alive 只能外部检测野指针

		UvTcpListener & listener;
		UvTcpPeer(UvTcpListener& listener);
		~UvTcpPeer();	// Release 取代 Dispose
		void DisconnectImpl() override;
		bool Disconnected() override;
		std::array<char, 64> ipBuf;
		char* ip();
	};

	class UvTcpClient : public UvTcpBase
	{
	public:
		std::function<void(int)> OnConnect;
		std::function<void()> OnDisconnect;

		UvTcpStates state = UvTcpStates::Disconnected;
		bool alive() const;	// state == UvTcpStates::Connected, 并不能取代外部野指针检测
		UvTcpClient(UvLoop& loop);
		~UvTcpClient();	// Release 取代 Dispose
		void SetAddress(char const* const& ipv4, int port);
		static void OnConnectCBImpl(void* req, int status);
		void Connect();
		void Disconnect();
		void DisconnectImpl() override;
		bool Disconnected() override;
	};

	class UvTimer : public Object
	{
	public:
		std::function<void()> OnFire;
		std::function<void()> OnDispose;

		UvLoop& loop;
		size_t index_at_container = -1;
		void* ptr = nullptr;
		UvTimer(UvLoop& loop, uint64_t timeoutMS, uint64_t repeatIntervalMS, std::function<void()>&& OnFire = nullptr);
		~UvTimer();
		static void OnTimerCBImpl(void* handle);
		void SetRepeat(uint64_t repeatIntervalMS);
		void Again();
		void Stop();
	};

	class UvTimeouter : public Object
	{
	public:
		UvTimer* timer = nullptr;
		List<UvTimeouterBase*> timerss;
		int cursor = 0;
		int defaultInterval;
		UvTimeouter(UvLoop& loop, uint64_t intervalMS, int wheelLen, int defaultInterval);
		~UvTimeouter();
		void Process();
		void Clear();
		void Add(UvTimeouterBase* t, int interval = 0);
		void Remove(UvTimeouterBase* t);
		void AddOrUpdate(UvTimeouterBase* t, int interval = 0);
	};

	class UvAsync : public Object
	{
	public:
		std::function<void()> OnFire;
		std::function<void()> OnDispose;

		UvLoop& loop;
		size_t index_at_container = -1;
		std::mutex mtx;
		Queue<std::function<void()>> actions;
		void* ptr = nullptr;
		UvAsync(UvLoop& loop);
		~UvAsync();
		static void OnAsyncCBImpl(void* handle);
		void Dispatch(std::function<void()>&& a);
		void OnFireImpl();
	};

	class UvRpcManager : public Object
	{
	public:
		UvTimer* timer = nullptr;
		uint32_t serial = 0;
		Dict<uint32_t, std::function<void(uint32_t, BBuffer*)>> mapping;
		Queue<std::pair<int, uint32_t>> serials;
		int defaultInterval = 0;
		int ticks = 0;
		size_t Count();
		UvRpcManager(UvLoop& loop, uint64_t intervalMS, int defaultInterval);
		~UvRpcManager();
		void Process();
		uint32_t Register(std::function<void(uint32_t, BBuffer*)>&& cb, int interval = 0);
		void Unregister(uint32_t serial);
		void Callback(uint32_t serial, BBuffer* bb);
	};

	class UvContextBase : public Object
	{
	public:
		UvTcpUdpBase* peer = nullptr;

		UvContextBase(MemPool* mp);
		~UvContextBase();
		bool PeerAlive();
		bool BindPeer(UvTcpUdpBase* p);
		void KickPeer(bool immediately = true);
		void OnPeerReceiveRequest(uint32_t serial, BBuffer& bb);
		void OnPeerReceivePackage(BBuffer& bb);
		void OnPeerDisconnect();
		virtual void HandleRequest(uint32_t serial, Ptr<Object>& o) = 0;
		virtual void HandlePackage(Ptr<Object>& o) = 0;
		virtual void HandleDisconnect() = 0;
	};

	class UvUdpListener : public UvListenerBase
	{
	public:
		std::function<UvUdpPeer*(Guid const&)> OnCreatePeer;
		std::function<void(UvUdpPeer*)> OnAccept;
		Dict<Guid, UvUdpPeer*> peers;

		UvUdpListener(UvLoop& loop);
		~UvUdpListener();
		static void OnRecvCBImpl(void* udp, ptrdiff_t nread, void* buf_t, void* addr, uint32_t flags);
		void OnReceiveImpl(char const* bufPtr, int len, void* addr);

		void Bind(char const* ipv4, int port);
		void Listen();
		void StopListen();

		UvUdpPeer* CreatePeer(Guid const& g
			, int sndwnd = 128, int rcvwnd = 128
			, int nodelay = 1/*, int interval = 10*/, int resend = 2, int nc = 1, int minrto = 100);
	};

	class UvUdpBase : public UvTcpUdpBase
	{
	public:
		Guid guid;
		uint32_t nextUpdateTicks = 0;
		UvUdpBase(UvLoop& loop);
	};

	class UvUdpPeer : public UvUdpBase
	{
	public:
		UvUdpListener& listener;

		UvUdpPeer(UvUdpListener& listener
			, Guid const& g = Guid()
			, int sndwnd = 128, int rcvwnd = 128
			, int nodelay = 1/*, int interval = 10*/, int resend = 2, int nc = 1, int minrto = 100);
		~UvUdpPeer();

		static int OutputImpl(char const* buf, int len, void* kcp);
		void Update(uint32_t current);
		void Input(char const* data, int len);
		void SendBytes(char const* data, int len = 0) override;
		void DisconnectImpl() override;
		size_t GetSendQueueSize() override;
		bool Disconnected() override;

		std::array<char, 64> ipBuf;
		char* ip();
	};

	class UvUdpClient : public UvUdpBase
	{
	public:
		void* kcpPtr = nullptr;
		UvUdpClient(UvLoop& loop);
		~UvUdpClient();

		void Connect(Guid const& guid
			, int sndwnd = 128, int rcvwnd = 128
			, int nodelay = 1/*, int interval = 10*/, int resend = 2, int nc = 1, int minrto = 100);

		static void OnRecvCBImpl(void* udp, ptrdiff_t nread, void* buf_t, void* addr, uint32_t flags);
		void OnReceiveImpl(char const* bufPtr, int len, void* addr);
		static int OutputImpl(char const* buf, int len, void* kcp);
		void Update(uint32_t current);
		void Disconnect();
		void SetAddress(char const* ipv4, int port);
		void SendBytes(char const* data, int len = 0) override;
		void DisconnectImpl() override;
		bool Disconnected() override;
		size_t GetSendQueueSize() override;
	};




	template<typename T>
	inline void UvTcpUdpBase::Send(T const & pkg)
	{
		if (!ptr) throw - 1;
		bbSend.Clear();
		bbSend.BeginWritePackage();
		bbSend.WriteRoot(pkg);
		bbSend.EndWritePackage();
		SendBytes(bbSend);
	}

	template<typename T>
	inline uint32_t UvTcpUdpBase::SendRequest(T const & pkg, std::function<void(uint32_t, BBuffer*)>&& cb, int interval)
	{
		if (!ptr) throw - 1;
		auto serial = loop.rpcMgr->Register(std::move(cb), interval);
		bbSend.Clear();
		bbSend.BeginWritePackage(1, serial);
		bbSend.WriteRoot(pkg);
		bbSend.EndWritePackage();
		SendBytes(bbSend);
		return serial;
	}

	template<typename T>
	inline void UvTcpUdpBase::SendResponse(uint32_t serial, T const & pkg)
	{
		bbSend.Clear();
		bbSend.BeginWritePackage(2, serial);
		bbSend.WriteRoot(pkg);
		bbSend.EndWritePackage();
		SendBytes(bbSend);
	}

	template<typename T>
	inline void UvTcpUdpBase::SendRouting(xx::String& rAddr, T const & pkg)
	{
		if (!ptr) throw - 1;
		bbSend.Clear();
		bbSend.BeginWritePackage();	// todo
		bbSend.WriteRoot(pkg);
		bbSend.EndWritePackage();	// todo
		SendBytes(bbSend);
	}

	template<typename T>
	inline uint32_t UvTcpUdpBase::SendRoutingRequest(xx::String& rAddr, T const & pkg, std::function<void(uint32_t, BBuffer*)>&& cb, int interval)
	{
		if (!ptr) throw - 1;
		auto serial = loop.rpcMgr->Register(std::move(cb), interval);
		bbSend.Clear();
		bbSend.BeginWritePackage(1, serial);	// todo
		bbSend.WriteRoot(pkg);
		bbSend.EndWritePackage();	// todo
		SendBytes(bbSend);
		return serial;
	}

	template<typename T>
	inline void UvTcpUdpBase::SendRoutingResponse(xx::String& rAddr, uint32_t serial, T const & pkg)
	{
		bbSend.Clear();
		bbSend.BeginWritePackage(2, serial);	// todo
		bbSend.WriteRoot(pkg);
		bbSend.EndWritePackage();	// todo
		SendBytes(bbSend);
	}




	template<typename T>
	inline void UvTcpUdpBase::SendBig(T const & pkg)
	{
		if (!ptr) throw - 1;
		bbSend.Clear();
		bbSend.BeginWritePackage();	// todo
		bbSend.WriteRoot(pkg);
		bbSend.EndWritePackage();	// todo
		SendBytes(bbSend);
	}

	template<typename T>
	inline uint32_t UvTcpUdpBase::SendBigRequest(T const & pkg, std::function<void(uint32_t, BBuffer*)> cb, int interval)
	{
		if (!ptr) throw - 1;
		auto serial = loop.rpcMgr->Register(cb, interval);
		bbSend.Clear();
		bbSend.BeginWritePackage(1, serial);	// todo
		bbSend.WriteRoot(pkg);
		bbSend.EndWritePackage();	// todo
		SendBytes(bbSend);
		return serial;
	}

	template<typename T>
	inline void UvTcpUdpBase::SendBigResponse(uint32_t serial, T const & pkg)
	{
		template<typename T>
		bbSend.Clear();
		bbSend.BeginWritePackage(2, serial);	// todo
		bbSend.WriteRoot(pkg);
		bbSend.EndWritePackage();	// todo
		SendBytes(bbSend);
	}

	template<typename T>
	inline void UvTcpUdpBase::SendBigRouting(xx::String& rAddr, T const & pkg)
	{
		if (!ptr) throw - 1;
		bbSend.Clear();
		bbSend.BeginWritePackage();	// todo
		bbSend.WriteRoot(pkg);
		bbSend.EndWritePackage();	// todo
		SendBytes(bbSend);
	}

	template<typename T>
	inline uint32_t UvTcpUdpBase::SendBigRoutingRequest(xx::String& rAddr, T const & pkg, std::function<void(uint32_t, BBuffer*)> cb, int interval)
	{
		if (!ptr) throw - 1;
		auto serial = loop.rpcMgr->Register(cb, interval);
		bbSend.Clear();
		bbSend.BeginWritePackage(1, serial);	// todo
		bbSend.WriteRoot(pkg);
		bbSend.EndWritePackage();	// todo
		SendBytes(bbSend);
		return serial;
	}

	template<typename T>
	inline void UvTcpUdpBase::SendBigRoutingResponse(xx::String& rAddr, uint32_t serial, T const & pkg)
	{
		template<typename T>
		bbSend.Clear();
		bbSend.BeginWritePackage(2, serial);	// todo
		bbSend.WriteRoot(pkg);
		bbSend.EndWritePackage();	// todo
		SendBytes(bbSend);
	}




	// 万一会用到的一些声明
	using UvLoop_p = Ptr<UvLoop>;
	using UvListenerBase_p = Ptr<UvListenerBase>;
	using UvTcpListener_p = Ptr<UvTcpListener>;
	using UvTcpUdpBase_p = Ptr<UvTcpUdpBase>;
	using UvTcpBase_p = Ptr<UvTcpBase>;
	using UvTcpPeer_p = Ptr<UvTcpPeer>;
	using UvTcpClient_p = Ptr<UvTcpClient>;
	using UvTimer_p = Ptr<UvTimer>;
	using UvTimeouterBase_p = Ptr<UvTimeouterBase>;
	using UvTimeouter_p = Ptr<UvTimeouter>;
	using UvAsync_p = Ptr<UvAsync>;
	using UvRpcManager_p = Ptr<UvRpcManager>;
	using UvTimeouter_p = Ptr<UvTimeouter>;
	using UvContextBase_p = Ptr<UvContextBase>;
	using UvUdpListener_p = Ptr<UvUdpListener>;
	using UvUdpBase_p = Ptr<UvUdpBase>;
	using UvUdpPeer_p = Ptr<UvUdpPeer>;
	using UvUdpClient_p = Ptr<UvUdpClient>;

	using UvLoop_r = Ref<UvLoop>;
	using UvListenerBase_r = Ref<UvListenerBase>;
	using UvTcpListener_r = Ref<UvTcpListener>;
	using UvTcpUdpBase_r = Ref<UvTcpUdpBase>;
	using UvTcpBase_r = Ref<UvTcpBase>;
	using UvTcpPeer_r = Ref<UvTcpPeer>;
	using UvTcpClient_r = Ref<UvTcpClient>;
	using UvTimer_r = Ref<UvTimer>;
	using UvTimeouterBase_r = Ref<UvTimeouterBase>;
	using UvTimeouter_r = Ref<UvTimeouter>;
	using UvAsync_r = Ref<UvAsync>;
	using UvRpcManager_r = Ref<UvRpcManager>;
	using UvTimeouter_r = Ref<UvTimeouter>;
	using UvContextBase_r = Ref<UvContextBase>;
	using UvUdpListener_r = Ref<UvUdpListener>;
	using UvUdpBase_r = Ref<UvUdpBase>;
	using UvUdpPeer_r = Ref<UvUdpPeer>;
	using UvUdpClient_r = Ref<UvUdpClient>;

}
