#define _CRT_SECURE_NO_WARNINGS
#include "xx_uv.h"
#include "../pkg/RPC_class.h"
void Xxx(xx::Object_p& o)
{
	std::cout << o.GetRefs() << std::endl;
}
int main()
{
	xx::MemPool mp;
	auto s = mp.MPCreatePtr<xx::String>("xxx");
	Xxx(s);
	auto L = mp.MPCreatePtr<xx::List<int>>();
	L->Add(1);
	L->Add(2);
	Xxx(L);
	auto D = mp.MPCreatePtr<xx::Dict<xx::String_p, int>>();
	Xxx(D);
	auto p = mp.MPCreatePtr<RPC::Client_Login::Login>();
	Xxx(p);
	return 0;
}
