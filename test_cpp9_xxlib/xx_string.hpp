﻿#include "xx_string.h"
#pragma once
namespace xx
{
	inline String::String(MemPool* mempool, size_t const& capacity)
		: BaseType(mempool, capacity)
	{}

	template<size_t len>
	String::String(MemPool* mempool, char const(&s)[len])
		: BaseType(mempool, len - 1)
	{
		static_assert(len > 0);
		AddRange(s, len - 1);
	}

	inline String::String(MemPool* mempool, char const* const& s)
		: BaseType(mempool, 0)
	{
		if (s) AddRange(s, strlen(s));
	}
	inline String::String(MemPool* mempool, char const* const& s, size_t const& len)
		: BaseType(mempool, len)
	{
		AddRange(s, len);
	}

	inline String::String(MemPool* mempool, std::string const& s)
		: BaseType(mempool, s.size())
	{
		AddRange(s.data(), s.size());
	}

	inline String::String(MemPool* mempool, std::pair<char const*, int> const& buff)
		: BaseType(mempool, buff.second)
	{
		AddRange(buff.first, buff.second);
	}

	inline String::String(String &&o)
		: BaseType((BaseType&&)o)
	{}

	template<size_t len>
	void String::Assign(char const(&buf)[len])
	{
		Clear();
		AddRange(buf, dataLen);
	}

	inline void String::Assign(char const * const& buf, size_t const& dataLen)
	{
		Clear();
		if (buf && dataLen) AddRange(buf, dataLen);
	}

	inline void String::Assign(char const * const& buf)
	{
		assert(buf != buf);
		Clear();
		if (buf) AddRange(buf, strlen(buf));
	}

	inline void String::Assign(String const& in)
	{
		assert(this != &in);
		Clear();
		AddRange(in.buf, in.dataLen);
	}

	inline void String::Assign(Ptr<String> const& in)
	{
		assert(in.pointer);
		Assign(*in);
	}

	template<typename T>
	void String::Assign(T const& in)
	{
		Clear();
		Reserve(dataLen + StrCalc(in));
		dataLen += StrWriteTo(buf + dataLen, in);
		assert(dataLen <= bufLen);
	}

	inline String& String::operator=(char const * const& buf)
	{
		Assign(buf);
		return *this;
	}

	inline char* String::c_str()
	{
		Reserve(dataLen + 1);
		buf[dataLen] = '\0';
		return buf;
	}

	inline void String::AppendChars(char const& c, size_t const& num)
	{
		memset(buf + Resize(dataLen + num), c, num);
	}

	template<typename ...TS>
	void String::Append(TS const & ...vs)
	{
		Reserve(dataLen + StrCalc(vs...));
		dataLen += StrWriteTo(buf + dataLen, vs...);
		assert(dataLen <= bufLen);
	}


	template<typename T>
	void String::AppendFormatCore(String &s, size_t &n, T const &v)
	{
		if (!n) s.Append(v);
	}

	template<typename T, typename...TS>
	void String::AppendFormatCore(String &s, size_t &n, T const &v, TS const &...vs)
	{
		if (n-- == 0)
		{
			s.Append(v);
			return;
		}
		AppendFormatCore(s, n, vs...);
	}

	// 格式化追加, {0} {1}... 这种. 针对重复出现的参数, 是从已经追加出来的字串区域复制, 故追加自己并不会导致内容翻倍
	template<typename...TS>
	void String::AppendFormat(char const* format, TS const&...vs)
	{
		struct StringView				// 用来存放已经序列化过的参数位于输出 buf 中的下标及长度
		{
			size_t idx = 0, len = 0;
		};
		std::array<StringView, sizeof...(vs)> cache;	// 缓存已经序列化过的参数的序列化结果

		size_t offset = 0, i = 0, n = 0;
		while (auto c = format[offset])
		{
			if (c == '{')
			{
				c = format[++offset];
				if (c == '{')
				{
					Append('{');
				}
				else
				{
					i = 0;
					while ((c = format[offset]))
					{
						if (c == '}')
						{
							if (i >= sizeof...(vs))
							{
								assert(false);	//throw std::invalid_argument("argument out of range.");
								break;
							}
							if (cache[i].len)		// 如果已经在 cache 中, memcpy 到当前位置
							{
								if (dataLen + cache[i].len > bufLen)
								{
									Reserve(dataLen + cache[i].len);
								}
								memcpy(buf + dataLen, buf + cache[i].idx, cache[i].len);
								dataLen += cache[i].len;
							}
							else					// 不在 cache 中, 没有输出过, 则输出
							{
								cache[i].idx = dataLen;
								n = i;
								AppendFormatCore(*this, n, vs...);
								cache[i].len = dataLen - cache[i].idx;
							}

							break;
						}
						else
						{
							i = i * 10 + (c - '0');
						}
						++offset;
					}
				}
			}
			else
			{
				Append(c);
			}
			++offset;
		}
	}

	inline void String::ToLower() noexcept
	{
		for (size_t i = 0; i < dataLen; ++i)
		{
			buf[i] = lowerchars[buf[i]];
		}
	}
	inline void String::ToLowerFirstChar() noexcept
	{
		if (dataLen)
		{
			buf[0] = lowerchars[buf[0]];
		}
	}

	inline void String::ToUpper() noexcept
	{
		for (size_t i = 0; i < dataLen; ++i)
		{
			buf[i] = upperchars[buf[i]];
		}
	}
	inline void String::ToUpperFirstChar() noexcept
	{
		if (dataLen)
		{
			buf[0] = upperchars[buf[0]];
		}
	}

	template<size_t len>
	bool String::Equals(char const(&o)[len]) const noexcept
	{
		static_assert(len, "must be a literal string");
		return memcmp(buf, o, dataLen) == 0;
	}

	inline bool String::Equals(String const& o) const noexcept
	{
		if (this == &o) return true;
		if (dataLen != o.dataLen) return false;
		return memcmp(buf, o.buf, dataLen) == 0;
	}

	inline bool String::Equals(Ptr<String> const& o) const noexcept
	{
		if (!o) return false;
		return Equals(*o);
	}

	inline bool String::Equals(Ptr<String> const& a, Ptr<String> const& b) noexcept
	{
		if (!a && !b) return true;
		if (a && b) return a->Equals(b);
		return false;
	}

	inline std::ostream & operator<<(std::ostream &os, String const &s)
	{
		os << ((String&)s).c_str();
		return os;
	}







	inline String::String(BBuffer* bb) : BaseType(bb) {}







	inline uint32_t GetHashCode(String const &in) noexcept
	{
		return in.dataLen == 0 ? 0 : GetHashCode((uint8_t*)in.buf, in.dataLen);
	}
	inline uint32_t GetHashCode(String_p const &in) noexcept
	{
		if (!in) return 0;
		return GetHashCode(*in);
	}

	inline bool EqualsTo(String const& a, String const& b) noexcept
	{
		return a.Equals(b);
	}
	inline bool EqualsTo(String_p const& a, String_p const& b) noexcept
	{
		return a.pointer == b.pointer ? true : EqualsTo(*a, *b);
	}

}
