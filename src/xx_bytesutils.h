#pragma once
#include <cstdint>
#include <type_traits>
#include <limits>
#include <cmath>
#include <cstring>
#include <array>

namespace xx
{
	/**************************************************************************************************/
	// 一些处理变长读写的基础函数
	/**************************************************************************************************/

	// negative -> ZigZag positive.  效果：负数变正，正数 *= 2

	inline uint16_t ZigZagEncode(int16_t in)
	{
		return (in << 1) ^ (in >> 15);
	}
	inline uint32_t ZigZagEncode(int32_t in)
	{
		return (in << 1) ^ (in >> 31);
	}
	inline uint64_t ZigZagEncode(int64_t in)
	{
		return (in << 1) ^ (in >> 63);
	}

	// ZigZag positive -> negative
	inline int16_t ZigZagDecode(uint16_t in)
	{
		return (int16_t)(in >> 1) ^ (-(int16_t)(in & 1));
	}
	inline int32_t ZigZagDecode(uint32_t in)
	{
		return (int32_t)(in >> 1) ^ (-(int32_t)(in & 1));
	}
	inline int64_t ZigZagDecode(uint64_t in)
	{
		return (int64_t)(in >> 1) ^ (-(int64_t)(in & 1));
	}

	// uint16_t
	inline uint32_t VarWrite7(char *dstBuf, uint16_t in)
	{
		uint32_t len = 0;
	Lab1:
		auto b7 = (unsigned char)in;
		if ((in >>= 7))
		{
			dstBuf[len++] = b7 | 0x80u;
			goto Lab1;
		}
		dstBuf[len++] = b7;
		return len;
	}

	// uint16_t
	inline int VarRead7(char const *srcBuf, uint32_t dataLen, uint32_t& offset, uint16_t &out)
	{
		if (offset >= dataLen) return -1;// NotEnoughData;
		auto p = srcBuf + offset;
		uint32_t i = 0, b7;
		int32_t lshift = 0;
		uint32_t idx = 0;
		dataLen -= offset;
	Lab1:
		b7 = p[idx++];
		if (b7 > 0x7F)
		{
			if (idx == 3) return -2;// Overflow;
			if (idx >= dataLen) return -1;// NotEnoughData;
			i |= (b7 & 0x7F) << lshift;
			lshift += 7;
			goto Lab1;
		}
		else
		{
			if (idx == 3 && b7 > 3) return -2;// Overflow;
			else
			{
				i |= b7 << lshift;
			}
		}
		out = (uint16_t)i;
		offset += idx;
		return 0;// Success;
	}

	// uint32_t
	inline uint32_t VarWrite7(char *dstBuf, uint32_t in)
	{
		uint32_t len = 0;
	Lab1:
		auto b7 = (unsigned char)in;
		if ((in >>= 7))
		{
			dstBuf[len++] = b7 | 0x80u;
			goto Lab1;
		}
		dstBuf[len++] = b7;
		return len;
	}

	// uint32_t
	inline int VarRead7(char const *srcBuf, uint32_t dataLen, uint32_t& offset, uint32_t &out)
	{
		if (offset >= dataLen) return -1;// NotEnoughData;
		auto p = srcBuf + offset;
		uint32_t i = 0, b7;
		int32_t lshift = 0;
		uint32_t idx = 0;
		dataLen -= offset;
	Lab1:
		b7 = p[idx++];
		if (b7 > 0x7F)
		{
			if (idx == 5) return -2;// Overflow;
			if (idx >= dataLen) return -1;// NotEnoughData;
			i |= (b7 & 0x7F) << lshift;
			lshift += 7;
			goto Lab1;
		}
		else
		{
			if (idx == 5 && b7 > 15) return -2;// Overflow;
			else
			{
				i |= b7 << lshift;
			}
		}
		out = i;
		offset += idx;
		return 0;// Success;
	}

	// uint64_t
	inline uint32_t VarWrite7(char *dstBuf, uint64_t in)
	{
		uint32_t len = 0;
	Lab1:
		auto b7 = (unsigned char)in;
		if ((in >>= 7))
		{
			dstBuf[len++] = b7 | 0x80u;
			if (len == 8)
			{
				dstBuf[len++] = (unsigned char)in;
			}
			else goto Lab1;
		}
		else
		{
			dstBuf[len++] = b7;
		}
		return len;
	}

	// uint64_t
	inline int VarRead7(char const *srcBuf, uint32_t dataLen, uint32_t &offset, uint64_t &out)
	{
		if (offset >= dataLen) return -1;// NotEnoughData;
		auto p = srcBuf + offset;
		uint64_t i = 0, b7;
		int32_t lshift = 0;
		uint32_t idx = 0;
		dataLen -= offset;
	Lab1:
		b7 = p[idx++];
		if (b7 > 0x7F)
		{
			if (idx >= dataLen) return -1;// NotEnoughData;
			if (idx < 8)
			{
				i |= (b7 & 0x7F) << lshift;
				lshift += 7;
				goto Lab1;
			}
			else
			{
				i |= ((b7 & 0x7F) << lshift) | ((uint64_t)p[idx++] << 28 << 28);
			}
		}
		else
		{
			i |= b7 << lshift;
		}
		out = i;
		offset += idx;
		return 0;// Success;
	}


