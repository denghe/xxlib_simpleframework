#pragma warning disable 0169, 0414
using TemplateLibrary;

// Manage 向 DB 发的包( Manage 指代管理端, 可能是 web 或 app )
namespace Manage_DB
{
    [Desc("请求进入服务器")]
    class Login : Request
    {
        [Limit(50)]
        string username;
        [Limit(50)]
        string password;
    }


    [Desc("注销式退出")]
    class Logout : Request
    {
    }

    // 下列指令需要相关 身份 / 权限

    [Desc("拉管理员账号列表的 id 列表( 需要相关权限 )")]
    class SelectManageAccountIds : Request
    {
        [Desc("限制获取的 id 总个数")]
        int limit;

        [Desc("字段排序规则( 当前先只支持对一个字段排序 )")]
        Manage.AccountSorts sort;
    }

    [Desc("拉管理员账号列表( 需要相关权限 )")]
    class SelectManageAccounts : Request
    {
        [Desc("要拉的 id 列表")]
        [Limit(50)]
        List<long> ids;
    }
}
