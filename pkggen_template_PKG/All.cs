#pragma warning disable 0169, 0414
using TemplateLibrary;

[Desc("一个请求( 当前限定为 service 与 db 间 ), 通常携带一个流水号. 这是基类")]
class Request
{
    int serial;
}

[Desc("一个回应( 当前限定为 service 与 db 间 ), 通常携带一个请求发过来的流水号. 这是基类")]
class Response
{
    int requestSerial;
}

// 下面是一些动态属性相关结构

class Property
{
    string name;
}
class Property_long : Property
{
    long value;
}
class Property_double : Property
{
    double value;
}
class Property_string : Property
{
    string value;
}
class Properties : Property
{
    List<Property> value;
}


class UserInfo
{
    long id;
    List<Property> props;
}

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
    class Logout { }
}

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

