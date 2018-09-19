using System;
namespace RPC
{
    public static class PkgGenMd5
    {
        public const string value = "dfd6327ef3db0b69dc7ca345d134ba72"; 
    }

namespace Generic
{
    /// <summary>
    /// 服务类型列表
    /// </summary>
    public enum ServiceTypes : int
    {
        /// <summary>
        /// 登录
        /// </summary>
        Login = 0,
        /// <summary>
        /// 数据库
        /// </summary>
        DB = 1,
        /// <summary>
        /// 管理端
        /// </summary>
        Manage = 2,
    }
}
namespace DB_Manage
{
    public partial class MsgResult : xx.Object
    {
        public string txt;

        public override ushort GetPackageId()
        {
            return xx.TypeId<MsgResult>.value;
        }

        public override void ToBBuffer(xx.BBuffer bb)
        {
            bb.Write(this.txt);
        }

        public override void FromBBuffer(xx.BBuffer bb)
        {
            bb.readLengthLimit = 0;
            bb.Read(ref this.txt);
        }
        public override void ToString(System.Text.StringBuilder s)
        {
            if (__toStringing)
            {
        	    s.Append("[ \"***** recursived *****\" ]");
        	    return;
            }
            else __toStringing = true;

            s.Append("{ \"pkgTypeName\":\"DB_Manage.MsgResult\", \"pkgTypeId\":" + GetPackageId());
            ToStringCore(s);
            s.Append(" }");

            __toStringing = false;
        }
        public override void ToStringCore(System.Text.StringBuilder s)
        {
            if (txt != null) s.Append(", \"txt\":\"" + txt.ToString() + "\"");
            else s.Append(", \"txt\":nil");
        }
        public override string ToString()
        {
            var sb = new System.Text.StringBuilder();
            ToString(sb);
            return sb.ToString();
        }
        public override void MySqlAppend(System.Text.StringBuilder sb, bool ignoreReadOnly)
        {
        }
    }
}
namespace Manage_DB
{
    public partial class Msg : xx.Object
    {
        public string txt;

        public override ushort GetPackageId()
        {
            return xx.TypeId<Msg>.value;
        }

        public override void ToBBuffer(xx.BBuffer bb)
        {
            bb.Write(this.txt);
        }

        public override void FromBBuffer(xx.BBuffer bb)
        {
            bb.readLengthLimit = 200;
            bb.Read(ref this.txt);
        }
        public override void ToString(System.Text.StringBuilder s)
        {
            if (__toStringing)
            {
        	    s.Append("[ \"***** recursived *****\" ]");
        	    return;
            }
            else __toStringing = true;

            s.Append("{ \"pkgTypeName\":\"Manage_DB.Msg\", \"pkgTypeId\":" + GetPackageId());
            ToStringCore(s);
            s.Append(" }");

            __toStringing = false;
        }
        public override void ToStringCore(System.Text.StringBuilder s)
        {
            if (txt != null) s.Append(", \"txt\":\"" + txt.ToString() + "\"");
            else s.Append(", \"txt\":nil");
        }
        public override string ToString()
        {
            var sb = new System.Text.StringBuilder();
            ToString(sb);
            return sb.ToString();
        }
        public override void MySqlAppend(System.Text.StringBuilder sb, bool ignoreReadOnly)
        {
        }
    }
}
namespace Login_Client
{
    public partial class LoginSuccess : xx.Object
    {
        public int id;

        public override ushort GetPackageId()
        {
            return xx.TypeId<LoginSuccess>.value;
        }

        public override void ToBBuffer(xx.BBuffer bb)
        {
            bb.Write(this.id);
        }

