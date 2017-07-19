// xxuvlib_csharp.h

#pragma once

#pragma unmanaged
#include <xx_uv.h>
#include <xx_helpers.h>


/********************************************************************************************************************/
// MyUVAsync
/********************************************************************************************************************/

// 可创建多份的 async 跨线程信号量. 使用 UVWrapper 的 CreateAsync 来创建. 

typedef void(*Func_OnFire)();
struct MyUVAsync : xx::UVAsync
{
	// 创建后填充
	Func_OnFire onFire = nullptr;

	MyUVAsync(xx::UV* uv) : xx::UVAsync(uv) {}

	inline virtual void OnFire() override
	{
		assert(onFire);
		(*onFire)();
	}
};


/********************************************************************************************************************/
// MyUVTimer
/********************************************************************************************************************/

// 可创建多份的 timer. 使用 UVWrapper 的 CreateTimer 来创建. Start 启动. Release 析构.

typedef void(*Func_OnTicks)();
struct MyUVTimer : xx::UVTimer
{
	// 创建后填充
	Func_OnTicks onTicks = nullptr;

	MyUVTimer(xx::UV* uv) : xx::UVTimer(uv) {}

	inline virtual void OnFire() override
	{
		assert(onTicks);
		(*onTicks)();
	}
};


/********************************************************************************************************************/
// MyUVClientPeer
/********************************************************************************************************************/

// 可创建多份的 client peer. 使用 UVWrapper 的 CreateClientPeer 来创建

typedef void(*Func_OnReceivePackage)(char* buf, int dataLen);
typedef void(*Func_OnConnect)();
typedef void(*Func_OnDisconnect)();
struct MyUVClientPeer : xx::UVClientPeer
{
	// 创建后填充
	Func_OnReceivePackage onReceivePackage = nullptr;
	Func_OnConnect onConnect = nullptr;
	Func_OnDisconnect onDisconnect = nullptr;

	MyUVClientPeer(xx::UV* uv) : xx::UVClientPeer(uv) {}

	void OnReceivePackage(xx::BBuffer& bb) override
	{
		assert(onReceivePackage);
		(*onReceivePackage)(bb.buf, (int)bb.dataLen);
	}

	void OnConnect() override
	{
		assert(onConnect);
		(*onConnect)();
	}

	void OnDisconnect() override
	{
		this->xx::UVClientPeer::OnDisconnect();
		assert(onDisconnect);
		(*onDisconnect)();
	}
};



/********************************************************************************************************************/
// MyUVListener
/********************************************************************************************************************/

// 可创建多份的 listener. 使用 UVWrapper 的 CreateListener 来创建

typedef xx::UVServerPeer*(*Func_OnCreatePeer)();
struct MyUVListener : xx::UVListener
{
	// 创建后填充
	Func_OnCreatePeer onCreatePeer = nullptr;

	MyUVListener(xx::UV* uv, int port, int backlog) : xx::UVListener(uv, port, backlog) {}

	xx::UVServerPeer* OnCreatePeer() override
	{
		assert(onCreatePeer);
		return (*onCreatePeer)();
	}
};



/********************************************************************************************************************/
// MyUVServerPeer
/********************************************************************************************************************/

// 可创建多份的 listener. 使用 UVWrapper 的 CreateListener 来创建

struct MyUVServerPeer;
typedef void(*Func_OnAccept)(char* addr, int addrLen);
struct MyUVServerPeer : xx::UVServerPeer
{
	// 创建后填充
	Func_OnDisconnect onDisconnect = nullptr;
	Func_OnReceivePackage onReceivePackage = nullptr;

	MyUVServerPeer(MyUVListener* listener) : xx::UVServerPeer(listener)
	{
		GetPeerName();	// 填充 peer name 到 tmpStr, 以便下面的 C# 代码于创建之后模拟 Accept 事件时传参
	}

	void OnReceivePackage(xx::BBuffer& bb) override
	{
		assert(onReceivePackage);
		(*onReceivePackage)(bb.buf, (int)bb.dataLen);
	}

	void OnDisconnect() override
	{
		assert(onDisconnect);
		(*onDisconnect)();
	}
};










#pragma managed

// 交换删除法
#define SYSTEM_LIST_SWAP_REMOVE( listPtr, tarPtr, indexName )	\
do																\
{																\
	auto count_1 = listPtr->Count - 1;							\
	if (count_1 != tarPtr->indexName)							\
	{															\
		listPtr[tarPtr->indexName] = listPtr[count_1];			\
		listPtr[count_1]->indexName = tarPtr->indexName;		\
	}															\
	listPtr->RemoveAt(count_1);									\
																\
} while (false)


