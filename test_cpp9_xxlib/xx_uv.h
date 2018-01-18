#pragma once
#include "xx.h"
#include <mutex>

namespace xx
{
	class UvLoop;
	class UvTcpListener;
	class UvTcpUdpBase;
	class UvTcpBase;
	class UvTcpPeer;
	class UvTcpClient;
	class UvTimer;
	class UvTimerBase;
	class UvTimeouter;
	class UvAsync;
	class UvRpcManager;
	class UvTimeouter;
	class UvContextBase;	// todo
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

	class UvLoop
	{
	public:
		void* ptr = nullptr;
		MemPool mp;
		List<UvTcpListener*> tcpListeners;
		List<UvTcpClient*> tcpClients;
		List<UvUdpListener*> udpListeners;
		List<UvUdpClient*> udpClients;
		List<UvTimer*> timers;
		List<UvAsync*> asyncs;
		UvTimeouter* timeouter = nullptr;
		UvRpcManager* rpcMgr = nullptr;
		UvTimer* udpTimer = nullptr;
		uint64_t udpTicks = 0;
		char udpRecvBuf[65536];
		uint32_t kcpInterval = 0;

		explicit UvLoop();
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

	class UvTcpListener : public Object
	{
	public:
		std::function<UvTcpPeer*()> OnCreatePeer;
		std::function<void(UvTcpPeer*)> OnAccept;
		std::function<void()> OnDispose;

		UvLoop& loop;
		List<UvTcpPeer*> peers;
		size_t index_at_container;

		void* ptr;
		void* addrPtr;
		UvTcpListener(MemPool* mp, UvLoop& loop);
		~UvTcpListener();
		static void OnAcceptCB(void* server, int status);
		void Listen(int backlog = 128);
		void Bind(char const* const& ipv4, int port);
	};

	class UvTcpUdpBase : public UvTimerBase
	{
	public:
		std::function<void(BBuffer&)> OnReceivePackage;
		std::function<void(uint32_t, BBuffer&)> OnReceiveRequest;
		std::function<void()> OnDispose;

		UvLoop& loop;
		size_t index_at_container;

		void* ptr;
		void* addrPtr;

		BBuffer bbRecv;
		BBuffer bbSend;

		UvTcpUdpBase(MemPool* mp, UvLoop& loop);

		virtual void DisconnectImpl() = 0;
		virtual void ReceivePackageImpl(BBuffer const& bb) = 0;
		virtual void ReceiveRequestImpl(uint32_t serial, BBuffer const& bb) = 0;
		virtual void ReceiveResponseImpl(uint32_t serial, BBuffer const& bb) = 0;
		virtual bool Disconnected() = 0;
		virtual size_t GetSendQueueSize() = 0;
		virtual void SendBytes(char const* inBuf, int len = 0) = 0;

		void ReceiveImpl(char const* bufPtr, int len);

		void SendBytes(BBuffer const& bb);

		template<typename T>
		void Send(T const& pkg);

		template<typename T>
		uint32_t SendRequest(T const& pkg, std::function<void(uint32_t, BBuffer)> cb, int interval = 0);

		template<typename T>
		void SendResponse(uint32_t serial, T const& pkg);
	};

	class UvTcpBase : public UvTcpUdpBase
	{
	public:
		UvTcpBase(MemPool* mp, UvLoop& loop);

		void ReceivePackageImpl(BBuffer const& bb) override;
		void ReceiveRequestImpl(uint32_t serial, BBuffer const& bb) override;
		void ReceiveResponseImpl(uint32_t serial, BBuffer const& bb) override;
		void DisconnectImpl() override;
		size_t GetSendQueueSize() override;
		void SendBytes(char const* inBuf, int len = 0) override;

		static void OnReadCBImpl(void* stream, ptrdiff_t nread, const void* buf_t);
	};

	class UvTcpPeer : public UvTcpBase
	{
	public:
		// bool alive 只能外部检测野指针

		UvTcpListener & listener;
		UvTcpPeer(MemPool* mp, UvTcpListener& listener);
		~UvTcpPeer();	// Release 取代 Dispose
		void DisconnectImpl() override;
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
		UvTcpClient(MemPool* mp, UvLoop& loop);
		~UvTcpClient();	// Release 取代 Dispose
		void SetAddress(char const* const& ipv4, int port);
		static void OnConnectCBImpl(void* req, int status);
		void Connect();
		void Disconnect();
		void DisconnectImpl() override;
	};

	class UvTimer : public Object
	{
	public:
		std::function<void()> OnFire;
		std::function<void()> OnDispose;

		UvLoop& loop;
		size_t index_at_container;
		void* ptr;
		UvTimer(MemPool* mp, UvLoop& loop, uint64_t timeoutMS, uint64_t repeatIntervalMS, std::function<void()>&& OnFire = nullptr);
		~UvTimer();
		static void OnTimerCBImpl(void* handle);
		void SetRepeat(uint64_t repeatIntervalMS);
		void Again();
		void Stop();
	};

	class UvTimerBase : public Object
	{
	public:
		UvTimerBase(MemPool* mp);
		UvTimeouter* timerManager = nullptr;
		UvTimerBase* timerPrev = nullptr;
		UvTimerBase* timerNext = nullptr;
		int timerIndex = -1;
		std::function<void()> OnTimeout;