	/**************************************************************************************************/
	// 类型--操作适配模板区
	/**************************************************************************************************/

	// 基础适配模板
	template<typename T, typename ENABLE = void>
	struct BytesFunc
	{
		static uint32_t Calc(T const &in);
		static uint32_t WriteTo(char *dstBuf, T const &in);
		static int ReadFrom(char const *srcBuf, uint32_t const &dataLen, uint32_t &offset, T &out);
	};

	// 适配 1 字节长度的 数值 或 float( 这些类型直接 memcpy )
	template<typename T>
	struct BytesFunc<T, std::enable_if_t< (std::is_arithmetic<T>::value && sizeof(T) == 1) || (std::is_floating_point<T>::value && sizeof(T) == 4) >>
	{
		static inline uint32_t Calc(T const &in)
		{
			return sizeof(T);
		}
		static inline uint32_t WriteTo(char *dstBuf, T const &in)
		{
			std::memcpy(dstBuf, &in, sizeof(T));
			return sizeof(T);
		}
		static inline int ReadFrom(char const *srcBuf, uint32_t const &dataLen, uint32_t &offset, T &out)
		{
			if (offset + sizeof(T) > dataLen) return -1;
			std::memcpy(&out, srcBuf + offset, sizeof(T));
			offset += sizeof(T);
			return 0;
		}
	};

	// 适配 2+ 字节无符号整数( 变长读写 )
	template<typename T>
	struct BytesFunc<T, std::enable_if_t<std::is_integral<T>::value && sizeof(T) >= 2 && std::is_unsigned<T>::value>>
	{
		static inline uint32_t Calc(T const &in)
		{
			return sizeof(T) + 1;
		}
		static inline uint32_t WriteTo(char *dstBuf, T const &in)
		{
			return VarWrite7(dstBuf, in);
		}
		static inline int ReadFrom(char const *srcBuf, uint32_t const &dataLen, uint32_t &offset, T &out)
		{
			return VarRead7(srcBuf, dataLen, offset, out);
		}
	};

	// 适配 2+ 字节有符号整数( ZigZag 变长读写 )
	template<typename T>
	struct BytesFunc<T, std::enable_if_t<std::is_integral<T>::value && sizeof(T) >= 2 && !std::is_unsigned<T>::value>>
	{
		static inline uint32_t Calc(T const &in)
		{
			return sizeof(T) + 1;
		}
		static inline uint32_t WriteTo(char *dstBuf, T const &in)
		{
			return VarWrite7(dstBuf, ZigZagEncode(in));
		}
		static inline int ReadFrom(char const *srcBuf, uint32_t const &dataLen, uint32_t &offset, T &out)
		{
			std::make_unsigned_t<T> i = 0;
			auto rtv = VarRead7(srcBuf, dataLen, offset, i);
			out = ZigZagDecode(i);
			return rtv;
		}
	};

	// 适配 enum( 根据原始数据类型调上面的适配 )
	template<typename T>
	struct BytesFunc<T, std::enable_if_t<std::is_enum<T>::value>>
	{
		typedef std::underlying_type_t<T> UT;
		static inline uint32_t Calc(T const &in)
		{
			return BytesFunc<UT>::Calc((UT const&)in);
		}
		static inline uint32_t WriteTo(char *dstBuf, T const &in)
		{
			return BytesFunc<UT>::WriteTo(dstBuf, (UT const&)in);
		}
		static inline int ReadFrom(char const *srcBuf, uint32_t const &dataLen, uint32_t &offset, T &out)
		{
			return BytesFunc<UT>::ReadFrom(srcBuf, dataLen, offset, (UT&)out);
		}
	};

