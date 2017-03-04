#include "xx_luahelper.h"
#include <iostream>

// types defines
/***********************************************************************/

struct B;
struct A : xx::MPObject
{
	B* b;
	xx::MPtr<B> GetB();
	A();
	~A();
};

struct B : xx::MPObject
{
	xx::String* name = nullptr;
	void SetName(char const* name);
	xx::String* GetName();
	~B();
};

// MP defines
/***********************************************************************/

typedef xx::MemPool<A, xx::String, B> MP;

// impls
/***********************************************************************/

inline xx::MPtr<B> A::GetB()
{
	return b;
}
inline A::A()
{
	mempool<MP>().CreateTo(b);
}
inline A::~A()
{
	b->Release();
}

inline void B::SetName(char const* name)
{
	if (name)
	{
		if (this->name) this->name->Assign(name);
		else mempool<MP>().CreateTo(this->name, name);
	}
	else if (this->name)
	{
		this->name->Release();
		this->name = nullptr;
	}
}
inline xx::String* B::GetName()
{
	return name;
}
inline B::~B()
{
	if (name)
	{
		name->Release();
		name = nullptr;
	}
}

// main
/***********************************************************************/

int main()
{
	MP mp;
	auto L = xx::Lua_NewState(mp);

	// 开始 bind A 的函数
	xx::Lua_PushMetatable<MP, A>(L);
	xx::Lua_BindFunc_Ensure<MP, A>(L);
	xxLua_BindFunc(MP, L, A, GetB, false);
	xxLua_BindField(MP, L, A, b, true);
	lua_pop(L, 1);

	// 开始 bind B 的函数
	xx::Lua_PushMetatable<MP, B>(L);
	xx::Lua_BindFunc_Ensure<MP, B>(L);
	xxLua_BindFunc(MP, L, B, SetName, false);
	xxLua_BindFunc(MP, L, B, GetName, false);
	xxLua_BindField(MP, L, B, name, true);
	lua_pop(L, 1);

	// 造一个 a 出来压到 L 之 global
	auto a = mp.Create<A>();
	xx::Lua_SetGlobal<MP>(L, "a", a);
	auto rtv = luaL_dostring(L, R"%%(

	local a = _G.a
	print( a )
	local b = a:GetB()
	print( b )
	print( b:Ensure() )
	a:b( b )

	b:SetName( "asdf" )
	print( b:GetName() )
	b:SetName( nil )
	print( b:GetName() == nil )
	b:name( "asdf" )
	print( b:name() )
	b:name( 1 )
	print( b:name() )

)%%");
	if (rtv)
	{
		std::cout << lua_tostring(L, -1) << std::endl;
	}
	a->Release();

	lua_close(L);
	std::cin.get();
	return 0;
}
