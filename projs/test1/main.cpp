#include <xx_mempool.h>
#include <xx_bbuffer.h>
#include <iostream>

// todo: 改造序列化相关

// todo: 移除 autoRelease 的设计

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

int main()
{
	xx::RegisterTypes();
	xx::MemPool mp;
	auto bb = mp.Create<xx::BBuffer>();
	auto list = mp.Create<xx::List<int>>();
	list->AddMulti(1, 2, 3);
	bb->WriteRef(list);
	bb->Release();
	list->Release();
	return 0;
}
