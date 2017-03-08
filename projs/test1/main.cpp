#include <iostream>
#include <utility>
#include <tuple>
#include <functional>
#include "xx_mempool.h"
#include "xx_list.h"

using namespace std::placeholders;
struct Foo
{
	void Func(int a, int b)
	{
		std::cout << a << b;
	}
};

int main()
{
	auto foo = new Foo();
	auto tuple = std::make_tuple(1, 2);
	std::apply(std::bind(&Foo::Func, foo, _1, _2), tuple);
	return 0;
}
