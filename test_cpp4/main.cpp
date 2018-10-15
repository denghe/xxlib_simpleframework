#include "xx_uv.h"
#include "lua.hpp"
#include "xx_lua_helpers.h"
#include "xx_lua_bbuffer.h"
#include "xx_lua_uvloop.h"
#include "xx_lua_uvtcpclient.h"

int Lmain(lua_State *L)
{
	luaL_openlibs(L);

	if (int r = luaL_loadfile(L, "src.lua"))
	{
		std::cout << "r = " << r << ", errmsg = " << lua_tostring(L, -1) << std::endl;
		lua_pop(L, 1);
		return 0;
	}
	lua_call(L, 0, LUA_MULTRET);

	return 0;
}

int Test()
{
	xx::MemPool mp;
	xx::UvLoop uvloop(&mp);
	uvloop.InitRpcManager(1000, 10);

	auto L = luaL_newstate();
	if (!L) return -1;
	xx::ScopeGuard sgL([&] {lua_close(L); });

	xx::LuaRegisterMemPool(L, &mp);
	xx::LuaRegisterUvLoop(L, &uvloop);
	xx::LuaRegisterNull(L);
	xx::LuaRegisterInt64ToDateTime(L);
	xx::LuaRegisterInt64ToString(L);
	xx::LuaRegisterMakeRef(L);
	xx::LuaUvLoop::LuaRegister(L);
	xx::LuaBBuffer::LuaRegister(L);
	xx::LuaUvTcpClient::LuaRegister(L);

	lua_pushcclosure(L, &Lmain, 0);

	if (int r = lua_pcall(L, 0, LUA_MULTRET, 0))
	{
		std::cout << "r = " << r << ", errmsg = " << lua_tostring(L, -1) << std::endl;
		lua_pop(L, 1);
		return r;
	}

	return 0;
}

int main()
{
	Test();
	std::cin.get();
	//return Test();
}
