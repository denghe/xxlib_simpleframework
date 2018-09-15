#pragma execution_character_set("utf-8")
#include "../pkg/PKG_class.h"

// todo: 支持 List_p<xxxx_r> 序列化. 在生成 TypeId & Register 注册时 List<> 内部需要区分 _p _r
// todo: cpp 库那边应该还需要写适配模板
// todo: 对于 "真身" 不在序列化类中的情况, 考虑将 "真身" 放在 BBuffer 的临时队列中?

int main(int argc, char const* const* argv)
{
	PKG::AllTypesRegister();
	xx::MemPool mp;

	//auto c = mp.MPCreatePtr<PKG::Container>();
	//c->foos.MPCreate(&mp);

	//auto f = mp.MPCreatePtr<PKG::Foo>();
	//f->childs.MPCreate(&mp);
	//c->foos->Add(f);

	//auto fe = mp.MPCreatePtr<PKG::FooEx>();
	//c->foos->Add(fe);

	//f->childs->Add(f);
	//f->childs->Add(fe);
	//fe->parent = f;

	//xx::BBuffer bb(&mp);
	//bb.WriteRoot(c);
	//std::cout << bb << std::endl;

	return 0;
}
