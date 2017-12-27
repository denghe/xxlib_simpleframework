#pragma once
namespace xx
{
	// 抄自微软 .net 源代码
	// todo: 未来考虑是不是换成什么 xxhash 开源项目之类

	// 比较器
	template<typename T>
	bool EqualsTo(T const& a, T const& b) noexcept
	{
		return a == b;
	}

	inline uint32_t GetHashCode(bool in) noexcept { return in ? 1 : 0; }
	inline uint32_t GetHashCode(char in) noexcept { return (uint32_t)in; }
	inline uint32_t GetHashCode(uint8_t in) noexcept { return (uint32_t)in; }
	inline uint32_t GetHashCode(int8_t in) noexcept { return (uint8_t)in; }
	inline uint32_t GetHashCode(uint16_t in) noexcept { return (uint32_t)in; }
	inline uint32_t GetHashCode(int16_t in) noexcept { return (uint16_t)in; }
	inline uint32_t GetHashCode(int32_t in) noexcept { return (uint32_t)in; }
	inline uint32_t GetHashCode(uint32_t in) noexcept { return in; }
	inline uint32_t GetHashCode(uint64_t in) noexcept { return (in & 0xFFFFFFFFu) ^ (in >> 32); }
	inline uint32_t GetHashCode(int64_t in) noexcept { return GetHashCode((uint64_t)in); }
	inline uint32_t GetHashCode(float in) noexcept { return *(uint32_t*)(float*)&in; }
	inline uint32_t GetHashCode(double in) noexcept { return GetHashCode(*(uint64_t*)(double*)&in); }
	// 经验数据. 经测试发现 x64 下 vc new 至少是 16 的倍数, x86 至少是 8 的倍数
	inline uint32_t GetHashCode(void* in) noexcept { return (uint32_t)((size_t)(void*)in / (sizeof(size_t) * 2)); }

	inline uint32_t GetHashCode(uint8_t const *buf, size_t len) noexcept
	{
		assert((size_t)buf % 4 == 0);		// buf 要求 4 字节对齐
		if (!len)
		{
			return 0;
		}
		int32_t n2 = 0x15051505, n1 = 0x15051505;
		uint32_t mod = len % 8, i = 0;
		for (; i < len - mod; i += 8)
		{
			n1 = (((n1 << 5) + n1) + (n1 >> 0x1b)) ^ *(int32_t*)(buf + i);
			n2 = (((n2 << 5) + n2) + (n2 >> 0x1b)) ^ *(int32_t*)(buf + i + 4);
		}
		if (mod > 4)
		{
			n1 = (((n1 << 5) + n1) + (n1 >> 0x1b)) ^ *(int32_t*)(buf + i);
			n2 = (((n2 << 5) + n2) + (n2 >> 0x1b)) ^ (*(int32_t*)(buf + i + 4) & (0xFFFFFFFF >> ((8 - mod) * 8)));
		}
		else if (mod)
		{
			n1 = (((n1 << 5) + n1) + (n1 >> 0x1b)) ^ (*(int32_t*)(buf + i) & (0xFFFFFFFF >> ((4 - mod) * 8)));
		}
		return n2 + n1 * 0x5d588b65;
	}

	template<typename T>
	uint32_t GetHashCode(T *in) noexcept { return GetHashCode(*in); };

	template<typename T>
	uint32_t GetHashCode(T const &in) noexcept { return GetHashCode((uint8_t*)&in, sizeof(in)); };



	inline bool IsPrime(size_t candidate) noexcept
	{
		if ((candidate & 1) != 0)
		{
			size_t limit = size_t(std::sqrt(candidate));
			for (size_t divisor = 3; divisor <= limit; divisor += 2)
			{
				if ((candidate % divisor) == 0)
				{
					return false;
				}
			}
			return true;
		}
		return (candidate == 2);
	}

	inline int32_t GetPrime(int32_t capacity, int32_t dataSize) noexcept
	{
		auto memUsage = MemPool::Round2n(capacity * dataSize);		// 算内存实际用量
		auto maxCapacity = memUsage / dataSize;                     // 内存实际用量下 能放多少个 data

		if (maxCapacity == capacity)                                // if maxCapacity 为 2^N, 查表
		{
			return primes2n[MemPool::Calc2n(capacity)];
		}
		if (dataSize >= 8 && dataSize <= 512)                       // 数据长在 查表 范围内的
		{
			return *std::upper_bound(std::begin(primes), std::end(primes), (int32_t)maxCapacity);
		}
		for (size_t i = maxCapacity + 1; i <= 0x7fffffff; i += 2)   // maxCapacity 是双数. +1 即为单数
		{
			if (IsPrime(i)) return (int32_t)i;
		}
		assert(false);
		return -1;
	}
}
