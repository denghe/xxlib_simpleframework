#include "test.h"

#ifdef _WIN32

BOOL APIENTRY DllMain(HMODULE hModule,
	DWORD  ul_reason_for_call,
	LPVOID lpReserved
)
{
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
	case DLL_THREAD_ATTACH:
	case DLL_THREAD_DETACH:
	case DLL_PROCESS_DETACH:
		break;
	}
	return TRUE;
}
#else
#define sprintf_s snprintf
#endif

#include "xx.h"

LIBTEST1_API xx::MemPool* create_mp() noexcept
{
	return new (std::nothrow) xx::MemPool();
}
LIBTEST1_API void release_mp(xx::MemPool* mp) noexcept
{
	delete mp;
}
LIBTEST1_API void* mp_alloc(xx::MemPool* mp, size_t len) noexcept
{
	return mp->Alloc(len);
}
LIBTEST1_API void mp_free(xx::MemPool* mp, void* buf) noexcept
{
	mp->Free(buf);
}
LIBTEST1_API void* mp_resize(xx::MemPool* mp, void* buf, size_t newBuflen, size_t dataLen) noexcept
{
	return mp->Realloc(buf, newBuflen, dataLen);
}

LIBTEST1_API void xxmemcpy(void* src, void* dst, size_t dataLen) noexcept
{
	::memcpy(src, dst, dataLen);
}
