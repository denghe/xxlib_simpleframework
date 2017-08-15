#pragma once
#pragma execution_character_set("utf-8")
#pragma warning(disable:4819)

#include <cstdint>
#include <chrono>
#ifdef _WIN32
#include <intrin.h>     // _BitScanReverse  64
#else
#include <unistd.h>
#endif
#include <cassert>
#include <type_traits>
#include <array>
#include <functional>
using namespace std::placeholders;

// minor fix
#ifdef max
#undef max
#endif

#ifdef min
#undef min
#endif

/*
XX_IA
XX_64
XX_ALIGN2/4/8/16/32/64
XX_LIKELY / UNLIKELY / INLINE / NOINLINE
MIN / MAX
XX_ENUM_OPERATOR_EXT
XX_HAS_TYPEDEF
XX_LIST_SWAP_REMOVE
XX_HAS_FUNC
XX_CORO_
_countof
container_of
Sleep
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
GetFuncReturnType
GetFuncArgsCount
FuncTupleCaller
TupleIndexOf
BufMaker
Move
time_point <--> DateTime.Ticks converts
*/



/***********************************************************************************/
// XX_IA
/***********************************************************************************/

#if defined(__i386__) || defined(i386) || defined(_M_IX86) || defined(_X86_) || defined(__THW_INTEL) || defined(__x86_64__) || defined(_M_X64)
#    define XX_IA
#endif


/***********************************************************************************/
// XX_64
/***********************************************************************************/

#if defined(__LP64__) || __WORDSIZE == 64 || defined(_WIN64) || defined(_M_X64)
#    define XX_64
#endif


/***********************************************************************************/
// XX_ALIGN2/4/8/16/32/64
/***********************************************************************************/

#ifdef _MSC_VER
#    define XX_ALIGN2( x )		    __declspec(align(2)) x
#    define XX_ALIGN4( x )		    __declspec(align(4)) x
#    define XX_ALIGN8( x )		    __declspec(align(8)) x
#    define XX_ALIGN16( x )		    __declspec(align(16)) x
#    define XX_ALIGN32( x )		    __declspec(align(32)) x
#    define XX_ALIGN64( x )		    __declspec(align(64)) x
#else
#    define XX_ALIGN2( x )          x __attribute__ ((aligned (2)))
#    define XX_ALIGN4( x )          x __attribute__ ((aligned (4)))
#    define XX_ALIGN8( x )          x __attribute__ ((aligned (8)))
#    define XX_ALIGN16( x )         x __attribute__ ((aligned (16)))
#    define XX_ALIGN32( x )         x __attribute__ ((aligned (32)))
#    define XX_ALIGN64( x )         x __attribute__ ((aligned (64)))
#endif



/***********************************************************************************/
// INLINE / NOINLINE / LIKELY / UNLIKELY
/***********************************************************************************/

#ifdef _MSC_VER
#    define XX_INLINE                    __forceinline
#    define XX_NOINLINE                  __declspec(noinline)
#    define XX_LIKELY(x)                 (x)
#    define XX_UNLIKELY(x)               (x)
#else
#    define XX_INLINE                    inline __attribute__((__always_inline__))
#    define XX_NOINLINE                  __attribute__((__noinline__))
#    define XX_UNLIKELY(x)               __builtin_expect((x), 0)
#    define XX_LIKELY(x)                 __builtin_expect((x), 1)
#endif




/***********************************************************************************/
// MIN / MAX
/***********************************************************************************/

#ifndef MIN
#    define MIN( a, b )             ( (a) < (b) ? (a) : (b) )
#endif

#ifndef MAX
#    define MAX( a, b )             ( (a) > (b) ? (a) : (b) )
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
// Sleep
/***********************************************************************************/

// this_thread sleep_for will be create a new thread for sleep, so....
// btw: windows need include windows.h
#ifndef _WIN32
inline void Sleep(int ms)
{
	usleep(ms * 1000);
}
#endif 


/***********************************************************************************/
// _countof
/***********************************************************************************/

#ifndef _countof
template<typename T, size_t N>
size_t _countof(T const (&arr)[N])
{
	return N;
}
#endif


/***********************************************************************************/
// container_of
/***********************************************************************************/

#ifndef _offsetof
#define _offsetof(s,m) ((size_t)&reinterpret_cast<char const volatile&>((((s*)0)->m)))
#endif

