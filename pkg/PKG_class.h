#pragma once
#include "xx.h"

namespace PKG
{
	struct PkgGenMd5
	{
		static constexpr char const* value = "95ec433e381c309ee7fcee42f439d390";
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

    // 当前玩家自己的子弹打中鱼
    class Hit;
    using Hit_p = xx::Ptr<Hit>;
    using Hit_r = xx::Ref<Hit>;

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
namespace CatchFish::Events
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
        // 位于容器时的下标
        int32_t indexAtContainer = 0;
        // 序列号( 当发生碰撞时用于标识 )
        int32_t serialNumber = 0;
        // 创建时的帧编号
        int32_t bornFrameNumber = 0;
        // 创建时的位置
        ::xx::Pos bornPos;
        // 当前位置
        ::xx::Pos pos;
        // 一次性计算出来的每帧的坐标增量( 以稳定 fps 为大前提 )
        ::xx::Pos moveInc;
        // 逻辑角度
        float angle = 0;
        // 显示用主体精灵( 仅客户端 )
        ::Sprite spriteBody = nullptr;

        typedef MoveObject ThisType;
        typedef xx::Object BaseType;
	    MoveObject(xx::MemPool* const& mempool) noexcept;
	    MoveObject(xx::BBuffer *bb);
		MoveObject(MoveObject const&) = delete;
		MoveObject& operator=(MoveObject const&) = delete;
        void ToString(xx::String &str) const noexcept override;
        void ToStringCore(xx::String &str) const noexcept override;
        void ToBBuffer(xx::BBuffer &bb) const noexcept override;
        int FromBBuffer(xx::BBuffer &bb) noexcept override;
        void CopyTo(MoveObject* const& o) const noexcept;
        MoveObject* MakeCopy() const noexcept;
        MoveObject_p MakePtrCopy() const noexcept;
        inline static xx::Ptr<ThisType> defaultInstance;
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
	    JoinSuccess(xx::MemPool* const& mempool) noexcept;
	    JoinSuccess(xx::BBuffer *bb);
		JoinSuccess(JoinSuccess const&) = delete;
		JoinSuccess& operator=(JoinSuccess const&) = delete;
        void ToString(xx::String &str) const noexcept override;
        void ToStringCore(xx::String &str) const noexcept override;
        void ToBBuffer(xx::BBuffer &bb) const noexcept override;
        int FromBBuffer(xx::BBuffer &bb) noexcept override;
        void CopyTo(JoinSuccess* const& o) const noexcept;
        JoinSuccess* MakeCopy() const noexcept;
        JoinSuccess_p MakePtrCopy() const noexcept;
        inline static xx::Ptr<ThisType> defaultInstance;
    };
}
namespace CatchFish::Events
{
    // 玩家停止开火( 连发, 仅适合帧同步服务器算法 )
    class FireEnd : public xx::Object
    {
    public:
        // 座位索引( 0: 左上  1: 右上  2: 左下 3: 右下 )
        int32_t sitIndex = 0;

        typedef FireEnd ThisType;
        typedef xx::Object BaseType;
	    FireEnd(xx::MemPool* const& mempool) noexcept;
	    FireEnd(xx::BBuffer *bb);
		FireEnd(FireEnd const&) = delete;
		FireEnd& operator=(FireEnd const&) = delete;
        void ToString(xx::String &str) const noexcept override;
        void ToStringCore(xx::String &str) const noexcept override;
        void ToBBuffer(xx::BBuffer &bb) const noexcept override;
        int FromBBuffer(xx::BBuffer &bb) noexcept override;
        void CopyTo(FireEnd* const& o) const noexcept;
        FireEnd* MakeCopy() const noexcept;
        FireEnd_p MakePtrCopy() const noexcept;
        inline static xx::Ptr<ThisType> defaultInstance;
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
	    FireChangeAngle(xx::MemPool* const& mempool) noexcept;
	    FireChangeAngle(xx::BBuffer *bb);
		FireChangeAngle(FireChangeAngle const&) = delete;
		FireChangeAngle& operator=(FireChangeAngle const&) = delete;
        void ToString(xx::String &str) const noexcept override;
        void ToStringCore(xx::String &str) const noexcept override;
        void ToBBuffer(xx::BBuffer &bb) const noexcept override;
        int FromBBuffer(xx::BBuffer &bb) noexcept override;
        void CopyTo(FireChangeAngle* const& o) const noexcept;
        FireChangeAngle* MakeCopy() const noexcept;
        FireChangeAngle_p MakePtrCopy() const noexcept;
        inline static xx::Ptr<ThisType> defaultInstance;
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
	    FireBegin(xx::MemPool* const& mempool) noexcept;
	    FireBegin(xx::BBuffer *bb);
		FireBegin(FireBegin const&) = delete;
		FireBegin& operator=(FireBegin const&) = delete;
        void ToString(xx::String &str) const noexcept override;
        void ToStringCore(xx::String &str) const noexcept override;
        void ToBBuffer(xx::BBuffer &bb) const noexcept override;
        int FromBBuffer(xx::BBuffer &bb) noexcept override;
        void CopyTo(FireBegin* const& o) const noexcept;
        FireBegin* MakeCopy() const noexcept;
        FireBegin_p MakePtrCopy() const noexcept;
        inline static xx::Ptr<ThisType> defaultInstance;
    };
    // 玩家开火( 单次 )
    class Fire : public xx::Object
    {
    public:
        // 座位索引( 0: 左上  1: 右上  2: 左下 3: 右下 )
        int32_t sitIndex = 0;
        // 起始帧编号
        int32_t frameNumber = 0;
        // 子弹流水号
        int32_t bulletSerialNumber = 0;
        // 金币价值( 也可理解为倍率 )
        int64_t coin = 0;
        // 步进
        ::xx::Pos moveInc;

        typedef Fire ThisType;
        typedef xx::Object BaseType;
	    Fire(xx::MemPool* const& mempool) noexcept;
	    Fire(xx::BBuffer *bb);
		Fire(Fire const&) = delete;
		Fire& operator=(Fire const&) = delete;
        void ToString(xx::String &str) const noexcept override;
        void ToStringCore(xx::String &str) const noexcept override;
        void ToBBuffer(xx::BBuffer &bb) const noexcept override;
        int FromBBuffer(xx::BBuffer &bb) noexcept override;
        void CopyTo(Fire* const& o) const noexcept;
        Fire* MakeCopy() const noexcept;
        Fire_p MakePtrCopy() const noexcept;
        inline static xx::Ptr<ThisType> defaultInstance;
    };
    // 玩家进入
    class JoinPlayer : public xx::Object
    {
    public:
        // 玩家名字
        xx::String_p name;
        // 座位索引( 0: 左上  1: 右上  2: 左下 3: 右下 )
        int32_t sitIndex = 0;
        // 进入的玩家拥有的金币数量
        int64_t coin = 0;

        typedef JoinPlayer ThisType;
        typedef xx::Object BaseType;
	    JoinPlayer(xx::MemPool* const& mempool) noexcept;
	    JoinPlayer(xx::BBuffer *bb);
		JoinPlayer(JoinPlayer const&) = delete;
		JoinPlayer& operator=(JoinPlayer const&) = delete;
        void ToString(xx::String &str) const noexcept override;
        void ToStringCore(xx::String &str) const noexcept override;
        void ToBBuffer(xx::BBuffer &bb) const noexcept override;
        int FromBBuffer(xx::BBuffer &bb) noexcept override;
        void CopyTo(JoinPlayer* const& o) const noexcept;
        JoinPlayer* MakeCopy() const noexcept;
        JoinPlayer_p MakePtrCopy() const noexcept;
        inline static xx::Ptr<ThisType> defaultInstance;
    };
    // 玩家离开( 比进入的处理优先级高 )
    class LeavePlayer : public xx::Object
    {
    public:
        // 座位索引( 0: 左上  1: 右上  2: 左下 3: 右下 )
        int32_t sitIndex = 0;

        typedef LeavePlayer ThisType;
        typedef xx::Object BaseType;
	    LeavePlayer(xx::MemPool* const& mempool) noexcept;
	    LeavePlayer(xx::BBuffer *bb);
		LeavePlayer(LeavePlayer const&) = delete;
		LeavePlayer& operator=(LeavePlayer const&) = delete;
        void ToString(xx::String &str) const noexcept override;
        void ToStringCore(xx::String &str) const noexcept override;
        void ToBBuffer(xx::BBuffer &bb) const noexcept override;
        int FromBBuffer(xx::BBuffer &bb) noexcept override;
        void CopyTo(LeavePlayer* const& o) const noexcept;
        LeavePlayer* MakeCopy() const noexcept;
        LeavePlayer_p MakePtrCopy() const noexcept;
        inline static xx::Ptr<ThisType> defaultInstance;
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
        // 公用随机数发生器
        ::xx::Random_p rnd;
        // 所有玩家( 子弹在玩家下面 )
        xx::List_p<PKG::CatchFish::Player_p> players;
        // 鱼的自增流水号
        int32_t fishSerialNumber = 0;
        // 所有鱼
        xx::List_p<PKG::CatchFish::Fish_p> fishs;
        // 当前帧所有事件的合并包容器, 服务器专用, 独立发送
        PKG::CatchFish_Client::FrameEvents_p frameEvents;

        typedef Scene ThisType;
        typedef xx::Object BaseType;
	    Scene(xx::MemPool* const& mempool) noexcept;
	    Scene(xx::BBuffer *bb);
		Scene(Scene const&) = delete;
		Scene& operator=(Scene const&) = delete;
        void ToString(xx::String &str) const noexcept override;
        void ToStringCore(xx::String &str) const noexcept override;
        void ToBBuffer(xx::BBuffer &bb) const noexcept override;
        int FromBBuffer(xx::BBuffer &bb) noexcept override;
        void CopyTo(Scene* const& o) const noexcept;
        Scene* MakeCopy() const noexcept;
        Scene_p MakePtrCopy() const noexcept;
        inline static xx::Ptr<ThisType> defaultInstance;
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
        // 消失点配置
        ::xx::Pos moveTo;
        // 当前移动步数( 步数一到就认为鱼已移至目的地可以清掉了 )
        int32_t moveStep = 0;
        // 总移动步数
        int32_t moveStepCount = 0;

        typedef Fish ThisType;
        typedef PKG::CatchFish::MoveObject BaseType;
	    Fish(xx::MemPool* const& mempool) noexcept;
	    Fish(xx::BBuffer *bb);
		Fish(Fish const&) = delete;
		Fish& operator=(Fish const&) = delete;
        void ToString(xx::String &str) const noexcept override;
        void ToStringCore(xx::String &str) const noexcept override;
        void ToBBuffer(xx::BBuffer &bb) const noexcept override;
        int FromBBuffer(xx::BBuffer &bb) noexcept override;
        void CopyTo(Fish* const& o) const noexcept;
        Fish* MakeCopy() const noexcept;
        Fish_p MakePtrCopy() const noexcept;
        inline static xx::Ptr<ThisType> defaultInstance;
    };
    // 子弹
    class Bullet : public PKG::CatchFish::MoveObject
    {
    public:
        // 金币价值( 也可理解为倍率 )
        int64_t coin = 0;

        typedef Bullet ThisType;
        typedef PKG::CatchFish::MoveObject BaseType;
	    Bullet(xx::MemPool* const& mempool) noexcept;
	    Bullet(xx::BBuffer *bb);
		Bullet(Bullet const&) = delete;
		Bullet& operator=(Bullet const&) = delete;
        void ToString(xx::String &str) const noexcept override;
        void ToStringCore(xx::String &str) const noexcept override;
        void ToBBuffer(xx::BBuffer &bb) const noexcept override;
        int FromBBuffer(xx::BBuffer &bb) noexcept override;
        void CopyTo(Bullet* const& o) const noexcept;
        Bullet* MakeCopy() const noexcept;
        Bullet_p MakePtrCopy() const noexcept;
        inline static xx::Ptr<ThisType> defaultInstance;
    };
    // 玩家
    class Player : public xx::Object
    {
    public:
        // 位于容器时的下标
        int32_t indexAtContainer = 0;
        // 玩家名字
        xx::String_p name;
        // 座位索引( 0: 左上  1: 右上  2: 左下 3: 右下 ), 同时也用于标识玩家
        int32_t sitIndex = 0;
        // 当前金币数
        int64_t coin = 0;
        // 子弹的自增流水号
        int32_t bulletSerialNumber = 0;
        // 所有子弹
        xx::List_p<PKG::CatchFish::Bullet_p> bullets;
        // 玩家网络上下文, 不参与网络传输
        ::ClientPeer peer = nullptr;

