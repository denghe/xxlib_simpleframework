#pragma warning disable 0169, 0414
using TemplateLibrary;

// 大厅 -> 登录
namespace Lobby_Login
{
    [Desc("大厅 返回给 登录 给玩家分配的 token, 以及玩家接下来需要连的 服务类型 和 地址")]
    class JoinSuccess : Global.Response
    {
        int accountId;
        string token;
        [Desc("如果是断线重连, 则返回玩家当前所在服务的类型. 否则返回 Lobby")]
        Global.ServiceTypes serviceType;
        Global.IPAddress serviceAddress;
    }

    [Desc("大厅 返回给 登录 进入失败( 满员?? )")]
    class JoinFail : Global.Response
    {
        string reason;
    }
}
