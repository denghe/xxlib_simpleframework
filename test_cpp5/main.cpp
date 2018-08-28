// todo: std::optional 用起来以支持数据库可空字段, 进而统一 db & pkg 生成器
// todo: C# BBuffer 需要增加对 Nullable 的重载
// todo: C# LUA 的生成物需要同步修改

namespace xx{ struct Pos {}; }
using Sprite = void*;
using Animation = void*;
using ClientPeer = void*;
#include <xx.h>
#include "../pkg/PKG_class.h"

xx::MemPool mp_, *mp = &mp_;
#define var decltype(auto)
int main()
{
	xx::MemPool::RegisterInternals();
	PKG::AllTypesRegister();
	
	var f = mp->MPCreatePtr<PKG::Foo>();
	std::cout << f << std::endl;

	f->id = 1;
	f->age = 123;
	f->floats.MPCreate(mp);
	f->floats->Add(1.2f);
	f->floats->Emplace();
	f->floats->Add(3.4f);

	var bb = mp->MPCreatePtr<xx::BBuffer>();
	bb->WriteRoot(f);
	std::cout << bb << std::endl;

	decltype(f) f2;
	int r = bb->ReadRoot(f2);
	assert(!r);
	std::cout << f2 << std::endl;
	return 0;
}