        typedef Player ThisType;
        typedef xx::Object BaseType;
	    Player(xx::MemPool* const& mempool) noexcept;
	    Player(xx::BBuffer *bb);
		Player(Player const&) = delete;
		Player& operator=(Player const&) = delete;
        void ToString(xx::String &str) const noexcept override;
        void ToStringCore(xx::String &str) const noexcept override;
        void ToBBuffer(xx::BBuffer &bb) const noexcept override;
        int FromBBuffer(xx::BBuffer &bb) noexcept override;
        void CopyTo(Player* const& o) const noexcept;
        Player* MakeCopy() const noexcept;
        Player_p MakePtrCopy() const noexcept;
        inline static xx::Ptr<ThisType> defaultInstance;
    };
    // 游戏配置信息( 配置信息并不会随着网络同步而下发, 反序列化后需要手工还原 )
    class Config : public xx::Object
    {
    public:
        // 所有鱼的配置信息
        xx::List_p<PKG::CatchFish::FishConfig_p> fishCfgs;
        // 子弹配置( 当前就一种子弹，先这样放置 )
        int32_t bulletRadius = 0;
        // 玩家初始金币值
        int64_t playerInitCoin = 0;

        typedef Config ThisType;
        typedef xx::Object BaseType;
	    Config(xx::MemPool* const& mempool) noexcept;
	    Config(xx::BBuffer *bb);
		Config(Config const&) = delete;
		Config& operator=(Config const&) = delete;
        void ToString(xx::String &str) const noexcept override;
        void ToStringCore(xx::String &str) const noexcept override;
        void ToBBuffer(xx::BBuffer &bb) const noexcept override;
        int FromBBuffer(xx::BBuffer &bb) noexcept override;
        void CopyTo(Config* const& o) const noexcept;
        Config* MakeCopy() const noexcept;
        Config_p MakePtrCopy() const noexcept;
        inline static xx::Ptr<ThisType> defaultInstance;
    };
    // 鱼配置信息
    class FishConfig : public xx::Object
    {
    public:
        // 鱼类编号
        int32_t typeId = 0;
        // 鱼名
        xx::String_p name;
        // 打死鱼的金币所得基数( 也可理解成倍率 )
        int64_t coin = 0;
        // 帧总数
        int32_t frameCount = 0;
        // 基于整个鱼的最大晃动范围的圆形检测区( 粗判 )
        PKG::CatchFish::CollisionArea_p collisionArea;
        // 鱼帧动画信息集合
        xx::List_p<PKG::CatchFish::FishSpriteFrame_p> frames;
        // 鱼动画( 仅客户端 )
        ::Animation anim = nullptr;

        typedef FishConfig ThisType;
        typedef xx::Object BaseType;
	    FishConfig(xx::MemPool* const& mempool) noexcept;
	    FishConfig(xx::BBuffer *bb);
		FishConfig(FishConfig const&) = delete;
		FishConfig& operator=(FishConfig const&) = delete;
        void ToString(xx::String &str) const noexcept override;
        void ToStringCore(xx::String &str) const noexcept override;
        void ToBBuffer(xx::BBuffer &bb) const noexcept override;
        int FromBBuffer(xx::BBuffer &bb) noexcept override;
        void CopyTo(FishConfig* const& o) const noexcept;
        FishConfig* MakeCopy() const noexcept;
        FishConfig_p MakePtrCopy() const noexcept;
        inline static xx::Ptr<ThisType> defaultInstance;
    };
    // 鱼帧动画信息
    class FishSpriteFrame : public xx::Object
    {
    public:
        // 打包前的散文件名( 帧定位名 )
        xx::String_p fileName;
        // 基于当前帧图的圆形碰撞区明细表
        xx::List_p<PKG::CatchFish::CollisionArea_p> collisionAreas;

        typedef FishSpriteFrame ThisType;
        typedef xx::Object BaseType;
	    FishSpriteFrame(xx::MemPool* const& mempool) noexcept;
	    FishSpriteFrame(xx::BBuffer *bb);
		FishSpriteFrame(FishSpriteFrame const&) = delete;
		FishSpriteFrame& operator=(FishSpriteFrame const&) = delete;
        void ToString(xx::String &str) const noexcept override;
        void ToStringCore(xx::String &str) const noexcept override;
        void ToBBuffer(xx::BBuffer &bb) const noexcept override;
        int FromBBuffer(xx::BBuffer &bb) noexcept override;
        void CopyTo(FishSpriteFrame* const& o) const noexcept;
        FishSpriteFrame* MakeCopy() const noexcept;
        FishSpriteFrame_p MakePtrCopy() const noexcept;
        inline static xx::Ptr<ThisType> defaultInstance;
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
	    CollisionArea(xx::MemPool* const& mempool) noexcept;
	    CollisionArea(xx::BBuffer *bb);
		CollisionArea(CollisionArea const&) = delete;
		CollisionArea& operator=(CollisionArea const&) = delete;
        void ToString(xx::String &str) const noexcept override;
        void ToStringCore(xx::String &str) const noexcept override;
        void ToBBuffer(xx::BBuffer &bb) const noexcept override;
        int FromBBuffer(xx::BBuffer &bb) noexcept override;
        void CopyTo(CollisionArea* const& o) const noexcept;
        CollisionArea* MakeCopy() const noexcept;
        CollisionArea_p MakePtrCopy() const noexcept;
        inline static xx::Ptr<ThisType> defaultInstance;
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
	    FireEnd(xx::MemPool* const& mempool) noexcept;
	    FireEnd(xx::BBuffer *bb);
		FireEnd(FireEnd const&) = delete;
		FireEnd& operator=(FireEnd const&) = delete;
        void ToString(xx::String &str) const noexcept override;
        void ToStringCore(xx::String &str) const noexcept override;
        void ToBBuffer(xx::BBuffer &bb) const noexcept override;
        int FromBBuffer(xx::BBuffer &bb) noexcept override;
        void CopyTo(FireEnd* const& o) const noexcept;
        FireEnd* MakeCopy() const noexcept;
        FireEnd_p MakePtrCopy() const noexcept;
        inline static xx::Ptr<ThisType> defaultInstance;
    };
    // 玩家持续开火时调整角度( 连发, 仅适合帧同步服务器算法 )
    class FireChangeAngle : public xx::Object
    {
    public:
        // 射击角度( 相对于炮台自己的正方向角度 )
        float angle = 0;

        typedef FireChangeAngle ThisType;
        typedef xx::Object BaseType;
	    FireChangeAngle(xx::MemPool* const& mempool) noexcept;
	    FireChangeAngle(xx::BBuffer *bb);
		FireChangeAngle(FireChangeAngle const&) = delete;
		FireChangeAngle& operator=(FireChangeAngle const&) = delete;
        void ToString(xx::String &str) const noexcept override;
        void ToStringCore(xx::String &str) const noexcept override;
        void ToBBuffer(xx::BBuffer &bb) const noexcept override;
        int FromBBuffer(xx::BBuffer &bb) noexcept override;
        void CopyTo(FireChangeAngle* const& o) const noexcept;
        FireChangeAngle* MakeCopy() const noexcept;
        FireChangeAngle_p MakePtrCopy() const noexcept;
        inline static xx::Ptr<ThisType> defaultInstance;
    };
    // 玩家开始开火( 连发, 仅适合帧同步服务器算法 )
    class FireBegin : public xx::Object
    {
    public:
        // 射击角度( 相对于炮台自己的正方向角度 )
        float angle = 0;

        typedef FireBegin ThisType;
        typedef xx::Object BaseType;
	    FireBegin(xx::MemPool* const& mempool) noexcept;
	    FireBegin(xx::BBuffer *bb);
		FireBegin(FireBegin const&) = delete;
		FireBegin& operator=(FireBegin const&) = delete;
        void ToString(xx::String &str) const noexcept override;
        void ToStringCore(xx::String &str) const noexcept override;
        void ToBBuffer(xx::BBuffer &bb) const noexcept override;
        int FromBBuffer(xx::BBuffer &bb) noexcept override;
        void CopyTo(FireBegin* const& o) const noexcept;
        FireBegin* MakeCopy() const noexcept;
        FireBegin_p MakePtrCopy() const noexcept;
        inline static xx::Ptr<ThisType> defaultInstance;
    };
    // 当前玩家自己的子弹打中鱼
    class Hit : public xx::Object
    {
    public:
        // 子弹流水号
        int32_t bulletSerialNumber = 0;
        // 鱼流水号
        int32_t fishSerialNumber = 0;

        typedef Hit ThisType;
        typedef xx::Object BaseType;
	    Hit(xx::MemPool* const& mempool) noexcept;
	    Hit(xx::BBuffer *bb);
		Hit(Hit const&) = delete;
		Hit& operator=(Hit const&) = delete;
        void ToString(xx::String &str) const noexcept override;
        void ToStringCore(xx::String &str) const noexcept override;
        void ToBBuffer(xx::BBuffer &bb) const noexcept override;
        int FromBBuffer(xx::BBuffer &bb) noexcept override;
        void CopyTo(Hit* const& o) const noexcept;
        Hit* MakeCopy() const noexcept;
        Hit_p MakePtrCopy() const noexcept;
        inline static xx::Ptr<ThisType> defaultInstance;
    };
    // 开火( 单次, 或频繁单次模拟伪连发 )
    class Fire : public xx::Object
    {
    public:
        // 当时的帧编号
        int32_t frameNumber = 0;
        // 子弹流水号
        int32_t bulletSerialNumber = 0;
        // 金币价值( 也可理解为倍率 )
        int64_t coin = 0;
        // 步进
        ::xx::Pos moveInc;

        typedef Fire ThisType;
        typedef xx::Object BaseType;
	    Fire(xx::MemPool* const& mempool) noexcept;
	    Fire(xx::BBuffer *bb);
		Fire(Fire const&) = delete;
		Fire& operator=(Fire const&) = delete;
        void ToString(xx::String &str) const noexcept override;
        void ToStringCore(xx::String &str) const noexcept override;
        void ToBBuffer(xx::BBuffer &bb) const noexcept override;
        int FromBBuffer(xx::BBuffer &bb) noexcept override;
        void CopyTo(Fire* const& o) const noexcept;
        Fire* MakeCopy() const noexcept;
        Fire_p MakePtrCopy() const noexcept;
        inline static xx::Ptr<ThisType> defaultInstance;
    };
    // 正常离开游戏( 立即, 并非断线等待超时后清掉 )
    class Leave : public xx::Object
    {
    public:

        typedef Leave ThisType;
        typedef xx::Object BaseType;
	    Leave(xx::MemPool* const& mempool) noexcept;
	    Leave(xx::BBuffer *bb);
		Leave(Leave const&) = delete;
		Leave& operator=(Leave const&) = delete;
        void ToString(xx::String &str) const noexcept override;
        void ToStringCore(xx::String &str) const noexcept override;
        void ToBBuffer(xx::BBuffer &bb) const noexcept override;
        int FromBBuffer(xx::BBuffer &bb) noexcept override;
        void CopyTo(Leave* const& o) const noexcept;
        Leave* MakeCopy() const noexcept;
        Leave_p MakePtrCopy() const noexcept;
        inline static xx::Ptr<ThisType> defaultInstance;
    };
    // 客户端与服务器建立连接后的首包
    class Join : public xx::Object
    {
    public:
        // 客户端进游戏前自己填的名字. 可以写磁盘存在本地.
        xx::String_p username;

