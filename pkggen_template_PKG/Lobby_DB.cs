#pragma warning disable 0169, 0414
using TemplateLibrary;

// 大厅 -> 数据库
namespace Lobby_DB
{
    [Desc("试修改金币")]
    class ChangeCoin : Global.Request
    {
        [Desc("账号id")]
        int accountId;

        [Desc("原金币值")]
        long original;

        [Desc("加减值")]
        long val;

        [Desc("操作原因")]
        [Limit(250)]
        string reason;
    }

    [Desc("获取用户信息")]
    class GetUserInfo : Global.Request
    {
        [Desc("账号id")]
        int accountId;
    }
}
