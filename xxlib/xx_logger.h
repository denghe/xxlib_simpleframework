#pragma once
#include "xx.h"
#include "xx_sqlite.h"
#include <mutex>
#include <thread>

namespace xx
{
	// class Logger 主体类的代码位于 Log & LogFuncs 代码段的下方

	// 对应 log 日志表
	class Log;
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
	class Log : public Object
	{
	public:
		// 自增主键
		int64_t id = 0;
		// 日志级别
		LogLevel level = (LogLevel)0;
		// 发生时间
		int64_t time = 0;
		// 机器名
		String machine;
		// 进程/服务名
		String service;
		// 进程/实例id
		String instanceId;
		// 标题
		String title;
		// 操作代码
		int64_t opcode = 0;
		// 日志明细
		String desc;

		typedef Log ThisType;
		typedef Object BaseType;
		Log(MemPool* const& mp) noexcept;
		Log(Log const&) = delete;
		Log& operator=(Log const&) = delete;
		virtual void ToString(String &str) const noexcept override;
		virtual void ToStringCore(String &str) const noexcept override;
	};
	using Log_p = Ptr<Log>;
	using Log_r = Ref<Log>;


	inline Log::Log(MemPool* const& mp) noexcept
		: Object(mp)
		, machine(mp)
		, service(mp)
		, instanceId(mp)
		, title(mp)
		, desc(mp)
	{
	}

	inline void Log::ToString(String& str) const noexcept
	{
		if (memHeader().flags)
		{
			str.Append("[ \"***** recursived *****\" ]");
			return;
		}
		else memHeader().flags = 1;

		str.Append("{ \"type\" : \"Log\"");
		ToStringCore(str);
		str.Append(" }");

		memHeader().flags = 0;
	}
	inline void Log::ToStringCore(String& str) const noexcept
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


	// 从模板生成物复制小改而来
	struct LogFuncs
	{
		SQLite* db;
		SQLiteQuery_p query_InsertLog;

