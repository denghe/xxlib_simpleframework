using System;
using xx;
namespace PKG
{
    /// <summary>
    /// 一个请求( 当前限定为 service 与 db 间 ), 通常携带一个流水号. 这是基类
    /// </summary>
    public partial class Request : IBBuffer
    {
        public int serial;

        public virtual ushort GetPackageId()
        {
            return TypeIdMaps<Request>.typeId;
        }

        public virtual void ToBBuffer(BBuffer bb)
        {
            bb.Write(this.serial);
        }

        public virtual void FromBBuffer(BBuffer bb)
        {
            bb.Read(ref this.serial);
        }

    }
    /// <summary>
    /// 一个回应( 当前限定为 service 与 db 间 ), 通常携带一个请求发过来的流水号. 这是基类
    /// </summary>
    public partial class Response : IBBuffer
    {
        public int requestSerial;

        public virtual ushort GetPackageId()
        {
            return TypeIdMaps<Response>.typeId;
        }

        public virtual void ToBBuffer(BBuffer bb)
        {
            bb.Write(this.requestSerial);
        }

        public virtual void FromBBuffer(BBuffer bb)
        {
            bb.Read(ref this.requestSerial);
        }

    }
    public partial class Property : IBBuffer
    {
        public string name;

        public virtual ushort GetPackageId()
        {
            return TypeIdMaps<Property>.typeId;
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

    }
    public partial class Property_long : Property
    {
        public long value;

        public override ushort GetPackageId()
        {
            return TypeIdMaps<Property_long>.typeId;
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

    }
    public partial class Property_double : Property
    {
        public double value;

        public override ushort GetPackageId()
        {
            return TypeIdMaps<Property_double>.typeId;
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

    }
    public partial class Property_string : Property
    {
        public string value;

        public override ushort GetPackageId()
        {
            return TypeIdMaps<Property_string>.typeId;
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

    }
    public partial class Properties : Property
    {
        public List<Property> value;

        public override ushort GetPackageId()
        {
            return TypeIdMaps<Properties>.typeId;
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

    }
    public partial class UserInfo : IBBuffer
    {
        public long id;
        public List<Property> props;

        public virtual ushort GetPackageId()
        {
            return TypeIdMaps<UserInfo>.typeId;
        }

        public virtual void ToBBuffer(BBuffer bb)
        {
            bb.Write(this.id);
            bb.Write(this.props);
        }

        public virtual void FromBBuffer(BBuffer bb)
        {
            bb.Read(ref this.id);
            bb.readLengthLimit = 0;
            bb.Read(ref this.props);
        }

    }
namespace Server_Client
{
    /// <summary>
    /// 进入成功, 返回用户信息
    /// </summary>
    public partial class JoinSuccess : Response
    {
        public UserInfo self;
        public List<UserInfo> users;

        public override ushort GetPackageId()
        {
            return TypeIdMaps<JoinSuccess>.typeId;
        }

        public override void ToBBuffer(BBuffer bb)
        {
            base.ToBBuffer(bb);
            bb.Write(this.self);
            bb.Write(this.users);
        }

        public override void FromBBuffer(BBuffer bb)
        {
            base.FromBBuffer(bb);
            bb.Read(ref this.self);
            bb.readLengthLimit = 0;
            bb.Read(ref this.users);
        }

    }
    /// <summary>
    /// 进入失败, 返回错误信息
    /// </summary>
    public partial class JoinFail : Response
    {
        public string reason;

        public override ushort GetPackageId()
        {
            return TypeIdMaps<JoinFail>.typeId;
        }

        public override void ToBBuffer(BBuffer bb)
        {
            base.ToBBuffer(bb);
            bb.Write(this.reason);
        }

        public override void FromBBuffer(BBuffer bb)
        {
            base.FromBBuffer(bb);
            bb.readLengthLimit = 0;
            bb.Read(ref this.reason);
        }

    }
    /// <summary>
    /// 推送文字消息
    /// </summary>
    public partial class PushJoin : IBBuffer
    {
        public long id;

