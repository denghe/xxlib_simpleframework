#pragma warning disable 0169, 0414
using TemplateLibrary;

namespace Generic
{
    [Desc("通用错误返回")]
    class Error
    {
        int errNum;
        string errMsg;
    }

    [Desc("默认 rpc 成功返回")]
    class Success
    {
    }

    // 用于声明可能会临时用于序列化的各种集合类型
    class SomeLists
    {
        List<int> ints;
        List<long> longs;
        List<string> strings;

        List<Manager> Managers;
        List<Role> Roles;
        List<Permission> Permissions;
        List<BindManagerRole> BindManagerRoles;
        List<BindRolePermission> BindRolePermissions; 
    }
}
