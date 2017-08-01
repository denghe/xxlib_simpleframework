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


















// todo: 支持传入 SQLite 能用于 bind 的简单数据类型, 以及 List( num ) 主用于 where xx in (.........) 的生成
// 当遇到 List 时, 生成物将启用拼接模式, 剩下的其他类型参数则依次传给 SetParameters
// 同时也支持传入 类指针, 暂不支持 List< 类指针 >( 因为这种传值方式主要就是拿来做 insert values (),(),..., 但是直接反复执行一条构造好的 insert 查询应该更快 )
// 当遇到类指针时, 拼接将展开为 里面的 非只读字段 的个数 问号, 传参时将直接传其子成员

// 也就是说 SQLiteAppend 要根据上面的需求, 做相应的修改, 主要针对 T* , 及 List< 啥啥 >. 啥啥应该是 SQLite 支持的所有数据类型才对
// ignoreReadOnly 应该是默认启用的. 调用时根本不好传值.

namespace xx
{
	// 基础适配模板
	template<typename T>
	struct SQLiteAppend
	{
		static void Exec(String* ss, T const& in);/*
		{
			assert(false);
		}*/
	};
}

namespace xx
{
	struct SQLiteString : String
	{
		SQLiteString() : String() {}

		// 用于插入数字
		template<typename T>
		void SQLAppend(T const& v)
		{
			static_assert(std::is_arithmetic<T>::value, "");
			Append(v);
		}

		// 插入字串
		void SQLAppend(char const* const& v, uint32_t len = 0)
		{
			if (!v)
			{
				Append("null");
				return;
			}
			// 转义 '	// todo: 感觉有必要直接找到 sqlite 的转义函数抄一段. 这个先用着
			if (!len) len = (uint32_t)strlen(v);
			Reserve(dataLen + len * 2 + 2);
			buf[dataLen++] = '\'';
			for (uint32_t i = 0; i < len; ++i)
			{
				if (v[i] != '\'')
				{
					buf[dataLen++] = v[i];
				}
				else
				{
					buf[dataLen] = '\'';
					buf[dataLen + 1] = '\'';
					dataLen += 2;
				}
			}
			buf[dataLen++] = '\'';
		}

		// 同上
		void SQLAppend(String* const& v)
		{
			SQLAppend(v ? v->C_str() : nullptr, v ? v->dataLen : 0);
		}

		static constexpr char* const hexStr = "0123456789abcdef";
		// 插入 BLOB
		void SQLAppend(BBuffer* const& v)
		{
			if (!v)
			{
				Append("null");
				return;
			}
			// 将 v 转成 x'11aa22bb....' (  sqlite3.c  71051 行 )
			auto len = v->dataLen;
			Reserve(dataLen + len * 2 + 3);
			this->buf[dataLen] = 'x';
			this->buf[dataLen + 1] = '\'';
			this->buf[dataLen + len * 2 + 2] = '\'';
			dataLen += 2;
			for (uint32_t i = 0; i < len; i++)
			{
				v->buf[dataLen + i * 2 + 0] = hexStr[(uint8_t)buf[i] >> 4];
				v->buf[dataLen + i * 2 + 1] = hexStr[buf[i] & 0x0F];
			}
			dataLen += len * 2 + 1;
		}

		// List 的重载
		template<typename T>
		void SQLAppend(List<T>* const& os)
		{
			assert(os && os->dataLen);
			for (int i = 0; i < os->dataLen; ++i)
			{
				SQLAppend(os[i]);
				Append(", ");
			};
			dataLen -= 2;
		}


		// 用于插入类
		template<typename T>
		void SQLAppend(T* const& v)
		{
			assert(v);
			static_assert(IsMPObject_v<T>, "");
			SQLiteAppend<T>::Exec(this, *v);
		}
	};

	using SQLiteString_v = xx::MemHeaderBox<SQLiteString>;
}

#include "db\DB_class.h"

namespace xx
{
	template<>
	struct SQLiteAppend<DB::Account const>
	{
		static void Exec(String* ss, DB::Account const& in)
		{
			ss->Append("(?, ?)");
		}
	};
}


namespace DB
{
	// 模拟生成的函数长相. 函数执行需要 try

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
			s->Clear();
			s->Append(R"=-=(
insert into [account] ([username], [password])
values )=-=");
			s->SQLAppend(a);

			auto q = sqlite->CreateQuery(s->C_str(), s->dataLen);
			if (!q) return;
			if (q->SetParameters(a->username, a->password)) return;
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
				rtv = mp.Create<Account>();
				rtv->id = sr.ReadInt64(0);
				mp.CreateTo(rtv->username, sr.ReadString(1));
				mp.CreateTo(rtv->password, sr.ReadString(2));
			})) return rtv;
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

		a->username->Assign("a");
		a->password->Assign("1");
		fs.AddAccount(a);
		assert(!fs.hasError);

		a->username->Assign("b");
		a->password->Assign("2");
		fs.AddAccount(a);
		assert(!fs.hasError);
	}

	{
		auto a = fs.GetAccountByUsername("a");
		if (fs.hasError)
		{
			mp.Cout("errCode = ", fs.lastErrorCode(), "errMsg = ", fs.lastErrorMessage(), "\n");
			goto LabEnd;
		}
		if (a == nullptr)
		{
			mp.Cout("can't find account a!\n");
		}
		else
		{
			mp.Cout("found account a! id = ", a->id, " password = ", a->password, "\n");
			a->Release();
		}
	}

LabEnd:
	std::cin.get();
	return 0;
}
