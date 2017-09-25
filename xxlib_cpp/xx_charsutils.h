#pragma once
#pragma warning(disable:4996)
#include <cstdint>
#include <cstring>
#include <cstdio>
#include <cassert>
#include <cmath>
#include <cstdlib>
#include <type_traits>
#include <array>

namespace xx
{
	/**************************************************************************************************/
	// 整数转 string 用到的一些基础函数
	/**************************************************************************************************/

	// 下列代码抄自 https://github.com/miloyip/itoa-benchmark/blob/master/src/branchlut.cpp
	// 经改造, 增加了返回填充长度的功能

	constexpr char gDigitsLut[200] = {
		'0', '0', '0', '1', '0', '2', '0', '3', '0', '4', '0', '5', '0', '6', '0', '7', '0', '8', '0', '9',
		'1', '0', '1', '1', '1', '2', '1', '3', '1', '4', '1', '5', '1', '6', '1', '7', '1', '8', '1', '9',
		'2', '0', '2', '1', '2', '2', '2', '3', '2', '4', '2', '5', '2', '6', '2', '7', '2', '8', '2', '9',
		'3', '0', '3', '1', '3', '2', '3', '3', '3', '4', '3', '5', '3', '6', '3', '7', '3', '8', '3', '9',
		'4', '0', '4', '1', '4', '2', '4', '3', '4', '4', '4', '5', '4', '6', '4', '7', '4', '8', '4', '9',
		'5', '0', '5', '1', '5', '2', '5', '3', '5', '4', '5', '5', '5', '6', '5', '7', '5', '8', '5', '9',
		'6', '0', '6', '1', '6', '2', '6', '3', '6', '4', '6', '5', '6', '6', '6', '7', '6', '8', '6', '9',
		'7', '0', '7', '1', '7', '2', '7', '3', '7', '4', '7', '5', '7', '6', '7', '7', '7', '8', '7', '9',
		'8', '0', '8', '1', '8', '2', '8', '3', '8', '4', '8', '5', '8', '6', '8', '7', '8', '8', '8', '9',
		'9', '0', '9', '1', '9', '2', '9', '3', '9', '4', '9', '5', '9', '6', '9', '7', '9', '8', '9', '9'
	};

	// Branching for different cases (forward)
	// Use lookup table of two digits

	inline uint32_t u32toa_branchlut(uint32_t value, char* buffer) {
		auto bak = buffer;											// 为计算长度而加
		if (value < 10000) {
			const uint32_t d1 = (value / 100) << 1;
			const uint32_t d2 = (value % 100) << 1;

			if (value >= 1000)
				*buffer++ = gDigitsLut[d1];
			if (value >= 100)
				*buffer++ = gDigitsLut[d1 + 1];
			if (value >= 10)
				*buffer++ = gDigitsLut[d2];
			*buffer++ = gDigitsLut[d2 + 1];
		}
		else if (value < 100000000) {
			// value = bbbbcccc
			const uint32_t b = value / 10000;
			const uint32_t c = value % 10000;

			const uint32_t d1 = (b / 100) << 1;
			const uint32_t d2 = (b % 100) << 1;

			const uint32_t d3 = (c / 100) << 1;
			const uint32_t d4 = (c % 100) << 1;

			if (value >= 10000000)
				*buffer++ = gDigitsLut[d1];
			if (value >= 1000000)
				*buffer++ = gDigitsLut[d1 + 1];
			if (value >= 100000)
				*buffer++ = gDigitsLut[d2];
			*buffer++ = gDigitsLut[d2 + 1];

			*buffer++ = gDigitsLut[d3];
			*buffer++ = gDigitsLut[d3 + 1];
			*buffer++ = gDigitsLut[d4];
			*buffer++ = gDigitsLut[d4 + 1];
		}
		else {
			// value = aabbbbcccc in decimal

			const uint32_t a = value / 100000000; // 1 to 42
			value %= 100000000;

			if (a >= 10) {
				const unsigned i = a << 1;
				*buffer++ = gDigitsLut[i];
				*buffer++ = gDigitsLut[i + 1];
			}
			else
				*buffer++ = '0' + static_cast<char>(a);

			const uint32_t b = value / 10000; // 0 to 9999
			const uint32_t c = value % 10000; // 0 to 9999

			const uint32_t d1 = (b / 100) << 1;
			const uint32_t d2 = (b % 100) << 1;

			const uint32_t d3 = (c / 100) << 1;
			const uint32_t d4 = (c % 100) << 1;

			*buffer++ = gDigitsLut[d1];
			*buffer++ = gDigitsLut[d1 + 1];
			*buffer++ = gDigitsLut[d2];
			*buffer++ = gDigitsLut[d2 + 1];
			*buffer++ = gDigitsLut[d3];
			*buffer++ = gDigitsLut[d3 + 1];
			*buffer++ = gDigitsLut[d4];
			*buffer++ = gDigitsLut[d4 + 1];
		}
		//*buffer++ = '\0';
		return uint32_t(buffer - bak);							// 返回填充了多长
	}

