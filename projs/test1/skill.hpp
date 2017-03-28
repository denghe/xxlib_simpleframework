SkillBase::SkillBase(MonsterBase* owner)
	: owner(owner)
{
	owner->skills->AddDirect(this);
}

// 检查 gcd, cd, target, 距离
// 即便当前技能是 "无锁定", 但也走 focus & distance 判定( 简易AI )
bool SkillBase::Avaliable()
{
	return owner->skillsGcd <= scene().ticks
		&& this->cd <= scene().ticks
		&& owner->target
		&& cfg_distance.IntersectPow2(owner->DistancePow2(owner->target.pointer));
}

void SkillBase::Cast()
{
	// 更新cd, gcd
	cd = scene().ticks + cfg_cd;
	owner->skillsGcd = scene().ticks + this->cfg_gcd;

	// 扣血 & 通知
	auto& tarHP = owner->target->hp;
	if (tarHP > 0)
	{
		owner->target->Hurt(owner);
		tarHP -= cfg_damage;
		if (tarHP <= 0)
		{
			assert(scene().deadMonsters->Find(owner->target.pointer) == -1);
			scene().deadMonsters->Add(owner->target.pointer);
			// todo: 击杀计数? cout 
		}
	}
}

SkillNear::SkillNear(MonsterBase* owner) : SkillBase(owner)
{
	// 模拟载入配置
	this->cfg_distance = { 0, 3 };
	this->cfg_damage = 5;
	this->cfg_cd = 3;
	this->cfg_castStunTimespan = 5;
}

SkillFar::SkillFar(MonsterBase* owner) : SkillBase(owner)
{
	// 模拟载入配置
	this->cfg_distance = { 15, 20 };
	this->cfg_damage = 10;
	this->cfg_cd = 6;
	this->cfg_castStunTimespan = 0;
}
