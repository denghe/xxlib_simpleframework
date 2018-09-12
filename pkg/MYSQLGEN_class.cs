using System;
using xx;
namespace MYSQLGEN
{
    public static class PkgGenMd5
    {
        public const string value = "fc89e0204eff3d70f7ad98e684542acb"; 
    }

    public partial class DbTable : Tables.TABLES
    {
        public List<DbColumn> childs;
        public string createScript;

        public override ushort GetPackageId()
        {
            return TypeIdMaps<DbTable>.typeId;
        }

        public override void ToBBuffer(BBuffer bb)
        {
            base.ToBBuffer(bb);
            bb.Write(this.childs);
            bb.Write(this.createScript);
        }

        public override void FromBBuffer(BBuffer bb)
        {
            base.FromBBuffer(bb);
            bb.readLengthLimit = 0;
            bb.Read(ref this.childs);
            bb.readLengthLimit = 0;
            bb.Read(ref this.createScript);
        }
        public override void ToString(ref System.Text.StringBuilder str)
        {
            if (GetToStringFlag())
            {
        	    str.Append("[ \"***** recursived *****\" ]");
        	    return;
            }
            else SetToStringFlag(true);

            str.Append("{ \"pkgTypeName\":\"DbTable\", \"pkgTypeId\":" + GetPackageId());
            ToStringCore(ref str);
            str.Append(" }");
        
            SetToStringFlag(false);
        }
        public override void ToStringCore(ref System.Text.StringBuilder str)
        {
            base.ToStringCore(ref str);
            str.Append(", \"childs\":" + (childs == null ? "nil" : childs.ToString()));
            if (createScript != null) str.Append(", \"createScript\":\"" + createScript + "\"");
            else str.Append(", \"createScript\":nil");
        }
        public override string ToString()
        {
            var sb = new System.Text.StringBuilder();
            ToString(ref sb);
            return sb.ToString();
        }
    }
    public partial class DbColumn : Tables.COLUMNS
    {
        public DbTable parent;

        public override ushort GetPackageId()
        {
            return TypeIdMaps<DbColumn>.typeId;
        }

        public override void ToBBuffer(BBuffer bb)
        {
            base.ToBBuffer(bb);
            bb.Write(this.parent);
        }

        public override void FromBBuffer(BBuffer bb)
        {
            base.FromBBuffer(bb);
            bb.Read(ref this.parent);
        }
        public override void ToString(ref System.Text.StringBuilder str)
        {
            if (GetToStringFlag())
            {
        	    str.Append("[ \"***** recursived *****\" ]");
        	    return;
            }
            else SetToStringFlag(true);

            str.Append("{ \"pkgTypeName\":\"DbColumn\", \"pkgTypeId\":" + GetPackageId());
            ToStringCore(ref str);
            str.Append(" }");
        
            SetToStringFlag(false);
        }
        public override void ToStringCore(ref System.Text.StringBuilder str)
        {
            base.ToStringCore(ref str);
            str.Append(", \"parent\":" + (parent == null ? "nil" : parent.ToString()));
        }
        public override string ToString()
        {
            var sb = new System.Text.StringBuilder();
            ToString(ref sb);
            return sb.ToString();
        }
    }
namespace Tables
{
    public partial class TABLES : IBBuffer
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

        public virtual ushort GetPackageId()
        {
            return TypeIdMaps<TABLES>.typeId;
        }

        public virtual void ToBBuffer(BBuffer bb)
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

