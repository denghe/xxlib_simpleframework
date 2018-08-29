using System;
using System.Collections.Generic;
using System.Text;
using MySql.Data.MySqlClient;
using System.Diagnostics;

namespace PKG
{
    public static class MySqlAppendExt
    {
        public static void MySqlAppend<T>(this StringBuilder sb, T v)
        {
            sb.Append(v);
        }

        public static void MySqlAppend<T>(this StringBuilder sb, T? o) where T : struct
        {
            if (o.HasValue)
            {
                sb.MySqlAppend(o.Value);
            }
            else
            {
                sb.Append("null");
            }
        }

        public static void MySqlAppend(this StringBuilder sb, string v)
        {
            sb.Append("'" + v.Replace("'", "''") + "'");
        }

        public static void MySqlAppend(this StringBuilder sb, DateTime o)
        {
            sb.Append("'" + o.ToString("yyyy-MM-dd HH:mm:ss") + "'");
        }

        public static void MySqlAppend<T>(this StringBuilder sb, List<T> os) { Debug.Assert(os.Count > 0); for (int i = 0; i < os.Count; ++i) { sb.MySqlAppend(os[i]); sb.Append(", "); }; if (os.Count > 0) sb.Length -= 2; }
        public static void MySqlAppend(this StringBuilder sb, List<DateTime> os) { Debug.Assert(os.Count > 0); sb.Append("( "); for (int i = 0; i < os.Count; ++i) { sb.MySqlAppend(os[i]); sb.Append(", "); }; sb.Length -= 2; sb.Append(" )"); }
        public static void MySqlAppend(this StringBuilder sb, List<byte> os) { Debug.Assert(os.Count > 0); sb.Append("( "); for (int i = 0; i < os.Count; ++i) { sb.Append(os[i]); sb.Append(", "); }; sb.Length -= 2; sb.Append(" )"); }
        public static void MySqlAppend(this StringBuilder sb, List<ushort> os) { Debug.Assert(os.Count > 0); sb.Append("( "); for (int i = 0; i < os.Count; ++i) { sb.Append(os[i]); sb.Append(", "); }; sb.Length -= 2; sb.Append(" )"); }
        public static void MySqlAppend(this StringBuilder sb, List<uint> os) { Debug.Assert(os.Count > 0); sb.Append("( "); for (int i = 0; i < os.Count; ++i) { sb.Append(os[i]); sb.Append(", "); }; sb.Length -= 2; sb.Append(" )"); }
        public static void MySqlAppend(this StringBuilder sb, List<ulong> os) { Debug.Assert(os.Count > 0); sb.Append("( "); for (int i = 0; i < os.Count; ++i) { sb.Append(os[i]); sb.Append(", "); }; sb.Length -= 2; sb.Append(" )"); }
        public static void MySqlAppend(this StringBuilder sb, List<sbyte> os) { Debug.Assert(os.Count > 0); sb.Append("( "); for (int i = 0; i < os.Count; ++i) { sb.Append(os[i]); sb.Append(", "); }; sb.Length -= 2; sb.Append(" )"); }
        public static void MySqlAppend(this StringBuilder sb, List<short> os) { Debug.Assert(os.Count > 0); sb.Append("( "); for (int i = 0; i < os.Count; ++i) { sb.Append(os[i]); sb.Append(", "); }; sb.Length -= 2; sb.Append(" )"); }
        public static void MySqlAppend(this StringBuilder sb, List<int> os) { Debug.Assert(os.Count > 0); sb.Append("( "); for (int i = 0; i < os.Count; ++i) { sb.Append(os[i]); sb.Append(", "); }; sb.Length -= 2; sb.Append(" )"); }
        public static void MySqlAppend(this StringBuilder sb, List<long> os) { Debug.Assert(os.Count > 0); sb.Append("( "); for (int i = 0; i < os.Count; ++i) { sb.Append(os[i]); sb.Append(", "); }; sb.Length -= 2; sb.Append(" )"); }
        public static void MySqlAppend(this StringBuilder sb, List<double> os) { Debug.Assert(os.Count > 0); sb.Append("( "); for (int i = 0; i < os.Count; ++i) { sb.Append(os[i]); sb.Append(", "); }; sb.Length -= 2; sb.Append(" )"); }
        public static void MySqlAppend(this StringBuilder sb, List<float> os) { Debug.Assert(os.Count > 0); sb.Append("( "); for (int i = 0; i < os.Count; ++i) { sb.Append(os[i]); sb.Append(", "); }; sb.Length -= 2; sb.Append(" )"); }
        public static void MySqlAppend(this StringBuilder sb, List<bool> os) { Debug.Assert(os.Count > 0); sb.Append("( "); for (int i = 0; i < os.Count; ++i) { sb.Append(os[i]); sb.Append(", "); }; sb.Length -= 2; sb.Append(" )"); }


        // todo: Nullable<Enum>, BBuffer

        public static void MySqlAppend(this StringBuilder sb, List<Foo> os, bool ignoreReadOnly = false) { Debug.Assert(os.Count > 0); for (int i = 0; i < os.Count; ++i) { sb.MySqlAppend(os[i], ignoreReadOnly); sb.Append(", "); }; if (os.Count > 0) sb.Length -= 2; }
        public static void MySqlAppend(this StringBuilder sb, Foo o, bool ignoreReadOnly = false)
        {
            sb.Append("(");
            sb.Length -= 2;
            sb.Append(")");
        }
    }

    public class MySqlFuncs
    {
        public MySqlConnection conn;
        public MySqlCommand cmd;
        public StringBuilder sb = new StringBuilder();
        private List<int> recordsAffecteds = new List<int>();

        public List<int> RecordsAffecteds
        {
            get { return recordsAffecteds; }
        }

        public int RecordsAffected
        {
            get { return recordsAffecteds[0]; }
        }

        public MySqlFuncs(MySqlConnection conn)
        {
            // conn.Open();
            this.conn = conn;
            this.cmd = conn.CreateCommand();
        }


        public int? TestSql
        (
            int? n
            , MySqlTransaction tran_ = null)
        {
            cmd.Transaction = tran_;
            recordsAffecteds.Clear();
            sb.Clear();
            sb.Append(@"select ");
            sb.MySqlAppend(n);
            cmd.CommandText = sb.ToString();

            using (var r = cmd.ExecuteReader())
            {
                recordsAffecteds.Add(r.RecordsAffected);
                if (!r.Read())
                {
                    return default(int?);
                }
                return r.IsDBNull(0) ? null : (int?)r.GetInt32(0);
            }
        }
    }
}
