#pragma once
#include "DB_class.h"
#include "xx_sqlite.h"

namespace DB
{
    struct SQLiteInitFuncs
    {
		xx::SQLite& sqlite;
		xx::MemPool& mp;
		xx::SQLiteString_v s;
		bool hasError = false;
		int const& lastErrorCode() { return sqlite.lastErrorCode; }
		const char* const& lastErrorMessage() { return sqlite.lastErrorMessage; }

		SQLiteInitFuncs(xx::SQLite& sqlite) : sqlite(sqlite), mp(sqlite.mempool()), s(mp) {}


        xx::SQLiteQuery_p query_CreateTable_Account;
        // 建 account 表
        void CreateTable_Account()
        {
			hasError = true;
			auto& q = query_CreateTable_Account;

			if (!q)
			{
				q = sqlite.CreateQuery(R"=-=(
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
    };
    struct SQLiteFuncs
    {
		xx::SQLite& sqlite;
		xx::MemPool& mp;
		xx::SQLiteString_v s;
		bool hasError = false;
		int const& lastErrorCode() { return sqlite.lastErrorCode; }
		const char* const& lastErrorMessage() { return sqlite.lastErrorMessage; }

		SQLiteFuncs(xx::SQLite& sqlite) : sqlite(sqlite), mp(sqlite.mempool()), s(mp) {}


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

			if (!q)
			{
				q = sqlite.CreateQuery(R"=-=(
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

			if (!q)
			{
				q = sqlite.CreateQuery(R"=-=(
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
                rtv->username = mp.Create<xx::String>(sr.ReadString(1));
                rtv->password = mp.Create<xx::String>(sr.ReadString(2));
            })) return rtv;
			hasError = false;
			return rtv;
        }


        xx::SQLiteQuery_p query_GetAccountsByUsernames;
        // 根据用户名s查找并返回账号s. 未找到将返回 0 长 List
        xx::List_p<DB::Account_p> GetAccountsByUsernames
        (
            xx::List_p<xx::String_p> const& usernames
        )
        {
			hasError = true;
			auto& q = query_GetAccountsByUsernames;

            s->Clear();
            s->Append(R"=-=(
select [id], [username], [password]
  from [account]
 where [username] in )=-=");
            s->SQLAppend(usernames);
            q = sqlite.CreateQuery(s->C_str(), s->dataLen);
            xx::List_p<DB::Account_p> rtv;
			if (!q) return rtv;
            rtv.Create(mp);
			if (!q->Execute([&](xx::SQLiteReader& sr)
            {
				auto& r = rtv->EmplaceMP();
                r->id = sr.ReadInt64(0);
                r->username = mp.Create<xx::String>(sr.ReadString(1));
                r->password = mp.Create<xx::String>(sr.ReadString(2));
            }))
			{
				rtv = nullptr;
				return rtv;
			}
			hasError = false;
			return rtv;
        }


        xx::SQLiteQuery_p query_GetAccountIdsByUsernames;
        // 根据用户名s查找并返回Ids. 未找到将返回 0 长 List
        xx::List_p<int64_t> GetAccountIdsByUsernames
        (
            xx::List_p<xx::String_p> const& usernames
        )
        {
			hasError = true;
			auto& q = query_GetAccountIdsByUsernames;

            s->Clear();
            s->Append(R"=-=(
select [id]
  from [account]
 where [username] in )=-=");
            s->SQLAppend(usernames);
            q = sqlite.CreateQuery(s->C_str(), s->dataLen);
            xx::List_p<int64_t> rtv;
			if (!q) return rtv;
            rtv.Create(mp);
            if (!q->Execute([&](xx::SQLiteReader& sr)
            {
				rtv->Add(sr.ReadInt64(0));
            }))
			{
				rtv = nullptr;
				return rtv;
			}
			hasError = false;
			return rtv;
        }


        xx::SQLiteQuery_p query_GetAccountIdsByUsernames2;
        // 根据用户名s查找并返回Ids. 未找到将返回 0 长 List
        xx::List_p<std::optional<int64_t>> GetAccountIdsByUsernames2
        (
            xx::List_p<xx::String_p> const& usernames
        )
        {
			hasError = true;
			auto& q = query_GetAccountIdsByUsernames2;

            s->Clear();
            s->Append(R"=-=(
select [id]
  from [account]
 where [username] in )=-=");
            s->SQLAppend(usernames);
            q = sqlite.CreateQuery(s->C_str(), s->dataLen);
            xx::List_p<std::optional<int64_t>> rtv;
			if (!q) return rtv;
            rtv.Create(mp);
            if (!q->Execute([&](xx::SQLiteReader& sr)
            {
				if (sr.IsDBNull(0)) rtv->Emplace();
                else rtv->Add(sr.ReadInt64(0));
            }))
			{
				rtv = nullptr;
				return rtv;
			}
			hasError = false;
			return rtv;
        }
    };
}
