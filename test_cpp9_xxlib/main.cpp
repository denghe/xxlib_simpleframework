#include "main.h"
#include "xx_list.h"

class Foo : public xx::Object
{
public:
	typedef xx::Object BaseType;
	Foo(xx::MemPool* mempool) : BaseType(mempool) {}
};
using Foo_p = xx::Ptr<Foo>;

int main()
{
	xx::MemPool mp;

	auto list = xx::List<Foo>::CreatePtr(&mp);
	list->Emplace();
	list->Emplace();

	auto list2 = xx::List<Foo_p>::CreatePtr(&mp);
	list2->Emplace();
	list2->Emplace();

	auto list3 = xx::List<int>::CreatePtr(&mp);
	list3->Emplace();
	list3->Emplace();

	return 0;
}
