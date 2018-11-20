#pragma execution_character_set("utf-8")
#include "xx.h"
#define var decltype(auto)
xx::MemPool mp;
int main()
{
	for (size_t k = 0; k < 10; k++)
	{
		xx::Stopwatch sw;
		size_t count = 0;
		xx::BBuffer bb(&mp);
		for (int j = 0; j < 10000000; j++)
		{
			bb.Clear();
			for (int i = 0; i < 20; i++)
			{
				bb.Write(1234567);
			}
			count += bb.dataLen;
			var bb2 = mp.MPCreatePtr<xx::BBuffer>(std::make_pair(bb.buf, bb.dataLen));	// 模拟发包时复制
		}
		std::cout << "write 10000000 times, count = " << count << ", ms = " << sw() << std::endl;
	}
	return 0;
}
