#pragma warning disable 0169, 0414
using TemplateLibrary;


[SQLite, Desc("表创建相关")]
partial interface SQLiteInitFuncs
{
    [Sql(@"
CREATE TABLE [game_account](
    [id] INTEGER PRIMARY KEY AUTOINCREMENT NOT NULL UNIQUE, 
    [username] TEXT(50) NOT NULL UNIQUE, 
    [password] TEXT(50) NOT NULL
);")]
    void CreateTable_game_account();

    [Sql(@"
CREATE TABLE [manage_account](
    [id] INTEGER PRIMARY KEY AUTOINCREMENT NOT NULL UNIQUE, 
    [username] TEXT(50) NOT NULL UNIQUE, 
    [password] TEXT(50) NOT NULL
);")]
    void CreateTable_manage_account();

    [Sql(@"
CREATE TABLE [manage_permission](
    [id] INTEGER PRIMARY KEY NOT NULL UNIQUE, 
    [name] TEXT(50) NOT NULL, 
    [desc] TEXT(250) NOT NULL, 
    [group] TEXT(50) NOT NULL, 
    UNIQUE([name], [group])
);")]
    void CreateTable_manage_permission();

    [Sql(@"
CREATE TABLE [manage_role](
    [id] INTEGER PRIMARY KEY NOT NULL UNIQUE, 
    [name] TEXT(50) NOT NULL UNIQUE, 
    [desc] TEXT(250) NOT NULL
);")]
    void CreateTable_manage_role();

    [Sql(@"
CREATE TABLE [manage_bind_role_permission](
    [role_id] INTEGER NOT NULL REFERENCES manage_role([id]), 
    [permission_id] INTEGER NOT NULL REFERENCES manage_permission([id]), 
    PRIMARY KEY([role_id], [permission_id])
);")]
    void CreateTable_manage_bind_role_permission();

    [Sql(@"
CREATE TABLE [manage_bind_account_role](
    [account_id] INTEGER NOT NULL REFERENCES manage_account([id]), 
    [role_id] INTEGER NOT NULL REFERENCES manage_role([id]), 
    PRIMARY KEY([account_id], [role_id])
);")]
    void CreateTable_manage_bind_account_role();

}



[SQLite, Desc("玩家登录相关")]
partial interface SQLiteLoginFuncs
{
    [Desc("根据用户名查找并返回账号. 未找到将返回 null")]
    [Sql(@"
    select [id], [username], [password]
      from [game_account]
     where [username] = {0}")]
    Game.Account GetAccountByUsername(string username);
}


[SQLite, Desc("管理后台相关")]
partial interface SQLiteManageFuncs
{

    // 各种 Insert

    [Desc("插入一条 账号. 可能因为 username 已存在而失败")]
    [Sql("insert into [manage_account] ([username], [password]) values ({0}, {1})")]
    void InsertAccount(string username, string password);


    [Desc("插入一条 权限. 可能因为 name 已存在而失败")]
    [Sql("insert into [manage_permission] ([id], [name], [group], [desc]) values ({0}, {1}, {2}, {3})")]
    void InsertPermission(long id, string name, string group, string desc);


    [Desc("插入一条 身份. 可能因为 id 已存在, name 已存在而失败")]
    [Sql(@"insert into [manage_role] ([id], [name], [desc]) values ({0}, {1}, {2})")]
    void InsertRole(long id, string name, string desc);


    [Desc("插入一条 账号.身份 绑定")]
    [Sql(@"insert into [manage_bind_account_role] ([account_id], [role_id]) values ({0}, {1})")]
    void InsertBindAccountRole(long accountId, long roleId);


    [Desc("插入一条 身份.权限 绑定")]
    [Sql(@"insert into [manage_bind_role_permission] ([role_id], [permission_id]) values ({0}, {1})")]
    void InsertBindRolePermission(long roleId, long permissionId);



    // 各种 Update

    [Desc("改密码. 可能因 找不到 id 而失败")]
    [Sql("update [manage_account] set [password] = {1} where [id] = {0}")]
    void UpdateAccount_ChangePassword(long id, string newPassword);


    [Desc("改用户名. 可能因 找不到 id 或 新 username 已存在而失败")]
    [Sql("update [manage_account] set [username] = {1} where [id] = {0}")]
    void UpdateAccount_ChangeUsername(long id, string newUsername);


    [Desc("更新 身份 数据. 可能因 找不到 id 或 新 name 已存在而失败")]
    [Sql("update [manage_role] set [name] = {1}, [desc] = {2} where [id] = {0}")]
    void UpdateRole(long id, string newName, string newDesc);


    [Desc("更新 权限 数据. 可能因 找不到 id 或 新 name 已存在而失败")]
    [Sql("update [manage_permission] set [group] = {1}, [name] = {2}, [desc] = {3} where [id] = {0}")]
    void UpdatePermission(long id, string newGroup, string newName, string newDesc);



    // 各种 Delete

    [Desc("根据 accountId 删掉 账号.身份 绑定数据")]
    [Sql(@"delete from [manage_bind_account_role] where [account_id] = {0}")]
    void DeleteBindAccountRoleByAccountId(long accountId);


    [Desc("根据 roleId 删掉 账号.身份 绑定数据")]
    [Sql(@"delete from [manage_bind_account_role] where [role_id] = {0}")]
    void DeleteBindAccountRoleByRoleId(long roleId);


    [Desc("根据 roleId 删掉 身份.权限 绑定数据")]
    [Sql(@"delete from [manage_bind_role_permission] where [role_id] = {0}")]
    void DeleteBindRolePermissionByRoleId(long roleId);


    [Desc("根据主键删一条 账号. 需要先删相关绑定, 否则可能失败. 也有可能 id 找不到而没删到数据. 要用 GetAffectedRows 的值来判断")]
    [Sql(@"delete from [manage_account] where [id] = {0}")]
    void DeleteAccount(long id);


    [Desc("根据主键删一条 权限. 需要先删掉相关绑定, 否则可能失败")]
    [Sql("delete from [manage_permission] where [id] = {0}")]
    void DeletePermission(long id);


    [Desc("根据主键删一条 角色. 需要先删掉相关绑定, 否则可能失败")]
    [Sql("delete from [manage_role] where [id] = {0}")]
    void DeleteRole(long id);


    // 各种 Select all

    [Desc("获取账号表所有数据")]
    [Sql(@"select [id], [username], [password] from [manage_account]")]
    List<Manage.Account> SelectAccounts();


    [Desc("读所有 身份 数据")]
    [Sql("select [id], [name], [desc] from [manage_role]")]
    List<Manage.Role> SelectRoles();


    [Desc("读所有权限数据")]
    [Sql("select [id], [group], [name], [desc] from [manage_permission]")]
    List<Manage.Permission> SelectPermissions();


    [Desc("读所有 身份 数据")]
    [Sql("select [role_id], [permission_id] from [manage_bind_role_permission]")]
    List<Manage.BindRolePermission> SelectBindRolePermissions();


    [Desc("读所有权限数据")]
    [Sql("select [account_id], [role_id] from [manage_bind_account_role]")]
    List<Manage.BindAccountRole> SelectBindAccountRoles();



    // 各种 Select

    [Desc("根据用户名查找并返回一条账号记录. 未找到将返回 null")]
    [Sql(@"select [id], [username], [password] from [manage_account] where [username] = {0}")]
    Manage.Account SelectAccountByUsername(string username);


    [Desc("根据 accountId 获取 身份id列表 以及 去重后的 权限id列表")]
    [Sql(@"
select distinct b.[permission_id]
  from [manage_bind_account_role] a
  join [manage_bind_role_permission] b
    on a.[role_id] = b.[role_id]
 where a.[account_id] = {0};")]
    List<long> SelectPermissionIdsByAccountId(long accountId);


    [Desc("根据 accountId 获取 身份id列表")]
    [Sql(@"
select [role_id]
  from [manage_bind_account_role]
 where [account_id] = {0};")]
    List<long> SelectRoleIdsByAccountId(long accountId);

}
