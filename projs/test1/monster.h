
struct MonsterFSM_Idle : FSMBase
{
	int64_t sleepToTicks;
	FSMLua* breakCond;

	MonsterFSM_Idle(SceneObjBase* owner);
	void Init(int64_t ticks, FSMLua* breakCond);
	virtual int Update() override;
};

struct MonsterFSM_Move : FSMBase
{
	int xInc;
	FSMLua* breakCond;

	MonsterFSM_Move(SceneObjBase* owner);
	void Init(int xInc, FSMLua* breakCond);
	virtual int Update() override;
};

struct MonsterFSM_Cast : FSMBase
{
	int skillIndex;

	MonsterFSM_Cast(SceneObjBase* owner);
	void Init(int skillIndex);
	virtual int Update() override;
};

struct MonsterBase : SceneObjBase
{
	MonsterBase();
	~MonsterBase();

	xx::MPtr<MonsterBase> target;			// 当前目标/焦点
	xx::List_v<SkillBase*, true> skills;	// 怪的技能列表( 于怪创建时便固定创建了 )
	int64_t skillsGcd = 0;					// 技能的公共CD控制变量( 也就是说所有技能都会去 check 它 )
	int skillCursor = -1;					// AI 释放技能所使用的循环游标

	int cfg_watchDistance = 0;				// 视距( 警戒范围 )( 模拟config, 在具体类中初始化 )

	int hp = 100;							// 怪的初始血量
	int x = 0;								// 怪的坐标( 带坐标的似乎可以提取为另外一种基类 )

	// 预创建 fsms 备用
	xx::MemHeaderBox<MonsterFSM_Idle> fsmIdle;
	xx::MemHeaderBox<MonsterFSM_Move> fsmMove;
	xx::MemHeaderBox<MonsterFSM_Cast> fsmCast;

	// 存储已创建出来的条件判断用 lua 状态机( 以便随着对象清掉 )
	xx::List_v<FSMLua*, true> conds;


	// 计算与目标的距离
	int Distance(MonsterBase* other);		

	// 搜索目标, 根据视距查找敌方, 找到后将设置 target 并返回 true. 找不到返回 false
	bool SearchTarget();


	// todo: 攻击事件造成 target 设置

	// todo: 需要一个在 lua 里可以方便获取的 事件通知机制.
	// todo: 感觉可以为 lua 提供  GetMessageCount   , GetMessageByIndex   之类的函数
	// todo: 这样子 lua 就可以遍历 并进一步处理.  Message 类的长相需要进一步设计


	// 循环遍历技能, 找出一个可用的技能下标返回. 找一轮都找不到就返回 -1
	int ChooseOneAvaliableSkill();

	// 传入一段 lua 代码来创建一个 条件判断用状态机. 如果 Update 返回非 0 则表示执行完毕.
	// 主用于实现 Idle , Move 时的 "唤醒" 效果, 模拟事件发生. 
	// 状态机 Update 期间计算出来的结果, 这需要在 MonsterBase 中加结果容器变量用于持有 / 交互.
	xx::MPtr<FSMLua> Cond(char const* luacode);

	// 创建一个 MonsterFSM_Idle 状态机并 Push. yield 函数.
	// ticks 为 idle 时长. cond 为当 Update 返回非 0 时, 认为条件达成
	void Idle(int64_t ticks, FSMLua* cond);

	// 创建一个 MonsterFSM_Move 状态机并 Push. yield 函数.
	// xInc 为 x 每帧递增值. cond 为当 Update 返回非 0 时, 认为条件达成
	void Move(int xInc, FSMLua* cond);

	// 创建一个 MonsterFSM_Cast 状态机并 Push. yield 函数.
	// skillIndex 为 技能下标. 当技能播放完毕或终止时该状态将弹出
	void Cast(int skillIndex);
};


struct Monster1 : MonsterBase
{
	Monster1();
};
struct Monster2 : MonsterBase
{
	Monster2();
};
