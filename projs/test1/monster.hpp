MonsterBase::MonsterBase()
	: skills(scene())
	, fsmConds(scene())
	, fsmIdle(scene(), this)
	, fsmMove(scene(), this)
	, fsmCast(scene(), this)
	, fsmAI(scene(), this)
	, fsmAlertCondition(scene(), this)
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



Monster1::Monster1() : MonsterBase()
{
	// 模拟 载入配置
	this->cfg_watchDistance = 30;

	// 模拟 根据配置 创建技能
	scene().Create<SkillNear>(this);
	scene().Create<SkillFar>(this);


	// 模拟 根据配置 载入初始AI
	SetFSM(fsmAI);

	// 载入 LUA 版初始 AI
	//SetFSM(CreateFSM<FSMLua>(this, "monster1.lua"));
}



Monster2::Monster2() : MonsterBase()
{
	// 模拟 载入配置
	this->cfg_watchDistance = 20;

	// 模拟 根据配置 创建技能
	scene().Create<SkillNear>(this);
	scene().Create<SkillFar>(this);

	// 模拟 根据配置 载入初始AI
	SetFSM(fsmAI);

	// 载入 LUA 版初始 AI
	//SetFSM(CreateFSM<FSMLua>(this, "monster2.lua"));
}



int MonsterBase::Distance(MonsterBase* other)
{
	return std::abs(this->x - other->x);
}

xx::MPtr<MonsterBase> MonsterBase::SearchTarget()
{
	MonsterBase* r = nullptr;
	int minDistance = std::numeric_limits<int>::max();
	for (auto& m : *scene().monsters)
	{
		if (m == this) continue;
		auto d = Distance(m);
		if (d <= cfg_watchDistance && minDistance > d)
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


int MonsterBase::AnyAvaliableSkillId()
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

xx::MPtr<FSMLua> MonsterBase::LuaCondition(char const* luacode)
{
	auto fsm = scene().Create<FSMLua>(this, luacode, false);
	fsmConds->AddDirect(fsm);
	return fsm;
}

template<typename T, typename ... Args>
xx::MPtr<T> MonsterBase::Condition(Args&&...args)
{
	auto fsm = scene().Create<T>(this, std::forward<Args>(args)...);
	fsmConds->AddDirect(fsm);
	return fsm;
}

void MonsterBase::Idle(int64_t ticks, FSMBase* cond)
{
	assert(cond);
	fsmIdle->Init(ticks, cond);
	PushFSM(fsmIdle);
}

void MonsterBase::Move(int xInc, int count, FSMBase* cond)
{
	assert(cond);
	fsmMove->Init(xInc, count, cond);
	PushFSM(fsmMove);
}

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





MonsterFSM_Idle::MonsterFSM_Idle(SceneObjBase* owner) : MonsterFSMBase(owner)
{
}
void MonsterFSM_Idle::Init(int64_t ticks, FSMBase* breakCond)
{
	this->sleepToTicks = scene().ticks + ticks;
	this->breakCond = breakCond;
}
int MonsterFSM_Idle::Update()
{
	if (sleepToTicks <= scene().ticks) return 1;
	return breakCond ? breakCond->Update() : 0;
}




MonsterFSM_Move::MonsterFSM_Move(SceneObjBase* owner) : MonsterFSMBase(owner)
{
}
void MonsterFSM_Move::Init(int xInc, int count, FSMBase* breakCond)
{
	this->xInc = xInc;
	this->toTicks = scene().ticks + count;
	this->breakCond = breakCond;
}
int MonsterFSM_Move::Update()
{
	if (scene().ticks >= this->toTicks) return 1;
	ctx().x += xInc;
	return breakCond ? breakCond->Update() : 0;
}




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
	return 0;
}



MonsterFSM_AlertCondition::MonsterFSM_AlertCondition(SceneObjBase* owner) : MonsterFSMBase(owner) {}

// 有怪进入范围则条件满足, 填充 target 属性后返回非 0 ( 通知使用条件者退出 )
int MonsterFSM_AlertCondition::Update()
{
	assert(!ctx().target);
	auto tar = ctx().SearchTarget();
	if (tar)
	{
		ctx().target = tar;
		ctx().originalX = ctx().x;
		return 1;
	}
	return 0;
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
}							\
return 0

#define CORO_YIELD(n)		\
{							\
	lineNumber = n;			\
	return 0;				\
}

#define CORO_GOTO(n)		\
goto Label##n


// 先来个 idle / move, 发现怪进入范围就 Cast
int MonsterFSM_AI::Update()
{
	CORO_BEGIN();
	{
		// 根据随机值 -1, 0, 1 来决定是 move 还是 idle
		assert(!ctx().target);
		auto& cond = ctx().fsmAlertCondition;
		auto v = scene().rnd->Next(-1, 2);
		assert(v >= -1 && v <= 1);
		if (v == 0)
		{
			ctx().Idle(scene().rnd->Next(1, 3), cond);
		}
		else
		{
			ctx().Move(v, scene().rnd->Next(1, 3), cond);
		}
		CORO_YIELD(0);
	}
	CORO_(1);
	{
		// 如果有目标, 则表明先前的 idle 过程中判定条件达成, 目标被设置, 则追杀( 持续向目标移动, 有技能放时就放技能 )
		if (ctx().target)
		{
			auto skillid = ctx().AnyAvaliableSkillId();
			if (skillid == -1)
			{
				int xInc = ctx().target->x > ctx().x ? 1 : -1;
				ctx().Move(xInc, 3, nullptr);
			}
			else
			{
				ctx().Cast(skillid);
			}
			CORO_YIELD(1);
		}
		else
		{
			// 如果目标丢失( 下线? 被打死? ), 执行回撤操作( 持续 move 到发现 target 瞬间记录的的坐标 )
			auto d = ctx().originalX - ctx().x;
			ctx().Move(d > 0 ? 1 : -1, std::abs(d), nullptr);
			CORO_YIELD(0);
		}
	}
	CORO_END();
}

MonsterFSM_AI::MonsterFSM_AI(SceneObjBase* owner) : MonsterFSMBase(owner) {}

