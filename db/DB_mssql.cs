using System;
using System.Collections.Generic;
using System.Text;
using System.Data.SqlClient;
using System.Diagnostics;

namespace DB
{
    public static class MsSqlAppendExt
    {
        public static void MsSqlAppend<T>(this StringBuilder sb, T v)
        {
            sb.Append(v);
        }

        public static void MsSqlAppend<T>(this StringBuilder sb, T? o) where T : struct
        {
            if (o.HasValue)
            {
                sb.MsSqlAppend(o.Value);
            }
            else
            {
                sb.Append("null");
            }
        }

        public static void MsSqlAppend(this StringBuilder sb, string v)
        {
            sb.Append("'" + v.Replace("'", "''") + "'");
        }

        public static void MsSqlAppend(this StringBuilder sb, DateTime o)
        {
            sb.Append("'" + o.ToString("yyyy-MM-dd HH:mm:ss") + "'");
        }

        public static void MsSqlAppend<T>(this StringBuilder sb, List<T> os) { Debug.Assert(os.Count > 0); for (int i = 0; i < os.Count; ++i) { sb.MsSqlAppend(os[i]); sb.Append(", "); }; if (os.Count > 0) sb.Length -= 2; }
        public static void MsSqlAppend(this StringBuilder sb, List<DateTime> os) { Debug.Assert(os.Count > 0); sb.Append("( "); for (int i = 0; i < os.Count; ++i) { sb.MsSqlAppend(os[i]); sb.Append(", "); }; sb.Length -= 2; sb.Append(" )"); }
        public static void MsSqlAppend(this StringBuilder sb, List<byte> os) { Debug.Assert(os.Count > 0); sb.Append("( "); for (int i = 0; i < os.Count; ++i) { sb.Append(os[i]); sb.Append(", "); }; sb.Length -= 2; sb.Append(" )"); }
        public static void MsSqlAppend(this StringBuilder sb, List<ushort> os) { Debug.Assert(os.Count > 0); sb.Append("( "); for (int i = 0; i < os.Count; ++i) { sb.Append(os[i]); sb.Append(", "); }; sb.Length -= 2; sb.Append(" )"); }
        public static void MsSqlAppend(this StringBuilder sb, List<uint> os) { Debug.Assert(os.Count > 0); sb.Append("( "); for (int i = 0; i < os.Count; ++i) { sb.Append(os[i]); sb.Append(", "); }; sb.Length -= 2; sb.Append(" )"); }
        public static void MsSqlAppend(this StringBuilder sb, List<ulong> os) { Debug.Assert(os.Count > 0); sb.Append("( "); for (int i = 0; i < os.Count; ++i) { sb.Append(os[i]); sb.Append(", "); }; sb.Length -= 2; sb.Append(" )"); }
        public static void MsSqlAppend(this StringBuilder sb, List<sbyte> os) { Debug.Assert(os.Count > 0); sb.Append("( "); for (int i = 0; i < os.Count; ++i) { sb.Append(os[i]); sb.Append(", "); }; sb.Length -= 2; sb.Append(" )"); }
        public static void MsSqlAppend(this StringBuilder sb, List<short> os) { Debug.Assert(os.Count > 0); sb.Append("( "); for (int i = 0; i < os.Count; ++i) { sb.Append(os[i]); sb.Append(", "); }; sb.Length -= 2; sb.Append(" )"); }
        public static void MsSqlAppend(this StringBuilder sb, List<int> os) { Debug.Assert(os.Count > 0); sb.Append("( "); for (int i = 0; i < os.Count; ++i) { sb.Append(os[i]); sb.Append(", "); }; sb.Length -= 2; sb.Append(" )"); }
        public static void MsSqlAppend(this StringBuilder sb, List<long> os) { Debug.Assert(os.Count > 0); sb.Append("( "); for (int i = 0; i < os.Count; ++i) { sb.Append(os[i]); sb.Append(", "); }; sb.Length -= 2; sb.Append(" )"); }
        public static void MsSqlAppend(this StringBuilder sb, List<double> os) { Debug.Assert(os.Count > 0); sb.Append("( "); for (int i = 0; i < os.Count; ++i) { sb.Append(os[i]); sb.Append(", "); }; sb.Length -= 2; sb.Append(" )"); }
        public static void MsSqlAppend(this StringBuilder sb, List<float> os) { Debug.Assert(os.Count > 0); sb.Append("( "); for (int i = 0; i < os.Count; ++i) { sb.Append(os[i]); sb.Append(", "); }; sb.Length -= 2; sb.Append(" )"); }
        public static void MsSqlAppend(this StringBuilder sb, List<bool> os) { Debug.Assert(os.Count > 0); sb.Append("( "); for (int i = 0; i < os.Count; ++i) { sb.Append(os[i]); sb.Append(", "); }; sb.Length -= 2; sb.Append(" )"); }


        // todo: Nullable<Enum>, BBuffer

        public static void MsSqlAppend(this StringBuilder sb, List<Account> os, bool ignoreReadOnly = false) { Debug.Assert(os.Count > 0); for (int i = 0; i < os.Count; ++i) { sb.MsSqlAppend(os[i], ignoreReadOnly); sb.Append(", "); }; if (os.Count > 0) sb.Length -= 2; }
        public static void MsSqlAppend(this StringBuilder sb, Account o, bool ignoreReadOnly = false)
        {
            sb.Append("(");
            if (!ignoreReadOnly)
            {
            sb.Append(o.id);
            sb.Append(", ");
            }
            sb.MsSqlAppend(o.username);
            sb.Append(", ");
            sb.MsSqlAppend(o.password);
            sb.Append(", ");
            sb.Length -= 2;
            sb.Append(")");
        }
    }

}
