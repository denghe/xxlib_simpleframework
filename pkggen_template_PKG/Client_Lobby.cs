#pragma warning disable 0169, 0414
using TemplateLibrary;

// 客户端 -> 大厅
namespace Client_Lobby
{
    [Desc("进入大厅")]
    class Join
    {
        [Desc("账号id")]
        int accountId;

        [Desc("临时票据")]
        [Limit(64)]
        string token;
    }

    [Desc("拉取房间列表")]
    class PullRooms
    {
        // todo: filter args?
    }

    [Desc("申请进入房间")]
    class JoinRoom
    {
        int roomId;
    }


















    [Desc("试修改金币( 模拟充值, 输/赢 )")]
    class ChangeCoin
    {
        [Desc("原金币值")]
        long original;

        [Desc("加减值")]
        long val;

        [Desc("操作原因")]
        [Limit(250)]
        string reason;
    }

}























/*
 

    [Desc("匹配游戏")]
    class Match
    {
        Global.ServiceTypes serviceType;
    }
     
*/
