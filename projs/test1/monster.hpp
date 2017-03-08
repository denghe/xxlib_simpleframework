MonsterBase::MonsterBase(SceneBase* scene) : SceneObjBase(scene) 
{
}

MonsterBase::~MonsterBase()
{
	// 从 所在容器 交换移除
	auto& buf = this->scene()->monsters->buf;
	auto& dataLen = this->scene()->monsters->dataLen;
	--dataLen;
	if (dataLen == this->sceneObjsIndex) return;	// last one
	else
	{
		buf[this->sceneObjsIndex] = buf[dataLen];
		buf[dataLen]->sceneObjsIndex = this->sceneObjsIndex;
	}
}

Scene* MonsterBase::scene()
{
	return (Scene*)sceneBase;
}

Monster1::Monster1(SceneBase* scene, char const* luacode) : MonsterBase(scene)
{
	SetFSM(CreateFSM<FSMLua>(this, luacode));
}
Monster2::Monster2(SceneBase* scene) : MonsterBase(scene)
{
}
