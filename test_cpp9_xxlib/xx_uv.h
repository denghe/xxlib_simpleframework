#pragma once
#include "xx.h"
#include <mutex>

namespace xx
{
	class UvLoop;
	class UvTcpListener;
	class UvTcpClient;
	class UvTcpPeer;
	class UvTimer;
	class UvAsync;
	class UvRpcManager;
	class UvTimeouter;

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

	class UvRpcManager : public Object
	{
	public:
		UvTimer timer;
		Dict<uint32_t, std::function<void(uint32_t, BBuffer*)>> mapping;
		Queue<std::pair<int, uint32_t>> serials;
		int defaultInterval;
		uint32_t serial;
		int ticks;
		size_t Count();
		UvRpcManager(MemPool* mp, UvLoop& loop, uint64_t intervalMS, int defaultInterval);
		void Process();
		uint32_t Register(std::function<void(uint32_t, BBuffer*)>&& cb, int interval = 0);
		void Unregister(uint32_t serial);
		void Callback(uint32_t serial, BBuffer* bb);
	};

	class UvLoop
	{
	public:
		MemPool mp;
		List<UvTcpListener*> listeners;
		List<UvTcpClient*> clients;
		List<UvTimer*> timers;
		List<UvAsync*> asyncs;
		UvRpcManager* rpcMgr;

		void* ptr;
		explicit UvLoop(uint64_t rpcIntervalMS = 1000, int rpcDefaultInterval = 5);
		~UvLoop();
		void Run(UvRunMode mode = UvRunMode::Default);
		void Stop();
		bool alive() const;

		UvTcpListener* CreateTcpListener();
		UvTcpClient* CreateClient();
		UvTimer* CreateTimer(uint64_t timeoutMS, uint64_t repeatIntervalMS, std::function<void()>&& OnFire = nullptr);
		UvAsync* CreateAsync();
		UvTimeouter* CreateTimeouter(uint64_t intervalMS = 1000, int wheelLen = 6, int defaultInterval = 5);
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
		UvTimer timer;
		List<UvTimerBase*> timerss;
		int cursor = 0;
		int defaultInterval;
		UvTimeouter(MemPool* mp, UvLoop& loop, uint64_t intervalMS, int wheelLen, int defaultInterval);
		void Process();
		void Clear();
		void Add(UvTimerBase* t, int interval = 0);
		void Remove(UvTimerBase* t);
		void AddOrUpdate(UvTimerBase* t, int interval = 0);
	};

	class UvTcpBase : public UvTimerBase
	{
	public:
		std::function<void(BBuffer&)> OnReceivePackage;
		std::function<void(uint32_t, BBuffer&)> OnReceiveRequest;
		std::function<void()> OnDispose;

		UvLoop& loop;
		size_t index_at_container;
		BBuffer bbRecv;
		BBuffer bbSend;

		void* ptr;
		void* addrPtr;
		Ref<UvTcpBase> self;	// for release check
		UvTcpBase(MemPool* mp, UvLoop& loop);

		virtual void DisconnectImpl() = 0;
		static void OnReadCBImpl(void* stream, ptrdiff_t nread, const void* buf_t);
		void OnReceiveImpl(char const* bufPtr, int len);
		void SendBytes(char* inBuf, int len = 0);
		void SendBytes(BBuffer const& bb);

		template<typename T>
		void Send(T const& pkg);

		template<typename T>
		uint32_t SendRequest(T const& pkg, std::function<void(uint32_t, BBuffer)> cb, int interval = 0);

		template<typename T>
		void SendResponse(uint32_t serial, T const& pkg);
	};

	class UvTcpPeer : public UvTcpBase
	{
	public:
		bool alive() const;

		UvTcpListener& listener;
		UvTcpPeer(MemPool* mp, UvTcpListener& listener);
		~UvTcpPeer();
		void DisconnectImpl() override;
		void Dispose();
		std::array<char, 64> ipBuf;
		char* ip();
	};

	class UvTcpClient : public UvTcpBase
	{
	public:
		std::function<void(int)> OnConnect;
		std::function<void()> OnDisconnect;

		UvTcpStates state = UvTcpStates::Disconnected;
		bool alive() const;
		UvTcpClient(MemPool* mp, UvLoop& loop);
		void SetAddress(char const* const& ipv4, int port);
		static void OnConnectCBImpl(void* req, int status);
		void Connect();
		void Disconnect();
		void DisconnectImpl() override;
		void Dispose();
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
		void Dispose();
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
