#pragma execution_character_set("utf-8")
#include <iostream>
#include "xx_luahelper.h"
#ifdef _WIN32
#include <Windows.h>	// Sleep
#include <mmsystem.h>	// timeBeginPeriod at winmm.lib
#pragma comment(lib, "winmm.lib") 
#endif

// 用宏向 xx_scene.h , xx_scene.hpp 传递类型名
#define SCENE_TYPE_NAME Scene
#define MEMPOOL_TYPE_NAME MP

#include "xx_scene.h"
#include "fsmlua.h"
#include "monster.h"

// 类型要按子--父倒排
typedef xx::MemPool<
	Monster1,					// : MonsterBase
	Monster2,					// : MonsterBase
	MonsterBase,				// : SceneObjBase
	Scene,						// : SceneBase
	SceneObjBase,
	FSMLua,
	xx::List<FSMBase*>,
	xx::List<MonsterBase*>,
	xx::String
> MP;
#include "scene.h"				// scene.h 就是放在 MP 的后面以便继承 MP

#include "xx_scene.hpp"
#include "scene.hpp"
#include "fsmlua.hpp"
#include "monster.hpp"


int main()
{
	Scene scene;
	if (auto rtv = luaL_dofile(scene.L, "init.lua"))
	{
		std::cout << "err code = " << rtv << ", err msg = " << scene.err->C_str() << std::endl;
		system("pause");
	}

	scene.LoadLuaFile("scene.lua");
	if (auto rtv = scene.Run())
	{
		std::cout << "err code = " << rtv << ", err msg = " << scene.err->C_str() << std::endl;
		system("pause");
	}
	return 0;
}