        public override void FromBBuffer(xx.BBuffer bb)
        {
            bb.Read(ref this.id);
        }
        public override void ToString(System.Text.StringBuilder s)
        {
            if (__toStringing)
            {
        	    s.Append("[ \"***** recursived *****\" ]");
        	    return;
            }
            else __toStringing = true;

            s.Append("{ \"pkgTypeName\":\"Login_Client.LoginSuccess\", \"pkgTypeId\":" + GetPackageId());
            ToStringCore(s);
            s.Append(" }");

            __toStringing = false;
        }
        public override void ToStringCore(System.Text.StringBuilder s)
        {
            s.Append(", \"id\":" + id.ToString());
        }
        public override string ToString()
        {
            var sb = new System.Text.StringBuilder();
            ToString(sb);
            return sb.ToString();
        }
        public override void MySqlAppend(System.Text.StringBuilder sb, bool ignoreReadOnly)
        {
        }
    }
}
namespace Client_Login
{
    public partial class Login : xx.Object
    {
        public string username;
        public string password;

        public override ushort GetPackageId()
        {
            return xx.TypeId<Login>.value;
        }

        public override void ToBBuffer(xx.BBuffer bb)
        {
            bb.Write(this.username);
            bb.Write(this.password);
        }

        public override void FromBBuffer(xx.BBuffer bb)
        {
            bb.readLengthLimit = 50;
            bb.Read(ref this.username);
            bb.readLengthLimit = 50;
            bb.Read(ref this.password);
        }
        public override void ToString(System.Text.StringBuilder s)
        {
            if (__toStringing)
            {
        	    s.Append("[ \"***** recursived *****\" ]");
        	    return;
            }
            else __toStringing = true;

            s.Append("{ \"pkgTypeName\":\"Client_Login.Login\", \"pkgTypeId\":" + GetPackageId());
            ToStringCore(s);
            s.Append(" }");

            __toStringing = false;
        }
        public override void ToStringCore(System.Text.StringBuilder s)
        {
            if (username != null) s.Append(", \"username\":\"" + username.ToString() + "\"");
            else s.Append(", \"username\":nil");
            if (password != null) s.Append(", \"password\":\"" + password.ToString() + "\"");
            else s.Append(", \"password\":nil");
        }
        public override string ToString()
        {
            var sb = new System.Text.StringBuilder();
            ToString(sb);
            return sb.ToString();
        }
        public override void MySqlAppend(System.Text.StringBuilder sb, bool ignoreReadOnly)
        {
        }
    }
}
namespace DB_Login
{
    public partial class AuthSuccess : xx.Object
    {
        public int id;

        public override ushort GetPackageId()
        {
            return xx.TypeId<AuthSuccess>.value;
        }

        public override void ToBBuffer(xx.BBuffer bb)
        {
            bb.Write(this.id);
        }

        public override void FromBBuffer(xx.BBuffer bb)
        {
            bb.Read(ref this.id);
        }
        public override void ToString(System.Text.StringBuilder s)
        {
            if (__toStringing)
            {
        	    s.Append("[ \"***** recursived *****\" ]");
        	    return;
            }
            else __toStringing = true;

            s.Append("{ \"pkgTypeName\":\"DB_Login.AuthSuccess\", \"pkgTypeId\":" + GetPackageId());
            ToStringCore(s);
            s.Append(" }");

            __toStringing = false;
        }
        public override void ToStringCore(System.Text.StringBuilder s)
        {
            s.Append(", \"id\":" + id.ToString());
        }
        public override string ToString()
        {
            var sb = new System.Text.StringBuilder();
            ToString(sb);
            return sb.ToString();
        }
        public override void MySqlAppend(System.Text.StringBuilder sb, bool ignoreReadOnly)
        {
        }
    }
}
namespace Login_DB
{
    public partial class Auth : xx.Object
    {
        public string username;
        public string password;

        public override ushort GetPackageId()
        {
            return xx.TypeId<Auth>.value;
        }

        public override void ToBBuffer(xx.BBuffer bb)
        {
            bb.Write(this.username);
            bb.Write(this.password);
        }

