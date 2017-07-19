#pragma warning disable 0169, 0414
using TemplateLibrary;

[Desc("对应 account 账号表")]
[MultiKeyDict]
class Account
{
    [Desc("主键")]
    [Column(true), Key]
    int id;

    [Desc("用户名( 有索引 )")]
    [Column, Key]
    string username;

    [Desc("密码( 无索引 )")]
    [Column]
    string password;

    [Desc("金币数( 无索引 )")]
    [Column]
    long coin;

    [Desc("创建时间( 无索引 )")]
    [Column]
    DateTime createtime;
}

[Desc("对应 coin_changelog 金币修改日志表")]
class CoinChangeLog
{
    [Desc("主键")]
    [Column(true)]
    int id;

    [Desc("用户id( 外键 -> account.id )")]
    [Column]
    int account_id;

    [Desc("原金币数")]
    [Column]
    long original;

    [Desc("操作数")]
    [Column]
    long value;

    [Desc("结果 / 当前金币数")]
    [Column]
    long current;

    [Desc("操作原因")]
    [Column]
    string reason;

    [Desc("创建时间")]
    [Column]
    DateTime createtime;
}
