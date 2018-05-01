
#include "xx.h"

namespace PKG
{
	struct PkgGenMd5
	{
		static constexpr char const* value = "bbd82eda64925618d4195ddb09310fc8";
    };

namespace CatchFish_Client
{
    // 进入游戏成功
    class JoinSuccess;
    using JoinSuccess_p = xx::Ptr<JoinSuccess>;
    using JoinSuccess_r = xx::Ref<JoinSuccess>;

    // 进入失败
    class JoinFail;
    using JoinFail_p = xx::Ptr<JoinFail>;
    using JoinFail_r = xx::Ref<JoinFail>;

    // 要按成员顺序优先级依次处理
    class FullSync;
    using FullSync_p = xx::Ptr<FullSync>;
    using FullSync_r = xx::Ref<FullSync>;

    // 要按成员顺序优先级依次处理
    class FrameEvents;
    using FrameEvents_p = xx::Ptr<FrameEvents>;
    using FrameEvents_r = xx::Ref<FrameEvents>;

}
namespace Client_CatchFish
{
    // 客户端与服务器建立连接后的首包
    class Join;
    using Join_p = xx::Ptr<Join>;
    using Join_r = xx::Ref<Join>;

    // 正常离开游戏( 立即, 并非断线等待超时后清掉 )
    class Leave;
    using Leave_p = xx::Ptr<Leave>;
    using Leave_r = xx::Ref<Leave>;

    // 开火( 单次, 或频繁单次模拟伪连发 )
    class Fire;
    using Fire_p = xx::Ptr<Fire>;
    using Fire_r = xx::Ref<Fire>;

    // 玩家开始开火( 连发, 仅适合帧同步服务器算法 )
    class FireBegin;
    using FireBegin_p = xx::Ptr<FireBegin>;
    using FireBegin_r = xx::Ref<FireBegin>;

    // 玩家持续开火时调整角度( 连发, 仅适合帧同步服务器算法 )
    class FireChangeAngle;
    using FireChangeAngle_p = xx::Ptr<FireChangeAngle>;
    using FireChangeAngle_r = xx::Ref<FireChangeAngle>;

    // 玩家停止持续开火( 连发, 仅适合帧同步服务器算法 )
    class FireEnd;
    using FireEnd_p = xx::Ptr<FireEnd>;
    using FireEnd_r = xx::Ref<FireEnd>;

}
namespace CatchFish
{
    // 圆形碰撞区
    class CollisionArea;
    using CollisionArea_p = xx::Ptr<CollisionArea>;
    using CollisionArea_r = xx::Ref<CollisionArea>;

    // 鱼帧动画信息
    class FishSpriteFrame;
    using FishSpriteFrame_p = xx::Ptr<FishSpriteFrame>;
    using FishSpriteFrame_r = xx::Ref<FishSpriteFrame>;

    // 鱼配置信息
    class FishConfig;
    using FishConfig_p = xx::Ptr<FishConfig>;
    using FishConfig_r = xx::Ref<FishConfig>;

    // 游戏配置信息( 配置信息并不会随着网络同步而下发, 反序列化后需要手工还原 )
    class Config;
    using Config_p = xx::Ptr<Config>;
    using Config_r = xx::Ref<Config>;

    // 玩家
    class Player;
    using Player_p = xx::Ptr<Player>;
    using Player_r = xx::Ref<Player>;

    // 鱼和子弹的基类
    class MoveObject;
    using MoveObject_p = xx::Ptr<MoveObject>;
    using MoveObject_r = xx::Ref<MoveObject>;

    // 子弹
    class Bullet;
    using Bullet_p = xx::Ptr<Bullet>;
    using Bullet_r = xx::Ref<Bullet>;

    // 鱼
    class Fish;
    using Fish_p = xx::Ptr<Fish>;
    using Fish_r = xx::Ref<Fish>;

    // 场景
    class Scene;
    using Scene_p = xx::Ptr<Scene>;
    using Scene_r = xx::Ref<Scene>;

}
namespace CatchFish.Events
{
    // 玩家离开( 比进入的处理优先级高 )
    class LeavePlayer;
    using LeavePlayer_p = xx::Ptr<LeavePlayer>;
    using LeavePlayer_r = xx::Ref<LeavePlayer>;

    // 玩家进入
    class JoinPlayer;
    using JoinPlayer_p = xx::Ptr<JoinPlayer>;
    using JoinPlayer_r = xx::Ref<JoinPlayer>;

    // 玩家开火( 单次 )
    class Fire;
    using Fire_p = xx::Ptr<Fire>;
    using Fire_r = xx::Ref<Fire>;

    // 玩家开始开火( 连发, 仅适合帧同步服务器算法 )
    class FireBegin;
    using FireBegin_p = xx::Ptr<FireBegin>;
    using FireBegin_r = xx::Ref<FireBegin>;

    // 玩家开始开火( 连发, 仅适合帧同步服务器算法 )
    class FireChangeAngle;
    using FireChangeAngle_p = xx::Ptr<FireChangeAngle>;
    using FireChangeAngle_r = xx::Ref<FireChangeAngle>;

    // 玩家停止开火( 连发, 仅适合帧同步服务器算法 )
    class FireEnd;
    using FireEnd_p = xx::Ptr<FireEnd>;
    using FireEnd_r = xx::Ref<FireEnd>;

    // 子弹命中( 与鱼死分离. 鱼死相关可能要等服务器跨线程回调送回结果才能下发 )
    class BulletHit;
    using BulletHit_p = xx::Ptr<BulletHit>;
    using BulletHit_r = xx::Ref<BulletHit>;

    // 鱼被打死
    class FishDead;
    using FishDead_p = xx::Ptr<FishDead>;
    using FishDead_r = xx::Ref<FishDead>;

}
namespace CatchFish
{
    // 鱼和子弹的基类
    class MoveObject : public xx::Object
    {
    public:
        // 所在场景
        PKG::CatchFish::Scene_p scene;
        // 序列号( 当发生碰撞时用于标识 )
        int32_t serial = 0;
        // 创建时的帧编号
        int32_t bornFrameNumber = 0;
        // 创建时的位置
        ::xx::Pos bornPos;
        // 当前位置
        ::xx::Pos pos;
        // 每帧的移动增量
        ::xx::Pos moveInc;
        // 逻辑角度
        float angle = 0;

        typedef MoveObject ThisType;
        typedef xx::Object BaseType;
	    MoveObject(xx::MemPool* mempool);
	    MoveObject(xx::BBuffer *bb);
		MoveObject(MoveObject const&) = delete;
		MoveObject& operator=(MoveObject const&) = delete;
        void ToString(xx::String &str) const override;
        void ToStringCore(xx::String &str) const override;
        void ToBBuffer(xx::BBuffer &bb) const override;
        int FromBBuffer(xx::BBuffer &bb) override;
    };
}
namespace CatchFish_Client
{
    // 进入游戏成功
    class JoinSuccess : public xx::Object
    {
    public:
        // 座位索引( 0: 左上  1: 右上  2: 左下 3: 右下 )
        int32_t sitIndex = 0;

        typedef JoinSuccess ThisType;
        typedef xx::Object BaseType;
	    JoinSuccess(xx::MemPool* mempool);
	    JoinSuccess(xx::BBuffer *bb);
		JoinSuccess(JoinSuccess const&) = delete;
		JoinSuccess& operator=(JoinSuccess const&) = delete;
        void ToString(xx::String &str) const override;
        void ToStringCore(xx::String &str) const override;
        void ToBBuffer(xx::BBuffer &bb) const override;
        int FromBBuffer(xx::BBuffer &bb) override;
    };
}
namespace CatchFish.Events
{
    // 玩家停止开火( 连发, 仅适合帧同步服务器算法 )
    class FireEnd : public xx::Object
    {
    public:
        // 座位索引( 0: 左上  1: 右上  2: 左下 3: 右下 )
        int32_t sitIndex = 0;

        typedef FireEnd ThisType;
        typedef xx::Object BaseType;
	    FireEnd(xx::MemPool* mempool);
	    FireEnd(xx::BBuffer *bb);
		FireEnd(FireEnd const&) = delete;
		FireEnd& operator=(FireEnd const&) = delete;
        void ToString(xx::String &str) const override;
        void ToStringCore(xx::String &str) const override;
        void ToBBuffer(xx::BBuffer &bb) const override;
        int FromBBuffer(xx::BBuffer &bb) override;
    };
    // 玩家开始开火( 连发, 仅适合帧同步服务器算法 )
    class FireChangeAngle : public xx::Object
    {
    public:
        // 座位索引( 0: 左上  1: 右上  2: 左下 3: 右下 )
        int32_t sitIndex = 0;
        // 射击角度( 相对于炮台自己的正方向角度 )
        float angle = 0;

        typedef FireChangeAngle ThisType;
        typedef xx::Object BaseType;
	    FireChangeAngle(xx::MemPool* mempool);
	    FireChangeAngle(xx::BBuffer *bb);
		FireChangeAngle(FireChangeAngle const&) = delete;
		FireChangeAngle& operator=(FireChangeAngle const&) = delete;
        void ToString(xx::String &str) const override;
        void ToStringCore(xx::String &str) const override;
        void ToBBuffer(xx::BBuffer &bb) const override;
        int FromBBuffer(xx::BBuffer &bb) override;
    };
    // 玩家开始开火( 连发, 仅适合帧同步服务器算法 )
    class FireBegin : public xx::Object
    {
    public:
        // 座位索引( 0: 左上  1: 右上  2: 左下 3: 右下 )
        int32_t sitIndex = 0;
        // 射击角度( 相对于炮台自己的正方向角度 )
        float angle = 0;

        typedef FireBegin ThisType;
        typedef xx::Object BaseType;
	    FireBegin(xx::MemPool* mempool);
	    FireBegin(xx::BBuffer *bb);
		FireBegin(FireBegin const&) = delete;
		FireBegin& operator=(FireBegin const&) = delete;
        void ToString(xx::String &str) const override;
        void ToStringCore(xx::String &str) const override;
        void ToBBuffer(xx::BBuffer &bb) const override;
        int FromBBuffer(xx::BBuffer &bb) override;
    };
    // 玩家开火( 单次 )
    class Fire : public xx::Object
    {
    public:
        // 座位索引( 0: 左上  1: 右上  2: 左下 3: 右下 )
        int32_t sitIndex = 0;
        // 当时的帧编号
        int32_t frameNumber = 0;
        // 子弹流水号
        int32_t bulletSerial = 0;
        // 金币价值( 也可理解为倍率 )
        int64_t coin = 0;
        // 射击角度( 相对于炮台自己的正方向角度 )
        float angle = 0;

