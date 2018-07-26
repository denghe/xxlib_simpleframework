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
	// 判断两线段( p0-p1, p2-p3 )是否相交, 并且往 p 填充交点
	inline bool GetSegmentIntersection(Pos p0, Pos p1, Pos p2, Pos p3, Pos* p = nullptr)
	{
		Pos s02, s10, s32;
		float s_numer, t_numer, denom, t;
		s10.x = p1.x - p0.x;
		s10.y = p1.y - p0.y;
		s32.x = p3.x - p2.x;
		s32.y = p3.y - p2.y;

		denom = s10.x * s32.y - s32.x * s10.y;
		if (denom == 0) return false; // Collinear
		bool denomPositive = denom > 0;

		s02.x = p0.x - p2.x;
		s02.y = p0.y - p2.y;
		s_numer = s10.x * s02.y - s10.y * s02.x;
		if ((s_numer < 0) == denomPositive) return false; // No collision

		t_numer = s32.x * s02.y - s32.y * s02.x;
		if ((t_numer < 0) == denomPositive) return false; // No collision

		if (((s_numer > denom) == denomPositive) || ((t_numer > denom) == denomPositive))
			return false; // No collision
						  // Collision detected
		t = t_numer / denom;
		if (p)
		{
			p->x = p0.x + (t * s10.x);
			p->y = p0.y + (t * s10.y);
		}

		return true;
	}
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
