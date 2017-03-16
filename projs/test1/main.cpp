#include "xx_bbuffer.h"
#include "xx_dict.h"
#include <iostream>

int main()
{
	xx::MemPoolBase mp;
	xx::BBuffer_v bb(mp);
	xx::String_v s(mp);
	bb->Write(1, 2, 3, 4, 5);
	bb->ToString(*s);
	std::cout << s->C_str() << std::endl;

	return 0;
}
