#include "xx_scene.hpp"

void Scene::SetLuaCode(char const* luacode)
{
	assert(luacode);

	// 创建协程( 在协程中直接用 local self = scene 来访问 )
	co = xx::Lua_RegisterCoroutine(L, this);

	// 加载协程 lua 代码
	luaL_loadstring(co, luacode);
}

Scene::Scene()
{
	L = xx::Lua_NewState(mempool<MP>());
	// err place here
	mempool<MP>().CreateTo(err);

	// LuaBind: Scene
	xx::Lua_PushMetatable<MP, Scene>(L);
	xxLua_BindField(MP, L, Scene, ticks, false);
	xxLua_BindFunc(MP, L, SceneBase, Release, false);
	xxLua_BindFunc(MP, L, Scene, CreateMonster1, false);
	xxLua_BindFunc(MP, L, Scene, CreateMonster2, false);
	lua_pop(L, 1);

	// LuaBind: Monster1
	xx::Lua_PushMetatable<MP, Monster1>(L);
	//xxLua_BindFunc
	//xxLua_BindField
	lua_pop(L, 1);

	// LuaBind: Monster2
	xx::Lua_PushMetatable<MP, Monster2>(L);
	//xxLua_BindFunc
	//xxLua_BindField
	lua_pop(L, 1);

	// set global scene
	xx::Lua_SetGlobal<MP>(L, "scene", this);
}

xx::MPtr<Monster1> Scene::CreateMonster1(char const* luacode)
{
	return this->Create<Monster1>(this, luacode);
}
xx::MPtr<Monster2> Scene::CreateMonster2()
{
	return this->Create<Monster2>(this);
}

Scene::~Scene()
{
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

	for (auto i = (int)this->objs->dataLen - 1; i >= 0; --i)
	{
		auto& o = this->objs->At(i);
		auto r = o->Update();
		if (r)
		{
			// todo: r < 0 log ?
			this->Release(o);
		}
	}

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
