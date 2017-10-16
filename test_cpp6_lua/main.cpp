#pragma execution_character_set("utf-8")

#include "luaex.h"
#include "lua_xx_bbuffer.h"
#include "std_cout_helper.h"

static int InitLua(lua_State *L)
{
	luaL_openlibs(L);
	xx::Lua_BBuffer::Init(L);
	return 0;
}

static int TestBBuffer(lua_State *L)
{
	auto rtv = luaL_loadstring(L, R"-==-(
local bb = BBuffer.Create()
--bb:WriteUInt8( 123 )
--bb:WriteUInt16( 1234 )
	)-==-");
	if (rtv != LUA_OK) return lua_error(L);
	lua_call(L, 0, 0);
	return 0;
}

int main()
{
	xx::MemPool mp;
	auto L = lua_newstate([](void *ud, void *ptr, size_t osize, size_t nsize)
	{
		return ((xx::MemPool*)ud)->Realloc(ptr, nsize, osize);
	}, &mp);
	if (!L)
	{
		CoutLine("create lua state failed. not enough memory");
		return -1;
	}

	//CoutLine("top = ", lua_gettop(L));
	int rtv = 0;
	auto exec = [&](auto&& fn)
	{
		lua_pushcfunction(L, fn);
		rtv = lua_pcall(L, 0, 0, 0);
		if (rtv != LUA_OK)
		{
			CoutLine("pcall rtv = ", rtv, " errmsg = ", lua_tostring(L, -1));
			lua_pop(L, 1);
		}
	};

	exec(InitLua);
	xx::Stopwatch sw;
	for (int i = 0; i < 999999; ++i)
	{
		exec(TestBBuffer);
	}
	CoutLine(sw());
	sw.Reset();
	uint64_t xx = 0;		// 防优化
	for (int i = 0; i < 999999; ++i)
	{
		auto bb = mp.CreatePtr<xx::BBuffer>();
		bb->Write((uint8_t)123);
		bb->Write((uint16_t)1234);
		xx += bb->dataLen;
	}
	CoutLine(sw(), "        ", xx);

	lua_close(L);
	return 0;
}


//
//auto rtv = luaL_loadstring(L, R"-==-(
//
//--print( "lua exec..." )
//
//local bb = BBuffer.Create()
//--print( bb:GetDataLen() )
//
//--bb:WriteUInt8( 123 )
//--print( bb:GetDataLen() )
//
//--bb:WriteUInt16( 1234 )
//--print( bb:GetDataLen() )
//
//-- read_xx
//-- .... length, offset
//
//	)-==-");