using namespace System;
using namespace System::Collections::Generic;
using namespace System::Runtime::InteropServices;


/********************************************************************************************************************/
// MPtrWrapper
/********************************************************************************************************************/

public ref class MPtrWrapper abstract
{
protected:
	System::IntPtr^ ptr = nullptr;
	uint64_t versionNumber = 0;
public:

	virtual void InitPtr(xx::MPObject* ptr)
	{
		this->ptr = gcnew System::IntPtr(ptr);
		versionNumber = ptr->versionNumber();
	}

	bool Ensure()
	{
		return versionNumber != 0 && ptr->ToPointer() != nullptr && ((xx::MPObject*)ptr->ToPointer())->versionNumber() == versionNumber;
	}

	void AssertEnsure()
	{
		System::Diagnostics::Debug::Assert(versionNumber != 0 && ptr->ToPointer() != nullptr && ((xx::MPObject*)ptr->ToPointer())->versionNumber() == versionNumber);
	}

	~MPtrWrapper()
	{
		AssertEnsure();
		((xx::MPObject*)ptr->ToPointer())->Release();
		this->ptr = nullptr;
		versionNumber = 0;
	}
};
ref class UVWrapper;

/********************************************************************************************************************/
// UVClientPeerWrapper
/********************************************************************************************************************/

public enum class UVPeerStates
{
	Disconnected,
	Connecting,
	Connected,
	Disconnecting
};

public delegate void(Delegate_OnConnect)();
public delegate void(Delegate_OnDisconnect)();
public delegate void(Delegate_OnReceivePackage)(char* buf, int dataLen);

public ref class UVPeerWrapper abstract : MPtrWrapper
{
protected:
	Delegate_OnConnect^ onConnect;
	Delegate_OnDisconnect^ onDisconnect;
	Delegate_OnReceivePackage^ onReceivePackage;
	void OnReceivePackageCore(char* buf, int dataLen)
	{
		System::IntPtr bufPtr(buf);
		auto data = gcnew array<System::Byte>(dataLen);
		Marshal::Copy(bufPtr, data, 0, dataLen);
		OnReceivePackage(data);
	}
public:
	virtual void OnConnect() = 0;
	virtual void OnDisconnect() = 0;
	virtual void OnReceivePackage(array<Byte>^ data) = 0;

	UVPeerWrapper()
	{
		onReceivePackage = gcnew Delegate_OnReceivePackage(this, &UVPeerWrapper::OnReceivePackageCore);
		onConnect = gcnew Delegate_OnConnect(this, &UVPeerWrapper::OnConnect);
		onDisconnect = gcnew Delegate_OnDisconnect(this, &UVPeerWrapper::OnDisconnect);
	}
	//~UVPeerWrapper();

	virtual void InitPtr(xx::MPObject* ptr) override = 0;

	int Disconnect(bool immediately)
	{
		AssertEnsure();
		auto p = (MyUVServerPeer*)ptr->ToPointer();
		if (p->state != xx::UVPeerStates::Connected) return -1;
		p->Disconnect(immediately);
		return 0;
	}
	bool Send(array<System::Byte>^ buf, int offset, int dataLen)
	{
		if (buf->Length < offset + dataLen) throw gcnew System::IndexOutOfRangeException();
		AssertEnsure();
		auto p = (MyUVServerPeer*)ptr->ToPointer();
		if (p->state != xx::UVPeerStates::Connected) return false;

		// 获得一个发送用的 bb( 可能里面已经有数据了 )
		auto bb = p->GetSendBB(dataLen);

		// 复制追加. 未来考虑对外暴露 BBuffer 原生对象 从而实现 0 copy
		pin_ptr<System::Byte> pin = &buf[0];
		unsigned char* pinbuf = pin;
		bb->WriteBuf((char*)pinbuf + offset, dataLen);

		// 发出( 压到发送队列 )
		return p->Send(bb) == 0;
	}
	property UVPeerStates State
	{
		UVPeerStates get()
		{
			AssertEnsure();
			auto p = (MyUVServerPeer*)ptr->ToPointer();
			return (UVPeerStates)(int)p->state;
		}
	}
	property String^ PeerName
	{
		String^ get()
		{
			AssertEnsure();
			auto p = (MyUVServerPeer*)ptr->ToPointer();
			auto& s = p->GetPeerName();
			return gcnew String((const char*)s.buf, 0, s.dataLen);
		}
	}
	// 这个属性可不需要 Ensure 直接用 以方便 判断 peer 的状态
	property bool Alive
	{
		bool get()
		{
			return Ensure() && State == UVPeerStates::Connected;
		}
	}
};



