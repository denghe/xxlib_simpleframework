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
