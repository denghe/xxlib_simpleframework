#include "xx_uv.h"
#include "xx_helpers.h"
#include "pkg\PKG_class.h"
#include <xx_sqlite.h>
#include <mutex>
#include <thread>
#include <optional>

struct Peer;
struct Service;
struct Listener;
struct TaskManager;
struct Dispacher;

/******************************************************************************/

struct Peer : xx::UVServerPeer
{
	Service* service;

	Peer(Listener* listener, Service* service);
	virtual void OnReceivePackage(xx::BBuffer & bb) override;
	virtual void OnDisconnect() override;
};

struct Listener : xx::UVListener
{
	Service* service;

	Listener(xx::UV* uv, int port, int backlog, Service* service);
	virtual xx::UVServerPeer * OnCreatePeer() override;
};

struct TaskManager : xx::MPObject
{
	Service* service;
	Dispacher* dispacher = nullptr;
	std::mutex tasksMutex;
	std::mutex resultsMutex;
	xx::Queue_v<std::function<void()>> tasks;
	xx::Queue_v<std::function<void()>> results;
	void AddTask(std::function<void()>&& f);
	void AddTask(std::function<void()> const& f);
	void AddResult(std::function<void()>&& f);
	void AddResult(std::function<void()> const& f);

	TaskManager(Service* service);
	~TaskManager();
	volatile bool running = true;
	volatile bool stoped = false;
	void ThreadProcess();
};

struct Service : xx::MPObject
{
	xx::UV_v uv;
	xx::SQLite_v sqldb;
	Listener* listener = nullptr;
	xx::MemHeaderBox<TaskManager> tm;

	Service();
	int Run();
};

struct Dispacher : xx::UVAsync
{
	TaskManager* tm;

	Dispacher(xx::UV* uv, TaskManager* tm);
	virtual void OnFire() override;
};


/******************************************************************************/

Listener::Listener(xx::UV* uv, int port, int backlog, Service* service)
	: xx::UVListener(uv, port, backlog)
	, service(service)
{
}

xx::UVServerPeer* Listener::OnCreatePeer()
{
	return mempool().Create<Peer>(this, service);
}


/******************************************************************************/


Dispacher::Dispacher(xx::UV* uv, TaskManager* tm)
	: xx::UVAsync(uv)
	, tm(tm)
{
}

void Dispacher::OnFire()
{
	while (!tm->results->Empty())
	{
		tm->results->Top()();
		tm->results->Pop();
	}
}

/******************************************************************************/

TaskManager::TaskManager(Service* service)
	: service(service)
	, tasks(mempool())
	, results(mempool())
{
	this->dispacher = service->uv->CreateAsync<Dispacher>(this);
	if (!this->dispacher) throw nullptr;
	std::thread t([this] { ThreadProcess(); });
	t.detach();
}

TaskManager::~TaskManager()
{
	running = false;
	while (!stoped) Sleep(1);
}

void TaskManager::AddTask(std::function<void()>&& f)
{
	std::lock_guard<std::mutex> lock(tasksMutex);
	tasks->Push(std::move(f));
}

void TaskManager::AddTask(std::function<void()> const& f)
{
	std::lock_guard<std::mutex> lock(tasksMutex);
	tasks->Push(std::move(f));
}

void TaskManager::AddResult(std::function<void()>&& f)
{
	{
		std::lock_guard<std::mutex> lock(resultsMutex);
		results->Push(std::move(f));
	}
	dispacher->Fire();
}

void TaskManager::AddResult(std::function<void()> const& f)
{
	{
		std::lock_guard<std::mutex> lock(resultsMutex);
		results->Push(std::move(f));
	}
	dispacher->Fire();
}

void TaskManager::ThreadProcess()
{
	std::function<void()> func;
	while (running)
	{
		bool gotFunc = false;
		{
			std::lock_guard<std::mutex> lock(tasksMutex);
			gotFunc = tasks->TryPop(func);
		}
		if (gotFunc) func();
		else Sleep(1);
	}
	stoped = true;
}

/******************************************************************************/

Service::Service()
	: uv(mempool())
	, sqldb(mempool(), "data.db")
	, tm(mempool(), this)
{
	sqldb->SetPragmas(xx::SQLiteJournalModes::WAL);
}

