using System;
namespace DB
{
    /// <summary>
    /// 对应 account 账号表
    /// </summary>
    public partial class Account
    {
        /// <summary>
        /// 主键
        /// </summary>
        public int id;
        /// <summary>
        /// 用户名( 有索引 )
        /// </summary>
        public string username;
        /// <summary>
        /// 密码( 无索引 )
        /// </summary>
        public string password;
        /// <summary>
        /// 金币数( 无索引 )
        /// </summary>
        public long coin;
        /// <summary>
        /// 创建时间( 无索引 )
        /// </summary>
        public DateTime createtime;
    }
    /// <summary>
    /// 对应 coin_changelog 金币修改日志表
    /// </summary>
    public partial class CoinChangeLog
    {
        /// <summary>
        /// 主键
        /// </summary>
        public int id;
        /// <summary>
        /// 用户id( 外键 -> account.id )
        /// </summary>
        public int account_id;
        /// <summary>
        /// 原金币数
        /// </summary>
        public long original;
        /// <summary>
        /// 操作数
        /// </summary>
        public long value;
        /// <summary>
        /// 结果 / 当前金币数
        /// </summary>
        public long current;
        /// <summary>
        /// 操作原因
        /// </summary>
        public string reason;
        /// <summary>
        /// 创建时间
        /// </summary>
        public DateTime createtime;
    }
}
