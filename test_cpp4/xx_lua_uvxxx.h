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

		// 向 lua 映射全局的 BBuffer 表/元表
		inline static void LuaRegister(lua_State *L)
		{
			luaL_Reg funcs[] =
			{
				{ "__gc", __gc },
			{ "Create", Create },
			{ "Register", Register },


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

			lua_setglobal(L, LuaKey_BBuffer);					// 

			/* 在注册表中创建 typeId 到 Proto 的映射表 */
			lua_pushlightuserdata(L, (void*)LuaKey_BBuffer);	// lud
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
			LuaAttachTypeTag(L, (void*)LuaKey_BBuffer);
			lua_getglobal(L, LuaKey_BBuffer);		// ..., ud, mt

			auto mp = LuaGetMemPool(L);				// 从注册表拿出 mp
			if (!(self = mp->Create<LuaUvTcpClient>(L)))
			{
				lua_pop(L, 2);						// ...
				return 0;
			}

			lua_setmetatable(L, -2);				// ..., ud
			return 1;
		}

		// 注册 Proto 表. 参数为 Proto 表.
		inline static int Register(lua_State* L)
		{
			if (lua_gettop(L) != 1)					// t
			{
				luaL_error(L, "bad args nums. expect 1");
			}
			if (!lua_istable(L, 1))
			{
				luaL_error(L, "the arg's type must be a proto table");
			}
			lua_getfield(L, 1, "typeId");			// t, int
			auto typeId = lua_tointeger(L, -1);

			lua_pushlightuserdata(L, (void*)LuaKey_BBuffer);	// t, int, name
			lua_rawget(L, LUA_REGISTRYINDEX);		// t, int, typeIdProtos
			lua_pushvalue(L, 1);					// t, int, typeIdProtos, t
			lua_rawseti(L, -2, typeId);				// t, int, typeIdProtos
			lua_pop(L, 2);							// t
			return 0;
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
			std::string s;
			s += "{ \"len\" : " + std::to_string(self.dataLen) + ", \"offset\" : " + std::to_string(self.offset) + ", \"data\" : [";
			for (uint32_t i = 0; i < self.dataLen; ++i)
			{
				s += i ? ", " : " ";
				s += std::to_string((uint8_t)self.buf[i]);
				//s += hexs[self.buf[i] % 16];
				//s += hexs[self.buf[i] >> 4];
			}
			s += self.dataLen ? " ]" : "]";
			s += " }";
			lua_pushlstring(L, s.c_str(), s.size());
			return 1;
		}

	};
}
