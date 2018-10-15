#pragma execution_character_set("utf-8")
#pragma once
#include "xx_lua_helpers.h"

namespace xx
{
	struct LuaBBuffer : public BBuffer
	{
		LuaBBuffer(lua_State* L) : BBuffer(LuaGetMemPool(L)) {}
		LuaBBuffer(LuaBBuffer const&) = delete;
		LuaBBuffer& operator=(LuaBBuffer const&) = delete;

		// 潜规则类名变量
		inline static const char * const name = "BBuffer";

		// 向 lua 映射全局的 BBuffer 表/元表
		inline static void LuaRegister(lua_State *L)
		{
			luaL_Reg funcs[] =
			{
				{ "__gc", __gc },
			{ "Create", Create },
			{ "Register", Register },


			{ "WriteBoolean", WriteBoolean },
			{ "WriteSByte", WriteInt8 },
			{ "WriteInt8", WriteInt8 },
			{ "WriteInt16", WriteInt16 },
			{ "WriteInt32", WriteInt32 },
			{ "WriteInt64", WriteInt64 },
			{ "WriteByte", WriteUInt8 },
			{ "WriteUInt8", WriteUInt8 },
			{ "WriteUInt16", WriteUInt16 },
			{ "WriteUInt32", WriteUInt32 },
			{ "WriteUInt64", WriteUInt64 },
			{ "WriteSingle", WriteFloat },
			{ "WriteFloat", WriteFloat },
			{ "WriteDouble", WriteDouble },
			{ "WriteObject", WriteObject },
			{ "WriteRoot", WriteRoot },

			{ "ReadBoolean", ReadBoolean },
			{ "ReadSByte", ReadInt8 },
			{ "ReadInt8", ReadInt8 },
			{ "ReadInt16", ReadInt16 },
			{ "ReadInt32", ReadInt32 },
			{ "ReadInt64", ReadInt64 },
			{ "ReadByte", ReadUInt8 },
			{ "ReadUInt8", ReadUInt8 },
			{ "ReadUInt16", ReadUInt16 },
			{ "ReadUInt32", ReadUInt32 },
			{ "ReadUInt64", ReadUInt64 },
			{ "ReadFloat", ReadFloat },
			{ "ReadSingle", ReadFloat },
			{ "ReadDouble", ReadDouble },
			{ "ReadObject", ReadObject },
			{ "ReadRoot", ReadRoot },


			{ "WriteNullableBoolean", WriteNullableBoolean },
			{ "WriteNullableSByte", WriteNullableInt8 },
			{ "WriteNullableInt8", WriteNullableInt8 },
			{ "WriteNullableInt16", WriteNullableInt16 },
			{ "WriteNullableInt32", WriteNullableInt32 },
			{ "WriteNullableInt64", WriteNullableInt64 },
			{ "WriteNullableByte", WriteNullableUInt8 },
			{ "WriteNullableUInt8", WriteNullableUInt8 },
			{ "WriteNullableUInt16", WriteNullableUInt16 },
			{ "WriteNullableUInt32", WriteNullableUInt32 },
			{ "WriteNullableUInt64", WriteNullableUInt64 },
			{ "WriteNullableSingle", WriteNullableFloat },
			{ "WriteNullableFloat", WriteNullableFloat },
			{ "WriteNullableDouble", WriteNullableDouble },

			{ "ReadNullableBoolean", ReadNullableBoolean },
			{ "ReadNullableSByte", ReadNullableInt8 },
			{ "ReadNullableInt8", ReadNullableInt8 },
			{ "ReadNullableInt16", ReadNullableInt16 },
			{ "ReadNullableInt32", ReadNullableInt32 },
			{ "ReadNullableInt64", ReadNullableInt64 },
			{ "ReadNullableByte", ReadNullableUInt8 },
			{ "ReadNullableUInt8", ReadNullableUInt8 },
			{ "ReadNullableUInt16", ReadNullableUInt16 },
			{ "ReadNullableUInt32", ReadNullableUInt32 },
			{ "ReadNullableUInt64", ReadNullableUInt64 },
			{ "ReadNullableFloat", ReadNullableFloat },
			{ "ReadNullableSingle", ReadNullableFloat },
			{ "ReadNullableDouble", ReadNullableDouble },


			{ "GetDataLen", GetDataLen },
			{ "GetOffset", GetOffset },
			{ "SetOffset", SetOffset },
			{ "Clear", Clear },
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

			/* 在注册表中创建 typeId 到 Proto 的映射表 */
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
			auto& self = *(LuaBBuffer**)lua_touserdata(L, -1);
			self->~LuaBBuffer();
			self->ReleasePtrDict(L);					// 异常: 因为是移除, 应该不会抛
			self->ReleaseIdxDict(L);
			return 0;
		}

		// 创建, 无参数
		inline static int Create(lua_State* L)
		{
			auto& self = *(LuaBBuffer**)lua_newuserdata(L, sizeof(void*));	// ..., ud
			lua_getglobal(L, name);					// ..., ud, mt

			auto mp = LuaGetMemPool(L);				// 从注册表拿出 mp
			if (!(self = mp->Create<LuaBBuffer>(L)))
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
			auto typeId = (int)lua_tointeger(L, -1);

			lua_pushlightuserdata(L, (void*)name);	// t, int, name
			lua_rawget(L, LUA_REGISTRYINDEX);		// t, int, typeIdProtos
			lua_pushvalue(L, 1);					// t, int, typeIdProtos, t
			lua_rawseti(L, -2, typeId);				// t, int, typeIdProtos
			lua_pop(L, 2);							// t
			return 0;
		}



		inline static LuaBBuffer& GetSelf(lua_State* L, int top)
		{
			if (lua_gettop(L) < top)
			{
				luaL_error(L, "less arg nums. expect %d+", top);
			}
			auto p = LuaGetPointer<LuaBBuffer>(L, 1);
			if (!p)
			{
				luaL_error(L, "first arg isn't BBuffer( forget \":\" ? )");
			}
			return *p;
		}

		inline static int GetDataLen(lua_State* L)
		{
			auto& self = GetSelf(L, 1);
			lua_pushinteger(L, self.dataLen);
			return 1;
		}

		inline static int GetOffset(lua_State* L)
		{
			auto& self = GetSelf(L, 1);
			lua_pushinteger(L, self.offset);
			return 1;
		}

		inline static int SetOffset(lua_State* L)
		{
			auto& self = GetSelf(L, 2);
			auto offset = (uint32_t)lua_tointeger(L, 2);
			if (offset > self.dataLen)
			{
				luaL_error(L, "the args[ 2 ] offset is out of range. dataLen = %d", self.dataLen);
			}
			self.offset = offset;
			return 0;
		}

		inline static int Clear(lua_State* L)
		{
			auto& self = GetSelf(L, 1);
			self.dataLen = 0;
			self.offset = 0;
			return 0;
		}

		inline static int __tostring(lua_State* L)
		{
			auto& self = GetSelf(L, 1);
			auto s = self.mempool->Str();
			self.ToString(*s);
			lua_pushlstring(L, s->c_str(), s->dataLen);
			return 1;
		}


		template<typename T>
		inline void WriteNum_(lua_State* L, int i)
		{
			if constexpr (std::is_same<T, bool>::value)
			{
				if (!lua_isboolean(L, i))
				{
					luaL_error(L, "the args[ %d ]'s type must be a boolean", i);
				}
				Write(lua_toboolean(L, i) != 0);
			}
			else if constexpr (std::is_floating_point<T>::value)
			{
				if (!lua_isnumber(L, i))
				{
					luaL_error(L, "the args[ %d ]'s type must be a number", i);
				}
				T v;
				v = (T)lua_tonumber(L, i);
				Write(v);
			}
#if LUA_VERSION_NUM < 503
			else if constexpr (std::is_same<T, int64_t>::value)
			{
				if (!lua_isuserdata(L, i))
				{
					luaL_error(L, "the args[ %d ]'s type must be a userdata(int64)", i);
				}
				T v = *(int64_t*)lua_touserdata(L, i);//tolua_toint64(L, i);
				Write(v);
			}
			else if constexpr (std::is_same<T, uint64_t>::value)
			{
				if (!lua_isuserdata(L, i))
				{
					luaL_error(L, "the args[ %d ]'s type must be a userdata(uint64)", i);
				}
				T v = *(uint64_t*)lua_touserdata(L, i);//tolua_touint64(L, i);
				Write(v);
			}
#endif
			else
			{
				if (!lua_isnumber(L, i))
				{
					luaL_error(L, "the args[ %d ]'s type must be a integer", i);
				}
				T v;
				v = (T)lua_tointeger(L, i);
				Write(v);
			}
		}

		template<typename T>
		inline static int WriteNum(lua_State* L)
		{
			static_assert(std::is_arithmetic<T>::value);
			auto& self = GetSelf(L, 2);
			auto top = lua_gettop(L);
			for (int i = 2; i <= top; ++i)
			{
				self.WriteNum_<T>(L, i);
			}
			return 0;
		}
		inline static int WriteBoolean(lua_State* L) { return WriteNum<bool>(L); }
		inline static int WriteInt8(lua_State* L) { return    WriteNum<int8_t>(L); }
		inline static int WriteInt16(lua_State* L) { return   WriteNum<int16_t>(L); }
		inline static int WriteInt32(lua_State* L) { return   WriteNum<int32_t>(L); }
		inline static int WriteInt64(lua_State* L) { return   WriteNum<int64_t>(L); }
		inline static int WriteUInt8(lua_State* L) { return   WriteNum<uint8_t>(L); }
		inline static int WriteUInt16(lua_State* L) { return  WriteNum<uint16_t>(L); }
		inline static int WriteUInt32(lua_State* L) { return  WriteNum<uint32_t>(L); }
		inline static int WriteUInt64(lua_State* L) { return  WriteNum<uint64_t>(L); }
		inline static int WriteFloat(lua_State* L) { return   WriteNum<float>(L); }
		inline static int WriteDouble(lua_State* L) { return  WriteNum<double>(L); }


		template<typename T>
		inline void WriteNullableNum_(lua_State* L, int i)
		{
			// 判断参数是不是 nil / null(lightuserdata). 如果是, 就写入 0 并 return. 
			if (lua_isnil(L, i) || lua_islightuserdata(L, i))
			{
				Write((uint8_t)0);
				return;
			}
			WriteNum_<T>(L, i);
		}

		template<typename T>
		inline static int WriteNullableNum(lua_State* L)
		{
			static_assert(std::is_arithmetic<T>::value);
			auto& self = GetSelf(L, 2);
			auto top = lua_gettop(L);
			for (int i = 2; i <= top; ++i)
			{
				self.WriteNullableNum_<T>(L, i);
			}
			return 0;
		}

		inline static int WriteNullableBoolean(lua_State* L) { return WriteNullableNum<bool>(L); }
		inline static int WriteNullableInt8(lua_State* L) { return    WriteNullableNum<int8_t>(L); }
		inline static int WriteNullableInt16(lua_State* L) { return   WriteNullableNum<int16_t>(L); }
		inline static int WriteNullableInt32(lua_State* L) { return   WriteNullableNum<int32_t>(L); }
		inline static int WriteNullableInt64(lua_State* L) { return   WriteNullableNum<int64_t>(L); }
		inline static int WriteNullableUInt8(lua_State* L) { return   WriteNullableNum<uint8_t>(L); }
		inline static int WriteNullableUInt16(lua_State* L) { return  WriteNullableNum<uint16_t>(L); }
		inline static int WriteNullableUInt32(lua_State* L) { return  WriteNullableNum<uint32_t>(L); }
		inline static int WriteNullableUInt64(lua_State* L) { return  WriteNullableNum<uint64_t>(L); }
		inline static int WriteNullableFloat(lua_State* L) { return   WriteNullableNum<float>(L); }
		inline static int WriteNullableDouble(lua_State* L) { return  WriteNullableNum<double>(L); }



		// 读变长数据之 LUA 报错版
		template<typename T>
		void Read(lua_State* L, T& v)
		{
			if (auto rtv = this->BBuffer::Read<T>(v))
			{
				if (rtv == 1)
				{
					luaL_error(L, "read error: not enough data. offset = %d, dataLen = %d", offset, dataLen);
				}
				else // 2
				{
					luaL_error(L, "read error: overflow. offset = %d, dataLen = %d", offset, dataLen);
				}
			}
		}

		template<typename T>
		void ReadNum_(lua_State* L)
		{
			T v;
			Read(L, v);
			if constexpr (std::is_same<T, bool>::value)			// bool
			{
				lua_pushboolean(L, v);
			}
			else if constexpr (std::is_floating_point<T>::value)	// float or double
			{
				lua_pushnumber(L, (lua_Number)v);
			}
#if LUA_VERSION_NUM < 503
			else if constexpr (std::is_same<T, int64_t>::value)
			{
				*(int64_t*)lua_newuserdata(L, sizeof(int64_t)) = v;	// tolua_pushint64(L, v);
			}
			else if constexpr (std::is_same<T, uint64_t>::value)
			{
				*(uint64_t*)lua_newuserdata(L, sizeof(uint64_t)) = v;	// tolua_pushuint64(L, v);
			}
#endif
			else											// int32 or 64(lua5.3+)
			{
				lua_pushinteger(L, (lua_Integer)v);
			}
		}

		template<typename T>
		static int ReadNum(lua_State* L)
		{
			static_assert(std::is_arithmetic<T>::value);
			auto& self = GetSelf(L, 1);
			auto top = lua_gettop(L);
			int readCount = 1;
			if (top == 2)
			{
				readCount = (int)lua_tointeger(L, 2);
			}
			if (readCount > LUA_MINSTACK - 2 && !lua_checkstack(L, readCount))
			{
				luaL_error(L, "lua_checkstack fail. current top = %d, expect +%d", top, readCount);
			}
			for (int i = 0; i < readCount; ++i)
			{
				self.ReadNum_<T>(L);
			}
			return readCount;
		}
		inline static int ReadBoolean(lua_State* L) { return ReadNum<bool>(L); }
		inline static int ReadInt8(lua_State* L) { return    ReadNum<int8_t>(L); }
		inline static int ReadInt16(lua_State* L) { return   ReadNum<int16_t>(L); }
		inline static int ReadInt32(lua_State* L) { return   ReadNum<int32_t>(L); }
		inline static int ReadInt64(lua_State* L) { return   ReadNum<int64_t>(L); }
		inline static int ReadUInt8(lua_State* L) { return   ReadNum<uint8_t>(L); }
		inline static int ReadUInt16(lua_State* L) { return  ReadNum<uint16_t>(L); }
		inline static int ReadUInt32(lua_State* L) { return  ReadNum<uint32_t>(L); }
		inline static int ReadUInt64(lua_State* L) { return  ReadNum<uint64_t>(L); }
		inline static int ReadFloat(lua_State* L) { return   ReadNum<float>(L); }
		inline static int ReadDouble(lua_State* L) { return  ReadNum<double>(L); }


		template<typename T>
		void ReadNullableNum_(lua_State* L)
		{
			// 判断是否有值. 没有值就压入 null
			uint8_t hasValue = 0;
			Read(L, hasValue);
			if (!hasValue)
			{
				lua_pushlightuserdata(L, 0);
				return;
			}
			ReadNum_<T>(L);
		}

		template<typename T>
		static int ReadNullableNum(lua_State* L)
		{
			static_assert(std::is_arithmetic<T>::value);
			auto& self = GetSelf(L, 1);
			auto top = lua_gettop(L);
			int readCount = 1;
			if (top == 2)
			{
				readCount = (int)lua_tointeger(L, 2);
			}
			if (readCount > LUA_MINSTACK - 2 && !lua_checkstack(L, readCount))
			{
				luaL_error(L, "lua_checkstack fail. current top = %d, expect +%d", top, readCount);
			}
			for (int i = 0; i < readCount; ++i)
			{
				self.ReadNullableNum_<T>(L);
			}
			return readCount;
		}
		inline static int ReadNullableBoolean(lua_State* L) { return ReadNullableNum<bool>(L); }
		inline static int ReadNullableInt8(lua_State* L) { return    ReadNullableNum<int8_t>(L); }
		inline static int ReadNullableInt16(lua_State* L) { return   ReadNullableNum<int16_t>(L); }
		inline static int ReadNullableInt32(lua_State* L) { return   ReadNullableNum<int32_t>(L); }
		inline static int ReadNullableInt64(lua_State* L) { return   ReadNullableNum<int64_t>(L); }
		inline static int ReadNullableUInt8(lua_State* L) { return   ReadNullableNum<uint8_t>(L); }
		inline static int ReadNullableUInt16(lua_State* L) { return  ReadNullableNum<uint16_t>(L); }
		inline static int ReadNullableUInt32(lua_State* L) { return  ReadNullableNum<uint32_t>(L); }
		inline static int ReadNullableUInt64(lua_State* L) { return  ReadNullableNum<uint64_t>(L); }
		inline static int ReadNullableFloat(lua_State* L) { return   ReadNullableNum<float>(L); }
		inline static int ReadNullableDouble(lua_State* L) { return  ReadNullableNum<double>(L); }


		inline void CreatePtrDict(lua_State* L)
		{
			lua_pushlightuserdata(L, this);
			lua_createtable(L, 0, 64);
			lua_rawset(L, LUA_REGISTRYINDEX);
		}
		inline void CreateIdxDict(lua_State* L)
		{
			lua_pushlightuserdata(L, (char*)this + 1);
			lua_createtable(L, 0, 64);
			lua_rawset(L, LUA_REGISTRYINDEX);
		}

		inline void ReleasePtrDict(lua_State* L)
		{
			lua_pushlightuserdata(L, this);
			lua_pushnil(L);
			lua_rawset(L, LUA_REGISTRYINDEX);
		}
		inline void ReleaseIdxDict(lua_State* L)
		{
			lua_pushlightuserdata(L, (char*)this + 1);
			lua_pushnil(L);
			lua_rawset(L, LUA_REGISTRYINDEX);
		}

		inline void PushPtrDict(lua_State* L)
		{
			lua_pushlightuserdata(L, this);
			lua_rawget(L, LUA_REGISTRYINDEX);
			if (lua_isnil(L, -1))
			{
				luaL_error(L, "the ptrDict is nil. forget WriteRoot?");
			}
		}
		inline void PushIdxDict(lua_State* L)
		{
			lua_pushlightuserdata(L, (char*)this + 1);
			lua_rawget(L, LUA_REGISTRYINDEX);
			if (lua_isnil(L, -1))
			{
				luaL_error(L, "the idxDict is nil. forget ReadRoot?");
			}
		}


		// 针对 string, bbuffer, table, 写 dataLen - offsetRoot 到 buf, 返回是否第一次写入
		inline bool WriteOffset(lua_State* L, int i)
		{
			if (!lua_checkstack(L, 3))
			{
				luaL_error(L, "lua_checkstack fail. current top = %d, expect +3", lua_gettop(L));
			}

			int type = lua_type(L, i);

			switch (lua_type(L, i))					// ..., o, ...
			{
			case LUA_TSTRING:
			case LUA_TTABLE:
			case LUA_TUSERDATA:
				PushPtrDict(L);						// ..., o, ..., dict				定位到注册表中的 ptrDict
				lua_pushvalue(L, i);				// ..., o, ..., dict, o				查询当前对象是否已经记录过
				lua_rawget(L, -2);					// ..., o, ..., dict, nil/offset
				if (lua_isnil(L, -1))				// 如果未记录则记录 + 写buf
				{
					auto offset = dataLen - offsetRoot;
					Write(offset);					// 写buf

					lua_pop(L, 1);					// ..., o, ..., dict
					lua_pushvalue(L, i);			// ..., o, ..., dict, o
					lua_pushinteger(L, offset);		// ..., o, ..., dict, o, offset
					lua_rawset(L, -3);				// ..., o, ..., dict
					lua_pop(L, 1);					// ..., o, ...
					return true;					// 返回首次出现
				}
				else								// 记录过则取其 value 来写buf
				{
					auto offset = (uint32_t)lua_tointeger(L, -1);
					Write(offset);					// 写buf
					lua_pop(L, 2);					// ..., o, ...
					return false;
				}
			default:
				luaL_error(L, "the arg's type must be a string / table / BBuffer");
				return false;
			}
		}

		inline static int WriteObject(lua_State* L)
		{
			auto& self = GetSelf(L, 2);				// bb, o1, o2, ...
			auto top = lua_gettop(L);
			for (int i = 2; i <= top; ++i)
			{
				self.WriteObject_(L, i);
			}
			return 0;
		}


		// 写对象到 buf. 如果为空就写 typeId 0, 否则写 typeId, offset[, content ]
		inline void WriteObject_(lua_State* L, int i)
		{
			if (lua_isnil(L, i) ||					// bb, ..., o, ...
				(lua_islightuserdata(L, i) && (size_t)lua_touserdata(L, i) == 0))
			{
				Write((uint8_t)0);
			}
			else
			{
				if (!lua_checkstack(L, 5))
				{
					luaL_error(L, "lua_checkstack fail. current top = %d, expect +5", lua_gettop(L));
				}

				if (lua_isstring(L, i))				// string
				{
					Write((uint8_t)1);
					if (WriteOffset(L, i))
					{
						size_t len;
						auto s = lua_tolstring(L, i, &len);
						Write(len);
						WriteBuf(s, len);
					}
					return;
				}
				else if (lua_isuserdata(L, i))		// bbuffer
				{
					Write((uint8_t)2);
					if (WriteOffset(L, i))
					{
						auto bb = LuaGetPointer<LuaBBuffer>(L, i);
						if (!bb)
						{
							luaL_error(L, "WriteObject only support userdata is BBuffer.");
						}
						Write(bb->dataLen);
						WriteBuf(bb);
					}
					return;
				}

				lua_getfield(L, i, "__proto");		// bb, ..., o, ..., proto
				lua_getfield(L, -1, "typeId");		// bb, ..., o, ..., proto, typeId
				auto typeId = (uint16_t)lua_tonumber(L, -1);
				Write(typeId);
				lua_pop(L, 1);						// bb, ..., o, ..., proto
				lua_pushvalue(L, i);				// bb, ..., o, ..., proto, o
				if (WriteOffset(L, i))				// bb, ..., o, ..., proto, o
				{
					lua_getfield(L, -2, "ToBBuffer");//bb, ..., o, ..., proto, o, func
					lua_pushvalue(L, 1);			// bb, ..., o, ..., proto, o, func, bb
					lua_pushvalue(L, i);			// bb, ..., o, ..., proto, o, func, bb, o
					lua_call(L, 2, 0);				// bb, ..., o, ..., proto, o
					lua_pop(L, 2);					// bb, ..., o, ...
				}
			}
		}

		inline void BeginWrite_(lua_State* L)
		{
			offsetRoot = dataLen;
			CreatePtrDict(L);
		}

		inline void EndWrite_(lua_State* L)
		{
			ReleasePtrDict(L);
		}

		inline void BeginRead_(lua_State* L)
		{
			offsetRoot = offset;
			CreateIdxDict(L);
		}

		inline void EndRead_(lua_State* L)
		{
			ReleaseIdxDict(L);
		}



		inline static int WriteRoot(lua_State* L)
		{
			auto& self = GetSelf(L, 2);				// bb, o
			self.BeginWrite_(L);
			self.WriteObject_(L, 2);				// bb, o
			self.EndWrite_(L);
			return 0;
		}

		inline static int ReadRoot(lua_State* L)
		{
			auto& self = GetSelf(L, 1);				// bb
			self.BeginRead_(L);
			self.ReadObject_(L);					// bb, rtv
			self.EndRead_(L);
			return 1;
		}

		inline static int ReadObject(lua_State* L)
		{
			auto& self = GetSelf(L, 1);
			auto top = lua_gettop(L);
			int readCount = 1;
			if (top == 2)
			{
				readCount = (int)lua_tointeger(L, 2);
			}
			if (readCount > LUA_MINSTACK - 2 && !lua_checkstack(L, readCount))
			{
				luaL_error(L, "lua_checkstack fail. current top = %d, expect +%d", top, readCount);
			}
			for (int i = 0; i < readCount; ++i)
			{
				self.ReadObject_(L);
			}
			return readCount;
		}

		inline void StoreOffset(lua_State* L, uint32_t ptr_offset)
		{
			PushIdxDict(L);						// ..., o, dict
			lua_pushvalue(L, -2);				// ..., o, dict, o
			lua_rawseti(L, -2, ptr_offset);		// ..., o, dict
			lua_pop(L, 1);						// ..., o, 
		}

		inline void ReadObject_(lua_State* L)
		{
			uint16_t typeId;
			Read(L, typeId);
			if (!typeId)
			{
				lua_pushlightuserdata(L, 0);		// bb, ..., null
				return;
			}

			if (typeId > 2)		// 1, 2 没有 proto
			{
				lua_pushlightuserdata(L, (void*)name);
				lua_rawget(L, LUA_REGISTRYINDEX);	// bb, ..., typeIdProtos
				lua_rawgeti(L, -1, typeId);			// bb, ..., typeIdProtos, proto?
				if (lua_isnil(L, -1))
				{
					luaL_error(L, "buf read offset: %d, invalid typeId: %d", offset, typeId);
				}
			}
			// else									// bb, ...

			uint32_t ptr_offset = 0, bb_offset_bak = uint32_t(offset - offsetRoot);
			Read(L, ptr_offset);
			if (ptr_offset == bb_offset_bak)		// 首次出现, 后面是内容
			{
				if (typeId == 1)	// string
				{
					uint32_t len;
					Read(L, len);
					if (offset + len > dataLen)
					{
						luaL_error(L, "string's len: %d out of range. buf.offset = %d, buf.dataLen = %d", len, offset, dataLen);
					}
					lua_pushlstring(L, buf + offset, len);	// ..., str
					offset += len;
					StoreOffset(L, ptr_offset);
					return;
				}
				else if (typeId == 2)	// bbuffer
				{
					uint32_t len;
					Read(L, len);
					if (offset + len > dataLen)
					{
						lua_pop(L, 1);				// ...
						luaL_error(L, "BBuffer's len: %d out of range. buf.offset = %d, buf.dataLen = %d", len, offset, dataLen);
					}
					Create(L);						// ..., bb
					if (len)
					{
						auto& bb = *(LuaBBuffer**)lua_touserdata(L, -1);
						bb->Reserve(len);
						memcpy(bb->buf, buf + offset, len);
						offset += len;
					}
					StoreOffset(L, ptr_offset);
					return;
				}
				// bb, ..., typeIdProtos, proto
				lua_remove(L, -2);					// bb, ..., proto
				lua_getfield(L, -1, "Create");		// bb, ..., proto, Create
				lua_call(L, 0, 1);					// bb, ..., proto, o
				StoreOffset(L, ptr_offset);
				lua_insert(L, -2);					// bb, ..., o, proto
				lua_getfield(L, -1, "FromBBuffer");	// bb, ..., o, proto, FromBBuffer
				lua_pushvalue(L, 1);				// bb, ..., o, proto, FromBBuffer, bb
				lua_pushvalue(L, -4);				// bb, ..., o, proto, FromBBuffer, bb, o
				lua_call(L, 2, 0);					// bb, ..., o, proto
				lua_pop(L, 1);						// bb, ..., o
			}
			else
			{
				if (typeId > 2)						// bb, ..., typeIdProtos, proto
				{
					lua_pop(L, 2);					// bb, ..., 
				}
				PushIdxDict(L);						// bb, ..., dict
				lua_rawgeti(L, -1, ptr_offset);		// bb, ..., dict, o
				lua_insert(L, -2);					// bb, ..., o, dict
				lua_pop(L, 1);						// bb, ..., o
			}
		}

	};
}
