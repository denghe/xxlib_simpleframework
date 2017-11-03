#include "Precompile.h"

MAKE_INSTANCE_CPP( FileCache );

FileCache::FileCache()
{
    FileUtils::_getDataFromCache = [ this ]( string const & fn )
    {
        lock_guard<mutex> lg( _mutex );
        auto d = _cache.get( fn );
        if( d && !d->getSize() ) d = nullptr;
        return d;
    };

    FileUtils::_pushDataToCache = [ this ]( string const & fn, Data & data )
    {
        // todo: 解码
        // 当前方案为以最大版本号作为 seed, 自然顺序依次随出一个 4字节 值, 放到文件尾( 最后面放 1c ac 00 01 作为 混淆标志 )
        // 该值为 32 字节循环异或表 的 seed

        int siz = (int)data.getSize();
        if( siz >= 8 )
        {
            auto p = (byte*)data.getBytes();
            if( p[ siz - 4 ] == 0x1c && p[ siz - 3 ] == 0xac && p[ siz - 2 ] == 0x00 && p[ siz - 1 ] == 0x01 )
            {
                uint seed = 0;
                auto buf = (byte*)&seed;
                buf[ 0 ] = p[ siz - 8 ];
                buf[ 1 ] = p[ siz - 7 ];
                buf[ 2 ] = p[ siz - 6 ];
                buf[ 3 ] = p[ siz - 5 ];


                uint nums[ 8 ];
                mt19937 r( seed );
                for( int i = 0; i < 8; ++i )
                {
                    nums[ i ] = r();
                }
                //CCLOG( "fn=%s, seed=%u, nums=%u,%u,%u,%u,%u,%u,%u,%u"
                //       , fn.c_str()
                //       , seed
                //       , nums[ 0 ], nums[ 1 ], nums[ 2 ], nums[ 3 ]
                //       , nums[ 4 ], nums[ 5 ], nums[ 6 ], nums[ 7 ]
                //       );

                auto o = (uint*)p;
                siz = MIN( 2048, siz - 8 );
                auto len = siz / 4 + ( siz % 4 > 0 ? 1 : 0 );
                for( int i = 0; i < len; ++i )
                {
                    o[ i ] ^= nums[ i % 8 ];
                }

                data.fastSet( data.getBytes(), data.getSize() - 8 );
            }
        }

#if (CC_TARGET_PLATFORM != CC_PLATFORM_WP8)
        if( data.getSize() < 20000 )        // 先设个 20k, 小于这个值的 都放 cache
        {
            lock_guard<mutex> lg( _mutex );
            _cache.add( fn, data, false );
        }
#endif
    };
}

FileCache::~FileCache()
{
}

void FileCache::clear()
{
    _cache.clear();
}
