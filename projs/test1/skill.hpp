SkillBase::SkillBase(MonsterBase* owner)
{
	owner->skills->Add(this);
}

// 检查 gcd, cd, target, 距离
// 即便当前技能是 "无锁定", 但也走 focus & distance 判定( 简易AI )
bool SkillBase::Avaliable()
{
	return owner->skillsGcd <= scene().ticks
		&& this->cd <= scene().ticks
		&& owner->target
		&& cfg_distance.Test(owner->Distance(owner->target.pointer));
}

void SkillBase::Cast()
{
	// 更新cd, gcd
	cd = scene().ticks + cfg_cd;
	owner->skillsGcd = scene().ticks + this->cfg_gcd;

	// 扣血
	auto& tarHP = owner->target->hp;
	if (tarHP)
	{
		tarHP -= cfg_damage;
		if (tarHP <= 0)
		{
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
}

SkillFar::SkillFar(MonsterBase* owner) : SkillBase(owner)
{
	// 模拟载入配置
	this->cfg_distance = { 15, 20 };
	this->cfg_damage = 10;
	this->cfg_cd = 6;
}
