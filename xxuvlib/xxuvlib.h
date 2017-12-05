#pragma once

#ifdef _WIN32
	#include <SDKDDKVer.h>
	#define WIN32_LEAN_AND_MEAN
	#include <windows.h>
	#ifdef xxuvlib_EXPORTS
	#define XXUVLIB_API __declspec(dllexport)
	#else
	#define XXUVLIB_API __declspec(dllimport)
	#endif
#else
	#define XXUVLIB_API
#endif

#ifdef __cplusplus
extern "C" {
#endif

	XXUVLIB_API void* xxuvNew();

	XXUVLIB_API void xxuvDelete();

#ifdef __cplusplus
}
#endif
