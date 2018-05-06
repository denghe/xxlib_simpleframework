using System;
using xx;
namespace PKG
{
    public static class PkgGenMd5
    {
        public const string value = "73fe90a587f1d8cfc9b7aaca273a0806"; 
    }

namespace CatchFish_Client
{
    /// <summary>
    /// 进入游戏成功
    /// </summary>
    public partial class JoinSuccess : IBBuffer
    {
        /// <summary>
        /// 座位索引( 0: 左上  1: 右上  2: 左下 3: 右下 )
        /// </summary>
        public int sitIndex;

        public virtual ushort GetPackageId()
        {
            return TypeIdMaps<JoinSuccess>.typeId;
        }

        public virtual void ToBBuffer(BBuffer bb)
        {
            bb.Write(this.sitIndex);
        }

        public virtual void FromBBuffer(BBuffer bb)
        {
            bb.Read(ref this.sitIndex);
        }

    }
    /// <summary>
    /// 进入失败
    /// </summary>
    public partial class JoinFail : IBBuffer
    {
        /// <summary>
        /// 错误提示信息
        /// </summary>
        public string errMsg;

        public virtual ushort GetPackageId()
        {
            return TypeIdMaps<JoinFail>.typeId;
        }

        public virtual void ToBBuffer(BBuffer bb)
        {
            bb.Write(this.errMsg);
        }

        public virtual void FromBBuffer(BBuffer bb)
        {
            bb.readLengthLimit = 0;
            bb.Read(ref this.errMsg);
        }

    }
    /// <summary>
    /// 要按成员顺序优先级依次处理
    /// </summary>
    public partial class FullSync : IBBuffer
    {
        public CatchFish.Scene scene;

        public virtual ushort GetPackageId()
        {
            return TypeIdMaps<FullSync>.typeId;
        }

        public virtual void ToBBuffer(BBuffer bb)
        {
            bb.Write(this.scene);
        }

        public virtual void FromBBuffer(BBuffer bb)
        {
            bb.Read(ref this.scene);
        }

    }
    /// <summary>
    /// 要按成员顺序优先级依次处理
    /// </summary>
    public partial class FrameEvents : IBBuffer
    {
        /// <summary>
        /// 当时的帧编号
        /// </summary>
        public int frameNumber;
        /// <summary>
        /// 一帧内所有离开的玩家列表( 有序 )
        /// </summary>
        public List<CatchFish.Events.LeavePlayer> leaves;
        /// <summary>
        /// 一帧内所有进入的玩家列表( 有序 )
        /// </summary>
        public List<CatchFish.Events.JoinPlayer> joins;
        /// <summary>
        /// 多条鱼 死亡 & 结算 信息
        /// </summary>
        public List<CatchFish.Events.FishDead> fishDeads;
        /// <summary>
        /// 多个玩家的单次开火信息( 自带 frameNumber, 可能小于当前值 )
        /// </summary>
        public List<CatchFish.Events.Fire> fires;
        /// <summary>
        /// 多个玩家的持续开火结束信息
        /// </summary>
        public List<CatchFish.Events.FireEnd> fireEnds;
        /// <summary>
        /// 多个玩家的持续开火开始信息
        /// </summary>
        public List<CatchFish.Events.FireBegin> fireBegins;
        /// <summary>
        /// 多个玩家的持续开火角度调整信息
        /// </summary>
        public List<CatchFish.Events.FireChangeAngle> fireChangeAngles;

        public virtual ushort GetPackageId()
        {
            return TypeIdMaps<FrameEvents>.typeId;
        }

        public virtual void ToBBuffer(BBuffer bb)
        {
            bb.Write(this.frameNumber);
            bb.Write(this.leaves);
            bb.Write(this.joins);
            bb.Write(this.fishDeads);
            bb.Write(this.fires);
            bb.Write(this.fireEnds);
            bb.Write(this.fireBegins);
            bb.Write(this.fireChangeAngles);
        }