        public virtual void FromBBuffer(BBuffer bb)
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
        public virtual void ToString(ref System.Text.StringBuilder str)
        {
            if (GetToStringFlag())
            {
        	    str.Append("[ \"***** recursived *****\" ]");
        	    return;
            }
            else SetToStringFlag(true);

            str.Append("{ \"pkgTypeName\":\"Tables.TABLES\", \"pkgTypeId\":" + GetPackageId());
            ToStringCore(ref str);
            str.Append(" }");
        
            SetToStringFlag(false);
        }
        public virtual void ToStringCore(ref System.Text.StringBuilder str)
        {
            if (TABLE_CATALOG != null) str.Append(", \"TABLE_CATALOG\":\"" + TABLE_CATALOG + "\"");
            else str.Append(", \"TABLE_CATALOG\":nil");
            if (TABLE_SCHEMA != null) str.Append(", \"TABLE_SCHEMA\":\"" + TABLE_SCHEMA + "\"");
            else str.Append(", \"TABLE_SCHEMA\":nil");
            if (TABLE_NAME != null) str.Append(", \"TABLE_NAME\":\"" + TABLE_NAME + "\"");
            else str.Append(", \"TABLE_NAME\":nil");
            if (TABLE_TYPE != null) str.Append(", \"TABLE_TYPE\":\"" + TABLE_TYPE + "\"");
            else str.Append(", \"TABLE_TYPE\":nil");
            if (ENGINE != null) str.Append(", \"ENGINE\":\"" + ENGINE + "\"");
            else str.Append(", \"ENGINE\":nil");
            str.Append(", \"VERSION\":" + (VERSION.HasValue ? VERSION.Value.ToString() : "nil"));
            if (ROW_FORMAT != null) str.Append(", \"ROW_FORMAT\":\"" + ROW_FORMAT + "\"");
            else str.Append(", \"ROW_FORMAT\":nil");
            str.Append(", \"TABLE_ROWS\":" + (TABLE_ROWS.HasValue ? TABLE_ROWS.Value.ToString() : "nil"));
            str.Append(", \"AVG_ROW_LENGTH\":" + (AVG_ROW_LENGTH.HasValue ? AVG_ROW_LENGTH.Value.ToString() : "nil"));
            str.Append(", \"DATA_LENGTH\":" + (DATA_LENGTH.HasValue ? DATA_LENGTH.Value.ToString() : "nil"));
            str.Append(", \"MAX_DATA_LENGTH\":" + (MAX_DATA_LENGTH.HasValue ? MAX_DATA_LENGTH.Value.ToString() : "nil"));
            str.Append(", \"INDEX_LENGTH\":" + (INDEX_LENGTH.HasValue ? INDEX_LENGTH.Value.ToString() : "nil"));
            str.Append(", \"DATA_FREE\":" + (DATA_FREE.HasValue ? DATA_FREE.Value.ToString() : "nil"));
            str.Append(", \"AUTO_INCREMENT\":" + (AUTO_INCREMENT.HasValue ? AUTO_INCREMENT.Value.ToString() : "nil"));
            if (CREATE_TIME != null) str.Append(", \"CREATE_TIME\":\"" + CREATE_TIME + "\"");
            else str.Append(", \"CREATE_TIME\":nil");
            if (UPDATE_TIME != null) str.Append(", \"UPDATE_TIME\":\"" + UPDATE_TIME + "\"");
            else str.Append(", \"UPDATE_TIME\":nil");
            if (CHECK_TIME != null) str.Append(", \"CHECK_TIME\":\"" + CHECK_TIME + "\"");
            else str.Append(", \"CHECK_TIME\":nil");
            if (TABLE_COLLATION != null) str.Append(", \"TABLE_COLLATION\":\"" + TABLE_COLLATION + "\"");
            else str.Append(", \"TABLE_COLLATION\":nil");
            str.Append(", \"CHECKSUM\":" + (CHECKSUM.HasValue ? CHECKSUM.Value.ToString() : "nil"));
            if (CREATE_OPTIONS != null) str.Append(", \"CREATE_OPTIONS\":\"" + CREATE_OPTIONS + "\"");
            else str.Append(", \"CREATE_OPTIONS\":nil");
            if (TABLE_COMMENT != null) str.Append(", \"TABLE_COMMENT\":\"" + TABLE_COMMENT + "\"");
            else str.Append(", \"TABLE_COMMENT\":nil");
        }
        public override string ToString()
        {
            var sb = new System.Text.StringBuilder();
            ToString(ref sb);
            return sb.ToString();
        }
        bool toStringFlag;
        public void SetToStringFlag(bool doing)
        {
            toStringFlag = doing;
        }
        public bool GetToStringFlag()
        {
            return toStringFlag;
        }
    }
    public partial class COLUMNS : IBBuffer
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

