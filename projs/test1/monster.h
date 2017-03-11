struct MonsterBase : SceneObjBase
{
	int x = 0;
	~MonsterBase();
};
struct Monster1 : MonsterBase
{
	Monster1(char const* luacode);
};
struct Monster2 : MonsterBase
{
	Monster2();
	virtual int Update() override;
};
