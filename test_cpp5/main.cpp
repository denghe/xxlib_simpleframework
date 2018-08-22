// todo: 用 sg 语法改进 uv 库
// todo: 所有回调理论上讲都应该用版本检查来探测回调后是否 this 变野
// todo: 所有 c api 回调实现如果有 call 用户回调, 都应该 try
// todo: cpp 版 uv 库不再用 try
// todo: 一些 catch ... 要改成 Release Disconnect 之类?
// todo: 一些 void 返回值要改成 int
// todo: serial 考虑改成 int
// todo: 将回调的 try 用宏包起来做开关

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
