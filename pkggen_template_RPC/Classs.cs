#pragma warning disable 0169, 0414
using TemplateLibrary;

namespace Generic
{
    [Desc("服务类型列表")]
    enum ServiceTypes
    {
        [Desc("登录")]
        Login,
        [Desc("数据库")]
        DB,
        [Desc("管理端")]
        Manage,
    }

    [Desc("服务间创建连接, 刚连上时, 首包需要发这个来说明自己是谁")]
    class ServiceInfo
    {
        [Desc("服务类型")]
        ServiceTypes type;
        // ...
    }

    [Desc("通用错误返回")]
    class Error
    {
        [Desc("错误码")]
        int errNo;

        [Desc("错误文字")]
        string errMsg;
    }

    [Desc("心跳保持兼延迟测试 -- 请求")]
    class Ping
    {
        double ticks;
    }

    [Desc("心跳保持兼延迟测试 -- 回应")]
    class Pong
    {
        double ticks;
    }

}

// Login 发往 DB
namespace Login_DB
{
    class Auth
    {
        string username;
        string password;
    }
}

// DB 发往 Login
namespace DB_Login
{
    class AuthSuccess
    {
        int id;
    }
}

// Client 发往 Login  ( 不可信原则, 发向服务器的, 数组性质的类型, 需要限长 )
namespace Client_Login
{
    class Login
    {
        [Limit(50)]
        string username;
        [Limit(50)]
        string password;
    }
}

// Login 发往 Client
namespace Login_Client
{
    class LoginSuccess
    {
        int id;
    }
}

// Manage 发往 DB
namespace Manage_DB
{
    class Msg
    {
        [Limit(200)]
        string txt;
    }
}

// DB 发往 Manage
namespace DB_Manage
{
    class MsgResult
    {
        string txt;
    }
}
