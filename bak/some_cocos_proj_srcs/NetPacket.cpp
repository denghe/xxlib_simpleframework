#include "Precompile.h"


NetPacket::NetPacket() : _wo( 0 )
, _ro( 0 )
, _bufLen( (int)NetConfigs::PacketBufferSize - sizeof( Header ) )
{
    _buf = new char[ (int)NetConfigs::PacketBufferSize ] + sizeof( Header );
}

NetPacket::NetPacket( int maxPkgLen ) : _wo( 0 )
, _ro( 0 )
{
    if( maxPkgLen == 0 )
    {
        maxPkgLen = (int)NetConfigs::PacketBufferSize;
        _bufLen = (int)NetConfigs::PacketBufferSize - sizeof( Header );
    }
    else _bufLen = maxPkgLen - sizeof( Header );
    assert( maxPkgLen >= sizeof( Header ) );
    _buf = new char[ maxPkgLen ] + sizeof( Header );
}

NetPacket::NetPacket( NetPacket const & other )
: _ro( other._ro )
, _wo( other._wo )
, _bufLen( other._bufLen )
{
    _buf = new char[ other.bufLen() ];
    memcpy( _buf, other.bufData(), other.bufLen() );
}

NetPacket::NetPacket( NetPacket && other )
: _ro( other._ro )
, _wo( other._wo )
, _bufLen( other._bufLen )
, _buf( other._buf )
{
    other._buf = nullptr;
}

NetPacket & NetPacket::operator=( NetPacket const & other )
{
    _ro = other._ro;
    _wo = other._wo;
    _bufLen = other._bufLen;
    if( _buf )
        delete[] bufData();
    _buf = new char[ other.bufLen() ];
    memcpy( _buf, other.bufData(), other.bufLen() );
    return *this;
}

NetPacket & NetPacket::operator=( NetPacket && other )
{
    _ro = other._ro;
    _wo = other._wo;
    _bufLen = other._bufLen;
    swap( _buf, other._buf );
    return *this;
}

NetPacket::~NetPacket()
{
    if( _buf )
        delete[] bufData();
}

string NetPacket::dump()
{
    prepare();
    char buf[ 128 ];
    sprintf( buf, "Packet Dump\nHeader: Packet Length = %d, OpCode = %d\nBuffer: Data Buffer Length = %d, Data Length = %d"
             , pkgLen(), opcode(), _bufLen, _wo );
    string s = buf;
    BufferHelper::dump( s, pkgData(), pkgLen() );
    printf( "%s\n", s.c_str() );  // 立即输出， lua 可能收接不下
    return "";
}

void NetPacket::dwu4( uint32 v )
{
    BufferHelper::write( _buf, _wo, v );
}

void NetPacket::dwu2( uint16 v )
{
    BufferHelper::write( _buf, _wo, v );
}

void NetPacket::dwu1( uint8 v )
{
    BufferHelper::write( _buf, _wo, v );
}

void NetPacket::dw4( int32 v )
{
    BufferHelper::write( _buf, _wo, v );
}

void NetPacket::dw2( int16 v )
{
    BufferHelper::write( _buf, _wo, v );
}

void NetPacket::dw1( int8 v )
{
    BufferHelper::write( _buf, _wo, v );
}

void NetPacket::dwf( float v )
{
    BufferHelper::write( _buf, _wo, v );
}

void NetPacket::dwd( double v )
{
    BufferHelper::write( _buf, _wo, v );
}

void NetPacket::dws( const char * v )
{
    int len = (int)strlen( v ) + 1; BufferHelper::write( _buf, _wo, v, len );
}

uint32 NetPacket::dru4()
{
    uint32 v; BufferHelper::read( v, _buf, _ro ); return v;
}

uint16 NetPacket::dru2()
{
    uint16 v; BufferHelper::read( v, _buf, _ro ); return v;
}

uint8 NetPacket::dru1()
{
    uint8 v; BufferHelper::read( v, _buf, _ro ); return v;
}

int32 NetPacket::dr4()
{
    int32 v; BufferHelper::read( v, _buf, _ro ); return v;
}

int16 NetPacket::dr2()
{
    int16 v; BufferHelper::read( v, _buf, _ro ); return v;
}

int8 NetPacket::dr1()
{
    int8 v; BufferHelper::read( v, _buf, _ro ); return v;
}

float NetPacket::drf()
{
    float v; BufferHelper::read( v, _buf, _ro ); return v;
}

double NetPacket::drd()
{
    double v; BufferHelper::read( v, _buf, _ro ); return v;
}

string NetPacket::drs()
{
    string v; BufferHelper::read( v, _buf, _ro ); return v;
}

void NetPacket::clearRo()
{
    _ro = 0;
}

void NetPacket::clearWo()
{
    _wo = 0;
}

void NetPacket::clear()
{
    _ro = _wo = 0;
}

int NetPacket::owfree( int offset ) const
{
    return _bufLen - offset;
}

int NetPacket::orfree( int offset ) const
{
    return _wo - offset;
}

int NetPacket::wfree() const
{
    return _bufLen - _wo;
}

int NetPacket::rfree() const
{
    return _wo - _ro;
}

int NetPacket::roffset() const
{
    return _ro;
}

void NetPacket::roffset( int v )
{
    _ro = v;
}

int NetPacket::woffset() const
{
    return _wo;
}

void NetPacket::woffset( int v )
{
    _wo = v;
}

uint16 NetPacket::opcode() const
{
    return header().opcode;
}

void NetPacket::opcode( uint16 v )
{
    header().opcode = v;
}

int NetPacket::bufLen() const
{
    return _bufLen + sizeof( Header );
}

char * NetPacket::bufData() const
{
    return _buf - sizeof( Header );
}

int NetPacket::pkgLen() const
{
    return _wo + sizeof( Header );
}

char * NetPacket::pkgData() const
{
    return _buf - sizeof( Header );
}

char * NetPacket::data() const
{
    return _buf;
}

int NetPacket::dataLen() const
{
    return _wo;
}

void NetPacket::prepare()
{
    header().pkgLen = pkgLen();
}

NetPacket* NetPacket::create( int maxPkgLen /*= 0 */ )
{
    auto rtv = new (nothrow) NetPacket( maxPkgLen );
    if( !rtv ) return nullptr;
    rtv->autorelease();
    return rtv;
}