        typedef Join ThisType;
        typedef xx::Object BaseType;
	    Join(xx::MemPool* const& mempool) noexcept;
	    Join(xx::BBuffer *bb);
		Join(Join const&) = delete;
		Join& operator=(Join const&) = delete;
        void ToString(xx::String &str) const noexcept override;
        void ToStringCore(xx::String &str) const noexcept override;
        void ToBBuffer(xx::BBuffer &bb) const noexcept override;
        int FromBBuffer(xx::BBuffer &bb) noexcept override;
        void CopyTo(Join* const& o) const noexcept;
        Join* MakeCopy() const noexcept;
        Join_p MakePtrCopy() const noexcept;
        inline static xx::Ptr<ThisType> defaultInstance;
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
	    FrameEvents(xx::MemPool* const& mempool) noexcept;
	    FrameEvents(xx::BBuffer *bb);
		FrameEvents(FrameEvents const&) = delete;
		FrameEvents& operator=(FrameEvents const&) = delete;
        void ToString(xx::String &str) const noexcept override;
        void ToStringCore(xx::String &str) const noexcept override;
        void ToBBuffer(xx::BBuffer &bb) const noexcept override;
        int FromBBuffer(xx::BBuffer &bb) noexcept override;
        void CopyTo(FrameEvents* const& o) const noexcept;
        FrameEvents* MakeCopy() const noexcept;
        FrameEvents_p MakePtrCopy() const noexcept;
        inline static xx::Ptr<ThisType> defaultInstance;
    };
    // 要按成员顺序优先级依次处理
    class FullSync : public xx::Object
    {
    public:
        PKG::CatchFish::Scene_p scene;

        typedef FullSync ThisType;
        typedef xx::Object BaseType;
	    FullSync(xx::MemPool* const& mempool) noexcept;
	    FullSync(xx::BBuffer *bb);
		FullSync(FullSync const&) = delete;
		FullSync& operator=(FullSync const&) = delete;
        void ToString(xx::String &str) const noexcept override;
        void ToStringCore(xx::String &str) const noexcept override;
        void ToBBuffer(xx::BBuffer &bb) const noexcept override;
        int FromBBuffer(xx::BBuffer &bb) noexcept override;
        void CopyTo(FullSync* const& o) const noexcept;
        FullSync* MakeCopy() const noexcept;
        FullSync_p MakePtrCopy() const noexcept;
        inline static xx::Ptr<ThisType> defaultInstance;
    };
    // 进入失败
    class JoinFail : public xx::Object
    {
    public:
        // 错误提示信息
        xx::String_p errMsg;

        typedef JoinFail ThisType;
        typedef xx::Object BaseType;
	    JoinFail(xx::MemPool* const& mempool) noexcept;
	    JoinFail(xx::BBuffer *bb);
		JoinFail(JoinFail const&) = delete;
		JoinFail& operator=(JoinFail const&) = delete;
        void ToString(xx::String &str) const noexcept override;
        void ToStringCore(xx::String &str) const noexcept override;
        void ToBBuffer(xx::BBuffer &bb) const noexcept override;
        int FromBBuffer(xx::BBuffer &bb) noexcept override;
        void CopyTo(JoinFail* const& o) const noexcept;
        JoinFail* MakeCopy() const noexcept;
        JoinFail_p MakePtrCopy() const noexcept;
        inline static xx::Ptr<ThisType> defaultInstance;
    };
}
namespace CatchFish::Events
{
    // 子弹命中( 与鱼死分离. 鱼死相关可能要等服务器跨线程回调送回结果才能下发 )
    class BulletHit : public xx::Object
    {
    public:
        // 座位索引( 0: 左上  1: 右上  2: 左下 3: 右下 )
        int32_t sitIndex = 0;
        // 子弹流水号
        int32_t bulletSerialNumber = 0;

        typedef BulletHit ThisType;
        typedef xx::Object BaseType;
	    BulletHit(xx::MemPool* const& mempool) noexcept;
	    BulletHit(xx::BBuffer *bb);
		BulletHit(BulletHit const&) = delete;
		BulletHit& operator=(BulletHit const&) = delete;
        void ToString(xx::String &str) const noexcept override;
        void ToStringCore(xx::String &str) const noexcept override;
        void ToBBuffer(xx::BBuffer &bb) const noexcept override;
        int FromBBuffer(xx::BBuffer &bb) noexcept override;
        void CopyTo(BulletHit* const& o) const noexcept;
        BulletHit* MakeCopy() const noexcept;
        BulletHit_p MakePtrCopy() const noexcept;
        inline static xx::Ptr<ThisType> defaultInstance;
    };
    // 鱼被打死
    class FishDead : public xx::Object
    {
    public:
        // 座位索引( 0: 左上  1: 右上  2: 左下 3: 右下 )
        int32_t sitIndex = 0;
        // 鱼流水号
        int32_t fishSerialNumber = 0;
        // 金币所得
        int64_t coin = 0;

        typedef FishDead ThisType;
        typedef xx::Object BaseType;
	    FishDead(xx::MemPool* const& mempool) noexcept;
	    FishDead(xx::BBuffer *bb);
		FishDead(FishDead const&) = delete;
		FishDead& operator=(FishDead const&) = delete;
        void ToString(xx::String &str) const noexcept override;
        void ToStringCore(xx::String &str) const noexcept override;
        void ToBBuffer(xx::BBuffer &bb) const noexcept override;
        int FromBBuffer(xx::BBuffer &bb) noexcept override;
        void CopyTo(FishDead* const& o) const noexcept;
        FishDead* MakeCopy() const noexcept;
        FishDead_p MakePtrCopy() const noexcept;
        inline static xx::Ptr<ThisType> defaultInstance;
    };
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
	template<> struct TypeId<PKG::CatchFish::Events::LeavePlayer> { static const uint16_t value = 39; };
	template<> struct TypeId<xx::List<PKG::CatchFish::Events::JoinPlayer_p>> { static const uint16_t value = 9; };
	template<> struct TypeId<PKG::CatchFish::Events::JoinPlayer> { static const uint16_t value = 40; };
	template<> struct TypeId<xx::List<PKG::CatchFish::Events::FishDead_p>> { static const uint16_t value = 11; };
	template<> struct TypeId<PKG::CatchFish::Events::FishDead> { static const uint16_t value = 46; };
	template<> struct TypeId<xx::List<PKG::CatchFish::Events::Fire_p>> { static const uint16_t value = 12; };
	template<> struct TypeId<PKG::CatchFish::Events::Fire> { static const uint16_t value = 41; };
	template<> struct TypeId<xx::List<PKG::CatchFish::Events::FireEnd_p>> { static const uint16_t value = 13; };
	template<> struct TypeId<PKG::CatchFish::Events::FireEnd> { static const uint16_t value = 44; };
	template<> struct TypeId<xx::List<PKG::CatchFish::Events::FireBegin_p>> { static const uint16_t value = 14; };
	template<> struct TypeId<PKG::CatchFish::Events::FireBegin> { static const uint16_t value = 42; };
	template<> struct TypeId<xx::List<PKG::CatchFish::Events::FireChangeAngle_p>> { static const uint16_t value = 15; };
	template<> struct TypeId<PKG::CatchFish::Events::FireChangeAngle> { static const uint16_t value = 43; };
	template<> struct TypeId<PKG::Client_CatchFish::Join> { static const uint16_t value = 16; };
	template<> struct TypeId<PKG::Client_CatchFish::Leave> { static const uint16_t value = 17; };
	template<> struct TypeId<PKG::Client_CatchFish::Fire> { static const uint16_t value = 18; };
	template<> struct TypeId<PKG::Client_CatchFish::Hit> { static const uint16_t value = 50; };
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
	template<> struct TypeId<xx::List<PKG::CatchFish::Bullet_p>> { static const uint16_t value = 31; };
	template<> struct TypeId<PKG::CatchFish::Bullet> { static const uint16_t value = 34; };
	template<> struct TypeId<PKG::CatchFish::MoveObject> { static const uint16_t value = 33; };
	template<> struct TypeId<PKG::CatchFish::Fish> { static const uint16_t value = 35; };
	template<> struct TypeId<::xx::Random> { static const uint16_t value = 36; };
	template<> struct TypeId<xx::List<PKG::CatchFish::Player_p>> { static const uint16_t value = 37; };
	template<> struct TypeId<xx::List<PKG::CatchFish::Fish_p>> { static const uint16_t value = 38; };
	template<> struct TypeId<PKG::CatchFish::Events::BulletHit> { static const uint16_t value = 45; };
}
namespace PKG
{
namespace CatchFish_Client
{
	inline JoinSuccess::JoinSuccess(xx::MemPool* const& mempool) noexcept
        : xx::Object(mempool)
	{
	}
	inline JoinSuccess::JoinSuccess(xx::BBuffer *bb)
        : xx::Object(bb->mempool)
	{
        if (int rtv = FromBBuffer(*bb)) throw rtv;
	}
    inline void JoinSuccess::ToBBuffer(xx::BBuffer &bb) const noexcept
    {
        bb.Write(this->sitIndex);
    }
    inline int JoinSuccess::FromBBuffer(xx::BBuffer &bb) noexcept
    {
        int rtv = 0;
        if ((rtv = bb.Read(this->sitIndex))) return rtv;
        return rtv;
    }

    inline void JoinSuccess::ToString(xx::String &str) const noexcept
    {
        if (memHeader().flags)
        {
        	str.Append("[ \"***** recursived *****\" ]");
        	return;
        }
        else memHeader().flags = 1;

        str.Append("{ \"pkgTypeName\":\"CatchFish_Client.JoinSuccess\", \"pkgTypeId\":", xx::TypeId_v<ThisType>);
        ToStringCore(str);
        str.Append(" }");
        
        memHeader().flags = 0;
    }
    inline void JoinSuccess::ToStringCore(xx::String &str) const noexcept
    {
        this->BaseType::ToStringCore(str);
        str.Append(", \"sitIndex\":", this->sitIndex);
    }
    inline void JoinSuccess::CopyTo(JoinSuccess* const& o) const noexcept
    {
        o->sitIndex = this->sitIndex;
    }
    inline JoinSuccess* JoinSuccess::MakeCopy() const noexcept
    {
        auto rtv = mempool->MPCreate<JoinSuccess>();
        this->CopyTo(rtv);
        return rtv;
    }
    inline JoinSuccess_p JoinSuccess::MakePtrCopy() const noexcept
    {
        return JoinSuccess_p(this->MakeCopy());
    }

	inline JoinFail::JoinFail(xx::MemPool* const& mempool) noexcept
        : xx::Object(mempool)
	{
	}
	inline JoinFail::JoinFail(xx::BBuffer *bb)
        : xx::Object(bb->mempool)
	{
        if (int rtv = FromBBuffer(*bb)) throw rtv;
	}
    inline void JoinFail::ToBBuffer(xx::BBuffer &bb) const noexcept
    {
        bb.Write(this->errMsg);
    }
    inline int JoinFail::FromBBuffer(xx::BBuffer &bb) noexcept
    {
        int rtv = 0;
        bb.readLengthLimit = 0;
        if ((rtv = bb.Read(this->errMsg))) return rtv;
        return rtv;
    }

    inline void JoinFail::ToString(xx::String &str) const noexcept
    {
        if (memHeader().flags)
        {
        	str.Append("[ \"***** recursived *****\" ]");
        	return;
        }
        else memHeader().flags = 1;

        str.Append("{ \"pkgTypeName\":\"CatchFish_Client.JoinFail\", \"pkgTypeId\":", xx::TypeId_v<ThisType>);
        ToStringCore(str);
        str.Append(" }");
        
        memHeader().flags = 0;
    }
    inline void JoinFail::ToStringCore(xx::String &str) const noexcept
    {
        this->BaseType::ToStringCore(str);
        if (this->errMsg) str.Append(", \"errMsg\":\"", this->errMsg, "\"");
        else str.Append(", \"errMsg\":nil");
    }
    inline void JoinFail::CopyTo(JoinFail* const& o) const noexcept
    {
        o->errMsg = this->errMsg;
    }
    inline JoinFail* JoinFail::MakeCopy() const noexcept
    {
        auto rtv = mempool->MPCreate<JoinFail>();
        this->CopyTo(rtv);
        return rtv;
    }
    inline JoinFail_p JoinFail::MakePtrCopy() const noexcept
    {
        return JoinFail_p(this->MakeCopy());
    }

	inline FullSync::FullSync(xx::MemPool* const& mempool) noexcept
        : xx::Object(mempool)
	{
	}
	inline FullSync::FullSync(xx::BBuffer *bb)
        : xx::Object(bb->mempool)
	{
        if (int rtv = FromBBuffer(*bb)) throw rtv;
	}
    inline void FullSync::ToBBuffer(xx::BBuffer &bb) const noexcept
    {
        bb.Write(this->scene);
    }
    inline int FullSync::FromBBuffer(xx::BBuffer &bb) noexcept
    {
        int rtv = 0;
        if ((rtv = bb.Read(this->scene))) return rtv;
        return rtv;
    }

