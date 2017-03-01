#include "xx_luahelper.h"
#include <iostream>

int main()
{
	xx::MemPoolBase mp;
	auto list = mp.CreateWithoutTypeId<xx::List<int>>();
	list->AddMulti(1, 2, 3, 4, 5);
	return 0;
}
