using System;
namespace LOGDB
{
    /// <summary>
    /// 日志级别
    /// </summary>
    public enum Level : int
    {
        Off = 0,
        Fatal = 1,
        Error = 2,
        Warning = 3,
        Info = 4,
        Debug = 5,
        All = 6,
    }
    /// <summary>
    /// 对应 log 日志表
    /// </summary>
    public partial class Log
    {
        /// <summary>
        /// 自增主键
        /// </summary>
        public long id;
        /// <summary>
        /// 日志级别
        /// </summary>
        public Level level;
        /// <summary>
        /// 发生时间
        /// </summary>
        public long time;
        /// <summary>
        /// 机器名
        /// </summary>
        public string machine;
        /// <summary>
        /// 进程/服务名
        /// </summary>
        public string service;
        /// <summary>
        /// 进程/实例id
        /// </summary>
        public string instanceId;
        /// <summary>
        /// 标题
        /// </summary>
        public string title;
        /// <summary>
        /// 操作代码
        /// </summary>
        public long opcode;
        /// <summary>
        /// 日志明细
        /// </summary>
        public string desc;
    }
}