        public virtual void FromBBuffer(BBuffer bb)
        {
            bb.Read(ref this.frameNumber);
            bb.readLengthLimit = 0;
            bb.Read(ref this.leaves);
            bb.readLengthLimit = 0;
            bb.Read(ref this.joins);
            bb.readLengthLimit = 0;
            bb.Read(ref this.fishDeads);
            bb.readLengthLimit = 0;
            bb.Read(ref this.fires);
            bb.readLengthLimit = 0;
            bb.Read(ref this.fireEnds);
            bb.readLengthLimit = 0;
            bb.Read(ref this.fireBegins);
            bb.readLengthLimit = 0;
            bb.Read(ref this.fireChangeAngles);
        }

    }
}
namespace Client_CatchFish
{
    /// <summary>
    /// 客户端与服务器建立连接后的首包
    /// </summary>
    public partial class Join : IBBuffer
    {
        /// <summary>
        /// 客户端进游戏前自己填的名字. 可以写磁盘存在本地.
        /// </summary>
        public string username;

        public virtual ushort GetPackageId()
        {
            return TypeIdMaps<Join>.typeId;
        }

        public virtual void ToBBuffer(BBuffer bb)
        {
            bb.Write(this.username);
        }

        public virtual void FromBBuffer(BBuffer bb)
        {
            bb.readLengthLimit = 64;
            bb.Read(ref this.username);
        }

    }
    /// <summary>
    /// 正常离开游戏( 立即, 并非断线等待超时后清掉 )
    /// </summary>
    public partial class Leave : IBBuffer
    {

        public virtual ushort GetPackageId()
        {
            return TypeIdMaps<Leave>.typeId;
        }

        public virtual void ToBBuffer(BBuffer bb)
        {
        }

        public virtual void FromBBuffer(BBuffer bb)
        {
        }

    }
    /// <summary>
    /// 开火( 单次, 或频繁单次模拟伪连发 )
    /// </summary>
    public partial class Fire : IBBuffer
    {
        /// <summary>
        /// 当时的帧编号
        /// </summary>
        public int frameNumber;
        /// <summary>
        /// 子弹流水号
        /// </summary>
        public int bulletSerialNumber;
        /// <summary>
        /// 金币价值( 也可理解为倍率 )
        /// </summary>
        public long coin;
        /// <summary>
        /// 步进
        /// </summary>
        public xx.Pos moveInc;

        public virtual ushort GetPackageId()
        {
            return TypeIdMaps<Fire>.typeId;
        }

        public virtual void ToBBuffer(BBuffer bb)
        {
            bb.Write(this.frameNumber);
            bb.Write(this.bulletSerialNumber);
            bb.Write(this.coin);
            ((IBBuffer)this.moveInc).ToBBuffer(bb);
        }

        public virtual void FromBBuffer(BBuffer bb)
        {
            bb.Read(ref this.frameNumber);
            bb.Read(ref this.bulletSerialNumber);
            bb.Read(ref this.coin);
            ((IBBuffer)this.moveInc).FromBBuffer(bb);
        }

    }
    /// <summary>
    /// 当前玩家自己的子弹打中鱼
    /// </summary>
    public partial class Hit : IBBuffer
    {
        /// <summary>
        /// 子弹流水号
        /// </summary>
        public int bulletSerialNumber;
        /// <summary>
        /// 鱼流水号
        /// </summary>
        public int fishSerialNumber;

        public virtual ushort GetPackageId()
        {
            return TypeIdMaps<Hit>.typeId;
        }

        public virtual void ToBBuffer(BBuffer bb)
        {
            bb.Write(this.bulletSerialNumber);
            bb.Write(this.fishSerialNumber);
        }

        public virtual void FromBBuffer(BBuffer bb)
        {
            bb.Read(ref this.bulletSerialNumber);
            bb.Read(ref this.fishSerialNumber);
        }

    }
    /// <summary>
    /// 玩家开始开火( 连发, 仅适合帧同步服务器算法 )
    /// </summary>
    public partial class FireBegin : IBBuffer
    {
        /// <summary>
        /// 射击角度( 相对于炮台自己的正方向角度 )
        /// </summary>
        public float angle;

        public virtual ushort GetPackageId()
        {
            return TypeIdMaps<FireBegin>.typeId;
        }

        public virtual void ToBBuffer(BBuffer bb)
        {
            bb.Write(this.angle);
        }

