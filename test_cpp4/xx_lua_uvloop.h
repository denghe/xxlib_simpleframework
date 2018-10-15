#pragma execution_character_set("utf-8")
#pragma once
#include "xx_lua_helpers.h"

namespace xx
{
	struct LuaUvLoop
	{
		LuaUvLoop(LuaUvLoop const&) = delete;
		LuaUvLoop& operator=(LuaUvLoop const&) = delete;

		// 潜规则类名变量
		inline static const char * const name = "UvLoop";

		// 向 lua 映射全局的 UvTcpClient 表/元表
		inline static void LuaRegister(lua_State *L)
		{
			luaL_Reg funcs[] =
			{
				{ "Run", Run_ },								// 无阻塞执行 uv 底层逻辑

				// todo: dns resolver 相关

			{ nullptr, nullptr }
			};

			lua_createtable(L, 0, _countof(funcs));				// mt
			luaL_setfuncs(L, funcs, 0);							// mt
			lua_setglobal(L, name);								// 
		}

		inline static int Run_(lua_State *L)
		{
			auto loop = LuaGetUvLoop(L);
			auto r = loop->Run(xx::UvRunMode::NoWait);
			lua_pushinteger(L, r);
			return 1;
		}
	};
}
