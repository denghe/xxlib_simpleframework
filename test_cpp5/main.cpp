#define _CRT_SECURE_NO_WARNINGS
#include "xx_uv.h"
#include "xx_pos.h"

// 备注和文档在客户端
struct BornLine
{
	int o1, o2, x1, x2, y1, y2;
};
static std::array<BornLine, 8> bornLines
{ BornLine
{ 3, 4, -160, 640, 880, 880 },
{ 5, 6, 640, 1440, 880, 880 },
{ 5, 6, 1440, 1440, 360, 880 },
{ 7, 0, 1440, 1440, -160, 360 },
{ 7, 0, 640, 1440, -160, -160 },
{ 1, 2, -160, 640, -160, -160 },
{ 1, 2, -160, -160, -160, 360 },
{ 3, 4, -160, -160, 360, 880 },
};




/*************************************************************************************/

class ServiceCatchFish;

namespace PKG::CatchFish
{
	class Player;
}

// 玩家上下文( 在 service 中放置，下标与 scene.players 保持同步 )
class PlayerContext : public xx::UvContextBase
{
public:
	ServiceCatchFish* service;
	PKG::CatchFish::Player* player;
	PlayerContext(ServiceCatchFish* service, PKG::CatchFish::Player* player);

	void HandlePackage(xx::Object_p& o) override;
	void HandleRequest(uint32_t serial, xx::Object_p& o) override;
	void HandleDisconnect() override;
};
using PlayerContext_p = xx::Ptr<PlayerContext>;



#include "../pkg/PKG_class.h"

// 服务上下文
class ServiceCatchFish : public xx::Object
{
public:
	// 引用到 loop
	xx::UvLoop& loop;

	// 接受客户端的连接
	xx::UvTcpListener listener;

	// 帧驱动用 timer
	xx::UvTimer timer;

	// 鱼的场景
	PKG::CatchFish::Scene_p scene;

	// 反复使用的，用来选座的数组
	xx::List<int> freeSits;

	// 传入 x, y 范围, 返回一个随机坐标
	inline xx::Pos RandVec2(int const& x1, int const& x2, int const& y1, int const& y2)
	{
		assert(x1 <= x2 && y1 <= y2);
		return xx::Pos{ float(scene->rnd->NextDouble() * (x2 - x1) + x1), float(scene->rnd->NextDouble() * (y2 - y1) + y1) };
	}

	// 网络初始化
	inline void NetInit()
	{
		// 初始化网络监听部分
		listener.Bind("0.0.0.0", 12345);
		listener.Listen();
		listener.OnAccept = [this](xx::UvTcpPeer *p)
		{
			// 未绑定到上下文之前，作为匿名连接，先接收 Join 指令
			p->OnReceivePackage = [this, p](xx::BBuffer& bb)
			{
				xx::Object_p o_;

				// 试解包, 解包失败, 断线
				if (int r = bb.ReadRoot(o_))
				{
					p->Release();
					return;
				}

				// 收到非法包, 断线
				if (!o_)
				{
					p->Release();
					return;
				}

				// 收到 join 包，开始逻辑处理
				if (o_->memHeader().typeId == xx::TypeId_v<PKG::Client_CatchFish::Join>)
				{
					// 硬转还原出真实类型备用
					auto o = (PKG::Client_CatchFish::Join*)o_.pointer;

					// 如果玩家已在线( name已存在 ), 就禁止登陆，踢掉
					if (scene->players->Exists([o](PKG::CatchFish::Player_p& plr)
					{
						return plr->name->Equals(o->username);
					}))
					{
						p->Release();
						return;
					}

					// 选空位
					freeSits.Clear();
					freeSits.Add(0);
					freeSits.Add(1);
					freeSits.Add(2);
					freeSits.Add(3);
					for (size_t i = 0; i < scene->players->dataLen; ++i)
					{
						freeSits.Remove(scene->players->At(i)->sitIndex);
					}

					// 没有空位了( 理论上讲如果是大厅分配过来的，不应该出现这种情况 )
					if (freeSits.dataLen == 0)
					{
						p->Release();
						return;
					}

					// 创建玩家及网络上下文
					PKG::CatchFish::Player_p plr;
					plr.MPCreate(mempool);
					plr->bullets.MPCreate(mempool);
					plr->coin = 10000;						// todo: 读配置
					plr->name = std::move(o->username);
					plr->sitIndex = freeSits[0];
					plr->ctx.Create(mempool, this, plr.pointer);
					auto r = plr->ctx->BindPeer(p);
					assert(r);								// 必然成功

					// 向场景每帧包合并变量压入 Join 事件

				}
			};
		};
	}