		void TimerClear();
		void TimeoutReset(int interval = 0);
		void TimerStop();
		void BindTo(UvTimeouter* tm);
		void UnbindTimerManager();
		bool timering();
	};

	class UvTimeouter : public Object
	{
	public:
		UvTimer * timer;
		List<UvTimerBase*> timerss;
		int cursor = 0;
		int defaultInterval;
		UvTimeouter(MemPool* mp, UvLoop& loop, uint64_t intervalMS, int wheelLen, int defaultInterval);
		~UvTimeouter();
		void Process();
		void Clear();
		void Add(UvTimerBase* t, int interval = 0);
		void Remove(UvTimerBase* t);
		void AddOrUpdate(UvTimerBase* t, int interval = 0);
	};

	class UvAsync : public Object
	{
	public:
		std::function<void()> OnFire;
		std::function<void()> OnDispose;

		UvLoop& loop;
		size_t index_at_container;
		std::mutex mtx;
		Queue<std::function<void()>> actions;
		void* ptr;
		UvAsync(MemPool* mp, UvLoop& loop);
		~UvAsync();
		static void OnAsyncCBImpl(void* handle);
		void Dispatch(std::function<void()>&& a);
		void OnFireImpl();
	};

	class UvRpcManager : public Object
	{
	public:
		UvTimer * timer;
		uint32_t serial;
		Dict<uint32_t, std::function<void(uint32_t, BBuffer*)>> mapping;
		Queue<std::pair<int, uint32_t>> serials;
		int defaultInterval;
		int ticks;
		size_t count();
		UvRpcManager(MemPool* mp, UvLoop& loop, uint64_t intervalMS, int defaultInterval);
		~UvRpcManager();
		void Process();
		uint32_t Register(std::function<void(uint32_t, BBuffer*)>&& cb, int interval = 0);
		void Unregister(uint32_t serial);
		void Callback(uint32_t serial, BBuffer* bb);
	};

	class UvContextBase : public Object
	{
	public:
		UvContextBase(MemPool* mp);
		UvTcpUdpBase* peer;
		bool peerAlive();
		bool BindPeer(UvTcpUdpBase* p);
		void KickPeer(bool immediately = true);
		void OnPeerReceiveRequest(uint32_t serial, BBuffer& bb);
		void OnPeerReceivePackage(BBuffer& bb);
		void OnPeerDisconnect();
		virtual void HandleRequest(uint32_t serial, Ptr<Object>& o) = 0;
		virtual void HandlePackage(Ptr<Object>& o) = 0;
		virtual void HandleDisconnect() = 0;
	};

	class UvUdpListener : public Object
	{
	public:
		std::function<UvUdpPeer*()> OnCreatePeer;
		std::function<void(UvUdpPeer*)> OnAccept;
		std::function<void()> OnDispose;

		Dict<Guid, UvUdpPeer*> peers;
		UvLoop& loop;
		int index_at_container;

		void* ptr;
		void* addrPtr;

		UvUdpListener(MemPool* mp, UvLoop& loop);
		static void OnRecvCBImpl(void* udp, size_t nread, void* buf_t, void* addr, uint32_t flags);
		void OnReceiveImpl(char const* bufPtr, int len, void* addr);

		void RecvStart();
		void RecvStop();
		void Bind(char const* ipv4, int port);
	};

	class UvUdpBase : public UvTcpUdpBase
	{
		Guid guid;

		uint32_t nextUpdateTicks;

		void ReceivePackageImpl(BBuffer const& bb) override;
		void ReceiveRequestImpl(uint32_t serial, BBuffer const& bb) override;
		void ReceiveResponseImpl(uint32_t serial, BBuffer const& bb) override;
	};

	class UvUdpPeer : public UvUdpBase
	{
		UvUdpListener listener;

		UvUdpPeer(UvUdpListener listener, Guid guid, char const* rawData
			, int sndwnd = 128, int rcvwnd = 128
			, int nodelay = 1, int interval = 10, int resend = 2, int nc = 1);

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
		void* kcpPtr;
		UvUdpClient(UvLoop loop);
		void Connect(Guid guid
			, int sndwnd = 128, int rcvwnd = 128
			, int nodelay = 1, int interval = 10, int resend = 2, int nc = 1);

		static void OnRecvCBImpl(void* udp, void* nread, void* buf_t, void* addr, uint32_t flags);
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
	inline void UvTcpBase::Send(T const & pkg)
	{
		if (!ptr) throw - 1;
		bbSend.Clear();
		bbSend.BeginWritePackage();
		bbSend.WriteRoot(pkg);
		bbSend.EndWritePackage();
		SendBytes(bbSend);
	}

	template<typename T>
	inline uint32_t UvTcpBase::SendRequest(T const & pkg, std::function<void(uint32_t, BBuffer)> cb, int interval)
	{
		if (!ptr) throw - 1;
		auto serial = loop.rpcMgr.Register(cb, interval);
		bbSend.Clear();
		bbSend.BeginWritePackage(1, serial);
		bbSend.WriteRoot(pkg);
		bbSend.EndWritePackage();
		SendBytes(bbSend);
		return serial;
	}

	template<typename T>
	inline void UvTcpBase::SendResponse(uint32_t serial, T const & pkg)
	{
		template<typename T>
		bbSend.Clear();
		bbSend.BeginWritePackage(2, serial);
		bbSend.WriteRoot(pkg);
		bbSend.EndWritePackage();
		SendBytes(bbSend);
	}

}
