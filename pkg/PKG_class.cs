using System;
using xx;
namespace PKG
{
namespace DB_Login
{
    public enum AuthResultTypes : int
    {
        Success = 0,
        UsernameNotFound = 1,
        WrongPassword = 2,
        Disabled = 3,
    }
}
namespace Global
{
    public enum ServiceTypes : byte
    {
        DB = 0,
        Login = 1,
        Lobby = 2,
        Game1 = 3,
    }
}
namespace Poker
{
    /// <summary>
    /// 花色
    /// </summary>
    public enum Sets : byte
    {
        /// <summary>
        /// 黑桃
        /// </summary>
        Spades = 1,
        /// <summary>
        /// 红心
        /// </summary>
        Hearts = 2,
        /// <summary>
        /// 樱桃/梅花
        /// </summary>
        Clubs = 3,
        /// <summary>
        /// 方片/方块
        /// </summary>
        Diamonds = 4,
    }
    /// <summary>
    /// 点数
    /// </summary>
    public enum Points : byte
    {
        Ace = 1,
        _2 = 2,
        _3 = 3,
        _4 = 4,
        _5 = 5,
        _6 = 6,
        _7 = 7,
        _8 = 8,
        _9 = 9,
        _10 = 10,
        Jack = 11,
        Queen = 12,
        King = 13,
        /// <summary>
        /// 小王/小鬼
        /// </summary>
        Joker = 14,
        /// <summary>
        /// 大王/大鬼
        /// </summary>
        JokerEx = 15,
    }
}
namespace Game1
{
    /// <summary>
    /// 游戏阶段列表
    /// </summary>
    public enum Stages : byte
    {
        /// <summary>
        /// 等人点准备以开始游戏
        /// </summary>
        WaitReady = 0,
        /// <summary>
        /// 洗牌
        /// </summary>
        Shuffle = 1,
        /// <summary>
        /// 发牌
        /// </summary>
        Deal = 2,
        /// <summary>
        /// 下底注
        /// </summary>
        BaseBet = 3,
        /// <summary>
        /// 看牌
        /// </summary>
        Watch = 4,
        /// <summary>
        /// 依次操作
        /// </summary>
        Foreach = 5,
        /// <summary>
        /// 最终比牌
        /// </summary>
        Match = 6,
        /// <summary>
        /// 局终报告
        /// </summary>
        Report = 7,
    }
}
namespace Client_Lobby
{
    /// <summary>
    /// 进入大厅
    /// </summary>
    public partial class Join : IBBuffer
    {
        /// <summary>
        /// 账号id
        /// </summary>
        public int accountId;
        /// <summary>
        /// 临时票据
        /// </summary>
        public string token;

        public virtual ushort GetPackageId()
        {
            return TypeIdMaps<Join>.typeId;
        }

        public virtual void ToBBuffer(BBuffer bb)
        {
            bb.Write(this.accountId);
            bb.Write(this.token);
        }

        public virtual void FromBBuffer(BBuffer bb)
        {
            bb.Read(ref this.accountId);
            bb.readLengthLimit = 64;
            bb.Read(ref this.token);
        }

    }
    /// <summary>
    /// 拉取房间列表
    /// </summary>
    public partial class PullRooms : IBBuffer
    {

        public virtual ushort GetPackageId()
        {
            return TypeIdMaps<PullRooms>.typeId;
        }

        public virtual void ToBBuffer(BBuffer bb)
        {
        }

        public virtual void FromBBuffer(BBuffer bb)
        {
        }

    }
    /// <summary>
    /// 申请进入房间
    /// </summary>
    public partial class JoinRoom : IBBuffer
    {
        public int roomId;

        public virtual ushort GetPackageId()
        {
            return TypeIdMaps<JoinRoom>.typeId;
        }

        public virtual void ToBBuffer(BBuffer bb)
        {
            bb.Write(this.roomId);
        }

        public virtual void FromBBuffer(BBuffer bb)
        {
            bb.Read(ref this.roomId);
        }

    }
    /// <summary>
    /// 试修改金币( 模拟充值, 输/赢 )
    /// </summary>
    public partial class ChangeCoin : IBBuffer
    {
        /// <summary>
        /// 原金币值
        /// </summary>
        public long original;
        /// <summary>
        /// 加减值
        /// </summary>
        public long val;
        /// <summary>
        /// 操作原因
        /// </summary>
        public string reason;

        public virtual ushort GetPackageId()
        {
            return TypeIdMaps<ChangeCoin>.typeId;
        }

