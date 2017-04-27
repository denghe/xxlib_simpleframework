#include "xx_queue.h"
#include <iostream>
#include <queue>
int main()
{
	//{
	//	xx::Stopwatch sw;
	//	size_t counter = 0;
	//	for (int j = 0; j < 999; j++)
	//	{
	//		std::queue<std::string> q;
	//		for (int i = 0; i < 9999; i++)
	//		{
	//			q.push(std::to_string(i) + "12345678901234567890");
	//		}
	//		while (!q.empty())
	//		{
	//			counter += q.front().size();
	//			q.pop();
	//		}
	//	}
	//	std::cout << "elapsed ms = " << sw() << ", counter = " << counter << std::endl;
	//}
	//{
	//	xx::MemPoolBase mpb;
	//	xx::Stopwatch sw;
	//	size_t counter = 0;
	//	for (int j = 0; j < 999; j++)
	//	{
	//		xx::Queue_v<xx::String_v> q(mpb);
	//		for (int i = 0; i < 9999; i++)
	//		{
	//			q->Emplace(mpb)->Append(i, "12345678901234567890");
	//		}
	//		while (!q->Empty())
	//		{
	//			counter += q->Peek()->dataLen;
	//			q->Pop();
	//		}
	//	}
	//	std::cout << "elapsed ms = " << sw() << ", counter = " << counter << std::endl;
	//}

	{
		xx::Stopwatch sw;
		size_t counter = 0;
		for (int j = 0; j < 9999; j++)
		{
			std::queue<int> q;
			for (int i = 0; i < 9999; i++)
			{
				q.push(i);
			}
			while (!q.empty())
			{
				counter += q.front();
				q.pop();
			}
		}
		std::cout << "elapsed ms = " << sw() << ", counter = " << counter << std::endl;
	}
	{
		xx::Stopwatch sw;
		size_t counter = 0;
		{
			xx::MemPoolBase mpb;
			for (int j = 0; j < 9999; j++)
			{
				xx::Queue_v<int> q(mpb);
				for (int i = 0; i < 9999; i++)
				{
					q->Emplace(i);
				}
				while (!q->Empty())
				{
					counter += q->Peek();
					q->Pop();
				}
			}
		}
		std::cout << "elapsed ms = " << sw() << ", counter = " << counter << std::endl;
	}

	return 0;
}
