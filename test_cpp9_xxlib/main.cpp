#include "xx_uv.h"

int main()
{
	xx::UvLoop loop;
	try
	{
		auto listener = loop.CreateTcpListener();
	}
	catch (int e)
	{
		std::cout << "catch e = " << e << std::endl;
	}
	return 0;
}
