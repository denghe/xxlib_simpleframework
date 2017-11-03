#include "Precompile.h"

void BufferHelper::convertHex( string & s, char v )
{
    static char const Int2HexTable[] = { '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F' };
    s.push_back( Int2HexTable[ (uint8)v >> 4 ] );
    s.push_back( Int2HexTable[ v & 15 ] );
}

void BufferHelper::convertHex( string & s, int v )
{
    auto p = (char*)&v;
    convertHex( s, p[ 3 ] );
    convertHex( s, p[ 2 ] );
    convertHex( s, p[ 1 ] );
    convertHex( s, p[ 0 ] );
}

void BufferHelper::dumpAscII( string & s, char const * buf, int len )
{
    for( int i = 0; i < len; ++i )
    {
        uint8 c = buf[ i ];
        if( c < 32 || c > 126 ) s.push_back( '.' );
        else s.push_back( c );
    }
}

string BufferHelper::dump( string & s, char const * const & buf, int len )
{
    if( len == 0 ) return s;
    char bf[ 128 ];
    sprintf( bf, "\n--------  0  1  2  3 | 4  5  6  7 | 8  9  A  B | C  D  E  F" );
    s.append( bf );
    if( len == 0 ) return s;

    int i = 0;
    for( ; i < len; ++i )
    {
        if( ( i % 16 ) == 0 )
        {
            if( i )
            {           // ÔÚÐÐÎ²Êä³ö ascii
                s.append( "  " );
                dumpAscII( s, buf + i - 16, 16 );
            }
            s.push_back( '\n' );
            convertHex( s, i );
            s.append( "  " );
        }
        else if( i && ( i % 4 == 0 ) )
        {
            s.append( "  " );
        }
        else s.push_back( ' ' );
        convertHex( s, buf[ i ] );
    }
    int left = i % 16;
    if( left )
    {
        len = len + 16 - left;
        for( ; i < len; ++i )
        {
            if( i && ( i % 4 == 0 ) )
                s.append( "  " );
            else s.push_back( ' ' );
            s.append( "  " );
        }
        s.append( "  " );
        dumpAscII( s, buf + i - 16, left );
    }
    return s;
    //return string();
}

void BufferHelper::set( char * const & buf, int const & offset, char const * const & v, int const & len )
{
    memcpy( buf + offset, v, len );
}

void BufferHelper::set( char * const & buf, int const & offset, string const & v )
{
    memcpy( buf + offset, v.c_str(), v.size() + 1 );
}

void BufferHelper::write( char * const & buf, int & offset, char const * const & v, int const & len )
{
    memcpy( buf + offset, v, len );
    offset += len;
}

void BufferHelper::write( char * const & buf, int & offset, string const & v )
{
    memcpy( buf + offset, v.c_str(), v.size() + 1 );
    offset += (int)v.size() + 1;
}

void BufferHelper::get( char * const & v, char const * const & buf, int const & offset, int const & len )
{
    memcpy( v, buf + offset, len );
}

void BufferHelper::get( string & v, char const * const & buf, int const & offset )
{
    v.assign( buf + offset );
}

void BufferHelper::read( char * const & v, char const * const & buf, int & offset, int const & len )
{
    memcpy( v, buf + offset, len );
    offset += len;
}

void BufferHelper::read( string & v, char const * const & buf, int & offset )
{
    v.assign( buf + offset );
    offset += (int)v.size() + 1;
}

