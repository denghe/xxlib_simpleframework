#pragma execution_character_set("utf-8")
#pragma once

// todo: 各种 check stack 补足
// todo: 有空直接将序列化的代码整合过来, 该类不再继承, 配合 lua_mempool 做独立版

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
				{ "Register", Register },

				{ "WriteBoolean", WriteBoolean },
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
				{ "WriteRoot", WriteRoot },
				{ "WriteObject", WriteObject },

				{ "ReadBoolean", ReadBoolean },
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
				{ "ReadRoot", ReadRoot },
				{ "ReadObject", ReadObject },

				{ "GetDataLen", GetDataLen },
				{ "GetOffset", GetOffset },
				{ "Dump", Dump },
				// todo: write package ?

				{ nullptr, nullptr }
			};
			lua_createtable(L, 0, _countof(funcs));	// mt
			luaL_setfuncs(L, funcs, 0);				// mt

			// 设成自查找 for metatable
			lua_pushvalue(L, -1);					// mt, mt
			lua_setfield(L, -2, "__index");			// mt

			// 用 lud: 0 来代表空值占位符的元素
			lua_pushlightuserdata(L, 0);			// mt, lud
			lua_setfield(L, -2, "null");			// mt			

			lua_setglobal(L, name);					// 

			// 在注册表中创建 typeId 到 Proto 的映射表	
			lua_pushlightuserdata(L, (void*)name);	// lud
			lua_createtable(L, 128, 0);				// lud, typeIdProtos
			lua_rawset(L, LUA_REGISTRYINDEX);		// 

			return 0;
		}


		// 读 lua_State 的内存分配函数绑定来得到内存分配器的指针
		inline static MemPool* GetMP(lua_State* L)
		{
			MemPool* mp;
			lua_getallocf(L, (void**)&mp);
			return mp;
		}

		// 析构
		inline static int __gc(lua_State* L)
		{
			auto& self = *(Lua_BBuffer_v*)lua_touserdata(L, -1);
			self.~Lua_BBuffer_v();
			self->ReleasePtrDict(L);				// 异常: 因为是移除, 应该不会抛
			self->ReleaseIdxDict(L);
			return 0;
		}

		// 创建, 无参数
		inline static int Create(lua_State* L)
		{
			if (lua_gettop(L) != 0)					// t
			{
				luaL_error(L, "bad args nums. expect 0");
			}
			auto& self = *(Lua_BBuffer_v*)lua_newuserdata(L, sizeof(Lua_BBuffer_v));	//	异常: 无副作用
			auto mp = GetMP(L);						// ud
			lua_getglobal(L, name);					// ud, mt							异常: 未执行构造函数, 不需要析构
			new (&self) Lua_BBuffer_v(*mp);			//									异常: 因为未 bind 元表, 不会执行到析构
			lua_setmetatable(L, -2);				// ud
			return 1;
		}

		// 注册 Proto 表. 参数为 Proto 表.
		inline static int Register(lua_State* L)
		{
			if (lua_gettop(L) != 1)				// t
			{
				luaL_error(L, "bad args nums. expect 1");
			}
			if (!lua_istable(L, 1))
			{
				luaL_error(L, "the arg's type must be a proto table");
			}
			lua_getfield(L, 1, "typeId");			// t, int
			int isnum;
			auto typeId = lua_tointegerx(L, -1, &isnum);
			if (!isnum)
			{
				luaL_error(L, "the proto's typeId must be a int");
			}

			lua_rawgetp(L, LUA_REGISTRYINDEX, (void*)name);	// t, int, typeIdProtos
			lua_pushvalue(L, 1);					// t, int, typeIdProtos, t
			lua_rawseti(L, -2, typeId);				// t, int, typeIdProtos
			lua_pop(L, 2);							// t
			return 0;
		}

		inline static Lua_BBuffer_v& GetSelf(lua_State* L, int top)
		{
			if (lua_gettop(L) != top)
			{
				luaL_error(L, "bad args nums. expect %d", top);
			}
			auto selfptr = (Lua_BBuffer_v*)lua_touserdata(L, 1);
			if (!selfptr)
			{
				luaL_error(L, "first arg isn't BBuffer( forget \":\" ? )");
			}
			return *selfptr;
		}

		inline static int WriteBoolean(lua_State* L)
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


		inline static int ReadBoolean(lua_State* L)
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





		inline void CreatePtrDict(lua_State* L)
		{
			lua_createtable(L, 0, 64);
			lua_rawsetp(L, LUA_REGISTRYINDEX, this);
		}
		inline void CreateIdxDict(lua_State* L)
		{
			lua_createtable(L, 0, 64);
			lua_rawsetp(L, LUA_REGISTRYINDEX, (char*)this + 1);
		}

		inline void ReleasePtrDict(lua_State* L)
		{
			lua_pushnil(L);
			lua_rawsetp(L, LUA_REGISTRYINDEX, this);
		}
		inline void ReleaseIdxDict(lua_State* L)
		{
			lua_pushnil(L);
			lua_rawsetp(L, LUA_REGISTRYINDEX, (char*)this + 1);
		}

		inline void PushPtrDict(lua_State* L)
		{
			lua_rawgetp(L, LUA_REGISTRYINDEX, this);
		}
		inline void PushIdxDict(lua_State* L)
		{
			lua_rawgetp(L, LUA_REGISTRYINDEX, (char*)this + 1);
		}


		// 针对 string, bbuffer, table, 写 dataLen - offsetRoot 到 buf, 返回是否第一次写入
		inline bool WriteOffset(lua_State* L)
		{
			switch (lua_type(L, -1))				// ..., o
			{
			case LUA_TSTRING:
			case LUA_TTABLE:
			case LUA_TUSERDATA:
				PushPtrDict(L);						// ..., o, dict					定位到注册表中的 ptrDict
				lua_pushvalue(L, -2);				// ..., o, dict, o				查询当前对象是否已经记录过
				lua_rawget(L, -2);					// ..., o, dict, nil/offset
				if (lua_isnil(L, -1))				// 如果未记录则记录 + 写buf
				{
					auto offset = dataLen - offsetRoot;
					Write(offset);					// 写buf

					lua_pop(L, 1);					// ..., o, dict
					lua_pushvalue(L, -2);			// ..., o, dict, o
					lua_pushinteger(L, offset);		// ..., o, dict, o, offset
					lua_rawset(L, -3);				// ..., o, dict
					lua_pop(L, 1);					// ..., o
					return true;					// 返回首次出现
				}
				else								// 记录过则取其 value 来写buf
				{
					auto offset = (uint32_t)lua_tointeger(L, -1);
					Write(offset);					// 写buf
					lua_pop(L, 2);					// ..., o
					return false;
				}
			default:
				luaL_error(L, "the arg's type must be a string / table / BBuffer");
				return false;
			}
		}

		inline static int WriteObject(lua_State* L)
		{
			auto& self = GetSelf(L, 2);				// bb, o
			self->WriteObject_(L);					// bb, o
			return 0;
		}


		// 写对象到 buf. 如果为空就写 typeId 0, 否则写 typeId, offset[, content ]
		inline void WriteObject_(lua_State* L)
		{
			if (lua_isnil(L, -1) ||					// bb, o
				lua_islightuserdata(L, -1) && (size_t)lua_touserdata(L, -1) == 0)
			{
				Write((uint8_t)0);
			}
			else
			{
				lua_getfield(L, 2, "__proto");		// bb, o, proto
				lua_getfield(L, 3, "typeId");		// bb, o, proto, typeId
				auto typeId = (uint16_t)lua_tonumber(L, -1);
				Write(typeId);
				lua_pop(L, 1);						// bb, o, proto
				lua_insert(L, 2);					// bb, proto, o
				if (WriteOffset(L))					// bb, proto, o
				{
					lua_insert(L, 2);				// bb, o, proto
					lua_getfield(L, 3, "ToBBuffer");// bb, o, proto, func
					lua_pushvalue(L, 1);			// bb, o, proto, func, bb
					lua_pushvalue(L, 2);			// bb, o, proto, func, bb, o
					lua_call(L, 2, 0);				// bb, o, proto
					lua_pop(L, 1);					// bb, o
				}
			}
		}

		inline static int WriteRoot(lua_State* L)
		{
			auto& self = GetSelf(L, 2);				// bb, o
			self->offsetRoot = self->dataLen;
			self->CreatePtrDict(L);

			self->WriteObject_(L);					// bb, o

			self->ReleasePtrDict(L);
			return 0;
		}





		inline static int ReadRoot(lua_State* L)
		{
			auto& self = GetSelf(L, 1);				// bb
			self->offsetRoot = self->offset;
			self->CreateIdxDict(L);

			self->ReadObject_(L);					// bb, rtv

			self->ReleaseIdxDict(L);
			return 1;
		}

		inline static int ReadObject(lua_State* L)
		{
			auto& self = GetSelf(L, 1);				// bb
			self->ReadObject_(L);					// bb, rtv
			return 1;
		}

		inline void ReadObject_(lua_State* L)
		{
			uint16_t typeId;
			if (Read(typeId))
			{
				luaL_error(L, "buf read offset: %d, read typeId error", offset);
			}
			if (!typeId)
			{
				lua_pushlightuserdata(L, 0);		// bb, null
				return;
			}

			// todo: 判断 typeId 是否为 1 或 2 ?

			lua_rawgetp(L, LUA_REGISTRYINDEX, (void*)name);	// bb, typeIdProtos
			lua_rawgeti(L, -1, typeId);				// bb, typeIdProtos, proto?
			if (lua_isnil(L, -1))
			{
				luaL_error(L, "buf read offset: %d, invalid typeId: %d", offset, typeId);
			}

			uint32_t ptr_offset = 0, bb_offset_bak = offset - offsetRoot;
			if (Read(ptr_offset))
			{
				luaL_error(L, "buf read offset: %d, read offset error", offset);
			}
			if (ptr_offset == bb_offset_bak)		// 首次出现, 后面是内容
			{
				lua_remove(L, 2);					// bb, proto
				lua_getfield(L, 2, "Create");		// bb, proto, Create
				lua_call(L, 0, 1);					// bb, proto, o
				lua_insert(L, 2);					// bb, o, proto
				PushIdxDict(L);						// bb, o, proto, dict
				lua_pushvalue(L, -3);				// bb, o, proto, dict, o
				lua_rawseti(L, -2, ptr_offset);		// bb, o, proto, dict
				lua_pop(L, 1);						// bb, o, proto
				lua_getfield(L, -1, "FromBBuffer");	// bb, o, proto, FromBBuffer
				lua_pushvalue(L, 1);				// bb, o, proto, FromBBuffer, bb
				lua_pushvalue(L, 2);				// bb, o, proto, FromBBuffer, bb, o
				lua_call(L, 2, 0);					// bb, o, proto
				lua_pop(L, 1);						// bb, o
			}
			else
			{
				lua_pop(L, 2);						// bb
				PushIdxDict(L);						// bb, dict
				lua_rawgeti(L, -1, ptr_offset);		// bb, dict, o
				lua_insert(L, -2);					// bb, o, dict
				lua_pop(L, 1);						// bb, o
			}
		}

	};
};
