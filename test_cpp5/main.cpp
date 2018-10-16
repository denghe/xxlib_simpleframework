#pragma execution_character_set("utf-8")
//#include "../pkg/PKG_class.h"

// todo: DictEx Update 函数还需要进一步测试. 看上去有老 HashCode 没被更新的问题

#include "xx_uv.h"

xx::MemPool mp;

template<typename...Args>
void Cout(Args const&...args)
{
	xx::String s(&mp);
	s.Append(args...);
	std::cout << s;
}

struct Foo
{
	int v1, v2;
};

int main2()
{
	xx::DictEx<Foo, decltype(Foo::v1), decltype(Foo::v2)> de(&mp);

	{
		Foo f{ 1, 2 };

		auto r = de.Add(f, f.v1, f.v2);
		Cout(r.success, ", ", r.index, '\n');

		r = de.Add(f, f.v1, f.v2);
		Cout(r.success, ", ", r.index, '\n');	// false
	}

	{
		Foo f{ 2, 3 };

		auto r = de.Add(f, f.v1, f.v2);
		Cout(r.success, ", ", r.index, '\n');

		f.v1 = 3;
		r = de.Add(f, f.v1, f.v2);
		Cout(r.success, ", ", r.index, '\n');	// false
	}

	de.Add(Foo{}, 3, 4);
	de.Add(Foo{}, 4, 5);
	de.Add(Foo{}, 5, 6);
	de.Add(Foo{}, 6, 7);

	Cout("find 0/3, 1/4 : ", de.Find<0>(3), " ", de.Find<1>(4), "\n");

	de.Remove<1>(4);
	assert(de.Find<0>(3) == -1);
	int idx = de.Find<0>(4);
	de.RemoveAt(idx);

	bool ok = de.Update<0>(1, 3);
	assert(ok);

	for (decltype(auto) iv : de)
	{
		Cout("k0: ", de.KeyAt<0>(iv.index), ", k1: ", de.KeyAt<1>(iv.index), ", foo = ", iv.value.v1, ", ", iv.value.v2, "\n");
	}

	return 0;
}

int main()
{
	main2();
	return 0;
}

