using System;
namespace DB
{
namespace Manage
{
    /// <summary>
    /// 对应 manage_account 账号表
    /// </summary>
    public partial class Account
    {
        /// <summary>
        /// 自增主键
        /// </summary>
        public long id;
        /// <summary>
        /// 用户名( 唯一索引 )
        /// </summary>
        public string username;
        /// <summary>
        /// 密码( 无索引 )
        /// </summary>
        public string password;
    }
    /// <summary>
    /// 对应 manage_role 身份表
    /// </summary>
    public partial class Role
    {
        /// <summary>
        /// 主键
        /// </summary>
        public int id;
        /// <summary>
        /// 身份名
        /// </summary>
        public string name;
        /// <summary>
        /// 备注
        /// </summary>
        public string desc;
    }
    /// <summary>
    /// 对应 manage_permission 权限表
    /// </summary>
    public partial class Permission
    {
        /// <summary>
        /// 主键
        /// </summary>
        public int id;
        /// <summary>
        /// 分组依据( 仅用于显示 )
        /// </summary>
        public string group;
        /// <summary>
        /// 权限名
        /// </summary>
        public string name;
        /// <summary>
        /// 备注
        /// </summary>
        public string desc;
    }
    /// <summary>
    /// 对应 manage_bind_account_role 管理人员 & 身份 绑定表
    /// </summary>
    public partial class BindAccountRole
    {
        /// <summary>
        /// 管理人员主键
        /// </summary>
        public int account_id;
        /// <summary>
        /// 身份主键
        /// </summary>
        public int role_id;
    }
    /// <summary>
    /// 对应 manage_bind_role_permission 身份 & 权限 绑定表
    /// </summary>
    public partial class BindRolePermission
    {
        /// <summary>
        /// 身份主键
        /// </summary>
        public int role_id;
        /// <summary>
        /// 权限主键
        /// </summary>
        public int permission_id;
    }
}
namespace Game
{
    /// <summary>
    /// 对应 game_account 账号表
    /// </summary>
    public partial class Account
    {
        /// <summary>
        /// 自增主键
        /// </summary>
        public long id;
        /// <summary>
        /// 用户名( 唯一索引 )
        /// </summary>
        public string username;
        /// <summary>
        /// 密码( 无索引 )
        /// </summary>
        public string password;
    }
}
}
