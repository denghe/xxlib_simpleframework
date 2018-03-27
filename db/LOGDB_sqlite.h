#pragma once
#include "LOGDB_class.h"
#include "xx_sqlite.h"

namespace LOGDB
{
    // 日志操作相关
    class SQLiteFuncs : xx::Object
    {
    public:
		xx::SQLite& sqlite;
		xx::SQLiteString s;

		inline SQLiteFuncs(xx::SQLite& sqlite)
            : xx::Object(sqlite.mempool)
            , sqlite(sqlite)
            , s(sqlite.mempool)
        {
        }


        xx::SQLiteQuery_p query_CreateTable_log;
        // 建 log 表
        inline void CreateTable_log()
        {

			if (!this->query_CreateTable_log)
			{
				this->query_CreateTable_log = this->sqlite.CreateQuery(R"=-=(
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
            this->query_CreateTable_log->Execute();
        }


        xx::SQLiteQuery_p query_InsertLog;
        // 插入一条 log. time 传入 DateTime.Now.Ticks
        inline void InsertLog
        (
            LOGDB::Level const& level,
            int64_t const& time,
            xx::String_p const& machine,
            xx::String_p const& service,
            xx::String_p const& instanceId,
            xx::String_p const& title,
            int64_t const& opcode,
            xx::String_p const& desc
        )
        {

			if (!this->query_InsertLog)
			{
				this->query_InsertLog = this->sqlite.CreateQuery(R"=-=(
insert into [log] ([level], [time], [machine], [service], [instanceId], [title], [opcode], [desc]) 
values (?, ?, ?, ?, ?, ?, ?, ?))=-=");
			}
            this->query_InsertLog->SetParameters(level, time, machine, service, instanceId, title, opcode, desc);
            this->query_InsertLog->Execute();
        }


        // xx::SQLiteQuery_p query_InsertLog;
        // 插入一条 log. time 传入 DateTime.Now.Ticks
        inline void InsertLog
        (
            LOGDB::Level const& level,
            int64_t const& time,
            char const* const& machine,
            char const* const& service,
            char const* const& instanceId,
            char const* const& title,
            int64_t const& opcode,
            char const* const& desc
        )
        {

			if (!this->query_InsertLog)
			{
				this->query_InsertLog = this->sqlite.CreateQuery(R"=-=(
insert into [log] ([level], [time], [machine], [service], [instanceId], [title], [opcode], [desc]) 
values (?, ?, ?, ?, ?, ?, ?, ?))=-=");
			}
            this->query_InsertLog->SetParameters(level, time, machine, service, instanceId, title, opcode, desc);
            this->query_InsertLog->Execute();
        }


        xx::SQLiteQuery_p query_DeleteLog;
        // 根据 id 删掉一条 log 
        inline void DeleteLog
        (
            int64_t const& id
        )
        {

			if (!this->query_DeleteLog)
			{
				this->query_DeleteLog = this->sqlite.CreateQuery(R"=-=(delete from [log] where [id] = ?)=-=");
			}
            this->query_DeleteLog->SetParameters(id);
            this->query_DeleteLog->Execute();
        }


        xx::SQLiteQuery_p query_DeleteLogByTimeRange;
        // 根据 time 范围( 大于等于, 小于等于 ) 删掉一批 log 
        inline void DeleteLogByTimeRange
        (
            int64_t const& timeFrom,
            int64_t const& timeTo
        )
        {

			if (!this->query_DeleteLogByTimeRange)
			{
				this->query_DeleteLogByTimeRange = this->sqlite.CreateQuery(R"=-=(delete from [log] where [time] >= ? and [time] <= ?)=-=");
			}
            this->query_DeleteLogByTimeRange->SetParameters(timeFrom, timeFrom);
            this->query_DeleteLogByTimeRange->Execute();
        }


        xx::SQLiteQuery_p query_SelectLogs;
        // 获取所有数据
        inline xx::List_p<LOGDB::Log_p> SelectLogs()
        {

			if (!this->query_SelectLogs)
			{
				this->query_SelectLogs = this->sqlite.CreateQuery(R"=-=(select [id], [level], [time], [machine], [service], [instanceId], [title], [opcode], [desc] from [log])=-=");
			}
            xx::List_p<LOGDB::Log_p> rtv;
            this->mempool.MPCreateTo(rtv);
			this->query_SelectLogs->Execute([&](xx::SQLiteReader& sr)
            {
				auto& r = rtv->Emplace(this->mempool);
                r->id = sr.ReadInt64(0);
                r->level = (Level)sr.ReadInt32(1);
                r->time = sr.ReadInt64(2);
                r->machine = mp.Create<xx::String>(sr.ReadString(3));
                r->service = mp.Create<xx::String>(sr.ReadString(4));
                r->instanceId = mp.Create<xx::String>(sr.ReadString(5));
                r->title = mp.Create<xx::String>(sr.ReadString(6));
                r->opcode = sr.ReadInt64(7);
                r->desc = mp.Create<xx::String>(sr.ReadString(8));
            });
			return rtv;
        }


        xx::SQLiteQuery_p query_SelectLog;
        // 根据 主键 获取一条 log
        inline xx::List_p<int64_t> SelectLog
        (
            int64_t const& id
        )
        {

			if (!this->query_SelectLog)
			{
				this->query_SelectLog = this->sqlite.CreateQuery(R"=-=(
select [id], [level], [time], [machine], [service], [instanceId], [title], [opcode], [desc]
  from [log]
 where [id] = ?;)=-=");
			}
            xx::List_p<int64_t> rtv;
            this->mempool.MPCreateTo(rtv);
            this->query_SelectLog->SetParameters(id);
            this->query_SelectLog->Execute([&](xx::SQLiteReader& sr)
            {
				rtv->Add(sr.ReadInt64(0));
            });
			return rtv;
        }


        xx::SQLiteQuery_p query_SelectLogsByTimeRange;
        // 根据 time 范围( 大于等于, 小于等于 ) 删掉一批 log 
        inline xx::List_p<LOGDB::Log_p> SelectLogsByTimeRange
        (
            int64_t const& timeFrom,
            int64_t const& timeTo
        )
        {

			if (!this->query_SelectLogsByTimeRange)
			{
				this->query_SelectLogsByTimeRange = this->sqlite.CreateQuery(R"=-=(delete from [log] where [time] >= ? and [time] <= ?)=-=");
			}
            xx::List_p<LOGDB::Log_p> rtv;
            this->mempool.MPCreateTo(rtv);
            this->query_SelectLogsByTimeRange->SetParameters(timeFrom, timeFrom);
			this->query_SelectLogsByTimeRange->Execute([&](xx::SQLiteReader& sr)
            {
				auto& r = rtv->Emplace(this->mempool);
                r->id = sr.ReadInt64(0);
                r->level = (Level)sr.ReadInt32(1);
                r->time = sr.ReadInt64(2);
                r->machine = mp.Create<xx::String>(sr.ReadString(3));
                r->service = mp.Create<xx::String>(sr.ReadString(4));
                r->instanceId = mp.Create<xx::String>(sr.ReadString(5));
                r->title = mp.Create<xx::String>(sr.ReadString(6));
                r->opcode = sr.ReadInt64(7);
                r->desc = mp.Create<xx::String>(sr.ReadString(8));
            });
			return rtv;
        }
    };
}
