#include <iostream>
#include "lua.hpp"

struct B
{
};
struct A
{
	B* b;
};

int main()
{
	//size_t lua_rawlen

	// todo: 用 ud 替代 lud. 考虑在 ud 的头部放点类型啊 长度之类的信息 以确定 ud 是 T* MPtr<T> T. 似乎比起读 metatable 感觉要更轻量


	auto L = luaL_newstate();
	luaL_openlibs(L);
	auto a = new A();
	lua_pushlightuserdata(L, a);					// a
	lua_newtable(L);								// a, mt
	lua_pushstring(L, "__index");					// a, mt, __index
	lua_pushvalue(L, -2);							// a, mt, __index, mt
	lua_rawset(L, -3);								// a, mt
	lua_pushstring(L, "b");							// a, mt, "b"
	lua_pushcclosure(L, [](lua_State* L)			// a, mt, "b", fn
	{
		auto self = (A*)lua_touserdata(L, 1);
		lua_pushlightuserdata(L, self->b);				// ... b
		//lua_newtable(L);								// ... b, mt
		//lua_setmetatable(L, -2);						// ... b
		return 1;
	}, 0);
	lua_rawset(L, -3);								// a, mt
	lua_setmetatable(L, -2);						// a
	lua_setglobal(L, "a");							//
	auto result = luaL_dostring(L, R"%%(

print( a )
print( getmetatable( a ) )
local b = a:b()
print( getmetatable( a ) )
print( getmetatable( b ) )
print( b )

)%%");
	if (result)
	{
		std::cout << lua_tostring(L, -1) << std::endl;
	}

	// todo: release
}
