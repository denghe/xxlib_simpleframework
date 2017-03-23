MonsterBase::MonsterBase()
	: skills(scene())
	, fsmIdle(scene(), this)
	, fsmMove(scene(), this)
	, fsmCast(scene(), this)
	, conds(scene())
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

int MonsterBase::Distance(MonsterBase* other)
{
	return std::abs(this->x - other->x);
}

bool MonsterBase::SearchTarget()
{
	for (auto& m : *scene().monsters)
	{
		if (m == this) continue;
		// todo: 条件检查
		target = m;
		return true;
	}
	return false;
}

int MonsterBase::ChooseOneAvaliableSkill()
{
	// 循环移动 skillCursor, 并检测技能是否可用. 最多移动一圈. 返回第1个发现的可用技能
	for (int i = 0; i < (int)skills->dataLen; ++i)
	{
		skillCursor++;
		if (skillCursor >= skills->dataLen) skillCursor = 0;
		if (skills->At(skillCursor)->Avaliable()) return skillCursor;
	}
	return -1;
}

xx::MPtr<FSMLua> MonsterBase::Cond(char const* luacode)
{
	auto fsm = scene().Create<FSMLua>(this, luacode, false);
	conds->AddDirect(fsm);
	return fsm;
}

void MonsterBase::Idle(int64_t ticks, FSMLua* cond)
{
	assert(cond);
	fsmIdle->AddRef();
	fsmIdle->Init(ticks, cond);
	PushFSM(fsmIdle);
}
void MonsterBase::Move(int xInc, FSMLua* cond)
{
	assert(cond);
	fsmMove->AddRef();
	fsmMove->Init(xInc, cond);
	PushFSM(fsmMove);
}
void MonsterBase::Cast(int skillIndex)
{
	fsmCast->AddRef();
	fsmCast->Init(skillIndex);
	PushFSM(fsmCast);
}


MonsterFSM_Idle::MonsterFSM_Idle(SceneObjBase* owner) : FSMBase(owner)
{
}
void MonsterFSM_Idle::Init(int64_t ticks, FSMLua* breakCond)
{
	this->sleepToTicks = scene().ticks + ticks;
	this->breakCond = breakCond;
}
int MonsterFSM_Idle::Update()
{
	// todo
	return 0;
}

MonsterFSM_Move::MonsterFSM_Move(SceneObjBase* owner) : FSMBase(owner)
{
}
void MonsterFSM_Move::Init(int xInc, FSMLua* breakCond)
{
	this->xInc = xInc;
	this->breakCond = breakCond;
}
int MonsterFSM_Move::Update()
{
	// todo
	return 0;
}

MonsterFSM_Cast::MonsterFSM_Cast(SceneObjBase* owner) : FSMBase(owner)
{
}
void MonsterFSM_Cast::Init(int skillIndex)
{
	this->skillIndex = skillIndex;
}
int MonsterFSM_Cast::Update()
{
	// todo
	return 0;
}

Monster1::Monster1() : MonsterBase()
{
	this->cfg_watchDistance = 30;
	// todo: 填充技能
	SetFSM(CreateFSM<FSMLua>(this, "monster1.lua"));
}

Monster2::Monster2() : MonsterBase()
{
	this->cfg_watchDistance = 20;
	// todo: 填充技能
	SetFSM(CreateFSM<FSMLua>(this, "monster2.lua"));
}