        typedef Fire ThisType;
        typedef xx::Object BaseType;
	    Fire(xx::MemPool* mempool);
	    Fire(xx::BBuffer *bb);
		Fire(Fire const&) = delete;
		Fire& operator=(Fire const&) = delete;
        void ToString(xx::String &str) const override;
        void ToStringCore(xx::String &str) const override;
        void ToBBuffer(xx::BBuffer &bb) const override;
        int FromBBuffer(xx::BBuffer &bb) override;
    };
    // 玩家进入
    class JoinPlayer : public xx::Object
    {
    public:
        // 玩家名字
        xx::String_p name;
        // 座位索引( 0: 左上  1: 右上  2: 左下 3: 右下 )
        int32_t sitIndex = 0;

        typedef JoinPlayer ThisType;
        typedef xx::Object BaseType;
	    JoinPlayer(xx::MemPool* mempool);
	    JoinPlayer(xx::BBuffer *bb);
		JoinPlayer(JoinPlayer const&) = delete;
		JoinPlayer& operator=(JoinPlayer const&) = delete;
        void ToString(xx::String &str) const override;
        void ToStringCore(xx::String &str) const override;
        void ToBBuffer(xx::BBuffer &bb) const override;
        int FromBBuffer(xx::BBuffer &bb) override;
    };
    // 玩家离开( 比进入的处理优先级高 )
    class LeavePlayer : public xx::Object
    {
    public:
        // 座位索引( 0: 左上  1: 右上  2: 左下 3: 右下 )
        int32_t sitIndex = 0;

        typedef LeavePlayer ThisType;
        typedef xx::Object BaseType;
	    LeavePlayer(xx::MemPool* mempool);
	    LeavePlayer(xx::BBuffer *bb);
		LeavePlayer(LeavePlayer const&) = delete;
		LeavePlayer& operator=(LeavePlayer const&) = delete;
        void ToString(xx::String &str) const override;
        void ToStringCore(xx::String &str) const override;
        void ToBBuffer(xx::BBuffer &bb) const override;
        int FromBBuffer(xx::BBuffer &bb) override;
    };
}
namespace CatchFish
{
    // 场景
    class Scene : public xx::Object
    {
    public:
        // 帧编号, 每帧 + 1
        int32_t frameNumber = 0;
        xx::List_p<PKG::CatchFish::Player_p> players;
        xx::List_p<PKG::CatchFish::Fish_p> fishs;
        xx::List_p<PKG::CatchFish::Bullet_p> bullets;

        typedef Scene ThisType;
        typedef xx::Object BaseType;
	    Scene(xx::MemPool* mempool);
	    Scene(xx::BBuffer *bb);
		Scene(Scene const&) = delete;
		Scene& operator=(Scene const&) = delete;
        void ToString(xx::String &str) const override;
        void ToStringCore(xx::String &str) const override;
        void ToBBuffer(xx::BBuffer &bb) const override;
        int FromBBuffer(xx::BBuffer &bb) override;
    };
    // 鱼
    class Fish : public PKG::CatchFish::MoveObject
    {
    public:
        // 鱼类编号
        int32_t typeId = 0;
        // 逻辑角度之整数版, 创建鱼或更新角度时同步更新, 方便在 cfg 中对 offsets 查表
        int32_t intAngle = 0;
        // 鱼的配置信息( 不参与网络传输, 需要根据 typeId 去 cfgs 定位手工还原 )
        PKG::CatchFish::FishConfig_p cfg;

        typedef Fish ThisType;
        typedef PKG::CatchFish::MoveObject BaseType;
	    Fish(xx::MemPool* mempool);
	    Fish(xx::BBuffer *bb);
		Fish(Fish const&) = delete;
		Fish& operator=(Fish const&) = delete;
        void ToString(xx::String &str) const override;
        void ToStringCore(xx::String &str) const override;
        void ToBBuffer(xx::BBuffer &bb) const override;
        int FromBBuffer(xx::BBuffer &bb) override;
    };
    // 子弹
    class Bullet : public PKG::CatchFish::MoveObject
    {
    public:
        // 哪个玩家射的
        PKG::CatchFish::Player_p shooter;
        // 金币价值( 也可理解为倍率 )
        int64_t coin = 0;

        typedef Bullet ThisType;
        typedef PKG::CatchFish::MoveObject BaseType;
	    Bullet(xx::MemPool* mempool);
	    Bullet(xx::BBuffer *bb);
		Bullet(Bullet const&) = delete;
		Bullet& operator=(Bullet const&) = delete;
        void ToString(xx::String &str) const override;
        void ToStringCore(xx::String &str) const override;
        void ToBBuffer(xx::BBuffer &bb) const override;
        int FromBBuffer(xx::BBuffer &bb) override;
    };
    // 玩家
    class Player : public xx::Object
    {
    public:
        // 所在场景
        PKG::CatchFish::Scene_p scene;
        // 位于 Scene.players 中的下标, 会因为交换删除而变化
        int32_t indexAtScenePlayers = 0;
        // 玩家名字
        xx::String_p name;
        // 座位索引( 0: 左上  1: 右上  2: 左下 3: 右下 ), 同时也用于标识玩家
        int32_t sitIndex = 0;
        // 当前金币数
        int64_t coin = 0;

        typedef Player ThisType;
        typedef xx::Object BaseType;
	    Player(xx::MemPool* mempool);
	    Player(xx::BBuffer *bb);
		Player(Player const&) = delete;
		Player& operator=(Player const&) = delete;
        void ToString(xx::String &str) const override;
        void ToStringCore(xx::String &str) const override;
        void ToBBuffer(xx::BBuffer &bb) const override;
        int FromBBuffer(xx::BBuffer &bb) override;
    };
}
namespace CatchFish.Events
{
    // 子弹命中( 与鱼死分离. 鱼死相关可能要等服务器跨线程回调送回结果才能下发 )
    class BulletHit : public xx::Object
    {
    public:
        // 座位索引( 0: 左上  1: 右上  2: 左下 3: 右下 )
        int32_t sitIndex = 0;
        // 子弹流水号
        int32_t bulletSerial = 0;

        typedef BulletHit ThisType;
        typedef xx::Object BaseType;
	    BulletHit(xx::MemPool* mempool);
	    BulletHit(xx::BBuffer *bb);
		BulletHit(BulletHit const&) = delete;
		BulletHit& operator=(BulletHit const&) = delete;
        void ToString(xx::String &str) const override;
        void ToStringCore(xx::String &str) const override;
        void ToBBuffer(xx::BBuffer &bb) const override;
        int FromBBuffer(xx::BBuffer &bb) override;
    };
}
namespace CatchFish
{
    // 游戏配置信息( 配置信息并不会随着网络同步而下发, 反序列化后需要手工还原 )
    class Config : public xx::Object
    {
    public:
        // 所有鱼的配置信息
        xx::List_p<PKG::CatchFish::FishConfig_p> fishCfgs;

        typedef Config ThisType;
        typedef xx::Object BaseType;
	    Config(xx::MemPool* mempool);
	    Config(xx::BBuffer *bb);
		Config(Config const&) = delete;
		Config& operator=(Config const&) = delete;
        void ToString(xx::String &str) const override;
        void ToStringCore(xx::String &str) const override;
        void ToBBuffer(xx::BBuffer &bb) const override;
        int FromBBuffer(xx::BBuffer &bb) override;
    };
    // 鱼帧动画信息
    class FishSpriteFrame : public xx::Object
    {
    public:
        // 打包前的散文件名
        xx::String_p fileName;
        // 基于当前帧图的圆形碰撞区明细表
        xx::List_p<PKG::CatchFish::CollisionArea_p> collisionAreas;

        typedef FishSpriteFrame ThisType;
        typedef xx::Object BaseType;
	    FishSpriteFrame(xx::MemPool* mempool);
	    FishSpriteFrame(xx::BBuffer *bb);
		FishSpriteFrame(FishSpriteFrame const&) = delete;
		FishSpriteFrame& operator=(FishSpriteFrame const&) = delete;
        void ToString(xx::String &str) const override;
        void ToStringCore(xx::String &str) const override;
        void ToBBuffer(xx::BBuffer &bb) const override;
        int FromBBuffer(xx::BBuffer &bb) override;
    };
    // 圆形碰撞区
    class CollisionArea : public xx::Object
    {
    public:
        // 初始方向的位置偏移( 运算时要根据角度来算实际偏移值 )
        ::xx::Pos offset;
        // 初始方向的位置偏移( 按 256 划分精度的角度值为下标, 运行时先行计算出来 )
        xx::List_p<::xx::Pos> offsets;
        // 半径
        int32_t radius = 0;

        typedef CollisionArea ThisType;
        typedef xx::Object BaseType;
	    CollisionArea(xx::MemPool* mempool);
	    CollisionArea(xx::BBuffer *bb);
		CollisionArea(CollisionArea const&) = delete;
		CollisionArea& operator=(CollisionArea const&) = delete;
        void ToString(xx::String &str) const override;
        void ToStringCore(xx::String &str) const override;
        void ToBBuffer(xx::BBuffer &bb) const override;
        int FromBBuffer(xx::BBuffer &bb) override;
    };
}
namespace Client_CatchFish
{
    // 玩家停止持续开火( 连发, 仅适合帧同步服务器算法 )
    class FireEnd : public xx::Object
    {
    public:

        typedef FireEnd ThisType;
        typedef xx::Object BaseType;
	    FireEnd(xx::MemPool* mempool);
	    FireEnd(xx::BBuffer *bb);
		FireEnd(FireEnd const&) = delete;
		FireEnd& operator=(FireEnd const&) = delete;
        void ToString(xx::String &str) const override;
        void ToStringCore(xx::String &str) const override;
        void ToBBuffer(xx::BBuffer &bb) const override;
        int FromBBuffer(xx::BBuffer &bb) override;
    };
    // 玩家持续开火时调整角度( 连发, 仅适合帧同步服务器算法 )
    class FireChangeAngle : public xx::Object
    {
    public:
        // 射击角度( 相对于炮台自己的正方向角度 )
        float angle = 0;

