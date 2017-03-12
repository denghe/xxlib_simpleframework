﻿#pragma execution_character_set("utf-8")

/*********************************************************************************/
// declares
/*********************************************************************************/

#include "xx_luahelper.h"

#include "bases.h"
#include "fsmlua.h"
#include "monster.h"
// 类型要按子--父倒排( 会被 MP 直接创建的或是要做 luabind 的就需要放入这个声明队列 )
typedef xx::MemPool<
	Monster1,					// : MonsterBase
	Monster2,					// : MonsterBase
	MonsterBase,				// : SceneObjBase
	SceneObjBase,
	FSMLua,
	Scene
> MP;
#include "scene.h"				// scene.h 就是放在 MP 的后面以便继承 MP


/*********************************************************************************/
// impls
/*********************************************************************************/

#include <iostream>
#ifdef _WIN32
#include <Windows.h>	// Sleep
#include <mmsystem.h>	// timeBeginPeriod at winmm.lib
#pragma comment(lib, "winmm.lib") 
#endif

#include "bases.hpp"
#include "scene.hpp"
#include "fsmlua.hpp"
#include "monster.hpp"


/*********************************************************************************/
// impls
/*********************************************************************************/

#include <vector>
int main()
{
	xx::MemPoolBase mp;

	{
		xx::Stopwatch sw;
		size_t counter = 0;
		for (int j = 0; j < 9999999; ++j)
		{
			std::vector<int> list;
			for (int i = 0; i < 99; ++i)
			{
				list.push_back(i);
			}
			for (auto& i : list) counter += i;
		}
		std::cout << counter << ", ms = " << sw() << std::endl;
	}

	{
		xx::Stopwatch sw;
		size_t counter = 0;
		for (int j = 0; j < 9999999; ++j)
		{
			xx::List_v<int> list(mp);
			for (int i = 0; i < 99; ++i)
			{
				list->Add(i);
			}
			for (auto& i : *list) counter += i;
		}
		std::cout << counter << ", ms = " << sw() << std::endl;
	}
	return 0;

	//Scene scene;
	//if (auto rtv = luaL_dofile(scene.L, "init.lua"))
	//{
	//	std::cout << "err code = " << rtv << ", err msg = " << scene.err.C_str() << std::endl;
	//	system("pause");
	//}

	//scene.LoadLuaFile("scene.lua");
	//if (auto rtv = scene.Run())
	//{
	//	std::cout << "err code = " << rtv << ", err msg = " << scene.err.C_str() << std::endl;
	//	system("pause");
	//}
	return 0;
}
