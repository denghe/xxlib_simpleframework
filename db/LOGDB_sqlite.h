#pragma once
#include "LOGDB_class.h"
#include "xx_sqlite.h"

namespace LOGDB
{
    // 日志操作相关
    struct SQLiteFuncs
    {
		xx::SQLite& sqlite;
		xx::MemPool& mp;
		xx::SQLiteString_v s;

		inline SQLiteFuncs(xx::SQLite& sqlite)
            : sqlite(sqlite)
            , mp(sqlite.mempool())
            , s(mp)
        {
        }


        xx::SQLiteQuery_p query_CreateTable_log;
        // 建 log 表
        inline void CreateTable_log()
        {
			auto& q = query_CreateTable_log;

			if (!q)
			{
				q = sqlite.CreateQuery(R"=-=(
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
			auto& q = query_InsertLog;

			if (!q)
			{
				q = sqlite.CreateQuery(R"=-=(
insert into [log] ([level], [time], [machine], [service], [instanceId], [title], [opcode], [desc]) 
values (?, ?, ?, ?, ?, ?, ?, ?))=-=");
			}
            q->SetParameters(level, time, machine, service, instanceId, title, opcode, desc);
            q->Execute();
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
			auto& q = query_InsertLog;

			if (!q)
			{
				q = sqlite.CreateQuery(R"=-=(
insert into [log] ([level], [time], [machine], [service], [instanceId], [title], [opcode], [desc]) 
values (?, ?, ?, ?, ?, ?, ?, ?))=-=");
			}
            q->SetParameters(level, time, machine, service, instanceId, title, opcode, desc);
            q->Execute();
        }


        xx::SQLiteQuery_p query_DeleteLog;
        // 根据 id 删掉一条 log 
        inline void DeleteLog
        (
            int64_t const& id
        )
        {
			auto& q = query_DeleteLog;

			if (!q)
			{
				q = sqlite.CreateQuery(R"=-=(delete from [log] where [id] = ?)=-=");
			}
            q->SetParameters(id);
            q->Execute();
        }


        xx::SQLiteQuery_p query_DeleteLogByTimeRange;
        // 根据 time 范围( 大于等于, 小于等于 ) 删掉一批 log 
        inline void DeleteLogByTimeRange
        (
            int64_t const& timeFrom,
            int64_t const& timeTo
        )
        {
			auto& q = query_DeleteLogByTimeRange;

			if (!q)
			{
				q = sqlite.CreateQuery(R"=-=(delete from [log] where [time] >= ? and [time] <= ?)=-=");
			}
            q->SetParameters(timeFrom, timeFrom);
            q->Execute();
        }


        xx::SQLiteQuery_p query_SelectLogs;
        // 获取所有数据
        inline xx::List_p<LOGDB::Log_p> SelectLogs()
        {
			auto& q = query_SelectLogs;

			if (!q)
			{
				q = sqlite.CreateQuery(R"=-=(select [id], [level], [time], [machine], [service], [instanceId], [title], [opcode], [desc] from [log])=-=");
			}
            xx::List_p<LOGDB::Log_p> rtv;
            rtv.Create(mp);
			q->Execute([&](xx::SQLiteReader& sr)
            {
				auto& r = rtv->EmplaceMP();
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
			auto& q = query_SelectLog;

			if (!q)
			{
				q = sqlite.CreateQuery(R"=-=(
select [id], [level], [time], [machine], [service], [instanceId], [title], [opcode], [desc]
  from [log]
 where [id] = ?;)=-=");
			}
            xx::List_p<int64_t> rtv;
            rtv.Create(mp);
            q->SetParameters(id);
            q->Execute([&](xx::SQLiteReader& sr)
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
			auto& q = query_SelectLogsByTimeRange;

			if (!q)
			{
				q = sqlite.CreateQuery(R"=-=(delete from [log] where [time] >= ? and [time] <= ?)=-=");
			}
            xx::List_p<LOGDB::Log_p> rtv;
            rtv.Create(mp);
            q->SetParameters(timeFrom, timeFrom);
			q->Execute([&](xx::SQLiteReader& sr)
            {
				auto& r = rtv->EmplaceMP();
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
