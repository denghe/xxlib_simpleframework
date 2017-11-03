#ifndef _BUFFERHELPER_H_
#define _BUFFERHELPER_H_


namespace BufferHelper
{
    // 将 v 转为 16 进制 push 到 s
    void convertHex( string & s, char v );

    // 将 v 转为 16 进制 push 到 s
    void convertHex( string & s, int v );

    // 将 buf 转为 ascii 显示效果 push 到 s ( dump 函数专用 )
    void dumpAscII( string & s, char const * buf, int len );

    // 将 buf 转为 16 进制 push 到 s 并 return s
    string dump( string & s, char const * const & buf, int len );

    // set 系列将直接往 buf 的 offset 偏移地址　写入 v 的数据，不修改 offset

    void set( char * const & buf, int const & offset, char const * const & v, int const & len );

    void set( char * const & buf, int const & offset, string const & v );

    template<typename T>
    void set( char * const & buf, int const & offset, T const & v )
    {
#ifdef __IA
        *(T*)( buf + offset ) = v;
#else
        auto b = buf + offset;
        auto p = (char*)&v;
        if( sizeof(v) >= 1  ) {
            b[ 0 ] = p[ 0 ];
        }
        if( sizeof(v) >= 2 ) {
            b[ 1 ] = p[ 1 ];
        }
        if( sizeof(v) >= 4 ) {
            b[ 2 ] = p[ 2 ];
            b[ 3 ] = p[ 3 ];
        }
        if( sizeof(v) == 8 ) {
            b[ 4 ] = p[ 4 ];
            b[ 5 ] = p[ 5 ];
            b[ 6 ] = p[ 6 ];
            b[ 7 ] = p[ 7 ];
        }
#endif
    }

    // write 系列将往 char* 的 offset 起始地址　写入 v 的数据，同时修改 offset，以实现顺序写入的效果

    void write( char * const & buf, int & offset, char const * const & v, int const & len );

    void write( char * const & buf, int & offset, string const & v );

    template<typename T>
    void write( char * const & buf, int & offset, T const & v )
    {
        set( buf, offset, v );
        offset += sizeof( v );
    }

    // read 系列将直接从 buf 的 offset 偏移地址　读出数据写入 v

    void get( char * const & v, char const * const & buf, int const & offset, int const & len );

    void get( string & v, char const * const & buf, int const & offset );

    template<typename T>
    void get( T & v, char const * const & buf, int const & offset )
    {
#ifdef __IA
        v = *(T*)( buf + offset );
#else
        auto b = buf + offset;
        auto p = (char*)&v;
        if( sizeof(v) >= 1 ) {
            p[ 0 ] = b[ 0 ];
        }
        if( sizeof(v) >= 2 ) {
            p[ 1 ] = b[ 1 ];
        }
        if( sizeof(v) >= 4 ) {
            p[ 2 ] = b[ 2 ];
            p[ 3 ] = b[ 3 ];
        }
        if( sizeof(v) >= 8 ) {
            p[ 4 ] = b[ 4 ];
            p[ 5 ] = b[ 5 ];
            p[ 6 ] = b[ 6 ];
            p[ 7 ] = b[ 7 ];
        }
#endif
    }

    // read 系列将从 buf 的 offset 偏移地址　读出数据写入 v，同时修改 offset，以实现顺序读取的效果

    void read( char * const & v, char const * const & buf, int & offset, int const & len );

    void read( string & v, char const * const & buf, int & offset );

    template<typename T>
    void read( T & v, char const * const & buf, int & offset )
    {
        get( v, buf, offset );
        offset += sizeof( v );
    }

};

#endif
