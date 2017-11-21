#pragma execution_character_set("utf-8")
#pragma once
#include "xxmempool.h"


// 含有最基础的数据序列化功能, 针对 c#, c++, lua 需要各自扩展相应的 引用 继承 部分
struct XxBuf
{
	XxMemPool*				mempool;
	char*					buf = nullptr;
	uint32_t				dataLen = 0;
	uint32_t				offset = 0;			// 已 读 | 发送 长度

	XxBuf(XxMemPool* mempool, char const* const& s, int const& len)
		: mempool(mempool)
		, dataLen(len)
		, offset(0)
	{
		if (!len)
		{
			buf = nullptr;
			return;
		}
		buf = (char*)mempool->Alloc(len);
		if (!buf) throw 1;
		memcpy(buf, s, len);
	}
	~XxBuf()
	{
		mempool->Free(buf);
	}
	XxBuf(XxBuf const&) = delete;
	XxBuf& operator=(XxBuf const&) = delete;


	// 内容扩容
	// 为简化设计, 内存不足时并不抛异常, 而是置空. 故下列所有 Write 操作均不会异常
	inline void Reserve(uint32_t const& capacity) noexcept
	{
		buf = (char*)mempool->Realloc(buf, capacity, dataLen);
		if (!buf) dataLen = 0;					
	}

	// 变长写入数值类型
	template<typename T>
	void Write(T const& v) noexcept
	{
		Reserve(dataLen + CalcWriteLen<T>());
		dataLen += WriteTo(buf + dataLen, v);
	}

	// memcpy 写入值类型
	template<typename T>
	void WritePod(T const& v) noexcept
	{
		Reserve(dataLen + sizeof(v));
		memcpy(buf + dataLen, &v, sizeof(v));
		dataLen += sizeof(v);
	}

	// 写入 变长长度 + 内容
	inline void WriteLenData(char const* const& s, uint32_t const& len) noexcept
	{
		Reserve(dataLen + CalcWriteLen<uint32_t>() + len);
		dataLen += WriteTo(buf + dataLen, len);
		if (len)
		{
			memcpy(buf + dataLen, s, len);
			dataLen += len;
		}
	}

	// 读出变长数值
	template<typename T>
	void Read(T& v)
	{
		if (auto rtv = ReadFrom(buf, dataLen, offset, v)) throw rtv;
	}

	template<typename T>
	void ReadPod(T& v)
	{
		if (offset + sizeof(T) > dataLen) throw 1;
		memcpy(&v, buf + offset, sizeof(T));
		offset += sizeof(T);
	}

	// 读出 变长长度 + 内容的指针( 危险 )
	inline void ReadLenData(char const*& s, uint32_t& len)
	{
		Read(len);
		if (offset + len > dataLen) throw 1;
		s = buf + offset;
	}

	// 清长度和读偏移
	inline void Clear() noexcept
	{
		dataLen = 0;
		offset = 0;
	}


	// 读出变长数值( 不抛版 )
	template<typename T>
	int NoExceptRead(T* v) noexcept
	{
		return ReadFrom(buf, dataLen, offset, *v);
	}

	// 读出变长数值( 不抛版 )
	template<typename T>
	int NoExceptReadPod(T* v) noexcept
	{
		if (offset + sizeof(T) > dataLen) return 1;
		memcpy(v, buf + offset, sizeof(T));
		offset += sizeof(T);
		return 0;
	}

	// 读出 变长长度 + 内容的指针( 危险 )( 不抛版 )
	inline int NoExceptReadLenData(char const** s, uint32_t* len) noexcept
	{
		if (auto rtv = NoExceptRead(len)) return rtv;
		if (offset + *len > dataLen) return 1;
		*s = buf + offset;
		return 0;
	}


	/**************************************************************************************************/
	// 一些处理变长读写的基础函数
	/**************************************************************************************************/

	// negative -> ZigZag positive.  效果：负数变正，正数 *= 2
	inline static uint16_t ZigZagEncode(int16_t in) noexcept
	{
		return (in << 1) ^ (in >> 15);
	}
	inline static uint32_t ZigZagEncode(int32_t in) noexcept
	{
		return (in << 1) ^ (in >> 31);
	}
	inline static uint64_t ZigZagEncode(int64_t in) noexcept
	{
		return (in << 1) ^ (in >> 63);
	}

	// ZigZag positive -> negative
	inline static int16_t ZigZagDecode(uint16_t in) noexcept
	{
		return (int16_t)(in >> 1) ^ (-(int16_t)(in & 1));
	}
	inline static int32_t ZigZagDecode(uint32_t in) noexcept
	{
		return (int32_t)(in >> 1) ^ (-(int32_t)(in & 1));
	}
	inline static int64_t ZigZagDecode(uint64_t in) noexcept
	{
		return (int64_t)(in >> 1) ^ (-(int64_t)(in & 1));
	}

	// uint16_t 变长写, 返回实际写入长度
	inline static uint32_t VarWrite7(char *dstBuf, uint16_t in) noexcept
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

	// uint32_t 变长写, 返回实际写入长度
	inline static uint32_t VarWrite7(char *dstBuf, uint32_t in) noexcept
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

	// uint64_t 变长写, 返回实际写入长度
	inline static uint32_t VarWrite7(char *dstBuf, uint64_t in) noexcept
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

