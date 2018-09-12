using System;
using xx;

namespace MYSQLGEN
{
    public static class MySqlAppendExt
    {
        #region MySqlAppends
        #region Orginal
        public static void MySqlAppend<T>(this System.Text.StringBuilder sb, List<T> os, bool ignoreReadOnly = false) { if (os == null || os.dataLen == 0) throw new ArgumentNullException(); for (int i = 0; i < os.dataLen; ++i) { sb.MySqlAppend(os[i], ignoreReadOnly); sb.Append(", "); }; sb.Length -= 2; }

        public static void MySqlAppend<T>(this System.Text.StringBuilder sb, T v, bool ignoreReadOnly = false) { sb.Append(v); }
        public static void MySqlAppend<T>(this System.Text.StringBuilder sb, T? o, bool ignoreReadOnly = false) where T : struct { if (o.HasValue) { sb.MySqlAppend(o.Value); } else { sb.Append("null"); } }
        public static void MySqlAppend(this System.Text.StringBuilder sb, string v, bool ignoreReadOnly = false) { sb.Append("'" + v.Replace("'", "''") + "'"); }
        public static void MySqlAppend(this System.Text.StringBuilder sb, DateTime o, bool ignoreReadOnly = false) { sb.Append("'" + o.ToString("yyyy-MM-dd HH:mm:ss") + "'"); }
        public static void MySqlAppend(this System.Text.StringBuilder sb, List<DateTime> os, bool ignoreReadOnly = false) { if (os == null || os.dataLen == 0) throw new ArgumentNullException(); sb.Append("( "); for (int i = 0; i < os.dataLen; ++i) { sb.MySqlAppend(os[i]); sb.Append(", "); }; sb.Length -= 2; sb.Append(" )"); }
        public static void MySqlAppend(this System.Text.StringBuilder sb, List<byte> os, bool ignoreReadOnly = false) { if (os == null || os.dataLen == 0) throw new ArgumentNullException(); sb.Append("( "); for (int i = 0; i < os.dataLen; ++i) { sb.Append(os[i]); sb.Append(", "); }; sb.Length -= 2; sb.Append(" )"); }
        public static void MySqlAppend(this System.Text.StringBuilder sb, List<ushort> os, bool ignoreReadOnly = false) { if (os == null || os.dataLen == 0) throw new ArgumentNullException(); sb.Append("( "); for (int i = 0; i < os.dataLen; ++i) { sb.Append(os[i]); sb.Append(", "); }; sb.Length -= 2; sb.Append(" )"); }
        public static void MySqlAppend(this System.Text.StringBuilder sb, List<uint> os, bool ignoreReadOnly = false) { if (os == null || os.dataLen == 0) throw new ArgumentNullException(); sb.Append("( "); for (int i = 0; i < os.dataLen; ++i) { sb.Append(os[i]); sb.Append(", "); }; sb.Length -= 2; sb.Append(" )"); }
        public static void MySqlAppend(this System.Text.StringBuilder sb, List<ulong> os, bool ignoreReadOnly = false) { if (os == null || os.dataLen == 0) throw new ArgumentNullException(); sb.Append("( "); for (int i = 0; i < os.dataLen; ++i) { sb.Append(os[i]); sb.Append(", "); }; sb.Length -= 2; sb.Append(" )"); }
        public static void MySqlAppend(this System.Text.StringBuilder sb, List<sbyte> os, bool ignoreReadOnly = false) { if (os == null || os.dataLen == 0) throw new ArgumentNullException(); sb.Append("( "); for (int i = 0; i < os.dataLen; ++i) { sb.Append(os[i]); sb.Append(", "); }; sb.Length -= 2; sb.Append(" )"); }
        public static void MySqlAppend(this System.Text.StringBuilder sb, List<short> os, bool ignoreReadOnly = false) { if (os == null || os.dataLen == 0) throw new ArgumentNullException(); sb.Append("( "); for (int i = 0; i < os.dataLen; ++i) { sb.Append(os[i]); sb.Append(", "); }; sb.Length -= 2; sb.Append(" )"); }
        public static void MySqlAppend(this System.Text.StringBuilder sb, List<int> os, bool ignoreReadOnly = false) { if (os == null || os.dataLen == 0) throw new ArgumentNullException(); sb.Append("( "); for (int i = 0; i < os.dataLen; ++i) { sb.Append(os[i]); sb.Append(", "); }; sb.Length -= 2; sb.Append(" )"); }
        public static void MySqlAppend(this System.Text.StringBuilder sb, List<long> os, bool ignoreReadOnly = false) { if (os == null || os.dataLen == 0) throw new ArgumentNullException(); sb.Append("( "); for (int i = 0; i < os.dataLen; ++i) { sb.Append(os[i]); sb.Append(", "); }; sb.Length -= 2; sb.Append(" )"); }
        public static void MySqlAppend(this System.Text.StringBuilder sb, List<double> os, bool ignoreReadOnly = false) { if (os == null || os.dataLen == 0) throw new ArgumentNullException(); sb.Append("( "); for (int i = 0; i < os.dataLen; ++i) { sb.Append(os[i]); sb.Append(", "); }; sb.Length -= 2; sb.Append(" )"); }
        public static void MySqlAppend(this System.Text.StringBuilder sb, List<float> os, bool ignoreReadOnly = false) { if (os == null || os.dataLen == 0) throw new ArgumentNullException(); sb.Append("( "); for (int i = 0; i < os.dataLen; ++i) { sb.Append(os[i]); sb.Append(", "); }; sb.Length -= 2; sb.Append(" )"); }
        public static void MySqlAppend(this System.Text.StringBuilder sb, List<bool> os, bool ignoreReadOnly = false) { if (os == null || os.dataLen == 0) throw new ArgumentNullException(); sb.Append("( "); for (int i = 0; i < os.dataLen; ++i) { sb.Append(os[i]); sb.Append(", "); }; sb.Length -= 2; sb.Append(" )"); }
        #endregion

