#pragma warning disable 0169, 0414
using TemplateLibrary;

// 大厅 -> 客户端
namespace Lobby_Client
{
    [Desc("大厅进入成功")]
    class JoinSuccess
    {
        [Desc("当前金币数")]
        long coin;

        // todo: more properties
    }

    [Desc("拉取房间列表成功: 内含具体的列表")]
    class PullRoomsSuccess
    {
        List<Global.RoomInfo> rooms;
    }

    [Desc("拉取房间列表失败: 内含原因")]
    class PullRoomsFail
    {
        string reason;    // todo: 原因枚举化
    }

    [Desc("申请进入房间成功: 内含连接信息")]
    class JoinRoomSuccess
    {
        string token;
        Global.IPAddress serviceAddress;
    }

    [Desc("申请进入房间失败: 内含原因")]
    class JoinRoomFail
    {
        string reason;    // todo: 原因枚举化
    }














    [Desc("金币变化之推送消息 向客户端转发( 不带账号id )")]
    class SyncCoin
    {
        [Desc("版本号( 如果从多个渠道收到通知则保留该值最大的 )")]
        int versionNumber;

        [Desc("原金币值")]
        long original;

        [Desc("加减值")]
        long val;

        [Desc("当前金币值")]
        long curr;

        [Desc("操作原因")]
        string reason;
    }


    [Desc("试修改金币 成功")]
    class ChangeCoinSuccess
    {
    }

    [Desc("试修改金币 失败")]
    class ChangeCoinFail
    {
        string reason;
    }

}
































/*

    [Desc("大厅进入失败, 被重定向到其他服务")]
    class JoinRedirect
    {
        string token;
        Global.ServiceTypes serviceType;
        Global.IPAddress serviceAddress;
    }

    [Desc("匹配游戏成功")]
    class MatchSuccess
    {
        string token;
        Global.ServiceTypes serviceType;
        Global.IPAddress serviceAddress;
    }

 */
