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
		std::array<char, 65536> udpRecvBuf;
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
		UvTimeouter* timeouter = nullptr;
		UvTimeouterBase* timerPrev = nullptr;
		UvTimeouterBase* timerNext = nullptr;
		int timerIndex = -1;
		std::function<void()> OnTimeout;

		void TimeouterClear();
		void TimeoutReset(int interval = 0);
		void TimeouterStop();
		virtual void BindTimeouter(UvTimeouter*) = 0;
		/*
		void xx::T::BindTimeouter(UvTimeouter* tm = nullptr)
		{
			if (timeouter) throw - 1;
			timeouter = tm ? tm : loop.timeouter;
		}
		*/
		void UnbindTimeouter();
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
		// 3 个 size_t 代表 含包头的总包长, 地址起始偏移, 地址长度( 方便替换地址并 memcpy )
		// BBuffer 的 offset 停在包头起始处
		std::function<void(BBuffer&, size_t, size_t, size_t)> OnReceiveRouting;


		std::function<void()> OnDispose;


		UvLoop& loop;
		size_t index_at_container = -1;

		void* ptr = nullptr;
		void* addrPtr = nullptr;

		BBuffer bbRecv;
		BBuffer bbSend;

		UvTcpUdpBase(UvLoop& loop);

		virtual void BindTimeouter(UvTimeouter* t = nullptr);

		virtual void DisconnectImpl() = 0;
		virtual bool Disconnected() = 0;
		virtual size_t GetSendQueueSize() = 0;
		virtual void SendBytes(char const* inBuf, int len = 0) = 0;

		virtual void ReceiveImpl(char const* bufPtr, int len);

		void SendBytes(BBuffer& bb);



		// 三种常用 Send 函数

		template<typename T>
		void Send(T const& pkg);

		template<typename T>
		uint32_t SendRequest(T const& pkg, std::function<void(uint32_t, BBuffer*)>&& cb, int interval = 0);

		template<typename T>
		void SendResponse(uint32_t serial, T const& pkg);



		// 下面是上面 3 个 Send 的 Routing 版( 客户端, 非路由服务端专用 )

		// 向路由服务发包
		template<typename T>
		void SendRouting(char const* serviceAddr, size_t serviceAddrLen, T const& pkg);

		// 向路由服务发请求
		template<typename T>
		uint32_t SendRoutingRequest(char const* serviceAddr, size_t serviceAddrLen, T const& pkg, std::function<void(uint32_t, BBuffer*)>&& cb, int interval = 0);

		// 向路由服务发回应
		template<typename T>
		void SendRoutingResponse(char const* serviceAddr, size_t serviceAddrLen, uint32_t serial, T const& pkg);



		// 下面是路由服务专用

		// 纯下发地址. 
		void SendRoutingAddress(char const* buf, size_t len);

		// 读出上面函数下发的地址长度
		static size_t GetRoutingAddressLength(BBuffer& bb);

		// 在不解析数据的情况下直接替换地址部分转发( 路由专用 )
		void SendRoutingEx(xx::BBuffer& bb, size_t pkgLen, size_t addrOffset, size_t addrLen, char const* senderAddr, size_t senderAddrLen);
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
		UvTimer * timer = nullptr;
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
		UvTimer * timer = nullptr;
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
		UvTcpUdpBase * peer = nullptr;

		UvContextBase(MemPool* mp);
		~UvContextBase();
		bool PeerAlive();

		// 重要!! 如果该函数执行点位于 peer 之 OnXxxxx 中, 则在 Bind 成功之时, 先前的 lambda 上下文将被 "顶掉", 所有捕获将变"野", 故该函数只能于此类函数退出前执行
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
		UvUdpListener & listener;

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
		bbSend.Reserve(5);
		bbSend.dataLen = 5;
		bbSend.WriteRoot(pkg);
		auto dataLen = bbSend.dataLen - 5;
		if (dataLen <= std::numeric_limits<uint16_t>::max())
		{
			auto p = bbSend.buf + 2;
			p[0] = 0;
			p[1] = (uint8_t)dataLen;
			p[2] = (uint8_t)(dataLen >> 8);
			SendBytes(p, (int)(dataLen + 3));
		}
		else
		{
			auto p = bbSend.buf;
			p[0] = 0b00000100;
			p[1] = (uint8_t)dataLen;
			p[2] = (uint8_t)(dataLen >> 8);
			p[3] = (uint8_t)(dataLen >> 16);
			p[4] = (uint8_t)(dataLen >> 24);
			SendBytes(p, (int)(dataLen + 5));
		}
	}

	template<typename T>
	inline uint32_t UvTcpUdpBase::SendRequest(T const & pkg, std::function<void(uint32_t, BBuffer*)>&& cb, int interval)
	{
		if (!ptr) throw - 1;
		bbSend.Clear();
		bbSend.Reserve(5);
		bbSend.dataLen = 5;
		auto serial = loop.rpcMgr->Register(std::move(cb), interval);	// 注册回调并得到流水号
		bbSend.Write(serial);											// 在包前写入流水号
		bbSend.WriteRoot(pkg);
		auto dataLen = bbSend.dataLen - 5;
		if (dataLen <= std::numeric_limits<uint16_t>::max())
		{
			auto p = bbSend.buf + 2;
			p[0] = 0b00000001;											// 这里标记包头为 Request 类型
			p[1] = (uint8_t)dataLen;
			p[2] = (uint8_t)(dataLen >> 8);
			SendBytes(p, (int)(dataLen + 3));
		}
		else
		{
			auto p = bbSend.buf;
			p[0] = 0b00000101;											// 这里标记包头为 Big + Request 类型
			p[1] = (uint8_t)dataLen;
			p[2] = (uint8_t)(dataLen >> 8);
			p[3] = (uint8_t)(dataLen >> 16);
			p[4] = (uint8_t)(dataLen >> 24);
			SendBytes(p, (int)(dataLen + 5));
		}
		return serial;													// 返回流水号
	}

	template<typename T>
	inline void UvTcpUdpBase::SendResponse(uint32_t serial, T const & pkg)
	{
		if (!ptr) throw - 1;
		bbSend.Clear();
		bbSend.Reserve(5);
		bbSend.dataLen = 5;
		bbSend.Write(serial);											// 在包前写入流水号
		bbSend.WriteRoot(pkg);
		auto dataLen = bbSend.dataLen - 5;
		if (dataLen <= std::numeric_limits<uint16_t>::max())
		{
			auto p = bbSend.buf + 2;
			p[0] = 0b00000010;											// 这里标记包头为 Response 类型
			p[1] = (uint8_t)dataLen;
			p[2] = (uint8_t)(dataLen >> 8);
			SendBytes(p, (int)(dataLen + 3));
		}
		else
		{
			auto p = bbSend.buf;
			p[0] = 0b00000110;											// 这里标记包头为 Big + Response 类型
			p[1] = (uint8_t)dataLen;
			p[2] = (uint8_t)(dataLen >> 8);
			p[3] = (uint8_t)(dataLen >> 16);
			p[4] = (uint8_t)(dataLen >> 24);
			SendBytes(p, (int)(dataLen + 5));
		}
	}


	template<typename T>
	inline void UvTcpUdpBase::SendRouting(char const* serviceAddr, size_t serviceAddrLen, T const & pkg)
	{
		if (!ptr) throw - 1;
		bbSend.Clear();
		bbSend.Reserve(5);
		bbSend.dataLen = 5;
		bbSend.WriteBuf(serviceAddr, serviceAddrLen);					// 在包前写入地址
		bbSend.WriteRoot(pkg);
		auto dataLen = bbSend.dataLen - 5;
		if (dataLen <= std::numeric_limits<uint16_t>::max())
		{
			auto p = bbSend.buf + 2;
			p[0] = (uint8_t)(0b00001000 | ((serviceAddrLen - 1) << 4));	// 拼接为 XXXX1000 的含长度信息的路由包头
			p[1] = (uint8_t)dataLen;
			p[2] = (uint8_t)(dataLen >> 8);
			SendBytes(p, (int)(dataLen + 3));
		}
		else
		{
			auto p = bbSend.buf;
			p[0] = (uint8_t)(0b00001100 | ((serviceAddrLen - 1) << 4));	// 拼接为 XXXX1100 的含长度信息的 Big 路由包头
			p[1] = (uint8_t)dataLen;
			p[2] = (uint8_t)(dataLen >> 8);
			p[3] = (uint8_t)(dataLen >> 16);
			p[4] = (uint8_t)(dataLen >> 24);
			SendBytes(p, (int)(dataLen + 5));
		}
	}

	template<typename T>
	inline uint32_t UvTcpUdpBase::SendRoutingRequest(char const* serviceAddr, size_t serviceAddrLen, T const & pkg, std::function<void(uint32_t, BBuffer*)>&& cb, int interval)
	{
		if (!ptr) throw - 1;
		bbSend.Clear();
		bbSend.Reserve(5);
		bbSend.dataLen = 5;
		bbSend.WriteBuf(serviceAddr, serviceAddrLen);					// 在包前写入地址
		auto serial = loop.rpcMgr->Register(std::move(cb), interval);	// 注册回调并得到流水号
		bbSend.Write(serial);											// 在包前写入流水号
		bbSend.WriteRoot(pkg);
		auto dataLen = bbSend.dataLen - 5;
		if (dataLen <= std::numeric_limits<uint16_t>::max())
		{
			auto p = bbSend.buf + 2;
			p[0] = (uint8_t)(0b00001001 | ((serviceAddrLen - 1) << 4));	// 这里标记包头为 addrLen + Routing + Request 类型
			p[1] = (uint8_t)dataLen;
			p[2] = (uint8_t)(dataLen >> 8);
			SendBytes(p, (int)(dataLen + 3));
		}
		else
		{
			auto p = bbSend.buf;
			p[0] = (uint8_t)(0b00001101 | ((serviceAddrLen - 1) << 4));	// 这里标记包头为 addrLen + Routing + Big + Request 类型
			p[1] = (uint8_t)dataLen;
			p[2] = (uint8_t)(dataLen >> 8);
			p[3] = (uint8_t)(dataLen >> 16);
			p[4] = (uint8_t)(dataLen >> 24);
			SendBytes(p, (int)(dataLen + 5));
		}
		return serial;													// 返回流水号
	}

	template<typename T>
	inline void UvTcpUdpBase::SendRoutingResponse(char const* serviceAddr, size_t serviceAddrLen, uint32_t serial, T const & pkg)
	{
		if (!ptr) throw - 1;
		bbSend.Clear();
		bbSend.Reserve(5);
		bbSend.dataLen = 5;
		bbSend.WriteBuf(serviceAddr, serviceAddrLen);					// 在包前写入地址
		bbSend.Write(serial);											// 在包前写入流水号
		bbSend.WriteRoot(pkg);
		auto dataLen = bbSend.dataLen - 5;
		if (dataLen <= std::numeric_limits<uint16_t>::max())
		{
			auto p = bbSend.buf + 2;
			p[0] = (uint8_t)(0b00001010 | ((serviceAddrLen - 1) << 4));	// 这里标记包头为 addrLen + Routing + Response 类型
			p[1] = (uint8_t)dataLen;
			p[2] = (uint8_t)(dataLen >> 8);
			SendBytes(p, (int)(dataLen + 3));
		}
		else
		{
			auto p = bbSend.buf;
			p[0] = (uint8_t)(0b00001110 | ((serviceAddrLen - 1) << 4));	// 这里标记包头为 addrLen + Routing + Big + Response 类型
			p[1] = (uint8_t)dataLen;
			p[2] = (uint8_t)(dataLen >> 8);
			p[3] = (uint8_t)(dataLen >> 16);
			p[4] = (uint8_t)(dataLen >> 24);
			SendBytes(p, (int)(dataLen + 5));
		}
	}

}
