#pragma execution_character_set("utf-8")
#include "../pkg/PKG_class.h"

void Test()
{
	PKG::AllTypesRegister();
	xx::MemPool mp;
	auto f = mp.MPCreatePtr<PKG::Foo>();
	f->refFoo = f;
}

int main(int argc, char const* const* argv)
{
	Test();
	Test();
	Test();
	Test();
	Test();
	Test();
	return 0;
}