	// uint16_t 变长读, 返回错误码或 0( 成功 )
	inline static int VarRead7(char const *srcBuf, uint32_t dataLen, uint32_t& offset, uint16_t &out) noexcept
	{
		if (offset >= dataLen) return 1;
		auto p = srcBuf + offset;
		uint32_t i = 0, b7;
		int32_t lshift = 0;
		uint32_t idx = 0;
		dataLen -= offset;
	Lab1:
		b7 = p[idx++];
		if (b7 > 0x7F)
		{
			if (idx == 3) return 2;
			if (idx >= dataLen) return 1;
			i |= (b7 & 0x7F) << lshift;
			lshift += 7;
			goto Lab1;
		}
		else
		{
			if (idx == 3 && b7 > 3) return 2;
			else
			{
				i |= b7 << lshift;
			}
		}
		out = (uint16_t)i;
		offset += idx;
		return 0;
	}

	// uint32_t 变长读, 返回错误码或 0( 成功 )
	inline static int VarRead7(char const *srcBuf, uint32_t dataLen, uint32_t& offset, uint32_t &out) noexcept
	{
		if (offset >= dataLen) return 1;
		auto p = srcBuf + offset;
		uint32_t i = 0, b7;
		int32_t lshift = 0;
		uint32_t idx = 0;
		dataLen -= offset;
	Lab1:
		b7 = p[idx++];
		if (b7 > 0x7F)
		{
			if (idx == 5) return 2;
			if (idx >= dataLen) return 1;
			i |= (b7 & 0x7F) << lshift;
			lshift += 7;
			goto Lab1;
		}
		else
		{
			if (idx == 5 && b7 > 15) return 2;
			else
			{
				i |= b7 << lshift;
			}
		}
		out = i;
		offset += idx;
		return 0;
	}

	// uint64_t 变长读, 返回错误码或 0( 成功 )
	inline static int VarRead7(char const *srcBuf, uint32_t dataLen, uint32_t &offset, uint64_t &out) noexcept
	{
		if (offset >= dataLen) return 1;
		auto p = srcBuf + offset;
		uint64_t i = 0, b7;
		int32_t lshift = 0;
		uint32_t idx = 0;
		dataLen -= offset;
	Lab1:
		b7 = p[idx++];
		if (b7 > 0x7F)
		{
			if (idx >= dataLen) return 1;
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
		return 0;
	}


	/**************************************************************************************************/
	// 上面基础函数的 算长 & 读 & 写的模板封装
	/**************************************************************************************************/

	// 计算写入 buf 后的最大长. 单字节 或 float 类型原样 copy, 其他类型均变长写入, 最长时会比原长 + 1
	template<typename T>
	static uint32_t CalcWriteLen() noexcept
	{
		if constexpr(sizeof(T) == 1 || std::is_same_v<float, T>) return sizeof(T);
		else return sizeof(T) + 1;
	}

	// 写入 buf. 需要在调用前提前预留最大长的内存
	template<typename T>
	static uint32_t WriteTo(char *dstBuf, T const &in) noexcept
	{
		if constexpr(std::is_same_v<bool, T>)
		{
			*dstBuf = in ? 1 : 0;
			return 1;
		}
		else if constexpr(sizeof(T) == 1 || std::is_same_v<float, T>)
		{
			memcpy(dstBuf, &in, sizeof(T));
			return sizeof(T);
		}
		else if constexpr(std::is_integral_v<T>)
		{
			if constexpr(std::is_unsigned_v<T>) return VarWrite7(dstBuf, in);
			else return VarWrite7(dstBuf, ZigZagEncode(in));
		}
		else // should be double
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
					return 1 + VarWrite7(dstBuf, ZigZagEncode(i));
				}
				else
				{
					dstBuf[0] = 5;
					memcpy(dstBuf + 1, &in, sizeof(double));
					return sizeof(double) + 1;
				}
			}
		}
	}

	// 从 buf 读出数据. 返回 0 表示成功, 非 0 是错误码. 不会抛错误
	template<typename T>
	static int ReadFrom(char const *srcBuf, uint32_t const &dataLen, uint32_t &offset, T &out) noexcept
	{
		if constexpr(std::is_same_v<bool, T>)
		{
			out = srcBuf[offset] != 0;
			offset += 1;
			return 0;
		}
		else if constexpr(sizeof(T) == 1 || std::is_same_v<float, T>)
		{
			if (offset + sizeof(T) > dataLen) return 1;
			memcpy(&out, srcBuf + offset, sizeof(T));
			offset += sizeof(T);
			return 0;
		}
		else if constexpr(std::is_integral_v<T>)
		{
			if constexpr(std::is_unsigned_v<T>)
			{
				return VarRead7(srcBuf, dataLen, offset, out);
			}
			else
			{
				std::make_unsigned_t<T> i = 0;
				auto rtv = VarRead7(srcBuf, dataLen, offset, i);
				out = ZigZagDecode(i);
				return rtv;
			}
		}
		else // should be double
		{
			if (offset >= dataLen) return 1;
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
				uint32_t i = 0;
				auto rtv = VarRead7(srcBuf, dataLen, offset, i);
				out = ZigZagDecode(i);
				return rtv;
			}
			case 5:
			{
				if (dataLen < offset + sizeof(double)) return 1;
				memcpy(&out, srcBuf + offset, sizeof(double));
				offset += sizeof(double);
				return 0;
			}
			default:
				return 2;
			}
		}
	}


};
