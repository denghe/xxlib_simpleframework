//#include "xx_luahelper.h"
#include "xx_mempool.h"
#include <iostream>

// types defines
/***********************************************************************/
struct A : xx::MPObject
{
};
struct B : xx::MPObject
{
	A* a;
	B();
	~B();
};

// MP defines
/***********************************************************************/

typedef xx::MemPool<::A> MP;

// impls
/***********************************************************************/

inline B::B()
{
	mempool<MP>().CreateTo(a);
}
inline B::~B()
{
	a->Release();
}

// main
/***********************************************************************/



int main()
{

	//	MP mp;
	//	auto L = xx::Lua_NewState(mp);
	//
	//	//xx::Lua_PushMetatable<MP, Factor>(L);
	//	//lua_pop(L, 1);
	//
	//	//xx::Lua_SetGlobal<MP>(L, "factor", Factor());
	//	auto rtv = luaL_dostring(L, R"%%(
	//
	//
	//)%%");
	//	if (rtv)
	//	{
	//		std::cout << lua_tostring(L, -1) << std::endl;
	//	}
	//
	//	lua_close(L);
	//	std::cin.get();
	return 0;
}
