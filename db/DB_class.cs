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