		LogFuncs(SQLite* db) noexcept
			: db(db)
		{
			if (!db->TableExists("log"))
			{
				auto q = db->CreateQuery(R"=-=(
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
			query_InsertLog = db->CreateQuery(R"=-=(
insert into [log] ([level], [time], [machine], [service], [instanceId], [title], [opcode], [desc]) 
values (?, ?, ?, ?, ?, ?, ?, ?))=-=");

		}

		// 插入一条 log. time 传入 NowEpoch10m
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

	};




	/*
	// 日志写入器主体类. 示例:

	inline xx::Logger* logger = nullptr;
int main(int argc, char const* const* argv)
{
	logger = new xx::Logger(argv[0], true);
	logger->SetDefaultValue("", "test_cpp5", "0");

	...

	logger->Write.............


	time 字段转 DateTime:
	select *, datetime(`time`/10000000,'unixepoch') as `dt` from log

	*/
	class Logger
	{
	public:
		MemPool mp;
		SQLite db;
		LogFuncs funcs;

		static constexpr int nameLenLimit = 200;
		String machine;
		String service;
		String instanceId;

		// 为每个 queue 创建一个 mp　专用．这样只需要在切换时 lock
		MemPool mp1;
		Queue<Log_p> logMsgs1;			// 切换使用

		MemPool mp2;
		Queue<Log_p> logMsgs2;			// 切换使用

		Queue<Log_p>* logMsgs;			// 指向当前正在使用的 logMsgs
		Queue<Log_p>* bakMsgs;          // 指向后台 logMsgs
		std::mutex mtx;

		uint64_t writeLimit = 0;		// 当前队列写入深度如果超过这个值就不再写入( 如果设置的话 )
		int64_t counter = 0;			// 总写入量的统计值
		bool disposing = false;			// 通知后台线程退出的标志位
		std::function<void()> OnRelease;

		// 可传入完整路径文件名, 或前缀 argv[0], true 以便将日志创建到和 exe 所在位置相同. 
		Logger(std::string fn, bool fnIsPrefix = false) noexcept
			: db(&mp, !fnIsPrefix ? fn.c_str() : (fn + ".log.db3").c_str())
			, funcs(&db)
			, machine(&mp, nameLenLimit)
			, service(&mp, nameLenLimit)
			, instanceId(&mp, nameLenLimit)
			, logMsgs1(&mp1)
			, logMsgs2(&mp2)
			, logMsgs(&logMsgs1)
			, bakMsgs(&logMsgs2)
		{
			std::thread t([this]
			{
				while (true)
				{
					// 切换前后台队列( 如果有数据. 没有就 sleep 一下继续扫 )
					{
						std::lock_guard<std::mutex> lg(mtx);
						if (!logMsgs->Count()) goto LabEnd;
						std::swap(logMsgs, bakMsgs);
					}

					// 开始批量插入( 这期间前台可以继续操作 )
					try
					{
						db.BeginTransaction();
						while (!bakMsgs->Empty())
						{
							auto& o = *bakMsgs->Top();
							if (o.id == 1)
							{
								funcs.InsertLog(o.level, o.time, machine, service, instanceId, o.title, o.opcode, o.desc);
							}
							else if (o.id == 2)
							{
								funcs.InsertLog(o.level, o.time, o.machine, o.service, o.instanceId, o.title, o.opcode, o.desc);
							}
							else
							{
								machine.Assign(o.machine);
								service.Assign(o.service);
								instanceId.Assign(o.instanceId);
							}
							bakMsgs->Pop();
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
			if (OnRelease) OnRelease();
		}


		// 完整写入所有参数. 返回 false 表示写入失败
		template<typename MachineType, typename ServiceType, typename InstanceIdType, typename TitleType, typename DescType>
		bool WriteAll(LogLevel const& level, int64_t const& time
			, MachineType const& machine, ServiceType const& service, InstanceIdType const& instanceId
			, TitleType const& title, int64_t const& opcode, DescType const& desc) noexcept
		{
			std::lock_guard<std::mutex> lg(mtx);
			if (writeLimit && logMsgs->Count() > writeLimit) return false;

			auto o = logMsgs->mempool->MPCreatePtr<Log>();

			o->id = 2;             // 用来标记是通过 WaitAll 写入的
			o->level = level;
			o->time = time;
			o->machine.Assign(machine);
			o->service.Assign(service);
			o->instanceId.Assign(instanceId);
			o->title.Assign(title);
			o->opcode = opcode;
			o->desc.Assign(desc);

			logMsgs->Emplace(std::move(o));
			return true;
		}

		// 设置部分写入参数的默认值, 进一步可以使用下面的简化形态函数. 返回 false 表示设置失败
		template<typename MachineType, typename ServiceType, typename InstanceIdType>
		bool SetDefaultValue(MachineType const& machine, ServiceType const& service, InstanceIdType const& instanceId) noexcept
		{
			std::lock_guard<std::mutex> lg(mtx);
			if (writeLimit && logMsgs->Count() > writeLimit) return false;

			auto o = logMsgs->mempool->MPCreatePtr<Log>();

			o->id = 0;				// 用来标记是通过 SetDefaultValue 写入的
			o->machine.Assign(machine);
			o->service.Assign(service);
			o->instanceId.Assign(instanceId);

			logMsgs->Emplace(std::move(o));
			return true;
		}

		// 下面的 write 需要先 SetDefaultValue 才能用

		template<typename TitleType, typename DescType>
		bool Write(LogLevel level, TitleType const& title, int64_t const& opcode, DescType const& desc) noexcept
		{
			std::lock_guard<std::mutex> lg(mtx);
			if (writeLimit && logMsgs->Count() > writeLimit) return false;

			auto o = logMsgs->mempool->MPCreatePtr<Log>();

			o->id = 1;             // 用来标记是通过 Write 写入的
			o->level = level;

			o->time = NowEpoch10m();
			o->title.Assign(title);
			o->opcode = opcode;
			o->desc.Assign(desc);

			logMsgs->Emplace(std::move(o));
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
	};
}
