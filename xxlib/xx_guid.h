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
				unsigned long  data1;
				unsigned short data2;
				unsigned short data3;
				unsigned char  data4[8];
			};
		};

		explicit inline Guid(bool fill = true) noexcept;
		Guid(Guid const& o) noexcept = default;
		Guid& operator=(Guid const& o) noexcept = default;
		bool operator==(Guid const& o) const noexcept;
		bool operator!=(Guid const& o) const noexcept;

		void Fill();
		void Fill(char const* const& buf);

		bool IsZero() noexcept;
	};
}
