using System;
using xx;
namespace PKG
{
    public static class PkgGenMd5
    {
        public const string value = "a6a6b3c8d511000a913006017240448c"; 
    }

    public partial class Foo : IBBuffer
    {
        public int id;
        public int? age;
        public string info;
        public List<Foo> childs;
        public IBBuffer o;
        public List<IBBuffer> os;

        public virtual ushort GetPackageId()
        {
            return TypeIdMaps<Foo>.typeId;
        }

        public virtual void ToBBuffer(BBuffer bb)
        {
            bb.Write(this.id);
            bb.Write(this.age);
            bb.Write(this.info);
            bb.Write(this.childs);
            bb.Write(this.o);
            bb.Write(this.os);
        }

        public virtual void FromBBuffer(BBuffer bb)
        {
            bb.Read(ref this.id);
            bb.Read(ref this.age);
            bb.readLengthLimit = 0;
            bb.Read(ref this.info);
            bb.readLengthLimit = 0;
            bb.Read(ref this.childs);
            bb.Read(ref this.o);
            bb.readLengthLimit = 0;
            bb.Read(ref this.os);
        }
        public virtual void ToString(ref System.Text.StringBuilder str)
        {
            if (GetToStringFlag())
            {
        	    str.Append("[ \"***** recursived *****\" ]");
        	    return;
            }
            else SetToStringFlag(true);

            str.Append("{ \"pkgTypeName\":\"Foo\", \"pkgTypeId\":" + GetPackageId());
            ToStringCore(ref str);
            str.Append(" }");
        
            SetToStringFlag(false);
        }
        public virtual void ToStringCore(ref System.Text.StringBuilder str)
        {
            str.Append(", \"id\":" + id);
            str.Append(", \"age\":" + (age.HasValue ? age.Value.ToString() : "nil"));
            if (info != null) str.Append(", \"info\":\"" + info + "\"");
            else str.Append(", \"info\":nil");
            str.Append(", \"childs\":" + (childs == null ? "nil" : childs.ToString()));
            str.Append(", \"o\":" + o);
            str.Append(", \"os\":" + (os == null ? "nil" : os.ToString()));
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
    public partial class FooEx : Foo
    {

        public override ushort GetPackageId()
        {
            return TypeIdMaps<FooEx>.typeId;
        }

        public override void ToBBuffer(BBuffer bb)
        {
            base.ToBBuffer(bb);
        }

        public override void FromBBuffer(BBuffer bb)
        {
            base.FromBBuffer(bb);
        }
        public override void ToString(ref System.Text.StringBuilder str)
        {
            if (GetToStringFlag())
            {
        	    str.Append("[ \"***** recursived *****\" ]");
        	    return;
            }
            else SetToStringFlag(true);

            str.Append("{ \"pkgTypeName\":\"FooEx\", \"pkgTypeId\":" + GetPackageId());
            ToStringCore(ref str);
            str.Append(" }");
        
            SetToStringFlag(false);
        }
        public override void ToStringCore(ref System.Text.StringBuilder str)
        {
            base.ToStringCore(ref str);
        }
        public override string ToString()
        {
            var sb = new System.Text.StringBuilder();
            ToString(ref sb);
            return sb.ToString();
        }
    }
    public partial class Node : Tables.node
    {
        public Node parent;
        public List<Node> childs;

        public override ushort GetPackageId()
        {
            return TypeIdMaps<Node>.typeId;
        }

        public override void ToBBuffer(BBuffer bb)
        {
            base.ToBBuffer(bb);
            bb.Write(this.parent);
            bb.Write(this.childs);
        }

        public override void FromBBuffer(BBuffer bb)
        {
            base.FromBBuffer(bb);
            bb.Read(ref this.parent);
            bb.readLengthLimit = 0;
            bb.Read(ref this.childs);
        }
        public override void ToString(ref System.Text.StringBuilder str)
        {
            if (GetToStringFlag())
            {
        	    str.Append("[ \"***** recursived *****\" ]");
        	    return;
            }
            else SetToStringFlag(true);

            str.Append("{ \"pkgTypeName\":\"Node\", \"pkgTypeId\":" + GetPackageId());
            ToStringCore(ref str);
            str.Append(" }");
        
            SetToStringFlag(false);
        }
        public override void ToStringCore(ref System.Text.StringBuilder str)
        {
            base.ToStringCore(ref str);
            str.Append(", \"parent\":" + (parent == null ? "nil" : parent.ToString()));
            str.Append(", \"childs\":" + (childs == null ? "nil" : childs.ToString()));
        }
        public override string ToString()
        {
            var sb = new System.Text.StringBuilder();
            ToString(ref sb);
            return sb.ToString();
        }
    }
    public partial class DataSet : IBBuffer
    {
        public List<Table> tables;

        public virtual ushort GetPackageId()
        {
            return TypeIdMaps<DataSet>.typeId;
        }

        public virtual void ToBBuffer(BBuffer bb)
        {
            bb.Write(this.tables);
        }

        public virtual void FromBBuffer(BBuffer bb)
        {
            bb.readLengthLimit = 0;
            bb.Read(ref this.tables);
        }
        public virtual void ToString(ref System.Text.StringBuilder str)
        {
            if (GetToStringFlag())
            {
        	    str.Append("[ \"***** recursived *****\" ]");
        	    return;
            }
            else SetToStringFlag(true);

            str.Append("{ \"pkgTypeName\":\"DataSet\", \"pkgTypeId\":" + GetPackageId());
            ToStringCore(ref str);
            str.Append(" }");
        
            SetToStringFlag(false);
        }
        public virtual void ToStringCore(ref System.Text.StringBuilder str)
        {
            str.Append(", \"tables\":" + (tables == null ? "nil" : tables.ToString()));
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
    public partial class Table : IBBuffer
    {
        public DataSet parent;
        public string name;
        public List<TableColumn> columns;
        public List<TableRow> rows;

        public virtual ushort GetPackageId()
        {
            return TypeIdMaps<Table>.typeId;
        }

        public virtual void ToBBuffer(BBuffer bb)
        {
            bb.Write(this.parent);
            bb.Write(this.name);
            bb.Write(this.columns);
            bb.Write(this.rows);
        }

        public virtual void FromBBuffer(BBuffer bb)
        {
            bb.Read(ref this.parent);
            bb.readLengthLimit = 0;
            bb.Read(ref this.name);
            bb.readLengthLimit = 0;
            bb.Read(ref this.columns);
            bb.readLengthLimit = 0;
            bb.Read(ref this.rows);
        }
        public virtual void ToString(ref System.Text.StringBuilder str)
        {
            if (GetToStringFlag())
            {
        	    str.Append("[ \"***** recursived *****\" ]");
        	    return;
            }
            else SetToStringFlag(true);

            str.Append("{ \"pkgTypeName\":\"Table\", \"pkgTypeId\":" + GetPackageId());
            ToStringCore(ref str);
            str.Append(" }");
        
            SetToStringFlag(false);
        }
        public virtual void ToStringCore(ref System.Text.StringBuilder str)
        {
            str.Append(", \"parent\":" + (parent == null ? "nil" : parent.ToString()));
            if (name != null) str.Append(", \"name\":\"" + name + "\"");
            else str.Append(", \"name\":nil");
            str.Append(", \"columns\":" + (columns == null ? "nil" : columns.ToString()));
            str.Append(", \"rows\":" + (rows == null ? "nil" : rows.ToString()));
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
    public partial class TableColumn : IBBuffer
    {
        public string name;

        public virtual ushort GetPackageId()
        {
            return TypeIdMaps<TableColumn>.typeId;
        }

        public virtual void ToBBuffer(BBuffer bb)
        {
            bb.Write(this.name);
        }

        public virtual void FromBBuffer(BBuffer bb)
        {
            bb.readLengthLimit = 0;
            bb.Read(ref this.name);
        }
        public virtual void ToString(ref System.Text.StringBuilder str)
        {
            if (GetToStringFlag())
            {
        	    str.Append("[ \"***** recursived *****\" ]");
        	    return;
            }
            else SetToStringFlag(true);

            str.Append("{ \"pkgTypeName\":\"TableColumn\", \"pkgTypeId\":" + GetPackageId());
            ToStringCore(ref str);
            str.Append(" }");
        
            SetToStringFlag(false);
        }
        public virtual void ToStringCore(ref System.Text.StringBuilder str)
        {
            if (name != null) str.Append(", \"name\":\"" + name + "\"");
            else str.Append(", \"name\":nil");
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
    public partial class TableRow : IBBuffer
    {
        public List<TableRowValue> values;

        public virtual ushort GetPackageId()
        {
            return TypeIdMaps<TableRow>.typeId;
        }

        public virtual void ToBBuffer(BBuffer bb)
        {
            bb.Write(this.values);
        }

        public virtual void FromBBuffer(BBuffer bb)
        {
            bb.readLengthLimit = 0;
            bb.Read(ref this.values);
        }
        public virtual void ToString(ref System.Text.StringBuilder str)
        {
            if (GetToStringFlag())
            {
        	    str.Append("[ \"***** recursived *****\" ]");
        	    return;
            }
            else SetToStringFlag(true);

            str.Append("{ \"pkgTypeName\":\"TableRow\", \"pkgTypeId\":" + GetPackageId());
            ToStringCore(ref str);
            str.Append(" }");
        
            SetToStringFlag(false);
        }
        public virtual void ToStringCore(ref System.Text.StringBuilder str)
        {
            str.Append(", \"values\":" + (values == null ? "nil" : values.ToString()));
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
    public partial class TableRowValue : IBBuffer
    {

        public virtual ushort GetPackageId()
        {
            return TypeIdMaps<TableRowValue>.typeId;
        }

        public virtual void ToBBuffer(BBuffer bb)
        {
        }

        public virtual void FromBBuffer(BBuffer bb)
        {
        }
        public virtual void ToString(ref System.Text.StringBuilder str)
        {
            if (GetToStringFlag())
            {
        	    str.Append("[ \"***** recursived *****\" ]");
        	    return;
            }
            else SetToStringFlag(true);

            str.Append("{ \"pkgTypeName\":\"TableRowValue\", \"pkgTypeId\":" + GetPackageId());
            ToStringCore(ref str);
            str.Append(" }");
        
            SetToStringFlag(false);
        }
        public virtual void ToStringCore(ref System.Text.StringBuilder str)
        {
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
    public partial class TableRowValue_Int : TableRowValue
    {
        public int value;

        public override ushort GetPackageId()
        {
            return TypeIdMaps<TableRowValue_Int>.typeId;
        }

        public override void ToBBuffer(BBuffer bb)
        {
            base.ToBBuffer(bb);
            bb.Write(this.value);
        }

        public override void FromBBuffer(BBuffer bb)
        {
            base.FromBBuffer(bb);
            bb.Read(ref this.value);
        }
        public override void ToString(ref System.Text.StringBuilder str)
        {
            if (GetToStringFlag())
            {
        	    str.Append("[ \"***** recursived *****\" ]");
        	    return;
            }
            else SetToStringFlag(true);

            str.Append("{ \"pkgTypeName\":\"TableRowValue_Int\", \"pkgTypeId\":" + GetPackageId());
            ToStringCore(ref str);
            str.Append(" }");
        
            SetToStringFlag(false);
        }
        public override void ToStringCore(ref System.Text.StringBuilder str)
        {
            base.ToStringCore(ref str);
            str.Append(", \"value\":" + value);
        }
        public override string ToString()
        {
            var sb = new System.Text.StringBuilder();
            ToString(ref sb);
            return sb.ToString();
        }
    }
    public partial class TableRowValue_NullableInt : TableRowValue
    {
        public int? value;

        public override ushort GetPackageId()
        {
            return TypeIdMaps<TableRowValue_NullableInt>.typeId;
        }

        public override void ToBBuffer(BBuffer bb)
        {
            base.ToBBuffer(bb);
            bb.Write(this.value);
        }

        public override void FromBBuffer(BBuffer bb)
        {
            base.FromBBuffer(bb);
            bb.Read(ref this.value);
        }
        public override void ToString(ref System.Text.StringBuilder str)
        {
            if (GetToStringFlag())
            {
        	    str.Append("[ \"***** recursived *****\" ]");
        	    return;
            }
            else SetToStringFlag(true);

            str.Append("{ \"pkgTypeName\":\"TableRowValue_NullableInt\", \"pkgTypeId\":" + GetPackageId());
            ToStringCore(ref str);
            str.Append(" }");
        
            SetToStringFlag(false);
        }
        public override void ToStringCore(ref System.Text.StringBuilder str)
        {
            base.ToStringCore(ref str);
            str.Append(", \"value\":" + (value.HasValue ? value.Value.ToString() : "nil"));
        }
        public override string ToString()
        {
            var sb = new System.Text.StringBuilder();
            ToString(ref sb);
            return sb.ToString();
        }
    }
    public partial class TableRowValue_String : TableRowValue
    {
        public string value;

        public override ushort GetPackageId()
        {
            return TypeIdMaps<TableRowValue_String>.typeId;
        }

        public override void ToBBuffer(BBuffer bb)
        {
            base.ToBBuffer(bb);
            bb.Write(this.value);
        }

        public override void FromBBuffer(BBuffer bb)
        {
            base.FromBBuffer(bb);
            bb.readLengthLimit = 0;
            bb.Read(ref this.value);
        }
        public override void ToString(ref System.Text.StringBuilder str)
        {
            if (GetToStringFlag())
            {
        	    str.Append("[ \"***** recursived *****\" ]");
        	    return;
            }
            else SetToStringFlag(true);

            str.Append("{ \"pkgTypeName\":\"TableRowValue_String\", \"pkgTypeId\":" + GetPackageId());
            ToStringCore(ref str);
            str.Append(" }");
        
            SetToStringFlag(false);
        }
        public override void ToStringCore(ref System.Text.StringBuilder str)
        {
            base.ToStringCore(ref str);
            if (value != null) str.Append(", \"value\":\"" + value + "\"");
            else str.Append(", \"value\":nil");
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
    public partial class account : IBBuffer
    {
        public string id;
        public string username;
        public string nickname;
        public string avatar_id;
        public string phone;
        public string password;
        public string money;
        public string money_safe;
        public string total_recharge;
        public string total_consume;
        public string total_withdraw;
        public string create_time;
        public string enabled;
        public string enter_lobby_time;
        public string log_game_id;

        public virtual ushort GetPackageId()
        {
            return TypeIdMaps<account>.typeId;
        }

        public virtual void ToBBuffer(BBuffer bb)
        {
            bb.Write(this.id);
            bb.Write(this.username);
            bb.Write(this.nickname);
            bb.Write(this.avatar_id);
            bb.Write(this.phone);
            bb.Write(this.password);
            bb.Write(this.money);
            bb.Write(this.money_safe);
            bb.Write(this.total_recharge);
            bb.Write(this.total_consume);
            bb.Write(this.total_withdraw);
            bb.Write(this.create_time);
            bb.Write(this.enabled);
            bb.Write(this.enter_lobby_time);
            bb.Write(this.log_game_id);
        }

        public virtual void FromBBuffer(BBuffer bb)
        {
            bb.readLengthLimit = 0;
            bb.Read(ref this.id);
            bb.readLengthLimit = 0;
            bb.Read(ref this.username);
            bb.readLengthLimit = 0;
            bb.Read(ref this.nickname);
            bb.readLengthLimit = 0;
            bb.Read(ref this.avatar_id);
            bb.readLengthLimit = 0;
            bb.Read(ref this.phone);
            bb.readLengthLimit = 0;
            bb.Read(ref this.password);
            bb.readLengthLimit = 0;
            bb.Read(ref this.money);
            bb.readLengthLimit = 0;
            bb.Read(ref this.money_safe);
            bb.readLengthLimit = 0;
            bb.Read(ref this.total_recharge);
            bb.readLengthLimit = 0;
            bb.Read(ref this.total_consume);
            bb.readLengthLimit = 0;
            bb.Read(ref this.total_withdraw);
            bb.readLengthLimit = 0;
            bb.Read(ref this.create_time);
            bb.readLengthLimit = 0;
            bb.Read(ref this.enabled);
            bb.readLengthLimit = 0;
            bb.Read(ref this.enter_lobby_time);
            bb.readLengthLimit = 0;
            bb.Read(ref this.log_game_id);
        }
        public virtual void ToString(ref System.Text.StringBuilder str)
        {
            if (GetToStringFlag())
            {
        	    str.Append("[ \"***** recursived *****\" ]");
        	    return;
            }
            else SetToStringFlag(true);

            str.Append("{ \"pkgTypeName\":\"Tables.account\", \"pkgTypeId\":" + GetPackageId());
            ToStringCore(ref str);
            str.Append(" }");
        
            SetToStringFlag(false);
        }
        public virtual void ToStringCore(ref System.Text.StringBuilder str)
        {
            if (id != null) str.Append(", \"id\":\"" + id + "\"");
            else str.Append(", \"id\":nil");
            if (username != null) str.Append(", \"username\":\"" + username + "\"");
            else str.Append(", \"username\":nil");
            if (nickname != null) str.Append(", \"nickname\":\"" + nickname + "\"");
            else str.Append(", \"nickname\":nil");
            if (avatar_id != null) str.Append(", \"avatar_id\":\"" + avatar_id + "\"");
            else str.Append(", \"avatar_id\":nil");
            if (phone != null) str.Append(", \"phone\":\"" + phone + "\"");
            else str.Append(", \"phone\":nil");
            if (password != null) str.Append(", \"password\":\"" + password + "\"");
            else str.Append(", \"password\":nil");
            if (money != null) str.Append(", \"money\":\"" + money + "\"");
            else str.Append(", \"money\":nil");
            if (money_safe != null) str.Append(", \"money_safe\":\"" + money_safe + "\"");
            else str.Append(", \"money_safe\":nil");
            if (total_recharge != null) str.Append(", \"total_recharge\":\"" + total_recharge + "\"");
            else str.Append(", \"total_recharge\":nil");
            if (total_consume != null) str.Append(", \"total_consume\":\"" + total_consume + "\"");
            else str.Append(", \"total_consume\":nil");
            if (total_withdraw != null) str.Append(", \"total_withdraw\":\"" + total_withdraw + "\"");
            else str.Append(", \"total_withdraw\":nil");
            if (create_time != null) str.Append(", \"create_time\":\"" + create_time + "\"");
            else str.Append(", \"create_time\":nil");
            if (enabled != null) str.Append(", \"enabled\":\"" + enabled + "\"");
            else str.Append(", \"enabled\":nil");
            if (enter_lobby_time != null) str.Append(", \"enter_lobby_time\":\"" + enter_lobby_time + "\"");
            else str.Append(", \"enter_lobby_time\":nil");
            if (log_game_id != null) str.Append(", \"log_game_id\":\"" + log_game_id + "\"");
            else str.Append(", \"log_game_id\":nil");
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
    public partial class chat : IBBuffer
    {
        public string id;
        public string content;
        public string account_id;
        public string create_time;

        public virtual ushort GetPackageId()
        {
            return TypeIdMaps<chat>.typeId;
        }

        public virtual void ToBBuffer(BBuffer bb)
        {
            bb.Write(this.id);
            bb.Write(this.content);
            bb.Write(this.account_id);
            bb.Write(this.create_time);
        }

        public virtual void FromBBuffer(BBuffer bb)
        {
            bb.readLengthLimit = 0;
            bb.Read(ref this.id);
            bb.readLengthLimit = 0;
            bb.Read(ref this.content);
            bb.readLengthLimit = 0;
            bb.Read(ref this.account_id);
            bb.readLengthLimit = 0;
            bb.Read(ref this.create_time);
        }
        public virtual void ToString(ref System.Text.StringBuilder str)
        {
            if (GetToStringFlag())
            {
        	    str.Append("[ \"***** recursived *****\" ]");
        	    return;
            }
            else SetToStringFlag(true);

            str.Append("{ \"pkgTypeName\":\"Tables.chat\", \"pkgTypeId\":" + GetPackageId());
            ToStringCore(ref str);
            str.Append(" }");
        
            SetToStringFlag(false);
        }
        public virtual void ToStringCore(ref System.Text.StringBuilder str)
        {
            if (id != null) str.Append(", \"id\":\"" + id + "\"");
            else str.Append(", \"id\":nil");
            if (content != null) str.Append(", \"content\":\"" + content + "\"");
            else str.Append(", \"content\":nil");
            if (account_id != null) str.Append(", \"account_id\":\"" + account_id + "\"");
            else str.Append(", \"account_id\":nil");
            if (create_time != null) str.Append(", \"create_time\":\"" + create_time + "\"");
            else str.Append(", \"create_time\":nil");
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
    public partial class config : IBBuffer
    {
        public string key;
        public string datatype_name;
        public string title;
        public string desc;
        public string create_time;

        public virtual ushort GetPackageId()
        {
            return TypeIdMaps<config>.typeId;
        }

        public virtual void ToBBuffer(BBuffer bb)
        {
            bb.Write(this.key);
            bb.Write(this.datatype_name);
            bb.Write(this.title);
            bb.Write(this.desc);
            bb.Write(this.create_time);
        }

        public virtual void FromBBuffer(BBuffer bb)
        {
            bb.readLengthLimit = 0;
            bb.Read(ref this.key);
            bb.readLengthLimit = 0;
            bb.Read(ref this.datatype_name);
            bb.readLengthLimit = 0;
            bb.Read(ref this.title);
            bb.readLengthLimit = 0;
            bb.Read(ref this.desc);
            bb.readLengthLimit = 0;
            bb.Read(ref this.create_time);
        }
        public virtual void ToString(ref System.Text.StringBuilder str)
        {
            if (GetToStringFlag())
            {
        	    str.Append("[ \"***** recursived *****\" ]");
        	    return;
            }
            else SetToStringFlag(true);

            str.Append("{ \"pkgTypeName\":\"Tables.config\", \"pkgTypeId\":" + GetPackageId());
            ToStringCore(ref str);
            str.Append(" }");
        
            SetToStringFlag(false);
        }
        public virtual void ToStringCore(ref System.Text.StringBuilder str)
        {
            if (key != null) str.Append(", \"key\":\"" + key + "\"");
            else str.Append(", \"key\":nil");
            if (datatype_name != null) str.Append(", \"datatype_name\":\"" + datatype_name + "\"");
            else str.Append(", \"datatype_name\":nil");
            if (title != null) str.Append(", \"title\":\"" + title + "\"");
            else str.Append(", \"title\":nil");
            if (desc != null) str.Append(", \"desc\":\"" + desc + "\"");
            else str.Append(", \"desc\":nil");
            if (create_time != null) str.Append(", \"create_time\":\"" + create_time + "\"");
            else str.Append(", \"create_time\":nil");
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
    public partial class config_value : IBBuffer
    {
        public string key;
        public string value_bigint;
        public string value_double;
        public string value_varchar;
        public string create_time;

        public virtual ushort GetPackageId()
        {
            return TypeIdMaps<config_value>.typeId;
        }

        public virtual void ToBBuffer(BBuffer bb)
        {
            bb.Write(this.key);
            bb.Write(this.value_bigint);
            bb.Write(this.value_double);
            bb.Write(this.value_varchar);
            bb.Write(this.create_time);
        }

        public virtual void FromBBuffer(BBuffer bb)
        {
            bb.readLengthLimit = 0;
            bb.Read(ref this.key);
            bb.readLengthLimit = 0;
            bb.Read(ref this.value_bigint);
            bb.readLengthLimit = 0;
            bb.Read(ref this.value_double);
            bb.readLengthLimit = 0;
            bb.Read(ref this.value_varchar);
            bb.readLengthLimit = 0;
            bb.Read(ref this.create_time);
        }
        public virtual void ToString(ref System.Text.StringBuilder str)
        {
            if (GetToStringFlag())
            {
        	    str.Append("[ \"***** recursived *****\" ]");
        	    return;
            }
            else SetToStringFlag(true);

            str.Append("{ \"pkgTypeName\":\"Tables.config_value\", \"pkgTypeId\":" + GetPackageId());
            ToStringCore(ref str);
            str.Append(" }");
        
            SetToStringFlag(false);
        }
        public virtual void ToStringCore(ref System.Text.StringBuilder str)
        {
            if (key != null) str.Append(", \"key\":\"" + key + "\"");
            else str.Append(", \"key\":nil");
            if (value_bigint != null) str.Append(", \"value_bigint\":\"" + value_bigint + "\"");
            else str.Append(", \"value_bigint\":nil");
            if (value_double != null) str.Append(", \"value_double\":\"" + value_double + "\"");
            else str.Append(", \"value_double\":nil");
            if (value_varchar != null) str.Append(", \"value_varchar\":\"" + value_varchar + "\"");
            else str.Append(", \"value_varchar\":nil");
            if (create_time != null) str.Append(", \"create_time\":\"" + create_time + "\"");
            else str.Append(", \"create_time\":nil");
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
    public partial class datatype : IBBuffer
    {
        public string name;

        public virtual ushort GetPackageId()
        {
            return TypeIdMaps<datatype>.typeId;
        }

        public virtual void ToBBuffer(BBuffer bb)
        {
            bb.Write(this.name);
        }

        public virtual void FromBBuffer(BBuffer bb)
        {
            bb.readLengthLimit = 0;
            bb.Read(ref this.name);
        }
        public virtual void ToString(ref System.Text.StringBuilder str)
        {
            if (GetToStringFlag())
            {
        	    str.Append("[ \"***** recursived *****\" ]");
        	    return;
            }
            else SetToStringFlag(true);

            str.Append("{ \"pkgTypeName\":\"Tables.datatype\", \"pkgTypeId\":" + GetPackageId());
            ToStringCore(ref str);
            str.Append(" }");
        
            SetToStringFlag(false);
        }
        public virtual void ToStringCore(ref System.Text.StringBuilder str)
        {
            if (name != null) str.Append(", \"name\":\"" + name + "\"");
            else str.Append(", \"name\":nil");
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
    public partial class fish : IBBuffer
    {
        public string id;
        public string name;
        public string minCoin;
        public string maxCoin;

        public virtual ushort GetPackageId()
        {
            return TypeIdMaps<fish>.typeId;
        }

        public virtual void ToBBuffer(BBuffer bb)
        {
            bb.Write(this.id);
            bb.Write(this.name);
            bb.Write(this.minCoin);
            bb.Write(this.maxCoin);
        }

        public virtual void FromBBuffer(BBuffer bb)
        {
            bb.readLengthLimit = 0;
            bb.Read(ref this.id);
            bb.readLengthLimit = 0;
            bb.Read(ref this.name);
            bb.readLengthLimit = 0;
            bb.Read(ref this.minCoin);
            bb.readLengthLimit = 0;
            bb.Read(ref this.maxCoin);
        }
        public virtual void ToString(ref System.Text.StringBuilder str)
        {
            if (GetToStringFlag())
            {
        	    str.Append("[ \"***** recursived *****\" ]");
        	    return;
            }
            else SetToStringFlag(true);

            str.Append("{ \"pkgTypeName\":\"Tables.fish\", \"pkgTypeId\":" + GetPackageId());
            ToStringCore(ref str);
            str.Append(" }");
        
            SetToStringFlag(false);
        }
        public virtual void ToStringCore(ref System.Text.StringBuilder str)
        {
            if (id != null) str.Append(", \"id\":\"" + id + "\"");
            else str.Append(", \"id\":nil");
            if (name != null) str.Append(", \"name\":\"" + name + "\"");
            else str.Append(", \"name\":nil");
            if (minCoin != null) str.Append(", \"minCoin\":\"" + minCoin + "\"");
            else str.Append(", \"minCoin\":nil");
            if (maxCoin != null) str.Append(", \"maxCoin\":\"" + maxCoin + "\"");
            else str.Append(", \"maxCoin\":nil");
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
    public partial class game : IBBuffer
    {
        public string id;
        public string name;
        public string type_name;
        public string enabled;

        public virtual ushort GetPackageId()
        {
            return TypeIdMaps<game>.typeId;
        }

        public virtual void ToBBuffer(BBuffer bb)
        {
            bb.Write(this.id);
            bb.Write(this.name);
            bb.Write(this.type_name);
            bb.Write(this.enabled);
        }

        public virtual void FromBBuffer(BBuffer bb)
        {
            bb.readLengthLimit = 0;
            bb.Read(ref this.id);
            bb.readLengthLimit = 0;
            bb.Read(ref this.name);
            bb.readLengthLimit = 0;
            bb.Read(ref this.type_name);
            bb.readLengthLimit = 0;
            bb.Read(ref this.enabled);
        }
        public virtual void ToString(ref System.Text.StringBuilder str)
        {
            if (GetToStringFlag())
            {
        	    str.Append("[ \"***** recursived *****\" ]");
        	    return;
            }
            else SetToStringFlag(true);

            str.Append("{ \"pkgTypeName\":\"Tables.game\", \"pkgTypeId\":" + GetPackageId());
            ToStringCore(ref str);
            str.Append(" }");
        
            SetToStringFlag(false);
        }
        public virtual void ToStringCore(ref System.Text.StringBuilder str)
        {
            if (id != null) str.Append(", \"id\":\"" + id + "\"");
            else str.Append(", \"id\":nil");
            if (name != null) str.Append(", \"name\":\"" + name + "\"");
            else str.Append(", \"name\":nil");
            if (type_name != null) str.Append(", \"type_name\":\"" + type_name + "\"");
            else str.Append(", \"type_name\":nil");
            if (enabled != null) str.Append(", \"enabled\":\"" + enabled + "\"");
            else str.Append(", \"enabled\":nil");
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
    public partial class game_catchfish : IBBuffer
    {
        public string id;
        public string exchange_coin_ratio;

        public virtual ushort GetPackageId()
        {
            return TypeIdMaps<game_catchfish>.typeId;
        }

        public virtual void ToBBuffer(BBuffer bb)
        {
            bb.Write(this.id);
            bb.Write(this.exchange_coin_ratio);
        }

        public virtual void FromBBuffer(BBuffer bb)
        {
            bb.readLengthLimit = 0;
            bb.Read(ref this.id);
            bb.readLengthLimit = 0;
            bb.Read(ref this.exchange_coin_ratio);
        }
        public virtual void ToString(ref System.Text.StringBuilder str)
        {
            if (GetToStringFlag())
            {
        	    str.Append("[ \"***** recursived *****\" ]");
        	    return;
            }
            else SetToStringFlag(true);

            str.Append("{ \"pkgTypeName\":\"Tables.game_catchfish\", \"pkgTypeId\":" + GetPackageId());
            ToStringCore(ref str);
            str.Append(" }");
        
            SetToStringFlag(false);
        }
        public virtual void ToStringCore(ref System.Text.StringBuilder str)
        {
            if (id != null) str.Append(", \"id\":\"" + id + "\"");
            else str.Append(", \"id\":nil");
            if (exchange_coin_ratio != null) str.Append(", \"exchange_coin_ratio\":\"" + exchange_coin_ratio + "\"");
            else str.Append(", \"exchange_coin_ratio\":nil");
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
    public partial class game_catchfish_account : IBBuffer
    {
        public string account_id;
        public string game_id;
        public string level_id;
        public string last_bet;
        public string last_cannon_id;

        public virtual ushort GetPackageId()
        {
            return TypeIdMaps<game_catchfish_account>.typeId;
        }

        public virtual void ToBBuffer(BBuffer bb)
        {
            bb.Write(this.account_id);
            bb.Write(this.game_id);
            bb.Write(this.level_id);
            bb.Write(this.last_bet);
            bb.Write(this.last_cannon_id);
        }

        public virtual void FromBBuffer(BBuffer bb)
        {
            bb.readLengthLimit = 0;
            bb.Read(ref this.account_id);
            bb.readLengthLimit = 0;
            bb.Read(ref this.game_id);
            bb.readLengthLimit = 0;
            bb.Read(ref this.level_id);
            bb.readLengthLimit = 0;
            bb.Read(ref this.last_bet);
            bb.readLengthLimit = 0;
            bb.Read(ref this.last_cannon_id);
        }
        public virtual void ToString(ref System.Text.StringBuilder str)
        {
            if (GetToStringFlag())
            {
        	    str.Append("[ \"***** recursived *****\" ]");
        	    return;
            }
            else SetToStringFlag(true);

            str.Append("{ \"pkgTypeName\":\"Tables.game_catchfish_account\", \"pkgTypeId\":" + GetPackageId());
            ToStringCore(ref str);
            str.Append(" }");
        
            SetToStringFlag(false);
        }
        public virtual void ToStringCore(ref System.Text.StringBuilder str)
        {
            if (account_id != null) str.Append(", \"account_id\":\"" + account_id + "\"");
            else str.Append(", \"account_id\":nil");
            if (game_id != null) str.Append(", \"game_id\":\"" + game_id + "\"");
            else str.Append(", \"game_id\":nil");
            if (level_id != null) str.Append(", \"level_id\":\"" + level_id + "\"");
            else str.Append(", \"level_id\":nil");
            if (last_bet != null) str.Append(", \"last_bet\":\"" + last_bet + "\"");
            else str.Append(", \"last_bet\":nil");
            if (last_cannon_id != null) str.Append(", \"last_cannon_id\":\"" + last_cannon_id + "\"");
            else str.Append(", \"last_cannon_id\":nil");
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
    public partial class game_catchfish_cannon : IBBuffer
    {
        public string id;
        public string vip_id;

        public virtual ushort GetPackageId()
        {
            return TypeIdMaps<game_catchfish_cannon>.typeId;
        }

        public virtual void ToBBuffer(BBuffer bb)
        {
            bb.Write(this.id);
            bb.Write(this.vip_id);
        }

        public virtual void FromBBuffer(BBuffer bb)
        {
            bb.readLengthLimit = 0;
            bb.Read(ref this.id);
            bb.readLengthLimit = 0;
            bb.Read(ref this.vip_id);
        }
        public virtual void ToString(ref System.Text.StringBuilder str)
        {
            if (GetToStringFlag())
            {
        	    str.Append("[ \"***** recursived *****\" ]");
        	    return;
            }
            else SetToStringFlag(true);

            str.Append("{ \"pkgTypeName\":\"Tables.game_catchfish_cannon\", \"pkgTypeId\":" + GetPackageId());
            ToStringCore(ref str);
            str.Append(" }");
        
            SetToStringFlag(false);
        }
        public virtual void ToStringCore(ref System.Text.StringBuilder str)
        {
            if (id != null) str.Append(", \"id\":\"" + id + "\"");
            else str.Append(", \"id\":nil");
            if (vip_id != null) str.Append(", \"vip_id\":\"" + vip_id + "\"");
            else str.Append(", \"vip_id\":nil");
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
    public partial class game_catchfish_level : IBBuffer
    {
        public string game_id;
        public string level_id;
        public string min_bet;
        public string max_bet;
        public string min_money;
        public string enabled;

        public virtual ushort GetPackageId()
        {
            return TypeIdMaps<game_catchfish_level>.typeId;
        }

        public virtual void ToBBuffer(BBuffer bb)
        {
            bb.Write(this.game_id);
            bb.Write(this.level_id);
            bb.Write(this.min_bet);
            bb.Write(this.max_bet);
            bb.Write(this.min_money);
            bb.Write(this.enabled);
        }

        public virtual void FromBBuffer(BBuffer bb)
        {
            bb.readLengthLimit = 0;
            bb.Read(ref this.game_id);
            bb.readLengthLimit = 0;
            bb.Read(ref this.level_id);
            bb.readLengthLimit = 0;
            bb.Read(ref this.min_bet);
            bb.readLengthLimit = 0;
            bb.Read(ref this.max_bet);
            bb.readLengthLimit = 0;
            bb.Read(ref this.min_money);
            bb.readLengthLimit = 0;
            bb.Read(ref this.enabled);
        }
        public virtual void ToString(ref System.Text.StringBuilder str)
        {
            if (GetToStringFlag())
            {
        	    str.Append("[ \"***** recursived *****\" ]");
        	    return;
            }
            else SetToStringFlag(true);

            str.Append("{ \"pkgTypeName\":\"Tables.game_catchfish_level\", \"pkgTypeId\":" + GetPackageId());
            ToStringCore(ref str);
            str.Append(" }");
        
            SetToStringFlag(false);
        }
        public virtual void ToStringCore(ref System.Text.StringBuilder str)
        {
            if (game_id != null) str.Append(", \"game_id\":\"" + game_id + "\"");
            else str.Append(", \"game_id\":nil");
            if (level_id != null) str.Append(", \"level_id\":\"" + level_id + "\"");
            else str.Append(", \"level_id\":nil");
            if (min_bet != null) str.Append(", \"min_bet\":\"" + min_bet + "\"");
            else str.Append(", \"min_bet\":nil");
            if (max_bet != null) str.Append(", \"max_bet\":\"" + max_bet + "\"");
            else str.Append(", \"max_bet\":nil");
            if (min_money != null) str.Append(", \"min_money\":\"" + min_money + "\"");
            else str.Append(", \"min_money\":nil");
            if (enabled != null) str.Append(", \"enabled\":\"" + enabled + "\"");
            else str.Append(", \"enabled\":nil");
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
    public partial class game_catchfish_level_room : IBBuffer
    {
        public string game_id;
        public string level_id;
        public string room_id;
        public string enabled;

        public virtual ushort GetPackageId()
        {
            return TypeIdMaps<game_catchfish_level_room>.typeId;
        }

        public virtual void ToBBuffer(BBuffer bb)
        {
            bb.Write(this.game_id);
            bb.Write(this.level_id);
            bb.Write(this.room_id);
            bb.Write(this.enabled);
        }

        public virtual void FromBBuffer(BBuffer bb)
        {
            bb.readLengthLimit = 0;
            bb.Read(ref this.game_id);
            bb.readLengthLimit = 0;
            bb.Read(ref this.level_id);
            bb.readLengthLimit = 0;
            bb.Read(ref this.room_id);
            bb.readLengthLimit = 0;
            bb.Read(ref this.enabled);
        }
        public virtual void ToString(ref System.Text.StringBuilder str)
        {
            if (GetToStringFlag())
            {
        	    str.Append("[ \"***** recursived *****\" ]");
        	    return;
            }
            else SetToStringFlag(true);

            str.Append("{ \"pkgTypeName\":\"Tables.game_catchfish_level_room\", \"pkgTypeId\":" + GetPackageId());
            ToStringCore(ref str);
            str.Append(" }");
        
            SetToStringFlag(false);
        }
        public virtual void ToStringCore(ref System.Text.StringBuilder str)
        {
            if (game_id != null) str.Append(", \"game_id\":\"" + game_id + "\"");
            else str.Append(", \"game_id\":nil");
            if (level_id != null) str.Append(", \"level_id\":\"" + level_id + "\"");
            else str.Append(", \"level_id\":nil");
            if (room_id != null) str.Append(", \"room_id\":\"" + room_id + "\"");
            else str.Append(", \"room_id\":nil");
            if (enabled != null) str.Append(", \"enabled\":\"" + enabled + "\"");
            else str.Append(", \"enabled\":nil");
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
    public partial class log_account_avatar : IBBuffer
    {
        public string id;
        public string account_id;
        public string avatar_id;
        public string create_time;

        public virtual ushort GetPackageId()
        {
            return TypeIdMaps<log_account_avatar>.typeId;
        }

        public virtual void ToBBuffer(BBuffer bb)
        {
            bb.Write(this.id);
            bb.Write(this.account_id);
            bb.Write(this.avatar_id);
            bb.Write(this.create_time);
        }

        public virtual void FromBBuffer(BBuffer bb)
        {
            bb.readLengthLimit = 0;
            bb.Read(ref this.id);
            bb.readLengthLimit = 0;
            bb.Read(ref this.account_id);
            bb.readLengthLimit = 0;
            bb.Read(ref this.avatar_id);
            bb.readLengthLimit = 0;
            bb.Read(ref this.create_time);
        }
        public virtual void ToString(ref System.Text.StringBuilder str)
        {
            if (GetToStringFlag())
            {
        	    str.Append("[ \"***** recursived *****\" ]");
        	    return;
            }
            else SetToStringFlag(true);

            str.Append("{ \"pkgTypeName\":\"Tables.log_account_avatar\", \"pkgTypeId\":" + GetPackageId());
            ToStringCore(ref str);
            str.Append(" }");
        
            SetToStringFlag(false);
        }
        public virtual void ToStringCore(ref System.Text.StringBuilder str)
        {
            if (id != null) str.Append(", \"id\":\"" + id + "\"");
            else str.Append(", \"id\":nil");
            if (account_id != null) str.Append(", \"account_id\":\"" + account_id + "\"");
            else str.Append(", \"account_id\":nil");
            if (avatar_id != null) str.Append(", \"avatar_id\":\"" + avatar_id + "\"");
            else str.Append(", \"avatar_id\":nil");
            if (create_time != null) str.Append(", \"create_time\":\"" + create_time + "\"");
            else str.Append(", \"create_time\":nil");
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
    public partial class log_account_enabled : IBBuffer
    {
        public string id;
        public string account_id;
        public string enabled;
        public string memo;
        public string create_time;

        public virtual ushort GetPackageId()
        {
            return TypeIdMaps<log_account_enabled>.typeId;
        }

        public virtual void ToBBuffer(BBuffer bb)
        {
            bb.Write(this.id);
            bb.Write(this.account_id);
            bb.Write(this.enabled);
            bb.Write(this.memo);
            bb.Write(this.create_time);
        }

        public virtual void FromBBuffer(BBuffer bb)
        {
            bb.readLengthLimit = 0;
            bb.Read(ref this.id);
            bb.readLengthLimit = 0;
            bb.Read(ref this.account_id);
            bb.readLengthLimit = 0;
            bb.Read(ref this.enabled);
            bb.readLengthLimit = 0;
            bb.Read(ref this.memo);
            bb.readLengthLimit = 0;
            bb.Read(ref this.create_time);
        }
        public virtual void ToString(ref System.Text.StringBuilder str)
        {
            if (GetToStringFlag())
            {
        	    str.Append("[ \"***** recursived *****\" ]");
        	    return;
            }
            else SetToStringFlag(true);

            str.Append("{ \"pkgTypeName\":\"Tables.log_account_enabled\", \"pkgTypeId\":" + GetPackageId());
            ToStringCore(ref str);
            str.Append(" }");
        
            SetToStringFlag(false);
        }
        public virtual void ToStringCore(ref System.Text.StringBuilder str)
        {
            if (id != null) str.Append(", \"id\":\"" + id + "\"");
            else str.Append(", \"id\":nil");
            if (account_id != null) str.Append(", \"account_id\":\"" + account_id + "\"");
            else str.Append(", \"account_id\":nil");
            if (enabled != null) str.Append(", \"enabled\":\"" + enabled + "\"");
            else str.Append(", \"enabled\":nil");
            if (memo != null) str.Append(", \"memo\":\"" + memo + "\"");
            else str.Append(", \"memo\":nil");
            if (create_time != null) str.Append(", \"create_time\":\"" + create_time + "\"");
            else str.Append(", \"create_time\":nil");
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
    public partial class log_account_money_safe : IBBuffer
    {
        public string id;
        public string account_id;
        public string value;
        public string create_time;

        public virtual ushort GetPackageId()
        {
            return TypeIdMaps<log_account_money_safe>.typeId;
        }

        public virtual void ToBBuffer(BBuffer bb)
        {
            bb.Write(this.id);
            bb.Write(this.account_id);
            bb.Write(this.value);
            bb.Write(this.create_time);
        }

        public virtual void FromBBuffer(BBuffer bb)
        {
            bb.readLengthLimit = 0;
            bb.Read(ref this.id);
            bb.readLengthLimit = 0;
            bb.Read(ref this.account_id);
            bb.readLengthLimit = 0;
            bb.Read(ref this.value);
            bb.readLengthLimit = 0;
            bb.Read(ref this.create_time);
        }
        public virtual void ToString(ref System.Text.StringBuilder str)
        {
            if (GetToStringFlag())
            {
        	    str.Append("[ \"***** recursived *****\" ]");
        	    return;
            }
            else SetToStringFlag(true);

            str.Append("{ \"pkgTypeName\":\"Tables.log_account_money_safe\", \"pkgTypeId\":" + GetPackageId());
            ToStringCore(ref str);
            str.Append(" }");
        
            SetToStringFlag(false);
        }
        public virtual void ToStringCore(ref System.Text.StringBuilder str)
        {
            if (id != null) str.Append(", \"id\":\"" + id + "\"");
            else str.Append(", \"id\":nil");
            if (account_id != null) str.Append(", \"account_id\":\"" + account_id + "\"");
            else str.Append(", \"account_id\":nil");
            if (value != null) str.Append(", \"value\":\"" + value + "\"");
            else str.Append(", \"value\":nil");
            if (create_time != null) str.Append(", \"create_time\":\"" + create_time + "\"");
            else str.Append(", \"create_time\":nil");
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
    public partial class log_account_nickname : IBBuffer
    {
        public string id;
        public string account_id;
        public string nickname;
        public string create_time;

        public virtual ushort GetPackageId()
        {
            return TypeIdMaps<log_account_nickname>.typeId;
        }

        public virtual void ToBBuffer(BBuffer bb)
        {
            bb.Write(this.id);
            bb.Write(this.account_id);
            bb.Write(this.nickname);
            bb.Write(this.create_time);
        }

        public virtual void FromBBuffer(BBuffer bb)
        {
            bb.readLengthLimit = 0;
            bb.Read(ref this.id);
            bb.readLengthLimit = 0;
            bb.Read(ref this.account_id);
            bb.readLengthLimit = 0;
            bb.Read(ref this.nickname);
            bb.readLengthLimit = 0;
            bb.Read(ref this.create_time);
        }
        public virtual void ToString(ref System.Text.StringBuilder str)
        {
            if (GetToStringFlag())
            {
        	    str.Append("[ \"***** recursived *****\" ]");
        	    return;
            }
            else SetToStringFlag(true);

            str.Append("{ \"pkgTypeName\":\"Tables.log_account_nickname\", \"pkgTypeId\":" + GetPackageId());
            ToStringCore(ref str);
            str.Append(" }");
        
            SetToStringFlag(false);
        }
        public virtual void ToStringCore(ref System.Text.StringBuilder str)
        {
            if (id != null) str.Append(", \"id\":\"" + id + "\"");
            else str.Append(", \"id\":nil");
            if (account_id != null) str.Append(", \"account_id\":\"" + account_id + "\"");
            else str.Append(", \"account_id\":nil");
            if (nickname != null) str.Append(", \"nickname\":\"" + nickname + "\"");
            else str.Append(", \"nickname\":nil");
            if (create_time != null) str.Append(", \"create_time\":\"" + create_time + "\"");
            else str.Append(", \"create_time\":nil");
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
    public partial class log_account_password : IBBuffer
    {
        public string id;
        public string account_id;
        public string password;
        public string create_time;

        public virtual ushort GetPackageId()
        {
            return TypeIdMaps<log_account_password>.typeId;
        }

        public virtual void ToBBuffer(BBuffer bb)
        {
            bb.Write(this.id);
            bb.Write(this.account_id);
            bb.Write(this.password);
            bb.Write(this.create_time);
        }

        public virtual void FromBBuffer(BBuffer bb)
        {
            bb.readLengthLimit = 0;
            bb.Read(ref this.id);
            bb.readLengthLimit = 0;
            bb.Read(ref this.account_id);
            bb.readLengthLimit = 0;
            bb.Read(ref this.password);
            bb.readLengthLimit = 0;
            bb.Read(ref this.create_time);
        }
        public virtual void ToString(ref System.Text.StringBuilder str)
        {
            if (GetToStringFlag())
            {
        	    str.Append("[ \"***** recursived *****\" ]");
        	    return;
            }
            else SetToStringFlag(true);

            str.Append("{ \"pkgTypeName\":\"Tables.log_account_password\", \"pkgTypeId\":" + GetPackageId());
            ToStringCore(ref str);
            str.Append(" }");
        
            SetToStringFlag(false);
        }
        public virtual void ToStringCore(ref System.Text.StringBuilder str)
        {
            if (id != null) str.Append(", \"id\":\"" + id + "\"");
            else str.Append(", \"id\":nil");
            if (account_id != null) str.Append(", \"account_id\":\"" + account_id + "\"");
            else str.Append(", \"account_id\":nil");
            if (password != null) str.Append(", \"password\":\"" + password + "\"");
            else str.Append(", \"password\":nil");
            if (create_time != null) str.Append(", \"create_time\":\"" + create_time + "\"");
            else str.Append(", \"create_time\":nil");
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
    public partial class log_account_phone : IBBuffer
    {
        public string id;
        public string account_id;
        public string phone;
        public string create_time;

        public virtual ushort GetPackageId()
        {
            return TypeIdMaps<log_account_phone>.typeId;
        }

        public virtual void ToBBuffer(BBuffer bb)
        {
            bb.Write(this.id);
            bb.Write(this.account_id);
            bb.Write(this.phone);
            bb.Write(this.create_time);
        }

        public virtual void FromBBuffer(BBuffer bb)
        {
            bb.readLengthLimit = 0;
            bb.Read(ref this.id);
            bb.readLengthLimit = 0;
            bb.Read(ref this.account_id);
            bb.readLengthLimit = 0;
            bb.Read(ref this.phone);
            bb.readLengthLimit = 0;
            bb.Read(ref this.create_time);
        }
        public virtual void ToString(ref System.Text.StringBuilder str)
        {
            if (GetToStringFlag())
            {
        	    str.Append("[ \"***** recursived *****\" ]");
        	    return;
            }
            else SetToStringFlag(true);

            str.Append("{ \"pkgTypeName\":\"Tables.log_account_phone\", \"pkgTypeId\":" + GetPackageId());
            ToStringCore(ref str);
            str.Append(" }");
        
            SetToStringFlag(false);
        }
        public virtual void ToStringCore(ref System.Text.StringBuilder str)
        {
            if (id != null) str.Append(", \"id\":\"" + id + "\"");
            else str.Append(", \"id\":nil");
            if (account_id != null) str.Append(", \"account_id\":\"" + account_id + "\"");
            else str.Append(", \"account_id\":nil");
            if (phone != null) str.Append(", \"phone\":\"" + phone + "\"");
            else str.Append(", \"phone\":nil");
            if (create_time != null) str.Append(", \"create_time\":\"" + create_time + "\"");
            else str.Append(", \"create_time\":nil");
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
    public partial class log_account_recharge : IBBuffer
    {
        public string token;
        public string account_id;
        public string money;
        public string memo;
        public string create_time;

        public virtual ushort GetPackageId()
        {
            return TypeIdMaps<log_account_recharge>.typeId;
        }

        public virtual void ToBBuffer(BBuffer bb)
        {
            bb.Write(this.token);
            bb.Write(this.account_id);
            bb.Write(this.money);
            bb.Write(this.memo);
            bb.Write(this.create_time);
        }

        public virtual void FromBBuffer(BBuffer bb)
        {
            bb.readLengthLimit = 0;
            bb.Read(ref this.token);
            bb.readLengthLimit = 0;
            bb.Read(ref this.account_id);
            bb.readLengthLimit = 0;
            bb.Read(ref this.money);
            bb.readLengthLimit = 0;
            bb.Read(ref this.memo);
            bb.readLengthLimit = 0;
            bb.Read(ref this.create_time);
        }
        public virtual void ToString(ref System.Text.StringBuilder str)
        {
            if (GetToStringFlag())
            {
        	    str.Append("[ \"***** recursived *****\" ]");
        	    return;
            }
            else SetToStringFlag(true);

            str.Append("{ \"pkgTypeName\":\"Tables.log_account_recharge\", \"pkgTypeId\":" + GetPackageId());
            ToStringCore(ref str);
            str.Append(" }");
        
            SetToStringFlag(false);
        }
        public virtual void ToStringCore(ref System.Text.StringBuilder str)
        {
            if (token != null) str.Append(", \"token\":\"" + token + "\"");
            else str.Append(", \"token\":nil");
            if (account_id != null) str.Append(", \"account_id\":\"" + account_id + "\"");
            else str.Append(", \"account_id\":nil");
            if (money != null) str.Append(", \"money\":\"" + money + "\"");
            else str.Append(", \"money\":nil");
            if (memo != null) str.Append(", \"memo\":\"" + memo + "\"");
            else str.Append(", \"memo\":nil");
            if (create_time != null) str.Append(", \"create_time\":\"" + create_time + "\"");
            else str.Append(", \"create_time\":nil");
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
    public partial class log_game : IBBuffer
    {
        public string id;
        public string game_id;

        public virtual ushort GetPackageId()
        {
            return TypeIdMaps<log_game>.typeId;
        }

        public virtual void ToBBuffer(BBuffer bb)
        {
            bb.Write(this.id);
            bb.Write(this.game_id);
        }

        public virtual void FromBBuffer(BBuffer bb)
        {
            bb.readLengthLimit = 0;
            bb.Read(ref this.id);
            bb.readLengthLimit = 0;
            bb.Read(ref this.game_id);
        }
        public virtual void ToString(ref System.Text.StringBuilder str)
        {
            if (GetToStringFlag())
            {
        	    str.Append("[ \"***** recursived *****\" ]");
        	    return;
            }
            else SetToStringFlag(true);

            str.Append("{ \"pkgTypeName\":\"Tables.log_game\", \"pkgTypeId\":" + GetPackageId());
            ToStringCore(ref str);
            str.Append(" }");
        
            SetToStringFlag(false);
        }
        public virtual void ToStringCore(ref System.Text.StringBuilder str)
        {
            if (id != null) str.Append(", \"id\":\"" + id + "\"");
            else str.Append(", \"id\":nil");
            if (game_id != null) str.Append(", \"game_id\":\"" + game_id + "\"");
            else str.Append(", \"game_id\":nil");
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
    public partial class log_game_catchfish : IBBuffer
    {
        public string id;
        public string level_id;
        public string room_id;
        public string account_id;
        public string enter_time;
        public string enter_money;
        public string leave_time;
        public string leave_money;
        public string consume;

        public virtual ushort GetPackageId()
        {
            return TypeIdMaps<log_game_catchfish>.typeId;
        }

        public virtual void ToBBuffer(BBuffer bb)
        {
            bb.Write(this.id);
            bb.Write(this.level_id);
            bb.Write(this.room_id);
            bb.Write(this.account_id);
            bb.Write(this.enter_time);
            bb.Write(this.enter_money);
            bb.Write(this.leave_time);
            bb.Write(this.leave_money);
            bb.Write(this.consume);
        }

        public virtual void FromBBuffer(BBuffer bb)
        {
            bb.readLengthLimit = 0;
            bb.Read(ref this.id);
            bb.readLengthLimit = 0;
            bb.Read(ref this.level_id);
            bb.readLengthLimit = 0;
            bb.Read(ref this.room_id);
            bb.readLengthLimit = 0;
            bb.Read(ref this.account_id);
            bb.readLengthLimit = 0;
            bb.Read(ref this.enter_time);
            bb.readLengthLimit = 0;
            bb.Read(ref this.enter_money);
            bb.readLengthLimit = 0;
            bb.Read(ref this.leave_time);
            bb.readLengthLimit = 0;
            bb.Read(ref this.leave_money);
            bb.readLengthLimit = 0;
            bb.Read(ref this.consume);
        }
        public virtual void ToString(ref System.Text.StringBuilder str)
        {
            if (GetToStringFlag())
            {
        	    str.Append("[ \"***** recursived *****\" ]");
        	    return;
            }
            else SetToStringFlag(true);

            str.Append("{ \"pkgTypeName\":\"Tables.log_game_catchfish\", \"pkgTypeId\":" + GetPackageId());
            ToStringCore(ref str);
            str.Append(" }");
        
            SetToStringFlag(false);
        }
        public virtual void ToStringCore(ref System.Text.StringBuilder str)
        {
            if (id != null) str.Append(", \"id\":\"" + id + "\"");
            else str.Append(", \"id\":nil");
            if (level_id != null) str.Append(", \"level_id\":\"" + level_id + "\"");
            else str.Append(", \"level_id\":nil");
            if (room_id != null) str.Append(", \"room_id\":\"" + room_id + "\"");
            else str.Append(", \"room_id\":nil");
            if (account_id != null) str.Append(", \"account_id\":\"" + account_id + "\"");
            else str.Append(", \"account_id\":nil");
            if (enter_time != null) str.Append(", \"enter_time\":\"" + enter_time + "\"");
            else str.Append(", \"enter_time\":nil");
            if (enter_money != null) str.Append(", \"enter_money\":\"" + enter_money + "\"");
            else str.Append(", \"enter_money\":nil");
            if (leave_time != null) str.Append(", \"leave_time\":\"" + leave_time + "\"");
            else str.Append(", \"leave_time\":nil");
            if (leave_money != null) str.Append(", \"leave_money\":\"" + leave_money + "\"");
            else str.Append(", \"leave_money\":nil");
            if (consume != null) str.Append(", \"consume\":\"" + consume + "\"");
            else str.Append(", \"consume\":nil");
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
    public partial class log_game_catchfish_event : IBBuffer
    {
        public string id;
        public string log_game_catchfish_id;
        public string event_type_id;
        public string frame_number;
        public string create_time;

        public virtual ushort GetPackageId()
        {
            return TypeIdMaps<log_game_catchfish_event>.typeId;
        }

        public virtual void ToBBuffer(BBuffer bb)
        {
            bb.Write(this.id);
            bb.Write(this.log_game_catchfish_id);
            bb.Write(this.event_type_id);
            bb.Write(this.frame_number);
            bb.Write(this.create_time);
        }

        public virtual void FromBBuffer(BBuffer bb)
        {
            bb.readLengthLimit = 0;
            bb.Read(ref this.id);
            bb.readLengthLimit = 0;
            bb.Read(ref this.log_game_catchfish_id);
            bb.readLengthLimit = 0;
            bb.Read(ref this.event_type_id);
            bb.readLengthLimit = 0;
            bb.Read(ref this.frame_number);
            bb.readLengthLimit = 0;
            bb.Read(ref this.create_time);
        }
        public virtual void ToString(ref System.Text.StringBuilder str)
        {
            if (GetToStringFlag())
            {
        	    str.Append("[ \"***** recursived *****\" ]");
        	    return;
            }
            else SetToStringFlag(true);

            str.Append("{ \"pkgTypeName\":\"Tables.log_game_catchfish_event\", \"pkgTypeId\":" + GetPackageId());
            ToStringCore(ref str);
            str.Append(" }");
        
            SetToStringFlag(false);
        }
        public virtual void ToStringCore(ref System.Text.StringBuilder str)
        {
            if (id != null) str.Append(", \"id\":\"" + id + "\"");
            else str.Append(", \"id\":nil");
            if (log_game_catchfish_id != null) str.Append(", \"log_game_catchfish_id\":\"" + log_game_catchfish_id + "\"");
            else str.Append(", \"log_game_catchfish_id\":nil");
            if (event_type_id != null) str.Append(", \"event_type_id\":\"" + event_type_id + "\"");
            else str.Append(", \"event_type_id\":nil");
            if (frame_number != null) str.Append(", \"frame_number\":\"" + frame_number + "\"");
            else str.Append(", \"frame_number\":nil");
            if (create_time != null) str.Append(", \"create_time\":\"" + create_time + "\"");
            else str.Append(", \"create_time\":nil");
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
    public partial class log_game_catchfish_event_bulletdie : IBBuffer
    {
        public string id;
        public string log_game_catchfish_id;
        public string type_id;
        public string bullet_id;
        public string coin;
        public string money;

        public virtual ushort GetPackageId()
        {
            return TypeIdMaps<log_game_catchfish_event_bulletdie>.typeId;
        }

        public virtual void ToBBuffer(BBuffer bb)
        {
            bb.Write(this.id);
            bb.Write(this.log_game_catchfish_id);
            bb.Write(this.type_id);
            bb.Write(this.bullet_id);
            bb.Write(this.coin);
            bb.Write(this.money);
        }

        public virtual void FromBBuffer(BBuffer bb)
        {
            bb.readLengthLimit = 0;
            bb.Read(ref this.id);
            bb.readLengthLimit = 0;
            bb.Read(ref this.log_game_catchfish_id);
            bb.readLengthLimit = 0;
            bb.Read(ref this.type_id);
            bb.readLengthLimit = 0;
            bb.Read(ref this.bullet_id);
            bb.readLengthLimit = 0;
            bb.Read(ref this.coin);
            bb.readLengthLimit = 0;
            bb.Read(ref this.money);
        }
        public virtual void ToString(ref System.Text.StringBuilder str)
        {
            if (GetToStringFlag())
            {
        	    str.Append("[ \"***** recursived *****\" ]");
        	    return;
            }
            else SetToStringFlag(true);

            str.Append("{ \"pkgTypeName\":\"Tables.log_game_catchfish_event_bulletdie\", \"pkgTypeId\":" + GetPackageId());
            ToStringCore(ref str);
            str.Append(" }");
        
            SetToStringFlag(false);
        }
        public virtual void ToStringCore(ref System.Text.StringBuilder str)
        {
            if (id != null) str.Append(", \"id\":\"" + id + "\"");
            else str.Append(", \"id\":nil");
            if (log_game_catchfish_id != null) str.Append(", \"log_game_catchfish_id\":\"" + log_game_catchfish_id + "\"");
            else str.Append(", \"log_game_catchfish_id\":nil");
            if (type_id != null) str.Append(", \"type_id\":\"" + type_id + "\"");
            else str.Append(", \"type_id\":nil");
            if (bullet_id != null) str.Append(", \"bullet_id\":\"" + bullet_id + "\"");
            else str.Append(", \"bullet_id\":nil");
            if (coin != null) str.Append(", \"coin\":\"" + coin + "\"");
            else str.Append(", \"coin\":nil");
            if (money != null) str.Append(", \"money\":\"" + money + "\"");
            else str.Append(", \"money\":nil");
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
    public partial class log_game_catchfish_event_bulletmiss : IBBuffer
    {
        public string id;
        public string log_game_catchfish_id;
        public string type_id;
        public string bullet_id;
        public string coin;
        public string money;

        public virtual ushort GetPackageId()
        {
            return TypeIdMaps<log_game_catchfish_event_bulletmiss>.typeId;
        }

        public virtual void ToBBuffer(BBuffer bb)
        {
            bb.Write(this.id);
            bb.Write(this.log_game_catchfish_id);
            bb.Write(this.type_id);
            bb.Write(this.bullet_id);
            bb.Write(this.coin);
            bb.Write(this.money);
        }

        public virtual void FromBBuffer(BBuffer bb)
        {
            bb.readLengthLimit = 0;
            bb.Read(ref this.id);
            bb.readLengthLimit = 0;
            bb.Read(ref this.log_game_catchfish_id);
            bb.readLengthLimit = 0;
            bb.Read(ref this.type_id);
            bb.readLengthLimit = 0;
            bb.Read(ref this.bullet_id);
            bb.readLengthLimit = 0;
            bb.Read(ref this.coin);
            bb.readLengthLimit = 0;
            bb.Read(ref this.money);
        }
        public virtual void ToString(ref System.Text.StringBuilder str)
        {
            if (GetToStringFlag())
            {
        	    str.Append("[ \"***** recursived *****\" ]");
        	    return;
            }
            else SetToStringFlag(true);

            str.Append("{ \"pkgTypeName\":\"Tables.log_game_catchfish_event_bulletmiss\", \"pkgTypeId\":" + GetPackageId());
            ToStringCore(ref str);
            str.Append(" }");
        
            SetToStringFlag(false);
        }
        public virtual void ToStringCore(ref System.Text.StringBuilder str)
        {
            if (id != null) str.Append(", \"id\":\"" + id + "\"");
            else str.Append(", \"id\":nil");
            if (log_game_catchfish_id != null) str.Append(", \"log_game_catchfish_id\":\"" + log_game_catchfish_id + "\"");
            else str.Append(", \"log_game_catchfish_id\":nil");
            if (type_id != null) str.Append(", \"type_id\":\"" + type_id + "\"");
            else str.Append(", \"type_id\":nil");
            if (bullet_id != null) str.Append(", \"bullet_id\":\"" + bullet_id + "\"");
            else str.Append(", \"bullet_id\":nil");
            if (coin != null) str.Append(", \"coin\":\"" + coin + "\"");
            else str.Append(", \"coin\":nil");
            if (money != null) str.Append(", \"money\":\"" + money + "\"");
            else str.Append(", \"money\":nil");
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
    public partial class log_game_catchfish_event_fire : IBBuffer
    {
        public string id;
        public string log_game_catchfish_id;
        public string type_id;
        public string bullet_id;
        public string coin;
        public string money;

        public virtual ushort GetPackageId()
        {
            return TypeIdMaps<log_game_catchfish_event_fire>.typeId;
        }

        public virtual void ToBBuffer(BBuffer bb)
        {
            bb.Write(this.id);
            bb.Write(this.log_game_catchfish_id);
            bb.Write(this.type_id);
            bb.Write(this.bullet_id);
            bb.Write(this.coin);
            bb.Write(this.money);
        }

        public virtual void FromBBuffer(BBuffer bb)
        {
            bb.readLengthLimit = 0;
            bb.Read(ref this.id);
            bb.readLengthLimit = 0;
            bb.Read(ref this.log_game_catchfish_id);
            bb.readLengthLimit = 0;
            bb.Read(ref this.type_id);
            bb.readLengthLimit = 0;
            bb.Read(ref this.bullet_id);
            bb.readLengthLimit = 0;
            bb.Read(ref this.coin);
            bb.readLengthLimit = 0;
            bb.Read(ref this.money);
        }
        public virtual void ToString(ref System.Text.StringBuilder str)
        {
            if (GetToStringFlag())
            {
        	    str.Append("[ \"***** recursived *****\" ]");
        	    return;
            }
            else SetToStringFlag(true);

            str.Append("{ \"pkgTypeName\":\"Tables.log_game_catchfish_event_fire\", \"pkgTypeId\":" + GetPackageId());
            ToStringCore(ref str);
            str.Append(" }");
        
            SetToStringFlag(false);
        }
        public virtual void ToStringCore(ref System.Text.StringBuilder str)
        {
            if (id != null) str.Append(", \"id\":\"" + id + "\"");
            else str.Append(", \"id\":nil");
            if (log_game_catchfish_id != null) str.Append(", \"log_game_catchfish_id\":\"" + log_game_catchfish_id + "\"");
            else str.Append(", \"log_game_catchfish_id\":nil");
            if (type_id != null) str.Append(", \"type_id\":\"" + type_id + "\"");
            else str.Append(", \"type_id\":nil");
            if (bullet_id != null) str.Append(", \"bullet_id\":\"" + bullet_id + "\"");
            else str.Append(", \"bullet_id\":nil");
            if (coin != null) str.Append(", \"coin\":\"" + coin + "\"");
            else str.Append(", \"coin\":nil");
            if (money != null) str.Append(", \"money\":\"" + money + "\"");
            else str.Append(", \"money\":nil");
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
    public partial class log_game_catchfish_event_fishdie : IBBuffer
    {
        public string id;
        public string log_game_catchfish_id;
        public string fish_type_id;
        public string fish_id;
        public string fish_ratio;
        public string bullet_coin;
        public string coin;
        public string money;

        public virtual ushort GetPackageId()
        {
            return TypeIdMaps<log_game_catchfish_event_fishdie>.typeId;
        }

        public virtual void ToBBuffer(BBuffer bb)
        {
            bb.Write(this.id);
            bb.Write(this.log_game_catchfish_id);
            bb.Write(this.fish_type_id);
            bb.Write(this.fish_id);
            bb.Write(this.fish_ratio);
            bb.Write(this.bullet_coin);
            bb.Write(this.coin);
            bb.Write(this.money);
        }

        public virtual void FromBBuffer(BBuffer bb)
        {
            bb.readLengthLimit = 0;
            bb.Read(ref this.id);
            bb.readLengthLimit = 0;
            bb.Read(ref this.log_game_catchfish_id);
            bb.readLengthLimit = 0;
            bb.Read(ref this.fish_type_id);
            bb.readLengthLimit = 0;
            bb.Read(ref this.fish_id);
            bb.readLengthLimit = 0;
            bb.Read(ref this.fish_ratio);
            bb.readLengthLimit = 0;
            bb.Read(ref this.bullet_coin);
            bb.readLengthLimit = 0;
            bb.Read(ref this.coin);
            bb.readLengthLimit = 0;
            bb.Read(ref this.money);
        }
        public virtual void ToString(ref System.Text.StringBuilder str)
        {
            if (GetToStringFlag())
            {
        	    str.Append("[ \"***** recursived *****\" ]");
        	    return;
            }
            else SetToStringFlag(true);

            str.Append("{ \"pkgTypeName\":\"Tables.log_game_catchfish_event_fishdie\", \"pkgTypeId\":" + GetPackageId());
            ToStringCore(ref str);
            str.Append(" }");
        
            SetToStringFlag(false);
        }
        public virtual void ToStringCore(ref System.Text.StringBuilder str)
        {
            if (id != null) str.Append(", \"id\":\"" + id + "\"");
            else str.Append(", \"id\":nil");
            if (log_game_catchfish_id != null) str.Append(", \"log_game_catchfish_id\":\"" + log_game_catchfish_id + "\"");
            else str.Append(", \"log_game_catchfish_id\":nil");
            if (fish_type_id != null) str.Append(", \"fish_type_id\":\"" + fish_type_id + "\"");
            else str.Append(", \"fish_type_id\":nil");
            if (fish_id != null) str.Append(", \"fish_id\":\"" + fish_id + "\"");
            else str.Append(", \"fish_id\":nil");
            if (fish_ratio != null) str.Append(", \"fish_ratio\":\"" + fish_ratio + "\"");
            else str.Append(", \"fish_ratio\":nil");
            if (bullet_coin != null) str.Append(", \"bullet_coin\":\"" + bullet_coin + "\"");
            else str.Append(", \"bullet_coin\":nil");
            if (coin != null) str.Append(", \"coin\":\"" + coin + "\"");
            else str.Append(", \"coin\":nil");
            if (money != null) str.Append(", \"money\":\"" + money + "\"");
            else str.Append(", \"money\":nil");
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
    public partial class log_login_out : IBBuffer
    {
        public string id;
        public string account_id;
        public string type_id;
        public string ip;
        public string create_time;

        public virtual ushort GetPackageId()
        {
            return TypeIdMaps<log_login_out>.typeId;
        }

        public virtual void ToBBuffer(BBuffer bb)
        {
            bb.Write(this.id);
            bb.Write(this.account_id);
            bb.Write(this.type_id);
            bb.Write(this.ip);
            bb.Write(this.create_time);
        }

        public virtual void FromBBuffer(BBuffer bb)
        {
            bb.readLengthLimit = 0;
            bb.Read(ref this.id);
            bb.readLengthLimit = 0;
            bb.Read(ref this.account_id);
            bb.readLengthLimit = 0;
            bb.Read(ref this.type_id);
            bb.readLengthLimit = 0;
            bb.Read(ref this.ip);
            bb.readLengthLimit = 0;
            bb.Read(ref this.create_time);
        }
        public virtual void ToString(ref System.Text.StringBuilder str)
        {
            if (GetToStringFlag())
            {
        	    str.Append("[ \"***** recursived *****\" ]");
        	    return;
            }
            else SetToStringFlag(true);

            str.Append("{ \"pkgTypeName\":\"Tables.log_login_out\", \"pkgTypeId\":" + GetPackageId());
            ToStringCore(ref str);
            str.Append(" }");
        
            SetToStringFlag(false);
        }
        public virtual void ToStringCore(ref System.Text.StringBuilder str)
        {
            if (id != null) str.Append(", \"id\":\"" + id + "\"");
            else str.Append(", \"id\":nil");
            if (account_id != null) str.Append(", \"account_id\":\"" + account_id + "\"");
            else str.Append(", \"account_id\":nil");
            if (type_id != null) str.Append(", \"type_id\":\"" + type_id + "\"");
            else str.Append(", \"type_id\":nil");
            if (ip != null) str.Append(", \"ip\":\"" + ip + "\"");
            else str.Append(", \"ip\":nil");
            if (create_time != null) str.Append(", \"create_time\":\"" + create_time + "\"");
            else str.Append(", \"create_time\":nil");
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
    public partial class log_withdrawals : IBBuffer
    {
        public string id;
        public string withdrawals_id;
        public string person_in_charge;
        public string state_id;
        public string description;
        public string create_time;

        public virtual ushort GetPackageId()
        {
            return TypeIdMaps<log_withdrawals>.typeId;
        }

        public virtual void ToBBuffer(BBuffer bb)
        {
            bb.Write(this.id);
            bb.Write(this.withdrawals_id);
            bb.Write(this.person_in_charge);
            bb.Write(this.state_id);
            bb.Write(this.description);
            bb.Write(this.create_time);
        }

        public virtual void FromBBuffer(BBuffer bb)
        {
            bb.readLengthLimit = 0;
            bb.Read(ref this.id);
            bb.readLengthLimit = 0;
            bb.Read(ref this.withdrawals_id);
            bb.readLengthLimit = 0;
            bb.Read(ref this.person_in_charge);
            bb.readLengthLimit = 0;
            bb.Read(ref this.state_id);
            bb.readLengthLimit = 0;
            bb.Read(ref this.description);
            bb.readLengthLimit = 0;
            bb.Read(ref this.create_time);
        }
        public virtual void ToString(ref System.Text.StringBuilder str)
        {
            if (GetToStringFlag())
            {
        	    str.Append("[ \"***** recursived *****\" ]");
        	    return;
            }
            else SetToStringFlag(true);

            str.Append("{ \"pkgTypeName\":\"Tables.log_withdrawals\", \"pkgTypeId\":" + GetPackageId());
            ToStringCore(ref str);
            str.Append(" }");
        
            SetToStringFlag(false);
        }
        public virtual void ToStringCore(ref System.Text.StringBuilder str)
        {
            if (id != null) str.Append(", \"id\":\"" + id + "\"");
            else str.Append(", \"id\":nil");
            if (withdrawals_id != null) str.Append(", \"withdrawals_id\":\"" + withdrawals_id + "\"");
            else str.Append(", \"withdrawals_id\":nil");
            if (person_in_charge != null) str.Append(", \"person_in_charge\":\"" + person_in_charge + "\"");
            else str.Append(", \"person_in_charge\":nil");
            if (state_id != null) str.Append(", \"state_id\":\"" + state_id + "\"");
            else str.Append(", \"state_id\":nil");
            if (description != null) str.Append(", \"description\":\"" + description + "\"");
            else str.Append(", \"description\":nil");
            if (create_time != null) str.Append(", \"create_time\":\"" + create_time + "\"");
            else str.Append(", \"create_time\":nil");
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
    public partial class mail : IBBuffer
    {
        public string id;
        public string content;
        public string account_id;
        public string sender;
        public string create_time;
        public string is_read;

        public virtual ushort GetPackageId()
        {
            return TypeIdMaps<mail>.typeId;
        }

        public virtual void ToBBuffer(BBuffer bb)
        {
            bb.Write(this.id);
            bb.Write(this.content);
            bb.Write(this.account_id);
            bb.Write(this.sender);
            bb.Write(this.create_time);
            bb.Write(this.is_read);
        }

        public virtual void FromBBuffer(BBuffer bb)
        {
            bb.readLengthLimit = 0;
            bb.Read(ref this.id);
            bb.readLengthLimit = 0;
            bb.Read(ref this.content);
            bb.readLengthLimit = 0;
            bb.Read(ref this.account_id);
            bb.readLengthLimit = 0;
            bb.Read(ref this.sender);
            bb.readLengthLimit = 0;
            bb.Read(ref this.create_time);
            bb.readLengthLimit = 0;
            bb.Read(ref this.is_read);
        }
        public virtual void ToString(ref System.Text.StringBuilder str)
        {
            if (GetToStringFlag())
            {
        	    str.Append("[ \"***** recursived *****\" ]");
        	    return;
            }
            else SetToStringFlag(true);

            str.Append("{ \"pkgTypeName\":\"Tables.mail\", \"pkgTypeId\":" + GetPackageId());
            ToStringCore(ref str);
            str.Append(" }");
        
            SetToStringFlag(false);
        }
        public virtual void ToStringCore(ref System.Text.StringBuilder str)
        {
            if (id != null) str.Append(", \"id\":\"" + id + "\"");
            else str.Append(", \"id\":nil");
            if (content != null) str.Append(", \"content\":\"" + content + "\"");
            else str.Append(", \"content\":nil");
            if (account_id != null) str.Append(", \"account_id\":\"" + account_id + "\"");
            else str.Append(", \"account_id\":nil");
            if (sender != null) str.Append(", \"sender\":\"" + sender + "\"");
            else str.Append(", \"sender\":nil");
            if (create_time != null) str.Append(", \"create_time\":\"" + create_time + "\"");
            else str.Append(", \"create_time\":nil");
            if (is_read != null) str.Append(", \"is_read\":\"" + is_read + "\"");
            else str.Append(", \"is_read\":nil");
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
    public partial class marquee : IBBuffer
    {
        public string id;
        public string content;
        public string frequency;
        public string update_time;
        public string create_time;
        public string enable;

        public virtual ushort GetPackageId()
        {
            return TypeIdMaps<marquee>.typeId;
        }

        public virtual void ToBBuffer(BBuffer bb)
        {
            bb.Write(this.id);
            bb.Write(this.content);
            bb.Write(this.frequency);
            bb.Write(this.update_time);
            bb.Write(this.create_time);
            bb.Write(this.enable);
        }

        public virtual void FromBBuffer(BBuffer bb)
        {
            bb.readLengthLimit = 0;
            bb.Read(ref this.id);
            bb.readLengthLimit = 0;
            bb.Read(ref this.content);
            bb.readLengthLimit = 0;
            bb.Read(ref this.frequency);
            bb.readLengthLimit = 0;
            bb.Read(ref this.update_time);
            bb.readLengthLimit = 0;
            bb.Read(ref this.create_time);
            bb.readLengthLimit = 0;
            bb.Read(ref this.enable);
        }
        public virtual void ToString(ref System.Text.StringBuilder str)
        {
            if (GetToStringFlag())
            {
        	    str.Append("[ \"***** recursived *****\" ]");
        	    return;
            }
            else SetToStringFlag(true);

            str.Append("{ \"pkgTypeName\":\"Tables.marquee\", \"pkgTypeId\":" + GetPackageId());
            ToStringCore(ref str);
            str.Append(" }");
        
            SetToStringFlag(false);
        }
        public virtual void ToStringCore(ref System.Text.StringBuilder str)
        {
            if (id != null) str.Append(", \"id\":\"" + id + "\"");
            else str.Append(", \"id\":nil");
            if (content != null) str.Append(", \"content\":\"" + content + "\"");
            else str.Append(", \"content\":nil");
            if (frequency != null) str.Append(", \"frequency\":\"" + frequency + "\"");
            else str.Append(", \"frequency\":nil");
            if (update_time != null) str.Append(", \"update_time\":\"" + update_time + "\"");
            else str.Append(", \"update_time\":nil");
            if (create_time != null) str.Append(", \"create_time\":\"" + create_time + "\"");
            else str.Append(", \"create_time\":nil");
            if (enable != null) str.Append(", \"enable\":\"" + enable + "\"");
            else str.Append(", \"enable\":nil");
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
    public partial class notice : IBBuffer
    {
        public string id;
        public string content;
        public string notice_type_id;
        public string start_time;
        public string end_time;
        public string create_time;

        public virtual ushort GetPackageId()
        {
            return TypeIdMaps<notice>.typeId;
        }

        public virtual void ToBBuffer(BBuffer bb)
        {
            bb.Write(this.id);
            bb.Write(this.content);
            bb.Write(this.notice_type_id);
            bb.Write(this.start_time);
            bb.Write(this.end_time);
            bb.Write(this.create_time);
        }

        public virtual void FromBBuffer(BBuffer bb)
        {
            bb.readLengthLimit = 0;
            bb.Read(ref this.id);
            bb.readLengthLimit = 0;
            bb.Read(ref this.content);
            bb.readLengthLimit = 0;
            bb.Read(ref this.notice_type_id);
            bb.readLengthLimit = 0;
            bb.Read(ref this.start_time);
            bb.readLengthLimit = 0;
            bb.Read(ref this.end_time);
            bb.readLengthLimit = 0;
            bb.Read(ref this.create_time);
        }
        public virtual void ToString(ref System.Text.StringBuilder str)
        {
            if (GetToStringFlag())
            {
        	    str.Append("[ \"***** recursived *****\" ]");
        	    return;
            }
            else SetToStringFlag(true);

            str.Append("{ \"pkgTypeName\":\"Tables.notice\", \"pkgTypeId\":" + GetPackageId());
            ToStringCore(ref str);
            str.Append(" }");
        
            SetToStringFlag(false);
        }
        public virtual void ToStringCore(ref System.Text.StringBuilder str)
        {
            if (id != null) str.Append(", \"id\":\"" + id + "\"");
            else str.Append(", \"id\":nil");
            if (content != null) str.Append(", \"content\":\"" + content + "\"");
            else str.Append(", \"content\":nil");
            if (notice_type_id != null) str.Append(", \"notice_type_id\":\"" + notice_type_id + "\"");
            else str.Append(", \"notice_type_id\":nil");
            if (start_time != null) str.Append(", \"start_time\":\"" + start_time + "\"");
            else str.Append(", \"start_time\":nil");
            if (end_time != null) str.Append(", \"end_time\":\"" + end_time + "\"");
            else str.Append(", \"end_time\":nil");
            if (create_time != null) str.Append(", \"create_time\":\"" + create_time + "\"");
            else str.Append(", \"create_time\":nil");
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
    public partial class notice_read : IBBuffer
    {
        public string notice_id;
        public string account_id;
        public string create_time;

        public virtual ushort GetPackageId()
        {
            return TypeIdMaps<notice_read>.typeId;
        }

        public virtual void ToBBuffer(BBuffer bb)
        {
            bb.Write(this.notice_id);
            bb.Write(this.account_id);
            bb.Write(this.create_time);
        }

        public virtual void FromBBuffer(BBuffer bb)
        {
            bb.readLengthLimit = 0;
            bb.Read(ref this.notice_id);
            bb.readLengthLimit = 0;
            bb.Read(ref this.account_id);
            bb.readLengthLimit = 0;
            bb.Read(ref this.create_time);
        }
        public virtual void ToString(ref System.Text.StringBuilder str)
        {
            if (GetToStringFlag())
            {
        	    str.Append("[ \"***** recursived *****\" ]");
        	    return;
            }
            else SetToStringFlag(true);

            str.Append("{ \"pkgTypeName\":\"Tables.notice_read\", \"pkgTypeId\":" + GetPackageId());
            ToStringCore(ref str);
            str.Append(" }");
        
            SetToStringFlag(false);
        }
        public virtual void ToStringCore(ref System.Text.StringBuilder str)
        {
            if (notice_id != null) str.Append(", \"notice_id\":\"" + notice_id + "\"");
            else str.Append(", \"notice_id\":nil");
            if (account_id != null) str.Append(", \"account_id\":\"" + account_id + "\"");
            else str.Append(", \"account_id\":nil");
            if (create_time != null) str.Append(", \"create_time\":\"" + create_time + "\"");
            else str.Append(", \"create_time\":nil");
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
    public partial class notice_type : IBBuffer
    {
        public string id;
        public string name;

        public virtual ushort GetPackageId()
        {
            return TypeIdMaps<notice_type>.typeId;
        }

        public virtual void ToBBuffer(BBuffer bb)
        {
            bb.Write(this.id);
            bb.Write(this.name);
        }

        public virtual void FromBBuffer(BBuffer bb)
        {
            bb.readLengthLimit = 0;
            bb.Read(ref this.id);
            bb.readLengthLimit = 0;
            bb.Read(ref this.name);
        }
        public virtual void ToString(ref System.Text.StringBuilder str)
        {
            if (GetToStringFlag())
            {
        	    str.Append("[ \"***** recursived *****\" ]");
        	    return;
            }
            else SetToStringFlag(true);

            str.Append("{ \"pkgTypeName\":\"Tables.notice_type\", \"pkgTypeId\":" + GetPackageId());
            ToStringCore(ref str);
            str.Append(" }");
        
            SetToStringFlag(false);
        }
        public virtual void ToStringCore(ref System.Text.StringBuilder str)
        {
            if (id != null) str.Append(", \"id\":\"" + id + "\"");
            else str.Append(", \"id\":nil");
            if (name != null) str.Append(", \"name\":\"" + name + "\"");
            else str.Append(", \"name\":nil");
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
    public partial class verify_code : IBBuffer
    {
        public string phone;
        public string content;
        public string create_time;

        public virtual ushort GetPackageId()
        {
            return TypeIdMaps<verify_code>.typeId;
        }

        public virtual void ToBBuffer(BBuffer bb)
        {
            bb.Write(this.phone);
            bb.Write(this.content);
            bb.Write(this.create_time);
        }

        public virtual void FromBBuffer(BBuffer bb)
        {
            bb.readLengthLimit = 0;
            bb.Read(ref this.phone);
            bb.readLengthLimit = 0;
            bb.Read(ref this.content);
            bb.readLengthLimit = 0;
            bb.Read(ref this.create_time);
        }
        public virtual void ToString(ref System.Text.StringBuilder str)
        {
            if (GetToStringFlag())
            {
        	    str.Append("[ \"***** recursived *****\" ]");
        	    return;
            }
            else SetToStringFlag(true);

            str.Append("{ \"pkgTypeName\":\"Tables.verify_code\", \"pkgTypeId\":" + GetPackageId());
            ToStringCore(ref str);
            str.Append(" }");
        
            SetToStringFlag(false);
        }
        public virtual void ToStringCore(ref System.Text.StringBuilder str)
        {
            if (phone != null) str.Append(", \"phone\":\"" + phone + "\"");
            else str.Append(", \"phone\":nil");
            if (content != null) str.Append(", \"content\":\"" + content + "\"");
            else str.Append(", \"content\":nil");
            if (create_time != null) str.Append(", \"create_time\":\"" + create_time + "\"");
            else str.Append(", \"create_time\":nil");
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
    public partial class vip : IBBuffer
    {
        public string id;
        public string total_recharge;

        public virtual ushort GetPackageId()
        {
            return TypeIdMaps<vip>.typeId;
        }

        public virtual void ToBBuffer(BBuffer bb)
        {
            bb.Write(this.id);
            bb.Write(this.total_recharge);
        }

        public virtual void FromBBuffer(BBuffer bb)
        {
            bb.readLengthLimit = 0;
            bb.Read(ref this.id);
            bb.readLengthLimit = 0;
            bb.Read(ref this.total_recharge);
        }
        public virtual void ToString(ref System.Text.StringBuilder str)
        {
            if (GetToStringFlag())
            {
        	    str.Append("[ \"***** recursived *****\" ]");
        	    return;
            }
            else SetToStringFlag(true);

            str.Append("{ \"pkgTypeName\":\"Tables.vip\", \"pkgTypeId\":" + GetPackageId());
            ToStringCore(ref str);
            str.Append(" }");
        
            SetToStringFlag(false);
        }
        public virtual void ToStringCore(ref System.Text.StringBuilder str)
        {
            if (id != null) str.Append(", \"id\":\"" + id + "\"");
            else str.Append(", \"id\":nil");
            if (total_recharge != null) str.Append(", \"total_recharge\":\"" + total_recharge + "\"");
            else str.Append(", \"total_recharge\":nil");
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
    public partial class withdrawals : IBBuffer
    {
        public string withdrawals_id;
        public string account_id;
        public string money;
        public string requirement;
        public string state_id;
        public string description;
        public string create_time;
        public string last_create_time;

        public virtual ushort GetPackageId()
        {
            return TypeIdMaps<withdrawals>.typeId;
        }

        public virtual void ToBBuffer(BBuffer bb)
        {
            bb.Write(this.withdrawals_id);
            bb.Write(this.account_id);
            bb.Write(this.money);
            bb.Write(this.requirement);
            bb.Write(this.state_id);
            bb.Write(this.description);
            bb.Write(this.create_time);
            bb.Write(this.last_create_time);
        }

        public virtual void FromBBuffer(BBuffer bb)
        {
            bb.readLengthLimit = 0;
            bb.Read(ref this.withdrawals_id);
            bb.readLengthLimit = 0;
            bb.Read(ref this.account_id);
            bb.readLengthLimit = 0;
            bb.Read(ref this.money);
            bb.readLengthLimit = 0;
            bb.Read(ref this.requirement);
            bb.readLengthLimit = 0;
            bb.Read(ref this.state_id);
            bb.readLengthLimit = 0;
            bb.Read(ref this.description);
            bb.readLengthLimit = 0;
            bb.Read(ref this.create_time);
            bb.readLengthLimit = 0;
            bb.Read(ref this.last_create_time);
        }
        public virtual void ToString(ref System.Text.StringBuilder str)
        {
            if (GetToStringFlag())
            {
        	    str.Append("[ \"***** recursived *****\" ]");
        	    return;
            }
            else SetToStringFlag(true);

            str.Append("{ \"pkgTypeName\":\"Tables.withdrawals\", \"pkgTypeId\":" + GetPackageId());
            ToStringCore(ref str);
            str.Append(" }");
        
            SetToStringFlag(false);
        }
        public virtual void ToStringCore(ref System.Text.StringBuilder str)
        {
            if (withdrawals_id != null) str.Append(", \"withdrawals_id\":\"" + withdrawals_id + "\"");
            else str.Append(", \"withdrawals_id\":nil");
            if (account_id != null) str.Append(", \"account_id\":\"" + account_id + "\"");
            else str.Append(", \"account_id\":nil");
            if (money != null) str.Append(", \"money\":\"" + money + "\"");
            else str.Append(", \"money\":nil");
            if (requirement != null) str.Append(", \"requirement\":\"" + requirement + "\"");
            else str.Append(", \"requirement\":nil");
            if (state_id != null) str.Append(", \"state_id\":\"" + state_id + "\"");
            else str.Append(", \"state_id\":nil");
            if (description != null) str.Append(", \"description\":\"" + description + "\"");
            else str.Append(", \"description\":nil");
            if (create_time != null) str.Append(", \"create_time\":\"" + create_time + "\"");
            else str.Append(", \"create_time\":nil");
            if (last_create_time != null) str.Append(", \"last_create_time\":\"" + last_create_time + "\"");
            else str.Append(", \"last_create_time\":nil");
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
    public partial class withdrawals_state : IBBuffer
    {
        public string id;
        public string name;

        public virtual ushort GetPackageId()
        {
            return TypeIdMaps<withdrawals_state>.typeId;
        }

        public virtual void ToBBuffer(BBuffer bb)
        {
            bb.Write(this.id);
            bb.Write(this.name);
        }

        public virtual void FromBBuffer(BBuffer bb)
        {
            bb.readLengthLimit = 0;
            bb.Read(ref this.id);
            bb.readLengthLimit = 0;
            bb.Read(ref this.name);
        }
        public virtual void ToString(ref System.Text.StringBuilder str)
        {
            if (GetToStringFlag())
            {
        	    str.Append("[ \"***** recursived *****\" ]");
        	    return;
            }
            else SetToStringFlag(true);

            str.Append("{ \"pkgTypeName\":\"Tables.withdrawals_state\", \"pkgTypeId\":" + GetPackageId());
            ToStringCore(ref str);
            str.Append(" }");
        
            SetToStringFlag(false);
        }
        public virtual void ToStringCore(ref System.Text.StringBuilder str)
        {
            if (id != null) str.Append(", \"id\":\"" + id + "\"");
            else str.Append(", \"id\":nil");
            if (name != null) str.Append(", \"name\":\"" + name + "\"");
            else str.Append(", \"name\":nil");
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
    public partial class node : IBBuffer
    {
        public int id;
        public int? pid;

        public virtual ushort GetPackageId()
        {
            return TypeIdMaps<node>.typeId;
        }

        public virtual void ToBBuffer(BBuffer bb)
        {
            bb.Write(this.id);
            bb.Write(this.pid);
        }

        public virtual void FromBBuffer(BBuffer bb)
        {
            bb.Read(ref this.id);
            bb.Read(ref this.pid);
        }
        public virtual void ToString(ref System.Text.StringBuilder str)
        {
            if (GetToStringFlag())
            {
        	    str.Append("[ \"***** recursived *****\" ]");
        	    return;
            }
            else SetToStringFlag(true);

            str.Append("{ \"pkgTypeName\":\"Tables.node\", \"pkgTypeId\":" + GetPackageId());
            ToStringCore(ref str);
            str.Append(" }");
        
            SetToStringFlag(false);
        }
        public virtual void ToStringCore(ref System.Text.StringBuilder str)
        {
            str.Append(", \"id\":" + id);
            str.Append(", \"pid\":" + (pid.HasValue ? pid.Value.ToString() : "nil"));
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
            BBuffer.Register<Foo>(3);
            BBuffer.Register<List<Foo>>(4);
            BBuffer.Register<List<IBBuffer>>(5);
            BBuffer.Register<FooEx>(6);
            BBuffer.Register<Node>(7);
            BBuffer.Register<Tables.node>(8);
            BBuffer.Register<List<Node>>(9);
            BBuffer.Register<DataSet>(10);
            BBuffer.Register<List<Table>>(11);
            BBuffer.Register<Table>(12);
            BBuffer.Register<List<TableColumn>>(13);
            BBuffer.Register<TableColumn>(14);
            BBuffer.Register<List<TableRow>>(15);
            BBuffer.Register<TableRow>(16);
            BBuffer.Register<List<TableRowValue>>(17);
            BBuffer.Register<TableRowValue>(18);
            BBuffer.Register<TableRowValue_Int>(19);
            BBuffer.Register<TableRowValue_NullableInt>(20);
            BBuffer.Register<TableRowValue_String>(21);
            BBuffer.Register<Tables.account>(22);
            BBuffer.Register<Tables.chat>(23);
            BBuffer.Register<Tables.config>(24);
            BBuffer.Register<Tables.config_value>(25);
            BBuffer.Register<Tables.datatype>(26);
            BBuffer.Register<Tables.fish>(27);
            BBuffer.Register<Tables.game>(28);
            BBuffer.Register<Tables.game_catchfish>(29);
            BBuffer.Register<Tables.game_catchfish_account>(30);
            BBuffer.Register<Tables.game_catchfish_cannon>(31);
            BBuffer.Register<Tables.game_catchfish_level>(32);
            BBuffer.Register<Tables.game_catchfish_level_room>(33);
            BBuffer.Register<Tables.log_account_avatar>(34);
            BBuffer.Register<Tables.log_account_enabled>(35);
            BBuffer.Register<Tables.log_account_money_safe>(36);
            BBuffer.Register<Tables.log_account_nickname>(37);
            BBuffer.Register<Tables.log_account_password>(38);
            BBuffer.Register<Tables.log_account_phone>(39);
            BBuffer.Register<Tables.log_account_recharge>(40);
            BBuffer.Register<Tables.log_game>(41);
            BBuffer.Register<Tables.log_game_catchfish>(42);
            BBuffer.Register<Tables.log_game_catchfish_event>(43);
            BBuffer.Register<Tables.log_game_catchfish_event_bulletdie>(44);
            BBuffer.Register<Tables.log_game_catchfish_event_bulletmiss>(45);
            BBuffer.Register<Tables.log_game_catchfish_event_fire>(46);
            BBuffer.Register<Tables.log_game_catchfish_event_fishdie>(47);
            BBuffer.Register<Tables.log_login_out>(48);
            BBuffer.Register<Tables.log_withdrawals>(49);
            BBuffer.Register<Tables.mail>(50);
            BBuffer.Register<Tables.marquee>(51);
            BBuffer.Register<Tables.notice>(52);
            BBuffer.Register<Tables.notice_read>(53);
            BBuffer.Register<Tables.notice_type>(54);
            BBuffer.Register<Tables.verify_code>(55);
            BBuffer.Register<Tables.vip>(56);
            BBuffer.Register<Tables.withdrawals>(57);
            BBuffer.Register<Tables.withdrawals_state>(58);
        }
    }
}