    inline void FullSync::ToString(xx::String &str) const noexcept
    {
        if (memHeader().flags)
        {
        	str.Append("[ \"***** recursived *****\" ]");
        	return;
        }
        else memHeader().flags = 1;

        str.Append("{ \"pkgTypeName\":\"CatchFish_Client.FullSync\", \"pkgTypeId\":", xx::TypeId_v<ThisType>);
        ToStringCore(str);
        str.Append(" }");
        
        memHeader().flags = 0;
    }
    inline void FullSync::ToStringCore(xx::String &str) const noexcept
    {
        this->BaseType::ToStringCore(str);
        str.Append(", \"scene\":", this->scene);
    }
    inline void FullSync::CopyTo(FullSync* const& o) const noexcept
    {
        o->scene = this->scene;
    }
    inline FullSync* FullSync::MakeCopy() const noexcept
    {
        auto rtv = mempool->MPCreate<FullSync>();
        this->CopyTo(rtv);
        return rtv;
    }
    inline FullSync_p FullSync::MakePtrCopy() const noexcept
    {
        return FullSync_p(this->MakeCopy());
    }

	inline FrameEvents::FrameEvents(xx::MemPool* const& mempool) noexcept
        : xx::Object(mempool)
	{
	}
	inline FrameEvents::FrameEvents(xx::BBuffer *bb)
        : xx::Object(bb->mempool)
	{
        if (int rtv = FromBBuffer(*bb)) throw rtv;
	}
    inline void FrameEvents::ToBBuffer(xx::BBuffer &bb) const noexcept
    {
        bb.Write(this->frameNumber);
        bb.Write(this->leaves);
        bb.Write(this->joins);
        bb.Write(this->fishDeads);
        bb.Write(this->fires);
        bb.Write(this->fireEnds);
        bb.Write(this->fireBegins);
        bb.Write(this->fireChangeAngles);
    }
    inline int FrameEvents::FromBBuffer(xx::BBuffer &bb) noexcept
    {
        int rtv = 0;
        if ((rtv = bb.Read(this->frameNumber))) return rtv;
        bb.readLengthLimit = 0;
        if ((rtv = bb.Read(this->leaves))) return rtv;
        bb.readLengthLimit = 0;
        if ((rtv = bb.Read(this->joins))) return rtv;
        bb.readLengthLimit = 0;
        if ((rtv = bb.Read(this->fishDeads))) return rtv;
        bb.readLengthLimit = 0;
        if ((rtv = bb.Read(this->fires))) return rtv;
        bb.readLengthLimit = 0;
        if ((rtv = bb.Read(this->fireEnds))) return rtv;
        bb.readLengthLimit = 0;
        if ((rtv = bb.Read(this->fireBegins))) return rtv;
        bb.readLengthLimit = 0;
        if ((rtv = bb.Read(this->fireChangeAngles))) return rtv;
        return rtv;
    }

    inline void FrameEvents::ToString(xx::String &str) const noexcept
    {
        if (memHeader().flags)
        {
        	str.Append("[ \"***** recursived *****\" ]");
        	return;
        }
        else memHeader().flags = 1;

        str.Append("{ \"pkgTypeName\":\"CatchFish_Client.FrameEvents\", \"pkgTypeId\":", xx::TypeId_v<ThisType>);
        ToStringCore(str);
        str.Append(" }");
        
        memHeader().flags = 0;
    }
    inline void FrameEvents::ToStringCore(xx::String &str) const noexcept
    {
        this->BaseType::ToStringCore(str);
        str.Append(", \"frameNumber\":", this->frameNumber);
        str.Append(", \"leaves\":", this->leaves);
        str.Append(", \"joins\":", this->joins);
        str.Append(", \"fishDeads\":", this->fishDeads);
        str.Append(", \"fires\":", this->fires);
        str.Append(", \"fireEnds\":", this->fireEnds);
        str.Append(", \"fireBegins\":", this->fireBegins);
        str.Append(", \"fireChangeAngles\":", this->fireChangeAngles);
    }
    inline void FrameEvents::CopyTo(FrameEvents* const& o) const noexcept
    {
        o->frameNumber = this->frameNumber;
        o->leaves = this->leaves;
        o->joins = this->joins;
        o->fishDeads = this->fishDeads;
        o->fires = this->fires;
        o->fireEnds = this->fireEnds;
        o->fireBegins = this->fireBegins;
        o->fireChangeAngles = this->fireChangeAngles;
    }
    inline FrameEvents* FrameEvents::MakeCopy() const noexcept
    {
        auto rtv = mempool->MPCreate<FrameEvents>();
        this->CopyTo(rtv);
        return rtv;
    }
    inline FrameEvents_p FrameEvents::MakePtrCopy() const noexcept
    {
        return FrameEvents_p(this->MakeCopy());
    }

}
namespace Client_CatchFish
{
	inline Join::Join(xx::MemPool* const& mempool) noexcept
        : xx::Object(mempool)
	{
	}
	inline Join::Join(xx::BBuffer *bb)
        : xx::Object(bb->mempool)
	{
        if (int rtv = FromBBuffer(*bb)) throw rtv;
	}
    inline void Join::ToBBuffer(xx::BBuffer &bb) const noexcept
    {
        bb.Write(this->username);
    }
    inline int Join::FromBBuffer(xx::BBuffer &bb) noexcept
    {
        int rtv = 0;
        bb.readLengthLimit = 64;
        if ((rtv = bb.Read(this->username))) return rtv;
        return rtv;
    }

    inline void Join::ToString(xx::String &str) const noexcept
    {
        if (memHeader().flags)
        {
        	str.Append("[ \"***** recursived *****\" ]");
        	return;
        }
        else memHeader().flags = 1;

        str.Append("{ \"pkgTypeName\":\"Client_CatchFish.Join\", \"pkgTypeId\":", xx::TypeId_v<ThisType>);
        ToStringCore(str);
        str.Append(" }");
        
        memHeader().flags = 0;
    }
    inline void Join::ToStringCore(xx::String &str) const noexcept
    {
        this->BaseType::ToStringCore(str);
        if (this->username) str.Append(", \"username\":\"", this->username, "\"");
        else str.Append(", \"username\":nil");
    }
    inline void Join::CopyTo(Join* const& o) const noexcept
    {
        o->username = this->username;
    }
    inline Join* Join::MakeCopy() const noexcept
    {
        auto rtv = mempool->MPCreate<Join>();
        this->CopyTo(rtv);
        return rtv;
    }
    inline Join_p Join::MakePtrCopy() const noexcept
    {
        return Join_p(this->MakeCopy());
    }

	inline Leave::Leave(xx::MemPool* const& mempool) noexcept
        : xx::Object(mempool)
	{
	}
	inline Leave::Leave(xx::BBuffer *bb)
        : xx::Object(bb->mempool)
	{
        if (int rtv = FromBBuffer(*bb)) throw rtv;
	}
    inline void Leave::ToBBuffer(xx::BBuffer &bb) const noexcept
    {
    }
    inline int Leave::FromBBuffer(xx::BBuffer &bb) noexcept
    {
        int rtv = 0;
        return rtv;
    }

    inline void Leave::ToString(xx::String &str) const noexcept
    {
        if (memHeader().flags)
        {
        	str.Append("[ \"***** recursived *****\" ]");
        	return;
        }
        else memHeader().flags = 1;

        str.Append("{ \"pkgTypeName\":\"Client_CatchFish.Leave\", \"pkgTypeId\":", xx::TypeId_v<ThisType>);
        ToStringCore(str);
        str.Append(" }");
        
        memHeader().flags = 0;
    }
    inline void Leave::ToStringCore(xx::String &str) const noexcept
    {
        this->BaseType::ToStringCore(str);
    }
    inline void Leave::CopyTo(Leave* const& o) const noexcept
    {
    }
    inline Leave* Leave::MakeCopy() const noexcept
    {
        auto rtv = mempool->MPCreate<Leave>();
        this->CopyTo(rtv);
        return rtv;
    }
    inline Leave_p Leave::MakePtrCopy() const noexcept
    {
        return Leave_p(this->MakeCopy());
    }

	inline Fire::Fire(xx::MemPool* const& mempool) noexcept
        : xx::Object(mempool)
	{
	}
	inline Fire::Fire(xx::BBuffer *bb)
        : xx::Object(bb->mempool)
	{
        if (int rtv = FromBBuffer(*bb)) throw rtv;
	}
    inline void Fire::ToBBuffer(xx::BBuffer &bb) const noexcept
    {
        bb.Write(this->frameNumber);
        bb.Write(this->bulletSerialNumber);
        bb.Write(this->coin);
        bb.Write(this->moveInc);
    }
    inline int Fire::FromBBuffer(xx::BBuffer &bb) noexcept
    {
        int rtv = 0;
        if ((rtv = bb.Read(this->frameNumber))) return rtv;
        if ((rtv = bb.Read(this->bulletSerialNumber))) return rtv;
        if ((rtv = bb.Read(this->coin))) return rtv;
        if ((rtv = bb.Read(this->moveInc))) return rtv;
        return rtv;
    }

    inline void Fire::ToString(xx::String &str) const noexcept
    {
        if (memHeader().flags)
        {
        	str.Append("[ \"***** recursived *****\" ]");
        	return;
        }
        else memHeader().flags = 1;

        str.Append("{ \"pkgTypeName\":\"Client_CatchFish.Fire\", \"pkgTypeId\":", xx::TypeId_v<ThisType>);
        ToStringCore(str);
        str.Append(" }");
        
        memHeader().flags = 0;
    }
    inline void Fire::ToStringCore(xx::String &str) const noexcept
    {
        this->BaseType::ToStringCore(str);
        str.Append(", \"frameNumber\":", this->frameNumber);
        str.Append(", \"bulletSerialNumber\":", this->bulletSerialNumber);
        str.Append(", \"coin\":", this->coin);
        str.Append(", \"moveInc\":", this->moveInc);
    }
    inline void Fire::CopyTo(Fire* const& o) const noexcept
    {
        o->frameNumber = this->frameNumber;
        o->bulletSerialNumber = this->bulletSerialNumber;
        o->coin = this->coin;
        o->moveInc = this->moveInc;
    }
    inline Fire* Fire::MakeCopy() const noexcept
    {
        auto rtv = mempool->MPCreate<Fire>();
        this->CopyTo(rtv);
        return rtv;
    }
    inline Fire_p Fire::MakePtrCopy() const noexcept
    {
        return Fire_p(this->MakeCopy());
    }

	inline Hit::Hit(xx::MemPool* const& mempool) noexcept
        : xx::Object(mempool)
	{
	}
	inline Hit::Hit(xx::BBuffer *bb)
        : xx::Object(bb->mempool)
	{
        if (int rtv = FromBBuffer(*bb)) throw rtv;
	}
    inline void Hit::ToBBuffer(xx::BBuffer &bb) const noexcept
    {
        bb.Write(this->bulletSerialNumber);
        bb.Write(this->fishSerialNumber);
    }
    inline int Hit::FromBBuffer(xx::BBuffer &bb) noexcept
    {
        int rtv = 0;
        if ((rtv = bb.Read(this->bulletSerialNumber))) return rtv;
        if ((rtv = bb.Read(this->fishSerialNumber))) return rtv;
        return rtv;
    }

    inline void Hit::ToString(xx::String &str) const noexcept
    {
        if (memHeader().flags)
        {
        	str.Append("[ \"***** recursived *****\" ]");
        	return;
        }
        else memHeader().flags = 1;

        str.Append("{ \"pkgTypeName\":\"Client_CatchFish.Hit\", \"pkgTypeId\":", xx::TypeId_v<ThisType>);
        ToStringCore(str);
        str.Append(" }");
        
        memHeader().flags = 0;
    }
    inline void Hit::ToStringCore(xx::String &str) const noexcept
    {
        this->BaseType::ToStringCore(str);
        str.Append(", \"bulletSerialNumber\":", this->bulletSerialNumber);
        str.Append(", \"fishSerialNumber\":", this->fishSerialNumber);
    }
    inline void Hit::CopyTo(Hit* const& o) const noexcept
    {
        o->bulletSerialNumber = this->bulletSerialNumber;
        o->fishSerialNumber = this->fishSerialNumber;
    }
    inline Hit* Hit::MakeCopy() const noexcept
    {
        auto rtv = mempool->MPCreate<Hit>();
        this->CopyTo(rtv);
        return rtv;
    }
    inline Hit_p Hit::MakePtrCopy() const noexcept
    {
        return Hit_p(this->MakeCopy());
    }

