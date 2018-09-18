#include "xx_uv.h"
#include "lua.hpp"

int Lmain(lua_State *L)
{
	luaL_openlibs(L);

	auto s = R"(
local t
for j = 1, 10, 1 do
	for i = 1, 9999999, 1 do
		t = {}
	end
	for i = 1, 9999999, 1 do
		t = nil
	end
end
)";
	if (int r = luaL_loadstring(L, s))
	{
		std::cout << "r = " << r << ", errmsg = " << lua_tostring(L, -1) << std::endl;
		lua_pop(L, 1);
		return 0;
	}
	lua_call(L, 0, LUA_MULTRET);

	return 0;
}

int main()
{
	auto L = luaL_newstate();
	if (!L) return -1;

	lua_pushcclosure(L, &Lmain, 0);

	if (int r = lua_pcall(L, 0, LUA_MULTRET, 0))
	{
		std::cout << "r = " << r << ", errmsg = " << lua_tostring(L, -1) << std::endl;
		lua_pop(L, 1);
		return r;
	}

	return 0;
}
