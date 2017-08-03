#pragma once
#include "DB_class.h"
namespace DB
{

    struct SQLiteFuncs
    {
		xx::SQLite* sqlite;
		xx::MemPool& mp;
		xx::SQLiteString_v s;
		bool hasError = false;
		int const& lastErrorCode() { return sqlite->lastErrorCode; }
		const char* const& lastErrorMessage() { return sqlite->lastErrorMessage; }

        //int recordsAffected // todo

		SQLiteFuncs(xx::SQLite* sqlite) : sqlite(sqlite), mp(sqlite->mempool()), s(mp) {}


        xx::SQLiteQuery_p query_CreateAccountTable;


        // 建 account 表
        void CreateAccountTable()
        {
			hasError = true;
			auto& q = query_CreateAccountTable;
            // recordsAffecteds.Clear();

			if (!q)
			{
				q = sqlite->CreateQuery(R"=-=(
create table [account]
(
    [id] integer primary key autoincrement, 
    [username] text(64) not null unique, 
    [password] text(64) not null
))=-=");
			}
			if (!q) return;
            if (!q->Execute()) return;
        }
        xx::SQLiteQuery_p query_AddAccount;


        // 数据插入
        void AddAccount
        (
            xx::String_p const& username,
            xx::String_p const& password
        )
        {
			hasError = true;
			auto& q = query_AddAccount;
            // recordsAffecteds.Clear();

			if (!q)
			{
				q = sqlite->CreateQuery(R"=-=(
insert into [account] ([username], [password])
values (?, ?)
)=-=");
			}
			if (!q) return;
            if (q->SetParameters(username, password)) return;

            if (!q->Execute()) return;
        }
        xx::SQLiteQuery_p query_GetAccountByUsername;


        // 根据用户名查找并返回账号. 未找到将返回 null
        DB::Account_p GetAccountByUsername
        (
            xx::String_p const& username
        )
        {
			hasError = true;
			auto& q = query_GetAccountByUsername;
            // recordsAffecteds.Clear();

			if (!q)
			{
				q = sqlite->CreateQuery(R"=-=(
select [id], [username], [password]
  from [account]
 where [username] = ?)=-=");
			}
            DB::Account_p rtv;
			if (!q) return rtv;
            rtv.Create(mp);
            if (q->SetParameters(username)) return rtv;

			if (!q->Execute([&](xx::SQLiteReader& sr)
            {
                rtv->id = sr.ReadInt64(0);
                if (!sr.IsDBNull(0)) rtv->username = mp.Create<xx::String>(sr.ReadString(1));
                if (!sr.IsDBNull(0)) rtv->password = mp.Create<xx::String>(sr.ReadString(2));
            })) return rtv;
			hasError = false;
			return rtv;
        }
    };
}