        typedef FireChangeAngle ThisType;
        typedef xx::Object BaseType;
	    FireChangeAngle(xx::MemPool* mempool);
	    FireChangeAngle(xx::BBuffer *bb);
		FireChangeAngle(FireChangeAngle const&) = delete;
		FireChangeAngle& operator=(FireChangeAngle const&) = delete;
        void ToString(xx::String &str) const override;
        void ToStringCore(xx::String &str) const override;
        void ToBBuffer(xx::BBuffer &bb) const override;
        int FromBBuffer(xx::BBuffer &bb) override;
    };
    // 玩家开始开火( 连发, 仅适合帧同步服务器算法 )
    class FireBegin : public xx::Object
    {
    public:
        // 射击角度( 相对于炮台自己的正方向角度 )
        float angle = 0;

        typedef FireBegin ThisType;
        typedef xx::Object BaseType;
	    FireBegin(xx::MemPool* mempool);
	    FireBegin(xx::BBuffer *bb);
		FireBegin(FireBegin const&) = delete;
		FireBegin& operator=(FireBegin const&) = delete;
        void ToString(xx::String &str) const override;
        void ToStringCore(xx::String &str) const override;
        void ToBBuffer(xx::BBuffer &bb) const override;
        int FromBBuffer(xx::BBuffer &bb) override;
    };
    // 开火( 单次, 或频繁单次模拟伪连发 )
    class Fire : public xx::Object
    {
    public:
        // 当时的帧编号
        int32_t frameNumber = 0;
        // 子弹流水号
        int32_t bulletSerial = 0;
        // 金币价值( 也可理解为倍率 )
        int64_t coin = 0;
        // 射击角度( 相对于炮台自己的正方向角度 )
        float angle = 0;

        typedef Fire ThisType;
        typedef xx::Object BaseType;
	    Fire(xx::MemPool* mempool);
	    Fire(xx::BBuffer *bb);
		Fire(Fire const&) = delete;
		Fire& operator=(Fire const&) = delete;
        void ToString(xx::String &str) const override;
        void ToStringCore(xx::String &str) const override;
        void ToBBuffer(xx::BBuffer &bb) const override;
        int FromBBuffer(xx::BBuffer &bb) override;
    };
    // 正常离开游戏( 立即, 并非断线等待超时后清掉 )
    class Leave : public xx::Object
    {
    public:

        typedef Leave ThisType;
        typedef xx::Object BaseType;
	    Leave(xx::MemPool* mempool);
	    Leave(xx::BBuffer *bb);
		Leave(Leave const&) = delete;
		Leave& operator=(Leave const&) = delete;
        void ToString(xx::String &str) const override;
        void ToStringCore(xx::String &str) const override;
        void ToBBuffer(xx::BBuffer &bb) const override;
        int FromBBuffer(xx::BBuffer &bb) override;
    };
    // 客户端与服务器建立连接后的首包
    class Join : public xx::Object
    {
    public:
        // 客户端进游戏前自己填的名字. 可以写磁盘存在本地.
        xx::String_p username;

        typedef Join ThisType;
        typedef xx::Object BaseType;
	    Join(xx::MemPool* mempool);
	    Join(xx::BBuffer *bb);
		Join(Join const&) = delete;
		Join& operator=(Join const&) = delete;
        void ToString(xx::String &str) const override;
        void ToStringCore(xx::String &str) const override;
        void ToBBuffer(xx::BBuffer &bb) const override;
        int FromBBuffer(xx::BBuffer &bb) override;
    };
}
namespace CatchFish_Client
{
    // 要按成员顺序优先级依次处理
    class FrameEvents : public xx::Object
    {
    public:
        // 当时的帧编号
        int32_t frameNumber = 0;
        // 一帧内所有离开的玩家列表( 有序 )
        xx::List_p<PKG::CatchFish::Events::LeavePlayer_p> leaves;
        // 一帧内所有进入的玩家列表( 有序 )
        xx::List_p<PKG::CatchFish::Events::JoinPlayer_p> joins;
        // 多个玩家的子弹 命中 信息( 相同玩家可能有多条 )
        xx::List_p<PKG::CatchFish::Events::BulletHit_p> hitss;
        // 多条鱼 死亡 & 结算 信息
        xx::List_p<PKG::CatchFish::Events::FishDead_p> fishDeads;
        // 多个玩家的单次开火信息( 自带 frameNumber, 可能小于当前值 )
        xx::List_p<PKG::CatchFish::Events::Fire_p> fires;
        // 多个玩家的持续开火结束信息
        xx::List_p<PKG::CatchFish::Events::FireEnd_p> fireEnds;
        // 多个玩家的持续开火开始信息
        xx::List_p<PKG::CatchFish::Events::FireBegin_p> fireBegins;
        // 多个玩家的持续开火角度调整信息
        xx::List_p<PKG::CatchFish::Events::FireChangeAngle_p> fireChangeAngles;

        typedef FrameEvents ThisType;
        typedef xx::Object BaseType;
	    FrameEvents(xx::MemPool* mempool);
	    FrameEvents(xx::BBuffer *bb);
		FrameEvents(FrameEvents const&) = delete;
		FrameEvents& operator=(FrameEvents const&) = delete;
        void ToString(xx::String &str) const override;
        void ToStringCore(xx::String &str) const override;
        void ToBBuffer(xx::BBuffer &bb) const override;
        int FromBBuffer(xx::BBuffer &bb) override;
    };
    // 要按成员顺序优先级依次处理
    class FullSync : public xx::Object
    {
    public:
        PKG::CatchFish::Scene_p scene;

        typedef FullSync ThisType;
        typedef xx::Object BaseType;
	    FullSync(xx::MemPool* mempool);
	    FullSync(xx::BBuffer *bb);
		FullSync(FullSync const&) = delete;
		FullSync& operator=(FullSync const&) = delete;
        void ToString(xx::String &str) const override;
        void ToStringCore(xx::String &str) const override;
        void ToBBuffer(xx::BBuffer &bb) const override;
        int FromBBuffer(xx::BBuffer &bb) override;
    };
    // 进入失败
    class JoinFail : public xx::Object
    {
    public:
        // 错误提示信息
        xx::String_p errMsg;

        typedef JoinFail ThisType;
        typedef xx::Object BaseType;
	    JoinFail(xx::MemPool* mempool);
	    JoinFail(xx::BBuffer *bb);
		JoinFail(JoinFail const&) = delete;
		JoinFail& operator=(JoinFail const&) = delete;
        void ToString(xx::String &str) const override;
        void ToStringCore(xx::String &str) const override;
        void ToBBuffer(xx::BBuffer &bb) const override;
        int FromBBuffer(xx::BBuffer &bb) override;
    };
}
namespace CatchFish
{
    // 鱼配置信息
    class FishConfig : public xx::Object
    {
    public:
        // 鱼类编号
        int32_t typeId = 0;
        // 打死鱼的金币所得基数( 也可理解成倍率 )
        int64_t coin = 0;
        // 基于整个鱼的最大晃动范围的圆形检测区( 粗判 )
        PKG::CatchFish::CollisionArea_p collisionArea;
        // 鱼帧动画信息集合
        xx::List_p<PKG::CatchFish::FishSpriteFrame_p> frames;

        typedef FishConfig ThisType;
        typedef xx::Object BaseType;
	    FishConfig(xx::MemPool* mempool);
	    FishConfig(xx::BBuffer *bb);
		FishConfig(FishConfig const&) = delete;
		FishConfig& operator=(FishConfig const&) = delete;
        void ToString(xx::String &str) const override;
        void ToStringCore(xx::String &str) const override;
        void ToBBuffer(xx::BBuffer &bb) const override;
        int FromBBuffer(xx::BBuffer &bb) override;
    };
}
namespace CatchFish.Events
{
    // 鱼被打死
    class FishDead : public xx::Object
    {
    public:
        // 座位索引( 0: 左上  1: 右上  2: 左下 3: 右下 )
        int32_t sitIndex = 0;
        // 鱼流水号
        int32_t fishSerial = 0;
        // 金币所得
        int64_t coin = 0;

        typedef FishDead ThisType;
        typedef xx::Object BaseType;
	    FishDead(xx::MemPool* mempool);
	    FishDead(xx::BBuffer *bb);
		FishDead(FishDead const&) = delete;
		FishDead& operator=(FishDead const&) = delete;
        void ToString(xx::String &str) const override;
        void ToStringCore(xx::String &str) const override;
        void ToBBuffer(xx::BBuffer &bb) const override;
        int FromBBuffer(xx::BBuffer &bb) override;
    };
}
namespace CatchFish_Client
{
	inline JoinSuccess::JoinSuccess(xx::MemPool* mempool)
        : xx::Object(mempool)
	{
	}
	inline JoinSuccess::JoinSuccess(xx::BBuffer *bb)
        : xx::Object(bb)
	{
	    int rtv = 0;
        if (rtv = bb->Read(sitIndex)) throw rtv;
	}
    inline void JoinSuccess::ToBBuffer(xx::BBuffer &bb) const
    {
        bb.Write(this->sitIndex);
    }
    inline int JoinSuccess::FromBBuffer(xx::BBuffer &bb)
    {
        int rtv = 0;
        if (rtv = bb.Read(this->sitIndex)) return rtv;
        return rtv;
    }

    inline void JoinSuccess::ToString(xx::String &str) const
    {
        if (memHeader().flags)
        {
        	str.Append("[ \"***** recursived *****\" ]");
        	return;
        }
        else memHeader().flags = 1;

        str.Append("{ \"type\" : \"JoinSuccess\"");
        ToStringCore(str);
        str.Append(" }");
        
        memHeader().flags = 0;
    }
    inline void JoinSuccess::ToStringCore(xx::String &str) const
    {
        this->BaseType::ToStringCore(str);
        str.Append(", \"sitIndex\" : ", this->sitIndex);
    }


	inline JoinFail::JoinFail(xx::MemPool* mempool)
        : xx::Object(mempool)
	{
	}
	inline JoinFail::JoinFail(xx::BBuffer *bb)
        : xx::Object(bb)
	{
	    int rtv = 0;
        bb->readLengthLimit = 0;
        if (rtv = bb->Read(errMsg)) throw rtv;
	}
    inline void JoinFail::ToBBuffer(xx::BBuffer &bb) const
    {
        bb.Write(this->errMsg);
    }
    inline int JoinFail::FromBBuffer(xx::BBuffer &bb)
    {
        int rtv = 0;
        bb.readLengthLimit = 0;
        if (rtv = bb.Read(this->errMsg)) return rtv;
        return rtv;
    }

