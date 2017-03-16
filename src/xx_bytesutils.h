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
	inline uint32_t ZigZagEncode(int32_t in)
	{
		return (in << 1) ^ (in >> 31);
	}
	inline uint64_t ZigZagEncode(int64_t in)
	{
		return (in << 1) ^ (in >> 63);
	}

	// ZigZag positive -> negative
	inline int32_t ZigZagDecode(uint32_t in)
	{
		return (int32_t)(in >> 1) ^ (-(int32_t)(in & 1));
	}
	inline int64_t ZigZagDecode(uint64_t in)
	{
		return (int64_t)(in >> 1) ^ (-(int64_t)(in & 1));
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
	// 写入长度大致预估函数( 声明和实现 )
	/**************************************************************************************************/

	template<typename T> uint32_t BBCalc(T const &in)
	{
		static_assert(std::is_enum<T>::value);
		return BBCalc((std::underlying_type_t<T> const&)in);
	}
	template<> inline uint32_t BBCalc(bool     const &in) { return 1; }
	template<> inline uint32_t BBCalc(char     const &in) { return 1; }
	template<> inline uint32_t BBCalc(int8_t   const &in) { return 1; }
	template<> inline uint32_t BBCalc(uint8_t  const &in) { return 1; }
	template<> inline uint32_t BBCalc(int16_t  const &in) { return 2; }
	template<> inline uint32_t BBCalc(uint16_t const &in) { return 2; }
	template<> inline uint32_t BBCalc(int32_t  const &in) { return 5; }
	template<> inline uint32_t BBCalc(uint32_t const &in) { return 5; }
	template<> inline uint32_t BBCalc(int64_t  const &in) { return 9; }
	template<> inline uint32_t BBCalc(uint64_t const &in) { return 9; }
	template<> inline uint32_t BBCalc(float    const &in) { return 4; }
	template<> inline uint32_t BBCalc(double   const &in) { return 9; }
	template<> inline uint32_t BBCalc(void*    const &in) { return sizeof(void*) + 1; }

	/**************************************************************************************************/
	// 写入函数( 声明 )
	/**************************************************************************************************/

	template<typename T> uint32_t BBWriteTo(char *dstBuf, T const &in)
	{
		static_assert(std::is_enum<T>::value);
		return BBWriteTo(dstBuf, (std::underlying_type_t<T> const&)in);
	}
	template<> uint32_t BBWriteTo(char *dstBuf, bool     const &in);
	template<> uint32_t BBWriteTo(char *dstBuf, char     const &in);
	template<> uint32_t BBWriteTo(char *dstBuf, int8_t   const &in);
	template<> uint32_t BBWriteTo(char *dstBuf, uint8_t  const &in);
	template<> uint32_t BBWriteTo(char *dstBuf, int16_t  const &in);
	template<> uint32_t BBWriteTo(char *dstBuf, uint16_t const &in);
	template<> uint32_t BBWriteTo(char *dstBuf, int32_t  const &in);
	template<> uint32_t BBWriteTo(char *dstBuf, uint32_t const &in);
	template<> uint32_t BBWriteTo(char *dstBuf, int64_t  const &in);
	template<> uint32_t BBWriteTo(char *dstBuf, uint64_t const &in);
	template<> uint32_t BBWriteTo(char *dstBuf, float    const &in);
	template<> uint32_t BBWriteTo(char *dstBuf, double   const &in);
	template<> uint32_t BBWriteTo(char *dstBuf, void*    const &in);

	/**************************************************************************************************/
	// 读取函数( 声明 )
	/**************************************************************************************************/

	template<typename T> int BBReadFrom(char const *srcBuf, uint32_t const &dataLen, uint32_t &offset, T &out)
	{
		static_assert(std::is_enum<T>::value);
		return BBReadFrom(srcBuf, dataLen, offset, (std::underlying_type_t<T>&)out);
	}
	template<> int BBReadFrom(char const *srcBuf, uint32_t const &dataLen, uint32_t &offset, bool     &out);
	template<> int BBReadFrom(char const *srcBuf, uint32_t const &dataLen, uint32_t &offset, char     &out);
	template<> int BBReadFrom(char const *srcBuf, uint32_t const &dataLen, uint32_t &offset, int8_t   &out);
	template<> int BBReadFrom(char const *srcBuf, uint32_t const &dataLen, uint32_t &offset, uint8_t  &out);
	template<> int BBReadFrom(char const *srcBuf, uint32_t const &dataLen, uint32_t &offset, int16_t  &out);
	template<> int BBReadFrom(char const *srcBuf, uint32_t const &dataLen, uint32_t &offset, uint16_t &out);
	template<> int BBReadFrom(char const *srcBuf, uint32_t const &dataLen, uint32_t &offset, int32_t  &out);
	template<> int BBReadFrom(char const *srcBuf, uint32_t const &dataLen, uint32_t &offset, uint32_t &out);
	template<> int BBReadFrom(char const *srcBuf, uint32_t const &dataLen, uint32_t &offset, int64_t  &out);
	template<> int BBReadFrom(char const *srcBuf, uint32_t const &dataLen, uint32_t &offset, uint64_t &out);
	template<> int BBReadFrom(char const *srcBuf, uint32_t const &dataLen, uint32_t &offset, float    &out);
	template<> int BBReadFrom(char const *srcBuf, uint32_t const &dataLen, uint32_t &offset, double   &out);
	template<> int BBReadFrom(char const *srcBuf, uint32_t const &dataLen, uint32_t &offset, void *    &out);

	/**************************************************************************************************/
	// 下面是各种实现
	/**************************************************************************************************/

	// memcpy 系列 write
	template<typename T>
	uint32_t BBDirectWriteToCore(char *dstBuf, T const &in)
	{
		memcpy(dstBuf, &in, sizeof(T));
		return sizeof(T);
	}
	template<> inline uint32_t BBWriteTo(char *dstBuf, int8_t   const &in) { *dstBuf = (char)in; return 1; }
	template<> inline uint32_t BBWriteTo(char *dstBuf, uint8_t  const &in) { *dstBuf = (char)in; return 1; }
	template<> inline uint32_t BBWriteTo(char *dstBuf, int16_t  const &in) { return BBDirectWriteToCore(dstBuf, in); }
	template<> inline uint32_t BBWriteTo(char *dstBuf, uint16_t const &in) { return BBDirectWriteToCore(dstBuf, in); }
	template<> inline uint32_t BBWriteTo(char *dstBuf, float    const &in) { return BBDirectWriteToCore(dstBuf, in); }
	template<> inline uint32_t BBWriteTo(char *dstBuf, bool	  const &in) { *dstBuf = in ? 1 : 0; return 1; }
	template<> inline uint32_t BBWriteTo(char *dstBuf, char	  const &in) { *dstBuf = in; return 1; }

	// memcpy 系列 read
	template<typename T>
	int BBDirectReadFromCore(char const *srcBuf, uint32_t const &dataLen, uint32_t &offset, T &out)
	{
		if (offset + sizeof(T) > dataLen) return -1;
		memcpy(&out, srcBuf + offset, sizeof(T));
		offset += sizeof(T);
		return 0;
	}
	template<> inline int BBReadFrom(char const *srcBuf, uint32_t const &dataLen, uint32_t &offset, int8_t   &out) { if (offset >= dataLen) return -1; out = (int8_t)srcBuf[offset++]; return 0; }
	template<> inline int BBReadFrom(char const *srcBuf, uint32_t const &dataLen, uint32_t &offset, uint8_t  &out) { if (offset >= dataLen) return -1; out = (uint8_t)srcBuf[offset++]; return 0; }
	template<> inline int BBReadFrom(char const *srcBuf, uint32_t const &dataLen, uint32_t &offset, int16_t  &out) { return BBDirectReadFromCore(srcBuf, dataLen, offset, out); }
	template<> inline int BBReadFrom(char const *srcBuf, uint32_t const &dataLen, uint32_t &offset, uint16_t &out) { return BBDirectReadFromCore(srcBuf, dataLen, offset, out); }
	template<> inline int BBReadFrom(char const *srcBuf, uint32_t const &dataLen, uint32_t &offset, float    &out) { return BBDirectReadFromCore(srcBuf, dataLen, offset, out); }
	template<> inline int BBReadFrom(char const *srcBuf, uint32_t const &dataLen, uint32_t &offset, bool     &out) { if (offset >= dataLen) return -1; out = srcBuf[offset++] ? true : false; return 0; }
	template<> inline int BBReadFrom(char const *srcBuf, uint32_t const &dataLen, uint32_t &offset, char     &out) { if (offset >= dataLen) return -1; out = srcBuf[offset++]; return 0; }

	// 变长系列 write
	template<typename T>
	uint32_t BBVarWriteToCore(char *dstBuf, std::enable_if_t<std::is_signed<T>::value, T> const &in)
	{
		return VarWrite7(dstBuf, ZigZagEncode(in));
	}
	template<typename T>
	uint32_t BBVarWriteToCore(char *dstBuf, std::enable_if_t<std::is_unsigned<T>::value, T> const &in)
	{
		return VarWrite7(dstBuf, in);
	}
	template<> inline uint32_t BBWriteTo(char *dstBuf, int32_t  const &in) { return BBVarWriteToCore<int32_t>(dstBuf, in); }
	template<> inline uint32_t BBWriteTo(char *dstBuf, uint32_t const &in) { return BBVarWriteToCore<uint32_t>(dstBuf, in); }
	template<> inline uint32_t BBWriteTo(char *dstBuf, int64_t  const &in) { return BBVarWriteToCore<int64_t>(dstBuf, in); }
	template<> inline uint32_t BBWriteTo(char *dstBuf, uint64_t const &in) { return BBVarWriteToCore<uint64_t>(dstBuf, in); }
	template<> inline uint32_t BBWriteTo(char *dstBuf, void*    const &in) { return BBVarWriteToCore<uint64_t>(dstBuf, (uint64_t const &)in); }
	template<> inline uint32_t BBWriteTo(char *dstBuf, double   const &in)
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
			auto i = (int)in;
			if (in == (double)i)
			{
				dstBuf[0] = 4;
				return 1 + BBWriteTo(dstBuf + 1, i);
			}
			else
			{
				dstBuf[0] = 5;
				memcpy(dstBuf + 1, &in, sizeof(double));
				return sizeof(double) + 1;
			}
		}
	}

	// 变长系列 read
	template<typename T>
	int BBVarReadFromCore(char const *srcBuf, uint32_t const &dataLen, uint32_t &offset, std::enable_if_t<std::is_signed<T>::value, T> &out)
	{
		std::make_unsigned_t<T> i;
		auto rtv = VarRead7(srcBuf, dataLen, offset, i);
		out = ZigZagDecode(i);
		return rtv;
	}
	template<typename T>
	int BBVarReadFromCore(char const *srcBuf, uint32_t const &dataLen, uint32_t &offset, std::enable_if_t<std::is_unsigned<T>::value, T> &out)
	{
		return VarRead7(srcBuf, dataLen, offset, out);
	}
	template<> inline int BBReadFrom(char const *srcBuf, uint32_t const &dataLen, uint32_t &offset, int32_t  &out) { return BBVarReadFromCore<int32_t>(srcBuf, dataLen, offset, out); }
	template<> inline int BBReadFrom(char const *srcBuf, uint32_t const &dataLen, uint32_t &offset, uint32_t &out) { return BBVarReadFromCore<uint32_t>(srcBuf, dataLen, offset, out); }
	template<> inline int BBReadFrom(char const *srcBuf, uint32_t const &dataLen, uint32_t &offset, int64_t  &out) { return BBVarReadFromCore<int64_t>(srcBuf, dataLen, offset, out); }
	template<> inline int BBReadFrom(char const *srcBuf, uint32_t const &dataLen, uint32_t &offset, uint64_t &out) { return BBVarReadFromCore<uint64_t>(srcBuf, dataLen, offset, out); }
	template<> inline int BBReadFrom(char const *srcBuf, uint32_t const &dataLen, uint32_t &offset, void *   &out) { return BBVarReadFromCore<uint64_t>(srcBuf, dataLen, offset, (uint64_t &)out); }
	template<> inline int BBReadFrom(char const *srcBuf, uint32_t const &dataLen, uint32_t &offset, double &out)
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
			int i = 0;
			auto rtv = BBReadFrom(srcBuf, dataLen, offset, i);
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





	/**************************************************************************************************/
	// 定长数组支持
	/**************************************************************************************************/

	// std array 
	template<typename T, size_t len>
	uint32_t BBCalc(std::array<T, len> const&in)
	{
		return BBCalc(T()) * len;
	}

	template<typename T, size_t len>
	uint32_t BBWriteTo(char *dstBuf, std::array<T, len> const&in)
	{
		if (sizeof(T) <= 2 || std::is_same<T, float>::value)
		{
			memcpy(dstBuf, &in, sizeof(T) * len);
			return sizeof(T) * len;
		}
		else
		{
			uint32_t offset = 0;
			for (auto& o : in) offset += BBWriteTo(dstBuf + offset, o);
			return offset;
		}
	}

	template<typename T, size_t len>
	int BBReadFrom(char const *srcBuf, uint32_t const &dataLen, uint32_t &offset, std::array<T, len>&out)
	{
		if (sizeof(T) <= 2 || std::is_same<T, float>::value)
		{
			if (dataLen < offset + len * sizeof(T)) return -1;
			memcpy(&out, srcBuf + offset, len * sizeof(T));
			offset += len * sizeof(T);
		}
		else
		{
			for (auto& o : out)
			{
				if (auto rtv = BBReadFrom(srcBuf + offset, dataLen, offset, o)) return rtv;
			}
		}
		return 0;
	}

	// 传统数组

	template<typename T, size_t len>
	uint32_t BBCalc(T const(&in)[len])
	{
		return BBCalc(T()) * len;
	}

	template<typename T, size_t len>
	uint32_t BBWriteTo(char *dstBuf, T const(&in)[len])
	{
		if (sizeof(T) <= 2 || std::is_same<T, float>::value)
		{
			memcpy(dstBuf, &in, sizeof(T) * len);
			return sizeof(T) * len;
		}
		else
		{
			uint32_t offset = 0;
			for (auto& o : in) offset += BBWriteTo(dstBuf + offset, o);
			return offset;
		}
	}

	template<typename T, size_t len>
	int BBReadFrom(char const *srcBuf, uint32_t const &dataLen, uint32_t &offset, T(&out)[len])
	{
		if (sizeof(T) <= 2 || std::is_same<T, float>::value)
		{
			if (dataLen < offset + len * sizeof(T)) return -1;
			memcpy(out, srcBuf + offset, len * sizeof(T));
			offset += len * sizeof(T);
			return 0;
		}
		else
		{
			for (auto& o : out)
			{
				if (auto rtv = BBReadFrom(srcBuf, dataLen, offset, o)) return rtv;
			}
		}
		return 0;
	}

}