        public virtual void FromBBuffer(BBuffer bb)
        {
            bb.Read(ref this.angle);
        }

    }
    /// <summary>
    /// 玩家持续开火时调整角度( 连发, 仅适合帧同步服务器算法 )
    /// </summary>
    public partial class FireChangeAngle : IBBuffer
    {
        /// <summary>
        /// 射击角度( 相对于炮台自己的正方向角度 )
        /// </summary>
        public float angle;

        public virtual ushort GetPackageId()
        {
            return TypeIdMaps<FireChangeAngle>.typeId;
        }

        public virtual void ToBBuffer(BBuffer bb)
        {
            bb.Write(this.angle);
        }

        public virtual void FromBBuffer(BBuffer bb)
        {
            bb.Read(ref this.angle);
        }

    }
    /// <summary>
    /// 玩家停止持续开火( 连发, 仅适合帧同步服务器算法 )
    /// </summary>
    public partial class FireEnd : IBBuffer
    {

        public virtual ushort GetPackageId()
        {
            return TypeIdMaps<FireEnd>.typeId;
        }

        public virtual void ToBBuffer(BBuffer bb)
        {
        }

        public virtual void FromBBuffer(BBuffer bb)
        {
        }

    }
}
namespace CatchFish
{
    /// <summary>
    /// 圆形碰撞区
    /// </summary>
    public partial class CollisionArea : IBBuffer
    {
        /// <summary>
        /// 初始方向的位置偏移( 运算时要根据角度来算实际偏移值 )
        /// </summary>
        public xx.Pos offset;
        /// <summary>
        /// 初始方向的位置偏移( 按 256 划分精度的角度值为下标, 运行时先行计算出来 )
        /// </summary>
        public List<xx.Pos> offsets;
        /// <summary>
        /// 半径
        /// </summary>
        public int radius;

        public virtual ushort GetPackageId()
        {
            return TypeIdMaps<CollisionArea>.typeId;
        }

        public virtual void ToBBuffer(BBuffer bb)
        {
            ((IBBuffer)this.offset).ToBBuffer(bb);
            bb.Write(this.offsets);
            bb.Write(this.radius);
        }

        public virtual void FromBBuffer(BBuffer bb)
        {
            ((IBBuffer)this.offset).FromBBuffer(bb);
            bb.readLengthLimit = 0;
            bb.Read(ref this.offsets);
            bb.Read(ref this.radius);
        }

    }
    /// <summary>
    /// 鱼帧动画信息
    /// </summary>
    public partial class FishSpriteFrame : IBBuffer
    {
        /// <summary>
        /// 打包前的散文件名( 帧定位名 )
        /// </summary>
        public string fileName;
        /// <summary>
        /// 基于当前帧图的圆形碰撞区明细表
        /// </summary>
        public List<CatchFish.CollisionArea> collisionAreas;

        public virtual ushort GetPackageId()
        {
            return TypeIdMaps<FishSpriteFrame>.typeId;
        }

        public virtual void ToBBuffer(BBuffer bb)
        {
            bb.Write(this.fileName);
            bb.Write(this.collisionAreas);
        }

        public virtual void FromBBuffer(BBuffer bb)
        {
            bb.readLengthLimit = 0;
            bb.Read(ref this.fileName);
            bb.readLengthLimit = 0;
            bb.Read(ref this.collisionAreas);
        }

    }
    /// <summary>
    /// 鱼配置信息
    /// </summary>
    public partial class FishConfig : IBBuffer
    {
        /// <summary>
        /// 鱼类编号
        /// </summary>
        public int typeId;
        /// <summary>
        /// 鱼名
        /// </summary>
        public string name;
        /// <summary>
        /// 打死鱼的金币所得基数( 也可理解成倍率 )
        /// </summary>
        public long coin;
        /// <summary>
        /// 帧总数
        /// </summary>
        public int frameCount;
        /// <summary>
        /// 基于整个鱼的最大晃动范围的圆形检测区( 粗判 )
        /// </summary>
        public CatchFish.CollisionArea collisionArea;
        /// <summary>
        /// 鱼帧动画信息集合
        /// </summary>
        public List<CatchFish.FishSpriteFrame> frames;
        /// <summary>
        /// 鱼动画( 仅客户端 )
        /// </summary>
        public Animation anim;

