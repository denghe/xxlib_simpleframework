MonsterBase::MonsterBase()
	: skills(scene())
	//, fsmConds(scene())
	//, fsmIdle(scene(), this)
	//, fsmMove(scene(), this)
	, fsmCast(scene(), this)
	, fsmAI(scene(), this)
	//, fsmAlertCondition(scene(), this)
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
	cfg_traceMaxDistance = 30;							// 追杀到 30 米时不再追杀
	cfg_traceMaxTimespan = 400;							// 追杀 20 秒后不再追杀
	cfg_traceKeepDistanceRange = { 5.0f, 15.0f };		// 追杀时保持距离 5 ~ 15 米
	cfg_moveSpeed = 0.2f;								// 每帧移动 0.2 米, 每秒移动 4 米
	cfg_alertDistancePow2 = cfg_alertDistance * cfg_alertDistance;
	cfg_traceMaxDistancePow2 = cfg_traceMaxDistance * cfg_traceMaxDistance;
	cfg_traceKeepDistanceRangePow2 = cfg_traceKeepDistanceRange * cfg_traceKeepDistanceRange;

	alertInterval = 0;									// 出生后立即检测
	moveTicks = scene().ticks + cfg_moveInterval;		// 出生后先休息
	moveSpeed = 0;										// 速度为 0 表示休息
	moveAngle = (uint8_t)scene().NextInteger(0, 256);	// 随机角度

	xy = { scene().NextDouble(0, 100), scene().NextDouble(0, 100) };
	bornXY = xy;
	originalXY = xy;

	hp = 100;

	// 模拟 根据配置 创建技能
	scene().Create<SkillFar>(this);


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
	cfg_traceMaxDistance = 30;							// 追杀到 30 米时不再追杀
	cfg_traceMaxTimespan = 400;							// 追杀 20 秒后不再追杀
	cfg_traceKeepDistanceRange = { 5.0f, 15.0f };		// 追杀时保持距离 5 ~ 15 米
	cfg_moveSpeed = 0.1f;								// 每帧移动 0.1 米, 每秒移动 2 米
	cfg_alertDistancePow2 = cfg_alertDistance * cfg_alertDistance;
	cfg_traceMaxDistancePow2 = cfg_traceMaxDistance * cfg_traceMaxDistance;
	cfg_traceKeepDistanceRangePow2 = cfg_traceKeepDistanceRange * cfg_traceKeepDistanceRange;

	alertInterval = 0;									// 出生后立即检测
	moveTicks = scene().ticks + cfg_moveInterval;		// 出生后先休息
	moveSpeed = 0;										// 速度为 0 表示休息
	moveAngle = (uint8_t)scene().NextInteger(0, 256);	// 随机角度

	xy = { scene().NextDouble(0, 100), scene().NextDouble(0, 100) };
	bornXY = xy;
	originalXY = xy;

	hp = 100;

	// 模拟 根据配置 创建技能
	scene().Create<SkillNear>(this);

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


int MonsterBase::TakeAvaliableSkillId()
{
	// 先判断 gcd. 如果 gcd 中直接返回 -1. 避免因 gcd 导致无脑跳技能
	if (skillsGcd > scene().ticks) return -1;

	// 循环移动 skillCursor, 并检测技能是否可用. 最多移动一圈. 返回第1个发现的可用技能
	for (int i = 0; i < (int)skills->dataLen; ++i)
	{
		skillCursor++;
		if (skillCursor >= (int)skills->dataLen) skillCursor = 0;
		if (skills->At(skillCursor)->Avaliable()) return skillCursor;
	}
	return -1;
}

//xx::MPtr<FSMLua> MonsterBase::LuaCondition(char const* luacode)
//{
//	auto fsm = scene().Create<FSMLua>(this, luacode, false);
//	fsmConds->AddDirect(fsm);
//	return fsm;
//}
//
//template<typename T, typename ... Args>
//xx::MPtr<T> MonsterBase::Condition(Args&&...args)
//{
//	auto fsm = scene().Create<T>(this, std::forward<Args>(args)...);
//	fsmConds->AddDirect(fsm);
//	return fsm;
//}

//void MonsterBase::Idle(int64_t ticks, FSMBase* cond)
//{
//	fsmIdle->Init(ticks, cond);
//	PushFSM(fsmIdle);
//}
//
//void MonsterBase::Move(int xInc, int count, FSMBase* cond)
//{
//	fsmMove->Init(xInc, count, cond);
//	PushFSM(fsmMove);
//}

void MonsterBase::Cast(int skillIndex)
{
	fsmCast->Init(skillIndex);
	PushFSM(fsmCast);
}





MonsterBase& MonsterFSMBase::ctx()
{
	return *(MonsterBase*)owner;
}
MonsterFSMBase::MonsterFSMBase(SceneObjBase* owner) : FSMBase(owner)
{
}





