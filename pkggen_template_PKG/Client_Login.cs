#pragma warning disable 0169, 0414
using TemplateLibrary;

// 客户端 -> 登录
namespace Client_Login
{
    [Desc("客户端通过用户名和密码登录")]
    class Login
    {
        [Limit(64)]
        string username;
        [Limit(64)]
        string password;
    }
}
