#pragma once
#include <xx_bbuffer.h>
#include <optional>

namespace LOGDB
{
    // 对应 log 日志表
    struct Log;
    // 日志级别
    enum class Level : int32_t
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
    struct Log : xx::Object
    {
        // 自增主键
        int64_t id = 0;
        // 日志级别
        LOGDB::Level level = (LOGDB::Level)0;
        // 发生时间
        int64_t time = 0;
        // 机器名
        xx::String_p machine;
        // 进程/服务名
        xx::String_p service;
        // 进程/实例id
        xx::String_p instanceId;
        // 标题
        xx::String_p title;
        // 操作代码
        int64_t opcode = 0;
        // 日志明细
        xx::String_p desc;

        typedef Log ThisType;
        typedef xx::Object BaseType;
	    Log();
		Log(Log const&) = delete;
		Log& operator=(Log const&) = delete;
        virtual void ToString(xx::String &str) const override;
        virtual void ToStringCore(xx::String &str) const override;
    };
    using Log_p = xx::Ptr<Log>;
	using Log_v = xx::Dock<Log>;


	inline Log::Log()
	{
	}

    inline void Log::ToString(xx::String &str) const
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
    inline void Log::ToStringCore(xx::String &str) const
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
}
namespace xx
{
	template<>
	struct MemmoveSupport<LOGDB::Log_v>
	{
		static const bool value = true;
    };
}

namespace xx
{
}
