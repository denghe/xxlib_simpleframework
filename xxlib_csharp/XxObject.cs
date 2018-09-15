using System;
using System.Runtime.InteropServices;
using System.Text;

namespace xx
{
    /// <summary>
    /// 序列化基础接口. 主要为兼容 struct
    /// </summary>
    public interface IObject
    {
        // 获取包编号( 不能用 get, 无法虚覆盖 )
        ushort GetPackageId();

        // 序列化接口之序列化数据到 bb
        void ToBBuffer(BBuffer bb);

        // 序列化接口之从 bb 还原 /  填充数据( 这个需要 try )
        void FromBBuffer(BBuffer bb);

        // ToString 相关
        void ToString(StringBuilder sb);
        void ToStringCore(StringBuilder sb);
    }

    /// <summary>
    /// 类似 C++ 下的 xx::Object
    /// </summary>
    public class Object : IObject
    {
        public virtual ushort GetPackageId() { return 0; }

        public virtual void FromBBuffer(BBuffer bb) { }
        public virtual void ToBBuffer(BBuffer bb) { }

        public bool __toStringing;
        public virtual void ToString(StringBuilder sb) { }
        public virtual void ToStringCore(StringBuilder sb) { }


        // 数据库表生成 SQL 专用
        public virtual void MySqlAppend(StringBuilder sb, bool ignoreReadOnly) { }


        // 可用 Release 来模拟 C++ Ref 效果
        public virtual void Release() { __isReleased = true; }
        public bool __isReleased;

        //public static explicit operator bool(Object o)
        //{
        //    return o != null && !o.__isReleased;
        //}


        // 作为接收变量默认值比较方便
        public static readonly Object instance = new Object();


        // 类型相关的通用代码. 先放这.
        public delegate IObject TypeIdCreatorFunc();
        public static TypeIdCreatorFunc[] typeIdCreatorMappings = new TypeIdCreatorFunc[ushort.MaxValue + 1];
        public static Dict<Type, ushort> typeTypeIdMappings = new Dict<Type, ushort>();

        public static void Register<T>(ushort typeId) where T : IObject, new()
        {
            var r = typeTypeIdMappings.Add(typeof(T), typeId);
            System.Diagnostics.Debug.Assert(r.success || typeTypeIdMappings.ValueAt(r.index) == typeId);
            TypeId<T>.value = typeId;
            typeIdCreatorMappings[typeId] = () => { return new T(); };
        }
        public static void RegisterInternals()
        {
            Register<BBuffer>(2);
        }

        public static IObject CreateByTypeId(ushort typeId)
        {
            var f = typeIdCreatorMappings[typeId];
            if (f != null) return f();
            return null;
        }
    }


    // todo: 序列化器, List 都需要做 Ref<T> 相应适配

    public interface IRef
    {
        bool NotNull();
        IObject Lock();
        void Reset(IObject o);
    }

    /// <summary>
    /// 模拟 C++ 下的 xx::Ref 弱引用效果. 主体需要调用 Release() 函数并最好置空
    /// </summary>
    public struct Ref<T> : IRef where T : Object
    {
        public T pointer;

        public IObject Lock()
        {
            return this ? pointer : null;
        }
        public void Reset(IObject o = null)
        {
            pointer = (T)o;
        }

        public bool NotNull()
        {
            return this;
        }

        // 可快速检测是否已 Release
        public static implicit operator bool(Ref<T> o)
        {
            if (o.pointer != null)
            {
                if (o.pointer.__isReleased)
                {
                    o.pointer = null;
                    return false;
                }
                return true;
            }
            return false;
        }

        public override string ToString()
        {
            if (this) return pointer.ToString();
            return "nil";
        }
    }

    /// <summary>
    /// 存放类型到 typeid 的映射 for 序列化
    /// </summary>
    public sealed class TypeId<T>
    {
        public static ushort value = 0xFFFF;
    }


    public struct Pair<First, Second>
    {
        public First first;
        public Second second;
    }

    /// <summary>
    /// 用于浮点到各长度整型的快速转换 
    /// </summary>
    [StructLayout(LayoutKind.Explicit, Size = 8, CharSet = CharSet.Ansi)]
    public struct FloatingInteger
    {
        [FieldOffset(0)] public double d;
        [FieldOffset(0)] public ulong ul;
        [FieldOffset(0)] public float f;
        [FieldOffset(0)] public uint u;
        [FieldOffset(0)] public byte b0;
        [FieldOffset(1)] public byte b1;
        [FieldOffset(2)] public byte b2;
        [FieldOffset(3)] public byte b3;
        [FieldOffset(4)] public byte b4;
        [FieldOffset(5)] public byte b5;
        [FieldOffset(6)] public byte b6;
        [FieldOffset(7)] public byte b7;
    }


    public static class Utils
    {
        /// <summary>
        /// 得到当前时间点的 epoch (精度为秒后 7 个 0)
        /// </summary>
        public static long NowEpoch10m()
        {
            return DateTime.UtcNow.Ticks - 621355968000000000;
        }

        /// <summary>
        /// 时间( Local或Utc )转 epoch (精度为秒后 7 个 0)
        /// </summary>
        public static long DateTimeToEpoch10m(this DateTime dt)
        {
            return dt.ToUniversalTime().Ticks - 621355968000000000;
        }

        /// <summary>
        /// epoch (精度为秒后 7 个 0) 转为 Utc 时间
        /// </summary>
        public static DateTime Epoch10mToUtcDateTime(long epoch10m)
        {
            return new DateTime(epoch10m + 621355968000000000, DateTimeKind.Utc);
        }

        /// <summary>
        /// epoch (精度为秒后 7 个 0) 转为 本地 时间
        /// </summary>
        public static DateTime Epoch10mToLocalDateTime(long epoch10m)
        {
            var dt = new DateTime(epoch10m + 621355968000000000, DateTimeKind.Utc);
            return dt.ToLocalTime();
        }


        // 下面是精度为 秒 的版本.

        public static int NowEpoch()
        {
            return (int)(DateTime.Now.ToUniversalTime().Ticks - 621355968000000000 / 10000000);
        }

        public static int DateTimeToEpoch(this DateTime dt)
        {
            return (int)((dt.ToUniversalTime().Ticks - 621355968000000000) / 10000000);
        }

        public static DateTime EpochToUtcDateTime(int epoch)
        {
            return new DateTime((long)epoch * 10000000 + 621355968000000000, DateTimeKind.Utc);
        }
        public static DateTime EpochToLocalDateTime(int epoch)
        {
            var dt = new DateTime((long)epoch * 10000000 + 621355968000000000, DateTimeKind.Utc);
            return dt.ToLocalTime();
        }

    }
}
