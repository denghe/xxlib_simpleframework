#pragma once
namespace xx
{
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

	inline size_t u32toa_branchlut(uint32_t value, char* buffer) noexcept
	{
		auto bak = buffer;											// 为计算长度而加
		if (value < 10000) {
			const uint32_t d1 = (value / 100) << 1;
			const uint32_t d2 = (value % 100) << 1;

			if (value >= 1000)
				*buffer++ = gDigitsLut[d1];
			if (value >= 100)
				*buffer++ = gDigitsLut[d1 + 1];
			if (value >= 10)
				*buffer++ = gDigitsLut[d2];
			*buffer++ = gDigitsLut[d2 + 1];
		}
		else if (value < 100000000) {
			// value = bbbbcccc
			const uint32_t b = value / 10000;
			const uint32_t c = value % 10000;

			const uint32_t d1 = (b / 100) << 1;
			const uint32_t d2 = (b % 100) << 1;

			const uint32_t d3 = (c / 100) << 1;
			const uint32_t d4 = (c % 100) << 1;

			if (value >= 10000000)
				*buffer++ = gDigitsLut[d1];
			if (value >= 1000000)
				*buffer++ = gDigitsLut[d1 + 1];
			if (value >= 100000)
				*buffer++ = gDigitsLut[d2];
			*buffer++ = gDigitsLut[d2 + 1];

			*buffer++ = gDigitsLut[d3];
			*buffer++ = gDigitsLut[d3 + 1];
			*buffer++ = gDigitsLut[d4];
			*buffer++ = gDigitsLut[d4 + 1];
		}
		else {
			// value = aabbbbcccc in decimal

			const uint32_t a = value / 100000000; // 1 to 42
			value %= 100000000;

			if (a >= 10) {
				const unsigned i = a << 1;
				*buffer++ = gDigitsLut[i];
				*buffer++ = gDigitsLut[i + 1];
			}
			else
				*buffer++ = '0' + static_cast<char>(a);

			const uint32_t b = value / 10000; // 0 to 9999
			const uint32_t c = value % 10000; // 0 to 9999

			const uint32_t d1 = (b / 100) << 1;
			const uint32_t d2 = (b % 100) << 1;

			const uint32_t d3 = (c / 100) << 1;
			const uint32_t d4 = (c % 100) << 1;

			*buffer++ = gDigitsLut[d1];
			*buffer++ = gDigitsLut[d1 + 1];
			*buffer++ = gDigitsLut[d2];
			*buffer++ = gDigitsLut[d2 + 1];
			*buffer++ = gDigitsLut[d3];
			*buffer++ = gDigitsLut[d3 + 1];
			*buffer++ = gDigitsLut[d4];
			*buffer++ = gDigitsLut[d4 + 1];
		}
		//*buffer++ = '\0';
		return uint32_t(buffer - bak);							// 返回填充了多长
	}

	inline size_t i32toa_branchlut(int32_t value, char* buffer) noexcept
	{
		uint32_t u = static_cast<uint32_t>(value);
		if (value < 0) {
			*buffer++ = '-';
			u = ~u + 1;
			return u32toa_branchlut(u, buffer) + 1;				// 返回填充了多长
		}
		return u32toa_branchlut(u, buffer);						// 返回填充了多长
	}