#ifndef container_of
#define container_of(ptr, type, member) \
  ((type *) ((char *) (ptr) - _offsetof(type, member)))
#endif



/***********************************************************************************/
// XX_ENUM_OPERATOR_EXT
/***********************************************************************************/

#define XX_ENUM_OPERATOR_EXT( EnumTypeName )                                                                    \
inline EnumTypeName operator+(EnumTypeName const &a, std::underlying_type_t<EnumTypeName> const &b)				\
{                                                                                                               \
    return EnumTypeName((std::underlying_type_t<EnumTypeName>)(a) + b);											\
}                                                                                                               \
inline EnumTypeName operator+(EnumTypeName const &a, EnumTypeName const &b)                                     \
{                                                                                                               \
    return EnumTypeName((std::underlying_type_t<EnumTypeName>)(a) + (std::underlying_type_t<EnumTypeName>)(b));	\
}                                                                                                               \
inline EnumTypeName operator-(EnumTypeName const &a, std::underlying_type_t<EnumTypeName> const &b)				\
{                                                                                                               \
    return EnumTypeName((std::underlying_type_t<EnumTypeName>)(a) - b);											\
}                                                                                                               \
inline std::underlying_type_t<EnumTypeName> operator-(EnumTypeName const &a, EnumTypeName const &b)				\
{                                                                                                               \
    return (std::underlying_type_t<EnumTypeName>)(a) - (std::underlying_type_t<EnumTypeName>)(b);				\
}                                                                                                               \
inline EnumTypeName operator++(EnumTypeName &a)                                                                 \
{                                                                                                               \
    a = EnumTypeName((std::underlying_type_t<EnumTypeName>)(a) + 1);											\
    return a;                                                                                                   \
}



/*
SFINAE check menber function exists
sample：

XX_HAS_FUNC( xxxxxxxxfunc_checker, FuncName, RT ( T::* )( params... ) const );
*/
#define XX_HAS_FUNC( CN, FN, FT )   \
template<typename CT>                                                               \
class CN                                                                            \
{                                                                                   \
    template<typename T, FT> struct FuncMatcher;                                    \
    template<typename T> static char HasFunc( FuncMatcher<T, &T::FN>* );            \
    template<typename T> static int HasFunc( ... );                                 \
public:                                                                             \
    static const bool value = sizeof( HasFunc<CT>( nullptr ) ) == sizeof(char);     \
}




