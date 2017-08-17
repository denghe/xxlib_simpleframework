#pragma warning disable 0169, 0414
using TemplateLibrary;

// Server 向 Client 发的包
namespace Server_Client
{
    [Desc("进入成功, 返回用户信息")]
    class JoinSuccess : Response
    {
        UserInfo self;
        List<UserInfo> users;
    }
    [Desc("进入失败, 返回错误信息")]
    class JoinFail : Response
    {
        string reason;
    }

    [Desc("推送文字消息")]
    class PushJoin
    {
        long id;
    }
    [Desc("推送文字消息")]
    class PushMessage
    {
        long id;
        string text;
    }
    [Desc("推送退出消息")]
    class PushLogout
    {
        long id;
        [Desc("退出原因( 主动? 掉线?")]
        string reason;
    }
}