public ref class UVClientPeerWrapper abstract : UVPeerWrapper
{
public:
	UVWrapper^ creator = nullptr;				// 加持以防 uv 比 this 死的早

	// 位于父容器的下标
	int uv_clientPeers_index = -1;

	~UVClientPeerWrapper();

	virtual void InitPtr(xx::MPObject* ptr) override;

	int SetAddress(String^ ip, int port)
	{
		if (String::IsNullOrWhiteSpace(ip)) return -1;
		AssertEnsure();
		auto p = (MyUVClientPeer*)ptr->ToPointer();
		auto ipPtr = Marshal::StringToHGlobalAnsi(ip);
		return p->SetAddress((const char*)ipPtr.ToPointer(), port);
	}

	int Connect()
	{
		AssertEnsure();
		auto p = (MyUVClientPeer*)ptr->ToPointer();
		return p->Connect();
	}
};


/********************************************************************************************************************/
// UVServerPeerWrapper
/********************************************************************************************************************/

ref class UVListenerWrapper;
public ref class UVServerPeerWrapper abstract : UVPeerWrapper
{
public:
	UVListenerWrapper^ creator = nullptr;				// 加持以防 listener 比 this 死的早

	// 位于父容器的下标
	int listener_serverPeers_index = -1;

	~UVServerPeerWrapper();

	virtual void InitPtr(xx::MPObject* ptr) override;
};

/********************************************************************************************************************/
// UVListenerWrapper
/********************************************************************************************************************/

public delegate xx::UVServerPeer*(Delegate_OnCreatePeer)();
public ref class UVListenerWrapper abstract : MPtrWrapper
{
public:
	UVWrapper^ creator = nullptr;				// 加持以防 uv 比 this 死的早
	System::Collections::Generic::List<UVServerPeerWrapper^>^ serverPeers;
protected:
	Delegate_OnCreatePeer^ onCreatePeer;
	xx::UVServerPeer* OnCreatePeerCore()
	{
		auto spWrapper = OnCreatePeer();
		System::Diagnostics::Debug::Assert(spWrapper != nullptr);
		auto spPtr = (xx::UVServerPeer*)ptr->ToPointer();
		System::Diagnostics::Debug::Assert(spPtr != nullptr);
		return spPtr;
	}
public:
	virtual UVServerPeerWrapper^ OnCreatePeer() = 0;	// return CreateServerPeer<T>();

	// 位于父容器的下标
	int uv_listeners_index = -1;

	UVListenerWrapper()
	{
		onCreatePeer = gcnew Delegate_OnCreatePeer(this, &UVListenerWrapper::OnCreatePeerCore);
		serverPeers = gcnew List<UVServerPeerWrapper^>();
	}
	~UVListenerWrapper();
	virtual void InitPtr(xx::MPObject* ptr) override;

	// 仅用作 OnCreatePeer 内部创建 server peer 返回
	generic<typename T> where T : UVServerPeerWrapper
		T CreateServerPeer(T wrapper)
	{
		System::Diagnostics::Debug::Assert(wrapper->creator == nullptr);
		wrapper->creator = this;
		auto t = ((xx::MPObject*)ptr->ToPointer())->mempool().Create<MyUVServerPeer>((MyUVListener*)ptr->ToPointer());
		if (t == nullptr) return T();

		wrapper->InitPtr(t);
		wrapper->OnConnect();
		return wrapper;
	}
	generic<typename T> where T : UVServerPeerWrapper, gcnew()
		T CreateServerPeer()
	{
		return CreateServerPeer(gcnew T());
	}
};


/********************************************************************************************************************/
// UVTimerWrapper
/********************************************************************************************************************/

public delegate void(Delegate_OnTicks)();
public ref class UVTimerWrapper abstract : MPtrWrapper
{
public:
	UVWrapper^ creator = nullptr;				// 加持以防 uv 比 this 死的早
protected:
	Delegate_OnTicks^ onTicks;
public:
	virtual void OnCreated() = 0;
	virtual void OnTicks() = 0;

	// 位于父容器的下标
	int uv_timers_index = -1;

	UVTimerWrapper()
	{
		onTicks = gcnew Delegate_OnTicks(this, &UVTimerWrapper::OnTicks);
	}
	~UVTimerWrapper();
	virtual void InitPtr(xx::MPObject* ptr) override;
	int Start(System::UInt64 timeoutMS, UInt64 repeatIntervalMS)
	{
		AssertEnsure();
		auto p = (MyUVTimer*)ptr->ToPointer();
		return p->Start(timeoutMS, repeatIntervalMS);
	}
};


