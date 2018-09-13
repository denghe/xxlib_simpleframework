using System;
using System.Runtime.InteropServices;

namespace xx
{
    /// <summary>
    /// xx 框架基础接口. 类似 C++ 下的 xx::Object
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
        void ToString(ref System.Text.StringBuilder sb);
        void ToStringCore(ref System.Text.StringBuilder sb);
        void SetToStringFlag(bool doing);
        bool GetToStringFlag();

        // MySql查询拼接相关
        void MySqlAppend(ref System.Text.StringBuilder sb, bool ignoreReadOnly);
    }

    /// <summary>
    /// 存放类型到 typeid 的映射 for 序列化
    /// </summary>
    public sealed class TypeIdMaps<T>
    {
        public static ushort typeId = 0xFFFF;
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
