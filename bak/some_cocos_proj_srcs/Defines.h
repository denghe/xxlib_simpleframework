#ifndef __DEFINES_H__
#define __DEFINES_H__

// 各种宏 / 函数, 便利性质的东西
/*
index:

    Common type defines

    clz
    round2n

    SAFE_DELETE         SAFE_DELETE_ARRAY

    BIT_ENABLED         BIT_DISABLED            SET_BITS            CLR_BITS            ROUND_SIZE
    L32                 H32                     L16                 H16
    MAKE_UINT64

    sleepMS

    _countof

    MAKE_INSTANCE_H     MAKE_INSTANCE_CPP

    DELETE_COPY_ASSIGN  DEPRECATED

    strdup              snprintf                stricmp

    PROP_GET_SET        PROP_SET_IS             PROP_IS             PROP_GET_SET_H      PROP_SET_H_IS

    最后是各种 sTypeName 对应 *TypeName::getInstance()
*/


/* Common types */
typedef int8_t  int8, sbyte;
typedef int16_t int16;
typedef int32_t int32;
typedef int64_t int64;

typedef uint8_t  uint8, byte;
typedef uint16_t uint16, ushort;
typedef uint32_t uint32, uint;
typedef uint64_t uint64;

typedef unsigned long ulong;    // x64: vc 32bit, gcc 64bit
typedef wchar_t wchar;          // win/ios: 16bit, linux, android 32bit



// calc lead zero （数 2 进制数高位头上的 0 的个数
#ifdef __GNUC__
#define clz(x) __builtin_clz(x)
#elif defined(__IA) && defined(_MSC_VER)
#include <intrin.h>
INLINE int clz( size_t x )
{
    unsigned long r = 0;
#ifdef __X64
    _BitScanReverse64( &r, x );
    return 63 - r;
#else
    _BitScanReverse( &r, x );
    return 31 - r;
#endif
}
#else
#ifdef __X64
#error not support
#endif
INLINE int popcnt( size_t x )
{
    x -= ( ( x >> 1 ) & 0x55555555 );
    x = ( ( ( x >> 2 ) & 0x33333333 ) + ( x & 0x33333333 ) );
    x = ( ( ( x >> 4 ) + x ) & 0x0f0f0f0f );
    x += ( x >> 8 );
    x += ( x >> 16 );
    return x & 0x0000003f;
}
//INLINE int ctz( size_t x ) {
//    return popcnt((x & -x) - 1);
//}
INLINE int clz( size_t x )
{
    x |= ( x >> 1 );
    x |= ( x >> 2 );
    x |= ( x >> 4 );
    x |= ( x >> 8 );
    x |= ( x >> 16 );
    return 32 - popcnt( x );
}
#endif


// 返回刚好大于 x 的 2^n 的值用于内存分配
INLINE size_t round2n( size_t len )
{
#ifdef __X64
    int bits = 63 - clz( len );
#else
    int bits = 31 - clz( len );
#endif
    size_t rtv = size_t( 1 ) << bits;
    if( rtv == len ) return len;
    return rtv << 1;
}


// safe delete
#define SAFE_DELETE(p)  \
do                      \
{                       \
    delete p;           \
    p = 0;              \
} while( 0 )

#define SAFE_DELETE_ARRAY(p) \
do                           \
{                            \
    delete[] p;              \
    p = 0;                   \
} while( 0 )




// bit operations
#define BIT_ENABLED(v, bit)  ((v) & (bit))
#define BIT_DISABLED(v, bit) (!((v) & (bit)))

#define SET_BITS(v, bits) (v |= (bits))
#define CLR_BITS(v, bits) (v &= ~(bits))

#define ROUND_SIZE(S, n) (((S) + (n) - 1) & ~((n) - 1))

/// Low, High bits
#define L32(x) (uint32)((x) & 0xffffffff)
#define H32(x) (uint32)((x) >> 32 & 0xffffffff)
#define L16(x) (uint16)((x) & 0xffff)
#define H16(x) (uint16)((x) >> 16 & 0xffff)

/// make uint64
#define MAKE_UINT64(h, l) \
    ( (uint64)( h ) << 32 | ( l ) )