	inline size_t u64toa_branchlut(uint64_t value, char* buffer) noexcept
	{
		auto bak = buffer;										// 为计算长度而加
		if (value < 100000000) {
			uint32_t v = static_cast<uint32_t>(value);
			if (v < 10000) {
				const uint32_t d1 = (v / 100) << 1;
				const uint32_t d2 = (v % 100) << 1;

				if (v >= 1000)
					*buffer++ = gDigitsLut[d1];
				if (v >= 100)
					*buffer++ = gDigitsLut[d1 + 1];
				if (v >= 10)
					*buffer++ = gDigitsLut[d2];
				*buffer++ = gDigitsLut[d2 + 1];
			}
			else {
				// value = bbbbcccc
				const uint32_t b = v / 10000;
				const uint32_t c = v % 10000;

				const uint32_t d1 = (b / 100) << 1;
				const uint32_t d2 = (b % 100) << 1;

				const uint32_t d3 = (c / 100) << 1;
				const uint32_t d4 = (c % 100) << 1;

				if (value >= 10000000)
					*buffer++ = gDigitsLut[d1];
				if (value >= 1000000)
					*buffer++ = gDigitsLut[d1 + 1];
				if (value >= 100000)
					*buffer++ = gDigitsLut[d2];
				*buffer++ = gDigitsLut[d2 + 1];

				*buffer++ = gDigitsLut[d3];
				*buffer++ = gDigitsLut[d3 + 1];
				*buffer++ = gDigitsLut[d4];
				*buffer++ = gDigitsLut[d4 + 1];
			}
		}
		else if (value < 10000000000000000) {
			const uint32_t v0 = static_cast<uint32_t>(value / 100000000);
			const uint32_t v1 = static_cast<uint32_t>(value % 100000000);

			const uint32_t b0 = v0 / 10000;
			const uint32_t c0 = v0 % 10000;

			const uint32_t d1 = (b0 / 100) << 1;
			const uint32_t d2 = (b0 % 100) << 1;

			const uint32_t d3 = (c0 / 100) << 1;
			const uint32_t d4 = (c0 % 100) << 1;

			const uint32_t b1 = v1 / 10000;
			const uint32_t c1 = v1 % 10000;

			const uint32_t d5 = (b1 / 100) << 1;
			const uint32_t d6 = (b1 % 100) << 1;

			const uint32_t d7 = (c1 / 100) << 1;
			const uint32_t d8 = (c1 % 100) << 1;

			if (value >= 1000000000000000)
				*buffer++ = gDigitsLut[d1];
			if (value >= 100000000000000)
				*buffer++ = gDigitsLut[d1 + 1];
			if (value >= 10000000000000)
				*buffer++ = gDigitsLut[d2];
			if (value >= 1000000000000)
				*buffer++ = gDigitsLut[d2 + 1];
			if (value >= 100000000000)
				*buffer++ = gDigitsLut[d3];
			if (value >= 10000000000)
				*buffer++ = gDigitsLut[d3 + 1];
			if (value >= 1000000000)
				*buffer++ = gDigitsLut[d4];
			if (value >= 100000000)
				*buffer++ = gDigitsLut[d4 + 1];

			*buffer++ = gDigitsLut[d5];
			*buffer++ = gDigitsLut[d5 + 1];
			*buffer++ = gDigitsLut[d6];
			*buffer++ = gDigitsLut[d6 + 1];
			*buffer++ = gDigitsLut[d7];
			*buffer++ = gDigitsLut[d7 + 1];
			*buffer++ = gDigitsLut[d8];
			*buffer++ = gDigitsLut[d8 + 1];
		}
		else {
			const uint32_t a = static_cast<uint32_t>(value / 10000000000000000); // 1 to 1844
			value %= 10000000000000000;

			if (a < 10)
				*buffer++ = '0' + static_cast<char>(a);
			else if (a < 100) {
				const uint32_t i = a << 1;
				*buffer++ = gDigitsLut[i];
				*buffer++ = gDigitsLut[i + 1];
			}
			else if (a < 1000) {
				*buffer++ = '0' + static_cast<char>(a / 100);

				const uint32_t i = (a % 100) << 1;
				*buffer++ = gDigitsLut[i];
				*buffer++ = gDigitsLut[i + 1];
			}
			else {
				const uint32_t i = (a / 100) << 1;
				const uint32_t j = (a % 100) << 1;
				*buffer++ = gDigitsLut[i];
				*buffer++ = gDigitsLut[i + 1];
				*buffer++ = gDigitsLut[j];
				*buffer++ = gDigitsLut[j + 1];
			}

			const uint32_t v0 = static_cast<uint32_t>(value / 100000000);
			const uint32_t v1 = static_cast<uint32_t>(value % 100000000);

			const uint32_t b0 = v0 / 10000;
			const uint32_t c0 = v0 % 10000;

			const uint32_t d1 = (b0 / 100) << 1;
			const uint32_t d2 = (b0 % 100) << 1;

			const uint32_t d3 = (c0 / 100) << 1;
			const uint32_t d4 = (c0 % 100) << 1;

			const uint32_t b1 = v1 / 10000;
			const uint32_t c1 = v1 % 10000;

			const uint32_t d5 = (b1 / 100) << 1;
			const uint32_t d6 = (b1 % 100) << 1;

			const uint32_t d7 = (c1 / 100) << 1;
			const uint32_t d8 = (c1 % 100) << 1;

			*buffer++ = gDigitsLut[d1];
			*buffer++ = gDigitsLut[d1 + 1];
			*buffer++ = gDigitsLut[d2];
			*buffer++ = gDigitsLut[d2 + 1];
			*buffer++ = gDigitsLut[d3];
			*buffer++ = gDigitsLut[d3 + 1];
			*buffer++ = gDigitsLut[d4];
			*buffer++ = gDigitsLut[d4 + 1];
			*buffer++ = gDigitsLut[d5];
			*buffer++ = gDigitsLut[d5 + 1];
			*buffer++ = gDigitsLut[d6];
			*buffer++ = gDigitsLut[d6 + 1];
			*buffer++ = gDigitsLut[d7];
			*buffer++ = gDigitsLut[d7 + 1];
			*buffer++ = gDigitsLut[d8];
			*buffer++ = gDigitsLut[d8 + 1];
		}

		//*buffer = '\0';
		return size_t(buffer - bak);							// 返回填充了多长
	}

