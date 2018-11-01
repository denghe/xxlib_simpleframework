using System;
namespace MYSQLGEN
{
    public static class PkgGenMd5
    {
        public const string value = "bd227b96adcf623609c7b4f49ded85fd"; 
    }

    public partial class DbTable : Tables.TABLES
    {
        public xx.List<DbColumn> childs;
        public string createScript;

        public override ushort GetPackageId()
        {
            return xx.TypeId<DbTable>.value;
        }

        public override void ToBBuffer(xx.BBuffer bb)
        {
            base.ToBBuffer(bb);
            bb.Write(this.childs);
            bb.Write(this.createScript);
        }

        public override void FromBBuffer(xx.BBuffer bb)
        {
            base.FromBBuffer(bb);
            bb.readLengthLimit = 0;
            bb.Read(ref this.childs);
            bb.readLengthLimit = 0;
            bb.Read(ref this.createScript);
        }
        public override void ToString(System.Text.StringBuilder s)
        {
            if (__toStringing)
            {
        	    s.Append("[ \"***** recursived *****\" ]");
        	    return;
            }
            else __toStringing = true;

            s.Append("{ \"pkgTypeName\":\"DbTable\", \"pkgTypeId\":" + GetPackageId());
            ToStringCore(s);
            s.Append(" }");

            __toStringing = false;
        }
        public override void ToStringCore(System.Text.StringBuilder s)
        {
            base.ToStringCore(s);
            s.Append(", \"childs\":" + (childs == null ? "nil" : childs.ToString()));
            if (createScript != null) s.Append(", \"createScript\":\"" + createScript.ToString() + "\"");
            else s.Append(", \"createScript\":nil");
        }
        public override string ToString()
        {
            var sb = new System.Text.StringBuilder();
            ToString(sb);
            return sb.ToString();
        }
        public override void MySqlAppend(System.Text.StringBuilder sb, bool ignoreReadOnly)
        {
            base.MySqlAppend(sb, ignoreReadOnly);
        }
    }
    public partial class DbColumn : Tables.COLUMNS
    {
        public DbTable parent;

        public override ushort GetPackageId()
        {
            return xx.TypeId<DbColumn>.value;
        }

        public override void ToBBuffer(xx.BBuffer bb)
        {
            base.ToBBuffer(bb);
            bb.Write(this.parent);
        }

        public override void FromBBuffer(xx.BBuffer bb)
        {
            base.FromBBuffer(bb);
            bb.Read(ref this.parent);
        }
        public override void ToString(System.Text.StringBuilder s)
        {
            if (__toStringing)
            {
        	    s.Append("[ \"***** recursived *****\" ]");
        	    return;
            }
            else __toStringing = true;

            s.Append("{ \"pkgTypeName\":\"DbColumn\", \"pkgTypeId\":" + GetPackageId());
            ToStringCore(s);
            s.Append(" }");

            __toStringing = false;
        }
        public override void ToStringCore(System.Text.StringBuilder s)
        {
            base.ToStringCore(s);
            s.Append(", \"parent\":" + (parent == null ? "nil" : parent.ToString()));
        }
        public override string ToString()
        {
            var sb = new System.Text.StringBuilder();
            ToString(sb);
            return sb.ToString();
        }
        public override void MySqlAppend(System.Text.StringBuilder sb, bool ignoreReadOnly)
        {
            base.MySqlAppend(sb, ignoreReadOnly);
        }
    }
namespace Tables
{
    public partial class TABLES : xx.Object
    {
        public string TABLE_CATALOG;
        public string TABLE_SCHEMA;
        public string TABLE_NAME;
        public string TABLE_TYPE;
        public string ENGINE;
        public long? VERSION;
        public string ROW_FORMAT;
        public long? TABLE_ROWS;
        public long? AVG_ROW_LENGTH;
        public long? DATA_LENGTH;
        public long? MAX_DATA_LENGTH;
        public long? INDEX_LENGTH;
        public long? DATA_FREE;
        public long? AUTO_INCREMENT;
        public string CREATE_TIME;
        public string UPDATE_TIME;
        public string CHECK_TIME;
        public string TABLE_COLLATION;
        public long? CHECKSUM;
        public string CREATE_OPTIONS;
        public string TABLE_COMMENT;

        public override ushort GetPackageId()
        {
            return xx.TypeId<TABLES>.value;
        }

