void Scene::LoadLuaFile(char const* fn)
{
	assert(fn);

	// 创建协程( 在协程中直接用 local self = scene 来访问 )
	co = xx::Lua_RegisterCoroutine(L, this);

	// 加载协程 lua 代码
	luaL_loadfile(co, fn);
	xx::Lua<MP, Scene*>::Push(co, this);
	xx::Lua_Resume(co, &err, 1);
}

Scene::Scene()
	: monstersMH(*this)
	, errMH(*this)
{
	// more...

	L = xx::Lua_NewState(*this);

	// LuaBind: Scene
	xx::Lua_PushMetatable<MP, Scene>(L);
	xxLua_BindField(MP, L, Scene, ticks, false);
	xxLua_BindFunc(MP, L, Scene, CreateMonster1, false);
	xxLua_BindFunc(MP, L, Scene, CreateMonster2, false);
	lua_pop(L, 1);

	// LuaBind: MonsterBase
	xx::Lua_PushMetatable<MP, MonsterBase>(L);
	xxLua_BindField(MP, L, MonsterBase, x, true);
	lua_pop(L, 1);

	// more bind here...

	// 由根至叶, 逐级复制所有元素到下一级( 不覆盖 )
	xx::Lua_CloneParentMetatables(*this, L);

	// set global scene
	xx::Lua_SetGlobal<MP>(L, "scene", this);

	// set global funcs
	xx::Lua_SetGlobalFunc_Log(L);

	// custom bind: versionNumber()
	lua_pushcclosure(L, [](lua_State* L)
	{
		lua_pushinteger(L, xx::Lua_GetMemPool<MP>(L).versionNumber);
		return 1;
	}, 0);
	lua_setglobal(L, "versionNumber");
}

xx::MPtr<Monster1> Scene::CreateMonster1(char const* luacode)
{
	return this->Create<Monster1>(luacode);
}
xx::MPtr<Monster2> Scene::CreateMonster2()
{
	return this->Create<Monster2>();
}

Scene::~Scene()
{
	while (monsters.dataLen)
	{
		monsters.Top()->Release();
	}

	co = nullptr;
	if (L)
	{
		lua_close(L);
		L = nullptr;
	}
}



int Scene::Update()
{
	if (co) if (auto rtv = xx::Lua_Resume(co, &err)) return rtv;

	for (auto i = (int)monsters.dataLen - 1; i >= 0; --i)
	{
		auto& o = monsters[i];
		auto r = o->Update();
		if (r)
		{
			// todo: r < 0 log ?
			o->Release();
		}
	}
	// more for ....

	return 0;
}

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
