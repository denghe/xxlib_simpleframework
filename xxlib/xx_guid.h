#pragma once

namespace xx
{
	class Guid
	{
	public:
		union
		{
			struct
			{
				uint64_t part1;
				uint64_t part2;
			};
			struct // for ToString
			{
				uint32_t  data1;
				unsigned short data2;
				unsigned short data3;
				unsigned char  data4[8];
			};
		};

		explicit Guid(bool const& fill = true) noexcept;
		Guid(Guid const& o) noexcept = default;
		Guid& operator=(Guid const& o) noexcept = default;
		bool operator==(Guid const& o) const noexcept;
		bool operator!=(Guid const& o) const noexcept;

		void Fill() noexcept;
		void Fill(char const* const& buf) noexcept;

		bool IsZero() noexcept;
	};
}
