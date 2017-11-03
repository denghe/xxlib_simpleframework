#include "Precompile.h"
#include "lua_ww_manual.hpp"

#ifdef __cplusplus
extern "C" {
#endif
#include  "tolua_fix.h"
#ifdef __cplusplus
}
#endif

#include "cocos2d.h"
#include "LuaBasicConversions.h"
#include "LuaScriptHandlerMgr.h"
#include "CCLuaValue.h"



static int tolua_lua_ww_NetHttp_request( lua_State* tolua_S )
{
    int argc = 0;
    bool ok = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif

#if COCOS2D_DEBUG >= 1
    if( !tolua_isusertype( tolua_S, 1, "NetHttp", 0, &tolua_err ) ) goto tolua_lerror;
#endif

    argc = lua_gettop( tolua_S ) - 1;

    if( argc == 4 )
    {
        std::string  arg0;
        std::string  arg1;
        std::string  arg2;
        LUA_FUNCTION arg3;
        ok &= luaval_to_std_string( tolua_S, 2, &arg0 );
        ok &= luaval_to_std_string( tolua_S, 3, &arg1 );
        ok &= luaval_to_std_string( tolua_S, 4, &arg2 );
        if( !ok ) return 0;
#if COCOS2D_DEBUG >= 1
        if( !toluafix_isfunction( tolua_S, 5, "LUA_FUNCTION", 0, &tolua_err ) )
            goto tolua_lerror;
#endif
        arg3 = toluafix_ref_function( tolua_S, 5, 0 );

        NetHttp::getInstance()->request( arg0.c_str(), arg1.c_str(), arg2.size() ? arg2.c_str() : nullptr, arg3 );
        return 0;
    }
    CCLOG( "%s has wrong number of arguments: %d, was expecting %d\n ", "NetHttp::request", argc, 4 );
    return 0;
#if COCOS2D_DEBUG >= 1
tolua_lerror:
    tolua_error( tolua_S, "#ferror in function 'tolua_lua_ww_NetHttp_request'.", &tolua_err );
#endif
    return 0;
}
static void extendNetHttp( lua_State* tolua_S )
{
    lua_pushstring( tolua_S, "NetHttp" );
    lua_rawget( tolua_S, LUA_REGISTRYINDEX );
    if( lua_istable( tolua_S, -1 ) )
    {
        lua_pushstring( tolua_S, "request" );
        lua_pushcfunction( tolua_S, tolua_lua_ww_NetHttp_request );
        lua_rawset( tolua_S, -3 );
    }

    lua_pop( tolua_S, 1 );
}



static int tolua_lua_ww_FileManager_registerLoadIndexEventHandler( lua_State* tolua_S )
{
    FileManager* self;
    int argc;
#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
    if( !tolua_isusertype( tolua_S, 1, "FileManager", 0, &tolua_err ) ) goto tolua_lerror;
#endif
    self = static_cast<FileManager*>( tolua_tousertype( tolua_S, 1, 0 ) );

    argc = lua_gettop( tolua_S ) - 1;
    if( argc == 1 )
    {
#if COCOS2D_DEBUG >= 1
        if( !toluafix_isfunction( tolua_S, 2, "LUA_FUNCTION", 0, &tolua_err ) )
            goto tolua_lerror;
#endif
        auto f = toluafix_ref_function( tolua_S, 2, 0 );

        self->registerLoadIndexEventHandler( f );
        return 0;
    }
    CCLOG( "%s has wrong number of arguments: %d, was expecting %d\n ", "FileManager::registerLoadIndexEventHandler", argc, 1 );

#if COCOS2D_DEBUG >= 1
tolua_lerror:
    tolua_error( tolua_S, "#ferror in function 'tolua_lua_ww_FileManager_registerLoadIndexEventHandler'.", &tolua_err );
#endif
    return 0;
}
static int tolua_lua_ww_FileManager_registerDownloadEventHandler( lua_State* tolua_S )
{
    FileManager* self;
    int argc;
#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
    if( !tolua_isusertype( tolua_S, 1, "FileManager", 0, &tolua_err ) ) goto tolua_lerror;
#endif
    self = static_cast<FileManager*>( tolua_tousertype( tolua_S, 1, 0 ) );
    argc = lua_gettop( tolua_S ) - 1;
    if( argc == 1 )
    {
#if COCOS2D_DEBUG >= 1
        if( !toluafix_isfunction( tolua_S, 2, "LUA_FUNCTION", 0, &tolua_err ) )
            goto tolua_lerror;
#endif
        auto f = toluafix_ref_function( tolua_S, 2, 0 );
        self->registerDownloadEventHandler( f );
        return 0;
    }
    CCLOG( "%s has wrong number of arguments: %d, was expecting %d\n ", "FileManager::registerDownloadEventHandler", argc, 1 );
#if COCOS2D_DEBUG >= 1
tolua_lerror:
    tolua_error( tolua_S, "#ferror in function 'tolua_lua_ww_NetHttp_registerDownloadEventHandler'.", &tolua_err );
#endif
    return 0;
}
static int tolua_lua_ww_FileManager_registerLoadEventHandler( lua_State* tolua_S )
{
    FileManager* self;
    int argc;
#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
    if( !tolua_isusertype( tolua_S, 1, "FileManager", 0, &tolua_err ) ) goto tolua_lerror;
#endif
    self = static_cast<FileManager*>( tolua_tousertype( tolua_S, 1, 0 ) );
    argc = lua_gettop( tolua_S ) - 1;
    if( argc == 1 )
    {
#if COCOS2D_DEBUG >= 1
        if( !toluafix_isfunction( tolua_S, 2, "LUA_FUNCTION", 0, &tolua_err ) )
            goto tolua_lerror;
#endif
        auto f = toluafix_ref_function( tolua_S, 2, 0 );
        self->registerLoadEventHandler( f );
        return 0;
    }
    CCLOG( "%s has wrong number of arguments: %d, was expecting %d\n ", "FileManager::registerLoadEventHandler", argc, 1 );
#if COCOS2D_DEBUG >= 1
tolua_lerror:
    tolua_error( tolua_S, "#ferror in function 'tolua_lua_ww_NetHttp_registerLoadIndexEventHandler'.", &tolua_err );
#endif
    return 0;
}
static void extendFileManager( lua_State* tolua_S )
{
    lua_pushstring( tolua_S, "FileManager" );
    lua_rawget( tolua_S, LUA_REGISTRYINDEX );
    if( lua_istable( tolua_S, -1 ) )
    {
        lua_pushstring( tolua_S, "registerLoadIndexEventHandler" );
        lua_pushcfunction( tolua_S, tolua_lua_ww_FileManager_registerLoadIndexEventHandler );
        lua_rawset( tolua_S, -3 );
        lua_pushstring( tolua_S, "registerDownloadEventHandler" );
        lua_pushcfunction( tolua_S, tolua_lua_ww_FileManager_registerDownloadEventHandler );
        lua_rawset( tolua_S, -3 );
        lua_pushstring( tolua_S, "registerLoadEventHandler" );
        lua_pushcfunction( tolua_S, tolua_lua_ww_FileManager_registerLoadEventHandler );
        lua_rawset( tolua_S, -3 );
    }

    lua_pop( tolua_S, 1 );
}



