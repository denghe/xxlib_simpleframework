#pragma warning disable 0169, 0414
using TemplateLibrary;

[SQLite]
partial interface SQLiteFuncs
{
    [Desc("建 account 表")]
    [Sql(@"
create table [account]
(
    [id] integer primary key autoincrement, 
    [username] text(64) not null unique, 
    [password] text(64) not null
)")]
    void CreateAccountTable();

    [Desc("数据插入")]
    [Sql(@"
insert into [account] ([username], [password])
values (?, ?)
")]
    void AddAccount(string username, string password);


    [Desc("根据用户名查找并返回账号. 未找到将返回 null")]
    [Sql(@"
select [id], [username], [password]
  from [account]
 where [username] = ?")]
    Account GetAccountByUsername(string username);

}