	// 场景初始化
	inline void SceneInit()
	{
		// 初始化鱼场景
		scene.MPCreate(mempool);
		scene->rnd.MPCreate(mempool, 123);	// 先随便来个 seed
		scene->players.MPCreate(mempool);
		scene->fishs.MPCreate(mempool);
		scene->cfg.MPCreate(mempool);
		scene->cfg->fishCfgs.MPCreate(mempool);

		// 捏造鱼的配置信息( 当前就不具体到每一帧了，coin 同时拿来当作 判定半径使用， 如果需要的话 )
		{
			auto fc = mempool->MPCreate<PKG::CatchFish::FishConfig>();
			fc->typeId = 0;
			fc->coin = 55;
			fc->name.MPCreate(mempool, "angler");
			scene->cfg->fishCfgs->Add(std::move(fc));
		}
		{
			auto fc = mempool->MPCreate<PKG::CatchFish::FishConfig>();
			fc->typeId = 1;
			fc->coin = 55;
			fc->name.MPCreate(mempool, "bomb");
			scene->cfg->fishCfgs->Add(std::move(fc));
		}
		{
			auto fc = mempool->MPCreate<PKG::CatchFish::FishConfig>();
			fc->typeId = 2;
			fc->coin = 40;
			fc->name.MPCreate(mempool, "butterfly");
			scene->cfg->fishCfgs->Add(std::move(fc));
		}
		{
			auto fc = mempool->MPCreate<PKG::CatchFish::FishConfig>();
			fc->typeId = 3;
			fc->coin = 50;
			fc->name.MPCreate(mempool, "flat");
			scene->cfg->fishCfgs->Add(std::move(fc));
		}
		{
			auto fc = mempool->MPCreate<PKG::CatchFish::FishConfig>();
			fc->typeId = 4;
			fc->coin = 50;
			fc->name.MPCreate(mempool, "lobster");
			scene->cfg->fishCfgs->Add(std::move(fc));
		}
		{
			auto fc = mempool->MPCreate<PKG::CatchFish::FishConfig>();
			fc->typeId = 5;
			fc->coin = 30;
			fc->name.MPCreate(mempool, "nemo");
			scene->cfg->fishCfgs->Add(std::move(fc));
		}
		{
			auto fc = mempool->MPCreate<PKG::CatchFish::FishConfig>();
			fc->typeId = 6;
			fc->coin = 50;
			fc->name.MPCreate(mempool, "octopus");
			scene->cfg->fishCfgs->Add(std::move(fc));
		}
		{
			auto fc = mempool->MPCreate<PKG::CatchFish::FishConfig>();
			fc->typeId = 7;
			fc->coin = 50;
			fc->name.MPCreate(mempool, "sali");
			scene->cfg->fishCfgs->Add(std::move(fc));
		}
		{
			auto fc = mempool->MPCreate<PKG::CatchFish::FishConfig>();
			fc->typeId = 8;
			fc->coin = 50;
			fc->name.MPCreate(mempool, "seasnail");
			scene->cfg->fishCfgs->Add(std::move(fc));
		}
		{
			auto fc = mempool->MPCreate<PKG::CatchFish::FishConfig>();
			fc->typeId = 9;
			fc->coin = 60;
			fc->name.MPCreate(mempool, "seaturtle");
			scene->cfg->fishCfgs->Add(std::move(fc));
		}
		{
			auto fc = mempool->MPCreate<PKG::CatchFish::FishConfig>();
			fc->typeId = 10;
			fc->coin = 80;
			fc->name.MPCreate(mempool, "shark");
			scene->cfg->fishCfgs->Add(std::move(fc));
		}

		// 填充子弹半径配置
		scene->cfg->bulletRadius = 20;
	}


