#include <xx_mempool.h>
#include <xx_bbuffer.h>
#include <iostream>

// todo: 改造序列化相关
// todo: 移除 autoRelease 的设计, 考虑增加对 Xxxxx_v 的进一步支持, 比如用于容器 item 时, call 默认构造函数传 mempool()

struct Foo : xx::MPObject
{
	xx::List<Foo*>* childs = nullptr;

#pragma region ctor interfaces
	Foo()
	{
		mempool().CreateTo(childs);
	}
	Foo(xx::BBuffer *bb)
	{
		if (auto rtv = bb->Read(childs)) throw rtv;
	}
	~Foo()
	{
		mempool().SafeRelease(childs);
	}

	inline virtual void ToString(xx::String &str) const override
	{
		if (tsFlags())
		{
			str.Append("{ ... }");
			return;
		}
		else tsFlags() = 1;

		str.Append("{ \"childs\" : ", childs, " }");

		tsFlags() = 0;
	}

	inline virtual void ToBBuffer(xx::BBuffer &bb) const override
	{
		//this->BaseType::ToBBuffer(bb);
		bb.Write(childs);
	};

	inline virtual int FromBBuffer(xx::BBuffer &bb) override
	{
		//if (auto rtv = this->BaseType::FromBBuffer(bb)) return rtv;
		if (auto rtv = bb.Read(childs)) return rtv;
		return 0;
	};
#pragma endregion
};

namespace xx
{
	template<> struct TypeId<::Foo> { static const uint16_t value = 2; };
	template<> struct TypeId<List<Foo*>> { static const uint16_t value = 3; };
	inline void RegisterTypes()
	{
		xx::MemPool::Register<::Foo, xx::MPObject>();
		xx::MemPool::Register<xx::List<::Foo*>, xx::MPObject>();
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
	{
		xx::BBuffer_v bb(mp);
		auto foo = mp.Create<Foo>();
		foo->childs->Add(foo);
		Dump(foo);
		bb->WriteRoot(foo);
		Dump(bb);
		Foo* foo2;
		auto rtv = bb->ReadRoot(foo2);
		assert(!rtv);
		Dump(foo2);
		mp.Release(foo);
		mp.Release(foo2);
	}
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