        public override void ToBBuffer(xx.BBuffer bb)
        {
            bb.Write(this.TABLE_CATALOG);
            bb.Write(this.TABLE_SCHEMA);
            bb.Write(this.TABLE_NAME);
            bb.Write(this.TABLE_TYPE);
            bb.Write(this.ENGINE);
            bb.Write(this.VERSION);
            bb.Write(this.ROW_FORMAT);
            bb.Write(this.TABLE_ROWS);
            bb.Write(this.AVG_ROW_LENGTH);
            bb.Write(this.DATA_LENGTH);
            bb.Write(this.MAX_DATA_LENGTH);
            bb.Write(this.INDEX_LENGTH);
            bb.Write(this.DATA_FREE);
            bb.Write(this.AUTO_INCREMENT);
            bb.Write(this.CREATE_TIME);
            bb.Write(this.UPDATE_TIME);
            bb.Write(this.CHECK_TIME);
            bb.Write(this.TABLE_COLLATION);
            bb.Write(this.CHECKSUM);
            bb.Write(this.CREATE_OPTIONS);
            bb.Write(this.TABLE_COMMENT);
        }

        public override void FromBBuffer(xx.BBuffer bb)
        {
            bb.readLengthLimit = 0;
            bb.Read(ref this.TABLE_CATALOG);
            bb.readLengthLimit = 0;
            bb.Read(ref this.TABLE_SCHEMA);
            bb.readLengthLimit = 0;
            bb.Read(ref this.TABLE_NAME);
            bb.readLengthLimit = 0;
            bb.Read(ref this.TABLE_TYPE);
            bb.readLengthLimit = 0;
            bb.Read(ref this.ENGINE);
            bb.Read(ref this.VERSION);
            bb.readLengthLimit = 0;
            bb.Read(ref this.ROW_FORMAT);
            bb.Read(ref this.TABLE_ROWS);
            bb.Read(ref this.AVG_ROW_LENGTH);
            bb.Read(ref this.DATA_LENGTH);
            bb.Read(ref this.MAX_DATA_LENGTH);
            bb.Read(ref this.INDEX_LENGTH);
            bb.Read(ref this.DATA_FREE);
            bb.Read(ref this.AUTO_INCREMENT);
            bb.readLengthLimit = 0;
            bb.Read(ref this.CREATE_TIME);
            bb.readLengthLimit = 0;
            bb.Read(ref this.UPDATE_TIME);
            bb.readLengthLimit = 0;
            bb.Read(ref this.CHECK_TIME);
            bb.readLengthLimit = 0;
            bb.Read(ref this.TABLE_COLLATION);
            bb.Read(ref this.CHECKSUM);
            bb.readLengthLimit = 0;
            bb.Read(ref this.CREATE_OPTIONS);
            bb.readLengthLimit = 0;
            bb.Read(ref this.TABLE_COMMENT);
        }
        public override void ToString(System.Text.StringBuilder s)
        {
            if (__toStringing)
            {
        	    s.Append("[ \"***** recursived *****\" ]");
        	    return;
            }
            else __toStringing = true;

            s.Append("{ \"pkgTypeName\":\"Tables.TABLES\", \"pkgTypeId\":" + GetPackageId());
            ToStringCore(s);
            s.Append(" }");

            __toStringing = false;
        }
        public override void ToStringCore(System.Text.StringBuilder s)
        {
            if (TABLE_CATALOG != null) s.Append(", \"TABLE_CATALOG\":\"" + TABLE_CATALOG.ToString() + "\"");
            else s.Append(", \"TABLE_CATALOG\":nil");
            if (TABLE_SCHEMA != null) s.Append(", \"TABLE_SCHEMA\":\"" + TABLE_SCHEMA.ToString() + "\"");
            else s.Append(", \"TABLE_SCHEMA\":nil");
            if (TABLE_NAME != null) s.Append(", \"TABLE_NAME\":\"" + TABLE_NAME.ToString() + "\"");
            else s.Append(", \"TABLE_NAME\":nil");
            if (TABLE_TYPE != null) s.Append(", \"TABLE_TYPE\":\"" + TABLE_TYPE.ToString() + "\"");
            else s.Append(", \"TABLE_TYPE\":nil");
            if (ENGINE != null) s.Append(", \"ENGINE\":\"" + ENGINE.ToString() + "\"");
            else s.Append(", \"ENGINE\":nil");
            s.Append(", \"VERSION\":" + (VERSION.HasValue ? VERSION.Value.ToString() : "nil"));
            if (ROW_FORMAT != null) s.Append(", \"ROW_FORMAT\":\"" + ROW_FORMAT.ToString() + "\"");
            else s.Append(", \"ROW_FORMAT\":nil");
            s.Append(", \"TABLE_ROWS\":" + (TABLE_ROWS.HasValue ? TABLE_ROWS.Value.ToString() : "nil"));
            s.Append(", \"AVG_ROW_LENGTH\":" + (AVG_ROW_LENGTH.HasValue ? AVG_ROW_LENGTH.Value.ToString() : "nil"));
            s.Append(", \"DATA_LENGTH\":" + (DATA_LENGTH.HasValue ? DATA_LENGTH.Value.ToString() : "nil"));
            s.Append(", \"MAX_DATA_LENGTH\":" + (MAX_DATA_LENGTH.HasValue ? MAX_DATA_LENGTH.Value.ToString() : "nil"));
            s.Append(", \"INDEX_LENGTH\":" + (INDEX_LENGTH.HasValue ? INDEX_LENGTH.Value.ToString() : "nil"));
            s.Append(", \"DATA_FREE\":" + (DATA_FREE.HasValue ? DATA_FREE.Value.ToString() : "nil"));
            s.Append(", \"AUTO_INCREMENT\":" + (AUTO_INCREMENT.HasValue ? AUTO_INCREMENT.Value.ToString() : "nil"));
            if (CREATE_TIME != null) s.Append(", \"CREATE_TIME\":\"" + CREATE_TIME.ToString() + "\"");
            else s.Append(", \"CREATE_TIME\":nil");
            if (UPDATE_TIME != null) s.Append(", \"UPDATE_TIME\":\"" + UPDATE_TIME.ToString() + "\"");
            else s.Append(", \"UPDATE_TIME\":nil");
            if (CHECK_TIME != null) s.Append(", \"CHECK_TIME\":\"" + CHECK_TIME.ToString() + "\"");
            else s.Append(", \"CHECK_TIME\":nil");
            if (TABLE_COLLATION != null) s.Append(", \"TABLE_COLLATION\":\"" + TABLE_COLLATION.ToString() + "\"");
            else s.Append(", \"TABLE_COLLATION\":nil");
            s.Append(", \"CHECKSUM\":" + (CHECKSUM.HasValue ? CHECKSUM.Value.ToString() : "nil"));
            if (CREATE_OPTIONS != null) s.Append(", \"CREATE_OPTIONS\":\"" + CREATE_OPTIONS.ToString() + "\"");
            else s.Append(", \"CREATE_OPTIONS\":nil");
            if (TABLE_COMMENT != null) s.Append(", \"TABLE_COMMENT\":\"" + TABLE_COMMENT.ToString() + "\"");
            else s.Append(", \"TABLE_COMMENT\":nil");
        }
        public override string ToString()
        {
            var sb = new System.Text.StringBuilder();
            ToString(sb);
            return sb.ToString();
        }
        public override void MySqlAppend(System.Text.StringBuilder sb, bool ignoreReadOnly)
        {
            sb.Append("(");
            sb.Append(this.TABLE_CATALOG == null ? "null" : ("'" + this.TABLE_CATALOG.Replace("'", "''") + "'"));
            sb.Append(", ");
            sb.Append(this.TABLE_SCHEMA == null ? "null" : ("'" + this.TABLE_SCHEMA.Replace("'", "''") + "'"));
            sb.Append(", ");
            sb.Append(this.TABLE_NAME == null ? "null" : ("'" + this.TABLE_NAME.Replace("'", "''") + "'"));
            sb.Append(", ");
            sb.Append(this.TABLE_TYPE == null ? "null" : ("'" + this.TABLE_TYPE.Replace("'", "''") + "'"));
            sb.Append(", ");
            sb.Append(this.ENGINE == null ? "null" : ("'" + this.ENGINE.Replace("'", "''") + "'"));
            sb.Append(", ");
            sb.Append(this.VERSION.HasValue ? this.VERSION.Value.ToString() : "null");
            sb.Append(", ");
            sb.Append(this.ROW_FORMAT == null ? "null" : ("'" + this.ROW_FORMAT.Replace("'", "''") + "'"));
            sb.Append(", ");
            sb.Append(this.TABLE_ROWS.HasValue ? this.TABLE_ROWS.Value.ToString() : "null");
            sb.Append(", ");
            sb.Append(this.AVG_ROW_LENGTH.HasValue ? this.AVG_ROW_LENGTH.Value.ToString() : "null");
            sb.Append(", ");
            sb.Append(this.DATA_LENGTH.HasValue ? this.DATA_LENGTH.Value.ToString() : "null");
            sb.Append(", ");
            sb.Append(this.MAX_DATA_LENGTH.HasValue ? this.MAX_DATA_LENGTH.Value.ToString() : "null");
            sb.Append(", ");
            sb.Append(this.INDEX_LENGTH.HasValue ? this.INDEX_LENGTH.Value.ToString() : "null");
            sb.Append(", ");
            sb.Append(this.DATA_FREE.HasValue ? this.DATA_FREE.Value.ToString() : "null");
            sb.Append(", ");
            sb.Append(this.AUTO_INCREMENT.HasValue ? this.AUTO_INCREMENT.Value.ToString() : "null");
            sb.Append(", ");
            sb.Append(this.CREATE_TIME == null ? "null" : ("'" + this.CREATE_TIME.Replace("'", "''") + "'"));
            sb.Append(", ");
            sb.Append(this.UPDATE_TIME == null ? "null" : ("'" + this.UPDATE_TIME.Replace("'", "''") + "'"));
            sb.Append(", ");
            sb.Append(this.CHECK_TIME == null ? "null" : ("'" + this.CHECK_TIME.Replace("'", "''") + "'"));
            sb.Append(", ");
            sb.Append(this.TABLE_COLLATION == null ? "null" : ("'" + this.TABLE_COLLATION.Replace("'", "''") + "'"));
            sb.Append(", ");
            sb.Append(this.CHECKSUM.HasValue ? this.CHECKSUM.Value.ToString() : "null");
            sb.Append(", ");
            sb.Append(this.CREATE_OPTIONS == null ? "null" : ("'" + this.CREATE_OPTIONS.Replace("'", "''") + "'"));
            sb.Append(", ");
            sb.Append(this.TABLE_COMMENT == null ? "null" : ("'" + this.TABLE_COMMENT.Replace("'", "''") + "'"));
            sb.Append(", ");
            sb.Length -= 2;
            sb.Append(")");

        }
    }
    public partial class COLUMNS : xx.Object
    {
        public string TABLE_CATALOG;
        public string TABLE_SCHEMA;
        public string TABLE_NAME;
        public string COLUMN_NAME;
        public long ORDINAL_POSITION;
        public string COLUMN_DEFAULT;
        public string IS_NULLABLE;
        public string DATA_TYPE;
        public long? CHARACTER_MAXIMUM_LENGTH;
        public long? CHARACTER_OCTET_LENGTH;
        public long? NUMERIC_PRECISION;
        public long? NUMERIC_SCALE;
        public long? DATETIME_PRECISION;
        public string CHARACTER_SET_NAME;
        public string COLLATION_NAME;
        public string COLUMN_TYPE;
        public string COLUMN_KEY;
        public string EXTRA;
        public string PRIVILEGES;
        public string COLUMN_COMMENT;
        public string GENERATION_EXPRESSION;

