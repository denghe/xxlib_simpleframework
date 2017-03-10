#include "xx_scene.hpp"

void Scene::LoadLuaFile(char const* fn)
{
	assert(fn);

	// 创建协程( 在协程中直接用 local self = scene 来访问 )
	co = xx::Lua_RegisterCoroutine(L, this);

	// 加载协程 lua 代码
	luaL_loadfile(co, fn);
}

Scene::Scene()
{
	decltype(auto) mp = mempool<MP>();

	mp.CreateTo(monsters);
	mp.CreateTo(err);
	// more...

	L = xx::Lua_NewState(mp);

	// LuaBind: Scene
	xx::Lua_PushMetatable<MP, Scene>(L);
	xxLua_BindField(MP, L, Scene, ticks, false);
	xxLua_BindFunc(MP, L, Scene, CreateMonster1, false);
	xxLua_BindFunc(MP, L, Scene, CreateMonster2, false);
	lua_pop(L, 1);

	// LuaBind: MonsterBase( 在 MP 中这是 Monster1/2 的直接基类 )
	xx::Lua_PushMetatable<MP, MonsterBase>(L);
	xx::Lua_BindFunc_Ensure<MP>(L);						// 重复注册以提速, 方便派生类复制
	xx::Lua_BindFunc_Release<MP>(L);					// 同上
	xxLua_BindField(MP, L, MonsterBase, x, true);
	lua_pop(L, 1);


	// 如果不直接在最上层 mt 中 bind, 会导致 lua mt 多级查找. 1亿次Ensure 的结果: 1层 5.2   2层 6.2   3层 8.22 秒

	// LuaBind: Monster1
	xx::Lua_PushMetatable<MP, Monster1>(L);
	xx::Lua_CloneParentMetatable<MP, Monster1>(mp, L);
	lua_pop(L, 1);

	// LuaBind: Monster2
	xx::Lua_PushMetatable<MP, Monster2>(L);
	xx::Lua_CloneParentMetatable<MP, Monster2>(mp, L);
	lua_pop(L, 1);

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
	return mempool<MP>().Create<Monster1>(this, luacode);
}
xx::MPtr<Monster2> Scene::CreateMonster2()
{
	return mempool<MP>().Create<Monster2>(this);
}

Scene::~Scene()
{
	if (monsters)
	{
		while (monsters->dataLen)
		{
			monsters->Top()->Release();
		}
		monsters->Release();
		monsters = nullptr;
	}
	// more....

	if (err)
	{
		err->Release();
		err = nullptr;
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
	if (co) if (auto rtv = xx::Lua_Resume(co, err)) return rtv;

	for (auto i = (int)this->monsters->dataLen - 1; i >= 0; --i)
	{
		auto& o = this->monsters->At(i);
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