        public override void FromBBuffer(xx.BBuffer bb)
        {
            bb.readLengthLimit = 0;
            bb.Read(ref this.username);
            bb.readLengthLimit = 0;
            bb.Read(ref this.password);
        }
        public override void ToString(System.Text.StringBuilder s)
        {
            if (__toStringing)
            {
        	    s.Append("[ \"***** recursived *****\" ]");
        	    return;
            }
            else __toStringing = true;

            s.Append("{ \"pkgTypeName\":\"Login_DB.Auth\", \"pkgTypeId\":" + GetPackageId());
            ToStringCore(s);
            s.Append(" }");

            __toStringing = false;
        }
        public override void ToStringCore(System.Text.StringBuilder s)
        {
            if (username != null) s.Append(", \"username\":\"" + username.ToString() + "\"");
            else s.Append(", \"username\":nil");
            if (password != null) s.Append(", \"password\":\"" + password.ToString() + "\"");
            else s.Append(", \"password\":nil");
        }
        public override string ToString()
        {
            var sb = new System.Text.StringBuilder();
            ToString(sb);
            return sb.ToString();
        }
        public override void MySqlAppend(System.Text.StringBuilder sb, bool ignoreReadOnly)
        {
        }
    }
}
namespace Generic
{
    /// <summary>
    /// 服务间创建连接, 刚连上时, 首包需要发这个来说明自己是谁
    /// </summary>
    public partial class ServiceInfo : xx.Object
    {
        /// <summary>
        /// 服务类型
        /// </summary>
        public Generic.ServiceTypes type;

        public override ushort GetPackageId()
        {
            return xx.TypeId<ServiceInfo>.value;
        }

        public override void ToBBuffer(xx.BBuffer bb)
        {
            bb.Write((int)this.type);
        }

        public override void FromBBuffer(xx.BBuffer bb)
        {
            {
                int tmp = 0;
                bb.Read(ref tmp);
                this.type = (Generic.ServiceTypes)tmp;
            }
        }
        public override void ToString(System.Text.StringBuilder s)
        {
            if (__toStringing)
            {
        	    s.Append("[ \"***** recursived *****\" ]");
        	    return;
            }
            else __toStringing = true;

            s.Append("{ \"pkgTypeName\":\"Generic.ServiceInfo\", \"pkgTypeId\":" + GetPackageId());
            ToStringCore(s);
            s.Append(" }");

            __toStringing = false;
        }
        public override void ToStringCore(System.Text.StringBuilder s)
        {
            s.Append(", \"type\":" + type.ToString());
        }
        public override string ToString()
        {
            var sb = new System.Text.StringBuilder();
            ToString(sb);
            return sb.ToString();
        }
        public override void MySqlAppend(System.Text.StringBuilder sb, bool ignoreReadOnly)
        {
        }
    }
    /// <summary>
    /// 通用错误返回
    /// </summary>
    public partial class Error : xx.Object
    {
        /// <summary>
        /// 错误码
        /// </summary>
        public int errNo;
        /// <summary>
        /// 错误文字
        /// </summary>
        public string errMsg;

        public override ushort GetPackageId()
        {
            return xx.TypeId<Error>.value;
        }

        public override void ToBBuffer(xx.BBuffer bb)
        {
            bb.Write(this.errNo);
            bb.Write(this.errMsg);
        }

