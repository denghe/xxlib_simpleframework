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

struct XxMemPool
{
	static_assert(sizeof(size_t) <= sizeof(void*), "");
	std::array<void*, sizeof(void*) * 8> headers;

	XxMemPool()
	{
		headers.fill(nullptr);
	}

	~XxMemPool()
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

		auto p = headers[idx];
		if (p) headers[idx] = *(void**)p;
		else p = malloc(siz);
		if (!p) return nullptr;
		*(size_t*)p = idx;
		return (void**)p + 1;
	}

	inline void Free(void* p)
	{
		if (!p) return;
		p = (void**)p - 1;
		auto idx = *(size_t*)p;
		*(void**)p = headers[idx];
		headers[idx] = p;
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
		memcpy(np, p, std::min(originalSize, dataLen));
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
		return int(31 - __builtin_clz(n));
# endif
#endif
	}

	inline static size_t Round2n(size_t n)
	{
		auto rtv = size_t(1) << Calc2n(n);
		if (rtv == n) return n;
		else return rtv << 1;
	}


	template<typename T>
	T* Alloc()
	{
		static_assert(std::is_pod_v<T>);
		return (T*)Alloc(sizeof(T));
	}

	template<typename T, typename ...Args>
	T* Create(Args &&... args)
	{
		auto p = (void**)Alloc(sizeof(T) + sizeof(void*));
		*p = this;
		try
		{
			return new (p + 1) T(std::forward<Args>(args)...);
		}
		catch (...)
		{
			Free(p);
			throw -1;
		}
	}

	template<typename T>
	void Release(T* p)
	{
		p->~T();
		Free((void**)p - 1);
	}

	template<typename T>
	void SafeRelease(T*& p)
	{
		Release(p);
		p = nullptr;
	}

	inline static XxMemPool& Get(void* thiz)
	{
		return *(XxMemPool*)*((void**)thiz - 1);
	}
};
