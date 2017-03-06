struct MonsterBase : SceneObjBase
{
	Scene* scene();
	MonsterBase(SceneBase* scene);
};
struct Monster1 : MonsterBase
{
	Monster1(SceneBase* scene, char const* luacode);
};
struct Monster2 : MonsterBase
{
	Monster2(SceneBase* scene);
};
