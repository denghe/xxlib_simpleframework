#pragma once
namespace xx
{
	inline uint16_t ZigZagEncode(int16_t in) noexcept
	{
		return (in << 1) ^ (in >> 15);
	}
	inline uint32_t ZigZagEncode(int32_t in) noexcept
	{
		return (in << 1) ^ (in >> 31);
	}
	inline uint64_t ZigZagEncode(int64_t in) noexcept
	{
		return (in << 1) ^ (in >> 63);
	}

	inline int16_t ZigZagDecode(uint16_t in) noexcept
	{
		return (int16_t)(in >> 1) ^ (-(int16_t)(in & 1));
	}
	inline int32_t ZigZagDecode(uint32_t in) noexcept
	{
		return (int32_t)(in >> 1) ^ (-(int32_t)(in & 1));
	}
	inline int64_t ZigZagDecode(uint64_t in) noexcept
	{
		return (int64_t)(in >> 1) ^ (-(int64_t)(in & 1));
	}

	inline size_t VarWrite7(char *dstBuf, uint16_t in) noexcept
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

	inline int VarRead7(char const *srcBuf, size_t dataLen, size_t& offset, uint16_t &out) noexcept
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

	inline size_t VarWrite7(char *dstBuf, uint32_t in) noexcept
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

	inline int VarRead7(char const *srcBuf, size_t dataLen, size_t& offset, uint32_t &out) noexcept
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

	inline size_t VarWrite7(char *dstBuf, uint64_t in) noexcept
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

	inline int VarRead7(char const *srcBuf, size_t dataLen, size_t& offset, uint64_t &out) noexcept
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




	// 适配 1 字节长度的 数值 或 float( 这些类型直接 memcpy )
	template<typename T>
	struct BytesFunc<T, std::enable_if_t< (std::is_arithmetic_v<T> && sizeof(T) == 1) || (std::is_floating_point_v<T> && sizeof(T) == 4) >>
	{
		static inline size_t Calc(T const &in)
		{
			return sizeof(T);
		}
		static inline void WriteTo(BBuffer& bb, T const &in)
		{
			memcpy(bb.buf, &in, sizeof(T));
			bb.dataLen += sizeof(T);
		}
		static inline int ReadFrom(BBuffer& bb, T &out)
		{
			if (bb.offset + sizeof(T) > bb.dataLen) return -1;
			memcpy(&out, bb.buf + bb.offset, sizeof(T));
			bb.offset += sizeof(T);
			return 0;
		}
	};

	// 适配 2+ 字节无符号整数( 变长读写 )
	template<typename T>
	struct BytesFunc<T, std::enable_if_t<std::is_integral_v<T> && sizeof(T) >= 2 && std::is_unsigned_v<T>>>
	{
		static inline size_t Calc(T const &in)
		{
			return sizeof(T) + 1;
		}
		static inline void WriteTo(BBuffer& bb, T const &in)
		{
			bb.dataLen += VarWrite7(bb.buf, in);
		}
		static inline int ReadFrom(BBuffer& bb, T &out)
		{
			return VarRead7(bb.buf, bb.dataLen, bb.offset, out);
		}
	};

	// 适配 2+ 字节有符号整数( ZigZag 变长读写 )
	template<typename T>
	struct BytesFunc<T, std::enable_if_t<std::is_integral_v<T> && sizeof(T) >= 2 && !std::is_unsigned_v<T>>>
	{
		static inline size_t Calc(T const &in)
		{
			return sizeof(T) + 1;
		}
		static inline void WriteTo(BBuffer& bb, T const &in)
		{
			bb.dataLen += VarWrite7(bb.buf, ZigZagEncode(in));
		}
		static inline int ReadFrom(BBuffer& bb, T &out)
		{
			std::make_unsigned_t<T> i = 0;
			auto rtv = VarRead7(bb.buf, bb.dataLen, bb.offset, i);
			out = ZigZagDecode(i);
			return rtv;
		}
	};

