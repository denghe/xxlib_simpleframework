#include <xx_mempool.h>
#include <xx_bbuffer.h>
#include <iostream>

// todo: 改造序列化相关
// todo: 移除 autoRelease 的设计, 考虑增加对 Xxxxx_v 的进一步支持, 比如用于容器 item 时, call 默认构造函数传 mempool()

namespace xx
{
	template<> struct TypeId<BBuffer> { static const uint16_t value = 2; };
	template<> struct TypeId<List<int>> { static const uint16_t value = 3; };
	inline void RegisterTypes()
	{
		xx::MemPool::Register<xx::BBuffer, xx::MPObject>();
		xx::MemPool::Register<xx::List<int>, xx::MPObject>();
	}
}

void Dump(xx::MemPool& mp, xx::MPObject* o)
{
	xx::String_v str(mp);
	o->ToString(*str);
	std::cout << str->C_str() << std::endl;
}

int main()
{
	xx::RegisterTypes();
	xx::MemPool mp;
	xx::BBuffer_v bb(mp);
	xx::List_v<xx::String_v> list(mp);
	list->Emplace(mp, "a");
	list->Emplace(mp, "bb");
	list->Add(xx::String_v(mp, "ccc"));
	Dump(mp, list);
	bb->Write(list);
	Dump(mp, bb);
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
	Dump(mp, list);
	bb->Write(list);
	Dump(mp, bb);
	return 0;
}

*/