	inline FireBegin::FireBegin(xx::MemPool* const& mempool) noexcept
        : xx::Object(mempool)
	{
	}
	inline FireBegin::FireBegin(xx::BBuffer *bb)
        : xx::Object(bb->mempool)
	{
        if (int rtv = FromBBuffer(*bb)) throw rtv;
	}
    inline void FireBegin::ToBBuffer(xx::BBuffer &bb) const noexcept
    {
        bb.Write(this->angle);
    }
    inline int FireBegin::FromBBuffer(xx::BBuffer &bb) noexcept
    {
        int rtv = 0;
        if ((rtv = bb.Read(this->angle))) return rtv;
        return rtv;
    }

    inline void FireBegin::ToString(xx::String &str) const noexcept
    {
        if (memHeader().flags)
        {
        	str.Append("[ \"***** recursived *****\" ]");
        	return;
        }
        else memHeader().flags = 1;

        str.Append("{ \"pkgTypeName\":\"Client_CatchFish.FireBegin\", \"pkgTypeId\":", xx::TypeId_v<ThisType>);
        ToStringCore(str);
        str.Append(" }");
        
        memHeader().flags = 0;
    }
    inline void FireBegin::ToStringCore(xx::String &str) const noexcept
    {
        this->BaseType::ToStringCore(str);
        str.Append(", \"angle\":", this->angle);
    }
    inline void FireBegin::CopyTo(FireBegin* const& o) const noexcept
    {
        o->angle = this->angle;
    }
    inline FireBegin* FireBegin::MakeCopy() const noexcept
    {
        auto rtv = mempool->MPCreate<FireBegin>();
        this->CopyTo(rtv);
        return rtv;
    }
    inline FireBegin_p FireBegin::MakePtrCopy() const noexcept
    {
        return FireBegin_p(this->MakeCopy());
    }

	inline FireChangeAngle::FireChangeAngle(xx::MemPool* const& mempool) noexcept
        : xx::Object(mempool)
	{
	}
	inline FireChangeAngle::FireChangeAngle(xx::BBuffer *bb)
        : xx::Object(bb->mempool)
	{
        if (int rtv = FromBBuffer(*bb)) throw rtv;
	}
    inline void FireChangeAngle::ToBBuffer(xx::BBuffer &bb) const noexcept
    {
        bb.Write(this->angle);
    }
    inline int FireChangeAngle::FromBBuffer(xx::BBuffer &bb) noexcept
    {
        int rtv = 0;
        if ((rtv = bb.Read(this->angle))) return rtv;
        return rtv;
    }

    inline void FireChangeAngle::ToString(xx::String &str) const noexcept
    {
        if (memHeader().flags)
        {
        	str.Append("[ \"***** recursived *****\" ]");
        	return;
        }
        else memHeader().flags = 1;

        str.Append("{ \"pkgTypeName\":\"Client_CatchFish.FireChangeAngle\", \"pkgTypeId\":", xx::TypeId_v<ThisType>);
        ToStringCore(str);
        str.Append(" }");
        
        memHeader().flags = 0;
    }
    inline void FireChangeAngle::ToStringCore(xx::String &str) const noexcept
    {
        this->BaseType::ToStringCore(str);
        str.Append(", \"angle\":", this->angle);
    }
    inline void FireChangeAngle::CopyTo(FireChangeAngle* const& o) const noexcept
    {
        o->angle = this->angle;
    }
    inline FireChangeAngle* FireChangeAngle::MakeCopy() const noexcept
    {
        auto rtv = mempool->MPCreate<FireChangeAngle>();
        this->CopyTo(rtv);
        return rtv;
    }
    inline FireChangeAngle_p FireChangeAngle::MakePtrCopy() const noexcept
    {
        return FireChangeAngle_p(this->MakeCopy());
    }

	inline FireEnd::FireEnd(xx::MemPool* const& mempool) noexcept
        : xx::Object(mempool)
	{
	}
	inline FireEnd::FireEnd(xx::BBuffer *bb)
        : xx::Object(bb->mempool)
	{
        if (int rtv = FromBBuffer(*bb)) throw rtv;
	}
    inline void FireEnd::ToBBuffer(xx::BBuffer &bb) const noexcept
    {
    }
    inline int FireEnd::FromBBuffer(xx::BBuffer &bb) noexcept
    {
        int rtv = 0;
        return rtv;
    }

    inline void FireEnd::ToString(xx::String &str) const noexcept
    {
        if (memHeader().flags)
        {
        	str.Append("[ \"***** recursived *****\" ]");
        	return;
        }
        else memHeader().flags = 1;

        str.Append("{ \"pkgTypeName\":\"Client_CatchFish.FireEnd\", \"pkgTypeId\":", xx::TypeId_v<ThisType>);
        ToStringCore(str);
        str.Append(" }");
        
        memHeader().flags = 0;
    }
    inline void FireEnd::ToStringCore(xx::String &str) const noexcept
    {
        this->BaseType::ToStringCore(str);
    }
    inline void FireEnd::CopyTo(FireEnd* const& o) const noexcept
    {
    }
    inline FireEnd* FireEnd::MakeCopy() const noexcept
    {
        auto rtv = mempool->MPCreate<FireEnd>();
        this->CopyTo(rtv);
        return rtv;
    }
    inline FireEnd_p FireEnd::MakePtrCopy() const noexcept
    {
        return FireEnd_p(this->MakeCopy());
    }

}
namespace CatchFish
{
	inline CollisionArea::CollisionArea(xx::MemPool* const& mempool) noexcept
        : xx::Object(mempool)
	{
	}
	inline CollisionArea::CollisionArea(xx::BBuffer *bb)
        : xx::Object(bb->mempool)
	{
        if (int rtv = FromBBuffer(*bb)) throw rtv;
	}
    inline void CollisionArea::ToBBuffer(xx::BBuffer &bb) const noexcept
    {
        bb.Write(this->offset);
        bb.Write(this->offsets);
        bb.Write(this->radius);
    }
    inline int CollisionArea::FromBBuffer(xx::BBuffer &bb) noexcept
    {
        int rtv = 0;
        if ((rtv = bb.Read(this->offset))) return rtv;
        bb.readLengthLimit = 0;
        if ((rtv = bb.Read(this->offsets))) return rtv;
        if ((rtv = bb.Read(this->radius))) return rtv;
        return rtv;
    }

    inline void CollisionArea::ToString(xx::String &str) const noexcept
    {
        if (memHeader().flags)
        {
        	str.Append("[ \"***** recursived *****\" ]");
        	return;
        }
        else memHeader().flags = 1;

        str.Append("{ \"pkgTypeName\":\"CatchFish.CollisionArea\", \"pkgTypeId\":", xx::TypeId_v<ThisType>);
        ToStringCore(str);
        str.Append(" }");
        
        memHeader().flags = 0;
    }
    inline void CollisionArea::ToStringCore(xx::String &str) const noexcept
    {
        this->BaseType::ToStringCore(str);
        str.Append(", \"offset\":", this->offset);
        str.Append(", \"offsets\":", this->offsets);
        str.Append(", \"radius\":", this->radius);
    }
    inline void CollisionArea::CopyTo(CollisionArea* const& o) const noexcept
    {
        o->offset = this->offset;
        o->offsets = this->offsets;
        o->radius = this->radius;
    }
    inline CollisionArea* CollisionArea::MakeCopy() const noexcept
    {
        auto rtv = mempool->MPCreate<CollisionArea>();
        this->CopyTo(rtv);
        return rtv;
    }
    inline CollisionArea_p CollisionArea::MakePtrCopy() const noexcept
    {
        return CollisionArea_p(this->MakeCopy());
    }

	inline FishSpriteFrame::FishSpriteFrame(xx::MemPool* const& mempool) noexcept
        : xx::Object(mempool)
	{
	}
	inline FishSpriteFrame::FishSpriteFrame(xx::BBuffer *bb)
        : xx::Object(bb->mempool)
	{
        if (int rtv = FromBBuffer(*bb)) throw rtv;
	}
    inline void FishSpriteFrame::ToBBuffer(xx::BBuffer &bb) const noexcept
    {
        bb.Write(this->fileName);
        bb.Write(this->collisionAreas);
    }
    inline int FishSpriteFrame::FromBBuffer(xx::BBuffer &bb) noexcept
    {
        int rtv = 0;
        bb.readLengthLimit = 0;
        if ((rtv = bb.Read(this->fileName))) return rtv;
        bb.readLengthLimit = 0;
        if ((rtv = bb.Read(this->collisionAreas))) return rtv;
        return rtv;
    }

    inline void FishSpriteFrame::ToString(xx::String &str) const noexcept
    {
        if (memHeader().flags)
        {
        	str.Append("[ \"***** recursived *****\" ]");
        	return;
        }
        else memHeader().flags = 1;

        str.Append("{ \"pkgTypeName\":\"CatchFish.FishSpriteFrame\", \"pkgTypeId\":", xx::TypeId_v<ThisType>);
        ToStringCore(str);
        str.Append(" }");
        
        memHeader().flags = 0;
    }
    inline void FishSpriteFrame::ToStringCore(xx::String &str) const noexcept
    {
        this->BaseType::ToStringCore(str);
        if (this->fileName) str.Append(", \"fileName\":\"", this->fileName, "\"");
        else str.Append(", \"fileName\":nil");
        str.Append(", \"collisionAreas\":", this->collisionAreas);
    }
    inline void FishSpriteFrame::CopyTo(FishSpriteFrame* const& o) const noexcept
    {
        o->fileName = this->fileName;
        o->collisionAreas = this->collisionAreas;
    }
    inline FishSpriteFrame* FishSpriteFrame::MakeCopy() const noexcept
    {
        auto rtv = mempool->MPCreate<FishSpriteFrame>();
        this->CopyTo(rtv);
        return rtv;
    }
    inline FishSpriteFrame_p FishSpriteFrame::MakePtrCopy() const noexcept
    {
        return FishSpriteFrame_p(this->MakeCopy());
    }

	inline FishConfig::FishConfig(xx::MemPool* const& mempool) noexcept
        : xx::Object(mempool)
	{
	}
	inline FishConfig::FishConfig(xx::BBuffer *bb)
        : xx::Object(bb->mempool)
	{
        if (int rtv = FromBBuffer(*bb)) throw rtv;
	}
    inline void FishConfig::ToBBuffer(xx::BBuffer &bb) const noexcept
    {
        bb.Write(this->typeId);
        bb.Write(this->name);
        bb.Write(this->coin);
        bb.Write(this->frameCount);
        bb.Write(this->collisionArea);
        bb.Write(this->frames);
    }
    inline int FishConfig::FromBBuffer(xx::BBuffer &bb) noexcept
    {
        int rtv = 0;
        if ((rtv = bb.Read(this->typeId))) return rtv;
        bb.readLengthLimit = 0;
        if ((rtv = bb.Read(this->name))) return rtv;
        if ((rtv = bb.Read(this->coin))) return rtv;
        if ((rtv = bb.Read(this->frameCount))) return rtv;
        if ((rtv = bb.Read(this->collisionArea))) return rtv;
        bb.readLengthLimit = 0;
        if ((rtv = bb.Read(this->frames))) return rtv;
        return rtv;
    }

    inline void FishConfig::ToString(xx::String &str) const noexcept
    {
        if (memHeader().flags)
        {
        	str.Append("[ \"***** recursived *****\" ]");
        	return;
        }
        else memHeader().flags = 1;

        str.Append("{ \"pkgTypeName\":\"CatchFish.FishConfig\", \"pkgTypeId\":", xx::TypeId_v<ThisType>);
        ToStringCore(str);
        str.Append(" }");
        
        memHeader().flags = 0;
    }
    inline void FishConfig::ToStringCore(xx::String &str) const noexcept
    {
        this->BaseType::ToStringCore(str);
        str.Append(", \"typeId\":", this->typeId);
        if (this->name) str.Append(", \"name\":\"", this->name, "\"");
        else str.Append(", \"name\":nil");
        str.Append(", \"coin\":", this->coin);
        str.Append(", \"frameCount\":", this->frameCount);
        str.Append(", \"collisionArea\":", this->collisionArea);
        str.Append(", \"frames\":", this->frames);
    }
    inline void FishConfig::CopyTo(FishConfig* const& o) const noexcept
    {
        o->typeId = this->typeId;
        o->name = this->name;
        o->coin = this->coin;
        o->frameCount = this->frameCount;
        o->collisionArea = this->collisionArea;
        o->frames = this->frames;
        o->anim = this->anim;
    }
    inline FishConfig* FishConfig::MakeCopy() const noexcept
    {
        auto rtv = mempool->MPCreate<FishConfig>();
        this->CopyTo(rtv);
        return rtv;
    }
    inline FishConfig_p FishConfig::MakePtrCopy() const noexcept
    {
        return FishConfig_p(this->MakeCopy());
    }

