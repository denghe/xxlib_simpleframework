#pragma warning disable 0169, 0414
using TemplateLibrary;

// Manage 的相关数据结构
namespace Manage
{
    [Desc("账号获取时所用的排序规则")]
    enum AccountSorts
    {
        id_asc,
        id_desc,
        username_asc,
        username_desc,
    }

    [Desc("账号")]
    class Account
    {
        long id;
        string username;
    }

    [Desc("身份")]
    class Role
    {
        int id;
        string name;
        string desc;
    }

    [Desc("权限")]
    class Permission
    {
        int id;
        string group;
        string name;
        string desc;
    }

    [Desc("管理人员 & 身份 绑定")]
    class BindAccountRole
    {
        int account_id;
        int role_id;
    }

    [Desc("身份 & 权限 绑定")]
    class BindRolePermission
    {
        int role_id;
        int permission_id;
    }
}
