#pragma warning disable 0169, 0414
using TemplateLibrary;

// 客户端直连Fish服务器的简易设计包

namespace CatchFish
{
    [Desc("圆形碰撞区")]
    class CollisionArea
    {
        [Desc("初始方向的位置偏移( 运算时要根据角度来算实际偏移值 )")]
        xx.Pos offset;

        [Desc("初始方向的位置偏移( 按 256 划分精度的角度值为下标, 运行时先行计算出来 )")]
        List<xx.Pos> offsets;

        [Desc("半径")]
        int radius;
    }

    [Desc("鱼帧动画信息")]
    class FishSpriteFrame
    {
        [Desc("打包前的散文件名( 帧定位名 )")]
        string fileName;

        [Desc("基于当前帧图的圆形碰撞区明细表")]
        List<CollisionArea> collisionAreas;
    }

    [Desc("鱼配置信息")]
    class FishConfig
    {
        [Desc("鱼类编号")]
        int typeId;

        [Desc("鱼名")]
        string name;

        [Desc("打死鱼的金币所得基数( 也可理解成倍率 )")]
        long coin;

        [Desc("帧总数")]
        int frameCount;

        [Desc("基于整个鱼的最大晃动范围的圆形检测区( 粗判 )")]
        CollisionArea collisionArea;

        [Desc("鱼帧动画信息集合")]
        List<FishSpriteFrame> frames;

        [Desc("鱼动画( 仅客户端 )")]
        Animation anim;
    }

    [Desc("游戏配置信息( 配置信息并不会随着网络同步而下发, 反序列化后需要手工还原 )")]
    class Config
    {
        [Desc("所有鱼的配置信息")]
        List<FishConfig> fishCfgs;

        [Desc("子弹配置( 当前就一种子弹，先这样放置 )")]
        int bulletRadius;

        [Desc("玩家初始金币值")]
        long playerInitCoin;
    }


    [Desc("玩家")]
    class Player
    {
        [Desc("位于容器时的下标")]
        int indexAtContainer;

        [Desc("玩家名字")]
        string name;

        [Desc("座位索引( 0: 左上  1: 右上  2: 左下 3: 右下 ), 同时也用于标识玩家")]
        int sitIndex;

        [Desc("当前金币数")]
        long coin;

        [Desc("子弹的自增流水号")]
        int bulletSerialNumber;

        [Desc("所有子弹")]
        List<Bullet> bullets;

        [Desc("玩家网络上下文, 不参与网络传输")]
        PlayerContext ctx;
    }

    [Desc("鱼和子弹的基类")]
    class MoveObject
    {
        [Desc("位于容器时的下标")]
        int indexAtContainer;

        [Desc("序列号( 当发生碰撞时用于标识 )")]
        int serialNumber;

        [Desc("创建时的帧编号")]
        int bornFrameNumber;

        [Desc("创建时的位置")]
        xx.Pos bornPos;

        [Desc("当前位置")]
        xx.Pos pos;

        [Desc("一次性计算出来的每帧的坐标增量( 以稳定 fps 为大前提 )")]
        xx.Pos moveInc;

        [Desc("逻辑角度")]
        float angle;

        [Desc("显示用主体精灵( 仅客户端 )")]
        Sprite spriteBody;
    }


    [Desc("子弹")]
    class Bullet : MoveObject
    {
        [Desc("金币价值( 也可理解为倍率 )")]
        long coin;
    }

    [Desc("鱼")]
    class Fish : MoveObject
    {
        [Desc("鱼类编号")]
        int typeId;

        [Desc("逻辑角度之整数版, 创建鱼或更新角度时同步更新, 方便在 cfg 中对 offsets 查表")]
        int intAngle;

        [Desc("鱼的配置信息( 不参与网络传输, 需要根据 typeId 去 cfgs 定位手工还原 )")]
        [NotSerialize]
        FishConfig cfg;

        [Desc("消失点配置")]
        xx.Pos moveTo;

        [Desc("当前移动步数( 步数一到就认为鱼已移至目的地可以清掉了 )")]
        int moveStep = 0;

        [Desc("总移动步数")]
        int moveStepCount = 0;
    }

    [Desc("场景")]
    class Scene
    {
        [Desc("帧编号, 每帧 + 1")]
        int frameNumber;

        [Desc("公用随机数发生器")]
        xx.Random rnd;

        [Desc("公用配置信息( 不参与网络传输, 需要根据 typeId 去 cfgs 定位手工还原 )")]
        [NotSerialize]
        Config cfg;

        [Desc("所有玩家( 子弹在玩家下面 )")]
        List<Player> players;

        [Desc("鱼的自增流水号")]
        int fishSerialNumber;

        [Desc("所有鱼")]
        List<Fish> fishs;

        //[Desc("所有子弹")]
        //List<Bullet> bullets;

        [Desc("当前帧所有事件的合并包容器, 服务器专用, 独立发送")]
        [NotSerialize]
        CatchFish_Client.FrameEvents frameEvents;
    }


    // 事件相关, 主用于从服务器单独下发的简易版玩家信息, 不含依赖类
    namespace Events
    {
        [Desc("玩家离开( 比进入的处理优先级高 )")]
        class LeavePlayer
        {
            [Desc("座位索引( 0: 左上  1: 右上  2: 左下 3: 右下 )")]
            int sitIndex;
        }

        [Desc("玩家进入")]
        class JoinPlayer
        {
            [Desc("玩家名字")]
            string name;