        public virtual void ToBBuffer(BBuffer bb)
        {
            bb.Write(this.original);
            bb.Write(this.val);
            bb.Write(this.reason);
        }

        public virtual void FromBBuffer(BBuffer bb)
        {
            bb.Read(ref this.original);
            bb.Read(ref this.val);
            bb.readLengthLimit = 250;
            bb.Read(ref this.reason);
        }

    }
}
namespace Game1_Client
{
    /// <summary>
    /// 游戏1进入成功
    /// </summary>
    public partial class JoinSuccess : IBBuffer
    {

        public virtual ushort GetPackageId()
        {
            return TypeIdMaps<JoinSuccess>.typeId;
        }

        public virtual void ToBBuffer(BBuffer bb)
        {
        }

        public virtual void FromBBuffer(BBuffer bb)
        {
        }

    }
}
namespace Client_Game1
{
    /// <summary>
    /// 进入大厅
    /// </summary>
    public partial class Join : IBBuffer
    {
        /// <summary>
        /// 账号id
        /// </summary>
        public int accountId;
        /// <summary>
        /// 临时票据
        /// </summary>
        public string token;

        public virtual ushort GetPackageId()
        {
            return TypeIdMaps<Join>.typeId;
        }

        public virtual void ToBBuffer(BBuffer bb)
        {
            bb.Write(this.accountId);
            bb.Write(this.token);
        }

        public virtual void FromBBuffer(BBuffer bb)
        {
            bb.Read(ref this.accountId);
            bb.readLengthLimit = 64;
            bb.Read(ref this.token);
        }

    }
    /// <summary>
    /// 主动退出
    /// </summary>
    public partial class Quit : IBBuffer
    {

        public virtual ushort GetPackageId()
        {
            return TypeIdMaps<Quit>.typeId;
        }

        public virtual void ToBBuffer(BBuffer bb)
        {
        }

        public virtual void FromBBuffer(BBuffer bb)
        {
        }

    }
}
namespace Lobby_Client
{
    /// <summary>
    /// 大厅进入成功
    /// </summary>
    public partial class JoinSuccess : IBBuffer
    {
        /// <summary>
        /// 当前金币数
        /// </summary>
        public long coin;

        public virtual ushort GetPackageId()
        {
            return TypeIdMaps<JoinSuccess>.typeId;
        }

        public virtual void ToBBuffer(BBuffer bb)
        {
            bb.Write(this.coin);
        }

        public virtual void FromBBuffer(BBuffer bb)
        {
            bb.Read(ref this.coin);
        }

    }
    /// <summary>
    /// 拉取房间列表成功: 内含具体的列表
    /// </summary>
    public partial class PullRoomsSuccess : IBBuffer
    {
        public List<Global.RoomInfo> rooms;

        public virtual ushort GetPackageId()
        {
            return TypeIdMaps<PullRoomsSuccess>.typeId;
        }

        public virtual void ToBBuffer(BBuffer bb)
        {
            bb.Write(this.rooms);
        }

        public virtual void FromBBuffer(BBuffer bb)
        {
            bb.readLengthLimit = 0;
            bb.Read(ref this.rooms);
        }

    }
    /// <summary>
    /// 拉取房间列表失败: 内含原因
    /// </summary>
    public partial class PullRoomsFail : IBBuffer
    {
        public string reason;

        public virtual ushort GetPackageId()
        {
            return TypeIdMaps<PullRoomsFail>.typeId;
        }

        public virtual void ToBBuffer(BBuffer bb)
        {
            bb.Write(this.reason);
        }

        public virtual void FromBBuffer(BBuffer bb)
        {
            bb.readLengthLimit = 0;
            bb.Read(ref this.reason);
        }

    }
    /// <summary>
    /// 申请进入房间成功: 内含连接信息
    /// </summary>
    public partial class JoinRoomSuccess : IBBuffer
    {
        public string token;
        public Global.IPAddress serviceAddress;

        public virtual ushort GetPackageId()
        {
            return TypeIdMaps<JoinRoomSuccess>.typeId;
        }

        public virtual void ToBBuffer(BBuffer bb)
        {
            bb.Write(this.token);
            bb.Write(this.serviceAddress);
        }

        public virtual void FromBBuffer(BBuffer bb)
        {
            bb.readLengthLimit = 0;
            bb.Read(ref this.token);
            bb.Read(ref this.serviceAddress);
        }

    }
    /// <summary>
    /// 申请进入房间失败: 内含原因
    /// </summary>
    public partial class JoinRoomFail : IBBuffer
    {
        public string reason;

