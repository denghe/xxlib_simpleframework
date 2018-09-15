using System;
using xx;
namespace LOGDB
{
    public static class PkgGenMd5
    {
        public const string value = "3a19241f3d54204cda9f65ed628fac5f"; 
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
    public partial class Log : IObject
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

        public virtual ushort GetPackageId()
        {
            return TypeIdMaps<Log>.typeId;
        }

        public virtual void ToBBuffer(BBuffer bb)
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

        public virtual void FromBBuffer(BBuffer bb)
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
        public virtual void ToString(ref System.Text.StringBuilder str)
        {
            if (GetToStringFlag())
            {
        	    str.Append("[ \"***** recursived *****\" ]");
        	    return;
            }
            else SetToStringFlag(true);

            str.Append("{ \"pkgTypeName\":\"Log\", \"pkgTypeId\":" + GetPackageId());
            ToStringCore(ref str);
            str.Append(" }");
        
            SetToStringFlag(false);
        }
        public virtual void ToStringCore(ref System.Text.StringBuilder str)
        {
            str.Append(", \"id\":" + id);
            str.Append(", \"level\":" + level);
            str.Append(", \"time\":" + time);
            if (machine != null) str.Append(", \"machine\":\"" + machine + "\"");
            else str.Append(", \"machine\":nil");
            if (service != null) str.Append(", \"service\":\"" + service + "\"");
            else str.Append(", \"service\":nil");
            if (instanceId != null) str.Append(", \"instanceId\":\"" + instanceId + "\"");
            else str.Append(", \"instanceId\":nil");
            if (title != null) str.Append(", \"title\":\"" + title + "\"");
            else str.Append(", \"title\":nil");
            str.Append(", \"opcode\":" + opcode);
            if (desc != null) str.Append(", \"desc\":\"" + desc + "\"");
            else str.Append(", \"desc\":nil");
        }
        public override string ToString()
        {
            var sb = new System.Text.StringBuilder();
            ToString(ref sb);
            return sb.ToString();
        }
        bool toStringFlag;
        public void SetToStringFlag(bool doing)
        {
            toStringFlag = doing;
        }
        public bool GetToStringFlag()
        {
            return toStringFlag;
        }
        public virtual void MySqlAppend(ref System.Text.StringBuilder sb, bool ignoreReadOnly)
        {
        }
    }
    public static class AllTypes
    {
        public static void Register()
        {
            xx.BBuffer.RegisterInternals();
            BBuffer.Register<Log>(3);
        }
    }
}
