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
            sb.Append(o.coin);
            sb.Append(", ");
            sb.MsSqlAppend(o.createtime);
            sb.Append(", ");
            sb.Length -= 2;
            sb.Append(")");
        }
        public static void MsSqlAppend(this StringBuilder sb, List<CoinChangeLog> os, bool ignoreReadOnly = false) { Debug.Assert(os.Count > 0); for (int i = 0; i < os.Count; ++i) { sb.MsSqlAppend(os[i], ignoreReadOnly); sb.Append(", "); }; if (os.Count > 0) sb.Length -= 2; }
        public static void MsSqlAppend(this StringBuilder sb, CoinChangeLog o, bool ignoreReadOnly = false)
        {
            sb.Append("(");
            if (!ignoreReadOnly)
            {
            sb.Append(o.id);
            sb.Append(", ");
            }
            sb.Append(o.account_id);
            sb.Append(", ");
            sb.Append(o.original);
            sb.Append(", ");
            sb.Append(o.value);
            sb.Append(", ");
            sb.Append(o.current);
            sb.Append(", ");
            sb.MsSqlAppend(o.reason);
            sb.Append(", ");
            sb.MsSqlAppend(o.createtime);
            sb.Append(", ");
            sb.Length -= 2;
            sb.Append(")");
        }
    }

    public class MsSqlFuncs
    {
        private SqlConnection conn;
        private SqlCommand cmd;
        private StringBuilder sb = new StringBuilder();
        private List<int> recordsAffecteds = new List<int>();

        public List<int> RecordsAffecteds
        {
            get { return recordsAffecteds; }
        }

        public int RecordsAffected
        {
            get { return recordsAffecteds[0]; }
        }

        public MsSqlFuncs(SqlConnection conn)
        {
            // conn.Open();
            this.conn = conn;
            this.cmd = conn.CreateCommand();
        }


        /// <summary>
        /// 根据用户名查找并返回账号. 未找到将返回 null
        /// </summary>
        public Account Account_GetByUsername
        (
            string username
        )
        {
            recordsAffecteds.Clear();
            sb.Clear();
            sb.Append(@"
select [id], [username], [password], [coin], [createtime]
  from [account]
 where [username] = ");
            sb.MsSqlAppend(username);
            cmd.CommandText = sb.ToString();

            using (var r = cmd.ExecuteReader())
            {
                recordsAffecteds.Add(r.RecordsAffected);
                if (!r.Read())
                {
                    return null;
                }
                return new Account
                {
                    id = r.GetInt32(0), 
                    username = r.IsDBNull(1) ? null : (string)r.GetString(1), 
                    password = r.IsDBNull(2) ? null : (string)r.GetString(2), 
                    coin = r.GetInt64(3), 
                    createtime = r.GetDateTime(4)
                };
            }
        }

        /// <summary>
        /// 直接将指定用户的金币改成指定值( for test )
        /// </summary>
        public void Account_SetCoin
        (
            int id,
            long val
        )
        {
            recordsAffecteds.Clear();
            sb.Clear();
            sb.Append(@"
update [account]
   set [coin] = ");
            sb.Append(val);
            sb.Append(@"
 where [id] = ");
            sb.Append(id);
            cmd.CommandText = sb.ToString();

            recordsAffecteds.Add(cmd.ExecuteNonQuery());
        }

        /// <summary>
        /// 获取指定用户的钱数( for test )
        /// </summary>
        public long? Account_GetCoin
        (
            int id
        )
        {
            recordsAffecteds.Clear();
            sb.Clear();
            sb.Append(@"
select [coin]
  from [account]
 where [id] = ");
            sb.Append(id);
            cmd.CommandText = sb.ToString();

            using (var r = cmd.ExecuteReader())
            {
                recordsAffecteds.Add(r.RecordsAffected);
                if (!r.Read())
                {
                    return default(long?);
                }
                return r.GetInt64(0);
            }
        }

        /// <summary>
        /// 获取指定用户的钱变化日志( for test )
        /// </summary>
        public List<CoinChangeLog> Account_GetCoinChangeLogs
        (
            int id
        )
        {
            recordsAffecteds.Clear();
            sb.Clear();
            sb.Append(@"
select [id], [account_id], [original], [value], [current], [reason], cast( [createtime] as datetime )
  from [coin_changelog]
 where [account_id] = ");
            sb.Append(id);
            cmd.CommandText = sb.ToString();

            var rtv = new List<CoinChangeLog>();
            using (var r = cmd.ExecuteReader())
            {
                recordsAffecteds.Add(r.RecordsAffected);
                while (r.Read())
                {
                    rtv.Add(new CoinChangeLog
                    {
                        id = r.GetInt32(0),
                        account_id = r.GetInt32(1),
                        original = r.GetInt64(2),
                        value = r.GetInt64(3),
                        current = r.GetInt64(4),
                        reason = r.GetString(5),
                        createtime = r.GetDateTime(6)
                    });
                }
            }
            return rtv;
        }

        /// <summary>
        /// 清空指定用户的钱变化日志( for test )
        /// </summary>
        public void Account_ClearCoinChangeLogs
        (
            int id
        )
        {
            recordsAffecteds.Clear();
            sb.Clear();
            sb.Append(@"
delete
  from [coin_changelog]
 where [account_id] = ");
            sb.Append(id);
            cmd.CommandText = sb.ToString();

            recordsAffecteds.Add(cmd.ExecuteNonQuery());
        }

        /// <summary>
        /// 给指定用户加 / 减金币. 
        /// 传入 用户id, 原始值, 加/减多少, 原因, 创建时间
        /// 返回当前值( 如果等同于先前值, 表示操作失败, 返回空表示 id 未找到 ). 
        /// </summary>
        public long? Account_RelativeChangeCoin
        (
            int id,
            long original,
            long val,
            string reason,
            DateTime createtime
        )
        {
            recordsAffecteds.Clear();
            sb.Clear();
            sb.Append(@"
update [account]
   set [coin] = [coin] + ");
            sb.Append(val);
            sb.Append(@"
 where [id] = ");
            sb.Append(id);
            sb.Append(@" and [coin] = ");
            sb.Append(original);
            sb.Append(@";
if @@ROWCOUNT = 0
    select [coin]
      from [account]
     where [id] = ");
            sb.Append(id);
            sb.Append(@";
else begin
    insert into [coin_changelog] 
           ( [account_id], [original], [value], [current], [reason], [createtime] )
    values ( ");
            sb.Append(id);
            sb.Append(@", ");
            sb.Append(original);
            sb.Append(@", ");
            sb.Append(val);
            sb.Append(@", ");
            sb.Append(original);
            sb.Append(@" + ");
            sb.Append(val);
            sb.Append(@", ");
            sb.MsSqlAppend(reason);
            sb.Append(@", ");
            sb.MsSqlAppend(createtime);
            sb.Append(@"  );
    select cast( (");
            sb.Append(original);
            sb.Append(@" + ");
            sb.Append(val);
            sb.Append(@") as bigint );
end");
            cmd.CommandText = sb.ToString();

            using (var r = cmd.ExecuteReader())
            {
                recordsAffecteds.Add(r.RecordsAffected);
                if (!r.Read())
                {
                    return default(long?);
                }
                return r.GetInt64(0);
            }
        }
    }
}