/********************************************************************************************************************/
// UVAsyncWrapper
/********************************************************************************************************************/

public delegate void(Delegate_OnFire)();
public ref class UVAsyncWrapper abstract : MPtrWrapper
{
public:
	UVWrapper^ creator = nullptr;				// 加持以防 uv 比 this 死的早
protected:
	Delegate_OnFire^ onFire;
public:
	virtual void OnFire() = 0;

	// 位于父容器的下标
	int uv_asyncs_index = -1;

	UVAsyncWrapper()
	{
		onFire = gcnew Delegate_OnFire(this, &UVAsyncWrapper::OnFire);
	}
	~UVAsyncWrapper();
	virtual void InitPtr(xx::MPObject* ptr) override;

	void Fire();
};

/********************************************************************************************************************/
// UVWrapper
/********************************************************************************************************************/

public ref class UVWrapper
{
public:
	xx::MemPool* mp;
	xx::UV* uv;
	List<UVClientPeerWrapper^>^ clientPeers;
	List<UVListenerWrapper^>^ listeners;
	List<UVTimerWrapper^>^ timers;
	List<UVAsyncWrapper^>^ asyncs;

	UVWrapper()
	{
		mp = new xx::MemPool();
		uv = mp->Create<xx::UV>();
		clientPeers = gcnew List<UVClientPeerWrapper^>();
		listeners = gcnew List<UVListenerWrapper^>();
		timers = gcnew List<UVTimerWrapper^>();
		asyncs = gcnew List<UVAsyncWrapper^>();
	}
	~UVWrapper()
	{
		// todo: foreach clientPeers, listeners, timers, syncs Dispose ?

		uv->Stop();
		uv->Release();
		uv = nullptr;
		delete mp;
		mp = nullptr;
	}


	/***************************************************************************************/
	// CreateTimer

	generic<typename T> where T : UVTimerWrapper
		T CreateTimer(T wrapper)
	{
		System::Diagnostics::Debug::Assert(wrapper->creator == nullptr);
		wrapper->creator = this;

		auto t = uv->CreateTimer<MyUVTimer>();
		if (t == nullptr) return T();

		wrapper->InitPtr(t);
		wrapper->OnCreated();
		return wrapper;
	}
	generic<typename T> where T : UVTimerWrapper, gcnew()
		T CreateTimer()
	{
		return CreateTimer(gcnew T());
	}


	/***************************************************************************************/
	// CreateAsync

	generic<typename T> where T : UVAsyncWrapper
		T CreateAsync(T wrapper)
	{
		System::Diagnostics::Debug::Assert(wrapper->creator == nullptr);
		wrapper->creator = this;

		auto t = uv->CreateAsync<MyUVAsync>();
		if (t == nullptr) return T();

		wrapper->InitPtr(t);
		return wrapper;
	}
	generic<typename T> where T : UVAsyncWrapper, gcnew()
		T CreateAsync()
	{
		return CreateAsync(gcnew T());
	}


	/***************************************************************************************/
	// CreateClientPeer

	generic<typename T> where T : UVClientPeerWrapper
		T CreateClientPeer(T wrapper)
	{
		System::Diagnostics::Debug::Assert(wrapper->creator == nullptr);
		wrapper->creator = this;

		auto t = uv->CreateClientPeer<MyUVClientPeer>();
		if (t == nullptr) return T();

		wrapper->InitPtr(t);
		return wrapper;
	}
	generic<typename T> where T : UVClientPeerWrapper, gcnew()
		T CreateClientPeer()
	{
		return CreateClientPeer(gcnew T());
	}


	/***************************************************************************************/
	// CreateListener

	generic<typename T> where T : UVListenerWrapper
		T CreateListener(T wrapper, int port, int backlog)
	{
		System::Diagnostics::Debug::Assert(wrapper->creator == nullptr);
		wrapper->creator = this;

		auto t = uv->CreateListener<MyUVListener>(port, backlog);
		if (t == nullptr) return T();

		wrapper->InitPtr(t);
		return wrapper;
	}
	generic<typename T> where T : UVListenerWrapper, gcnew()
		T CreateListener(int port, int backlog)
	{
		return CreateListener(gcnew T(), port, backlog);
	}


	/***************************************************************************************/
	// Run

	void Run()
	{
		uv->Run();
	}

	void Stop()
	{
		uv->Stop();
	}
};




