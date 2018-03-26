#pragma once
#include <cassert>
#include <cstdio>
#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <cmath>
#include <functional>
#include <array>
#include <algorithm>
#include <limits>
#include <type_traits>
#include <string>
#include <iostream>
#include <chrono>

#ifdef _WIN32
#include <intrin.h>     // _BitScanReverse  64
#include <objbase.h>
#else
#include <uuid/uuid.h>
typedef struct _GUID {
	unsigned int   Data1;
	unsigned short Data2;
	unsigned short Data3;
	unsigned char  Data4[8];
} GUID;
#endif

#ifdef min
#undef min
#endif

#ifdef max
#undef max
#endif

#ifndef _countof
template<typename T, size_t N>
size_t _countof(T const (&arr)[N])
{
	return N;
}
#endif

#ifndef _offsetof
#define _offsetof(s,m) ((size_t)&reinterpret_cast<char const volatile&>((((s*)0)->m)))
#endif

#ifndef container_of
#define container_of(ptr, type, member) \
  ((type *) ((char *) (ptr) - _offsetof(type, member)))
#endif



/***********************************************************************************/
// CORO_ BEGIN END YIELDTO GOTO
/***********************************************************************************/
// need define lineNumber var 

#define XX_CORO_BEGIN()		\
switch (lineNumber)			\
{							\
case 0:						\
Label0:						\
{

#define XX_CORO_(n)			\
}							\
lineNumber = n;				\
return 0;					\
case n:						\
Label##n:					\
{

#define XX_CORO_END()		\
}							\
}

#define XX_CORO_YIELDTO(n)	\
{							\
	lineNumber = n;			\
	return 0;				\
}

#define XX_CORO_GOTO(n)		\
goto Label##n



/***********************************************************************************/
// Stopwatch
/***********************************************************************************/

namespace xx
{
	struct Stopwatch
	{
		Stopwatch() { Reset(); }
		inline void Reset() { beginTime = std::chrono::high_resolution_clock::now(); }
		inline int64_t operator()()
		{
			auto bak = beginTime;
			Reset();
			return std::chrono::duration_cast<std::chrono::milliseconds>(beginTime - bak).count();
		}
		inline int64_t micros()
		{
			auto bak = beginTime;
			Reset();
			return std::chrono::duration_cast<std::chrono::microseconds>(beginTime - bak).count();
		}
	private:
		std::chrono::time_point<std::chrono::high_resolution_clock> beginTime;
	};
}


/************************************************************************************/
// time_point <--> DateTime.Ticks converts
/************************************************************************************/

inline int64_t TimeSinceEpochToDateTimeTicks(int64_t const& val)
{
	return val + 621356256000000000LL;
}

inline int64_t TimePointToDateTimeTicks(std::chrono::system_clock::time_point const& val)
{
	return val.time_since_epoch().count() + 621356256000000000LL;
}

inline int64_t DateTimeTicksToTimeSinceEpoch(int64_t const& val)
{
	return val - 621356256000000000LL;
}

inline std::chrono::system_clock::time_point TimeSinceEpochToTimePoint(int64_t const& val)
{
	return std::chrono::system_clock::time_point(std::chrono::system_clock::time_point::duration(val));
}

inline std::chrono::system_clock::time_point DateTimeTicksToTimePoint(int64_t const& val)
{
	return TimeSinceEpochToTimePoint(val - 621356256000000000LL);
}

inline std::chrono::system_clock::time_point GetNowTimePoint()
{
	return std::chrono::system_clock::now();
}

inline int64_t GetNowTimeSinceEpoch()
{
	return GetNowTimePoint().time_since_epoch().count();
}

inline int64_t GetNowDateTimeTicks()
{
	return GetNowTimeSinceEpoch() + 621356256000000000LL;
}



#include "xx_mempool.h"
#include "xx_list.h"
#include "xx_queue.h"
#include "xx_hashutils.h"
#include "xx_dict.h"
#include "xx_charsutils.h"
#include "xx_string.h"
#include "xx_bytesutils.h"
#include "xx_bbuffer.h"
#include "xx_guid.h"

#include "xx_mempool.hpp"
#include "xx_list.hpp"
#include "xx_queue.hpp"
#include "xx_hashutils.hpp"
#include "xx_dict.hpp"
#include "xx_charsutils.hpp"
#include "xx_string.hpp"
#include "xx_bytesutils.hpp"
#include "xx_bbuffer.hpp"
#include "xx_guid.hpp"