        public virtual ushort GetPackageId()
        {
            return TypeIdMaps<FishConfig>.typeId;
        }

        public virtual void ToBBuffer(BBuffer bb)
        {
            bb.Write(this.typeId);
            bb.Write(this.name);
            bb.Write(this.coin);
            bb.Write(this.frameCount);
            bb.Write(this.collisionArea);
            bb.Write(this.frames);
            bb.Write(this.anim);
        }

        public virtual void FromBBuffer(BBuffer bb)
        {
            bb.Read(ref this.typeId);
            bb.readLengthLimit = 0;
            bb.Read(ref this.name);
            bb.Read(ref this.coin);
            bb.Read(ref this.frameCount);
            bb.Read(ref this.collisionArea);
            bb.readLengthLimit = 0;
            bb.Read(ref this.frames);
            bb.Read(ref this.anim);
        }

    }
    /// <summary>
    /// 游戏配置信息( 配置信息并不会随着网络同步而下发, 反序列化后需要手工还原 )
    /// </summary>
    public partial class Config : IBBuffer
    {
        /// <summary>
        /// 所有鱼的配置信息
        /// </summary>
        public List<CatchFish.FishConfig> fishCfgs;
        /// <summary>
        /// 子弹配置( 当前就一种子弹，先这样放置 )
        /// </summary>
        public int bulletRadius;
        /// <summary>
        /// 玩家初始金币值
        /// </summary>
        public long playerInitCoin;

        public virtual ushort GetPackageId()
        {
            return TypeIdMaps<Config>.typeId;
        }

        public virtual void ToBBuffer(BBuffer bb)
        {
            bb.Write(this.fishCfgs);
            bb.Write(this.bulletRadius);
            bb.Write(this.playerInitCoin);
        }

        public virtual void FromBBuffer(BBuffer bb)
        {
            bb.readLengthLimit = 0;
            bb.Read(ref this.fishCfgs);
            bb.Read(ref this.bulletRadius);
            bb.Read(ref this.playerInitCoin);
        }

    }
    /// <summary>
    /// 玩家
    /// </summary>
    public partial class Player : IBBuffer
    {
        /// <summary>
        /// 位于容器时的下标
        /// </summary>
        public int indexAtContainer;
        /// <summary>
        /// 玩家名字
        /// </summary>
        public string name;
        /// <summary>
        /// 座位索引( 0: 左上  1: 右上  2: 左下 3: 右下 ), 同时也用于标识玩家
        /// </summary>
        public int sitIndex;
        /// <summary>
        /// 当前金币数
        /// </summary>
        public long coin;
        /// <summary>
        /// 子弹的自增流水号
        /// </summary>
        public int bulletSerialNumber;
        /// <summary>
        /// 所有子弹
        /// </summary>
        public List<CatchFish.Bullet> bullets;
        /// <summary>
        /// 玩家网络上下文, 不参与网络传输
        /// </summary>
        public ClientPeer peer;

        public virtual ushort GetPackageId()
        {
            return TypeIdMaps<Player>.typeId;
        }

        public virtual void ToBBuffer(BBuffer bb)
        {
            bb.Write(this.indexAtContainer);
            bb.Write(this.name);
            bb.Write(this.sitIndex);
            bb.Write(this.coin);
            bb.Write(this.bulletSerialNumber);
            bb.Write(this.bullets);
            bb.Write(this.peer);
        }

        public virtual void FromBBuffer(BBuffer bb)
        {
            bb.Read(ref this.indexAtContainer);
            bb.readLengthLimit = 0;
            bb.Read(ref this.name);
            bb.Read(ref this.sitIndex);
            bb.Read(ref this.coin);
            bb.Read(ref this.bulletSerialNumber);
            bb.readLengthLimit = 0;
            bb.Read(ref this.bullets);
            bb.Read(ref this.peer);
        }

    }
    /// <summary>
    /// 鱼和子弹的基类
    /// </summary>
    public partial class MoveObject : IBBuffer
    {
        /// <summary>
        /// 位于容器时的下标
        /// </summary>
        public int indexAtContainer;
        /// <summary>
        /// 序列号( 当发生碰撞时用于标识 )
        /// </summary>
        public int serialNumber;
        /// <summary>
        /// 创建时的帧编号
        /// </summary>
        public int bornFrameNumber;
        /// <summary>
        /// 创建时的位置
        /// </summary>
        public xx.Pos bornPos;
        /// <summary>
        /// 当前位置
        /// </summary>
        public xx.Pos pos;
        /// <summary>
        /// 一次性计算出来的每帧的坐标增量( 以稳定 fps 为大前提 )
        /// </summary>
        public xx.Pos moveInc;
        /// <summary>
        /// 逻辑角度
        /// </summary>
        public float angle;
        /// <summary>
        /// 显示用主体精灵( 仅客户端 )
        /// </summary>
        public Sprite spriteBody;

