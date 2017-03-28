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
	cfg_alertInterval = 20;								// 主动警戒扫描每 1 秒发生一次
	cfg_alertDistance = 10;								// 警戒距离 10 米
	cfg_moveTimespan = 20;								// 每次移动都坚持 1 秒
	cfg_moveInterval = 20;								// 移动之后休息 1 秒
	cfg_moveBackInterval = 5;							// 随机移动 5 次来 1 次出生点方向的移动
	cfg_traceMaxDistance = 30;							// 追杀到 30 米时不再追杀
	cfg_traceMaxTimespan = 400;							// 追杀 20 秒后不再追杀
	cfg_moveSpeed = 0.2f;								// 每帧移动 0.2 米, 每秒移动 4 米
	cfg_radius = 0.5f;									// 怪投影到地面的圆半径 0.5 米
	cfg_traceKeepDistanceRange = { 5.0f, 15.0f };		// 追杀时保持距离 5 ~ 15 米
	cfg_enableTraceKeepDistanceRange = true;			// 启用保持距离

	// 模拟 根据配置 创建技能
	scene().Create<SkillFar>(this);

	// 根据遍历技能施放范围 修正 保持距离. 
	assert(cfg_traceKeepDistanceRange.f <= cfg_traceKeepDistanceRange.t);
	for (auto& skill : *this->skills)
	{
		if (skill->cfg_distanceRange.f > cfg_traceKeepDistanceRange.f)
		{
			cfg_traceKeepDistanceRange.f = skill->cfg_distanceRange.f;
		}
	}
	// 最小值修正: 为确保玩家和怪的模型尽量不要重叠显示, 最小保持距离即为怪的半径
	if (cfg_traceKeepDistanceRange.f < cfg_radius) cfg_traceKeepDistanceRange.f = cfg_radius;
	if (cfg_traceKeepDistanceRange.t < cfg_radius) cfg_traceKeepDistanceRange.t = cfg_radius;

	// 填充各种计算 cache 变量
	cfg_moveSpeedPow2 = cfg_moveSpeed * cfg_moveSpeed;
	cfg_alertDistancePow2 = cfg_alertDistance * cfg_alertDistance;
	cfg_traceMaxDistancePow2 = cfg_traceMaxDistance * cfg_traceMaxDistance;
	cfg_traceKeepDistanceRangePow2 = cfg_traceKeepDistanceRange * cfg_traceKeepDistanceRange;

	// 初始化运行时变量
	moveSpeed = 0;										// 速度为 0 表示休息
	moveAngle = (uint8_t)scene().NextInteger(0, 256);	// 随机角度

	xy = { (float)scene().NextDouble(0, 100), (float)scene().NextDouble(0, 100) };
	bornXY = xy;
	originalXY = xy;

	hp = 100;


	// 模拟 根据配置 载入初始AI
	SetFSM(fsmAI);

	// 载入 LUA 版初始 AI
	//SetFSM(CreateFSM<FSMLua>(this, "monster1.lua"));
}


