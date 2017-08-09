#pragma warning disable 0169, 0414
using TemplateLibrary;

namespace Game
{

    [Desc("对应 game_account 账号表")]
    class Account
    {
        [Desc("自增主键")]
        long id;

        [NotNull, Desc("用户名( 唯一索引 )")]
        string username;

        [NotNull, Desc("密码( 无索引 )")]
        string password;
    }

}

namespace Manage
{
    [Desc("对应 manage_account 账号表")]
    class Account
    {
        [Desc("自增主键")]
        long id;

        [NotNull, Desc("用户名( 唯一索引 )")]
        string username;

        [NotNull, Desc("密码( 无索引 )")]
        string password;
    }

    [Desc("对应 manage_role 身份表")]
    class Role
    {
        [Desc("主键")]
        int id;

        [Desc("身份名")]
        string name;

        [Desc("备注")]
        string desc;
    }

    [Desc("对应 manage_permission 权限表")]
    class Permission
    {
        [Desc("主键")]
        int id;

        [Desc("分组依据( 仅用于显示 )")]
        string group;

        [Desc("权限名")]
        string name;

        [Desc("备注")]
        string desc;
    }

    [Desc("对应 manage_bind_account_role 管理人员 & 身份 绑定表")]
    class BindAccountRole
    {
        [Desc("管理人员主键")]
        int account_id;

        [Desc("身份主键")]
        int role_id;
    }

    [Desc("对应 manage_bind_role_permission 身份 & 权限 绑定表")]
    class BindRolePermission
    {
        [Desc("身份主键")]
        int role_id;

        [Desc("权限主键")]
        int permission_id;
    }
}
