using System;
using xx;

namespace MYSQLGEN
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
            sb.Append(dbDatabase == null ? "null" : ("'" + dbDatabase.Replace("'", "''") + "'"));
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
            sb.Append(dbDatabase == null ? "null" : ("'" + dbDatabase.Replace("'", "''") + "'"));
            sb.Append(@"
   and TABLE_NAME = ");
            sb.Append(tableName == null ? "null" : ("'" + tableName.Replace("'", "''") + "'"));
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