// 未配有保持距离的近战怪
Monster2::Monster2() : MonsterBase()
{
	// 模拟 载入配置
	cfg_alertInterval = 20;								// 主动警戒扫描每 1 秒发生一次
	cfg_alertDistance = 10;								// 警戒距离 10 米
	cfg_moveTimespan = 20;								// 每次移动都坚持 1 秒
	cfg_moveInterval = 20;								// 移动之后休息 1 秒
	cfg_moveBackInterval = 5;							// 随机移动 5 次来 1 次出生点方向的移动
	cfg_traceMaxDistance = 30;							// 追杀到 30 米时不再追杀
	cfg_traceMaxTimespan = 400;							// 追杀 20 秒后不再追杀
	cfg_moveSpeed = 0.1f;								// 每帧移动 0.1 米, 每秒移动 2 米
	cfg_radius = 0.3f;									// 怪投影到地面的圆半径 0.3 米
	cfg_traceKeepDistanceRange = { 0.0f, 0.0f };		// 不启用保持距离
	cfg_enableTraceKeepDistanceRange = false;			// 不启用保持距离


	// 模拟 根据配置 创建技能
	scene().Create<SkillNear>(this);

	// 根据遍历技能施放范围 计算 保持距离. 
	assert(cfg_traceKeepDistanceRange.f <= cfg_traceKeepDistanceRange.t);
	for (auto& skill : *this->skills)
	{
		if (skill->cfg_distanceRange.f > cfg_traceKeepDistanceRange.f)
		{
			cfg_traceKeepDistanceRange.f = skill->cfg_distanceRange.f;
		}
	}
	// 最小值修正: 为确保玩家和怪的模型尽量不要重叠显示, 最小保持距离即为怪的半径
	if (cfg_traceKeepDistanceRange.f < cfg_radius) cfg_traceKeepDistanceRange.f = cfg_radius;
	if (cfg_traceKeepDistanceRange.t < cfg_radius) cfg_traceKeepDistanceRange.t = cfg_radius;

	// 填充各种计算 cache 变量
	cfg_moveSpeedPow2 = cfg_moveSpeed * cfg_moveSpeed;
	cfg_alertDistancePow2 = cfg_alertDistance * cfg_alertDistance;
	cfg_traceMaxDistancePow2 = cfg_traceMaxDistance * cfg_traceMaxDistance;
	cfg_traceKeepDistanceRangePow2 = cfg_traceKeepDistanceRange * cfg_traceKeepDistanceRange;

	// 初始化运行时变量

	moveSpeed = 0;										// 速度为 0 表示休息
	moveAngle = (uint8_t)scene().NextInteger(0, 256);	// 随机角度

	xy = { (float)scene().NextDouble(0, 100), (float)scene().NextDouble(0, 100) };
	bornXY = xy;
	originalXY = xy;

	hp = 100;

	// 模拟 根据配置 载入初始AI
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
		if (d <= cfg_alertDistance && minDistance > d)
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

MonsterFSM_AI::MonsterFSM_AI(SceneObjBase* owner) : FSMBase(owner)
{
	auto& c = *(MonsterBase*)owner;
	moveTicks = scene().ticks + c.cfg_moveInterval;		// 出生后先休息
}

// 试警戒一把( 如果 cd 到了的话 ). 顺便把挨打检测也做在这
bool MonsterFSM_AI::Alert()
{
	auto& c = *(MonsterBase*)owner;
	auto& s = scene();
	if (c.target) return true;
	if (alertInterval <= s.ticks)
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
		}

		// 如果 idle 的时间到了, 就切到 move
		if (moveTicks <= s.ticks) CORO_YIELDTO(1);

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

		bool needSync = false;
		// 算增量
		if (c.moveSpeed != speed || c.moveAngle != angle)
		{
			c.moveSpeed = speed;
			c.moveAngle = angle;
			xyInc = xyMath.GetXyInc(c.moveAngle) * c.moveSpeed;

			needSync = true;
		}

		// 移动		// todo: 需要检测 xy + inc 之后的点是否合法. 需要补反转 xyInc 代码
		c.xy.Add(xyInc);				// todo: 如果 moveCount == 0 期间移动被阻挡, 则启用导航功能返回出生点

		if (needSync)
		{
			// todo: send move msg to client ?
		}


		// 如果 move 的时间到了, 就切到 idle
		if (moveTicks <= s.ticks) CORO_YIELDTO(0);

		// 循环
		CORO_YIELDTO(1);
	}
	CORO_(2);													// 追杀( 先做无视地型无脑向目标前进的版本 )
	{
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
				castStunTicks = s.ticks + skill->cfg_castStunTimespan;
				CORO_GOTO(4);
			}
		}

		// 先将计算结果存到下面的临时变量, 以便于最后判断变化 & 同步到客户端
		uint8_t angle = 0;
		auto speed = 0.0f;

		// 保持距离

		// 先算出与目标的距离pow2备用
		auto tarDistPow2 = xyMath.GetDistancePow2(c.target->xy - c.xy);

		// 判断 距离 是否已 小于 最小保持距离, 反向移动
		if (c.cfg_traceKeepDistanceRangePow2.f > tarDistPow2)
		{
			angle = xyMath.GetAngle(c.xy - c.target->xy);
			speed = c.cfg_moveSpeed;
		}
		// 判断 距离 是否明显大于 最小保持距离, 移动( 所谓明显大于是指距离大于一帧的移动距离 )
		else if ((c.cfg_enableTraceKeepDistanceRange && c.cfg_traceKeepDistanceRangePow2.t > tarDistPow2)
			|| ((c.cfg_traceKeepDistanceRange.f + c.cfg_moveSpeed) * (c.cfg_traceKeepDistanceRange.f + c.cfg_moveSpeed) < tarDistPow2))
		{
			angle = xyMath.GetAngle(c.target->xy - c.xy);
			speed = c.cfg_moveSpeed;
		}
		// 不需要移动
		else
		{
			angle = xyMath.GetAngle(c.target->xy - c.xy);
			speed = 0.0f;
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
			// 将坐标设为初生点, 跳到休息状态
			c.xy = c.bornXY;
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
		}

		CORO_YIELDTO(3);
		// todo: 超时检测?  
	}
	CORO_(4)															// 技能后僵直
	{
		if (castStunTicks > s.ticks)
		{
			CORO_YIELDTO(4);
		}
		else
		{
			CORO_YIELDTO(2);
		}
	}
	CORO_END();
	return 0;
}