	inline Config::Config(xx::MemPool* const& mempool) noexcept
        : xx::Object(mempool)
	{
	}
	inline Config::Config(xx::BBuffer *bb)
        : xx::Object(bb->mempool)
	{
        if (int rtv = FromBBuffer(*bb)) throw rtv;
	}
    inline void Config::ToBBuffer(xx::BBuffer &bb) const noexcept
    {
        bb.Write(this->fishCfgs);
        bb.Write(this->bulletRadius);
        bb.Write(this->playerInitCoin);
    }
    inline int Config::FromBBuffer(xx::BBuffer &bb) noexcept
    {
        int rtv = 0;
        bb.readLengthLimit = 0;
        if ((rtv = bb.Read(this->fishCfgs))) return rtv;
        if ((rtv = bb.Read(this->bulletRadius))) return rtv;
        if ((rtv = bb.Read(this->playerInitCoin))) return rtv;
        return rtv;
    }

    inline void Config::ToString(xx::String &str) const noexcept
    {
        if (memHeader().flags)
        {
        	str.Append("[ \"***** recursived *****\" ]");
        	return;
        }
        else memHeader().flags = 1;

        str.Append("{ \"pkgTypeName\":\"CatchFish.Config\", \"pkgTypeId\":", xx::TypeId_v<ThisType>);
        ToStringCore(str);
        str.Append(" }");
        
        memHeader().flags = 0;
    }
    inline void Config::ToStringCore(xx::String &str) const noexcept
    {
        this->BaseType::ToStringCore(str);
        str.Append(", \"fishCfgs\":", this->fishCfgs);
        str.Append(", \"bulletRadius\":", this->bulletRadius);
        str.Append(", \"playerInitCoin\":", this->playerInitCoin);
    }
    inline void Config::CopyTo(Config* const& o) const noexcept
    {
        o->fishCfgs = this->fishCfgs;
        o->bulletRadius = this->bulletRadius;
        o->playerInitCoin = this->playerInitCoin;
    }
    inline Config* Config::MakeCopy() const noexcept
    {
        auto rtv = mempool->MPCreate<Config>();
        this->CopyTo(rtv);
        return rtv;
    }
    inline Config_p Config::MakePtrCopy() const noexcept
    {
        return Config_p(this->MakeCopy());
    }

	inline Player::Player(xx::MemPool* const& mempool) noexcept
        : xx::Object(mempool)
	{
	}
	inline Player::Player(xx::BBuffer *bb)
        : xx::Object(bb->mempool)
	{
        if (int rtv = FromBBuffer(*bb)) throw rtv;
	}
    inline void Player::ToBBuffer(xx::BBuffer &bb) const noexcept
    {
        bb.Write(this->indexAtContainer);
        bb.Write(this->name);
        bb.Write(this->sitIndex);
        bb.Write(this->coin);
        bb.Write(this->bulletSerialNumber);
        bb.Write(this->bullets);
    }
    inline int Player::FromBBuffer(xx::BBuffer &bb) noexcept
    {
        int rtv = 0;
        if ((rtv = bb.Read(this->indexAtContainer))) return rtv;
        bb.readLengthLimit = 0;
        if ((rtv = bb.Read(this->name))) return rtv;
        if ((rtv = bb.Read(this->sitIndex))) return rtv;
        if ((rtv = bb.Read(this->coin))) return rtv;
        if ((rtv = bb.Read(this->bulletSerialNumber))) return rtv;
        bb.readLengthLimit = 0;
        if ((rtv = bb.Read(this->bullets))) return rtv;
        return rtv;
    }

    inline void Player::ToString(xx::String &str) const noexcept
    {
        if (memHeader().flags)
        {
        	str.Append("[ \"***** recursived *****\" ]");
        	return;
        }
        else memHeader().flags = 1;

        str.Append("{ \"pkgTypeName\":\"CatchFish.Player\", \"pkgTypeId\":", xx::TypeId_v<ThisType>);
        ToStringCore(str);
        str.Append(" }");
        
        memHeader().flags = 0;
    }
    inline void Player::ToStringCore(xx::String &str) const noexcept
    {
        this->BaseType::ToStringCore(str);
        str.Append(", \"indexAtContainer\":", this->indexAtContainer);
        if (this->name) str.Append(", \"name\":\"", this->name, "\"");
        else str.Append(", \"name\":nil");
        str.Append(", \"sitIndex\":", this->sitIndex);
        str.Append(", \"coin\":", this->coin);
        str.Append(", \"bulletSerialNumber\":", this->bulletSerialNumber);
        str.Append(", \"bullets\":", this->bullets);
    }
    inline void Player::CopyTo(Player* const& o) const noexcept
    {
        o->indexAtContainer = this->indexAtContainer;
        o->name = this->name;
        o->sitIndex = this->sitIndex;
        o->coin = this->coin;
        o->bulletSerialNumber = this->bulletSerialNumber;
        o->bullets = this->bullets;
        o->peer = this->peer;
    }
    inline Player* Player::MakeCopy() const noexcept
    {
        auto rtv = mempool->MPCreate<Player>();
        this->CopyTo(rtv);
        return rtv;
    }
    inline Player_p Player::MakePtrCopy() const noexcept
    {
        return Player_p(this->MakeCopy());
    }

	inline MoveObject::MoveObject(xx::MemPool* const& mempool) noexcept
        : xx::Object(mempool)
	{
	}
	inline MoveObject::MoveObject(xx::BBuffer *bb)
        : xx::Object(bb->mempool)
	{
        if (int rtv = FromBBuffer(*bb)) throw rtv;
	}
    inline void MoveObject::ToBBuffer(xx::BBuffer &bb) const noexcept
    {
        bb.Write(this->indexAtContainer);
        bb.Write(this->serialNumber);
        bb.Write(this->bornFrameNumber);
        bb.Write(this->bornPos);
        bb.Write(this->pos);
        bb.Write(this->moveInc);
        bb.Write(this->angle);
    }
    inline int MoveObject::FromBBuffer(xx::BBuffer &bb) noexcept
    {
        int rtv = 0;
        if ((rtv = bb.Read(this->indexAtContainer))) return rtv;
        if ((rtv = bb.Read(this->serialNumber))) return rtv;
        if ((rtv = bb.Read(this->bornFrameNumber))) return rtv;
        if ((rtv = bb.Read(this->bornPos))) return rtv;
        if ((rtv = bb.Read(this->pos))) return rtv;
        if ((rtv = bb.Read(this->moveInc))) return rtv;
        if ((rtv = bb.Read(this->angle))) return rtv;
        return rtv;
    }

    inline void MoveObject::ToString(xx::String &str) const noexcept
    {
        if (memHeader().flags)
        {
        	str.Append("[ \"***** recursived *****\" ]");
        	return;
        }
        else memHeader().flags = 1;

        str.Append("{ \"pkgTypeName\":\"CatchFish.MoveObject\", \"pkgTypeId\":", xx::TypeId_v<ThisType>);
        ToStringCore(str);
        str.Append(" }");
        
        memHeader().flags = 0;
    }
    inline void MoveObject::ToStringCore(xx::String &str) const noexcept
    {
        this->BaseType::ToStringCore(str);
        str.Append(", \"indexAtContainer\":", this->indexAtContainer);
        str.Append(", \"serialNumber\":", this->serialNumber);
        str.Append(", \"bornFrameNumber\":", this->bornFrameNumber);
        str.Append(", \"bornPos\":", this->bornPos);
        str.Append(", \"pos\":", this->pos);
        str.Append(", \"moveInc\":", this->moveInc);
        str.Append(", \"angle\":", this->angle);
    }
    inline void MoveObject::CopyTo(MoveObject* const& o) const noexcept
    {
        o->indexAtContainer = this->indexAtContainer;
        o->serialNumber = this->serialNumber;
        o->bornFrameNumber = this->bornFrameNumber;
        o->bornPos = this->bornPos;
        o->pos = this->pos;
        o->moveInc = this->moveInc;
        o->angle = this->angle;
        o->spriteBody = this->spriteBody;
    }
    inline MoveObject* MoveObject::MakeCopy() const noexcept
    {
        auto rtv = mempool->MPCreate<MoveObject>();
        this->CopyTo(rtv);
        return rtv;
    }
    inline MoveObject_p MoveObject::MakePtrCopy() const noexcept
    {
        return MoveObject_p(this->MakeCopy());
    }

	inline Bullet::Bullet(xx::MemPool* const& mempool) noexcept
        : PKG::CatchFish::MoveObject(mempool)
	{
	}
	inline Bullet::Bullet(xx::BBuffer *bb)
        : PKG::CatchFish::MoveObject(bb->mempool)
	{
        if (int rtv = FromBBuffer(*bb)) throw rtv;
	}
    inline void Bullet::ToBBuffer(xx::BBuffer &bb) const noexcept
    {
        this->BaseType::ToBBuffer(bb);
        bb.Write(this->coin);
    }
    inline int Bullet::FromBBuffer(xx::BBuffer &bb) noexcept
    {
        int rtv = 0;
        if ((rtv = this->BaseType::FromBBuffer(bb))) return rtv;
        if ((rtv = bb.Read(this->coin))) return rtv;
        return rtv;
    }

    inline void Bullet::ToString(xx::String &str) const noexcept
    {
        if (memHeader().flags)
        {
        	str.Append("[ \"***** recursived *****\" ]");
        	return;
        }
        else memHeader().flags = 1;

        str.Append("{ \"pkgTypeName\":\"CatchFish.Bullet\", \"pkgTypeId\":", xx::TypeId_v<ThisType>);
        ToStringCore(str);
        str.Append(" }");
        
        memHeader().flags = 0;
    }
    inline void Bullet::ToStringCore(xx::String &str) const noexcept
    {
        this->BaseType::ToStringCore(str);
        str.Append(", \"coin\":", this->coin);
    }
    inline void Bullet::CopyTo(Bullet* const& o) const noexcept
    {
        this->BaseType::CopyTo(o);
        o->coin = this->coin;
    }
    inline Bullet* Bullet::MakeCopy() const noexcept
    {
        auto rtv = mempool->MPCreate<Bullet>();
        this->CopyTo(rtv);
        return rtv;
    }
    inline Bullet_p Bullet::MakePtrCopy() const noexcept
    {
        return Bullet_p(this->MakeCopy());
    }

	inline Fish::Fish(xx::MemPool* const& mempool) noexcept
        : PKG::CatchFish::MoveObject(mempool)
	{
	}
	inline Fish::Fish(xx::BBuffer *bb)
        : PKG::CatchFish::MoveObject(bb->mempool)
	{
        if (int rtv = FromBBuffer(*bb)) throw rtv;
	}
    inline void Fish::ToBBuffer(xx::BBuffer &bb) const noexcept
    {
        this->BaseType::ToBBuffer(bb);
        bb.Write(this->typeId);
        bb.Write(this->intAngle);
        bb.WriteDefaultValue<PKG::CatchFish::FishConfig_p>();
        bb.Write(this->moveTo);
        bb.Write(this->moveStep);
        bb.Write(this->moveStepCount);
    }
    inline int Fish::FromBBuffer(xx::BBuffer &bb) noexcept
    {
        int rtv = 0;
        if ((rtv = this->BaseType::FromBBuffer(bb))) return rtv;
        if ((rtv = bb.Read(this->typeId))) return rtv;
        if ((rtv = bb.Read(this->intAngle))) return rtv;
        if ((rtv = bb.Read(this->cfg))) return rtv;
        if ((rtv = bb.Read(this->moveTo))) return rtv;
        if ((rtv = bb.Read(this->moveStep))) return rtv;
        if ((rtv = bb.Read(this->moveStepCount))) return rtv;
        return rtv;
    }

