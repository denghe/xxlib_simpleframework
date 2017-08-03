#pragma once
#include "xx_list.h"
#include "xx_links.h"
#include "xx_charsutils.h"
#include "xx_hashutils.h"
#include <string>
#include <iostream>

namespace xx
{
	// 一些 gcc 要求必须声明在外面的路由类
	template<bool>
	struct StringAppendSwitcher;
	template<>
	struct StringAppendSwitcher<false>;
	template<>
	struct StringAppendSwitcher<true>;

	struct BBuffer;

	// 类似 std::string 的容器
	struct String : public List<char>
	{
		typedef List<char> BaseType;

		explicit String(uint32_t capacity = 0) : BaseType(capacity) {}

		template<uint32_t len>
		String(char const(&s)[len]) : BaseType(len - 1)
		{
			static_assert(len > 0, "");
			AddRange(s, len - 1);
		}
		String(char const *s, uint32_t len) : BaseType(len)
		{
			AddRange(s, len);
		}
		String(std::string const& s) : BaseType((uint32_t)s.size())
		{
			AddRange(s.data(), (uint32_t)s.size());
		}

		String(BBuffer* bb);

		String(String const&o) = delete;
		String(String &&o)
			: BaseType(std::move(o))
		{
		}
		String& operator=(String const&o) = delete;

		inline void Assign(char const * const& buf, uint32_t const& dataLen)
		{
			Clear();
			if (buf && dataLen) AddRange(buf, dataLen);
		}
		inline void Assign(char const * const& buf)
		{
			Clear();
			if (buf) AddRange(buf, (uint32_t)strlen(buf));
		}
		inline void Assign(MPObject const* const& in)
		{
			Clear();
			if (in) in->ToString(*this);
		}
		inline void Assign(String const& in)
		{
			Clear();
			Reserve(in.dataLen);
			memcpy(buf, in.buf, in.dataLen);
			dataLen = in.dataLen;
		}
		inline void Assign(String const* const& in)
		{
			Clear();
			if (!in) return;
			Reserve(in->dataLen);
			memcpy(buf, in->buf, in->dataLen);
			dataLen = in->dataLen;
		}
		// todo: more

		template<typename T>
		void Assign(T const& in)
		{
			Clear();
			Reserve(dataLen + StrCalc(in));
			dataLen += StrWriteTo(buf + dataLen, in);
			assert(dataLen <= bufLen);
		}

		// todo: 上面 Assign 中单个参数的实现, 做成 operator=
		// 先来一发最常见的.
		String& operator=(char const * const& buf)
		{
			Assign(buf);
			return *this;
		}


		inline virtual void ToString(String& str) const override
		{
			auto thisDataLen = dataLen;
			str.Reserve(str.dataLen + thisDataLen + 2);
			str.buf[str.dataLen] = '\"';
			memcpy(str.buf + str.dataLen + 1, buf, thisDataLen);
			str.buf[str.dataLen + thisDataLen + 1] = '\"';
			str.dataLen += thisDataLen + 2;
		}

		inline char* C_str()
		{
			Reserve(dataLen + 1);
			buf[dataLen] = '\0';
			return buf;
		}

		inline void AppendChars(char const& c = ' ', uint32_t const& num = 1)
		{
			memset(buf + Resize(dataLen + num), c, num);
		}

		template<typename ...TS>
		void Append(TS const & ...vs)
		{
			StringAppendSwitcher<ExistsMPObject<TS...>::value>::Exec(*this, vs...);
		}

	protected:

		friend StringAppendSwitcher<true>;
		friend StringAppendSwitcher<false>;

		void AppendPtr(MPObject const* const& v)
		{
			if (v) v->ToString(*this);
			else Append("nil");
		}

		template<typename T>
		void AppendFormatCore(String &s, uint32_t &n, T const &v)
		{
			if (!n) s.Append(v);
		}

		template<typename T, typename...TS>
		void AppendFormatCore(String &s, uint32_t &n, T const &v, TS const &...vs)
		{
			if (n-- == 0)
			{
				s.Append(v);
				return;
			}
			AppendFormatCore(s, n, vs...);
		}
	public:

