#pragma execution_character_set("utf-8")
#include "../pkg/PKG_class.h"

// todo: 对于 "真身" 不在序列化类中的情况, 考虑将 "真身" 放在 BBuffer 的临时队列中?

int main(int argc, char const* const* argv)
{
	PKG::AllTypesRegister();
	xx::MemPool mp;

	auto f = mp.MPCreatePtr<PKG::Foo>();
	f->refFoo = f;
	f->refFoos.MPCreate(&mp);
	f->refFoos->Add(f);

	std::cout << f << std::endl;
	std::cout << f.GetRefs() << std::endl;

	xx::BBuffer bb(&mp);
	bb.WriteRoot(f);
	std::cout << bb << std::endl;

	xx::Object_p o;
	auto r = bb.ReadRoot(o);
	assert(!r);
	std::cout << o << std::endl;
	std::cout << o.GetRefs() << std::endl;

	return 0;
}
