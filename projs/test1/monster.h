struct MonsterBase : SceneObjBase
{
	int x = 0;
	Scene* scene();
	MonsterBase(SceneBase* scene);
	~MonsterBase();
};
struct Monster1 : MonsterBase
{
	Monster1(SceneBase* scene, char const* luacode);
};
struct Monster2 : MonsterBase
{
	Monster2(SceneBase* scene);
};
