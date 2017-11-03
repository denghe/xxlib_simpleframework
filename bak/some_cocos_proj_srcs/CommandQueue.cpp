#include "Precompile.h"

MAKE_INSTANCE_CPP( CommandQueue );

CommandQueue::CommandQueue()
{
}

CommandQueue::~CommandQueue()
{
}

void CommandQueue::push( FT f )
{
    lock_guard<mutex> g( _m );
    _fs.push_back( f );
}

void CommandQueue::push( LUA_FUNCTION f )
{
    lock_guard<mutex> g( _m );
    _fs.push_back( [ f ] 
    {
        LuaHelper::executeFunction( f );
    } );
}

void CommandQueue::update()
{
    if( _fs.empty() ) return;

    vector<FT> fs;
    {
        lock_guard<mutex> g( _m );
        fs = _fs;
        _fs.clear();
    }

    for( auto f : fs ) f();
    fs.clear();
}
