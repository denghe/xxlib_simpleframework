#include "xx.h"
int main()
{
	xx::MemPool mp;
	xx::String str(&mp);
	str.Append("asdf", 123, "qwer");
	std::cout << str << std::endl;
	return 0;
}
