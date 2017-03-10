#include "lua_mempool.h"
#include "lua.hpp"
#include "xx_mempool.h"
#include "xx_luahelper.h"

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
		auto L2 = luaL_newstate();
		luaL_openlibs(L2);
		luaL_dostring(L2, code);
		lua_close(L2);
	}
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
		xx::MemPool<> mp;
		auto L3 = xx::Lua_NewState(mp);
		luaL_dostring(L3, code);
		lua_close(L3);
	}
	return 0;
}