        public virtual ushort GetPackageId()
        {
            return TypeIdMaps<JoinRoomFail>.typeId;
        }

        public virtual void ToBBuffer(BBuffer bb)
        {
            bb.Write(this.reason);
        }

        public virtual void FromBBuffer(BBuffer bb)
        {
            bb.readLengthLimit = 0;
            bb.Read(ref this.reason);
        }

    }
    /// <summary>
    /// 金币变化之推送消息 向客户端转发( 不带账号id )
    /// </summary>
    public partial class SyncCoin : IBBuffer
    {
        /// <summary>
        /// 版本号( 如果从多个渠道收到通知则保留该值最大的 )
        /// </summary>
        public int versionNumber;
        /// <summary>
        /// 原金币值
        /// </summary>
        public long original;
        /// <summary>
        /// 加减值
        /// </summary>
        public long val;
        /// <summary>
        /// 当前金币值
        /// </summary>
        public long curr;
        /// <summary>
        /// 操作原因
        /// </summary>
        public string reason;

        public virtual ushort GetPackageId()
        {
            return TypeIdMaps<SyncCoin>.typeId;
        }

        public virtual void ToBBuffer(BBuffer bb)
        {
            bb.Write(this.versionNumber);
            bb.Write(this.original);
            bb.Write(this.val);
            bb.Write(this.curr);
            bb.Write(this.reason);
        }

        public virtual void FromBBuffer(BBuffer bb)
        {
            bb.Read(ref this.versionNumber);
            bb.Read(ref this.original);
            bb.Read(ref this.val);
            bb.Read(ref this.curr);
            bb.readLengthLimit = 0;
            bb.Read(ref this.reason);
        }

    }
    /// <summary>
    /// 试修改金币 成功
    /// </summary>
    public partial class ChangeCoinSuccess : IBBuffer
    {

        public virtual ushort GetPackageId()
        {
            return TypeIdMaps<ChangeCoinSuccess>.typeId;
        }

        public virtual void ToBBuffer(BBuffer bb)
        {
        }

        public virtual void FromBBuffer(BBuffer bb)
        {
        }

    }
    /// <summary>
    /// 试修改金币 失败
    /// </summary>
    public partial class ChangeCoinFail : IBBuffer
    {
        public string reason;

        public virtual ushort GetPackageId()
        {
            return TypeIdMaps<ChangeCoinFail>.typeId;
        }

        public virtual void ToBBuffer(BBuffer bb)
        {
            bb.Write(this.reason);
        }

        public virtual void FromBBuffer(BBuffer bb)
        {
            bb.readLengthLimit = 0;
            bb.Read(ref this.reason);
        }

    }
}
namespace Game1_Lobby
{
    /// <summary>
    /// 游戏1 返回给 大厅 给玩家分配的 token
    /// </summary>
    public partial class JoinSuccess : Global.Response
    {
        /// <summary>
        /// 该 token 由 Game1 生成, 下发给 client, 并带超时时间. 在规定时间内, 如果 client 没拿着 token 连上来, 则过期, 重新通知大厅 缺人, 等大厅裁决
        /// </summary>
        public string token;

        public override ushort GetPackageId()
        {
            return TypeIdMaps<JoinSuccess>.typeId;
        }

        public override void ToBBuffer(BBuffer bb)
        {
            base.ToBBuffer(bb);
            bb.Write(this.token);
        }

        public override void FromBBuffer(BBuffer bb)
        {
            base.FromBBuffer(bb);
            bb.readLengthLimit = 0;
            bb.Read(ref this.token);
        }

    }
    /// <summary>
    /// 游戏1 返回给 大厅 进入失败( 满员?? 无资格?? 内部错误 )
    /// </summary>
    public partial class JoinFail : Global.Response
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
    /// client 并没有在规定时间拿着 token 来连, 或是玩家逻辑断线 / 一局完 / 强行退出
    /// </summary>
    public partial class Quit : IBBuffer
    {
        public int accountId;
        public string reason;

        public virtual ushort GetPackageId()
        {
            return TypeIdMaps<Quit>.typeId;
        }

        public virtual void ToBBuffer(BBuffer bb)
        {
            bb.Write(this.accountId);
            bb.Write(this.reason);
        }

        public virtual void FromBBuffer(BBuffer bb)
        {
            bb.Read(ref this.accountId);
            bb.readLengthLimit = 0;
            bb.Read(ref this.reason);
        }

    }
}
namespace Lobby_Game1
{
    /// <summary>
    /// 大厅通知游戏有玩家要进来
    /// </summary>
    public partial class Join : Global.Request
    {
        public int accountId;