static int tolua_lua_ww_ExtButton_registerTouchEventHandler( lua_State* tolua_S )
{
    if( nullptr == tolua_S )
        return 0;

    int argc = 0;
    ExtButton* self = nullptr;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
    if( !tolua_isusertype( tolua_S, 1, "ExtButton", 0, &tolua_err ) ) goto tolua_lerror;
#endif

    self = static_cast<ExtButton*>( tolua_tousertype( tolua_S, 1, 0 ) );

#if COCOS2D_DEBUG >= 1
    if( nullptr == self )
    {
        tolua_error( tolua_S, "invalid 'self' in function 'tolua_lua_ww_ExtButton_registerTouchEventHandler'\n", NULL );
        return 0;
    }
#endif

    argc = lua_gettop( tolua_S ) - 1;

    if( 1 == argc )
    {
#if COCOS2D_DEBUG >= 1
        if( !toluafix_isfunction( tolua_S, 2, "LUA_FUNCTION", 0, &tolua_err ) )
        {
            goto tolua_lerror;
        }
#endif
        LUA_FUNCTION handler = toluafix_ref_function( tolua_S, 2, 0 );
        self->registerTouchEventHandler( handler );
        return 0;
    }

    CCLOG( "'registerTouchEventHandler' function of Control has wrong number of arguments: %d, was expecting %d\n", argc, 1 );
    return 0;

#if COCOS2D_DEBUG >= 1
tolua_lerror:
    tolua_error( tolua_S, "#ferror in function 'tolua_lua_ww_ExtButton_registerTouchEventHandler'.", &tolua_err );
    return 0;
#endif
}
static int tolua_lua_ww_ExtButton_registerHighlightHandler( lua_State* tolua_S )
{
    if( nullptr == tolua_S )
        return 0;

    int argc = 0;
    ExtButton* self = nullptr;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
    if( !tolua_isusertype( tolua_S, 1, "ExtButton", 0, &tolua_err ) ) goto tolua_lerror;
#endif

    self = static_cast<ExtButton*>( tolua_tousertype( tolua_S, 1, 0 ) );

#if COCOS2D_DEBUG >= 1
    if( nullptr == self )
    {
        tolua_error( tolua_S, "invalid 'self' in function 'tolua_lua_ww_ExtButton_registerHighlightHandler'\n", NULL );
        return 0;
    }
#endif

    argc = lua_gettop( tolua_S ) - 1;

    if( 1 == argc )
    {
#if COCOS2D_DEBUG >= 1
        if( !toluafix_isfunction( tolua_S, 2, "LUA_FUNCTION", 0, &tolua_err ) )
        {
            goto tolua_lerror;
        }
#endif
        LUA_FUNCTION handler = toluafix_ref_function( tolua_S, 2, 0 );
        self->registerHighlightHandler( handler );
        return 0;
    }

    CCLOG( "'registerHighlightHandler' function of Control has wrong number of arguments: %d, was expecting %d\n", argc, 1 );
    return 0;

#if COCOS2D_DEBUG >= 1
tolua_lerror:
    tolua_error( tolua_S, "#ferror in function 'tolua_lua_ww_ExtButton_registerHighlightHandler'.", &tolua_err );
    return 0;
#endif
}
static int tolua_lua_ww_ExtButton_registerEnableHandler( lua_State* tolua_S )
{
    if( nullptr == tolua_S )
        return 0;

    int argc = 0;
    ExtButton* self = nullptr;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
    if( !tolua_isusertype( tolua_S, 1, "ExtButton", 0, &tolua_err ) ) goto tolua_lerror;
#endif

    self = static_cast<ExtButton*>( tolua_tousertype( tolua_S, 1, 0 ) );

#if COCOS2D_DEBUG >= 1
    if( nullptr == self )
    {
        tolua_error( tolua_S, "invalid 'self' in function 'tolua_lua_ww_ExtButton_registerEnableHandler'\n", NULL );
        return 0;
    }
#endif

    argc = lua_gettop( tolua_S ) - 1;

    if( 1 == argc )
    {
#if COCOS2D_DEBUG >= 1
        if( !toluafix_isfunction( tolua_S, 2, "LUA_FUNCTION", 0, &tolua_err ) )
        {
            goto tolua_lerror;
        }
#endif
        LUA_FUNCTION handler = toluafix_ref_function( tolua_S, 2, 0 );
        self->registerEnableHandler( handler );
        return 0;
    }

    CCLOG( "'registerEnableHandler' function of Control has wrong number of arguments: %d, was expecting %d\n", argc, 1 );
    return 0;

#if COCOS2D_DEBUG >= 1
tolua_lerror:
    tolua_error( tolua_S, "#ferror in function 'tolua_lua_ww_ExtButton_registerEnableHandler'.", &tolua_err );
    return 0;
#endif
}
static void extendExtButton( lua_State* tolua_S )
{
    lua_pushstring( tolua_S, "ExtButton" );
    lua_rawget( tolua_S, LUA_REGISTRYINDEX );
    if( lua_istable( tolua_S, -1 ) )
    {
        lua_pushstring( tolua_S, "registerTouchEventHandler" );
        lua_pushcfunction( tolua_S, tolua_lua_ww_ExtButton_registerTouchEventHandler );
        lua_rawset( tolua_S, -3 );
        lua_pushstring( tolua_S, "registerHighlightHandler" );
        lua_pushcfunction( tolua_S, tolua_lua_ww_ExtButton_registerHighlightHandler );
        lua_rawset( tolua_S, -3 );
        lua_pushstring( tolua_S, "registerEnableHandler" );
        lua_pushcfunction( tolua_S, tolua_lua_ww_ExtButton_registerEnableHandler );
        lua_rawset( tolua_S, -3 );
    }

    lua_pop( tolua_S, 1 );
}



