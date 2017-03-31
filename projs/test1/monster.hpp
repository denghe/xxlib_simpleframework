MonsterBase::MonsterBase()
	: skills(scene())
	, fsmAI(scene(), this)
{
	auto& ms = *scene().monsters;
	sceneContainerIndex = (uint32_t)ms.dataLen;
	ms.Add(this);
}

MonsterBase::~MonsterBase()
{
	// Create 时如果 throw exception 可能导致这个情况发生
	if (sceneContainerIndex < 0) return;

	// 从 所在容器 交换移除, 同步下标
	auto& ms = *scene().monsters;
	auto& buf = ms.buf;
	auto& dataLen = ms.dataLen;
	--dataLen;
	if (dataLen == this->sceneContainerIndex) return;	// last one
	else
	{
		buf[this->sceneContainerIndex] = buf[dataLen];
		buf[dataLen]->sceneContainerIndex = this->sceneContainerIndex;
	}
}


// 配有保持距离的远程怪
Monster1::Monster1() : MonsterBase()
{
	// 模拟 载入配置
	cfg_alertInterval = 10;
	cfg_alertDistance = 20;
	cfg_moveTimespan = 10;
	cfg_moveInterval = 5;
	cfg_moveBackInterval = 5;
	cfg_traceMaxDistance = 30;
	cfg_traceMaxTimespan = 400;
	cfg_moveSpeed = 0.3f;
	cfg_radius = 1.0f;
	cfg_traceKeepDistanceRange = { 5.0f, 15.0f };
	cfg_enableTraceKeepDistanceRange = true;
	cfg_isInitiative = true;

	// 模拟 根据配置 创建技能
	scene().Create<SkillFar>(this);

	// 填充各种计算 cache 变量
	cfg_moveSpeedPow2 = cfg_moveSpeed * cfg_moveSpeed;
	cfg_alertDistancePow2 = cfg_alertDistance * cfg_alertDistance;
	cfg_traceMaxDistancePow2 = cfg_traceMaxDistance * cfg_traceMaxDistance;

	// 初始化运行时变量
	moveSpeed = 0;										// 速度为 0 表示休息
	moveAngle = (uint8_t)scene().NextInteger(0, 256);	// 随机角度

	xy = { (float)scene().NextDouble(0, scene().mapSize.w), (float)scene().NextDouble(0, scene().mapSize.h) };
	bornXY = xy;
	originalXY = xy;

	hp = 100;


	// 模拟 根据配置 载入初始AI
	fsmAI->Init();
	SetFSM(fsmAI);

	// 载入 LUA 版初始 AI
	//SetFSM(CreateFSM<FSMLua>(this, "monster1.lua"));
}


// 未配有保持距离的近战怪
Monster2::Monster2() : MonsterBase()
{
	// 模拟 载入配置
	cfg_alertInterval = 10;
	cfg_alertDistance = 20;
	cfg_moveTimespan = 10;
	cfg_moveInterval = 5;
	cfg_moveBackInterval = 5;
	cfg_traceMaxDistance = 30;
	cfg_traceMaxTimespan = 400;
	cfg_moveSpeed = 0.4f;
	cfg_radius = 0.7f;
	cfg_traceKeepDistanceRange = { 0, 0 };
	cfg_enableTraceKeepDistanceRange = false;
	cfg_isInitiative = true;


	// 模拟 根据配置 创建技能
	scene().Create<SkillNear>(this);

	// 填充各种计算 cache 变量
	cfg_moveSpeedPow2 = cfg_moveSpeed * cfg_moveSpeed;
	cfg_alertDistancePow2 = cfg_alertDistance * cfg_alertDistance;
	cfg_traceMaxDistancePow2 = cfg_traceMaxDistance * cfg_traceMaxDistance;

	// 初始化运行时变量
	moveSpeed = 0;										// 速度为 0 表示休息
	moveAngle = (uint8_t)scene().NextInteger(0, 256);	// 随机角度

	xy = { (float)scene().NextDouble(0, scene().mapSize.w), (float)scene().NextDouble(0, scene().mapSize.h) };
	bornXY = xy;
	originalXY = xy;

	hp = 100;

	// 模拟 根据配置 载入初始AI
	fsmAI->Init();
	SetFSM(fsmAI);

	// 载入 LUA 版初始 AI
	//SetFSM(CreateFSM<FSMLua>(this, "monster2.lua"));
}



float MonsterBase::DistancePow2(MonsterBase* other)
{
	return xyMath.GetDistancePow2(this->xy - other->xy);
}

