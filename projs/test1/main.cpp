#include "lua_mempool.h"
#include "lua.hpp"

int main()
{
	auto code = R"##(

local bt = os.clock()
for i = 1, 10000000 do
	t = {}
	t.a = i
	t.b = i
	t.c = i
end
print( os.clock() - bt )

)##";
	{
		Lua_MemPool lmp;
		auto L1 = lua_newstate([](void *ud, void *ptr, size_t osize, size_t nsize)
		{
			return ((Lua_MemPool*)ud)->Realloc(ptr, nsize, osize);
		}, &lmp);
		luaL_openlibs(L1);
		luaL_dostring(L1, code);
		lua_close(L1);
	}
	{
		auto L2 = luaL_newstate();
		luaL_openlibs(L2);
		luaL_dostring(L2, code);
		lua_close(L2);
	}
	return 0;
}
