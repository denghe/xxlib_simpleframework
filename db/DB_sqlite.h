#pragma once
#include "DB_class.h"
#include "xx_sqlite.h"

namespace DB
{
    struct SQLiteInitFuncs
    {
		xx::SQLite& sqlite;
		xx::MemPool& mp;
		bool& hasError;
		xx::SQLiteString_v s;

		SQLiteInitFuncs(xx::SQLite& sqlite)
            : sqlite(sqlite)
            , mp(sqlite.mempool())
            , hasError(sqlite.hasError)
            , s(mp)
        {
        }


        xx::SQLiteQuery_p query_CreateTable_game_account;
        // 建一系列表
        void CreateTable_game_account()
        {
			hasError = true;
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
			if (hasError) return;
            q->Execute();
        }


        xx::SQLiteQuery_p query_CreateTable_manage_account;
        // 建一系列表
        void CreateTable_manage_account()
        {
			hasError = true;
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
			if (hasError) return;
            q->Execute();
        }


        xx::SQLiteQuery_p query_CreateTable_manage_permission;
        // 建一系列表
        void CreateTable_manage_permission()
        {
			hasError = true;
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
			if (hasError) return;
            q->Execute();
        }


        xx::SQLiteQuery_p query_CreateTable_manage_role;
        // 建一系列表
        void CreateTable_manage_role()
        {
			hasError = true;
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
			if (hasError) return;
            q->Execute();
        }


        xx::SQLiteQuery_p query_CreateTable_manage_bind_role_permission;
        // 建一系列表
        void CreateTable_manage_bind_role_permission()
        {
			hasError = true;
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
			if (hasError) return;
            q->Execute();
        }


        xx::SQLiteQuery_p query_CreateTable_manage_bind_account_role;
        // 建一系列表
        void CreateTable_manage_bind_account_role()
        {
			hasError = true;
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
			if (hasError) return;
            q->Execute();
        }
    };
    struct SQLiteGameFuncs
    {
		xx::SQLite& sqlite;
		xx::MemPool& mp;
		bool& hasError;
		xx::SQLiteString_v s;

		SQLiteGameFuncs(xx::SQLite& sqlite)
            : sqlite(sqlite)
            , mp(sqlite.mempool())
            , hasError(sqlite.hasError)
            , s(mp)
        {
        }
    };
}
