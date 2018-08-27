#include "xx_string.h"
#pragma once
namespace xx
{
	inline String::String(MemPool* const& mempool) noexcept
		: BaseType(mempool, 0)
	{}

	inline String::String(String&& o) noexcept
		: BaseType((BaseType&&)o)
	{}

	inline String::String(MemPool* const& mempool, char const* const& s, size_t const& len) noexcept
		: BaseType(mempool, 0)
	{
		if (s && len) AddRange(s, len);
	}
	inline String::String(MemPool* const& mempool, wchar_t const* const& ws, size_t const& len) noexcept
		: BaseType(mempool, 0)
	{
		Reserve(len * 3);
		if (ws)
		{
			for (size_t i = 0; i < len; ++i)
			{
				dataLen += ToUtf8(buf + i, ws[i]);
			}
		}
	}

	template<typename T>
	inline String::String(MemPool* const& mempool, T const& in) noexcept
		: BaseType(mempool, 0)
	{
		StrFunc<T>::WriteTo(*this, in);
		assert(dataLen <= bufLen);
	}


	inline String* String::Assign(char const* const& s, size_t const& len) noexcept
	{
		Clear();
		if (s && len) AddRange(s, len);
		return this;
	}

	inline String* String::Assign(wchar_t const* const& ws, size_t const& len) noexcept
	{
		Clear();
		Reserve(len * 3);
		if (ws)
		{
			for (size_t i = 0; i < len; ++i)
			{
				dataLen += ToUtf8(buf + i, ws[i]);
			}
		}
		return this;
	}

	template<typename T>
	inline String* String::Assign(T const& in) noexcept
	{
		Clear();
		StrFunc<T>::WriteTo(*this, in);
		assert(dataLen <= bufLen);
		return this;
	}

	inline String& String::operator=(char const* const& buf) noexcept
	{
		Assign(buf);
		return *this;
	}

	inline char* String::c_str() noexcept
	{
		Reserve(dataLen + 1);
		buf[dataLen] = '\0';
		return buf;
	}

	inline void String::AppendChars(char const& c, size_t const& num) noexcept
	{
		memset(buf + Resize(dataLen + num), c, num);
	}

	template<typename...TS>
	void String::Append(TS const&...vs) noexcept
	{
		std::initializer_list<int> n{ (StrFunc<TS>::WriteTo(*this, vs), 0)... };
		assert(dataLen <= bufLen);
	}

	template<typename T>
	void String::AppendFormatCore(String& s, size_t& n, T const& v) noexcept
	{
		if (!n) s.Append(v);
	}

	template<typename T, typename...TS>
	void String::AppendFormatCore(String& s, size_t& n, T const& v, TS const&...vs) noexcept
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
	void String::AppendFormat(char const* const& format, TS const&...vs) noexcept
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
		if (dataLen != len - 1) return false;
		return memcmp(buf, o, dataLen) == 0;
	}

	inline bool String::Equals(char const* const& o) const noexcept
	{
		if (!o) return false;
		auto len = strlen(o);
		if (dataLen != len) return false;
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

	inline std::ostream & operator<<(std::ostream& os, String const& s) noexcept
	{
		os << ((String&)s).c_str();
		return os;
	}







	inline String::String(BBuffer* const& bb)
		: BaseType(bb)
	{}

	inline void String::ToString(String& s) const noexcept
	{
		s.Reserve(s.dataLen + dataLen);
		memcpy(s.buf + s.dataLen, buf, dataLen);
		s.dataLen += dataLen;
	}






	template<>
	struct HashFunc<String, void>
	{
		static uint32_t GetHashCode(String const& in) noexcept
		{
			return in.dataLen == 0 ? 0 : HashFunc<std::pair<char*, size_t>>::GetHashCode(std::make_pair(in.buf, in.dataLen));
		}
	};

	template<>
	struct HashFunc<String_p, void>
	{
		static uint32_t GetHashCode(String_p const& in) noexcept
		{
			return in ? HashFunc<String>::GetHashCode(*in) : 0;
		}
	};

	template<>
	struct HashFunc<String*, void>
	{
		static uint32_t GetHashCode(String* const& in) noexcept
		{
			return in ? HashFunc<String>::GetHashCode(*in) : 0;
		}
	};

	template<>
	struct HashFunc<String const*, void>
	{
		static uint32_t GetHashCode(String const* const& in) noexcept
		{
			return in ? HashFunc<String>::GetHashCode(*in) : 0;
		}
	};

	template<>
	struct HashFunc<String_r, void>
	{
		static uint32_t GetHashCode(String_r const& in) noexcept
		{
			return in ? HashFunc<String const*>::GetHashCode(in.pointer) : 0;
		}
	};





	template<>
	struct EqualsFunc<String, void>
	{
		static bool EqualsTo(String const& a, String const& b) noexcept
		{
			return a.Equals(b);
		}
	};

	template<>
	struct EqualsFunc<String_p, void>
	{
		static bool EqualsTo(String_p const& a, String_p const& b) noexcept
		{
			if (a.pointer == b.pointer) return true;
			if (a && b) return a->Equals(*b);
			return false;
		}
	};

	template<>
	struct EqualsFunc<String_r, void>
	{
		static bool EqualsTo(String_r const& a, String_r const& b) noexcept
		{
			if (a.pointer == b.pointer) return true;
			if (a && b) return a->Equals(*b);
			return false;
		}
	};

	template<>
	struct EqualsFunc<String const*, void>
	{
		static bool EqualsTo(String const* const& a, String const* const& b) noexcept
		{
			if (a == b) return true;
			if (a && b) return a->Equals(*b);
			return false;
		}
	};

	template<>
	struct EqualsFunc<String*, void>
	{
		static bool EqualsTo(String* const& a, String* const& b) noexcept
		{
			if (a == b) return true;
			if (a && b) return a->Equals(*b);
			return false;
		}
	};

}
