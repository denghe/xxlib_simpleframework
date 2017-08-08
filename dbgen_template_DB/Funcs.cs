#pragma warning disable 0169, 0414
using TemplateLibrary;


[SQLite]
partial interface SQLiteInitFuncs
{
    [Desc("建一系列表")]
    [Sql(@"
CREATE TABLE [game_account](
    [id] INTEGER PRIMARY KEY AUTOINCREMENT NOT NULL UNIQUE, 
    [username] TEXT(50) NOT NULL UNIQUE, 
    [password] TEXT(50) NOT NULL
);
")]
    void CreateTable_game_account();

    [Desc("建一系列表")]
    [Sql(@"
CREATE TABLE [manage_account](
    [id] INTEGER PRIMARY KEY AUTOINCREMENT NOT NULL UNIQUE, 
    [username] TEXT(50) NOT NULL UNIQUE, 
    [password] TEXT(50) NOT NULL
);
")]
    void CreateTable_manage_account();

    [Desc("建一系列表")]
    [Sql(@"
CREATE TABLE [manage_permission](
    [id] INTEGER PRIMARY KEY NOT NULL UNIQUE, 
    [name] TEXT(50) NOT NULL UNIQUE, 
    [desc] TEXT(250) NOT NULL,
    [group] TEXT(50) NOT NULL 
);
")]
    void CreateTable_manage_permission();

    [Desc("建一系列表")]
    [Sql(@"
CREATE TABLE [manage_role](
    [id] INTEGER PRIMARY KEY NOT NULL UNIQUE, 
    [name] TEXT(50) NOT NULL UNIQUE, 
    [desc] TEXT(250) NOT NULL
);
")]
    void CreateTable_manage_role();

    [Desc("建一系列表")]
    [Sql(@"
CREATE TABLE [manage_bind_role_permission](
    [role_id] INTEGER NOT NULL REFERENCES manage_role([id]), 
    [permission_id] INTEGER NOT NULL REFERENCES manage_permission([id]), 
    PRIMARY KEY([role_id], [permission_id])
);
")]
    void CreateTable_manage_bind_role_permission();

    [Desc("建一系列表")]
    [Sql(@"
CREATE TABLE [manage_bind_account_role](
    [account_id] INTEGER NOT NULL REFERENCES manage_account([id]), 
    [role_id] INTEGER NOT NULL REFERENCES manage_role([id]), 
    PRIMARY KEY([account_id], [role_id])
);
")]
    void CreateTable_manage_bind_account_role();

}





[SQLite]
partial interface SQLiteGameFuncs
{



//    [Desc("数据插入")]
//    [Sql(@"
//insert into [account] ([username], [password])
//values ({0}, {1})
//")]
//    void AddAccount(string username, string password);


//    [Desc("根据用户名查找并返回账号. 未找到将返回 null")]
//    [Sql(@"
//select [id], [username], [password]
//  from [account]
// where [username] = {0}")]
//    Game.Account GetAccountByUsername(string username);


//    [Desc("根据用户名s查找并返回账号s. 未找到将返回 0 长 List")]
//    [Sql(@"
//select [id], [username], [password]
//  from [account]
// where [username] in {0}")]
//    List<Game.Account> GetAccountsByUsernames(List<string> usernames);


//    [Desc("根据用户名s查找并返回Ids. 未找到将返回 0 长 List")]
//    [Sql(@"
//select [id]
//  from [account]
// where [username] in {0}")]
//    List<long> GetAccountIdsByUsernames(List<string> usernames);


//    [Desc("根据用户名s查找并返回Ids. 未找到将返回 0 长 List")]
//    [Sql(@"
//select [id]
//  from [account]
// where [username] in {0}")]
//    List<long?> GetAccountIdsByUsernames2(List<string> usernames);

}