	inline size_t i64toa_branchlut(int64_t value, char* buffer) noexcept
	{
		uint64_t u = static_cast<uint64_t>(value);
		if (value < 0) {
			*buffer++ = '-';
			u = ~u + 1;
			return u64toa_branchlut(u, buffer) + 1;				// 返回填充了多长
		}
		return u64toa_branchlut(u, buffer);						// 返回填充了多长
	}



	// c-style char* 转为各种长度的 有符号整数. Out 取值范围： int8~64
	template <typename OutType>
	void ToInt(char const * in, OutType & out) noexcept
	{
		auto in_ = in;
		if (*in_ == '0')
		{
			out = 0;
			return;
		}
		bool b;
		if (*in_ == '-')
		{
			b = true;
			++in_;
		}
		else b = false;
		OutType r = *in_ - '0';
		char c;
		while ((c = *(++in_))) r = r * 10 + (c - '0');
		out = b ? -r : r;
	}

	// c-style char* (不能有减号打头) 转为各种长度的 无符号整数. Out 取值范围： uint8, uint16, uint32, uint64
	template <typename OutType>
	void ToUInt(char const * in, OutType & out) noexcept
	{
		assert(in);
		auto in_ = in;
		if (*in_ == '0')
		{
			out = 0;
			return;
		}
		OutType r = *(in_)-'0';
		char c;
		while ((c = *(++in_))) r = r * 10 + (c - '0');
		out = r;
	}






	/**************************************************************************************************/
	// 类型--操作适配模板区
	/**************************************************************************************************/


	// 适配 无符号整数 8 ~ 32bit
	template<typename T>
	struct StrFunc<T, std::enable_if_t<std::is_integral<T>::value && std::is_unsigned<T>::value && sizeof(T) <= 4>>
	{
		static inline void WriteTo(String& s, T const &in)
		{
			s.Reserve(s.dataLen + sizeof(T) * 4);
			s.dataLen += u32toa_branchlut(in, s.buf + s.dataLen);
		}
	};

	// 适配 有符号整数 8 ~ 32bit
	template<typename T>
	struct StrFunc<T, std::enable_if_t<std::is_integral<T>::value && !std::is_unsigned<T>::value && sizeof(T) <= 4>>
	{
		static inline void WriteTo(String& s, T const &in)
		{
			s.Reserve(s.dataLen + sizeof(T) * 4);
			s.dataLen += i32toa_branchlut(in, s.buf + s.dataLen);
		}
	};

	// 适配 无符号整数 64bit
	template<typename T>
	struct StrFunc<T, std::enable_if_t<std::is_integral<T>::value && std::is_unsigned<T>::value && sizeof(T) == 8>>
	{
		static inline void WriteTo(String& s, T const &in)
		{
			s.Reserve(s.dataLen + sizeof(T) * 4);
			s.dataLen += u64toa_branchlut(in, s.buf + s.dataLen);
		}
	};

	// 适配 有符号整数 64bit
	template<typename T>
	struct StrFunc<T, std::enable_if_t<std::is_integral<T>::value && !std::is_unsigned<T>::value && sizeof(T) == 8>>
	{
		static inline void WriteTo(String& s, T const &in)
		{
			s.Reserve(s.dataLen + sizeof(T) * 4);
			s.dataLen += i64toa_branchlut(in, s.buf + s.dataLen);
		}
	};

