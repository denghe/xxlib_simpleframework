#pragma once
#include "xx.h"
#include <mutex>

// 注意: 除了 UvLoop, 其他类型只能以指针方式 Create 出来用. 否则会出现重复 Release 的情况.

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
	class UvAsync;
	class UvRpcManager;
	class UvTimeoutManager;
	class UvContextBase;
	class UvUdpListener;
	class UvUdpBase;
	class UvUdpPeer;
	class UvUdpClient;
	class UvHttpPeer;
	class UvHttpClient;

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
		UvTimeoutManager* timeoutManager = nullptr;
		UvRpcManager* rpcMgr = nullptr;
		UvTimer* udpTimer = nullptr;
		uint32_t udpTicks = 0;
		std::array<char, 65536> udpRecvBuf;
		uint32_t kcpInterval = 0;

		explicit UvLoop(MemPool* const& mp);
		~UvLoop();

		void InitTimeoutManager(uint64_t const& intervalMS = 1000, int const& wheelLen = 6, int const& defaultInterval = 5);
		void InitRpcManager(uint64_t const& rpcIntervalMS = 1000, int const& rpcDefaultInterval = 5);
		void InitKcpFlushInterval(uint32_t const& interval = 10);

		void Run(UvRunMode const& mode = UvRunMode::Default);
		void Stop();
		bool alive() const;

		UvTcpListener* CreateTcpListener();
		UvTcpClient* CreateTcpClient();
		UvUdpListener* CreateUdpListener();
		UvUdpClient* CreateUdpClient();
		UvTimer* CreateTimer(uint64_t const& timeoutMS, uint64_t const& repeatIntervalMS, std::function<void()>&& OnFire = nullptr);
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
		void Bind(char const* const& ipv4, int const& port);
		void Listen(int const& backlog = 128);
	};

	class UvTimeouterBase : public Object
	{
	public:
		UvTimeouterBase(MemPool* const& mp);
		~UvTimeouterBase();
		UvTimeoutManager* timeoutManager = nullptr;
		UvTimeouterBase* timeouterPrev = nullptr;
		UvTimeouterBase* timeouterNext = nullptr;
		int timeouterIndex = -1;
		std::function<void()> OnTimeout;

		void TimeouterClear();
		void TimeoutReset(int const& interval = 0);
		void TimeouterStop();
		virtual void BindTimeoutManager(UvTimeoutManager* const&);
		void UnbindTimeoutManager();
		bool timeouting();
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


		// 用来放 serial 以便断线时及时发起 Request 超时回调
		xx::HashSet_p<uint32_t> rpcSerials;


		UvTcpUdpBase(UvLoop& loop);

		virtual void BindTimeoutManager(UvTimeoutManager* const& t = nullptr);

		virtual void DisconnectImpl() = 0;
		virtual bool Disconnected() = 0;
		virtual size_t GetSendQueueSize() = 0;
		virtual void SendBytes(char const* const& inBuf, int const& len = 0) = 0;

		virtual void ReceiveImpl(char const* const& bufPtr, int const& len);

		void SendBytes(BBuffer& bb);



		// 三种常用 Send 函数

		template<typename T>
		void Send(T const& pkg);

		template<typename T>
		uint32_t SendRequest(T const& pkg, std::function<void(uint32_t, BBuffer*)>&& cb, int const& interval = 0);

		template<typename T>
		void SendResponse(uint32_t const& serial, T const& pkg);



		// 下面是上面 3 个 Send 的 Routing 版( 客户端, 非路由服务端专用 )

		// 向路由服务发包
		template<typename T>
		void SendRouting(char const* const& serviceAddr, size_t const& serviceAddrLen, T const& pkg);

		// 向路由服务发请求
		template<typename T>
		uint32_t SendRoutingRequest(char const* const& serviceAddr, size_t const& serviceAddrLen, T const& pkg, std::function<void(uint32_t, BBuffer*)>&& cb, int const& interval = 0);

		// 向路由服务发回应
		template<typename T>
		void SendRoutingResponse(char const* const& serviceAddr, size_t const& serviceAddrLen, uint32_t const& serial, T const& pkg);



		// 下面是路由服务专用

		// 纯下发地址. 
		void SendRoutingAddress(char const* const& buf, size_t const& len);

		// 读出上面函数下发的地址长度
		static size_t GetRoutingAddressLength(BBuffer& bb);

		// 在不解析数据的情况下直接替换地址部分转发( 路由专用 )
		void SendRoutingByRouter(xx::BBuffer& bb, size_t const& pkgLen, size_t const& addrOffset, size_t const& addrLen, char const* const& senderAddr, size_t const& senderAddrLen);



		// 超时回调所有被跟踪 rpc 流水号并清空( 内部函数. 会自动在 OnDispose, OnDisconnect 事件前调用以触发超时回调 )
		void RpcTraceCallback();

		// 增强的 SendRequest 实现 断线时 立即发起相关 rpc 超时回调. 封装了解包操作. 
		template<typename T>
		void SendRequestEx(T const& pkg, std::function<void(uint32_t, xx::Object_p&)>&& cb, int const& interval = 0);

		// 会清除掉 OnReceiveXxxxxx, OnDispose 的各种事件, BindTimeoutManager 并在 OnTimeout 时 Release
		void DelayRelease(int const& interval = 0);
	};

	class UvTcpBase : public UvTcpUdpBase
	{
	public:
		UvTcpBase(UvLoop& loop);

		size_t GetSendQueueSize() override;
		void SendBytes(char const* const& inBuf, int const& len = 0) override;

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
		const char* ip();
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
		void SetAddress(char const* const& ipv4, int const& port);
		static void OnConnectCBImpl(void* req, int status);
		void Connect();
		int TryConnect(char const* const& ip, int const& port);	// 等同于 Disconnect + SetAddress + Connect
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
		UvTimer(UvLoop& loop, uint64_t const& timeoutMS, uint64_t const& repeatIntervalMS, std::function<void()>&& OnFire = nullptr);
		~UvTimer();
		static void OnTimerCBImpl(void* handle);
		void SetRepeat(uint64_t const& repeatIntervalMS);
		void Again();
		void Stop();
	};

	class UvTimeoutManager : public Object
	{
	public:
		UvTimer * timer = nullptr;
		List<UvTimeouterBase*> timeouterss;
		int cursor = 0;
		int defaultInterval;
		UvTimeoutManager(UvLoop& loop, uint64_t const& intervalMS, int const& wheelLen, int const& defaultInterval);
		~UvTimeoutManager();
		void Process();
		void Clear();
		void Add(UvTimeouterBase* const& t, int interval = 0);
		void Remove(UvTimeouterBase* const& t);
		void AddOrUpdate(UvTimeouterBase* const& t, int const& interval = 0);
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
		UvRpcManager(UvLoop& loop, uint64_t const& intervalMS, int const& defaultInterval);
		~UvRpcManager();
		void Process();
		uint32_t Register(std::function<void(uint32_t, BBuffer*)>&& cb, int interval = 0);
		void Unregister(uint32_t const& serial);
		void Callback(uint32_t const& serial, BBuffer* const& bb);
	};

	// 暂不建议使用
	class UvContextBase : public Object
	{
	public:
		UvTcpUdpBase * peer = nullptr;

		UvContextBase(MemPool* const& mp);
		~UvContextBase();
		bool PeerAlive();

		// 重要!! 如果该函数执行点位于 peer 之 OnXxxxx 中, 则在 Bind 成功之时, 先前的 lambda 上下文将被 "顶掉", 所有捕获将变"野", 故该函数只能于此类函数退出前执行
		bool BindPeer(UvTcpUdpBase* const& p);
		void KickPeer(bool const& immediately = true);
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
		void OnReceiveImpl(char const* const& bufPtr, int const& len, void* const& addr);

		void Bind(char const* const& ipv4, int const& port);
		void Listen();
		void StopListen();

		UvUdpPeer* CreatePeer(Guid const& g
			, int const& sndwnd = 128, int const& rcvwnd = 128
			, int const& nodelay = 1/*, int const& interval = 10*/, int const& resend = 2, int const& nc = 1, int const& minrto = 100);
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
			, int const& sndwnd = 128, int const& rcvwnd = 128
			, int const& nodelay = 1/*, int const& interval = 10*/, int const& resend = 2, int const& nc = 1, int const& minrto = 100);
		~UvUdpPeer();

		static int OutputImpl(char const* buf, int len, void* kcp);
		void Update(uint32_t const& current);
		void Input(char const* const& data, int const& len);
		void SendBytes(char const* const& data, int const& len = 0) override;
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
			, int const& sndwnd = 128, int const& rcvwnd = 128
			, int const& nodelay = 1/*, int const& interval = 10*/, int const& resend = 2, int const& nc = 1, int const& minrto = 100);

		static void OnRecvCBImpl(void* udp, ptrdiff_t nread, void* buf_t, void* addr, uint32_t flags);
		void OnReceiveImpl(char const* const& bufPtr, int const& len, void* const& addr);
		static int OutputImpl(char const* buf, int len, void* kcp);
		void Update(uint32_t const& current);
		void Disconnect();
		void SetAddress(char const* const& ipv4, int const& port);
		void SendBytes(char const* const& data, int const& len = 0) override;
		void DisconnectImpl() override;
		bool Disconnected() override;
		size_t GetSendQueueSize() override;
	};


	typedef struct http_parser http_parser;
	typedef struct http_parser_settings http_parser_settings;

	class UvHttpPeer : public UvTcpPeer
	{
	public:
		// 来自 libuv 的转换器及配置
		http_parser* parser;
		http_parser_settings* parser_settings;

		// 头部所有键值对
		xx::Dict<xx::String, xx::String> headers;

		// 正文
		xx::String body;

		// 原始 url 串
		xx::String url;

		// 当收到 key 时, 先往这 append. 出现 value 时再塞 headers
		xx::String lastKey;

		// 指向最后一次塞入 headers 的 value 以便 append
		xx::String* lastValue = nullptr;

		// 成功接收完一段信息时的回调.
		std::function<int()> OnMessageComplete;

		// 接收出错回调. 接着会发生 Release
		std::function<void(uint32_t errorNumber, char const* errorMessage)> OnError;

		// 原始数据( 如果不为空, 收到数据时将向它追加, 方便调试啥的 )
		xx::String_p rawData;

		UvHttpPeer(UvTcpListener& listener);
		~UvHttpPeer();

		virtual void ReceiveImpl(char const* const& bufPtr, int const& len) override;

		// todo: 提供更多返回内容的简单拼接下发
		inline static const char* responsePartialHeader_text = "HTTP/1.1 200 OK\r\n"	// 17
			"Content-Type: text/plain\r\n"	// 26
			"Content-Length: "	// 16
			; // 59

		void SendHttpResponse(char const* const& bufPtr, size_t const& len);
	};

	// 代码从 UvHttpPeer 复制小改
	class UvHttpClient : public UvTcpClient
	{
	public:
		// 来自 libuv 的转换器及配置
		http_parser * parser;
		http_parser_settings* parser_settings;

		// 头部所有键值对
		xx::Dict<xx::String, xx::String> headers;

		// 正文
		xx::String body;

		// 原始 url 串
		xx::String url;

		// 当收到 key 时, 先往这 append. 出现 value 时再塞 headers
		xx::String lastKey;

		// 指向最后一次塞入 headers 的 value 以便 append
		xx::String* lastValue = nullptr;

		// 成功接收完一段信息时的回调.
		std::function<int()> OnMessageComplete;

		// 接收出错回调. 接着会发生 Release
		std::function<void(uint32_t errorNumber, char const* errorMessage)> OnError;

		// 原始数据( 如果不为空, 收到数据时将向它追加, 方便调试啥的 )
		xx::String_p rawData;

		UvHttpClient(UvLoop& loop);
		~UvHttpClient();

		virtual void ReceiveImpl(char const* const& bufPtr, int const& len) override;
	};







	template<typename T>
	inline void UvTcpUdpBase::Send(T const & pkg)
	{
		//assert(pkg);
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
	inline uint32_t UvTcpUdpBase::SendRequest(T const & pkg, std::function<void(uint32_t, BBuffer*)>&& cb, int const& interval)
	{
		//assert(pkg);
		if (!loop.rpcMgr) throw - 1;
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
	inline void UvTcpUdpBase::SendResponse(uint32_t const& serial, T const & pkg)
	{
		//assert(pkg);
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
	inline void UvTcpUdpBase::SendRouting(char const* const& serviceAddr, size_t const& serviceAddrLen, T const & pkg)
	{
		//assert(pkg);
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
	inline uint32_t UvTcpUdpBase::SendRoutingRequest(char const* const& serviceAddr, size_t const& serviceAddrLen, T const & pkg, std::function<void(uint32_t, BBuffer*)>&& cb, int const& interval)
	{
		//assert(pkg);
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
	inline void UvTcpUdpBase::SendRoutingResponse(char const* const& serviceAddr, size_t const& serviceAddrLen, uint32_t const& serial, T const & pkg)
	{
		//assert(pkg);
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


	template<typename T>
	inline void UvTcpUdpBase::SendRequestEx(T const& pkg, std::function<void(uint32_t, xx::Object_p&)>&& cb, int const& interval)
	{
		auto serial = SendRequest(pkg, [this, cb = std::move(cb)](uint32_t ser, BBuffer* bb)
		{
			rpcSerials->Remove(ser);
			xx::Object_p inPkg;		// 如果 超时或 read 异常, inPkg 设空值
			if (bb)
			{
				if (bb->ReadRoot(inPkg))
				{
					inPkg.Reset();
				}
			}
			cb(ser, inPkg);	// call 原始 lambda
		}, interval);

		if (!rpcSerials)
		{
			rpcSerials.MPCreate(mempool);
		}
		rpcSerials->Add(serial);
	}


	using UvLoop_r = Ref<UvLoop>;
	using UvListenerBase_r = Ref<UvListenerBase>;
	using UvTcpListener_r = Ref<UvTcpListener>;
	using UvTcpUdpBase_r = Ref<UvTcpUdpBase>;
	using UvTcpBase_r = Ref<UvTcpBase>;
	using UvTcpPeer_r = Ref<UvTcpPeer>;
	using UvTcpClient_r = Ref<UvTcpClient>;
	using UvTimer_r = Ref<UvTimer>;
	using UvTimeouterBase_r = Ref<UvTimeouterBase>;
	using UvTimeouter_r = Ref<UvTimeoutManager>;
	using UvAsync_r = Ref<UvAsync>;
	using UvRpcManager_r = Ref<UvRpcManager>;
	using UvTimeouter_r = Ref<UvTimeoutManager>;
	using UvContextBase_r = Ref<UvContextBase>;
	using UvUdpListener_r = Ref<UvUdpListener>;
	using UvUdpBase_r = Ref<UvUdpBase>;
	using UvUdpPeer_r = Ref<UvUdpPeer>;
	using UvUdpClient_r = Ref<UvUdpClient>;

}
