#include "main.h"
#include "xx.h"
using namespace xx;

class Foo : public Object
{
public:
	inline static int fooCounter = 0;
	int index;
	Foo(MemPool* mempool)
		: Object(mempool)
		, index(fooCounter++)
	{
		std::cout << "Foo() index = " << index << std::endl;
	}
	Foo(Foo const&) = delete;
	Foo& operator=(Foo const&) = delete;
	Foo(Foo &&) = default;
	Foo& operator=(Foo &&) = default;
	~Foo()
	{
		std::cout << "~Foo()" << std::endl;
	}

	virtual void ToString(String & s) const override
	{
		s.Append("foo", index);
	}
};

int main()
{
	MemPool mp;
	{
		auto foo = mp.Create<Foo>();
		Ref<Foo> f(foo);
		std::cout << f << std::endl;
	}
	{
		Ref<Foo> f(mp.Create<Foo>());
		std::cout << f << std::endl;
	}
	{
		auto foo = mp.Create<Foo>();
		Ref<Foo> f(foo);
		std::cout << f << std::endl;
		decltype(auto) foo2 = f.Lock();
		foo.Release();
		std::cout << f << std::endl;
	}
	return 0;
}