    inline void JoinFail::ToString(xx::String &str) const
    {
        if (memHeader().flags)
        {
        	str.Append("[ \"***** recursived *****\" ]");
        	return;
        }
        else memHeader().flags = 1;

        str.Append("{ \"type\" : \"JoinFail\"");
        ToStringCore(str);
        str.Append(" }");
        
        memHeader().flags = 0;
    }
    inline void JoinFail::ToStringCore(xx::String &str) const
    {
        this->BaseType::ToStringCore(str);
        if (this->errMsg) str.Append(", \"errMsg\" : \"", this->errMsg, "\"");
        else str.Append(", \"errMsg\" : nil");
    }


	inline FullSync::FullSync(xx::MemPool* mempool)
        : xx::Object(mempool)
	{
	}
	inline FullSync::FullSync(xx::BBuffer *bb)
        : xx::Object(bb)
	{
	    int rtv = 0;
        if (rtv = bb->Read(scene)) throw rtv;
	}
    inline void FullSync::ToBBuffer(xx::BBuffer &bb) const
    {
        bb.Write(this->scene);
    }
    inline int FullSync::FromBBuffer(xx::BBuffer &bb)
    {
        int rtv = 0;
        if (rtv = bb.Read(this->scene)) return rtv;
        return rtv;
    }

    inline void FullSync::ToString(xx::String &str) const
    {
        if (memHeader().flags)
        {
        	str.Append("[ \"***** recursived *****\" ]");
        	return;
        }
        else memHeader().flags = 1;

        str.Append("{ \"type\" : \"FullSync\"");
        ToStringCore(str);
        str.Append(" }");
        
        memHeader().flags = 0;
    }
    inline void FullSync::ToStringCore(xx::String &str) const
    {
        this->BaseType::ToStringCore(str);
        str.Append(", \"scene\" : ", this->scene);
    }


	inline FrameEvents::FrameEvents(xx::MemPool* mempool)
        : xx::Object(mempool)
	{
	}
	inline FrameEvents::FrameEvents(xx::BBuffer *bb)
        : xx::Object(bb)
	{
	    int rtv = 0;
        if (rtv = bb->Read(frameNumber)) throw rtv;
        bb->readLengthLimit = 0;
        if (rtv = bb->Read(leaves)) throw rtv;
        bb->readLengthLimit = 0;
        if (rtv = bb->Read(joins)) throw rtv;
        bb->readLengthLimit = 0;
        if (rtv = bb->Read(hitss)) throw rtv;
        bb->readLengthLimit = 0;
        if (rtv = bb->Read(fishDeads)) throw rtv;
        bb->readLengthLimit = 0;
        if (rtv = bb->Read(fires)) throw rtv;
        bb->readLengthLimit = 0;
        if (rtv = bb->Read(fireEnds)) throw rtv;
        bb->readLengthLimit = 0;
        if (rtv = bb->Read(fireBegins)) throw rtv;
        bb->readLengthLimit = 0;
        if (rtv = bb->Read(fireChangeAngles)) throw rtv;
	}
    inline void FrameEvents::ToBBuffer(xx::BBuffer &bb) const
    {
        bb.Write(this->frameNumber);
        bb.Write(this->leaves);
        bb.Write(this->joins);
        bb.Write(this->hitss);
        bb.Write(this->fishDeads);
        bb.Write(this->fires);
        bb.Write(this->fireEnds);
        bb.Write(this->fireBegins);
        bb.Write(this->fireChangeAngles);
    }
    inline int FrameEvents::FromBBuffer(xx::BBuffer &bb)
    {
        int rtv = 0;
        if (rtv = bb.Read(this->frameNumber)) return rtv;
        bb.readLengthLimit = 0;
        if (rtv = bb.Read(this->leaves)) return rtv;
        bb.readLengthLimit = 0;
        if (rtv = bb.Read(this->joins)) return rtv;
        bb.readLengthLimit = 0;
        if (rtv = bb.Read(this->hitss)) return rtv;
        bb.readLengthLimit = 0;
        if (rtv = bb.Read(this->fishDeads)) return rtv;
        bb.readLengthLimit = 0;
        if (rtv = bb.Read(this->fires)) return rtv;
        bb.readLengthLimit = 0;
        if (rtv = bb.Read(this->fireEnds)) return rtv;
        bb.readLengthLimit = 0;
        if (rtv = bb.Read(this->fireBegins)) return rtv;
        bb.readLengthLimit = 0;
        if (rtv = bb.Read(this->fireChangeAngles)) return rtv;
        return rtv;
    }

    inline void FrameEvents::ToString(xx::String &str) const
    {
        if (memHeader().flags)
        {
        	str.Append("[ \"***** recursived *****\" ]");
        	return;
        }
        else memHeader().flags = 1;

        str.Append("{ \"type\" : \"FrameEvents\"");
        ToStringCore(str);
        str.Append(" }");
        
        memHeader().flags = 0;
    }
    inline void FrameEvents::ToStringCore(xx::String &str) const
    {
        this->BaseType::ToStringCore(str);
        str.Append(", \"frameNumber\" : ", this->frameNumber);
        str.Append(", \"leaves\" : ", this->leaves);
        str.Append(", \"joins\" : ", this->joins);
        str.Append(", \"hitss\" : ", this->hitss);
        str.Append(", \"fishDeads\" : ", this->fishDeads);
        str.Append(", \"fires\" : ", this->fires);
        str.Append(", \"fireEnds\" : ", this->fireEnds);
        str.Append(", \"fireBegins\" : ", this->fireBegins);
        str.Append(", \"fireChangeAngles\" : ", this->fireChangeAngles);
    }


}
namespace Client_CatchFish
{
	inline Join::Join(xx::MemPool* mempool)
        : xx::Object(mempool)
	{
	}
	inline Join::Join(xx::BBuffer *bb)
        : xx::Object(bb)
	{
	    int rtv = 0;
        bb->readLengthLimit = 16;
        if (rtv = bb->Read(username)) throw rtv;
	}
    inline void Join::ToBBuffer(xx::BBuffer &bb) const
    {
        bb.Write(this->username);
    }
    inline int Join::FromBBuffer(xx::BBuffer &bb)
    {
        int rtv = 0;
        bb.readLengthLimit = 16;
        if (rtv = bb.Read(this->username)) return rtv;
        return rtv;
    }

    inline void Join::ToString(xx::String &str) const
    {
        if (memHeader().flags)
        {
        	str.Append("[ \"***** recursived *****\" ]");
        	return;
        }
        else memHeader().flags = 1;

        str.Append("{ \"type\" : \"Join\"");
        ToStringCore(str);
        str.Append(" }");
        
        memHeader().flags = 0;
    }
    inline void Join::ToStringCore(xx::String &str) const
    {
        this->BaseType::ToStringCore(str);
        if (this->username) str.Append(", \"username\" : \"", this->username, "\"");
        else str.Append(", \"username\" : nil");
    }


	inline Leave::Leave(xx::MemPool* mempool)
        : xx::Object(mempool)
	{
	}
	inline Leave::Leave(xx::BBuffer *bb)
        : xx::Object(bb)
	{
	}
    inline void Leave::ToBBuffer(xx::BBuffer &bb) const
    {
    }
    inline int Leave::FromBBuffer(xx::BBuffer &bb)
    {
        int rtv = 0;
        return rtv;
    }

    inline void Leave::ToString(xx::String &str) const
    {
        if (memHeader().flags)
        {
        	str.Append("[ \"***** recursived *****\" ]");
        	return;
        }
        else memHeader().flags = 1;

        str.Append("{ \"type\" : \"Leave\"");
        ToStringCore(str);
        str.Append(" }");
        
        memHeader().flags = 0;
    }
    inline void Leave::ToStringCore(xx::String &str) const
    {
        this->BaseType::ToStringCore(str);
    }


	inline Fire::Fire(xx::MemPool* mempool)
        : xx::Object(mempool)
	{
	}
	inline Fire::Fire(xx::BBuffer *bb)
        : xx::Object(bb)
	{
	    int rtv = 0;
        if (rtv = bb->Read(frameNumber)) throw rtv;
        if (rtv = bb->Read(bulletSerial)) throw rtv;
        if (rtv = bb->Read(coin)) throw rtv;
        if (rtv = bb->Read(angle)) throw rtv;
	}
    inline void Fire::ToBBuffer(xx::BBuffer &bb) const
    {
        bb.Write(this->frameNumber);
        bb.Write(this->bulletSerial);
        bb.Write(this->coin);
        bb.Write(this->angle);
    }
    inline int Fire::FromBBuffer(xx::BBuffer &bb)
    {
        int rtv = 0;
        if (rtv = bb.Read(this->frameNumber)) return rtv;
        if (rtv = bb.Read(this->bulletSerial)) return rtv;
        if (rtv = bb.Read(this->coin)) return rtv;
        if (rtv = bb.Read(this->angle)) return rtv;
        return rtv;
    }

    inline void Fire::ToString(xx::String &str) const
    {
        if (memHeader().flags)
        {
        	str.Append("[ \"***** recursived *****\" ]");
        	return;
        }
        else memHeader().flags = 1;

        str.Append("{ \"type\" : \"Fire\"");
        ToStringCore(str);
        str.Append(" }");
        
        memHeader().flags = 0;
    }
    inline void Fire::ToStringCore(xx::String &str) const
    {
        this->BaseType::ToStringCore(str);
        str.Append(", \"frameNumber\" : ", this->frameNumber);
        str.Append(", \"bulletSerial\" : ", this->bulletSerial);
        str.Append(", \"coin\" : ", this->coin);
        str.Append(", \"angle\" : ", this->angle);
    }


	inline FireBegin::FireBegin(xx::MemPool* mempool)
        : xx::Object(mempool)
	{
	}
	inline FireBegin::FireBegin(xx::BBuffer *bb)
        : xx::Object(bb)
	{
	    int rtv = 0;
        if (rtv = bb->Read(angle)) throw rtv;
	}
    inline void FireBegin::ToBBuffer(xx::BBuffer &bb) const
    {
        bb.Write(this->angle);
    }
    inline int FireBegin::FromBBuffer(xx::BBuffer &bb)
    {
        int rtv = 0;
        if (rtv = bb.Read(this->angle)) return rtv;
        return rtv;
    }

