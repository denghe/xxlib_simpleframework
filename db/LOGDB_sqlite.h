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
        template<typename LevelType_Level
            ,typename TimeType_Int64
            ,typename MachineType_String
            ,typename ServiceType_String
            ,typename InstanceIdType_String
            ,typename TitleType_String
            ,typename OpcodeType_Int64
            ,typename DescType_String>
        inline void InsertLog
        (
            LevelType_Level const& level,
            TimeType_Int64 const& time,
            MachineType_String const& machine,
            ServiceType_String const& service,
            InstanceIdType_String const& instanceId,
            TitleType_String const& title,
            OpcodeType_Int64 const& opcode,
            DescType_String const& desc
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
        template<typename IdType_Int64>
        inline void DeleteLog
        (
            IdType_Int64 const& id
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
        template<typename TimeFromType_Int64
            ,typename TimeToType_Int64>
        inline void DeleteLogByTimeRange
        (
            TimeFromType_Int64 const& timeFrom,
            TimeToType_Int64 const& timeTo
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
            this->mempool->MPCreateTo(rtv);
			this->query_SelectLogs->Execute([&](xx::SQLiteReader& sr)
            {
				auto& r = rtv->Emplace();
                this->mempool->MPCreateTo(r);
                r->id = sr.ReadInt64(0);
                r->level = (Level)sr.ReadInt32(1);
                r->time = sr.ReadInt64(2);
                r->machine = this->mempool->MPCreate<xx::String>(sr.ReadString(3));
                r->service = this->mempool->MPCreate<xx::String>(sr.ReadString(4));
                r->instanceId = this->mempool->MPCreate<xx::String>(sr.ReadString(5));
                r->title = this->mempool->MPCreate<xx::String>(sr.ReadString(6));
                r->opcode = sr.ReadInt64(7);
                r->desc = this->mempool->MPCreate<xx::String>(sr.ReadString(8));
            });
			return rtv;
        }


        xx::SQLiteQuery_p query_SelectLog;
        // 根据 主键 获取一条 log
        template<typename IdType_Int64>
        inline xx::List_p<int64_t> SelectLog
        (
            IdType_Int64 const& id
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
            this->mempool->MPCreateTo(rtv);
            this->query_SelectLog->SetParameters(id);
            this->query_SelectLog->Execute([&](xx::SQLiteReader& sr)
            {
				rtv->Add(sr.ReadInt64(0));
            });
			return rtv;
        }


        xx::SQLiteQuery_p query_SelectLogsByTimeRange;
        // 根据 time 范围( 大于等于, 小于等于 ) 删掉一批 log 
        template<typename TimeFromType_Int64
            ,typename TimeToType_Int64>
        inline xx::List_p<LOGDB::Log_p> SelectLogsByTimeRange
        (
            TimeFromType_Int64 const& timeFrom,
            TimeToType_Int64 const& timeTo
        )
        {

			if (!this->query_SelectLogsByTimeRange)
			{
				this->query_SelectLogsByTimeRange = this->sqlite.CreateQuery(R"=-=(delete from [log] where [time] >= ? and [time] <= ?)=-=");
			}
            xx::List_p<LOGDB::Log_p> rtv;
            this->mempool->MPCreateTo(rtv);
            this->query_SelectLogsByTimeRange->SetParameters(timeFrom, timeFrom);
			this->query_SelectLogsByTimeRange->Execute([&](xx::SQLiteReader& sr)
            {
				auto& r = rtv->Emplace();
                this->mempool->MPCreateTo(r);
                r->id = sr.ReadInt64(0);
                r->level = (Level)sr.ReadInt32(1);
                r->time = sr.ReadInt64(2);
                r->machine = this->mempool->MPCreate<xx::String>(sr.ReadString(3));
                r->service = this->mempool->MPCreate<xx::String>(sr.ReadString(4));
                r->instanceId = this->mempool->MPCreate<xx::String>(sr.ReadString(5));
                r->title = this->mempool->MPCreate<xx::String>(sr.ReadString(6));
                r->opcode = sr.ReadInt64(7);
                r->desc = this->mempool->MPCreate<xx::String>(sr.ReadString(8));
            });
			return rtv;
        }
    };

    using SQLiteFuncs_p = xx::Ptr<SQLiteFuncs>;
	using SQLiteFuncs_r = xx::Ref<SQLiteFuncs>;

}
