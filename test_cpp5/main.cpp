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



// 适配 PKG 生成物
class Sprite_p {};
class Animation_p {};

class ClientPeer;
using ClientPeer_p = ClientPeer * ;


#include "../pkg/PKG_class.h"
#include "fill_fish_cfg.h"

class ServiceCatchFish;

// 派生一下以方便放置 fishScene, player 指针
class ClientPeer : public xx::UvTcpPeer
{
public:
	// 保存一个引用
	PKG::CatchFish::Player* player = nullptr;

	// 收包队列1
	xx::Queue<PKG::Client_CatchFish::Fire_p> recvFires;

	// 收包队列2
	xx::Queue<PKG::Client_CatchFish::Hit_p> recvHits;

	ClientPeer(ServiceCatchFish* service);
};


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

	// 鱼的场景										// todo: 把配置从 scene 中移除
	PKG::CatchFish::Scene_p fishScene;

	// 预加载鱼配置
	PKG::CatchFish::Config_p cfg;

	// 反复使用的，用来选座的数组
	xx::List<int> freeSits;

	// 传入 x, y 范围, 返回一个随机坐标
	inline xx::Pos RandVec2(int const& x1, int const& x2, int const& y1, int const& y2)
	{
		assert(x1 <= x2 && y1 <= y2);
		return xx::Pos{ float(fishScene->rnd->NextDouble() * (x2 - x1) + x1), float(fishScene->rnd->NextDouble() * (y2 - y1) + y1) };
	}



	// 出鱼逻辑
	inline void MakeFishs()
	{
		// 每 xx 帧生成 1 条鱼
		if ((fishScene->frameNumber % 15) == 0)
		{
			auto& rnd = fishScene->rnd;

			// 创建鱼之上下文
			PKG::CatchFish::Fish_p fish;
			fish.MPCreate(mempool);
			fish->indexAtContainer = (int)fishScene->fishs->dataLen;

			// 先随机出鱼边
			auto idx = rnd->Next(8);

			// 定位到边
			auto line = &bornLines[idx];

			// 随出具体坐标
			fish->pos = fish->bornPos = RandVec2(line->x1, line->x2, line->y1, line->y2);

			// 再随机消失边
			idx = rnd->Next(2) ? bornLines[idx].o1 : bornLines[idx].o2;

			// 定位到边
			line = &bornLines[idx];

			// 随出具体坐标
			fish->moveTo = RandVec2(line->x1, line->x2, line->y1, line->y2);

			// 随机一条鱼下标
			idx = rnd->Next((int)cfg->fishCfgs->dataLen);

			// 设置其配置 & id
			fish->cfg = cfg->fishCfgs->At(idx);
			fish->typeId = fish->cfg->typeId;

#ifdef CC_TARGET_PLATFORM
			// 画鱼
			auto sprite = cocos2d::Sprite::create();
			fish->spriteBody = sprite;
			addChild(sprite);

			// 设鱼的起点和转向前进方向
			sprite->setPosition(fish->bornPos);
			HelloWorld::RotateTo(sprite, fish->moveTo);

			// 简单的创建一个游鱼动画
			auto anim = fish->cfg->anim;
			sprite->runAction(cocos2d::Animate::create(anim));
#endif

			// 计划鱼需要花 20 秒从起点游到终点. 1 秒 60帖, 如果每 3 帧画 1 条鱼, 每秒会出 20 条, 20 秒后将有 400 条同屏
			// 算鱼每帧的移动增量( 每秒60帧 )
			fish->moveInc.x = (fish->moveTo.x - fish->bornPos.x) / (60 * 20);	// 移20秒
			fish->moveInc.y = (fish->moveTo.y - fish->bornPos.y) / (60 * 20);	// 移20秒
			fish->moveStepCount = 60 * 20;
			fish->moveStep = 0;
			fish->serialNumber = fishScene->fishSerialNumber++;

			fishScene->fishs->Add(std::move(fish));
		}
	}

	// 鱼移动逻辑
	inline void MoveFishs()
	{
		auto& fishs = *fishScene->fishs;

		// 遍历所有鱼 令其前进( 倒着扫以兼容交换删除 )
		for (int i = (int)fishs.dataLen - 1; i >= 0; --i)
		{
			auto f = fishs[i].pointer;
			++f->moveStep;
			if (f->moveStep < f->moveStepCount)
			{
#ifdef CC_TARGET_PLATFORM
				f->pos += f->moveInc;
				f->spriteBody->setPosition(f->pos);		// 同步显示( 位移 )
#endif
			}
			else
			{
#ifdef CC_TARGET_PLATFORM
				// 资源回收
				f->spriteBody->removeFromParentAndCleanup(true);	// 同步显示( 移除 )
				f->spriteBody = nullptr;
#endif

				// 将数据上下文从容器交换移除
				fishs[fishs.dataLen - 1]->indexAtContainer = f->indexAtContainer;
				fishs.SwapRemoveAt(f->indexAtContainer);
			}
		}
	}

	// 处理开火
	inline void Shoot()
	{
		for (auto& plr : *fishScene->players)
		{
			// 解包容器变量
			PKG::Client_CatchFish::Fire_p o;

			// 一次性处理光
			while (plr->peer->recvFires.TryPop(o))
			{
				// 扣玩家钱，判断钱够不够, 不够就忽略这个消息	// todo: 日志，调公式
				if (plr->coin < o->coin) return;
				plr->coin -= o->coin;

				// 压入帧事件集 转发出去, 合法性判断，cd 判断先不加
				PKG::CatchFish::Events::Fire_p f;
				f.MPCreate(mempool);
				f->moveInc = o->moveInc;
				f->bulletSerialNumber = o->bulletSerialNumber;
				f->coin = o->coin;
				f->frameNumber = o->frameNumber;
				f->sitIndex = plr->sitIndex;

				fishScene->frameEvents->fires->Add(std::move(f));

				// 创建子弹上下文
				PKG::CatchFish::Bullet_p bullet;
				bullet.MPCreate(mempool);
				bullet->indexAtContainer = (int)plr->bullets->dataLen;
				bullet->bornFrameNumber = o->frameNumber;
				bullet->serialNumber = o->bulletSerialNumber;
				bullet->coin = o->coin;
				bullet->pos = xx::Pos{ 1280 / 2, 0 };
				bullet->moveInc = o->moveInc;
				plr->bullets->Add(std::move(bullet));

				// todo: 存储子弹创建时间？以便于清除存在太长时间的子弹（可能因掉线而变野，或是掉线时清除子弹，回收金币?）
			}
		}
	}

	// 处理命中
	inline void MoveBullets()
	{
		for (auto& plr : *fishScene->players)
		{
			// 解包容器变量
			PKG::Client_CatchFish::Hit_p o;

			// 一次性处理光
			while (plr->peer->recvHits.TryPop(o))
			{
				auto& bullets = *plr->bullets;

				// 先粗暴判断鱼和子弹在不在，在就算命中并下发( 压入帧事件 )
				PKG::CatchFish::Bullet* b_ = nullptr;
				for (auto& b : bullets)
				{
					if (b->serialNumber == o->bulletSerialNumber)
					{
						b_ = b.pointer;
						break;
					}
				}

				PKG::CatchFish::Fish* f_ = nullptr;
				for (auto& f : *fishScene->fishs)
				{
					if (f->serialNumber == o->fishSerialNumber)
					{
						f_ = f.pointer;
						break;
					}
				}
				if (b_ && f_)
				{
					// todo: 调用杀鱼公式

					// 这里假设一定打死鱼
					PKG::CatchFish::Events::FishDead_p fd;
					fd.MPCreate(mempool);
					fd->coin = b_->coin * f_->cfg->coin;
					fd->fishSerialNumber = f_->serialNumber;
					fd->sitIndex = plr->sitIndex;
					plr->coin += fd->coin;
					fishScene->frameEvents->fishDeads->Add(std::move(fd));

					// 将子弹干掉
					bullets[bullets.dataLen - 1]->indexAtContainer = b_->indexAtContainer;
					bullets.SwapRemoveAt(b_->indexAtContainer);
				}
			}
		}
	}

	// 下发当前帧的网络消息
	inline void SendFramePackages()
	{
		// 广播给本帧进入的玩家 FullSync	
		// 广播给所有玩家( 除了本帧进入的 ) frameEvents
		// todo: 先序列化 fishScene, frameEvents 再批量发送序列化后的结果
		for (auto& p : *fishScene->players)
		{
			if (fishScene->frameEvents->joins->Exists([&](auto& j) { return j->sitIndex == p->sitIndex; }))
			{
				PKG::CatchFish_Client::FullSync_p fs;
				fs.MPCreate(mempool);
				fs->scene = this->fishScene;
				p->peer->Send(fs);
			}
			else
			{
				// 检查 this->fishScene->frameEvents 的各项是否有填充内容，空的就不用发了. 
				auto& fe = this->fishScene->frameEvents;
				if (fe->fireBegins->dataLen
					|| fe->fireChangeAngles->dataLen
					|| fe->fireEnds->dataLen
					|| fe->fires->dataLen
					|| fe->fishDeads->dataLen
					|| fe->joins->dataLen
					|| fe->leaves->dataLen
					|| (this->fishScene->frameNumber % 16 == 0))	// 每 16 帧下发一个包, 方便客户端及时校准
				{
					p->peer->Send(fe);
				}
			}
		}

		ClearFrameEvents();
	}

	// 网络初始化
	inline void NetInit()
	{
		// 初始化网络监听部分
		listener.Bind("0.0.0.0", 12345);
		listener.Listen();
		listener.OnCreatePeer = [this] { return mempool->Create<ClientPeer>(this); };
		listener.OnAccept = [this](xx::UvTcpPeer *p_)
		{
			std::cout << p_->ip() << " accepted." << std::endl;

			// 未绑定到上下文之前，作为匿名连接，先接收 Join 指令
			p_->OnReceivePackage = [this, p_](xx::BBuffer& bb)
			{
				//// debug: 打印收到的数据
				//std::cout << bb << std::endl;

				// 恢复 peer 原本的类型
				auto p = (ClientPeer*)p_;

				// 解包容器变量
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

				// debug: 打印收到的数据
				std::cout << o_ << std::endl;

				// 收到 join 包
				if (o_->memHeader().typeId == xx::TypeId_v<PKG::Client_CatchFish::Join>)
				{
					// 硬转还原出真实类型备用
					auto& o = *(PKG::Client_CatchFish::Join_p*)&o_;

					// 前置检查: peer 得是没有绑定过的
					if (p->player)
					{
						p->Release();
						return;
					}

					// 如果玩家已在线( name已存在 ), 就禁止登陆，踢掉
					if (fishScene->players->Exists([&o](PKG::CatchFish::Player_p& plr)
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
					for (size_t i = 0; i < fishScene->players->dataLen; ++i)
					{
						freeSits.Remove(fishScene->players->At(i)->sitIndex);
					}

					// 没有空位了( 理论上讲如果是大厅分配过来的，不应该出现这种情况 )
					if (freeSits.dataLen == 0)
					{
						p->Release();
						return;
					}

					// 创建玩家
					PKG::CatchFish::Player_p plr;
					plr.MPCreate(mempool);
					plr->indexAtContainer = (int)fishScene->players->dataLen;
					plr->bullets.MPCreate(mempool);
					plr->coin = cfg->playerInitCoin;
					plr->name = std::move(o->username);
					plr->sitIndex = freeSits[0];

					// plr 绑定 peer
					plr->peer = p;
					p->player = plr.pointer;
					std::cout << "join: plr = " << plr.pointer << ", p = " << p << std::endl;

					// 向场景每帧包合并变量压入 Join 事件
					PKG::CatchFish::Events::JoinPlayer_p j;
					j.MPCreate(mempool);
					j->name = plr->name;
					j->sitIndex = plr->sitIndex;
					j->coin = plr->coin;
					fishScene->frameEvents->joins->Add(std::move(j));

					// 最后将 player 移入队列
					fishScene->players->Add(std::move(plr));
				}
				// 其他包全压队列, 以便于在正确的生命周期中处理
				else
				{
					// 前置检查：需要是 plr 绑定 peer 过的
					if (!p->player)
					{
						p->Release();
						return;
					}

					// 收到开火
					if (o_->memHeader().typeId == xx::TypeId_v<PKG::Client_CatchFish::Fire>)
					{
						// 硬转还原出真实类型备用
						auto& o = *(PKG::Client_CatchFish::Fire_p*)&o_;

						// 移进队列
						p->recvFires.Push(std::move(o));
					}
					// 收到命中
					else if (o_->memHeader().typeId == xx::TypeId_v<PKG::Client_CatchFish::Hit>)
					{
						// 硬转还原出真实类型备用
						auto& o = *(PKG::Client_CatchFish::Hit_p*)&o_;

						// 移进队列
						p->recvHits.Push(std::move(o));
					}
				}


			};

			p_->OnDispose = [this, p_]
			{
				std::cout << p_->ip() << " disconnected." << std::endl;

				// 恢复 peer 原本的类型
				auto p = (ClientPeer*)p_;

				// 前置检查: 看是否已 join
				if (p->player)
				{
					// 压 Leave 事件, 清除指定玩家在游戏内的所有数据
					PKG::CatchFish::Events::LeavePlayer_p lp;
					lp.MPCreate(mempool);
					lp->sitIndex = p->player->sitIndex;
					fishScene->frameEvents->leaves->Add(std::move(lp));

					// 从队列清除 player, 随着 player 被 Release, player 中的 ctx( 当前类 ) 也被析构，故下面就不写其他代码了
					auto& players = *fishScene->players;
					players[players.dataLen - 1]->indexAtContainer = p->player->indexAtContainer;
					players.SwapRemoveAt(p->player->indexAtContainer);
				}
				else
				{
					// 不必理会了
				}

				// 清除 p 的事件绑定( 这步可能导致 lambda 上下文立即回收，故下面不应该有 this, p_ 等捕获对象的访问
				p->OnReceivePackage = nullptr;
				p->OnDispose = nullptr;
			};
		};
	}

	// 场景初始化
	inline void SceneInit()
	{
		// 初始化鱼场景成员变量( 包括部分子的子成员 )
		fishScene.MPCreate(mempool);
		fishScene->frameNumber = 0;
		fishScene->rnd.MPCreate(mempool, 123);	// 先随便来个 seed
		fishScene->players.MPCreate(mempool);
		fishScene->fishs.MPCreate(mempool);
		fishScene->frameEvents.MPCreate(mempool);
		fishScene->frameEvents->frameNumber = fishScene->frameNumber;
		fishScene->frameEvents->leaves.MPCreate(mempool);
		fishScene->frameEvents->joins.MPCreate(mempool);
		fishScene->frameEvents->fishDeads.MPCreate(mempool);
		fishScene->frameEvents->fires.MPCreate(mempool);
		fishScene->frameEvents->fireEnds.MPCreate(mempool);
		fishScene->frameEvents->fireBegins.MPCreate(mempool);
		fishScene->frameEvents->fireChangeAngles.MPCreate(mempool);
	}


	ServiceCatchFish(xx::UvLoop& loop)
		: Object(loop.mempool)
		, loop(loop)
		, listener(loop)
		, timer(loop, 0, 1000 / 60)	// 每秒 60 帧	// todo: 这个似乎无法稳定在每秒 60 帧. 需要考虑每毫秒都触发的时间蓄水池稳帧方案
		, freeSits(loop.mempool)
	{
		NetInit();
		SceneInit();
		FillFishCfg(mempool, cfg);


		// 相当于客户端的 update
		timer.OnFire = [this]
		{
			// 生命周期序列:  处理玩家进入, 玩家离开, 生成鱼, 发射, 移动子弹, 处理开火, 处理命中, 移动鱼, 发包, 帧号++
			MakeFishs();
			Shoot();
			MoveBullets();
			MoveFishs();
			SendFramePackages();
			++fishScene->frameNumber;
		};
	}

	// 在每帧的结尾处清除累计事件数据
	inline void ClearFrameEvents()
	{
		fishScene->frameEvents->frameNumber = fishScene->frameNumber;
		fishScene->frameEvents->leaves.MPCreate(mempool);
		fishScene->frameEvents->joins.MPCreate(mempool);
		fishScene->frameEvents->fishDeads.MPCreate(mempool);
		fishScene->frameEvents->fires.MPCreate(mempool);
		fishScene->frameEvents->fireEnds.MPCreate(mempool);
		fishScene->frameEvents->fireBegins.MPCreate(mempool);
		fishScene->frameEvents->fireChangeAngles.MPCreate(mempool);
	}

	~ServiceCatchFish()
	{
	}
};

ClientPeer::ClientPeer(ServiceCatchFish * service)
	: xx::UvTcpPeer(service->listener)
	, recvFires(service->mempool)
	, recvHits(service->mempool)
{
}

int main()
{
	// typeid 映射
	xx::MemPool::RegisterInternal();
	PKG::AllTypesRegister();

	// 创建基础内存池
	xx::MemPool mp;
	xx::UvLoop loop(&mp);
	loop.InitRpcManager();
	ServiceCatchFish service(loop);
	std::cout << "CatchFish service runing ... port = 12345" << std::endl;
	loop.Run();
	return 0;
}