    inline void FireBegin::ToString(xx::String &str) const
    {
        if (memHeader().flags)
        {
        	str.Append("[ \"***** recursived *****\" ]");
        	return;
        }
        else memHeader().flags = 1;

        str.Append("{ \"type\" : \"FireBegin\"");
        ToStringCore(str);
        str.Append(" }");
        
        memHeader().flags = 0;
    }
    inline void FireBegin::ToStringCore(xx::String &str) const
    {
        this->BaseType::ToStringCore(str);
        str.Append(", \"angle\" : ", this->angle);
    }


	inline FireChangeAngle::FireChangeAngle(xx::MemPool* mempool)
        : xx::Object(mempool)
	{
	}
	inline FireChangeAngle::FireChangeAngle(xx::BBuffer *bb)
        : xx::Object(bb)
	{
	    int rtv = 0;
        if (rtv = bb->Read(angle)) throw rtv;
	}
    inline void FireChangeAngle::ToBBuffer(xx::BBuffer &bb) const
    {
        bb.Write(this->angle);
    }
    inline int FireChangeAngle::FromBBuffer(xx::BBuffer &bb)
    {
        int rtv = 0;
        if (rtv = bb.Read(this->angle)) return rtv;
        return rtv;
    }

    inline void FireChangeAngle::ToString(xx::String &str) const
    {
        if (memHeader().flags)
        {
        	str.Append("[ \"***** recursived *****\" ]");
        	return;
        }
        else memHeader().flags = 1;

        str.Append("{ \"type\" : \"FireChangeAngle\"");
        ToStringCore(str);
        str.Append(" }");
        
        memHeader().flags = 0;
    }
    inline void FireChangeAngle::ToStringCore(xx::String &str) const
    {
        this->BaseType::ToStringCore(str);
        str.Append(", \"angle\" : ", this->angle);
    }


	inline FireEnd::FireEnd(xx::MemPool* mempool)
        : xx::Object(mempool)
	{
	}
	inline FireEnd::FireEnd(xx::BBuffer *bb)
        : xx::Object(bb)
	{
	}
    inline void FireEnd::ToBBuffer(xx::BBuffer &bb) const
    {
    }
    inline int FireEnd::FromBBuffer(xx::BBuffer &bb)
    {
        int rtv = 0;
        return rtv;
    }

    inline void FireEnd::ToString(xx::String &str) const
    {
        if (memHeader().flags)
        {
        	str.Append("[ \"***** recursived *****\" ]");
        	return;
        }
        else memHeader().flags = 1;

        str.Append("{ \"type\" : \"FireEnd\"");
        ToStringCore(str);
        str.Append(" }");
        
        memHeader().flags = 0;
    }
    inline void FireEnd::ToStringCore(xx::String &str) const
    {
        this->BaseType::ToStringCore(str);
    }


}
namespace CatchFish
{
	inline CollisionArea::CollisionArea(xx::MemPool* mempool)
        : xx::Object(mempool)
	{
	}
	inline CollisionArea::CollisionArea(xx::BBuffer *bb)
        : xx::Object(bb)
	{
	    int rtv = 0;
        if (rtv = bb->Read(offset)) throw rtv;
        bb->readLengthLimit = 0;
        if (rtv = bb->Read(offsets)) throw rtv;
        if (rtv = bb->Read(radius)) throw rtv;
	}
    inline void CollisionArea::ToBBuffer(xx::BBuffer &bb) const
    {
        bb.Write(this->offset);
        bb.Write(this->offsets);
        bb.Write(this->radius);
    }
    inline int CollisionArea::FromBBuffer(xx::BBuffer &bb)
    {
        int rtv = 0;
        if (rtv = bb.Read(this->offset)) return rtv;
        bb.readLengthLimit = 0;
        if (rtv = bb.Read(this->offsets)) return rtv;
        if (rtv = bb.Read(this->radius)) return rtv;
        return rtv;
    }

    inline void CollisionArea::ToString(xx::String &str) const
    {
        if (memHeader().flags)
        {
        	str.Append("[ \"***** recursived *****\" ]");
        	return;
        }
        else memHeader().flags = 1;

        str.Append("{ \"type\" : \"CollisionArea\"");
        ToStringCore(str);
        str.Append(" }");
        
        memHeader().flags = 0;
    }
    inline void CollisionArea::ToStringCore(xx::String &str) const
    {
        this->BaseType::ToStringCore(str);
        str.Append(", \"offset\" : ", this->offset);
        str.Append(", \"offsets\" : ", this->offsets);
        str.Append(", \"radius\" : ", this->radius);
    }


	inline FishSpriteFrame::FishSpriteFrame(xx::MemPool* mempool)
        : xx::Object(mempool)
	{
	}
	inline FishSpriteFrame::FishSpriteFrame(xx::BBuffer *bb)
        : xx::Object(bb)
	{
	    int rtv = 0;
        bb->readLengthLimit = 0;
        if (rtv = bb->Read(fileName)) throw rtv;
        bb->readLengthLimit = 0;
        if (rtv = bb->Read(collisionAreas)) throw rtv;
	}
    inline void FishSpriteFrame::ToBBuffer(xx::BBuffer &bb) const
    {
        bb.Write(this->fileName);
        bb.Write(this->collisionAreas);
    }
    inline int FishSpriteFrame::FromBBuffer(xx::BBuffer &bb)
    {
        int rtv = 0;
        bb.readLengthLimit = 0;
        if (rtv = bb.Read(this->fileName)) return rtv;
        bb.readLengthLimit = 0;
        if (rtv = bb.Read(this->collisionAreas)) return rtv;
        return rtv;
    }

    inline void FishSpriteFrame::ToString(xx::String &str) const
    {
        if (memHeader().flags)
        {
        	str.Append("[ \"***** recursived *****\" ]");
        	return;
        }
        else memHeader().flags = 1;

        str.Append("{ \"type\" : \"FishSpriteFrame\"");
        ToStringCore(str);
        str.Append(" }");
        
        memHeader().flags = 0;
    }
    inline void FishSpriteFrame::ToStringCore(xx::String &str) const
    {
        this->BaseType::ToStringCore(str);
        if (this->fileName) str.Append(", \"fileName\" : \"", this->fileName, "\"");
        else str.Append(", \"fileName\" : nil");
        str.Append(", \"collisionAreas\" : ", this->collisionAreas);
    }


	inline FishConfig::FishConfig(xx::MemPool* mempool)
        : xx::Object(mempool)
	{
	}
	inline FishConfig::FishConfig(xx::BBuffer *bb)
        : xx::Object(bb)
	{
	    int rtv = 0;
        if (rtv = bb->Read(typeId)) throw rtv;
        if (rtv = bb->Read(coin)) throw rtv;
        if (rtv = bb->Read(collisionArea)) throw rtv;
        bb->readLengthLimit = 0;
        if (rtv = bb->Read(frames)) throw rtv;
	}
    inline void FishConfig::ToBBuffer(xx::BBuffer &bb) const
    {
        bb.Write(this->typeId);
        bb.Write(this->coin);
        bb.Write(this->collisionArea);
        bb.Write(this->frames);
    }
    inline int FishConfig::FromBBuffer(xx::BBuffer &bb)
    {
        int rtv = 0;
        if (rtv = bb.Read(this->typeId)) return rtv;
        if (rtv = bb.Read(this->coin)) return rtv;
        if (rtv = bb.Read(this->collisionArea)) return rtv;
        bb.readLengthLimit = 0;
        if (rtv = bb.Read(this->frames)) return rtv;
        return rtv;
    }

    inline void FishConfig::ToString(xx::String &str) const
    {
        if (memHeader().flags)
        {
        	str.Append("[ \"***** recursived *****\" ]");
        	return;
        }
        else memHeader().flags = 1;

        str.Append("{ \"type\" : \"FishConfig\"");
        ToStringCore(str);
        str.Append(" }");
        
        memHeader().flags = 0;
    }
    inline void FishConfig::ToStringCore(xx::String &str) const
    {
        this->BaseType::ToStringCore(str);
        str.Append(", \"typeId\" : ", this->typeId);
        str.Append(", \"coin\" : ", this->coin);
        str.Append(", \"collisionArea\" : ", this->collisionArea);
        str.Append(", \"frames\" : ", this->frames);
    }


	inline Config::Config(xx::MemPool* mempool)
        : xx::Object(mempool)
	{
	}
	inline Config::Config(xx::BBuffer *bb)
        : xx::Object(bb)
	{
	    int rtv = 0;
        bb->readLengthLimit = 0;
        if (rtv = bb->Read(fishCfgs)) throw rtv;
	}
    inline void Config::ToBBuffer(xx::BBuffer &bb) const
    {
        bb.Write(this->fishCfgs);
    }
    inline int Config::FromBBuffer(xx::BBuffer &bb)
    {
        int rtv = 0;
        bb.readLengthLimit = 0;
        if (rtv = bb.Read(this->fishCfgs)) return rtv;
        return rtv;
    }

    inline void Config::ToString(xx::String &str) const
    {
        if (memHeader().flags)
        {
        	str.Append("[ \"***** recursived *****\" ]");
        	return;
        }
        else memHeader().flags = 1;

        str.Append("{ \"type\" : \"Config\"");
        ToStringCore(str);
        str.Append(" }");
        
        memHeader().flags = 0;
    }
    inline void Config::ToStringCore(xx::String &str) const
    {
        this->BaseType::ToStringCore(str);
        str.Append(", \"fishCfgs\" : ", this->fishCfgs);
    }


	inline Player::Player(xx::MemPool* mempool)
        : xx::Object(mempool)
	{
	}
	inline Player::Player(xx::BBuffer *bb)
        : xx::Object(bb)
	{
	    int rtv = 0;
        if (rtv = bb->Read(scene)) throw rtv;
        if (rtv = bb->Read(indexAtScenePlayers)) throw rtv;
        bb->readLengthLimit = 0;
        if (rtv = bb->Read(name)) throw rtv;
        if (rtv = bb->Read(sitIndex)) throw rtv;
        if (rtv = bb->Read(coin)) throw rtv;
	}
    inline void Player::ToBBuffer(xx::BBuffer &bb) const
    {
        bb.Write(this->scene);
        bb.Write(this->indexAtScenePlayers);
        bb.Write(this->name);
        bb.Write(this->sitIndex);
        bb.Write(this->coin);
    }
    inline int Player::FromBBuffer(xx::BBuffer &bb)
    {
        int rtv = 0;
        if (rtv = bb.Read(this->scene)) return rtv;
        if (rtv = bb.Read(this->indexAtScenePlayers)) return rtv;
        bb.readLengthLimit = 0;
        if (rtv = bb.Read(this->name)) return rtv;
        if (rtv = bb.Read(this->sitIndex)) return rtv;
        if (rtv = bb.Read(this->coin)) return rtv;
        return rtv;
    }

