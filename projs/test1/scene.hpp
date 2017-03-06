#include "xx_scene.hpp"

Scene::Scene()
{
	L = xx::Lua_NewState(mempool<MP>());

	// LuaBind: Scene
	xx::Lua_PushMetatable<MP, Scene>(L);
	//xxLua_BindFunc
	//xxLua_BindField
	lua_pop(L, 1);

	// LuaBind: Monster1
	xx::Lua_PushMetatable<MP, Monster1>(L);
	//xxLua_BindFunc
	//xxLua_BindField
	lua_pop(L, 1);

	// set global scene
	xx::Lua_SetGlobal<MP>(L, "scene", this);
}

Scene::~Scene()
{
	if (err)
	{
		err->Release();
		err = nullptr;
	}
	// co 不需要理会
	if (L)
	{
		lua_close(L);
		L = nullptr;
	}
}

void Scene::Update()
{
	std::cout << ticks << std::endl;
}

int Scene::Run()
{
#ifdef _WIN32
	timeBeginPeriod(1);
#endif
	int64_t accumulatMS = 0;
	xx::Stopwatch sw;
	while (running)
	{
		auto durationMS = sw();
		if (durationMS > msPerFrame)
		{
			// todo: 日志超长帧
			durationMS = msPerFrame;
		}

		accumulatMS += durationMS;
		bool executed = false;
		while (running && accumulatMS >= msPerFrame)
		{
			executed = true;
			accumulatMS -= msPerFrame;

			Update();
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
