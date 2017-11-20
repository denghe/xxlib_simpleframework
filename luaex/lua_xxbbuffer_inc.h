
/**************************************************************************************************/
// Read & Write Format support. 
/**************************************************************************************************/

inline static int BeginWrite(lua_State* L)
{
	auto& self = GetSelf(L, 1);
	self.BeginWrite_(L);
	return 0;
}
inline static int EndWrite(lua_State* L)
{
	auto& self = GetSelf(L, 1);
	self.EndWrite_(L);
	return 0;
}
inline static int BeginRead(lua_State* L)
{
	auto& self = GetSelf(L, 1);
	self.BeginRead_(L);
	return 1;
}
inline static int EndRead(lua_State* L)
{
	auto& self = GetSelf(L, 1);
	self.EndRead_(L);
	return 1;
}

typedef void (Lua_XxBBuffer::*WriteFunc)(lua_State*, int);
inline void WriteBoolean_(lua_State* L, int i) { WriteNum_<bool>(L, i); }
inline void WriteInt8_(lua_State* L, int i) { WriteNum_<int8_t>(L, i); }
inline void WriteInt16_(lua_State* L, int i) { WriteNum_<int16_t>(L, i); }
inline void WriteInt32_(lua_State* L, int i) { WriteNum_<int32_t>(L, i); }
inline void WriteInt64_(lua_State* L, int i) { WriteNum_<int64_t>(L, i); }
inline void WriteUInt8_(lua_State* L, int i) { WriteNum_<uint8_t>(L, i); }
inline void WriteUInt16_(lua_State* L, int i) { WriteNum_<uint16_t>(L, i); }
inline void WriteUInt32_(lua_State* L, int i) { WriteNum_<uint32_t>(L, i); }
inline void WriteUInt64_(lua_State* L, int i) { WriteNum_<uint64_t>(L, i); }
inline void WriteFloat_(lua_State* L, int i) { WriteNum_<float>(L, i); }
inline void WriteDouble_(lua_State* L, int i) { WriteNum_<double>(L, i); }

constexpr static WriteFunc writeFuncs[] =
{
	nullptr,
	&WriteBoolean_,
	&WriteInt8_,
	&WriteInt16_,
	&WriteInt32_,
	&WriteInt64_,
	&WriteUInt8_,
	&WriteUInt16_,
	&WriteUInt32_,
	&WriteUInt64_,
	&WriteFloat_,
	&WriteDouble_,
	&WriteObject_
};
constexpr static int asciiFuncIndex[] =
{
	0,0,0,0,0,0,0,0,0,0,		0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,
	0,0,0,						// NUL ~ (space)
	6,0,0,8,0,0,0,0,0,9,		// !"#$%&'()*      !$*
	0,0,0,0,0,0,				// + ~ 0
	2,3,0,4,0,0,0,5,			// 1 ~ 8
	0,0,0,0,0,0,0,				// 9 ~ ?
	7,							//					@
	0,0,0,0,0,0,0,0,0,0,		0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,
	0,0,0,						// A ~ a
	1,0,11,0,10,				// b ~ f			bdf
	0,0,0,0,0,0,0,0,			// g ~ n
	12,							//					o
	0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,				// p ~ DEL
	0,0,0,0,0,0,0,0,0,0,		0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,		0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,		0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,		0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,		0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,		0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0
};
static_assert(
	_countof(asciiFuncIndex) == 256
	&& asciiFuncIndex['1'] == 2
	&& asciiFuncIndex['2'] == 3
	&& asciiFuncIndex['4'] == 4
	&& asciiFuncIndex['8'] == 5

	&& asciiFuncIndex['!'] == 6
	&& asciiFuncIndex['@'] == 7
	&& asciiFuncIndex['$'] == 8
	&& asciiFuncIndex['*'] == 9

	&& asciiFuncIndex['b'] == 1
	&& asciiFuncIndex['f'] == 10
	&& asciiFuncIndex['d'] == 11
	&& asciiFuncIndex['o'] == 12
	);

