#include "xx_func.h"
#include <vector>
#include <memory>
#include <iostream>

int main()
{
	std::vector<xx::Func<void()>> fs;

	std::unique_ptr<int> ii(new int(123));
	fs.push_back([ i = std::move(ii) ]
	{
		std::cout << *i << std::endl;
	});

	for (auto& f : fs) if(f) f();
}
