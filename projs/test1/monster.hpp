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
	cfg_moveBackInterval = 5;							// 随机移动 5 次来 1 次出生点方向的移动
	cfg_traceMaxDistance = 30;							// 追杀到 30 米时不再追杀
	cfg_traceMaxTimespan = 400;							// 追杀 20 秒后不再追杀
	cfg_traceKeepDistanceRange = { 5.0f, 15.0f };		// 追杀时保持距离 5 ~ 15 米
	cfg_moveSpeed = 0.2f;								// 每帧移动 0.2 米, 每秒移动 4 米
	cfg_radius = 0.5f;									// 怪投影到地面的圆半径 0.5 米

	cfg_moveSpeedPow2 = cfg_moveSpeed * cfg_moveSpeed;
	cfg_alertDistancePow2 = cfg_alertDistance * cfg_alertDistance;
	cfg_traceMaxDistancePow2 = cfg_traceMaxDistance * cfg_traceMaxDistance;
	cfg_traceKeepDistanceRangePow2 = cfg_traceKeepDistanceRange * cfg_traceKeepDistanceRange;

	moveSpeed = 0;										// 速度为 0 表示休息
	moveAngle = (uint8_t)scene().NextInteger(0, 256);	// 随机角度

	xy = { (float)scene().NextDouble(0, 100), (float)scene().NextDouble(0, 100) };
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
	cfg_moveBackInterval = 5;							// 随机移动 5 次来 1 次出生点方向的移动
	cfg_traceMaxDistance = 30;							// 追杀到 30 米时不再追杀
	cfg_traceMaxTimespan = 400;							// 追杀 20 秒后不再追杀
	cfg_traceKeepDistanceRange = { 5.0f, 15.0f };		// 追杀时保持距离 5 ~ 15 米
	cfg_moveSpeed = 0.1f;								// 每帧移动 0.1 米, 每秒移动 2 米
	cfg_radius = 0.3f;									// 怪投影到地面的圆半径 0.3 米

	cfg_moveSpeedPow2 = cfg_moveSpeed * cfg_moveSpeed;
	cfg_alertDistancePow2 = cfg_alertDistance * cfg_alertDistance;
	cfg_traceMaxDistancePow2 = cfg_traceMaxDistance * cfg_traceMaxDistance;
	cfg_traceKeepDistanceRangePow2 = cfg_traceKeepDistanceRange * cfg_traceKeepDistanceRange;

	moveSpeed = 0;										// 速度为 0 表示休息
	moveAngle = (uint8_t)scene().NextInteger(0, 256);	// 随机角度

	xy = { (float)scene().NextDouble(0, 100), (float)scene().NextDouble(0, 100) };
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

//void MonsterBase::Cast(int skillIndex)
//{
//	fsmCast->Init(skillIndex);
//	PushFSM(fsmCast);
//}





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

#define CORO_YIELDTO(n)		\
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
//		CORO_YIELDTO(0);								// 下次进入时从 CORO_BEGIN 处开始执行
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
//			CORO_YIELDTO(1);							// 下次进入时从 CORO_(1) 处开始执行
//		}
//		else
//		{
//			// 如果目标丢失( 下线? 被打死? ), 执行回撤操作( 持续 move 到发现 target 瞬间记录的的坐标 )
//			auto d = c.originalX - c.x;
//			c.Move(d > 0 ? 1 : -1, std::abs(d), nullptr);	// 无条件回撤
//
//			CORO_YIELDTO(0);							// 下次进入时从 CORO_BEGIN 处开始执行
//		}
//	}
//	CORO_END();
//	return 0;
//}

MonsterFSM_AI::MonsterFSM_AI(SceneObjBase* owner) : MonsterFSMBase(owner)
{
	moveTicks = scene().ticks + ctx().cfg_moveInterval;		// 出生后先休息
}

