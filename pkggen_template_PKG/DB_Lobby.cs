#pragma warning disable 0169, 0414
using TemplateLibrary;

// 数据库 -> 大厅
namespace DB_Lobby
{
    [Desc("试修改金币 成功")]
    class ChangeCoinSuccess : Global.Response
    {
    }

    [Desc("试修改金币 失败")]
    class ChangeCoinFail : Global.Response
    {
        [Desc("失败原因")]
        string reason;
    }

    [Desc("金币变化之消息推送( 发生于金币变化后 )")]
    class SyncCoin
    {
        [Desc("版本号( 如果从多个渠道收到通知则保留该值最大的 )")]
        int versionNumber;

        [Desc("账号id")]
        int accountId;

        [Desc("原金币值")]
        long original;

        [Desc("加减值")]
        long val;

        [Desc("当前金币值")]
        long curr;

        [Desc("操作原因")]
        string reason;
    }

    [Desc("获取用户信息成功")]
    class GetUserInfoSuccess : Global.Response
    {
        Global.UserInfo userInfo;
    }

    [Desc("获取用户信息失败")]
    class GetUserInfoFail : Global.Response
    {
        [Desc("失败原因")]
        string reason;
    }
}