static int tolua_lua_ww_ExtScrollView_registerActionEventHandler( lua_State* tolua_S )
{
    if( nullptr == tolua_S )
        return 0;

    int argc = 0;
    ExtScrollView* self = nullptr;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
    if( !tolua_isusertype( tolua_S, 1, "ExtScrollView", 0, &tolua_err ) ) goto tolua_lerror;
#endif

    self = static_cast<ExtScrollView*>( tolua_tousertype( tolua_S, 1, 0 ) );

#if COCOS2D_DEBUG >= 1
    if( nullptr == self )
    {
        tolua_error( tolua_S, "invalid 'self' in function 'tolua_lua_ww_ExtScrollView_registerActionEventHandler'\n", NULL );
        return 0;
    }
#endif

    argc = lua_gettop( tolua_S ) - 1;

    if( 1 == argc )
    {
#if COCOS2D_DEBUG >= 1
        if( !toluafix_isfunction( tolua_S, 2, "LUA_FUNCTION", 0, &tolua_err ) )
        {
            goto tolua_lerror;
        }
#endif
        LUA_FUNCTION handler = toluafix_ref_function( tolua_S, 2, 0 );
        self->registerActionEventHandler( handler );
        return 0;
    }

    CCLOG( "'registerActionEventHandler' function of Control has wrong number of arguments: %d, was expecting %d\n", argc, 1 );
    return 0;

#if COCOS2D_DEBUG >= 1
tolua_lerror:
    tolua_error( tolua_S, "#ferror in function 'tolua_lua_ww_ExtScrollView_registerActionEventHandler'.", &tolua_err );
    return 0;
#endif
}
static void extendExtScrollView( lua_State* tolua_S )
{
    lua_pushstring( tolua_S, "ExtScrollView" );
    lua_rawget( tolua_S, LUA_REGISTRYINDEX );
    if( lua_istable( tolua_S, -1 ) )
    {
        lua_pushstring( tolua_S, "registerActionEventHandler" );
        lua_pushcfunction( tolua_S, tolua_lua_ww_ExtScrollView_registerActionEventHandler );
        lua_rawset( tolua_S, -3 );
    }

    lua_pop( tolua_S, 1 );
}




