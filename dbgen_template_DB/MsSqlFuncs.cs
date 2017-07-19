#pragma warning disable 0169, 0414
using TemplateLibrary;

[MsSql]
partial interface MsSqlFuncs
{

    [Desc("根据用户名查找( 脏读 )并返回账号. 未找到将返回 null")]
    [Sql(@"
select [id], [username], [password], [coin], [createtime]
  from [account] with(nolock)
 where [username] = {0}")]
    Account Account_GetByUsername(string username);



    [Desc(@"直接将指定用户的金币改成指定值( for test )")]
    [Sql(@"
update [account]
   set [coin] = {1}
 where [id] = {0}")]
    void Account_SetCoin(int id, long val);



    [Desc(@"获取指定用户的钱数( for test )")]
    [Sql(@"
select [coin]
  from [account]
 where [id] = {0}")]
    long? Account_GetCoin(int id);



    [Desc(@"获取指定用户的钱变化日志( for test )")]
    [Sql(@"
select [id], [account_id], [original], [value], [current], [reason], cast( [createtime] as datetime )
  from [coin_changelog]
 where [account_id] = {0}")]
    List<CoinChangeLog> Account_GetCoinChangeLogs(int id);



    [Desc(@"清空指定用户的钱变化日志( for test )")]
    [Sql(@"
delete
  from [coin_changelog]
 where [account_id] = {0}")]
    void Account_ClearCoinChangeLogs(int id);



    [Desc(@"给指定用户加 / 减金币. 
传入 用户id, 原始值, 加/减多少, 原因, 创建时间
返回当前值( 如果等同于先前值, 表示操作失败, 返回空表示 id 未找到 ). ")]
    [Sql(@"
update [account]
   set [coin] = [coin] + {2}
 where [id] = {0} and [coin] = {1};
if @@ROWCOUNT = 0
    select [coin]
      from [account]
     where [id] = {0};
else begin
    insert into [coin_changelog] 
           ( [account_id], [original], [value], [current], [reason], [createtime] )
    values ( {0}, {1}, {2}, {1} + {2}, {3}, {4}  );
    select cast( ({1} + {2}) as bigint );
end")]
    // todo: 已知问题, 后半段 insert into 有可能失败, 比如外键约束啥的. 
    long? Account_RelativeChangeCoin(int id, long original, long val, string reason, DateTime createtime);
   
}