    inline void Fish::ToString(xx::String &str) const noexcept
    {
        if (memHeader().flags)
        {
        	str.Append("[ \"***** recursived *****\" ]");
        	return;
        }
        else memHeader().flags = 1;

        str.Append("{ \"pkgTypeName\":\"CatchFish.Fish\", \"pkgTypeId\":", xx::TypeId_v<ThisType>);
        ToStringCore(str);
        str.Append(" }");
        
        memHeader().flags = 0;
    }
    inline void Fish::ToStringCore(xx::String &str) const noexcept
    {
        this->BaseType::ToStringCore(str);
        str.Append(", \"typeId\":", this->typeId);
        str.Append(", \"intAngle\":", this->intAngle);
        str.Append(", \"cfg\":", this->cfg);
        str.Append(", \"moveTo\":", this->moveTo);
        str.Append(", \"moveStep\":", this->moveStep);
        str.Append(", \"moveStepCount\":", this->moveStepCount);
    }
    inline void Fish::CopyTo(Fish* const& o) const noexcept
    {
        this->BaseType::CopyTo(o);
        o->typeId = this->typeId;
        o->intAngle = this->intAngle;
        o->cfg = this->cfg;
        o->moveTo = this->moveTo;
        o->moveStep = this->moveStep;
        o->moveStepCount = this->moveStepCount;
    }
    inline Fish* Fish::MakeCopy() const noexcept
    {
        auto rtv = mempool->MPCreate<Fish>();
        this->CopyTo(rtv);
        return rtv;
    }
    inline Fish_p Fish::MakePtrCopy() const noexcept
    {
        return Fish_p(this->MakeCopy());
    }

	inline Scene::Scene(xx::MemPool* const& mempool) noexcept
        : xx::Object(mempool)
	{
	}
	inline Scene::Scene(xx::BBuffer *bb)
        : xx::Object(bb->mempool)
	{
        if (int rtv = FromBBuffer(*bb)) throw rtv;
	}
    inline void Scene::ToBBuffer(xx::BBuffer &bb) const noexcept
    {
        bb.Write(this->frameNumber);
        bb.Write(this->rnd);
        bb.Write(this->players);
        bb.Write(this->fishSerialNumber);
        bb.Write(this->fishs);
        bb.WriteDefaultValue<PKG::CatchFish_Client::FrameEvents_p>();
    }
    inline int Scene::FromBBuffer(xx::BBuffer &bb) noexcept
    {
        int rtv = 0;
        if ((rtv = bb.Read(this->frameNumber))) return rtv;
        if ((rtv = bb.Read(this->rnd))) return rtv;
        bb.readLengthLimit = 0;
        if ((rtv = bb.Read(this->players))) return rtv;
        if ((rtv = bb.Read(this->fishSerialNumber))) return rtv;
        bb.readLengthLimit = 0;
        if ((rtv = bb.Read(this->fishs))) return rtv;
        if ((rtv = bb.Read(this->frameEvents))) return rtv;
        return rtv;
    }

    inline void Scene::ToString(xx::String &str) const noexcept
    {
        if (memHeader().flags)
        {
        	str.Append("[ \"***** recursived *****\" ]");
        	return;
        }
        else memHeader().flags = 1;

        str.Append("{ \"pkgTypeName\":\"CatchFish.Scene\", \"pkgTypeId\":", xx::TypeId_v<ThisType>);
        ToStringCore(str);
        str.Append(" }");
        
        memHeader().flags = 0;
    }
    inline void Scene::ToStringCore(xx::String &str) const noexcept
    {
        this->BaseType::ToStringCore(str);
        str.Append(", \"frameNumber\":", this->frameNumber);
        str.Append(", \"rnd\":", this->rnd);
        str.Append(", \"players\":", this->players);
        str.Append(", \"fishSerialNumber\":", this->fishSerialNumber);
        str.Append(", \"fishs\":", this->fishs);
        str.Append(", \"frameEvents\":", this->frameEvents);
    }
    inline void Scene::CopyTo(Scene* const& o) const noexcept
    {
        o->frameNumber = this->frameNumber;
        o->rnd = this->rnd;
        o->players = this->players;
        o->fishSerialNumber = this->fishSerialNumber;
        o->fishs = this->fishs;
        o->frameEvents = this->frameEvents;
    }
    inline Scene* Scene::MakeCopy() const noexcept
    {
        auto rtv = mempool->MPCreate<Scene>();
        this->CopyTo(rtv);
        return rtv;
    }
    inline Scene_p Scene::MakePtrCopy() const noexcept
    {
        return Scene_p(this->MakeCopy());
    }

}
namespace CatchFish::Events
{
	inline LeavePlayer::LeavePlayer(xx::MemPool* const& mempool) noexcept
        : xx::Object(mempool)
	{
	}
	inline LeavePlayer::LeavePlayer(xx::BBuffer *bb)
        : xx::Object(bb->mempool)
	{
        if (int rtv = FromBBuffer(*bb)) throw rtv;
	}
    inline void LeavePlayer::ToBBuffer(xx::BBuffer &bb) const noexcept
    {
        bb.Write(this->sitIndex);
    }
    inline int LeavePlayer::FromBBuffer(xx::BBuffer &bb) noexcept
    {
        int rtv = 0;
        if ((rtv = bb.Read(this->sitIndex))) return rtv;
        return rtv;
    }

    inline void LeavePlayer::ToString(xx::String &str) const noexcept
    {
        if (memHeader().flags)
        {
        	str.Append("[ \"***** recursived *****\" ]");
        	return;
        }
        else memHeader().flags = 1;

        str.Append("{ \"pkgTypeName\":\"CatchFish.Events.LeavePlayer\", \"pkgTypeId\":", xx::TypeId_v<ThisType>);
        ToStringCore(str);
        str.Append(" }");
        
        memHeader().flags = 0;
    }
    inline void LeavePlayer::ToStringCore(xx::String &str) const noexcept
    {
        this->BaseType::ToStringCore(str);
        str.Append(", \"sitIndex\":", this->sitIndex);
    }
    inline void LeavePlayer::CopyTo(LeavePlayer* const& o) const noexcept
    {
        o->sitIndex = this->sitIndex;
    }
    inline LeavePlayer* LeavePlayer::MakeCopy() const noexcept
    {
        auto rtv = mempool->MPCreate<LeavePlayer>();
        this->CopyTo(rtv);
        return rtv;
    }
    inline LeavePlayer_p LeavePlayer::MakePtrCopy() const noexcept
    {
        return LeavePlayer_p(this->MakeCopy());
    }

	inline JoinPlayer::JoinPlayer(xx::MemPool* const& mempool) noexcept
        : xx::Object(mempool)
	{
	}
	inline JoinPlayer::JoinPlayer(xx::BBuffer *bb)
        : xx::Object(bb->mempool)
	{
        if (int rtv = FromBBuffer(*bb)) throw rtv;
	}
    inline void JoinPlayer::ToBBuffer(xx::BBuffer &bb) const noexcept
    {
        bb.Write(this->name);
        bb.Write(this->sitIndex);
        bb.Write(this->coin);
    }
    inline int JoinPlayer::FromBBuffer(xx::BBuffer &bb) noexcept
    {
        int rtv = 0;
        bb.readLengthLimit = 0;
        if ((rtv = bb.Read(this->name))) return rtv;
        if ((rtv = bb.Read(this->sitIndex))) return rtv;
        if ((rtv = bb.Read(this->coin))) return rtv;
        return rtv;
    }

    inline void JoinPlayer::ToString(xx::String &str) const noexcept
    {
        if (memHeader().flags)
        {
        	str.Append("[ \"***** recursived *****\" ]");
        	return;
        }
        else memHeader().flags = 1;

        str.Append("{ \"pkgTypeName\":\"CatchFish.Events.JoinPlayer\", \"pkgTypeId\":", xx::TypeId_v<ThisType>);
        ToStringCore(str);
        str.Append(" }");
        
        memHeader().flags = 0;
    }
    inline void JoinPlayer::ToStringCore(xx::String &str) const noexcept
    {
        this->BaseType::ToStringCore(str);
        if (this->name) str.Append(", \"name\":\"", this->name, "\"");
        else str.Append(", \"name\":nil");
        str.Append(", \"sitIndex\":", this->sitIndex);
        str.Append(", \"coin\":", this->coin);
    }
    inline void JoinPlayer::CopyTo(JoinPlayer* const& o) const noexcept
    {
        o->name = this->name;
        o->sitIndex = this->sitIndex;
        o->coin = this->coin;
    }
    inline JoinPlayer* JoinPlayer::MakeCopy() const noexcept
    {
        auto rtv = mempool->MPCreate<JoinPlayer>();
        this->CopyTo(rtv);
        return rtv;
    }
    inline JoinPlayer_p JoinPlayer::MakePtrCopy() const noexcept
    {
        return JoinPlayer_p(this->MakeCopy());
    }

	inline Fire::Fire(xx::MemPool* const& mempool) noexcept
        : xx::Object(mempool)
	{
	}
	inline Fire::Fire(xx::BBuffer *bb)
        : xx::Object(bb->mempool)
	{
        if (int rtv = FromBBuffer(*bb)) throw rtv;
	}
    inline void Fire::ToBBuffer(xx::BBuffer &bb) const noexcept
    {
        bb.Write(this->sitIndex);
        bb.Write(this->frameNumber);
        bb.Write(this->bulletSerialNumber);
        bb.Write(this->coin);
        bb.Write(this->moveInc);
    }
    inline int Fire::FromBBuffer(xx::BBuffer &bb) noexcept
    {
        int rtv = 0;
        if ((rtv = bb.Read(this->sitIndex))) return rtv;
        if ((rtv = bb.Read(this->frameNumber))) return rtv;
        if ((rtv = bb.Read(this->bulletSerialNumber))) return rtv;
        if ((rtv = bb.Read(this->coin))) return rtv;
        if ((rtv = bb.Read(this->moveInc))) return rtv;
        return rtv;
    }

    inline void Fire::ToString(xx::String &str) const noexcept
    {
        if (memHeader().flags)
        {
        	str.Append("[ \"***** recursived *****\" ]");
        	return;
        }
        else memHeader().flags = 1;

        str.Append("{ \"pkgTypeName\":\"CatchFish.Events.Fire\", \"pkgTypeId\":", xx::TypeId_v<ThisType>);
        ToStringCore(str);
        str.Append(" }");
        
        memHeader().flags = 0;
    }
    inline void Fire::ToStringCore(xx::String &str) const noexcept
    {
        this->BaseType::ToStringCore(str);
        str.Append(", \"sitIndex\":", this->sitIndex);
        str.Append(", \"frameNumber\":", this->frameNumber);
        str.Append(", \"bulletSerialNumber\":", this->bulletSerialNumber);
        str.Append(", \"coin\":", this->coin);
        str.Append(", \"moveInc\":", this->moveInc);
    }
    inline void Fire::CopyTo(Fire* const& o) const noexcept
    {
        o->sitIndex = this->sitIndex;
        o->frameNumber = this->frameNumber;
        o->bulletSerialNumber = this->bulletSerialNumber;
        o->coin = this->coin;
        o->moveInc = this->moveInc;
    }
    inline Fire* Fire::MakeCopy() const noexcept
    {
        auto rtv = mempool->MPCreate<Fire>();
        this->CopyTo(rtv);
        return rtv;
    }
    inline Fire_p Fire::MakePtrCopy() const noexcept
    {
        return Fire_p(this->MakeCopy());
    }

	inline FireBegin::FireBegin(xx::MemPool* const& mempool) noexcept
        : xx::Object(mempool)
	{
	}
	inline FireBegin::FireBegin(xx::BBuffer *bb)
        : xx::Object(bb->mempool)
	{
        if (int rtv = FromBBuffer(*bb)) throw rtv;
	}
    inline void FireBegin::ToBBuffer(xx::BBuffer &bb) const noexcept
    {
        bb.Write(this->sitIndex);
        bb.Write(this->angle);
    }
    inline int FireBegin::FromBBuffer(xx::BBuffer &bb) noexcept
    {
        int rtv = 0;
        if ((rtv = bb.Read(this->sitIndex))) return rtv;
        if ((rtv = bb.Read(this->angle))) return rtv;
        return rtv;
    }