        public override ushort GetPackageId()
        {
            return TypeIdMaps<Join>.typeId;
        }

        public override void ToBBuffer(BBuffer bb)
        {
            base.ToBBuffer(bb);
            bb.Write(this.accountId);
        }

        public override void FromBBuffer(BBuffer bb)
        {
            base.FromBBuffer(bb);
            bb.Read(ref this.accountId);
        }

    }
}
namespace Lobby_Login
{
    /// <summary>
    /// 大厅 返回给 登录 给玩家分配的 token, 以及玩家接下来需要连的 服务类型 和 地址
    /// </summary>
    public partial class JoinSuccess : Global.Response
    {
        public int accountId;
        public string token;
        /// <summary>
        /// 如果是断线重连, 则返回玩家当前所在服务的类型. 否则返回 Lobby
        /// </summary>
        public Global.ServiceTypes serviceType;
        public Global.IPAddress serviceAddress;

        public override ushort GetPackageId()
        {
            return TypeIdMaps<JoinSuccess>.typeId;
        }

        public override void ToBBuffer(BBuffer bb)
        {
            base.ToBBuffer(bb);
            bb.Write(this.accountId);
            bb.Write(this.token);
            bb.Write((byte)this.serviceType);
            bb.Write(this.serviceAddress);
        }

        public override void FromBBuffer(BBuffer bb)
        {
            base.FromBBuffer(bb);
            bb.Read(ref this.accountId);
            bb.readLengthLimit = 0;
            bb.Read(ref this.token);
            {
                byte tmp = 0;
                bb.Read(ref tmp);
                this.serviceType = (Global.ServiceTypes)tmp;
            }
            bb.Read(ref this.serviceAddress);
        }

    }
    /// <summary>
    /// 大厅 返回给 登录 进入失败( 满员?? )
    /// </summary>
    public partial class JoinFail : Global.Response
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
}
namespace Login_Lobby
{
    /// <summary>
    /// 登录通知大厅有玩家要进来
    /// </summary>
    public partial class Join : Global.Request
    {
        public int accountId;

        public override ushort GetPackageId()
        {
            return TypeIdMaps<Join>.typeId;
        }

        public override void ToBBuffer(BBuffer bb)
        {
            base.ToBBuffer(bb);
            bb.Write(this.accountId);
        }

        public override void FromBBuffer(BBuffer bb)
        {
            base.FromBBuffer(bb);
            bb.Read(ref this.accountId);
        }

    }
}
namespace Login_Client
{
    /// <summary>
    /// 登录成功, 返回导航地址和 token
    /// </summary>
    public partial class LoginSuccess : IBBuffer
    {
        public int accountId;
        public string token;
        public Global.IPAddress serviceAddress;
        /// <summary>
        /// 目标地址可能是大厅也可能是游戏
        /// </summary>
        public Global.ServiceTypes serviceType;

        public virtual ushort GetPackageId()
        {
            return TypeIdMaps<LoginSuccess>.typeId;
        }

        public virtual void ToBBuffer(BBuffer bb)
        {
            bb.Write(this.accountId);
            bb.Write(this.token);
            bb.Write(this.serviceAddress);
            bb.Write((byte)this.serviceType);
        }

        public virtual void FromBBuffer(BBuffer bb)
        {
            bb.Read(ref this.accountId);
            bb.readLengthLimit = 0;
            bb.Read(ref this.token);
            bb.Read(ref this.serviceAddress);
            {
                byte tmp = 0;
                bb.Read(ref tmp);
                this.serviceType = (Global.ServiceTypes)tmp;
            }
        }

    }
    /// <summary>
    /// 登录失败( 含原因 )
    /// </summary>
    public partial class LoginFail : IBBuffer
    {
        public int errorCode;
        public string reason;

        public virtual ushort GetPackageId()
        {
            return TypeIdMaps<LoginFail>.typeId;
        }

        public virtual void ToBBuffer(BBuffer bb)
        {
            bb.Write(this.errorCode);
            bb.Write(this.reason);
        }

        public virtual void FromBBuffer(BBuffer bb)
        {
            bb.Read(ref this.errorCode);
            bb.readLengthLimit = 0;
            bb.Read(ref this.reason);
        }

    }
}
namespace DB_Login
{
    /// <summary>
    /// DB 给 Login 返回 Auth 结果
    /// </summary>
    public partial class AuthResult : Global.Response
    {
        public DB_Login.AuthResultTypes type;
        public int accountId;

