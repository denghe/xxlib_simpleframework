#include "xx_func.h"
#include "xx_uv.h"
#include "xx_helpers.h"
#include "pkg\PKG_class.h"
#include "db\DB_sqlite.h"


struct Peer;
struct Service;
struct Listener;
struct TaskManager;
struct Dispacher;

/******************************************************************************/

struct Peer : xx::UVServerPeer
{
	// 指向服务主体
	Service* service;

	Peer(Listener* listener);
	virtual void OnReceivePackage(xx::BBuffer & bb) override;
	virtual void OnDisconnect() override;
};

struct Listener : xx::UVListener
{
	// 指向服务主体
	Service* service;

	Listener(xx::UV* uv, int port, int backlog, Service* service);
	virtual xx::UVServerPeer * OnCreatePeer() override;
};

struct Worker : xx::UVWorker
{
	// 指向服务主体
	Service* service;
	Worker(Service* service);

	// 工作闭包( 由主线程创建 参数传入, 在工作线程中执行 )
	xx::Func<void()> task;

	// 结果处理 & 参数回收闭包( 由工作线程创建 结果传入, 在主线程中执行 )
	xx::Func<void()> result;

	// 结果回收闭包( 在主线程中赋值, 由工作线程执行以清除上次的结果 )
	xx::Func<void()> resultKiller;

	// 在工作线程中执行
	virtual void OnWork() override;

	// 在主线程中执行
	virtual void OnAfterWork(int status) override;
};

struct Service : xx::Object
{
	xx::MemPool sqlmp;			// 给 SQLite 及其工作线程使用的 mp
	xx::SQLite_v sqldb;			// SQLite 走独立的内存池, 和主线程的分离
	DB::SQLiteManageFuncs fs;	// SQL 语句集

	xx::UV_v uv;
	Listener* listener = nullptr;

	xx::Dock<Worker> worker;
	xx::Queue_v<xx::Func<void()>> tasks;
	void AddTask(xx::Func<void()>&& fn);
	void SetResult(xx::Func<void()>&& fn);
	void SetResultKiller(xx::Func<void()>&& fn);

	Service();
	int Run();
};








/******************************************************************************/

inline Listener::Listener(xx::UV* uv, int port, int backlog, Service* service)
	: xx::UVListener(uv, port, backlog)
	, service(service)
{
}

inline xx::UVServerPeer* Listener::OnCreatePeer()
{
	return mempool().Create<Peer>(this);
}


/******************************************************************************/

inline Worker::Worker(Service* service)
	: UVWorker(&*service->uv)
	, service(service)
{
}

inline void Worker::OnWork()
{
	if (resultKiller)					// 先清理上次创建的结果集
	{
		resultKiller();
		resultKiller = nullptr;
	}

	task();								// 执行任务
}

inline void Worker::OnAfterWork(int status)
{
	if (result)							// 先执行结果处理
	{
		result();
		result = nullptr;
	}

	if (service->tasks->TryPop(task))	// 如果还有后续工作, 继续取出来执行
	{
		Start();
	}
	else
	{
		task = nullptr;
	}
}

/******************************************************************************/

inline Service::Service()
	: sqldb(sqlmp, "data.db")
	, fs(*sqldb)
	, uv(mempool())
	, tasks(mempool())
	, worker(mempool(), this)
{
	//sqldb->Attach("log", "log.db");
	sqldb->SetPragmaJournalMode(xx::SQLiteJournalModes::WAL);
	sqldb->SetPragmaForeignKeys(true);
}

inline int Service::Run()
{
	// todo: 检查 db 看是不是新建的, 是就执行建表脚本

	listener = uv->CreateListener<Listener>(12345, 128, this);
	if (!listener) return -1;
	uv->Run();
	return 0;
}

inline void Service::AddTask(xx::Func<void()>&& fn)
{
	if (!worker->working)
	{
		assert(!tasks->Count());
		worker->task = std::move(fn);
		worker->Start();
	}
	else
	{
		tasks->Emplace(std::move(fn));
	}
}

inline void Service::SetResult(xx::Func<void()>&& fn)
{
	assert(!worker->result);
	worker->result = std::move(fn);
}

inline void Service::SetResultKiller(xx::Func<void()>&& fn)
{
	assert(!worker->resultKiller);
	worker->resultKiller = std::move(fn);
}

/******************************************************************************/

inline Peer::Peer(Listener* listener)
	: xx::UVServerPeer(listener)
	, service(listener->service)
{
}

inline void Peer::OnDisconnect() {}

#include <thread>

inline void Peer::OnReceivePackage(xx::BBuffer& bb)
{
	// todo: 收到包, 解析, 向任务容器压函数, 转到后台线程执行


	// 模拟传一批参数
	struct Args : xx::Object
	{
		Args()
		{
			std::cout << "Args(), thread id = " << std::this_thread::get_id() << std::endl;
		}
		Args(Args&&) = default;
		xx::String_p un;
		~Args()
		{
			std::cout << "~Args(), thread id = " << std::this_thread::get_id() << std::endl;
		}
	};
	auto args = mempool().CreatePtr<Args>();
	// 填充 args .........
	// ...

	// 向 SQL 线程压入函数( service 是指针, 直接复制. peer 捕获安全引用类型, 传递到 result 函数中使用. args 捕获右值移动 )
	service->AddTask([service = this->service, peer = xx::Ref<Peer>(this), args = std::move(args)]()
	{
		std::cout << "Task, thread id = " << std::this_thread::get_id() << std::endl;

		// 执行 SQL 语句, 得到结果
		// 期间只能从 service->sqlmp 分配内存. rtv 创建为类似 Args 的集合类似乎更佳
		auto rtv = service->fs.SelectAccountByUsername(args->un);

		// 到主线程去处理结果, 设置结果函数( service, peer 直接复制, rtv 移动, 顺便将 args 移进去以便回收 )
		service->SetResult([service, peer, rtv = std::move(rtv), args = xx::Move(args)]
		{
			std::cout << "Result, thread id = " << std::this_thread::get_id() << std::endl;

			// handle( rtv )
			if (peer && peer->state == xx::UVPeerStates::Connected)			// 如果 peer 还活着, 回发
			{
				//peer->SendPackages
			}

			// 到工作线程去回收结果, 设置结果回收函数( 将 rtv 移进去即可 )
			service->SetResultKiller([rtv = xx::Move(rtv)]
			{
				std::cout << "ResultKiller, thread id = " << std::this_thread::get_id() << std::endl;
			});  			
		});
	});
}



