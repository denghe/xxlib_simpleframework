#pragma execution_character_set("utf-8")
#include <iostream>
#include "xx_luahelper.h"
#ifdef _WIN32
#include <Windows.h>	// Sleep
#include <mmsystem.h>	// timeBeginPeriod at winmm.lib
#pragma comment(lib, "winmm.lib") 
#endif

#include "scene.h"
#include "fsmlua.h"
#include "monster.h"

// 类型要按子--父倒排
typedef xx::MemPool<
	Monster1,					// : MonsterBase
	Monster2,					// : MonsterBase
	MonsterBase,				// : SceneObjBase
	Scene,						// : SceneBase
	SceneObjBase,
	SceneBase,
	FSMLua,
	xx::List<FSMBase*>,
	xx::List<MonsterBase*>,
	xx::String
> MP;

#include "scene.hpp"
#include "fsmlua.hpp"
#include "monster.hpp"

#include "ajson.hpp"
#include <vector>
int main()
{
	MP mp;
	auto scene = mp.Create<Scene>();
	if (auto rtv = luaL_dofile(scene->L, "init.lua"))
	{
		std::cout << "err code = " << rtv << ", err msg = " << scene->err->C_str() << std::endl;
		system("pause");
	}

	scene->LoadLuaFile("scene.lua");
	if (auto rtv = scene->Run())
	{
		std::cout << "err code = " << rtv << ", err msg = " << scene->err->C_str() << std::endl;
		system("pause");
	}
	mp.Release(scene);
	return 0;
}
