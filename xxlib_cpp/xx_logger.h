#pragma once
#include "xx_sqlite.h"
#include "xx_queue.h"
#include <mutex>
#include <thread>

namespace xx
{
	// 对应 log 日志表
	struct Log;
	// 日志级别
	enum class LogLevel : int32_t
	{
		Off = 0,
		Fatal = 1,
		Error = 2,
		Warning = 3,
		Info = 4,
		Debug = 5,
		All = 6,
	};

	// 对应 log 日志表
	struct Log : Object
	{
		// 自增主键
		int64_t id = 0;
		// 日志级别
		LogLevel level = (LogLevel)0;
		// 发生时间
		int64_t time = 0;
		// 机器名
		String_p machine;
		// 进程/服务名
		String_p service;
		// 进程/实例id
		String_p instanceId;
		// 标题
		String_p title;
		// 操作代码
		int64_t opcode = 0;
		// 日志明细
		String_p desc;

		typedef Log ThisType;
		typedef Object BaseType;
		Log();
		Log(Log const&) = delete;
		Log& operator=(Log const&) = delete;
		virtual void ToString(String &str) const override;
		virtual void ToStringCore(String &str) const override;
	};
	using Log_p = Ptr<Log>;
	using Log_v = Dock<Log>;


	inline Log::Log()
	{
	}

	inline void Log::ToString(String &str) const
	{
		if (tsFlags())
		{
			str.Append("[ \"***** recursived *****\" ]");
			return;
		}
		else tsFlags() = 1;

		str.Append("{ \"type\" : \"Log\"");
		ToStringCore(str);
		str.Append(" }");

		tsFlags() = 0;
	}
	inline void Log::ToStringCore(String &str) const
	{
		this->BaseType::ToStringCore(str);
		str.Append(", \"id\" : ", this->id);
		str.Append(", \"level\" : ", this->level);
		str.Append(", \"time\" : ", this->time);
		str.Append(", \"machine\" : ", this->machine);
		str.Append(", \"service\" : ", this->service);
		str.Append(", \"instanceId\" : ", this->instanceId);
		str.Append(", \"title\" : ", this->title);
		str.Append(", \"opcode\" : ", this->opcode);
		str.Append(", \"desc\" : ", this->desc);
	}

	template<>
	struct MemmoveSupport<Log_v>
	{
		static const bool value = true;
	};



	struct LogFuncs
	{
		SQLite& db;
		LogFuncs(SQLite& db) : db(db) {}

		SQLiteQuery_p query_CreateTable_log;
		// 建 log 表
		void CreateTable_log()
		{
			auto& q = query_CreateTable_log;

			if (!q)
			{
				q = db.CreateQuery(R"=-=(
CREATE TABLE [log](
    [id] INTEGER PRIMARY KEY AUTOINCREMENT NOT NULL UNIQUE, 
    [level] INT NOT NULL,
    [time] INTEGER NOT NULL, 
    [machine] TEXT(250) NOT NULL, 
    [service] TEXT(50) NOT NULL, 
    [instanceId] TEXT(50) NOT NULL, 
    [title] TEXT(250) NOT NULL,
    [opcode] INTEGER NOT NULL,
    [desc] TEXT NOT NULL
);)=-=");
			}
			q->Execute();
		}


		SQLiteQuery_p query_InsertLog;
		// 插入一条 log. time 传入 DateTime.Now.Ticks
		void InsertLog
		(
			LogLevel const& level,
			int64_t const& time,
			String_p const& machine,
			String_p const& service,
			String_p const& instanceId,
			String_p const& title,
			int64_t const& opcode,
			String_p const& desc
		)
		{
			auto& q = query_InsertLog;

			if (!q)
			{
				q = db.CreateQuery(R"=-=(
insert into [log] ([level], [time], [machine], [service], [instanceId], [title], [opcode], [desc]) 
values (?, ?, ?, ?, ?, ?, ?, ?))=-=");
			}
			q->SetParameters(level, time, machine, service, instanceId, title, opcode, desc);
			q->Execute();
		}



		//xx::SQLiteQuery_p query_InsertLog;
		// 插入一条 log. time 传入 DateTime.Now.Ticks
		void InsertLog
		(
			LogLevel const& level,
			int64_t const& time,
			char const* const& machine,
			char const* const& service,
			char const* const& instanceId,
			char const* const& title,
			int64_t const& opcode,
			char const* const& desc
		)
		{
			auto& q = query_InsertLog;

			if (!q)
			{
				q = db.CreateQuery(R"=-=(
insert into [log] ([level], [time], [machine], [service], [instanceId], [title], [opcode], [desc]) 
values (?, ?, ?, ?, ?, ?, ?, ?))=-=");
			}
			q->SetParameters(level, time, machine, service, instanceId, title, opcode, desc);
			q->Execute();
		}
	};





	struct Logger
	{
		MemPool mp;
		SQLite_v db;
		LogFuncs funcs;

		static const int nameLenLimit = 200;
		String_v machine;
		String_v service;
		String_v instanceId;

