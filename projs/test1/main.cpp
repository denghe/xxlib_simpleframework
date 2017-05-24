#include <xx_mempool.h>
#include <xx_bbuffer.h>
#include <iostream>

// todo: 改造序列化相关
// todo: 移除 autoRelease 的设计, 考虑增加对 Xxxxx_v 的进一步支持, 比如用于容器 item 时, call 默认构造函数传 mempool()

struct Foo : xx::MPObject
{
	xx::List_v<Foo*> childs;

	Foo() 
		: childs(mempool()) 
	{
	}

	Foo(xx::BBuffer *bb)
		: childs(mempool())
	{
		if (auto rtv = bb->Read(childs)) throw rtv;
	}

	inline virtual void ToBBuffer(xx::BBuffer &bb) const override
	{
		bb.Write(childs);
	};

	inline virtual int FromBBuffer(xx::BBuffer &bb) override
	{
		//if (auto rtv = this->BaseType::FromBBuffer(bb)) return rtv;
		return bb.Read(childs);
	};
};

namespace xx
{
	template<> struct TypeId<BBuffer> { static const uint16_t value = 2; };
	template<> struct TypeId<List<int>> { static const uint16_t value = 3; };
	template<> struct TypeId<::Foo> { static const uint16_t value = 4; };
	inline void RegisterTypes()
	{
		xx::MemPool::Register<xx::BBuffer, xx::MPObject>();
		xx::MemPool::Register<xx::List<int>, xx::MPObject>();
		xx::MemPool::Register<::Foo, xx::MPObject>();
	}
}

void Dump(xx::MPObject* o)
{
	if (!o) return;
	xx::String_v str(o->mempool());
	o->ToString(*str);
	std::cout << str->C_str() << std::endl;
}

int main()
{
	xx::RegisterTypes();
	xx::MemPool mp;
	xx::BBuffer_v bb(mp);
	auto foo = mp.Create<Foo>();
	foo->childs->Add(foo);
	bb->WriteRoot(foo);
	Dump(bb);
	Foo* foo2;
	auto rtv = bb->ReadRoot(foo2);
	assert(!rtv);
	std::cout << (size_t)foo2 << std::endl;
	std::cout << (size_t)foo2->childs->At(0) << std::endl;
	return 0;
}


/*

// 测试 MHBox 值类型容器操作与序列化

int main()
{
	xx::RegisterTypes();
	xx::MemPool mp;
	xx::BBuffer_v bb(mp);
	xx::List_v<xx::String_v> list(mp);
	list->Emplace(mp, "a");
	list->Emplace(mp, "bb");
	list->Add(xx::String_v(mp, "ccc"));
	Dump(list);
	bb->Write(list);
	Dump(bb);

	xx::List_v<xx::String_v> list2(mp);
	if (auto rtv = bb->Read(list2))
	{
		std::cout << rtv << std::endl;
	}
	else
	{
		Dump(list2);
	}

	bb->offset = 0;
	try
	{
		xx::List_v<xx::String_v> list3(mp, bb);
		Dump(list3);
	}
	catch (int e)
	{
		std::cout << e << std::endl;
	}
	return 0;
}


int main()
{
	xx::RegisterTypes();
	xx::MemPool mp;
	xx::BBuffer_v bb(mp);
	xx::List_v<uint32_t> list(mp);
	list->AddMulti(1, 2, 3);
	Dump(list);
	bb->Write(list);
	Dump(bb);
	xx::List_v<uint32_t> list2(mp);
	if (auto rtv = bb->Read(list2))
	{
		std::cout << rtv << std::endl;
	}
	else
	{
		Dump(list2);
	}
	return 0;
}



*/
