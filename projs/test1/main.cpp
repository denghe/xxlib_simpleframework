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

int main()
{
	auto code = R"##(

yield = coroutine.yield

function Sleep( ticks, cond )
	for i = 1, ticks do
		if cond() then return end
		yield()
	end
end

-- todo: more utils

)##";

	MP mp;
	auto scene = mp.Create<Scene>();
	if (auto rtv = luaL_dostring(scene->L, code))
	{
		std::cout << "err code = " << rtv << ", err msg = " << scene->err->C_str() << std::endl;
		system("pause");
	}

	scene->SetLuaCode(R"##(
local m = scene:CreateMonster1([[
	local x = self:x()				-- bak
	Sleep( 5, function() 
		return self:x() - x > 3;	-- x 有较大变化则 sleep 终止
	end )
]])

while true do
	print( "ticks = "..scene:ticks() )
	if m:Ensure() then
		print( "m is alive" )
		m:x( m:x() + 1 )
	else
		print( "m is released" )
	end
	yield()
end
)##");

	if (auto rtv = scene->Run())
	{
		std::cout << "err code = " << rtv << ", err msg = " << scene->err->C_str() << std::endl;
		system("pause");
	}
	mp.Release(scene);
	return 0;
}
