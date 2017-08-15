#pragma warning disable 0169, 0414
using TemplateLibrary;


[Desc("日志级别")]
enum Level
{
    Off,
    Fatal,
    Error,
    Warning,
    Info,
    Debug,
    All
}

[Desc("对应 log 日志表")]
class Log
{
    [Desc("自增主键")]
    long id;

    [Desc("日志级别")]
    Level level;

    [NotNull, Desc("发生时间")]
    long time;

    [NotNull, Desc("机器名")]
    string machine;

    [NotNull, Desc("进程/服务名")]
    string service;

    [NotNull, Desc("进程/实例id")]
    string instanceId;

    [NotNull, Desc("标题")]
    string title;

    [Desc("操作代码")]
    long opcode;

    [NotNull, Desc("日志明细")]
    string desc;
}