        public override ushort GetPackageId()
        {
            return xx.TypeId<COLUMNS>.value;
        }

        public override void ToBBuffer(xx.BBuffer bb)
        {
            bb.Write(this.TABLE_CATALOG);
            bb.Write(this.TABLE_SCHEMA);
            bb.Write(this.TABLE_NAME);
            bb.Write(this.COLUMN_NAME);
            bb.Write(this.ORDINAL_POSITION);
            bb.Write(this.COLUMN_DEFAULT);
            bb.Write(this.IS_NULLABLE);
            bb.Write(this.DATA_TYPE);
            bb.Write(this.CHARACTER_MAXIMUM_LENGTH);
            bb.Write(this.CHARACTER_OCTET_LENGTH);
            bb.Write(this.NUMERIC_PRECISION);
            bb.Write(this.NUMERIC_SCALE);
            bb.Write(this.DATETIME_PRECISION);
            bb.Write(this.CHARACTER_SET_NAME);
            bb.Write(this.COLLATION_NAME);
            bb.Write(this.COLUMN_TYPE);
            bb.Write(this.COLUMN_KEY);
            bb.Write(this.EXTRA);
            bb.Write(this.PRIVILEGES);
            bb.Write(this.COLUMN_COMMENT);
            bb.Write(this.GENERATION_EXPRESSION);
        }

