﻿#pragma execution_character_set("utf-8")
#include "xx.h"
#define var decltype(auto)

#include <unordered_map>
#include <vector>
#include <memory>
struct Foo
{
	std::vector<int> ints;
	int a, b, c, d, e, f;
};
using FooPtr = std::shared_ptr<Foo>;
using FooPtrs = std::vector<FooPtr>;
using FooPtrsPtr = std::shared_ptr<FooPtrs>;
using Map = std::unordered_map<int, FooPtrsPtr>;
using MapPtr = std::shared_ptr<Map>;

int main()
{
	{
		xx::Stopwatch sw;
		auto map = MapPtr(new Map());
		for (int i = 0; i < 10000; i++)
		{
			auto foos = FooPtrsPtr(std::make_shared<FooPtrs>());
			for (int j = 0; j < 100; j++)
			{
				auto foo = std::make_shared<Foo>(Foo{ {1,2,3}, 1,2,3,4,5,6 });
				foos->push_back(foo);
			}
			map->operator[](i) = foos;
		}
		std::cout << sw() << std::endl;
		std::cout << map->size() << std::endl;

		size_t counter = 0;
		for (size_t n = 0; n < 100; n++)
		{
			for (int i = 0; i < 10000; i++)
			{
				for (int j = 0; j < 100; j++)
				{
					var iter = map->find(i);
					auto foo = iter->second->at(j);
					counter += foo->a;
					counter += foo->b;
					counter += foo->c;
					counter += foo->d;
					counter += foo->e;
					counter += foo->f;
					for (var o : foo->ints) counter += o;
				}
			}
		}
		std::cout << sw() << std::endl;
		std::cout << counter << std::endl;
	}

	std::cout << std::endl;

	{
		xx::Stopwatch sw;
		std::unordered_map<int, std::vector<Foo>> map;
		for (int i = 0; i < 10000; i++)
		{
			var kv = map.emplace(i, std::vector<Foo>());
			for (int j = 0; j < 100; j++)
			{
				kv.first->second.push_back(Foo{ {1,2,3}, 1,2,3,4,5,6 });
			}
		}
		std::cout << sw() << std::endl;
		std::cout << map.size() << std::endl;

		size_t counter = 0;
		for (size_t n = 0; n < 100; n++)
		{
			for (int i = 0; i < 10000; i++)
			{
				for (int j = 0; j < 100; j++)
				{
					var iter = map.find(i);
					var foo = iter->second[j];
					counter += foo.a;
					counter += foo.b;
					counter += foo.c;
					counter += foo.d;
					counter += foo.e;
					counter += foo.f;
					for (var o : foo.ints) counter += o;
				}
			}
		}
		std::cout << sw() << std::endl;
		std::cout << counter << std::endl;
	}

	std::cout << std::endl;

	{
		xx::Stopwatch sw;
		xx::MemPool mp;
		xx::Dict<int, xx::List<Foo>> map(&mp);
		for (int i = 0; i < 10000; i++)
		{
			var r = map.Add(i, xx::List<Foo>(&mp));
			for (int j = 0; j < 100; j++)
			{
				map.At(r.index).Emplace(Foo{ {1,2,3}, 1,2,3,4,5,6 });
			}
		}
		std::cout << sw() << std::endl;
		std::cout << map.Count() << std::endl;

		size_t counter = 0;
		for (size_t n = 0; n < 100; n++)
		{
			for (int i = 0; i < 10000; i++)
			{
				for (int j = 0; j < 100; j++)
				{
					var idx = map.Find(i);
					var foo = map.ValueAt(idx)[j];
					counter += foo.a;
					counter += foo.b;
					counter += foo.c;
					counter += foo.d;
					counter += foo.e;
					counter += foo.f;
					for (var o : foo.ints) counter += o;
				}
			}
		}
		std::cout << sw() << std::endl;
		std::cout << counter << std::endl;
	}

	return 0;
}


//#pragma execution_character_set("utf-8")
//#include "xx.h"
//#define var decltype(auto)
//xx::MemPool mp;
//int main()
//{
//	xx::String_p s1;
//	xx::String_p s2;
//	xx::String_p s;
//	s1.MPCreate(&mp, "asdf");
//
//	s.Copy(s1);
//	std::cout << s << std::endl;
//
//	s.Copy(s2);
//	std::cout << s << std::endl;
//	return 0;
//}
