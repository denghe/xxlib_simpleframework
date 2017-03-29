

int Scene::Run()
{
#ifdef _WIN32
	timeBeginPeriod(1);
#endif
	int64_t accumulatMS = 0;
	xx::Stopwatch sw;
	while (true)
	{
		auto durationMS = sw();
		if (durationMS > msPerFrame)
		{
			// todo: 日志超长帧
			durationMS = msPerFrame;
		}

		accumulatMS += durationMS;
		bool executed = false;
		while (accumulatMS >= msPerFrame)
		{
			executed = true;
			accumulatMS -= msPerFrame;

			if (auto rtv = Update()) return rtv;
			++ticks;
		}
		if (!executed)
		{
			Sleep(1);     // 省点 cpu
						  // todo: 日志清闲帧
		}
	}
#ifdef _WIN32
	timeEndPeriod(1);
#endif
	return 0;
}


Scene::~Scene()
{
	while (monsters->dataLen)
	{
		monsters->Top()->Release();
	}

	co = nullptr;
	if (L)
	{
		lua_close(L);
		L = nullptr;
	}
}


int Scene::NextInteger(int min, int max)
{
	return rnd->Next(min, max);
}
double Scene::NextDouble(double min, double max)
{
	return rnd->NextDouble(min, max);
}

uint32_t Scene::Monsters_Count()
{
	return monsters->dataLen;
}
xx::MPtr<MonsterBase> Scene::Monsters_At(uint32_t index)
{
	if (index >= monsters->dataLen) return nullptr;
	return monsters->At(index);
}








int Scene::Update()
{
	if (co) if (auto rtv = xx::Lua_Resume(co, err)) return rtv;

	for (auto i = (int)monsters->dataLen - 1; i >= 0; --i)
	{
		auto& o = monsters->At(i);
		auto r = o->Update();
		if (r)
		{
			// todo: r < 0 log ?
			o->Release();
		}
	}
	for (auto& o : *deadMonsters)
	{
		o->Release();
	}
	deadMonsters->Clear();

	// more for ....

	return 0;
}

void Scene::LoadLuaFile(char const* fn)
{
	assert(fn);

	// 创建协程( 在协程中直接用 local self = scene 来访问 )
	co = xx::Lua_RegisterCoroutine(L, this);

	// 加载协程 lua 代码
	luaL_loadfile(co, fn);
	xx::LuaFunc<MP, Scene*>::Push(co, this);
	//lua_newtable(co);							// todo: 创建或载入一张 lua 表以供 scene 使用
	xx::Lua_Resume(co, err, 1);
}

Scene::Scene()
	: rnd(*this)
	, deadMonsters(*this)
	, monsters(*this)
	, err(*this)
{
	// create lua state
	L = xx::Lua_NewState(*this);

	// LuaBind: Scene
	xx::Lua_PushMetatable<MP, Scene>(L);
	xxLua_BindField(MP, L, Scene, ticks, false);
	xxLua_BindFunc(MP, L, Scene, CreateMonster, false);
	xxLua_BindFunc(MP, L, Scene, NextInteger, false);
	xxLua_BindFunc(MP, L, Scene, NextDouble, false);
	xxLua_BindFunc(MP, L, Scene, Monsters_Count, false);
	xxLua_BindFunc(MP, L, Scene, Monsters_At, false);
	lua_pop(L, 1);

	// LuaBind: XY
	xx::Lua_PushMetatable<MP, XY>(L);
	xxLua_BindField(MP, L, XY, x, false);
	xxLua_BindField(MP, L, XY, y, false);
	lua_pop(L, 1);

	// LuaBind: SkillBase
	xx::Lua_PushMetatable<MP, SkillBase>(L);
	xxLua_BindFunc(MP, L, SkillBase, Cast, false);
	lua_pop(L, 1);

	// LuaBind: MonsterBase
	xx::Lua_PushMetatable<MP, MonsterBase>(L);
	xxLua_BindField(MP, L, MonsterBase, xy, false);
	xxLua_BindField(MP, L, MonsterBase, hp, false);
	xxLua_BindField(MP, L, MonsterBase, moveAngle, false);
	xxLua_BindField(MP, L, MonsterBase, moveSpeed, false);
	xxLua_BindField(MP, L, MonsterBase, target, false);
	xxLua_BindFunc(MP, L, MonsterBase, DistancePow2, false);
	xxLua_BindFunc(MP, L, MonsterBase, SearchTarget, false);
	xxLua_BindFunc(MP, L, MonsterBase, SetTarget, false);
	xxLua_BindFunc(MP, L, MonsterBase, Hurt, false);
	xxLua_BindFunc(MP, L, MonsterBase, TakeAvaliableSkill, false);
	lua_pop(L, 1);

	// more bind here...

	// 由根至叶, 逐级复制所有元素到下一级( 不覆盖 )
	xx::Lua_CloneParentMetatables(*this, L);

	// set global scene
	xx::Lua_SetGlobal<MP>(L, "scene", this);
}

xx::MPtr<MonsterBase> Scene::CreateMonster(char const* classname)
{
	if (!std::strcmp(classname, "Monster1"))
	{
		return Create<Monster1>();
	}
	else if (!std::strcmp(classname, "Monster2"))
	{
		return Create<Monster2>();
	}
	return nullptr;
}
