struct MonsterBase : SceneObjBase
{
	Scene* scene();
};
struct Monster1 : MonsterBase
{
	Monster1(char const* luacode);
};