	inline uint32_t i32toa_branchlut(int32_t value, char* buffer) {
		uint32_t u = static_cast<uint32_t>(value);
		if (value < 0) {
			*buffer++ = '-';
			u = ~u + 1;
			return u32toa_branchlut(u, buffer) + 1;				// 返回填充了多长
		}
		return u32toa_branchlut(u, buffer);						// 返回填充了多长
	}

	inline uint32_t u64toa_branchlut(uint64_t value, char* buffer) {
		auto bak = buffer;										// 为计算长度而加
		if (value < 100000000) {
			uint32_t v = static_cast<uint32_t>(value);
			if (v < 10000) {
				const uint32_t d1 = (v / 100) << 1;
				const uint32_t d2 = (v % 100) << 1;

				if (v >= 1000)
					*buffer++ = gDigitsLut[d1];
				if (v >= 100)
					*buffer++ = gDigitsLut[d1 + 1];
				if (v >= 10)
					*buffer++ = gDigitsLut[d2];
				*buffer++ = gDigitsLut[d2 + 1];
			}
			else {
				// value = bbbbcccc
				const uint32_t b = v / 10000;
				const uint32_t c = v % 10000;

				const uint32_t d1 = (b / 100) << 1;
				const uint32_t d2 = (b % 100) << 1;

				const uint32_t d3 = (c / 100) << 1;
				const uint32_t d4 = (c % 100) << 1;

				if (value >= 10000000)
					*buffer++ = gDigitsLut[d1];
				if (value >= 1000000)
					*buffer++ = gDigitsLut[d1 + 1];
				if (value >= 100000)
					*buffer++ = gDigitsLut[d2];
				*buffer++ = gDigitsLut[d2 + 1];

				*buffer++ = gDigitsLut[d3];
				*buffer++ = gDigitsLut[d3 + 1];
				*buffer++ = gDigitsLut[d4];
				*buffer++ = gDigitsLut[d4 + 1];
			}
		}
		else if (value < 10000000000000000) {
			const uint32_t v0 = static_cast<uint32_t>(value / 100000000);
			const uint32_t v1 = static_cast<uint32_t>(value % 100000000);

			const uint32_t b0 = v0 / 10000;
			const uint32_t c0 = v0 % 10000;

			const uint32_t d1 = (b0 / 100) << 1;
			const uint32_t d2 = (b0 % 100) << 1;

			const uint32_t d3 = (c0 / 100) << 1;
			const uint32_t d4 = (c0 % 100) << 1;

			const uint32_t b1 = v1 / 10000;
			const uint32_t c1 = v1 % 10000;

			const uint32_t d5 = (b1 / 100) << 1;
			const uint32_t d6 = (b1 % 100) << 1;

			const uint32_t d7 = (c1 / 100) << 1;
			const uint32_t d8 = (c1 % 100) << 1;

			if (value >= 1000000000000000)
				*buffer++ = gDigitsLut[d1];
			if (value >= 100000000000000)
				*buffer++ = gDigitsLut[d1 + 1];
			if (value >= 10000000000000)
				*buffer++ = gDigitsLut[d2];
			if (value >= 1000000000000)
				*buffer++ = gDigitsLut[d2 + 1];
			if (value >= 100000000000)
				*buffer++ = gDigitsLut[d3];
			if (value >= 10000000000)
				*buffer++ = gDigitsLut[d3 + 1];
			if (value >= 1000000000)
				*buffer++ = gDigitsLut[d4];
			if (value >= 100000000)
				*buffer++ = gDigitsLut[d4 + 1];

			*buffer++ = gDigitsLut[d5];
			*buffer++ = gDigitsLut[d5 + 1];
			*buffer++ = gDigitsLut[d6];
			*buffer++ = gDigitsLut[d6 + 1];
			*buffer++ = gDigitsLut[d7];
			*buffer++ = gDigitsLut[d7 + 1];
			*buffer++ = gDigitsLut[d8];
			*buffer++ = gDigitsLut[d8 + 1];
		}
		else {
			const uint32_t a = static_cast<uint32_t>(value / 10000000000000000); // 1 to 1844
			value %= 10000000000000000;

			if (a < 10)
				*buffer++ = '0' + static_cast<char>(a);
			else if (a < 100) {
				const uint32_t i = a << 1;
				*buffer++ = gDigitsLut[i];
				*buffer++ = gDigitsLut[i + 1];
			}
			else if (a < 1000) {
				*buffer++ = '0' + static_cast<char>(a / 100);

				const uint32_t i = (a % 100) << 1;
				*buffer++ = gDigitsLut[i];
				*buffer++ = gDigitsLut[i + 1];
			}
			else {
				const uint32_t i = (a / 100) << 1;
				const uint32_t j = (a % 100) << 1;
				*buffer++ = gDigitsLut[i];
				*buffer++ = gDigitsLut[i + 1];
				*buffer++ = gDigitsLut[j];
				*buffer++ = gDigitsLut[j + 1];
			}

			const uint32_t v0 = static_cast<uint32_t>(value / 100000000);
			const uint32_t v1 = static_cast<uint32_t>(value % 100000000);

			const uint32_t b0 = v0 / 10000;
			const uint32_t c0 = v0 % 10000;

			const uint32_t d1 = (b0 / 100) << 1;
			const uint32_t d2 = (b0 % 100) << 1;

			const uint32_t d3 = (c0 / 100) << 1;
			const uint32_t d4 = (c0 % 100) << 1;

			const uint32_t b1 = v1 / 10000;
			const uint32_t c1 = v1 % 10000;

			const uint32_t d5 = (b1 / 100) << 1;
			const uint32_t d6 = (b1 % 100) << 1;

			const uint32_t d7 = (c1 / 100) << 1;
			const uint32_t d8 = (c1 % 100) << 1;

			*buffer++ = gDigitsLut[d1];
			*buffer++ = gDigitsLut[d1 + 1];
			*buffer++ = gDigitsLut[d2];
			*buffer++ = gDigitsLut[d2 + 1];
			*buffer++ = gDigitsLut[d3];
			*buffer++ = gDigitsLut[d3 + 1];
			*buffer++ = gDigitsLut[d4];
			*buffer++ = gDigitsLut[d4 + 1];
			*buffer++ = gDigitsLut[d5];
			*buffer++ = gDigitsLut[d5 + 1];
			*buffer++ = gDigitsLut[d6];
			*buffer++ = gDigitsLut[d6 + 1];
			*buffer++ = gDigitsLut[d7];
			*buffer++ = gDigitsLut[d7 + 1];
			*buffer++ = gDigitsLut[d8];
			*buffer++ = gDigitsLut[d8 + 1];
		}

		//*buffer = '\0';
		return uint32_t(buffer - bak);							// 返回填充了多长
	}

