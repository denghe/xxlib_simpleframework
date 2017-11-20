#pragma execution_character_set("utf-8")

#include "lua_xxall.h"
#include "std_cout_helper.h"

#include "../pkg/PKG2_class.h"

int InitLua(lua_State *L)
{
	luaL_openlibs(L);
	Lua_XxNBSocket::LuaInit(L);
	Lua_XxBBuffer::LuaInit(L);
	return 0;
}

int TestBBuffer(lua_State *L)
{
	if (luaL_loadfile(L, "test.lua")) return lua_error(L);
	lua_call(L, 0, 0);
	return 0;
}

int TestLua()
{
	XxMemPool mp;
	auto L = LuaNewState(&mp);
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

int main()
{
	SetUtf8Console();
	XxNBSocket::SockInit();
	TestLua();
	return 0;
}