    inline void Player::ToString(xx::String &str) const
    {
        if (memHeader().flags)
        {
        	str.Append("[ \"***** recursived *****\" ]");
        	return;
        }
        else memHeader().flags = 1;

        str.Append("{ \"type\" : \"Player\"");
        ToStringCore(str);
        str.Append(" }");
        
        memHeader().flags = 0;
    }
    inline void Player::ToStringCore(xx::String &str) const
    {
        this->BaseType::ToStringCore(str);
        str.Append(", \"scene\" : ", this->scene);
        str.Append(", \"indexAtScenePlayers\" : ", this->indexAtScenePlayers);
        if (this->name) str.Append(", \"name\" : \"", this->name, "\"");
        else str.Append(", \"name\" : nil");
        str.Append(", \"sitIndex\" : ", this->sitIndex);
        str.Append(", \"coin\" : ", this->coin);
    }


	inline MoveObject::MoveObject(xx::MemPool* mempool)
        : xx::Object(mempool)
	{
	}
	inline MoveObject::MoveObject(xx::BBuffer *bb)
        : xx::Object(bb)
	{
	    int rtv = 0;
        if (rtv = bb->Read(scene)) throw rtv;
        if (rtv = bb->Read(serial)) throw rtv;
        if (rtv = bb->Read(bornFrameNumber)) throw rtv;
        if (rtv = bb->Read(bornPos)) throw rtv;
        if (rtv = bb->Read(pos)) throw rtv;
        if (rtv = bb->Read(moveInc)) throw rtv;
        if (rtv = bb->Read(angle)) throw rtv;
	}
    inline void MoveObject::ToBBuffer(xx::BBuffer &bb) const
    {
        bb.Write(this->scene);
        bb.Write(this->serial);
        bb.Write(this->bornFrameNumber);
        bb.Write(this->bornPos);
        bb.Write(this->pos);
        bb.Write(this->moveInc);
        bb.Write(this->angle);
    }
    inline int MoveObject::FromBBuffer(xx::BBuffer &bb)
    {
        int rtv = 0;
        if (rtv = bb.Read(this->scene)) return rtv;
        if (rtv = bb.Read(this->serial)) return rtv;
        if (rtv = bb.Read(this->bornFrameNumber)) return rtv;
        if (rtv = bb.Read(this->bornPos)) return rtv;
        if (rtv = bb.Read(this->pos)) return rtv;
        if (rtv = bb.Read(this->moveInc)) return rtv;
        if (rtv = bb.Read(this->angle)) return rtv;
        return rtv;
    }

    inline void MoveObject::ToString(xx::String &str) const
    {
        if (memHeader().flags)
        {
        	str.Append("[ \"***** recursived *****\" ]");
        	return;
        }
        else memHeader().flags = 1;

        str.Append("{ \"type\" : \"MoveObject\"");
        ToStringCore(str);
        str.Append(" }");
        
        memHeader().flags = 0;
    }
    inline void MoveObject::ToStringCore(xx::String &str) const
    {
        this->BaseType::ToStringCore(str);
        str.Append(", \"scene\" : ", this->scene);
        str.Append(", \"serial\" : ", this->serial);
        str.Append(", \"bornFrameNumber\" : ", this->bornFrameNumber);
        str.Append(", \"bornPos\" : ", this->bornPos);
        str.Append(", \"pos\" : ", this->pos);
        str.Append(", \"moveInc\" : ", this->moveInc);
        str.Append(", \"angle\" : ", this->angle);
    }


	inline Bullet::Bullet(xx::MemPool* mempool)
        : PKG::CatchFish::MoveObject(mempool)
	{
	}
	inline Bullet::Bullet(xx::BBuffer *bb)
        : PKG::CatchFish::MoveObject(bb)
	{
	    int rtv = 0;
        if (rtv = bb->Read(shooter)) throw rtv;
        if (rtv = bb->Read(coin)) throw rtv;
	}
    inline void Bullet::ToBBuffer(xx::BBuffer &bb) const
    {
        this->BaseType::ToBBuffer(bb);
        bb.Write(this->shooter);
        bb.Write(this->coin);
    }
    inline int Bullet::FromBBuffer(xx::BBuffer &bb)
    {
        int rtv = 0;
        if (rtv = this->BaseType::FromBBuffer(bb)) return rtv;
        if (rtv = bb.Read(this->shooter)) return rtv;
        if (rtv = bb.Read(this->coin)) return rtv;
        return rtv;
    }

    inline void Bullet::ToString(xx::String &str) const
    {
        if (memHeader().flags)
        {
        	str.Append("[ \"***** recursived *****\" ]");
        	return;
        }
        else memHeader().flags = 1;

        str.Append("{ \"type\" : \"Bullet\"");
        ToStringCore(str);
        str.Append(" }");
        
        memHeader().flags = 0;
    }
    inline void Bullet::ToStringCore(xx::String &str) const
    {
        this->BaseType::ToStringCore(str);
        str.Append(", \"shooter\" : ", this->shooter);
        str.Append(", \"coin\" : ", this->coin);
    }


	inline Fish::Fish(xx::MemPool* mempool)
        : PKG::CatchFish::MoveObject(mempool)
	{
	}
	inline Fish::Fish(xx::BBuffer *bb)
        : PKG::CatchFish::MoveObject(bb)
	{
	    int rtv = 0;
        if (rtv = bb->Read(typeId)) throw rtv;
        if (rtv = bb->Read(intAngle)) throw rtv;
        if (rtv = bb->Read(cfg)) throw rtv;
	}
    inline void Fish::ToBBuffer(xx::BBuffer &bb) const
    {
        this->BaseType::ToBBuffer(bb);
        bb.Write(this->typeId);
        bb.Write(this->intAngle);
        bb.WriteDefaultValue<PKG::CatchFish::FishConfig_p>();
    }
    inline int Fish::FromBBuffer(xx::BBuffer &bb)
    {
        int rtv = 0;
        if (rtv = this->BaseType::FromBBuffer(bb)) return rtv;
        if (rtv = bb.Read(this->typeId)) return rtv;
        if (rtv = bb.Read(this->intAngle)) return rtv;
        if (rtv = bb.Read(this->cfg)) return rtv;
        return rtv;
    }

    inline void Fish::ToString(xx::String &str) const
    {
        if (memHeader().flags)
        {
        	str.Append("[ \"***** recursived *****\" ]");
        	return;
        }
        else memHeader().flags = 1;

        str.Append("{ \"type\" : \"Fish\"");
        ToStringCore(str);
        str.Append(" }");
        
        memHeader().flags = 0;
    }
    inline void Fish::ToStringCore(xx::String &str) const
    {
        this->BaseType::ToStringCore(str);
        str.Append(", \"typeId\" : ", this->typeId);
        str.Append(", \"intAngle\" : ", this->intAngle);
        str.Append(", \"cfg\" : ", this->cfg);
    }


	inline Scene::Scene(xx::MemPool* mempool)
        : xx::Object(mempool)
	{
	}
	inline Scene::Scene(xx::BBuffer *bb)
        : xx::Object(bb)
	{
	    int rtv = 0;
        if (rtv = bb->Read(frameNumber)) throw rtv;
        bb->readLengthLimit = 0;
        if (rtv = bb->Read(players)) throw rtv;
        bb->readLengthLimit = 0;
        if (rtv = bb->Read(fishs)) throw rtv;
        bb->readLengthLimit = 0;
        if (rtv = bb->Read(bullets)) throw rtv;
	}
    inline void Scene::ToBBuffer(xx::BBuffer &bb) const
    {
        bb.Write(this->frameNumber);
        bb.Write(this->players);
        bb.Write(this->fishs);
        bb.Write(this->bullets);
    }
    inline int Scene::FromBBuffer(xx::BBuffer &bb)
    {
        int rtv = 0;
        if (rtv = bb.Read(this->frameNumber)) return rtv;
        bb.readLengthLimit = 0;
        if (rtv = bb.Read(this->players)) return rtv;
        bb.readLengthLimit = 0;
        if (rtv = bb.Read(this->fishs)) return rtv;
        bb.readLengthLimit = 0;
        if (rtv = bb.Read(this->bullets)) return rtv;
        return rtv;
    }

    inline void Scene::ToString(xx::String &str) const
    {
        if (memHeader().flags)
        {
        	str.Append("[ \"***** recursived *****\" ]");
        	return;
        }
        else memHeader().flags = 1;

        str.Append("{ \"type\" : \"Scene\"");
        ToStringCore(str);
        str.Append(" }");
        
        memHeader().flags = 0;
    }
    inline void Scene::ToStringCore(xx::String &str) const
    {
        this->BaseType::ToStringCore(str);
        str.Append(", \"frameNumber\" : ", this->frameNumber);
        str.Append(", \"players\" : ", this->players);
        str.Append(", \"fishs\" : ", this->fishs);
        str.Append(", \"bullets\" : ", this->bullets);
    }


}
namespace CatchFish.Events
{
	inline LeavePlayer::LeavePlayer(xx::MemPool* mempool)
        : xx::Object(mempool)
	{
	}
	inline LeavePlayer::LeavePlayer(xx::BBuffer *bb)
        : xx::Object(bb)
	{
	    int rtv = 0;
        if (rtv = bb->Read(sitIndex)) throw rtv;
	}
    inline void LeavePlayer::ToBBuffer(xx::BBuffer &bb) const
    {
        bb.Write(this->sitIndex);
    }
    inline int LeavePlayer::FromBBuffer(xx::BBuffer &bb)
    {
        int rtv = 0;
        if (rtv = bb.Read(this->sitIndex)) return rtv;
        return rtv;
    }

