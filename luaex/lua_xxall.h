#pragma once

#ifndef __cplusplus
#error need cpp compiler
#endif

extern "C"
{
#include "../lua/lua.h"
#include "../lua/lauxlib.h"
#include "../lua/lualib.h"
}
void SetUtf8Console();

#include "xxmempool.h"

// 提供 L 到 mempool 的转换, 以方便写各种构造函数
inline static XxMemPool* GetMemPool(lua_State* L)
{
	XxMemPool* mempool;
	lua_getallocf(L, (void**)&mempool);
	assert(mempool);
	return mempool;
}

// 提供以内存池创建 lua state 的快捷函数
inline static lua_State* LuaNewState(XxMemPool* mp)
{
	return lua_newstate([](void *ud, void *ptr, size_t osize, size_t nsize)
	{
		return ((XxMemPool*)ud)->Realloc(ptr, nsize, osize);
	}, mp);
}

// 带类型校验的 userdata -> T*. 需求 创建时 lua_setuservalue  T::name
template<typename T>
inline static T* GetPointer(lua_State* L, int idx)
{
	auto p = (T*)lua_touserdata(L, idx);
	if (!p) return nullptr;
	auto t = lua_getuservalue(L, idx);
	if (t != LUA_TLIGHTUSERDATA || lua_touserdata(L, -1) != T::name)
	{
		p = nullptr;
	}
	lua_pop(L, 1);
	return p;
}

#include "lua_xxnbsocket.h"
