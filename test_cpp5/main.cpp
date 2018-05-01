#include "xx_uv.h"

namespace xx
{
	// 用来在 CatchFish 中存坐标
	struct Pos
	{
		float x = 0, y = 0;
	};

	// 适配 Pos 之 ToString
	template<>
	struct StrFunc<Pos, void>
	{
		static inline void WriteTo(String& s, Pos const &in)
		{
			s.Reserve(s.dataLen + 40);
			s.Append("{ \"x\":", in.x, ", \"y\":", in.y, " }");
		}
	};

	// 适配 Pos 之 序列化 & 反序列化
	template<>
	struct BytesFunc<Pos, void>
	{
		static inline void WriteTo(BBuffer& bb, Pos const &in)
		{
			bb.Reserve(bb.dataLen + sizeof(Pos));
			memcpy(bb.buf + bb.dataLen, &in, sizeof(Pos));
			bb.dataLen += sizeof(Pos);
		}
		static inline int ReadFrom(BBuffer& bb, Pos &out)
		{
			if (bb.offset + sizeof(Pos) > bb.dataLen) return -1;
			memcpy(&out, bb.buf + bb.offset, sizeof(Pos));
			bb.offset += sizeof(Pos);
			return 0;
		}
	};
}

#include "../pkg/PKG_class.h"

class Scene : public PKG::CatchFish::Scene
{

};

int main()
{


	return 0;
}