	ServiceCatchFish(xx::UvLoop& loop)
		: Object(loop.mempool)
		, loop(loop)
		, listener(loop)
		, timer(loop, 0, 1000 / 60)	// 每秒 60 帧
		, freeSits(loop.mempool)
	{
		NetInit();
		SceneInit();

		// 针对 scene 做 update ( 随机出鱼, 鱼生命周期管理 )
		timer.OnFire = [this]
		{
			auto& scene = *this->scene;
			auto& fishs = *scene.fishs;
			auto& frameNumber = scene.frameNumber;
			auto& rnd = *scene.rnd;
			auto& fishCfgs = *scene.cfg->fishCfgs;

			// 遍历所有鱼 令其前进( 倒着扫以兼容交换删除 )
			for (int i = (int)fishs.dataLen - 1; i >= 0; --i)
			{
				auto& f = fishs[i];
				++f->moveStep;
				if (f->moveStep < f->moveStepCount)
				{
					//f->pos += f->posFrameInc;
					//f->spriteBody->setPosition(f->pos);		// 同步显示( 位移 )
				}
				else
				{
					// 将数据上下文从容器交换移除
					auto lastIdx = fishs.dataLen - 1;
					if (lastIdx > 0 && f->indexAtContainer != lastIdx)
					{
						fishs[lastIdx]->indexAtContainer = f->indexAtContainer;
						fishs[f->indexAtContainer] = std::move(fishs[lastIdx]);
					}
					fishs.Pop();

					// 资源回收
					//f->spriteBody->removeFromParentAndCleanup(true);	// 同步显示( 移除 )
					//delete f;
				}
			}

			// 每 3 帧生成 1 条鱼
			++frameNumber;
			if ((frameNumber % 3) > 0) return;


			// 创建鱼之上下文
			auto fish = mempool->MPCreate<PKG::CatchFish::Fish>();
			fish->indexAtContainer = (int)fishs.dataLen;

			// 先随机出鱼边
			auto idx = rand() % 8;

			// 定位到边
			auto line = &bornLines[idx];

			// 随出具体坐标
			fish->pos = fish->posFrom = RandVec2(line->x1, line->x2, line->y1, line->y2);

			// 再随机消失边
			idx = (rand() & 1) ? bornLines[idx].o1 : bornLines[idx].o2;

			// 定位到边
			line = &bornLines[idx];

			// 随出具体坐标
			fish->posTo = RandVec2(line->x1, line->x2, line->y1, line->y2);

			// 随机一条鱼下标
			idx = rnd.Next((int)fishCfgs.dataLen);

			// 存相关信息及半径( 暂行方案 )
			fish->cfg = fishCfgs[idx];

			//// 画鱼
			//auto sprite = Sprite::create();
			//fish->spriteBody = sprite;
			//addChild(sprite);

			//// 设鱼的起点和转向前进方向
			//sprite->setPosition(fish->posFrom);
			//::RotateTo(sprite, fish->posTo);

			//// 简单的创建一个游鱼动画
			//auto anim = fish->animInfo->anim;
			//sprite->runAction(Animate::create(anim));

			// 计划鱼需要花 20 秒从起点游到终点. 1 秒 60帖, 如果每 3 帧画 1 条鱼, 每秒会出 20 条, 20 秒后将有 400 条同屏
			// 算鱼每帧的移动增量( 每秒 60 帧 )
			fish->posFrameInc.x = (fish->posTo.x - fish->posFrom.x) / (60 * 20);	// 移20秒
			fish->posFrameInc.y = (fish->posTo.y - fish->posFrom.y) / (60 * 20);	// 移20秒
			fish->moveStepCount = 60 * 20;
			fish->moveStep = 0;

			// 将鱼放入容器
			fishs.Add(std::move(fish));
		};
	}

	~ServiceCatchFish()
	{
	}
};

inline PlayerContext::PlayerContext(ServiceCatchFish* service, PKG::CatchFish::Player* player)
	: xx::UvContextBase(service->mempool)
	, service(service)
	, player(player)
{
}

inline void PlayerContext::HandlePackage(xx::Object_p& o)
{

}

inline void PlayerContext::HandleRequest(uint32_t serial, xx::Object_p& o) {	}
inline void PlayerContext::HandleDisconnect() {	}

int main()
{
	// typeid 映射
	xx::MemPool::RegisterInternal();
	PKG::AllTypesRegister();

	// 创建基础内存池
	xx::MemPool mp;

	return 0;
}
