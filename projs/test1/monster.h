struct MonsterBase;
struct MonsterFSMBase : FSMBase
{
	MonsterBase& ctx();
	MonsterFSMBase(SceneObjBase* owner);
};

struct MonsterFSM_Idle : MonsterFSMBase
{
	int64_t sleepToTicks;
	FSMBase* breakCond = nullptr;

	MonsterFSM_Idle(SceneObjBase* owner);
	void Init(int64_t ticks, FSMBase* breakCond);
	virtual int Update() override;
};

struct MonsterFSM_Move : MonsterFSMBase
{
	int xInc;
	int64_t toTicks;
	FSMBase* breakCond;

	MonsterFSM_Move(SceneObjBase* owner);
	void Init(int xInc, int count, FSMBase* breakCond);
	virtual int Update() override;
};

struct MonsterFSM_Cast : MonsterFSMBase
{
	int skillIndex;

	MonsterFSM_Cast(SceneObjBase* owner);
	void Init(int skillIndex);
	virtual int Update() override;
};

struct MonsterFSM_AI : MonsterFSMBase
{
	int lineNumber = 0;
	MonsterFSM_AI(SceneObjBase* owner);
	virtual int Update() override;
};

struct MonsterFSM_AlertCondition : MonsterFSMBase
{
	MonsterFSM_AlertCondition(SceneObjBase* owner);
	virtual int Update() override;
};

struct MonsterBase : SceneObjBase
{
	MonsterBase();
	~MonsterBase();

	xx::List_v<SkillBase*, true> skills;		// 怪的技能列表( 于怪创建时便固定创建了 )( LUA 不可见 )
	int64_t skillsGcd = 0;						// 技能的公共CD控制变量( 所有技能都 check 它 )( LUA 不可见 )
	int skillCursor = -1;						// AI 释放技能所使用的循环游标( LUA 不可见 )

	int cfg_watchDistance = 0;					// 视距( 警戒范围 )( 模拟config, 在具体类中初始化 )
	// todo: 保持距离相关, 追击范围相关, 出生点相关

	int hp = 100;								// 怪的初始血量( LUA 只读 )
	int x = 0;									// 怪的坐标( 带坐标的似乎可以提取为另外一种基类 )( LUA 只读 )
	int originalX = 0;							// 当怪脱离警戒状态时, 该值用于存放原始 x 以便回撤时使用
	xx::MPtr<MonsterBase> target;				// 当前目标/焦点( LUA 只读 )

	xx::List_v<FSMBase*, true> fsmConds;		// 存储已创建出来的条件判断用 lua 状态机( 以便随着对象清掉 )

	// 静态创建各种状态( 一部分通过 Init 初始化复用 )
	xx::MemHeaderBox<MonsterFSM_Idle> fsmIdle;
	xx::MemHeaderBox<MonsterFSM_Move> fsmMove;
	xx::MemHeaderBox<MonsterFSM_Cast> fsmCast;
	xx::MemHeaderBox<MonsterFSM_AI> fsmAI;
	xx::MemHeaderBox<MonsterFSM_AlertCondition> fsmAlertCondition;

	int Distance(MonsterBase* other);			// 计算与目标的距离
	xx::MPtr<MonsterBase> SearchTarget();		// 搜索目标, 根据视距查找敌方并返回
	void SetTarget(MonsterBase* target);		// 设置当前 target( 传入空指针就是清除 )
	void Hurt(MonsterBase* attacker);			// 本函数仅起一个事件通知的作用, 并不具体执行减血的效果
	int AnyAvaliableSkillId();					// 循环遍历技能, 找出一个可用的技能下标返回. 找一轮都找不到就返回 -1


	// 传入一段 lua 代码来创建一个 条件判断用状态机. 如果 Update 返回非 0 则表示执行完毕.
	// 主用于实现 Idle , Move 时的 "唤醒" 效果, 模拟事件发生. 
	// 状态机 Update 期间计算出来的结果, 这需要在 MonsterBase 中加结果容器变量用于持有 / 交互.
	xx::MPtr<FSMLua> LuaCondition(char const* luacode);

	// 创建状态机并放入 fsmConds 容器当中.
	template<typename T, typename ... Args>
	xx::MPtr<T> Condition(Args&&...args);

	// 创建一个 MonsterFSM_Idle 状态机并 Push. yield 函数.
	// ticks 为 idle 时长. cond 为当 Update 返回非 0 时, 认为条件达成
	void Idle(int64_t ticks, FSMBase* cond);

	// 创建一个 MonsterFSM_Move 状态机并 Push. yield 函数.
	// xInc 为 x 每帧递增值. cond 为当 Update 返回非 0 时, 认为条件达成
	void Move(int xInc, int count, FSMBase* cond);

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