            [Desc("座位索引( 0: 左上  1: 右上  2: 左下 3: 右下 )")]
            int sitIndex;

            [Desc("进入的玩家拥有的金币数量")]
            long coin;
        }

        [Desc("玩家开火( 单次 )")]
        class Fire
        {
            [Desc("座位索引( 0: 左上  1: 右上  2: 左下 3: 右下 )")]
            int sitIndex;

            [Desc("起始帧编号")]
            int frameNumber;

            [Desc("子弹流水号")]
            int bulletSerialNumber;

            [Desc("金币价值( 也可理解为倍率 )")]
            long coin;

            [Desc("步进")]
            xx.Pos moveInc;
        }

        [Desc("玩家开始开火( 连发, 仅适合帧同步服务器算法 )")]
        class FireBegin
        {
            [Desc("座位索引( 0: 左上  1: 右上  2: 左下 3: 右下 )")]
            int sitIndex;

            [Desc("射击角度( 相对于炮台自己的正方向角度 )")]
            float angle;
        }

        [Desc("玩家开始开火( 连发, 仅适合帧同步服务器算法 )")]
        class FireChangeAngle
        {
            [Desc("座位索引( 0: 左上  1: 右上  2: 左下 3: 右下 )")]
            int sitIndex;

            [Desc("射击角度( 相对于炮台自己的正方向角度 )")]
            float angle;
        }

        [Desc("玩家停止开火( 连发, 仅适合帧同步服务器算法 )")]
        class FireEnd
        {
            [Desc("座位索引( 0: 左上  1: 右上  2: 左下 3: 右下 )")]
            int sitIndex;
        }

        [Desc("子弹命中( 与鱼死分离. 鱼死相关可能要等服务器跨线程回调送回结果才能下发 )")]
        class BulletHit
        {
            [Desc("座位索引( 0: 左上  1: 右上  2: 左下 3: 右下 )")]
            int sitIndex;

            [Desc("子弹流水号")]
            int bulletSerialNumber;
        }

        [Desc("鱼被打死")]
        class FishDead
        {
            [Desc("座位索引( 0: 左上  1: 右上  2: 左下 3: 右下 )")]
            int sitIndex;

            [Desc("鱼流水号")]
            int fishSerialNumber;

            [Desc("金币所得")]
            long coin;
        }

    }
}

// Client 向 CatchFish Server 发的包
namespace Client_CatchFish
{
    [Desc("客户端与服务器建立连接后的首包")]
    class Join
    {
        [Desc("客户端进游戏前自己填的名字. 可以写磁盘存在本地.")]
        [Limit(64)]
        string username;
    }

    [Desc("正常离开游戏( 立即, 并非断线等待超时后清掉 )")]
    class Leave
    {
    }

    [Desc("开火( 单次, 或频繁单次模拟伪连发 )")]
    class Fire
    {
        [Desc("当时的帧编号")]
        int frameNumber;

        [Desc("子弹流水号")]
        int bulletSerialNumber;

        [Desc("金币价值( 也可理解为倍率 )")]
        long coin;

        [Desc("步进")]
        xx.Pos moveInc;
    }

    [Desc("当前玩家自己的子弹打中鱼")]
    class Hit
    {
        [Desc("子弹流水号")]
        int bulletSerialNumber;

        [Desc("鱼流水号")]
        int fishSerialNumber;
    }

    [Desc("玩家开始开火( 连发, 仅适合帧同步服务器算法 )")]
    class FireBegin
    {
        [Desc("射击角度( 相对于炮台自己的正方向角度 )")]
        float angle;
    }

    [Desc("玩家持续开火时调整角度( 连发, 仅适合帧同步服务器算法 )")]
    class FireChangeAngle
    {
        [Desc("射击角度( 相对于炮台自己的正方向角度 )")]
        float angle;
    }

    [Desc("玩家停止持续开火( 连发, 仅适合帧同步服务器算法 )")]
    class FireEnd
    {
    }
}

// CatchFish Server 向 Client 发的包
namespace CatchFish_Client
{
    [Desc("进入游戏成功")]
    class JoinSuccess
    {
        [Desc("座位索引( 0: 左上  1: 右上  2: 左下 3: 右下 )")]
        int sitIndex;
    }

    [Desc("进入失败")]
    class JoinFail
    {
        [Desc("错误提示信息")]
        string errMsg;
    }

    [Desc("要按成员顺序优先级依次处理")]
    class FullSync
    {
        CatchFish.Scene scene;
    }

    [Desc("要按成员顺序优先级依次处理")]
    class FrameEvents
    {
        [Desc("当时的帧编号")]
        int frameNumber;

        [Desc("一帧内所有离开的玩家列表( 有序 )")]
        List<CatchFish.Events.LeavePlayer> leaves;

        [Desc("一帧内所有进入的玩家列表( 有序 )")]
        List<CatchFish.Events.JoinPlayer> joins;

        [Desc("多条鱼 死亡 & 结算 信息")]
        List<CatchFish.Events.FishDead> fishDeads;

        [Desc("多个玩家的单次开火信息( 自带 frameNumber, 可能小于当前值 )")]
        List<CatchFish.Events.Fire> fires;

        [Desc("多个玩家的持续开火结束信息")]
        List<CatchFish.Events.FireEnd> fireEnds;

        [Desc("多个玩家的持续开火开始信息")]
        List<CatchFish.Events.FireBegin> fireBegins;

        [Desc("多个玩家的持续开火角度调整信息")]
        List<CatchFish.Events.FireChangeAngle> fireChangeAngles;
    }
}
