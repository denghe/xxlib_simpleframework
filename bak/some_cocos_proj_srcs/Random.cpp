#include "Precompile.h"


Random::Random( int seed )
    : _rnd( seed )
{
}

Random* Random::create( int seed )
{
    auto rtv = new (nothrow)Random( seed );
    if( !rtv ) return nullptr;
    rtv->autorelease();
    return rtv;
}

int Random::nextInt( int a, int b )
{
    if( b )
    {
        if( b <= a ) b = a + 1;
        return int( ( _rnd() % ( b - a + 1 ) ) + a );
    }
    else
    {
        if( a < 2 ) a = 2;
        return int( _rnd() % a ) + 1;
    }
}

double Random::nextDouble()
{
    return (double)_rnd() / (double)0x100000000u;
}
