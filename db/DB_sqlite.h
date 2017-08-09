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

		SQLiteInitFuncs(xx::SQLite& sqlite)
            : sqlite(sqlite)
            , mp(sqlite.mempool())
            , s(mp)
        {
        }


        xx::SQLiteQuery_p query_CreateTable_game_account;
        // 建一系列表
        void CreateTable_game_account()
        {
			auto& q = query_CreateTable_game_account;

			if (!q)
			{
				q = sqlite.CreateQuery(R"=-=(
CREATE TABLE [game_account](
    [id] INTEGER PRIMARY KEY AUTOINCREMENT NOT NULL UNIQUE, 
    [username] TEXT(50) NOT NULL UNIQUE, 
    [password] TEXT(50) NOT NULL
);
)=-=");
			}
            q->Execute();
        }


        xx::SQLiteQuery_p query_CreateTable_manage_account;
        // 建一系列表
        void CreateTable_manage_account()
        {
			auto& q = query_CreateTable_manage_account;

			if (!q)
			{
				q = sqlite.CreateQuery(R"=-=(
CREATE TABLE [manage_account](
    [id] INTEGER PRIMARY KEY AUTOINCREMENT NOT NULL UNIQUE, 
    [username] TEXT(50) NOT NULL UNIQUE, 
    [password] TEXT(50) NOT NULL
);
)=-=");
			}
            q->Execute();
        }


        xx::SQLiteQuery_p query_CreateTable_manage_permission;
        // 建一系列表
        void CreateTable_manage_permission()
        {
			auto& q = query_CreateTable_manage_permission;

			if (!q)
			{
				q = sqlite.CreateQuery(R"=-=(
CREATE TABLE [manage_permission](
    [id] INTEGER PRIMARY KEY NOT NULL UNIQUE, 
    [name] TEXT(50) NOT NULL UNIQUE, 
    [desc] TEXT(250) NOT NULL,
    [group] TEXT(50) NOT NULL 
);
)=-=");
			}
            q->Execute();
        }


        xx::SQLiteQuery_p query_CreateTable_manage_role;
        // 建一系列表
        void CreateTable_manage_role()
        {
			auto& q = query_CreateTable_manage_role;

			if (!q)
			{
				q = sqlite.CreateQuery(R"=-=(
CREATE TABLE [manage_role](
    [id] INTEGER PRIMARY KEY NOT NULL UNIQUE, 
    [name] TEXT(50) NOT NULL UNIQUE, 
    [desc] TEXT(250) NOT NULL
);
)=-=");
			}
            q->Execute();
        }


        xx::SQLiteQuery_p query_CreateTable_manage_bind_role_permission;
        // 建一系列表
        void CreateTable_manage_bind_role_permission()
        {
			auto& q = query_CreateTable_manage_bind_role_permission;

			if (!q)
			{
				q = sqlite.CreateQuery(R"=-=(
CREATE TABLE [manage_bind_role_permission](
    [role_id] INTEGER NOT NULL REFERENCES manage_role([id]), 
    [permission_id] INTEGER NOT NULL REFERENCES manage_permission([id]), 
    PRIMARY KEY([role_id], [permission_id])
);
)=-=");
			}
            q->Execute();
        }


        xx::SQLiteQuery_p query_CreateTable_manage_bind_account_role;
        // 建一系列表
        void CreateTable_manage_bind_account_role()
        {
			auto& q = query_CreateTable_manage_bind_account_role;

			if (!q)
			{
				q = sqlite.CreateQuery(R"=-=(
CREATE TABLE [manage_bind_account_role](
    [account_id] INTEGER NOT NULL REFERENCES manage_account([id]), 
    [role_id] INTEGER NOT NULL REFERENCES manage_role([id]), 
    PRIMARY KEY([account_id], [role_id])
);
)=-=");
			}
            q->Execute();
        }
    };
    struct SQLiteGameFuncs
    {
		xx::SQLite& sqlite;
		xx::MemPool& mp;
		xx::SQLiteString_v s;

		SQLiteGameFuncs(xx::SQLite& sqlite)
            : sqlite(sqlite)
            , mp(sqlite.mempool())
            , s(mp)
        {
        }


        xx::SQLiteQuery_p query_GetAll_GameAccount;
        xx::List_p<DB::Game::Account_p> GetAll_GameAccount()
        {
			auto& q = query_GetAll_GameAccount;

			if (!q)
			{
				q = sqlite.CreateQuery(R"=-=(
    select [id], [username], [password]
      from [game_account])=-=");
			}
            xx::List_p<DB::Game::Account_p> rtv;
            rtv.Create(mp);
			q->Execute([&](xx::SQLiteReader& sr)
            {
				auto& r = rtv->EmplaceMP();
                r->id = sr.ReadInt64(0);
                r->username = mp.Create<xx::String>(sr.ReadString(1));
                r->password = mp.Create<xx::String>(sr.ReadString(2));
            });
			return rtv;
        }


        xx::SQLiteQuery_p query_AddAccount;
        void AddAccount
        (
            xx::String_p const& username,
            xx::String_p const& password
        )
        {
			auto& q = query_AddAccount;

			if (!q)
			{
				q = sqlite.CreateQuery(R"=-=(
    insert into [game_account] ([username], [password])
    values (?, ?)
    )=-=");
			}
            q->SetParameters(username, password);
            q->Execute();
        }
    };
}
