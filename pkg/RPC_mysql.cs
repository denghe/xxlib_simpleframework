using System;
using System.Collections.Generic;
using System.Text;
using MySql.Data.MySqlClient;
using System.Diagnostics;

namespace RPC
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

        public static void MySqlAppend(this StringBuilder sb, List<Generic.ServiceTypes> os) { Debug.Assert(os.Count > 0); sb.Append("( "); for (int i = 0; i < os.Count; ++i) { sb.Append(", "); sb.Append((int)os[i]); }; sb.Length -= 2; sb.Append(" )"); }

        // todo: Nullable<Enum>, BBuffer

        public static void MySqlAppend(this StringBuilder sb, List<DB_Manage.MsgResult> os, bool ignoreReadOnly = false) { Debug.Assert(os.Count > 0); for (int i = 0; i < os.Count; ++i) { sb.MySqlAppend(os[i], ignoreReadOnly); sb.Append(", "); }; if (os.Count > 0) sb.Length -= 2; }
        public static void MySqlAppend(this StringBuilder sb, DB_Manage.MsgResult o, bool ignoreReadOnly = false)
        {
            sb.Append("(");
            sb.Length -= 2;
            sb.Append(")");
        }
        public static void MySqlAppend(this StringBuilder sb, List<Manage_DB.Msg> os, bool ignoreReadOnly = false) { Debug.Assert(os.Count > 0); for (int i = 0; i < os.Count; ++i) { sb.MySqlAppend(os[i], ignoreReadOnly); sb.Append(", "); }; if (os.Count > 0) sb.Length -= 2; }
        public static void MySqlAppend(this StringBuilder sb, Manage_DB.Msg o, bool ignoreReadOnly = false)
        {
            sb.Append("(");
            sb.Length -= 2;
            sb.Append(")");
        }
        public static void MySqlAppend(this StringBuilder sb, List<Login_Client.LoginSuccess> os, bool ignoreReadOnly = false) { Debug.Assert(os.Count > 0); for (int i = 0; i < os.Count; ++i) { sb.MySqlAppend(os[i], ignoreReadOnly); sb.Append(", "); }; if (os.Count > 0) sb.Length -= 2; }
        public static void MySqlAppend(this StringBuilder sb, Login_Client.LoginSuccess o, bool ignoreReadOnly = false)
        {
            sb.Append("(");
            sb.Length -= 2;
            sb.Append(")");
        }
        public static void MySqlAppend(this StringBuilder sb, List<Client_Login.Login> os, bool ignoreReadOnly = false) { Debug.Assert(os.Count > 0); for (int i = 0; i < os.Count; ++i) { sb.MySqlAppend(os[i], ignoreReadOnly); sb.Append(", "); }; if (os.Count > 0) sb.Length -= 2; }
        public static void MySqlAppend(this StringBuilder sb, Client_Login.Login o, bool ignoreReadOnly = false)
        {
            sb.Append("(");
            sb.Length -= 2;
            sb.Append(")");
        }
        public static void MySqlAppend(this StringBuilder sb, List<DB_Login.AuthSuccess> os, bool ignoreReadOnly = false) { Debug.Assert(os.Count > 0); for (int i = 0; i < os.Count; ++i) { sb.MySqlAppend(os[i], ignoreReadOnly); sb.Append(", "); }; if (os.Count > 0) sb.Length -= 2; }
        public static void MySqlAppend(this StringBuilder sb, DB_Login.AuthSuccess o, bool ignoreReadOnly = false)
        {
            sb.Append("(");
            sb.Length -= 2;
            sb.Append(")");
        }
        public static void MySqlAppend(this StringBuilder sb, List<Login_DB.Auth> os, bool ignoreReadOnly = false) { Debug.Assert(os.Count > 0); for (int i = 0; i < os.Count; ++i) { sb.MySqlAppend(os[i], ignoreReadOnly); sb.Append(", "); }; if (os.Count > 0) sb.Length -= 2; }
        public static void MySqlAppend(this StringBuilder sb, Login_DB.Auth o, bool ignoreReadOnly = false)
        {
            sb.Append("(");
            sb.Length -= 2;
            sb.Append(")");
        }
        public static void MySqlAppend(this StringBuilder sb, List<Generic.ServiceInfo> os, bool ignoreReadOnly = false) { Debug.Assert(os.Count > 0); for (int i = 0; i < os.Count; ++i) { sb.MySqlAppend(os[i], ignoreReadOnly); sb.Append(", "); }; if (os.Count > 0) sb.Length -= 2; }
        public static void MySqlAppend(this StringBuilder sb, Generic.ServiceInfo o, bool ignoreReadOnly = false)
        {
            sb.Append("(");
            sb.Length -= 2;
            sb.Append(")");
        }
        public static void MySqlAppend(this StringBuilder sb, List<Generic.Error> os, bool ignoreReadOnly = false) { Debug.Assert(os.Count > 0); for (int i = 0; i < os.Count; ++i) { sb.MySqlAppend(os[i], ignoreReadOnly); sb.Append(", "); }; if (os.Count > 0) sb.Length -= 2; }
        public static void MySqlAppend(this StringBuilder sb, Generic.Error o, bool ignoreReadOnly = false)
        {
            sb.Append("(");
            sb.Length -= 2;
            sb.Append(")");
        }
        public static void MySqlAppend(this StringBuilder sb, List<Generic.Ping> os, bool ignoreReadOnly = false) { Debug.Assert(os.Count > 0); for (int i = 0; i < os.Count; ++i) { sb.MySqlAppend(os[i], ignoreReadOnly); sb.Append(", "); }; if (os.Count > 0) sb.Length -= 2; }
        public static void MySqlAppend(this StringBuilder sb, Generic.Ping o, bool ignoreReadOnly = false)
        {
            sb.Append("(");
            sb.Length -= 2;
            sb.Append(")");
        }
        public static void MySqlAppend(this StringBuilder sb, List<Generic.Pong> os, bool ignoreReadOnly = false) { Debug.Assert(os.Count > 0); for (int i = 0; i < os.Count; ++i) { sb.MySqlAppend(os[i], ignoreReadOnly); sb.Append(", "); }; if (os.Count > 0) sb.Length -= 2; }
        public static void MySqlAppend(this StringBuilder sb, Generic.Pong o, bool ignoreReadOnly = false)
        {
            sb.Append("(");
            sb.Length -= 2;
            sb.Append(")");
        }
    }

}
