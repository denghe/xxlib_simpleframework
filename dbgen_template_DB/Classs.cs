#pragma warning disable 0169, 0414
using TemplateLibrary;

[Desc("对应 account 账号表")]
class Account
{
    [Column(true), Desc("自增主键")]
    long id;

    [Column, CreateInstance, Desc("用户名( 唯一索引 )")]
    string username;

    [Column, CreateInstance, Desc("密码( 无索引 )")]
    string password;
}