        public virtual ushort GetPackageId()
        {
            return TypeIdMaps<PushJoin>.typeId;
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
    /// <summary>
    /// 推送文字消息
    /// </summary>
    public partial class PushMessage : IBBuffer
    {
        public long id;
        public string text;

        public virtual ushort GetPackageId()
        {
            return TypeIdMaps<PushMessage>.typeId;
        }

        public virtual void ToBBuffer(BBuffer bb)
        {
            bb.Write(this.id);
            bb.Write(this.text);
        }

        public virtual void FromBBuffer(BBuffer bb)
        {
            bb.Read(ref this.id);
            bb.readLengthLimit = 0;
            bb.Read(ref this.text);
        }

    }
    /// <summary>
    /// 推送退出消息
    /// </summary>
    public partial class PushLogout : IBBuffer
    {
        public long id;
        /// <summary>
        /// 退出原因( 主动? 掉线?
        /// </summary>
        public string reason;

        public virtual ushort GetPackageId()
        {
            return TypeIdMaps<PushLogout>.typeId;
        }

        public virtual void ToBBuffer(BBuffer bb)
        {
            bb.Write(this.id);
            bb.Write(this.reason);
        }

        public virtual void FromBBuffer(BBuffer bb)
        {
            bb.Read(ref this.id);
            bb.readLengthLimit = 0;
            bb.Read(ref this.reason);
        }

    }
}
namespace Client_Server
{
    /// <summary>
    /// 请求进入服务器
    /// </summary>
    public partial class Join : Request
    {
        public string username;
        public string password;

        public override ushort GetPackageId()
        {
            return TypeIdMaps<Join>.typeId;
        }

        public override void ToBBuffer(BBuffer bb)
        {
            base.ToBBuffer(bb);
            bb.Write(this.username);
            bb.Write(this.password);
        }

        public override void FromBBuffer(BBuffer bb)
        {
            base.FromBBuffer(bb);
            bb.readLengthLimit = 16;
            bb.Read(ref this.username);
            bb.readLengthLimit = 16;
            bb.Read(ref this.password);
        }

    }
    /// <summary>
    /// 发消息
    /// </summary>
    public partial class Message : IBBuffer
    {
        public string text;

        public virtual ushort GetPackageId()
        {
            return TypeIdMaps<Message>.typeId;
        }

        public virtual void ToBBuffer(BBuffer bb)
        {
            bb.Write(this.text);
        }

        public virtual void FromBBuffer(BBuffer bb)
        {
            bb.readLengthLimit = 256;
            bb.Read(ref this.text);
        }

    }
    /// <summary>
    /// 主动退出
    /// </summary>
    public partial class Logout : IBBuffer
    {

        public virtual ushort GetPackageId()
        {
            return TypeIdMaps<Logout>.typeId;
        }

        public virtual void ToBBuffer(BBuffer bb)
        {
        }

        public virtual void FromBBuffer(BBuffer bb)
        {
        }

    }
}
    public static class AllTypes
    {
        public static void Register()
        {
            // BBuffer.Register<string>(1);
            BBuffer.Register<BBuffer>(2);
            BBuffer.Register<Request>(3);
            BBuffer.Register<Response>(4);
            BBuffer.Register<Property>(5);
            BBuffer.Register<Property_long>(6);
            BBuffer.Register<Property_double>(7);
            BBuffer.Register<Property_string>(8);
            BBuffer.Register<Properties>(9);
            BBuffer.Register<List<Property>>(10);
            BBuffer.Register<UserInfo>(11);
            BBuffer.Register<Server_Client.JoinSuccess>(12);
            BBuffer.Register<List<UserInfo>>(13);
            BBuffer.Register<Server_Client.JoinFail>(14);
            BBuffer.Register<Server_Client.PushJoin>(15);
            BBuffer.Register<Server_Client.PushMessage>(16);
            BBuffer.Register<Server_Client.PushLogout>(17);
            BBuffer.Register<Client_Server.Join>(18);
            BBuffer.Register<Client_Server.Message>(19);
            BBuffer.Register<Client_Server.Logout>(20);

            BBuffer.handlers = new Action<IBBuffer>[21];
        }
    }
}
