#pragma execution_character_set("utf-8")
#include "xx.h"
#define var decltype(auto)
xx::MemPool mp;
int main()
{
	xx::String_p s1;
	xx::String_p s2;
	xx::String_p s;
	s1.MPCreate(&mp, "asdf");

	s.Copy(s1);
	std::cout << s << std::endl;

	s.Copy(s2);
	std::cout << s << std::endl;
	return 0;
}
