#pragma once
#include "xx.h"
#include <mutex>

// 注意: 除了 UvLoop, 其他类型只能以指针方式 Create 出来用. 否则将导致版本号检测变野失败. 所有回调都属于 noexcept, 如有异常, 需要自己 try

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

		int InitTimeoutManager(uint64_t const& intervalMS = 1000, int const& wheelLen = 6, int const& defaultInterval = 5) noexcept;
		int InitRpcManager(uint64_t const& rpcIntervalMS = 1000, int const& rpcDefaultInterval = 5) noexcept;
		int InitKcpFlushInterval(uint32_t const& interval = 10) noexcept;

		int Run(UvRunMode const& mode = UvRunMode::Default) noexcept;
		void Stop() noexcept;
		bool Alive() const noexcept;

		UvTcpListener* CreateTcpListener() noexcept;
		UvTcpClient* CreateTcpClient() noexcept;
		UvUdpListener* CreateUdpListener() noexcept;
		UvUdpClient* CreateUdpClient() noexcept;
		UvTimer* CreateTimer(uint64_t const& timeoutMS, uint64_t const& repeatIntervalMS, std::function<void()>&& OnFire = nullptr) noexcept;
		UvAsync* CreateAsync() noexcept;
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

		static void OnAcceptCB(void* server, int status) noexcept;
		int Bind(char const* const& ipv4, int const& port) noexcept;
		int Listen(int const& backlog = 128) noexcept;
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

		void TimeouterClear() noexcept;
		void TimeoutReset(int const& interval = 0) noexcept;
		void TimeouterStop() noexcept;
		virtual void BindTimeoutManager(UvTimeoutManager* const&) noexcept;
		void UnbindTimeoutManager() noexcept;
		bool Timeouting() noexcept;
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
		String routingAddress;
		String senderAddress;

		// (BBuffer& bb, size_t pkgOffset, size_t pkgLen, size_t addrOffset, size_t addrLen)
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
		HashSet_p<uint32_t> rpcSerials;


		UvTcpUdpBase(UvLoop& loop);

		virtual void BindTimeoutManager(UvTimeoutManager* const& t = nullptr) noexcept;

		virtual void DisconnectImpl() noexcept = 0;
		virtual bool Disconnected() noexcept = 0;
		virtual size_t GetSendQueueSize() noexcept = 0;
		virtual int SendBytes(char const* const& inBuf, int const& len = 0) noexcept = 0;

		virtual void ReceiveImpl(char const* const& bufPtr, int const& len) noexcept;

		int SendBytes(BBuffer& bb) noexcept;



		// 三种常用 Send 函数

		template<typename T>
		int Send(T const& pkg) noexcept;

		template<typename T>
		uint32_t SendRequest(T const& pkg, std::function<void(uint32_t, BBuffer*)>&& cb, int const& interval = 0) noexcept;

		template<typename T>
		int SendResponse(uint32_t const& serial, T const& pkg) noexcept;



		// 下面是上面 3 个 Send 的 Routing 版( 客户端, 非路由服务端专用 )

		// 向路由服务发包
		template<typename T>
		int SendRouting(char const* const& serviceAddr, size_t const& serviceAddrLen, T const& pkg) noexcept;

		// 向路由服务发请求
		template<typename T>
		uint32_t SendRoutingRequest(char const* const& serviceAddr, size_t const& serviceAddrLen, T const& pkg, std::function<void(uint32_t, BBuffer*)>&& cb, int const& interval = 0) noexcept;

		// 向路由服务发回应
		template<typename T>
		int SendRoutingResponse(char const* const& serviceAddr, size_t const& serviceAddrLen, uint32_t const& serial, T const& pkg) noexcept;



		// 下面是路由服务专用

		// 纯下发地址. 
		int SendRoutingAddress(char const* const& buf, size_t const& len) noexcept;

		// 读出上面函数下发的地址长度
		static size_t GetRoutingAddressLength(BBuffer& bb) noexcept;

		// 在不解析数据的情况下直接替换地址部分转发( 路由专用 )
		int SendRoutingByRouter(BBuffer& bb, size_t const& pkgLen, size_t const& addrOffset, size_t const& addrLen, char const* const& senderAddr, size_t const& senderAddrLen) noexcept;



		// 超时回调所有被跟踪 rpc 流水号并清空( 内部函数. 会自动在 OnDispose, OnDisconnect 事件前调用以触发超时回调 )
		void RpcTraceCallback() noexcept;

		// 增强的 SendRequest 实现 断线时 立即发起相关 rpc 超时回调. 封装了解包操作. 
		template<typename T>
		void SendRequestEx(T const& pkg, std::function<void(uint32_t, Object_p&)>&& cb, int const& interval = 0) noexcept;

		// 会清除掉 OnReceiveXxxxxx, OnDispose 的各种事件, BindTimeoutManager 并在 OnTimeout 时 Release
		void DelayRelease(int const& interval = 0) noexcept;
	};

	class UvTcpBase : public UvTcpUdpBase
	{
	public:
		UvTcpBase(UvLoop& loop);

		size_t GetSendQueueSize() noexcept override;
		int SendBytes(char const* const& inBuf, int const& len = 0) noexcept override;

		static void OnReadCBImpl(void* stream, ptrdiff_t nread, const void* buf_t) noexcept;
	};

	class UvTcpPeer : public UvTcpBase
	{
	public:
		UvTcpListener & listener;
		UvTcpPeer(UvTcpListener& listener);
		~UvTcpPeer();	// Release 取代 Dispose
		void DisconnectImpl() noexcept override;
		bool Disconnected() noexcept override;
		std::array<char, 64> ipBuf;
		const char* Ip() noexcept;
	};

	class UvTcpClient : public UvTcpBase
	{
	public:
		std::function<void(int)> OnConnect;
		std::function<void()> OnDisconnect;

		UvTcpStates state = UvTcpStates::Disconnected;
		UvTcpClient(UvLoop& loop);
		~UvTcpClient();	// Release 取代 Dispose
		bool Alive() const noexcept;	// state == UvTcpStates::Connected, 并不能取代外部野指针检测
		int SetAddress(char const* const& ipv4, int const& port) noexcept;
		static void OnConnectCBImpl(void* req, int status) noexcept;
		int Connect() noexcept;
		int ConnectEx(char const* const& Ip, int const& port) noexcept;	// 等同于 Disconnect + SetAddress + Connect
		void Disconnect() noexcept;
		void DisconnectImpl() noexcept override;
		bool Disconnected() noexcept override;
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
		static void OnTimerCBImpl(void* handle) noexcept;
		void SetRepeat(uint64_t const& repeatIntervalMS) noexcept;
		int Again() noexcept;
		int Stop() noexcept;
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
		void Process() noexcept;
		void Clear() noexcept;
		void Add(UvTimeouterBase* const& t, int interval = 0) noexcept;
		void Remove(UvTimeouterBase* const& t) noexcept;
		void AddOrUpdate(UvTimeouterBase* const& t, int const& interval = 0) noexcept;
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
		static void OnAsyncCBImpl(void* handle) noexcept;
		int Dispatch(std::function<void()>&& a) noexcept;
		void OnFireImpl() noexcept;
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
		UvRpcManager(UvLoop& loop, uint64_t const& intervalMS, int const& defaultInterval);
		~UvRpcManager();
		void Process() noexcept;
		uint32_t Register(std::function<void(uint32_t, BBuffer*)>&& cb, int interval = 0) noexcept;
		void Unregister(uint32_t const& serial) noexcept;
		void Callback(uint32_t const& serial, BBuffer* const& bb) noexcept;
		size_t Count() noexcept;
	};

	// 暂不建议使用
	class UvContextBase : public Object
	{
	public:
		UvTcpUdpBase * peer = nullptr;

		UvContextBase(MemPool* const& mp);
		~UvContextBase();
		bool PeerAlive() noexcept;

		// 重要!! 如果该函数执行点位于 peer 之 OnXxxxx 中, 则在 Bind 成功之时, 先前的 lambda 上下文将被 "顶掉", 所有捕获将变"野", 故该函数只能于此类函数退出前执行
		bool BindPeer(UvTcpUdpBase* const& p) noexcept;
		void KickPeer(bool const& immediately = true) noexcept;
		void OnPeerReceiveRequest(uint32_t serial, BBuffer& bb) noexcept;
		void OnPeerReceivePackage(BBuffer& bb) noexcept;
		void OnPeerDisconnect() noexcept;
		virtual void HandleRequest(uint32_t serial, Ptr<Object>& o) noexcept = 0;
		virtual void HandlePackage(Ptr<Object>& o) noexcept = 0;
		virtual void HandleDisconnect() noexcept = 0;
	};

	class UvUdpListener : public UvListenerBase
	{
	public:
		std::function<UvUdpPeer*(Guid const&)> OnCreatePeer;
		std::function<void(UvUdpPeer*)> OnAccept;
		Dict<Guid, UvUdpPeer*> peers;

		UvUdpListener(UvLoop& loop);
		~UvUdpListener();
		static void OnRecvCBImpl(void* udp, ptrdiff_t nread, void* buf_t, void* addr, uint32_t flags) noexcept;
		void OnReceiveImpl(char const* const& bufPtr, int const& len, void* const& addr) noexcept;

		int Bind(char const* const& ipv4, int const& port) noexcept;
		int Listen() noexcept;
		int StopListen() noexcept;

		UvUdpPeer* CreatePeer(Guid const& g
			, int const& sndwnd = 128, int const& rcvwnd = 128
			, int const& nodelay = 1/*, int const& interval = 10*/, int const& resend = 2, int const& nc = 1, int const& minrto = 100) noexcept;
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

		static int OutputImpl(char const* buf, int len, void* kcp) noexcept;
		void Update(uint32_t const& current) noexcept;
		int Input(char const* const& data, int const& len) noexcept;
		int SendBytes(char const* const& data, int const& len = 0) noexcept override;
		void DisconnectImpl() noexcept override;
		size_t GetSendQueueSize() noexcept override;
		bool Disconnected() noexcept override;

		std::array<char, 64> ipBuf;
		char* Ip() noexcept;
	};

	class UvUdpClient : public UvUdpBase
	{
	public:
		void* kcpPtr = nullptr;
		UvUdpClient(UvLoop& loop);
		~UvUdpClient();

		int Connect(Guid const& guid
			, int const& sndwnd = 128, int const& rcvwnd = 128
			, int const& nodelay = 1/*, int const& interval = 10*/, int const& resend = 2, int const& nc = 1, int const& minrto = 100) noexcept;

		static void OnRecvCBImpl(void* udp, ptrdiff_t nread, void* buf_t, void* addr, uint32_t flags) noexcept;
		void OnReceiveImpl(char const* const& bufPtr, int const& len, void* const& addr) noexcept;
		static int OutputImpl(char const* buf, int len, void* kcp) noexcept;
		void Update(uint32_t const& current) noexcept;
		void Disconnect() noexcept;
		int SetAddress(char const* const& ipv4, int const& port) noexcept;
		int SendBytes(char const* const& data, int const& len = 0) noexcept override;
		void DisconnectImpl() noexcept override;
		bool Disconnected() noexcept override;
		size_t GetSendQueueSize() noexcept override;
	};


	typedef struct http_parser http_parser;
	typedef struct http_parser_settings http_parser_settings;

	class UvHttpPeer : public UvTcpPeer
	{
	public:
		// 来自 libuv 的转换器及配置
		http_parser_settings * parser_settings = nullptr;
		http_parser * parser = nullptr;

		// GET / POST / ...
		String method;

		// 头部所有键值对
		Dict<String, String> headers;

		// 是否保持连接
		bool keepAlive = false;

		// 正文
		String body;

		// 原始 url 串( 未 urldecode 解码 )
		String url;

		// url decode 后的结果 url 串, 同时也是 url parse 后的容器, 不可以修改内容
		String urlDecoded;

		// ParseUrl 后将填充下面三个属性
		char* path = nullptr;
		List<std::pair<char*, char*>> queries;	// 键值对
		char* fragment = nullptr;

		// 原始 status 串
		String status;

		// 当收到 key 时, 先往这 append. 出现 value 时再塞 headers
		String lastKey;

		// 指向最后一次塞入 headers 的 value 以便 append
		String* lastValue = nullptr;

		// 成功接收完一段信息时的回调.
		std::function<void()> OnReceiveHttp;

		// 接收出错回调. 接着会发生 Release
		std::function<void(uint32_t errorNumber, char const* errorMessage)> OnError;

		// 原始数据( 如果不为空, 收到数据时将向它追加, 方便调试啥的 )
		String_p rawData;

		// 可用于堆砌返回数据
		String s;

		UvHttpPeer(UvTcpListener& listener);
		~UvHttpPeer();

		virtual void ReceiveImpl(char const* const& bufPtr, int const& len) noexcept override;

		// todo: 提供更多返回内容的简单拼接下发
		inline static const std::string responsePartialHeader_text = 
			"HTTP/1.1 200 OK\r\n"
			"Content-Type: text/plain;charset=utf-8\r\n"		// text/json
			"Content-Length: "
			;

		// 发送 buf
		void SendHttpResponse(char const* const& bufPtr, size_t const& len) noexcept;

		// 会发送 o ToString 后的结果
		template<typename T>
		void SendHttpResponse(T const& o) noexcept;

		void SendHttpResponse() noexcept;	// 会发送 s 的内容

		// 会 urldecode 并 填充 path, queries, fragment
		void ParseUrl() noexcept;
	};

	// 代码从 UvHttpPeer 复制小改
	class UvHttpClient : public UvTcpClient
	{
	public:
		// 来自 libuv 的转换器及配置
		http_parser_settings* parser_settings = nullptr;
		http_parser * parser = nullptr;

		// 头部所有键值对
		Dict<String, String> headers;

		// 是否保持连接
		bool keepAlive = false;

		// 正文
		String body;

		// 原始 url 串
		String url;

		// 原始 status 串
		String status;

		// 当收到 key 时, 先往这 append. 出现 value 时再塞 headers
		String lastKey;

		// 指向最后一次塞入 headers 的 value 以便 append
		String* lastValue = nullptr;

		// 成功接收完一段信息时的回调.
		std::function<void()> OnReceiveHttp;

		// 接收出错回调. 接着会发生 Release
		std::function<void(uint32_t errorNumber, char const* errorMessage)> OnError;

		// 原始数据( 如果不为空, 收到数据时将向它追加, 方便调试啥的 )
		String_p rawData;

		UvHttpClient(UvLoop& loop);
		~UvHttpClient();

		virtual void ReceiveImpl(char const* const& bufPtr, int const& len) noexcept override;

		// todo: 提供更多请求串拼接函数 以便于使用
	};







	template<typename T>
	inline int UvTcpUdpBase::Send(T const & pkg) noexcept
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
			return SendBytes(p, (int)(dataLen + 3));
		}
		else
		{
			auto p = bbSend.buf;
			p[0] = 0b00000100;
			p[1] = (uint8_t)dataLen;
			p[2] = (uint8_t)(dataLen >> 8);
			p[3] = (uint8_t)(dataLen >> 16);
			p[4] = (uint8_t)(dataLen >> 24);
			return SendBytes(p, (int)(dataLen + 5));
		}
	}

	template<typename T>
	inline uint32_t UvTcpUdpBase::SendRequest(T const & pkg, std::function<void(uint32_t, BBuffer*)>&& cb, int const& interval) noexcept
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
	inline int UvTcpUdpBase::SendResponse(uint32_t const& serial, T const & pkg) noexcept
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
			return endBytes(p, (int)(dataLen + 3));
		}
		else
		{
			auto p = bbSend.buf;
			p[0] = 0b00000110;											// 这里标记包头为 Big + Response 类型
			p[1] = (uint8_t)dataLen;
			p[2] = (uint8_t)(dataLen >> 8);
			p[3] = (uint8_t)(dataLen >> 16);
			p[4] = (uint8_t)(dataLen >> 24);
			return SendBytes(p, (int)(dataLen + 5));
		}
	}


	template<typename T>
	inline int UvTcpUdpBase::SendRouting(char const* const& serviceAddr, size_t const& serviceAddrLen, T const & pkg) noexcept
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
			return SendBytes(p, (int)(dataLen + 3));
		}
		else
		{
			auto p = bbSend.buf;
			p[0] = (uint8_t)(0b00001100 | ((serviceAddrLen - 1) << 4));	// 拼接为 XXXX1100 的含长度信息的 Big 路由包头
			p[1] = (uint8_t)dataLen;
			p[2] = (uint8_t)(dataLen >> 8);
			p[3] = (uint8_t)(dataLen >> 16);
			p[4] = (uint8_t)(dataLen >> 24);
			return SendBytes(p, (int)(dataLen + 5));
		}
	}

	template<typename T>
	inline uint32_t UvTcpUdpBase::SendRoutingRequest(char const* const& serviceAddr, size_t const& serviceAddrLen, T const & pkg, std::function<void(uint32_t, BBuffer*)>&& cb, int const& interval) noexcept
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
	inline int UvTcpUdpBase::SendRoutingResponse(char const* const& serviceAddr, size_t const& serviceAddrLen, uint32_t const& serial, T const & pkg) noexcept
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
			return SendBytes(p, (int)(dataLen + 3));
		}
		else
		{
			auto p = bbSend.buf;
			p[0] = (uint8_t)(0b00001110 | ((serviceAddrLen - 1) << 4));	// 这里标记包头为 addrLen + Routing + Big + Response 类型
			p[1] = (uint8_t)dataLen;
			p[2] = (uint8_t)(dataLen >> 8);
			p[3] = (uint8_t)(dataLen >> 16);
			p[4] = (uint8_t)(dataLen >> 24);
			return SendBytes(p, (int)(dataLen + 5));
		}
	}


	template<typename T>
	inline void UvTcpUdpBase::SendRequestEx(T const& pkg, std::function<void(uint32_t, Object_p&)>&& cb, int const& interval) noexcept
	{
		auto serial = SendRequest(pkg, [this, cb = std::move(cb)](uint32_t ser, BBuffer* bb)
		{
			rpcSerials->Remove(ser);
			Object_p inPkg;		// 如果 超时或 read 异常, inPkg 设空值
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




	template<typename T>
	inline void UvHttpPeer::SendHttpResponse(T const& o) noexcept
	{
		s.Assign(o);
		SendHttpResponse();
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
