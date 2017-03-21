MonsterBase::MonsterBase()
{
	auto& ms = *scene().monsters;
	sceneContainerIndex = (uint32_t)ms.dataLen;
	ms.Add(this);
}
MonsterBase::~MonsterBase()
{
	// Create 时如果 throw exception 可能导致这个情况发生
	if (sceneContainerIndex < 0) return;

	// 从 所在容器 交换移除, 同步下标
	auto& ms = *scene().monsters;
	auto& buf = ms.buf;
	auto& dataLen = ms.dataLen;
	--dataLen;
	if (dataLen == this->sceneContainerIndex) return;	// last one
	else
	{
		buf[this->sceneContainerIndex] = buf[dataLen];
		buf[dataLen]->sceneContainerIndex = this->sceneContainerIndex;
	}
}

Monster1::Monster1() : MonsterBase()
{
	SetFSM(CreateFSM<FSMLua>(this, "monster1.lua"));
}
Monster2::Monster2() : MonsterBase()
{
	SetFSM(CreateFSM<FSMLua>(this, "monster2.lua"));
}
