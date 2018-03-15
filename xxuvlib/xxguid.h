#pragma once
#include <stdint.h>

class Guid
{
public:
	uint64_t part1;
	uint64_t part2;

	inline Guid() noexcept
		: part1(0)
		, part2(0)
	{
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

	inline bool IsZero() noexcept
	{
		return part1 == 0 && part2 == 0;
	}

	void Fill();
};








#ifdef _WIN32
#include <objbase.h>
inline void Guid::Fill()
{
	CoCreateGuid((GUID*)this);
}
#endif


#ifdef __APPLE__
#include <CoreFoundation/CFUUID.h>
inline void Guid::Fill()
{
	auto newId = CFUUIDCreate(NULL);
	*this = *(Guid*)&CFUUIDGetUUIDBytes(newId);
	CFRelease(newId);
}
#endif


#ifdef __linux__
#ifdef __ANDROID__

#include <random>
#include <chrono>
#include <limits>
#include <functional>
inline void Guid::Fill()
{
	// 将就用了. 不适合短时间内多次生成. 效率也成问题. 除非加上下文保存随机数. 
	// 进一步的, 为了不让 Guid 变为非 16 字节, 上下文似乎应该存为 static. 
	// 进一步的, 为了 thead safe, 该 static 要用线程安全标记包起来

	auto seed = std::chrono::system_clock::now().time_since_epoch().count();
	seed ^= std::random_device{}();
	std::mt19937 mt(seed);
	std::uniform_int_distribution<uint64_t> dist(0, std::numeric_limits<uint64_t>::max());
	auto dice = std::bind(dist, mt);
	part1 = dice();
	part2 = dice();
}
#else
#include <uuid/uuid.h>
inline void Guid::Fill()
{
	uuid_generate((uuid_t)this);
}
#endif
#endif

