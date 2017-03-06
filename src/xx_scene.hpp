// 如果名字不对就改这里
#ifndef MPTYPENAME
#define MPTYPENAME MP
#endif

SceneObjBase::SceneObjBase()
{
	mempool<MPTYPENAME>().CreateTo(fsmStack);
}

template<typename T, typename ...Args>
T* SceneObjBase::CreateFSM(Args&&...args)
{
	static_assert(std::is_base_of<FSMBase, T>::value, "the T must be inherit of FSMBase.");
	auto p = mempool<MPTYPENAME>().Create<T>(std::forward<Args>(args)...);
	p->owner = this;
	return p;
}
void SceneObjBase::SetFSM(FSMBase* fsm)
{
	assert(fsm);
	assert(!deadFSM);
	deadFSM = currFSM;
	currFSM = fsm;
}
void SceneObjBase::PushFSM(FSMBase* fsm)
{
	fsmStack->Add(currFSM);
	currFSM = fsm;
}
void SceneObjBase::PopFSM()
{
	assert(!deadFSM);
	deadFSM = currFSM;
	currFSM = fsmStack->Top();
	fsmStack->Pop();
}
int SceneObjBase::Update()
{
	auto rtv = currFSM->Update();
	assert(currFSM);				// 不允许自杀
	if (deadFSM)
	{
		deadFSM->Release();
		deadFSM = nullptr;
	}
	return rtv;
}
SceneObjBase::~SceneObjBase()
{
	if (deadFSM)
	{
		deadFSM->Release();
		deadFSM = nullptr;
	}
	if (currFSM)
	{
		currFSM->Release();
		currFSM = nullptr;
	}
	while (fsmStack->dataLen)
	{
		fsmStack->Top()->Release();
		fsmStack->Pop();
	}
	fsmStack->Release();
}

SceneBase::SceneBase()
{
	mempool<MPTYPENAME>().CreateTo(objs);
}
SceneBase::~SceneBase()
{
	if (objs)
	{
		objs->Release();
		objs = nullptr;
	}
}

template<typename T, typename...Args>
xx::MPtr<T> SceneBase::Create(Args&&...args)
{
	auto p = mempool<MPTYPENAME>().Create<T>(std::forward<Args>(args)...);
	p->sceneBase = this;
	p->sceneObjsIndex = (uint32_t)objs->dataLen;
	objs->AddDirect(p);
	return p;
}

template<typename T, typename...Args>
void SceneBase::CreateTo(xx::MPtr<T>& tar, Args&&...args)
{
	tar = Create<T>(std::forward<Args>(args)...);
}

// 调目标析构并入池, 从 objs 移除. 使用交换移除法, 返回是否产生过交换行为
bool SceneBase::Release(SceneObjBase* p)
{
	auto idx = p->sceneObjsIndex;
	if (objs->SwapRemoveAt(idx))
	{
		objs->At(idx)->sceneObjsIndex = idx;
		return true;
	}
	return false;
}
