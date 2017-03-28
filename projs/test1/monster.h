struct MonsterBase;
struct MonsterFSMBase : FSMBase
{
	MonsterBase& ctx();
	MonsterFSMBase(SceneObjBase* owner);
};

//struct MonsterFSM_Idle : MonsterFSMBase
//{
//	int64_t sleepToTicks;
//	FSMBase* breakCond = nullptr;
//
//	MonsterFSM_Idle(SceneObjBase* owner);
//	void Init(int64_t ticks, FSMBase* breakCond);
//	virtual int Update() override;
//};
//
//struct MonsterFSM_Move : MonsterFSMBase
//{
//	int xInc;
//	int64_t toTicks;
//	FSMBase* breakCond;
//
//	MonsterFSM_Move(SceneObjBase* owner);
//	void Init(int xInc, int count, FSMBase* breakCond);
//	virtual int Update() override;
//};

struct MonsterFSM_Cast : MonsterFSMBase
{
	int skillIndex;

	MonsterFSM_Cast(SceneObjBase* owner);
	void Init(int skillIndex);
	virtual int Update() override;
};

struct MonsterFSM_AI : MonsterFSMBase
{
	int64_t alertInterval = 0;				// 当前警戒 ticks. 出生后立即检测
	int moveCount = 0;						// 移动次数计数 for 几次后必向出生点移动 1 次
	int64_t moveTicks;						// 当前移动 / 休息 ticks( 构造函数中初始化 )
	XY xyInc;								// 移动增量
	XY targetXyBak;							// 备份目标坐标以方便移动判断
	int64_t castStunTicks = 0;				// 使用技能造成的僵直 ticks 计数器

	bool Alert();							// 试警戒一把( 如果 cd 到了的话 ), 如果发现目标就返回 true
	bool stateIsChanged = false;			// 用来判断是否为首次move或idle

	MonsterFSM_AI(SceneObjBase* owner);
	int lineNumber = 0;						// stackless 协程行号
	virtual int Update() override;
};

//struct MonsterFSM_AlertCondition : MonsterFSMBase
//{
//	MonsterFSM_AlertCondition(SceneObjBase* owner);
//	virtual int Update() override;
//};

struct MonsterBase : SceneObjBase
{
	MonsterBase();
	~MonsterBase();

	xx::List_v<SkillBase*, true> skills;					// 怪的技能列表( 于怪创建时便固定创建了 )( LUA 不可见 )
	int skillCursor = -1;									// AI 释放技能所使用的循环游标( LUA 不可见 )
	int64_t skillsGcd = 0;									// 技能的公共 CD ticks( LUA 不可见 )

	// 模拟 cfg( 当前以 20 ticks 为 1 秒来估计 )			
	int cfg_alertInterval;									// 警戒间隔( 每隔多少 ticks 执行一次警戒判定 )
	int cfg_moveTimespan;									// 每移动一次所花时间
	int cfg_moveInterval;									// 每移动一次后的休息时间
	int cfg_moveBackInterval;								// 每多少次移动行为之后, 必然有一次正对出生点的移动
	int cfg_traceMaxTimespan;								// 追杀最长时间( 从本次警戒切到追杀的时间点算起 )
	float cfg_traceMaxDistance;								// 追杀最远距离( 由当前点出发的半径 )
	float cfg_alertDistance;								// 警戒距离( 由当前点出发的半径 )
	xx::Range<float> cfg_traceKeepDistanceRange;			// 追杀时与敌人保持距离的范围( 如果敌人比最小距离近, 则持续远离之. 远离直接取反方向移动. 如果移不动就不移了 )
	float cfg_moveSpeed;									// 每帧移动米数
	float cfg_radius;										// 身体半径( AI 向目标移动时 以身体有所重叠为最小距离保持 )
	//bool cfg_retreatAlert = false;						// 逃离时警戒

