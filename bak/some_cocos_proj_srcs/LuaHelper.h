#ifndef _LUAHELPER_H_
#define _LUAHELPER_H_

namespace LuaHelper
{
    // 各种 lua push

    inline void push( int intValue )
    {
        lua_pushinteger( Global::L, intValue );
    }

    inline void push( float floatValue )
    {
        lua_pushnumber( Global::L, floatValue );
    }

    inline void push( double floatValue )
    {
        lua_pushnumber( Global::L, floatValue );
    }

    inline void push( bool boolValue )
    {
        lua_pushboolean( Global::L, boolValue );
    }

    inline void push( const char * stringValue )
    {
        lua_pushstring( Global::L, stringValue );
    }

    inline void push( std::string stringValue )
    {
        lua_pushlstring( Global::L, stringValue.c_str(), stringValue.size() );
    }

    template<typename T>
    void push( vector<T> const & vs )
    {
        lua_newtable( Global::L );
        for( decltype( vs.size() ) i = 0; i < vs.size(); ++i )
        {
            push( (int)i );
            push( vs[ i ] );
            lua_settable( Global::L, -3 );
        }
    }

    template<typename KT, typename VT>
    void push( unordered_map<KT, VT> const & kvs )
    {
        lua_newtable( Global::L );
        for( auto &kv : kvs )
        {
            push( kv.first );
            push( kv.second );
            lua_settable( Global::L, -3 );
        }
    }

    template<typename KT, typename VT>
    void push( pair<KT, VT> const & p )
    {
        lua_newtable( Global::L );
        push( p.first );
        push( p.second );
        lua_settable( Global::L, -3 );
    }

    template<typename KT, typename VT>
    void push( map<KT, VT> const & kvs )
    {
        lua_newtable( Global::L );
        for( auto &kv : kvs )
        {
            push( kv.first );
            push( kv.second );
            lua_settable( Global::L, -3 );
        }
    }

    inline void clean()
    {
        lua_settop( Global::L, 0 );
    }

    inline void pushNil()
    {
        lua_pushnil( Global::L );
    }

    template<typename T>
    void push( T* objectValue, const char * typeName )
    {
        if( !objectValue ) pushNil();
        else
        {
            if( is_base_of<Ref, T>::value )
            {
                Global::LS->pushObject( (Ref*)objectValue, typeName );
            }
            else
            {
                tolua_pushusertype( Global::L, objectValue, typeName );
            }
        }
    }

    template<typename T>
    void push( T* objectValue, string const & typeName )
    {
        push( objectValue, typeName.c_str() );
    }

    // 调用 handler 指向的 lua 函数( numArgs 为 已 push 的参数个数 )
    inline int executeFunction( int nHandler, int numArgs = 0 )
    {
        return Global::LS->executeFunctionByHandler( nHandler, numArgs );
    }

    // 上面函数的字串返回值版
    inline int executeFunction( int nHandler, int numArgs, string& rtv )
    {
        return Global::LS->executeFunction( nHandler, numArgs, 1, [&]( lua_State* ls, int numRtvs )
        {
            size_t len;
            if( lua_isstring( ls, -1 ) )
            {
                auto s = lua_tolstring( ls, -1, &len );
                rtv.assign( s, len );
            }
            else rtv.clear();
        } );
    }
}


#endif
