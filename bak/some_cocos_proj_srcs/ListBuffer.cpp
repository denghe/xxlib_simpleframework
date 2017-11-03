#include "Precompile.h"

ListBuffer::ListBuffer( int block_min_size )
: _header( 0 )
, _tail( 0 )
, _header_ptr( 0 )
, _tail_ptr( 0 )
, _size( 0 )
{
    if( block_min_size < (int)NetConfigs::ListBufferMinBlockSize )
    {
        _block_min_size = (uint32_t)round2n( (int)NetConfigs::ListBufferMinBlockSize );
    }
    else
    {
        _block_min_size = (uint32_t)round2n( block_min_size );
    }
}

ListBuffer::~ListBuffer()
{
    Block *b_to_free;
    while( _header )
    {
        b_to_free = _header;
        _header = _header->next;
        free_block( b_to_free );
    }
}

void ListBuffer::push( const char *buf, int len )
{
    if( len == 0 ) return;

    // allocate first block if the list is empty
    if( !_tail )
    {
        append_block( len );
        _header = _tail;
        _header_ptr = 0;
    }

    // copy data to blocks
    int avail = _tail->size - _tail_ptr;
    int left = len;
    if( avail == 0 )
    {
        append_block( len );
        avail = _tail->size;
    }

    if( left >= avail )
    {
        memcpy( _tail->data() + _tail_ptr, buf, avail );

        buf += avail;
        left -= avail;
        _tail_ptr += avail;

        if( left > 0 ) append_block( left );
    }

    if( left > 0 )
    {
        memcpy( _tail->data() + _tail_ptr, buf, left );
        _tail_ptr += left;
    }

    _size += len;
}

int ListBuffer::copy( char *buf, int len )
{
    if( len > _size ) len = _size;
    if( len == 0 ) return 0;

    Block *b = _header;
    int avail = b->size - _header_ptr;
    char *p = b->data() + _header_ptr;
    int left = len;
    while( left > avail )
    {
        memcpy( buf, p, avail );

        buf += avail;
        left -= avail;

        b = b->next;
        avail = b->size;
        p = b->data();
    }

    if( left > 0 )
    {
        memcpy( buf, p, left );
    }

    return len;
}

int ListBuffer::pop( int len )
{
    if( len > _size ) len = _size;
    if( len == 0 ) return 0;

    int avail = _header->size - _header_ptr;
    int left = len;
    Block *b_to_free;
    while( left >= avail )
    {
        b_to_free = _header;
        _header = _header->next;
        free_block( b_to_free );

        left -= avail;

        if( !_header ) break;
        avail = _header->size;
    }

    if( left > 0 ) _header_ptr += left;

    if( !_header )
    {
        _tail = 0;
        _tail_ptr = 0;
    }

    _size -= len;

    return len;
}

void ListBuffer::append_block( int len )
{
    // adjust length for allocating
    int to_alloc = len + sizeof(int)+sizeof( Block * );

    // alloc block
    to_alloc = (uint32_t)round2n( to_alloc );
    if( to_alloc < _block_min_size ) to_alloc = _block_min_size;

    Block *block = (Block *)malloc( to_alloc );

    block->size = len;
    block->next = 0;

    // append
    if( _tail ) _tail->next = block;

    _tail = block;
    _tail_ptr = 0;
}

void ListBuffer::free_block( Block *block )
{
    free( block );
    _header_ptr = 0;
}

int ListBuffer::size() const
{
    return _size;
}

void ListBuffer::clear()
{
    pop( _size );
}

int ListBuffer::empty() const
{
    return _size == 0;
}

char * ListBuffer::Block::data() const
{
    return (char *)&next + sizeof( Block * );
}
