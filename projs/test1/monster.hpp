MonsterBase::MonsterBase(SceneBase* scene) : SceneObjBase(scene) {}

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
