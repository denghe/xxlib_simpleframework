#pragma once
#include "xx.h"
#include <optional>


namespace LOGDB
{
    // 对应 log 日志表
    class Log;
    using Log_p = xx::Ptr<Log>;
	using Log_r = xx::Ref<Log>;

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
    class Log : public xx::Object
    {
    public:
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
        virtual void ToString(xx::String &s) const override;
        virtual void ToStringCore(xx::String &s) const override;
    };


	inline Log::Log()
	{
	}

    inline void Log::ToString(xx::String &s) const
    {
        if (memHeader().flags)
        {
        	s.Append("[ \"***** recursived *****\" ]");
        	return;
        }
        else memHeader().flags = 1;

        s.Append("{ \"type\" : \"Log\"");
        ToStringCore(s);
        s.Append(" }");
        
        memHeader().flags = 0;
    }
    inline void Log::ToStringCore(xx::String &s) const
    {
        this->BaseType::ToStringCore(s);
        str.Append(", \"id\" : ", this->id);
        str.Append(", \"level\" : ", this->level);
        str.Append(", \"time\" : ", this->time);
        if (this->machine) str.Append(", \"machine\" : \"", this->machine, "\"");
        else str.Append(", \"machine\" : nil");
        if (this->service) str.Append(", \"service\" : \"", this->service, "\"");
        else str.Append(", \"service\" : nil");
        if (this->instanceId) str.Append(", \"instanceId\" : \"", this->instanceId, "\"");
        else str.Append(", \"instanceId\" : nil");
        if (this->title) str.Append(", \"title\" : \"", this->title, "\"");
        else str.Append(", \"title\" : nil");
        str.Append(", \"opcode\" : ", this->opcode);
        if (this->desc) str.Append(", \"desc\" : \"", this->desc, "\"");
        else str.Append(", \"desc\" : nil");
    }
}