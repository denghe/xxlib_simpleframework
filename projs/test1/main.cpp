#pragma execution_character_set("utf-8")

/*********************************************************************************/
// declares
/*********************************************************************************/

#include "xx_luahelper.h"

#include "bases.h"
#include "fsmlua.h"
#include "monster.h"
#include "character.h"
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
#include "character.hpp"


/*********************************************************************************/
// impls
/*********************************************************************************/

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
		std::cout << "err code = " << rtv << ", err msg = " << scene.err.C_str() << std::endl;
		return 0;
	}
	return 0;
}