	// 适配 枚举( 转为整数处理 )
	template<typename T>
	struct StrFunc<T, std::enable_if_t<std::is_enum<T>::value>>
	{
		static inline void WriteTo(String& s, T const &in)
		{
			StrFunc<std::underlying_type_t<T>>::WriteTo(s, (std::underlying_type_t<T> const&)in);
		}
	};

	// 适配 浮点 float / double
	template<typename T>
	struct StrFunc<T, std::enable_if_t<std::is_floating_point<T>::value>>
	{
		static inline void WriteTo(String& s, T const &in)
		{
			s.Reserve(s.dataLen + sizeof(T) * 3);
			sprintf(s.buf + s.dataLen, "%g", in);
		}
	};

	// 适配 bool
	template<>
	struct StrFunc<bool, void>
	{
		static inline void WriteTo(String& s, bool const &in)
		{
			s.Reserve(s.dataLen + 5);
			if (in)
			{
				memcpy(s.buf + s.dataLen, "true", 4);
				s.dataLen += 4;
			}
			else
			{
				memcpy(s.buf + s.dataLen, "false", 5);
				s.dataLen += 5;
			}
		}
	};

	// 适配 单字符
	template<>
	struct StrFunc<char, void>
	{
		static inline void WriteTo(String& s, char const &in)
		{
			s.Reserve(s.dataLen + 1);
			s.buf[s.dataLen++] = in;
		}
	};

	// 适配 char* \0 结尾 字串( 不是很高效 )
	template<>
	struct StrFunc<char const*, void>
	{
		static inline void WriteTo(String& s, char const* const &in)
		{
			if (in)
			{
				auto len = strlen(in);
				s.Reserve(s.dataLen + len);
				memcpy(s.buf + s.dataLen, in, len);
				s.dataLen += len;
			};
		}
	};

	// 适配 literal char[len] string
	template<size_t len>
	struct StrFunc<char[len], void>
	{
		static inline void WriteTo(String& s, char const(&in)[len])
		{
			s.Reserve(s.dataLen + len - 1);
			memcpy(s.buf + s.dataLen, in, len - 1);
			s.dataLen += len - 1;
		}
	};




	// 适配 wchar_t
	template<>
	struct StrFunc<wchar_t, void>
	{
		static inline void WriteTo(String& s, wchar_t const &in)
		{
			s.Reserve(s.dataLen + 3);
			s.dataLen += ToUtf8(s.buf + s.dataLen, in);
		}
	};

	// 适配 wchar_t* \0 结尾 字串( 不是很高效 )
	template<>
	struct StrFunc<wchar_t const*, void>
	{
		static inline void WriteTo(String& s, wchar_t const* const &in)
		{
			if (in)
			{
				size_t i = 0;
				while (auto c = in[i])
				{
					StrFunc<wchar_t>::WriteTo(s, c);
				}
			}
		}
	};

	// 适配 literal wchar_t[len] string
	template<size_t len>
	struct StrFunc<wchar_t[len], void>
	{
		static inline void WriteTo(String& s, wchar_t const(&in)[len])
		{
			s.Reserve(s.dataLen + (len - 1) * 3);
			for (size_t i = 0; i < len - 1; ++i)
			{
				s.dataLen += ToUtf8(s.buf + s.dataLen, in[i]);
			}
		}
	};





	// 适配 Object
	template<typename T>
	struct StrFunc<T, std::enable_if_t<std::is_base_of_v<Object, T>>>
	{
		static inline void WriteTo(String& s, T const &in)
		{
			in.ToString(s);
		}
	};

	// 适配 Ptr<T>
	template<typename T>
	struct StrFunc<T, std::enable_if_t<IsPtr_v<T>>>
	{
		static inline void WriteTo(String& s, T const &in)
		{
			if (in)
			{
				in->ToString(s);
			}
			else
			{
				s.Append("nil");
			}
		}
	};

	// 适配 Ref<T>
	template<typename T>
	struct StrFunc<T, std::enable_if_t<IsRef_v<T>>>
	{
		static inline void WriteTo(String& s, T const &in)
		{
			if (in)
			{
				in->ToString(s);
			}
			else
			{
				s.Append("nil");
			}
		}
	};

}
