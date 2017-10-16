#pragma execution_character_set("utf-8")
#pragma once

#include "xx_bbuffer.h"
namespace xx
{
	struct Lua_BBuffer;
	typedef Dock<Lua_BBuffer> Lua_BBuffer_v;
	struct Lua_BBuffer : BBuffer
	{
		constexpr static const char* name = "BBuffer";

		// 向 lua 映射全局的 BBuffer 表/元表
		inline static int Init(lua_State *L)
		{
			luaL_Reg funcs[] =
			{
				{ "__gc", __gc },
				{ "Create", Create },
				{ "WriteInt8", WriteInt8 },
				{ "WriteInt16", WriteInt16 },
				{ "WriteInt32", WriteInt32 },
				{ "WriteInt64", WriteInt64 },
				{ "WriteUInt8", WriteUInt8 },
				{ "WriteUInt16", WriteUInt16 },
				{ "WriteUInt32", WriteUInt32 },
				{ "WriteUInt64", WriteUInt64 },
				{ "WriteFloat", WriteFloat },
				{ "WriteDouble", WriteDouble },
				{ "WriteString", WriteString },

				{ "GetDataLen", GetDataLen },
				{ "GetOffset", GetOffset },

				// todo: more

				{ nullptr, nullptr }
			};
			lua_createtable(L, 0, _countof(funcs));		// mt
			luaL_setfuncs(L, funcs, 0);					// mt
			lua_pushvalue(L, -1);					    // mt, mt
			lua_setfield(L, -2, "__index");				// mt
			lua_setglobal(L, name);						// 
			return 0;
		}

		inline static MemPool* GetMP(lua_State* L)
		{
			MemPool* mp;
			lua_getallocf(L, (void**)&mp);
			return mp;
		}

		inline static int __gc(lua_State* L)
		{
			auto& bb = *(Lua_BBuffer_v*)lua_touserdata(L, -1);
			bb.~Lua_BBuffer_v();
			return 0;
		}

		inline static int Create(lua_State* L)
		{
			auto& bb = *(Lua_BBuffer_v*)lua_newuserdata(L, sizeof(Lua_BBuffer_v));
			auto mp = GetMP(L);							// ud
			new (&bb) Lua_BBuffer_v(*mp);
			lua_getglobal(L, name);						// ud, mt
			lua_setmetatable(L, -2);					// ud
			return 1;
		}

		inline static Lua_BBuffer_v& GetSelf(lua_State* L, int top)
		{
			if (lua_gettop(L) != top)
			{
				luaL_error(L, "bad args nums");
			}
			auto selfptr = (Lua_BBuffer_v*)lua_touserdata(L, 1);
			if (!selfptr)
			{
				luaL_error(L, "first arg isn't BBuffer( forget \":\" ? )");
			}
			return *selfptr;
		}

		template<typename T>
		inline static int WriteNum(lua_State* L)
		{
			static_assert(std::is_arithmetic_v<T>);
			auto& self = GetSelf(L, 2);
			int isnum;
			T v;
			if constexpr(!std::is_floating_point_v<T>)
			{
				v = (T)lua_tointegerx(L, 2, &isnum);
			}
			else
			{
				v = (T)lua_tonumberx(L, 2, &isnum);
			}
			if (!isnum)
			{
				luaL_error(L, "the arg's type must be a integer / number");
			}
			self->Write(v);
			return 0;
		}

		inline static int WriteInt8(lua_State* L)
		{
			return WriteNum<int8_t>(L);
		}
		inline static int WriteInt16(lua_State* L)
		{
			return WriteNum<int16_t>(L);
		}
		inline static int WriteInt32(lua_State* L)
		{
			return WriteNum<int32_t>(L);
		}
		inline static int WriteInt64(lua_State* L)
		{
			return WriteNum<int64_t>(L);
		}

		inline static int WriteUInt8(lua_State* L)
		{
			return WriteNum<uint8_t>(L);
		}
		inline static int WriteUInt16(lua_State* L)
		{
			return WriteNum<uint16_t>(L);
		}
		inline static int WriteUInt32(lua_State* L)
		{
			return WriteNum<uint32_t>(L);
		}
		inline static int WriteUInt64(lua_State* L)
		{
			return WriteNum<uint64_t>(L);
		}

		inline static int WriteFloat(lua_State* L)
		{
			return WriteNum<float>(L);
		}
		inline static int WriteDouble(lua_State* L)
		{
			return WriteNum<double>(L);
		}

		inline static int GetDataLen(lua_State* L)
		{
			auto& self = GetSelf(L, 1);
			lua_pushinteger(L, self->dataLen);
			return 1;
		}
		inline static int GetOffset(lua_State* L)
		{
			auto& self = GetSelf(L, 1);
			lua_pushinteger(L, self->offset);
			return 1;
		}

		// 这里按写入 xx::String* 的编码规则来写. nil 写入 0, 非 nil 写入 typeId(1) + 长度 + 内容
		inline static int WriteString(lua_State* L)
		{
			auto& self = GetSelf(L, 2);
			if (lua_isnil(L, 2))
			{
				self->Write((uint8_t)0);
				return 0;
			}
			if (!lua_isstring(L, 2))
			{
				luaL_error(L, "the arg's type must be a string / nil");
			}
			size_t dataLen;
			auto v = lua_tolstring(L, 2, &dataLen);
			self->Reserve(self->dataLen + 6 + (uint32_t)dataLen);
			self->Write((uint8_t)1);
			self->Write(dataLen);
			if (dataLen)
			{
				memcpy(self->buf + self->dataLen, v, dataLen);
				self->dataLen += (uint32_t)dataLen;
			}
			return 0;
		}
	};
};
