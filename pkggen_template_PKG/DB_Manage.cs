#pragma warning disable 0169, 0414
using TemplateLibrary;

// DB 向 Manager 发的包
namespace DB_Manage
{
    [Desc("登录成功")]
    class LoginSuccess : Success
    {
        long id;
        string token;
    }

    [Desc("登录失败")]
    class LoginFail : Fail { }



    [Desc("注销成功")]
    class LogoutSuccess : Success { }


    [Desc("注销失败")]
    class LogoutFail : Fail { }



    [Desc("拉管理员账号列表的 id 列表 成功")]
    class SelectManageAccountIdsSuccess : Success
    {
        List<long> ids;
    }

    [Desc("拉管理员账号列表的 id 列表 失败")]

    class SelectManageAccountIdsFail : Fail { }


    [Desc("拉管理员账号列表 成功")]

    class SelectManageAccountsSuccess : Success
    {
        List<Manage.Account> rows;
    }

    [Desc("拉管理员账号列表 失败")]

    class SelectManageAccountsFail : Fail { }

}
