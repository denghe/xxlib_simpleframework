#pragma execution_character_set("utf-8")
#pragma once
#include "xx_lua_helpers.h"

namespace xx
{
	struct LuaUvTcpClient : UvTcpClient
	{
		LuaUvTcpClient(lua_State* L) : UvTcpClient(*LuaGetUvLoop(L)) {}
		LuaUvTcpClient(LuaUvTcpClient const&) = delete;
		LuaUvTcpClient& operator=(LuaUvTcpClient const&) = delete;

		// 潜规则类名变量
		inline static const char * const name = "UvTcpClient";

		// 向 lua 映射全局的 UvTcpClient 表/元表
		inline static void LuaRegister(lua_State *L)
		{
			luaL_Reg funcs[] =
			{
				{ "__gc", __gc },
			{ "Create", Create },

			{ "__tostring", __tostring },
			{ nullptr, nullptr }
			};

			lua_createtable(L, 0, _countof(funcs));				// mt
			luaL_setfuncs(L, funcs, 0);							// mt

			/* 设成自查找 for metatable */
			lua_pushvalue(L, -1);								// mt, mt
			lua_setfield(L, -2, "__index");						// mt

			/* 设置保护元表 */
			lua_pushvalue(L, -1);								// mt, mt
			lua_setfield(L, -2, "__metatable");					// mt

			lua_setglobal(L, name);								// 

			/* 在注册表中创建 funcId 到 lua function 的映射表 */
			lua_pushlightuserdata(L, (void*)name);				// lud
			lua_createtable(L, 128, 0);							// lud, typeIdProtos
			lua_rawset(L, LUA_REGISTRYINDEX);					// 
		}


		/**************************************************************************************************/
		// 下面是各种映射函数的具体实现
		/**************************************************************************************************/

		// 析构
		inline static int __gc(lua_State* L)
		{
			auto& self = *(LuaUvTcpClient**)lua_touserdata(L, -1);
			self->~LuaUvTcpClient();
			return 0;
		}

		// 创建, 无参数
		inline static int Create(lua_State* L)
		{
			auto& self = *(LuaUvTcpClient**)lua_newuserdata(L, sizeof(void*));	// ..., ud
			lua_getglobal(L, name);		// ..., ud, mt

			auto mp = LuaGetMemPool(L);				// 从注册表拿出 mp
			if (!(self = mp->Create<LuaUvTcpClient>(L)))
			{
				lua_pop(L, 2);						// ...
				return 0;
			}

			lua_setmetatable(L, -2);				// ..., ud
			return 1;
		}

		inline static LuaUvTcpClient& GetSelf(lua_State* L, int top)
		{
			if (lua_gettop(L) < top)
			{
				luaL_error(L, "less arg nums. expect %d+", top);
			}
			auto p = LuaGetPointer<LuaUvTcpClient>(L, 1);
			if (!p)
			{
				luaL_error(L, "first arg isn't UvTcpClient( forget \":\" ? )");
			}
			return *p;
		}

		inline static int __tostring(lua_State* L)
		{
			auto& self = GetSelf(L, 1);
			auto s = self.mempool->Str();
			s->Append("{ \"state\":\"");
			switch (self.state)
			{
			case UvTcpStates::Disconnected:
				s->Append("Disconnected");
				break;
			case UvTcpStates::Connecting:
				s->Append("Connecting");
				break;
			case UvTcpStates::Connected:
				s->Append("Connected");
				break;
			case UvTcpStates::Disconnecting:
				s->Append("Disconnecting");
				break;
			default:
				s->Append("Unknown(", (int)self.state, ")");
				break;
			}
			s->Append("\" }");
			return 1;
		}
	};
}
