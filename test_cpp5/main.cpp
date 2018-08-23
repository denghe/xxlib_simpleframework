// todo: noexcept 狂加一波
// todo: 用 sg 语法改进各种库
// todo: xx_uv 从 c# 那边复制备注

#include <iostream>
#include <functional>

template<typename Function>
struct test
{
	Function fuck_;
	void set(Function&& fuck)
	{
		auto fuck_lambda = [this, fuck = std::move(fuck)]
		{
			fuck();
		};
		std::cout << std::boolalpha;
		std::cout << std::is_trivially_copyable<decltype(fuck_lambda)>::value << std::endl;

		fuck_ = [this, fuck = std::move(fuck)]
		{
			fuck();
		};
	}
	void operator()() { fuck_(); }
};

int main()
{
	test<std::function<void()>> Test;
	Test.set([] {});
	return 0;
}