	inline uint32_t i64toa_branchlut(int64_t value, char* buffer) {
		uint64_t u = static_cast<uint64_t>(value);
		if (value < 0) {
			*buffer++ = '-';
			u = ~u + 1;
			return u64toa_branchlut(u, buffer) + 1;				// 返回填充了多长
		}
		return u64toa_branchlut(u, buffer);						// 返回填充了多长
	}


	/**************************************************************************************************/
	// 类型--操作适配模板区
	/**************************************************************************************************/

	// 基础适配模板
	template<typename T, typename ENABLE = void>
	struct StrFunc
	{
		static uint32_t Calc(T const &in)
		{
			assert(false);
			return 0;
		}
		static uint32_t WriteTo(char *dstBuf, T const &in)
		{
			assert(false);
			return 0;
		}
	};

	// 匹配无符号整数 32bit
	template<typename T>
	struct StrFunc<T, std::enable_if_t<std::is_integral<T>::value && std::is_unsigned<T>::value && sizeof(T) <= 4>>
	{
		static inline uint32_t Calc(T const &in)
		{
			return sizeof(T) * 4;
		}
		static inline uint32_t WriteTo(char *dstBuf, T const &in)
		{
			return u32toa_branchlut(in, dstBuf);
		}
	};

	// 匹配有符号整数 32bit
	template<typename T>
	struct StrFunc<T, std::enable_if_t<std::is_integral<T>::value && !std::is_unsigned<T>::value && sizeof(T) <= 4>>
	{
		static inline uint32_t Calc(T const &in)
		{
			return sizeof(T) * 4;
		}
		static inline uint32_t WriteTo(char *dstBuf, T const &in)
		{
			return i32toa_branchlut(in, dstBuf);
		}
	};

