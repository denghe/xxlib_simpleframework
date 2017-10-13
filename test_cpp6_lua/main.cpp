#pragma execution_character_set("utf-8")	// 为方便测试, 不加 L的"字串" 中文内容直接就是 utf8 编码

#include "luaex.h"
#include "std_cout_helper.h"

static int test中文变量(lua_State *L)
{
	luaL_openlibs(L);

	auto rtv = luaL_loadstring(L, R"-==-(
local 测试一下中文变量 = 123
print( 测试一下中文变量 )
	)-==-");

	if (rtv != LUA_OK) return lua_error(L);

	lua_call(L, 0, 0);
	return 0;
}

struct Foo
{
	Foo()
	{
		CoutLine("Foo();");
	}
	~Foo()
	{
		CoutLine("~Foo();");
	}
};

static int test异常RAII(lua_State *L)
{
	Foo foo;					// 测试是否会被析构
	CoutLine("begin test");
	for (int i = 0; i < 5; ++i)
	{
		CoutLine("i = ", i, " begin");
		lua_newuserdata(L, 0x0FFFFFFF);	// 32bit 下执行只分配得到 2 次, 第 3 次将会内存不足
		CoutLine("i = ", i, " end");
	}
	CoutLine("end test");
	return 0;
}

int main()
{
	Lua_MemPool lmp;
	auto L = lua_newstate([](void *ud, void *ptr, size_t osize, size_t nsize)
	{
		return ((Lua_MemPool*)ud)->Realloc(ptr, nsize, osize);
	}, &lmp);

	if (!L)
	{
		CoutLine("create lua state failed. not enough memory");
		return -1;
	}

	//CoutLine("top = ", lua_gettop(L));
	int rtv = 0;

	lua_pushcfunction(L, &test异常RAII);
	rtv = lua_pcall(L, 0, 0, 0);
	CoutLine("pcall rtv = ", rtv);
	if (rtv != LUA_OK)
	{
		CoutLine("errmsg = ", lua_tostring(L, -1));
		lua_pop(L, 1);
	}

	lua_pushcfunction(L, &test中文变量);
	rtv = lua_pcall(L, 0, 0, 0);
	CoutLine("pcall rtv = ", rtv);
	if (rtv != LUA_OK)
	{
		CoutLine("errmsg = ", lua_tostring(L, -1));
		lua_pop(L, 1);
	}

	lua_close(L);
	return 0;
}
