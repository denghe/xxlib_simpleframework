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
		Ptr<Foo> foo2 = f.Lock();
		foo2.Assign(std::move(foo));
		foo2.Release();
		std::cout << f << std::endl;
	}
	{
		Dict<String, String> d(&mp);
		std::cout << d.Count() << std::endl;
		d.Add(String(&mp, "a"), String(&mp, "b"));
		std::cout << d.Count() << std::endl;
		d.Add(String(&mp, "a"), String(&mp, "b"));
		std::cout << d.Count() << std::endl;
	}
	{
		Dict<String_p, String_p> d(&mp);
		std::cout << d.Count() << std::endl;
		auto a = mp.Create<String>("a");
		d.Add(mp.Create<String>("a"), mp.Create<String>("b"));
		std::cout << d.Count() << std::endl;
		d.Add(mp.Create<String>("a"), mp.Create<String>("b"));
		std::cout << d.Count() << std::endl;
	}
	return 0;
}
