#pragma execution_character_set("utf-8")
#pragma once
#include "lua_mempool.h"

struct Lua_BBuffer
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
			{ "WriteObject", WriteObject },
			{ "WriteRoot", WriteRoot },

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
			{ "ReadObject", ReadObject },
			{ "ReadRoot", ReadRoot },

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

	// 析构
	inline static int __gc(lua_State* L)
	{
		auto& self = *(Lua_BBuffer*)lua_touserdata(L, -1);
		self.~Lua_BBuffer();
		self.ReleasePtrDict(L);				// 异常: 因为是移除, 应该不会抛
		self.ReleaseIdxDict(L);
		return 0;
	}

	// 创建, 无参数
	inline static int Create(lua_State* L)
	{
		if (lua_gettop(L) != 0)					// t
		{
			luaL_error(L, "bad args nums. expect 0");
		}
		auto& self = *(Lua_BBuffer*)lua_newuserdata(L, sizeof(Lua_BBuffer));	//	异常: 无副作用
		lua_getglobal(L, name);					// ud, mt							异常: 未执行构造函数, 不需要析构
		new (&self) Lua_BBuffer(L);				//									异常: 因为未 bind 元表, 不会执行到析构
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

	inline static Lua_BBuffer& GetSelf(lua_State* L, int top)
	{
		if (lua_gettop(L) != top)
		{
			luaL_error(L, "bad args nums. expect %d", top);
		}
		auto selfptr = (Lua_BBuffer*)lua_touserdata(L, 1);
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
		self.Write(lua_toboolean(L, 2) != 0);
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
		self.Write(v);
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
		lua_pushinteger(L, self.dataLen);
		return 1;
	}

	inline static int GetOffset(lua_State* L)
	{
		auto& self = GetSelf(L, 1);
		lua_pushinteger(L, self.offset);
		return 1;
	}


	// todo: String BBuffer 序列化函数 改为经由 WriteObject 根据 typeId 来路由调用, 不暴露在外


	// 写入 长度 + 内容( typeId , offset 由调用方写入 )
	inline void WriteString(lua_State* L)
	{
		size_t len;
		auto v = lua_tolstring(L, 2, &len);
		Reserve(dataLen + 5 + (uint32_t)len);
		Write(len);
		if (len)
		{
			memcpy(buf + dataLen, v, len);
			dataLen += (uint32_t)len;
		}
	}

	// 写入 长度 + 内容( typeId , offset 由调用方写入 )
	inline void WriteBBuffer(lua_State* L)
	{
		auto& bb = *(Lua_BBuffer*)lua_touserdata(L, 2);
		bb->ToBBuffer(*this);
	}

	inline static int Dump(lua_State* L)
	{
		// todo
		return 0;
	}


	inline static int ReadBoolean(lua_State* L)
	{
		auto& self = GetSelf(L, 1);
		bool v;
		if (self.Read(v))
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
		if (self.Read(v))
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

	inline void ReadString(lua_State* L)
	{
		uint32_t len;
		if (Read(len))
		{
			luaL_error(L, "read string len error. buf.offset = %d", offset);
		}
		if (offset + len > dataLen)
		{
			luaL_error(L, "string's len: %d out of range. buf.offset = %d, buf.dataLen = %d", len, offset, dataLen);
		}
		lua_pushlstring(L, buf + offset, len);
	}

	inline void ReadBBuffer(lua_State* L)
	{
		Create(L);											// ..., bb
		auto& bb = *(Lua_BBuffer*)lua_touserdata(L, 2);
		if (bb->FromBBuffer(*this))
		{
			lua_pop(L, 1);									// ...
			luaL_error(L, "read BBuffer len + data error. buf.offset = %d", offset);
		}
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
		self.WriteObject_(L);					// bb, o
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
			if (lua_isstring(L, 2))
			{
				Write((uint8_t)1);
				if (WriteOffset(L))
				{
					WriteString(L);
				}
				return;
			}
			else if (lua_isuserdata(L, 2))
			{
				Write((uint8_t)2);
				if (WriteOffset(L))
				{
					WriteBBuffer(L);
				}
				return;
			}

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
		self.offsetRoot = self.dataLen;
		self.CreatePtrDict(L);

		self.WriteObject_(L);					// bb, o

		self.ReleasePtrDict(L);
		return 0;
	}





	inline static int ReadRoot(lua_State* L)
	{
		auto& self = GetSelf(L, 1);				// bb
		self.offsetRoot = self.offset;
		self.CreateIdxDict(L);

		self.ReadObject_(L);					// bb, rtv

		self.ReleaseIdxDict(L);
		return 1;
	}

	inline static int ReadObject(lua_State* L)
	{
		auto& self = GetSelf(L, 1);				// bb
		self.ReadObject_(L);					// bb, rtv
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

		if (typeId > 2)		// 1, 2 没有 proto
		{
			lua_rawgetp(L, LUA_REGISTRYINDEX, (void*)name);	// bb, typeIdProtos
			lua_rawgeti(L, -1, typeId);				// bb, typeIdProtos, proto?
			if (lua_isnil(L, -1))
			{
				luaL_error(L, "buf read offset: %d, invalid typeId: %d", offset, typeId);
			}
		}

		uint32_t ptr_offset = 0, bb_offset_bak = offset - offsetRoot;
		if (Read(ptr_offset))
		{
			luaL_error(L, "buf read offset: %d, read offset error", offset);
		}
		if (ptr_offset == bb_offset_bak)		// 首次出现, 后面是内容
		{
			if (typeId == 1)
			{
				ReadString(L);
				return;
			}
			else if (typeId == 2)
			{
				ReadBBuffer(L);
				return;
			}

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





	/**************************************************************************************************/
	// 一些处理变长读写的基础函数
	/**************************************************************************************************/

	// negative -> ZigZag positive.  效果：负数变正，正数 *= 2
	inline static uint16_t ZigZagEncode(int16_t in) noexcept
	{
		return (in << 1) ^ (in >> 15);
	}
	inline static uint32_t ZigZagEncode(int32_t in) noexcept
	{
		return (in << 1) ^ (in >> 31);
	}
	inline static uint64_t ZigZagEncode(int64_t in) noexcept
	{
		return (in << 1) ^ (in >> 63);
	}

	// ZigZag positive -> negative
	inline static int16_t ZigZagDecode(uint16_t in) noexcept
	{
		return (int16_t)(in >> 1) ^ (-(int16_t)(in & 1));
	}
	inline static int32_t ZigZagDecode(uint32_t in) noexcept
	{
		return (int32_t)(in >> 1) ^ (-(int32_t)(in & 1));
	}
	inline static int64_t ZigZagDecode(uint64_t in) noexcept
	{
		return (int64_t)(in >> 1) ^ (-(int64_t)(in & 1));
	}

	// uint16_t 变长写, 返回实际写入长度
	inline static uint32_t VarWrite7(char *dstBuf, uint16_t in) noexcept
	{
		uint32_t len = 0;
	Lab1:
		auto b7 = (unsigned char)in;
		if ((in >>= 7))
		{
			dstBuf[len++] = b7 | 0x80u;
			goto Lab1;
		}
		dstBuf[len++] = b7;
		return len;
	}

	// uint32_t 变长写, 返回实际写入长度
	inline static uint32_t VarWrite7(char *dstBuf, uint32_t in) noexcept
	{
		uint32_t len = 0;
	Lab1:
		auto b7 = (unsigned char)in;
		if ((in >>= 7))
		{
			dstBuf[len++] = b7 | 0x80u;
			goto Lab1;
		}
		dstBuf[len++] = b7;
		return len;
	}

	// uint64_t 变长写, 返回实际写入长度
	inline static uint32_t VarWrite7(char *dstBuf, uint64_t in) noexcept
	{
		uint32_t len = 0;
	Lab1:
		auto b7 = (unsigned char)in;
		if ((in >>= 7))
		{
			dstBuf[len++] = b7 | 0x80u;
			if (len == 8)
			{
				dstBuf[len++] = (unsigned char)in;
			}
			else goto Lab1;
		}
		else
		{
			dstBuf[len++] = b7;
		}
		return len;
	}

	// uint16_t 变长读, 可能会抛 lua error
	inline static void VarRead7(lua_State* L, char const *srcBuf, uint32_t dataLen, uint32_t& offset, uint16_t &out)
	{
		if (offset >= dataLen) luaL_error(L, "uint16_t VarRead7 error: not enough data. offset = %d, dataLen = %d", offset, dataLen);
		auto p = srcBuf + offset;
		uint32_t i = 0, b7;
		int32_t lshift = 0;
		uint32_t idx = 0;
		dataLen -= offset;
	Lab1:
		b7 = p[idx++];
		if (b7 > 0x7F)
		{
			if (idx == 3) luaL_error(L, "uint16_t VarRead7 error: overflow. offset = %d, dataLen = %d", offset, dataLen);
			if (idx >= dataLen) luaL_error(L, "uint16_t VarRead7 error: not enough data. offset = %d, dataLen = %d", offset, dataLen); 
			i |= (b7 & 0x7F) << lshift;
			lshift += 7;
			goto Lab1;
		}
		else
		{
			if (idx == 3 && b7 > 3) luaL_error(L, "uint16_t VarRead7 error: overflow. offset = %d, dataLen = %d", offset, dataLen);
			else
			{
				i |= b7 << lshift;
			}
		}
		out = (uint16_t)i;
		offset += idx;
	}

	// uint32_t 变长读, 可能会抛 lua error
	inline static void VarRead7(lua_State* L, char const *srcBuf, uint32_t dataLen, uint32_t& offset, uint32_t &out)
	{
		if (offset >= dataLen) luaL_error(L, "uint32_t VarRead7 error: not enough data. offset = %d, dataLen = %d", offset, dataLen);
		auto p = srcBuf + offset;
		uint32_t i = 0, b7;
		int32_t lshift = 0;
		uint32_t idx = 0;
		dataLen -= offset;
	Lab1:
		b7 = p[idx++];
		if (b7 > 0x7F)
		{
			if (idx == 5) luaL_error(L, "uint32_t VarRead7 error: not enough data. offset = %d, dataLen = %d", offset, dataLen);
			if (idx >= dataLen) luaL_error(L, "uint32_t VarRead7 error: not enough data. offset = %d, dataLen = %d", offset, dataLen);
			i |= (b7 & 0x7F) << lshift;
			lshift += 7;
			goto Lab1;
		}
		else
		{
			if (idx == 5 && b7 > 15) luaL_error(L, "uint32_t VarRead7 error: not enough data. offset = %d, dataLen = %d", offset, dataLen);
			else
			{
				i |= b7 << lshift;
			}
		}
		out = i;
		offset += idx;
	}

	// uint64_t 变长读, 可能会抛 lua error
	inline static void VarRead7(lua_State* L, char const *srcBuf, uint32_t dataLen, uint32_t &offset, uint64_t &out)
	{
		if (offset >= dataLen) luaL_error(L, "uint64_t VarRead7 error: not enough data. offset = %d, dataLen = %d", offset, dataLen);
		auto p = srcBuf + offset;
		uint64_t i = 0, b7;
		int32_t lshift = 0;
		uint32_t idx = 0;
		dataLen -= offset;
	Lab1:
		b7 = p[idx++];
		if (b7 > 0x7F)
		{
			if (idx >= dataLen) luaL_error(L, "uint64_t VarRead7 error: not enough data. offset = %d, dataLen = %d", offset, dataLen);
			if (idx < 8)
			{
				i |= (b7 & 0x7F) << lshift;
				lshift += 7;
				goto Lab1;
			}
			else
			{
				i |= ((b7 & 0x7F) << lshift) | ((uint64_t)p[idx++] << 28 << 28);
			}
		}
		else
		{
			i |= b7 << lshift;
		}
		out = i;
		offset += idx;
	}


	/**************************************************************************************************/
	// 上面基础函数的 算长 & 读 & 写的模板封装
	/**************************************************************************************************/

	// 计算写入 buf 后的最大长. 单字节 或 float 类型原样 copy, 其他类型均变长写入, 最长时会比原长 + 1
	template<typename T>
	static uint32_t CalcWriteLen(T const &in) noexcept
	{
		if constexpr(sizeof(T) == 1 || std::is_same_v<float, T>) return sizeof(T);
		else return sizeof(T) + 1;
	}

	// 写入 buf. 需要在调用前提前预留最大长的内存
	template<typename T>
	static uint32_t WriteTo(char *dstBuf, T const &in) noexcept
	{
		if constexpr(sizeof(T) == 1 || std::is_same_v<float, T>)
		{
			memcpy(dstBuf, &in, sizeof(T));
			return sizeof(T);
		}
		else if constexpr(std::is_integral_v<T>)
		{
			if constexpr(std::is_unsigned_v<T>) return VarWrite7(dstBuf, in);
			else return VarWrite7(dstBuf, ZigZagEncode(in));
		}
		else // should be double
		{
			if (in == 0)
			{
				dstBuf[0] = 0;
				return 1;
			}
			else if (std::isnan(in))
			{
				dstBuf[0] = 1;
				return 1;
			}
			else if (in == -std::numeric_limits<double>::infinity())	// negative infinity
			{
				dstBuf[0] = 2;
				return 1;
			}
			else if (in == std::numeric_limits<double>::infinity())		// positive infinity
			{
				dstBuf[0] = 3;
				return 1;
			}
			else
			{
				auto i = (int32_t)in;
				if (in == (double)i)
				{
					dstBuf[0] = 4;
					return 1 + WriteTo(dstBuf + 1, i);
				}
				else
				{
					dstBuf[0] = 5;
					memcpy(dstBuf + 1, &in, sizeof(double));
					return sizeof(double) + 1;
				}
			}
		}
	}

	// 从 buf 读出数据. 可能抛 lua error.
	template<typename T>
	static void ReadFrom(lua_State* L, char const *srcBuf, uint32_t const &dataLen, uint32_t &offset, T &out)
	{
		if constexpr(sizeof(T) == 1 || std::is_same_v<float, T>)
		{
			if (offset + sizeof(T) > dataLen) luaL_error(L, "1byte or float ReadFrom error: not enough data. offset = %d, dataLen = %d", offset, dataLen);
			memcpy(&out, srcBuf + offset, sizeof(T));
			offset += sizeof(T);
		}
		else if constexpr(std::is_integral_v<T>)
		{
			if constexpr(std::is_unsigned_v<T>)
			{
				VarRead7(L, srcBuf, dataLen, offset, out);
			}
			else
			{
				std::make_unsigned_t<T> i = 0;
				VarRead7(L, srcBuf, dataLen, offset, i);
				out = ZigZagDecode(i);
			}
		}
		else // should be double
		{
			if (offset >= dataLen) luaL_error(L, "double ReadFrom error: not enough data. offset = %d, dataLen = %d", offset, dataLen);
			switch (srcBuf[offset++])				// 跳过 1 字节
			{
			case 0:
				out = 0;
				return 0;
			case 1:
				out = std::numeric_limits<double>::quiet_NaN();
				return 0;
			case 2:
				out = -std::numeric_limits<double>::infinity();
				return 0;
			case 3:
				out = std::numeric_limits<double>::infinity();
				return 0;
			case 4:
			{
				int32_t i = 0;
				auto rtv = ReadFrom(L, srcBuf, dataLen, offset, i);
				if (rtv) return rtv;
				out = i;
				return 0;
			}
			case 5:
			{
				if (dataLen < offset + sizeof(double)) return -1;
				memcpy(&out, srcBuf + offset, sizeof(double));
				offset += sizeof(double);
				return 0;
			}
			default:
				luaL_error(L, "double ReadFrom error: bad double type. offset = %d, dataLen = %d", offset, dataLen);
			}
		}
	}

	/**************************************************************************************************/
	// 基于上面的模板封装写的成员函数
	/**************************************************************************************************/

	char*		buf;
	uint32_t    bufLen;
	uint32_t    dataLen;
	uint32_t	offset = 0;					// 读指针偏移量
	uint32_t	offsetRoot = 0;				// offset 值写入修正
	uint32_t	dataLenBak = 0;				// WritePackage 时用于备份当前数据写入偏移

	// 从池中构造 BBuffer. 可能抛 lua 异常
	Lua_BBuffer(lua_State* L)
	{
		Lua_MemPool* mp;
		lua_getallocf(L, (void**)&mp);		// 得到内存分配器
		if (!mp)
		{
			luaL_error(L, "lua_State is not create by mempool ??");
		}
		// todo
	}
	Lua_BBuffer(Lua_BBuffer const&) = delete;

};
