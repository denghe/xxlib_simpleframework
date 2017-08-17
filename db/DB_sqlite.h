#pragma once
#include "DB_class.h"
#include "xx_sqlite.h"

namespace DB
{
    // 游戏相关
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


        xx::SQLiteQuery_p query_CreateTable_game_account;
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
);)=-=");
			}
            q->Execute();
        }


        xx::SQLiteQuery_p query_SelectAccountByUsername;
        // 根据用户名查找并返回账号. 未找到将返回 null
        DB::Game::Account_p SelectAccountByUsername
        (
            xx::String_p const& username
        )
        {
			auto& q = query_SelectAccountByUsername;

			if (!q)
			{
				q = sqlite.CreateQuery(R"=-=(
    select [id], [username], [password]
      from [game_account]
     where [username] = ?)=-=");
			}
            DB::Game::Account_p rtv;
            rtv.Create(mp);
            q->SetParameters(username);
			q->Execute([&](xx::SQLiteReader& sr)
            {
                rtv->id = sr.ReadInt64(0);
                rtv->username = mp.Create<xx::String>(sr.ReadString(1));
                rtv->password = mp.Create<xx::String>(sr.ReadString(2));
            });
            return rtv;
        }


        // xx::SQLiteQuery_p query_SelectAccountByUsername;
        // 根据用户名查找并返回账号. 未找到将返回 null
        DB::Game::Account_p SelectAccountByUsername
        (
            char const* const& username
        )
        {
			auto& q = query_SelectAccountByUsername;

			if (!q)
			{
				q = sqlite.CreateQuery(R"=-=(
    select [id], [username], [password]
      from [game_account]
     where [username] = ?)=-=");
			}
            DB::Game::Account_p rtv;
            rtv.Create(mp);
            q->SetParameters(username);
			q->Execute([&](xx::SQLiteReader& sr)
            {
                rtv->id = sr.ReadInt64(0);
                rtv->username = mp.Create<xx::String>(sr.ReadString(1));
                rtv->password = mp.Create<xx::String>(sr.ReadString(2));
            });
            return rtv;
        }
    };
    // 管理后台相关
    struct SQLiteManageFuncs
    {
		xx::SQLite& sqlite;
		xx::MemPool& mp;
		xx::SQLiteString_v s;

		SQLiteManageFuncs(xx::SQLite& sqlite)
            : sqlite(sqlite)
            , mp(sqlite.mempool())
            , s(mp)
        {
        }


        xx::SQLiteQuery_p query_CreateTable_manage_account;
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
);)=-=");
			}
            q->Execute();
        }


        xx::SQLiteQuery_p query_CreateTable_manage_permission;
        void CreateTable_manage_permission()
        {
			auto& q = query_CreateTable_manage_permission;

			if (!q)
			{
				q = sqlite.CreateQuery(R"=-=(
CREATE TABLE [manage_permission](
    [id] INTEGER PRIMARY KEY NOT NULL UNIQUE, 
    [name] TEXT(50) NOT NULL, 
    [desc] TEXT(250) NOT NULL, 
    [group] TEXT(50) NOT NULL, 
    UNIQUE([name], [group])
);)=-=");
			}
            q->Execute();
        }


        xx::SQLiteQuery_p query_CreateTable_manage_role;
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
);)=-=");
			}
            q->Execute();
        }


        xx::SQLiteQuery_p query_CreateTable_manage_bind_role_permission;
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
);)=-=");
			}
            q->Execute();
        }


        xx::SQLiteQuery_p query_CreateTable_manage_bind_account_role;
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
);)=-=");
			}
            q->Execute();
        }


        xx::SQLiteQuery_p query_InsertAccount;
        // 插入一条 账号. 可能因为 username 已存在而失败
        void InsertAccount
        (
            xx::String_p const& username,
            xx::String_p const& password
        )
        {
			auto& q = query_InsertAccount;

			if (!q)
			{
				q = sqlite.CreateQuery(R"=-=(insert into [manage_account] ([username], [password]) values (?, ?))=-=");
			}
            q->SetParameters(username, password);
            q->Execute();
        }


        // xx::SQLiteQuery_p query_InsertAccount;
        // 插入一条 账号. 可能因为 username 已存在而失败
        void InsertAccount
        (
            char const* const& username,
            char const* const& password
        )
        {
			auto& q = query_InsertAccount;

			if (!q)
			{
				q = sqlite.CreateQuery(R"=-=(insert into [manage_account] ([username], [password]) values (?, ?))=-=");
			}
            q->SetParameters(username, password);
            q->Execute();
        }


        xx::SQLiteQuery_p query_InsertPermission;
        // 插入一条 权限. 可能因为 name 已存在而失败
        void InsertPermission
        (
            int64_t const& id,
            xx::String_p const& name,
            xx::String_p const& group,
            xx::String_p const& desc
        )
        {
			auto& q = query_InsertPermission;

			if (!q)
			{
				q = sqlite.CreateQuery(R"=-=(insert into [manage_permission] ([id], [name], [group], [desc]) values (?, ?, ?, ?))=-=");
			}
            q->SetParameters(id, name, group, desc);
            q->Execute();
        }


        // xx::SQLiteQuery_p query_InsertPermission;
        // 插入一条 权限. 可能因为 name 已存在而失败
        void InsertPermission
        (
            int64_t const& id,
            char const* const& name,
            char const* const& group,
            char const* const& desc
        )
        {
			auto& q = query_InsertPermission;

			if (!q)
			{
				q = sqlite.CreateQuery(R"=-=(insert into [manage_permission] ([id], [name], [group], [desc]) values (?, ?, ?, ?))=-=");
			}
            q->SetParameters(id, name, group, desc);
            q->Execute();
        }


        xx::SQLiteQuery_p query_InsertRole;
        // 插入一条 身份. 可能因为 id 已存在, name 已存在而失败
        void InsertRole
        (
            int64_t const& id,
            xx::String_p const& name,
            xx::String_p const& desc
        )
        {
			auto& q = query_InsertRole;

			if (!q)
			{
				q = sqlite.CreateQuery(R"=-=(insert into [manage_role] ([id], [name], [desc]) values (?, ?, ?))=-=");
			}
            q->SetParameters(id, name, desc);
            q->Execute();
        }


        // xx::SQLiteQuery_p query_InsertRole;
        // 插入一条 身份. 可能因为 id 已存在, name 已存在而失败
        void InsertRole
        (
            int64_t const& id,
            char const* const& name,
            char const* const& desc
        )
        {
			auto& q = query_InsertRole;

			if (!q)
			{
				q = sqlite.CreateQuery(R"=-=(insert into [manage_role] ([id], [name], [desc]) values (?, ?, ?))=-=");
			}
            q->SetParameters(id, name, desc);
            q->Execute();
        }


        xx::SQLiteQuery_p query_InsertBindAccountRole;
        // 插入一条 账号.身份 绑定
        void InsertBindAccountRole
        (
            int64_t const& accountId,
            int64_t const& roleId
        )
        {
			auto& q = query_InsertBindAccountRole;

			if (!q)
			{
				q = sqlite.CreateQuery(R"=-=(insert into [manage_bind_account_role] ([account_id], [role_id]) values (?, ?))=-=");
			}
            q->SetParameters(accountId, roleId);
            q->Execute();
        }


        xx::SQLiteQuery_p query_InsertBindRolePermission;
        // 插入一条 身份.权限 绑定
        void InsertBindRolePermission
        (
            int64_t const& roleId,
            int64_t const& permissionId
        )
        {
			auto& q = query_InsertBindRolePermission;

			if (!q)
			{
				q = sqlite.CreateQuery(R"=-=(insert into [manage_bind_role_permission] ([role_id], [permission_id]) values (?, ?))=-=");
			}
            q->SetParameters(roleId, permissionId);
            q->Execute();
        }


        xx::SQLiteQuery_p query_UpdateAccount_ChangePassword;
        // 改密码. 可能因 找不到 id 而失败
        void UpdateAccount_ChangePassword
        (
            int64_t const& id,
            xx::String_p const& newPassword
        )
        {
			auto& q = query_UpdateAccount_ChangePassword;

			if (!q)
			{
				q = sqlite.CreateQuery(R"=-=(update [manage_account] set [password] = ? where [id] = ?)=-=");
			}
            q->SetParameters(newPassword, id);
            q->Execute();
        }


        // xx::SQLiteQuery_p query_UpdateAccount_ChangePassword;
        // 改密码. 可能因 找不到 id 而失败
        void UpdateAccount_ChangePassword
        (
            int64_t const& id,
            char const* const& newPassword
        )
        {
			auto& q = query_UpdateAccount_ChangePassword;

			if (!q)
			{
				q = sqlite.CreateQuery(R"=-=(update [manage_account] set [password] = ? where [id] = ?)=-=");
			}
            q->SetParameters(newPassword, id);
            q->Execute();
        }


        xx::SQLiteQuery_p query_UpdateAccount_ChangeUsername;
        // 改用户名. 可能因 找不到 id 或 新 username 已存在而失败
        void UpdateAccount_ChangeUsername
        (
            int64_t const& id,
            xx::String_p const& newUsername
        )
        {
			auto& q = query_UpdateAccount_ChangeUsername;

			if (!q)
			{
				q = sqlite.CreateQuery(R"=-=(update [manage_account] set [username] = ? where [id] = ?)=-=");
			}
            q->SetParameters(newUsername, id);
            q->Execute();
        }


        // xx::SQLiteQuery_p query_UpdateAccount_ChangeUsername;
        // 改用户名. 可能因 找不到 id 或 新 username 已存在而失败
        void UpdateAccount_ChangeUsername
        (
            int64_t const& id,
            char const* const& newUsername
        )
        {
			auto& q = query_UpdateAccount_ChangeUsername;

			if (!q)
			{
				q = sqlite.CreateQuery(R"=-=(update [manage_account] set [username] = ? where [id] = ?)=-=");
			}
            q->SetParameters(newUsername, id);
            q->Execute();
        }


        xx::SQLiteQuery_p query_UpdateRole;
        // 更新 身份 数据. 可能因 找不到 id 或 新 name 已存在而失败
        void UpdateRole
        (
            int64_t const& id,
            xx::String_p const& newName,
            xx::String_p const& newDesc
        )
        {
			auto& q = query_UpdateRole;

			if (!q)
			{
				q = sqlite.CreateQuery(R"=-=(update [manage_role] set [name] = ?, [desc] = ? where [id] = ?)=-=");
			}
            q->SetParameters(newName, newDesc, id);
            q->Execute();
        }


        // xx::SQLiteQuery_p query_UpdateRole;
        // 更新 身份 数据. 可能因 找不到 id 或 新 name 已存在而失败
        void UpdateRole
        (
            int64_t const& id,
            char const* const& newName,
            char const* const& newDesc
        )
        {
			auto& q = query_UpdateRole;

			if (!q)
			{
				q = sqlite.CreateQuery(R"=-=(update [manage_role] set [name] = ?, [desc] = ? where [id] = ?)=-=");
			}
            q->SetParameters(newName, newDesc, id);
            q->Execute();
        }


        xx::SQLiteQuery_p query_UpdatePermission;
        // 更新 权限 数据. 可能因 找不到 id 或 新 name 已存在而失败
        void UpdatePermission
        (
            int64_t const& id,
            xx::String_p const& newGroup,
            xx::String_p const& newName,
            xx::String_p const& newDesc
        )
        {
			auto& q = query_UpdatePermission;

			if (!q)
			{
				q = sqlite.CreateQuery(R"=-=(update [manage_permission] set [group] = ?, [name] = ?, [desc] = ? where [id] = ?)=-=");
			}
            q->SetParameters(newGroup, newName, newDesc, id);
            q->Execute();
        }


        // xx::SQLiteQuery_p query_UpdatePermission;
        // 更新 权限 数据. 可能因 找不到 id 或 新 name 已存在而失败
        void UpdatePermission
        (
            int64_t const& id,
            char const* const& newGroup,
            char const* const& newName,
            char const* const& newDesc
        )
        {
			auto& q = query_UpdatePermission;

			if (!q)
			{
				q = sqlite.CreateQuery(R"=-=(update [manage_permission] set [group] = ?, [name] = ?, [desc] = ? where [id] = ?)=-=");
			}
            q->SetParameters(newGroup, newName, newDesc, id);
            q->Execute();
        }


        xx::SQLiteQuery_p query_DeleteBindAccountRoleByAccountId;
        // 根据 accountId 删掉 账号.身份 绑定数据
        void DeleteBindAccountRoleByAccountId
        (
            int64_t const& accountId
        )
        {
			auto& q = query_DeleteBindAccountRoleByAccountId;

			if (!q)
			{
				q = sqlite.CreateQuery(R"=-=(delete from [manage_bind_account_role] where [account_id] = ?)=-=");
			}
            q->SetParameters(accountId);
            q->Execute();
        }


        xx::SQLiteQuery_p query_DeleteBindAccountRoleByRoleId;
        // 根据 roleId 删掉 账号.身份 绑定数据
        void DeleteBindAccountRoleByRoleId
        (
            int64_t const& roleId
        )
        {
			auto& q = query_DeleteBindAccountRoleByRoleId;

			if (!q)
			{
				q = sqlite.CreateQuery(R"=-=(delete from [manage_bind_account_role] where [role_id] = ?)=-=");
			}
            q->SetParameters(roleId);
            q->Execute();
        }


        xx::SQLiteQuery_p query_DeleteBindRolePermissionByRoleId;
        // 根据 roleId 删掉 身份.权限 绑定数据
        void DeleteBindRolePermissionByRoleId
        (
            int64_t const& roleId
        )
        {
			auto& q = query_DeleteBindRolePermissionByRoleId;

			if (!q)
			{
				q = sqlite.CreateQuery(R"=-=(delete from [manage_bind_role_permission] where [role_id] = ?)=-=");
			}
            q->SetParameters(roleId);
            q->Execute();
        }


        xx::SQLiteQuery_p query_DeleteAccount;
        // 根据主键删一条 账号. 需要先删相关绑定, 否则可能失败. 也有可能 id 找不到而没删到数据. 要用 GetAffectedRows 的值来判断
        void DeleteAccount
        (
            int64_t const& id
        )
        {
			auto& q = query_DeleteAccount;

			if (!q)
			{
				q = sqlite.CreateQuery(R"=-=(delete from [manage_account] where [id] = ?)=-=");
			}
            q->SetParameters(id);
            q->Execute();
        }


        xx::SQLiteQuery_p query_DeletePermission;
        // 根据主键删一条 权限. 需要先删掉相关绑定, 否则可能失败
        void DeletePermission
        (
            int64_t const& id
        )
        {
			auto& q = query_DeletePermission;

			if (!q)
			{
				q = sqlite.CreateQuery(R"=-=(delete from [manage_permission] where [id] = ?)=-=");
			}
            q->SetParameters(id);
            q->Execute();
        }


        xx::SQLiteQuery_p query_DeleteRole;
        // 根据主键删一条 角色. 需要先删掉相关绑定, 否则可能失败
        void DeleteRole
        (
            int64_t const& id
        )
        {
			auto& q = query_DeleteRole;

			if (!q)
			{
				q = sqlite.CreateQuery(R"=-=(delete from [manage_role] where [id] = ?)=-=");
			}
            q->SetParameters(id);
            q->Execute();
        }


        xx::SQLiteQuery_p query_SelectAccounts;
        // 获取账号表所有数据
        xx::List_p<DB::Manage::Account_p> SelectAccounts()
        {
			auto& q = query_SelectAccounts;

			if (!q)
			{
				q = sqlite.CreateQuery(R"=-=(select [id], [username], [password] from [manage_account])=-=");
			}
            xx::List_p<DB::Manage::Account_p> rtv;
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


        xx::SQLiteQuery_p query_SelectRoles;
        // 读所有 身份 数据
        xx::List_p<DB::Manage::Role_p> SelectRoles()
        {
			auto& q = query_SelectRoles;

			if (!q)
			{
				q = sqlite.CreateQuery(R"=-=(select [id], [name], [desc] from [manage_role])=-=");
			}
            xx::List_p<DB::Manage::Role_p> rtv;
            rtv.Create(mp);
			q->Execute([&](xx::SQLiteReader& sr)
            {
				auto& r = rtv->EmplaceMP();
                r->id = sr.ReadInt32(0);
                if (!sr.IsDBNull(0)) r->name = mp.Create<xx::String>(sr.ReadString(1));
                if (!sr.IsDBNull(0)) r->desc = mp.Create<xx::String>(sr.ReadString(2));
            });
			return rtv;
        }


        xx::SQLiteQuery_p query_SelectPermissions;
        // 读所有权限数据
        xx::List_p<DB::Manage::Permission_p> SelectPermissions()
        {
			auto& q = query_SelectPermissions;

			if (!q)
			{
				q = sqlite.CreateQuery(R"=-=(select [id], [group], [name], [desc] from [manage_permission])=-=");
			}
            xx::List_p<DB::Manage::Permission_p> rtv;
            rtv.Create(mp);
			q->Execute([&](xx::SQLiteReader& sr)
            {
				auto& r = rtv->EmplaceMP();
                r->id = sr.ReadInt32(0);
                if (!sr.IsDBNull(0)) r->group = mp.Create<xx::String>(sr.ReadString(1));
                if (!sr.IsDBNull(0)) r->name = mp.Create<xx::String>(sr.ReadString(2));
                if (!sr.IsDBNull(0)) r->desc = mp.Create<xx::String>(sr.ReadString(3));
            });
			return rtv;
        }


        xx::SQLiteQuery_p query_SelectBindRolePermissions;
        // 读所有 身份 数据
        xx::List_p<DB::Manage::BindRolePermission_p> SelectBindRolePermissions()
        {
			auto& q = query_SelectBindRolePermissions;

			if (!q)
			{
				q = sqlite.CreateQuery(R"=-=(select [role_id], [permission_id] from [manage_bind_role_permission])=-=");
			}
            xx::List_p<DB::Manage::BindRolePermission_p> rtv;
            rtv.Create(mp);
			q->Execute([&](xx::SQLiteReader& sr)
            {
				auto& r = rtv->EmplaceMP();
                r->role_id = sr.ReadInt32(0);
                r->permission_id = sr.ReadInt32(1);
            });
			return rtv;
        }


        xx::SQLiteQuery_p query_SelectBindAccountRoles;
        // 读所有权限数据
        xx::List_p<DB::Manage::BindAccountRole_p> SelectBindAccountRoles()
        {
			auto& q = query_SelectBindAccountRoles;

			if (!q)
			{
				q = sqlite.CreateQuery(R"=-=(select [account_id], [role_id] from [manage_bind_account_role])=-=");
			}
            xx::List_p<DB::Manage::BindAccountRole_p> rtv;
            rtv.Create(mp);
			q->Execute([&](xx::SQLiteReader& sr)
            {
				auto& r = rtv->EmplaceMP();
                r->account_id = sr.ReadInt32(0);
                r->role_id = sr.ReadInt32(1);
            });
			return rtv;
        }


        xx::SQLiteQuery_p query_SelectAccountIdsBySortLimit;
        // 根据排序规则返回 limit 行账号记录.
        xx::List_p<int64_t> SelectAccountIdsBySortLimit
        (
            xx::String_p const& sort,
            int64_t const& limit
        )
        {
			auto& q = query_SelectAccountIdsBySortLimit;

            s->Clear();
            s->Append(R"=-=(select [id] from [manage_account] order by )=-=");
            s->Append(sort);
            s->Append(R"=-=( limit 0, )=-=");
            s->Append(limit);
            q = sqlite.CreateQuery(s->C_str(), s->dataLen);
            xx::List_p<int64_t> rtv;
            rtv.Create(mp);
            q->SetParameters(sort, limit);
            q->Execute([&](xx::SQLiteReader& sr)
            {
				rtv->Add(sr.ReadInt64(0));
            });
			return rtv;
        }


        // xx::SQLiteQuery_p query_SelectAccountIdsBySortLimit;
        // 根据排序规则返回 limit 行账号记录.
        xx::List_p<int64_t> SelectAccountIdsBySortLimit
        (
            char const* const& sort,
            int64_t const& limit
        )
        {
			auto& q = query_SelectAccountIdsBySortLimit;

            s->Clear();
            s->Append(R"=-=(select [id] from [manage_account] order by )=-=");
            s->Append(sort);
            s->Append(R"=-=( limit 0, )=-=");
            s->Append(limit);
            q = sqlite.CreateQuery(s->C_str(), s->dataLen);
            xx::List_p<int64_t> rtv;
            rtv.Create(mp);
            q->SetParameters(sort, limit);
            q->Execute([&](xx::SQLiteReader& sr)
            {
				rtv->Add(sr.ReadInt64(0));
            });
			return rtv;
        }


        xx::SQLiteQuery_p query_SelectAccountsByIds;
        // 根据用户 ids 查找并返回一批账号记录.
        xx::List_p<DB::Manage::Account_p> SelectAccountsByIds
        (
            xx::List_p<int64_t> const& ids
        )
        {
			auto& q = query_SelectAccountsByIds;

            s->Clear();
            s->Append(R"=-=(select [id], [username] from [manage_account] where [id] in )=-=");
            s->SQLAppend(ids);
            q = sqlite.CreateQuery(s->C_str(), s->dataLen);
            xx::List_p<DB::Manage::Account_p> rtv;
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


        xx::SQLiteQuery_p query_SelectAccountByUsername;
        // 根据用户名查找并返回一条账号记录. 未找到将返回 null
        DB::Manage::Account_p SelectAccountByUsername
        (
            xx::String_p const& username
        )
        {
			auto& q = query_SelectAccountByUsername;

			if (!q)
			{
				q = sqlite.CreateQuery(R"=-=(select [id], [username], [password] from [manage_account] where [username] = ?)=-=");
			}
            DB::Manage::Account_p rtv;
            rtv.Create(mp);
            q->SetParameters(username);
			q->Execute([&](xx::SQLiteReader& sr)
            {
                rtv->id = sr.ReadInt64(0);
                rtv->username = mp.Create<xx::String>(sr.ReadString(1));
                rtv->password = mp.Create<xx::String>(sr.ReadString(2));
            });
            return rtv;
        }


        // xx::SQLiteQuery_p query_SelectAccountByUsername;
        // 根据用户名查找并返回一条账号记录. 未找到将返回 null
        DB::Manage::Account_p SelectAccountByUsername
        (
            char const* const& username
        )
        {
			auto& q = query_SelectAccountByUsername;

			if (!q)
			{
				q = sqlite.CreateQuery(R"=-=(select [id], [username], [password] from [manage_account] where [username] = ?)=-=");
			}
            DB::Manage::Account_p rtv;
            rtv.Create(mp);
            q->SetParameters(username);
			q->Execute([&](xx::SQLiteReader& sr)
            {
                rtv->id = sr.ReadInt64(0);
                rtv->username = mp.Create<xx::String>(sr.ReadString(1));
                rtv->password = mp.Create<xx::String>(sr.ReadString(2));
            });
            return rtv;
        }


        xx::SQLiteQuery_p query_SelectPermissionIdsByAccountId;
        // 根据 accountId 获取 身份id列表 以及 去重后的 权限id列表
        xx::List_p<int64_t> SelectPermissionIdsByAccountId
        (
            int64_t const& accountId
        )
        {
			auto& q = query_SelectPermissionIdsByAccountId;

			if (!q)
			{
				q = sqlite.CreateQuery(R"=-=(
select distinct b.[permission_id]
  from [manage_bind_account_role] a
  join [manage_bind_role_permission] b
    on a.[role_id] = b.[role_id]
 where a.[account_id] = ?;)=-=");
			}
            xx::List_p<int64_t> rtv;
            rtv.Create(mp);
            q->SetParameters(accountId);
            q->Execute([&](xx::SQLiteReader& sr)
            {
				rtv->Add(sr.ReadInt64(0));
            });
			return rtv;
        }


        xx::SQLiteQuery_p query_SelectRoleIdsByAccountId;
        // 根据 accountId 获取 身份id列表
        xx::List_p<int64_t> SelectRoleIdsByAccountId
        (
            int64_t const& accountId
        )
        {
			auto& q = query_SelectRoleIdsByAccountId;

			if (!q)
			{
				q = sqlite.CreateQuery(R"=-=(
select [role_id]
  from [manage_bind_account_role]
 where [account_id] = ?;)=-=");
			}
            xx::List_p<int64_t> rtv;
            rtv.Create(mp);
            q->SetParameters(accountId);
            q->Execute([&](xx::SQLiteReader& sr)
            {
				rtv->Add(sr.ReadInt64(0));
            });
			return rtv;
        }
    };
}