	// 匹配无符号整数 64bit
	template<typename T>
	struct StrFunc<T, std::enable_if_t<std::is_integral<T>::value && std::is_unsigned<T>::value && sizeof(T) == 8>>
	{
		static inline uint32_t Calc(T const &in)
		{
			return sizeof(T) * 4;
		}
		static inline uint32_t WriteTo(char *dstBuf, T const &in)
		{
			return u64toa_branchlut(in, dstBuf);
		}
	};

	// 匹配有符号整数 64bit
	template<typename T>
	struct StrFunc<T, std::enable_if_t<std::is_integral<T>::value && !std::is_unsigned<T>::value && sizeof(T) == 8>>
	{
		static inline uint32_t Calc(T const &in)
		{
			return sizeof(T) * 4;
		}
		static inline uint32_t WriteTo(char *dstBuf, T const &in)
		{
			return i64toa_branchlut(in, dstBuf);
		}
	};

	// 匹配枚举
	template<typename T>
	struct StrFunc<T, std::enable_if_t<std::is_enum<T>::value>>
	{
		static inline uint32_t Calc(T const &in)
		{
			return StrFunc<std::underlying_type_t<T>>::Calc((std::underlying_type_t<T> const&)in);
		}
		static inline uint32_t WriteTo(char *dstBuf, T const &in)
		{
			return StrFunc<std::underlying_type_t<T>>::WriteTo(dstBuf, (std::underlying_type_t<T> const&)in);
		}
	};

	// 匹配浮点
	template<typename T>
	struct StrFunc<T, std::enable_if_t<std::is_floating_point<T>::value>>
	{
		static inline uint32_t Calc(T const &in)
		{
			return sizeof(T) * 3;
		}
		static inline uint32_t WriteTo(char *dstBuf, T const &in)
		{
			return sprintf(dstBuf, "%g", in);
		}
	};

	template<>
	struct StrFunc<bool, void>
	{
		static inline uint32_t Calc(bool const &in)
		{
			return 5;
		}
		static inline uint32_t WriteTo(char *dstBuf, bool const &in)
		{
			if (in) { memcpy(dstBuf, "true", 4); return 4; }
			else { memcpy(dstBuf, "false", 5); return 5; }
		}
	};

	template<>
	struct StrFunc<char, void>
	{
		static inline uint32_t Calc(char const &in)
		{
			return 1;
		}
		static inline uint32_t WriteTo(char *dstBuf, char const &in)
		{
			*dstBuf = in;
			return 1;
		}
	};

	template<>
	struct StrFunc<char const*, void>
	{
		static inline uint32_t Calc(char const* const &in)
		{
			if (!in) return 0; return (uint32_t)strlen(in);
		}
		static inline uint32_t WriteTo(char *dstBuf, char const* const &in)
		{
			if (!in) return 0;
			else
			{
				auto len = (uint32_t)strlen(in);
				memcpy(dstBuf, in, len);
				return len;
			};
		}
	};

	// 定长数组支持
	template<uint32_t len>
	struct StrFunc<char [len], void>
	{
		static inline uint32_t Calc(char const(&in)[len])
		{
			return len - 1;
		}
		static inline uint32_t WriteTo(char *dstBuf, char const(&in)[len])
		{
			memcpy(dstBuf, in, len - 1);
			return len - 1;
		}
	};

	// std::array 支持
	template<typename T, size_t len>
	struct StrFunc<std::array<T, len>, void>
	{
		typedef std::array<T, len> AT;
		static inline uint32_t Calc(AT const& in)
		{
			uint32_t rtv = 0;
			for (auto& o : in) rtv += StrFunc<T>::Calc(o);
			return rtv;
		}
		static inline uint32_t WriteTo(char *dstBuf, AT const& in)
		{
			uint32_t offset = 0;
			offset += StrFunc<T>::WriteTo(dstBuf + offset, "{ ");
			for (auto& o : in) offset += StrFunc<T>::WriteTo(dstBuf + offset, o, ", ");
			offset += StrFunc<T>::WriteTo(dstBuf + offset - 2, " }");
			return offset;
		}
	};



	// todo: more

