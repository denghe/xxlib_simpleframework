#include "xx_uv.h"
#include "lua.hpp"
#include "xx_lua_helpers.h"
#include "xx_lua_bbuffer.h"
#include "../pkg/PKG_class.h"

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
	
//	local bb = BBuffer.Create()
//local foo = PKG_Foo.Create()
//foo.refFoo.Reset( foo )
//foo.refFoos = List_Ref_PKG_Foo_.Create()
//foo.refFoos[1] = MakeRef( foo )
//bb:WriteRoot( foo )
//print( bb )
//bb:Clear();
//foo.Release();
//bb:WriteRoot( foo )
//print( bb )
	{
		auto bb = mp.MPCreatePtr<xx::BBuffer>();
		auto foo = mp.MPCreatePtr<PKG::Foo>();
		foo->refFoo = foo;
		foo->refFoos.MPCreate(&mp);
		foo->refFoos->Add(foo);
		bb->WriteRoot(foo);
		std::cout << bb << std::endl;
	}



	xx::UvLoop uvloop(&mp);

	auto L = luaL_newstate();
	if (!L) return -1;
	xx::ScopeGuard sgL([&] {lua_close(L); });

	xx::LuaRegisterMemPool(L, &mp);
	xx::LuaRegisterUvLoop(L, &uvloop);
	xx::LuaRegisterNull(L);
	xx::LuaRegisterInt64ToDateTime(L);
	xx::LuaRegisterInt64ToString(L);
	xx::LuaRegisterMakeRef(L);
	xx::LuaBBuffer::LuaRegister(L);

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
	return Test();
}
