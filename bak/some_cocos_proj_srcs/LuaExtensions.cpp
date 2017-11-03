#include "Precompile.h"

extern "C"
{
    #include "cjson/lua_cjson.h"
    // ... more c lib impl include

    static luaL_Reg luax_exts[] = 
    {
        { "cjson", luaopen_cjson },
        // ...
    };

    void luaopen_lua_extensions_ww( lua_State *L )
    {
//        luaL_Reg* lib = luax_exts;
        lua_getglobal( L, "package" );
        lua_getfield( L, -1, "preload" );

        for( auto& lib : luax_exts )
        {
            lua_pushcfunction( L, lib.func );
            lua_setfield( L, -2, lib.name );
        }
        lua_pop( L, 2 );
    }
}