		// 格式化追加, {0} {1}... 这种. 针对重复出现的参数, 是从已经追加出来的字串区域复制, 故追加自己并不会导致内容翻倍
		template<typename...TS>
		void AppendFormat(char const* format, TS const&...vs)
		{
			struct flag
			{
				uint32_t idx, len;
			};
			flag flags[sizeof...(vs)];
			memset(flags, 0, sizeof(flag) * sizeof...(vs));

#ifdef _MSC_VER
			__declspec(align(8)) char numBuf[32];
#else
			char numBuf[32] __attribute__((aligned(8)));
#endif
			uint32_t numBuf_dataLen = 0;

			uint32_t offset = 0, i = 0, n = 0;
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
						while ((c = format[offset]))
						{
							if (c == '}')
							{
								numBuf[numBuf_dataLen] = '\0';				// for terminate that FromString is need it
								FromString(i, numBuf);
								numBuf_dataLen = 0;
								if (i < 0 || i >= sizeof...(vs))
								{
									assert(false);	//throw std::invalid_argument("argument out of range.");
									break;
								}
								if (flags[i].len)
								{
									if (dataLen + flags[i].len > bufLen)
									{
										Reserve(dataLen + flags[i].len);
									}
									memcpy(buf + dataLen, buf + flags[i].idx, flags[i].len);
									dataLen += flags[i].len;
								}
								else
								{
									flags[i].idx = dataLen;
									n = i;
									AppendFormatCore(*this, n, vs...);
									flags[i].len = dataLen - flags[i].idx;
								}

								break;
							}
							else
							{
								numBuf[numBuf_dataLen++] = c;
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

		static unsigned char constexpr lowerchars[] =
		{
			0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26,
			27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51,
			52, 53, 54, 55, 56, 57, 58, 59, 60, 61, 62, 63, 64, 97, 98, 99, 100, 101, 102, 103, 104, 105, 106,
			107, 108, 109, 110, 111, 112, 113, 114, 115, 116, 117, 118, 119, 120, 121, 122, 91, 92, 93, 94, 95,
			96, 97, 98, 99, 100, 101, 102, 103, 104, 105, 106, 107, 108, 109, 110, 111, 112, 113, 114, 115, 116,
			117, 118, 119, 120, 121, 122, 123, 124, 125, 126, 127, 128, 129, 130, 131, 132, 133, 134, 135, 136,
			137, 138, 139, 140, 141, 142, 143, 144, 145, 146, 147, 148, 149, 150, 151, 152, 153, 154, 155, 156,
			157, 158, 159, 160, 161, 162, 163, 164, 165, 166, 167, 168, 169, 170, 171, 172, 173, 174, 175, 176,
			177, 178, 179, 180, 181, 182, 183, 184, 185, 186, 187, 188, 189, 190, 191, 192, 193, 194, 195, 196,
			197, 198, 199, 200, 201, 202, 203, 204, 205, 206, 207, 208, 209, 210, 211, 212, 213, 214, 215, 216,
			217, 218, 219, 220, 221, 222, 223, 224, 225, 226, 227, 228, 229, 230, 231, 232, 233, 234, 235, 236,
			237, 238, 239, 240, 241, 242, 243, 244, 245, 246, 247, 248, 249, 250, 251, 252, 253, 254, 255
		};
		static unsigned char constexpr upperchars[] =
		{
			0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26,
			27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51,
			52, 53, 54, 55, 56, 57, 58, 59, 60, 61, 62, 63, 64, 65, 66, 67, 68, 69, 70, 71, 72, 73, 74, 75, 76,
			77, 78, 79, 80, 81, 82, 83, 84, 85, 86, 87, 88, 89, 90, 91, 92, 93, 94, 95, 96, 65, 66, 67, 68, 69,
			70, 71, 72, 73, 74, 75, 76, 77, 78, 79, 80, 81, 82, 83, 84, 85, 86, 87, 88, 89, 90, 123, 124, 125,
			126, 127, 128, 129, 130, 131, 132, 133, 134, 135, 136, 137, 138, 139, 140, 141, 142, 143, 144, 145,
			146, 147, 148, 149, 150, 151, 152, 153, 154, 155, 156, 157, 158, 159, 160, 161, 162, 163, 164, 165,
			166, 167, 168, 169, 170, 171, 172, 173, 174, 175, 176, 177, 178, 179, 180, 181, 182, 183, 184, 185,
			186, 187, 188, 189, 190, 191, 192, 193, 194, 195, 196, 197, 198, 199, 200, 201, 202, 203, 204, 205,
			206, 207, 208, 209, 210, 211, 212, 213, 214, 215, 216, 217, 218, 219, 220, 221, 222, 223, 224, 225,
			226, 227, 228, 229, 230, 231, 232, 233, 234, 235, 236, 237, 238, 239, 240, 241, 242, 243, 244, 245,
			246, 247, 248, 249, 250, 251, 252, 253, 254, 255
		};

		inline void ToLower()
		{
			for (uint32_t i = 0; i < dataLen; ++i)
			{
				buf[i] = lowerchars[buf[i]];
			}
		}
		inline void ToLowerFirstChar()
		{
			if (dataLen)
			{
				buf[0] = lowerchars[buf[0]];
			}
		}

		inline void ToUpper()
		{
			for (uint32_t i = 0; i < dataLen; ++i)
			{
				buf[i] = upperchars[buf[i]];
			}
		}
		inline void ToUpperFirstChar()
		{
			if (dataLen)
			{
				buf[0] = upperchars[buf[0]];
			}
		}

		template<uint32_t len>
		bool Equals(char const(&o)[len]) const
		{
			static_assert(len, "must be a literal string");
			return memcmp(buf, o, dataLen) == 0;
		}
		inline bool Equals(String const& o) const
		{
			if (this == &o) return true;
			if (dataLen != o.dataLen) return false;
			return memcmp(buf, o.buf, dataLen) == 0;
		}
	};





	template<>
	struct StringAppendSwitcher<false>
	{
		template<typename ...TS>
		static void Exec(String& str, TS const& ...vs)
		{
			str.Reserve(str.dataLen + StrCalc(vs...));
			str.dataLen += StrWriteTo(str.buf + str.dataLen, vs...);
			assert(str.dataLen <= str.bufLen);
		}
	};

	template<>
	struct StringAppendSwitcher<true>
	{
		// T ( mpobject )
		template<typename T>
		static void WriteTo(String& str, std::enable_if_t< IsMPObjectStruct_v<T>, T> const& v)
		{
			v.ToString(str);
		}

		// T* ( mpobject )
		template<typename T>
		static void WriteTo(String& str, std::enable_if_t< IsMPObjectPointer_v<T>, T> const& v)
		{
			str.AppendPtr(v);
		}

		// Ptr<T>
		template<typename T>
		static void WriteTo(String& str, std::enable_if_t< IsPtr_v<T>, T> const& v)
		{
			str.AppendPtr(v.pointer);
		}

		// MPtr<T>
		template<typename T>
		static void WriteTo(String& str, std::enable_if_t< IsMPtr_v<T>, T> const& v)
		{
			str.AppendPtr(v ? v.pointer : nullptr);
		}

		// Dock<T>
		template<typename T>
		static void WriteTo(String& str, std::enable_if_t< IsDock_v<T>, T> const& v)
		{
			v->ToString(str);
		}

		// not mpobject
		template<typename T>
		static void WriteTo(String& str, std::enable_if_t<!IsMPObject_v<T>, T> const& v)
		{
			str.Reserve(str.dataLen + StrCalc(v));
			str.dataLen += StrWriteTo(str.buf + str.dataLen, v);
			assert(str.dataLen <= str.bufLen);
		}

		template<typename ...TS>
		static void Exec(String& str, TS const& ...vs)
		{
			std::initializer_list<int>{ (WriteTo<TS>(str, vs), 0)... };
		}
	};


	/*************************************************************************/
	// 实现值类型使用类型声明
	/*************************************************************************/

	using String_v = Dock<String>;
	using String_p = Ptr<String>;

	template<>
	struct MemmoveSupport<String_v>
	{
		static const bool value = true;
	};





	// MPObject 默认的 ToString 的实现代码

	inline void MPObject::ToString(String &str) const
	{
		if (tsFlags())
		{
			str.Append("{ ... }");
			return;
		}
		else tsFlags() = 1;

		str.Append("{ \"type\" : \"MPObject\"");
		ToStringCore(str);
		str.Append(" }");

		tsFlags() = 0;
	}

	inline void MPObject::ToStringCore(String &str) const
	{
	}


	template<typename ...Args>
	void MPObject::Cout(Args const& ... args)
	{
		mempool().Cout(args...);
	}

	template<typename ...Args>
	void MemPool::Cout(Args const& ... args)
	{
		xx::String_v s(*this);
		s->Append(args...);
		std::cout << s->C_str() << std::flush;
	}


	template<typename T, uint32_t reservedHeaderLen>
	void List<T, reservedHeaderLen>::ToString(String &str) const
	{
		if (tsFlags())
		{
			str.Append("{ ... }");
			return;
		}
		else tsFlags() = 1;

		str.Append("{ \"type\" : \"List\", \"items\" : [ ");
		for (size_t i = 0; i < dataLen; i++)
		{
			str.Append(buf[i], ", ");
		}
		if (dataLen) str.dataLen -= 2;
		str.Append(" ] }");

		tsFlags() = 0;
	}


	template <typename T>
	void Links<T>::ToString(String &str) const
	{
		if (tsFlags())
		{
			str.Append("{ ... }");
			return;
		}
		else tsFlags() = 1;

		str.Append("{ \"type\" : \"Links\", \"nodes\" : [ ");
		for (int i = header; i != -1; i = nodes[i].next)
		{
			str.Append("{ \"type\" : \"Node\", \"idx\" : ", i, ", \"prev\" : ", nodes[i].prev, ", \"next\" : ", nodes[i].next, ", \"value\" : ", nodes[i].value, " }, ");
		}
		if (Count()) str.dataLen -= 2;
		str.Append(" ] }");

		tsFlags() = 0;
	}



	// 各种适配函数的实现

	inline uint32_t GetHashCode(String const &in)
	{
		return in.dataLen == 0 ? 0 : GetHashCode((uint8_t*)in.buf, in.dataLen);
	}
	inline uint32_t GetHashCode(String* const &in)
	{
		if (!in) return 0;
		return in->dataLen == 0 ? 0 : GetHashCode((uint8_t*)in->buf, in->dataLen);
	}
	inline uint32_t GetHashCode(String const* const &in)
	{
		if (!in) return 0;
		return in->dataLen == 0 ? 0 : GetHashCode((uint8_t*)in->buf, in->dataLen);
	}
	inline uint32_t GetHashCode(MPtr<String> const &in)
	{
		return GetHashCode(in.Ensure());
	}
	inline uint32_t GetHashCode(String_p const &in)
	{
		return GetHashCode(*in);
	}
	inline uint32_t GetHashCode(String_v const &in)
	{
		return GetHashCode(*in);
	}

	inline bool EqualsTo(String const& a, String const& b)
	{
		return a.Equals(b);
	}
	inline bool EqualsTo(String* const& a, String* const& b)
	{
		if (a == b) return true;
		return a->Equals(*b);
	}
	inline bool EqualsTo(String const* const& a, String const* const& b)
	{
		if (a == b) return true;
		return a->Equals(*b);
	}
	inline bool EqualsTo(MPtr<String> const& a, MPtr<String> const& b)
	{
		return EqualsTo(a.Ensure(), b.Ensure());
	}
	inline bool EqualsTo(String_p const& a, String_p const& b)
	{
		return EqualsTo(*a, *b);
	}
	inline bool EqualsTo(String_v const& a, String_v const& b)
	{
		return EqualsTo(*a, *b);
	}

}
