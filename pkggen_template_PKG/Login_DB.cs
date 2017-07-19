#pragma warning disable 0169, 0414
using TemplateLibrary;

// 登录 -> 数据库
namespace Login_DB
{
    [Desc("登录服务器找 DB 做登录判断")]
    class Auth : Global.Request
    {
        string username;
        string password;
    }
}
