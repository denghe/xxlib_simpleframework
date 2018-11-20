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
#include <experimental/optional>
namespace std
{
	template<typename T>
	using optional = std::experimental::optional<T>;
}
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
	template<class T>
	inline constexpr bool is_pointer_v = is_pointer<T>::value;
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
#else
#include <optional>
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
size_t _countof_helper(T const (&arr)[N])
{
	return N;
}
#define _countof(_Array) _countof_helper(_Array)
#endif

#ifndef _offsetof
#define _offsetof(s,m) ((size_t)&reinterpret_cast<char const volatile&>((((s*)0)->m)))
#endif

#ifndef container_of
#define container_of(ptr, type, member) \
  ((type *) ((char *) (ptr) - _offsetof(type, member)))
#endif


#if defined _MSC_VER
	#define XX_SSCANF sscanf_s;
#else
	#define XX_SSCANF sscanf;
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



#include "fixed_function.hpp"

namespace xx
{

	/***********************************************************************************/
	// Stopwatch
	/***********************************************************************************/

	struct Stopwatch
	{
		Stopwatch() { Reset(); }
		inline void Reset() noexcept { beginTime = std::chrono::high_resolution_clock::now(); }
		inline int64_t operator()() noexcept
		{
			auto bak = beginTime;
			Reset();
			return std::chrono::duration_cast<std::chrono::milliseconds>(beginTime - bak).count();
		}
		inline int64_t micros() noexcept
		{
			auto bak = beginTime;
			Reset();
			return std::chrono::duration_cast<std::chrono::microseconds>(beginTime - bak).count();
		}
	private:
		std::chrono::time_point<std::chrono::high_resolution_clock> beginTime;
	};



	/***********************************************************************************/
	// ScopeGuard
	/***********************************************************************************/

	struct ScopeGuard
	{
		template<typename T>
		ScopeGuard(T&& f) : func(std::forward<T>(f)) {}
		~ScopeGuard() { Run(); }
		inline void RunAndCancel() noexcept { Run(); Cancel(); }
		inline void Run() noexcept { if (func) func(); }
		inline void Cancel() noexcept { func = nullptr; }
		template<typename T>
		inline void Set(T&& f) noexcept { func = std::forward<T>(f); }
	private:
		kapala::fixed_function<void()> func;
		ScopeGuard(ScopeGuard const&) = delete;
		ScopeGuard &operator=(ScopeGuard const&) = delete;
	};




	/***********************************************************************************/
	// RtvChecker
	/***********************************************************************************/

	template<typename T>
	class Weak;
	template<typename T>
	class Ref;

	struct RtvChecker
	{
		template<typename T>
		void operator=(Weak<T> const& rtv)
		{
			assert(rtv);
		}

		template<typename T>
		void operator=(Ref<T> const& rtv)
		{
			assert(rtv);
		}

		void operator=(int const& rtv)
		{
			assert(!rtv);
		}

		void operator=(void* const& rtv)
		{
			assert(rtv);
		}
	};




	/************************************************************************************/
	// time_point <--> .net DateTime.Now.ToUniversalTime().Ticks converts
	/************************************************************************************/

	/*

	// some example for output:

	#include <ctime>
	#include <iomanip>

	std::time_t t = std::time(nullptr);
	std::tm tm;
	localtime_s(&tm, &t);
	std::cout << std::put_time(&tm, "%Y-%m-%d %X") << std::endl;

	auto tp = xx::Epoch10mToTimePoint(15xxxxxxxxxxxxxxxxxxx);
	t = std::chrono::system_clock::to_time_t(tp);
	localtime_s(&tm, &t);
	std::cout << std::put_time(&tm, "%Y-%m-%d %X") << std::endl;

	*/

	// 经历时间精度: 秒后 7 个 0( 这是 windows 下最高精度. android/ios 会低1个0的精度 )
	typedef std::chrono::duration<long long, std::ratio<1LL, 10000000LL>> duration_10m;

	// 时间点 转 epoch (精度为秒后 7 个 0)
	inline int64_t TimePointToEpoch10m(std::chrono::system_clock::time_point const& val) noexcept
	{
		return std::chrono::duration_cast<duration_10m>(val.time_since_epoch()).count();
	}

	//  epoch (精度为秒后 7 个 0) 转 时间点
	inline std::chrono::system_clock::time_point Epoch10mToTimePoint(int64_t const& val) noexcept
	{
		return std::chrono::system_clock::time_point(std::chrono::duration_cast<std::chrono::system_clock::duration>(duration_10m(val)));
	}


	// 得到当前时间点
	inline std::chrono::system_clock::time_point NowTimePoint() noexcept
	{
		return std::chrono::system_clock::now();
	}

	// 得到当前时间点的 epoch (精度为秒后 7 个 0)
	inline int64_t NowEpoch10m() noexcept
	{
		return TimePointToEpoch10m(NowTimePoint());
	}


	// epoch (精度为秒后 7 个 0) 转为 .Net DateTime Utc Ticks
	inline int64_t Epoch10mToUtcDateTimeTicks(int64_t const& val) noexcept
	{
		return val + 621355968000000000LL;
	}

	// .Net DateTime Utc Ticks 转为 epoch (精度为秒后 7 个 0)
	inline int64_t UtcDateTimeTicksToEpoch10m(int64_t const& val) noexcept
	{
		return val - 621355968000000000LL;
	}


	// 时间点 转 epoch (精度为秒)
	inline int32_t TimePointToEpoch(std::chrono::system_clock::time_point const& val) noexcept
	{
		return (int32_t)(val.time_since_epoch().count() / 10000000);
	}

	//  epoch (精度为秒) 转 时间点
	inline std::chrono::system_clock::time_point EpochToTimePoint(int32_t const& val) noexcept
	{
		return std::chrono::system_clock::time_point(std::chrono::system_clock::time_point::duration((int64_t)val * 10000000));
	}



	/************************************************************************************/
	// Windows Console utf8 output support
	/************************************************************************************/

	/*
#ifdef _WIN32
	// 控制台显示乱码纠正, 设置字符集  system("chcp 65001");
	SetConsoleOutputCP(65001);
	CONSOLE_FONT_INFOEX info = { 0 };
	// 以下设置字体来支持中文显示。  
	info.cbSize = sizeof(info);
	info.dwFontSize.Y = 18;
	info.dwFontSize.X = 10;
	info.FontWeight = FW_NORMAL;
	wcscpy(info.FaceName, L"新宋体");
	SetCurrentConsoleFontEx(GetStdHandle(STD_OUTPUT_HANDLE), NULL, &info);
#endif
	*/
}


#include "xx_mempool.h"
#include "xx_list.h"
#include "xx_queue.h"
#include "xx_hashutils.h"
#include "xx_dict.h"
#include "xx_dictex.h"
#include "xx_charsutils.h"
#include "xx_string.h"
#include "xx_bytesutils.h"
#include "xx_bbuffer.h"
#include "xx_guid.h"
#include "xx_random.h"
#include "xx_hashset.h"

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
#include "xx_hashset.hpp"
