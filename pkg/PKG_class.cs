using System;
using xx;
namespace PKG
{
    public static class PkgGenMd5
    {
        public const string value = "dde27285e5784277142135589ddf9e51"; 
    }

    public partial class Foo : IObject
    {
        public int id;
        public int? age;
        public string info;
        public List<Foo> childs;
        public IObject o;
        public List<IObject> os;

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
        public virtual void MySqlAppend(ref System.Text.StringBuilder sb, bool ignoreReadOnly)
        {
            sb.Append("(");
            sb.Append(this.id);
            sb.Append(", ");
            sb.Append(this.age.HasValue ? this.age.Value.ToString() : "null");
            sb.Append(", ");
            sb.Append(this.info == null ? "null" : ("'" + this.info.Replace("'", "''") + "'"));
            sb.Append(", ");
            sb.Length -= 2;
            sb.Append(")");

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
        public override void MySqlAppend(ref System.Text.StringBuilder sb, bool ignoreReadOnly)
        {
            base.MySqlAppend(ref sb, ignoreReadOnly);
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
        public override void MySqlAppend(ref System.Text.StringBuilder sb, bool ignoreReadOnly)
        {
            base.MySqlAppend(ref sb, ignoreReadOnly);
        }
    }
    public partial class DataSet : IObject
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
        public virtual void MySqlAppend(ref System.Text.StringBuilder sb, bool ignoreReadOnly)
        {
        }
    }
    public partial class Table : IObject
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
        public virtual void MySqlAppend(ref System.Text.StringBuilder sb, bool ignoreReadOnly)
        {
        }
    }
    public partial class TableColumn : IObject
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
        public virtual void MySqlAppend(ref System.Text.StringBuilder sb, bool ignoreReadOnly)
        {
        }
    }
    public partial class TableRow : IObject
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
        public virtual void MySqlAppend(ref System.Text.StringBuilder sb, bool ignoreReadOnly)
        {
        }
    }
    public partial class TableRowValue : IObject
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
        public virtual void MySqlAppend(ref System.Text.StringBuilder sb, bool ignoreReadOnly)
        {
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
        public override void MySqlAppend(ref System.Text.StringBuilder sb, bool ignoreReadOnly)
        {
            base.MySqlAppend(ref sb, ignoreReadOnly);
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
        public override void MySqlAppend(ref System.Text.StringBuilder sb, bool ignoreReadOnly)
        {
            base.MySqlAppend(ref sb, ignoreReadOnly);
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
        public override void MySqlAppend(ref System.Text.StringBuilder sb, bool ignoreReadOnly)
        {
            base.MySqlAppend(ref sb, ignoreReadOnly);
        }
    }
namespace Tables
{
    /// <summary>
    /// 玩家账号表( 基表 )
    /// </summary>
    public partial class account : IObject
    {
        /// <summary>
        /// 用户Id ( 随机 8 位整数 )
        /// </summary>
        public int id;
        /// <summary>
        /// 原始用户名 唯一( GUID )
        /// </summary>
        public string username;
        /// <summary>
        /// 昵称 唯一( 默认用某种规则生成 )
        /// </summary>
        public string nickname;
        /// <summary>
        /// 头像
        /// </summary>
        public int avatar_id;
        /// <summary>
        /// 电话号码 唯一( 默认填充 username GUID )
        /// </summary>
        public string phone;
        /// <summary>
        /// 密码( 默认为空 )
        /// </summary>
        public string password;
        /// <summary>
        /// 账户余额( 保留4位小数位, 进部分游戏时会被清0, 结束时会兑换返还 )
        /// </summary>
        public double money;
        /// <summary>
        /// 保险箱( 玩家可在账户余额间搬运数据 )
        /// </summary>
        public double money_safe;
        /// <summary>
        /// 累计充值金额
        /// </summary>
        public double total_recharge;
        /// <summary>
        /// 累计消费金额( 比如在鱼里一共打了多少钱的炮 )
        /// </summary>
        public double total_consume;
        /// <summary>
        /// 累计提现金额
        /// </summary>
        public double total_withdraw;
        /// <summary>
        /// 创建时间. epoch 10m 精度. 所有表的这个字段都是这个格式
        /// </summary>
        public long create_time;
        /// <summary>
        /// 启用标记( 1 : 启用 )
        /// </summary>
        public int enabled;
        /// <summary>
        /// 玩家进入大厅时间(为空 则离开大厅)
        /// </summary>
        public long? enter_lobby_time;
        /// <summary>
        /// 正在游戏中的玩家日志id(为空 则离开游戏)
        /// </summary>
        public int? log_game_id;

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
            bb.Read(ref this.id);
            bb.readLengthLimit = 0;
            bb.Read(ref this.username);
            bb.readLengthLimit = 0;
            bb.Read(ref this.nickname);
            bb.Read(ref this.avatar_id);
            bb.readLengthLimit = 0;
            bb.Read(ref this.phone);
            bb.readLengthLimit = 0;
            bb.Read(ref this.password);
            bb.Read(ref this.money);
            bb.Read(ref this.money_safe);
            bb.Read(ref this.total_recharge);
            bb.Read(ref this.total_consume);
            bb.Read(ref this.total_withdraw);
            bb.Read(ref this.create_time);
            bb.Read(ref this.enabled);
            bb.Read(ref this.enter_lobby_time);
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
            str.Append(", \"id\":" + id);
            if (username != null) str.Append(", \"username\":\"" + username + "\"");
            else str.Append(", \"username\":nil");
            if (nickname != null) str.Append(", \"nickname\":\"" + nickname + "\"");
            else str.Append(", \"nickname\":nil");
            str.Append(", \"avatar_id\":" + avatar_id);
            if (phone != null) str.Append(", \"phone\":\"" + phone + "\"");
            else str.Append(", \"phone\":nil");
            if (password != null) str.Append(", \"password\":\"" + password + "\"");
            else str.Append(", \"password\":nil");
            str.Append(", \"money\":" + money);
            str.Append(", \"money_safe\":" + money_safe);
            str.Append(", \"total_recharge\":" + total_recharge);
            str.Append(", \"total_consume\":" + total_consume);
            str.Append(", \"total_withdraw\":" + total_withdraw);
            str.Append(", \"create_time\":" + create_time);
            str.Append(", \"enabled\":" + enabled);
            str.Append(", \"enter_lobby_time\":" + (enter_lobby_time.HasValue ? enter_lobby_time.Value.ToString() : "nil"));
            str.Append(", \"log_game_id\":" + (log_game_id.HasValue ? log_game_id.Value.ToString() : "nil"));
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
        public virtual void MySqlAppend(ref System.Text.StringBuilder sb, bool ignoreReadOnly)
        {
            sb.Append("(");
            sb.Append(this.id);
            sb.Append(", ");
            sb.Append(this.username == null ? "null" : ("'" + this.username.Replace("'", "''") + "'"));
            sb.Append(", ");
            sb.Append(this.nickname == null ? "null" : ("'" + this.nickname.Replace("'", "''") + "'"));
            sb.Append(", ");
            sb.Append(this.avatar_id);
            sb.Append(", ");
            sb.Append(this.phone == null ? "null" : ("'" + this.phone.Replace("'", "''") + "'"));
            sb.Append(", ");
            sb.Append(this.password == null ? "null" : ("'" + this.password.Replace("'", "''") + "'"));
            sb.Append(", ");
            sb.Append(this.money);
            sb.Append(", ");
            sb.Append(this.money_safe);
            sb.Append(", ");
            sb.Append(this.total_recharge);
            sb.Append(", ");
            sb.Append(this.total_consume);
            sb.Append(", ");
            sb.Append(this.total_withdraw);
            sb.Append(", ");
            sb.Append(this.create_time);
            sb.Append(", ");
            sb.Append(this.enabled);
            sb.Append(", ");
            sb.Append(this.enter_lobby_time.HasValue ? this.enter_lobby_time.Value.ToString() : "null");
            sb.Append(", ");
            sb.Append(this.log_game_id.HasValue ? this.log_game_id.Value.ToString() : "null");
            sb.Append(", ");
            sb.Length -= 2;
            sb.Append(")");

        }
    }
    /// <summary>
    /// 在线聊天
    /// </summary>
    public partial class chat : IObject
    {
        public int id;
        /// <summary>
        /// 内容
        /// </summary>
        public string content;
        /// <summary>
        /// 聊天人id
        /// </summary>
        public int account_id;
        /// <summary>
        /// 创建时间
        /// </summary>
        public long create_time;

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
            bb.Read(ref this.id);
            bb.readLengthLimit = 0;
            bb.Read(ref this.content);
            bb.Read(ref this.account_id);
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
            str.Append(", \"id\":" + id);
            if (content != null) str.Append(", \"content\":\"" + content + "\"");
            else str.Append(", \"content\":nil");
            str.Append(", \"account_id\":" + account_id);
            str.Append(", \"create_time\":" + create_time);
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
        public virtual void MySqlAppend(ref System.Text.StringBuilder sb, bool ignoreReadOnly)
        {
            sb.Append("(");
            if (!ignoreReadOnly)
            {
            sb.Append(this.id);
            sb.Append(", ");
            }
            sb.Append(this.content == null ? "null" : ("'" + this.content.Replace("'", "''") + "'"));
            sb.Append(", ");
            sb.Append(this.account_id);
            sb.Append(", ");
            sb.Append(this.create_time);
            sb.Append(", ");
            sb.Length -= 2;
            sb.Append(")");

        }
    }
    /// <summary>
    /// 通用配置表( 带版本号的键值对集合 )
    /// </summary>
    public partial class config : IObject
    {
        /// <summary>
        /// 键名
        /// </summary>
        public string key;
        /// <summary>
        /// 数据类型名
        /// </summary>
        public string datatype_name;
        /// <summary>
        /// 显示名
        /// </summary>
        public string title;
        /// <summary>
        /// 说明
        /// </summary>
        public string desc;
        /// <summary>
        /// 创建时间
        /// </summary>
        public long create_time;

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
            str.Append(", \"create_time\":" + create_time);
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
        public virtual void MySqlAppend(ref System.Text.StringBuilder sb, bool ignoreReadOnly)
        {
            sb.Append("(");
            sb.Append(this.key == null ? "null" : ("'" + this.key.Replace("'", "''") + "'"));
            sb.Append(", ");
            sb.Append(this.datatype_name == null ? "null" : ("'" + this.datatype_name.Replace("'", "''") + "'"));
            sb.Append(", ");
            sb.Append(this.title == null ? "null" : ("'" + this.title.Replace("'", "''") + "'"));
            sb.Append(", ");
            sb.Append(this.desc == null ? "null" : ("'" + this.desc.Replace("'", "''") + "'"));
            sb.Append(", ");
            sb.Append(this.create_time);
            sb.Append(", ");
            sb.Length -= 2;
            sb.Append(")");

        }
    }
    /// <summary>
    /// 配置项所对应的值( 保留历史变化, 通常只有最新的那一个生效 )
    /// </summary>
    public partial class config_value : IObject
    {
        /// <summary>
        /// 键名
        /// </summary>
        public string key;
        /// <summary>
        /// 对应 bigint 数据类型的值( int64 )
        /// </summary>
        public long? value_bigint;
        /// <summary>
        /// 对应 double 数据类型的值
        /// </summary>
        public double? value_double;
        /// <summary>
        /// 对应 varchar 数据类型的值( string )
        /// </summary>
        public string value_varchar;
        /// <summary>
        /// 创建时间
        /// </summary>
        public long create_time;

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
            bb.Read(ref this.value_bigint);
            bb.Read(ref this.value_double);
            bb.readLengthLimit = 0;
            bb.Read(ref this.value_varchar);
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
            str.Append(", \"value_bigint\":" + (value_bigint.HasValue ? value_bigint.Value.ToString() : "nil"));
            str.Append(", \"value_double\":" + (value_double.HasValue ? value_double.Value.ToString() : "nil"));
            if (value_varchar != null) str.Append(", \"value_varchar\":\"" + value_varchar + "\"");
            else str.Append(", \"value_varchar\":nil");
            str.Append(", \"create_time\":" + create_time);
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
        public virtual void MySqlAppend(ref System.Text.StringBuilder sb, bool ignoreReadOnly)
        {
            sb.Append("(");
            sb.Append(this.key == null ? "null" : ("'" + this.key.Replace("'", "''") + "'"));
            sb.Append(", ");
            sb.Append(this.value_bigint.HasValue ? this.value_bigint.Value.ToString() : "null");
            sb.Append(", ");
            sb.Append(this.value_double.HasValue ? this.value_double.Value.ToString() : "null");
            sb.Append(", ");
            sb.Append(this.value_varchar == null ? "null" : ("'" + this.value_varchar.Replace("'", "''") + "'"));
            sb.Append(", ");
            sb.Append(this.create_time);
            sb.Append(", ");
            sb.Length -= 2;
            sb.Append(")");

        }
    }
    /// <summary>
    /// 数据类型列表( 主要供 config 用 )
    /// </summary>
    public partial class datatype : IObject
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
        public virtual void MySqlAppend(ref System.Text.StringBuilder sb, bool ignoreReadOnly)
        {
            sb.Append("(");
            sb.Append(this.name == null ? "null" : ("'" + this.name.Replace("'", "''") + "'"));
            sb.Append(", ");
            sb.Length -= 2;
            sb.Append(")");

        }
    }
    /// <summary>
    /// 鱼种类表
    /// </summary>
    public partial class fish : IObject
    {
        /// <summary>
        /// 鱼配置id
        /// </summary>
        public int id;
        /// <summary>
        /// 名称
        /// </summary>
        public string name;
        /// <summary>
        /// 打死鱼的金币所得最小基数( 也可理解成倍率 )
        /// </summary>
        public int minCoin;
        /// <summary>
        ///  打死鱼的金币所得最大基数( 也可理解成倍率 )
        /// </summary>
        public int maxCoin;

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
            bb.Read(ref this.id);
            bb.readLengthLimit = 0;
            bb.Read(ref this.name);
            bb.Read(ref this.minCoin);
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
            str.Append(", \"id\":" + id);
            if (name != null) str.Append(", \"name\":\"" + name + "\"");
            else str.Append(", \"name\":nil");
            str.Append(", \"minCoin\":" + minCoin);
            str.Append(", \"maxCoin\":" + maxCoin);
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
        public virtual void MySqlAppend(ref System.Text.StringBuilder sb, bool ignoreReadOnly)
        {
            sb.Append("(");
            sb.Append(this.id);
            sb.Append(", ");
            sb.Append(this.name == null ? "null" : ("'" + this.name.Replace("'", "''") + "'"));
            sb.Append(", ");
            sb.Append(this.minCoin);
            sb.Append(", ");
            sb.Append(this.maxCoin);
            sb.Append(", ");
            sb.Length -= 2;
            sb.Append(")");

        }
    }
    /// <summary>
    /// 所有游戏列表( 基表 )
    /// </summary>
    public partial class game : IObject
    {
        /// <summary>
        /// 游戏编号( 非自增, 直接填死 )
        /// </summary>
        public int id;
        /// <summary>
        /// 游戏名
        /// </summary>
        public string name;
        /// <summary>
        /// 类型名( 派生表名 )
        /// </summary>
        public string type_name;
        /// <summary>
        /// 是否启用 1: true
        /// </summary>
        public int enabled;

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
            bb.Read(ref this.id);
            bb.readLengthLimit = 0;
            bb.Read(ref this.name);
            bb.readLengthLimit = 0;
            bb.Read(ref this.type_name);
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
            str.Append(", \"id\":" + id);
            if (name != null) str.Append(", \"name\":\"" + name + "\"");
            else str.Append(", \"name\":nil");
            if (type_name != null) str.Append(", \"type_name\":\"" + type_name + "\"");
            else str.Append(", \"type_name\":nil");
            str.Append(", \"enabled\":" + enabled);
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
        public virtual void MySqlAppend(ref System.Text.StringBuilder sb, bool ignoreReadOnly)
        {
            sb.Append("(");
            sb.Append(this.id);
            sb.Append(", ");
            sb.Append(this.name == null ? "null" : ("'" + this.name.Replace("'", "''") + "'"));
            sb.Append(", ");
            sb.Append(this.type_name == null ? "null" : ("'" + this.type_name.Replace("'", "''") + "'"));
            sb.Append(", ");
            sb.Append(this.enabled);
            sb.Append(", ");
            sb.Length -= 2;
            sb.Append(")");

        }
    }
    /// <summary>
    /// game 的 捕鱼游戏 派生表( 存放专属字段 )
    /// </summary>
    public partial class game_catchfish : IObject
    {
        /// <summary>
        /// 基表游戏编号
        /// </summary>
        public int id;
        /// <summary>
        /// 进出游戏时 money 自动兑换成 coin 要 乘除 的系数
        /// </summary>
        public int exchange_coin_ratio;

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
            bb.Read(ref this.id);
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
            str.Append(", \"id\":" + id);
            str.Append(", \"exchange_coin_ratio\":" + exchange_coin_ratio);
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
        public virtual void MySqlAppend(ref System.Text.StringBuilder sb, bool ignoreReadOnly)
        {
            sb.Append("(");
            sb.Append(this.id);
            sb.Append(", ");
            sb.Append(this.exchange_coin_ratio);
            sb.Append(", ");
            sb.Length -= 2;
            sb.Append(")");

        }
    }
    /// <summary>
    /// 玩家于捕鱼游戏的个人配置信息
    /// </summary>
    public partial class game_catchfish_account : IObject
    {
        /// <summary>
        /// 账号id
        /// </summary>
        public int account_id;
        /// <summary>
        /// 捕鱼游戏id
        /// </summary>
        public int game_id;
        /// <summary>
        /// 级别id
        /// </summary>
        public int level_id;
        /// <summary>
        /// 玩家上次游戏退出前用的子弹的倍率( 游戏内金币整数值 )
        /// </summary>
        public long last_bet;
        /// <summary>
        /// 玩家上次游戏退出前用的炮台id
        /// </summary>
        public int last_cannon_id;

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
            bb.Read(ref this.account_id);
            bb.Read(ref this.game_id);
            bb.Read(ref this.level_id);
            bb.Read(ref this.last_bet);
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
            str.Append(", \"account_id\":" + account_id);
            str.Append(", \"game_id\":" + game_id);
            str.Append(", \"level_id\":" + level_id);
            str.Append(", \"last_bet\":" + last_bet);
            str.Append(", \"last_cannon_id\":" + last_cannon_id);
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
        public virtual void MySqlAppend(ref System.Text.StringBuilder sb, bool ignoreReadOnly)
        {
            sb.Append("(");
            sb.Append(this.account_id);
            sb.Append(", ");
            sb.Append(this.game_id);
            sb.Append(", ");
            sb.Append(this.level_id);
            sb.Append(", ");
            sb.Append(this.last_bet);
            sb.Append(", ");
            sb.Append(this.last_cannon_id);
            sb.Append(", ");
            sb.Length -= 2;
            sb.Append(")");

        }
    }
    /// <summary>
    /// 捕鱼游戏内炮台skin与累计充值金额的对应表
    /// </summary>
    public partial class game_catchfish_cannon : IObject
    {
        /// <summary>
        /// 唯一编号 & 炮台Skin
        /// </summary>
        public int id;
        /// <summary>
        /// VIP等级
        /// </summary>
        public int vip_id;

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
            bb.Read(ref this.id);
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
            str.Append(", \"id\":" + id);
            str.Append(", \"vip_id\":" + vip_id);
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
        public virtual void MySqlAppend(ref System.Text.StringBuilder sb, bool ignoreReadOnly)
        {
            sb.Append("(");
            sb.Append(this.id);
            sb.Append(", ");
            sb.Append(this.vip_id);
            sb.Append(", ");
            sb.Length -= 2;
            sb.Append(")");

        }
    }
    /// <summary>
    /// 捕鱼游戏的分级表( 1对多 )
    /// </summary>
    public partial class game_catchfish_level : IObject
    {
        /// <summary>
        /// 游戏id
        /// </summary>
        public int game_id;
        /// <summary>
        /// 游戏级别id. 0 1 2....: 新手厅  进阶厅  高手厅 ....
        /// </summary>
        public int level_id;
        /// <summary>
        /// 炮值(从)(游戏内金币)
        /// </summary>
        public long min_bet;
        /// <summary>
        /// 炮值(到)(游戏内金币)
        /// </summary>
        public long max_bet;
        /// <summary>
        /// 最低准入金额
        /// </summary>
        public double min_money;
        /// <summary>
        /// 是否启用 1: true
        /// </summary>
        public int enabled;

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
            bb.Read(ref this.game_id);
            bb.Read(ref this.level_id);
            bb.Read(ref this.min_bet);
            bb.Read(ref this.max_bet);
            bb.Read(ref this.min_money);
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
            str.Append(", \"game_id\":" + game_id);
            str.Append(", \"level_id\":" + level_id);
            str.Append(", \"min_bet\":" + min_bet);
            str.Append(", \"max_bet\":" + max_bet);
            str.Append(", \"min_money\":" + min_money);
            str.Append(", \"enabled\":" + enabled);
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
        public virtual void MySqlAppend(ref System.Text.StringBuilder sb, bool ignoreReadOnly)
        {
            sb.Append("(");
            sb.Append(this.game_id);
            sb.Append(", ");
            sb.Append(this.level_id);
            sb.Append(", ");
            sb.Append(this.min_bet);
            sb.Append(", ");
            sb.Append(this.max_bet);
            sb.Append(", ");
            sb.Append(this.min_money);
            sb.Append(", ");
            sb.Append(this.enabled);
            sb.Append(", ");
            sb.Length -= 2;
            sb.Append(")");

        }
    }
    /// <summary>
    /// 捕鱼游戏的分级表 下属房间表( 1对多 )
    /// </summary>
    public partial class game_catchfish_level_room : IObject
    {
        /// <summary>
        /// 游戏id
        /// </summary>
        public int game_id;
        /// <summary>
        /// 游戏级别id
        /// </summary>
        public int level_id;
        /// <summary>
        /// 房号( 从 1 开始填充, 新增取最大值+1 )
        /// </summary>
        public int room_id;
        /// <summary>
        /// 是否启用 1: true
        /// </summary>
        public int enabled;

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
            bb.Read(ref this.game_id);
            bb.Read(ref this.level_id);
            bb.Read(ref this.room_id);
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
            str.Append(", \"game_id\":" + game_id);
            str.Append(", \"level_id\":" + level_id);
            str.Append(", \"room_id\":" + room_id);
            str.Append(", \"enabled\":" + enabled);
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
        public virtual void MySqlAppend(ref System.Text.StringBuilder sb, bool ignoreReadOnly)
        {
            sb.Append("(");
            if (!ignoreReadOnly)
            {
            sb.Append(this.game_id);
            sb.Append(", ");
            }
            sb.Append(this.level_id);
            sb.Append(", ");
            sb.Append(this.room_id);
            sb.Append(", ");
            sb.Append(this.enabled);
            sb.Append(", ");
            sb.Length -= 2;
            sb.Append(")");

        }
    }
    /// <summary>
    /// 头像变更历史表
    /// </summary>
    public partial class log_account_avatar : IObject
    {
        public int id;
        /// <summary>
        /// 帐号id
        /// </summary>
        public int account_id;
        /// <summary>
        /// 头像
        /// </summary>
        public int avatar_id;
        /// <summary>
        /// 创建时间
        /// </summary>
        public long create_time;

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
            bb.Read(ref this.id);
            bb.Read(ref this.account_id);
            bb.Read(ref this.avatar_id);
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
            str.Append(", \"id\":" + id);
            str.Append(", \"account_id\":" + account_id);
            str.Append(", \"avatar_id\":" + avatar_id);
            str.Append(", \"create_time\":" + create_time);
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
        public virtual void MySqlAppend(ref System.Text.StringBuilder sb, bool ignoreReadOnly)
        {
            sb.Append("(");
            if (!ignoreReadOnly)
            {
            sb.Append(this.id);
            sb.Append(", ");
            }
            sb.Append(this.account_id);
            sb.Append(", ");
            sb.Append(this.avatar_id);
            sb.Append(", ");
            sb.Append(this.create_time);
            sb.Append(", ");
            sb.Length -= 2;
            sb.Append(")");

        }
    }
    /// <summary>
    /// 帐号启用标记变更历史表
    /// </summary>
    public partial class log_account_enabled : IObject
    {
        public int id;
        /// <summary>
        /// 帐号id
        /// </summary>
        public int account_id;
        /// <summary>
        /// 启用标记
        /// </summary>
        public int enabled;
        /// <summary>
        /// 备注( 含修改人 & 原因 )
        /// </summary>
        public string memo;
        /// <summary>
        /// 日志创建时间( 修改时间 )
        /// </summary>
        public long create_time;

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
            bb.Read(ref this.id);
            bb.Read(ref this.account_id);
            bb.Read(ref this.enabled);
            bb.readLengthLimit = 0;
            bb.Read(ref this.memo);
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
            str.Append(", \"id\":" + id);
            str.Append(", \"account_id\":" + account_id);
            str.Append(", \"enabled\":" + enabled);
            if (memo != null) str.Append(", \"memo\":\"" + memo + "\"");
            else str.Append(", \"memo\":nil");
            str.Append(", \"create_time\":" + create_time);
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
        public virtual void MySqlAppend(ref System.Text.StringBuilder sb, bool ignoreReadOnly)
        {
            sb.Append("(");
            if (!ignoreReadOnly)
            {
            sb.Append(this.id);
            sb.Append(", ");
            }
            sb.Append(this.account_id);
            sb.Append(", ");
            sb.Append(this.enabled);
            sb.Append(", ");
            sb.Append(this.memo == null ? "null" : ("'" + this.memo.Replace("'", "''") + "'"));
            sb.Append(", ");
            sb.Append(this.create_time);
            sb.Append(", ");
            sb.Length -= 2;
            sb.Append(")");

        }
    }
    /// <summary>
    /// 保险箱操作日志
    /// </summary>
    public partial class log_account_money_safe : IObject
    {
        public int id;
        /// <summary>
        /// 帐号id
        /// </summary>
        public int account_id;
        /// <summary>
        /// 金额( 正数为存入保险箱, 负数为从保险箱提钱 )
        /// </summary>
        public double value;
        /// <summary>
        /// 创建时间
        /// </summary>
        public long create_time;

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
            bb.Read(ref this.id);
            bb.Read(ref this.account_id);
            bb.Read(ref this.value);
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
            str.Append(", \"id\":" + id);
            str.Append(", \"account_id\":" + account_id);
            str.Append(", \"value\":" + value);
            str.Append(", \"create_time\":" + create_time);
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
        public virtual void MySqlAppend(ref System.Text.StringBuilder sb, bool ignoreReadOnly)
        {
            sb.Append("(");
            if (!ignoreReadOnly)
            {
            sb.Append(this.id);
            sb.Append(", ");
            }
            sb.Append(this.account_id);
            sb.Append(", ");
            sb.Append(this.value);
            sb.Append(", ");
            sb.Append(this.create_time);
            sb.Append(", ");
            sb.Length -= 2;
            sb.Append(")");

        }
    }
    /// <summary>
    /// 昵称变更历史表
    /// </summary>
    public partial class log_account_nickname : IObject
    {
        public int id;
        /// <summary>
        /// 帐号id
        /// </summary>
        public int account_id;
        /// <summary>
        /// 昵称
        /// </summary>
        public string nickname;
        /// <summary>
        /// 创建时间
        /// </summary>
        public long create_time;

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
            bb.Read(ref this.id);
            bb.Read(ref this.account_id);
            bb.readLengthLimit = 0;
            bb.Read(ref this.nickname);
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
            str.Append(", \"id\":" + id);
            str.Append(", \"account_id\":" + account_id);
            if (nickname != null) str.Append(", \"nickname\":\"" + nickname + "\"");
            else str.Append(", \"nickname\":nil");
            str.Append(", \"create_time\":" + create_time);
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
        public virtual void MySqlAppend(ref System.Text.StringBuilder sb, bool ignoreReadOnly)
        {
            sb.Append("(");
            if (!ignoreReadOnly)
            {
            sb.Append(this.id);
            sb.Append(", ");
            }
            sb.Append(this.account_id);
            sb.Append(", ");
            sb.Append(this.nickname == null ? "null" : ("'" + this.nickname.Replace("'", "''") + "'"));
            sb.Append(", ");
            sb.Append(this.create_time);
            sb.Append(", ");
            sb.Length -= 2;
            sb.Append(")");

        }
    }
    /// <summary>
    /// 密码变更历史表
    /// </summary>
    public partial class log_account_password : IObject
    {
        public int id;
        /// <summary>
        /// 帐号id
        /// </summary>
        public int account_id;
        /// <summary>
        /// 密码
        /// </summary>
        public string password;
        /// <summary>
        /// 创建时间
        /// </summary>
        public long create_time;

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
            bb.Read(ref this.id);
            bb.Read(ref this.account_id);
            bb.readLengthLimit = 0;
            bb.Read(ref this.password);
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
            str.Append(", \"id\":" + id);
            str.Append(", \"account_id\":" + account_id);
            if (password != null) str.Append(", \"password\":\"" + password + "\"");
            else str.Append(", \"password\":nil");
            str.Append(", \"create_time\":" + create_time);
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
        public virtual void MySqlAppend(ref System.Text.StringBuilder sb, bool ignoreReadOnly)
        {
            sb.Append("(");
            if (!ignoreReadOnly)
            {
            sb.Append(this.id);
            sb.Append(", ");
            }
            sb.Append(this.account_id);
            sb.Append(", ");
            sb.Append(this.password == null ? "null" : ("'" + this.password.Replace("'", "''") + "'"));
            sb.Append(", ");
            sb.Append(this.create_time);
            sb.Append(", ");
            sb.Length -= 2;
            sb.Append(")");

        }
    }
    /// <summary>
    /// 电话号码变更历史表
    /// </summary>
    public partial class log_account_phone : IObject
    {
        public int id;
        /// <summary>
        /// 帐号id
        /// </summary>
        public int account_id;
        /// <summary>
        /// 电话号码
        /// </summary>
        public string phone;
        /// <summary>
        /// 创建时间
        /// </summary>
        public long create_time;

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
            bb.Read(ref this.id);
            bb.Read(ref this.account_id);
            bb.readLengthLimit = 0;
            bb.Read(ref this.phone);
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
            str.Append(", \"id\":" + id);
            str.Append(", \"account_id\":" + account_id);
            if (phone != null) str.Append(", \"phone\":\"" + phone + "\"");
            else str.Append(", \"phone\":nil");
            str.Append(", \"create_time\":" + create_time);
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
        public virtual void MySqlAppend(ref System.Text.StringBuilder sb, bool ignoreReadOnly)
        {
            sb.Append("(");
            if (!ignoreReadOnly)
            {
            sb.Append(this.id);
            sb.Append(", ");
            }
            sb.Append(this.account_id);
            sb.Append(", ");
            sb.Append(this.phone == null ? "null" : ("'" + this.phone.Replace("'", "''") + "'"));
            sb.Append(", ");
            sb.Append(this.create_time);
            sb.Append(", ");
            sb.Length -= 2;
            sb.Append(")");

        }
    }
    /// <summary>
    /// 充值记录表
    /// </summary>
    public partial class log_account_recharge : IObject
    {
        /// <summary>
        /// 充值请求原始令牌
        /// </summary>
        public string token;
        /// <summary>
        /// 帐号id
        /// </summary>
        public int account_id;
        /// <summary>
        /// 充值金额
        /// </summary>
        public double money;
        /// <summary>
        /// 备注( 渠道, 操作人等 )
        /// </summary>
        public string memo;
        /// <summary>
        /// 日志创建时间( 充值时间 )
        /// </summary>
        public long create_time;

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
            bb.Read(ref this.account_id);
            bb.Read(ref this.money);
            bb.readLengthLimit = 0;
            bb.Read(ref this.memo);
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
            str.Append(", \"account_id\":" + account_id);
            str.Append(", \"money\":" + money);
            if (memo != null) str.Append(", \"memo\":\"" + memo + "\"");
            else str.Append(", \"memo\":nil");
            str.Append(", \"create_time\":" + create_time);
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
        public virtual void MySqlAppend(ref System.Text.StringBuilder sb, bool ignoreReadOnly)
        {
            sb.Append("(");
            sb.Append(this.token == null ? "null" : ("'" + this.token.Replace("'", "''") + "'"));
            sb.Append(", ");
            sb.Append(this.account_id);
            sb.Append(", ");
            sb.Append(this.money);
            sb.Append(", ");
            sb.Append(this.memo == null ? "null" : ("'" + this.memo.Replace("'", "''") + "'"));
            sb.Append(", ");
            sb.Append(this.create_time);
            sb.Append(", ");
            sb.Length -= 2;
            sb.Append(")");

        }
    }
    /// <summary>
    /// 玩家实时对局数据日志表(基表 )
    /// </summary>
    public partial class log_game : IObject
    {
        public int id;
        /// <summary>
        /// 游戏id(game表id)
        /// </summary>
        public int game_id;

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
            bb.Read(ref this.id);
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
            str.Append(", \"id\":" + id);
            str.Append(", \"game_id\":" + game_id);
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
        public virtual void MySqlAppend(ref System.Text.StringBuilder sb, bool ignoreReadOnly)
        {
            sb.Append("(");
            if (!ignoreReadOnly)
            {
            sb.Append(this.id);
            sb.Append(", ");
            }
            sb.Append(this.game_id);
            sb.Append(", ");
            sb.Length -= 2;
            sb.Append(")");

        }
    }
    /// <summary>
    /// 捕鱼房内玩家实时对局数据日志表( 数据于玩家进游戏时创建, 出游戏时将继续填充这个表剩余字段 )
    /// </summary>
    public partial class log_game_catchfish : IObject
    {
        /// <summary>
        /// 游戏日志id(log_game表id)
        /// </summary>
        public int id;
        /// <summary>
        /// 游戏级别id
        /// </summary>
        public int level_id;
        /// <summary>
        /// 房号
        /// </summary>
        public int room_id;
        /// <summary>
        /// 账号id
        /// </summary>
        public int account_id;
        /// <summary>
        /// 进房时间
        /// </summary>
        public long enter_time;
        /// <summary>
        /// 进房时的账户余额
        /// </summary>
        public double enter_money;
        /// <summary>
        /// 离开时间( 日志记录创建时间 )
        /// </summary>
        public long leave_time;
        /// <summary>
        /// 离开时游戏内剩余金币换算成的金额
        /// </summary>
        public double leave_money;
        /// <summary>
        /// 当次游戏内花掉的金币换算成的金额总量
        /// </summary>
        public double consume;

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
            bb.Read(ref this.id);
            bb.Read(ref this.level_id);
            bb.Read(ref this.room_id);
            bb.Read(ref this.account_id);
            bb.Read(ref this.enter_time);
            bb.Read(ref this.enter_money);
            bb.Read(ref this.leave_time);
            bb.Read(ref this.leave_money);
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
            str.Append(", \"id\":" + id);
            str.Append(", \"level_id\":" + level_id);
            str.Append(", \"room_id\":" + room_id);
            str.Append(", \"account_id\":" + account_id);
            str.Append(", \"enter_time\":" + enter_time);
            str.Append(", \"enter_money\":" + enter_money);
            str.Append(", \"leave_time\":" + leave_time);
            str.Append(", \"leave_money\":" + leave_money);
            str.Append(", \"consume\":" + consume);
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
        public virtual void MySqlAppend(ref System.Text.StringBuilder sb, bool ignoreReadOnly)
        {
            sb.Append("(");
            sb.Append(this.id);
            sb.Append(", ");
            sb.Append(this.level_id);
            sb.Append(", ");
            sb.Append(this.room_id);
            sb.Append(", ");
            sb.Append(this.account_id);
            sb.Append(", ");
            sb.Append(this.enter_time);
            sb.Append(", ");
            sb.Append(this.enter_money);
            sb.Append(", ");
            sb.Append(this.leave_time);
            sb.Append(", ");
            sb.Append(this.leave_money);
            sb.Append(", ");
            sb.Append(this.consume);
            sb.Append(", ");
            sb.Length -= 2;
            sb.Append(")");

        }
    }
    /// <summary>
    /// 捕鱼玩家事件日志基表
    /// </summary>
    public partial class log_game_catchfish_event : IObject
    {
        /// <summary>
        /// 由游戏服自己生成的自增id. 用于派生表关联
        /// </summary>
        public int id;
        /// <summary>
        /// 所属对局记录id. 用于派生表关联
        /// </summary>
        public int log_game_catchfish_id;
        /// <summary>
        /// 事件类型编号. 1: fire   2: fishdie   3: bulletdie    4: bulletmiss
        /// </summary>
        public int event_type_id;
        /// <summary>
        /// 发生时的帧编号
        /// </summary>
        public int frame_number;
        /// <summary>
        /// 发生时间
        /// </summary>
        public long create_time;

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
            bb.Read(ref this.id);
            bb.Read(ref this.log_game_catchfish_id);
            bb.Read(ref this.event_type_id);
            bb.Read(ref this.frame_number);
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
            str.Append(", \"id\":" + id);
            str.Append(", \"log_game_catchfish_id\":" + log_game_catchfish_id);
            str.Append(", \"event_type_id\":" + event_type_id);
            str.Append(", \"frame_number\":" + frame_number);
            str.Append(", \"create_time\":" + create_time);
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
        public virtual void MySqlAppend(ref System.Text.StringBuilder sb, bool ignoreReadOnly)
        {
            sb.Append("(");
            sb.Append(this.id);
            sb.Append(", ");
            sb.Append(this.log_game_catchfish_id);
            sb.Append(", ");
            sb.Append(this.event_type_id);
            sb.Append(", ");
            sb.Append(this.frame_number);
            sb.Append(", ");
            sb.Append(this.create_time);
            sb.Append(", ");
            sb.Length -= 2;
            sb.Append(")");

        }
    }
    /// <summary>
    /// 捕鱼玩家事件日志之子弹没打死鱼, 消耗掉了(bulletdie)
    /// </summary>
    public partial class log_game_catchfish_event_bulletdie : IObject
    {
        /// <summary>
        /// 关联到基表
        /// </summary>
        public int id;
        /// <summary>
        /// 关联到基表
        /// </summary>
        public int log_game_catchfish_id;
        /// <summary>
        /// 所属类型编号. 0: 普通子弹    1: 钻头蟹    2: 炸弹蟹    3: 闪电鲨
        /// </summary>
        public int type_id;
        /// <summary>
        /// 子弹序号
        /// </summary>
        public int bullet_id;
        /// <summary>
        /// 子弹金币数/倍率/强度
        /// </summary>
        public long coin;
        /// <summary>
        /// 折算成 account.money 相同货币单位的金额
        /// </summary>
        public double money;

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
            bb.Read(ref this.id);
            bb.Read(ref this.log_game_catchfish_id);
            bb.Read(ref this.type_id);
            bb.Read(ref this.bullet_id);
            bb.Read(ref this.coin);
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
            str.Append(", \"id\":" + id);
            str.Append(", \"log_game_catchfish_id\":" + log_game_catchfish_id);
            str.Append(", \"type_id\":" + type_id);
            str.Append(", \"bullet_id\":" + bullet_id);
            str.Append(", \"coin\":" + coin);
            str.Append(", \"money\":" + money);
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
        public virtual void MySqlAppend(ref System.Text.StringBuilder sb, bool ignoreReadOnly)
        {
            sb.Append("(");
            sb.Append(this.id);
            sb.Append(", ");
            sb.Append(this.log_game_catchfish_id);
            sb.Append(", ");
            sb.Append(this.type_id);
            sb.Append(", ");
            sb.Append(this.bullet_id);
            sb.Append(", ");
            sb.Append(this.coin);
            sb.Append(", ");
            sb.Append(this.money);
            sb.Append(", ");
            sb.Length -= 2;
            sb.Append(")");

        }
    }
    /// <summary>
    /// 捕鱼玩家事件日志之子弹打空, 得返还(bulletmiss)
    /// </summary>
    public partial class log_game_catchfish_event_bulletmiss : IObject
    {
        /// <summary>
        /// 关联到基表
        /// </summary>
        public int id;
        /// <summary>
        /// 关联到基表
        /// </summary>
        public int log_game_catchfish_id;
        /// <summary>
        /// 所属类型编号. 0: 普通子弹    1: 钻头蟹    2: 炸弹蟹    3: 闪电鲨
        /// </summary>
        public int type_id;
        /// <summary>
        /// 子弹序号
        /// </summary>
        public int bullet_id;
        /// <summary>
        /// 子弹金币数/倍率/强度
        /// </summary>
        public long coin;
        /// <summary>
        /// 折算成 account.money 相同货币单位的金额
        /// </summary>
        public double money;

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
            bb.Read(ref this.id);
            bb.Read(ref this.log_game_catchfish_id);
            bb.Read(ref this.type_id);
            bb.Read(ref this.bullet_id);
            bb.Read(ref this.coin);
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
            str.Append(", \"id\":" + id);
            str.Append(", \"log_game_catchfish_id\":" + log_game_catchfish_id);
            str.Append(", \"type_id\":" + type_id);
            str.Append(", \"bullet_id\":" + bullet_id);
            str.Append(", \"coin\":" + coin);
            str.Append(", \"money\":" + money);
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
        public virtual void MySqlAppend(ref System.Text.StringBuilder sb, bool ignoreReadOnly)
        {
            sb.Append("(");
            sb.Append(this.id);
            sb.Append(", ");
            sb.Append(this.log_game_catchfish_id);
            sb.Append(", ");
            sb.Append(this.type_id);
            sb.Append(", ");
            sb.Append(this.bullet_id);
            sb.Append(", ");
            sb.Append(this.coin);
            sb.Append(", ");
            sb.Append(this.money);
            sb.Append(", ");
            sb.Length -= 2;
            sb.Append(")");

        }
    }
    /// <summary>
    /// 捕鱼玩家事件日志之开火(fire) 注意: 非普通子弹属于服务器强制开火
    /// </summary>
    public partial class log_game_catchfish_event_fire : IObject
    {
        /// <summary>
        /// 关联到基表
        /// </summary>
        public int id;
        /// <summary>
        /// 关联到基表
        /// </summary>
        public int log_game_catchfish_id;
        /// <summary>
        /// 所属类型编号. 0: 普通子弹    1: 钻头蟹    2: 炸弹蟹    3: 闪电鲨
        /// </summary>
        public int type_id;
        /// <summary>
        /// 子弹序号
        /// </summary>
        public int bullet_id;
        /// <summary>
        /// 子弹金币数/倍率/强度( 爆炸等效果大批量剩余子弹可合并填写, miss 表也对应合并 )
        /// </summary>
        public long coin;
        /// <summary>
        /// 折算成 account.money 相同货币单位的金额
        /// </summary>
        public double money;

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
            bb.Read(ref this.id);
            bb.Read(ref this.log_game_catchfish_id);
            bb.Read(ref this.type_id);
            bb.Read(ref this.bullet_id);
            bb.Read(ref this.coin);
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
            str.Append(", \"id\":" + id);
            str.Append(", \"log_game_catchfish_id\":" + log_game_catchfish_id);
            str.Append(", \"type_id\":" + type_id);
            str.Append(", \"bullet_id\":" + bullet_id);
            str.Append(", \"coin\":" + coin);
            str.Append(", \"money\":" + money);
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
        public virtual void MySqlAppend(ref System.Text.StringBuilder sb, bool ignoreReadOnly)
        {
            sb.Append("(");
            sb.Append(this.id);
            sb.Append(", ");
            sb.Append(this.log_game_catchfish_id);
            sb.Append(", ");
            sb.Append(this.type_id);
            sb.Append(", ");
            sb.Append(this.bullet_id);
            sb.Append(", ");
            sb.Append(this.coin);
            sb.Append(", ");
            sb.Append(this.money);
            sb.Append(", ");
            sb.Length -= 2;
            sb.Append(")");

        }
    }
    /// <summary>
    /// 捕鱼玩家事件日志之打死鱼得到钱
    /// </summary>
    public partial class log_game_catchfish_event_fishdie : IObject
    {
        /// <summary>
        /// 关联到基表
        /// </summary>
        public int id;
        /// <summary>
        /// 关联到基表
        /// </summary>
        public int log_game_catchfish_id;
        /// <summary>
        /// 鱼类型/配置编号. miss 情况下为 -1
        /// </summary>
        public int fish_type_id;
        /// <summary>
        /// 鱼内部编号. miss 情况下为 0
        /// </summary>
        public int fish_id;
        /// <summary>
        /// 鱼倍率. miss 情况下为 1
        /// </summary>
        public long fish_ratio;
        /// <summary>
        /// 子弹金币数
        /// </summary>
        public long bullet_coin;
        /// <summary>
        /// 最终玩家所得 = 子弹金币数 * 鱼倍率
        /// </summary>
        public long coin;
        /// <summary>
        /// 最终玩家所得 折算成 account.money 相同货币单位的金额
        /// </summary>
        public double money;

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
            bb.Read(ref this.id);
            bb.Read(ref this.log_game_catchfish_id);
            bb.Read(ref this.fish_type_id);
            bb.Read(ref this.fish_id);
            bb.Read(ref this.fish_ratio);
            bb.Read(ref this.bullet_coin);
            bb.Read(ref this.coin);
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
            str.Append(", \"id\":" + id);
            str.Append(", \"log_game_catchfish_id\":" + log_game_catchfish_id);
            str.Append(", \"fish_type_id\":" + fish_type_id);
            str.Append(", \"fish_id\":" + fish_id);
            str.Append(", \"fish_ratio\":" + fish_ratio);
            str.Append(", \"bullet_coin\":" + bullet_coin);
            str.Append(", \"coin\":" + coin);
            str.Append(", \"money\":" + money);
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
        public virtual void MySqlAppend(ref System.Text.StringBuilder sb, bool ignoreReadOnly)
        {
            sb.Append("(");
            sb.Append(this.id);
            sb.Append(", ");
            sb.Append(this.log_game_catchfish_id);
            sb.Append(", ");
            sb.Append(this.fish_type_id);
            sb.Append(", ");
            sb.Append(this.fish_id);
            sb.Append(", ");
            sb.Append(this.fish_ratio);
            sb.Append(", ");
            sb.Append(this.bullet_coin);
            sb.Append(", ");
            sb.Append(this.coin);
            sb.Append(", ");
            sb.Append(this.money);
            sb.Append(", ");
            sb.Length -= 2;
            sb.Append(")");

        }
    }
    /// <summary>
    /// 记录玩家上下线情况的日志. 分4种情况:   1. 上线     2. 断线     3. 重连.    4. 下线
    /// </summary>
    public partial class log_login_out : IObject
    {
        public int id;
        /// <summary>
        /// 帐号id
        /// </summary>
        public int account_id;
        /// <summary>
        /// 1. 上线     2. 断线     3. 重连.    4. 下线
        /// </summary>
        public int type_id;
        /// <summary>
        /// 上线或重连时的IP地址
        /// </summary>
        public string ip;
        /// <summary>
        /// 创建时间
        /// </summary>
        public long create_time;

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
            bb.Read(ref this.id);
            bb.Read(ref this.account_id);
            bb.Read(ref this.type_id);
            bb.readLengthLimit = 0;
            bb.Read(ref this.ip);
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
            str.Append(", \"id\":" + id);
            str.Append(", \"account_id\":" + account_id);
            str.Append(", \"type_id\":" + type_id);
            if (ip != null) str.Append(", \"ip\":\"" + ip + "\"");
            else str.Append(", \"ip\":nil");
            str.Append(", \"create_time\":" + create_time);
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
        public virtual void MySqlAppend(ref System.Text.StringBuilder sb, bool ignoreReadOnly)
        {
            sb.Append("(");
            if (!ignoreReadOnly)
            {
            sb.Append(this.id);
            sb.Append(", ");
            }
            sb.Append(this.account_id);
            sb.Append(", ");
            sb.Append(this.type_id);
            sb.Append(", ");
            sb.Append(this.ip == null ? "null" : ("'" + this.ip.Replace("'", "''") + "'"));
            sb.Append(", ");
            sb.Append(this.create_time);
            sb.Append(", ");
            sb.Length -= 2;
            sb.Append(")");

        }
    }
    /// <summary>
    /// 申请提现状态修改日志表
    /// </summary>
    public partial class log_withdrawals : IObject
    {
        public int id;
        /// <summary>
        /// 申请提现表id
        /// </summary>
        public int withdrawals_id;
        /// <summary>
        /// 操作人(客服)
        /// </summary>
        public string person_in_charge;
        /// <summary>
        /// 操作状态
        /// </summary>
        public int state_id;
        /// <summary>
        /// 操作描述
        /// </summary>
        public string description;
        /// <summary>
        /// 创建时间
        /// </summary>
        public long create_time;

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
            bb.Read(ref this.id);
            bb.Read(ref this.withdrawals_id);
            bb.readLengthLimit = 0;
            bb.Read(ref this.person_in_charge);
            bb.Read(ref this.state_id);
            bb.readLengthLimit = 0;
            bb.Read(ref this.description);
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
            str.Append(", \"id\":" + id);
            str.Append(", \"withdrawals_id\":" + withdrawals_id);
            if (person_in_charge != null) str.Append(", \"person_in_charge\":\"" + person_in_charge + "\"");
            else str.Append(", \"person_in_charge\":nil");
            str.Append(", \"state_id\":" + state_id);
            if (description != null) str.Append(", \"description\":\"" + description + "\"");
            else str.Append(", \"description\":nil");
            str.Append(", \"create_time\":" + create_time);
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
        public virtual void MySqlAppend(ref System.Text.StringBuilder sb, bool ignoreReadOnly)
        {
            sb.Append("(");
            if (!ignoreReadOnly)
            {
            sb.Append(this.id);
            sb.Append(", ");
            }
            sb.Append(this.withdrawals_id);
            sb.Append(", ");
            sb.Append(this.person_in_charge == null ? "null" : ("'" + this.person_in_charge.Replace("'", "''") + "'"));
            sb.Append(", ");
            sb.Append(this.state_id);
            sb.Append(", ");
            sb.Append(this.description == null ? "null" : ("'" + this.description.Replace("'", "''") + "'"));
            sb.Append(", ");
            sb.Append(this.create_time);
            sb.Append(", ");
            sb.Length -= 2;
            sb.Append(")");

        }
    }
    /// <summary>
    /// 邮箱表
    /// </summary>
    public partial class mail : IObject
    {
        public int id;
        /// <summary>
        /// 内容
        /// </summary>
        public string content;
        /// <summary>
        /// 收件人
        /// </summary>
        public int account_id;
        /// <summary>
        /// 发件人
        /// </summary>
        public string sender;
        /// <summary>
        /// 创建时间
        /// </summary>
        public long create_time;
        /// <summary>
        /// 是否已读(0未读, 1已读)
        /// </summary>
        public int is_read;

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
            bb.Read(ref this.id);
            bb.readLengthLimit = 0;
            bb.Read(ref this.content);
            bb.Read(ref this.account_id);
            bb.readLengthLimit = 0;
            bb.Read(ref this.sender);
            bb.Read(ref this.create_time);
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
            str.Append(", \"id\":" + id);
            if (content != null) str.Append(", \"content\":\"" + content + "\"");
            else str.Append(", \"content\":nil");
            str.Append(", \"account_id\":" + account_id);
            if (sender != null) str.Append(", \"sender\":\"" + sender + "\"");
            else str.Append(", \"sender\":nil");
            str.Append(", \"create_time\":" + create_time);
            str.Append(", \"is_read\":" + is_read);
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
        public virtual void MySqlAppend(ref System.Text.StringBuilder sb, bool ignoreReadOnly)
        {
            sb.Append("(");
            if (!ignoreReadOnly)
            {
            sb.Append(this.id);
            sb.Append(", ");
            }
            sb.Append(this.content == null ? "null" : ("'" + this.content.Replace("'", "''") + "'"));
            sb.Append(", ");
            sb.Append(this.account_id);
            sb.Append(", ");
            sb.Append(this.sender == null ? "null" : ("'" + this.sender.Replace("'", "''") + "'"));
            sb.Append(", ");
            sb.Append(this.create_time);
            sb.Append(", ");
            sb.Append(this.is_read);
            sb.Append(", ");
            sb.Length -= 2;
            sb.Append(")");

        }
    }
    /// <summary>
    /// 跑马灯表
    /// </summary>
    public partial class marquee : IObject
    {
        /// <summary>
        /// 自增id
        /// </summary>
        public int id;
        /// <summary>
        /// 内容
        /// </summary>
        public string content;
        /// <summary>
        /// 喊话频率(多长时间循环执行一次，单位 秒)
        /// </summary>
        public int frequency;
        /// <summary>
        /// 更新时间
        /// </summary>
        public long update_time;
        /// <summary>
        /// 创建时间
        /// </summary>
        public long create_time;
        /// <summary>
        /// 是否有效(1有效)
        /// </summary>
        public int enable;

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
            bb.Read(ref this.id);
            bb.readLengthLimit = 0;
            bb.Read(ref this.content);
            bb.Read(ref this.frequency);
            bb.Read(ref this.update_time);
            bb.Read(ref this.create_time);
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
            str.Append(", \"id\":" + id);
            if (content != null) str.Append(", \"content\":\"" + content + "\"");
            else str.Append(", \"content\":nil");
            str.Append(", \"frequency\":" + frequency);
            str.Append(", \"update_time\":" + update_time);
            str.Append(", \"create_time\":" + create_time);
            str.Append(", \"enable\":" + enable);
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
        public virtual void MySqlAppend(ref System.Text.StringBuilder sb, bool ignoreReadOnly)
        {
            sb.Append("(");
            if (!ignoreReadOnly)
            {
            sb.Append(this.id);
            sb.Append(", ");
            }
            sb.Append(this.content == null ? "null" : ("'" + this.content.Replace("'", "''") + "'"));
            sb.Append(", ");
            sb.Append(this.frequency);
            sb.Append(", ");
            sb.Append(this.update_time);
            sb.Append(", ");
            sb.Append(this.create_time);
            sb.Append(", ");
            sb.Append(this.enable);
            sb.Append(", ");
            sb.Length -= 2;
            sb.Append(")");

        }
    }
    /// <summary>
    /// 公告表
    /// </summary>
    public partial class notice : IObject
    {
        public int id;
        /// <summary>
        /// 公告内容
        /// </summary>
        public string content;
        /// <summary>
        /// 公告类型
        /// </summary>
        public int notice_type_id;
        /// <summary>
        /// 有效起始时间
        /// </summary>
        public long? start_time;
        /// <summary>
        /// 有效结束时间
        /// </summary>
        public long? end_time;
        /// <summary>
        /// 创建时间
        /// </summary>
        public long create_time;

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
            bb.Read(ref this.id);
            bb.readLengthLimit = 0;
            bb.Read(ref this.content);
            bb.Read(ref this.notice_type_id);
            bb.Read(ref this.start_time);
            bb.Read(ref this.end_time);
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
            str.Append(", \"id\":" + id);
            if (content != null) str.Append(", \"content\":\"" + content + "\"");
            else str.Append(", \"content\":nil");
            str.Append(", \"notice_type_id\":" + notice_type_id);
            str.Append(", \"start_time\":" + (start_time.HasValue ? start_time.Value.ToString() : "nil"));
            str.Append(", \"end_time\":" + (end_time.HasValue ? end_time.Value.ToString() : "nil"));
            str.Append(", \"create_time\":" + create_time);
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
        public virtual void MySqlAppend(ref System.Text.StringBuilder sb, bool ignoreReadOnly)
        {
            sb.Append("(");
            if (!ignoreReadOnly)
            {
            sb.Append(this.id);
            sb.Append(", ");
            }
            sb.Append(this.content == null ? "null" : ("'" + this.content.Replace("'", "''") + "'"));
            sb.Append(", ");
            sb.Append(this.notice_type_id);
            sb.Append(", ");
            sb.Append(this.start_time.HasValue ? this.start_time.Value.ToString() : "null");
            sb.Append(", ");
            sb.Append(this.end_time.HasValue ? this.end_time.Value.ToString() : "null");
            sb.Append(", ");
            sb.Append(this.create_time);
            sb.Append(", ");
            sb.Length -= 2;
            sb.Append(")");

        }
    }
    /// <summary>
    /// 公告读取表
    /// </summary>
    public partial class notice_read : IObject
    {
        /// <summary>
        /// 公告id
        /// </summary>
        public int notice_id;
        /// <summary>
        /// 玩家id
        /// </summary>
        public int account_id;
        /// <summary>
        /// 创建时间
        /// </summary>
        public long create_time;

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
            bb.Read(ref this.notice_id);
            bb.Read(ref this.account_id);
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
            str.Append(", \"notice_id\":" + notice_id);
            str.Append(", \"account_id\":" + account_id);
            str.Append(", \"create_time\":" + create_time);
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
        public virtual void MySqlAppend(ref System.Text.StringBuilder sb, bool ignoreReadOnly)
        {
            sb.Append("(");
            sb.Append(this.notice_id);
            sb.Append(", ");
            sb.Append(this.account_id);
            sb.Append(", ");
            sb.Append(this.create_time);
            sb.Append(", ");
            sb.Length -= 2;
            sb.Append(")");

        }
    }
    /// <summary>
    /// 公告类型表
    /// </summary>
    public partial class notice_type : IObject
    {
        public int id;
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
            str.Append(", \"id\":" + id);
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
        public virtual void MySqlAppend(ref System.Text.StringBuilder sb, bool ignoreReadOnly)
        {
            sb.Append("(");
            sb.Append(this.id);
            sb.Append(", ");
            sb.Append(this.name == null ? "null" : ("'" + this.name.Replace("'", "''") + "'"));
            sb.Append(", ");
            sb.Length -= 2;
            sb.Append(")");

        }
    }
    /// <summary>
    /// 短信校验状态数据( 数据有有效期. 数据服定时删掉超时数据 )
    /// </summary>
    public partial class verify_code : IObject
    {
        /// <summary>
        /// 收短信的电话号码
        /// </summary>
        public string phone;
        /// <summary>
        /// 短信验证码内容
        /// </summary>
        public string content;
        /// <summary>
        /// 下发时间( 用于判断数据是否过期, 过期就删除 )
        /// </summary>
        public long create_time;

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
            str.Append(", \"create_time\":" + create_time);
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
        public virtual void MySqlAppend(ref System.Text.StringBuilder sb, bool ignoreReadOnly)
        {
            sb.Append("(");
            sb.Append(this.phone == null ? "null" : ("'" + this.phone.Replace("'", "''") + "'"));
            sb.Append(", ");
            sb.Append(this.content == null ? "null" : ("'" + this.content.Replace("'", "''") + "'"));
            sb.Append(", ");
            sb.Append(this.create_time);
            sb.Append(", ");
            sb.Length -= 2;
            sb.Append(")");

        }
    }
    /// <summary>
    /// VIP与累计充值金额的对应表
    /// </summary>
    public partial class vip : IObject
    {
        /// <summary>
        /// 唯一编号 & 级别
        /// </summary>
        public int id;
        /// <summary>
        /// 累计充值金额要求
        /// </summary>
        public double total_recharge;

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
            bb.Read(ref this.id);
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
            str.Append(", \"id\":" + id);
            str.Append(", \"total_recharge\":" + total_recharge);
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
        public virtual void MySqlAppend(ref System.Text.StringBuilder sb, bool ignoreReadOnly)
        {
            sb.Append("(");
            sb.Append(this.id);
            sb.Append(", ");
            sb.Append(this.total_recharge);
            sb.Append(", ");
            sb.Length -= 2;
            sb.Append(")");

        }
    }
    /// <summary>
    /// 申请提现表
    /// </summary>
    public partial class withdrawals : IObject
    {
        /// <summary>
        /// 提现订单号( 随机 9 位整数 )
        /// </summary>
        public int withdrawals_id;
        /// <summary>
        /// 帐号id
        /// </summary>
        public int account_id;
        /// <summary>
        /// 提现金额
        /// </summary>
        public double money;
        /// <summary>
        /// 提现请求( 渠道, 账号等 )
        /// </summary>
        public string requirement;
        /// <summary>
        /// 操作状态
        /// </summary>
        public int state_id;
        /// <summary>
        /// 操作描述
        /// </summary>
        public string description;
        /// <summary>
        /// 提现申请时间
        /// </summary>
        public long create_time;
        /// <summary>
        /// 最后处理时间
        /// </summary>
        public long last_create_time;

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
            bb.Read(ref this.withdrawals_id);
            bb.Read(ref this.account_id);
            bb.Read(ref this.money);
            bb.readLengthLimit = 0;
            bb.Read(ref this.requirement);
            bb.Read(ref this.state_id);
            bb.readLengthLimit = 0;
            bb.Read(ref this.description);
            bb.Read(ref this.create_time);
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
            str.Append(", \"withdrawals_id\":" + withdrawals_id);
            str.Append(", \"account_id\":" + account_id);
            str.Append(", \"money\":" + money);
            if (requirement != null) str.Append(", \"requirement\":\"" + requirement + "\"");
            else str.Append(", \"requirement\":nil");
            str.Append(", \"state_id\":" + state_id);
            if (description != null) str.Append(", \"description\":\"" + description + "\"");
            else str.Append(", \"description\":nil");
            str.Append(", \"create_time\":" + create_time);
            str.Append(", \"last_create_time\":" + last_create_time);
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
        public virtual void MySqlAppend(ref System.Text.StringBuilder sb, bool ignoreReadOnly)
        {
            sb.Append("(");
            sb.Append(this.withdrawals_id);
            sb.Append(", ");
            sb.Append(this.account_id);
            sb.Append(", ");
            sb.Append(this.money);
            sb.Append(", ");
            sb.Append(this.requirement == null ? "null" : ("'" + this.requirement.Replace("'", "''") + "'"));
            sb.Append(", ");
            sb.Append(this.state_id);
            sb.Append(", ");
            sb.Append(this.description == null ? "null" : ("'" + this.description.Replace("'", "''") + "'"));
            sb.Append(", ");
            sb.Append(this.create_time);
            sb.Append(", ");
            sb.Append(this.last_create_time);
            sb.Append(", ");
            sb.Length -= 2;
            sb.Append(")");

        }
    }
    /// <summary>
    /// 提现状态表
    /// </summary>
    public partial class withdrawals_state : IObject
    {
        public int id;
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
            str.Append(", \"id\":" + id);
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
        public virtual void MySqlAppend(ref System.Text.StringBuilder sb, bool ignoreReadOnly)
        {
            sb.Append("(");
            sb.Append(this.id);
            sb.Append(", ");
            sb.Append(this.name == null ? "null" : ("'" + this.name.Replace("'", "''") + "'"));
            sb.Append(", ");
            sb.Length -= 2;
            sb.Append(")");

        }
    }
    public partial class node : IObject
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
        public virtual void MySqlAppend(ref System.Text.StringBuilder sb, bool ignoreReadOnly)
        {
            sb.Append("(");
            sb.Append(this.id);
            sb.Append(", ");
            sb.Append(this.pid.HasValue ? this.pid.Value.ToString() : "null");
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
            xx.BBuffer.RegisterInternals();
            BBuffer.Register<Foo>(3);
            BBuffer.Register<List<Foo>>(4);
            BBuffer.Register<List<IObject>>(5);
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