xx::MPtr<MonsterBase> MonsterBase::SearchTarget()
{
	MonsterBase* r = nullptr;
	auto minDistance = std::numeric_limits<float>::max();
	for (auto& m : *scene().monsters)
	{
		if (m == this) continue;
		auto d = DistancePow2(m);
		if (d <= cfg_alertDistancePow2 && minDistance > d)
		{
			minDistance = d;
			r = m;
		}
	}
	return r;
}

void MonsterBase::Hurt(MonsterBase* attacker)
{
	// 当前服务于 AI 代码, 只针对首个发起攻击的目标做 target 记录
	if (target) return;
	target = attacker;
}

void MonsterBase::SetTarget(MonsterBase* target)
{
	this->target = target;
}


xx::MPtr<SkillBase> MonsterBase::TakeAvaliableSkill()
{
	// 先判断 gcd. 如果 gcd 中直接返回 -1. 避免因 gcd 导致无脑跳技能
	if (skillsGcd > scene().ticks) return nullptr;

	// 循环移动 skillCursor, 并检测技能是否可用. 最多移动一圈. 返回第1个发现的可用技能
	for (int i = 0; i < (int)skills->dataLen; ++i)
	{
		skillCursor++;
		if (skillCursor >= (int)skills->dataLen) skillCursor = 0;
		auto& skill = skills->At(skillCursor);
		if (skill->Avaliable()) return skill;
	}
	return nullptr;
}




void MonsterBase::ToString(xx::String &str) const
{
	str.Append("{ id : ", pureVersionNumber(), ", hp : ", hp, ", xy : [ ");
	str.Reserve(str.dataLen + 100);
	str.dataLen += std::sprintf(str.buf + str.dataLen, "%.2f, %.2f", xy.x, xy.y);
	str.Append(" ] }");
}





MonsterFSM_AI::MonsterFSM_AI(SceneObjBase* owner) : FSMBase(owner) {}

void MonsterFSM_AI::Init()
{
	auto& c = *(MonsterBase*)owner;
	moveTicks = scene().ticks + c.cfg_moveInterval;		// 出生后先休息
}

// 试警戒一把( 如果为主动怪, 且 cd 到了的话 ). 因 idle / move 状态挨打而设置目标也返回 true
bool MonsterFSM_AI::Alert()
{
	auto& c = *(MonsterBase*)owner;
	auto& s = scene();
	if (c.target) return true;
	if (c.cfg_isInitiative && alertInterval <= s.ticks)
	{
		auto tar = c.SearchTarget();
		if (tar)
		{
			c.target = tar;
			c.originalXY = c.xy;
			return true;
		}
	}
	return false;
}




#define CORO_BEGIN()		\
switch (lineNumber)			\
{							\
case 0:						\
Label0:						\
{

#define CORO_(n)			\
}							\
lineNumber = n;				\
return 0;					\
case n:						\
Label##n:					\
{

#define CORO_END()			\
}							\
}

#define CORO_YIELDTO(n)		\
{							\
	lineNumber = n;			\
	return 0;				\
}

#define CORO_GOTO(n)		\
goto Label##n



