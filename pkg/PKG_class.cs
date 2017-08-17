using System;
using xx;
namespace PKG
{
namespace Manage
{
    /// <summary>
    /// 账号获取时所用的排序规则
    /// </summary>
    public enum AccountSorts : int
    {
        id_asc = 0,
        id_desc = 1,
        username_asc = 2,
        username_desc = 3,
    }
}
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
    /// <summary>
    /// 成功
    /// </summary>
    public partial class Success : Response
    {

        public override ushort GetPackageId()
        {
            return TypeIdMaps<Success>.typeId;
        }

        public override void ToBBuffer(BBuffer bb)
        {
            base.ToBBuffer(bb);
        }

        public override void FromBBuffer(BBuffer bb)
        {
            base.FromBBuffer(bb);
        }

    }
    /// <summary>
    /// 失败
    /// </summary>
    public partial class Fail : Response
    {
        public string reason;

        public override ushort GetPackageId()
        {
            return TypeIdMaps<Fail>.typeId;
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
namespace Manage_DB
{
    /// <summary>
    /// 请求进入服务器
    /// </summary>
    public partial class Login : Request
    {
        public string username;
        public string password;

        public override ushort GetPackageId()
        {
            return TypeIdMaps<Login>.typeId;
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
            bb.readLengthLimit = 50;
            bb.Read(ref this.username);
            bb.readLengthLimit = 50;
            bb.Read(ref this.password);
        }

    }
    /// <summary>
    /// 注销式退出
    /// </summary>
    public partial class Logout : Request
    {

        public override ushort GetPackageId()
        {
            return TypeIdMaps<Logout>.typeId;
        }

        public override void ToBBuffer(BBuffer bb)
        {
            base.ToBBuffer(bb);
        }

        public override void FromBBuffer(BBuffer bb)
        {
            base.FromBBuffer(bb);
        }

    }
    /// <summary>
    /// 拉管理员账号列表的 id 列表( 需要相关权限 )
    /// </summary>
    public partial class SelectManageAccountIds : Request
    {
        /// <summary>
        /// 限制获取的 id 总个数
        /// </summary>
        public int limit;
        /// <summary>
        /// 字段排序规则( 当前先只支持对一个字段排序 )
        /// </summary>
        public Manage.AccountSorts sort;

        public override ushort GetPackageId()
        {
            return TypeIdMaps<SelectManageAccountIds>.typeId;
        }

        public override void ToBBuffer(BBuffer bb)
        {
            base.ToBBuffer(bb);
            bb.Write(this.limit);
            bb.Write((int)this.sort);
        }

        public override void FromBBuffer(BBuffer bb)
        {
            base.FromBBuffer(bb);
            bb.Read(ref this.limit);
            {
                int tmp = 0;
                bb.Read(ref tmp);
                this.sort = (Manage.AccountSorts)tmp;
            }
        }

    }
    /// <summary>
    /// 拉管理员账号列表( 需要相关权限 )
    /// </summary>
    public partial class SelectManageAccounts : Request
    {
        /// <summary>
        /// 要拉的 id 列表
        /// </summary>
        public List<long> ids;

        public override ushort GetPackageId()
        {
            return TypeIdMaps<SelectManageAccounts>.typeId;
        }

        public override void ToBBuffer(BBuffer bb)
        {
            base.ToBBuffer(bb);
            bb.Write(this.ids);
        }

        public override void FromBBuffer(BBuffer bb)
        {
            base.FromBBuffer(bb);
            bb.readLengthLimit = 50;
            bb.Read(ref this.ids);
        }

    }
}
namespace Manage
{
    /// <summary>
    /// 账号
    /// </summary>
    public partial class Account : IBBuffer
    {
        public long id;
        public string username;

        public virtual ushort GetPackageId()
        {
            return TypeIdMaps<Account>.typeId;
        }

        public virtual void ToBBuffer(BBuffer bb)
        {
            bb.Write(this.id);
            bb.Write(this.username);
        }

        public virtual void FromBBuffer(BBuffer bb)
        {
            bb.Read(ref this.id);
            bb.readLengthLimit = 0;
            bb.Read(ref this.username);
        }

    }
    /// <summary>
    /// 身份
    /// </summary>
    public partial class Role : IBBuffer
    {
        public int id;
        public string name;
        public string desc;

        public virtual ushort GetPackageId()
        {
            return TypeIdMaps<Role>.typeId;
        }

        public virtual void ToBBuffer(BBuffer bb)
        {
            bb.Write(this.id);
            bb.Write(this.name);
            bb.Write(this.desc);
        }

        public virtual void FromBBuffer(BBuffer bb)
        {
            bb.Read(ref this.id);
            bb.readLengthLimit = 0;
            bb.Read(ref this.name);
            bb.readLengthLimit = 0;
            bb.Read(ref this.desc);
        }

    }
    /// <summary>
    /// 权限
    /// </summary>
    public partial class Permission : IBBuffer
    {
        public int id;
        public string group;
        public string name;
        public string desc;

        public virtual ushort GetPackageId()
        {
            return TypeIdMaps<Permission>.typeId;
        }

        public virtual void ToBBuffer(BBuffer bb)
        {
            bb.Write(this.id);
            bb.Write(this.group);
            bb.Write(this.name);
            bb.Write(this.desc);
        }

        public virtual void FromBBuffer(BBuffer bb)
        {
            bb.Read(ref this.id);
            bb.readLengthLimit = 0;
            bb.Read(ref this.group);
            bb.readLengthLimit = 0;
            bb.Read(ref this.name);
            bb.readLengthLimit = 0;
            bb.Read(ref this.desc);
        }

    }
    /// <summary>
    /// 管理人员 & 身份 绑定
    /// </summary>
    public partial class BindAccountRole : IBBuffer
    {
        public int account_id;
        public int role_id;

        public virtual ushort GetPackageId()
        {
            return TypeIdMaps<BindAccountRole>.typeId;
        }

        public virtual void ToBBuffer(BBuffer bb)
        {
            bb.Write(this.account_id);
            bb.Write(this.role_id);
        }

        public virtual void FromBBuffer(BBuffer bb)
        {
            bb.Read(ref this.account_id);
            bb.Read(ref this.role_id);
        }

    }
    /// <summary>
    /// 身份 & 权限 绑定
    /// </summary>
    public partial class BindRolePermission : IBBuffer
    {
        public int role_id;
        public int permission_id;

        public virtual ushort GetPackageId()
        {
            return TypeIdMaps<BindRolePermission>.typeId;
        }

        public virtual void ToBBuffer(BBuffer bb)
        {
            bb.Write(this.role_id);
            bb.Write(this.permission_id);
        }

        public virtual void FromBBuffer(BBuffer bb)
        {
            bb.Read(ref this.role_id);
            bb.Read(ref this.permission_id);
        }

    }
}
namespace DB_Manage
{
    /// <summary>
    /// 登录成功
    /// </summary>
    public partial class LoginSuccess : Success
    {
        public long id;
        public string token;

        public override ushort GetPackageId()
        {
            return TypeIdMaps<LoginSuccess>.typeId;
        }

        public override void ToBBuffer(BBuffer bb)
        {
            base.ToBBuffer(bb);
            bb.Write(this.id);
            bb.Write(this.token);
        }

        public override void FromBBuffer(BBuffer bb)
        {
            base.FromBBuffer(bb);
            bb.Read(ref this.id);
            bb.readLengthLimit = 0;
            bb.Read(ref this.token);
        }

    }
    /// <summary>
    /// 登录失败
    /// </summary>
    public partial class LoginFail : Fail
    {

        public override ushort GetPackageId()
        {
            return TypeIdMaps<LoginFail>.typeId;
        }

        public override void ToBBuffer(BBuffer bb)
        {
            base.ToBBuffer(bb);
        }

        public override void FromBBuffer(BBuffer bb)
        {
            base.FromBBuffer(bb);
        }

    }
    /// <summary>
    /// 注销成功
    /// </summary>
    public partial class LogoutSuccess : Success
    {

        public override ushort GetPackageId()
        {
            return TypeIdMaps<LogoutSuccess>.typeId;
        }

        public override void ToBBuffer(BBuffer bb)
        {
            base.ToBBuffer(bb);
        }

        public override void FromBBuffer(BBuffer bb)
        {
            base.FromBBuffer(bb);
        }

    }
    /// <summary>
    /// 注销失败
    /// </summary>
    public partial class LogoutFail : Fail
    {

        public override ushort GetPackageId()
        {
            return TypeIdMaps<LogoutFail>.typeId;
        }

        public override void ToBBuffer(BBuffer bb)
        {
            base.ToBBuffer(bb);
        }

        public override void FromBBuffer(BBuffer bb)
        {
            base.FromBBuffer(bb);
        }

    }
    /// <summary>
    /// 拉管理员账号列表的 id 列表 成功
    /// </summary>
    public partial class SelectManageAccountIdsSuccess : Success
    {
        public List<long> ids;

        public override ushort GetPackageId()
        {
            return TypeIdMaps<SelectManageAccountIdsSuccess>.typeId;
        }

        public override void ToBBuffer(BBuffer bb)
        {
            base.ToBBuffer(bb);
            bb.Write(this.ids);
        }

        public override void FromBBuffer(BBuffer bb)
        {
            base.FromBBuffer(bb);
            bb.readLengthLimit = 0;
            bb.Read(ref this.ids);
        }

    }
    /// <summary>
    /// 拉管理员账号列表的 id 列表 失败
    /// </summary>
    public partial class SelectManageAccountIdsFail : Fail
    {

        public override ushort GetPackageId()
        {
            return TypeIdMaps<SelectManageAccountIdsFail>.typeId;
        }

        public override void ToBBuffer(BBuffer bb)
        {
            base.ToBBuffer(bb);
        }

        public override void FromBBuffer(BBuffer bb)
        {
            base.FromBBuffer(bb);
        }

    }
    /// <summary>
    /// 拉管理员账号列表 成功
    /// </summary>
    public partial class SelectManageAccountsSuccess : Success
    {
        public List<Manage.Account> rows;

        public override ushort GetPackageId()
        {
            return TypeIdMaps<SelectManageAccountsSuccess>.typeId;
        }

        public override void ToBBuffer(BBuffer bb)
        {
            base.ToBBuffer(bb);
            bb.Write(this.rows);
        }

        public override void FromBBuffer(BBuffer bb)
        {
            base.FromBBuffer(bb);
            bb.readLengthLimit = 0;
            bb.Read(ref this.rows);
        }

    }
    /// <summary>
    /// 拉管理员账号列表 失败
    /// </summary>
    public partial class SelectManageAccountsFail : Fail
    {

        public override ushort GetPackageId()
        {
            return TypeIdMaps<SelectManageAccountsFail>.typeId;
        }

        public override void ToBBuffer(BBuffer bb)
        {
            base.ToBBuffer(bb);
        }

        public override void FromBBuffer(BBuffer bb)
        {
            base.FromBBuffer(bb);
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
            BBuffer.Register<Success>(5);
            BBuffer.Register<Fail>(6);
            BBuffer.Register<Property>(7);
            BBuffer.Register<Property_long>(8);
            BBuffer.Register<Property_double>(9);
            BBuffer.Register<Property_string>(10);
            BBuffer.Register<Properties>(11);
            BBuffer.Register<List<Property>>(12);
            BBuffer.Register<UserInfo>(13);
            BBuffer.Register<Client_Server.Join>(14);
            BBuffer.Register<Client_Server.Message>(15);
            BBuffer.Register<Client_Server.Logout>(16);
            BBuffer.Register<Server_Client.JoinSuccess>(17);
            BBuffer.Register<List<UserInfo>>(18);
            BBuffer.Register<Server_Client.JoinFail>(19);
            BBuffer.Register<Server_Client.PushJoin>(20);
            BBuffer.Register<Server_Client.PushMessage>(21);
            BBuffer.Register<Server_Client.PushLogout>(22);
            BBuffer.Register<Manage_DB.Login>(23);
            BBuffer.Register<Manage_DB.Logout>(24);
            BBuffer.Register<Manage_DB.SelectManageAccountIds>(25);
            BBuffer.Register<Manage_DB.SelectManageAccounts>(26);
            BBuffer.Register<List<long>>(27);
            BBuffer.Register<Manage.Account>(28);
            BBuffer.Register<Manage.Role>(29);
            BBuffer.Register<Manage.Permission>(30);
            BBuffer.Register<Manage.BindAccountRole>(31);
            BBuffer.Register<Manage.BindRolePermission>(32);
            BBuffer.Register<DB_Manage.LoginSuccess>(33);
            BBuffer.Register<DB_Manage.LoginFail>(34);
            BBuffer.Register<DB_Manage.LogoutSuccess>(35);
            BBuffer.Register<DB_Manage.LogoutFail>(36);
            BBuffer.Register<DB_Manage.SelectManageAccountIdsSuccess>(37);
            BBuffer.Register<DB_Manage.SelectManageAccountIdsFail>(38);
            BBuffer.Register<DB_Manage.SelectManageAccountsSuccess>(39);
            BBuffer.Register<List<Manage.Account>>(40);
            BBuffer.Register<DB_Manage.SelectManageAccountsFail>(41);

            BBuffer.handlers = new Action<IBBuffer>[42];
        }
    }
}
