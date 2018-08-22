// todo: 用 sg 语法改进 uv 库

// todo: noexcept 狂加一波

#include "xx.h"

class Foo
{
public:
	char* s1 = nullptr;
	char* s2 = nullptr;
	std::string str;
	Foo()
	{
		s1 = new char[100];
		xx::ScopeGuard sg_s1([this] { delete[] s1; });

		s2 = new char[200];
		xx::ScopeGuard sg_s2([this] { delete[] s2; });

		str.reserve(1000000000000000);	// will throw bad alloc

		sg_s2.Cancel();
		sg_s1.Cancel();
	}
};

int main()
{
	for (int i = 0; i < 99; ++i)
	{
		try
		{
			Foo f;
		}
		catch (...)
		{
		}
	}
	return 0;
}
