struct MonsterBase : SceneObjBase
{
	xx::MPtr<MonsterBase> target;			// 当前目标/焦点
	xx::List_v<SkillBase*, true> skills;	// 怪的技能列表( 于怪创建时便固定创建了 )

	int hp = 100;							// 怪的初始血量
	int x = 0;								// 怪的坐标( 带坐标的似乎可以提取为另外一种基类 )

	int Distance(MonsterBase* other);		// 计算与目标的距离

	// todo: 目标查找函数? 范围内查找到符合条件的目标, 设置 focus

	// todo: 技能循环遍历, 找出可用的下一个技能. 以不超过 "技能总数次" 移动游标, 无 avaliable 返回空

	// todo: 当前指令( AI 代码在执行过程中, 将生成指令填充到此. 未来 N 帧内, 将执行这个指令 )
	// 指令更像是一个碎片状态, 此物需要发到客户端, 客户端同步播放
	// 指令设计与播放. 即 Monster 的 Update 中会具体执行该指令
	// 理论上讲, 指令有 移动, 停止, 攻击啥的. 攻击即释放技能, 理论上讲释放技能行为会导致创建出具体的技能实例
	// 换句话说, 怪身上会有一个容器, 存放 正在释放的技能 ?
	// AI 发起指令后, 理论上讲将进行一种条件等待状态, 以便指令执行一段时间.
	// 这期间如果出现某种变故, 可能需要修订 / 变更先前的指令内容
	// 也就是说同步到 client 的指令, 可能含有时间戳/ticks, 以及少量的绝对同步数据, 以配合指令变更行为

	MonsterBase();
	~MonsterBase();
};
struct Monster1 : MonsterBase
{
	Monster1();
};
struct Monster2 : MonsterBase
{
	Monster2();
};