        public static void MySqlAppend(this System.Text.StringBuilder sb, Tables.TABLES o, bool ignoreReadOnly = false)
        {
            sb.Append("(");
            sb.MySqlAppend(o.TABLE_CATALOG);
            sb.Append(", ");
            sb.MySqlAppend(o.TABLE_SCHEMA);
            sb.Append(", ");
            sb.MySqlAppend(o.TABLE_NAME);
            sb.Append(", ");
            sb.MySqlAppend(o.TABLE_TYPE);
            sb.Append(", ");
            sb.MySqlAppend(o.ENGINE);
            sb.Append(", ");
            sb.MySqlAppend(o.VERSION);
            sb.Append(", ");
            sb.MySqlAppend(o.ROW_FORMAT);
            sb.Append(", ");
            sb.MySqlAppend(o.TABLE_ROWS);
            sb.Append(", ");
            sb.MySqlAppend(o.AVG_ROW_LENGTH);
            sb.Append(", ");
            sb.MySqlAppend(o.DATA_LENGTH);
            sb.Append(", ");
            sb.MySqlAppend(o.MAX_DATA_LENGTH);
            sb.Append(", ");
            sb.MySqlAppend(o.INDEX_LENGTH);
            sb.Append(", ");
            sb.MySqlAppend(o.DATA_FREE);
            sb.Append(", ");
            sb.MySqlAppend(o.AUTO_INCREMENT);
            sb.Append(", ");
            sb.MySqlAppend(o.CREATE_TIME);
            sb.Append(", ");
            sb.MySqlAppend(o.UPDATE_TIME);
            sb.Append(", ");
            sb.MySqlAppend(o.CHECK_TIME);
            sb.Append(", ");
            sb.MySqlAppend(o.TABLE_COLLATION);
            sb.Append(", ");
            sb.MySqlAppend(o.CHECKSUM);
            sb.Append(", ");
            sb.MySqlAppend(o.CREATE_OPTIONS);
            sb.Append(", ");
            sb.MySqlAppend(o.TABLE_COMMENT);
            sb.Append(", ");
            sb.Length -= 2;
            sb.Append(")");
        }
        public static void MySqlAppend(this System.Text.StringBuilder sb, Tables.COLUMNS o, bool ignoreReadOnly = false)
        {
            sb.Append("(");
            sb.MySqlAppend(o.TABLE_CATALOG);
            sb.Append(", ");
            sb.MySqlAppend(o.TABLE_SCHEMA);
            sb.Append(", ");
            sb.MySqlAppend(o.TABLE_NAME);
            sb.Append(", ");
            sb.MySqlAppend(o.COLUMN_NAME);
            sb.Append(", ");
            sb.Append(o.ORDINAL_POSITION);
            sb.Append(", ");
            sb.MySqlAppend(o.COLUMN_DEFAULT);
            sb.Append(", ");
            sb.MySqlAppend(o.IS_NULLABLE);
            sb.Append(", ");
            sb.MySqlAppend(o.DATA_TYPE);
            sb.Append(", ");
            sb.MySqlAppend(o.CHARACTER_MAXIMUM_LENGTH);
            sb.Append(", ");
            sb.MySqlAppend(o.CHARACTER_OCTET_LENGTH);
            sb.Append(", ");
            sb.MySqlAppend(o.NUMERIC_PRECISION);
            sb.Append(", ");
            sb.MySqlAppend(o.NUMERIC_SCALE);
            sb.Append(", ");
            sb.MySqlAppend(o.DATETIME_PRECISION);
            sb.Append(", ");
            sb.MySqlAppend(o.CHARACTER_SET_NAME);
            sb.Append(", ");
            sb.MySqlAppend(o.COLLATION_NAME);
            sb.Append(", ");
            sb.MySqlAppend(o.COLUMN_TYPE);
            sb.Append(", ");
            sb.MySqlAppend(o.COLUMN_KEY);
            sb.Append(", ");
            sb.MySqlAppend(o.EXTRA);
            sb.Append(", ");
            sb.MySqlAppend(o.PRIVILEGES);
            sb.Append(", ");
            sb.MySqlAppend(o.COLUMN_COMMENT);
            sb.Append(", ");
            sb.MySqlAppend(o.GENERATION_EXPRESSION);
            sb.Append(", ");
            sb.Length -= 2;
            sb.Append(")");
        }
        public static void MySqlAppend(this System.Text.StringBuilder sb, Tables.show_create_table o, bool ignoreReadOnly = false)
        {
            sb.Append("(");
            sb.MySqlAppend(o.Table);
            sb.Append(", ");
            sb.MySqlAppend(o.CreateTable);
            sb.Append(", ");
            sb.Length -= 2;
            sb.Append(")");
        }
        #endregion
    }

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
        /// 查出库中有哪些表
        /// </summary>
        public List<T> GetTables<T>
        (
            string dbDatabase
            , MySql.Data.MySqlClient.MySqlTransaction tran_ = null) where T : Tables.TABLES, new()
        {
            cmd.Transaction = tran_;
            recordsAffecteds.Clear();
            sb.Clear();
            sb.Append(@"
select * from information_schema.TABLES
 where TABLE_SCHEMA = ");
            sb.MySqlAppend(dbDatabase);
            sb.Append(@";
");
            cmd.CommandText = sb.ToString();

            var rtv = new List<T>();
            using (var r = cmd.ExecuteReader())
            {
                recordsAffecteds.Add(r.RecordsAffected);
                while (r.Read())
                {
                    rtv.Add(new T
                    {
                        TABLE_CATALOG = r.IsDBNull(0) ? null : (string)r.GetString(0),
                        TABLE_SCHEMA = r.IsDBNull(1) ? null : (string)r.GetString(1),
                        TABLE_NAME = r.IsDBNull(2) ? null : (string)r.GetString(2),
                        TABLE_TYPE = r.IsDBNull(3) ? null : (string)r.GetString(3),
                        ENGINE = r.IsDBNull(4) ? null : (string)r.GetString(4),
                        VERSION = r.IsDBNull(5) ? null : (long?)r.GetInt64(5),
                        ROW_FORMAT = r.IsDBNull(6) ? null : (string)r.GetString(6),
                        TABLE_ROWS = r.IsDBNull(7) ? null : (long?)r.GetInt64(7),
                        AVG_ROW_LENGTH = r.IsDBNull(8) ? null : (long?)r.GetInt64(8),
                        DATA_LENGTH = r.IsDBNull(9) ? null : (long?)r.GetInt64(9),
                        MAX_DATA_LENGTH = r.IsDBNull(10) ? null : (long?)r.GetInt64(10),
                        INDEX_LENGTH = r.IsDBNull(11) ? null : (long?)r.GetInt64(11),
                        DATA_FREE = r.IsDBNull(12) ? null : (long?)r.GetInt64(12),
                        AUTO_INCREMENT = r.IsDBNull(13) ? null : (long?)r.GetInt64(13),
                        CREATE_TIME = r.IsDBNull(14) ? null : (string)r.GetString(14),
                        UPDATE_TIME = r.IsDBNull(15) ? null : (string)r.GetString(15),
                        CHECK_TIME = r.IsDBNull(16) ? null : (string)r.GetString(16),
                        TABLE_COLLATION = r.IsDBNull(17) ? null : (string)r.GetString(17),
                        CHECKSUM = r.IsDBNull(18) ? null : (long?)r.GetInt64(18),
                        CREATE_OPTIONS = r.IsDBNull(19) ? null : (string)r.GetString(19),
                        TABLE_COMMENT = r.IsDBNull(20) ? null : (string)r.GetString(20)
                    });
                }
            }
            return rtv;
        }

        /// <summary>
        /// 查出表的字段列表
        /// </summary>
        public List<T> GetColumns<T>
        (
            string dbDatabase,
            string tableName
            , MySql.Data.MySqlClient.MySqlTransaction tran_ = null) where T : Tables.COLUMNS, new()
        {
            cmd.Transaction = tran_;
            recordsAffecteds.Clear();
            sb.Clear();
            sb.Append(@"
select * from information_schema.COLUMNS
 where TABLE_SCHEMA = ");
            sb.MySqlAppend(dbDatabase);
            sb.Append(@"
   and TABLE_NAME = ");
            sb.MySqlAppend(tableName);
            sb.Append(@";
");
            cmd.CommandText = sb.ToString();

            var rtv = new List<T>();
            using (var r = cmd.ExecuteReader())
            {
                recordsAffecteds.Add(r.RecordsAffected);
                while (r.Read())
                {
                    rtv.Add(new T
                    {
                        TABLE_CATALOG = r.IsDBNull(0) ? null : (string)r.GetString(0),
                        TABLE_SCHEMA = r.IsDBNull(1) ? null : (string)r.GetString(1),
                        TABLE_NAME = r.IsDBNull(2) ? null : (string)r.GetString(2),
                        COLUMN_NAME = r.IsDBNull(3) ? null : (string)r.GetString(3),
                        ORDINAL_POSITION = r.GetInt64(4),
                        COLUMN_DEFAULT = r.IsDBNull(5) ? null : (string)r.GetString(5),
                        IS_NULLABLE = r.IsDBNull(6) ? null : (string)r.GetString(6),
                        DATA_TYPE = r.IsDBNull(7) ? null : (string)r.GetString(7),
                        CHARACTER_MAXIMUM_LENGTH = r.IsDBNull(8) ? null : (long?)r.GetInt64(8),
                        CHARACTER_OCTET_LENGTH = r.IsDBNull(9) ? null : (long?)r.GetInt64(9),
                        NUMERIC_PRECISION = r.IsDBNull(10) ? null : (long?)r.GetInt64(10),
                        NUMERIC_SCALE = r.IsDBNull(11) ? null : (long?)r.GetInt64(11),
                        DATETIME_PRECISION = r.IsDBNull(12) ? null : (long?)r.GetInt64(12),
                        CHARACTER_SET_NAME = r.IsDBNull(13) ? null : (string)r.GetString(13),
                        COLLATION_NAME = r.IsDBNull(14) ? null : (string)r.GetString(14),
                        COLUMN_TYPE = r.IsDBNull(15) ? null : (string)r.GetString(15),
                        COLUMN_KEY = r.IsDBNull(16) ? null : (string)r.GetString(16),
                        EXTRA = r.IsDBNull(17) ? null : (string)r.GetString(17),
                        PRIVILEGES = r.IsDBNull(18) ? null : (string)r.GetString(18),
                        COLUMN_COMMENT = r.IsDBNull(19) ? null : (string)r.GetString(19),
                        GENERATION_EXPRESSION = r.IsDBNull(20) ? null : (string)r.GetString(20)
                    });
                }
            }
            return rtv;
        }

        /// <summary>
        /// 查出表建表脚本
        /// </summary>
        public T GetCreateScript<T>
        (
            string dbDatabase,
            string tableName
            , MySql.Data.MySqlClient.MySqlTransaction tran_ = null) where T : Tables.show_create_table, new()
        {
            cmd.Transaction = tran_;
            recordsAffecteds.Clear();
            sb.Clear();
            sb.Append(@"
SHOW CREATE TABLE `");
            sb.Append(dbDatabase);
            sb.Append(@"`.`");
            sb.Append(tableName);
            sb.Append(@"`;
");
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
                    Table = r.IsDBNull(0) ? null : (string)r.GetString(0), 
                    CreateTable = r.IsDBNull(1) ? null : (string)r.GetString(1)
                };
            }
        }
    }
}
