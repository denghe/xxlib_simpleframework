using System;
using xx;
namespace RPC
{
    public static class PkgGenMd5
    {
        public const string value = "437fc6cea321fd85cca8a679e2822a54"; 
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
    public partial class MsgResult : IBBuffer
    {
        public string txt;

        public virtual ushort GetPackageId()
        {
            return TypeIdMaps<MsgResult>.typeId;
        }

        public virtual void ToBBuffer(BBuffer bb)
        {
            bb.Write(this.txt);
        }

        public virtual void FromBBuffer(BBuffer bb)
        {
            bb.readLengthLimit = 0;
            bb.Read(ref this.txt);
        }

    }
}
namespace Manage_DB
{
    public partial class Msg : IBBuffer
    {
        public string txt;

        public virtual ushort GetPackageId()
        {
            return TypeIdMaps<Msg>.typeId;
        }

        public virtual void ToBBuffer(BBuffer bb)
        {
            bb.Write(this.txt);
        }

        public virtual void FromBBuffer(BBuffer bb)
        {
            bb.readLengthLimit = 200;
            bb.Read(ref this.txt);
        }

    }
}
namespace Login_Client
{
    public partial class LoginSuccess : IBBuffer
    {
        public int id;

        public virtual ushort GetPackageId()
        {
            return TypeIdMaps<LoginSuccess>.typeId;
        }

        public virtual void ToBBuffer(BBuffer bb)
        {
            bb.Write(this.id);
        }

        public virtual void FromBBuffer(BBuffer bb)
        {
            bb.Read(ref this.id);
        }

    }
}
namespace Client_Login
{
    public partial class Login : IBBuffer
    {
        public string username;
        public string password;

        public virtual ushort GetPackageId()
        {
            return TypeIdMaps<Login>.typeId;
        }

        public virtual void ToBBuffer(BBuffer bb)
        {
            bb.Write(this.username);
            bb.Write(this.password);
        }

        public virtual void FromBBuffer(BBuffer bb)
        {
            bb.readLengthLimit = 50;
            bb.Read(ref this.username);
            bb.readLengthLimit = 50;
            bb.Read(ref this.password);
        }

    }
}
namespace DB_Login
{
    public partial class AuthSuccess : IBBuffer
    {
        public int id;

        public virtual ushort GetPackageId()
        {
            return TypeIdMaps<AuthSuccess>.typeId;
        }

        public virtual void ToBBuffer(BBuffer bb)
        {
            bb.Write(this.id);
        }

        public virtual void FromBBuffer(BBuffer bb)
        {
            bb.Read(ref this.id);
        }

    }
}
namespace Login_DB
{
    public partial class Auth : IBBuffer
    {
        public string username;
        public string password;

        public virtual ushort GetPackageId()
        {
            return TypeIdMaps<Auth>.typeId;
        }

        public virtual void ToBBuffer(BBuffer bb)
        {
            bb.Write(this.username);
            bb.Write(this.password);
        }

        public virtual void FromBBuffer(BBuffer bb)
        {
            bb.readLengthLimit = 0;
            bb.Read(ref this.username);
            bb.readLengthLimit = 0;
            bb.Read(ref this.password);
        }

    }
}
namespace Generic
{
    /// <summary>
    /// 服务间创建连接, 刚连上时, 首包需要发这个来说明自己是谁
    /// </summary>
    public partial class ServiceInfo : IBBuffer
    {
        /// <summary>
        /// 服务类型
        /// </summary>
        public Generic.ServiceTypes type;

        public virtual ushort GetPackageId()
        {
            return TypeIdMaps<ServiceInfo>.typeId;
        }

        public virtual void ToBBuffer(BBuffer bb)
        {
            bb.Write((int)this.type);
        }

        public virtual void FromBBuffer(BBuffer bb)
        {
            {
                int tmp = 0;
                bb.Read(ref tmp);
                this.type = (Generic.ServiceTypes)tmp;
            }
        }

    }
    /// <summary>
    /// 通用错误返回
    /// </summary>
    public partial class Error : IBBuffer
    {
        /// <summary>
        /// 错误码
        /// </summary>
        public int errNo;
        /// <summary>
        /// 错误文字
        /// </summary>
        public string errMsg;

        public virtual ushort GetPackageId()
        {
            return TypeIdMaps<Error>.typeId;
        }

        public virtual void ToBBuffer(BBuffer bb)
        {
            bb.Write(this.errNo);
            bb.Write(this.errMsg);
        }

        public virtual void FromBBuffer(BBuffer bb)
        {
            bb.Read(ref this.errNo);
            bb.readLengthLimit = 0;
            bb.Read(ref this.errMsg);
        }

    }
    /// <summary>
    /// 心跳保持兼延迟测试 -- 请求
    /// </summary>
    public partial class Ping : IBBuffer
    {
        public double ticks;

        public virtual ushort GetPackageId()
        {
            return TypeIdMaps<Ping>.typeId;
        }

        public virtual void ToBBuffer(BBuffer bb)
        {
            bb.Write(this.ticks);
        }

        public virtual void FromBBuffer(BBuffer bb)
        {
            bb.Read(ref this.ticks);
        }

    }
    /// <summary>
    /// 心跳保持兼延迟测试 -- 回应
    /// </summary>
    public partial class Pong : IBBuffer
    {
        public double ticks;

        public virtual ushort GetPackageId()
        {
            return TypeIdMaps<Pong>.typeId;
        }

        public virtual void ToBBuffer(BBuffer bb)
        {
            bb.Write(this.ticks);
        }

        public virtual void FromBBuffer(BBuffer bb)
        {
            bb.Read(ref this.ticks);
        }

    }
}
    public static class AllTypes
    {
        public static void Register()
        {
            // BBuffer.Register<string>(1);
            BBuffer.Register<BBuffer>(2);
            BBuffer.Register<DB_Manage.MsgResult>(11);
            BBuffer.Register<Manage_DB.Msg>(12);
            BBuffer.Register<Login_Client.LoginSuccess>(4);
            BBuffer.Register<Client_Login.Login>(5);
            BBuffer.Register<DB_Login.AuthSuccess>(6);
            BBuffer.Register<Login_DB.Auth>(7);
            BBuffer.Register<Generic.ServiceInfo>(3);
            BBuffer.Register<Generic.Error>(8);
            BBuffer.Register<Generic.Ping>(9);
            BBuffer.Register<Generic.Pong>(10);
        }
    }
}
