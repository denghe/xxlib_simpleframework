#pragma once

#ifdef _WIN32
#include <SDKDDKVer.h>
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#ifdef libtest1_EXPORTS
#define LIBTEST1_API __declspec(dllexport)
#else
#define LIBTEST1_API __declspec(dllimport)
#endif
#else
#define LIBTEST1_API
#endif

namespace xx
{
	class MemPool;
}

#ifdef __cplusplus
extern "C" {
#endif

	LIBTEST1_API xx::MemPool* create_mp() noexcept;
	LIBTEST1_API void release_mp(xx::MemPool* mp) noexcept;
	LIBTEST1_API void* mp_alloc(xx::MemPool* mp, size_t len) noexcept;
	LIBTEST1_API void mp_free(xx::MemPool* mp, void* buf) noexcept;
	LIBTEST1_API void* mp_resize(xx::MemPool* mp, void* buf, size_t newBuflen, size_t dataLen) noexcept;
	LIBTEST1_API void xxmemcpy(void* src, void* dst, size_t dataLen) noexcept;

#ifdef __cplusplus
}
#endif
