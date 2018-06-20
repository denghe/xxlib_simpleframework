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

// 当 IOS 最低版本兼容参数低于 11 时无法启用 C++17, 故启用 C++14 结合下面的各种造假来解决
#ifdef __IPHONE_OS_VERSION_MIN_REQUIRED
#if __IPHONE_OS_VERSION_MIN_REQUIRED < 110000
namespace std
{
	template<class B, class D>
    inline constexpr bool is_base_of_v = is_base_of<B, D>::value;
	template<class T>
	inline constexpr bool is_arithmetic_v = is_arithmetic<T>::value;
	template<class T>
	inline constexpr bool is_floating_point_v = is_floating_point<T>::value;
	template<class T>
	inline constexpr bool is_integral_v = is_integral<T>::value;
	template<class T>
	inline constexpr bool is_unsigned_v = is_unsigned<T>::value;
	template<class T>
	inline constexpr bool is_enum_v = is_enum<T>::value;
	template<class T1, class T2>
	inline constexpr bool is_same_v = is_same<T1, T2>::value;

	template<class MutexType>
	class scoped_lock
	{
	public:
		explicit scoped_lock(MutexType& m) : m(m) { m.lock(); }
		~scoped_lock() { m.unlock(); }
		scoped_lock(const scoped_lock&) = delete;
		scoped_lock& operator=(const scoped_lock&) = delete;
	private:
		MutexType& m;
	};
}
#endif
#endif

#ifdef _WIN32
#include <intrin.h>     // _BitScanReverse  64
#include <objbase.h>
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



namespace xx
{

	/***********************************************************************************/
	// Stopwatch
	/***********************************************************************************/

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


	/************************************************************************************/
	// time_point <--> .net DateTime.Now.Ticks converts
	/************************************************************************************/

	/*
	
	// some example for output:

	#include <ctime>
	#include <iomanip>

	std::time_t t = std::time(nullptr);
	std::tm tm;
	localtime_s(&tm, &t);
	std::cout << std::put_time(&tm, "%Y-%m-%d %X") << std::endl;

	auto tp = xx::DateTimeTicksToTimePoint(636650305817699126);
	t = std::chrono::system_clock::to_time_t(tp);
	localtime_s(&tm, &t);
	std::cout << std::put_time(&tm, "%Y-%m-%d %X") << std::endl;

	*/


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

	inline int64_t GetDateTimeNowTicks()
	{
		return GetNowTimeSinceEpoch() + 621356256000000000LL;
	}

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
#include "xx_random.h"

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
#include "xx_random.hpp"
