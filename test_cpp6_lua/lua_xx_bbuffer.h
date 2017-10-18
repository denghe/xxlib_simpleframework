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
				{ "WriteBool", WriteBool },
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
				{ "WriteBBuffer", WriteBBuffer },

				// todo: class/struct
				// todo: write package ?

				{ "ReadBool", ReadBool },
				{ "ReadInt8", ReadInt8 },
				{ "ReadInt16", ReadInt16 },
				{ "ReadInt32", ReadInt32 },
				{ "ReadInt64", ReadInt64 },
				{ "ReadUInt8", ReadUInt8 },
				{ "ReadUInt16", ReadUInt16 },
				{ "ReadUInt32", ReadUInt32 },
				{ "ReadUInt64", ReadUInt64 },
				{ "ReadFloat", ReadFloat },
				{ "ReadDouble", ReadDouble },
				{ "ReadString", ReadString },
				{ "ReadBBuffer", ReadBBuffer },

				// todo: class/struct

				{ "GetDataLen", GetDataLen },
				{ "GetOffset", GetOffset },
				{ "Dump", Dump },

				// todo: more
				{ "WriteUInt8Zero", WriteUInt8Zero },
				{ "WriteTypeId", WriteTypeId },
				{ "ReadTypeId", ReadTypeId },

				{ "GetOffsetRoot", GetOffsetRoot },
				{ "SetOffsetRoot", SetOffsetRoot },
				// todo: 字典操作函数

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

			// clear table for ptrStore
			lua_pushlightuserdata(L, &bb);				// &ud
			lua_pushnil(L);								// &ud, nil
			lua_rawset(L, LUA_REGISTRYINDEX);			//

			// clear table for idxStore
			lua_pushlightuserdata(L, (char*)&bb + 1);	// &ud
			lua_pushnil(L);								// &ud, nil
			lua_rawset(L, LUA_REGISTRYINDEX);			//

			return 0;
		}

		inline static int Create(lua_State* L)
		{
			auto& bb = *(Lua_BBuffer_v*)lua_newuserdata(L, sizeof(Lua_BBuffer_v));
			auto mp = GetMP(L);							// ud
			new (&bb) Lua_BBuffer_v(*mp);
			lua_getglobal(L, name);						// ud, mt
			lua_setmetatable(L, -2);					// ud

			// table for ptrStore
			lua_pushlightuserdata(L, &bb);				// ud, &ud
			lua_createtable(L, 0, 64);					// ud, &ud, t
			lua_rawset(L, LUA_REGISTRYINDEX);			// ud

			// table for idxStore
			lua_pushlightuserdata(L, (char*)&bb + 1);	// ud, &ud
			lua_createtable(L, 0, 64);					// ud, &ud, t
			lua_rawset(L, LUA_REGISTRYINDEX);			// ud

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

		inline static int WriteBool(lua_State* L)
		{
			auto& self = GetSelf(L, 2);
			if (!lua_isboolean(L, 2))
			{
				luaL_error(L, "the arg's type must be a bool");
			}
			self->Write(lua_toboolean(L, 2) != 0);
			return 0;
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

		inline static int WriteInt8(lua_State* L) { return WriteNum<int8_t>(L); }
		inline static int WriteInt16(lua_State* L) { return WriteNum<int16_t>(L); }
		inline static int WriteInt32(lua_State* L) { return WriteNum<int32_t>(L); }
		inline static int WriteInt64(lua_State* L) { return WriteNum<int64_t>(L); }
		inline static int WriteUInt8(lua_State* L) { return WriteNum<uint8_t>(L); }
		inline static int WriteUInt16(lua_State* L) { return WriteNum<uint16_t>(L); }
		inline static int WriteUInt32(lua_State* L) { return WriteNum<uint32_t>(L); }
		inline static int WriteUInt64(lua_State* L) { return WriteNum<uint64_t>(L); }
		inline static int WriteFloat(lua_State* L) { return WriteNum<float>(L); }
		inline static int WriteDouble(lua_State* L) { return WriteNum<double>(L); }

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


		// nil 写入 0, 非 nil 写入 typeId(1) + 长度 + 内容
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

		// nil 写入 0, 非 nil 写入 typeId(2) + 长度 + 内容
		inline static int WriteBBuffer(lua_State* L)
		{
			auto& self = GetSelf(L, 2);
			if (lua_isnil(L, 2))
			{
				self->Write((uint8_t)0);
				return 0;
			}
			if (!lua_isuserdata(L, 2))	// 这里先这样简单检查. 暂不检查其是否具备特定元表
			{
				luaL_error(L, "the arg's type must be a BBuffer / nil");
			}

			auto& bb = *(Lua_BBuffer_v*)lua_touserdata(L, 2);
			self->Write((uint8_t)2);
			bb->ToBBuffer(*self);
			return 0;
		}

		inline static int Dump(lua_State* L)
		{
			auto& self = GetSelf(L, 1);
			xx::String_v s(self->mempool());
			self->ToString(s.instance);
			lua_pushlstring(L, s->C_str(), s->dataLen);
			return 1;
		}


		inline static int ReadBool(lua_State* L)
		{
			auto& self = GetSelf(L, 1);
			bool v;
			if (self->Read(v))
			{
				luaL_error(L, "read bool error!");
			}
			lua_pushboolean(L, v);
			return 1;
		}

		template<typename T>
		inline static int ReadNum(lua_State* L)
		{
			static_assert(std::is_arithmetic_v<T>);
			auto& self = GetSelf(L, 1);
			T v;
			if (self->Read(v))
			{
				luaL_error(L, "read num error!");
			}
			if constexpr(std::is_integral_v<T>)
			{
				lua_pushinteger(L, (lua_Integer)v);
			}
			else
			{
				lua_pushnumber(L, (lua_Number)v);
			}
			return 1;
		}

		inline static int ReadInt8(lua_State* L) { return ReadNum<int8_t>(L); }
		inline static int ReadInt16(lua_State* L) { return  ReadNum<int16_t>(L); }
		inline static int ReadInt32(lua_State* L) { return  ReadNum<int32_t>(L); }
		inline static int ReadInt64(lua_State* L) { return  ReadNum<int64_t>(L); }
		inline static int ReadUInt8(lua_State* L) { return  ReadNum<uint8_t>(L); }
		inline static int ReadUInt16(lua_State* L) { return ReadNum<uint16_t>(L); }
		inline static int ReadUInt32(lua_State* L) { return ReadNum<uint32_t>(L); }
		inline static int ReadUInt64(lua_State* L) { return ReadNum<uint64_t>(L); }
		inline static int ReadFloat(lua_State* L) { return  ReadNum<float>(L); }
		inline static int ReadDouble(lua_State* L) { return ReadNum<double>(L); }

		inline static int ReadString(lua_State* L)
		{
			auto& self = GetSelf(L, 1);
			uint8_t typeId;
			if (self->Read(typeId))
			{
				luaL_error(L, "read string typeId error");
			}
			if (typeId)
			{
				if (typeId != 1)
				{
					luaL_error(L, "read string typeId error: typeId != 1");
				}
				xx::String_v s(self->mempool());
				if (self->Read(s))
				{
					luaL_error(L, "read string content error");
				}
				lua_pushlstring(L, s->C_str(), s->dataLen);
			}
			else
			{
				lua_pushnil(L);
			}
			return 1;
		}

		inline static int ReadBBuffer(lua_State* L)
		{
			auto& self = GetSelf(L, 1);
			uint8_t typeId;
			if (self->Read(typeId))
			{
				luaL_error(L, "read BBuffer typeId error");
			}
			if (typeId)
			{
				if (typeId != 2)
				{
					luaL_error(L, "read BBuffer typeId error: typeId != 2");
				}
				Create(L);											// self, bb
				auto& bb = *(Lua_BBuffer_v*)lua_touserdata(L, 2);
				if (bb->FromBBuffer(*self))
				{
					lua_pop(L, 1);									// self
					luaL_error(L, "read string content error");
				}
			}
			else
			{
				lua_pushnil(L);
			}
			return 1;
		}


		inline static int WriteUInt8Zero(lua_State* L)
		{
			auto& self = GetSelf(L, 2);
			self->Write((uint8_t)0);
			return 0;
		}

		inline static int WriteTypeId(lua_State* L)
		{
			return WriteUInt16(L);
		}
		inline static int ReadTypeId(lua_State* L)
		{
			return ReadUInt16(L);
		}





		inline static int GetOffsetRoot(lua_State* L)
		{
			auto& self = GetSelf(L, 1);
			lua_pushinteger(L, self->offsetRoot);
			return 1;
		}

		inline static int SetOffsetRoot(lua_State* L)
		{
			auto& self = GetSelf(L, 2); 
			int isnum;
			self->offsetRoot = (uint32_t)lua_tointegerx(L, 2, &isnum);
			if (!isnum)
			{
				luaL_error(L, "the arg's type must be a integer / number");
			}
			return 0;
		}

		// 只需要传 key( string, bbuffer, table ). value 由 dataLen - offsetRoot 算出来
		inline static int WriteOffset(lua_State* L)
		{
			auto& self = GetSelf(L, 2);
			switch (lua_type(L, 2))
			{
			case LUA_TSTRING:
			case LUA_TTABLE:
			case LUA_TUSERDATA:
				lua_pushlightuserdata(L, &self);						// bb, v, k
				lua_rawget(L, LUA_REGISTRYINDEX);						// bb, v, t
				lua_pushvalue(L, 2);									// bb, v, t, v
				lua_pushinteger(L, self->dataLen - self->offsetRoot);	// bb, v, t, v, offset
				lua_rawset(L, 3);										// bb, v, t
				lua_pop(L, 1);											// bb, v
				break;
			default:
				luaL_error(L, "the arg's type must be a string / table / BBuffer");
			}
			return 0;
		}

		// todo: readOffset
	};
};
