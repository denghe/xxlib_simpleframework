#pragma execution_character_set("utf-8")

#include "luaex.h"
#include "lua_bbuffer.h"
#include "std_cout_helper.h"

#include "../pkg/PKG2_class.h"

int InitLua(lua_State *L)
{
	luaL_openlibs(L);
	Lua_BBuffer::Init(L);
	return 0;
}

int TestBBuffer(lua_State *L)
{
	if (luaL_loadfile(L, "test2.lua")) return lua_error(L);
	lua_call(L, 0, 0);
	return 0;
}

int TestLua()
{
	Lua_MemPool mp;
	auto L = lua_newstate([](void *ud, void *ptr, size_t osize, size_t nsize)
	{
		return ((Lua_MemPool*)ud)->Realloc(ptr, nsize, osize);
	}, &mp);
	if (!L)
	{
		CoutLine("create lua state failed. not enough memory");
		return -1;
	}

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

void TestCpp()
{
	CoutLine("TestCpp");
	PKG2::AllTypesRegister();
	xx::MemPool mp;
	xx::BBuffer_p bb(mp);

	PKG2::基类_p o(mp);
	o->不淋 = true; //Boolean
	o->白特 = 1; //Byte
	o->撕白特 = -2; //SByte
	o->吸哦特 = -3; //Int16
	o->又吸哦特 = 4; //UInt16
	o->硬特 = -5; //Int32
	o->又硬特 = 6; //UInt32
	o->浪 = -7; //Int64
	o->又浪 = 8; //UInt64
	o->扶裸特 = 9.1f; //Single
	o->大波 = 10.2; //Double
	o->湿最硬.Create(mp, "这是个串儿"); // String
	o->屄拔扶儿.Create(mp); // BBuffer
	o->屄拔扶儿->Write((uint8_t)1);
	o->屄拔扶儿->Write((uint8_t)2);
	o->屄拔扶儿->Write((uint8_t)3);

	PKG2::派生类_p p(mp);
	p->立丝特基类.Create(mp);
	p->立丝特基类->Add(p.Copy<PKG2::基类>());
	p->立丝特基类->Add(o.Move());
	p->立丝特屄拔扶儿.Create(mp);
	p->立丝特屄拔扶儿->Add(mp.CreatePtr<xx::BBuffer>());
	p->立丝特白特.Create(mp);
	p->立丝特白特->Add(1);
	p->立丝特白特->Add(2);
	p->立丝特白特->Add(3);
	bb->Clear();
	bb->WriteRoot(p);
	mp.Cout(bb, '\n');

	//mp.Cout(p, '\n');
	//PKG2::基类_p p2;
	//bb->ReadRoot(p2);
	//mp.Cout(p2, '\n');
}

int main()
{
	SetUtf8Console();
	TestLua();
	TestCpp();

	return 0;
}
