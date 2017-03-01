#include "xx_mempool.h"
#include "xx_list.h"
#include <iostream>

using namespace xx;

struct A;
struct B;
typedef MemPool<
	A, 
	B, 
	List<A*>
> MP;

struct A : MPObject {};
struct B : MPObject
{
	A* a;
	B()
	{
		mempool<MP>().CreateTo(a);
	}
	~B()
	{
		a->Release();
	}
};

int main()
{
	MP mp;
	auto b = mp.Create<B>();
	b->Release();

	auto list = mp.Create<List<A*>>();
	list->Add(mp.Create<A>());
	list->Add(mp.Create<A>());
	list->Add(mp.Create<A>());
	list->Add(mp.Create<A>());

	return 0;
}