		// 为每个 queue 创建一个 mp　专用．这样只需要在切换时 lock
		MemPool mp1;
		Queue_v<Log_p> logMsgs1;		// 切换使用

		MemPool mp2;
		Queue_v<Log_p> logMsgs2;		// 切换使用

		Queue<Log_p>* logMsgs;			// 指向当前正在使用的 logMsgs
		std::mutex mtx;

		int64_t writeLimit = 1000000;	// 当前队列写入深度如果超过这个值就不再写入
		int64_t counter = 0;			// 总写入量的统计值
		bool disposing = false;			// 通知后台线程退出的标志位

		Logger(char const* const& fn)
			: db(mp, fn)
			, funcs(*db)
			, machine(mp, nameLenLimit)
			, service(mp, nameLenLimit)
			, instanceId(mp, nameLenLimit)
			, logMsgs1(mp1)
			, logMsgs2(mp2)
			, logMsgs(&*logMsgs1)
		{
			if (!db->TableExists("log")) funcs.CreateTable_log();
			std::thread t([this]
			{
				Queue<Log_p>* backMsgs;

				while (!disposing)
				{
					// 切换前后台队列( 如果有数据. 没有就 sleep 一下继续扫 )
					{
						std::lock_guard<std::mutex> lg(mtx);

						if (!logMsgs->Count()) goto LabEnd;

						if (logMsgs == &*logMsgs1)
						{
							backMsgs = &*logMsgs1;
							logMsgs = &*logMsgs2;
						}
						else
						{
							backMsgs = &*logMsgs2;
							logMsgs = &*logMsgs1;
						}
					}

					// 开始批量插入( 这期间前台可以继续操作 )
					try
					{
						db->BeginTransaction();
						while (!backMsgs->Empty())
						{
							auto& o = *backMsgs->Top();
							if (o.id)
							{
								funcs.InsertLog(o.level, o.time, o.machine, o.service, o.instanceId, o.title, o.opcode, o.desc);
							}
							else
							{
								machine->Assign(o.machine);
								service->Assign(o.service);
								instanceId->Assign(o.instanceId);
							}
							backMsgs->Pop();
							++counter;
						}
						db->EndTransaction();
					}
					catch (...)
					{
						// 似乎只能忽略错误
						std::cout << "logdb insert error! errNO = " << db->lastErrorCode << " errMsg = " << db->lastErrorMessage << std::endl;
					}
				LabEnd:
					Sleep(50);
				}
				disposing = false;
			});
			t.detach();
		}

		Logger(Logger const&) = delete;
		Logger operator=(Logger const&) = delete;

		~Logger()
		{
			disposing = true;
			while (disposing) Sleep(1);
			std::cout << "~Logger()" << std::endl;
		}

		template<typename MachineType, typename ServiceType, typename InstanceIdType, typename TitleType, typename DescType>
		bool WriteAll(LogLevel const& level, int64_t const& time
			, MachineType const& machine, ServiceType const& service, InstanceIdType const& instanceId
			, TitleType const& title, int64_t const& opcode, DescType const& desc)
		{
			std::lock_guard<std::mutex> lg(mtx);
			if (logMsgs->Count() > writeLimit) return false;

			auto& mp = logMsgs->mempool();
			auto o = mp.CreatePtr<Log>();

			o->id = 2;             // 用来标记是通过 WaitAll 写入的
			o->level = level;
			o->time = time;
			o->machine.Create(mp, machine);
			o->service.Create(mp, service);
			o->instanceId.Create(mp, instanceId);
			o->title.Create(mp, title);
			o->opcode = opcode;
			o->desc.Create(mp, desc);

			logMsgs->Emplace(std::move(o));
			return true;
		}

		template<typename TitleType, typename DescType>
		bool Write(LogLevel level, TitleType const& title, int64_t const& opcode, DescType const& desc)
		{
			std::lock_guard<std::mutex> lg(mtx);
			if (logMsgs->Count() > writeLimit) return false;

			auto& mp = logMsgs->mempool();
			auto o = mp.CreatePtr<Log>();

			o->id = 1;             // 用来标记是通过 Write 写入的
			o->level = level;

			o->time = GetNowDateTimeTicks();
			o->machine = &*machine;
			o->service = &*service;
			o->instanceId = &*instanceId;

			o->title.Create(mp, title);
			o->opcode = opcode;
			o->desc.Create(mp, desc);

			logMsgs->Emplace(std::move(o));
			return true;
		}

		template<typename MachineType, typename ServiceType, typename InstanceIdType>
		bool SetDefaultValue(MachineType const& machine, ServiceType const& service, InstanceIdType const& instanceId)
		{
			std::lock_guard<std::mutex> lg(mtx);
			if (logMsgs->Count() > writeLimit) return false;

			auto& mp = logMsgs->mempool();
			auto o = mp.CreatePtr<Log>();

			o->id = 0;				// 用来标记是通过 SetDefaultValue 写入的
			o->machine.Create(mp, machine);
			o->service.Create(mp, service);
			o->instanceId.Create(mp, instanceId);

			logMsgs->Emplace(std::move(o));
			return true;
		}
	};
}
