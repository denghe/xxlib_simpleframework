MonsterBase::MonsterBase(SceneBase* scene) : SceneObjBase(scene) 
{
}

MonsterBase::~MonsterBase()
{
	if (sceneContainerIndex < 0) return;	// Create 时如果 throw exception 可能导致这个情况发生

	// 从 所在容器 交换移除, 同步下标
	auto& buf = this->scene()->monsters->buf;
	auto& dataLen = this->scene()->monsters->dataLen;
	--dataLen;
	if (dataLen == this->sceneContainerIndex) return;	// last one
	else
	{
		buf[this->sceneContainerIndex] = buf[dataLen];
		buf[dataLen]->sceneContainerIndex = this->sceneContainerIndex;
	}
}

Scene* MonsterBase::scene()
{
	return (Scene*)sceneBase;
}

Monster1::Monster1(SceneBase* sb, char const* luacode) : MonsterBase(sb)
{
	sceneContainerIndex = (uint32_t)scene()->monsters->dataLen;
	scene()->monsters->Add(this);
	SetFSM(CreateFSM<FSMLua>(this, luacode));
}
Monster2::Monster2(SceneBase* sb) : MonsterBase(sb)
{
	sceneContainerIndex = (uint32_t)scene()->monsters->dataLen;
	scene()->monsters->Add(this);
}

int Monster2::Update()
{
	return 0;
}
