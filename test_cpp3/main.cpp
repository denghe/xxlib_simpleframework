#include "xx_mempool.h"
#include <algorithm>

namespace 麻将
{
	enum class 牌 : uint8_t
	{
		一筒 = 1,
		二筒,
		三筒,
		四筒,
		五筒,
		六筒,
		七筒,
		八筒,
		九筒,
		一条 = 11,
		二条,
		三条,
		四条,
		五条,
		六条,
		七条,
		八条,
		九条,
		一万 = 21,
		二万,
		三万,
		四万,
		五万,
		六万,
		七万,
		八万,
		九万,
		//红中 = 31,
		//发财 = 32,  // 33
		//白板 = 33,  // 35
	};

	struct 牌张
	{
		牌 牌;
		uint8_t 张;
	};

	struct 手牌 : xx::Object
	{
		xx::List_p<牌> 牌s;
		xx::List_p<牌张> 牌张s;
		手牌()
		{
			mempool().CreateTo(牌张s, 14);
		}

		void 填充牌张s()
		{
			assert(牌s->dataLen > 0);
			std::sort(牌s->buf, 牌s->buf + 牌s->dataLen);
			牌张s->Clear();
			auto p = 牌s->At(0);
			uint8_t c = 1;
			for (int i = 1; i < 牌s->dataLen; ++i)
			{
				if (牌s->At(i) == p)
				{
					c++;
				}
				else
				{
					牌张s->Add(牌张{ p, c });
					p = 牌s->At(i);
					c = 1;
				}
			}
			牌张s->Add(牌张{ p, c });
		}

		// len:1, count:2/3 代表拿掉 对子/刻子.   len:3, count:1 代表拿掉顺子
		xx::List_p<牌张> 拿掉牌(xx::List_p<牌张> const& 牌张s, int idx, int len, int count)
		{
			auto ps = mempool().CreatePtr<xx::List<牌张>>(14);
			auto& buf = 牌张s->buf;
			for (int i = 0; i < idx; ++i)
			{
				ps->Add(buf[i]);
			}
			for (int i = idx; i < idx + len; ++i)
			{
				auto p = buf[i];
				assert(p.张 >= count);
				if (p.张 > count)
				{
					ps->Add(牌张{ p.牌, (uint8_t)(p.张 - count) });
				}
			}
			for (int i = idx + len; i < 牌张s->dataLen; ++i)
			{
				ps->Add(buf[i]);
			}
			return ps;
		}

		bool 简单判断是否能胡()
		{
			for (int i = 0; i < 牌张s->dataLen; ++i)
			{
				if (牌张s->At(i).张 >= 2 && 简单判断是否能胡(拿掉牌(牌张s, i, 1, 2))) return true;
			}
			return false;
		}

		bool 简单判断是否能胡(xx::List_p<牌张> const& 牌张s)
		{
			if (牌张s->dataLen == 0) return true;
			// 拿掉刻
			for (int i = 0; i < 牌张s->dataLen; ++i)
			{
				if (牌张s->At(i).张 >= 3 && 简单判断是否能胡(拿掉牌(牌张s, i, 1, 3))) return true;
			}
			// 拿掉顺
			for (int i = 0; i < 牌张s->dataLen - 2; ++i)
			{
				if ((int)牌张s->At(i).牌 + 1 == (int)牌张s->At(i + 1).牌
					&& (int)牌张s->At(i).牌 + 2 == (int)牌张s->At(i + 2).牌
					&& 简单判断是否能胡(拿掉牌(牌张s, i, 3, 1))) return true;
			}
			return false;
		}
	};
}

int main()
{
	xx::MemPool mp;
	auto sp = mp.CreatePtr<麻将::手牌>();
	mp.CreateTo(sp->牌s);
	sp->牌s->AddMulti(
		//牌s = new xx.List<麻将.牌>(
		//    麻将.牌.二万, 麻将.牌.二万, 麻将.牌.二万,
		//    麻将.牌.一万, 麻将.牌.一万, 麻将.牌.一万,
		//    麻将.牌.五万, 麻将.牌.五万,
		//    麻将.牌.三万, 麻将.牌.三万, 麻将.牌.三万,
		//    麻将.牌.四万, 麻将.牌.四万, 麻将.牌.四万
		//)
		麻将::牌::一万, 麻将::牌::二万, 麻将::牌::三万,
		麻将::牌::一条, 麻将::牌::二条, 麻将::牌::三条,
		麻将::牌::五万, 麻将::牌::五万,
		麻将::牌::一筒, 麻将::牌::二筒, 麻将::牌::三筒,
		麻将::牌::四筒, 麻将::牌::二筒, 麻将::牌::三筒
	);
	sp->填充牌张s();
	xx::Stopwatch sw;
	int count = 0;
	for (int i = 0; i < 10000000; i++)
	{
		if (sp->简单判断是否能胡()) ++count;
	}
	mp.Cout("count = ", count, ", sw = ", sw());
}