        public override ushort GetPackageId()
        {
            return TypeIdMaps<AuthResult>.typeId;
        }

        public override void ToBBuffer(BBuffer bb)
        {
            base.ToBBuffer(bb);
            bb.Write((int)this.type);
            bb.Write(this.accountId);
        }

        public override void FromBBuffer(BBuffer bb)
        {
            base.FromBBuffer(bb);
            {
                int tmp = 0;
                bb.Read(ref tmp);
                this.type = (DB_Login.AuthResultTypes)tmp;
            }
            bb.Read(ref this.accountId);
        }

    }
}
namespace Login_DB
{
    /// <summary>
    /// 登录服务器找 DB 做登录判断
    /// </summary>
    public partial class Auth : Global.Request
    {
        public string username;
        public string password;

        public override ushort GetPackageId()
        {
            return TypeIdMaps<Auth>.typeId;
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
            bb.readLengthLimit = 0;
            bb.Read(ref this.username);
            bb.readLengthLimit = 0;
            bb.Read(ref this.password);
        }

    }
}
namespace Client_Login
{
    /// <summary>
    /// 客户端通过用户名和密码登录
    /// </summary>
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
            bb.readLengthLimit = 64;
            bb.Read(ref this.username);
            bb.readLengthLimit = 64;
            bb.Read(ref this.password);
        }

    }
}
namespace Global
{
    /// <summary>
    /// 服务地址信息( 用于各服务器下发导航信息
    /// </summary>
    public partial class IPAddress : IBBuffer
    {
        /// <summary>
        /// IPV4 或 V6 的地址
        /// </summary>
        public string ip;
        /// <summary>
        /// 端口
        /// </summary>
        public int port;

        public virtual ushort GetPackageId()
        {
            return TypeIdMaps<IPAddress>.typeId;
        }

        public virtual void ToBBuffer(BBuffer bb)
        {
            bb.Write(this.ip);
            bb.Write(this.port);
        }

