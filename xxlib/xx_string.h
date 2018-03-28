#pragma once
namespace xx
{
	// std::string like
	class String : public List<char>
	{
	public:
		typedef List<char> BaseType;

		explicit String(MemPool* mempool, size_t const& capacity = 0);

		template<size_t len>
		String(MemPool* mempool, char const(&s)[len]);
		template<size_t len>
		String(MemPool* mempool, wchar_t const(&ws)[len]);
		String(MemPool* mempool, char const* const& s);
		String(MemPool* mempool, char const* const& s, size_t const& len);
		String(MemPool* mempool, std::string const& s);
		String(MemPool* mempool, std::pair<char const*, int> const& str_len);


		String(String &&o);
		String(String const&o) = delete;
		String& operator=(String const&o) = delete;

		template<size_t len>
		void Assign(char const(&s)[len]);
		template<size_t len>
		void Assign(wchar_t const(&ws)[len]);
		void Assign(char const* const& s);
		void Assign(char const* const& s, size_t const& len);
		void Assign(wchar_t const* const& ws, size_t const& len);
		void Assign(String const& s);
		void Assign(Ptr<String> const& s);

		template<typename T>
		void Assign(T const& in);

		String& operator=(char const * const& buf);

		char* c_str();
		void AppendChars(char const& c = ' ', size_t const& num = 1);

		template<typename ...TS>
		void Append(TS const & ...vs);

	protected:

		template<typename T>
		void AppendFormatCore(String &s, size_t &n, T const &v);

		template<typename T, typename...TS>
		void AppendFormatCore(String &s, size_t &n, T const &v, TS const &...vs);

	public:

		// 格式化追加, {0} {1}... 这种. 针对重复出现的参数, 是从已经追加出来的字串区域复制, 故追加自己并不会导致内容翻倍
		template<typename...TS>
		void AppendFormat(char const* format, TS const&...vs);

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

		void ToLower() noexcept;
		void ToLowerFirstChar() noexcept;
		void ToUpper() noexcept;
		void ToUpperFirstChar() noexcept;

		template<size_t len>
		bool Equals(char const(&o)[len]) const noexcept;
		bool Equals(String const& o) const noexcept;
		bool Equals(Ptr<String> const& o) const noexcept;

		static bool Equals(Ptr<String> const& a, Ptr<String> const& b) noexcept;

	private:
		friend std::ostream& operator<<(std::ostream&, const String&);


	public:
		// 实现一些 Object 的接口

		String(BBuffer* bb);
		// WriteTo, ReadFrom 直接使用基类的

		void ToString(String &s) const override;
	};


	using String_p = Ptr<String>;

	using String_r = Ref<String>;
}