    inline void LeavePlayer::ToString(xx::String &str) const
    {
        if (memHeader().flags)
        {
        	str.Append("[ \"***** recursived *****\" ]");
        	return;
        }
        else memHeader().flags = 1;

        str.Append("{ \"type\" : \"LeavePlayer\"");
        ToStringCore(str);
        str.Append(" }");
        
        memHeader().flags = 0;
    }
    inline void LeavePlayer::ToStringCore(xx::String &str) const
    {
        this->BaseType::ToStringCore(str);
        str.Append(", \"sitIndex\" : ", this->sitIndex);
    }


	inline JoinPlayer::JoinPlayer(xx::MemPool* mempool)
        : xx::Object(mempool)
	{
	}
	inline JoinPlayer::JoinPlayer(xx::BBuffer *bb)
        : xx::Object(bb)
	{
	    int rtv = 0;
        bb->readLengthLimit = 0;
        if (rtv = bb->Read(name)) throw rtv;
        if (rtv = bb->Read(sitIndex)) throw rtv;
	}
    inline void JoinPlayer::ToBBuffer(xx::BBuffer &bb) const
    {
        bb.Write(this->name);
        bb.Write(this->sitIndex);
    }
    inline int JoinPlayer::FromBBuffer(xx::BBuffer &bb)
    {
        int rtv = 0;
        bb.readLengthLimit = 0;
        if (rtv = bb.Read(this->name)) return rtv;
        if (rtv = bb.Read(this->sitIndex)) return rtv;
        return rtv;
    }

    inline void JoinPlayer::ToString(xx::String &str) const
    {
        if (memHeader().flags)
        {
        	str.Append("[ \"***** recursived *****\" ]");
        	return;
        }
        else memHeader().flags = 1;

        str.Append("{ \"type\" : \"JoinPlayer\"");
        ToStringCore(str);
        str.Append(" }");
        
        memHeader().flags = 0;
    }
    inline void JoinPlayer::ToStringCore(xx::String &str) const
    {
        this->BaseType::ToStringCore(str);
        if (this->name) str.Append(", \"name\" : \"", this->name, "\"");
        else str.Append(", \"name\" : nil");
        str.Append(", \"sitIndex\" : ", this->sitIndex);
    }


	inline Fire::Fire(xx::MemPool* mempool)
        : xx::Object(mempool)
	{
	}
	inline Fire::Fire(xx::BBuffer *bb)
        : xx::Object(bb)
	{
	    int rtv = 0;
        if (rtv = bb->Read(sitIndex)) throw rtv;
        if (rtv = bb->Read(frameNumber)) throw rtv;
        if (rtv = bb->Read(bulletSerial)) throw rtv;
        if (rtv = bb->Read(coin)) throw rtv;
        if (rtv = bb->Read(angle)) throw rtv;
	}
    inline void Fire::ToBBuffer(xx::BBuffer &bb) const
    {
        bb.Write(this->sitIndex);
        bb.Write(this->frameNumber);
        bb.Write(this->bulletSerial);
        bb.Write(this->coin);
        bb.Write(this->angle);
    }
    inline int Fire::FromBBuffer(xx::BBuffer &bb)
    {
        int rtv = 0;
        if (rtv = bb.Read(this->sitIndex)) return rtv;
        if (rtv = bb.Read(this->frameNumber)) return rtv;
        if (rtv = bb.Read(this->bulletSerial)) return rtv;
        if (rtv = bb.Read(this->coin)) return rtv;
        if (rtv = bb.Read(this->angle)) return rtv;
        return rtv;
    }

    inline void Fire::ToString(xx::String &str) const
    {
        if (memHeader().flags)
        {
        	str.Append("[ \"***** recursived *****\" ]");
        	return;
        }
        else memHeader().flags = 1;

        str.Append("{ \"type\" : \"Fire\"");
        ToStringCore(str);
        str.Append(" }");
        
        memHeader().flags = 0;
    }
    inline void Fire::ToStringCore(xx::String &str) const
    {
        this->BaseType::ToStringCore(str);
        str.Append(", \"sitIndex\" : ", this->sitIndex);
        str.Append(", \"frameNumber\" : ", this->frameNumber);
        str.Append(", \"bulletSerial\" : ", this->bulletSerial);
        str.Append(", \"coin\" : ", this->coin);
        str.Append(", \"angle\" : ", this->angle);
    }


	inline FireBegin::FireBegin(xx::MemPool* mempool)
        : xx::Object(mempool)
	{
	}
	inline FireBegin::FireBegin(xx::BBuffer *bb)
        : xx::Object(bb)
	{
	    int rtv = 0;
        if (rtv = bb->Read(sitIndex)) throw rtv;
        if (rtv = bb->Read(angle)) throw rtv;
	}
    inline void FireBegin::ToBBuffer(xx::BBuffer &bb) const
    {
        bb.Write(this->sitIndex);
        bb.Write(this->angle);
    }
    inline int FireBegin::FromBBuffer(xx::BBuffer &bb)
    {
        int rtv = 0;
        if (rtv = bb.Read(this->sitIndex)) return rtv;
        if (rtv = bb.Read(this->angle)) return rtv;
        return rtv;
    }

    inline void FireBegin::ToString(xx::String &str) const
    {
        if (memHeader().flags)
        {
        	str.Append("[ \"***** recursived *****\" ]");
        	return;
        }
        else memHeader().flags = 1;

        str.Append("{ \"type\" : \"FireBegin\"");
        ToStringCore(str);
        str.Append(" }");
        
        memHeader().flags = 0;
    }
    inline void FireBegin::ToStringCore(xx::String &str) const
    {
        this->BaseType::ToStringCore(str);
        str.Append(", \"sitIndex\" : ", this->sitIndex);
        str.Append(", \"angle\" : ", this->angle);
    }


	inline FireChangeAngle::FireChangeAngle(xx::MemPool* mempool)
        : xx::Object(mempool)
	{
	}
	inline FireChangeAngle::FireChangeAngle(xx::BBuffer *bb)
        : xx::Object(bb)
	{
	    int rtv = 0;
        if (rtv = bb->Read(sitIndex)) throw rtv;
        if (rtv = bb->Read(angle)) throw rtv;
	}
    inline void FireChangeAngle::ToBBuffer(xx::BBuffer &bb) const
    {
        bb.Write(this->sitIndex);
        bb.Write(this->angle);
    }
    inline int FireChangeAngle::FromBBuffer(xx::BBuffer &bb)
    {
        int rtv = 0;
        if (rtv = bb.Read(this->sitIndex)) return rtv;
        if (rtv = bb.Read(this->angle)) return rtv;
        return rtv;
    }

    inline void FireChangeAngle::ToString(xx::String &str) const
    {
        if (memHeader().flags)
        {
        	str.Append("[ \"***** recursived *****\" ]");
        	return;
        }
        else memHeader().flags = 1;

        str.Append("{ \"type\" : \"FireChangeAngle\"");
        ToStringCore(str);
        str.Append(" }");
        
        memHeader().flags = 0;
    }
    inline void FireChangeAngle::ToStringCore(xx::String &str) const
    {
        this->BaseType::ToStringCore(str);
        str.Append(", \"sitIndex\" : ", this->sitIndex);
        str.Append(", \"angle\" : ", this->angle);
    }


	inline FireEnd::FireEnd(xx::MemPool* mempool)
        : xx::Object(mempool)
	{
	}
	inline FireEnd::FireEnd(xx::BBuffer *bb)
        : xx::Object(bb)
	{
	    int rtv = 0;
        if (rtv = bb->Read(sitIndex)) throw rtv;
	}
    inline void FireEnd::ToBBuffer(xx::BBuffer &bb) const
    {
        bb.Write(this->sitIndex);
    }
    inline int FireEnd::FromBBuffer(xx::BBuffer &bb)
    {
        int rtv = 0;
        if (rtv = bb.Read(this->sitIndex)) return rtv;
        return rtv;
    }

    inline void FireEnd::ToString(xx::String &str) const
    {
        if (memHeader().flags)
        {
        	str.Append("[ \"***** recursived *****\" ]");
        	return;
        }
        else memHeader().flags = 1;

        str.Append("{ \"type\" : \"FireEnd\"");
        ToStringCore(str);
        str.Append(" }");
        
        memHeader().flags = 0;
    }
    inline void FireEnd::ToStringCore(xx::String &str) const
    {
        this->BaseType::ToStringCore(str);
        str.Append(", \"sitIndex\" : ", this->sitIndex);
    }


	inline BulletHit::BulletHit(xx::MemPool* mempool)
        : xx::Object(mempool)
	{
	}
	inline BulletHit::BulletHit(xx::BBuffer *bb)
        : xx::Object(bb)
	{
	    int rtv = 0;
        if (rtv = bb->Read(sitIndex)) throw rtv;
        if (rtv = bb->Read(bulletSerial)) throw rtv;
	}
    inline void BulletHit::ToBBuffer(xx::BBuffer &bb) const
    {
        bb.Write(this->sitIndex);
        bb.Write(this->bulletSerial);
    }
    inline int BulletHit::FromBBuffer(xx::BBuffer &bb)
    {
        int rtv = 0;
        if (rtv = bb.Read(this->sitIndex)) return rtv;
        if (rtv = bb.Read(this->bulletSerial)) return rtv;
        return rtv;
    }

    inline void BulletHit::ToString(xx::String &str) const
    {
        if (memHeader().flags)
        {
        	str.Append("[ \"***** recursived *****\" ]");
        	return;
        }
        else memHeader().flags = 1;

        str.Append("{ \"type\" : \"BulletHit\"");
        ToStringCore(str);
        str.Append(" }");
        
        memHeader().flags = 0;
    }
    inline void BulletHit::ToStringCore(xx::String &str) const
    {
        this->BaseType::ToStringCore(str);
        str.Append(", \"sitIndex\" : ", this->sitIndex);
        str.Append(", \"bulletSerial\" : ", this->bulletSerial);
    }


	inline FishDead::FishDead(xx::MemPool* mempool)
        : xx::Object(mempool)
	{
	}
	inline FishDead::FishDead(xx::BBuffer *bb)
        : xx::Object(bb)
	{
	    int rtv = 0;
        if (rtv = bb->Read(sitIndex)) throw rtv;
        if (rtv = bb->Read(fishSerial)) throw rtv;
        if (rtv = bb->Read(coin)) throw rtv;
	}
    inline void FishDead::ToBBuffer(xx::BBuffer &bb) const
    {
        bb.Write(this->sitIndex);
        bb.Write(this->fishSerial);
        bb.Write(this->coin);
    }
    inline int FishDead::FromBBuffer(xx::BBuffer &bb)
    {
        int rtv = 0;
        if (rtv = bb.Read(this->sitIndex)) return rtv;
        if (rtv = bb.Read(this->fishSerial)) return rtv;
        if (rtv = bb.Read(this->coin)) return rtv;
        return rtv;
    }

