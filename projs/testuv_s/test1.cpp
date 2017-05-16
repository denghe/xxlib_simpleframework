#include <uv.h>
#include <xx_bbqueue.h>
#include <iostream>

void Dump(xx::List<uv_buf_t> const& bufs)
{
	std::cout << "bufs.dataLen = " << bufs.dataLen << std::endl;
	for (auto& buf : bufs)
	{
		std::cout << "buf.len = " << buf.len << ", buf.base = " << (size_t)buf.base << std::endl;
	}
}

int main()
{
	xx::MemPoolBase mp;
	xx::BBQueue_v bbq(mp);
	xx::List_v<uv_buf_t> outBufs(mp);

	auto bb = mp.CreateWithoutTypeId<xx::BBuffer>();
	bb->Write("123");
	bbq->Push(bb);

	bb = bbq->PopLastBB();
	if (!bb) bb = mp.CreateWithoutTypeId<xx::BBuffer>();
	bb->Write("456");
	bbq->Push(bb);

	bb = bbq->PopLastBB();
	if (!bb) bb = mp.CreateWithoutTypeId<xx::BBuffer>();
	bb->Write("789");
	bbq->Push(bb);

	while (auto n = bbq->PopTo(*outBufs, 20))
	{
		std::cout << "pop size = " << n << std::endl;
		if (n) Dump(*outBufs);
	}

	return 0;
}
