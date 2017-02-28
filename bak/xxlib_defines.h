#pragma once
#pragma execution_character_set("utf-8")
#pragma warning(disable:4819)

#include <cstdint>
#include <functional>
#include <chrono>
#ifdef _WIN32
#include <intrin.h>     // _BitScanReverse  64
#endif
#include <cassert>

/*
XXLIB_IA
XXLIB_64BIT
XXLIB_ALIGN2/4/8/16/32/64
XXLIB_LIKELY / UNLIKELY / INLINE / NOINLINE
MIN / MAX
Sleep
_countof
XXLIB_ENUM_OPERATOR_EXT
XXLIB_HAS_TYPEDEF
TypeDeclare
Calc2n
Round2n
HasParms
MemmoveSupport
MaxSizeof
Stopwatch
ScopeGuard
BufAndDataLen
EqualsTo
TypeId
Singleton
*/



/************************************************************************/
/* XXLIB_IA                                                             */
/************************************************************************/

#if defined(__i386__) || defined(i386) || defined(_M_IX86) || defined(_X86_) || defined(__THW_INTEL) || defined(__x86_64__) || defined(_M_X64)
#    define XXLIB_IA
#endif


/************************************************************************/
/* XXLIB_64BIT                                                          */
/************************************************************************/

#if defined(__LP64__) || __WORDSIZE == 64 || defined(_WIN64) || defined(_M_X64)
#    define XXLIB_64BIT
#endif


/************************************************************************/
/* XX_ALIGN2/4/8/16/32/64                                               */
/************************************************************************/

#ifdef _MSC_VER
#    define XXLIB_ALIGN2( x )		    __declspec(align(2)) x
#    define XXLIB_ALIGN4( x )		    __declspec(align(4)) x
#    define XXLIB_ALIGN8( x )		    __declspec(align(8)) x
#    define XXLIB_ALIGN16( x )		    __declspec(align(16)) x
#    define XXLIB_ALIGN32( x )		    __declspec(align(32)) x
#    define XXLIB_ALIGN64( x )		    __declspec(align(64)) x
#else
#    define XXLIB_ALIGN2( x )          x __attribute__ ((aligned (2)))
#    define XXLIB_ALIGN4( x )          x __attribute__ ((aligned (4)))
#    define XXLIB_ALIGN8( x )          x __attribute__ ((aligned (8)))
#    define XXLIB_ALIGN16( x )         x __attribute__ ((aligned (16)))
#    define XXLIB_ALIGN32( x )         x __attribute__ ((aligned (32)))
#    define XXLIB_ALIGN64( x )         x __attribute__ ((aligned (64)))
#endif



/************************************************************************/
/* INLINE / NOINLINE / LIKELY / UNLIKELY                                */
/************************************************************************/

#ifdef _MSC_VER
#    define XXLIB_INLINE                    __forceinline
#    define XXLIB_NOINLINE                  __declspec(noinline)
#    define XX_LIKELY(x)                    (x)
#    define XX_UNLIKELY(x)                  (x)
#else
#    define XXLIB_INLINE                    inline __attribute__((__always_inline__))
#    define XXLIB_NOINLINE                  __attribute__((__noinline__))
#    define XX_UNLIKELY(x)                  __builtin_expect((x), 0)
#    define XX_LIKELY(x)                    __builtin_expect((x), 1)
#endif




/************************************************************************/
/* MIN / MAX                                                            */
/************************************************************************/

#ifndef MIN
#    define MIN( a, b )             ( (a) < (b) ? (a) : (b) )
#endif

#ifndef MAX
#    define MAX( a, b )             ( (a) > (b) ? (a) : (b) )
#endif



/************************************************************************/
/* Sleep                                                                */
/************************************************************************/

// this_thread sleep_for will be create a new thread for sleep, so....
// btw: windows need include windows.h
#ifdef _WIN32
#else
#    ifndef Sleep
#        define Sleep( x )          usleep( ( x ) * 1000 )
#    endif
#endif 


/************************************************************************/
/* _countof                                                             */
/************************************************************************/

// _countof
#ifndef _countof
template<typename T, size_t N>
size_t _countof(T const (&arr)[N])
{
	return N;
}
#endif