typedef void (Lua_XxBBuffer::*ReadFunc)(lua_State*);
inline void ReadBoolean_(lua_State* L) { ReadNum_<bool>(L); }
inline void ReadInt8_(lua_State* L) { ReadNum_<int8_t>(L); }
inline void ReadInt16_(lua_State* L) { ReadNum_<int16_t>(L); }
inline void ReadInt32_(lua_State* L) { ReadNum_<int32_t>(L); }
inline void ReadInt64_(lua_State* L) { ReadNum_<int64_t>(L); }
inline void ReadUInt8_(lua_State* L) { ReadNum_<uint8_t>(L); }
inline void ReadUInt16_(lua_State* L) { ReadNum_<uint16_t>(L); }
inline void ReadUInt32_(lua_State* L) { ReadNum_<uint32_t>(L); }
inline void ReadUInt64_(lua_State* L) { ReadNum_<uint64_t>(L); }
inline void ReadFloat_(lua_State* L) { ReadNum_<float>(L); }
inline void ReadDouble_(lua_State* L) { ReadNum_<double>(L); }

constexpr static ReadFunc readFuncs[] =
{
	nullptr,
	&ReadBoolean_,
	&ReadInt8_,
	&ReadInt16_,
	&ReadInt32_,
	&ReadInt64_,
	&ReadUInt8_,
	&ReadUInt16_,
	&ReadUInt32_,
	&ReadUInt64_,
	&ReadFloat_,
	&ReadDouble_,
	&ReadObject_
};

inline static int FormatError(lua_State* L)
{
	return luaL_error(L, "args[2] must be a valid format string. "
		"int: 1248   unsigned: !@$* ( shift-1248 )  boolean: b   float: f   double: d    string / bbuffer / table: o");
}

inline void WriteFormat_(lua_State* L)
{
	if (!lua_isstring(L, 2))
	{
		FormatError(L);
	}
	auto top = lua_gettop(L);
	size_t len;
	auto s = lua_tolstring(L, 2, &len);
	if (len == 0 || len != top - 2)
	{
		luaL_error(L, "bad format string. len must be == args's count");
	}

	for (int i = 3; i <= top; ++i)
	{
		auto c = (uint8_t)s[i - 3];
		auto f = writeFuncs[asciiFuncIndex[c]];
		if (!f)
		{
			FormatError(L);
		}
		(this->*f)(L, i);
	}
}
// bb:Write( "format str", args... )
inline static int WriteFormat(lua_State* L)
{
	auto& self = GetSelf(L, 3);
	self.WriteFormat_(L);
	return 0;
}
inline static int WriteRootFormat(lua_State* L)
{
	auto& self = GetSelf(L, 3);
	self.BeginWrite_(L);
	self.WriteFormat_(L);
	self.EndWrite_(L);
	return 0;
}

inline int ReadFormat_(lua_State* L)
{
	if (!lua_isstring(L, 2))
	{
		return FormatError(L);
	}
	size_t readCount;
	auto s = lua_tolstring(L, 2, &readCount);
	if (readCount == 0)
	{
		return FormatError(L);
	}
	if (!lua_checkstack(L, (int)readCount))
	{
		luaL_error(L, "lua_checkstack fail. current top = %d, expect +%d", lua_gettop(L), readCount);
	}

	for (size_t i = 0; i < readCount; ++i)
	{
		auto c = (uint8_t)s[i];
		auto f = readFuncs[asciiFuncIndex[c]];
		if (!f)
		{
			return FormatError(L);
		}
		(this->*f)(L);
	}
	return (int)readCount;
}

// bb:Read( "format str" )
inline static int ReadFormat(lua_State* L)
{
	auto& self = GetSelf(L, 2);
	return self.ReadFormat_(L);
}
inline static int ReadRootFormat(lua_State* L)
{
	auto& self = GetSelf(L, 2);
	self.BeginRead_(L);
	auto rtv = self.ReadFormat_(L);
	self.EndRead_(L);
	return rtv;
}

