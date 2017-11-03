#ifndef _NETPACKET_H_
#define _NETPACKET_H_

// 可设长度的定长网络包
class NetPacket : public Ref
{
public:
    NetPacket();
    explicit NetPacket( int maxPkgLen );
    ~NetPacket();
    NetPacket( NetPacket const & other );
    NetPacket( NetPacket && other );
    NetPacket & operator=( NetPacket const & other );
    NetPacket & operator=( NetPacket && other );

    static NetPacket* create( int maxPkgLen = 0 );

    // 直接系列，将没有任何越界和失败判断.

    // 命名规则说明
    // d: direct
    // w: write
    // r: read
    // u: unsigned
    // 4, 2, 1: bytes
    // f: float
    // d: double
    // s: string

    void        dwu4( uint32 v );
    void        dwu2( uint16 v );
    void        dwu1( uint8  v );
    void        dw4( int32   v );
    void        dw2( int16   v );
    void        dw1( int8    v );
    void        dwf( float   v );
    void        dwd( double  v );
    void        dws( const char * v );

    uint32      dru4();
    uint16      dru2();
    uint8       dru1();
    int32       dr4();
    int16       dr2();
    int8        dr1();
    float       drf();
    double      drd();
    string      drs();

    // 清 read offset
    void        clearRo();

    // 清 write offset
    void        clearWo();

    // 清 read & write offset
    void        clear();

    // 返回剩余可写长度( 从 offset 算起 )
    int         owfree( int offset ) const;

    // 返回剩余可读长度( 从 offset 算起 )
    int         orfree( int offset ) const;

    // 返回剩余可写长度
    int         wfree() const;

    // 返回剩余可读长度
    int         rfree() const;

    // 返回内存 dump string( 会 prepare 一把 )
    string      dump();

    // 返回 read offset
    int         roffset() const;

    // 返回 write offset
    int         woffset() const;

    // 设置 read offset
    void        roffset( int v );

    // 设置 write offset
    void        woffset( int v );

    // 返回 opcode
    uint16      opcode() const;

    // 设置 opcode
    void        opcode( uint16 v );

    // 返回 buffer 长度
    int         bufLen() const;

    // 得到 buffer 指针( 不映射到 LUA )
    char *      bufData() const;

    // 返回 包 长度
    int         pkgLen() const;

    // 返回 包 数据指针
    char *      pkgData() const;

    // 返回 数据区指针
    char *      data() const;

    // 返回 数据长度
    int         dataLen() const;

    // 发包前的准备( 于包头中 填充 包长度 )
    void        prepare();

    // 下面是 C++ 中方便使用的函数( 不映射到 LUA )

    template<typename T>
    NetPacket & operator<<( T v );

    template<typename T>
    NetPacket & operator>>( T & v );

    template<typename T>
    void dw( T const & v );

    template<typename T>
    void dr( T & v );

protected:

#pragma pack( push, 1 )
    // 该类被用于将 _buf 强转为该类指针方便操作，以及丈量长度
    struct Header
    {
        int32 pkgLen;
        uint16 opcode;
    };
#pragma pack( pop )

    // 将 _buf 头硬转为 Header 结构返回引用
    inline Header & header() { return *reinterpret_cast<Header*>( bufData() ); }
    inline Header const & header() const { return *reinterpret_cast<Header*>( bufData() ); }

    int         _ro, _wo;               // 读写 offset 值
    int         _bufLen;                // 这个记录的值为实际 buffer 长 - sizeof(Header)
    char *      _buf;                   // 指向数据区起始地址。减 sizeof(Header) 即为实际指针起始地址
};




template<typename T>
NetPacket & NetPacket::operator<<( T v )
{
    BufferHelper::write( _buf, _wo, v );
    return *this;
}

template<typename T>
NetPacket & NetPacket::operator>>( T & v )
{
    BufferHelper::read( _buf, v, _ro );
    return *this;
}

template<typename T>
void NetPacket::dr( T & v )
{
    BufferHelper::read( _buf, v, _ro );
}

template<typename T>
void NetPacket::dw( T const & v )
{
    BufferHelper::write( _buf, _wo, v );
}




#endif
