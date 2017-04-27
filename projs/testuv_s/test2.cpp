#include "xx_queue.h"
#include <iostream>
#include <queue>
int main()
{
	{
		std::queue<std::string> q;
		xx::Stopwatch sw;
		for (int i = 0; i < 9999; i++)
		{
			q.push(std::to_string(i));
		}
		size_t counter = 0;
		while (!q.empty())
		{
			counter += q.front().size();
			q.pop();
		}
		std::cout << "elapsed ms = " << sw() << ", counter = " << counter << std::endl;
	}
	{
		xx::MemPoolBase mpb;
		xx::MemHeaderBox<xx::Queue<std::string>> q(mpb);
		xx::Stopwatch sw;
		for (int i = 0; i < 9999; i++)
		{
			q->Enqueue(std::to_string(i));
		}
		size_t counter = 0;
		while (!q->Empty())
		{
			counter += q->Peek().size();
			q->Pop();
		}
		std::cout << "elapsed ms = " << sw() << ", counter = " << counter << std::endl;
	}

	return 0;
}
