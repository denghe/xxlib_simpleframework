#include "Precompile.h"

MAKE_INSTANCE_CPP( Destructor )

DestructorItem::DestructorItem()
{
}

DestructorItem::DestructorItem( DestructorItem const & other )
: _funcs( other._funcs )
{
}

DestructorItem::DestructorItem( DestructorItem && other )
: _funcs( move( other._funcs ) )
{
}

DestructorItem::~DestructorItem()
{
    run();
}

void DestructorItem::add( FuncType f )
{
    _funcs.push( f );
}

void DestructorItem::run()
{
    while( !_funcs.empty() )
    {
        _funcs.top()( );
        _funcs.pop();
    }
}






Destructor::Destructor()
: _top( nullptr )
{
}

Destructor::~Destructor()
{
    while( !_destructors.empty() ) pop();
}

void Destructor::add( DestructorItem::FuncType f )
{
    if( !_top ) push();
    _top->add( f );
}

void Destructor::run()
{
    if( _top ) _top->run();
}

void Destructor::pop()
{
    if( !_top ) return;
    _top->run();
    _destructors.pop();
    if( _destructors.empty() ) _top = nullptr;
    else _top = &_destructors.top();
}

void Destructor::push()
{
    _destructors.push( DestructorItem() );
    _top = &_destructors.top();
}





/*

Ê¾Àı:

T *p = new ....;
Destructor::getInstance()->add( p );


int * ttttt( int i )
{
    auto rtv = new int( i );
    Destructor::getInstance()->add( [ rtv ]
    {
        CCLOG( "delete %d", *rtv );
        delete rtv;
    } ); 
    return rtv;
}

void test()
{
    auto ds = Destructor::getInstance();
    ds->push();
    auto xxx1 = ttttt( 1 );
    CCLOG( "return %d", *xxx1 );
    auto xxx2 = ttttt( 2 );
    CCLOG( "return %d", *xxx2 );
    auto xxx3 = ttttt( 3 );
    CCLOG( "return %d", *xxx3 );
    ds->pop();
}

// helper º¯Êı³¤Ïñ
auto D_add = []( T* p )-> T* 
{
  Destructor::getInstance()->add( [p]
  {
    T::free( p ); 
  };
  return p;
}

auto t = D_add( T::create( ... ) );

*/
