struct Scene : MP
{
	
	xx::MemHeader_MPObject monstersMH;	// 给下面这个类提供内存头
	xx::List<MonsterBase*> monsters;	// 放所有的 monster

	xx::MemHeader_MPObject errMH;	// 给下面这个类提供内存头
	xx::String err;

	static const int64_t msPerFrame = 1000 / 20;
	int64_t ticks = 0;

	Scene();
	~Scene();

	int Update();				// call by Run
	int Run();


	/****************************************************************/
	// lua
	/****************************************************************/

	void LoadLuaFile(char const* fn);
	lua_State* L = nullptr;
	lua_State* co = nullptr;		// SceneManager 自己的脚本( 执行优先于 objs 之 Updates )

	// 方便在 lua 中创建 Monster1 对象
	xx::MPtr<Monster1> CreateMonster1(char const* luacode);
	xx::MPtr<Monster2> CreateMonster2();
};
