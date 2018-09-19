using System;
namespace LOGDB
{
    public static class PkgGenMd5
    {
        public const string value = "d19ec58ccf53bc06696e3e0eb7f3465c"; 
    }

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
    public partial class Log : xx.Object
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

        public override ushort GetPackageId()
        {
            return xx.TypeId<Log>.value;
        }

        public override void ToBBuffer(xx.BBuffer bb)
        {
            bb.Write(this.id);
            bb.Write((int)this.level);
            bb.Write(this.time);
            bb.Write(this.machine);
            bb.Write(this.service);
            bb.Write(this.instanceId);
            bb.Write(this.title);
            bb.Write(this.opcode);
            bb.Write(this.desc);
        }

        public override void FromBBuffer(xx.BBuffer bb)
        {
            bb.Read(ref this.id);
            {
                int tmp = 0;
                bb.Read(ref tmp);
                this.level = (Level)tmp;
            }
            bb.Read(ref this.time);
            bb.readLengthLimit = 0;
            bb.Read(ref this.machine);
            bb.readLengthLimit = 0;
            bb.Read(ref this.service);
            bb.readLengthLimit = 0;
            bb.Read(ref this.instanceId);
            bb.readLengthLimit = 0;
            bb.Read(ref this.title);
            bb.Read(ref this.opcode);
            bb.readLengthLimit = 0;
            bb.Read(ref this.desc);
        }
        public override void ToString(System.Text.StringBuilder s)
        {
            if (__toStringing)
            {
        	    s.Append("[ \"***** recursived *****\" ]");
        	    return;
            }
            else __toStringing = true;

            s.Append("{ \"pkgTypeName\":\"Log\", \"pkgTypeId\":" + GetPackageId());
            ToStringCore(s);
            s.Append(" }");

            __toStringing = false;
        }
        public override void ToStringCore(System.Text.StringBuilder s)
        {
            s.Append(", \"id\":" + id.ToString());
            s.Append(", \"level\":" + level.ToString());
            s.Append(", \"time\":" + time.ToString());
            if (machine != null) s.Append(", \"machine\":\"" + machine.ToString() + "\"");
            else s.Append(", \"machine\":nil");
            if (service != null) s.Append(", \"service\":\"" + service.ToString() + "\"");
            else s.Append(", \"service\":nil");
            if (instanceId != null) s.Append(", \"instanceId\":\"" + instanceId.ToString() + "\"");
            else s.Append(", \"instanceId\":nil");
            if (title != null) s.Append(", \"title\":\"" + title.ToString() + "\"");
            else s.Append(", \"title\":nil");
            s.Append(", \"opcode\":" + opcode.ToString());
            if (desc != null) s.Append(", \"desc\":\"" + desc.ToString() + "\"");
            else s.Append(", \"desc\":nil");
        }
        public override string ToString()
        {
            var sb = new System.Text.StringBuilder();
            ToString(sb);
            return sb.ToString();
        }
        public override void MySqlAppend(System.Text.StringBuilder sb, bool ignoreReadOnly)
        {
        }
    }
    public static class AllTypes
    {
        public static void Register()
        {
            xx.Object.RegisterInternals();
            xx.Object.Register<Log>(3);
        }
    }
}
