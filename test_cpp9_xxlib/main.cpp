#include "main.h"
#include "xx.h"

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

	auto queue = mp.CreatePtr<xx::Queue<Foo>>();
	queue->Emplace();
	queue->Emplace();

	auto dict = mp.CreatePtr<xx::Dict<int, Foo>>();
	dict->Emplace(false, 1);
	dict->Emplace(false, 2);

	auto str = mp.CreatePtr<xx::String>();
	str->Assign(123);
	str->Append(123, "asdf", str);
	//str->AppendFormat("  {0}  {0}", str);
	std::cout << str->c_str() << std::endl;

	return 0;
}
