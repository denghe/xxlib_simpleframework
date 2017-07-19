#pragma warning disable 0169, 0414
using TemplateLibrary;

// 数据库 -> 登录
namespace DB_Login
{
    enum AuthResultTypes
    {
        Success,
        UsernameNotFound,
        WrongPassword,
        Disabled
    }
    [Desc("DB 给 Login 返回 Auth 结果")]
    class AuthResult : Global.Response
    {
        AuthResultTypes type;
        int accountId;
    }
}