/************************************************************************/
/* XXLIB_ENUM_OPERATOR_EXT                                              */
/************************************************************************/

#define XXLIB_ENUM_OPERATOR_EXT( EnumTypeName )                                                                                                 \
    inline EnumTypeName operator+(EnumTypeName const &a, typename std::underlying_type<EnumTypeName>::type const &b)                            \
    {                                                                                                                                           \
        return EnumTypeName((typename std::underlying_type<EnumTypeName>::type)(a) + b);                                                        \
    }                                                                                                                                           \
    inline EnumTypeName operator+(EnumTypeName const &a, EnumTypeName const &b)                                                                 \
    {                                                                                                                                           \
        return EnumTypeName((typename std::underlying_type<EnumTypeName>::type)(a) + (typename std::underlying_type<EnumTypeName>::type)(b));   \
    }                                                                                                                                           \
    inline EnumTypeName operator-(EnumTypeName const &a, typename std::underlying_type<EnumTypeName>::type const &b)                            \
    {                                                                                                                                           \
        return EnumTypeName((typename std::underlying_type<EnumTypeName>::type)(a) - b);                                                        \
    }                                                                                                                                           \
    inline typename std::underlying_type<EnumTypeName>::type operator-(EnumTypeName const &a, EnumTypeName const &b)                            \
    {                                                                                                                                           \
        return (typename std::underlying_type<EnumTypeName>::type)(a) - (typename std::underlying_type<EnumTypeName>::type)(b);                 \
    }                                                                                                                                           \
    inline EnumTypeName operator++(EnumTypeName &a)                                                                                             \
    {                                                                                                                                           \
        a = EnumTypeName((typename std::underlying_type<EnumTypeName>::type)(a) + 1);                                                           \
        return a;                                                                                                                               \
    }



namespace xxlib
{

	// 计算 n 是 2 的几次方
	inline static int Calc2n(size_t n)
	{
#ifdef _MSC_VER
		unsigned long r = 0;
# ifdef XXLIB_64BIT
		_BitScanReverse64(&r, n);
# else
		_BitScanReverse(&r, n);
# endif
		return (int)r;
#else
# ifdef XXLIB_64BIT
		return int(63 - __builtin_clzl(n));
# else
		return int(32 - __builtin_clz(n));
# endif
#endif
	}

	// 返回 n 按 2^n 对齐的值
	inline static size_t Round2n(size_t n)
	{
		auto rtv = size_t(1) << Calc2n(n);
		if (rtv == n) return n;
		else return rtv << 1;
	}

	/************************************************************************/
	/* XXLIB_HAS_TYPEDEF                                                    */
	/************************************************************************/

	template<typename T>
	struct Typedef_Void
	{
		typedef void type;
	};

#define XXLIB_HAS_TYPEDEF( typedefName )	\
template<typename T, typename dummy = void> struct HasTypedef_##typedefName : std::false_type {};	\
template<typename T> struct HasTypedef_##typedefName<T, typename xxlib::Typedef_Void<typename T::typedefName>::type> : std::true_type {};



	/************************************************************************/
	/* MaxSizeof                                                            */
	/************************************************************************/

	template<typename T, typename... Args>
	class MaxSizeof
	{
	public:
		enum
		{
			value = sizeof(T) > MaxSizeof<Args...>::value
			? sizeof(T)
			: MaxSizeof<Args...>::value
		};
	};
	template<typename T>
	class MaxSizeof < T >
	{
	public:
		enum
		{
			value = sizeof(T)
		};
	};


	/************************************************************************/
	/* HasParms                                                             */
	/************************************************************************/

	template<typename...TS>
	struct HasParms
	{
		static const bool value = sizeof...(TS) > 0;
	};


	/************************************************************************/
	/* TypeDeclare                                                          */
	/************************************************************************/

	// for gcc explicit specialization template func
	template<typename T>
	class TypeDeclare
	{
	};

	/************************************************************************/
	/* MemmoveSupport                                                          */
	/************************************************************************/

	// 用于标记 存在于自己轮的容器类中时是否支持直接 memcpy memmove 操作而不伤语义
	template<typename T>
	struct MemmoveSupport
	{
		static const bool value = false;
	};


