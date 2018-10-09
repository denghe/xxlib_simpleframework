#pragma warning disable 0169, 0414
using TemplateLibrary;

namespace Generic
{
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