	// 适配 enum( 根据原始数据类型调上面的适配 )
	template<typename T>
	struct BytesFunc<T, std::enable_if_t<std::is_enum_v<T>>>
	{
		typedef std::underlying_type_t<T> UT;
		static inline size_t Calc(T const &in)
		{
			return BytesFunc<UT>::Calc((UT const&)in);
		}
		static inline void WriteTo(BBuffer& bb, T const &in)
		{
			BytesFunc<UT>::WriteTo(bb, (UT const&)in);
		}
		static inline int ReadFrom(BBuffer& bb, T &out)
		{
			return BytesFunc<UT>::ReadFrom(bb, (UT&)out);
		}
	};

	// 适配 double
	template<>
	struct BytesFunc<double, void>
	{
		static inline size_t Calc(double const &in)
		{
			return sizeof(double) + 1;
		}
		static inline void WriteTo(BBuffer& bb, double const &in)
		{
			if (in == 0)
			{
				bb[bb.dataLen++] = 0;
			}
			else if (std::isnan(in))
			{
				bb[bb.dataLen++] = 1;
			}
			else if (in == -std::numeric_limits<double>::infinity())	// negative infinity
			{
				bb[bb.dataLen++] = 2;
			}
			else if (in == std::numeric_limits<double>::infinity())		// positive infinity
			{
				bb[bb.dataLen++] = 3;
			}
			else
			{
				auto i = (int32_t)in;
				if (in == (double)i)
				{
					bb[bb.dataLen++] = 4;
					BytesFunc<int32_t>::WriteTo(bb, i);
				}
				else
				{
					bb[bb.dataLen] = 5;
					memcpy(bb.buf + 1, &in, sizeof(double));
					bb.dataLen += sizeof(double) + 1;
				}
			}
		}
		static inline int ReadFrom(BBuffer& bb, double &out)
		{
			if (bb.offset >= bb.dataLen) return -1;		// 确保还有 1 字节可读
			switch (bb[bb.offset++])					// 跳过 1 字节
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
				if (auto rtv = BytesFunc<int32_t>::ReadFrom(bb, i)) return rtv;
				out = i;
				return 0;
			}
			case 5:
			{
				if (bb.dataLen < bb.offset + sizeof(double)) return -1;
				memcpy(&out, bb.buf + bb.offset, sizeof(double));
				bb.offset += sizeof(double);
				return 0;
			}
			default:
				return -2;								// failed
			}
		}
	};

	// 适配 literal string ( 只是为方便测试. 转为 String 写入 )
	template<size_t len>
	struct BytesFunc<char[len], void>
	{
		typedef char(T)[len];
		static inline size_t Calc(T const &in)
		{
			return len - 1 + 9;
		}
		static inline void WriteTo(BBuffer& bb, T const &in)
		{
			bb.Write(String(bb.mempool, in));
		}
		static inline int ReadFrom(BBuffer& bb, T &out)
		{
			assert(false);
			return 0;
		}
	};

	// 适配 Object
	template<typename T>
	struct BytesFunc<T, std::enable_if_t<std::is_base_of_v<Object, T>>>
	{
		static inline size_t Calc(T const &in)
		{
			return 12;	// typeId + null flag + offset
		}
		static inline void WriteTo(BBuffer& bb, T const &in)
		{
			//in.BBWrite
		}
		static inline int ReadFrom(BBuffer& bb, T &out)
		{
			//return out.BBRead(bb);
			return 0;
		}
	};


	// 适配 Ptr<T>
	template<typename T>
	struct BytesFunc<T, std::enable_if_t<IsPtr_v<T>>>
	{
		typedef typename T::ChildType CT;
		static inline size_t Calc(T const &in)
		{
			return 12;	// typeId + null flag + offset
		}
		static inline void WriteTo(BBuffer& bb, T const &in)
		{
			bb.WritePtr(v.pointer);
		}
		static inline int ReadFrom(BBuffer& bb, T &out)
		{
			CT* t = nullptr;
			auto rtv = bb.ReadPtr(t);
			out = t;
			return rtv;
		}
	};

	// todo: more
}
