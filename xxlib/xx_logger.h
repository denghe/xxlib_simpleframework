#pragma once
#include "xx.h"
#include "xx_sqlite.h"
#include <mutex>
#include <thread>

namespace xx
{
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

	// 写入到 sqlite log表 中的结构体
	class Log : public Object
	{
	public:
		int64_t id = 0;					// 自增主键
		LogLevel level = (LogLevel)0;	// 日志级别
		int64_t time = 0;				// 发生时间
		String machine;					// 机器名
		String service;					// 进程/服务名
		String instanceId;				// 进程/实例id
		String title;					// 标题
		int64_t opcode = 0;				// 操作代码
		String desc;					// 日志明细

		Log(MemPool* const& mp) noexcept
			: Object(mp)
			, machine(mp)
			, service(mp)
			, instanceId(mp)
			, title(mp)
			, desc(mp)
		{}
		Log(Log const&) = delete;
		Log& operator=(Log const&) = delete;
	};
	using Log_p = Ptr<Log>;


	/*
	// 日志写入器主体类. 示例:

	inline xx::Logger* logger = nullptr;
int main(int argc, char const* const* argv)
{
	logger = new xx::Logger(argv[0], true);
	logger->SetDefaultValue("", "test_cpp5", "0");

	...

	logger->Write.............

	...

	delete logger;
	logger = nullptr;


	在数据浏览工具中查询, time 字段转 DateTime:
	select *, datetime(`time`/10000000,'unixepoch') as `dt` from log

	*/
	class Logger
	{
	protected:
		MemPool mp;
		SQLite db;
		SQLiteQuery_p query_InsertLog;

		static constexpr int nameLenLimit = 200;
		String machine;
		String service;
		String instanceId;

		// 为每个 queue 创建一个 mp　专用．这样只需要在切换时 lock
		MemPool mp1;
		Queue<Log_p> logs1;					// 切换使用

		MemPool mp2;
		Queue<Log_p> logs2;					// 切换使用

		Queue<Log_p>* logs;					// 指向当前正在使用的 logs
		Queue<Log_p>* bgLogs;				// 指向后台 logs
		std::mutex mtx;

		uint64_t limit = 0;					// 写等待队列深度限制. 如果非0, 写入队列如果超限, 将导致写入失败, 抛弃本次写入
		int64_t dbLimit = 0;				// 表数据行数限制. 如果非0, insert 的同时将执行 delete 删除最早的数据, 以维持不超过设定的总行数

		bool disposing = false;				// 通知后台线程退出的标志位

	public:
		int64_t counter = 0;				// 写入行数计数器

		// todo: dbLimit 相关代码

