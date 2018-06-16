#pragma once

namespace xx
{
	// 用来在 CatchFish 中存坐标
	struct Pos
	{
		float x = 0, y = 0;

		inline bool operator==(const Pos& v) const
		{
			return x == v.x && y == v.y;
		}

		inline Pos& operator+=(Pos const& v)
		{
			x += v.x;
			y += v.y;
			return *this;
		}
		inline Pos operator+(Pos const& v) const
		{
			return Pos{ x + v.x, y + v.y };
		}
		inline Pos operator-(Pos const& v) const
		{
			return Pos{ x - v.x, y - v.y };
		}
		inline Pos operator*(float s) const
		{
			return Pos{ this->x * s, this->y * s };
		}
		inline Pos operator/(float s) const
		{
			return Pos{ this->x / s, this->y / s };
		}
		inline float distance(Pos const& v) const
		{
			float dx = v.x - x;
			float dy = v.y - y;
			return std::sqrt(dx * dx + dy * dy);
		}
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
