#pragma once
#include "xx.h"

namespace LOGDB
{
	struct PkgGenMd5
	{
		static constexpr char const* value = "cef7d80349e61ed9ade7587729d512e3";
    };

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
	    Log(xx::MemPool* const& mempool) noexcept;
	    Log(xx::BBuffer* const& bb);
		Log(Log const&) = delete;
		Log& operator=(Log const&) = delete;
        void ToString(xx::String& s) const noexcept override;
        void ToStringCore(xx::String& s) const noexcept override;
        void ToBBuffer(xx::BBuffer& bb) const noexcept override;
        int FromBBuffer(xx::BBuffer& bb) noexcept override;
        int FromBBufferCore(xx::BBuffer& bb) noexcept;
        void CopyTo(Log* const& o) const noexcept;
        Log* MakeCopy() const noexcept;
        Log_p MakePtrCopy() const noexcept;
        inline static xx::Ptr<ThisType> defaultInstance;
    };
}
namespace xx
{
	template<> struct TypeId<LOGDB::Log> { static const uint16_t value = 3; };
}
namespace LOGDB
{
	inline Log::Log(xx::MemPool* const& mempool) noexcept
        : xx::Object(mempool)
	{
	}
	inline Log::Log(xx::BBuffer* const& bb)
        : xx::Object(bb)
	{
        if (int r = FromBBufferCore(*bb)) throw r;
	}
    inline void Log::ToBBuffer(xx::BBuffer& bb) const noexcept
    {
        bb.Write(this->id);
        bb.Write(this->level);
        bb.Write(this->time);
        bb.Write(this->machine);
        bb.Write(this->service);
        bb.Write(this->instanceId);
        bb.Write(this->title);
        bb.Write(this->opcode);
        bb.Write(this->desc);
    }
    inline int Log::FromBBuffer(xx::BBuffer& bb) noexcept
    {
        return this->FromBBufferCore(bb);
    }
    inline int Log::FromBBufferCore(xx::BBuffer& bb) noexcept
    {
        if (int r = bb.Read(this->id)) return r;
        if (int r = bb.Read(this->level)) return r;
        if (int r = bb.Read(this->time)) return r;
        bb.readLengthLimit = 0;
        if (int r = bb.Read(this->machine)) return r;
        bb.readLengthLimit = 0;
        if (int r = bb.Read(this->service)) return r;
        bb.readLengthLimit = 0;
        if (int r = bb.Read(this->instanceId)) return r;
        bb.readLengthLimit = 0;
        if (int r = bb.Read(this->title)) return r;
        if (int r = bb.Read(this->opcode)) return r;
        bb.readLengthLimit = 0;
        if (int r = bb.Read(this->desc)) return r;
        return 0;
    }

    inline void Log::ToString(xx::String& s) const noexcept
    {
        if (this->memHeader().flags)
        {
        	s.Append("[ \"***** recursived *****\" ]");
        	return;
        }
        else this->memHeader().flags = 1;

        s.Append("{ \"pkgTypeName\":\"Log\", \"pkgTypeId\":", xx::TypeId_v<ThisType>);
        ToStringCore(s);
        s.Append(" }");
        
        this->memHeader().flags = 0;
    }
    inline void Log::ToStringCore(xx::String& s) const noexcept
    {
        this->BaseType::ToStringCore(s);
        s.Append(", \"id\":", this->id);
        s.Append(", \"level\":", this->level);
        s.Append(", \"time\":", this->time);
        if (this->machine) s.Append(", \"machine\":\"", this->machine, "\"");
        else s.Append(", \"machine\":nil");
        if (this->service) s.Append(", \"service\":\"", this->service, "\"");
        else s.Append(", \"service\":nil");
        if (this->instanceId) s.Append(", \"instanceId\":\"", this->instanceId, "\"");
        else s.Append(", \"instanceId\":nil");
        if (this->title) s.Append(", \"title\":\"", this->title, "\"");
        else s.Append(", \"title\":nil");
        s.Append(", \"opcode\":", this->opcode);
        if (this->desc) s.Append(", \"desc\":\"", this->desc, "\"");
        else s.Append(", \"desc\":nil");
    }
    inline void Log::CopyTo(Log* const& o) const noexcept
    {
        o->id = this->id;
        o->level = this->level;
        o->time = this->time;
        o->machine = this->machine;
        o->service = this->service;
        o->instanceId = this->instanceId;
        o->title = this->title;
        o->opcode = this->opcode;
        o->desc = this->desc;
    }
    inline Log* Log::MakeCopy() const noexcept
    {
        auto o = mempool->MPCreate<Log>();
        this->CopyTo(o);
        return o;
    }
    inline Log_p Log::MakePtrCopy() const noexcept
    {
        return Log_p(this->MakeCopy());
    }

}
namespace LOGDB
{
	inline void AllTypesRegister() noexcept
	{
        xx::MemPool::RegisterInternals();
	    xx::MemPool::Register<LOGDB::Log, xx::Object>();
	}
}