        public override void FromBBuffer(xx.BBuffer bb)
        {
            bb.readLengthLimit = 0;
            bb.Read(ref this.TABLE_CATALOG);
            bb.readLengthLimit = 0;
            bb.Read(ref this.TABLE_SCHEMA);
            bb.readLengthLimit = 0;
            bb.Read(ref this.TABLE_NAME);
            bb.readLengthLimit = 0;
            bb.Read(ref this.COLUMN_NAME);
            bb.Read(ref this.ORDINAL_POSITION);
            bb.readLengthLimit = 0;
            bb.Read(ref this.COLUMN_DEFAULT);
            bb.readLengthLimit = 0;
            bb.Read(ref this.IS_NULLABLE);
            bb.readLengthLimit = 0;
            bb.Read(ref this.DATA_TYPE);
            bb.Read(ref this.CHARACTER_MAXIMUM_LENGTH);
            bb.Read(ref this.CHARACTER_OCTET_LENGTH);
            bb.Read(ref this.NUMERIC_PRECISION);
            bb.Read(ref this.NUMERIC_SCALE);
            bb.Read(ref this.DATETIME_PRECISION);
            bb.readLengthLimit = 0;
            bb.Read(ref this.CHARACTER_SET_NAME);
            bb.readLengthLimit = 0;
            bb.Read(ref this.COLLATION_NAME);
            bb.readLengthLimit = 0;
            bb.Read(ref this.COLUMN_TYPE);
            bb.readLengthLimit = 0;
            bb.Read(ref this.COLUMN_KEY);
            bb.readLengthLimit = 0;
            bb.Read(ref this.EXTRA);
            bb.readLengthLimit = 0;
            bb.Read(ref this.PRIVILEGES);
            bb.readLengthLimit = 0;
            bb.Read(ref this.COLUMN_COMMENT);
            bb.readLengthLimit = 0;
            bb.Read(ref this.GENERATION_EXPRESSION);
        }
        public override void ToString(System.Text.StringBuilder s)
        {
            if (__toStringing)
            {
        	    s.Append("[ \"***** recursived *****\" ]");
        	    return;
            }
            else __toStringing = true;

            s.Append("{ \"pkgTypeName\":\"Tables.COLUMNS\", \"pkgTypeId\":" + GetPackageId());
            ToStringCore(s);
            s.Append(" }");

            __toStringing = false;
        }
        public override void ToStringCore(System.Text.StringBuilder s)
        {
            if (TABLE_CATALOG != null) s.Append(", \"TABLE_CATALOG\":\"" + TABLE_CATALOG.ToString() + "\"");
            else s.Append(", \"TABLE_CATALOG\":nil");
            if (TABLE_SCHEMA != null) s.Append(", \"TABLE_SCHEMA\":\"" + TABLE_SCHEMA.ToString() + "\"");
            else s.Append(", \"TABLE_SCHEMA\":nil");
            if (TABLE_NAME != null) s.Append(", \"TABLE_NAME\":\"" + TABLE_NAME.ToString() + "\"");
            else s.Append(", \"TABLE_NAME\":nil");
            if (COLUMN_NAME != null) s.Append(", \"COLUMN_NAME\":\"" + COLUMN_NAME.ToString() + "\"");
            else s.Append(", \"COLUMN_NAME\":nil");
            s.Append(", \"ORDINAL_POSITION\":" + ORDINAL_POSITION.ToString());
            if (COLUMN_DEFAULT != null) s.Append(", \"COLUMN_DEFAULT\":\"" + COLUMN_DEFAULT.ToString() + "\"");
            else s.Append(", \"COLUMN_DEFAULT\":nil");
            if (IS_NULLABLE != null) s.Append(", \"IS_NULLABLE\":\"" + IS_NULLABLE.ToString() + "\"");
            else s.Append(", \"IS_NULLABLE\":nil");
            if (DATA_TYPE != null) s.Append(", \"DATA_TYPE\":\"" + DATA_TYPE.ToString() + "\"");
            else s.Append(", \"DATA_TYPE\":nil");
            s.Append(", \"CHARACTER_MAXIMUM_LENGTH\":" + (CHARACTER_MAXIMUM_LENGTH.HasValue ? CHARACTER_MAXIMUM_LENGTH.Value.ToString() : "nil"));
            s.Append(", \"CHARACTER_OCTET_LENGTH\":" + (CHARACTER_OCTET_LENGTH.HasValue ? CHARACTER_OCTET_LENGTH.Value.ToString() : "nil"));
            s.Append(", \"NUMERIC_PRECISION\":" + (NUMERIC_PRECISION.HasValue ? NUMERIC_PRECISION.Value.ToString() : "nil"));
            s.Append(", \"NUMERIC_SCALE\":" + (NUMERIC_SCALE.HasValue ? NUMERIC_SCALE.Value.ToString() : "nil"));
            s.Append(", \"DATETIME_PRECISION\":" + (DATETIME_PRECISION.HasValue ? DATETIME_PRECISION.Value.ToString() : "nil"));
            if (CHARACTER_SET_NAME != null) s.Append(", \"CHARACTER_SET_NAME\":\"" + CHARACTER_SET_NAME.ToString() + "\"");
            else s.Append(", \"CHARACTER_SET_NAME\":nil");
            if (COLLATION_NAME != null) s.Append(", \"COLLATION_NAME\":\"" + COLLATION_NAME.ToString() + "\"");
            else s.Append(", \"COLLATION_NAME\":nil");
            if (COLUMN_TYPE != null) s.Append(", \"COLUMN_TYPE\":\"" + COLUMN_TYPE.ToString() + "\"");
            else s.Append(", \"COLUMN_TYPE\":nil");
            if (COLUMN_KEY != null) s.Append(", \"COLUMN_KEY\":\"" + COLUMN_KEY.ToString() + "\"");
            else s.Append(", \"COLUMN_KEY\":nil");
            if (EXTRA != null) s.Append(", \"EXTRA\":\"" + EXTRA.ToString() + "\"");
            else s.Append(", \"EXTRA\":nil");
            if (PRIVILEGES != null) s.Append(", \"PRIVILEGES\":\"" + PRIVILEGES.ToString() + "\"");
            else s.Append(", \"PRIVILEGES\":nil");
            if (COLUMN_COMMENT != null) s.Append(", \"COLUMN_COMMENT\":\"" + COLUMN_COMMENT.ToString() + "\"");
            else s.Append(", \"COLUMN_COMMENT\":nil");
            if (GENERATION_EXPRESSION != null) s.Append(", \"GENERATION_EXPRESSION\":\"" + GENERATION_EXPRESSION.ToString() + "\"");
            else s.Append(", \"GENERATION_EXPRESSION\":nil");
        }
        public override string ToString()
        {
            var sb = new System.Text.StringBuilder();
            ToString(sb);
            return sb.ToString();
        }
        public override void MySqlAppend(System.Text.StringBuilder sb, bool ignoreReadOnly)
        {
            sb.Append("(");
            sb.Append(this.TABLE_CATALOG == null ? "null" : ("'" + this.TABLE_CATALOG.Replace("'", "''") + "'"));
            sb.Append(", ");
            sb.Append(this.TABLE_SCHEMA == null ? "null" : ("'" + this.TABLE_SCHEMA.Replace("'", "''") + "'"));
            sb.Append(", ");
            sb.Append(this.TABLE_NAME == null ? "null" : ("'" + this.TABLE_NAME.Replace("'", "''") + "'"));
            sb.Append(", ");
            sb.Append(this.COLUMN_NAME == null ? "null" : ("'" + this.COLUMN_NAME.Replace("'", "''") + "'"));
            sb.Append(", ");
            sb.Append(this.ORDINAL_POSITION);
            sb.Append(", ");
            sb.Append(this.COLUMN_DEFAULT == null ? "null" : ("'" + this.COLUMN_DEFAULT.Replace("'", "''") + "'"));
            sb.Append(", ");
            sb.Append(this.IS_NULLABLE == null ? "null" : ("'" + this.IS_NULLABLE.Replace("'", "''") + "'"));
            sb.Append(", ");
            sb.Append(this.DATA_TYPE == null ? "null" : ("'" + this.DATA_TYPE.Replace("'", "''") + "'"));
            sb.Append(", ");
            sb.Append(this.CHARACTER_MAXIMUM_LENGTH.HasValue ? this.CHARACTER_MAXIMUM_LENGTH.Value.ToString() : "null");
            sb.Append(", ");
            sb.Append(this.CHARACTER_OCTET_LENGTH.HasValue ? this.CHARACTER_OCTET_LENGTH.Value.ToString() : "null");
            sb.Append(", ");
            sb.Append(this.NUMERIC_PRECISION.HasValue ? this.NUMERIC_PRECISION.Value.ToString() : "null");
            sb.Append(", ");
            sb.Append(this.NUMERIC_SCALE.HasValue ? this.NUMERIC_SCALE.Value.ToString() : "null");
            sb.Append(", ");
            sb.Append(this.DATETIME_PRECISION.HasValue ? this.DATETIME_PRECISION.Value.ToString() : "null");
            sb.Append(", ");
            sb.Append(this.CHARACTER_SET_NAME == null ? "null" : ("'" + this.CHARACTER_SET_NAME.Replace("'", "''") + "'"));
            sb.Append(", ");
            sb.Append(this.COLLATION_NAME == null ? "null" : ("'" + this.COLLATION_NAME.Replace("'", "''") + "'"));
            sb.Append(", ");
            sb.Append(this.COLUMN_TYPE == null ? "null" : ("'" + this.COLUMN_TYPE.Replace("'", "''") + "'"));
            sb.Append(", ");
            sb.Append(this.COLUMN_KEY == null ? "null" : ("'" + this.COLUMN_KEY.Replace("'", "''") + "'"));
            sb.Append(", ");
            sb.Append(this.EXTRA == null ? "null" : ("'" + this.EXTRA.Replace("'", "''") + "'"));
            sb.Append(", ");
            sb.Append(this.PRIVILEGES == null ? "null" : ("'" + this.PRIVILEGES.Replace("'", "''") + "'"));
            sb.Append(", ");
            sb.Append(this.COLUMN_COMMENT == null ? "null" : ("'" + this.COLUMN_COMMENT.Replace("'", "''") + "'"));
            sb.Append(", ");
            sb.Append(this.GENERATION_EXPRESSION == null ? "null" : ("'" + this.GENERATION_EXPRESSION.Replace("'", "''") + "'"));
            sb.Append(", ");
            sb.Length -= 2;
            sb.Append(")");

        }
    }
    public partial class show_create_table : xx.Object
    {
        public string Table;
        public string CreateTable;