        public virtual ushort GetPackageId()
        {
            return TypeIdMaps<MoveObject>.typeId;
        }

        public virtual void ToBBuffer(BBuffer bb)
        {
            bb.Write(this.indexAtContainer);
            bb.Write(this.serialNumber);
            bb.Write(this.bornFrameNumber);
            ((IBBuffer)this.bornPos).ToBBuffer(bb);
            ((IBBuffer)this.pos).ToBBuffer(bb);
            ((IBBuffer)this.moveInc).ToBBuffer(bb);
            bb.Write(this.angle);
            bb.Write(this.spriteBody);
        }

        public virtual void FromBBuffer(BBuffer bb)
        {
            bb.Read(ref this.indexAtContainer);
            bb.Read(ref this.serialNumber);
            bb.Read(ref this.bornFrameNumber);
            ((IBBuffer)this.bornPos).FromBBuffer(bb);
            ((IBBuffer)this.pos).FromBBuffer(bb);
            ((IBBuffer)this.moveInc).FromBBuffer(bb);
            bb.Read(ref this.angle);
            bb.Read(ref this.spriteBody);
        }

    }
    /// <summary>
    /// 子弹
    /// </summary>
    public partial class Bullet : CatchFish.MoveObject
    {
        /// <summary>
        /// 金币价值( 也可理解为倍率 )
        /// </summary>
        public long coin;

        public override ushort GetPackageId()
        {
            return TypeIdMaps<Bullet>.typeId;
        }

        public override void ToBBuffer(BBuffer bb)
        {
            base.ToBBuffer(bb);
            bb.Write(this.coin);
        }

        public override void FromBBuffer(BBuffer bb)
        {
            base.FromBBuffer(bb);
            bb.Read(ref this.coin);
        }

    }
    /// <summary>
    /// 鱼
    /// </summary>
    public partial class Fish : CatchFish.MoveObject
    {
        /// <summary>
        /// 鱼类编号
        /// </summary>
        public int typeId;
        /// <summary>
        /// 逻辑角度之整数版, 创建鱼或更新角度时同步更新, 方便在 cfg 中对 offsets 查表
        /// </summary>
        public int intAngle;
        /// <summary>
        /// 鱼的配置信息( 不参与网络传输, 需要根据 typeId 去 cfgs 定位手工还原 )
        /// </summary>
        public CatchFish.FishConfig cfg;
        /// <summary>
        /// 消失点配置
        /// </summary>
        public xx.Pos moveTo;
        /// <summary>
        /// 当前移动步数( 步数一到就认为鱼已移至目的地可以清掉了 )
        /// </summary>
        public int moveStep;
        /// <summary>
        /// 总移动步数
        /// </summary>
        public int moveStepCount;

        public override ushort GetPackageId()
        {
            return TypeIdMaps<Fish>.typeId;
        }

        public override void ToBBuffer(BBuffer bb)
        {
            base.ToBBuffer(bb);
            bb.Write(this.typeId);
            bb.Write(this.intAngle);
            bb.Write(default(CatchFish.FishConfig));
            ((IBBuffer)this.moveTo).ToBBuffer(bb);
            bb.Write(this.moveStep);
            bb.Write(this.moveStepCount);
        }

