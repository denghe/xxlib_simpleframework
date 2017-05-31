#include <uv.h>
#include <xx_bbqueue.h>
#include <iostream>

namespace xx
{
	template<>
	struct StrFunc<uv_buf_t, void>
	{
		static inline uint32_t Calc(uv_buf_t const &in)
		{
			return 128 + in.len * 5;
		}
		static inline uint32_t WriteTo(char *dstBuf, uv_buf_t const &in)
		{
			uint32_t offset = 0;
			offset += StrWriteTo(dstBuf + offset, "{ \"type\" : \"uv_buf_t\", \"len\" : ", in.len, ", \"base\" : [ ");
			for (ULONG i = 0; i < in.len; ++i) offset += StrWriteTo(dstBuf + offset, (uint8_t)in.base[i], ", ");
			if (in.len) offset -= 2;
			offset += StrWriteTo(dstBuf + offset, " ] }");
			return offset;
		}
	};
}

void Dump(xx::MPObject* o)
{
	if (!o) return;
	xx::String_v str(o->mempool());
	o->ToString(*str);
	std::cout << str->C_str() << std::endl;
}

int main()
{
	xx::MemPool mp;
	xx::BBQueue_v bbq(mp);
	xx::List_v<uv_buf_t> outBufs(mp);

	auto bb = mp.Create<xx::BBuffer>();
	bb->Write("123");
	bbq->Push(bb);

	bb = mp.Create<xx::BBuffer>();
	bb->Write("456");
	bbq->Push(bb);

	bb = mp.Create<xx::BBuffer>();
	bb->Write("789");
	bbq->Push(bb);

	while (auto n = bbq->PopTo(*outBufs, 5))
	{
		std::cout << "pop size = " << n << std::endl;
		Dump(outBufs);
	}
	std::cout << std::endl;

	bb = bbq->PopLastBB();
	bb->Write("123");
	bbq->Push(bb);

	bb = bbq->PopLastBB();
	bb->Write("456");
	bbq->Push(bb);

	bb = bbq->PopLastBB();
	bb->Write("789");
	bbq->Push(bb);

	while (auto n = bbq->PopTo(*outBufs, 5))
	{
		std::cout << "pop size = " << n << std::endl;
		Dump(outBufs);
	}

	return 0;
}
