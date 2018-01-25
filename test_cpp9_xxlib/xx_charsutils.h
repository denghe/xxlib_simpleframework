#pragma once
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

	size_t u32toa_branchlut(uint32_t value, char* buffer) noexcept;
	size_t i32toa_branchlut(int32_t value, char* buffer) noexcept;
	size_t u64toa_branchlut(uint64_t value, char* buffer) noexcept;
	size_t i64toa_branchlut(int64_t value, char* buffer) noexcept;


	/**************************************************************************************************/
	// c-style char* 转为各种数值
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



	// todo: 重构下面的代码, 放弃对 char* 直接填充的支持, 改成类似 BBuffer 适配的长相. 适配函数移到 .hpp

	/**************************************************************************************************/
	// 类型--操作适配模板区
	/**************************************************************************************************/

	// 基础适配模板
	template<typename T, typename ENABLE = void>
	struct StrFunc
	{
		static size_t Calc(T const &in)
		{
			assert(false);
			return 0;
		}
		static size_t WriteTo(char *dstBuf, T const &in)
		{
			assert(false);
			return 0;
		}
	};

	// 匹配无符号整数 32bit
	template<typename T>
	struct StrFunc<T, std::enable_if_t<std::is_integral<T>::value && std::is_unsigned<T>::value && sizeof(T) <= 4>>
	{
		static inline size_t Calc(T const &in)
		{
			return sizeof(T) * 4;
		}
		static inline size_t WriteTo(char *dstBuf, T const &in)
		{
			return u32toa_branchlut(in, dstBuf);
		}
	};

	// 匹配有符号整数 32bit
	template<typename T>
	struct StrFunc<T, std::enable_if_t<std::is_integral<T>::value && !std::is_unsigned<T>::value && sizeof(T) <= 4>>
	{
		static inline size_t Calc(T const &in)
		{
			return sizeof(T) * 4;
		}
		static inline size_t WriteTo(char *dstBuf, T const &in)
		{
			return i32toa_branchlut(in, dstBuf);
		}
	};

	// 匹配无符号整数 64bit
	template<typename T>
	struct StrFunc<T, std::enable_if_t<std::is_integral<T>::value && std::is_unsigned<T>::value && sizeof(T) == 8>>
	{
		static inline size_t Calc(T const &in)
		{
			return sizeof(T) * 4;
		}
		static inline size_t WriteTo(char *dstBuf, T const &in)
		{
			return u64toa_branchlut(in, dstBuf);
		}
	};

	// 匹配有符号整数 64bit
	template<typename T>
	struct StrFunc<T, std::enable_if_t<std::is_integral<T>::value && !std::is_unsigned<T>::value && sizeof(T) == 8>>
	{
		static inline size_t Calc(T const &in)
		{
			return sizeof(T) * 4;
		}
		static inline size_t WriteTo(char *dstBuf, T const &in)
		{
			return i64toa_branchlut(in, dstBuf);
		}
	};

	// 匹配枚举
	template<typename T>
	struct StrFunc<T, std::enable_if_t<std::is_enum<T>::value>>
	{
		static inline size_t Calc(T const &in)
		{
			return StrFunc<std::underlying_type_t<T>>::Calc((std::underlying_type_t<T> const&)in);
		}
		static inline size_t WriteTo(char *dstBuf, T const &in)
		{
			return StrFunc<std::underlying_type_t<T>>::WriteTo(dstBuf, (std::underlying_type_t<T> const&)in);
		}
	};

	// 匹配浮点
	template<typename T>
	struct StrFunc<T, std::enable_if_t<std::is_floating_point<T>::value>>
	{
		static inline size_t Calc(T const &in)
		{
			return sizeof(T) * 3;
		}
		static inline size_t WriteTo(char *dstBuf, T const &in)
		{
			return sprintf(dstBuf, "%g", in);
		}
	};

	template<>
	struct StrFunc<bool, void>
	{
		static inline size_t Calc(bool const &in)
		{
			return 5;
		}
		static inline size_t WriteTo(char *dstBuf, bool const &in)
		{
			if (in) { memcpy(dstBuf, "true", 4); return 4; }
			else { memcpy(dstBuf, "false", 5); return 5; }
		}
	};

	template<>
	struct StrFunc<char, void>
	{
		static inline size_t Calc(char const &in)
		{
			return 1;
		}
		static inline size_t WriteTo(char *dstBuf, char const &in)
		{
			*dstBuf = in;
			return 1;
		}
	};

	template<>
	struct StrFunc<char const*, void>
	{
		static inline size_t Calc(char const* const &in)
		{
			if (!in) return 0; return strlen(in);
		}
		static inline size_t WriteTo(char *dstBuf, char const* const &in)
		{
			if (!in) return 0;
			else
			{
				auto len = strlen(in);
				memcpy(dstBuf, in, len);
				return len;
			};
		}
	};

	// 定长数组支持
	template<size_t len>
	struct StrFunc<char [len], void>
	{
		static inline size_t Calc(char const(&in)[len])
		{
			return len - 1;
		}
		static inline size_t WriteTo(char *dstBuf, char const(&in)[len])
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
		static inline size_t Calc(AT const& in)
		{
			size_t rtv = 0;
			for (auto& o : in) rtv += StrFunc<T>::Calc(o);
			return rtv;
		}
		static inline size_t WriteTo(char *dstBuf, AT const& in)
		{
			size_t offset = 0;
			offset += StrFunc<T>::WriteTo(dstBuf + offset, "{ ");
			for (auto& o : in) offset += StrFunc<T>::WriteTo(dstBuf + offset, o, ", ");
			offset += StrFunc<T>::WriteTo(dstBuf + offset - 2, " }");
			return offset;
		}
	};



	// todo: more

	/**************************************************************************************************/
	// StrFunc 映射到函数
	/**************************************************************************************************/

	template<typename T>
	size_t StrCalc(T const &in)
	{
		return StrFunc<T>::Calc(in);
	}

	template<typename T>
	size_t StrWriteTo(char *dstBuf, T const &in)
	{
		return StrFunc<T>::WriteTo(dstBuf, in);
	}

	/**************************************************************************************************/
	// 变参模板支持
	/**************************************************************************************************/

	template<typename T, typename...TS>
	size_t StrCalc(T const &in, TS const& ... ins)
	{
		return StrCalc(in) + StrCalc(ins...);
	}
	template<typename T, typename...TS>
	size_t StrWriteTo(char *dstBuf, T const &in, TS const& ... ins)
	{
		auto offset = StrWriteTo(dstBuf, in);
		return offset + StrWriteTo(dstBuf + offset, ins...);
	}

}
