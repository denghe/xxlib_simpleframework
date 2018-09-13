using System;
using xx;

namespace PKG
{
    public class MySqlFuncs
    {
        #region conn, cmd, sb, recordsAffecteds
        public MySql.Data.MySqlClient.MySqlConnection conn;
        public MySql.Data.MySqlClient.MySqlCommand cmd;
        public System.Text.StringBuilder sb = new System.Text.StringBuilder();
        private List<int> recordsAffecteds = new List<int>();

        public List<int> RecordsAffecteds
        {
            get { return recordsAffecteds; }
        }

        public int RecordsAffected
        {
            get { return recordsAffecteds[0]; }
        }

        public MySqlFuncs(MySql.Data.MySqlClient.MySqlConnection conn)
        {
            this.conn = conn;
            this.cmd = conn.CreateCommand();
        }
        #endregion


        /// <summary>
        /// 模拟从表查数据填充到 Foo. 顺序和数量一致应该与 Foo 中标有 Column 的类成员一致
        /// </summary>
        public T SelectFoo<T>
        (
            int id,
            int? age,
            string info
            , MySql.Data.MySqlClient.MySqlTransaction tran_ = null) where T : Foo, new()
        {
            cmd.Transaction = tran_;
            recordsAffecteds.Clear();
            sb.Clear();
            sb.Append(@"select ");
            sb.Append(id);
            sb.Append(@", ");
            sb.Append(age);
            sb.Append(@", ");
            sb.Append(info == null ? "null" : ("'" + info.Replace("'", "''") + "'"));
            cmd.CommandText = sb.ToString();

            using (var r = cmd.ExecuteReader())
            {
                recordsAffecteds.Add(r.RecordsAffected);
                if (!r.Read())
                {
                    return null;
                }
                return new T
                {
                    id = r.GetInt32(0), 
                    age = r.IsDBNull(1) ? null : (int?)r.GetInt32(1), 
                    info = r.IsDBNull(2) ? null : (string)r.GetString(2)
                };
            }
        }

        public Tuple <T0, int, List<T2>, int?> SelectFooFoo<T0, T2>
        (
            int id,
            int? age,
            string info
            , MySql.Data.MySqlClient.MySqlTransaction tran_ = null) where T0 : Foo, new() where T2 : Foo, new()
        {
            cmd.Transaction = tran_;
            recordsAffecteds.Clear();
            sb.Clear();
            sb.Append(@"
select ");
            sb.Append(id);
            sb.Append(@", ");
            sb.Append(age);
            sb.Append(@", ");
            sb.Append(info == null ? "null" : ("'" + info.Replace("'", "''") + "'"));
            sb.Append(@";
select ");
            sb.Append(id);
            sb.Append(@";
select ");
            sb.Append(id);
            sb.Append(@", ");
            sb.Append(age);
            sb.Append(@", ");
            sb.Append(info == null ? "null" : ("'" + info.Replace("'", "''") + "'"));
            sb.Append(@";
select ");
            sb.Append(age);
            sb.Append(@";
");
            cmd.CommandText = sb.ToString();

            using (var r = cmd.ExecuteReader())
            {
                recordsAffecteds.Add(r.RecordsAffected);
                recordsAffecteds.Add(r.RecordsAffected);
                T0 rtv1 = null;
                if (r.Read())
                {
                    rtv1 = new T0
                    {
                    id = r.GetInt32(0), 
                    age = r.IsDBNull(1) ? null : (int?)r.GetInt32(1), 
                    info = r.IsDBNull(2) ? null : (string)r.GetString(2)
                    };
                }
                r.NextResult();
                recordsAffecteds.Add(r.RecordsAffected);
                var rtv2 = default(int);
                if (r.Read())
                {
                    rtv2 = r.GetInt32(0);
                }
                r.NextResult();
                recordsAffecteds.Add(r.RecordsAffected);
                var rtv3 = new List<T2>();
                while (r.Read())
                {
                    rtv3.Add(new T2
                    {
                        id = r.GetInt32(0),
                        age = r.IsDBNull(1) ? null : (int?)r.GetInt32(1),
                        info = r.IsDBNull(2) ? null : (string)r.GetString(2)
                    });
                }
                r.NextResult();
                recordsAffecteds.Add(r.RecordsAffected);
                var rtv4 = default(int?);
                if (r.Read())
                {
                    rtv4 = r.IsDBNull(0) ? null : (int?)r.GetInt32(0);
                }
                return new Tuple <T0, int, List<T2>, int?>(rtv1, rtv2, rtv3, rtv4);
            }
        }

        public List<T> SelectNodes<T>(MySql.Data.MySqlClient.MySqlTransaction tran_ = null) where T : Tables.node, new()
        {
            cmd.Transaction = tran_;
            cmd.CommandText = @"
select 1, null
union all
select 2, 1
union all
select 3, 1
union all
select 4, 2;
";
            var rtv = new List<T>();
            using (var r = cmd.ExecuteReader())
            {
                recordsAffecteds.Add(r.RecordsAffected);
                while (r.Read())
                {
                    rtv.Add(new T
                    {
                        id = r.GetInt32(0),
                        pid = r.IsDBNull(1) ? null : (int?)r.GetInt32(1)
                    });
                }
            }
            return rtv;
        }
    }
}
