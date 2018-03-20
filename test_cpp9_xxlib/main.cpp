#include "xx.h"

int main()
{
	xx::MemPool mp;
	xx::String_p str;
	mp.MPCreateTo(str);
	str->Append("a");
	std::cout << str << std::endl;
	xx::BBuffer bb(&mp);
	bb.WriteRoot(str);
	std::cout << bb << std::endl;
	return 0;
}

//int main()
//{
//	xx::MemPool mp;
//	xx::String str(&mp);
//	str.Append("asdf", 123, "qwer");
//	std::cout << str << std::endl;
//	xx::BBuffer bb(&mp);
//	bb.Write(str);
//	std::cout << bb << std::endl;
//	return 0;
//}
