#include "main.h"
#include "xx.h"
using namespace xx;

// todo: 确定三种指针形态: Ptr, Ref, Weak   分别对应 std 之 unique_ptr, shared_ptr, weak_ptr

class Foo : public Object
{
public:
	inline static int fooCounter = 0;
	int index;
	Foo(MemPool* mempool)
		: Object(mempool)
		, index(fooCounter++)
	{}
	Foo(Foo const&) = delete;
	Foo& operator=(Foo const&) = delete;
	Foo(Foo &&) = default;
	Foo& operator=(Foo &&) = default;

	virtual void ToString(String & s) const override
	{
		s.Append("foo", index);
	}
};

int main()
{
	MemPool mp;
	std::cout << mp.Create<Foo>() << std::endl;

	return 0;
}
