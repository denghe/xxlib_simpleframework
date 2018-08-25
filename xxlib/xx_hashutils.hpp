#pragma once
namespace xx
{

	inline bool IsPrime(size_t const& candidate) noexcept
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

	inline int32_t GetPrime(int32_t const& capacity, int32_t const& dataSize) noexcept
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






	/**************************************************************************************************/
	// 类型--操作适配模板区
	/**************************************************************************************************/

	// 适配 4 字节整数或浮点
	template<typename T>
	struct HashFunc<T, std::enable_if_t<(std::is_integral_v<T> || std::is_floating_point_v<T>) && sizeof(T) == 4>>
	{
		static uint32_t GetHashCode(T const& in) noexcept
		{
			return *(uint32_t*)&in;
		}
	};

	// 适配 小于4字节的 整数
	template<typename T>
	struct HashFunc < T, std::enable_if_t<std::is_integral_v<T> && sizeof(T) < 4 > >
	{
		static uint32_t GetHashCode(T const& in) noexcept
		{
			return (uint32_t)(std::make_unsigned_t<T>)in;
		}
	};

	// 适配 8 字节整数或浮点
	template<typename T>
	struct HashFunc<T, std::enable_if_t<(std::is_integral_v<T> || std::is_floating_point_v<T>) && sizeof(T) == 8>>
	{
		static uint32_t GetHashCode(T const& in) noexcept
		{
			return ((uint32_t*)&in)[0] ^ ((uint32_t*)&in)[1];
		}
	};



	// 适配 void* 指针
	// 经验数据. 经测试发现 x64 下 vc new 至少是 16 的倍数, x86 至少是 8 的倍数
	template<>
	struct HashFunc<void*, void>
	{
		static uint32_t GetHashCode(void* const& in) noexcept
		{
			if constexpr(sizeof(in) == 4)
			{
				return (size_t)in >> 3;
			}
			else
			{
				return HashFunc<size_t>::GetHashCode((size_t)in >> 4);
			}
		}
	};

	// 适配一段 buf
	template<>
	struct HashFunc<std::pair<char*, size_t>, void>
	{
		// 抄自微软 .net 源代码
		// todo: 未来考虑是不是换成什么 xxhash 开源项目之类
		static uint32_t GetHashCode(std::pair<char*, size_t> const& in) noexcept
		{
			auto& buf = in.first;
			auto& len = in.second;

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
	};

	// 适配 char const* 字串
	template<>
	struct HashFunc<char const*, void>
	{
		static uint32_t GetHashCode(char const* const& in) noexcept
		{
			// come from zeno
			uint32_t seed = 0x811C9DC5U;
			if (!in) return seed;
			auto size = strlen(in);
			if (!size) return seed;
			const char* src = (const char*)in;
			const uint32_t fnv_prime = 0x1000193U;
			while (size--)
			{
				seed ^= ((uint32_t)(*src++));
				seed *= fnv_prime;
			}
			return seed;

			//auto len = strlen(in);
			//uint32_t hash, i;
			//for (hash = i = 0; i < len; ++i)
			//{
			//	hash += in[i];
			//	hash += (hash << 10);
			//	hash ^= (hash >> 6);
			//}
			//hash += (hash << 3);
			//hash ^= (hash >> 11);
			//hash += (hash << 15);
			//return hash;

			//return HashFunc<std::pair<char*, size_t>>::GetHashCode(std::make_pair((char*)in, strlen(in)));
		}
	};

	// 适配 std::string
	template<>
	struct HashFunc<std::string, void>
	{
		static uint32_t GetHashCode(std::string const& in) noexcept
		{
			return HashFunc<std::pair<char*, size_t>>::GetHashCode(std::make_pair((char*)in.data(), in.size()));
		}
	};





	// 比较器 适配 char const* 字串
	template<>
	struct EqualsFunc<char const*, void>
	{
		static bool EqualsTo(char const* const& a, char const* const& b) noexcept
		{
			return !strcmp(a, b);
		}
	};
}
