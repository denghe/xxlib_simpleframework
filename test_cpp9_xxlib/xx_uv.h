#pragma once
#include "xx.h"

// todo: 从 C# 翻抄

namespace xx
{
	class UvTcpListener;
	class UvTcpClient;
	class UvTcpPeer;
	class UvTimer;
	class UvAsync;
	class UvRpcManager;
	class UvTimeouter;

	int uv_write_(MemPool* mp, void* stream, char* inBuf, unsigned int offset, unsigned int len) noexcept;

	class UvLoop
	{
	public:
		MemPool mp;
		List<UvTcpListener*> listeners;
		//List<UvTcpClient*> clients;
		//List<UvTimer*> timers;
		//List<UvAsync*> asyncs;
		//UvRpcManager* rpcMgr;

		void* ptr;
		UvLoop(uint64_t rpcIntervalMS = 1000, int rpcDefaultInterval = 5);
		~UvLoop();
		void Run(int mode);
		void Stop();
		bool alive();
	};

	class UvTcpListener
	{
	public:
		std::function<UvTcpPeer*()> OnCreatePeer;
		std::function<void(UvTcpPeer*)> OnAccept;
		std::function<void()> OnDispose;

		UvLoop& loop;
		//List<UvTcpPeer*> peers;
		size_t index_at_container;

		void* ptr;
		void* addrPtr;
		UvTcpListener(UvLoop& loop);
		~UvTcpListener();
		static void OnAcceptCB(void* server, int status);
		void Listen(int backlog = 128);
		void Bind(char const* const& ipv4, int port);
	};

	class UvTimerBase
	{
	public:
		UvTimeouter * timerManager;
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

	class UvTimer
	{
	public:
		std::function<void()> OnFire;
		std::function<void()> OnDispose;

		UvLoop& loop;
		int index_at_container;
		void* ptr;
		UvTimer(UvLoop& loop, uint64_t timeoutMS, uint64_t repeatIntervalMS, std::function<void()>&& OnFire = nullptr);
		~UvTimer();
		static void OnTimerCBImpl(void* handle);
		void SetRepeat(uint64_t repeatIntervalMS);
		void Again();
		void Stop();
	};

	class UvTimeouter
	{
	public:
		UvTimer timer;
		List<UvTimerBase*> timerss;
		int cursor = 0;
		int defaultInterval;
		UvTimeouter(UvLoop& loop, uint64_t intervalMS, int wheelLen, int defaultInterval);
		void Process();
		void Clear();
		void Add(UvTimerBase* t, int interval = 0);
		void Remove(UvTimerBase* t);
		void AddOrUpdate(UvTimerBase* t, int interval = 0);
	};

	class UvTcpBase : UvTimerBase
	{
	public:
		std::function<void(BBuffer&)> OnReceivePackage;
		std::function<void(uint32_t, BBuffer&)> OnReceiveRequest;
		std::function<void()> OnDispose;

		UvLoop& loop;
		int index_at_container;
		BBuffer bbRecv;
		BBuffer bbSend;

		void* ptr;
		void* addrPtr;
		UvTcpBase(UvLoop& loop);

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
		bool alive() { return !ptr; }

		UvTcpListener& listener;
		UvTcpPeer(UvTcpListener& listener);
		~UvTcpPeer();
		void DisconnectImpl() override;
		std::array<char, 64> ipBuf;
		char* ip();
	};


































	template<typename T>
	inline void UvTcpBase::Send(T const & pkg)
	{
		if (!ptr) throw - 1;
		bbSend.Clear();
		bbSend.BeginWritePackage();
		bbSend.WriteRoot(pkg);
		bbSend.EndWritePackageEx();
		SendBytes(bbSend);
	}

	template<typename T>
	inline uint32_t UvTcpBase::SendRequest(T const & pkg, std::function<void(uint32_t, BBuffer)> cb, int interval)
	{
		if (!ptr) throw - 1;
		if (!loop.rpcMgr) throw - 2;
		auto serial = loop.rpcMgr->Register(cb, interval);
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
