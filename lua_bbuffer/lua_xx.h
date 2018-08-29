#pragma once
#include "lua.h"
#include "lauxlib.h"
#include "lualib.h"
#include <stdint.h>

// 当前所有 luaL_checkstack 都被省略掉了. 料想堆栈深度不会超过 20


// 在 ud ( -1 ) 上附加数据类型标记. 需求 T::name 指针
template<typename T>
inline static void AttachTypeTag(lua_State* L)
{
#if LUA_VERSION_NUM >= 503
	lua_pushlightuserdata(L, (void*)T::name);	// ..., ud, name
	lua_setuservalue(L, -2);					// ..., ud
#endif
}


// userdata 转 T*. 检查数据类型标记
template<typename T>
inline static T* GetPointer(lua_State* L, int idx)
{
	auto p = (T*)lua_touserdata(L, idx);
	if (!p) return nullptr;
#if LUA_VERSION_NUM >= 503
	auto t = lua_getuservalue(L, idx);			// ..., name
	if (t != LUA_TLIGHTUSERDATA || lua_touserdata(L, -1) != T::name)
	{
		lua_pop(L, 1);							// ...
		return nullptr;
	}
	return p;
#else
	// 判断 metatable 是否一致
	if (!lua_getmetatable(L, idx))				// ..., mt
	{
		return nullptr;
	}
	lua_getglobal(L, T::name);					// ..., mt, mt
	auto e = lua_rawequal(L, -1, -2);
	lua_pop(L, 2);
	return e ? p : nullptr;
#endif
}


extern "C"
{
	inline void luaL_setfuncs(lua_State *L, const luaL_Reg *l, int nup)
	{
		luaL_checkstack(L, nup, "too many upvalues");
		for (; l->name; l++) {
			int i;
			for (i = 0; i < nup; i++)  /* Copy upvalues to the top. */
				lua_pushvalue(L, -nup);
			lua_pushcclosure(L, l->func, nup);
			lua_setfield(L, -(nup + 2), l->name);
		}
		lua_pop(L, nup);  /* Remove upvalues. */
	}	
}


#include "lua_xxbbuffer.h"


extern "C"
{
	inline void xxlua_openxx(lua_State* L)
	{
		Lua_XxBBuffer::LuaInit(L);
	}
}
