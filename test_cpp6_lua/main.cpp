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
	xx::Stopwatch sw;

	if (luaL_loadfile(L, "test.lua")) return lua_error(L);
	//	auto rtv = luaL_loadstring(L, R"-==-(
	//
	//local bb = BBuffer.Create()
	//local bb2 = BBuffer.Create()
	//bb2:WriteUInt8( 255 )
	//bb:WriteBBuffer( bb2 )
	//print( bb:Dump() )
	//local bb3 = bb:ReadBBuffer()
	//print( bb:Dump() )
	//print( bb3:Dump() )
	//
	//	)-==-");
	//	if (rtv != LUA_OK) return lua_error(L);
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
	auto exec = [&](auto&& fn)->int
	{
		lua_pushcfunction(L, fn);
		rtv = lua_pcall(L, 0, 0, 0);
		if (rtv != LUA_OK)
		{
			CoutLine("pcall rtv = ", rtv, " errmsg = ", lua_tostring(L, -1));
			lua_pop(L, 1);
		}
		return rtv;
	};

	!exec(InitLua) && !exec(TestBBuffer);

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

/*

auto rtv = luaL_loadstring(L, R"-==-(
local xx = 0
for i = 0, 100000, 1 do
	local bb = BBuffer.Create()
	local w32 = bb.WriteUInt32
	local w16 = bb.WriteUInt16
	for j = 0, 1000, 1 do
		w32( bb, i )
		w16( bb, j )
	end
	xx = xx + bb:GetDataLen()
end
print ( "xx = " .. xx )
	)-==-");


*/