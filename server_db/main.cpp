#include "xx_uv.h"
#include "xx_helpers.h"
#include "pkg_cpp\PKG_class.h"
#include <xx_sqlite.h>
#include <mutex>
#include <thread>

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
	xx::Queue_v<std::function<void()>> tasks;
	xx::Queue_v<std::function<void()>> results;

	TaskManager(Service* service);
	~TaskManager();
	bool running = true;
	bool stoped = false;
	void ThreadProcess();
};

struct Service : xx::MPObject
{
	xx::UV_v uv;
	xx::SQLite_v lite;
	xx::MemHeaderBox<TaskManager> tm;

	Service();
	void Run();
};

struct Dispacher : xx::UVAsync
{
	TaskManager* tm;

	Dispacher(xx::UV* uv, TaskManager* tm);
	virtual void OnFire() override;
};

/******************************************************************************/

Peer::Peer(Listener* listener, Service* service)
	: xx::UVServerPeer(listener)
	, service(service)
{
}

void Peer::OnReceivePackage(xx::BBuffer & bb)
{
	// todo
}

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

Service::Service()
	: uv(mempool())
	, lite(mempool(), "data.db")
	, tm(mempool(), this)
{
	lite->SetPragmas(xx::SQLiteJournalModes::WAL);
}

void Service::Run()
{
	uv->Run();
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
}

TaskManager::~TaskManager()
{
	running = false;
	while (!stoped) Sleep(1);
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
		if (gotFunc)
		{
			func();
		}
		else Sleep(1);
	}
	stoped = true;
}

/******************************************************************************/

int main()
{
	PKG::AllTypesRegister();
	xx::MemPool mp;
	xx::MemHeaderBox<Service> s(mp);
	s->Run();
	return 0;
}