//MonsterFSM_Idle::MonsterFSM_Idle(SceneObjBase* owner) : MonsterFSMBase(owner)
//{
//}
//void MonsterFSM_Idle::Init(int64_t ticks, FSMBase* breakCond)
//{
//	this->sleepToTicks = scene().ticks + ticks;
//	this->breakCond = breakCond;
//}
//int MonsterFSM_Idle::Update()
//{
//	// 时间到或条件的 Update 返回非 0, 弹栈
//	if (scene().ticks >= sleepToTicks
//		|| (breakCond && breakCond->Update())) Pop();
//	return 0;
//}
//
//
//
//
//MonsterFSM_Move::MonsterFSM_Move(SceneObjBase* owner) : MonsterFSMBase(owner)
//{
//}
//void MonsterFSM_Move::Init(int xInc, int count, FSMBase* breakCond)
//{
//	this->xInc = xInc;
//	this->toTicks = scene().ticks + count;
//	this->breakCond = breakCond;
//}
//int MonsterFSM_Move::Update()
//{
//	if (scene().ticks >= toTicks
//		|| (breakCond && breakCond->Update())) Pop();
//	ctx().x += xInc;
//	return 0;
//}




MonsterFSM_Cast::MonsterFSM_Cast(SceneObjBase* owner) : MonsterFSMBase(owner)
{
}
void MonsterFSM_Cast::Init(int skillIndex)
{
	this->skillIndex = skillIndex;
}
int MonsterFSM_Cast::Update()
{
	assert(skillIndex >= 0
		&& (int)ctx().skills->dataLen > skillIndex
		&& ctx().skills->At(skillIndex)->Avaliable());
	ctx().skills->At(skillIndex)->Cast();
	Pop();
	return 0;
}



//MonsterFSM_AlertCondition::MonsterFSM_AlertCondition(SceneObjBase* owner) : MonsterFSMBase(owner) {}
//
//// 有怪进入范围则条件满足, 填充 target 属性后返回非 0 ( 通知使用条件者退出 )
//int MonsterFSM_AlertCondition::Update()
//{
//	assert(!ctx().target);
//	auto tar = ctx().SearchTarget();
//	if (tar)
//	{
//		ctx().target = tar;
//		ctx().originalX = ctx().x;
//		return 1;						// 作为条件使用, 通过 Update 返回值来达到结果告知的目的
//	}
//	return 0;
//}




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

#define CORO_YIELD(n)		\
{							\
	lineNumber = n;			\
	return 0;				\
}

#define CORO_GOTO(n)		\
goto Label##n


//// 先来个 idle / move, 发现怪进入范围就不停追杀接近并持续循环放技能, 直到目标失效时返回原点
//int MonsterFSM_AI::Update()
//{
//	auto& c = ctx();
//	auto& s = scene();
//	CORO_BEGIN();
//	{
//		if (c.target) CORO_GOTO(1);					// 如果有目标( 警戒条件类赋予 ), 切到追杀状态
//		
//		auto v = s.NextInteger(-1, 2);				// 根据随机值 -1, 0, 1 来决定是 idle 还是 move( v 值同时决定移动方向 )
//		if (v == 0)
//		{
//			c.Idle(s.NextInteger(1, 3), c.fsmAlertCondition);		// 压入 Idle 状态机
//		}
//		else
//		{
//			c.Move(v, s.NextInteger(1, 3), c.fsmAlertCondition);	// 压入 Move 状态机
//		}
//		CORO_YIELD(0);								// 下次进入时从 CORO_BEGIN 处开始执行
//	}
//	CORO_(1);
//	{
//		// 如果有目标, 则表明先前的 idle 过程中判定条件达成, 目标被设置, 则追杀( 持续向目标移动, 有技能放时就放技能 )
//		if (c.target)
//		{
//			auto skillid = c.AnyAvaliableSkillId();
//			if (skillid == -1)
//			{
//				int xInc = c.target->x > c.x ? 1 : -1;
//				c.Move(xInc, 3, nullptr);
//			}
//			else
//			{
//				c.Cast(skillid);
//			}
//			CORO_YIELD(1);							// 下次进入时从 CORO_(1) 处开始执行
//		}
//		else
//		{
//			// 如果目标丢失( 下线? 被打死? ), 执行回撤操作( 持续 move 到发现 target 瞬间记录的的坐标 )
//			auto d = c.originalX - c.x;
//			c.Move(d > 0 ? 1 : -1, std::abs(d), nullptr);	// 无条件回撤
//
//			CORO_YIELD(0);							// 下次进入时从 CORO_BEGIN 处开始执行
//		}
//	}
//	CORO_END();
//	return 0;
//}

MonsterFSM_AI::MonsterFSM_AI(SceneObjBase* owner) : MonsterFSMBase(owner) {}

// 严格按照策划案来的 AI 代码
int MonsterFSM_AI::Update()
{
	auto& c = ctx();
	auto& s = scene();
	CORO_BEGIN();
	{
		assert(c.moveSpeed == 0);
		if (c.alertInterval <= s.ticks)
		{
			//	assert(!ctx().target);
			//	auto tar = ctx().SearchTarget();
			//	if (tar)
			//	{
			//		ctx().target = tar;
			//		ctx().originalX = ctx().x;
			//		return 1;						// 作为条件使用, 通过 Update 返回值来达到结果告知的目的
			//	}
			//	return 0;

		}
	}
	CORO_(1);
	{
		assert(c.moveSpeed != 0);
	}
	CORO_END();
	return 0;
}