namespace xx
{
	// 计算 n 是 2 的几次方
	inline static int Calc2n(size_t n)
	{
		assert(n);
#ifdef _MSC_VER
		unsigned long r = 0;
# ifdef XX_64
		_BitScanReverse64(&r, n);
# else
		_BitScanReverse(&r, n);
# endif
		return (int)r;
#else
# ifdef XX_64
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
	//// 这个写法更慢
	//inline static size_t Round2n_2(size_t n)
	//{
	//	if (n <= 1) return 1;
	//	return size_t(1) << (Calc2n(n - 1) + 1);
	//}

	/***********************************************************************************/
	/* XX_HAS_TYPEDEF
	/***********************************************************************************/

	template<typename T>
	struct Typedef_Void
	{
		typedef void type;
	};

#define XX_HAS_TYPEDEF( typedefName )	\
template<typename T, typename dummy = void> struct HasTypedef_##typedefName : std::false_type {};	\
template<typename T> struct HasTypedef_##typedefName<T, typename xx::Typedef_Void<typename T::typedefName>::type> : std::true_type {};




	/***********************************************************************************/
	/* XX_LIST_SWAP_REMOVE
	/***********************************************************************************/

	// 交换删除 list 中的某元素( for T* or Ptr<T>, 避免在移出 list 时再次析构 )
#define XX_LIST_SWAP_REMOVE( listPtr, tarPtr, indexName )			\
do																	\
{																	\
	auto count_1 = listPtr->dataLen - 1;							\
	auto& buf = listPtr->buf;										\
	if (count_1 != tarPtr->indexName)								\
	{																\
		buf[count_1]->indexName = tarPtr->indexName;				\
		((void**)buf)[tarPtr->indexName] = ((void**)buf)[count_1];	\
	}																\
	--listPtr->dataLen;												\
} while (false)



	/***********************************************************************************/
	// MaxSizeof
	/***********************************************************************************/

	template<typename T, typename... Args>
	struct MaxSizeof
	{
		static const size_t value = sizeof(T) > MaxSizeof<Args...>::value
			? sizeof(T)
			: MaxSizeof<Args...>::value;
	};
	template<typename T>
	struct MaxSizeof<T>
	{
		static const size_t value = sizeof(T);
	};

	template<typename T, typename... Args>
	constexpr size_t MaxSizeof_v = MaxSizeof<T, Args...>::value;

	/***********************************************************************************/
	// HasParms
	/***********************************************************************************/

	template<typename...TS>
	struct HasParms
	{
		static const bool value = sizeof...(TS) > 0;
	};


	/***********************************************************************************/
	// TypeDeclare
	/***********************************************************************************/

	// for gcc explicit specialization template func
	template<typename T>
	class TypeDeclare
	{
	};

	/***********************************************************************************/
	// MemmoveSupport
	/***********************************************************************************/

	// 用于标记 存在于自己轮的容器类中时是否支持直接 memcpy memmove 操作而不伤语义
	template<typename T>
	struct MemmoveSupport
	{
		static const bool value = false;
	};
	template<typename T>
	constexpr bool MemmoveSupport_v = MemmoveSupport<T>::value;


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


	/***********************************************************************************/
	// ScopeGuard
	/***********************************************************************************/

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


	/***********************************************************************************/
	// BufAndDataLen
	/***********************************************************************************/

	template<typename SizeType = size_t>
	struct BufAndDataLen
	{
		char* buf;
		SizeType dataLen;
	};



	/***********************************************************************************/
	// TypeId 映射
	/***********************************************************************************/

	template<typename T>
	struct TypeId
	{
		static const uint16_t value = 0;
		//constexpr static const char* const& name = "";		// 会导致 vs2017 release 模式下编译崩溃( 含 libuv 的项目 )
	};

	template<typename T>
	constexpr uint16_t TypeId_v = TypeId<T>::value;

	template<typename T>
	constexpr const char* const& TypeId_n = TypeId<T>::name;

	typedef uint16_t TypeIdValueType;


	/***********************************************************************************/
	// EqualsTo( 重载后用于指针对象按内容对比 )
	/***********************************************************************************/

	template<typename T>
	bool EqualsTo(T const& a, T const& b)
	{
		return a == b;
	}

	/***********************************************************************************/
	// Singleton Template
	/***********************************************************************************/

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


	/***********************************************************************************/
	// GetFuncReturnType
	/***********************************************************************************/

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

	/***********************************************************************************/
	// GetFuncArgsCount
	/***********************************************************************************/

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

	template<typename T, typename Tuple>
	constexpr int TupleIndexOf_v = TupleIndexOf<T, Tuple>::value;


	/************************************************************************************/
	// BufMaker
	/************************************************************************************/

	template<typename T, typename ENABLE = void>
	struct BufMaker
	{
		static T Make(char* buf, uint32_t len);
	};



	/************************************************************************************/
	// Move
	/************************************************************************************/

	template<typename T>
	T&& Move(T const& o)
	{
		return (T&&)o;
	}


	/************************************************************************************/
	// time_point <--> DateTime.Ticks converts
	/************************************************************************************/

	int64_t TimeSinceEpochToDateTimeTicks(int64_t const& val)
	{
		return val + 621356256000000000LL;
	}

	int64_t TimePointToDateTimeTicks(std::chrono::system_clock::time_point const& val)
	{
		return val.time_since_epoch().count() + 621356256000000000LL;
	}

	int64_t DateTimeTicksToTimeSinceEpoch(int64_t const& val)
	{
		return val - 621356256000000000LL;
	}

	std::chrono::system_clock::time_point TimeSinceEpochToTimePoint(int64_t const& val)
	{
		return std::chrono::system_clock::time_point::time_point(std::chrono::system_clock::time_point::duration(val));
	}

	std::chrono::system_clock::time_point DateTimeTicksToTimePoint(int64_t const& val)
	{
		return TimeSinceEpochToTimePoint(val - 621356256000000000LL);
	}

	std::chrono::system_clock::time_point GetNowTimePoint()
	{
		return std::chrono::system_clock::now();
	}

	int64_t GetNowTimeSinceEpoch()
	{
		return GetNowTimePoint().time_since_epoch().count();
	}

	int64_t GetNowDateTimeTicks()
	{
		return GetNowTimeSinceEpoch() + 621356256000000000LL;
	}

}
