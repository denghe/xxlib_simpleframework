#pragma execution_character_set("utf-8")
#include "xx.h"

struct Foo
{
	std::string s1;
	Foo()
	{
		std::cout << "create" << std::endl;
	}
	Foo(Foo const& o)
		: s1(o.s1)
	{
		std::cout << "copy" << std::endl;
	}
	Foo(Foo&& o)
		: s1(std::move(o.s1))
	{
		std::cout << "move" << std::endl;
	}
	~Foo()
	{
		std::cout << "release" << std::endl;
	}
};

std::function<void()> func;
//kapala::fixed_function<void()> func;

int main()
{
	std::cout << "1" << std::endl;
	func = [f = std::move(Foo())]{};
	std::cout << "2" << std::endl;
	func = nullptr;
	std::cout << "3" << std::endl;

	return 0;
}
