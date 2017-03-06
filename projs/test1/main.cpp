#include "xx_luahelper.h"
#include <iostream>
#ifdef _WIN32
#include <Windows.h>	// Sleep
#include <mmsystem.h>	// timeBeginPeriod at winmm.lib
#pragma comment(lib, "winmm.lib") 
#endif

// types defines
/***********************************************************************/

#include "scene.h"
#include "fsmlua.h"
#include "monster.h"

// MP defines
/***********************************************************************/

typedef xx::MemPool<
	FSMLua,
	Scene,
	MonsterBase,
	Monster1,
	SceneObjBase,
	xx::List<FSMBase*>,
	xx::List<SceneObjBase*, true>,
	xx::String
> MP;

// impls
/***********************************************************************/

#include "scene.hpp"
#include "fsmlua.hpp"
#include "monster.hpp"



// main
/***********************************************************************/


int main()
{
	MP mp;
	auto scene = mp.Create<Scene>();
	scene->Run();
	mp.Release(scene);
	return 0;

	//	auto L = xx::Lua_NewState(mp);
	//
	//	//xx::Lua_PushMetatable<MP, Factor>(L);
	//	//lua_pop(L, 1);
	//
	//	//xx::Lua_SetGlobal<MP>(L, "factor", Factor());
	//	auto rtv = luaL_dostring(L, R"%%(
	//
	//
	//)%%");
	//	if (rtv)
	//	{
	//		std::cout << lua_tostring(L, -1) << std::endl;
	//	}
	//
	//	lua_close(L);
	//	std::cin.get();
	//return 0;
}