static int tolua_lua_ww_Hosts_registerGameLoopEventHandler( lua_State* tolua_S )
{
    int argc = 0;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif

#if COCOS2D_DEBUG >= 1
    if( !tolua_isusertype( tolua_S, 1, "Hosts", 0, &tolua_err ) ) goto tolua_lerror;
#endif

    argc = lua_gettop( tolua_S ) - 1;

    if( argc == 2 )
    {
        string arg0;
        LUA_FUNCTION arg1;

#if COCOS2D_DEBUG >= 1
        if( !tolua_isstring( tolua_S, 2, 0, &tolua_err ) )
            goto tolua_lerror;
        if( !toluafix_isfunction( tolua_S, 3, "LUA_FUNCTION", 0, &tolua_err ) )
            goto tolua_lerror;
#endif

        arg0 = tolua_tocppstring( tolua_S, 2, 0 );
        arg1 = toluafix_ref_function( tolua_S, 3, 0 );

        Hosts::getInstance()->registerGameLoopEventHandler( arg0, arg1 );
        return 0;
    }
    CCLOG( "%s has wrong number of arguments: %d, was expecting %d\n ", "Hosts::registerGameLoopEventHandler", argc, 1 );
    return 0;
#if COCOS2D_DEBUG >= 1
tolua_lerror:
    tolua_error( tolua_S, "#ferror in function 'tolua_lua_ww_Hosts_registerGameLoopEventHandler'.", &tolua_err );
#endif
    return 0;
}
static int tolua_lua_ww_Hosts_registerToForegroundEventHandler( lua_State* tolua_S )
{
    int argc = 0;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif

#if COCOS2D_DEBUG >= 1
    if( !tolua_isusertype( tolua_S, 1, "Hosts", 0, &tolua_err ) ) goto tolua_lerror;
#endif

    argc = lua_gettop( tolua_S ) - 1;

    if( argc == 1 )
    {
        LUA_FUNCTION arg0;

#if COCOS2D_DEBUG >= 1
        if( !toluafix_isfunction( tolua_S, 2, "LUA_FUNCTION", 0, &tolua_err )
            && !lua_isnil( tolua_S, 2 ) 
            )
            goto tolua_lerror;
#endif

        if( lua_isnil( tolua_S, 2 ) )
        {
            Hosts::getInstance()->registerToForegroundEventHandler( 0 );
        }
        else
        {
            arg0 = toluafix_ref_function( tolua_S, 2, 0 );

            Hosts::getInstance()->registerToForegroundEventHandler( arg0 );
        }
        return 0;
    }
    CCLOG( "%s has wrong number of arguments: %d, was expecting %d\n ", "Hosts::registerToForegroundEventHandler", argc, 1 );
    return 0;
#if COCOS2D_DEBUG >= 1
tolua_lerror:
    tolua_error( tolua_S, "#ferror in function 'tolua_lua_ww_Hosts_registerToForegroundEventHandler'.", &tolua_err );
#endif
    return 0;
}
static int tolua_lua_ww_Hosts_registerToBackgroundEventHandler( lua_State* tolua_S )
{
    int argc = 0;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif

#if COCOS2D_DEBUG >= 1
    if( !tolua_isusertype( tolua_S, 1, "Hosts", 0, &tolua_err ) ) goto tolua_lerror;
#endif

    argc = lua_gettop( tolua_S ) - 1;

    if( argc == 1 )
    {
        LUA_FUNCTION arg0;

#if COCOS2D_DEBUG >= 1
        if( !toluafix_isfunction( tolua_S, 2, "LUA_FUNCTION", 0, &tolua_err )
            && !lua_isnil( tolua_S, 2 )
            )
            goto tolua_lerror;
#endif
       
        if( lua_isnil( tolua_S, 2 ) )
        {
            Hosts::getInstance()->registerToBackgroundEventHandler( 0 );
        }
        else
        {
            arg0 = toluafix_ref_function( tolua_S, 2, 0 );
            Hosts::getInstance()->registerToBackgroundEventHandler( arg0 );
        }
        return 0;
    }
    CCLOG( "%s has wrong number of arguments: %d, was expecting %d\n ", "Hosts::registerToBackgroundEventHandler", argc, 1 );
    return 0;
#if COCOS2D_DEBUG >= 1
tolua_lerror:
    tolua_error( tolua_S, "#ferror in function 'tolua_lua_ww_Hosts_registerToBackgroundEventHandler'.", &tolua_err );
#endif
    return 0;
}
static int tolua_lua_ww_Hosts_getVersion( lua_State* tolua_S )
{
    CCLOG( "************ enter static int tolua_lua_ww_Hosts_getVersion(lua_State* tolua_S);" );

	int argc = 0;
	Hosts* cobj = nullptr;
	bool ok  = true;

#if COCOS2D_DEBUG >= 1
	tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
	if (!tolua_isusertype(tolua_S,1,"Hosts",0,&tolua_err)) goto tolua_lerror;
#endif

	cobj = (Hosts*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
	if (!cobj) 
	{
		tolua_error(tolua_S,"invalid 'cobj' in function 'tolua_lua_ww_Hosts_getVersion'", NULL);
		return 0;
	}
#endif

	argc = lua_gettop(tolua_S)-1;
	if (argc == 0) 
	{
		if(!ok)
			return 0;
        CCLOG( "************ before const std::pair<int, int>& ret = cobj->getVersion();" );
		const std::pair<int, int>& ret = cobj->getVersion();
		lua_newtable( Global::L );
		LuaHelper::push( "major" );
		LuaHelper::push( ret.first );
		lua_settable( Global::L, -3 );
		LuaHelper::push( "minor" );
		LuaHelper::push( ret.second );
		lua_settable( Global::L, -3 );
		return 1;
	}
	CCLOG("%s has wrong number of arguments: %d, was expecting %d \n", "getVersion",argc, 0);
	return 0;

#if COCOS2D_DEBUG >= 1
tolua_lerror:
	tolua_error(tolua_S,"#ferror in function 'tolua_lua_ww_Hosts_getVersion'.",&tolua_err);
#endif

	return 0;
}
static void extendHosts( lua_State* tolua_S )
{
    lua_pushstring( tolua_S, "Hosts" );
    lua_rawget( tolua_S, LUA_REGISTRYINDEX );
    if( lua_istable( tolua_S, -1 ) )
    {
        lua_pushstring( tolua_S, "registerGameLoopEventHandler" );
        lua_pushcfunction( tolua_S, tolua_lua_ww_Hosts_registerGameLoopEventHandler );
        lua_rawset( tolua_S, -3 );
        lua_pushstring( tolua_S, "registerToBackgroundEventHandler" );
        lua_pushcfunction( tolua_S, tolua_lua_ww_Hosts_registerToBackgroundEventHandler );
        lua_rawset( tolua_S, -3 );
        lua_pushstring( tolua_S, "registerToForegroundEventHandler" );
        lua_pushcfunction( tolua_S, tolua_lua_ww_Hosts_registerToForegroundEventHandler );
        lua_rawset( tolua_S, -3 );
        lua_pushstring( tolua_S, "getVersion" );
		lua_pushcfunction( tolua_S, tolua_lua_ww_Hosts_getVersion );
		lua_rawset( tolua_S, -3 );
    }

    lua_pop( tolua_S, 1 );
}



static int tolua_lua_ww_NetClient_registerNetEventHandler( lua_State* tolua_S )
{
    int argc = 0;
    NetClient *cobj = 0;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif

#if COCOS2D_DEBUG >= 1
    if( !tolua_isusertype( tolua_S, 1, "NetClient", 0, &tolua_err ) ) goto tolua_lerror;
#endif

    argc = lua_gettop( tolua_S ) - 1;

    cobj = (NetClient *)tolua_tousertype( tolua_S, 1, 0 );


    if( argc == 1 )
    {
        LUA_FUNCTION arg0;
#if COCOS2D_DEBUG >= 1
        if( !toluafix_isfunction( tolua_S, 2, "LUA_FUNCTION", 0, &tolua_err ) )
            goto tolua_lerror;
#endif
        arg0 = toluafix_ref_function( tolua_S, 2, 0 );

        cobj->registerNetEventHandler( arg0 );
        return 0;
    }
    CCLOG( "%s has wrong number of arguments: %d, was expecting %d\n ", "NetClient::registerNetEventHandler", argc, 1 );
    return 0;
#if COCOS2D_DEBUG >= 1
tolua_lerror:
    tolua_error( tolua_S, "#ferror in function 'tolua_lua_ww_NetClient_registerNetEventHandler'.", &tolua_err );
#endif
    return 0;
}
static void extendNetClient( lua_State* tolua_S )
{
    lua_pushstring( tolua_S, "NetClient" );
    lua_rawget( tolua_S, LUA_REGISTRYINDEX );
    if( lua_istable( tolua_S, -1 ) )
    {
        lua_pushstring( tolua_S, "registerNetEventHandler" );
        lua_pushcfunction( tolua_S, tolua_lua_ww_NetClient_registerNetEventHandler );
        lua_rawset( tolua_S, -3 );
    }

    lua_pop( tolua_S, 1 );
}






static int tolua_lua_ww_RichTextEx_registerTouchEventHandler( lua_State* tolua_S )
{
    if( nullptr == tolua_S )
        return 0;

    int argc = 0;
    RichTextEx* self = nullptr;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
    if( !tolua_isusertype( tolua_S, 1, "RichTextEx", 0, &tolua_err ) ) goto tolua_lerror;
#endif

    self = static_cast<RichTextEx*>( tolua_tousertype( tolua_S, 1, 0 ) );

#if COCOS2D_DEBUG >= 1
    if( nullptr == self )
    {
        tolua_error( tolua_S, "invalid 'self' in function 'tolua_lua_ww_RichTextEx_registerTouchEventHandler'\n", NULL );
        return 0;
    }
#endif

    argc = lua_gettop( tolua_S ) - 1;

    if( 1 == argc )
    {
#if COCOS2D_DEBUG >= 1
        if( !toluafix_isfunction( tolua_S, 2, "LUA_FUNCTION", 0, &tolua_err ) )
        {
            goto tolua_lerror;
        }
#endif
        LUA_FUNCTION handler = toluafix_ref_function( tolua_S, 2, 0 );
        self->registerTouchEventHandler( handler );
        return 0;
    }

    CCLOG( "'registerTouchEventHandler' function of Control has wrong number of arguments: %d, was expecting %d\n", argc, 1 );
    return 0;

#if COCOS2D_DEBUG >= 1
tolua_lerror:
    tolua_error( tolua_S, "#ferror in function 'tolua_lua_ww_RichTextEx_registerTouchEventHandler'.", &tolua_err );
    return 0;
#endif
}
static int tolua_lua_ww_RichTextEx_registerVariantEventHandler( lua_State* tolua_S )
{
    if( nullptr == tolua_S )
        return 0;

    int argc = 0;
    RichTextEx* self = nullptr;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
    if( !tolua_isusertype( tolua_S, 1, "RichTextEx", 0, &tolua_err ) ) goto tolua_lerror;
#endif

    self = static_cast<RichTextEx*>( tolua_tousertype( tolua_S, 1, 0 ) );

#if COCOS2D_DEBUG >= 1
    if( nullptr == self )
    {
        tolua_error( tolua_S, "invalid 'self' in function 'tolua_lua_ww_RichTextEx_registerVariantEventHandler'\n", NULL );
        return 0;
    }
#endif

    argc = lua_gettop( tolua_S ) - 1;

    if( 1 == argc )
    {
#if COCOS2D_DEBUG >= 1
        if( !toluafix_isfunction( tolua_S, 2, "LUA_FUNCTION", 0, &tolua_err ) )
        {
            goto tolua_lerror;
        }
#endif
        LUA_FUNCTION handler = toluafix_ref_function( tolua_S, 2, 0 );
        self->registerVariantEventHandler( handler );
        return 0;
    }

    CCLOG( "'registerVariantEventHandler' function of Control has wrong number of arguments: %d, was expecting %d\n", argc, 1 );
    return 0;

#if COCOS2D_DEBUG >= 1
tolua_lerror:
    tolua_error( tolua_S, "#ferror in function 'tolua_lua_ww_RichTextEx_registerVariantEventHandler'.", &tolua_err );
    return 0;
#endif
}
static int tolua_lua_ww_RichTextEx_registerErrorEventHandler( lua_State* tolua_S )
{
    if( nullptr == tolua_S )
        return 0;

    int argc = 0;
    RichTextEx* self = nullptr;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
    if( !tolua_isusertype( tolua_S, 1, "RichTextEx", 0, &tolua_err ) ) goto tolua_lerror;
#endif

    self = static_cast<RichTextEx*>( tolua_tousertype( tolua_S, 1, 0 ) );

#if COCOS2D_DEBUG >= 1
    if( nullptr == self )
    {
        tolua_error( tolua_S, "invalid 'self' in function 'tolua_lua_ww_RichTextEx_registerErrorEventHandler'\n", NULL );
        return 0;
    }
#endif

    argc = lua_gettop( tolua_S ) - 1;

    if( 1 == argc )
    {
#if COCOS2D_DEBUG >= 1
        if( !toluafix_isfunction( tolua_S, 2, "LUA_FUNCTION", 0, &tolua_err ) )
        {
            goto tolua_lerror;
        }
#endif
        LUA_FUNCTION handler = toluafix_ref_function( tolua_S, 2, 0 );
        self->registerErrorEventHandler( handler );
        return 0;
    }

    CCLOG( "'registerErrorEventHandler' function of Control has wrong number of arguments: %d, was expecting %d\n", argc, 1 );
    return 0;

#if COCOS2D_DEBUG >= 1
tolua_lerror:
    tolua_error( tolua_S, "#ferror in function 'tolua_lua_ww_RichTextEx_registerErrorEventHandler'.", &tolua_err );
    return 0;
#endif
}
static int tolua_lua_ww_RichTextEx_create( lua_State* tolua_S )
{
    int argc = 0;
    bool ok = true;
#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif

#if COCOS2D_DEBUG >= 1
    if( !tolua_isusertable( tolua_S, 1, "RichTextEx", 0, &tolua_err ) ) goto tolua_lerror;
#endif

    argc = lua_gettop( tolua_S ) - 1;

    do
    {
        if( argc == 2 )
        {
            std::string arg0;
            ok &= luaval_to_std_string( tolua_S, 2, &arg0 );
            if( !ok ) { break; }
            double arg1;
            ok &= luaval_to_number( tolua_S, 3, &arg1 );
            if( !ok ) { break; }
            RichTextEx* ret = RichTextEx::createLua( arg0, (float)arg1 );
            object_to_luaval<RichTextEx>( tolua_S, "RichTextEx", (RichTextEx*)ret );
            return 1;
        }
    } while( 0 );
    ok = true;
    do
    {
        if( argc == 3 )
        {
            std::string arg0;
            ok &= luaval_to_std_string( tolua_S, 2, &arg0 );
            if( !ok ) { break; }
            double arg1;
            ok &= luaval_to_number( tolua_S, 3, &arg1 );
            if( !ok ) { break; }
            LUA_FUNCTION arg2;
#if COCOS2D_DEBUG >= 1
//            if( !toluafix_isfunction( tolua_S, 4, "LUA_FUNCTION", 0, &tolua_error ) )
//            {
//                ok = false;
//                break;
//            }
#endif
            arg2 = toluafix_ref_function( tolua_S, 4, 0 );
            RichTextEx* ret = RichTextEx::createLua( arg0, (float)arg1, arg2 );
            object_to_luaval<RichTextEx>( tolua_S, "RichTextEx", (RichTextEx*)ret );
            return 1;
        }
    } while( 0 );
    ok = true;
    do
    {
        if( argc == 4 )
        {
            std::string arg0;
            ok &= luaval_to_std_string( tolua_S, 2, &arg0 );
            if( !ok ) { break; }
            double arg1;
            ok &= luaval_to_number( tolua_S, 3, &arg1 );
            if( !ok ) { break; }
            LUA_FUNCTION arg2;
#if COCOS2D_DEBUG >= 1
            if( !toluafix_isfunction( tolua_S, 4, "LUA_FUNCTION", 0, &tolua_err ) )
            {
                ok = false;
                break;
            }
#endif
            arg2 = toluafix_ref_function( tolua_S, 4, 0 );

            LUA_FUNCTION arg3;
#if COCOS2D_DEBUG >= 1
            if( !toluafix_isfunction( tolua_S, 5, "LUA_FUNCTION", 0, &tolua_err ) )
            {
                ok = false;
                break;
            }
#endif
            arg3 = toluafix_ref_function( tolua_S, 5, 0 );
            RichTextEx* ret = RichTextEx::createLua( arg0, (float)arg1, arg2, arg3 );
            object_to_luaval<RichTextEx>( tolua_S, "RichTextEx", (RichTextEx*)ret );
            return 1;
        }
    } while( 0 );
    ok = true;
    do
    {
        if( argc == 5 )
        {
            std::string arg0;
            ok &= luaval_to_std_string( tolua_S, 2, &arg0 );
            if( !ok ) { break; }
            double arg1;
            ok &= luaval_to_number( tolua_S, 3, &arg1 );
            if( !ok ) { break; }

            LUA_FUNCTION arg2;
#if COCOS2D_DEBUG >= 1
            if( !toluafix_isfunction( tolua_S, 4, "LUA_FUNCTION", 0, &tolua_err ) )
            {
                ok = false;
                break;
            }
#endif
            arg2 = toluafix_ref_function( tolua_S, 4, 0 );

            LUA_FUNCTION arg3;
#if COCOS2D_DEBUG >= 1
            if( !toluafix_isfunction( tolua_S, 5, "LUA_FUNCTION", 0, &tolua_err ) )
            {
                ok = false;
                break;
            }
#endif
            arg3 = toluafix_ref_function( tolua_S, 5, 0 );

            LUA_FUNCTION arg4;
#if COCOS2D_DEBUG >= 1
            if( !toluafix_isfunction( tolua_S, 6, "LUA_FUNCTION", 0, &tolua_err ) )
            {
                ok = false;
                break;
            }
#endif
            arg4 = toluafix_ref_function( tolua_S, 6, 0 );

            RichTextEx* ret = RichTextEx::createLua( arg0, (float)arg1, arg2, arg3, arg4 );
            object_to_luaval<RichTextEx>( tolua_S, "RichTextEx", (RichTextEx*)ret );
            return 1;
        }
    } while( 0 );
    ok = true;
    CCLOG( "%s has wrong number of arguments: %d, was expecting %d", "create", argc, 2 );
    return 0;
#if COCOS2D_DEBUG >= 1
tolua_lerror:
    tolua_error( tolua_S, "#ferror in function 'lua_ww_RichTextEx_create'.", &tolua_err );
#endif
    return 0;
}

static void extendRichTextEx( lua_State* tolua_S )
{
    lua_pushstring( tolua_S, "RichTextEx" );
    lua_rawget( tolua_S, LUA_REGISTRYINDEX );
    if( lua_istable( tolua_S, -1 ) )
    {
        lua_pushstring( tolua_S, "registerTouchEventHandler" );
        lua_pushcfunction( tolua_S, tolua_lua_ww_RichTextEx_registerTouchEventHandler );
        lua_rawset( tolua_S, -3 );
        lua_pushstring( tolua_S, "registerVariantEventHandler" );
        lua_pushcfunction( tolua_S, tolua_lua_ww_RichTextEx_registerVariantEventHandler );
        lua_rawset( tolua_S, -3 );
        lua_pushstring( tolua_S, "registerErrorEventHandler" );
        lua_pushcfunction( tolua_S, tolua_lua_ww_RichTextEx_registerErrorEventHandler );
        lua_rawset( tolua_S, -3 );
        lua_pushstring( tolua_S, "create" );
        lua_pushcfunction( tolua_S, tolua_lua_ww_RichTextEx_create );
        lua_rawset( tolua_S, -3 );
    }

    lua_pop( tolua_S, 1 );
}




static int tolua_lua_ww_Purchase_registerEventHandler( lua_State* tolua_S )
{
    if( nullptr == tolua_S )
        return 0;

    int argc = 0;
    Purchase* self = nullptr;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
    if( !tolua_isusertype( tolua_S, 1, "Purchase", 0, &tolua_err ) ) goto tolua_lerror;
#endif

    self = static_cast<Purchase*>( tolua_tousertype( tolua_S, 1, 0 ) );

#if COCOS2D_DEBUG >= 1
    if( nullptr == self )
    {
        tolua_error( tolua_S, "invalid 'self' in function 'tolua_lua_ww_Purchase_registerEventHandler'\n", NULL );
        return 0;
    }
#endif

    argc = lua_gettop( tolua_S ) - 1;

    if( 1 == argc )
    {
#if COCOS2D_DEBUG >= 1
        if( !toluafix_isfunction( tolua_S, 2, "LUA_FUNCTION", 0, &tolua_err ) )
        {
            goto tolua_lerror;
        }
#endif
        LUA_FUNCTION handler = toluafix_ref_function( tolua_S, 2, 0 );
        self->registerEventHandler( handler );
        return 0;
    }

    CCLOG( "'registerEventHandler' function of Control has wrong number of arguments: %d, was expecting %d\n", argc, 1 );
    return 0;

#if COCOS2D_DEBUG >= 1
tolua_lerror:
    tolua_error( tolua_S, "#ferror in function 'tolua_lua_ww_Purchase_registerEventHandler'.", &tolua_err );
    return 0;
#endif
}

static void extendPurchase( lua_State* tolua_S )
{
    lua_pushstring( tolua_S, "Purchase" );
    lua_rawget( tolua_S, LUA_REGISTRYINDEX );
    if( lua_istable( tolua_S, -1 ) )
    {
        lua_pushstring( tolua_S, "registerEventHandler" );
        lua_pushcfunction( tolua_S, tolua_lua_ww_Purchase_registerEventHandler );
        lua_rawset( tolua_S, -3 );
    }

    lua_pop( tolua_S, 1 );
}


static int tolua_lua_ww_ChatSys_registerToChatEventHandler(lua_State* tolua_S)
{
	if (nullptr == tolua_S)
		return 0;

	int argc = 0;
	ChatSys* self = nullptr;

#if COCOS2D_DEBUG >= 1
	tolua_Error tolua_err;
	if (!tolua_isusertype(tolua_S, 1, "ChatSys", 0, &tolua_err)) goto tolua_lerror;
#endif

	self = static_cast<ChatSys*>(tolua_tousertype(tolua_S, 1, 0));

#if COCOS2D_DEBUG >= 1
	if (nullptr == self)
	{
		tolua_error(tolua_S, "invalid 'self' in function 'tolua_lua_ww_ChatSys_registerToChatEventHandler'\n", NULL);
		return 0;
	}
#endif

	argc = lua_gettop(tolua_S) - 1;

	if (1 == argc)
	{
#if COCOS2D_DEBUG >= 1
		if (!toluafix_isfunction(tolua_S, 2, "LUA_FUNCTION", 0, &tolua_err))
		{
			goto tolua_lerror;
		}
#endif
		LUA_FUNCTION handler = toluafix_ref_function(tolua_S, 2, 0);
		self->registerToChatEventHandler(handler);
		return 0;
	}

	CCLOG("'registerToChatEventHandler' function of Control has wrong number of arguments: %d, was expecting %d\n", argc, 1);
	return 0;

#if COCOS2D_DEBUG >= 1
tolua_lerror:
	tolua_error(tolua_S, "#ferror in function 'tolua_lua_ww_ChatSys_registerToChatEventHandler'.", &tolua_err);
	return 0;
#endif
}


static void extendChatSys( lua_State* tolua_S )
{
    lua_pushstring( tolua_S, "ChatSys" );
    lua_rawget( tolua_S, LUA_REGISTRYINDEX );
    if( lua_istable( tolua_S, -1 ) )
    {
        lua_pushstring( tolua_S, "registerToChatEventHandler" );
        lua_pushcfunction( tolua_S, tolua_lua_ww_ChatSys_registerToChatEventHandler );
        lua_rawset( tolua_S, -3 );
    }

    lua_pop( tolua_S, 1 );
}

static int ww_band(lua_State *ls)
{
    int v1 = (int)luaL_checknumber(ls, 1);
    int v2 = (int)luaL_checknumber(ls, 2);

    lua_pushnumber(ls, (v1 & v2));
    return 1;
}

static int ww_bor(lua_State *ls)
{
    int v1 = (int)luaL_checknumber(ls, 1);
    int v2 = (int)luaL_checknumber(ls, 2);

    lua_pushnumber(ls, (v1 | v2));
    return 1;
}

static int ww_bnot(lua_State *ls)
{
    int v1 = (int)luaL_checknumber(ls, 1);

    lua_pushnumber(ls, (!v1));
    return 1;
}

static int ww_btest(lua_State *ls)
{
    int v = (int)luaL_checknumber(ls, 1);
    int f = (int)luaL_checknumber(ls, 2);

    lua_pushboolean(ls, (v & f));
    return 1;
}

static int ww_lshift(lua_State *ls)
{
    int v1 = (int)luaL_checknumber(ls, 1);
    int v2 = (int)luaL_checknumber(ls, 2);

    lua_pushnumber(ls, v1 << v2);
    return 1;
}

static int ww_rshift(lua_State *ls)
{
    int v1 = (int)luaL_checknumber(ls, 1);
    int v2 = (int)luaL_checknumber(ls, 2);

    lua_pushnumber(ls, v1 >> v2);
    return 1;
}

void extendStaticFunc(lua_State *ls)
{
    lua_pushstring( ls, "band" );   lua_pushcfunction( ls, ww_band   ); lua_rawset( ls, -3 );
    lua_pushstring( ls, "bor" );    lua_pushcfunction( ls, ww_bor    ); lua_rawset( ls, -3 );
    lua_pushstring( ls, "bnot" );   lua_pushcfunction( ls, ww_bnot   ); lua_rawset( ls, -3 );
    lua_pushstring( ls, "btest" );  lua_pushcfunction( ls, ww_btest  ); lua_rawset( ls, -3 );
    lua_pushstring( ls, "lshift" ); lua_pushcfunction( ls, ww_lshift ); lua_rawset( ls, -3 );
    lua_pushstring( ls, "rshift" ); lua_pushcfunction( ls, ww_rshift ); lua_rawset( ls, -3 );
}

int register_all_ww_manual( lua_State* tolua_S )
{
    if( nullptr == tolua_S )
        return 0;

    tolua_beginmodule( tolua_S, "ww" );

    extendStaticFunc( tolua_S );
    extendHosts( tolua_S );
    extendNetClient( tolua_S );
    extendNetHttp( tolua_S );
    extendFileManager( tolua_S );
    extendExtButton( tolua_S );
    extendExtScrollView( tolua_S );
    extendRichTextEx( tolua_S );
    extendPurchase( tolua_S );
    extendChatSys( tolua_S );

    tolua_endmodule( tolua_S );
    return 0;
}