        public virtual ushort GetPackageId()
        {
            return TypeIdMaps<COLUMNS>.typeId;
        }

        public virtual void ToBBuffer(BBuffer bb)
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

        public virtual void FromBBuffer(BBuffer bb)
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
        public virtual void ToString(ref System.Text.StringBuilder str)
        {
            if (GetToStringFlag())
            {
        	    str.Append("[ \"***** recursived *****\" ]");
        	    return;
            }
            else SetToStringFlag(true);

            str.Append("{ \"pkgTypeName\":\"Tables.COLUMNS\", \"pkgTypeId\":" + GetPackageId());
            ToStringCore(ref str);
            str.Append(" }");
        
            SetToStringFlag(false);
        }
        public virtual void ToStringCore(ref System.Text.StringBuilder str)
        {
            if (TABLE_CATALOG != null) str.Append(", \"TABLE_CATALOG\":\"" + TABLE_CATALOG + "\"");
            else str.Append(", \"TABLE_CATALOG\":nil");
            if (TABLE_SCHEMA != null) str.Append(", \"TABLE_SCHEMA\":\"" + TABLE_SCHEMA + "\"");
            else str.Append(", \"TABLE_SCHEMA\":nil");
            if (TABLE_NAME != null) str.Append(", \"TABLE_NAME\":\"" + TABLE_NAME + "\"");
            else str.Append(", \"TABLE_NAME\":nil");
            if (COLUMN_NAME != null) str.Append(", \"COLUMN_NAME\":\"" + COLUMN_NAME + "\"");
            else str.Append(", \"COLUMN_NAME\":nil");
            str.Append(", \"ORDINAL_POSITION\":" + ORDINAL_POSITION);
            if (COLUMN_DEFAULT != null) str.Append(", \"COLUMN_DEFAULT\":\"" + COLUMN_DEFAULT + "\"");
            else str.Append(", \"COLUMN_DEFAULT\":nil");
            if (IS_NULLABLE != null) str.Append(", \"IS_NULLABLE\":\"" + IS_NULLABLE + "\"");
            else str.Append(", \"IS_NULLABLE\":nil");
            if (DATA_TYPE != null) str.Append(", \"DATA_TYPE\":\"" + DATA_TYPE + "\"");
            else str.Append(", \"DATA_TYPE\":nil");
            str.Append(", \"CHARACTER_MAXIMUM_LENGTH\":" + (CHARACTER_MAXIMUM_LENGTH.HasValue ? CHARACTER_MAXIMUM_LENGTH.Value.ToString() : "nil"));
            str.Append(", \"CHARACTER_OCTET_LENGTH\":" + (CHARACTER_OCTET_LENGTH.HasValue ? CHARACTER_OCTET_LENGTH.Value.ToString() : "nil"));
            str.Append(", \"NUMERIC_PRECISION\":" + (NUMERIC_PRECISION.HasValue ? NUMERIC_PRECISION.Value.ToString() : "nil"));
            str.Append(", \"NUMERIC_SCALE\":" + (NUMERIC_SCALE.HasValue ? NUMERIC_SCALE.Value.ToString() : "nil"));
            str.Append(", \"DATETIME_PRECISION\":" + (DATETIME_PRECISION.HasValue ? DATETIME_PRECISION.Value.ToString() : "nil"));
            if (CHARACTER_SET_NAME != null) str.Append(", \"CHARACTER_SET_NAME\":\"" + CHARACTER_SET_NAME + "\"");
            else str.Append(", \"CHARACTER_SET_NAME\":nil");
            if (COLLATION_NAME != null) str.Append(", \"COLLATION_NAME\":\"" + COLLATION_NAME + "\"");
            else str.Append(", \"COLLATION_NAME\":nil");
            if (COLUMN_TYPE != null) str.Append(", \"COLUMN_TYPE\":\"" + COLUMN_TYPE + "\"");
            else str.Append(", \"COLUMN_TYPE\":nil");
            if (COLUMN_KEY != null) str.Append(", \"COLUMN_KEY\":\"" + COLUMN_KEY + "\"");
            else str.Append(", \"COLUMN_KEY\":nil");
            if (EXTRA != null) str.Append(", \"EXTRA\":\"" + EXTRA + "\"");
            else str.Append(", \"EXTRA\":nil");
            if (PRIVILEGES != null) str.Append(", \"PRIVILEGES\":\"" + PRIVILEGES + "\"");
            else str.Append(", \"PRIVILEGES\":nil");
            if (COLUMN_COMMENT != null) str.Append(", \"COLUMN_COMMENT\":\"" + COLUMN_COMMENT + "\"");
            else str.Append(", \"COLUMN_COMMENT\":nil");
            if (GENERATION_EXPRESSION != null) str.Append(", \"GENERATION_EXPRESSION\":\"" + GENERATION_EXPRESSION + "\"");
            else str.Append(", \"GENERATION_EXPRESSION\":nil");
        }
        public override string ToString()
        {
            var sb = new System.Text.StringBuilder();
            ToString(ref sb);
            return sb.ToString();
        }
        bool toStringFlag;
        public void SetToStringFlag(bool doing)
        {
            toStringFlag = doing;
        }
        public bool GetToStringFlag()
        {
            return toStringFlag;
        }
    }
    public partial class show_create_table : IBBuffer
    {
        public string Table;
        public string CreateTable;

        public virtual ushort GetPackageId()
        {
            return TypeIdMaps<show_create_table>.typeId;
        }

        public virtual void ToBBuffer(BBuffer bb)
        {
            bb.Write(this.Table);
            bb.Write(this.CreateTable);
        }

        public virtual void FromBBuffer(BBuffer bb)
        {
            bb.readLengthLimit = 0;
            bb.Read(ref this.Table);
            bb.readLengthLimit = 0;
            bb.Read(ref this.CreateTable);
        }
        public virtual void ToString(ref System.Text.StringBuilder str)
        {
            if (GetToStringFlag())
            {
        	    str.Append("[ \"***** recursived *****\" ]");
        	    return;
            }
            else SetToStringFlag(true);

            str.Append("{ \"pkgTypeName\":\"Tables.show_create_table\", \"pkgTypeId\":" + GetPackageId());
            ToStringCore(ref str);
            str.Append(" }");
        
            SetToStringFlag(false);
        }
        public virtual void ToStringCore(ref System.Text.StringBuilder str)
        {
            if (Table != null) str.Append(", \"Table\":\"" + Table + "\"");
            else str.Append(", \"Table\":nil");
            if (CreateTable != null) str.Append(", \"CreateTable\":\"" + CreateTable + "\"");
            else str.Append(", \"CreateTable\":nil");
        }
        public override string ToString()
        {
            var sb = new System.Text.StringBuilder();
            ToString(ref sb);
            return sb.ToString();
        }
        bool toStringFlag;
        public void SetToStringFlag(bool doing)
        {
            toStringFlag = doing;
        }
        public bool GetToStringFlag()
        {
            return toStringFlag;
        }
    }
}
    public static class AllTypes
    {
        public static void Register()
        {
            xx.BBuffer.RegisterInternals();
            BBuffer.Register<DbTable>(3);
            BBuffer.Register<Tables.TABLES>(4);
            BBuffer.Register<List<DbColumn>>(8);
            BBuffer.Register<DbColumn>(6);
            BBuffer.Register<Tables.COLUMNS>(7);
            BBuffer.Register<Tables.show_create_table>(9);
        }
    }
}
