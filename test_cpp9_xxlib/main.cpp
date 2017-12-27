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

	auto list = mp.CreatePtr<xx::List<Foo>>();
	list->Emplace();
	list->Emplace();

	auto list2 = mp.CreatePtr<xx::List<Foo_p>>();
	list2->Emplace();
	list2->Emplace();

	auto list3 = mp.CreatePtr<xx::List<int>>();
	list3->Emplace();
	list3->Emplace();

	return 0;
}
