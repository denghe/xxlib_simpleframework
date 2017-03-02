#include "xx_luahelper.h"
#include <iostream>

// types defines
/***********************************************************************/

struct B;
struct A : xx::MPObject
{
	xx::MPtr<B> GetB();
};

struct B : xx::MPObject
{
};

// MP defines
/***********************************************************************/

typedef xx::MemPool<A, B> MP;

// impls
/***********************************************************************/

inline xx::MPtr<B> A::GetB()
{
	return mempool<MP>().Create<B>();
}

// main
/***********************************************************************/

int main()
{
	MP mp;
	auto L = xx::Lua_NewState(mp);
	// todo: bind
	lua_pop(L, 1);
	lua_close(L);
	return 0;
}
