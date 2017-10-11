#pragma once
#include <cassert>
#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <array>
#include <algorithm>
#ifdef _WIN32
#include <intrin.h>     // _BitScanReverse  64
#endif

/*

sample:

Lua_MemPool lmp;
auto L = lua_newstate([](void *ud, void *ptr, size_t osize, size_t nsize)
{
	return ((Lua_MemPool*)ud)->Realloc(ptr, nsize, osize);
}, &lmp);

*/

struct Lua_MemPool
{
	static_assert(sizeof(size_t) <= sizeof(void*), "");
	std::array<void*, sizeof(void*) * 8> headers;

	Lua_MemPool()
	{
		headers.fill(nullptr);
	}

	~Lua_MemPool()
	{
		for (auto header : headers)
		{
			while (header)
			{
				auto next = *(void**)header;
				free(header);
				header = next;
			}
		}
	}

	inline void* Alloc(size_t siz)
	{
		assert(siz);
		siz += sizeof(void*);
		auto idx = Calc2n(siz);
		if (siz > (size_t(1) << idx)) siz = size_t(1) << ++idx;

		auto& p = headers[idx];
		if (p) p = *(void**)p;
		else p = malloc(siz);

		*(size_t*)p = idx;
		return (void**)p + 1;
	}

	inline void Free(void* p)
	{
		if (!p) return;
		p = *((void**)p - 1);
		auto& h = headers[*(size_t*)p];
		*(void**)p = h;
		h = p;
	}

	inline void* Realloc(void *p, size_t newSize, size_t dataLen = -1)
	{
		if (!newSize)
		{
			Free(p);
			return nullptr;
		}
		if (!p) return Alloc(newSize);

		auto originalSize = (size_t(1) << *(size_t*)((void**)p - 1)) - sizeof(void*);
		if (originalSize >= newSize) return p;

		auto np = Alloc(newSize);
		std::memcpy(np, p, std::min(originalSize, dataLen));
		Free(p);
		return np;
	}

	inline static size_t Calc2n(size_t n)
	{
		assert(n);
#ifdef _MSC_VER
		unsigned long r = 0;
#if defined(_WIN64) || defined(_M_X64)
		_BitScanReverse64(&r, n);
# else
		_BitScanReverse(&r, n);
# endif
		return (size_t)r;
#else
#if defined(__LP64__) || __WORDSIZE == 64
		return int(63 - __builtin_clzl(n));
# else
		return int(32 - __builtin_clz(n));
# endif
#endif
	}
};
