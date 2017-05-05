#include <uv.h>
#include <xx_bbqueue.h>
int main()
{
	xx::MemPoolBase mp;
	xx::BBQueue_v bbq(mp);
	xx::List_v<uv_buf_t> outBufs(mp);

	auto bb = mp.CreateWithoutTypeId<xx::BBuffer>();
	bb->Write((uint8_t)1, (uint8_t)2, (uint8_t)3);
	bbq->Push(bb);
	uint32_t n;
	n = bbq->PopTo(&outBufs.instance, 1);
	n = bbq->PopTo(&outBufs.instance, 1);
	n = bbq->PopTo(&outBufs.instance, 1);
	n = bbq->PopTo(&outBufs.instance, 1);

	return 0;
}
