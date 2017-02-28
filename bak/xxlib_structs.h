#pragma once
#include <array>
#include <cstdint>

namespace xxlib
{

#define TWO_SAME_TYPE_FIELD_CLASS_ATTACHCODE( T, CN, f1, f2 )		\
	inline void SetZero()											\
	{																\
		f1 = f2 = 0;												\
	}																\
																	\
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
																	\
	inline bool operator==(CN const& b)								\
	{																\
		return f1 == b.f1 && f2 == b.f2;							\
	}																\
																	\
	inline void Add(CN const &other)								\
	{																\
		f1 += other.f1;												\
		f2 += other.f2;												\
	}																\
	inline void Multiply(T ratio)									\
	{																\
		f1 *= ratio;												\
		f2 *= ratio;												\
	}


#define TWO_SAME_TYPE_FIELD_CLASS_ATTACHCODE_BB_STR( T, CN, f1, f2 ) \
	template<typename T> inline uint32_t BBCalc(CN<T> const &in) { return BBCalc(T()) << 1; }	\
	template<typename T> inline uint32_t BBWriteTo(char *dstBuf, CN<T> const &in) { return BBWriteTo(dstBuf, in.f1, in.f2); }	\
	template<typename T> inline int BBReadFrom(char const *srcBuf, uint32_t const &dataLen, uint32_t &offset, CN<T> &out) { return BBReadFrom(srcBuf, dataLen, offset, out.f1, out.f2); }	\
	template<typename T> inline uint32_t StrCalc(CN<T> const &in) { return (StrCalc(T()) << 1) + 12 + (uint32_t)strlen(#f1#f2); }	\
	template<typename T> inline uint32_t StrWriteTo(char *dstBuf, CN<T> const &in) { return StrWriteTo(dstBuf, "{ "#f1" = ", in.f1, ", "#f2" = ", in.f2, " }"); }


	template<typename T = double>
	struct Size
	{
		T w, h;
		TWO_SAME_TYPE_FIELD_CLASS_ATTACHCODE(T, Size, w, h);
	};
	TWO_SAME_TYPE_FIELD_CLASS_ATTACHCODE_BB_STR(T, Size, w, h);


	template<typename T = double>
	struct Position
	{
		T x, y;
		TWO_SAME_TYPE_FIELD_CLASS_ATTACHCODE(T, Position, x, y);
	};
	TWO_SAME_TYPE_FIELD_CLASS_ATTACHCODE_BB_STR(T, Position, x, y);


	template<typename T = double>
	struct Range
	{
		T f, t;
		TWO_SAME_TYPE_FIELD_CLASS_ATTACHCODE(T, Range, f, t);
	};
	TWO_SAME_TYPE_FIELD_CLASS_ATTACHCODE_BB_STR(T, Range, f, t);


	template<typename T = double>
	struct Rect
	{
		T x, y, w, h;
	};
	template<typename T> inline uint32_t BBCalc(Rect<T> const &in) { return BBCalc(T()) << 2; }
	template<typename T> inline uint32_t BBWriteTo(char *dstBuf, Rect<T> const &in) { return BBWriteTo(dstBuf, in.x, in.y, in.w, in.h); }
	template<typename T> inline int BBReadFrom(char const *srcBuf, uint32_t const &dataLen, uint32_t &offset, Rect<T> &out) { return BBReadFrom(srcBuf, dataLen, offset, out.x, out.y, out.w, out.h); }
	template<typename T> inline uint32_t StrCalc(Rect<T> const &in) { return (StrCalc(T()) << 4) + 26; }
	template<typename T> inline uint32_t StrWriteTo(char *dstBuf, Rect<T> const &in) { return StrWriteTo(dstBuf, "{ x = ", in.x, ", y = ", in.y, ", w = ", in.w, ", h = ", in.h, " }"); }


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