        public virtual void FromBBuffer(BBuffer bb)
        {
            bb.readLengthLimit = 64;
            bb.Read(ref this.ip);
            bb.Read(ref this.port);
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
    /// 当前用于 service 连接到 db 时简单描述自己是谁, 连接成功后的首包就需要发送.
    /// </summary>
    public partial class ServiceInfo : IBBuffer
    {
        public Global.ServiceTypes serviceType;
        /// <summary>
        /// 实例id. 对同一种类型的多个服务实例有意义, 方便做映射
        /// </summary>
        public int instanceId;

        public virtual ushort GetPackageId()
        {
            return TypeIdMaps<ServiceInfo>.typeId;
        }

        public virtual void ToBBuffer(BBuffer bb)
        {
            bb.Write((byte)this.serviceType);
            bb.Write(this.instanceId);
        }

        public virtual void FromBBuffer(BBuffer bb)
        {
            {
                byte tmp = 0;
                bb.Read(ref tmp);
                this.serviceType = (Global.ServiceTypes)tmp;
            }
            bb.Read(ref this.instanceId);
        }

    }
    /// <summary>
    /// 用户信息
    /// </summary>
    public partial class UserInfo : IBBuffer
    {
        public long coin;

        public virtual ushort GetPackageId()
        {
            return TypeIdMaps<UserInfo>.typeId;
        }

        public virtual void ToBBuffer(BBuffer bb)
        {
            bb.Write(this.coin);
        }

        public virtual void FromBBuffer(BBuffer bb)
        {
            bb.Read(ref this.coin);
        }

    }
    /// <summary>
    /// 游戏小厅信息
    /// </summary>
    public partial class RoomInfo : IBBuffer
    {
        public int id;

        public virtual ushort GetPackageId()
        {
            return TypeIdMaps<RoomInfo>.typeId;
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
    /// 游戏小厅信息派生: 扎金花
    /// </summary>
    public partial class RoomInfo_扎金花 : Global.RoomInfo
    {
        public long 进场基础金币要求;
        public long 底注多少金币;
        public long 封顶加注额度;
        public int 几轮后可直接比牌;
        public int 最多加注几轮;

        public override ushort GetPackageId()
        {
            return TypeIdMaps<RoomInfo_扎金花>.typeId;
        }

        public override void ToBBuffer(BBuffer bb)
        {
            base.ToBBuffer(bb);
            bb.Write(this.进场基础金币要求);
            bb.Write(this.底注多少金币);
            bb.Write(this.封顶加注额度);
            bb.Write(this.几轮后可直接比牌);
            bb.Write(this.最多加注几轮);
        }

        public override void FromBBuffer(BBuffer bb)
        {
            base.FromBBuffer(bb);
            bb.Read(ref this.进场基础金币要求);
            bb.Read(ref this.底注多少金币);
            bb.Read(ref this.封顶加注额度);
            bb.Read(ref this.几轮后可直接比牌);
            bb.Read(ref this.最多加注几轮);
        }

    }
}
namespace Poker
{
    /// <summary>
    /// 牌
    /// </summary>
    public partial struct Card : IBBuffer
    {
        public Poker.Sets set;
        public Poker.Points point;

        public ushort GetPackageId()
        {
            return TypeIdMaps<Card>.typeId;
        }

        public  void ToBBuffer(BBuffer bb)
        {
            bb.Write((byte)this.set);
            bb.Write((byte)this.point);
        }

        public  void FromBBuffer(BBuffer bb)
        {
            {
                byte tmp = 0;
                bb.Read(ref tmp);
                this.set = (Poker.Sets)tmp;
            }
            {
                byte tmp = 0;
                bb.Read(ref tmp);
                this.point = (Poker.Points)tmp;
            }
        }

    }
    /// <summary>
    /// 某玩家手上的牌
    /// </summary>
    public partial class Suit : IBBuffer
    {
        public int accountId;
        public List<Poker.Card> cards;

        public virtual ushort GetPackageId()
        {
            return TypeIdMaps<Suit>.typeId;
        }

        public virtual void ToBBuffer(BBuffer bb)
        {
            bb.Write(this.accountId);
            bb.Write(this.cards);
        }

        public virtual void FromBBuffer(BBuffer bb)
        {
            bb.Read(ref this.accountId);
            bb.readLengthLimit = 0;
            bb.Read(ref this.cards);
        }

    }
}
namespace Lobby_DB
{
    /// <summary>
    /// 试修改金币
    /// </summary>
    public partial class ChangeCoin : Global.Request
    {
        /// <summary>
        /// 账号id
        /// </summary>
        public int accountId;
        /// <summary>
        /// 原金币值
        /// </summary>
        public long original;
        /// <summary>
        /// 加减值
        /// </summary>
        public long val;
        /// <summary>
        /// 操作原因
        /// </summary>
        public string reason;

        public override ushort GetPackageId()
        {
            return TypeIdMaps<ChangeCoin>.typeId;
        }

        public override void ToBBuffer(BBuffer bb)
        {
            base.ToBBuffer(bb);
            bb.Write(this.accountId);
            bb.Write(this.original);
            bb.Write(this.val);
            bb.Write(this.reason);
        }

        public override void FromBBuffer(BBuffer bb)
        {
            base.FromBBuffer(bb);
            bb.Read(ref this.accountId);
            bb.Read(ref this.original);
            bb.Read(ref this.val);
            bb.readLengthLimit = 250;
            bb.Read(ref this.reason);
        }

    }
    /// <summary>
    /// 获取用户信息
    /// </summary>
    public partial class GetUserInfo : Global.Request
    {
        /// <summary>
        /// 账号id
        /// </summary>
        public int accountId;

        public override ushort GetPackageId()
        {
            return TypeIdMaps<GetUserInfo>.typeId;
        }

        public override void ToBBuffer(BBuffer bb)
        {
            base.ToBBuffer(bb);
            bb.Write(this.accountId);
        }

        public override void FromBBuffer(BBuffer bb)
        {
            base.FromBBuffer(bb);
            bb.Read(ref this.accountId);
        }

    }
}
namespace Game1
{
    /// <summary>
    /// 参与游戏的玩家
    /// </summary>
    public partial class Player : IBBuffer
    {
        /// <summary>
        /// 账号id
        /// </summary>
        public int accountId;
        /// <summary>
        /// 临时票据
        /// </summary>
        public string token;
        /// <summary>
        /// 当前金币数
        /// </summary>
        public long coin;
        /// <summary>
        /// 当前位于的桌
        /// </summary>
        public Game1.Desk desk;
        /// <summary>
        /// 自己位于容器中的下标
        /// </summary>
        public int desk_players_index;
        /// <summary>
        /// 本局游戏是否参与
        /// </summary>
        public bool joined = false;
        /// <summary>
        /// 手牌
        /// </summary>
        public List<Poker.Card> cards;
        /// <summary>
        /// 是否看过
        /// </summary>
        public bool shown = false;
        /// <summary>
        /// 已下注金额
        /// </summary>
        public long stake;
        /// <summary>
        /// 开牌目标
        /// </summary>
        public int openTarget;
        /// <summary>
        /// 步骤自增编号
        /// </summary>
        public int stepId;

        public virtual ushort GetPackageId()
        {
            return TypeIdMaps<Player>.typeId;
        }

        public virtual void ToBBuffer(BBuffer bb)
        {
            bb.Write(this.accountId);
            bb.Write(default(string));
            bb.Write(this.coin);
            bb.Write(this.desk);
            bb.Write(this.desk_players_index);
            bb.Write(this.joined);
            bb.CustomWrite(bb, this, "cards");
            bb.Write(this.shown);
            bb.Write(this.stake);
            bb.Write(this.openTarget);
            bb.Write(this.stepId);
        }

        public virtual void FromBBuffer(BBuffer bb)
        {
            bb.Read(ref this.accountId);
            bb.readLengthLimit = 0;
            bb.Read(ref this.token);
            bb.Read(ref this.coin);
            bb.Read(ref this.desk);
            bb.Read(ref this.desk_players_index);
            bb.Read(ref this.joined);
            bb.readLengthLimit = 0;
            bb.Read(ref this.cards);
            bb.Read(ref this.shown);
            bb.Read(ref this.stake);
            bb.Read(ref this.openTarget);
            bb.Read(ref this.stepId);
        }

    }
    /// <summary>
    /// 基本单位: 桌
    /// </summary>
    public partial class Desk : IBBuffer
    {
        /// <summary>
        /// 本桌游戏的流水号()
        /// </summary>
        public int id;
        /// <summary>
        /// 在座的所有玩家( 包括参与者与观众 )
        /// </summary>
        public List<Game1.Player> players;
        /// <summary>
        /// 初始 52 张牌
        /// </summary>
        public List<Poker.Card> cards;
        /// <summary>
        /// 已下注金额
        /// </summary>
        public long totalStake;

        public virtual ushort GetPackageId()
        {
            return TypeIdMaps<Desk>.typeId;
        }

        public virtual void ToBBuffer(BBuffer bb)
        {
            bb.Write(this.id);
            bb.Write(this.players);
            bb.Write(default(List<Poker.Card>));
            bb.Write(this.totalStake);
        }

        public virtual void FromBBuffer(BBuffer bb)
        {
            bb.Read(ref this.id);
            bb.readLengthLimit = 0;
            bb.Read(ref this.players);
            bb.readLengthLimit = 0;
            bb.Read(ref this.cards);
            bb.Read(ref this.totalStake);
        }

    }
}
namespace DB_Lobby
{
    /// <summary>
    /// 试修改金币 成功
    /// </summary>
    public partial class ChangeCoinSuccess : Global.Response
    {

        public override ushort GetPackageId()
        {
            return TypeIdMaps<ChangeCoinSuccess>.typeId;
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
    /// 试修改金币 失败
    /// </summary>
    public partial class ChangeCoinFail : Global.Response
    {
        /// <summary>
        /// 失败原因
        /// </summary>
        public string reason;

        public override ushort GetPackageId()
        {
            return TypeIdMaps<ChangeCoinFail>.typeId;
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
    /// 金币变化之消息推送( 发生于金币变化后 )
    /// </summary>
    public partial class SyncCoin : IBBuffer
    {
        /// <summary>
        /// 版本号( 如果从多个渠道收到通知则保留该值最大的 )
        /// </summary>
        public int versionNumber;
        /// <summary>
        /// 账号id
        /// </summary>
        public int accountId;
        /// <summary>
        /// 原金币值
        /// </summary>
        public long original;
        /// <summary>
        /// 加减值
        /// </summary>
        public long val;
        /// <summary>
        /// 当前金币值
        /// </summary>
        public long curr;
        /// <summary>
        /// 操作原因
        /// </summary>
        public string reason;

        public virtual ushort GetPackageId()
        {
            return TypeIdMaps<SyncCoin>.typeId;
        }

        public virtual void ToBBuffer(BBuffer bb)
        {
            bb.Write(this.versionNumber);
            bb.Write(this.accountId);
            bb.Write(this.original);
            bb.Write(this.val);
            bb.Write(this.curr);
            bb.Write(this.reason);
        }

        public virtual void FromBBuffer(BBuffer bb)
        {
            bb.Read(ref this.versionNumber);
            bb.Read(ref this.accountId);
            bb.Read(ref this.original);
            bb.Read(ref this.val);
            bb.Read(ref this.curr);
            bb.readLengthLimit = 0;
            bb.Read(ref this.reason);
        }

    }
    /// <summary>
    /// 获取用户信息成功
    /// </summary>
    public partial class GetUserInfoSuccess : Global.Response
    {
        public Global.UserInfo userInfo;

        public override ushort GetPackageId()
        {
            return TypeIdMaps<GetUserInfoSuccess>.typeId;
        }

        public override void ToBBuffer(BBuffer bb)
        {
            base.ToBBuffer(bb);
            bb.Write(this.userInfo);
        }

        public override void FromBBuffer(BBuffer bb)
        {
            base.FromBBuffer(bb);
            bb.Read(ref this.userInfo);
        }

    }
    /// <summary>
    /// 获取用户信息失败
    /// </summary>
    public partial class GetUserInfoFail : Global.Response
    {
        /// <summary>
        /// 失败原因
        /// </summary>
        public string reason;

        public override ushort GetPackageId()
        {
            return TypeIdMaps<GetUserInfoFail>.typeId;
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
}
    public static class AllTypes
    {
        public static void Register()
        {
            // BBuffer.Register<string>(1);
            BBuffer.Register<BBuffer>(2);
            BBuffer.Register<Client_Lobby.Join>(3);
            BBuffer.Register<Client_Lobby.PullRooms>(4);
            BBuffer.Register<Client_Lobby.JoinRoom>(5);
            BBuffer.Register<Client_Lobby.ChangeCoin>(6);
            BBuffer.Register<Game1_Client.JoinSuccess>(7);
            BBuffer.Register<Client_Game1.Join>(8);
            BBuffer.Register<Client_Game1.Quit>(9);
            BBuffer.Register<Lobby_Client.JoinSuccess>(10);
            BBuffer.Register<Lobby_Client.PullRoomsSuccess>(11);
            BBuffer.Register<List<Global.RoomInfo>>(12);
            BBuffer.Register<Lobby_Client.PullRoomsFail>(13);
            BBuffer.Register<Lobby_Client.JoinRoomSuccess>(14);
            BBuffer.Register<Global.IPAddress>(15);
            BBuffer.Register<Lobby_Client.JoinRoomFail>(16);
            BBuffer.Register<Lobby_Client.SyncCoin>(17);
            BBuffer.Register<Lobby_Client.ChangeCoinSuccess>(18);
            BBuffer.Register<Lobby_Client.ChangeCoinFail>(19);
            BBuffer.Register<Game1_Lobby.JoinSuccess>(20);
            BBuffer.Register<Global.Response>(21);
            BBuffer.Register<Game1_Lobby.JoinFail>(22);
            BBuffer.Register<Game1_Lobby.Quit>(23);
            BBuffer.Register<Lobby_Game1.Join>(24);
            BBuffer.Register<Global.Request>(25);
            BBuffer.Register<Lobby_Login.JoinSuccess>(26);
            BBuffer.Register<Lobby_Login.JoinFail>(27);
            BBuffer.Register<Login_Lobby.Join>(28);
            BBuffer.Register<Login_Client.LoginSuccess>(29);
            BBuffer.Register<Login_Client.LoginFail>(30);
            BBuffer.Register<DB_Login.AuthResult>(31);
            BBuffer.Register<Login_DB.Auth>(32);
            BBuffer.Register<Client_Login.Login>(33);
            BBuffer.Register<Global.ServiceInfo>(34);
            BBuffer.Register<Global.UserInfo>(35);
            BBuffer.Register<Global.RoomInfo>(36);
            BBuffer.Register<Global.RoomInfo_扎金花>(37);
            BBuffer.Register<Poker.Suit>(38);
            BBuffer.Register<List<Poker.Card>>(39);
            BBuffer.Register<Lobby_DB.ChangeCoin>(40);
            BBuffer.Register<Lobby_DB.GetUserInfo>(41);
            BBuffer.Register<Game1.Player>(42);
            BBuffer.Register<Game1.Desk>(43);
            BBuffer.Register<List<Game1.Player>>(44);
            BBuffer.Register<DB_Lobby.ChangeCoinSuccess>(45);
            BBuffer.Register<DB_Lobby.ChangeCoinFail>(46);
            BBuffer.Register<DB_Lobby.SyncCoin>(47);
            BBuffer.Register<DB_Lobby.GetUserInfoSuccess>(48);
            BBuffer.Register<DB_Lobby.GetUserInfoFail>(49);

            BBuffer.handlers = new Action<IBBuffer>[50];
        }
    }
}
