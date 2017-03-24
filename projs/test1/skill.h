struct MonsterBase;

struct Range
{
	int from, to;
	inline bool Test(int n)
	{
		assert(from <= to);
		return n >= from && n <= to;
	}
};

struct SkillBase : UpdateBase
{
	// 模拟配置
	static const int cfg_gcd = 3;

	Range cfg_distance;					// 施放距离范围
	int cfg_damage;						// 伤害值
	int cfg_cd;							// 瞬发后的延迟时长( 先这样简化 )

	// runtime
	int64_t cd = 0;						// 用于计算 cd 的 timer
	MonsterBase* owner;					// 技能挂在哪个怪身上
	SkillBase(MonsterBase* owner);

	virtual bool Avaliable();			// 返回当前是否可用的标志位
	virtual void Cast();				// 释放该技能, 更新 cd, 减目标血
	inline virtual int Update() override { return 0; }
};

// 近战片伤, 无锁定
// 随时都可以施放, 但是如果释放范围内无怪的话则无任何意义...
// 故逻辑上该技能其实需求范围内敌人查找, 提供比 avaliable 更佳的施放提示
// 也就是说, 针对具体技能, 可以有一系列 test 功能: 假设当前释放这个技能, 估计能得到怎样的收效
// 初步思考, 收效分为: 1. 命中列表.  2. 致死人数   3. 总伤害量....
// 此类技能于 AI 来讲, avaliable 没啥意义. 需要进一步追究该技能的 释放 性价比.
struct SkillNear : SkillBase
{
	SkillNear(MonsterBase* owner);
};

// 远程点杀, 在施放的时候需要 owner 锁定目标
// 也就是说怪的身上会有一个 焦点 属性, 当怪的焦点存在且有效时, 
// 当前技能可用的条件之一被满足( 另一条件通常是 cd 已到 )
struct SkillFar : SkillBase
{
	SkillFar(MonsterBase* owner);
};
