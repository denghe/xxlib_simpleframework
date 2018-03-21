#pragma once

namespace xx
{

	inline Guid::Guid(bool fill) noexcept
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

	inline void Guid::Fill(char const* const& buf)
	{
		memcpy(this, buf, 16);
	}

	inline bool Guid::IsZero() noexcept
	{
		return part1 == 0 && part2 == 0;
	}

	inline bool Guid::operator==(Guid const& o) const noexcept
	{
		return part1 == o.part1 && part2 == o.part2;
	}
	inline bool Guid::operator!=(Guid const& o) const noexcept
	{
		return part1 != o.part1 || part2 != o.part2;
	}

	inline void Guid::Fill()
	{
#ifdef _WIN32
		CoCreateGuid((GUID*)this);
#else
		uuid_generate(reinterpret_cast<unsigned char *>this);
#endif
	}



	template<>
	struct StrFunc<Guid, void>
	{
		static inline void WriteTo(String& s, Guid const& in)
		{
			s.Reserve(s.dataLen + 48);
			snprintf(
				s.buf + s.dataLen,
				48,
				"%08X-%04X-%04X-%02X%02X-%02X%02X%02X%02X%02X%02X",
				in.data1, in.data2, in.data3,
				in.data4[0], in.data4[1],
				in.data4[2], in.data4[3],
				in.data4[4], in.data4[5],
				in.data4[6], in.data4[7]);
			s.dataLen += 48;
		}
	};

	template<>
	struct HashFunc<Guid, void>
	{
		static uint32_t GetHashCode(Guid const& in)
		{
			return ((uint32_t*)&in)[0] ^ ((uint32_t*)&in)[1] ^ ((uint32_t*)&in)[2] ^ ((uint32_t*)&in)[3];
		}
	};

}
