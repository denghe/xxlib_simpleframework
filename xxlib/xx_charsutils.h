#pragma once
namespace xx
{

	/**************************************************************************************************/
	// 整数转 string 用到的一些基础函数
	/**************************************************************************************************/

	// 下列代码抄自 https://github.com/miloyip/itoa-benchmark/blob/master/src/branchlut.cpp
	// 经改造, 增加了返回填充长度的功能


	// Branching for different cases (forward)
	// Use lookup table of two digits

	size_t u32toa_branchlut(uint32_t value, char* buffer) noexcept;
	size_t i32toa_branchlut(int32_t value, char* buffer) noexcept;
	size_t u64toa_branchlut(uint64_t value, char* buffer) noexcept;
	size_t i64toa_branchlut(int64_t value, char* buffer) noexcept;






	/**************************************************************************************************/
	// c-style char* 转为各种数值( 典型的应用场景: sql 的查询结果到 数值 的转换 )
	/**************************************************************************************************/

	// c-style char* 转为各种长度的 有符号整数. Out 取值范围： int8~64
	template <typename OutType>
	void ToInt(char const * in, OutType & out) noexcept;

	// c-style char* (不能有减号打头) 转为各种长度的 无符号整数. Out 取值范围： uint8, uint16, uint32, uint64
	template <typename OutType>
	void ToUInt(char const * in, OutType & out) noexcept;

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



	/**************************************************************************************************/
	// 从 sqlite 源代码中复制来的 wchar -> utf8 ( 原型是 WRITE_UTF8 宏 ), 返回写入长度
	/**************************************************************************************************/

	template <typename WCharType>
	size_t ToUtf8(char* const& zOut, WCharType const& c) noexcept
	{

		if (c < 0x00080)
		{
			zOut[0] = (uint8_t)(c & 0xFF);
			return 1;
		}
		else if (c < 0x00800)
		{
			zOut[0] = 0xC0 + (uint8_t)((c >> 6) & 0x1F);
			zOut[1] = 0x80 + (uint8_t)(c & 0x3F);
			return 2;
		}
		else if (c < 0x10000)
		{
			zOut[0] = 0xE0 + (uint8_t)((c >> 12) & 0x0F);
			zOut[1] = 0x80 + (uint8_t)((c >> 6) & 0x3F);
			zOut[2] = 0x80 + (uint8_t)(c & 0x3F);
			return 3;
		}
		else
		{
			if constexpr(sizeof(WCharType) >= 4)
			{
				zOut[0] = 0xF0 + (uint8_t)((c >> 18) & 0x07);
				zOut[1] = 0x80 + (uint8_t)((c >> 12) & 0x3F);
				zOut[2] = 0x80 + (uint8_t)((c >> 6) & 0x3F);
				zOut[3] = 0x80 + (uint8_t)(c & 0x3F);
				return 4;
			}
		}
	}



	/**************************************************************************************************/
	// 类型--操作适配模板区
	/**************************************************************************************************/

	// 基础适配模板
	template<typename T, typename ENABLE = void>
	struct StrFunc
	{
		static void WriteTo(String& s, T const &in)
		{
			assert(false);
		}
	};

}
