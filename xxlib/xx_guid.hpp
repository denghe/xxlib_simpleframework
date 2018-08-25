#pragma once

#ifdef __ANDROID_NDK__
extern void uuid_generate(unsigned char* buf);
#else
#ifndef _WIN32
#include <uuid/uuid.h>
#endif
#endif

namespace xx
{
    inline Guid::Guid(bool const& fill) noexcept
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

	inline void Guid::Fill(char const* const& buf) noexcept
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

	inline void Guid::Fill() noexcept
	{
#ifdef _WIN32
		CoCreateGuid((GUID*)this);
#else
		uuid_generate((unsigned char*)this);
#endif
	}



	template<>
	struct StrFunc<Guid, void>
	{
		static inline void WriteTo(String& s, Guid const& in) noexcept
		{
			s.Reserve(s.dataLen + 48);
			s.dataLen += snprintf(
				s.buf + s.dataLen,
				48,
				"%08X-%04X-%04X-%02X%02X-%02X%02X%02X%02X%02X%02X",
				in.data1, in.data2, in.data3,
				in.data4[0], in.data4[1],
				in.data4[2], in.data4[3],
				in.data4[4], in.data4[5],
				in.data4[6], in.data4[7]);
		}
	};

	template<>
	struct HashFunc<Guid, void>
	{
		static uint32_t GetHashCode(Guid const& in) noexcept
		{
			return ((uint32_t*)&in)[0] ^ ((uint32_t*)&in)[1] ^ ((uint32_t*)&in)[2] ^ ((uint32_t*)&in)[3];
		}
	};

	// 适配 Guid 之 序列化 & 反序列化
	template<>
	struct BytesFunc<Guid, void>
	{
		static inline void WriteTo(BBuffer& bb, Guid const& in) noexcept
		{
			bb.Reserve(bb.dataLen + sizeof(Guid));
			memcpy(bb.buf + bb.dataLen, &in, sizeof(Guid));
			bb.dataLen += sizeof(Guid);
		}
		static inline int ReadFrom(BBuffer& bb, Guid& out) noexcept
		{
			if (bb.offset + sizeof(Guid) > bb.dataLen) return -1;
			memcpy(&out, bb.buf + bb.offset, sizeof(Guid));
			bb.offset += sizeof(Guid);
			return 0;
		}
	};

}