int Service::Run()
{
	// todo: 检查 db 看是不是新建的, 是就执行建表脚本
	// todo: 先去写一套生成器 for sqlite

	listener = uv->CreateListener<Listener>(12345, 128, this);
	if (!listener) return -1;
	uv->Run();
	return 0;
}

/******************************************************************************/

Peer::Peer(Listener* listener, Service* service)
	: xx::UVServerPeer(listener)
	, service(service)
{
}

void Peer::OnDisconnect() {}

void Peer::OnReceivePackage(xx::BBuffer& bb)
{
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



	service->tm->AddTask([service = this->service, peer = xx::MPtr<Peer>(this)/*, args*/]	// 转到 SQL 线程
	{
		// 执行 SQL 语句, 得到结果
		// auto rtv = sqlfs.execxxxxx( args... )

		service->tm->AddResult([service, peer/*, args, rtv */]	// 转到 uv 线程
		{
		// args->Release();
		// handle( rtv )
		if (peer && peer->state == xx::UVPeerStates::Connected)	// 如果 peer 还活着, 做一些回发功能
		{
			//mp->SendPackages
		}
		// service->tm->AddTask([rtv]{ rtv->Release(); })	// 转到 SQL 线程
	});
	});
}

/******************************************************************************/









struct Foo : xx::MPObject
{
	xx::String_p str;
	Foo() : str(mempool()) {}
	Foo(Foo&&) = default;
	Foo(Foo const&) = delete;
	Foo& operator=(Foo const&) = delete;
};
using Foo_p = xx::Ptr<Foo>;

Foo_p GetFoo(xx::MemPool& mp)
{
	Foo_p rtv;
	rtv.Create(mp);
	return rtv;
}



#include "db\DB_class.h"

namespace DB
{
	// 模拟生成的函数长相

	struct SQLiteFuncs
	{
		xx::SQLite* sqlite;
		xx::MemPool& mp;
		xx::SQLiteString_v s;
		bool hasError = false;
		int const& lastErrorCode() { return sqlite->lastErrorCode; }
		const char* const& lastErrorMessage() { return sqlite->lastErrorMessage; }

		SQLiteFuncs(xx::SQLite* sqlite) : sqlite(sqlite), mp(sqlite->mempool()), s(mp) {}
		~SQLiteFuncs()
		{
			if (query_CreateAccountTable) query_CreateAccountTable->Release();
			if (query_AddAccount) query_AddAccount->Release();
			if (query_GetAccountByUsername) query_GetAccountByUsername->Release();
		}

		xx::SQLiteQuery* query_CreateAccountTable = nullptr;
		void CreateAccountTable()
		{
			hasError = true;
			auto& q = query_CreateAccountTable;
			if (!q)
			{
				q = sqlite->CreateQuery(R"=-=(
create table [account]
(
    [id] integer primary key autoincrement, 
    [username] text(64) not null unique, 
    [password] text(64) not null
)
)=-=");
			}
			if (!q) return;
			if (!q->Execute()) return;
			hasError = false;
		}

		xx::SQLiteQuery* query_AddAccount = nullptr;
		void AddAccount(DB::Account const* const& a)
		{
			assert(a);
			hasError = true;
			auto& q = query_CreateAccountTable;
			if (!q)
			{
				q = sqlite->CreateQuery(R"=-=(
insert into [account] ([username], [password])
values (?, ?)
)=-=");
			}
			if (!q) return;
			if (q->SetParameters(a->username, a->password)) return;
			if (!q->Execute()) return;
			hasError = false;
		}

		void AddAccount2(char const* const& username, char const* const& password)
		{
			hasError = true;
			auto& q = query_CreateAccountTable;
			if (!q)
			{
				q = sqlite->CreateQuery(R"=-=(
insert into [account] ([username], [password])
values (?, ?)
)=-=");
			}
			if (!q) return;
			if (q->SetParameters(username, password)) return;
			if (!q->Execute()) return;
			hasError = false;
		}