    inline void FireBegin::ToString(xx::String &str) const noexcept
    {
        if (memHeader().flags)
        {
        	str.Append("[ \"***** recursived *****\" ]");
        	return;
        }
        else memHeader().flags = 1;

        str.Append("{ \"pkgTypeName\":\"CatchFish.Events.FireBegin\", \"pkgTypeId\":", xx::TypeId_v<ThisType>);
        ToStringCore(str);
        str.Append(" }");
        
        memHeader().flags = 0;
    }
    inline void FireBegin::ToStringCore(xx::String &str) const noexcept
    {
        this->BaseType::ToStringCore(str);
        str.Append(", \"sitIndex\":", this->sitIndex);
        str.Append(", \"angle\":", this->angle);
    }
    inline void FireBegin::CopyTo(FireBegin* const& o) const noexcept
    {
        o->sitIndex = this->sitIndex;
        o->angle = this->angle;
    }
    inline FireBegin* FireBegin::MakeCopy() const noexcept
    {
        auto rtv = mempool->MPCreate<FireBegin>();
        this->CopyTo(rtv);
        return rtv;
    }
    inline FireBegin_p FireBegin::MakePtrCopy() const noexcept
    {
        return FireBegin_p(this->MakeCopy());
    }

	inline FireChangeAngle::FireChangeAngle(xx::MemPool* const& mempool) noexcept
        : xx::Object(mempool)
	{
	}
	inline FireChangeAngle::FireChangeAngle(xx::BBuffer *bb)
        : xx::Object(bb->mempool)
	{
        if (int rtv = FromBBuffer(*bb)) throw rtv;
	}
    inline void FireChangeAngle::ToBBuffer(xx::BBuffer &bb) const noexcept
    {
        bb.Write(this->sitIndex);
        bb.Write(this->angle);
    }
    inline int FireChangeAngle::FromBBuffer(xx::BBuffer &bb) noexcept
    {
        int rtv = 0;
        if ((rtv = bb.Read(this->sitIndex))) return rtv;
        if ((rtv = bb.Read(this->angle))) return rtv;
        return rtv;
    }

    inline void FireChangeAngle::ToString(xx::String &str) const noexcept
    {
        if (memHeader().flags)
        {
        	str.Append("[ \"***** recursived *****\" ]");
        	return;
        }
        else memHeader().flags = 1;

        str.Append("{ \"pkgTypeName\":\"CatchFish.Events.FireChangeAngle\", \"pkgTypeId\":", xx::TypeId_v<ThisType>);
        ToStringCore(str);
        str.Append(" }");
        
        memHeader().flags = 0;
    }
    inline void FireChangeAngle::ToStringCore(xx::String &str) const noexcept
    {
        this->BaseType::ToStringCore(str);
        str.Append(", \"sitIndex\":", this->sitIndex);
        str.Append(", \"angle\":", this->angle);
    }
    inline void FireChangeAngle::CopyTo(FireChangeAngle* const& o) const noexcept
    {
        o->sitIndex = this->sitIndex;
        o->angle = this->angle;
    }
    inline FireChangeAngle* FireChangeAngle::MakeCopy() const noexcept
    {
        auto rtv = mempool->MPCreate<FireChangeAngle>();
        this->CopyTo(rtv);
        return rtv;
    }
    inline FireChangeAngle_p FireChangeAngle::MakePtrCopy() const noexcept
    {
        return FireChangeAngle_p(this->MakeCopy());
    }

	inline FireEnd::FireEnd(xx::MemPool* const& mempool) noexcept
        : xx::Object(mempool)
	{
	}
	inline FireEnd::FireEnd(xx::BBuffer *bb)
        : xx::Object(bb->mempool)
	{
        if (int rtv = FromBBuffer(*bb)) throw rtv;
	}
    inline void FireEnd::ToBBuffer(xx::BBuffer &bb) const noexcept
    {
        bb.Write(this->sitIndex);
    }
    inline int FireEnd::FromBBuffer(xx::BBuffer &bb) noexcept
    {
        int rtv = 0;
        if ((rtv = bb.Read(this->sitIndex))) return rtv;
        return rtv;
    }

    inline void FireEnd::ToString(xx::String &str) const noexcept
    {
        if (memHeader().flags)
        {
        	str.Append("[ \"***** recursived *****\" ]");
        	return;
        }
        else memHeader().flags = 1;

        str.Append("{ \"pkgTypeName\":\"CatchFish.Events.FireEnd\", \"pkgTypeId\":", xx::TypeId_v<ThisType>);
        ToStringCore(str);
        str.Append(" }");
        
        memHeader().flags = 0;
    }
    inline void FireEnd::ToStringCore(xx::String &str) const noexcept
    {
        this->BaseType::ToStringCore(str);
        str.Append(", \"sitIndex\":", this->sitIndex);
    }
    inline void FireEnd::CopyTo(FireEnd* const& o) const noexcept
    {
        o->sitIndex = this->sitIndex;
    }
    inline FireEnd* FireEnd::MakeCopy() const noexcept
    {
        auto rtv = mempool->MPCreate<FireEnd>();
        this->CopyTo(rtv);
        return rtv;
    }
    inline FireEnd_p FireEnd::MakePtrCopy() const noexcept
    {
        return FireEnd_p(this->MakeCopy());
    }

	inline BulletHit::BulletHit(xx::MemPool* const& mempool) noexcept
        : xx::Object(mempool)
	{
	}
	inline BulletHit::BulletHit(xx::BBuffer *bb)
        : xx::Object(bb->mempool)
	{
        if (int rtv = FromBBuffer(*bb)) throw rtv;
	}
    inline void BulletHit::ToBBuffer(xx::BBuffer &bb) const noexcept
    {
        bb.Write(this->sitIndex);
        bb.Write(this->bulletSerialNumber);
    }
    inline int BulletHit::FromBBuffer(xx::BBuffer &bb) noexcept
    {
        int rtv = 0;
        if ((rtv = bb.Read(this->sitIndex))) return rtv;
        if ((rtv = bb.Read(this->bulletSerialNumber))) return rtv;
        return rtv;
    }

    inline void BulletHit::ToString(xx::String &str) const noexcept
    {
        if (memHeader().flags)
        {
        	str.Append("[ \"***** recursived *****\" ]");
        	return;
        }
        else memHeader().flags = 1;

        str.Append("{ \"pkgTypeName\":\"CatchFish.Events.BulletHit\", \"pkgTypeId\":", xx::TypeId_v<ThisType>);
        ToStringCore(str);
        str.Append(" }");
        
        memHeader().flags = 0;
    }
    inline void BulletHit::ToStringCore(xx::String &str) const noexcept
    {
        this->BaseType::ToStringCore(str);
        str.Append(", \"sitIndex\":", this->sitIndex);
        str.Append(", \"bulletSerialNumber\":", this->bulletSerialNumber);
    }
    inline void BulletHit::CopyTo(BulletHit* const& o) const noexcept
    {
        o->sitIndex = this->sitIndex;
        o->bulletSerialNumber = this->bulletSerialNumber;
    }
    inline BulletHit* BulletHit::MakeCopy() const noexcept
    {
        auto rtv = mempool->MPCreate<BulletHit>();
        this->CopyTo(rtv);
        return rtv;
    }
    inline BulletHit_p BulletHit::MakePtrCopy() const noexcept
    {
        return BulletHit_p(this->MakeCopy());
    }

	inline FishDead::FishDead(xx::MemPool* const& mempool) noexcept
        : xx::Object(mempool)
	{
	}
	inline FishDead::FishDead(xx::BBuffer *bb)
        : xx::Object(bb->mempool)
	{
        if (int rtv = FromBBuffer(*bb)) throw rtv;
	}
    inline void FishDead::ToBBuffer(xx::BBuffer &bb) const noexcept
    {
        bb.Write(this->sitIndex);
        bb.Write(this->fishSerialNumber);
        bb.Write(this->coin);
    }
    inline int FishDead::FromBBuffer(xx::BBuffer &bb) noexcept
    {
        int rtv = 0;
        if ((rtv = bb.Read(this->sitIndex))) return rtv;
        if ((rtv = bb.Read(this->fishSerialNumber))) return rtv;
        if ((rtv = bb.Read(this->coin))) return rtv;
        return rtv;
    }

    inline void FishDead::ToString(xx::String &str) const noexcept
    {
        if (memHeader().flags)
        {
        	str.Append("[ \"***** recursived *****\" ]");
        	return;
        }
        else memHeader().flags = 1;

        str.Append("{ \"pkgTypeName\":\"CatchFish.Events.FishDead\", \"pkgTypeId\":", xx::TypeId_v<ThisType>);
        ToStringCore(str);
        str.Append(" }");
        
        memHeader().flags = 0;
    }
    inline void FishDead::ToStringCore(xx::String &str) const noexcept
    {
        this->BaseType::ToStringCore(str);
        str.Append(", \"sitIndex\":", this->sitIndex);
        str.Append(", \"fishSerialNumber\":", this->fishSerialNumber);
        str.Append(", \"coin\":", this->coin);
    }
    inline void FishDead::CopyTo(FishDead* const& o) const noexcept
    {
        o->sitIndex = this->sitIndex;
        o->fishSerialNumber = this->fishSerialNumber;
        o->coin = this->coin;
    }
    inline FishDead* FishDead::MakeCopy() const noexcept
    {
        auto rtv = mempool->MPCreate<FishDead>();
        this->CopyTo(rtv);
        return rtv;
    }
    inline FishDead_p FishDead::MakePtrCopy() const noexcept
    {
        return FishDead_p(this->MakeCopy());
    }

}
}
namespace PKG
{
	inline void AllTypesRegister() noexcept
	{
	    xx::MemPool::Register<PKG::CatchFish_Client::JoinSuccess, xx::Object>();
	    xx::MemPool::Register<PKG::CatchFish_Client::JoinFail, xx::Object>();
	    xx::MemPool::Register<PKG::CatchFish_Client::FullSync, xx::Object>();
	    xx::MemPool::Register<PKG::CatchFish::Scene, xx::Object>();
	    xx::MemPool::Register<PKG::CatchFish_Client::FrameEvents, xx::Object>();
	    xx::MemPool::Register<xx::List<PKG::CatchFish::Events::LeavePlayer_p>, xx::Object>();
	    xx::MemPool::Register<PKG::CatchFish::Events::LeavePlayer, xx::Object>();
	    xx::MemPool::Register<xx::List<PKG::CatchFish::Events::JoinPlayer_p>, xx::Object>();
	    xx::MemPool::Register<PKG::CatchFish::Events::JoinPlayer, xx::Object>();
	    xx::MemPool::Register<xx::List<PKG::CatchFish::Events::FishDead_p>, xx::Object>();
	    xx::MemPool::Register<PKG::CatchFish::Events::FishDead, xx::Object>();
	    xx::MemPool::Register<xx::List<PKG::CatchFish::Events::Fire_p>, xx::Object>();
	    xx::MemPool::Register<PKG::CatchFish::Events::Fire, xx::Object>();
	    xx::MemPool::Register<xx::List<PKG::CatchFish::Events::FireEnd_p>, xx::Object>();
	    xx::MemPool::Register<PKG::CatchFish::Events::FireEnd, xx::Object>();
	    xx::MemPool::Register<xx::List<PKG::CatchFish::Events::FireBegin_p>, xx::Object>();
	    xx::MemPool::Register<PKG::CatchFish::Events::FireBegin, xx::Object>();
	    xx::MemPool::Register<xx::List<PKG::CatchFish::Events::FireChangeAngle_p>, xx::Object>();
	    xx::MemPool::Register<PKG::CatchFish::Events::FireChangeAngle, xx::Object>();
	    xx::MemPool::Register<PKG::Client_CatchFish::Join, xx::Object>();
	    xx::MemPool::Register<PKG::Client_CatchFish::Leave, xx::Object>();
	    xx::MemPool::Register<PKG::Client_CatchFish::Fire, xx::Object>();
	    xx::MemPool::Register<PKG::Client_CatchFish::Hit, xx::Object>();
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
	    xx::MemPool::Register<xx::List<PKG::CatchFish::Bullet_p>, xx::Object>();
	    xx::MemPool::Register<PKG::CatchFish::Bullet, PKG::CatchFish::MoveObject>();
	    xx::MemPool::Register<PKG::CatchFish::MoveObject, xx::Object>();
	    xx::MemPool::Register<PKG::CatchFish::Fish, PKG::CatchFish::MoveObject>();
	    xx::MemPool::Register<::xx::Random, xx::Object>();
	    xx::MemPool::Register<xx::List<PKG::CatchFish::Player_p>, xx::Object>();
	    xx::MemPool::Register<xx::List<PKG::CatchFish::Fish_p>, xx::Object>();
	    xx::MemPool::Register<PKG::CatchFish::Events::BulletHit, xx::Object>();
	}
}
