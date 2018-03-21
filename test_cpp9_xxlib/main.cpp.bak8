#include "xx.h"

struct CoroutineTest
{
	int x = 0, y = 0;
	int lineNumber = 0;
	int Update()
	{
		XX_CORO_BEGIN()
		{
			x = 1;
			y = 2;
		}
		XX_CORO_(1)
		{
			if (x < y)
			{
				++x;
				XX_CORO_YIELDTO(1);
			}
		}
		XX_CORO_(2)
		{
			if (x > 0)
			{
				--x;
				XX_CORO_YIELDTO(2);
			}
		}
		XX_CORO_(3)
		{
			if (y < 9)
			{
				++y;
				XX_CORO_YIELDTO(1);
			}
			return -1;
		}
		XX_CORO_END()
			return 0;
	}
};

int main()
{
	CoroutineTest ct;
	while (!ct.Update())
	{
		std::cout << ct.x << std::endl;
	};
	return 0;
}