	/**************************************************************************************************/
	// 将 StrFunc 映射为全局函数以方便不借助 String 类来填充 buf
	/**************************************************************************************************/

	template<typename T>
	uint32_t StrCalc(T const &in)
	{
		return StrFunc<T>::Calc(in);
	}

	template<typename T>
	uint32_t StrWriteTo(char *dstBuf, T const &in)
	{
		return StrFunc<T>::WriteTo(dstBuf, in);
	}

	/**************************************************************************************************/
	// 变参模板支持
	/**************************************************************************************************/

	template<typename T, typename...TS>
	uint32_t StrCalc(T const &in, TS const& ... ins)
	{
		return StrCalc(in) + StrCalc(ins...);
	}
	template<typename T, typename...TS>
	uint32_t StrWriteTo(char *dstBuf, T const &in, TS const& ... ins)
	{
		uint32_t offset = StrWriteTo(dstBuf, in);
		return offset + StrWriteTo(dstBuf + offset, ins...);
	}






	/**************************************************************************************************/
	// c-style char* 转为各种数值
	/**************************************************************************************************/

	// c-style char* 转为各种长度的 有符号整数. Out 取值范围： int8~64
	template <typename OutType>
	void ToInt(char const * in, OutType & out)
	{
		auto in_ = in;
		if (*in_ == '0')
		{
			out = 0;
			return;
		}
		bool b;
		if (*in_ == '-')
		{
			b = true;
			++in_;
		}
		else b = false;
		OutType r = *in_ - '0';
		char c;
		while ((c = *(++in_))) r = r * 10 + (c - '0');
		out = b ? -r : r;
	}

	// c-style char* (不能有减号打头) 转为各种长度的 无符号整数. Out 取值范围： uint8, uint16, uint32, uint64
	template <typename OutType>
	void ToUInt(char const * in, OutType & out)
	{
		assert(in);
		auto in_ = in;
		if (*in_ == '0')
		{
			out = 0;
			return;
		}
		OutType r = *(in_)-'0';
		char c;
		while ((c = *(++in_))) r = r * 10 + (c - '0');
		out = r;
	}

	inline void FromString(bool     &out, char const *in) { out = (in[0] == '1' || in[0] == 'T' || in[0] == 't'); }
	inline void FromString(uint8_t  &out, char const *in) { ToUInt(in, out); }
	inline void FromString(uint16_t &out, char const *in) { ToUInt(in, out); }
	inline void FromString(uint32_t &out, char const *in) { ToUInt(in, out); }
	inline void FromString(uint64_t &out, char const *in) { ToUInt(in, out); }
	inline void FromString(int8_t   &out, char const *in) { ToInt(in, out); }
	inline void FromString(int16_t  &out, char const *in) { ToInt(in, out); }
	inline void FromString(int32_t  &out, char const *in) { ToInt(in, out); }
	inline void FromString(int64_t  &out, char const *in) { ToInt(in, out); }
	inline void FromString(double   &out, char const *in) { out = strtod(in, nullptr); }
	inline void FromString(float    &out, char const *in) { out = (float)strtod(in, nullptr); }




//    /**************************************************************************************************/
//    // Utf8 转为 char16_t* 的类
//    /**************************************************************************************************/
//
//    template<size_t bufCount>
//    struct Utf8Converter
//    {
//        std::array<std::u16string, bufCount> bufs;    // 如果被用于多参数环境时避免重复使用到一个 buf
//        uint32_t idx = 0;
//
//        char16_t const* Convert(char const * utf8Str)
//        {
//            assert(utf8Str);
//            auto& buf = bufs[idx++];
//            if (idx == bufs.size()) idx = 0;
//
//            auto len = strlen(utf8Str);
//            buf.resize(strlen(utf8Str));
//
//            uint32_t i = 0, j = 0;
//            while (uint8_t c = utf8Str[i++])
//            {
//                if (c < 0x80u)
//                {
//                    buf[j++] = c;
//                }
//                else if ((c & 0xC0u) == 0xC0u)
//                {
//                    buf[j++] = ((c & 0x1F) << 6) | (utf8Str[i++] & 0x3F);
//                }
//                else if ((c & 0xE0) == 0xE0)
//                {
//                    buf[j++] = ((c & 0x0F) << 12) | (((utf8Str[i]) & 0x3F) << 6) | ((utf8Str[i + 1]) & 0x3F);
//                    i += 2;
//                }
//            };
//            buf.resize(j);
//            return buf.c_str();
//        }
//    };

}
