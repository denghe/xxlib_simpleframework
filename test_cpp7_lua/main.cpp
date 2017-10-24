#pragma execution_character_set("utf-8")

#include "luaex.h"
#include "lua_bbuffer.h"
#include "std_cout_helper.h"

static int InitLua(lua_State *L)
{
	luaL_openlibs(L);
	Lua_BBuffer::Init(L);
	return 0;
}

static int TestBBuffer(lua_State *L)
{
	if (luaL_loadfile(L, "test2.lua")) return lua_error(L);
	lua_call(L, 0, 0);
	return 0;
}

int main()
{
	Lua_MemPool mp;
	auto L = lua_newstate([](void *ud, void *ptr, size_t osize, size_t nsize)
	{
		return ((Lua_MemPool*)ud)->Realloc(ptr, nsize, osize);
	}, &mp);
	if (!L)
	{
		CoutLine("create lua state failed. not enough memory");
		return -1;
	}

	int rtv = 0;
	auto exec = [&](auto&& fn)->int
	{
		lua_pushcfunction(L, fn);
		rtv = lua_pcall(L, 0, 0, 0);
		if (rtv != LUA_OK)
		{
			CoutLine("pcall rtv = ", rtv, " errmsg = ", lua_tostring(L, -1));
			lua_pop(L, 1);
		}
		return rtv;
	};

	!exec(InitLua) && !exec(TestBBuffer);

	lua_close(L);
	return 0;
}