	// 适配 double
	template<>
	struct BytesFunc<double, void>
	{
		static inline uint32_t Calc(double const &in)
		{
			return sizeof(double) + 1;
		}
		static inline uint32_t WriteTo(char *dstBuf, double const &in)
		{
			if (in == 0)
			{
				dstBuf[0] = 0;
				return 1;
			}
			else if (std::isnan(in))
			{
				dstBuf[0] = 1;
				return 1;
			}
			else if (in == -std::numeric_limits<double>::infinity())	// negative infinity
			{
				dstBuf[0] = 2;
				return 1;
			}
			else if (in == std::numeric_limits<double>::infinity())		// positive infinity
			{
				dstBuf[0] = 3;
				return 1;
			}
			else
			{
				auto i = (int32_t)in;
				if (in == (double)i)
				{
					dstBuf[0] = 4;
					return 1 + BytesFunc<int32_t>::WriteTo(dstBuf + 1, i);
				}
				else
				{
					dstBuf[0] = 5;
					memcpy(dstBuf + 1, &in, sizeof(double));
					return sizeof(double) + 1;
				}
			}
		}
		static inline int ReadFrom(char const *srcBuf, uint32_t const &dataLen, uint32_t &offset, double &out)
		{
			if (offset >= dataLen) return -1;	    // 确保还有 1 字节可读
			switch (srcBuf[offset++])				// 跳过 1 字节
			{
			case 0:
				out = 0;
				return 0;
			case 1:
				out = std::numeric_limits<double>::quiet_NaN();
				return 0;
			case 2:
				out = -std::numeric_limits<double>::infinity();
				return 0;
			case 3:
				out = std::numeric_limits<double>::infinity();
				return 0;
			case 4:
			{
				int32_t i = 0;
				auto rtv = BytesFunc<int32_t>::ReadFrom(srcBuf, dataLen, offset, i);
				if (rtv) return rtv;
				out = i;
				return 0;
			}
			case 5:
			{
				if (dataLen < offset + sizeof(double)) return -1;
				memcpy(&out, srcBuf + offset, sizeof(double));
				offset += sizeof(double);
				return 0;
			}
			default:
				return -2;                          // failed
			}
		}
	};

	// 适配 std::array 数组
	template<typename T, size_t len>
	struct BytesFunc<std::array<T, len>, void>
	{
		typedef std::array<T, len> AT;
		static inline uint32_t Calc(AT const &in)
		{
			return BytesFunc<T>::Calc(T()) * len;
		}
		static inline uint32_t WriteTo(char *dstBuf, T const &in)
		{
			if ((std::is_arithmetic<T>::value && sizeof(T) <= 2) || (std::is_floating_point<T>::value && sizeof(T) == 4))
			{
				std::memcpy(dstBuf, &in, sizeof(T) * len);
				return sizeof(T) * len;
			}
			else
			{
				uint32_t offset = 0;
				for (auto& o : in) offset += BytesFunc<T>::WriteTo(dstBuf + offset, o);
				return offset;
			}
		}
		static inline int ReadFrom(char const *srcBuf, uint32_t const &dataLen, uint32_t &offset, T &out)
		{
			if ((std::is_arithmetic<T>::value && sizeof(T) <= 2) || (std::is_floating_point<T>::value && sizeof(T) == 4))
			{
				if (dataLen < offset + len * sizeof(T)) return -1;
				std::memcpy(&out, srcBuf + offset, len * sizeof(T));
				offset += len * sizeof(T);
			}
			else
			{
				for (auto& o : out)
				{
					if (auto rtv = BytesFunc<AT>::ReadFrom(srcBuf + offset, dataLen, offset, o)) return rtv;
				}
			}
			return 0;
		}
	};

	// 适配 literal string( -1 是为了去掉尾部的 \0 )
	template<uint32_t len>
	struct BytesFunc<char [len], void>
	{
		typedef char (T)[len];
		static inline uint32_t Calc(T const &in)
		{
			return len - 1 + 5;
		}
		static inline uint32_t WriteTo(char *dstBuf, T const &in)
		{
			auto offset = BytesFunc<uint32_t>::WriteTo(dstBuf, len - 1);
			std::memcpy(dstBuf + offset, in, len - 1);
			return offset + len - 1;
		}
		static inline int ReadFrom(char const *srcBuf, uint32_t const &dataLen, uint32_t &offset, T &out)
		{
			assert(false);
			return 0;
		}
	};



	/**************************************************************************************************/
	// 将 BytesFunc 映射为全局函数以方便不借助 BBuffer 类来填充 buf
	/**************************************************************************************************/

	template<typename T> uint32_t BBCalc(T const &in)
	{
		return BytesFunc<T>::Calc(in);
	}
	template<typename T> uint32_t BBWriteTo(char *dstBuf, T const &in)
	{
		return BytesFunc<T>::WriteTo(dstBuf, in);
	}
	template<typename T> int BBReadFrom(char const *srcBuf, uint32_t const &dataLen, uint32_t &offset, T &out)
	{
		return BytesFunc<T>::ReadFrom(srcBuf, dataLen, offset, out);
	}

	/**************************************************************************************************/
	// 变参模板支持
	/**************************************************************************************************/

	template<typename T, typename...TS>
	uint32_t BBCalc(T const &in, TS const& ... ins)
	{
		return BBCalc(in) + BBCalc(ins...);
	}
	template<typename T, typename...TS>
	uint32_t BBWriteTo(char *dstBuf, T const &in, TS const& ... ins)
	{
		uint32_t offset = BBWriteTo(dstBuf, in);
		return offset + BBWriteTo(dstBuf + offset, ins...);
	}
	template<typename T, typename...TS>
	int BBReadFrom(char const *srcBuf, uint32_t const &dataLen, uint32_t &offset, T &out, TS &...outs)
	{
		auto rtv = BBReadFrom(srcBuf, dataLen, offset, out);
		if (rtv) return rtv;
		return BBReadFrom(srcBuf, dataLen, offset, outs...);
	}
}
