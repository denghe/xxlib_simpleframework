#pragma once
#include <stdio.h>

namespace xx
{
	class Guid
	{
	public:
		union
		{
			GUID guid;
			struct
			{
				uint64_t part1;
				uint64_t part2;
			};
		};

		explicit inline Guid(bool fill = true) noexcept
		{
			if (fill)
			{
				Fill();
			}
			else
			{
				part1 = 0;
				part2 = 0;
			}
		}

		inline void Fill()
		{
#ifdef _WIN32
			CoCreateGuid(&guid);
#else
			uuid_generate(reinterpret_cast<unsigned char *>(&guid));
#endif
		}

		inline void Fill(char const* const& buf)
		{
			memcpy(&guid, buf, 16);
		}

		inline bool IsZero() noexcept
		{
			return part1 == 0 && part2 == 0;
		}

		inline Guid(Guid const& o) noexcept
		{
			part1 = o.part1;
			part2 = o.part2;
		}
		inline Guid& operator=(Guid const& o) noexcept
		{
			part1 = o.part1;
			part2 = o.part2;
			return *this;
		}

		inline bool operator==(Guid const& o) const noexcept
		{
			return part1 == o.part1 && part2 == o.part2;
		}
		inline bool operator!=(Guid const& o) const noexcept
		{
			return part1 != o.part1 || part2 != o.part2;
		}

		inline void ToString(char* buf, size_t bufLen) noexcept
		{
			snprintf(
				buf,
				bufLen,
				"%08X-%04X-%04X-%02X%02X-%02X%02X%02X%02X%02X%02X",
				guid.Data1, guid.Data2, guid.Data3,
				guid.Data4[0], guid.Data4[1],
				guid.Data4[2], guid.Data4[3],
				guid.Data4[4], guid.Data4[5],
				guid.Data4[6], guid.Data4[7]);
		}
	};

	// todo: ToString 模板适配

	template<>
	inline uint32_t GetHashCode(Guid const &in) noexcept
	{
		return GetHashCode(in.part1 ^ in.part2);
	}
}
