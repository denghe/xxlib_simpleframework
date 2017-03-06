Scene* MonsterBase::scene()
{
	return (Scene*)sceneBase;
}
Monster1::Monster1(char const* luacode)
{
	SetFSM(CreateFSM<FSMLua>(luacode));
}
