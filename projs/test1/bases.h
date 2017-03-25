/*

MP 定义中须含有下面这些类型

typedef xx::MemPool<
.............
Scene,
SceneObjBase,
FSMBase,
xx::List<FSMBase*>, 
................> MP;

*/


struct Scene;
struct SceneObjBase;

// 带 Update 虚函数的类的基类
struct UpdateBase : xx::MPObject
{
	Scene& scene();							// 语法糖
	Scene& scene() const;
	virtual int Update() = 0;				// 返回非 0 表示自杀. 正数通常是正常结束生命, 负数通常表示出错
};

// 状态机基类. 只能用 SceneObjBase 的函数来创建. 不可以直接 Release. 自杀是用 PopFSM 或 Update 返回非 0
struct FSMBase : UpdateBase
{
	SceneObjBase* owner;
	void Pop();
	FSMBase(SceneObjBase* owner) : owner(owner) {}
};

// 场景的子的基类( 场景对象 )
struct SceneObjBase : UpdateBase
{
	uint32_t sceneContainerIndex = -1;		// 位于 scene 中某容器中的下标 ( List / Links / Dict )

	xx::List_v<FSMBase*> fsmStack;

	FSMBase* currFSM = nullptr;
	FSMBase* deadFSM = nullptr;		// 延迟杀掉
	// todo: 提供一个容器用来存上个 FSM 的执行结果( 正常结束, 满足退出条件, 打断事件来临之类 )

	SceneObjBase();
	~SceneObjBase();

	template<typename T, typename ...Args>
	T* CreateFSM(Args&&...args);
	void SetFSM(FSMBase* fsm);
	void PushFSM(FSMBase* fsm);
	void PopFSM();
	virtual int Update() override;
};