        public override void FromBBuffer(xx.BBuffer bb)
        {
            bb.Read(ref this.errNo);
            bb.readLengthLimit = 0;
            bb.Read(ref this.errMsg);
        }
        public override void ToString(System.Text.StringBuilder s)
        {
            if (__toStringing)
            {
        	    s.Append("[ \"***** recursived *****\" ]");
        	    return;
            }
            else __toStringing = true;

            s.Append("{ \"pkgTypeName\":\"Generic.Error\", \"pkgTypeId\":" + GetPackageId());
            ToStringCore(s);
            s.Append(" }");

            __toStringing = false;
        }
        public override void ToStringCore(System.Text.StringBuilder s)
        {
            s.Append(", \"errNo\":" + errNo.ToString());
            if (errMsg != null) s.Append(", \"errMsg\":\"" + errMsg.ToString() + "\"");
            else s.Append(", \"errMsg\":nil");
        }
        public override string ToString()
        {
            var sb = new System.Text.StringBuilder();
            ToString(sb);
            return sb.ToString();
        }
        public override void MySqlAppend(System.Text.StringBuilder sb, bool ignoreReadOnly)
        {
        }
    }
    /// <summary>
    /// 心跳保持兼延迟测试 -- 请求
    /// </summary>
    public partial class Ping : xx.Object
    {
        public double ticks;

        public override ushort GetPackageId()
        {
            return xx.TypeId<Ping>.value;
        }

        public override void ToBBuffer(xx.BBuffer bb)
        {
            bb.Write(this.ticks);
        }

        public override void FromBBuffer(xx.BBuffer bb)
        {
            bb.Read(ref this.ticks);
        }
        public override void ToString(System.Text.StringBuilder s)
        {
            if (__toStringing)
            {
        	    s.Append("[ \"***** recursived *****\" ]");
        	    return;
            }
            else __toStringing = true;

            s.Append("{ \"pkgTypeName\":\"Generic.Ping\", \"pkgTypeId\":" + GetPackageId());
            ToStringCore(s);
            s.Append(" }");

            __toStringing = false;
        }
        public override void ToStringCore(System.Text.StringBuilder s)
        {
            s.Append(", \"ticks\":" + ticks.ToString());
        }
        public override string ToString()
        {
            var sb = new System.Text.StringBuilder();
            ToString(sb);
            return sb.ToString();
        }
        public override void MySqlAppend(System.Text.StringBuilder sb, bool ignoreReadOnly)
        {
        }
    }
    /// <summary>
    /// 心跳保持兼延迟测试 -- 回应
    /// </summary>
    public partial class Pong : xx.Object
    {
        public double ticks;

        public override ushort GetPackageId()
        {
            return xx.TypeId<Pong>.value;
        }

        public override void ToBBuffer(xx.BBuffer bb)
        {
            bb.Write(this.ticks);
        }

        public override void FromBBuffer(xx.BBuffer bb)
        {
            bb.Read(ref this.ticks);
        }
        public override void ToString(System.Text.StringBuilder s)
        {
            if (__toStringing)
            {
        	    s.Append("[ \"***** recursived *****\" ]");
        	    return;
            }
            else __toStringing = true;

            s.Append("{ \"pkgTypeName\":\"Generic.Pong\", \"pkgTypeId\":" + GetPackageId());
            ToStringCore(s);
            s.Append(" }");

            __toStringing = false;
        }
        public override void ToStringCore(System.Text.StringBuilder s)
        {
            s.Append(", \"ticks\":" + ticks.ToString());
        }
        public override string ToString()
        {
            var sb = new System.Text.StringBuilder();
            ToString(sb);
            return sb.ToString();
        }
        public override void MySqlAppend(System.Text.StringBuilder sb, bool ignoreReadOnly)
        {
        }
    }
}
    public static class AllTypes
    {
        public static void Register()
        {
            xx.Object.RegisterInternals();
            xx.Object.Register<DB_Manage.MsgResult>(11);
            xx.Object.Register<Manage_DB.Msg>(12);
            xx.Object.Register<Login_Client.LoginSuccess>(4);
            xx.Object.Register<Client_Login.Login>(5);
            xx.Object.Register<DB_Login.AuthSuccess>(6);
            xx.Object.Register<Login_DB.Auth>(7);
            xx.Object.Register<Generic.ServiceInfo>(3);
            xx.Object.Register<Generic.Error>(8);
            xx.Object.Register<Generic.Ping>(9);
            xx.Object.Register<Generic.Pong>(10);
        }
    }
}
