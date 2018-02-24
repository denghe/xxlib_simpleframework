#pragma warning disable 0169, 0414
using TemplateLibrary;

// Client 向 Server 发的包
namespace Client_Server
{
    [Desc("请求进入服务器")]
    class Join : Request
    {
        [Limit(16)]
        string username;
        [Limit(16)]
        string password;
    }

    [Desc("发消息")]
    class Message
    {
        [Limit(256)]
        string text;
    }

    [Desc("主动退出")]
    class Logout
    {
        UserInfo ui;    // 测试一下 lua 生成过滤依赖
    }
}
