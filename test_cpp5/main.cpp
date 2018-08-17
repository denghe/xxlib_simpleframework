// todo: noexcept 狂加一波

#include "xx.h"
#include <iostream>
void f1(int n)
{
	std::unordered_map<std::string, size_t> map;
	size_t count = 0;
	xx::Stopwatch sw;
	auto s = /*std::to_string(i) + */"012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789";
	for (size_t i = 0; i < n; i++)
	{
		map[s] = i;
		count += i;	// 防优化
		map.erase(s);
	}
	std::cout << "elapsed ms = " << sw() << ", count = " << count << ", map.size = " << map.size() << std::endl;
}
void f2(int n)
{
	xx::MemPool mp;
	xx::Dict<xx::String, size_t> map(&mp);
	map.Reserve(n);
	size_t count = 0;
	xx::String s(&mp);
	s.Append(/*i, */"012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789");
	xx::Stopwatch sw;
	for (size_t i = 0; i < n; i++)
	{
		auto idx = map.Add(xx::String(&mp, s), i).index;
		count += i;	// 防优化
		map.RemoveAt(idx);
	}
	std::cout << "elapsed ms = " << sw() << ", count = " << count << ", map.size = " << map.Count() << std::endl;
}
int main()
{
	f1(1000000);
	f2(1000000);
	return 0;
}