		// 可传入完整路径文件名, 或前缀 argv[0], true 以便将日志创建到和 exe 所在位置相同. 
		Logger(std::string fn, bool fnIsPrefix = false, uint64_t limit = 0, int64_t dbLimit = 0) noexcept
			: db(&mp, !fnIsPrefix ? fn.c_str() : (fn + ".log.db3").c_str())
			, machine(&mp, nameLenLimit)
			, service(&mp, nameLenLimit)
			, instanceId(&mp, nameLenLimit)
			, logs1(&mp1)
			, logs2(&mp2)
			, logs(&logs1)
			, bgLogs(&logs2)
			, limit(limit)
			, dbLimit(dbLimit)
		{
			if (!db.TableExists("log"))
			{
				auto q = db.CreateQuery(R"=-=(
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
				q->Execute();
			}

			// 初始化插入查询
			query_InsertLog = db.CreateQuery(R"=-=(
insert into [log] ([level], [time], [machine], [service], [instanceId], [title], [opcode], [desc]) 
values (?, ?, ?, ?, ?, ?, ?, ?))=-=");

			// todo: 查 id 最大最小值存起来备用

			// 起一个后台线程用于日志写库
			std::thread t([this]
			{
				while (true)
				{
					// 切换前后台队列( 如果有数据. 没有就 sleep 一下继续扫 )
					{
						std::lock_guard<std::mutex> lg(mtx);
						if (!logs->Count()) goto LabEnd;
						std::swap(logs, bgLogs);
					}

					// 开始批量插入( 这期间前台可以继续操作 )
					try
					{
						db.BeginTransaction();
						while (!bgLogs->Empty())
						{
							auto& o = *bgLogs->Top();
							if (o.id == 1)
							{
								InsertLog(o.level, o.time, machine, service, instanceId, o.title, o.opcode, o.desc);
							}
							else if (o.id == 2)
							{
								InsertLog(o.level, o.time, o.machine, o.service, o.instanceId, o.title, o.opcode, o.desc);
							}
							else
							{
								machine.Assign(o.machine);
								service.Assign(o.service);
								instanceId.Assign(o.instanceId);
							}
							bgLogs->Pop();
							++counter;
						}
						db.EndTransaction();
					}
					catch (...)
					{
						// 似乎只能忽略错误
						std::cout << "logdb insert error! errNO = " << db.lastErrorCode << " errMsg = " << db.lastErrorMessage << std::endl;
					}
				LabEnd:
					if (disposing) break;
					Sleep(50);
				}
				disposing = false;
			});
			t.detach();
		}

		Logger(Logger const&) = delete;
		Logger operator=(Logger const&) = delete;

		~Logger() noexcept
		{
			disposing = true;
			while (disposing) Sleep(1);
		}


		// 向 db 插入一条 log. time 传入 NowEpoch10m
		template<typename MachineType, typename ServiceType, typename InstanceIdType, typename TitleType, typename DescType>
		inline void InsertLog
		(
			LogLevel const& level,
			int64_t const& time,
			MachineType const& machine,
			ServiceType const& service,
			InstanceIdType const& instanceId,
			TitleType const& title,
			int64_t const& opcode,
			DescType const& desc
		) noexcept
		{
			query_InsertLog->SetParameters(level, time, machine, service, instanceId, title, opcode, desc);
			query_InsertLog->Execute();
		}



		// 完整写入所有参数. 返回 false 表示写入失败
		template<typename MachineType, typename ServiceType, typename InstanceIdType, typename TitleType, typename DescType>
		bool WriteAll(LogLevel const& level, int64_t const& time
			, MachineType const& machine, ServiceType const& service, InstanceIdType const& instanceId
			, TitleType const& title, int64_t const& opcode, DescType const& desc) noexcept
		{
			std::lock_guard<std::mutex> lg(mtx);
			if (limit && logs->Count() > limit) return false;

			auto o = logs->mempool->MPCreatePtr<Log>();

			o->id = 2;             // 用来标记是通过 WaitAll 写入的
			o->level = level;
			o->time = time;
			o->machine.Assign(machine);
			o->service.Assign(service);
			o->instanceId.Assign(instanceId);
			o->title.Assign(title);
			o->opcode = opcode;
			o->desc.Assign(desc);

			logs->Emplace(std::move(o));
			return true;
		}

		// 设置部分写入参数的默认值, 进一步可以使用下面的简化形态函数. 返回 false 表示设置失败
		template<typename MachineType, typename ServiceType, typename InstanceIdType>
		bool SetDefaultValue(MachineType const& machine, ServiceType const& service, InstanceIdType const& instanceId) noexcept
		{
			std::lock_guard<std::mutex> lg(mtx);
			if (limit && logs->Count() > limit) return false;

			auto o = logs->mempool->MPCreatePtr<Log>();

			o->id = 0;				// 用来标记是通过 SetDefaultValue 写入的
			o->machine.Assign(machine);
			o->service.Assign(service);
			o->instanceId.Assign(instanceId);

			logs->Emplace(std::move(o));
			return true;
		}

		// 下面的 write 需要先 SetDefaultValue 才能用

		template<typename TitleType, typename DescType>
		bool Write(LogLevel level, TitleType const& title, int64_t const& opcode, DescType const& desc) noexcept
		{
			std::lock_guard<std::mutex> lg(mtx);
			if (limit && logs->Count() > limit) return false;

			auto o = logs->mempool->MPCreatePtr<Log>();

			o->id = 1;             // 用来标记是通过 Write 写入的
			o->level = level;

			o->time = NowEpoch10m();
			o->title.Assign(title);
			o->opcode = opcode;
			o->desc.Assign(desc);

			logs->Emplace(std::move(o));
			return true;
		}

		template<typename TitleType, typename DescType>
		bool WriteInfo(TitleType const& title, int64_t const& opcode, DescType const& desc) noexcept
		{
			return Write(LogLevel::Info, title, opcode, desc);
		}

		template<typename TitleType, typename DescType>
		bool WriteError(TitleType const& title, int64_t const& opcode, DescType const& desc) noexcept
		{
			return Write(LogLevel::Error, title, opcode, desc);
		}

		template<typename DescType>
		bool WriteInfo(DescType const& desc) noexcept
		{
			return Write(LogLevel::Info, "", 0, desc);
		}

		template<typename DescType>
		bool WriteError(DescType const& desc) noexcept
		{
			return Write(LogLevel::Error, "", 0, desc);
		}

		template<typename DescType>
		bool Write(DescType const& desc) noexcept
		{
			return Write(LogLevel::Info, "", 0, desc);
		}

		template<typename...TS>
		bool WriteFormat(char const* const& format, TS const&...vs) noexcept
		{
			String s(&mp);
			s.AppendFormat(format, vs...);
			return Write(LogLevel::Info, "", 0, s);
		}

		template<typename...TS>
		bool WriteAppend(TS const&...vs) noexcept
		{
			String s(&mp);
			s.Append(vs...);
			return Write(LogLevel::Info, "", 0, s);
		}
	};
}