	/************************************************************************/
	/* Stopwatch                                                            */
	/************************************************************************/

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


	/************************************************************************/
	/* ScopeGuard                                                           */
	/************************************************************************/

	struct ScopeGuard
	{
		template<typename T>
		ScopeGuard(T&& f) : func(std::forward<T>(f)) {}
		~ScopeGuard() { Run(); }
		inline void RunAndCancel() { Run(); Cancel(); }
		inline void Run() { if (func) func(); }
		inline void Cancel() { func = nullptr; }
		template<typename T>
		inline void Set(T&& f) { func = std::forward<T>(f); }
	private:
		std::function<void()> func;
		ScopeGuard(ScopeGuard const&) = delete;
		ScopeGuard &operator=(ScopeGuard const&) = delete;
	};


	/************************************************************************/
	/* BufAndDataLen                                                        */
	/************************************************************************/

	template<typename SizeType = size_t>
	struct BufAndDataLen
	{
		char* buf;
		SizeType dataLen;
	};



	/************************************************************************/
	// TypeId, TypeName 映射
	/************************************************************************/

	template<typename T>
	struct TypeId
	{
		static const uint16_t value = 0;
	};


	/************************************************************************/
	// EqualsTo( 重载后用于指针对象按内容对比 )
	/************************************************************************/

	template<typename T>
	bool EqualsTo(T const& a, T const& b)
	{
		return a == b;
	}

	/************************************************************************/
	// Singleton Template
	/************************************************************************/

	template<typename T>
	struct Singleton
	{
		inline static T& GetInstance()
		{
			assert(Instance());
			return *Instance();
		};
		inline static void InitInstance()
		{
			assert(!Instance());
			Instance() = new T();
		}
		inline static void DestroyInstance()
		{
			assert(Instance());
			delete Instance();
			Instance() = nullptr;
		}

	protected:
		inline static T*& Instance()
		{
			static T* data = nullptr;
			return data;
		};
	};

	/************************************************************************/
	// GetFuncReturnType
	/************************************************************************/

	// 用来取成员函数返回值的类型( decltype(GetFuncReturnType(&Type::Func)) )
	template<typename T, typename R, typename ...Args>
	R GetFuncReturnType(R(T::*)(Args...))
	{
		return *(R*)nullptr;
	}

	// 用来取静态函数返回值的类型( decltype(GetFuncReturnType(&Func)) )
	template<typename R, typename ...Args>
	R GetFuncReturnType(R(*)(Args...))
	{
		return *(R*)nullptr;
	}

	template<typename T, typename R>
	R GetFieldType(R T::*)
	{
		return *(R*)nullptr;
	}

	/************************************************************************/
	// GetFuncArgsCount
	/************************************************************************/

	template<typename T, typename R, typename ...Args>
	constexpr int GetFuncArgsCount(R(T::* f)(Args...))
	{
		return sizeof...(Args);
	}
	template<typename T, typename ...Args>
	constexpr int GetFuncArgsCount(void(T::* f)(Args...))
	{
		return sizeof...(Args);
	}

	/************************************************************************************/
	// FuncTupleCaller
	/************************************************************************************/

	template<typename R, typename T, typename...TS, size_t...I>
	R FuncTupleCaller(T* o, R(T::*f)(TS...), std::tuple<TS...>& tp, std::index_sequence<I...>)
	{
		return (o->*f)(std::get<I>(tp)...);
	}

	template<typename T, typename...TS, size_t...I>
	void FuncTupleCaller(T* o, void(T::*f)(TS...), std::tuple<TS...>& tp, std::index_sequence<I...>)
	{
		(o->*f)(std::get<I>(tp)...);
	}


	/************************************************************************************/
	// TupleIndexOf
	/************************************************************************************/

	template <class T, class Tuple>
	struct TupleIndexOf;

	template <class T, class... Types>
	struct TupleIndexOf<T, std::tuple<T, Types...>> {
		static const int value = 0;
	};

	template <class T, class U, class... Types>
	struct TupleIndexOf<T, std::tuple<U, Types...>> {
		static const int value = 1 + TupleIndexOf<T, std::tuple<Types...>>::value;
	};

}
