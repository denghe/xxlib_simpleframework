#pragma once

#include <iostream>
#include <initializer_list>
template<typename ...Args>
void Cout(Args const&... args)
{
	std::initializer_list<int>{ ((std::cout << args), 0)... };
	std::cout.flush();
}
template<typename ...Args>
void CoutLine(Args const&... args)
{
	std::initializer_list<int>{ ((std::cout << args), 0)... };
	std::cout << std::endl;
}
