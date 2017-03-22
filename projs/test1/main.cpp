#include "xx_luahelper.h"
#include <iostream>

#include "bases.h"
#include "fsmlua.h"
#include "skill.h"
#include "monster.h"
typedef xx::MemPool<
	Monster1,
	Monster2,
	MonsterBase,
	SceneObjBase,
	FSMLua,
	FSMBase,
	UpdateBase,
	Scene
> MP;
#include "scene.h"

#include "bases.hpp"
#include "fsmlua.hpp"
#include "skill.hpp"
#include "monster.hpp"


#ifdef _WIN32
#include <Windows.h>	// Sleep
#include <mmsystem.h>	// timeBeginPeriod at winmm.lib
#pragma comment(lib, "winmm.lib") 
#endif
#include "scene.hpp"

int main()
{

	Scene scene;
	if (auto rtv = luaL_dofile(scene.L, "init.lua"))
	{
		std::cout << "err code = " << rtv << ", err msg = " << lua_tostring(scene.L, -1) << std::endl;
		return 0;
	}

	scene.LoadLuaFile("scene.lua");
	if (auto rtv = scene.Run())
	{
		std::cout << "err code = " << rtv << ", err msg = " << scene.err->C_str() << std::endl;
		return 0;
	}

	return 0;
}