	// cfg 加载后期填充
	float cfg_moveSpeedPow2;								// cfg_moveSpeedPow2 * cfg_moveSpeedPow2
	float cfg_alertDistancePow2;							// cfg_alertDistance * cfg_alertDistance
	float cfg_traceMaxDistancePow2;							// cfg_traceMaxDistance * cfg_traceMaxDistance
	xx::Range<float> cfg_traceKeepDistanceRangePow2;		// cfg_traceKeepDistanceRange * cfg_traceKeepDistanceRange

	// 向客户端同步的指令考虑这样: 只有一种移动包, 含有 怪起始坐标, 朝向, 速度( 距离每秒 ) 这三个参数. 速度为0 就是原地休息. 在收到下一条指令前, 客户端持续模拟.
	// 也就是说, 下发数据的时机为: 怪的行为发生改变, 或是玩家格子同步

	// 下面这 2 个会同步到客户端
	uint8_t moveAngle;										// 当前移动方向
	float moveSpeed;										// 当前移动速度( 这个速度是指 1 帧的基于角度所对应的 xz 增量的乘法系数 ). 客户端通过这个值来判断怪当前是否有在移动.

	XY xy;													// 怪的当前坐标( LUA 只读 )
	XY bornXY;												// 当怪被放置到 map 时的初始坐标( for 回撤 )
	XY originalXY;											// 当怪脱离警戒状态时备份坐标
	int hp;													// 怪的血量( LUA 只读 )
	xx::MPtr<MonsterBase> target;							// 当前目标/焦点( LUA 只读 )

	//xx::List_v<FSMBase*, true> fsmConds;					// 存储已创建出来的条件判断用 lua 状态机( 以便随着对象清掉 )

	// 静态创建各种状态( 一部分通过 Init 初始化复用 )
	//xx::MemHeaderBox<MonsterFSM_Idle> fsmIdle;
	//xx::MemHeaderBox<MonsterFSM_Move> fsmMove;
	//xx::MemHeaderBox<MonsterFSM_AlertCondition> fsmAlertCondition;
	xx::MemHeaderBox<MonsterFSM_Cast> fsmCast;
	xx::MemHeaderBox<MonsterFSM_AI> fsmAI;

	float DistancePow2(MonsterBase* other);					// 计算与目标的距离的平方值
	xx::MPtr<MonsterBase> SearchTarget();					// 搜索目标, 根据视距查找敌方并返回
	void SetTarget(MonsterBase* target);					// 设置当前 target( 传入空指针就是清除 )
	void Hurt(MonsterBase* attacker);						// 如果没有 target 将导致 set target. 并不具体执行减血的效果
	int TakeAvaliableSkillId();								// 循环遍历技能, 找出一个可用的技能下标返回. 找一轮都找不到就返回 -1


	//// 传入一段 lua 代码来创建一个 条件判断用状态机. 如果 Update 返回非 0 则表示执行完毕.
	//// 主用于实现 Idle , Move 时的 "唤醒" 效果, 模拟事件发生. 
	//// 状态机 Update 期间计算出来的结果, 这需要在 MonsterBase 中加结果容器变量用于持有 / 交互.
	//xx::MPtr<FSMLua> LuaCondition(char const* luacode);

	//// 创建状态机并放入 fsmConds 容器当中.
	//template<typename T, typename ... Args>
	//xx::MPtr<T> Condition(Args&&...args);

	//// 创建一个 MonsterFSM_Idle 状态机并 Push. yield 函数.
	//// ticks 为 idle 时长. cond 为当 Update 返回非 0 时, 认为条件达成
	//void Idle(int64_t ticks, FSMBase* cond);

	//// 创建一个 MonsterFSM_Move 状态机并 Push. yield 函数.
	//// xInc 为 x 每帧递增值. cond 为当 Update 返回非 0 时, 认为条件达成
	//void Move(int xInc, int count, FSMBase* cond);

	//// 创建一个 MonsterFSM_Cast 状态机并 Push. yield 函数.
	//// skillIndex 为 技能下标. 当技能播放完毕或终止时该状态将弹出
	//void Cast(int skillIndex);
};


struct Monster1 : MonsterBase
{
	Monster1();
};
struct Monster2 : MonsterBase
{
	Monster2();
};
