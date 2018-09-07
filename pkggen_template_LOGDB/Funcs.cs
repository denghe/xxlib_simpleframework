#pragma warning disable 0169, 0414
using TemplateLibrary;


[SQLite, Desc("日志操作相关")]
partial interface SQLiteFuncs
{
    // 各种 Create

    [Desc("建 log 表")]
    [Sql(@"
CREATE TABLE [log](
    [id] INTEGER PRIMARY KEY AUTOINCREMENT NOT NULL UNIQUE, 
    [level] INT NOT NULL,
    [time] INTEGER NOT NULL, 
    [machine] TEXT(250) NOT NULL, 
    [service] TEXT(50) NOT NULL, 
    [instanceId] TEXT(50) NOT NULL, 
    [title] TEXT(250) NOT NULL,
    [opcode] INTEGER NOT NULL,
    [desc] TEXT NOT NULL
);")]
    void CreateTable_log();


    // 各种 Insert

    [Desc("插入一条 log. time 传入 DateTime.Now.Ticks")]
    [Sql(@"
insert into [log] ([level], [time], [machine], [service], [instanceId], [title], [opcode], [desc]) 
values ({0}, {1}, {2}, {3}, {4}, {5}, {6}, {7})")]
    void InsertLog(Level level, long time, string machine, string service, string instanceId, string title, long opcode, string desc);
    
    // 各种 Delete

    [Desc("根据 id 删掉一条 log ")]
    [Sql(@"delete from [log] where [id] = {0}")]
    void DeleteLog(long id);

    [Desc("根据 time 范围( 大于等于, 小于等于 ) 删掉一批 log ")]
    [Sql(@"delete from [log] where [time] >= {0} and [time] <= {0}")]
    void DeleteLogByTimeRange(long timeFrom, long timeTo);


    // 各种 Select all

    [Desc("获取所有数据")]
    [Sql(@"select [id], [level], [time], [machine], [service], [instanceId], [title], [opcode], [desc] from [log]")]
    List<Log> SelectLogs();


    // 各种条件 Select

    [Desc("根据 主键 获取一条 log")]
    [Sql(@"
select [id], [level], [time], [machine], [service], [instanceId], [title], [opcode], [desc]
  from [log]
 where [id] = {0};")]
    List<long> SelectLog(long id);


    [Desc("根据 time 范围( 大于等于, 小于等于 ) 删掉一批 log ")]
    [Sql(@"delete from [log] where [time] >= {0} and [time] <= {0}")]
    List<Log> SelectLogsByTimeRange(long timeFrom, long timeTo);
}
