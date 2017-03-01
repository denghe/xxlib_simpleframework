#include "xx_luahelper.h"
#include <iostream>
using namespace xx;
int main()
{

	auto code = R"~~(
for i = 1, 10000000, 1 do
	local t = {}
	t[i] = t
end
)~~";

	// 传统工艺
	auto L1 = luaL_newstate();
	luaL_openlibs(L1);

	// 内存池加持
	MemPoolBase mpb;
	auto L2 = lua_newstate([](void *ud, void *ptr, size_t osize, size_t nsize)
	{
		return ((MemPoolBase*)ud)->Realloc(ptr, nsize, osize);
	}, &mpb);
	luaL_openlibs(L2);

	Stopwatch sw;
	{
		auto rtv = luaL_dostring(L1, code);
		assert(!rtv);
	}
	std::cout << "L1 elapsed = " << sw() << std::endl;
	{
		auto rtv = luaL_dostring(L2, code);
		assert(!rtv);
	}
	std::cout << "L2 elapsed = " << sw() << std::endl;

	lua_close(L1);
	lua_close(L2);

	return 0;
}
