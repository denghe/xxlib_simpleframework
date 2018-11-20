#pragma once
#include "xx.h"
#include <mutex>

// 重要: 除了 UvLoop, 其他类型只能以指针方式 Create 出来用. 否则将导致版本号检测变野失败. 所有回调都属于 noexcept, 如有异常, 需要自己 try

// 提示: IOS 下面可以使用 uvloop.GetIPList 解析域名可以令程序强制弹 网络权限窗

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
	class UvUdpListener;
	class UvUdpBase;
	class UvUdpPeer;
	class UvUdpClient;
	class UvHttpPeer;
	class UvHttpClient;

	using UvLoop_u = Unique<UvLoop>;
	using UvListenerBase_u = Unique<UvListenerBase>;
	using UvTcpListener_u = Unique<UvTcpListener>;
	using UvTcpUdpBase_u = Unique<UvTcpUdpBase>;
	using UvTcpBase_u = Unique<UvTcpBase>;
	using UvTcpPeer_u = Unique<UvTcpPeer>;
	using UvTcpClient_u = Unique<UvTcpClient>;
	using UvTimer_u = Unique<UvTimer>;
	using UvTimeouterBase_u = Unique<UvTimeouterBase>;
	using UvTimeouter_u = Unique<UvTimeoutManager>;
	using UvAsync_u = Unique<UvAsync>;
	using UvRpcManager_u = Unique<UvRpcManager>;
	using UvTimeouter_u = Unique<UvTimeoutManager>;
	using UvUdpListener_u = Unique<UvUdpListener>;
	using UvUdpBase_u = Unique<UvUdpBase>;
	using UvUdpPeer_u = Unique<UvUdpPeer>;
	using UvUdpClient_u = Unique<UvUdpClient>;


	using UvLoop_w = Weak<UvLoop>;
	using UvListenerBase_w = Weak<UvListenerBase>;
	using UvTcpListener_w = Weak<UvTcpListener>;
	using UvTcpUdpBase_w = Weak<UvTcpUdpBase>;
	using UvTcpBase_w = Weak<UvTcpBase>;
	using UvTcpPeer_w = Weak<UvTcpPeer>;
	using UvTcpClient_w = Weak<UvTcpClient>;
	using UvTimer_w = Weak<UvTimer>;
	using UvTimeouterBase_w = Weak<UvTimeouterBase>;
	using UvTimeouter_w = Weak<UvTimeoutManager>;
	using UvAsync_w = Weak<UvAsync>;
	using UvRpcManager_w = Weak<UvRpcManager>;
	using UvTimeouter_w = Weak<UvTimeoutManager>;
	using UvUdpListener_w = Weak<UvUdpListener>;
	using UvUdpBase_w = Weak<UvUdpBase>;
	using UvUdpPeer_w = Weak<UvUdpPeer>;
	using UvUdpClient_w = Weak<UvUdpClient>;


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

	class UvDnsVisitor : public Object
	{
	public:
		UvLoop& loop;
		String_p domainName;
		int indexAtDict = -1;
		std::function<void(List<String_p>*)> cb;
		xx::Weak<UvTimer> timeouter;
		List<String_p> results;

		void* hints = nullptr;
		void* resolver = nullptr;
		static void OnResolvedCBImpl(void *resolver, int status, void *res);

		UvDnsVisitor(UvLoop* const& loop, String_p& domainName, std::function<void(List<String_p>*)>&& cb, int timeoutMS = 0);
		~UvDnsVisitor();
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
		Dict<String_p, UvDnsVisitor*> dnsVisitors;
		UvTimeoutManager* timeoutManager = nullptr;
		UvRpcManager* rpcMgr = nullptr;
		UvTimer* udpTimer = nullptr;
		uint32_t udpTicks = 0;
		std::array<char, 65536> udpRecvBuf;
		uint32_t kcpInterval = 0;

		explicit UvLoop(MemPool* const& mp);
		~UvLoop() noexcept;

		int InitPeerTimeoutManager(uint64_t const& intervalMS = 1000, int const& wheelLen = 6, int const& defaultInterval = 5) noexcept;
		int InitRpcTimeoutManager(uint64_t const& rpcIntervalMS = 1000, int const& rpcDefaultInterval = 5) noexcept;
		int InitKcpFlushInterval(uint32_t const& interval = 10) noexcept;

		int Run(UvRunMode const& mode = UvRunMode::Default) noexcept;
		void Stop() noexcept;
		bool Alive() const noexcept;


		// 根据域名得到 ip 列表. 超时触发空值回调. 如果反复针对相同域名发起查询, 且上次的查询还没触发回调, 将返回 false.
		bool GetIPList(char const* const& domainName, std::function<void(List<String_p>*)>&& cb, int timeoutMS = 0);


		// 延迟执行, 以实现执行 需要出了当前函数才能执行的代码. 本质是 timeoutMS, 0 的 timer, 函数执行过后 timer 将自杀. 如果 timer 创建失败将返回非 0.
		int DelayExecute(std::function<void()>&& func, int const& timeoutMS = 0) noexcept;

		// 创建一个 tcp client 并解析域名 & 连接指定端口. 多 ip 域名将返回最快连上的. 超时时间可能因域名解析而比指定的要长. 不会超过两倍
		// 如果域名解析失败, 所有ip全都连不上, 超时, 回调将传入空.
		// domainName 也可以直接就是一个 ip. 这样会达到在 ipv6 协议栈下自动转换 ip 格式的目的
		// 同时, 对 apple 手机应用来讲, 调用本函数 或 GetIPList 可达到弹出网络权限请求面板的效果
		// 如果反复针对相同域名发起查询, 且上次的查询还没触发回调, 将返回 false.
		bool CreateTcpClientEx(char const* const& domainName, int const& port, std::function<void(UvTcpClient_w)>&& cb, int const& timeoutMS = 0) noexcept;

		// 这组 create 都是建一个初始对象
		UvTcpListener_w CreateTcpListener() noexcept;
		UvTcpClient_w CreateTcpClient() noexcept;
		UvUdpListener_w CreateUdpListener() noexcept;
		UvUdpClient_w CreateUdpClient() noexcept;
		UvTimer_w CreateTimer(uint64_t const& timeoutMS, uint64_t const& repeatIntervalMS, std::function<void()>&& OnFire = nullptr) noexcept;
		UvAsync_w CreateAsync() noexcept;
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
		std::function<void(UvTcpPeer_w)> OnAccept;
		List<UvTcpPeer*> peers;

		UvTcpListener(UvLoop& loop);
		~UvTcpListener() noexcept;

		static void OnAcceptCB(void* server, int status) noexcept;
		int Bind(char const* const& ipv4, int const& port) noexcept;
		int Bind6(char const* const& ipv6, int const& port) noexcept;
		int Listen(int const& backlog = 128) noexcept;
	};

	class UvTimeouterBase : public Object
	{
	public:
		UvTimeouterBase(MemPool* const& mp);
		~UvTimeouterBase() noexcept;
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
		// 可以随便存点啥, 减少对继承的需求
		Object_p userObject;
		void* userData = nullptr;
		int64_t userNumber = 0;

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

		// 返回 <0 表示失败, 0 成功
		template<typename T>
		int Send(T const& pkg) noexcept;

		// 返回 0 表示失败, 非 0 为本次生成的 serial
		template<typename T>
		uint32_t SendRequest(T const& pkg, std::function<void(uint32_t, BBuffer*)>&& cb, int const& interval = 0) noexcept;

		// 返回 <0 表示失败, 0 成功
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
		uint32_t SendRequestEx(T const& pkg, std::function<void(Object_p&)>&& cb, int const& interval = 0) noexcept;

		// 清除掉 OnReceiveXxxxxx, OnDispose 的各种事件
		void ClearHandlers() noexcept;

		// 会清除掉 OnReceiveXxxxxx, OnDispose 的各种事件, BindTimeoutManager 并在 OnTimeout 时 Release
		void DelayRelease(int const& interval = 0, bool const& clearHandlers = false) noexcept;
	};

	class UvTcpBase : public UvTcpUdpBase
	{
	public:
		UvTcpBase(UvLoop& loop);

		// 存储最后一次发送的数据的指针及长度( 便于群发 )
		std::pair<char const*, int> lastSendData;

		size_t GetSendQueueSize() noexcept override;
		int SendBytes(char const* const& inBuf, int const& len = 0) noexcept override;
		// todo: SendBytes 支持传入 BBuffer_p 以利于群发, 支持直接拿走 bb 的内存免 copy

		static void OnReadCBImpl(void* stream, ptrdiff_t nread, const void* buf_t) noexcept;
	};

	class UvTcpPeer : public UvTcpBase
	{
	public:
		UvTcpListener & listener;
		UvTcpPeer(UvTcpListener& listener);
		~UvTcpPeer() noexcept;
		void DisconnectImpl() noexcept override;
		bool Disconnected() noexcept override;
		std::array<char, 64> ipBuf;
		const char* Ip(bool includePort = true) noexcept;
	};

	class UvTcpClient : public UvTcpBase
	{
	public:
		std::function<void(int)> OnConnect;	// int state 为 0 表示连接成功, 非 0 表示错误码
		std::function<void()> OnDisconnect;	// 唯有 state 从 Connected 变为 Disconnected 时才触发. 包括自己执行 Disconnect 函数. 要小心逻辑递归
		xx::Weak<UvTimer> connTimeouter;	// 超时 cancel 后 uv 还是会产生 OnConnectCBImpl 的回调
		UvTcpStates state = UvTcpStates::Disconnected;

		void* req = nullptr;			// 延迟删除
		UvTcpClient(UvLoop& loop);
		~UvTcpClient() noexcept;
		bool Alive() const noexcept;	// state == UvTcpStates::Connected, 并不能取代外部野指针检测
		int SetAddress(char const* const& ipv4, int const& port) noexcept;
		int SetAddress6(char const* const& ipv6, int const& port) noexcept;
		static void OnConnectCBImpl(void* req, int status) noexcept;
		int Connect(int const& timeoutMS = 0) noexcept;
		int ConnectEx(char const* const& ipv4, int const& port, int const& timeoutMS = 0) noexcept;	// 等同于 SetAddress + Connect
		int Connect6Ex(char const* const& ipv6, int const& port, int const& timeoutMS = 0) noexcept;	// 等同于 SetAddress6 + Connect
		void Disconnect(bool runCallback = true) noexcept;
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
		~UvTimer() noexcept;
		static void OnTimerCBImpl(void* handle) noexcept;
		void SetRepeat(uint64_t const& repeatIntervalMS) noexcept;
		int Again() noexcept;
		int Stop() noexcept;
	};

	class UvTimeoutManager : public Object
	{
	public:
		UvTimer_w timer;
		List<UvTimeouterBase*> timeouterss;
		int cursor = 0;
		int defaultInterval;
		UvTimeoutManager(UvLoop& loop, uint64_t const& intervalMS, int const& wheelLen, int const& defaultInterval);
		~UvTimeoutManager() noexcept;
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
		~UvAsync() noexcept;
		static void OnAsyncCBImpl(void* handle) noexcept;
		int Dispatch(std::function<void()>&& a) noexcept;
		void OnFireImpl() noexcept;
	};

	class UvRpcManager : public Object
	{
	public:
		UvTimer_w timer;
		uint32_t serial = 0;
		Dict<uint32_t, std::function<void(uint32_t, BBuffer*)>> mapping;
		Queue<std::pair<int, uint32_t>> serials;
		int defaultInterval = 0;
		int ticks = 0;
		UvRpcManager(UvLoop& loop, uint64_t const& intervalMS, int const& defaultInterval);
		~UvRpcManager() noexcept;
		void Process() noexcept;
		uint32_t Register(std::function<void(uint32_t, BBuffer*)>&& cb, int interval = 0) noexcept;
		void Unregister(uint32_t const& serial) noexcept;
		void Callback(uint32_t const& serial, BBuffer* const& bb) noexcept;
		size_t Count() noexcept;
	};

	class UvUdpListener : public UvListenerBase
	{
	public:
		std::function<UvUdpPeer*(Guid const&)> OnCreatePeer;
		std::function<void(UvUdpPeer_w)> OnAccept;
		Dict<Guid, UvUdpPeer*> peers;

		UvUdpListener(UvLoop& loop);
		~UvUdpListener() noexcept;
		static void OnRecvCBImpl(void* udp, ptrdiff_t nread, void* buf_t, void* addr, uint32_t flags) noexcept;
		void OnReceiveImpl(char const* const& bufPtr, int const& len, void* const& addr) noexcept;

		int Bind(char const* const& ipv4, int const& port) noexcept;
		int Bind6(char const* const& ipv6, int const& port) noexcept;
		int Listen() noexcept;
		int StopListen() noexcept;

		UvUdpPeer_w CreatePeer(Guid const& g
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
		~UvUdpPeer() noexcept;

		static int OutputImpl(char const* buf, int len, void* kcp) noexcept;
		void Update(uint32_t const& current) noexcept;
		int Input(char const* const& data, int const& len) noexcept;
		int SendBytes(char const* const& data, int const& len = 0) noexcept override;
		void DisconnectImpl() noexcept override;
		size_t GetSendQueueSize() noexcept override;
		bool Disconnected() noexcept override;

		std::array<char, 64> ipBuf;
		const char* Ip(bool includePort = true) noexcept;
	};

	class UvUdpClient : public UvUdpBase
	{
	public:
		void* kcpPtr = nullptr;
		UvUdpClient(UvLoop& loop);
		~UvUdpClient() noexcept;

		int Connect(Guid const& guid
			, int const& sndwnd = 128, int const& rcvwnd = 128
			, int const& nodelay = 1/*, int const& interval = 10*/, int const& resend = 2, int const& nc = 1, int const& minrto = 100) noexcept;

		static void OnRecvCBImpl(void* udp, ptrdiff_t nread, void* buf_t, void* addr, uint32_t flags) noexcept;
		void OnReceiveImpl(char const* const& bufPtr, int const& len, void* const& addr) noexcept;
		static int OutputImpl(char const* buf, int len, void* kcp) noexcept;
		void Update(uint32_t const& current) noexcept;
		void Disconnect() noexcept;
		int SetAddress(char const* const& ipv4, int const& port) noexcept;
		int SetAddress6(char const* const& ipv6, int const& port) noexcept;
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
		http_parser_settings* parser_settings = nullptr;
		http_parser* parser = nullptr;

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
		~UvHttpPeer() noexcept;

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
		http_parser* parser = nullptr;

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
		~UvHttpClient() noexcept;

		virtual void ReceiveImpl(char const* const& bufPtr, int const& len) noexcept override;

		// todo: 提供更多请求串拼接函数 以便于使用
	};





}

#include "xx_uv.hpp"
