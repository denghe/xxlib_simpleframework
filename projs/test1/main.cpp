#include "xx_mempool.h"
#include <iostream>

using namespace xx;

struct A : MPObject {};
struct B : A {};
struct C : B {};
struct D : A {};
struct E : MPObject {};

typedef MemPool<A, B, C, D, E> MP;

int main()
{
	MP mp;
	A* a = mp.Create<C>();

	Stopwatch sw;
	auto counter = 0ul;
	for (int i = 0; i < 99999999; ++i)
	{
		if (mp.TryCast<D>(a))
		{
			counter++;
		}
	}
	std::cout << counter << ", elapsed = " << sw() << std::endl;

	counter = 0ul;
	for (int i = 0; i < 99999999; ++i)
	{
		if (dynamic_cast<D*>(a))
		{
			counter++;
		}
	}
	std::cout << counter << ", elapsed = " << sw() << std::endl;

	return 0;
}
