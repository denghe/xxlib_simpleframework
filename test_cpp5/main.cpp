// todo: noexcept 狂加一波

#include "fixed_function.hpp"

struct a
{
	kapala::fixed_function<void(), 128> cb_;
	inline void set_cb(kapala::fixed_function<void()>&& cb)
	{
		cb_ = [this, cb = std::move(cb)]
		{
			cb();
		};
	}
	inline void operator()()
	{
		if (cb_) cb_();
	}
};

int main()
{
	a A;
	A.set_cb([] {});
	A();
	return 0;
}