// 试警戒一把( 如果 cd 到了的话 ). 顺便把挨打检测也做在这
bool MonsterFSM_AI::Alert()
{
	auto& c = ctx();
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


// 严格按照策划案来的 AI 代码. 整合了之前的 Idle, Move, Track, TurnBack 等状态. 只剩技能释放是 push 状态
int MonsterFSM_AI::Update()
{
	auto& c = ctx();
	auto& s = scene();
	CORO_BEGIN();														// idle
	{
		// 警戒一把. 如果成功( 选中 target ), 切到追杀代码
		if (Alert())
		{
			stateIsChanged = true;
			CORO_GOTO(2);
		}

		// 判断是否为持续 idle. 如果是首次, 则需要同步
		if (stateIsChanged)
		{
			// todo: send msg to client ?
			stateIsChanged = false;
		}

		// 如果 idle 的时间到了, 就切到 move
		if (moveTicks <= s.ticks)
		{
			stateIsChanged = true;
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
			stateIsChanged = true;
			CORO_GOTO(2);
		}

		// 判断是否持续相同方向移动中. 如果为刚开始移动, 或改变了方向 速度啥的, 则需要同步
		if (stateIsChanged)
		{
			if (++moveCount >= c.cfg_moveBackInterval)
			{
				// 每 cfg_moveBackInterval 次移动之后, 总有一次移动方向是正对出生点的, 以确保怪不会随机跑太远
				moveCount = 0;
				c.moveAngle = xyMath.GetAngle(c.bornXY - c.xy);
			}
			else
			{
				// 随机出前进角度
				c.moveAngle = (uint8_t)s.NextInteger(0, 256);
			}
			c.moveSpeed = c.cfg_moveSpeed;
			xyInc = xyMath.GetXyInc(c.moveAngle) * c.moveSpeed;

			// todo: send msg to client ?
			stateIsChanged = false;
		}

		// 向之前定的随机方向移动				// todo: 这里需要检测 xy + inc 之后的点是否合法. 如果非法, 还需要补反转 xyInc 的代码
		c.xy.Add(xyInc);						// todo: 如果 moveCount == 0 期间移动被阻挡, 则启用导航功能返回出生点

		// 如果 move 的时间到了, 就切到 idle
		if (moveTicks <= s.ticks)
		{
			stateIsChanged = true;
			CORO_YIELDTO(0);
		}

		// 循环
		CORO_YIELDTO(1);
	}
	CORO_(2);													// 追杀( 先做无视地型无脑向目标前进的版本 )
	{
		// 判断目标是否失效, 如果已失效, 则开始返回出生点( 跳到返回状态 )
		if (!c.target)
		{
			stateIsChanged = true;
			CORO_GOTO(3);
		}

		// 判断是否有可用技能. 有就用
		auto sid = c.TakeAvaliableSkillId();
		if (sid != -1)
		{
			auto& skill = c.skills->At(sid);
			skill->Cast();
			// send msg to client?
			if (skill->cfg_castStunTimespan)
			{
				castStunTicks = s.ticks + skill->cfg_castStunTimespan;
				CORO_GOTO(4);
			}
		}

		// 如果状态改变过或目标位置变化过, 重算一切
		if (stateIsChanged || targetXyBak != c.target->xy)
		{
			if (stateIsChanged)
			{
				c.moveSpeed = c.cfg_moveSpeed;
				stateIsChanged = false;
			}

			// 根据当前目标来得到角度, 算出帧移动增量
			c.moveAngle = xyMath.GetAngle(c.target->xy - c.xy);
			xyInc = xyMath.GetXyInc(c.moveAngle) * c.moveSpeed;

			// 记录目标当前坐标. 如果目标未曾移动, 则不需要反复计算增量或是下发同步包
			targetXyBak = c.target->xy;

			// todo: send msg to client ?
		}

		// todo: 坐标重叠检测 / 基本身位保持算法

		// todo: 保持距离
		// 向目标移动				// todo: 这里需要接入 navMesh
		c.xy.Add(xyInc);

		// 循环
		CORO_YIELDTO(2);
	}
	CORO_(3);															// 返回出生点
	{
		// 判断是否持续相同方向移动中. 如果为刚开始移动, 或改变了方向 速度啥的, 则需要同步
		if (stateIsChanged)
		{
			c.moveAngle = xyMath.GetAngle(c.bornXY - c.xy);
			c.moveSpeed = c.cfg_moveSpeed;
			xyInc = xyMath.GetXyInc(c.moveAngle) * c.moveSpeed;

			// todo: send msg to client ?
			stateIsChanged = false;
		}

		// 判断是否已经回到了出生点( 距离小于一帧的移动增量 )
		if (xyMath.GetDistancePow2(c.bornXY - c.xy) <= c.cfg_moveSpeedPow2)
		{
			// 将坐标设为初生点, 跳到休息状态
			c.xy = c.bornXY;
			stateIsChanged = true;
			c.moveSpeed = 0;
			CORO_YIELDTO(0);
		}

		// 向之前定的方向移动					// todo: 导航
		c.xy.Add(xyInc);
		CORO_YIELDTO(3);

		// todo: 超时检测? 始终跑不回去的异常情况处理 
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
