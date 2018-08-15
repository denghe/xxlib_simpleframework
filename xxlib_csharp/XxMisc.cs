using System;

namespace xx
{
    public struct Pair<First, Second>
    {
        public First first;
        public Second second;
    }


    public static class Utils
    {
        /// <summary>
        /// 得到当前时间点的 epoch (精度为秒后 7 个 0)
        /// </summary>
        public static long NowEpoch10m()
        {
            return DateTime.Now.ToUniversalTime().Ticks - 621355968000000000;
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