    inline void FishDead::ToString(xx::String &str) const
    {
        if (memHeader().flags)
        {
        	str.Append("[ \"***** recursived *****\" ]");
        	return;
        }
        else memHeader().flags = 1;

        str.Append("{ \"type\" : \"FishDead\"");
        ToStringCore(str);
        str.Append(" }");
        
        memHeader().flags = 0;
    }
    inline void FishDead::ToStringCore(xx::String &str) const
    {
        this->BaseType::ToStringCore(str);
        str.Append(", \"sitIndex\" : ", this->sitIndex);
        str.Append(", \"fishSerial\" : ", this->fishSerial);
        str.Append(", \"coin\" : ", this->coin);
    }


}
}
namespace xx
{
	template<> struct TypeId<PKG::CatchFish_Client::JoinSuccess> { static const uint16_t value = 3; };
	template<> struct TypeId<PKG::CatchFish_Client::JoinFail> { static const uint16_t value = 4; };
	template<> struct TypeId<PKG::CatchFish_Client::FullSync> { static const uint16_t value = 5; };
	template<> struct TypeId<PKG::CatchFish::Scene> { static const uint16_t value = 6; };
	template<> struct TypeId<PKG::CatchFish_Client::FrameEvents> { static const uint16_t value = 7; };
	template<> struct TypeId<xx::List<PKG::CatchFish::Events::LeavePlayer_p>> { static const uint16_t value = 8; };
	template<> struct TypeId<xx::List<PKG::CatchFish::Events::JoinPlayer_p>> { static const uint16_t value = 9; };
	template<> struct TypeId<xx::List<PKG::CatchFish::Events::BulletHit_p>> { static const uint16_t value = 10; };
	template<> struct TypeId<xx::List<PKG::CatchFish::Events::FishDead_p>> { static const uint16_t value = 11; };
	template<> struct TypeId<xx::List<PKG::CatchFish::Events::Fire_p>> { static const uint16_t value = 12; };
	template<> struct TypeId<xx::List<PKG::CatchFish::Events::FireEnd_p>> { static const uint16_t value = 13; };
	template<> struct TypeId<xx::List<PKG::CatchFish::Events::FireBegin_p>> { static const uint16_t value = 14; };
	template<> struct TypeId<xx::List<PKG::CatchFish::Events::FireChangeAngle_p>> { static const uint16_t value = 15; };
	template<> struct TypeId<PKG::Client_CatchFish::Join> { static const uint16_t value = 16; };
	template<> struct TypeId<PKG::Client_CatchFish::Leave> { static const uint16_t value = 17; };
	template<> struct TypeId<PKG::Client_CatchFish::Fire> { static const uint16_t value = 18; };
	template<> struct TypeId<PKG::Client_CatchFish::FireBegin> { static const uint16_t value = 19; };
	template<> struct TypeId<PKG::Client_CatchFish::FireChangeAngle> { static const uint16_t value = 20; };
	template<> struct TypeId<PKG::Client_CatchFish::FireEnd> { static const uint16_t value = 21; };
	template<> struct TypeId<PKG::CatchFish::CollisionArea> { static const uint16_t value = 22; };
	template<> struct TypeId<xx::List<::xx::Pos>> { static const uint16_t value = 23; };
	template<> struct TypeId<PKG::CatchFish::FishSpriteFrame> { static const uint16_t value = 24; };
	template<> struct TypeId<xx::List<PKG::CatchFish::CollisionArea_p>> { static const uint16_t value = 25; };
	template<> struct TypeId<PKG::CatchFish::FishConfig> { static const uint16_t value = 26; };
	template<> struct TypeId<xx::List<PKG::CatchFish::FishSpriteFrame_p>> { static const uint16_t value = 27; };
	template<> struct TypeId<PKG::CatchFish::Config> { static const uint16_t value = 28; };
	template<> struct TypeId<xx::List<PKG::CatchFish::FishConfig_p>> { static const uint16_t value = 29; };
	template<> struct TypeId<PKG::CatchFish::Player> { static const uint16_t value = 30; };
	template<> struct TypeId<PKG::CatchFish::MoveObject> { static const uint16_t value = 31; };
	template<> struct TypeId<PKG::CatchFish::Bullet> { static const uint16_t value = 32; };
	template<> struct TypeId<PKG::CatchFish::Fish> { static const uint16_t value = 33; };
	template<> struct TypeId<xx::List<PKG::CatchFish::Player_p>> { static const uint16_t value = 34; };
	template<> struct TypeId<xx::List<PKG::CatchFish::Fish_p>> { static const uint16_t value = 35; };
	template<> struct TypeId<xx::List<PKG::CatchFish::Bullet_p>> { static const uint16_t value = 36; };
	template<> struct TypeId<PKG::CatchFish::Events::LeavePlayer> { static const uint16_t value = 37; };
	template<> struct TypeId<PKG::CatchFish::Events::JoinPlayer> { static const uint16_t value = 38; };
	template<> struct TypeId<PKG::CatchFish::Events::Fire> { static const uint16_t value = 39; };
	template<> struct TypeId<PKG::CatchFish::Events::FireBegin> { static const uint16_t value = 40; };
	template<> struct TypeId<PKG::CatchFish::Events::FireChangeAngle> { static const uint16_t value = 41; };
	template<> struct TypeId<PKG::CatchFish::Events::FireEnd> { static const uint16_t value = 42; };
	template<> struct TypeId<PKG::CatchFish::Events::BulletHit> { static const uint16_t value = 43; };
	template<> struct TypeId<PKG::CatchFish::Events::FishDead> { static const uint16_t value = 44; };
}
namespace PKG
{
	inline void AllTypesRegister()
	{
	    xx::MemPool::Register<PKG::CatchFish_Client::JoinSuccess, xx::Object>();
	    xx::MemPool::Register<PKG::CatchFish_Client::JoinFail, xx::Object>();
	    xx::MemPool::Register<PKG::CatchFish_Client::FullSync, xx::Object>();
	    xx::MemPool::Register<PKG::CatchFish::Scene, xx::Object>();
	    xx::MemPool::Register<PKG::CatchFish_Client::FrameEvents, xx::Object>();
	    xx::MemPool::Register<xx::List<PKG::CatchFish::Events::LeavePlayer_p>, xx::Object>();
	    xx::MemPool::Register<xx::List<PKG::CatchFish::Events::JoinPlayer_p>, xx::Object>();
	    xx::MemPool::Register<xx::List<PKG::CatchFish::Events::BulletHit_p>, xx::Object>();
	    xx::MemPool::Register<xx::List<PKG::CatchFish::Events::FishDead_p>, xx::Object>();
	    xx::MemPool::Register<xx::List<PKG::CatchFish::Events::Fire_p>, xx::Object>();
	    xx::MemPool::Register<xx::List<PKG::CatchFish::Events::FireEnd_p>, xx::Object>();
	    xx::MemPool::Register<xx::List<PKG::CatchFish::Events::FireBegin_p>, xx::Object>();
	    xx::MemPool::Register<xx::List<PKG::CatchFish::Events::FireChangeAngle_p>, xx::Object>();
	    xx::MemPool::Register<PKG::Client_CatchFish::Join, xx::Object>();
	    xx::MemPool::Register<PKG::Client_CatchFish::Leave, xx::Object>();
	    xx::MemPool::Register<PKG::Client_CatchFish::Fire, xx::Object>();
	    xx::MemPool::Register<PKG::Client_CatchFish::FireBegin, xx::Object>();
	    xx::MemPool::Register<PKG::Client_CatchFish::FireChangeAngle, xx::Object>();
	    xx::MemPool::Register<PKG::Client_CatchFish::FireEnd, xx::Object>();
	    xx::MemPool::Register<PKG::CatchFish::CollisionArea, xx::Object>();
	    xx::MemPool::Register<xx::List<::xx::Pos>, xx::Object>();
	    xx::MemPool::Register<PKG::CatchFish::FishSpriteFrame, xx::Object>();
	    xx::MemPool::Register<xx::List<PKG::CatchFish::CollisionArea_p>, xx::Object>();
	    xx::MemPool::Register<PKG::CatchFish::FishConfig, xx::Object>();
	    xx::MemPool::Register<xx::List<PKG::CatchFish::FishSpriteFrame_p>, xx::Object>();
	    xx::MemPool::Register<PKG::CatchFish::Config, xx::Object>();
	    xx::MemPool::Register<xx::List<PKG::CatchFish::FishConfig_p>, xx::Object>();
	    xx::MemPool::Register<PKG::CatchFish::Player, xx::Object>();
	    xx::MemPool::Register<PKG::CatchFish::MoveObject, xx::Object>();
	    xx::MemPool::Register<PKG::CatchFish::Bullet, PKG::CatchFish::MoveObject>();
	    xx::MemPool::Register<PKG::CatchFish::Fish, PKG::CatchFish::MoveObject>();
	    xx::MemPool::Register<xx::List<PKG::CatchFish::Player_p>, xx::Object>();
	    xx::MemPool::Register<xx::List<PKG::CatchFish::Fish_p>, xx::Object>();
	    xx::MemPool::Register<xx::List<PKG::CatchFish::Bullet_p>, xx::Object>();
	    xx::MemPool::Register<PKG::CatchFish::Events::LeavePlayer, xx::Object>();
	    xx::MemPool::Register<PKG::CatchFish::Events::JoinPlayer, xx::Object>();
	    xx::MemPool::Register<PKG::CatchFish::Events::Fire, xx::Object>();
	    xx::MemPool::Register<PKG::CatchFish::Events::FireBegin, xx::Object>();
	    xx::MemPool::Register<PKG::CatchFish::Events::FireChangeAngle, xx::Object>();
	    xx::MemPool::Register<PKG::CatchFish::Events::FireEnd, xx::Object>();
	    xx::MemPool::Register<PKG::CatchFish::Events::BulletHit, xx::Object>();
	    xx::MemPool::Register<PKG::CatchFish::Events::FishDead, xx::Object>();
	}
}