        public override void FromBBuffer(BBuffer bb)
        {
            base.FromBBuffer(bb);
            bb.Read(ref this.typeId);
            bb.Read(ref this.intAngle);
            bb.Read(ref this.cfg);
            ((IBBuffer)this.moveTo).FromBBuffer(bb);
            bb.Read(ref this.moveStep);
            bb.Read(ref this.moveStepCount);
        }

    }
    /// <summary>
    /// 场景
    /// </summary>
    public partial class Scene : IBBuffer
    {
        /// <summary>
        /// 帧编号, 每帧 + 1
        /// </summary>
        public int frameNumber;
        /// <summary>
        /// 公用随机数发生器
        /// </summary>
        public xx.Random rnd;
        /// <summary>
        /// 所有玩家( 子弹在玩家下面 )
        /// </summary>
        public List<CatchFish.Player> players;
        /// <summary>
        /// 鱼的自增流水号
        /// </summary>
        public int fishSerialNumber;
        /// <summary>
        /// 所有鱼
        /// </summary>
        public List<CatchFish.Fish> fishs;
        /// <summary>
        /// 当前帧所有事件的合并包容器, 服务器专用, 独立发送
        /// </summary>
        public CatchFish_Client.FrameEvents frameEvents;

        public virtual ushort GetPackageId()
        {
            return TypeIdMaps<Scene>.typeId;
        }

        public virtual void ToBBuffer(BBuffer bb)
        {
            bb.Write(this.frameNumber);
            bb.Write(this.rnd);
            bb.Write(this.players);
            bb.Write(this.fishSerialNumber);
            bb.Write(this.fishs);
            bb.Write(default(CatchFish_Client.FrameEvents));
        }

        public virtual void FromBBuffer(BBuffer bb)
        {
            bb.Read(ref this.frameNumber);
            bb.Read(ref this.rnd);
            bb.readLengthLimit = 0;
            bb.Read(ref this.players);
            bb.Read(ref this.fishSerialNumber);
            bb.readLengthLimit = 0;
            bb.Read(ref this.fishs);
            bb.Read(ref this.frameEvents);
        }

    }
}
namespace CatchFish.Events
{
    /// <summary>
    /// 玩家离开( 比进入的处理优先级高 )
    /// </summary>
    public partial class LeavePlayer : IBBuffer
    {
        /// <summary>
        /// 座位索引( 0: 左上  1: 右上  2: 左下 3: 右下 )
        /// </summary>
        public int sitIndex;

        public virtual ushort GetPackageId()
        {
            return TypeIdMaps<LeavePlayer>.typeId;
        }

        public virtual void ToBBuffer(BBuffer bb)
        {
            bb.Write(this.sitIndex);
        }

        public virtual void FromBBuffer(BBuffer bb)
        {
            bb.Read(ref this.sitIndex);
        }

    }
    /// <summary>
    /// 玩家进入
    /// </summary>
    public partial class JoinPlayer : IBBuffer
    {
        /// <summary>
        /// 玩家名字
        /// </summary>
        public string name;
        /// <summary>
        /// 座位索引( 0: 左上  1: 右上  2: 左下 3: 右下 )
        /// </summary>
        public int sitIndex;
        /// <summary>
        /// 进入的玩家拥有的金币数量
        /// </summary>
        public long coin;

        public virtual ushort GetPackageId()
        {
            return TypeIdMaps<JoinPlayer>.typeId;
        }

        public virtual void ToBBuffer(BBuffer bb)
        {
            bb.Write(this.name);
            bb.Write(this.sitIndex);
            bb.Write(this.coin);
        }

        public virtual void FromBBuffer(BBuffer bb)
        {
            bb.readLengthLimit = 0;
            bb.Read(ref this.name);
            bb.Read(ref this.sitIndex);
            bb.Read(ref this.coin);
        }

    }
    /// <summary>
    /// 玩家开火( 单次 )
    /// </summary>
    public partial class Fire : IBBuffer
    {
        /// <summary>
        /// 座位索引( 0: 左上  1: 右上  2: 左下 3: 右下 )
        /// </summary>
        public int sitIndex;
        /// <summary>
        /// 起始帧编号
        /// </summary>
        public int frameNumber;
        /// <summary>
        /// 子弹流水号
        /// </summary>
        public int bulletSerialNumber;
        /// <summary>
        /// 金币价值( 也可理解为倍率 )
        /// </summary>
        public long coin;
        /// <summary>
        /// 步进
        /// </summary>
        public xx.Pos moveInc;

        public virtual ushort GetPackageId()
        {
            return TypeIdMaps<Fire>.typeId;
        }

