#pragma execution_character_set("utf-8")
#pragma once
#include "xx_uv.h"
#include "lua.hpp"

namespace xx
{
	// 将库中一些基础组件搞进 lua 注册表备用. 这些组件的生命周期最好是能覆盖 lua_State 的.
	// 为便于使用, 当前 lua 中只允许存在一份

	// 这些串用于全局名称或注册表 key( 注册表中转为 lightuserdata void* )
	inline const char * const LuaKey_MemPool = "MemPool";
	inline const char * const LuaKey_UvLoop = "UvLoop";
	inline const char * const LuaKey_null = "null";

	// 压 内存池 到 注册表
	inline void LuaRegisterMemPool(lua_State* L, xx::MemPool* mp)
	{
		lua_pushlightuserdata(L, (void*)LuaKey_MemPool);
		lua_pushlightuserdata(L, mp);
		lua_rawset(L, LUA_REGISTRYINDEX);
	}

	// 压 uvloop 到 注册表
	inline void LuaRegisterUvLoop(lua_State* L, xx::UvLoop* uvloop)
	{
		lua_pushlightuserdata(L, (void*)LuaKey_UvLoop);
		lua_pushlightuserdata(L, uvloop);
		lua_rawset(L, LUA_REGISTRYINDEX);
	}


	// 从注册表取出 mp
	inline xx::MemPool* LuaGetMemPool(lua_State* L)
	{
		lua_pushlightuserdata(L, (void*)LuaKey_MemPool);
		lua_rawget(L, LUA_REGISTRYINDEX);
		auto r = (xx::MemPool*)lua_touserdata(L, -1);
		assert(r);
		lua_pop(L, 1);
		return r;
	}

	// 从注册表取出 uvloop
	inline xx::UvLoop* LuaGetUvLoop(lua_State* L)
	{
		lua_pushlightuserdata(L, (void*)LuaKey_UvLoop);
		lua_rawget(L, LUA_REGISTRYINDEX);
		auto r = (xx::UvLoop*)lua_touserdata(L, -1);
		assert(r);
		lua_pop(L, 1);
		return r;
	}


	// 将 null 注册到全局( 赋空值时替代 nil )
	inline void LuaRegisterNull(lua_State* L)
	{
		lua_pushlightuserdata(L, 0);
		lua_setglobal(L, LuaKey_null);
	}



	// 将 userdata(int64) 存的 epoch10m 转为 年,月,日,时,分,秒,周几,年日 8个值返回
	inline int LuaInt64ToDateTime(lua_State* L)
	{
		auto top = lua_gettop(L);
		if (top != 1 || !lua_isuserdata(L, 1))
		{
			luaL_error(L, "Int64ToDateTime args should be userdata(int64)");
		}
		auto v = *(int64_t*)lua_touserdata(L, 1);

		std::tm tm;
		auto tp = std::chrono::system_clock::time_point(std::chrono::duration_cast<std::chrono::system_clock::duration>(std::chrono::duration<long long, std::ratio<1LL, 10000000LL>>(v)));
		auto t = std::chrono::system_clock::to_time_t(tp);
		tm = *localtime(&t);
		lua_pushinteger(L, tm.tm_year + 1900);
		lua_pushinteger(L, tm.tm_mon + 1);
		lua_pushinteger(L, tm.tm_mday);
		lua_pushinteger(L, tm.tm_hour);
		lua_pushinteger(L, tm.tm_min);
		lua_pushinteger(L, tm.tm_sec);
		lua_pushinteger(L, tm.tm_wday);
		lua_pushinteger(L, tm.tm_yday);

		return 8;
	}

	// 压 LuaInt64ToDateTime 函数 到 全局
	inline void LuaRegisterInt64ToDateTime(lua_State* L, xx::UvLoop* uvloop)
	{
		lua_pushstring(L, "Int64ToDateTime");
		lua_pushcclosure(L, LuaInt64ToDateTime, 0);
		lua_rawset(L, LUA_GLOBALSINDEX);
	}



	// 将 userdata(int64) 存的数值转为 string
	inline int LuaInt64ToString(lua_State* L)
	{
		auto top = lua_gettop(L);
		if (top != 1 || !lua_isuserdata(L, 1))
		{
			luaL_error(L, "Int64ToString args should be userdata(int64)");
		}
		auto v = *(int64_t*)lua_touserdata(L, 1);
		auto s = std::to_string(v);

		lua_pushstring(L, s.c_str());

		return 1;
	}
	// 压 LuaInt64ToString 函数 到 全局
	inline void LuaRegisterInt64ToString(lua_State* L, xx::UvLoop* uvloop)
	{
		lua_pushstring(L, "Int64ToString");
		lua_pushcclosure(L, LuaInt64ToDateTime, 0);
		lua_rawset(L, LUA_GLOBALSINDEX);
	}


	// userdata(T**) 转 T*. 检查数据类型标记. 有问题返回 nullptr
	template<typename T>
	inline T* LuaGetPointer(lua_State* L, int idx)
	{
		auto p = (T**)lua_touserdata(L, idx);
		if (!p) return nullptr;
		// 判断 metatable 是否一致
		if (!lua_getmetatable(L, idx))				// ..., mt?
		{
			return nullptr;
		}
		lua_getglobal(L, T::name);					// ..., mt, mt
		auto e = lua_rawequal(L, -1, -2);
		lua_pop(L, 2);
		return e ? *p : nullptr;
	}
}
