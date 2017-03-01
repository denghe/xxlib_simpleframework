#include "lua.hpp"
#include "xx_mempool.h"
#include <iostream>
int main()
{

	auto code = R"~~(
local t = { 1,1,1 }
t[1] = 1
t[2] = 2
t[3] = 3
t.x = 1
t.y = 2
t.z = 3
)~~";

	// 传统工艺
	auto L1 = luaL_newstate();
	luaL_openlibs(L1);

	// 内存池加持
	xx::MemPoolBase mpb;
	auto L2 = lua_newstate([](void *ud, void *ptr, size_t osize, size_t nsize)
	{
		return ((xx::MemPoolBase*)ud)->Realloc(ptr, nsize, osize);
	}, &mpb);
	luaL_openlibs(L2);

	xx::Stopwatch sw;
	for (int i = 0; i < 99999; ++i)
	{
		auto rtv = luaL_dostring(L1, code);
		assert(!rtv);
	}
	std::cout << "L1 elapsed = " << sw() << std::endl;
	for (int i = 0; i < 99999; ++i)
	{
		auto rtv = luaL_dostring(L2, code);
		assert(!rtv);
	}
	std::cout << "L2 elapsed = " << sw() << std::endl;

	lua_close(L1);
	lua_close(L2);

	return 0;
}