        public override ushort GetPackageId()
        {
            return xx.TypeId<show_create_table>.value;
        }

        public override void ToBBuffer(xx.BBuffer bb)
        {
            bb.Write(this.Table);
            bb.Write(this.CreateTable);
        }

        public override void FromBBuffer(xx.BBuffer bb)
        {
            bb.readLengthLimit = 0;
            bb.Read(ref this.Table);
            bb.readLengthLimit = 0;
            bb.Read(ref this.CreateTable);
        }
        public override void ToString(System.Text.StringBuilder s)
        {
            if (__toStringing)
            {
        	    s.Append("[ \"***** recursived *****\" ]");
        	    return;
            }
            else __toStringing = true;

            s.Append("{ \"pkgTypeName\":\"Tables.show_create_table\", \"pkgTypeId\":" + GetPackageId());
            ToStringCore(s);
            s.Append(" }");

            __toStringing = false;
        }
        public override void ToStringCore(System.Text.StringBuilder s)
        {
            if (Table != null) s.Append(", \"Table\":\"" + Table.ToString() + "\"");
            else s.Append(", \"Table\":nil");
            if (CreateTable != null) s.Append(", \"CreateTable\":\"" + CreateTable.ToString() + "\"");
            else s.Append(", \"CreateTable\":nil");
        }
        public override string ToString()
        {
            var sb = new System.Text.StringBuilder();
            ToString(sb);
            return sb.ToString();
        }
        public override void MySqlAppend(System.Text.StringBuilder sb, bool ignoreReadOnly)
        {
            sb.Append("(");
            sb.Append(this.Table == null ? "null" : ("'" + this.Table.Replace("'", "''") + "'"));
            sb.Append(", ");
            sb.Append(this.CreateTable == null ? "null" : ("'" + this.CreateTable.Replace("'", "''") + "'"));
            sb.Append(", ");
            sb.Length -= 2;
            sb.Append(")");

        }
    }
}
    public static class AllTypes
    {
        public static void Register()
        {
            xx.Object.RegisterInternals();
            xx.Object.Register<DbTable>(3);
            xx.Object.Register<Tables.TABLES>(4);
            xx.Object.Register<xx.List<DbColumn>>(8);
            xx.Object.Register<DbColumn>(6);
            xx.Object.Register<Tables.COLUMNS>(7);
            xx.Object.Register<Tables.show_create_table>(9);
        }
    }
}