// these code copy from c2dx
#define HOST_IS_BIG_ENDIAN (bool)(*(unsigned short *)"\0\xff" < 0x100) 
#define SWAP32(i)  ((i & 0x000000ff) << 24 | (i & 0x0000ff00) << 8 | (i & 0x00ff0000) >> 8 | (i & 0xff000000) >> 24)
#define SWAP16(i)  ((i & 0x00ff) << 8 | (i &0xff00) >> 8)   
#define SWAP_INT32_LITTLE_TO_HOST(i) ((HOST_IS_BIG_ENDIAN == true)? SWAP32(i) : (i) )
#define SWAP_INT16_LITTLE_TO_HOST(i) ((HOST_IS_BIG_ENDIAN == true)? SWAP16(i) : (i) )
#define SWAP_INT32_BIG_TO_HOST(i)    ((HOST_IS_BIG_ENDIAN == true)? (i) : SWAP32(i) )
#define SWAP_INT16_BIG_TO_HOST(i)    ((HOST_IS_BIG_ENDIAN == true)? (i):  SWAP16(i) )







INLINE void sleepMS( int ms )
{
    std::this_thread::sleep_for( std::chrono::milliseconds( ms ) );
}



// _countof
#ifndef _countof
template<typename T, size_t N>
int _countof( T const (&arr)[N] ) { return N; }
#endif



// 方便为某类加上 instance 相关的东西

#define MAKE_INSTANCE_H( TN )       \
public:                             \
    static TN* getInstance();       \
    static void initInstance();     \
    static void freeInstance();     \
private:                            \
    static TN* _instance;           \
public:


#define MAKE_INSTANCE_CPP( TN )     \
TN* TN::_instance = nullptr;        \
TN* TN::getInstance()               \
{                                   \
    return _instance;               \
}                                   \
void TN::initInstance()             \
{                                   \
    _instance = new TN();           \
}                                   \
void TN::freeInstance()             \
{                                   \
    SAFE_DELETE( _instance );       \
}



#define MAKE_INSTANCE( TN )                                                         \
private:                                                                            \
static TN* _instance;                                                               \
public:                                                                             \
    inline static TN* getInstance() { return _instance; }                           \
    inline static void initInstance() { _instance = new TN(); }                     \
    inline static void freeInstance() { delete _instance; _instance = nullptr; }




// crt fucking stuffs
#if __WIN

#ifndef _CRTDBG_MAP_ALLOC
#define strdup   _strdup
#endif

#define snprintf _snprintf
#define stricmp  _stricmp

#else// non win
#define stricmp  strcasecmp
#endif






// A macro to disallow the copy constructor and operator= functions
// This should be used in the private: declarations for a class
#if defined(__GNUC__) && ((__GNUC__ >= 5) || ((__GNUG__ == 4) && (__GNUC_MINOR__ >= 4))) \
    || (defined(__clang__) && (__clang_major__ >= 3)) || (_MSC_VER >= 1800)
#define DELETE_COPY_ASSIGN(TypeName)        \
    TypeName(const TypeName &) = delete;    \
    TypeName &operator =(const TypeName &) = delete;
#else
#define DELETE_COPY_ASSIGN(TypeName)        \
    TypeName(const TypeName &);             \
    TypeName &operator =(const TypeName &);
#endif


// deprecated attribute
#if defined(__GNUC__) && ((__GNUC__ >= 4) || ((__GNUC__ == 3) && (__GNUC_MINOR__ >= 1)))
#define CC_DEPRECATED_ATTRIBUTE __attribute__((deprecated))
#elif _MSC_VER >= 1400 //vs 2005 or higher
#define DEPRECATED __declspec(deprecated) 
#else
#define DEPRECATED
#endif 









// 创建私有变量及属性函数

#define PROP_GET( varType, varName, funName )                           \
protected: varType varName;                                             \
public: inline varType const & get##funName() const { return varName; }

#define PROP_GET_SET( varType, varName, funName )                       \
protected: varType varName;                                             \
public: inline varType const & get##funName() const { return varName; } \
public: inline void set##funName( varType const & v ) { varName = v; }

#define PROP_IS( varName, funName )                                     \
protected: bool varName;                                                \
public: inline bool is##funName() const { return varName; }

#define PROP_SET_IS( varName, funName )                                 \
protected: bool varName;                                                \
public: inline bool is##funName() const { return varName; }             \
public: inline void set##funName( bool b ) { varName = b; }


// 下面是 set 函数不提供实现的版本

#define PROP_GET_SET_H( varType, varName, funName )                     \
protected: varType varName;                                             \
public: inline varType const & get##funName() const { return varName; } \
public: void set##funName( varType const & v );


#define PROP_SET_H_IS( varName, funName )                               \
protected: bool varName;                                                \
public: inline bool is##funName() const { return varName; }             \
public: void set##funName( bool b );













#define sDestructor         (*Destructor         ::getInstance())
#define sFileManager        (*Destructor         ::getInstance())
#define sFileCache          (*FileCache          ::getInstance())
#define sNetEngine          (*NetEngine          ::getInstance())
#define sNetHttp            (*NetHttp            ::getInstance())
#define sCommandQueue       (*CommandQueue       ::getInstance())



#endif