		xx::SQLiteQuery* query_GetAccountByUsername = nullptr;
		Account* GetAccountByUsername(char const* const& username)
		{
			hasError = true;
			Account* rtv = nullptr;
			auto& q = query_GetAccountByUsername;
			if (!q)
			{
				q = sqlite->CreateQuery(R"=-=(
select [id], [username], [password]
  from [account]
 where [username] = ?
)=-=");
			}
			if (!q) return rtv;
			if (q->SetParameters(username)) return rtv;
			if (!q->Execute([&](xx::SQLiteReader& sr)
			{
				mp.CreateTo(rtv);
				rtv->id = sr.ReadInt64(0);
				mp.CreateTo(rtv->username, sr.ReadString(1));
				mp.CreateTo(rtv->password, sr.ReadString(2));
			})) return rtv;
			hasError = false;
			return rtv;
		}

		xx::SQLiteQuery* query_GetAccountsByUsernames = nullptr;
		xx::List<Account*>* GetAccountsByUsernames(xx::List_v<xx::String_v>& usernames)
		{
			hasError = true;
			xx::List<Account*>* rtv = nullptr;
			auto& q = query_GetAccountsByUsernames;
			if (q)
			{
				q->Release();
				q = nullptr;
			}
			if (!q)
			{
				s->Clear();
				s->Append(R"=-=(
select [id], [username], [password]
  from [account]
 where [username] in )=-=");
				s->SQLAppend(usernames);
				q = sqlite->CreateQuery(s->C_str(), s->dataLen);
			}
			if (!q) return rtv;
			mp.CreateTo(rtv);
			if (!q->Execute([&](xx::SQLiteReader& sr)
			{
				auto r = mp.Create<Account>();
				rtv->Add(r);
				r->id = sr.ReadInt64(0);
				if (!sr.IsNull(1)) mp.CreateTo(r->username, sr.ReadString(1));
				if (!sr.IsNull(2)) mp.CreateTo(r->password, sr.ReadString(2));
			}))
			{
				for (auto& o : *rtv) o->Release();
				rtv->Release();
				rtv = nullptr;
				return rtv;
			}
			hasError = false;
			return rtv;
		}
	};
}


int main()
{
	PKG::AllTypesRegister();
	//xx::MemHeaderBox<Service> s(mp);
	//s->Run();



	xx::MemPool mp;
	{
		auto foo = GetFoo(mp);
	}
	{
		Foo_p foo;
		foo.Create(mp);
		*foo->str = "xx";
		*foo.Create(mp)->str = "xxx";
		foo = nullptr;
	}










	xx::SQLite_v sql(mp, "data.db");
	DB::SQLiteFuncs fs(sql);

	auto r = sql->TableExists("account");
	if (r < 0)
	{
		mp.Cout("errCode = ", sql->lastErrorCode, "errMsg = ", sql->lastErrorMessage, "\n");
		goto LabEnd;
	}
	else if (r == 0)
	{
		fs.CreateAccountTable();
		assert(!fs.hasError);

		xx::MemHeaderBox<DB::Account> a(mp);
		mp.CreateTo(a->username);
		mp.CreateTo(a->password);

		a->username->Assign("a");
		a->password->Assign("1");
		fs.AddAccount(a);
		assert(!fs.hasError);

		fs.AddAccount2("b", "2");
		assert(!fs.hasError);
	}

	{
		auto a = fs.GetAccountByUsername("a");	//xx::UPtr<
		if (fs.hasError)
		{
			mp.Cout("errCode = ", fs.lastErrorCode(), "errMsg = ", fs.lastErrorMessage(), "\n");
			goto LabEnd;
		}
		else if (a == nullptr)
		{
			mp.Cout("can't find account a!\n");
		}
		else
		{
			mp.Cout("found account a! id = ", a->id, " password = ", a->password, "\n");
			a->Release();
		}
	}
	{
		xx::List_v<xx::String_v> usernames(mp);
		usernames->EmplaceMP("a");
		usernames->EmplaceMP("b");
		auto as = fs.GetAccountsByUsernames(usernames);
		for (auto& a : *as)
		{
			mp.Cout(a, "\n");
		}
		as->ReleaseWithItems();
	}

LabEnd:
	std::cin.get();
	return 0;
}