        public virtual void ToBBuffer(BBuffer bb)
        {
            bb.Write(this.sitIndex);
            bb.Write(this.frameNumber);
            bb.Write(this.bulletSerialNumber);
            bb.Write(this.coin);
            ((IBBuffer)this.moveInc).ToBBuffer(bb);
        }

        public virtual void FromBBuffer(BBuffer bb)
        {
            bb.Read(ref this.sitIndex);
            bb.Read(ref this.frameNumber);
            bb.Read(ref this.bulletSerialNumber);
            bb.Read(ref this.coin);
            ((IBBuffer)this.moveInc).FromBBuffer(bb);
        }

    }
    /// <summary>
    /// 玩家开始开火( 连发, 仅适合帧同步服务器算法 )
    /// </summary>
    public partial class FireBegin : IBBuffer
    {
        /// <summary>
        /// 座位索引( 0: 左上  1: 右上  2: 左下 3: 右下 )
        /// </summary>
        public int sitIndex;
        /// <summary>
        /// 射击角度( 相对于炮台自己的正方向角度 )
        /// </summary>
        public float angle;

        public virtual ushort GetPackageId()
        {
            return TypeIdMaps<FireBegin>.typeId;
        }

        public virtual void ToBBuffer(BBuffer bb)
        {
            bb.Write(this.sitIndex);
            bb.Write(this.angle);
        }

        public virtual void FromBBuffer(BBuffer bb)
        {
            bb.Read(ref this.sitIndex);
            bb.Read(ref this.angle);
        }

    }
    /// <summary>
    /// 玩家开始开火( 连发, 仅适合帧同步服务器算法 )
    /// </summary>
    public partial class FireChangeAngle : IBBuffer
    {
        /// <summary>
        /// 座位索引( 0: 左上  1: 右上  2: 左下 3: 右下 )
        /// </summary>
        public int sitIndex;
        /// <summary>
        /// 射击角度( 相对于炮台自己的正方向角度 )
        /// </summary>
        public float angle;

        public virtual ushort GetPackageId()
        {
            return TypeIdMaps<FireChangeAngle>.typeId;
        }

        public virtual void ToBBuffer(BBuffer bb)
        {
            bb.Write(this.sitIndex);
            bb.Write(this.angle);
        }

        public virtual void FromBBuffer(BBuffer bb)
        {
            bb.Read(ref this.sitIndex);
            bb.Read(ref this.angle);
        }

    }
    /// <summary>
    /// 玩家停止开火( 连发, 仅适合帧同步服务器算法 )
    /// </summary>
    public partial class FireEnd : IBBuffer
    {
        /// <summary>
        /// 座位索引( 0: 左上  1: 右上  2: 左下 3: 右下 )
        /// </summary>
        public int sitIndex;

        public virtual ushort GetPackageId()
        {
            return TypeIdMaps<FireEnd>.typeId;
        }

        public virtual void ToBBuffer(BBuffer bb)
        {
            bb.Write(this.sitIndex);
        }

        public virtual void FromBBuffer(BBuffer bb)
        {
            bb.Read(ref this.sitIndex);
        }

    }
    /// <summary>
    /// 子弹命中( 与鱼死分离. 鱼死相关可能要等服务器跨线程回调送回结果才能下发 )
    /// </summary>
    public partial class BulletHit : IBBuffer
    {
        /// <summary>
        /// 座位索引( 0: 左上  1: 右上  2: 左下 3: 右下 )
        /// </summary>
        public int sitIndex;
        /// <summary>
        /// 子弹流水号
        /// </summary>
        public int bulletSerialNumber;

        public virtual ushort GetPackageId()
        {
            return TypeIdMaps<BulletHit>.typeId;
        }

        public virtual void ToBBuffer(BBuffer bb)
        {
            bb.Write(this.sitIndex);
            bb.Write(this.bulletSerialNumber);
        }

        public virtual void FromBBuffer(BBuffer bb)
        {
            bb.Read(ref this.sitIndex);
            bb.Read(ref this.bulletSerialNumber);
        }

    }
    /// <summary>
    /// 鱼被打死
    /// </summary>
    public partial class FishDead : IBBuffer
    {
        /// <summary>
        /// 座位索引( 0: 左上  1: 右上  2: 左下 3: 右下 )
        /// </summary>
        public int sitIndex;
        /// <summary>
        /// 鱼流水号
        /// </summary>
        public int fishSerialNumber;
        /// <summary>
        /// 金币所得
        /// </summary>
        public long coin;

