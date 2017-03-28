#pragma once
#include <array>
#include <cstdint>

namespace xx
{

#define TWO_SAME_TYPE_FIELD_CLASS_ATTACHCODE( T, CN, f1, f2 )		\
	inline void SetZero()											\
	{																\
		f1 = f2 = 0;												\
	}																\
	inline friend CN operator*(CN const& a, T const& b)		 		\
	{																\
		return{ a.f1 * b, a.f2 * b };								\
	}																\
	friend CN operator*(CN const& a, CN const& b)	   				\
	{																\
		return{ a.f1 * b.f1, a.f2 * b.f2 };							\
	}																\
	inline friend CN operator/(CN const& a, T const& b)		   		\
	{																\
		return{ a.f1 / b, a.f2 / b };								\
	}																\
	inline friend CN operator/(CN const& a, CN const& b)	   		\
	{																\
		return{ a.f1 / b.f1, a.f2 / b.f2 };							\
	}																\
	inline friend CN operator+(CN const& a, T const& b)		   		\
	{																\
		return{ a.f1 + b, a.f2 + b };								\
	}																\
	inline friend CN operator+(CN const& a, CN const& b)	   		\
	{																\
		return{ a.f1 + b.f1, a.f2 + b.f2 };							\
	}																\
	inline friend CN operator-(CN const& a, T const& b)		   		\
	{																\
		return{ a.f1 - b, a.f2 - b };								\
	}																\
	inline friend CN operator-(CN const& a, CN const& b)	   		\
	{																\
		return{ a.f1 - b.f1, a.f2 - b.f2 };							\
	}																\
	inline bool operator==(CN const& b)								\
	{																\
		return f1 == b.f1 && f2 == b.f2;							\
	}																\
	inline bool operator!=(CN const& b)								\
	{																\
		return f1 != b.f1 || f2 != b.f2;							\
	}																\
	inline void Add(CN const &other)								\
	{																\
		f1 += other.f1;												\
		f2 += other.f2;												\
	}																\
	inline void Minus(CN const &other)								\
	{																\
		f1 -= other.f1;												\
		f2 -= other.f2;												\
	}																\
	inline void Multiply(T const& ratio)							\
	{																\
		f1 *= ratio;												\
		f2 *= ratio;												\
	}																\
	inline void Devide(T const& ratio)								\
	{																\
		f1 /= ratio;												\
		f2 /= ratio;												\
	}																\
	inline bool Intersect(T const& n)								\
	{																\
		assert(f1 <= f2);											\
		return n >= f1 && n <= f2;									\
	}																\
	inline bool IntersectPow2(T const& n)							\
	{																\
		assert(f1 <= f2);											\
		return n >= f1 * f1 && n <= f2 * f2;						\
	}																\
	inline bool IntersectOpen(T const& n)							\
	{																\
		assert(f1 <= f2);											\
		return n >= f1 && n < f2;									\
	}


	template<typename T = double>
	struct Size
	{
		T w, h;
		TWO_SAME_TYPE_FIELD_CLASS_ATTACHCODE(T, Size, w, h);
	};


	template<typename T = double>
	struct Position
	{
		T x, y;
		TWO_SAME_TYPE_FIELD_CLASS_ATTACHCODE(T, Position, x, y);
	};

	template<typename T = double>
	using XY = Position<T>;

	template<typename T = double>
	struct XZ
	{
		T x, z;
		TWO_SAME_TYPE_FIELD_CLASS_ATTACHCODE(T, Position, x, z);
	};

	template<typename T = double>
	struct Range
	{
		T f, t;
		TWO_SAME_TYPE_FIELD_CLASS_ATTACHCODE(T, Range, f, t);
	};

	template<typename T = double>
	struct Rect
	{
		T x, y, w, h;
	};

	union Color
	{
		struct
		{
			uint8_t b;
			uint8_t g;
			uint8_t r;
			uint8_t a;
		};
		uint32_t data;
		Color() : data(0) {}
		Color(uint8_t r, uint8_t g, uint8_t b, uint8_t a)
			: b(b)
			, g(g)
			, r(a)
			, a(a)
		{}
	};
}