/******************************************************************************/

struct Ctx
{
	xx::Func<void()> f1;
	xx::Func<void()> f2;
	xx::Func<void()> f3;
};
struct Foo
{
	bool val;
	Foo(bool val = false) : val(val)
	{
		std::cout << "Foo() val = " << (val ? "true" : "false") << std::endl;
	}
	Foo(Foo const& o) = delete;
	Foo(Foo && o)
		: val(false)
	{
		std::swap(val, o.val);
		std::cout << "Foo(Foo && o)" << std::endl;
	}
	~Foo()
	{
		std::cout << "~Foo() val = " << (val ? "true" : "false") << std::endl;
	}
};

int main()
{
	{
		Ctx ctx;
		{
			Foo foo(true);
			ctx.f1 = [&ctx, foo = xx::Move(foo)]()mutable
			{
				std::cout << "f1()" << std::endl;
				ctx.f2 = [&ctx, foo = xx::Move(foo)]()mutable
				{
					std::cout << "f2()" << std::endl;
					ctx.f3 = [&ctx, foo = xx::Move(foo)]()mutable
					{
						std::cout << "f3()" << std::endl;
					};
					std::cout << "~f2()" << std::endl;
				};
				std::cout << "~f1()" << std::endl;
			};
			std::cout << "1111111111" << std::endl;
			ctx.f1();
			ctx.f1 = nullptr;
			std::cout << "2222222222" << std::endl;
			ctx.f2();
			ctx.f2 = nullptr;
			std::cout << "3333333333" << std::endl;
			ctx.f3();
			ctx.f3 = nullptr;
			std::cout << "4444444444" << std::endl;
		}
		std::cout << "5555555555" << std::endl;
	}

	return 0;
}




//
//
//// 测试结果, UV_THREADPOOL_SIZE=1 的情况下, 每秒能执行 100 万次
//
//struct MyUV : xx::UV
//{
//	MyUV() : UV()
//	{
//		Cout("MyUV()\n");
//	}
//	~MyUV()
//	{
//		Cout("~MyUV()\n");
//	}
//};
//
//struct Worker : xx::UVWorker
//{
//	int i1 = 0, i2 = 0;
//	Worker(xx::UV* uv) : UVWorker(uv)
//	{
//		Cout("Worker()\n");
//	}
//	~Worker()
//	{
//		Cout("~Worker()\n");
//	}
//	virtual void OnWork() override
//	{
//		Cout("OnWork before sleep\n");
//		++i1;
//		Sleep(2000000);
//		Cout("OnWork sleeped\n");
//	}
//	virtual void OnAfterWork(int status) override
//	{
//		Cout("OnAfterWork\n");
//		++i2;
//		if (i2 < 10000000) Start();
//	}
//};
//
//struct Timer : xx::UVTimer
//{
//	Worker& w;
//	Timer(xx::UV* uv, Worker& w) : UVTimer(uv), w(w)
//	{
//		Start(1000, 1000);
//	}
//	virtual void OnFire() override
//	{
//		Cout("i1 = ", w.i1, ", i2 = ", w.i2, "\n");
//		uv->Stop();
//	}
//};
//
//int main()
//{
//	// todo: 用 Worker 改造上面的代码
//	xx::MemPool mp;
//	{
//		xx::Dock<MyUV> uv(mp);
//		auto w = uv->CreateWorker<Worker>();
//		w->Start();
//		auto t = uv->CreateTimer<Timer>(*w);
//		uv->Run();
//		mp.Cout("uv stoped.\n");
//	}
//	std::cin.get();
//	return 0;
//}




// todo: 收到包, 解析, 向任务容器压函数, 转到后台线程执行
// SQLite 走独立的内存池, 和主线程的分离

// 内存分配 & 回收流程( 基于 SQLite 只占 1 线, 使用 mempool 的情况 ): 
// 1. uv线程 分配 SQL线程 需要的数据, 后将处理函数压入 tasks
// 2. SQL线程 执行期间, 分配供 uv线程 处理结果所需数据, 后将处理函数压入 results
// 3. uv线程 读取结果数据, 回收 第1步分配的内存, 后将 第2步的内存回收函数压入 tasks
// 4. tasks 执行第2步分配的内存回收

// 上述流程的 多 work thread 的补充:
// 1. 做一个线程私有 tasks 队列及 mempool, 标记为 TLS
// 2. 上述流程第 2 步 记录私有 tasks 指针 并向下传递
// 3. 上述流程第 3 步 向 私有 tasks 压内存回收函数
// 4. 工作线程除了扫公共 tasks 以外, 还扫私有 tasks, 扫到就执行

// 这套方案的问题在于, 多线内存池会损耗比较大, 复用率低下. 