        public virtual ushort GetPackageId()
        {
            return TypeIdMaps<FishDead>.typeId;
        }

        public virtual void ToBBuffer(BBuffer bb)
        {
            bb.Write(this.sitIndex);
            bb.Write(this.fishSerialNumber);
            bb.Write(this.coin);
        }

        public virtual void FromBBuffer(BBuffer bb)
        {
            bb.Read(ref this.sitIndex);
            bb.Read(ref this.fishSerialNumber);
            bb.Read(ref this.coin);
        }

    }
}
    public static class AllTypes
    {
        public static void Register()
        {
            // BBuffer.Register<string>(1);
            BBuffer.Register<BBuffer>(2);
            BBuffer.Register<CatchFish_Client.JoinSuccess>(3);
            BBuffer.Register<CatchFish_Client.JoinFail>(4);
            BBuffer.Register<CatchFish_Client.FullSync>(5);
            BBuffer.Register<CatchFish.Scene>(6);
            BBuffer.Register<CatchFish_Client.FrameEvents>(7);
            BBuffer.Register<List<CatchFish.Events.LeavePlayer>>(8);
            BBuffer.Register<List<CatchFish.Events.JoinPlayer>>(9);
            BBuffer.Register<List<CatchFish.Events.FishDead>>(11);
            BBuffer.Register<List<CatchFish.Events.Fire>>(12);
            BBuffer.Register<List<CatchFish.Events.FireEnd>>(13);
            BBuffer.Register<List<CatchFish.Events.FireBegin>>(14);
            BBuffer.Register<List<CatchFish.Events.FireChangeAngle>>(15);
            BBuffer.Register<Client_CatchFish.Join>(16);
            BBuffer.Register<Client_CatchFish.Leave>(17);
            BBuffer.Register<Client_CatchFish.Fire>(18);
            BBuffer.Register<Client_CatchFish.Hit>(50);
            BBuffer.Register<Client_CatchFish.FireBegin>(19);
            BBuffer.Register<Client_CatchFish.FireChangeAngle>(20);
            BBuffer.Register<Client_CatchFish.FireEnd>(21);
            BBuffer.Register<CatchFish.CollisionArea>(22);
            BBuffer.Register<List<xx.Pos>>(23);
            BBuffer.Register<CatchFish.FishSpriteFrame>(24);
            BBuffer.Register<List<CatchFish.CollisionArea>>(25);
            BBuffer.Register<CatchFish.FishConfig>(26);
            BBuffer.Register<List<CatchFish.FishSpriteFrame>>(27);
            BBuffer.Register<Animation>(49);
            BBuffer.Register<CatchFish.Config>(28);
            BBuffer.Register<List<CatchFish.FishConfig>>(29);
            BBuffer.Register<CatchFish.Player>(30);
            BBuffer.Register<List<CatchFish.Bullet>>(31);
            BBuffer.Register<ClientPeer>(47);
            BBuffer.Register<CatchFish.MoveObject>(33);
            BBuffer.Register<Sprite>(48);
            BBuffer.Register<CatchFish.Bullet>(34);
            BBuffer.Register<CatchFish.Fish>(35);
            BBuffer.Register<xx.Random>(36);
            BBuffer.Register<List<CatchFish.Player>>(37);
            BBuffer.Register<List<CatchFish.Fish>>(38);
            BBuffer.Register<CatchFish.Events.LeavePlayer>(39);
            BBuffer.Register<CatchFish.Events.JoinPlayer>(40);
            BBuffer.Register<CatchFish.Events.Fire>(41);
            BBuffer.Register<CatchFish.Events.FireBegin>(42);
            BBuffer.Register<CatchFish.Events.FireChangeAngle>(43);
            BBuffer.Register<CatchFish.Events.FireEnd>(44);
            BBuffer.Register<CatchFish.Events.BulletHit>(45);
            BBuffer.Register<CatchFish.Events.FishDead>(46);
        }
    }
}