// 严格按照策划案来的 AI 代码. 整合了 Idle, Move, Track, TurnBack 等状态
int MonsterFSM_AI::Update()
{
	auto& c = *(MonsterBase*)owner;
	auto& s = scene();
	CORO_BEGIN();														// idle
	{
		// 警戒一把. 如果成功( 选中 target ), 切到追杀代码
		if (Alert())
		{
			// 记录开始追杀的时间 for 超时检查
			traceTicks = s.ticks + c.cfg_traceMaxTimespan;
			CORO_GOTO(2);
		}

		// 判断要不要发数据
		if (c.moveSpeed != 0.0f)
		{
			c.moveSpeed = 0.0f;
			// todo: send set pos msg to client ?
			auto& str = *s.err;	// 拿来临时拼接发送内容
			str.AppendFormat("mov {0} {1} {2} {3} {4} {5} {6} {7}", c.pureVersionNumber(), c.xy.x, 0, c.xy.y, c.cfg_radius, 0, 0, 0);
			s.udp->SetAddress("10.1.1.99", 6066);
			s.udp->Send(str.buf, str.dataLen);
			str.Clear();
			str.Append("act ", c.pureVersionNumber(), " idle");
			s.udp->Send(str.buf, str.dataLen);
			str.Clear();
		}

		// 如果 idle 的时间到了, 就切到 move
		if (moveTicks <= s.ticks)
		{
			moveTicks = s.ticks + c.cfg_moveTimespan;
			CORO_YIELDTO(1);
		}

		// 循环
		CORO_YIELDTO(0);
	}
	CORO_(1);															// 随机移动
	{
		// 警戒一把. 如果成功( 选中 target ), 切到追杀代码
		if (Alert())
		{
			// 记录开始追杀的时间 for 超时检查
			traceTicks = s.ticks + c.cfg_traceMaxTimespan;
			CORO_GOTO(2);
		}

		bool needSync = false;

		// 刚从 idle 切过来? 确定未来 moveTicks 时间内的移动方针
		if (c.moveSpeed == 0.0f)
		{
			auto speed = c.cfg_moveSpeed;
			uint8_t angle;

			if (++moveCount >= c.cfg_moveBackInterval)
			{
				// 每 cfg_moveBackInterval 次移动之后, 总有一次移动方向是正对出生点的, 以确保怪不会随机跑太远
				moveCount = 0;
				angle = xyMath.GetAngle(c.bornXY - c.xy);
			}
			else
			{
				// 随机出前进角度
				angle = (uint8_t)s.NextInteger(0, 256);
			}

			// 算增量
			if (c.moveSpeed != speed || c.moveAngle != angle)
			{
				c.moveSpeed = speed;
				c.moveAngle = angle;
				xyInc = xyMath.GetXyInc(c.moveAngle) * c.moveSpeed;

				needSync = true;
			}
		}

		// 移动		// todo: 需要检测 xy + inc 之后的点是否合法. 需要补反转 xyInc 代码
		c.xy.Add(xyInc);				// todo: 如果 moveCount == 0 期间移动被阻挡, 则启用导航功能返回出生点

		if (needSync)
		{
			// todo: send move msg to client ?
			auto& str = *s.err;	// 拿来临时拼接发送内容
			str.AppendFormat("mov {0} {1} {2} {3} {4} {5} {6} {7}", c.pureVersionNumber(), c.xy.x, 0, c.xy.y, c.cfg_radius, xyInc.x * 20, 0, xyInc.y * 20);
			s.udp->SetAddress("10.1.1.99", 6066);
			s.udp->Send(str.buf, str.dataLen);
			str.Clear();
			str.Append("act ", c.pureVersionNumber(), " move");
			s.udp->Send(str.buf, str.dataLen);
			str.Clear();
		}

		// 如果 move 的时间到了, 就切到 idle
		if (moveTicks <= s.ticks)
		{
			moveTicks = s.ticks + c.cfg_moveInterval;
			CORO_YIELDTO(0);
		}

		// 循环
		CORO_YIELDTO(1);
	}
	CORO_(2);													// 追杀( 先做无视地型无脑向目标前进的版本 )
	{
		// 技能后僵直
		if (castStunTicks > s.ticks)
		{
			CORO_YIELDTO(2);
		}

		// 目标失效, 追出最远距离限定, 超时, 都将返回出生点( 跳到返回状态 )
		if (!c.target
			|| traceTicks <= s.ticks
			|| xyMath.GetDistancePow2(c.bornXY - c.xy) > c.cfg_traceMaxDistancePow2)
		{
			CORO_GOTO(3);
		}

		// 判断是否有可用技能. 有就用
		if (auto skill = c.TakeAvaliableSkill())
		{
			skill->Cast();
			// send msg to client?

			if (skill->cfg_castStunTimespan)
			{
				if (c.moveSpeed != 0.0f)
				{
					auto& str = *s.err;	// 拿来临时拼接发送内容
					str.AppendFormat("mov {0} {1} {2} {3} {4} {5} {6} {7}", c.pureVersionNumber(), c.xy.x, 0, c.xy.y, c.cfg_radius, 0, 0, 0);
					s.udp->SetAddress("10.1.1.99", 6066);
					s.udp->Send(str.buf, str.dataLen);
					str.Clear();
					str.Append("act ", c.pureVersionNumber(), " cast_stun");
					s.udp->Send(str.buf, str.dataLen);
					str.Clear();
				}
				c.moveSpeed = 0.0f;

				castStunTicks = s.ticks + skill->cfg_castStunTimespan;
				CORO_YIELDTO(2);
			}
		}

		// 先将计算结果存到下面的临时变量, 以便于最后判断变化 & 同步到客户端
		uint8_t angle = 0;
		auto speed = 0.0f;


		// 先算出与目标的 中心点距离pow2 备用
		auto tarDistPow2 = xyMath.GetDistancePow2(c.target->xy - c.xy);

		// 远近距离 & pow2 预声明
		float rf, rt, rfPow2, rtPow2;
		if (c.cfg_enableTraceKeepDistanceRange)						// 保持距离
		{
			// 为消除抖动, 距离差如果小于帧移动距离, 则不再移动
			rf = c.cfg_traceKeepDistanceRange.f + c.target->cfg_radius + c.cfg_moveSpeed;
			rt = c.cfg_traceKeepDistanceRange.t + c.target->cfg_radius - c.cfg_moveSpeed;
			if (rt < rf) rt = rf + c.cfg_moveSpeed * 4;				// 抖动容差
			rfPow2 = rf * rf;
			rtPow2 = rt * rt;
		}
		else
		{
			rf = c.cfg_radius + c.target->cfg_radius + c.cfg_moveSpeed;
			rt = c.cfg_radius + c.target->cfg_radius + c.cfg_moveSpeed * 3;
			rfPow2 = rf * rf;
			rtPow2 = rt * rt;
		}

		if (rfPow2 <= tarDistPow2 && tarDistPow2 <= rtPow2)		// 范围内, 不需要移动
		{
			angle = xyMath.GetAngle(c.target->xy - c.xy);
			speed = 0.0f;
		}
		else if (rfPow2 >= tarDistPow2)							// 离太近, 后退
		{
			angle = xyMath.GetAngle(c.xy - c.target->xy);
			speed = c.cfg_moveSpeed;
		}
		else													// 离太远, 接近
		{
			angle = xyMath.GetAngle(c.target->xy - c.xy);
			speed = c.cfg_moveSpeed;
		}

		bool needSync = false;
		// 算增量
		if (c.moveSpeed != speed || c.moveAngle != angle)
		{
			c.moveSpeed = speed;
			c.moveAngle = angle;
			xyInc = xyMath.GetXyInc(angle) * speed;

			needSync = true;
		}

		// 移动					
		c.xy.Add(xyInc);

		if (needSync)
		{
			// todo: send move msg to client ?
			auto& str = *s.err;	// 拿来临时拼接发送内容
			str.AppendFormat("mov {0} {1} {2} {3} {4} {5} {6} {7}", c.pureVersionNumber(), c.xy.x, 0, c.xy.y, c.cfg_radius, xyInc.x * 20, 0, xyInc.y * 20);
			s.udp->SetAddress("10.1.1.99", 6066);
			s.udp->Send(str.buf, str.dataLen);
			str.Clear();
			str.Append("act ", c.pureVersionNumber(), " trace");
			s.udp->Send(str.buf, str.dataLen);
			str.Clear();

		}

		// 循环
		CORO_YIELDTO(2);
	}
	CORO_(3);															// 返回出生点
	{
		uint8_t angle = xyMath.GetAngle(c.bornXY - c.xy);
		auto speed = c.cfg_moveSpeed;

		// 判断是否已经回到了出生点( 距离小于一帧的移动增量 )
		if (xyMath.GetDistancePow2(c.bornXY - c.xy) <= c.cfg_moveSpeedPow2)
		{
			// 将坐标设为初生点
			c.xy = c.bornXY;

			// 如果在 返回出生点 路上被打, 会导致设置目标, 为避免 Alert 误判, 在这里清一把
			c.target = nullptr;

			// 跳到休息状态
			CORO_GOTO(0);
		}

		bool needSync = false;
		// 算增量
		if (c.moveSpeed != speed || c.moveAngle != angle)
		{
			c.moveSpeed = speed;
			c.moveAngle = angle;
			xyInc = xyMath.GetXyInc(c.moveAngle) * c.cfg_moveSpeed;

			needSync = true;
		}

		// 移动					
		c.xy.Add(xyInc);					// todo: 导航

		if (needSync)
		{
			// todo: send move msg to client ?
			auto& str = *s.err;	// 拿来临时拼接发送内容
			str.AppendFormat("mov {0} {1} {2} {3} {4} {5} {6} {7}", c.pureVersionNumber(), c.xy.x, 0, c.xy.y, c.cfg_radius, xyInc.x * 20, 0, xyInc.y * 20);
			s.udp->SetAddress("10.1.1.99", 6066);
			s.udp->Send(str.buf, str.dataLen);
			str.Clear();
			str.Append("act ", c.pureVersionNumber(), " turnback");
			s.udp->Send(str.buf, str.dataLen);
			str.Clear();
		}

		CORO_YIELDTO(3);
		// todo: 超时检测?  
	}
	CORO_END();
	return 0;
}