UVListenerWrapper::~UVListenerWrapper()
{
	// 从父容器中移除
	SYSTEM_LIST_SWAP_REMOVE(creator->listeners, this, uv_listeners_index);
}
void UVListenerWrapper::InitPtr(xx::MPObject* ptr)
{
	System::Diagnostics::Debug::Assert(this->ptr == nullptr);
	this->MPtrWrapper::InitPtr(ptr);
	auto p = (MyUVListener*)ptr;
	p->onCreatePeer = Func_OnCreatePeer(Marshal::GetFunctionPointerForDelegate(onCreatePeer).ToPointer());

	// 放进父容器以加持
	uv_listeners_index = creator->listeners->Count;
	creator->listeners->Add(this);
}



UVTimerWrapper::~UVTimerWrapper()
{
	// 从父容器中移除
	SYSTEM_LIST_SWAP_REMOVE(creator->timers, this, uv_timers_index);
}
void UVTimerWrapper::InitPtr(xx::MPObject* ptr)
{
	System::Diagnostics::Debug::Assert(this->ptr == nullptr);
	this->MPtrWrapper::InitPtr(ptr);
	auto p = (MyUVTimer*)ptr;
	p->onTicks = Func_OnTicks(Marshal::GetFunctionPointerForDelegate(onTicks).ToPointer());

	// 放进父容器以加持
	uv_timers_index = creator->timers->Count;
	creator->timers->Add(this);
}



UVAsyncWrapper::~UVAsyncWrapper()
{
	// 从父容器中移除
	SYSTEM_LIST_SWAP_REMOVE(creator->asyncs, this, uv_asyncs_index);
}
void UVAsyncWrapper::InitPtr(xx::MPObject* ptr)
{
	System::Diagnostics::Debug::Assert(this->ptr == nullptr);
	this->MPtrWrapper::InitPtr(ptr);
	auto p = (MyUVAsync*)ptr;
	p->onFire = Func_OnFire(Marshal::GetFunctionPointerForDelegate(onFire).ToPointer());

	// 放进父容器以加持
	uv_asyncs_index = creator->asyncs->Count;
	creator->asyncs->Add(this);
}
void UVAsyncWrapper::Fire()
{
	AssertEnsure();
	auto p = (MyUVAsync*)ptr->ToPointer();
	p->Fire();
}





UVClientPeerWrapper::~UVClientPeerWrapper()
{
	// 从父容器中移除
	SYSTEM_LIST_SWAP_REMOVE(creator->clientPeers, this, uv_clientPeers_index);
}
void UVClientPeerWrapper::InitPtr(xx::MPObject* ptr)
{
	System::Diagnostics::Debug::Assert(this->ptr == nullptr);
	this->MPtrWrapper::InitPtr(ptr);
	auto p = (MyUVClientPeer*)ptr;
	p->onReceivePackage = Func_OnReceivePackage(Marshal::GetFunctionPointerForDelegate(onReceivePackage).ToPointer());
	p->onConnect = Func_OnConnect(Marshal::GetFunctionPointerForDelegate(onConnect).ToPointer());
	p->onDisconnect = Func_OnDisconnect(Marshal::GetFunctionPointerForDelegate(onDisconnect).ToPointer());

	// 放进父容器以加持
	uv_clientPeers_index = creator->clientPeers->Count;
	creator->clientPeers->Add(this);
}




UVServerPeerWrapper::~UVServerPeerWrapper()
{
	// 从父容器中移除
	SYSTEM_LIST_SWAP_REMOVE(creator->serverPeers, this, listener_serverPeers_index);
}

void UVServerPeerWrapper::InitPtr(xx::MPObject* ptr)
{
	System::Diagnostics::Debug::Assert(this->ptr == nullptr);
	this->MPtrWrapper::InitPtr(ptr);
	auto p = (MyUVServerPeer*)ptr;
	p->onReceivePackage = Func_OnReceivePackage(Marshal::GetFunctionPointerForDelegate(onReceivePackage).ToPointer());
	p->onDisconnect = Func_OnDisconnect(Marshal::GetFunctionPointerForDelegate(onDisconnect).ToPointer());

	// 放进父容器以加持
	listener_serverPeers_index = creator->serverPeers->Count;
	creator->serverPeers->Add(this);
}
