#include "lua_ww_auto.hpp"
#include "Precompile.h"
#include "tolua_fix.h"
#include "LuaBasicConversions.h"



int lua_ww_FileCache_clear(lua_State* tolua_S)
{
    int argc = 0;
    FileCache* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"FileCache",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (FileCache*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_ww_FileCache_clear'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 0) 
    {
        if(!ok)
            return 0;
        cobj->clear();
        return 0;
    }
    CCLOG("%s has wrong number of arguments: %d, was expecting %d \n", "clear",argc, 0);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_ww_FileCache_clear'.",&tolua_err);
#endif

    return 0;
}
int lua_ww_FileCache_getInstance(lua_State* tolua_S)
{
    int argc = 0;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif

#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertable(tolua_S,1,"FileCache",0,&tolua_err)) goto tolua_lerror;
#endif

    argc = lua_gettop(tolua_S) - 1;

    if (argc == 0)
    {
        if(!ok)
            return 0;
        FileCache* ret = FileCache::getInstance();
        object_to_luaval<FileCache>(tolua_S, "FileCache",(FileCache*)ret);
        return 1;
    }
    CCLOG("%s has wrong number of arguments: %d, was expecting %d\n ", "getInstance",argc, 0);
    return 0;
#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_ww_FileCache_getInstance'.",&tolua_err);
#endif
    return 0;
}
static int lua_ww_FileCache_finalize(lua_State* tolua_S)
{
    printf("luabindings: finalizing LUA object (FileCache)");
    return 0;
}

int lua_register_ww_FileCache(lua_State* tolua_S)
{
    tolua_usertype(tolua_S,"FileCache");
    tolua_cclass(tolua_S,"FileCache","FileCache","",nullptr);

    tolua_beginmodule(tolua_S,"FileCache");
        tolua_function(tolua_S,"clear",lua_ww_FileCache_clear);
        tolua_function(tolua_S,"getInstance", lua_ww_FileCache_getInstance);
    tolua_endmodule(tolua_S);
    std::string typeName = typeid(FileCache).name();
    g_luaType[typeName] = "FileCache";
    g_typeCast["FileCache"] = "FileCache";
    return 1;
}

int lua_ww_NodeDumper_dumpLayer(lua_State* tolua_S)
{
    int argc = 0;
    NodeDumper* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"NodeDumper",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (NodeDumper*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_ww_NodeDumper_dumpLayer'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 3) 
    {
        cocos2d::Node* arg0;
        std::string arg1;
        std::string arg2;

        ok &= luaval_to_object<cocos2d::Node>(tolua_S, 2, "cc.Node",&arg0);

        ok &= luaval_to_std_string(tolua_S, 3,&arg1);

        ok &= luaval_to_std_string(tolua_S, 4,&arg2);
        if(!ok)
            return 0;
        bool ret = cobj->dumpLayer(arg0, arg1, arg2);
        tolua_pushboolean(tolua_S,(bool)ret);
        return 1;
    }
    if (argc == 4) 
    {
        cocos2d::Node* arg0;
        std::string arg1;
        std::string arg2;
        std::string arg3;

        ok &= luaval_to_object<cocos2d::Node>(tolua_S, 2, "cc.Node",&arg0);

        ok &= luaval_to_std_string(tolua_S, 3,&arg1);

        ok &= luaval_to_std_string(tolua_S, 4,&arg2);

        ok &= luaval_to_std_string(tolua_S, 5,&arg3);
        if(!ok)
            return 0;
        bool ret = cobj->dumpLayer(arg0, arg1, arg2, arg3);
        tolua_pushboolean(tolua_S,(bool)ret);
        return 1;
    }
    if (argc == 5) 
    {
        cocos2d::Node* arg0;
        std::string arg1;
        std::string arg2;
        std::string arg3;
        std::string arg4;

        ok &= luaval_to_object<cocos2d::Node>(tolua_S, 2, "cc.Node",&arg0);

        ok &= luaval_to_std_string(tolua_S, 3,&arg1);

        ok &= luaval_to_std_string(tolua_S, 4,&arg2);

        ok &= luaval_to_std_string(tolua_S, 5,&arg3);

        ok &= luaval_to_std_string(tolua_S, 6,&arg4);
        if(!ok)
            return 0;
        bool ret = cobj->dumpLayer(arg0, arg1, arg2, arg3, arg4);
        tolua_pushboolean(tolua_S,(bool)ret);
        return 1;
    }
    if (argc == 6) 
    {
        cocos2d::Node* arg0;
        std::string arg1;
        std::string arg2;
        std::string arg3;
        std::string arg4;
        bool arg5;

        ok &= luaval_to_object<cocos2d::Node>(tolua_S, 2, "cc.Node",&arg0);

        ok &= luaval_to_std_string(tolua_S, 3,&arg1);

        ok &= luaval_to_std_string(tolua_S, 4,&arg2);

        ok &= luaval_to_std_string(tolua_S, 5,&arg3);

        ok &= luaval_to_std_string(tolua_S, 6,&arg4);

        ok &= luaval_to_boolean(tolua_S, 7,&arg5);
        if(!ok)
            return 0;
        bool ret = cobj->dumpLayer(arg0, arg1, arg2, arg3, arg4, arg5);
        tolua_pushboolean(tolua_S,(bool)ret);
        return 1;
    }
    CCLOG("%s has wrong number of arguments: %d, was expecting %d \n", "dumpLayer",argc, 3);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_ww_NodeDumper_dumpLayer'.",&tolua_err);
#endif

    return 0;
}
int lua_ww_NodeDumper_saveToFile(lua_State* tolua_S)
{
    int argc = 0;
    NodeDumper* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"NodeDumper",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (NodeDumper*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_ww_NodeDumper_saveToFile'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 1) 
    {
        std::string arg0;

        ok &= luaval_to_std_string(tolua_S, 2,&arg0);
        if(!ok)
            return 0;
        cobj->saveToFile(arg0);
        return 0;
    }
    CCLOG("%s has wrong number of arguments: %d, was expecting %d \n", "saveToFile",argc, 1);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_ww_NodeDumper_saveToFile'.",&tolua_err);
#endif

    return 0;
}
int lua_ww_NodeDumper_dump(lua_State* tolua_S)
{
    int argc = 0;
    NodeDumper* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"NodeDumper",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (NodeDumper*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_ww_NodeDumper_dump'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 1) 
    {
        cocos2d::Node* arg0;

        ok &= luaval_to_object<cocos2d::Node>(tolua_S, 2, "cc.Node",&arg0);
        if(!ok)
            return 0;
        cobj->dump(arg0);
        return 0;
    }
    if (argc == 2) 
    {
        cocos2d::Node* arg0;
        std::string arg1;

        ok &= luaval_to_object<cocos2d::Node>(tolua_S, 2, "cc.Node",&arg0);

        ok &= luaval_to_std_string(tolua_S, 3,&arg1);
        if(!ok)
            return 0;
        cobj->dump(arg0, arg1);
        return 0;
    }
    if (argc == 3) 
    {
        cocos2d::Node* arg0;
        std::string arg1;
        std::string arg2;

        ok &= luaval_to_object<cocos2d::Node>(tolua_S, 2, "cc.Node",&arg0);

        ok &= luaval_to_std_string(tolua_S, 3,&arg1);

        ok &= luaval_to_std_string(tolua_S, 4,&arg2);
        if(!ok)
            return 0;
        cobj->dump(arg0, arg1, arg2);
        return 0;
    }
    if (argc == 4) 
    {
        cocos2d::Node* arg0;
        std::string arg1;
        std::string arg2;
        std::string arg3;

        ok &= luaval_to_object<cocos2d::Node>(tolua_S, 2, "cc.Node",&arg0);

        ok &= luaval_to_std_string(tolua_S, 3,&arg1);

        ok &= luaval_to_std_string(tolua_S, 4,&arg2);

        ok &= luaval_to_std_string(tolua_S, 5,&arg3);
        if(!ok)
            return 0;
        cobj->dump(arg0, arg1, arg2, arg3);
        return 0;
    }
    if (argc == 5) 
    {
        cocos2d::Node* arg0;
        std::string arg1;
        std::string arg2;
        std::string arg3;
        std::string arg4;

        ok &= luaval_to_object<cocos2d::Node>(tolua_S, 2, "cc.Node",&arg0);

        ok &= luaval_to_std_string(tolua_S, 3,&arg1);

        ok &= luaval_to_std_string(tolua_S, 4,&arg2);

        ok &= luaval_to_std_string(tolua_S, 5,&arg3);

        ok &= luaval_to_std_string(tolua_S, 6,&arg4);
        if(!ok)
            return 0;
        cobj->dump(arg0, arg1, arg2, arg3, arg4);
        return 0;
    }
    CCLOG("%s has wrong number of arguments: %d, was expecting %d \n", "dump",argc, 1);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_ww_NodeDumper_dump'.",&tolua_err);
#endif

    return 0;
}
int lua_ww_NodeDumper_clear(lua_State* tolua_S)
{
    int argc = 0;
    NodeDumper* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"NodeDumper",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (NodeDumper*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_ww_NodeDumper_clear'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 0) 
    {
        if(!ok)
            return 0;
        cobj->clear();
        return 0;
    }
    CCLOG("%s has wrong number of arguments: %d, was expecting %d \n", "clear",argc, 0);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_ww_NodeDumper_clear'.",&tolua_err);
#endif

    return 0;
}
int lua_ww_NodeDumper_dumpLabel(lua_State* tolua_S)
{
    int argc = 0;
    NodeDumper* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"NodeDumper",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (NodeDumper*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_ww_NodeDumper_dumpLabel'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 3) 
    {
        cocos2d::Node* arg0;
        std::string arg1;
        std::string arg2;

        ok &= luaval_to_object<cocos2d::Node>(tolua_S, 2, "cc.Node",&arg0);

        ok &= luaval_to_std_string(tolua_S, 3,&arg1);

        ok &= luaval_to_std_string(tolua_S, 4,&arg2);
        if(!ok)
            return 0;
        bool ret = cobj->dumpLabel(arg0, arg1, arg2);
        tolua_pushboolean(tolua_S,(bool)ret);
        return 1;
    }
    if (argc == 4) 
    {
        cocos2d::Node* arg0;
        std::string arg1;
        std::string arg2;
        std::string arg3;

        ok &= luaval_to_object<cocos2d::Node>(tolua_S, 2, "cc.Node",&arg0);

        ok &= luaval_to_std_string(tolua_S, 3,&arg1);

        ok &= luaval_to_std_string(tolua_S, 4,&arg2);

        ok &= luaval_to_std_string(tolua_S, 5,&arg3);
        if(!ok)
            return 0;
        bool ret = cobj->dumpLabel(arg0, arg1, arg2, arg3);
        tolua_pushboolean(tolua_S,(bool)ret);
        return 1;
    }
    if (argc == 5) 
    {
        cocos2d::Node* arg0;
        std::string arg1;
        std::string arg2;
        std::string arg3;
        std::string arg4;

        ok &= luaval_to_object<cocos2d::Node>(tolua_S, 2, "cc.Node",&arg0);

        ok &= luaval_to_std_string(tolua_S, 3,&arg1);

        ok &= luaval_to_std_string(tolua_S, 4,&arg2);

        ok &= luaval_to_std_string(tolua_S, 5,&arg3);

        ok &= luaval_to_std_string(tolua_S, 6,&arg4);
        if(!ok)
            return 0;
        bool ret = cobj->dumpLabel(arg0, arg1, arg2, arg3, arg4);
        tolua_pushboolean(tolua_S,(bool)ret);
        return 1;
    }
    if (argc == 6) 
    {
        cocos2d::Node* arg0;
        std::string arg1;
        std::string arg2;
        std::string arg3;
        std::string arg4;
        bool arg5;

        ok &= luaval_to_object<cocos2d::Node>(tolua_S, 2, "cc.Node",&arg0);

        ok &= luaval_to_std_string(tolua_S, 3,&arg1);

        ok &= luaval_to_std_string(tolua_S, 4,&arg2);

        ok &= luaval_to_std_string(tolua_S, 5,&arg3);

        ok &= luaval_to_std_string(tolua_S, 6,&arg4);

        ok &= luaval_to_boolean(tolua_S, 7,&arg5);
        if(!ok)
            return 0;
        bool ret = cobj->dumpLabel(arg0, arg1, arg2, arg3, arg4, arg5);
        tolua_pushboolean(tolua_S,(bool)ret);
        return 1;
    }
    CCLOG("%s has wrong number of arguments: %d, was expecting %d \n", "dumpLabel",argc, 3);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_ww_NodeDumper_dumpLabel'.",&tolua_err);
#endif

    return 0;
}
int lua_ww_NodeDumper_dumpNode(lua_State* tolua_S)
{
    int argc = 0;
    NodeDumper* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"NodeDumper",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (NodeDumper*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_ww_NodeDumper_dumpNode'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 3) 
    {
        cocos2d::Node* arg0;
        std::string arg1;
        std::string arg2;

        ok &= luaval_to_object<cocos2d::Node>(tolua_S, 2, "cc.Node",&arg0);

        ok &= luaval_to_std_string(tolua_S, 3,&arg1);

        ok &= luaval_to_std_string(tolua_S, 4,&arg2);
        if(!ok)
            return 0;
        cobj->dumpNode(arg0, arg1, arg2);
        return 0;
    }
    if (argc == 4) 
    {
        cocos2d::Node* arg0;
        std::string arg1;
        std::string arg2;
        std::string arg3;

        ok &= luaval_to_object<cocos2d::Node>(tolua_S, 2, "cc.Node",&arg0);

        ok &= luaval_to_std_string(tolua_S, 3,&arg1);

        ok &= luaval_to_std_string(tolua_S, 4,&arg2);

        ok &= luaval_to_std_string(tolua_S, 5,&arg3);
        if(!ok)
            return 0;
        cobj->dumpNode(arg0, arg1, arg2, arg3);
        return 0;
    }
    if (argc == 5) 
    {
        cocos2d::Node* arg0;
        std::string arg1;
        std::string arg2;
        std::string arg3;
        std::string arg4;

        ok &= luaval_to_object<cocos2d::Node>(tolua_S, 2, "cc.Node",&arg0);

        ok &= luaval_to_std_string(tolua_S, 3,&arg1);

        ok &= luaval_to_std_string(tolua_S, 4,&arg2);

        ok &= luaval_to_std_string(tolua_S, 5,&arg3);

        ok &= luaval_to_std_string(tolua_S, 6,&arg4);
        if(!ok)
            return 0;
        cobj->dumpNode(arg0, arg1, arg2, arg3, arg4);
        return 0;
    }
    if (argc == 6) 
    {
        cocos2d::Node* arg0;
        std::string arg1;
        std::string arg2;
        std::string arg3;
        std::string arg4;
        bool arg5;

        ok &= luaval_to_object<cocos2d::Node>(tolua_S, 2, "cc.Node",&arg0);

        ok &= luaval_to_std_string(tolua_S, 3,&arg1);

        ok &= luaval_to_std_string(tolua_S, 4,&arg2);

        ok &= luaval_to_std_string(tolua_S, 5,&arg3);

        ok &= luaval_to_std_string(tolua_S, 6,&arg4);

        ok &= luaval_to_boolean(tolua_S, 7,&arg5);
        if(!ok)
            return 0;
        cobj->dumpNode(arg0, arg1, arg2, arg3, arg4, arg5);
        return 0;
    }
    if (argc == 7) 
    {
        cocos2d::Node* arg0;
        std::string arg1;
        std::string arg2;
        std::string arg3;
        std::string arg4;
        bool arg5;
        bool arg6;

        ok &= luaval_to_object<cocos2d::Node>(tolua_S, 2, "cc.Node",&arg0);

        ok &= luaval_to_std_string(tolua_S, 3,&arg1);

        ok &= luaval_to_std_string(tolua_S, 4,&arg2);

        ok &= luaval_to_std_string(tolua_S, 5,&arg3);

        ok &= luaval_to_std_string(tolua_S, 6,&arg4);

        ok &= luaval_to_boolean(tolua_S, 7,&arg5);

        ok &= luaval_to_boolean(tolua_S, 8,&arg6);
        if(!ok)
            return 0;
        cobj->dumpNode(arg0, arg1, arg2, arg3, arg4, arg5, arg6);
        return 0;
    }
    CCLOG("%s has wrong number of arguments: %d, was expecting %d \n", "dumpNode",argc, 3);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_ww_NodeDumper_dumpNode'.",&tolua_err);
#endif

    return 0;
}
int lua_ww_NodeDumper_dumpExtButton(lua_State* tolua_S)
{
    int argc = 0;
    NodeDumper* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"NodeDumper",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (NodeDumper*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_ww_NodeDumper_dumpExtButton'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 3) 
    {
        cocos2d::Node* arg0;
        std::string arg1;
        std::string arg2;

        ok &= luaval_to_object<cocos2d::Node>(tolua_S, 2, "cc.Node",&arg0);

        ok &= luaval_to_std_string(tolua_S, 3,&arg1);

        ok &= luaval_to_std_string(tolua_S, 4,&arg2);
        if(!ok)
            return 0;
        bool ret = cobj->dumpExtButton(arg0, arg1, arg2);
        tolua_pushboolean(tolua_S,(bool)ret);
        return 1;
    }
    if (argc == 4) 
    {
        cocos2d::Node* arg0;
        std::string arg1;
        std::string arg2;
        std::string arg3;

        ok &= luaval_to_object<cocos2d::Node>(tolua_S, 2, "cc.Node",&arg0);

        ok &= luaval_to_std_string(tolua_S, 3,&arg1);

        ok &= luaval_to_std_string(tolua_S, 4,&arg2);

        ok &= luaval_to_std_string(tolua_S, 5,&arg3);
        if(!ok)
            return 0;
        bool ret = cobj->dumpExtButton(arg0, arg1, arg2, arg3);
        tolua_pushboolean(tolua_S,(bool)ret);
        return 1;
    }
    if (argc == 5) 
    {
        cocos2d::Node* arg0;
        std::string arg1;
        std::string arg2;
        std::string arg3;
        std::string arg4;

        ok &= luaval_to_object<cocos2d::Node>(tolua_S, 2, "cc.Node",&arg0);

        ok &= luaval_to_std_string(tolua_S, 3,&arg1);

        ok &= luaval_to_std_string(tolua_S, 4,&arg2);

        ok &= luaval_to_std_string(tolua_S, 5,&arg3);

        ok &= luaval_to_std_string(tolua_S, 6,&arg4);
        if(!ok)
            return 0;
        bool ret = cobj->dumpExtButton(arg0, arg1, arg2, arg3, arg4);
        tolua_pushboolean(tolua_S,(bool)ret);
        return 1;
    }
    if (argc == 6) 
    {
        cocos2d::Node* arg0;
        std::string arg1;
        std::string arg2;
        std::string arg3;
        std::string arg4;
        bool arg5;

        ok &= luaval_to_object<cocos2d::Node>(tolua_S, 2, "cc.Node",&arg0);

        ok &= luaval_to_std_string(tolua_S, 3,&arg1);

        ok &= luaval_to_std_string(tolua_S, 4,&arg2);

        ok &= luaval_to_std_string(tolua_S, 5,&arg3);

        ok &= luaval_to_std_string(tolua_S, 6,&arg4);

        ok &= luaval_to_boolean(tolua_S, 7,&arg5);
        if(!ok)
            return 0;
        bool ret = cobj->dumpExtButton(arg0, arg1, arg2, arg3, arg4, arg5);
        tolua_pushboolean(tolua_S,(bool)ret);
        return 1;
    }
    CCLOG("%s has wrong number of arguments: %d, was expecting %d \n", "dumpExtButton",argc, 3);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_ww_NodeDumper_dumpExtButton'.",&tolua_err);
#endif

    return 0;
}
int lua_ww_NodeDumper_dumpSprite(lua_State* tolua_S)
{
    int argc = 0;
    NodeDumper* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"NodeDumper",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (NodeDumper*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_ww_NodeDumper_dumpSprite'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 3) 
    {
        cocos2d::Node* arg0;
        std::string arg1;
        std::string arg2;

        ok &= luaval_to_object<cocos2d::Node>(tolua_S, 2, "cc.Node",&arg0);

        ok &= luaval_to_std_string(tolua_S, 3,&arg1);

        ok &= luaval_to_std_string(tolua_S, 4,&arg2);
        if(!ok)
            return 0;
        bool ret = cobj->dumpSprite(arg0, arg1, arg2);
        tolua_pushboolean(tolua_S,(bool)ret);
        return 1;
    }
    if (argc == 4) 
    {
        cocos2d::Node* arg0;
        std::string arg1;
        std::string arg2;
        std::string arg3;

        ok &= luaval_to_object<cocos2d::Node>(tolua_S, 2, "cc.Node",&arg0);

        ok &= luaval_to_std_string(tolua_S, 3,&arg1);

        ok &= luaval_to_std_string(tolua_S, 4,&arg2);

        ok &= luaval_to_std_string(tolua_S, 5,&arg3);
        if(!ok)
            return 0;
        bool ret = cobj->dumpSprite(arg0, arg1, arg2, arg3);
        tolua_pushboolean(tolua_S,(bool)ret);
        return 1;
    }
    if (argc == 5) 
    {
        cocos2d::Node* arg0;
        std::string arg1;
        std::string arg2;
        std::string arg3;
        std::string arg4;

        ok &= luaval_to_object<cocos2d::Node>(tolua_S, 2, "cc.Node",&arg0);

        ok &= luaval_to_std_string(tolua_S, 3,&arg1);

        ok &= luaval_to_std_string(tolua_S, 4,&arg2);

        ok &= luaval_to_std_string(tolua_S, 5,&arg3);

        ok &= luaval_to_std_string(tolua_S, 6,&arg4);
        if(!ok)
            return 0;
        bool ret = cobj->dumpSprite(arg0, arg1, arg2, arg3, arg4);
        tolua_pushboolean(tolua_S,(bool)ret);
        return 1;
    }
    if (argc == 6) 
    {
        cocos2d::Node* arg0;
        std::string arg1;
        std::string arg2;
        std::string arg3;
        std::string arg4;
        bool arg5;

        ok &= luaval_to_object<cocos2d::Node>(tolua_S, 2, "cc.Node",&arg0);

        ok &= luaval_to_std_string(tolua_S, 3,&arg1);

        ok &= luaval_to_std_string(tolua_S, 4,&arg2);

        ok &= luaval_to_std_string(tolua_S, 5,&arg3);

        ok &= luaval_to_std_string(tolua_S, 6,&arg4);

        ok &= luaval_to_boolean(tolua_S, 7,&arg5);
        if(!ok)
            return 0;
        bool ret = cobj->dumpSprite(arg0, arg1, arg2, arg3, arg4, arg5);
        tolua_pushboolean(tolua_S,(bool)ret);
        return 1;
    }
    CCLOG("%s has wrong number of arguments: %d, was expecting %d \n", "dumpSprite",argc, 3);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_ww_NodeDumper_dumpSprite'.",&tolua_err);
#endif

    return 0;
}
int lua_ww_NodeDumper_getBuffer(lua_State* tolua_S)
{
    int argc = 0;
    NodeDumper* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"NodeDumper",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (NodeDumper*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_ww_NodeDumper_getBuffer'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 0) 
    {
        if(!ok)
            return 0;
        char* ret = cobj->getBuffer();
        tolua_pushstring(tolua_S,(const char*)ret);
        return 1;
    }
    CCLOG("%s has wrong number of arguments: %d, was expecting %d \n", "getBuffer",argc, 0);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_ww_NodeDumper_getBuffer'.",&tolua_err);
#endif

    return 0;
}
int lua_ww_NodeDumper_dumpChilds(lua_State* tolua_S)
{
    int argc = 0;
    NodeDumper* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"NodeDumper",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (NodeDumper*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_ww_NodeDumper_dumpChilds'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 2) 
    {
        cocos2d::Node* arg0;
        std::string arg1;

        ok &= luaval_to_object<cocos2d::Node>(tolua_S, 2, "cc.Node",&arg0);

        ok &= luaval_to_std_string(tolua_S, 3,&arg1);
        if(!ok)
            return 0;
        cobj->dumpChilds(arg0, arg1);
        return 0;
    }
    if (argc == 3) 
    {
        cocos2d::Node* arg0;
        std::string arg1;
        std::string arg2;

        ok &= luaval_to_object<cocos2d::Node>(tolua_S, 2, "cc.Node",&arg0);

        ok &= luaval_to_std_string(tolua_S, 3,&arg1);

        ok &= luaval_to_std_string(tolua_S, 4,&arg2);
        if(!ok)
            return 0;
        cobj->dumpChilds(arg0, arg1, arg2);
        return 0;
    }
    if (argc == 4) 
    {
        cocos2d::Node* arg0;
        std::string arg1;
        std::string arg2;
        std::string arg3;

        ok &= luaval_to_object<cocos2d::Node>(tolua_S, 2, "cc.Node",&arg0);

        ok &= luaval_to_std_string(tolua_S, 3,&arg1);

        ok &= luaval_to_std_string(tolua_S, 4,&arg2);

        ok &= luaval_to_std_string(tolua_S, 5,&arg3);
        if(!ok)
            return 0;
        cobj->dumpChilds(arg0, arg1, arg2, arg3);
        return 0;
    }
    CCLOG("%s has wrong number of arguments: %d, was expecting %d \n", "dumpChilds",argc, 2);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_ww_NodeDumper_dumpChilds'.",&tolua_err);
#endif

    return 0;
}
int lua_ww_NodeDumper_constructor(lua_State* tolua_S)
{
    int argc = 0;
    NodeDumper* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif



    argc = lua_gettop(tolua_S)-1;
    if (argc == 0) 
    {
        if(!ok)
            return 0;
        cobj = new NodeDumper();
        tolua_pushusertype(tolua_S,(void*)cobj,"NodeDumper");
        tolua_register_gc(tolua_S,lua_gettop(tolua_S));
        return 1;
    }
    if (argc == 1) 
    {
        std::string arg0;

        ok &= luaval_to_std_string(tolua_S, 2,&arg0);
        if(!ok)
            return 0;
        cobj = new NodeDumper(arg0);
        tolua_pushusertype(tolua_S,(void*)cobj,"NodeDumper");
        tolua_register_gc(tolua_S,lua_gettop(tolua_S));
        return 1;
    }
    if (argc == 2) 
    {
        std::string arg0;
        int arg1;

        ok &= luaval_to_std_string(tolua_S, 2,&arg0);

        ok &= luaval_to_int32(tolua_S, 3,(int *)&arg1);
        if(!ok)
            return 0;
        cobj = new NodeDumper(arg0, arg1);
        tolua_pushusertype(tolua_S,(void*)cobj,"NodeDumper");
        tolua_register_gc(tolua_S,lua_gettop(tolua_S));
        return 1;
    }
    CCLOG("%s has wrong number of arguments: %d, was expecting %d \n", "NodeDumper",argc, 0);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_error(tolua_S,"#ferror in function 'lua_ww_NodeDumper_constructor'.",&tolua_err);
#endif

    return 0;
}

static int lua_ww_NodeDumper_finalize(lua_State* tolua_S)
{
    printf("luabindings: finalizing LUA object (NodeDumper)");
    return 0;
}

int lua_register_ww_NodeDumper(lua_State* tolua_S)
{
    tolua_usertype(tolua_S,"NodeDumper");
    tolua_cclass(tolua_S,"NodeDumper","NodeDumper","",nullptr);

    tolua_beginmodule(tolua_S,"NodeDumper");
        tolua_function(tolua_S,"new",lua_ww_NodeDumper_constructor);
        tolua_function(tolua_S,"dumpLayer",lua_ww_NodeDumper_dumpLayer);
        tolua_function(tolua_S,"saveToFile",lua_ww_NodeDumper_saveToFile);
        tolua_function(tolua_S,"dump",lua_ww_NodeDumper_dump);
        tolua_function(tolua_S,"clear",lua_ww_NodeDumper_clear);
        tolua_function(tolua_S,"dumpLabel",lua_ww_NodeDumper_dumpLabel);
        tolua_function(tolua_S,"dumpNode",lua_ww_NodeDumper_dumpNode);
        tolua_function(tolua_S,"dumpExtButton",lua_ww_NodeDumper_dumpExtButton);
        tolua_function(tolua_S,"dumpSprite",lua_ww_NodeDumper_dumpSprite);
        tolua_function(tolua_S,"getBuffer",lua_ww_NodeDumper_getBuffer);
        tolua_function(tolua_S,"dumpChilds",lua_ww_NodeDumper_dumpChilds);
    tolua_endmodule(tolua_S);
    std::string typeName = typeid(NodeDumper).name();
    g_luaType[typeName] = "NodeDumper";
    g_typeCast["NodeDumper"] = "NodeDumper";
    return 1;
}

int lua_ww_GameAnalyticsWrapper_purchase(lua_State* tolua_S)
{
    int argc = 0;
    GameAnalyticsWrapper* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"GameAnalyticsWrapper",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (GameAnalyticsWrapper*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_ww_GameAnalyticsWrapper_purchase'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 3) 
    {
        std::string arg0;
        int arg1;
        double arg2;

        ok &= luaval_to_std_string(tolua_S, 2,&arg0);

        ok &= luaval_to_int32(tolua_S, 3,(int *)&arg1);

        ok &= luaval_to_number(tolua_S, 4,&arg2);
        if(!ok)
            return 0;
        cobj->purchase(arg0, arg1, arg2);
        return 0;
    }
    CCLOG("%s has wrong number of arguments: %d, was expecting %d \n", "purchase",argc, 3);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_ww_GameAnalyticsWrapper_purchase'.",&tolua_err);
#endif

    return 0;
}
int lua_ww_GameAnalyticsWrapper_setLevel(lua_State* tolua_S)
{
    int argc = 0;
    GameAnalyticsWrapper* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"GameAnalyticsWrapper",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (GameAnalyticsWrapper*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_ww_GameAnalyticsWrapper_setLevel'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 1) 
    {
        int arg0;

        ok &= luaval_to_int32(tolua_S, 2,(int *)&arg0);
        if(!ok)
            return 0;
        cobj->setLevel(arg0);
        return 0;
    }
    CCLOG("%s has wrong number of arguments: %d, was expecting %d \n", "setLevel",argc, 1);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_ww_GameAnalyticsWrapper_setLevel'.",&tolua_err);
#endif

    return 0;
}
int lua_ww_GameAnalyticsWrapper_stageBegin(lua_State* tolua_S)
{
    int argc = 0;
    GameAnalyticsWrapper* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"GameAnalyticsWrapper",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (GameAnalyticsWrapper*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_ww_GameAnalyticsWrapper_stageBegin'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 1) 
    {
        std::string arg0;

        ok &= luaval_to_std_string(tolua_S, 2,&arg0);
        if(!ok)
            return 0;
        cobj->stageBegin(arg0);
        return 0;
    }
    CCLOG("%s has wrong number of arguments: %d, was expecting %d \n", "stageBegin",argc, 1);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_ww_GameAnalyticsWrapper_stageBegin'.",&tolua_err);
#endif

    return 0;
}
int lua_ww_GameAnalyticsWrapper_chargeRequest(lua_State* tolua_S)
{
    int argc = 0;
    GameAnalyticsWrapper* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"GameAnalyticsWrapper",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (GameAnalyticsWrapper*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_ww_GameAnalyticsWrapper_chargeRequest'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 6) 
    {
        std::string arg0;
        std::string arg1;
        double arg2;
        std::string arg3;
        double arg4;
        std::string arg5;

        ok &= luaval_to_std_string(tolua_S, 2,&arg0);

        ok &= luaval_to_std_string(tolua_S, 3,&arg1);

        ok &= luaval_to_number(tolua_S, 4,&arg2);

        ok &= luaval_to_std_string(tolua_S, 5,&arg3);

        ok &= luaval_to_number(tolua_S, 6,&arg4);

        ok &= luaval_to_std_string(tolua_S, 7,&arg5);
        if(!ok)
            return 0;
        cobj->chargeRequest(arg0, arg1, arg2, arg3, arg4, arg5);
        return 0;
    }
    CCLOG("%s has wrong number of arguments: %d, was expecting %d \n", "chargeRequest",argc, 6);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_ww_GameAnalyticsWrapper_chargeRequest'.",&tolua_err);
#endif

    return 0;
}
int lua_ww_GameAnalyticsWrapper_stageCompleted(lua_State* tolua_S)
{
    int argc = 0;
    GameAnalyticsWrapper* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"GameAnalyticsWrapper",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (GameAnalyticsWrapper*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_ww_GameAnalyticsWrapper_stageCompleted'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 1) 
    {
        std::string arg0;

        ok &= luaval_to_std_string(tolua_S, 2,&arg0);
        if(!ok)
            return 0;
        cobj->stageCompleted(arg0);
        return 0;
    }
    CCLOG("%s has wrong number of arguments: %d, was expecting %d \n", "stageCompleted",argc, 1);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_ww_GameAnalyticsWrapper_stageCompleted'.",&tolua_err);
#endif

    return 0;
}
int lua_ww_GameAnalyticsWrapper_setAccount(lua_State* tolua_S)
{
    int argc = 0;
    GameAnalyticsWrapper* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"GameAnalyticsWrapper",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (GameAnalyticsWrapper*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_ww_GameAnalyticsWrapper_setAccount'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 1) 
    {
        std::string arg0;

        ok &= luaval_to_std_string(tolua_S, 2,&arg0);
        if(!ok)
            return 0;
        cobj->setAccount(arg0);
        return 0;
    }
    CCLOG("%s has wrong number of arguments: %d, was expecting %d \n", "setAccount",argc, 1);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_ww_GameAnalyticsWrapper_setAccount'.",&tolua_err);
#endif

    return 0;
}
int lua_ww_GameAnalyticsWrapper_chargeSuccess(lua_State* tolua_S)
{
    int argc = 0;
    GameAnalyticsWrapper* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"GameAnalyticsWrapper",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (GameAnalyticsWrapper*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_ww_GameAnalyticsWrapper_chargeSuccess'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 1) 
    {
        std::string arg0;

        ok &= luaval_to_std_string(tolua_S, 2,&arg0);
        if(!ok)
            return 0;
        cobj->chargeSuccess(arg0);
        return 0;
    }
    CCLOG("%s has wrong number of arguments: %d, was expecting %d \n", "chargeSuccess",argc, 1);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_ww_GameAnalyticsWrapper_chargeSuccess'.",&tolua_err);
#endif

    return 0;
}
int lua_ww_GameAnalyticsWrapper_setAccountType(lua_State* tolua_S)
{
    int argc = 0;
    GameAnalyticsWrapper* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"GameAnalyticsWrapper",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (GameAnalyticsWrapper*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_ww_GameAnalyticsWrapper_setAccountType'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 1) 
    {
        int arg0;

        ok &= luaval_to_int32(tolua_S, 2,(int *)&arg0);
        if(!ok)
            return 0;
        cobj->setAccountType(arg0);
        return 0;
    }
    CCLOG("%s has wrong number of arguments: %d, was expecting %d \n", "setAccountType",argc, 1);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_ww_GameAnalyticsWrapper_setAccountType'.",&tolua_err);
#endif

    return 0;
}
int lua_ww_GameAnalyticsWrapper_startSession(lua_State* tolua_S)
{
    int argc = 0;
    GameAnalyticsWrapper* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"GameAnalyticsWrapper",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (GameAnalyticsWrapper*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_ww_GameAnalyticsWrapper_startSession'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 0) 
    {
        if(!ok)
            return 0;
        cobj->startSession();
        return 0;
    }
    CCLOG("%s has wrong number of arguments: %d, was expecting %d \n", "startSession",argc, 0);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_ww_GameAnalyticsWrapper_startSession'.",&tolua_err);
#endif

    return 0;
}
int lua_ww_GameAnalyticsWrapper_stopSession(lua_State* tolua_S)
{
    int argc = 0;
    GameAnalyticsWrapper* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"GameAnalyticsWrapper",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (GameAnalyticsWrapper*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_ww_GameAnalyticsWrapper_stopSession'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 0) 
    {
        if(!ok)
            return 0;
        cobj->stopSession();
        return 0;
    }
    CCLOG("%s has wrong number of arguments: %d, was expecting %d \n", "stopSession",argc, 0);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_ww_GameAnalyticsWrapper_stopSession'.",&tolua_err);
#endif

    return 0;
}
int lua_ww_GameAnalyticsWrapper_setGameServer(lua_State* tolua_S)
{
    int argc = 0;
    GameAnalyticsWrapper* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"GameAnalyticsWrapper",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (GameAnalyticsWrapper*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_ww_GameAnalyticsWrapper_setGameServer'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 1) 
    {
        std::string arg0;

        ok &= luaval_to_std_string(tolua_S, 2,&arg0);
        if(!ok)
            return 0;
        cobj->setGameServer(arg0);
        return 0;
    }
    CCLOG("%s has wrong number of arguments: %d, was expecting %d \n", "setGameServer",argc, 1);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_ww_GameAnalyticsWrapper_setGameServer'.",&tolua_err);
#endif

    return 0;
}
int lua_ww_GameAnalyticsWrapper_reward(lua_State* tolua_S)
{
    int argc = 0;
    GameAnalyticsWrapper* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"GameAnalyticsWrapper",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (GameAnalyticsWrapper*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_ww_GameAnalyticsWrapper_reward'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 2) 
    {
        double arg0;
        std::string arg1;

        ok &= luaval_to_number(tolua_S, 2,&arg0);

        ok &= luaval_to_std_string(tolua_S, 3,&arg1);
        if(!ok)
            return 0;
        cobj->reward(arg0, arg1);
        return 0;
    }
    CCLOG("%s has wrong number of arguments: %d, was expecting %d \n", "reward",argc, 2);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_ww_GameAnalyticsWrapper_reward'.",&tolua_err);
#endif

    return 0;
}
int lua_ww_GameAnalyticsWrapper_fireEvent(lua_State* tolua_S)
{
    int argc = 0;
    GameAnalyticsWrapper* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"GameAnalyticsWrapper",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (GameAnalyticsWrapper*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_ww_GameAnalyticsWrapper_fireEvent'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 2) 
    {
        std::string arg0;
        std::string arg1;

        ok &= luaval_to_std_string(tolua_S, 2,&arg0);

        ok &= luaval_to_std_string(tolua_S, 3,&arg1);
        if(!ok)
            return 0;
        cobj->fireEvent(arg0, arg1);
        return 0;
    }
    CCLOG("%s has wrong number of arguments: %d, was expecting %d \n", "fireEvent",argc, 2);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_ww_GameAnalyticsWrapper_fireEvent'.",&tolua_err);
#endif

    return 0;
}
int lua_ww_GameAnalyticsWrapper_stageFailed(lua_State* tolua_S)
{
    int argc = 0;
    GameAnalyticsWrapper* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"GameAnalyticsWrapper",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (GameAnalyticsWrapper*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_ww_GameAnalyticsWrapper_stageFailed'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 2) 
    {
        std::string arg0;
        std::string arg1;

        ok &= luaval_to_std_string(tolua_S, 2,&arg0);

        ok &= luaval_to_std_string(tolua_S, 3,&arg1);
        if(!ok)
            return 0;
        cobj->stageFailed(arg0, arg1);
        return 0;
    }
    CCLOG("%s has wrong number of arguments: %d, was expecting %d \n", "stageFailed",argc, 2);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_ww_GameAnalyticsWrapper_stageFailed'.",&tolua_err);
#endif

    return 0;
}
int lua_ww_GameAnalyticsWrapper_getInstance(lua_State* tolua_S)
{
    int argc = 0;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif

#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertable(tolua_S,1,"GameAnalyticsWrapper",0,&tolua_err)) goto tolua_lerror;
#endif

    argc = lua_gettop(tolua_S) - 1;

    if (argc == 0)
    {
        if(!ok)
            return 0;
        GameAnalyticsWrapper* ret = GameAnalyticsWrapper::getInstance();
        object_to_luaval<GameAnalyticsWrapper>(tolua_S, "GameAnalyticsWrapper",(GameAnalyticsWrapper*)ret);
        return 1;
    }
    CCLOG("%s has wrong number of arguments: %d, was expecting %d\n ", "getInstance",argc, 0);
    return 0;
#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_ww_GameAnalyticsWrapper_getInstance'.",&tolua_err);
#endif
    return 0;
}
static int lua_ww_GameAnalyticsWrapper_finalize(lua_State* tolua_S)
{
    printf("luabindings: finalizing LUA object (GameAnalyticsWrapper)");
    return 0;
}

int lua_register_ww_GameAnalyticsWrapper(lua_State* tolua_S)
{
    tolua_usertype(tolua_S,"GameAnalyticsWrapper");
    tolua_cclass(tolua_S,"GameAnalyticsWrapper","GameAnalyticsWrapper","",nullptr);

    tolua_beginmodule(tolua_S,"GameAnalyticsWrapper");
        tolua_function(tolua_S,"purchase",lua_ww_GameAnalyticsWrapper_purchase);
        tolua_function(tolua_S,"setLevel",lua_ww_GameAnalyticsWrapper_setLevel);
        tolua_function(tolua_S,"stageBegin",lua_ww_GameAnalyticsWrapper_stageBegin);
        tolua_function(tolua_S,"chargeRequest",lua_ww_GameAnalyticsWrapper_chargeRequest);
        tolua_function(tolua_S,"stageCompleted",lua_ww_GameAnalyticsWrapper_stageCompleted);
        tolua_function(tolua_S,"setAccount",lua_ww_GameAnalyticsWrapper_setAccount);
        tolua_function(tolua_S,"chargeSuccess",lua_ww_GameAnalyticsWrapper_chargeSuccess);
        tolua_function(tolua_S,"setAccountType",lua_ww_GameAnalyticsWrapper_setAccountType);
        tolua_function(tolua_S,"startSession",lua_ww_GameAnalyticsWrapper_startSession);
        tolua_function(tolua_S,"stopSession",lua_ww_GameAnalyticsWrapper_stopSession);
        tolua_function(tolua_S,"setGameServer",lua_ww_GameAnalyticsWrapper_setGameServer);
        tolua_function(tolua_S,"reward",lua_ww_GameAnalyticsWrapper_reward);
        tolua_function(tolua_S,"fireEvent",lua_ww_GameAnalyticsWrapper_fireEvent);
        tolua_function(tolua_S,"stageFailed",lua_ww_GameAnalyticsWrapper_stageFailed);
        tolua_function(tolua_S,"getInstance", lua_ww_GameAnalyticsWrapper_getInstance);
    tolua_endmodule(tolua_S);
    std::string typeName = typeid(GameAnalyticsWrapper).name();
    g_luaType[typeName] = "GameAnalyticsWrapper";
    g_typeCast["GameAnalyticsWrapper"] = "GameAnalyticsWrapper";
    return 1;
}

int lua_ww_ExtButton_setZoomOnTouchDown(lua_State* tolua_S)
{
    int argc = 0;
    ExtButton* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"ExtButton",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (ExtButton*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_ww_ExtButton_setZoomOnTouchDown'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 1) 
    {
        bool arg0;

        ok &= luaval_to_boolean(tolua_S, 2,&arg0);
        if(!ok)
            return 0;
        cobj->setZoomOnTouchDown(arg0);
        return 0;
    }
    CCLOG("%s has wrong number of arguments: %d, was expecting %d \n", "setZoomOnTouchDown",argc, 1);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_ww_ExtButton_setZoomOnTouchDown'.",&tolua_err);
#endif

    return 0;
}
int lua_ww_ExtButton_getState(lua_State* tolua_S)
{
    int argc = 0;
    ExtButton* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"ExtButton",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (ExtButton*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_ww_ExtButton_getState'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 0) 
    {
        if(!ok)
            return 0;
        int ret = (int)cobj->getState();
        tolua_pushnumber(tolua_S,(lua_Number)ret);
        return 1;
    }
    CCLOG("%s has wrong number of arguments: %d, was expecting %d \n", "getState",argc, 0);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_ww_ExtButton_getState'.",&tolua_err);
#endif

    return 0;
}
int lua_ww_ExtButton_setBg(lua_State* tolua_S)
{
    int argc = 0;
    ExtButton* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"ExtButton",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (ExtButton*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_ww_ExtButton_setBg'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 1) 
    {
        cocos2d::Node* arg0;

        ok &= luaval_to_object<cocos2d::Node>(tolua_S, 2, "cc.Node",&arg0);
        if(!ok)
            return 0;
        cobj->setBg(arg0);
        return 0;
    }
    CCLOG("%s has wrong number of arguments: %d, was expecting %d \n", "setBg",argc, 1);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_ww_ExtButton_setBg'.",&tolua_err);
#endif

    return 0;
}
int lua_ww_ExtButton_unregisterEnableHandler(lua_State* tolua_S)
{
    int argc = 0;
    ExtButton* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"ExtButton",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (ExtButton*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_ww_ExtButton_unregisterEnableHandler'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 0) 
    {
        if(!ok)
            return 0;
        cobj->unregisterEnableHandler();
        return 0;
    }
    CCLOG("%s has wrong number of arguments: %d, was expecting %d \n", "unregisterEnableHandler",argc, 0);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_ww_ExtButton_unregisterEnableHandler'.",&tolua_err);
#endif

    return 0;
}
int lua_ww_ExtButton_setEnabled(lua_State* tolua_S)
{
    int argc = 0;
    ExtButton* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"ExtButton",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (ExtButton*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_ww_ExtButton_setEnabled'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 1) 
    {
        bool arg0;

        ok &= luaval_to_boolean(tolua_S, 2,&arg0);
        if(!ok)
            return 0;
        cobj->setEnabled(arg0);
        return 0;
    }
    CCLOG("%s has wrong number of arguments: %d, was expecting %d \n", "setEnabled",argc, 1);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_ww_ExtButton_setEnabled'.",&tolua_err);
#endif

    return 0;
}
int lua_ww_ExtButton_getBg(lua_State* tolua_S)
{
    int argc = 0;
    ExtButton* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"ExtButton",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (ExtButton*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_ww_ExtButton_getBg'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 0) 
    {
        if(!ok)
            return 0;
        cocos2d::Node* ret = cobj->getBg();
        object_to_luaval<cocos2d::Node>(tolua_S, "cc.Node",(cocos2d::Node*)ret);
        return 1;
    }
    CCLOG("%s has wrong number of arguments: %d, was expecting %d \n", "getBg",argc, 0);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_ww_ExtButton_getBg'.",&tolua_err);
#endif

    return 0;
}
int lua_ww_ExtButton_getFg(lua_State* tolua_S)
{
    int argc = 0;
    ExtButton* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"ExtButton",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (ExtButton*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_ww_ExtButton_getFg'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 0) 
    {
        if(!ok)
            return 0;
        cocos2d::Node* ret = cobj->getFg();
        object_to_luaval<cocos2d::Node>(tolua_S, "cc.Node",(cocos2d::Node*)ret);
        return 1;
    }
    CCLOG("%s has wrong number of arguments: %d, was expecting %d \n", "getFg",argc, 0);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_ww_ExtButton_getFg'.",&tolua_err);
#endif

    return 0;
}
int lua_ww_ExtButton_isEnabled(lua_State* tolua_S)
{
    int argc = 0;
    ExtButton* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"ExtButton",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (ExtButton*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_ww_ExtButton_isEnabled'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 0) 
    {
        if(!ok)
            return 0;
        bool ret = cobj->isEnabled();
        tolua_pushboolean(tolua_S,(bool)ret);
        return 1;
    }
    CCLOG("%s has wrong number of arguments: %d, was expecting %d \n", "isEnabled",argc, 0);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_ww_ExtButton_isEnabled'.",&tolua_err);
#endif

    return 0;
}
int lua_ww_ExtButton_isPushed(lua_State* tolua_S)
{
    int argc = 0;
    ExtButton* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"ExtButton",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (ExtButton*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_ww_ExtButton_isPushed'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 0) 
    {
        if(!ok)
            return 0;
        bool ret = cobj->isPushed();
        tolua_pushboolean(tolua_S,(bool)ret);
        return 1;
    }
    CCLOG("%s has wrong number of arguments: %d, was expecting %d \n", "isPushed",argc, 0);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_ww_ExtButton_isPushed'.",&tolua_err);
#endif

    return 0;
}
int lua_ww_ExtButton_unregisterHighlightHandler(lua_State* tolua_S)
{
    int argc = 0;
    ExtButton* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"ExtButton",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (ExtButton*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_ww_ExtButton_unregisterHighlightHandler'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 0) 
    {
        if(!ok)
            return 0;
        cobj->unregisterHighlightHandler();
        return 0;
    }
    CCLOG("%s has wrong number of arguments: %d, was expecting %d \n", "unregisterHighlightHandler",argc, 0);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_ww_ExtButton_unregisterHighlightHandler'.",&tolua_err);
#endif

    return 0;
}
int lua_ww_ExtButton_isSwallow(lua_State* tolua_S)
{
    int argc = 0;
    ExtButton* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"ExtButton",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (ExtButton*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_ww_ExtButton_isSwallow'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 0) 
    {
        if(!ok)
            return 0;
        bool ret = cobj->isSwallow();
        tolua_pushboolean(tolua_S,(bool)ret);
        return 1;
    }
    CCLOG("%s has wrong number of arguments: %d, was expecting %d \n", "isSwallow",argc, 0);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_ww_ExtButton_isSwallow'.",&tolua_err);
#endif

    return 0;
}
int lua_ww_ExtButton_setFg(lua_State* tolua_S)
{
    int argc = 0;
    ExtButton* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"ExtButton",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (ExtButton*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_ww_ExtButton_setFg'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 1) 
    {
        cocos2d::Node* arg0;

        ok &= luaval_to_object<cocos2d::Node>(tolua_S, 2, "cc.Node",&arg0);
        if(!ok)
            return 0;
        cobj->setFg(arg0);
        return 0;
    }
    CCLOG("%s has wrong number of arguments: %d, was expecting %d \n", "setFg",argc, 1);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_ww_ExtButton_setFg'.",&tolua_err);
#endif

    return 0;
}
int lua_ww_ExtButton_clearTouchLock(lua_State* tolua_S)
{
    int argc = 0;
    ExtButton* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"ExtButton",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (ExtButton*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_ww_ExtButton_clearTouchLock'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 0) 
    {
        if(!ok)
            return 0;
        cobj->clearTouchLock();
        return 0;
    }
    CCLOG("%s has wrong number of arguments: %d, was expecting %d \n", "clearTouchLock",argc, 0);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_ww_ExtButton_clearTouchLock'.",&tolua_err);
#endif

    return 0;
}
int lua_ww_ExtButton_setHighlighted(lua_State* tolua_S)
{
    int argc = 0;
    ExtButton* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"ExtButton",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (ExtButton*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_ww_ExtButton_setHighlighted'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 1) 
    {
        bool arg0;

        ok &= luaval_to_boolean(tolua_S, 2,&arg0);
        if(!ok)
            return 0;
        cobj->setHighlighted(arg0);
        return 0;
    }
    CCLOG("%s has wrong number of arguments: %d, was expecting %d \n", "setHighlighted",argc, 1);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_ww_ExtButton_setHighlighted'.",&tolua_err);
#endif

    return 0;
}
int lua_ww_ExtButton_setSwallow(lua_State* tolua_S)
{
    int argc = 0;
    ExtButton* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"ExtButton",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (ExtButton*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_ww_ExtButton_setSwallow'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 1) 
    {
        bool arg0;

        ok &= luaval_to_boolean(tolua_S, 2,&arg0);
        if(!ok)
            return 0;
        cobj->setSwallow(arg0);
        return 0;
    }
    CCLOG("%s has wrong number of arguments: %d, was expecting %d \n", "setSwallow",argc, 1);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_ww_ExtButton_setSwallow'.",&tolua_err);
#endif

    return 0;
}
int lua_ww_ExtButton_isZoomOnTouchDown(lua_State* tolua_S)
{
    int argc = 0;
    ExtButton* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"ExtButton",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (ExtButton*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_ww_ExtButton_isZoomOnTouchDown'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 0) 
    {
        if(!ok)
            return 0;
        bool ret = cobj->isZoomOnTouchDown();
        tolua_pushboolean(tolua_S,(bool)ret);
        return 1;
    }
    CCLOG("%s has wrong number of arguments: %d, was expecting %d \n", "isZoomOnTouchDown",argc, 0);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_ww_ExtButton_isZoomOnTouchDown'.",&tolua_err);
#endif

    return 0;
}
int lua_ww_ExtButton_unregisterTouchEventHandler(lua_State* tolua_S)
{
    int argc = 0;
    ExtButton* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"ExtButton",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (ExtButton*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_ww_ExtButton_unregisterTouchEventHandler'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 0) 
    {
        if(!ok)
            return 0;
        cobj->unregisterTouchEventHandler();
        return 0;
    }
    CCLOG("%s has wrong number of arguments: %d, was expecting %d \n", "unregisterTouchEventHandler",argc, 0);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_ww_ExtButton_unregisterTouchEventHandler'.",&tolua_err);
#endif

    return 0;
}
int lua_ww_ExtButton_isHighlighted(lua_State* tolua_S)
{
    int argc = 0;
    ExtButton* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"ExtButton",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (ExtButton*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_ww_ExtButton_isHighlighted'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 0) 
    {
        if(!ok)
            return 0;
        bool ret = cobj->isHighlighted();
        tolua_pushboolean(tolua_S,(bool)ret);
        return 1;
    }
    CCLOG("%s has wrong number of arguments: %d, was expecting %d \n", "isHighlighted",argc, 0);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_ww_ExtButton_isHighlighted'.",&tolua_err);
#endif

    return 0;
}
int lua_ww_ExtButton_create(lua_State* tolua_S)
{
    int argc = 0;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif

#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertable(tolua_S,1,"ExtButton",0,&tolua_err)) goto tolua_lerror;
#endif

    argc = lua_gettop(tolua_S) - 1;

    if (argc == 1)
    {
        cocos2d::Node* arg0;
        ok &= luaval_to_object<cocos2d::Node>(tolua_S, 2, "cc.Node",&arg0);
        if(!ok)
            return 0;
        ExtButton* ret = ExtButton::create(arg0);
        object_to_luaval<ExtButton>(tolua_S, "ExtButton",(ExtButton*)ret);
        return 1;
    }
    if (argc == 2)
    {
        cocos2d::Node* arg0;
        cocos2d::Node* arg1;
        ok &= luaval_to_object<cocos2d::Node>(tolua_S, 2, "cc.Node",&arg0);
        ok &= luaval_to_object<cocos2d::Node>(tolua_S, 3, "cc.Node",&arg1);
        if(!ok)
            return 0;
        ExtButton* ret = ExtButton::create(arg0, arg1);
        object_to_luaval<ExtButton>(tolua_S, "ExtButton",(ExtButton*)ret);
        return 1;
    }
    if (argc == 3)
    {
        cocos2d::Node* arg0;
        cocos2d::Node* arg1;
        bool arg2;
        ok &= luaval_to_object<cocos2d::Node>(tolua_S, 2, "cc.Node",&arg0);
        ok &= luaval_to_object<cocos2d::Node>(tolua_S, 3, "cc.Node",&arg1);
        ok &= luaval_to_boolean(tolua_S, 4,&arg2);
        if(!ok)
            return 0;
        ExtButton* ret = ExtButton::create(arg0, arg1, arg2);
        object_to_luaval<ExtButton>(tolua_S, "ExtButton",(ExtButton*)ret);
        return 1;
    }
    if (argc == 4)
    {
        cocos2d::Node* arg0;
        cocos2d::Node* arg1;
        bool arg2;
        int arg3;
        ok &= luaval_to_object<cocos2d::Node>(tolua_S, 2, "cc.Node",&arg0);
        ok &= luaval_to_object<cocos2d::Node>(tolua_S, 3, "cc.Node",&arg1);
        ok &= luaval_to_boolean(tolua_S, 4,&arg2);
        ok &= luaval_to_int32(tolua_S, 5,(int *)&arg3);
        if(!ok)
            return 0;
        ExtButton* ret = ExtButton::create(arg0, arg1, arg2, arg3);
        object_to_luaval<ExtButton>(tolua_S, "ExtButton",(ExtButton*)ret);
        return 1;
    }
    CCLOG("%s has wrong number of arguments: %d, was expecting %d\n ", "create",argc, 1);
    return 0;
#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_ww_ExtButton_create'.",&tolua_err);
#endif
    return 0;
}
static int lua_ww_ExtButton_finalize(lua_State* tolua_S)
{
    printf("luabindings: finalizing LUA object (ExtButton)");
    return 0;
}

int lua_register_ww_ExtButton(lua_State* tolua_S)
{
    tolua_usertype(tolua_S,"ExtButton");
    tolua_cclass(tolua_S,"ExtButton","ExtButton","cc.Layer",nullptr);

    tolua_beginmodule(tolua_S,"ExtButton");
        tolua_function(tolua_S,"setZoomOnTouchDown",lua_ww_ExtButton_setZoomOnTouchDown);
        tolua_function(tolua_S,"getState",lua_ww_ExtButton_getState);
        tolua_function(tolua_S,"setBg",lua_ww_ExtButton_setBg);
        tolua_function(tolua_S,"unregisterEnableHandler",lua_ww_ExtButton_unregisterEnableHandler);
        tolua_function(tolua_S,"setEnabled",lua_ww_ExtButton_setEnabled);
        tolua_function(tolua_S,"getBg",lua_ww_ExtButton_getBg);
        tolua_function(tolua_S,"getFg",lua_ww_ExtButton_getFg);
        tolua_function(tolua_S,"isEnabled",lua_ww_ExtButton_isEnabled);
        tolua_function(tolua_S,"isPushed",lua_ww_ExtButton_isPushed);
        tolua_function(tolua_S,"unregisterHighlightHandler",lua_ww_ExtButton_unregisterHighlightHandler);
        tolua_function(tolua_S,"isSwallow",lua_ww_ExtButton_isSwallow);
        tolua_function(tolua_S,"setFg",lua_ww_ExtButton_setFg);
        tolua_function(tolua_S,"clearTouchLock",lua_ww_ExtButton_clearTouchLock);
        tolua_function(tolua_S,"setHighlighted",lua_ww_ExtButton_setHighlighted);
        tolua_function(tolua_S,"setSwallow",lua_ww_ExtButton_setSwallow);
        tolua_function(tolua_S,"isZoomOnTouchDown",lua_ww_ExtButton_isZoomOnTouchDown);
        tolua_function(tolua_S,"unregisterTouchEventHandler",lua_ww_ExtButton_unregisterTouchEventHandler);
        tolua_function(tolua_S,"isHighlighted",lua_ww_ExtButton_isHighlighted);
        tolua_function(tolua_S,"create", lua_ww_ExtButton_create);
    tolua_endmodule(tolua_S);
    std::string typeName = typeid(ExtButton).name();
    g_luaType[typeName] = "ExtButton";
    g_typeCast["ExtButton"] = "ExtButton";
    return 1;
}

int lua_ww_ExtClippingNode_setTouchMoved(lua_State* tolua_S)
{
    int argc = 0;
    ExtClippingNode* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"ExtClippingNode",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (ExtClippingNode*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_ww_ExtClippingNode_setTouchMoved'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 1) 
    {
        bool arg0;

        ok &= luaval_to_boolean(tolua_S, 2,&arg0);
        if(!ok)
            return 0;
        cobj->setTouchMoved(arg0);
        return 0;
    }
    CCLOG("%s has wrong number of arguments: %d, was expecting %d \n", "setTouchMoved",argc, 1);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_ww_ExtClippingNode_setTouchMoved'.",&tolua_err);
#endif

    return 0;
}
int lua_ww_ExtClippingNode_getClippingRegion(lua_State* tolua_S)
{
    int argc = 0;
    ExtClippingNode* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"ExtClippingNode",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (ExtClippingNode*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_ww_ExtClippingNode_getClippingRegion'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 0) 
    {
        if(!ok)
            return 0;
        cocos2d::Rect ret = cobj->getClippingRegion();
        rect_to_luaval(tolua_S, ret);
        return 1;
    }
    CCLOG("%s has wrong number of arguments: %d, was expecting %d \n", "getClippingRegion",argc, 0);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_ww_ExtClippingNode_getClippingRegion'.",&tolua_err);
#endif

    return 0;
}
int lua_ww_ExtClippingNode_isTouchMoved(lua_State* tolua_S)
{
    int argc = 0;
    ExtClippingNode* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"ExtClippingNode",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (ExtClippingNode*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_ww_ExtClippingNode_isTouchMoved'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 0) 
    {
        if(!ok)
            return 0;
        bool ret = cobj->isTouchMoved();
        tolua_pushboolean(tolua_S,(bool)ret);
        return 1;
    }
    CCLOG("%s has wrong number of arguments: %d, was expecting %d \n", "isTouchMoved",argc, 0);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_ww_ExtClippingNode_isTouchMoved'.",&tolua_err);
#endif

    return 0;
}
int lua_ww_ExtClippingNode_setClippingRegion(lua_State* tolua_S)
{
    int argc = 0;
    ExtClippingNode* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"ExtClippingNode",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (ExtClippingNode*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_ww_ExtClippingNode_setClippingRegion'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 1) 
    {
        cocos2d::Rect arg0;

        ok &= luaval_to_rect(tolua_S, 2, &arg0);
        if(!ok)
            return 0;
        cobj->setClippingRegion(arg0);
        return 0;
    }
    CCLOG("%s has wrong number of arguments: %d, was expecting %d \n", "setClippingRegion",argc, 1);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_ww_ExtClippingNode_setClippingRegion'.",&tolua_err);
#endif

    return 0;
}
int lua_ww_ExtClippingNode_create(lua_State* tolua_S)
{
    int argc = 0;
    bool ok  = true;
#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif

#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertable(tolua_S,1,"ExtClippingNode",0,&tolua_err)) goto tolua_lerror;
#endif

    argc = lua_gettop(tolua_S)-1;

    do 
    {
        if (argc == 0)
        {
            ExtClippingNode* ret = ExtClippingNode::create();
            object_to_luaval<ExtClippingNode>(tolua_S, "ExtClippingNode",(ExtClippingNode*)ret);
            return 1;
        }
    } while (0);
    ok  = true;
    do 
    {
        if (argc == 1)
        {
            cocos2d::Rect arg0;
            ok &= luaval_to_rect(tolua_S, 2, &arg0);
            if (!ok) { break; }
            ExtClippingNode* ret = ExtClippingNode::create(arg0);
            object_to_luaval<ExtClippingNode>(tolua_S, "ExtClippingNode",(ExtClippingNode*)ret);
            return 1;
        }
    } while (0);
    ok  = true;
    CCLOG("%s has wrong number of arguments: %d, was expecting %d", "create",argc, 1);
    return 0;
#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_ww_ExtClippingNode_create'.",&tolua_err);
#endif
    return 0;
}
static int lua_ww_ExtClippingNode_finalize(lua_State* tolua_S)
{
    printf("luabindings: finalizing LUA object (ExtClippingNode)");
    return 0;
}

int lua_register_ww_ExtClippingNode(lua_State* tolua_S)
{
    tolua_usertype(tolua_S,"ExtClippingNode");
    tolua_cclass(tolua_S,"ExtClippingNode","ExtClippingNode","cc.Node",nullptr);

    tolua_beginmodule(tolua_S,"ExtClippingNode");
        tolua_function(tolua_S,"setTouchMoved",lua_ww_ExtClippingNode_setTouchMoved);
        tolua_function(tolua_S,"getClippingRegion",lua_ww_ExtClippingNode_getClippingRegion);
        tolua_function(tolua_S,"isTouchMoved",lua_ww_ExtClippingNode_isTouchMoved);
        tolua_function(tolua_S,"setClippingRegion",lua_ww_ExtClippingNode_setClippingRegion);
        tolua_function(tolua_S,"create", lua_ww_ExtClippingNode_create);
    tolua_endmodule(tolua_S);
    std::string typeName = typeid(ExtClippingNode).name();
    g_luaType[typeName] = "ExtClippingNode";
    g_typeCast["ExtClippingNode"] = "ExtClippingNode";
    return 1;
}

int lua_ww_ExtScrollView_isClippingToBounds(lua_State* tolua_S)
{
    int argc = 0;
    ExtScrollView* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"ExtScrollView",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (ExtScrollView*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_ww_ExtScrollView_isClippingToBounds'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 0) 
    {
        if(!ok)
            return 0;
        bool ret = cobj->isClippingToBounds();
        tolua_pushboolean(tolua_S,(bool)ret);
        return 1;
    }
    CCLOG("%s has wrong number of arguments: %d, was expecting %d \n", "isClippingToBounds",argc, 0);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_ww_ExtScrollView_isClippingToBounds'.",&tolua_err);
#endif

    return 0;
}
int lua_ww_ExtScrollView_getMinContainerOffset(lua_State* tolua_S)
{
    int argc = 0;
    ExtScrollView* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"ExtScrollView",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (ExtScrollView*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_ww_ExtScrollView_getMinContainerOffset'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 0) 
    {
        if(!ok)
            return 0;
        cocos2d::Vec2 ret = cobj->getMinContainerOffset();
        vec2_to_luaval(tolua_S, ret);
        return 1;
    }
    CCLOG("%s has wrong number of arguments: %d, was expecting %d \n", "getMinContainerOffset",argc, 0);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_ww_ExtScrollView_getMinContainerOffset'.",&tolua_err);
#endif

    return 0;
}
int lua_ww_ExtScrollView_unregisterActionEventHandler(lua_State* tolua_S)
{
    int argc = 0;
    ExtScrollView* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"ExtScrollView",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (ExtScrollView*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_ww_ExtScrollView_unregisterActionEventHandler'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 0) 
    {
        if(!ok)
            return 0;
        cobj->unregisterActionEventHandler();
        return 0;
    }
    CCLOG("%s has wrong number of arguments: %d, was expecting %d \n", "unregisterActionEventHandler",argc, 0);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_ww_ExtScrollView_unregisterActionEventHandler'.",&tolua_err);
#endif

    return 0;
}
int lua_ww_ExtScrollView_setContentOffsetInDuration(lua_State* tolua_S)
{
    int argc = 0;
    ExtScrollView* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"ExtScrollView",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (ExtScrollView*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_ww_ExtScrollView_setContentOffsetInDuration'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 2) 
    {
        cocos2d::Vec2 arg0;
        double arg1;

        ok &= luaval_to_vec2(tolua_S, 2, &arg0);

        ok &= luaval_to_number(tolua_S, 3,&arg1);
        if(!ok)
            return 0;
        cobj->setContentOffsetInDuration(arg0, arg1);
        return 0;
    }
    CCLOG("%s has wrong number of arguments: %d, was expecting %d \n", "setContentOffsetInDuration",argc, 2);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_ww_ExtScrollView_setContentOffsetInDuration'.",&tolua_err);
#endif

    return 0;
}
int lua_ww_ExtScrollView_setZoomScaleInDuration(lua_State* tolua_S)
{
    int argc = 0;
    ExtScrollView* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"ExtScrollView",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (ExtScrollView*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_ww_ExtScrollView_setZoomScaleInDuration'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 2) 
    {
        double arg0;
        double arg1;

        ok &= luaval_to_number(tolua_S, 2,&arg0);

        ok &= luaval_to_number(tolua_S, 3,&arg1);
        if(!ok)
            return 0;
        cobj->setZoomScaleInDuration(arg0, arg1);
        return 0;
    }
    CCLOG("%s has wrong number of arguments: %d, was expecting %d \n", "setZoomScaleInDuration",argc, 2);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_ww_ExtScrollView_setZoomScaleInDuration'.",&tolua_err);
#endif

    return 0;
}
int lua_ww_ExtScrollView_setContainer(lua_State* tolua_S)
{
    int argc = 0;
    ExtScrollView* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"ExtScrollView",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (ExtScrollView*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_ww_ExtScrollView_setContainer'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 1) 
    {
        cocos2d::Node* arg0;

        ok &= luaval_to_object<cocos2d::Node>(tolua_S, 2, "cc.Node",&arg0);
        if(!ok)
            return 0;
        cobj->setContainer(arg0);
        return 0;
    }
    CCLOG("%s has wrong number of arguments: %d, was expecting %d \n", "setContainer",argc, 1);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_ww_ExtScrollView_setContainer'.",&tolua_err);
#endif

    return 0;
}
int lua_ww_ExtScrollView_setZoomScaleLimit(lua_State* tolua_S)
{
    int argc = 0;
    ExtScrollView* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"ExtScrollView",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (ExtScrollView*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_ww_ExtScrollView_setZoomScaleLimit'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 2) 
    {
        double arg0;
        double arg1;

        ok &= luaval_to_number(tolua_S, 2,&arg0);

        ok &= luaval_to_number(tolua_S, 3,&arg1);
        if(!ok)
            return 0;
        cobj->setZoomScaleLimit(arg0, arg1);
        return 0;
    }
    CCLOG("%s has wrong number of arguments: %d, was expecting %d \n", "setZoomScaleLimit",argc, 2);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_ww_ExtScrollView_setZoomScaleLimit'.",&tolua_err);
#endif

    return 0;
}
int lua_ww_ExtScrollView_getContainer(lua_State* tolua_S)
{
    int argc = 0;
    ExtScrollView* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"ExtScrollView",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (ExtScrollView*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_ww_ExtScrollView_getContainer'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 0) 
    {
        if(!ok)
            return 0;
        cocos2d::Node* ret = cobj->getContainer();
        object_to_luaval<cocos2d::Node>(tolua_S, "cc.Node",(cocos2d::Node*)ret);
        return 1;
    }
    CCLOG("%s has wrong number of arguments: %d, was expecting %d \n", "getContainer",argc, 0);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_ww_ExtScrollView_getContainer'.",&tolua_err);
#endif

    return 0;
}
int lua_ww_ExtScrollView_setCanMovedOut(lua_State* tolua_S)
{
    int argc = 0;
    ExtScrollView* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"ExtScrollView",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (ExtScrollView*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_ww_ExtScrollView_setCanMovedOut'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 1) 
    {
        bool arg0;

        ok &= luaval_to_boolean(tolua_S, 2,&arg0);
        if(!ok)
            return 0;
        cobj->setCanMovedOut(arg0);
        return 0;
    }
    CCLOG("%s has wrong number of arguments: %d, was expecting %d \n", "setCanMovedOut",argc, 1);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_ww_ExtScrollView_setCanMovedOut'.",&tolua_err);
#endif

    return 0;
}
int lua_ww_ExtScrollView_getDirection(lua_State* tolua_S)
{
    int argc = 0;
    ExtScrollView* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"ExtScrollView",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (ExtScrollView*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_ww_ExtScrollView_getDirection'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 0) 
    {
        if(!ok)
            return 0;
        int ret = (int)cobj->getDirection();
        tolua_pushnumber(tolua_S,(lua_Number)ret);
        return 1;
    }
    CCLOG("%s has wrong number of arguments: %d, was expecting %d \n", "getDirection",argc, 0);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_ww_ExtScrollView_getDirection'.",&tolua_err);
#endif

    return 0;
}
int lua_ww_ExtScrollView_getZoomScale(lua_State* tolua_S)
{
    int argc = 0;
    ExtScrollView* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"ExtScrollView",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (ExtScrollView*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_ww_ExtScrollView_getZoomScale'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 0) 
    {
        if(!ok)
            return 0;
        double ret = cobj->getZoomScale();
        tolua_pushnumber(tolua_S,(lua_Number)ret);
        return 1;
    }
    CCLOG("%s has wrong number of arguments: %d, was expecting %d \n", "getZoomScale",argc, 0);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_ww_ExtScrollView_getZoomScale'.",&tolua_err);
#endif

    return 0;
}
int lua_ww_ExtScrollView_pause(lua_State* tolua_S)
{
    int argc = 0;
    ExtScrollView* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"ExtScrollView",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (ExtScrollView*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_ww_ExtScrollView_pause'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 1) 
    {
        cocos2d::Ref* arg0;

        ok &= luaval_to_object<cocos2d::Ref>(tolua_S, 2, "cc.Ref",&arg0);
        if(!ok)
            return 0;
        cobj->pause(arg0);
        return 0;
    }
    CCLOG("%s has wrong number of arguments: %d, was expecting %d \n", "pause",argc, 1);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_ww_ExtScrollView_pause'.",&tolua_err);
#endif

    return 0;
}
int lua_ww_ExtScrollView_setDirection(lua_State* tolua_S)
{
    int argc = 0;
    ExtScrollView* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"ExtScrollView",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (ExtScrollView*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_ww_ExtScrollView_setDirection'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 1) 
    {
        ScrollViewDirection arg0;

        ok &= luaval_to_int32(tolua_S, 2,(int *)&arg0);
        if(!ok)
            return 0;
        cobj->setDirection(arg0);
        return 0;
    }
    CCLOG("%s has wrong number of arguments: %d, was expecting %d \n", "setDirection",argc, 1);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_ww_ExtScrollView_setDirection'.",&tolua_err);
#endif

    return 0;
}
int lua_ww_ExtScrollView_isEnabled(lua_State* tolua_S)
{
    int argc = 0;
    ExtScrollView* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"ExtScrollView",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (ExtScrollView*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_ww_ExtScrollView_isEnabled'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 0) 
    {
        if(!ok)
            return 0;
        bool ret = cobj->isEnabled();
        tolua_pushboolean(tolua_S,(bool)ret);
        return 1;
    }
    CCLOG("%s has wrong number of arguments: %d, was expecting %d \n", "isEnabled",argc, 0);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_ww_ExtScrollView_isEnabled'.",&tolua_err);
#endif

    return 0;
}
int lua_ww_ExtScrollView_setBounceable(lua_State* tolua_S)
{
    int argc = 0;
    ExtScrollView* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"ExtScrollView",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (ExtScrollView*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_ww_ExtScrollView_setBounceable'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 1) 
    {
        bool arg0;

        ok &= luaval_to_boolean(tolua_S, 2,&arg0);
        if(!ok)
            return 0;
        cobj->setBounceable(arg0);
        return 0;
    }
    CCLOG("%s has wrong number of arguments: %d, was expecting %d \n", "setBounceable",argc, 1);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_ww_ExtScrollView_setBounceable'.",&tolua_err);
#endif

    return 0;
}
int lua_ww_ExtScrollView_setContentOffset(lua_State* tolua_S)
{
    int argc = 0;
    ExtScrollView* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"ExtScrollView",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (ExtScrollView*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_ww_ExtScrollView_setContentOffset'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 1) 
    {
        cocos2d::Vec2 arg0;

        ok &= luaval_to_vec2(tolua_S, 2, &arg0);
        if(!ok)
            return 0;
        cobj->setContentOffset(arg0);
        return 0;
    }
    if (argc == 2) 
    {
        cocos2d::Vec2 arg0;
        bool arg1;

        ok &= luaval_to_vec2(tolua_S, 2, &arg0);

        ok &= luaval_to_boolean(tolua_S, 3,&arg1);
        if(!ok)
            return 0;
        cobj->setContentOffset(arg0, arg1);
        return 0;
    }
    CCLOG("%s has wrong number of arguments: %d, was expecting %d \n", "setContentOffset",argc, 1);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_ww_ExtScrollView_setContentOffset'.",&tolua_err);
#endif

    return 0;
}
int lua_ww_ExtScrollView_isDragging(lua_State* tolua_S)
{
    int argc = 0;
    ExtScrollView* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"ExtScrollView",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (ExtScrollView*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_ww_ExtScrollView_isDragging'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 0) 
    {
        if(!ok)
            return 0;
        bool ret = cobj->isDragging();
        tolua_pushboolean(tolua_S,(bool)ret);
        return 1;
    }
    CCLOG("%s has wrong number of arguments: %d, was expecting %d \n", "isDragging",argc, 0);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_ww_ExtScrollView_isDragging'.",&tolua_err);
#endif

    return 0;
}
int lua_ww_ExtScrollView_isBounceable(lua_State* tolua_S)
{
    int argc = 0;
    ExtScrollView* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"ExtScrollView",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (ExtScrollView*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_ww_ExtScrollView_isBounceable'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 0) 
    {
        if(!ok)
            return 0;
        bool ret = cobj->isBounceable();
        tolua_pushboolean(tolua_S,(bool)ret);
        return 1;
    }
    CCLOG("%s has wrong number of arguments: %d, was expecting %d \n", "isBounceable",argc, 0);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_ww_ExtScrollView_isBounceable'.",&tolua_err);
#endif

    return 0;
}
int lua_ww_ExtScrollView_getMaxContainerOffset(lua_State* tolua_S)
{
    int argc = 0;
    ExtScrollView* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"ExtScrollView",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (ExtScrollView*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_ww_ExtScrollView_getMaxContainerOffset'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 0) 
    {
        if(!ok)
            return 0;
        cocos2d::Vec2 ret = cobj->getMaxContainerOffset();
        vec2_to_luaval(tolua_S, ret);
        return 1;
    }
    CCLOG("%s has wrong number of arguments: %d, was expecting %d \n", "getMaxContainerOffset",argc, 0);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_ww_ExtScrollView_getMaxContainerOffset'.",&tolua_err);
#endif

    return 0;
}
int lua_ww_ExtScrollView_setEnabled(lua_State* tolua_S)
{
    int argc = 0;
    ExtScrollView* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"ExtScrollView",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (ExtScrollView*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_ww_ExtScrollView_setEnabled'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 1) 
    {
        bool arg0;

        ok &= luaval_to_boolean(tolua_S, 2,&arg0);
        if(!ok)
            return 0;
        cobj->setEnabled(arg0);
        return 0;
    }
    CCLOG("%s has wrong number of arguments: %d, was expecting %d \n", "setEnabled",argc, 1);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_ww_ExtScrollView_setEnabled'.",&tolua_err);
#endif

    return 0;
}
int lua_ww_ExtScrollView_getContentOffset(lua_State* tolua_S)
{
    int argc = 0;
    ExtScrollView* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"ExtScrollView",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (ExtScrollView*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_ww_ExtScrollView_getContentOffset'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 0) 
    {
        if(!ok)
            return 0;
        cocos2d::Vec2 ret = cobj->getContentOffset();
        vec2_to_luaval(tolua_S, ret);
        return 1;
    }
    CCLOG("%s has wrong number of arguments: %d, was expecting %d \n", "getContentOffset",argc, 0);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_ww_ExtScrollView_getContentOffset'.",&tolua_err);
#endif

    return 0;
}
int lua_ww_ExtScrollView_getCanMovedOut(lua_State* tolua_S)
{
    int argc = 0;
    ExtScrollView* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"ExtScrollView",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (ExtScrollView*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_ww_ExtScrollView_getCanMovedOut'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 0) 
    {
        if(!ok)
            return 0;
        bool ret = cobj->getCanMovedOut();
        tolua_pushboolean(tolua_S,(bool)ret);
        return 1;
    }
    CCLOG("%s has wrong number of arguments: %d, was expecting %d \n", "getCanMovedOut",argc, 0);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_ww_ExtScrollView_getCanMovedOut'.",&tolua_err);
#endif

    return 0;
}
int lua_ww_ExtScrollView_resume(lua_State* tolua_S)
{
    int argc = 0;
    ExtScrollView* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"ExtScrollView",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (ExtScrollView*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_ww_ExtScrollView_resume'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 1) 
    {
        cocos2d::Ref* arg0;

        ok &= luaval_to_object<cocos2d::Ref>(tolua_S, 2, "cc.Ref",&arg0);
        if(!ok)
            return 0;
        cobj->resume(arg0);
        return 0;
    }
    CCLOG("%s has wrong number of arguments: %d, was expecting %d \n", "resume",argc, 1);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_ww_ExtScrollView_resume'.",&tolua_err);
#endif

    return 0;
}
int lua_ww_ExtScrollView_setClippingToBounds(lua_State* tolua_S)
{
    int argc = 0;
    ExtScrollView* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"ExtScrollView",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (ExtScrollView*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_ww_ExtScrollView_setClippingToBounds'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 1) 
    {
        bool arg0;

        ok &= luaval_to_boolean(tolua_S, 2,&arg0);
        if(!ok)
            return 0;
        cobj->setClippingToBounds(arg0);
        return 0;
    }
    CCLOG("%s has wrong number of arguments: %d, was expecting %d \n", "setClippingToBounds",argc, 1);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_ww_ExtScrollView_setClippingToBounds'.",&tolua_err);
#endif

    return 0;
}
int lua_ww_ExtScrollView_setViewSize(lua_State* tolua_S)
{
    int argc = 0;
    ExtScrollView* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"ExtScrollView",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (ExtScrollView*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_ww_ExtScrollView_setViewSize'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 1) 
    {
        cocos2d::Size arg0;

        ok &= luaval_to_size(tolua_S, 2, &arg0);
        if(!ok)
            return 0;
        cobj->setViewSize(arg0);
        return 0;
    }
    CCLOG("%s has wrong number of arguments: %d, was expecting %d \n", "setViewSize",argc, 1);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_ww_ExtScrollView_setViewSize'.",&tolua_err);
#endif

    return 0;
}
int lua_ww_ExtScrollView_getViewSize(lua_State* tolua_S)
{
    int argc = 0;
    ExtScrollView* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"ExtScrollView",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (ExtScrollView*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_ww_ExtScrollView_getViewSize'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 0) 
    {
        if(!ok)
            return 0;
        cocos2d::Size ret = cobj->getViewSize();
        size_to_luaval(tolua_S, ret);
        return 1;
    }
    CCLOG("%s has wrong number of arguments: %d, was expecting %d \n", "getViewSize",argc, 0);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_ww_ExtScrollView_getViewSize'.",&tolua_err);
#endif

    return 0;
}
int lua_ww_ExtScrollView_isTouchMoved(lua_State* tolua_S)
{
    int argc = 0;
    ExtScrollView* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"ExtScrollView",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (ExtScrollView*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_ww_ExtScrollView_isTouchMoved'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 0) 
    {
        if(!ok)
            return 0;
        bool ret = cobj->isTouchMoved();
        tolua_pushboolean(tolua_S,(bool)ret);
        return 1;
    }
    CCLOG("%s has wrong number of arguments: %d, was expecting %d \n", "isTouchMoved",argc, 0);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_ww_ExtScrollView_isTouchMoved'.",&tolua_err);
#endif

    return 0;
}
int lua_ww_ExtScrollView_isNodeVisible(lua_State* tolua_S)
{
    int argc = 0;
    ExtScrollView* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"ExtScrollView",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (ExtScrollView*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_ww_ExtScrollView_isNodeVisible'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 1) 
    {
        cocos2d::Node* arg0;

        ok &= luaval_to_object<cocos2d::Node>(tolua_S, 2, "cc.Node",&arg0);
        if(!ok)
            return 0;
        bool ret = cobj->isNodeVisible(arg0);
        tolua_pushboolean(tolua_S,(bool)ret);
        return 1;
    }
    CCLOG("%s has wrong number of arguments: %d, was expecting %d \n", "isNodeVisible",argc, 1);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_ww_ExtScrollView_isNodeVisible'.",&tolua_err);
#endif

    return 0;
}
int lua_ww_ExtScrollView_setZoomScale(lua_State* tolua_S)
{
    int argc = 0;
    ExtScrollView* cobj = nullptr;
    bool ok  = true;
#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif

#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"ExtScrollView",0,&tolua_err)) goto tolua_lerror;
#endif
    cobj = (ExtScrollView*)tolua_tousertype(tolua_S,1,0);
#if COCOS2D_DEBUG >= 1
    if (!cobj)
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_ww_ExtScrollView_setZoomScale'", nullptr);
        return 0;
    }
#endif
    argc = lua_gettop(tolua_S)-1;
    do{
        if (argc == 2) {
            double arg0;
            ok &= luaval_to_number(tolua_S, 2,&arg0);

            if (!ok) { break; }
            bool arg1;
            ok &= luaval_to_boolean(tolua_S, 3,&arg1);

            if (!ok) { break; }
            cobj->setZoomScale(arg0, arg1);
            return 0;
        }
    }while(0);
    ok  = true;
    do{
        if (argc == 1) {
            double arg0;
            ok &= luaval_to_number(tolua_S, 2,&arg0);

            if (!ok) { break; }
            cobj->setZoomScale(arg0);
            return 0;
        }
    }while(0);
    ok  = true;
    CCLOG("%s has wrong number of arguments: %d, was expecting %d \n", "setZoomScale",argc, 1);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_ww_ExtScrollView_setZoomScale'.",&tolua_err);
#endif

    return 0;
}
int lua_ww_ExtScrollView_create(lua_State* tolua_S)
{
    int argc = 0;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif

#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertable(tolua_S,1,"ExtScrollView",0,&tolua_err)) goto tolua_lerror;
#endif

    argc = lua_gettop(tolua_S) - 1;

    if (argc == 1)
    {
        cocos2d::Size arg0;
        ok &= luaval_to_size(tolua_S, 2, &arg0);
        if(!ok)
            return 0;
        ExtScrollView* ret = ExtScrollView::create(arg0);
        object_to_luaval<ExtScrollView>(tolua_S, "ExtScrollView",(ExtScrollView*)ret);
        return 1;
    }
    if (argc == 2)
    {
        cocos2d::Size arg0;
        cocos2d::Node* arg1;
        ok &= luaval_to_size(tolua_S, 2, &arg0);
        ok &= luaval_to_object<cocos2d::Node>(tolua_S, 3, "cc.Node",&arg1);
        if(!ok)
            return 0;
        ExtScrollView* ret = ExtScrollView::create(arg0, arg1);
        object_to_luaval<ExtScrollView>(tolua_S, "ExtScrollView",(ExtScrollView*)ret);
        return 1;
    }
    CCLOG("%s has wrong number of arguments: %d, was expecting %d\n ", "create",argc, 1);
    return 0;
#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_ww_ExtScrollView_create'.",&tolua_err);
#endif
    return 0;
}
int lua_ww_ExtScrollView_convertDistanceFromPointToInch(lua_State* tolua_S)
{
    int argc = 0;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif

#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertable(tolua_S,1,"ExtScrollView",0,&tolua_err)) goto tolua_lerror;
#endif

    argc = lua_gettop(tolua_S) - 1;

    if (argc == 1)
    {
        double arg0;
        ok &= luaval_to_number(tolua_S, 2,&arg0);
        if(!ok)
            return 0;
        double ret = ExtScrollView::convertDistanceFromPointToInch(arg0);
        tolua_pushnumber(tolua_S,(lua_Number)ret);
        return 1;
    }
    CCLOG("%s has wrong number of arguments: %d, was expecting %d\n ", "convertDistanceFromPointToInch",argc, 1);
    return 0;
#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_ww_ExtScrollView_convertDistanceFromPointToInch'.",&tolua_err);
#endif
    return 0;
}
static int lua_ww_ExtScrollView_finalize(lua_State* tolua_S)
{
    printf("luabindings: finalizing LUA object (ExtScrollView)");
    return 0;
}

int lua_register_ww_ExtScrollView(lua_State* tolua_S)
{
    tolua_usertype(tolua_S,"ExtScrollView");
    tolua_cclass(tolua_S,"ExtScrollView","ExtScrollView","cc.Layer",nullptr);

    tolua_beginmodule(tolua_S,"ExtScrollView");
        tolua_function(tolua_S,"isClippingToBounds",lua_ww_ExtScrollView_isClippingToBounds);
        tolua_function(tolua_S,"getMinContainerOffset",lua_ww_ExtScrollView_getMinContainerOffset);
        tolua_function(tolua_S,"unregisterActionEventHandler",lua_ww_ExtScrollView_unregisterActionEventHandler);
        tolua_function(tolua_S,"setContentOffsetInDuration",lua_ww_ExtScrollView_setContentOffsetInDuration);
        tolua_function(tolua_S,"setZoomScaleInDuration",lua_ww_ExtScrollView_setZoomScaleInDuration);
        tolua_function(tolua_S,"setContainer",lua_ww_ExtScrollView_setContainer);
        tolua_function(tolua_S,"setZoomScaleLimit",lua_ww_ExtScrollView_setZoomScaleLimit);
        tolua_function(tolua_S,"getContainer",lua_ww_ExtScrollView_getContainer);
        tolua_function(tolua_S,"setCanMovedOut",lua_ww_ExtScrollView_setCanMovedOut);
        tolua_function(tolua_S,"getDirection",lua_ww_ExtScrollView_getDirection);
        tolua_function(tolua_S,"getZoomScale",lua_ww_ExtScrollView_getZoomScale);
        tolua_function(tolua_S,"pause",lua_ww_ExtScrollView_pause);
        tolua_function(tolua_S,"setDirection",lua_ww_ExtScrollView_setDirection);
        tolua_function(tolua_S,"isEnabled",lua_ww_ExtScrollView_isEnabled);
        tolua_function(tolua_S,"setBounceable",lua_ww_ExtScrollView_setBounceable);
        tolua_function(tolua_S,"setContentOffset",lua_ww_ExtScrollView_setContentOffset);
        tolua_function(tolua_S,"isDragging",lua_ww_ExtScrollView_isDragging);
        tolua_function(tolua_S,"isBounceable",lua_ww_ExtScrollView_isBounceable);
        tolua_function(tolua_S,"getMaxContainerOffset",lua_ww_ExtScrollView_getMaxContainerOffset);
        tolua_function(tolua_S,"setEnabled",lua_ww_ExtScrollView_setEnabled);
        tolua_function(tolua_S,"getContentOffset",lua_ww_ExtScrollView_getContentOffset);
        tolua_function(tolua_S,"getCanMovedOut",lua_ww_ExtScrollView_getCanMovedOut);
        tolua_function(tolua_S,"resume",lua_ww_ExtScrollView_resume);
        tolua_function(tolua_S,"setClippingToBounds",lua_ww_ExtScrollView_setClippingToBounds);
        tolua_function(tolua_S,"setViewSize",lua_ww_ExtScrollView_setViewSize);
        tolua_function(tolua_S,"getViewSize",lua_ww_ExtScrollView_getViewSize);
        tolua_function(tolua_S,"isTouchMoved",lua_ww_ExtScrollView_isTouchMoved);
        tolua_function(tolua_S,"isNodeVisible",lua_ww_ExtScrollView_isNodeVisible);
        tolua_function(tolua_S,"setZoomScale",lua_ww_ExtScrollView_setZoomScale);
        tolua_function(tolua_S,"create", lua_ww_ExtScrollView_create);
        tolua_function(tolua_S,"convertDistanceFromPointToInch", lua_ww_ExtScrollView_convertDistanceFromPointToInch);
    tolua_endmodule(tolua_S);
    std::string typeName = typeid(ExtScrollView).name();
    g_luaType[typeName] = "ExtScrollView";
    g_typeCast["ExtScrollView"] = "ExtScrollView";
    return 1;
}

int lua_ww_FontManager_addFont(lua_State* tolua_S)
{
    int argc = 0;
    FontManager* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"FontManager",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (FontManager*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_ww_FontManager_addFont'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 1) 
    {
        std::string arg0;

        ok &= luaval_to_std_string(tolua_S, 2,&arg0);
        if(!ok)
            return 0;
        int ret = cobj->addFont(arg0);
        tolua_pushnumber(tolua_S,(lua_Number)ret);
        return 1;
    }
    if (argc == 2) 
    {
        std::string arg0;
        int arg1;

        ok &= luaval_to_std_string(tolua_S, 2,&arg0);

        ok &= luaval_to_int32(tolua_S, 3,(int *)&arg1);
        if(!ok)
            return 0;
        int ret = cobj->addFont(arg0, arg1);
        tolua_pushnumber(tolua_S,(lua_Number)ret);
        return 1;
    }
    if (argc == 3) 
    {
        std::string arg0;
        int arg1;
        int arg2;

        ok &= luaval_to_std_string(tolua_S, 2,&arg0);

        ok &= luaval_to_int32(tolua_S, 3,(int *)&arg1);

        ok &= luaval_to_int32(tolua_S, 4,(int *)&arg2);
        if(!ok)
            return 0;
        int ret = cobj->addFont(arg0, arg1, arg2);
        tolua_pushnumber(tolua_S,(lua_Number)ret);
        return 1;
    }
    if (argc == 4) 
    {
        std::string arg0;
        int arg1;
        int arg2;
        cocos2d::Color4B arg3;

        ok &= luaval_to_std_string(tolua_S, 2,&arg0);

        ok &= luaval_to_int32(tolua_S, 3,(int *)&arg1);

        ok &= luaval_to_int32(tolua_S, 4,(int *)&arg2);

        ok &=luaval_to_color4b(tolua_S, 5, &arg3);
        if(!ok)
            return 0;
        int ret = cobj->addFont(arg0, arg1, arg2, arg3);
        tolua_pushnumber(tolua_S,(lua_Number)ret);
        return 1;
    }
    CCLOG("%s has wrong number of arguments: %d, was expecting %d \n", "addFont",argc, 1);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_ww_FontManager_addFont'.",&tolua_err);
#endif

    return 0;
}
int lua_ww_FontManager_clear(lua_State* tolua_S)
{
    int argc = 0;
    FontManager* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"FontManager",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (FontManager*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_ww_FontManager_clear'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 0) 
    {
        if(!ok)
            return 0;
        cobj->clear();
        return 0;
    }
    CCLOG("%s has wrong number of arguments: %d, was expecting %d \n", "clear",argc, 0);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_ww_FontManager_clear'.",&tolua_err);
#endif

    return 0;
}
int lua_ww_FontManager_getColorCount(lua_State* tolua_S)
{
    int argc = 0;
    FontManager* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"FontManager",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (FontManager*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_ww_FontManager_getColorCount'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 0) 
    {
        if(!ok)
            return 0;
        int ret = cobj->getColorCount();
        tolua_pushnumber(tolua_S,(lua_Number)ret);
        return 1;
    }
    CCLOG("%s has wrong number of arguments: %d, was expecting %d \n", "getColorCount",argc, 0);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_ww_FontManager_getColorCount'.",&tolua_err);
#endif

    return 0;
}
int lua_ww_FontManager_getFontCount(lua_State* tolua_S)
{
    int argc = 0;
    FontManager* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"FontManager",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (FontManager*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_ww_FontManager_getFontCount'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 0) 
    {
        if(!ok)
            return 0;
        int ret = cobj->getFontCount();
        tolua_pushnumber(tolua_S,(lua_Number)ret);
        return 1;
    }
    CCLOG("%s has wrong number of arguments: %d, was expecting %d \n", "getFontCount",argc, 0);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_ww_FontManager_getFontCount'.",&tolua_err);
#endif

    return 0;
}
int lua_ww_FontManager_addColor(lua_State* tolua_S)
{
    int argc = 0;
    FontManager* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"FontManager",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (FontManager*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_ww_FontManager_addColor'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 1) 
    {
        cocos2d::Color3B arg0;

        ok &= luaval_to_color3b(tolua_S, 2, &arg0);
        if(!ok)
            return 0;
        int ret = cobj->addColor(arg0);
        tolua_pushnumber(tolua_S,(lua_Number)ret);
        return 1;
    }
    if (argc == 2) 
    {
        cocos2d::Color3B arg0;
        std::string arg1;

        ok &= luaval_to_color3b(tolua_S, 2, &arg0);

        ok &= luaval_to_std_string(tolua_S, 3,&arg1);
        if(!ok)
            return 0;
        int ret = cobj->addColor(arg0, arg1);
        tolua_pushnumber(tolua_S,(lua_Number)ret);
        return 1;
    }
    CCLOG("%s has wrong number of arguments: %d, was expecting %d \n", "addColor",argc, 1);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_ww_FontManager_addColor'.",&tolua_err);
#endif

    return 0;
}
int lua_ww_FontManager_getDefaultColor(lua_State* tolua_S)
{
    int argc = 0;
    FontManager* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"FontManager",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (FontManager*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_ww_FontManager_getDefaultColor'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 0) 
    {
        if(!ok)
            return 0;
        cocos2d::Color3B ret = cobj->getDefaultColor();
        color3b_to_luaval(tolua_S, ret);
        return 1;
    }
    CCLOG("%s has wrong number of arguments: %d, was expecting %d \n", "getDefaultColor",argc, 0);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_ww_FontManager_getDefaultColor'.",&tolua_err);
#endif

    return 0;
}
int lua_ww_FontManager_getColor(lua_State* tolua_S)
{
    int argc = 0;
    FontManager* cobj = nullptr;
    bool ok  = true;
#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif

#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"FontManager",0,&tolua_err)) goto tolua_lerror;
#endif
    cobj = (FontManager*)tolua_tousertype(tolua_S,1,0);
#if COCOS2D_DEBUG >= 1
    if (!cobj)
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_ww_FontManager_getColor'", nullptr);
        return 0;
    }
#endif
    argc = lua_gettop(tolua_S)-1;
    do{
        if (argc == 1) {
            std::string arg0;
            ok &= luaval_to_std_string(tolua_S, 2,&arg0);

            if (!ok) { break; }
            cocos2d::Color3B ret = cobj->getColor(arg0);
            color3b_to_luaval(tolua_S, ret);
            return 1;
        }
    }while(0);
    ok  = true;
    do{
        if (argc == 1) {
            int arg0;
            ok &= luaval_to_int32(tolua_S, 2,(int *)&arg0);

            if (!ok) { break; }
            cocos2d::Color3B ret = cobj->getColor(arg0);
            color3b_to_luaval(tolua_S, ret);
            return 1;
        }
    }while(0);
    ok  = true;
    CCLOG("%s has wrong number of arguments: %d, was expecting %d \n", "getColor",argc, 1);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_ww_FontManager_getColor'.",&tolua_err);
#endif

    return 0;
}
int lua_ww_FontManager_createLabel(lua_State* tolua_S)
{
    int argc = 0;
    FontManager* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"FontManager",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (FontManager*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_ww_FontManager_createLabel'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 1) 
    {
        int arg0;

        ok &= luaval_to_int32(tolua_S, 2,(int *)&arg0);
        if(!ok)
            return 0;
        cocos2d::Label* ret = cobj->createLabel(arg0);
        object_to_luaval<cocos2d::Label>(tolua_S, "cc.Label",(cocos2d::Label*)ret);
        return 1;
    }
    if (argc == 2) 
    {
        int arg0;
        std::string arg1;

        ok &= luaval_to_int32(tolua_S, 2,(int *)&arg0);

        ok &= luaval_to_std_string(tolua_S, 3,&arg1);
        if(!ok)
            return 0;
        cocos2d::Label* ret = cobj->createLabel(arg0, arg1);
        object_to_luaval<cocos2d::Label>(tolua_S, "cc.Label",(cocos2d::Label*)ret);
        return 1;
    }
    if (argc == 3) 
    {
        int arg0;
        std::string arg1;
        int arg2;

        ok &= luaval_to_int32(tolua_S, 2,(int *)&arg0);

        ok &= luaval_to_std_string(tolua_S, 3,&arg1);

        ok &= luaval_to_int32(tolua_S, 4,(int *)&arg2);
        if(!ok)
            return 0;
        cocos2d::Label* ret = cobj->createLabel(arg0, arg1, arg2);
        object_to_luaval<cocos2d::Label>(tolua_S, "cc.Label",(cocos2d::Label*)ret);
        return 1;
    }
    CCLOG("%s has wrong number of arguments: %d, was expecting %d \n", "createLabel",argc, 1);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_ww_FontManager_createLabel'.",&tolua_err);
#endif

    return 0;
}
int lua_ww_FontManager_createSystemFontLabel(lua_State* tolua_S)
{
    int argc = 0;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif

#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertable(tolua_S,1,"FontManager",0,&tolua_err)) goto tolua_lerror;
#endif

    argc = lua_gettop(tolua_S) - 1;

    if (argc == 2)
    {
        std::string arg0;
        int arg1;
        ok &= luaval_to_std_string(tolua_S, 2,&arg0);
        ok &= luaval_to_int32(tolua_S, 3,(int *)&arg1);
        if(!ok)
            return 0;
        cocos2d::Label* ret = FontManager::createSystemFontLabel(arg0, arg1);
        object_to_luaval<cocos2d::Label>(tolua_S, "cc.Label",(cocos2d::Label*)ret);
        return 1;
    }
    if (argc == 3)
    {
        std::string arg0;
        int arg1;
        int arg2;
        ok &= luaval_to_std_string(tolua_S, 2,&arg0);
        ok &= luaval_to_int32(tolua_S, 3,(int *)&arg1);
        ok &= luaval_to_int32(tolua_S, 4,(int *)&arg2);
        if(!ok)
            return 0;
        cocos2d::Label* ret = FontManager::createSystemFontLabel(arg0, arg1, arg2);
        object_to_luaval<cocos2d::Label>(tolua_S, "cc.Label",(cocos2d::Label*)ret);
        return 1;
    }
    CCLOG("%s has wrong number of arguments: %d, was expecting %d\n ", "createSystemFontLabel",argc, 2);
    return 0;
#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_ww_FontManager_createSystemFontLabel'.",&tolua_err);
#endif
    return 0;
}
int lua_ww_FontManager_getInstance(lua_State* tolua_S)
{
    int argc = 0;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif

#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertable(tolua_S,1,"FontManager",0,&tolua_err)) goto tolua_lerror;
#endif

    argc = lua_gettop(tolua_S) - 1;

    if (argc == 0)
    {
        if(!ok)
            return 0;
        FontManager* ret = FontManager::getInstance();
        object_to_luaval<FontManager>(tolua_S, "FontManager",(FontManager*)ret);
        return 1;
    }
    CCLOG("%s has wrong number of arguments: %d, was expecting %d\n ", "getInstance",argc, 0);
    return 0;
#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_ww_FontManager_getInstance'.",&tolua_err);
#endif
    return 0;
}
static int lua_ww_FontManager_finalize(lua_State* tolua_S)
{
    printf("luabindings: finalizing LUA object (FontManager)");
    return 0;
}

int lua_register_ww_FontManager(lua_State* tolua_S)
{
    tolua_usertype(tolua_S,"FontManager");
    tolua_cclass(tolua_S,"FontManager","FontManager","",nullptr);

    tolua_beginmodule(tolua_S,"FontManager");
        tolua_function(tolua_S,"addFont",lua_ww_FontManager_addFont);
        tolua_function(tolua_S,"clear",lua_ww_FontManager_clear);
        tolua_function(tolua_S,"getColorCount",lua_ww_FontManager_getColorCount);
        tolua_function(tolua_S,"getFontCount",lua_ww_FontManager_getFontCount);
        tolua_function(tolua_S,"addColor",lua_ww_FontManager_addColor);
        tolua_function(tolua_S,"getDefaultColor",lua_ww_FontManager_getDefaultColor);
        tolua_function(tolua_S,"getColor",lua_ww_FontManager_getColor);
        tolua_function(tolua_S,"createLabel",lua_ww_FontManager_createLabel);
        tolua_function(tolua_S,"createSystemFontLabel", lua_ww_FontManager_createSystemFontLabel);
        tolua_function(tolua_S,"getInstance", lua_ww_FontManager_getInstance);
    tolua_endmodule(tolua_S);
    std::string typeName = typeid(FontManager).name();
    g_luaType[typeName] = "FontManager";
    g_typeCast["FontManager"] = "FontManager";
    return 1;
}

int lua_ww_RichText_setText(lua_State* tolua_S)
{
    int argc = 0;
    RichText* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"RichText",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (RichText*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_ww_RichText_setText'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 1) 
    {
        std::string arg0;

        ok &= luaval_to_std_string(tolua_S, 2,&arg0);
        if(!ok)
            return 0;
        cobj->setText(arg0);
        return 0;
    }
    CCLOG("%s has wrong number of arguments: %d, was expecting %d \n", "setText",argc, 1);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_ww_RichText_setText'.",&tolua_err);
#endif

    return 0;
}
int lua_ww_RichText_create(lua_State* tolua_S)
{
    int argc = 0;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif

#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertable(tolua_S,1,"RichText",0,&tolua_err)) goto tolua_lerror;
#endif

    argc = lua_gettop(tolua_S) - 1;

    if (argc == 2)
    {
        std::string arg0;
        double arg1;
        ok &= luaval_to_std_string(tolua_S, 2,&arg0);
        ok &= luaval_to_number(tolua_S, 3,&arg1);
        if(!ok)
            return 0;
        RichText* ret = RichText::create(arg0, arg1);
        object_to_luaval<RichText>(tolua_S, "RichText",(RichText*)ret);
        return 1;
    }
    if (argc == 3)
    {
        std::string arg0;
        double arg1;
        double arg2;
        ok &= luaval_to_std_string(tolua_S, 2,&arg0);
        ok &= luaval_to_number(tolua_S, 3,&arg1);
        ok &= luaval_to_number(tolua_S, 4,&arg2);
        if(!ok)
            return 0;
        RichText* ret = RichText::create(arg0, arg1, arg2);
        object_to_luaval<RichText>(tolua_S, "RichText",(RichText*)ret);
        return 1;
    }
    if (argc == 4)
    {
        std::string arg0;
        double arg1;
        double arg2;
        RichTextLineVAligns arg3;
        ok &= luaval_to_std_string(tolua_S, 2,&arg0);
        ok &= luaval_to_number(tolua_S, 3,&arg1);
        ok &= luaval_to_number(tolua_S, 4,&arg2);
        ok &= luaval_to_int32(tolua_S, 5,(int *)&arg3);
        if(!ok)
            return 0;
        RichText* ret = RichText::create(arg0, arg1, arg2, arg3);
        object_to_luaval<RichText>(tolua_S, "RichText",(RichText*)ret);
        return 1;
    }
    CCLOG("%s has wrong number of arguments: %d, was expecting %d\n ", "create",argc, 2);
    return 0;
#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_ww_RichText_create'.",&tolua_err);
#endif
    return 0;
}
static int lua_ww_RichText_finalize(lua_State* tolua_S)
{
    printf("luabindings: finalizing LUA object (RichText)");
    return 0;
}

int lua_register_ww_RichText(lua_State* tolua_S)
{
    tolua_usertype(tolua_S,"RichText");
    tolua_cclass(tolua_S,"RichText","RichText","cc.Node",nullptr);

    tolua_beginmodule(tolua_S,"RichText");
        tolua_function(tolua_S,"setText",lua_ww_RichText_setText);
        tolua_function(tolua_S,"create", lua_ww_RichText_create);
    tolua_endmodule(tolua_S);
    std::string typeName = typeid(RichText).name();
    g_luaType[typeName] = "RichText";
    g_typeCast["RichText"] = "RichText";
    return 1;
}

int lua_ww_RichTextEx_assign(lua_State* tolua_S)
{
    int argc = 0;
    RichTextEx* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"RichTextEx",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (RichTextEx*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_ww_RichTextEx_assign'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 1) 
    {
        std::string arg0;

        ok &= luaval_to_std_string(tolua_S, 2,&arg0);
        if(!ok)
            return 0;
        bool ret = cobj->assign(arg0);
        tolua_pushboolean(tolua_S,(bool)ret);
        return 1;
    }
    CCLOG("%s has wrong number of arguments: %d, was expecting %d \n", "assign",argc, 1);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_ww_RichTextEx_assign'.",&tolua_err);
#endif

    return 0;
}
int lua_ww_RichTextEx_init_analyzer(lua_State* tolua_S)
{
    int argc = 0;
    RichTextEx* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"RichTextEx",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (RichTextEx*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_ww_RichTextEx_init_analyzer'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 0) 
    {
        if(!ok)
            return 0;
        cobj->init_analyzer();
        return 0;
    }
    CCLOG("%s has wrong number of arguments: %d, was expecting %d \n", "init_analyzer",argc, 0);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_ww_RichTextEx_init_analyzer'.",&tolua_err);
#endif

    return 0;
}
static int lua_ww_RichTextEx_finalize(lua_State* tolua_S)
{
    printf("luabindings: finalizing LUA object (RichTextEx)");
    return 0;
}

int lua_register_ww_RichTextEx(lua_State* tolua_S)
{
    tolua_usertype(tolua_S,"RichTextEx");
    tolua_cclass(tolua_S,"RichTextEx","RichTextEx","cc.Node",nullptr);

    tolua_beginmodule(tolua_S,"RichTextEx");
        tolua_function(tolua_S,"assign",lua_ww_RichTextEx_assign);
        tolua_function(tolua_S,"init_analyzer",lua_ww_RichTextEx_init_analyzer);
    tolua_endmodule(tolua_S);
    std::string typeName = typeid(RichTextEx).name();
    g_luaType[typeName] = "RichTextEx";
    g_typeCast["RichTextEx"] = "RichTextEx";
    return 1;
}

int lua_ww_Lightning_setTextureLength(lua_State* tolua_S)
{
    int argc = 0;
    Lightning* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"Lightning",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (Lightning*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_ww_Lightning_setTextureLength'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 1) 
    {
        double arg0;

        ok &= luaval_to_number(tolua_S, 2,&arg0);
        if(!ok)
            return 0;
        cobj->setTextureLength(arg0);
        return 0;
    }
    CCLOG("%s has wrong number of arguments: %d, was expecting %d \n", "setTextureLength",argc, 1);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_ww_Lightning_setTextureLength'.",&tolua_err);
#endif

    return 0;
}
int lua_ww_Lightning_getSpeed(lua_State* tolua_S)
{
    int argc = 0;
    Lightning* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"Lightning",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (Lightning*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_ww_Lightning_getSpeed'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 0) 
    {
        if(!ok)
            return 0;
        double ret = cobj->getSpeed();
        tolua_pushnumber(tolua_S,(lua_Number)ret);
        return 1;
    }
    CCLOG("%s has wrong number of arguments: %d, was expecting %d \n", "getSpeed",argc, 0);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_ww_Lightning_getSpeed'.",&tolua_err);
#endif

    return 0;
}
int lua_ww_Lightning_setSpeed(lua_State* tolua_S)
{
    int argc = 0;
    Lightning* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"Lightning",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (Lightning*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_ww_Lightning_setSpeed'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 1) 
    {
        double arg0;

        ok &= luaval_to_number(tolua_S, 2,&arg0);
        if(!ok)
            return 0;
        cobj->setSpeed(arg0);
        return 0;
    }
    CCLOG("%s has wrong number of arguments: %d, was expecting %d \n", "setSpeed",argc, 1);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_ww_Lightning_setSpeed'.",&tolua_err);
#endif

    return 0;
}
int lua_ww_Lightning_setPoints(lua_State* tolua_S)
{
    int argc = 0;
    Lightning* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"Lightning",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (Lightning*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_ww_Lightning_setPoints'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 1) 
    {
        std::vector<cocos2d::Vec3, std::allocator<cocos2d::Vec3> > arg0;

        ok &= luaval_to_std_vector_vec3(tolua_S, 2, &arg0);
        if(!ok)
            return 0;
        bool ret = cobj->setPoints(arg0);
        tolua_pushboolean(tolua_S,(bool)ret);
        return 1;
    }
    CCLOG("%s has wrong number of arguments: %d, was expecting %d \n", "setPoints",argc, 1);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_ww_Lightning_setPoints'.",&tolua_err);
#endif

    return 0;
}
int lua_ww_Lightning_getStep(lua_State* tolua_S)
{
    int argc = 0;
    Lightning* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"Lightning",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (Lightning*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_ww_Lightning_getStep'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 0) 
    {
        if(!ok)
            return 0;
        double ret = cobj->getStep();
        tolua_pushnumber(tolua_S,(lua_Number)ret);
        return 1;
    }
    CCLOG("%s has wrong number of arguments: %d, was expecting %d \n", "getStep",argc, 0);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_ww_Lightning_getStep'.",&tolua_err);
#endif

    return 0;
}
int lua_ww_Lightning_getCenter(lua_State* tolua_S)
{
    int argc = 0;
    Lightning* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"Lightning",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (Lightning*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_ww_Lightning_getCenter'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 0) 
    {
        if(!ok)
            return 0;
        cocos2d::Vec3 ret = cobj->getCenter();
        vec3_to_luaval(tolua_S, ret);
        return 1;
    }
    CCLOG("%s has wrong number of arguments: %d, was expecting %d \n", "getCenter",argc, 0);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_ww_Lightning_getCenter'.",&tolua_err);
#endif

    return 0;
}
int lua_ww_Lightning_getWidth(lua_State* tolua_S)
{
    int argc = 0;
    Lightning* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"Lightning",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (Lightning*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_ww_Lightning_getWidth'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 0) 
    {
        if(!ok)
            return 0;
        double ret = cobj->getWidth();
        tolua_pushnumber(tolua_S,(lua_Number)ret);
        return 1;
    }
    CCLOG("%s has wrong number of arguments: %d, was expecting %d \n", "getWidth",argc, 0);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_ww_Lightning_getWidth'.",&tolua_err);
#endif

    return 0;
}
int lua_ww_Lightning_setStep(lua_State* tolua_S)
{
    int argc = 0;
    Lightning* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"Lightning",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (Lightning*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_ww_Lightning_setStep'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 1) 
    {
        double arg0;

        ok &= luaval_to_number(tolua_S, 2,&arg0);
        if(!ok)
            return 0;
        cobj->setStep(arg0);
        return 0;
    }
    CCLOG("%s has wrong number of arguments: %d, was expecting %d \n", "setStep",argc, 1);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_ww_Lightning_setStep'.",&tolua_err);
#endif

    return 0;
}
int lua_ww_Lightning_init(lua_State* tolua_S)
{
    int argc = 0;
    Lightning* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"Lightning",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (Lightning*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_ww_Lightning_init'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 0) 
    {
        if(!ok)
            return 0;
        bool ret = cobj->init();
        tolua_pushboolean(tolua_S,(bool)ret);
        return 1;
    }
    CCLOG("%s has wrong number of arguments: %d, was expecting %d \n", "init",argc, 0);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_ww_Lightning_init'.",&tolua_err);
#endif

    return 0;
}
int lua_ww_Lightning_setWidth(lua_State* tolua_S)
{
    int argc = 0;
    Lightning* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"Lightning",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (Lightning*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_ww_Lightning_setWidth'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 1) 
    {
        double arg0;

        ok &= luaval_to_number(tolua_S, 2,&arg0);
        if(!ok)
            return 0;
        cobj->setWidth(arg0);
        return 0;
    }
    CCLOG("%s has wrong number of arguments: %d, was expecting %d \n", "setWidth",argc, 1);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_ww_Lightning_setWidth'.",&tolua_err);
#endif

    return 0;
}
int lua_ww_Lightning_getTextureLength(lua_State* tolua_S)
{
    int argc = 0;
    Lightning* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"Lightning",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (Lightning*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_ww_Lightning_getTextureLength'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 0) 
    {
        if(!ok)
            return 0;
        double ret = cobj->getTextureLength();
        tolua_pushnumber(tolua_S,(lua_Number)ret);
        return 1;
    }
    CCLOG("%s has wrong number of arguments: %d, was expecting %d \n", "getTextureLength",argc, 0);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_ww_Lightning_getTextureLength'.",&tolua_err);
#endif

    return 0;
}
int lua_ww_Lightning_create(lua_State* tolua_S)
{
    int argc = 0;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif

#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertable(tolua_S,1,"Lightning",0,&tolua_err)) goto tolua_lerror;
#endif

    argc = lua_gettop(tolua_S) - 1;

    if (argc == 0)
    {
        if(!ok)
            return 0;
        Lightning* ret = Lightning::create();
        object_to_luaval<Lightning>(tolua_S, "Lightning",(Lightning*)ret);
        return 1;
    }
    CCLOG("%s has wrong number of arguments: %d, was expecting %d\n ", "create",argc, 0);
    return 0;
#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_ww_Lightning_create'.",&tolua_err);
#endif
    return 0;
}
static int lua_ww_Lightning_finalize(lua_State* tolua_S)
{
    printf("luabindings: finalizing LUA object (Lightning)");
    return 0;
}

int lua_register_ww_Lightning(lua_State* tolua_S)
{
    tolua_usertype(tolua_S,"Lightning");
    tolua_cclass(tolua_S,"Lightning","Lightning","cc.Layer",nullptr);

    tolua_beginmodule(tolua_S,"Lightning");
        tolua_function(tolua_S,"setTextureLength",lua_ww_Lightning_setTextureLength);
        tolua_function(tolua_S,"getSpeed",lua_ww_Lightning_getSpeed);
        tolua_function(tolua_S,"setSpeed",lua_ww_Lightning_setSpeed);
        tolua_function(tolua_S,"setPoints",lua_ww_Lightning_setPoints);
        tolua_function(tolua_S,"getStep",lua_ww_Lightning_getStep);
        tolua_function(tolua_S,"getCenter",lua_ww_Lightning_getCenter);
        tolua_function(tolua_S,"getWidth",lua_ww_Lightning_getWidth);
        tolua_function(tolua_S,"setStep",lua_ww_Lightning_setStep);
        tolua_function(tolua_S,"init",lua_ww_Lightning_init);
        tolua_function(tolua_S,"setWidth",lua_ww_Lightning_setWidth);
        tolua_function(tolua_S,"getTextureLength",lua_ww_Lightning_getTextureLength);
        tolua_function(tolua_S,"create", lua_ww_Lightning_create);
    tolua_endmodule(tolua_S);
    std::string typeName = typeid(Lightning).name();
    g_luaType[typeName] = "Lightning";
    g_typeCast["Lightning"] = "Lightning";
    return 1;
}

int lua_ww_RTTBlur_removeRTTNode(lua_State* tolua_S)
{
    int argc = 0;
    RTTBlur* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"RTTBlur",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (RTTBlur*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_ww_RTTBlur_removeRTTNode'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 1) 
    {
        cocos2d::Node* arg0;

        ok &= luaval_to_object<cocos2d::Node>(tolua_S, 2, "cc.Node",&arg0);
        if(!ok)
            return 0;
        cobj->removeRTTNode(arg0);
        return 0;
    }
    CCLOG("%s has wrong number of arguments: %d, was expecting %d \n", "removeRTTNode",argc, 1);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_ww_RTTBlur_removeRTTNode'.",&tolua_err);
#endif

    return 0;
}
int lua_ww_RTTBlur_enabledAutoRedraw(lua_State* tolua_S)
{
    int argc = 0;
    RTTBlur* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"RTTBlur",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (RTTBlur*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_ww_RTTBlur_enabledAutoRedraw'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 1) 
    {
        bool arg0;

        ok &= luaval_to_boolean(tolua_S, 2,&arg0);
        if(!ok)
            return 0;
        cobj->enabledAutoRedraw(arg0);
        return 0;
    }
    CCLOG("%s has wrong number of arguments: %d, was expecting %d \n", "enabledAutoRedraw",argc, 1);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_ww_RTTBlur_enabledAutoRedraw'.",&tolua_err);
#endif

    return 0;
}
int lua_ww_RTTBlur_getPixelFormat(lua_State* tolua_S)
{
    int argc = 0;
    RTTBlur* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"RTTBlur",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (RTTBlur*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_ww_RTTBlur_getPixelFormat'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 0) 
    {
        if(!ok)
            return 0;
        int ret = (int)cobj->getPixelFormat();
        tolua_pushnumber(tolua_S,(lua_Number)ret);
        return 1;
    }
    CCLOG("%s has wrong number of arguments: %d, was expecting %d \n", "getPixelFormat",argc, 0);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_ww_RTTBlur_getPixelFormat'.",&tolua_err);
#endif

    return 0;
}
int lua_ww_RTTBlur_addRTTNode(lua_State* tolua_S)
{
    int argc = 0;
    RTTBlur* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"RTTBlur",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (RTTBlur*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_ww_RTTBlur_addRTTNode'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 1) 
    {
        cocos2d::Node* arg0;

        ok &= luaval_to_object<cocos2d::Node>(tolua_S, 2, "cc.Node",&arg0);
        if(!ok)
            return 0;
        cobj->addRTTNode(arg0);
        return 0;
    }
    CCLOG("%s has wrong number of arguments: %d, was expecting %d \n", "addRTTNode",argc, 1);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_ww_RTTBlur_addRTTNode'.",&tolua_err);
#endif

    return 0;
}
int lua_ww_RTTBlur_clearRTTNodes(lua_State* tolua_S)
{
    int argc = 0;
    RTTBlur* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"RTTBlur",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (RTTBlur*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_ww_RTTBlur_clearRTTNodes'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 0) 
    {
        if(!ok)
            return 0;
        cobj->clearRTTNodes();
        return 0;
    }
    CCLOG("%s has wrong number of arguments: %d, was expecting %d \n", "clearRTTNodes",argc, 0);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_ww_RTTBlur_clearRTTNodes'.",&tolua_err);
#endif

    return 0;
}
int lua_ww_RTTBlur_setSampleNum(lua_State* tolua_S)
{
    int argc = 0;
    RTTBlur* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"RTTBlur",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (RTTBlur*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_ww_RTTBlur_setSampleNum'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 1) 
    {
        double arg0;

        ok &= luaval_to_number(tolua_S, 2,&arg0);
        if(!ok)
            return 0;
        cobj->setSampleNum(arg0);
        return 0;
    }
    CCLOG("%s has wrong number of arguments: %d, was expecting %d \n", "setSampleNum",argc, 1);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_ww_RTTBlur_setSampleNum'.",&tolua_err);
#endif

    return 0;
}
int lua_ww_RTTBlur_setBlurRadius(lua_State* tolua_S)
{
    int argc = 0;
    RTTBlur* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"RTTBlur",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (RTTBlur*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_ww_RTTBlur_setBlurRadius'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 1) 
    {
        double arg0;

        ok &= luaval_to_number(tolua_S, 2,&arg0);
        if(!ok)
            return 0;
        cobj->setBlurRadius(arg0);
        return 0;
    }
    CCLOG("%s has wrong number of arguments: %d, was expecting %d \n", "setBlurRadius",argc, 1);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_ww_RTTBlur_setBlurRadius'.",&tolua_err);
#endif

    return 0;
}
int lua_ww_RTTBlur_isAutoRedraw(lua_State* tolua_S)
{
    int argc = 0;
    RTTBlur* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"RTTBlur",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (RTTBlur*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_ww_RTTBlur_isAutoRedraw'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 0) 
    {
        if(!ok)
            return 0;
        bool ret = cobj->isAutoRedraw();
        tolua_pushboolean(tolua_S,(bool)ret);
        return 1;
    }
    CCLOG("%s has wrong number of arguments: %d, was expecting %d \n", "isAutoRedraw",argc, 0);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_ww_RTTBlur_isAutoRedraw'.",&tolua_err);
#endif

    return 0;
}
int lua_ww_RTTBlur_getSampleNum(lua_State* tolua_S)
{
    int argc = 0;
    RTTBlur* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"RTTBlur",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (RTTBlur*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_ww_RTTBlur_getSampleNum'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 0) 
    {
        if(!ok)
            return 0;
        double ret = cobj->getSampleNum();
        tolua_pushnumber(tolua_S,(lua_Number)ret);
        return 1;
    }
    CCLOG("%s has wrong number of arguments: %d, was expecting %d \n", "getSampleNum",argc, 0);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_ww_RTTBlur_getSampleNum'.",&tolua_err);
#endif

    return 0;
}
int lua_ww_RTTBlur_setPixelFormat(lua_State* tolua_S)
{
    int argc = 0;
    RTTBlur* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"RTTBlur",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (RTTBlur*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_ww_RTTBlur_setPixelFormat'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 1) 
    {
        cocos2d::Texture2D::PixelFormat arg0;

        ok &= luaval_to_int32(tolua_S, 2,(int *)&arg0);
        if(!ok)
            return 0;
        cobj->setPixelFormat(arg0);
        return 0;
    }
    CCLOG("%s has wrong number of arguments: %d, was expecting %d \n", "setPixelFormat",argc, 1);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_ww_RTTBlur_setPixelFormat'.",&tolua_err);
#endif

    return 0;
}
int lua_ww_RTTBlur_setRTTNodes(lua_State* tolua_S)
{
    int argc = 0;
    RTTBlur* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"RTTBlur",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (RTTBlur*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_ww_RTTBlur_setRTTNodes'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 1) 
    {
        cocos2d::Vector<cocos2d::Node *> arg0;

        ok &= luaval_to_ccvector(tolua_S, 2, &arg0);
        if(!ok)
            return 0;
        cobj->setRTTNodes(arg0);
        return 0;
    }
    CCLOG("%s has wrong number of arguments: %d, was expecting %d \n", "setRTTNodes",argc, 1);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_ww_RTTBlur_setRTTNodes'.",&tolua_err);
#endif

    return 0;
}
int lua_ww_RTTBlur_getBlurRadius(lua_State* tolua_S)
{
    int argc = 0;
    RTTBlur* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"RTTBlur",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (RTTBlur*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_ww_RTTBlur_getBlurRadius'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 0) 
    {
        if(!ok)
            return 0;
        double ret = cobj->getBlurRadius();
        tolua_pushnumber(tolua_S,(lua_Number)ret);
        return 1;
    }
    CCLOG("%s has wrong number of arguments: %d, was expecting %d \n", "getBlurRadius",argc, 0);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_ww_RTTBlur_getBlurRadius'.",&tolua_err);
#endif

    return 0;
}
int lua_ww_RTTBlur_create(lua_State* tolua_S)
{
    int argc = 0;
    bool ok  = true;
#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif

#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertable(tolua_S,1,"RTTBlur",0,&tolua_err)) goto tolua_lerror;
#endif

    argc = lua_gettop(tolua_S)-1;

    do 
    {
        if (argc == 2)
        {
            double arg0;
            ok &= luaval_to_number(tolua_S, 2,&arg0);
            if (!ok) { break; }
            double arg1;
            ok &= luaval_to_number(tolua_S, 3,&arg1);
            if (!ok) { break; }
            RTTBlur* ret = RTTBlur::create(arg0, arg1);
            object_to_luaval<RTTBlur>(tolua_S, "RTTBlur",(RTTBlur*)ret);
            return 1;
        }
    } while (0);
    ok  = true;
    do 
    {
        if (argc == 3)
        {
            double arg0;
            ok &= luaval_to_number(tolua_S, 2,&arg0);
            if (!ok) { break; }
            double arg1;
            ok &= luaval_to_number(tolua_S, 3,&arg1);
            if (!ok) { break; }
            cocos2d::Texture2D::PixelFormat arg2;
            ok &= luaval_to_int32(tolua_S, 4,(int *)&arg2);
            if (!ok) { break; }
            RTTBlur* ret = RTTBlur::create(arg0, arg1, arg2);
            object_to_luaval<RTTBlur>(tolua_S, "RTTBlur",(RTTBlur*)ret);
            return 1;
        }
    } while (0);
    ok  = true;
    do 
    {
        if (argc == 0)
        {
            RTTBlur* ret = RTTBlur::create();
            object_to_luaval<RTTBlur>(tolua_S, "RTTBlur",(RTTBlur*)ret);
            return 1;
        }
    } while (0);
    ok  = true;
    CCLOG("%s has wrong number of arguments: %d, was expecting %d", "create",argc, 0);
    return 0;
#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_ww_RTTBlur_create'.",&tolua_err);
#endif
    return 0;
}
static int lua_ww_RTTBlur_finalize(lua_State* tolua_S)
{
    printf("luabindings: finalizing LUA object (RTTBlur)");
    return 0;
}

int lua_register_ww_RTTBlur(lua_State* tolua_S)
{
    tolua_usertype(tolua_S,"RTTBlur");
    tolua_cclass(tolua_S,"RTTBlur","RTTBlur","cc.Node",nullptr);

    tolua_beginmodule(tolua_S,"RTTBlur");
        tolua_function(tolua_S,"removeRTTNode",lua_ww_RTTBlur_removeRTTNode);
        tolua_function(tolua_S,"enabledAutoRedraw",lua_ww_RTTBlur_enabledAutoRedraw);
        tolua_function(tolua_S,"getPixelFormat",lua_ww_RTTBlur_getPixelFormat);
        tolua_function(tolua_S,"addRTTNode",lua_ww_RTTBlur_addRTTNode);
        tolua_function(tolua_S,"clearRTTNodes",lua_ww_RTTBlur_clearRTTNodes);
        tolua_function(tolua_S,"setSampleNum",lua_ww_RTTBlur_setSampleNum);
        tolua_function(tolua_S,"setBlurRadius",lua_ww_RTTBlur_setBlurRadius);
        tolua_function(tolua_S,"isAutoRedraw",lua_ww_RTTBlur_isAutoRedraw);
        tolua_function(tolua_S,"getSampleNum",lua_ww_RTTBlur_getSampleNum);
        tolua_function(tolua_S,"setPixelFormat",lua_ww_RTTBlur_setPixelFormat);
        tolua_function(tolua_S,"setRTTNodes",lua_ww_RTTBlur_setRTTNodes);
        tolua_function(tolua_S,"getBlurRadius",lua_ww_RTTBlur_getBlurRadius);
        tolua_function(tolua_S,"create", lua_ww_RTTBlur_create);
    tolua_endmodule(tolua_S);
    std::string typeName = typeid(RTTBlur).name();
    g_luaType[typeName] = "RTTBlur";
    g_typeCast["RTTBlur"] = "RTTBlur";
    return 1;
}

int lua_ww_NetPacket_dump(lua_State* tolua_S)
{
    int argc = 0;
    NetPacket* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"NetPacket",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (NetPacket*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_ww_NetPacket_dump'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 0) 
    {
        if(!ok)
            return 0;
        std::string ret = cobj->dump();
        tolua_pushcppstring(tolua_S,ret);
        return 1;
    }
    CCLOG("%s has wrong number of arguments: %d, was expecting %d \n", "dump",argc, 0);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_ww_NetPacket_dump'.",&tolua_err);
#endif

    return 0;
}
int lua_ww_NetPacket_orfree(lua_State* tolua_S)
{
    int argc = 0;
    NetPacket* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"NetPacket",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (NetPacket*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_ww_NetPacket_orfree'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 1) 
    {
        int arg0;

        ok &= luaval_to_int32(tolua_S, 2,(int *)&arg0);
        if(!ok)
            return 0;
        int ret = cobj->orfree(arg0);
        tolua_pushnumber(tolua_S,(lua_Number)ret);
        return 1;
    }
    CCLOG("%s has wrong number of arguments: %d, was expecting %d \n", "orfree",argc, 1);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_ww_NetPacket_orfree'.",&tolua_err);
#endif

    return 0;
}
int lua_ww_NetPacket_clear(lua_State* tolua_S)
{
    int argc = 0;
    NetPacket* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"NetPacket",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (NetPacket*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_ww_NetPacket_clear'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 0) 
    {
        if(!ok)
            return 0;
        cobj->clear();
        return 0;
    }
    CCLOG("%s has wrong number of arguments: %d, was expecting %d \n", "clear",argc, 0);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_ww_NetPacket_clear'.",&tolua_err);
#endif

    return 0;
}
int lua_ww_NetPacket_wfree(lua_State* tolua_S)
{
    int argc = 0;
    NetPacket* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"NetPacket",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (NetPacket*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_ww_NetPacket_wfree'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 0) 
    {
        if(!ok)
            return 0;
        int ret = cobj->wfree();
        tolua_pushnumber(tolua_S,(lua_Number)ret);
        return 1;
    }
    CCLOG("%s has wrong number of arguments: %d, was expecting %d \n", "wfree",argc, 0);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_ww_NetPacket_wfree'.",&tolua_err);
#endif

    return 0;
}
int lua_ww_NetPacket_dws(lua_State* tolua_S)
{
    int argc = 0;
    NetPacket* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"NetPacket",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (NetPacket*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_ww_NetPacket_dws'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 1) 
    {
        const char* arg0;

        std::string arg0_tmp; ok &= luaval_to_std_string(tolua_S, 2, &arg0_tmp); arg0 = arg0_tmp.c_str();
        if(!ok)
            return 0;
        cobj->dws(arg0);
        return 0;
    }
    CCLOG("%s has wrong number of arguments: %d, was expecting %d \n", "dws",argc, 1);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_ww_NetPacket_dws'.",&tolua_err);
#endif

    return 0;
}
int lua_ww_NetPacket_prepare(lua_State* tolua_S)
{
    int argc = 0;
    NetPacket* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"NetPacket",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (NetPacket*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_ww_NetPacket_prepare'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 0) 
    {
        if(!ok)
            return 0;
        cobj->prepare();
        return 0;
    }
    CCLOG("%s has wrong number of arguments: %d, was expecting %d \n", "prepare",argc, 0);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_ww_NetPacket_prepare'.",&tolua_err);
#endif

    return 0;
}
int lua_ww_NetPacket_dr1(lua_State* tolua_S)
{
    int argc = 0;
    NetPacket* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"NetPacket",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (NetPacket*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_ww_NetPacket_dr1'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 0) 
    {
        if(!ok)
            return 0;
        int32_t ret = cobj->dr1();
        tolua_pushnumber(tolua_S,(lua_Number)ret);
        return 1;
    }
    CCLOG("%s has wrong number of arguments: %d, was expecting %d \n", "dr1",argc, 0);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_ww_NetPacket_dr1'.",&tolua_err);
#endif

    return 0;
}
int lua_ww_NetPacket_bufLen(lua_State* tolua_S)
{
    int argc = 0;
    NetPacket* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"NetPacket",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (NetPacket*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_ww_NetPacket_bufLen'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 0) 
    {
        if(!ok)
            return 0;
        int ret = cobj->bufLen();
        tolua_pushnumber(tolua_S,(lua_Number)ret);
        return 1;
    }
    CCLOG("%s has wrong number of arguments: %d, was expecting %d \n", "bufLen",argc, 0);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_ww_NetPacket_bufLen'.",&tolua_err);
#endif

    return 0;
}
int lua_ww_NetPacket_dr2(lua_State* tolua_S)
{
    int argc = 0;
    NetPacket* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"NetPacket",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (NetPacket*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_ww_NetPacket_dr2'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 0) 
    {
        if(!ok)
            return 0;
        int32_t ret = cobj->dr2();
        tolua_pushnumber(tolua_S,(lua_Number)ret);
        return 1;
    }
    CCLOG("%s has wrong number of arguments: %d, was expecting %d \n", "dr2",argc, 0);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_ww_NetPacket_dr2'.",&tolua_err);
#endif

    return 0;
}
int lua_ww_NetPacket_dr4(lua_State* tolua_S)
{
    int argc = 0;
    NetPacket* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"NetPacket",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (NetPacket*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_ww_NetPacket_dr4'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 0) 
    {
        if(!ok)
            return 0;
        int ret = cobj->dr4();
        tolua_pushnumber(tolua_S,(lua_Number)ret);
        return 1;
    }
    CCLOG("%s has wrong number of arguments: %d, was expecting %d \n", "dr4",argc, 0);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_ww_NetPacket_dr4'.",&tolua_err);
#endif

    return 0;
}
int lua_ww_NetPacket_owfree(lua_State* tolua_S)
{
    int argc = 0;
    NetPacket* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"NetPacket",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (NetPacket*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_ww_NetPacket_owfree'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 1) 
    {
        int arg0;

        ok &= luaval_to_int32(tolua_S, 2,(int *)&arg0);
        if(!ok)
            return 0;
        int ret = cobj->owfree(arg0);
        tolua_pushnumber(tolua_S,(lua_Number)ret);
        return 1;
    }
    CCLOG("%s has wrong number of arguments: %d, was expecting %d \n", "owfree",argc, 1);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_ww_NetPacket_owfree'.",&tolua_err);
#endif

    return 0;
}
int lua_ww_NetPacket_dwf(lua_State* tolua_S)
{
    int argc = 0;
    NetPacket* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"NetPacket",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (NetPacket*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_ww_NetPacket_dwf'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 1) 
    {
        double arg0;

        ok &= luaval_to_number(tolua_S, 2,&arg0);
        if(!ok)
            return 0;
        cobj->dwf(arg0);
        return 0;
    }
    CCLOG("%s has wrong number of arguments: %d, was expecting %d \n", "dwf",argc, 1);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_ww_NetPacket_dwf'.",&tolua_err);
#endif

    return 0;
}
int lua_ww_NetPacket_clearRo(lua_State* tolua_S)
{
    int argc = 0;
    NetPacket* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"NetPacket",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (NetPacket*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_ww_NetPacket_clearRo'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 0) 
    {
        if(!ok)
            return 0;
        cobj->clearRo();
        return 0;
    }
    CCLOG("%s has wrong number of arguments: %d, was expecting %d \n", "clearRo",argc, 0);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_ww_NetPacket_clearRo'.",&tolua_err);
#endif

    return 0;
}
int lua_ww_NetPacket_dwd(lua_State* tolua_S)
{
    int argc = 0;
    NetPacket* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"NetPacket",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (NetPacket*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_ww_NetPacket_dwd'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 1) 
    {
        double arg0;

        ok &= luaval_to_number(tolua_S, 2,&arg0);
        if(!ok)
            return 0;
        cobj->dwd(arg0);
        return 0;
    }
    CCLOG("%s has wrong number of arguments: %d, was expecting %d \n", "dwd",argc, 1);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_ww_NetPacket_dwd'.",&tolua_err);
#endif

    return 0;
}
int lua_ww_NetPacket_dru1(lua_State* tolua_S)
{
    int argc = 0;
    NetPacket* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"NetPacket",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (NetPacket*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_ww_NetPacket_dru1'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 0) 
    {
        if(!ok)
            return 0;
        uint16_t ret = cobj->dru1();
        tolua_pushnumber(tolua_S,(lua_Number)ret);
        return 1;
    }
    CCLOG("%s has wrong number of arguments: %d, was expecting %d \n", "dru1",argc, 0);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_ww_NetPacket_dru1'.",&tolua_err);
#endif

    return 0;
}
int lua_ww_NetPacket_woffset(lua_State* tolua_S)
{
    int argc = 0;
    NetPacket* cobj = nullptr;
    bool ok  = true;
#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif

#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"NetPacket",0,&tolua_err)) goto tolua_lerror;
#endif
    cobj = (NetPacket*)tolua_tousertype(tolua_S,1,0);
#if COCOS2D_DEBUG >= 1
    if (!cobj)
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_ww_NetPacket_woffset'", nullptr);
        return 0;
    }
#endif
    argc = lua_gettop(tolua_S)-1;
    do{
        if (argc == 1) {
            int arg0;
            ok &= luaval_to_int32(tolua_S, 2,(int *)&arg0);

            if (!ok) { break; }
            cobj->woffset(arg0);
            return 0;
        }
    }while(0);
    ok  = true;
    do{
        if (argc == 0) {
            int ret = cobj->woffset();
            tolua_pushnumber(tolua_S,(lua_Number)ret);
            return 1;
        }
    }while(0);
    ok  = true;
    CCLOG("%s has wrong number of arguments: %d, was expecting %d \n", "woffset",argc, 0);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_ww_NetPacket_woffset'.",&tolua_err);
#endif

    return 0;
}
int lua_ww_NetPacket_dru2(lua_State* tolua_S)
{
    int argc = 0;
    NetPacket* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"NetPacket",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (NetPacket*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_ww_NetPacket_dru2'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 0) 
    {
        if(!ok)
            return 0;
        unsigned short ret = cobj->dru2();
        tolua_pushnumber(tolua_S,(lua_Number)ret);
        return 1;
    }
    CCLOG("%s has wrong number of arguments: %d, was expecting %d \n", "dru2",argc, 0);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_ww_NetPacket_dru2'.",&tolua_err);
#endif

    return 0;
}
int lua_ww_NetPacket_dru4(lua_State* tolua_S)
{
    int argc = 0;
    NetPacket* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"NetPacket",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (NetPacket*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_ww_NetPacket_dru4'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 0) 
    {
        if(!ok)
            return 0;
        unsigned int ret = cobj->dru4();
        tolua_pushnumber(tolua_S,(lua_Number)ret);
        return 1;
    }
    CCLOG("%s has wrong number of arguments: %d, was expecting %d \n", "dru4",argc, 0);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_ww_NetPacket_dru4'.",&tolua_err);
#endif

    return 0;
}
int lua_ww_NetPacket_pkgLen(lua_State* tolua_S)
{
    int argc = 0;
    NetPacket* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"NetPacket",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (NetPacket*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_ww_NetPacket_pkgLen'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 0) 
    {
        if(!ok)
            return 0;
        int ret = cobj->pkgLen();
        tolua_pushnumber(tolua_S,(lua_Number)ret);
        return 1;
    }
    CCLOG("%s has wrong number of arguments: %d, was expecting %d \n", "pkgLen",argc, 0);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_ww_NetPacket_pkgLen'.",&tolua_err);
#endif

    return 0;
}
int lua_ww_NetPacket_dwu1(lua_State* tolua_S)
{
    int argc = 0;
    NetPacket* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"NetPacket",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (NetPacket*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_ww_NetPacket_dwu1'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 1) 
    {
        uint16_t arg0;

        ok &= luaval_to_uint16(tolua_S, 2,&arg0);
        if(!ok)
            return 0;
        cobj->dwu1(arg0);
        return 0;
    }
    CCLOG("%s has wrong number of arguments: %d, was expecting %d \n", "dwu1",argc, 1);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_ww_NetPacket_dwu1'.",&tolua_err);
#endif

    return 0;
}
int lua_ww_NetPacket_dwu2(lua_State* tolua_S)
{
    int argc = 0;
    NetPacket* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"NetPacket",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (NetPacket*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_ww_NetPacket_dwu2'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 1) 
    {
        unsigned short arg0;

        ok &= luaval_to_ushort(tolua_S, 2, &arg0);
        if(!ok)
            return 0;
        cobj->dwu2(arg0);
        return 0;
    }
    CCLOG("%s has wrong number of arguments: %d, was expecting %d \n", "dwu2",argc, 1);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_ww_NetPacket_dwu2'.",&tolua_err);
#endif

    return 0;
}
int lua_ww_NetPacket_dwu4(lua_State* tolua_S)
{
    int argc = 0;
    NetPacket* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"NetPacket",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (NetPacket*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_ww_NetPacket_dwu4'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 1) 
    {
        unsigned int arg0;

        ok &= luaval_to_uint32(tolua_S, 2,&arg0);
        if(!ok)
            return 0;
        cobj->dwu4(arg0);
        return 0;
    }
    CCLOG("%s has wrong number of arguments: %d, was expecting %d \n", "dwu4",argc, 1);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_ww_NetPacket_dwu4'.",&tolua_err);
#endif

    return 0;
}
int lua_ww_NetPacket_dw2(lua_State* tolua_S)
{
    int argc = 0;
    NetPacket* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"NetPacket",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (NetPacket*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_ww_NetPacket_dw2'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 1) 
    {
        int32_t arg0;

        ok &= luaval_to_int32(tolua_S, 2,&arg0);
        if(!ok)
            return 0;
        cobj->dw2(arg0);
        return 0;
    }
    CCLOG("%s has wrong number of arguments: %d, was expecting %d \n", "dw2",argc, 1);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_ww_NetPacket_dw2'.",&tolua_err);
#endif

    return 0;
}
int lua_ww_NetPacket_dw1(lua_State* tolua_S)
{
    int argc = 0;
    NetPacket* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"NetPacket",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (NetPacket*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_ww_NetPacket_dw1'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 1) 
    {
        int32_t arg0;

        ok &= luaval_to_int32(tolua_S, 2,&arg0);
        if(!ok)
            return 0;
        cobj->dw1(arg0);
        return 0;
    }
    CCLOG("%s has wrong number of arguments: %d, was expecting %d \n", "dw1",argc, 1);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_ww_NetPacket_dw1'.",&tolua_err);
#endif

    return 0;
}
int lua_ww_NetPacket_dw4(lua_State* tolua_S)
{
    int argc = 0;
    NetPacket* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"NetPacket",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (NetPacket*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_ww_NetPacket_dw4'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 1) 
    {
        int arg0;

        ok &= luaval_to_int32(tolua_S, 2,(int *)&arg0);
        if(!ok)
            return 0;
        cobj->dw4(arg0);
        return 0;
    }
    CCLOG("%s has wrong number of arguments: %d, was expecting %d \n", "dw4",argc, 1);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_ww_NetPacket_dw4'.",&tolua_err);
#endif

    return 0;
}
int lua_ww_NetPacket_clearWo(lua_State* tolua_S)
{
    int argc = 0;
    NetPacket* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"NetPacket",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (NetPacket*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_ww_NetPacket_clearWo'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 0) 
    {
        if(!ok)
            return 0;
        cobj->clearWo();
        return 0;
    }
    CCLOG("%s has wrong number of arguments: %d, was expecting %d \n", "clearWo",argc, 0);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_ww_NetPacket_clearWo'.",&tolua_err);
#endif

    return 0;
}
int lua_ww_NetPacket_rfree(lua_State* tolua_S)
{
    int argc = 0;
    NetPacket* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"NetPacket",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (NetPacket*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_ww_NetPacket_rfree'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 0) 
    {
        if(!ok)
            return 0;
        int ret = cobj->rfree();
        tolua_pushnumber(tolua_S,(lua_Number)ret);
        return 1;
    }
    CCLOG("%s has wrong number of arguments: %d, was expecting %d \n", "rfree",argc, 0);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_ww_NetPacket_rfree'.",&tolua_err);
#endif

    return 0;
}
int lua_ww_NetPacket_drs(lua_State* tolua_S)
{
    int argc = 0;
    NetPacket* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"NetPacket",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (NetPacket*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_ww_NetPacket_drs'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 0) 
    {
        if(!ok)
            return 0;
        std::string ret = cobj->drs();
        tolua_pushcppstring(tolua_S,ret);
        return 1;
    }
    CCLOG("%s has wrong number of arguments: %d, was expecting %d \n", "drs",argc, 0);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_ww_NetPacket_drs'.",&tolua_err);
#endif

    return 0;
}
int lua_ww_NetPacket_roffset(lua_State* tolua_S)
{
    int argc = 0;
    NetPacket* cobj = nullptr;
    bool ok  = true;
#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif

#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"NetPacket",0,&tolua_err)) goto tolua_lerror;
#endif
    cobj = (NetPacket*)tolua_tousertype(tolua_S,1,0);
#if COCOS2D_DEBUG >= 1
    if (!cobj)
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_ww_NetPacket_roffset'", nullptr);
        return 0;
    }
#endif
    argc = lua_gettop(tolua_S)-1;
    do{
        if (argc == 1) {
            int arg0;
            ok &= luaval_to_int32(tolua_S, 2,(int *)&arg0);

            if (!ok) { break; }
            cobj->roffset(arg0);
            return 0;
        }
    }while(0);
    ok  = true;
    do{
        if (argc == 0) {
            int ret = cobj->roffset();
            tolua_pushnumber(tolua_S,(lua_Number)ret);
            return 1;
        }
    }while(0);
    ok  = true;
    CCLOG("%s has wrong number of arguments: %d, was expecting %d \n", "roffset",argc, 0);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_ww_NetPacket_roffset'.",&tolua_err);
#endif

    return 0;
}
int lua_ww_NetPacket_opcode(lua_State* tolua_S)
{
    int argc = 0;
    NetPacket* cobj = nullptr;
    bool ok  = true;
#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif

#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"NetPacket",0,&tolua_err)) goto tolua_lerror;
#endif
    cobj = (NetPacket*)tolua_tousertype(tolua_S,1,0);
#if COCOS2D_DEBUG >= 1
    if (!cobj)
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_ww_NetPacket_opcode'", nullptr);
        return 0;
    }
#endif
    argc = lua_gettop(tolua_S)-1;
    do{
        if (argc == 1) {
            unsigned short arg0;
            ok &= luaval_to_ushort(tolua_S, 2, &arg0);

            if (!ok) { break; }
            cobj->opcode(arg0);
            return 0;
        }
    }while(0);
    ok  = true;
    do{
        if (argc == 0) {
            unsigned short ret = cobj->opcode();
            tolua_pushnumber(tolua_S,(lua_Number)ret);
            return 1;
        }
    }while(0);
    ok  = true;
    CCLOG("%s has wrong number of arguments: %d, was expecting %d \n", "opcode",argc, 0);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_ww_NetPacket_opcode'.",&tolua_err);
#endif

    return 0;
}
int lua_ww_NetPacket_drd(lua_State* tolua_S)
{
    int argc = 0;
    NetPacket* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"NetPacket",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (NetPacket*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_ww_NetPacket_drd'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 0) 
    {
        if(!ok)
            return 0;
        double ret = cobj->drd();
        tolua_pushnumber(tolua_S,(lua_Number)ret);
        return 1;
    }
    CCLOG("%s has wrong number of arguments: %d, was expecting %d \n", "drd",argc, 0);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_ww_NetPacket_drd'.",&tolua_err);
#endif

    return 0;
}
int lua_ww_NetPacket_drf(lua_State* tolua_S)
{
    int argc = 0;
    NetPacket* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"NetPacket",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (NetPacket*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_ww_NetPacket_drf'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 0) 
    {
        if(!ok)
            return 0;
        double ret = cobj->drf();
        tolua_pushnumber(tolua_S,(lua_Number)ret);
        return 1;
    }
    CCLOG("%s has wrong number of arguments: %d, was expecting %d \n", "drf",argc, 0);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_ww_NetPacket_drf'.",&tolua_err);
#endif

    return 0;
}
int lua_ww_NetPacket_create(lua_State* tolua_S)
{
    int argc = 0;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif

#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertable(tolua_S,1,"NetPacket",0,&tolua_err)) goto tolua_lerror;
#endif

    argc = lua_gettop(tolua_S) - 1;

    if (argc == 0)
    {
        if(!ok)
            return 0;
        NetPacket* ret = NetPacket::create();
        object_to_luaval<NetPacket>(tolua_S, "NetPacket",(NetPacket*)ret);
        return 1;
    }
    if (argc == 1)
    {
        int arg0;
        ok &= luaval_to_int32(tolua_S, 2,(int *)&arg0);
        if(!ok)
            return 0;
        NetPacket* ret = NetPacket::create(arg0);
        object_to_luaval<NetPacket>(tolua_S, "NetPacket",(NetPacket*)ret);
        return 1;
    }
    CCLOG("%s has wrong number of arguments: %d, was expecting %d\n ", "create",argc, 0);
    return 0;
#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_ww_NetPacket_create'.",&tolua_err);
#endif
    return 0;
}
static int lua_ww_NetPacket_finalize(lua_State* tolua_S)
{
    printf("luabindings: finalizing LUA object (NetPacket)");
    return 0;
}

int lua_register_ww_NetPacket(lua_State* tolua_S)
{
    tolua_usertype(tolua_S,"NetPacket");
    tolua_cclass(tolua_S,"NetPacket","NetPacket","cc.Ref",nullptr);

    tolua_beginmodule(tolua_S,"NetPacket");
        tolua_function(tolua_S,"dump",lua_ww_NetPacket_dump);
        tolua_function(tolua_S,"orfree",lua_ww_NetPacket_orfree);
        tolua_function(tolua_S,"clear",lua_ww_NetPacket_clear);
        tolua_function(tolua_S,"wfree",lua_ww_NetPacket_wfree);
        tolua_function(tolua_S,"dws",lua_ww_NetPacket_dws);
        tolua_function(tolua_S,"prepare",lua_ww_NetPacket_prepare);
        tolua_function(tolua_S,"dr1",lua_ww_NetPacket_dr1);
        tolua_function(tolua_S,"bufLen",lua_ww_NetPacket_bufLen);
        tolua_function(tolua_S,"dr2",lua_ww_NetPacket_dr2);
        tolua_function(tolua_S,"dr4",lua_ww_NetPacket_dr4);
        tolua_function(tolua_S,"owfree",lua_ww_NetPacket_owfree);
        tolua_function(tolua_S,"dwf",lua_ww_NetPacket_dwf);
        tolua_function(tolua_S,"clearRo",lua_ww_NetPacket_clearRo);
        tolua_function(tolua_S,"dwd",lua_ww_NetPacket_dwd);
        tolua_function(tolua_S,"dru1",lua_ww_NetPacket_dru1);
        tolua_function(tolua_S,"woffset",lua_ww_NetPacket_woffset);
        tolua_function(tolua_S,"dru2",lua_ww_NetPacket_dru2);
        tolua_function(tolua_S,"dru4",lua_ww_NetPacket_dru4);
        tolua_function(tolua_S,"pkgLen",lua_ww_NetPacket_pkgLen);
        tolua_function(tolua_S,"dwu1",lua_ww_NetPacket_dwu1);
        tolua_function(tolua_S,"dwu2",lua_ww_NetPacket_dwu2);
        tolua_function(tolua_S,"dwu4",lua_ww_NetPacket_dwu4);
        tolua_function(tolua_S,"dw2",lua_ww_NetPacket_dw2);
        tolua_function(tolua_S,"dw1",lua_ww_NetPacket_dw1);
        tolua_function(tolua_S,"dw4",lua_ww_NetPacket_dw4);
        tolua_function(tolua_S,"clearWo",lua_ww_NetPacket_clearWo);
        tolua_function(tolua_S,"rfree",lua_ww_NetPacket_rfree);
        tolua_function(tolua_S,"drs",lua_ww_NetPacket_drs);
        tolua_function(tolua_S,"roffset",lua_ww_NetPacket_roffset);
        tolua_function(tolua_S,"opcode",lua_ww_NetPacket_opcode);
        tolua_function(tolua_S,"drd",lua_ww_NetPacket_drd);
        tolua_function(tolua_S,"drf",lua_ww_NetPacket_drf);
        tolua_function(tolua_S,"create", lua_ww_NetPacket_create);
    tolua_endmodule(tolua_S);
    std::string typeName = typeid(NetPacket).name();
    g_luaType[typeName] = "NetPacket";
    g_typeCast["NetPacket"] = "NetPacket";
    return 1;
}

int lua_ww_NetEngine_closeAll(lua_State* tolua_S)
{
    int argc = 0;
    NetEngine* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"NetEngine",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (NetEngine*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_ww_NetEngine_closeAll'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 0) 
    {
        if(!ok)
            return 0;
        cobj->closeAll();
        return 0;
    }
    CCLOG("%s has wrong number of arguments: %d, was expecting %d \n", "closeAll",argc, 0);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_ww_NetEngine_closeAll'.",&tolua_err);
#endif

    return 0;
}
int lua_ww_NetEngine_shutdown(lua_State* tolua_S)
{
    int argc = 0;
    NetEngine* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"NetEngine",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (NetEngine*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_ww_NetEngine_shutdown'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 0) 
    {
        if(!ok)
            return 0;
        cobj->shutdown();
        return 0;
    }
    CCLOG("%s has wrong number of arguments: %d, was expecting %d \n", "shutdown",argc, 0);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_ww_NetEngine_shutdown'.",&tolua_err);
#endif

    return 0;
}
int lua_ww_NetEngine_getNetType(lua_State* tolua_S)
{
    int argc = 0;
    NetEngine* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"NetEngine",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (NetEngine*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_ww_NetEngine_getNetType'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 0) 
    {
        if(!ok)
            return 0;
        int ret = (int)cobj->getNetType();
        tolua_pushnumber(tolua_S,(lua_Number)ret);
        return 1;
    }
    CCLOG("%s has wrong number of arguments: %d, was expecting %d \n", "getNetType",argc, 0);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_ww_NetEngine_getNetType'.",&tolua_err);
#endif

    return 0;
}
int lua_ww_NetEngine_getInstance(lua_State* tolua_S)
{
    int argc = 0;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif

#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertable(tolua_S,1,"NetEngine",0,&tolua_err)) goto tolua_lerror;
#endif

    argc = lua_gettop(tolua_S) - 1;

    if (argc == 0)
    {
        if(!ok)
            return 0;
        NetEngine* ret = NetEngine::getInstance();
        object_to_luaval<NetEngine>(tolua_S, "NetEngine",(NetEngine*)ret);
        return 1;
    }
    CCLOG("%s has wrong number of arguments: %d, was expecting %d\n ", "getInstance",argc, 0);
    return 0;
#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_ww_NetEngine_getInstance'.",&tolua_err);
#endif
    return 0;
}
static int lua_ww_NetEngine_finalize(lua_State* tolua_S)
{
    printf("luabindings: finalizing LUA object (NetEngine)");
    return 0;
}

int lua_register_ww_NetEngine(lua_State* tolua_S)
{
    tolua_usertype(tolua_S,"NetEngine");
    tolua_cclass(tolua_S,"NetEngine","NetEngine","",nullptr);

    tolua_beginmodule(tolua_S,"NetEngine");
        tolua_function(tolua_S,"closeAll",lua_ww_NetEngine_closeAll);
        tolua_function(tolua_S,"shutdown",lua_ww_NetEngine_shutdown);
        tolua_function(tolua_S,"getNetType",lua_ww_NetEngine_getNetType);
        tolua_function(tolua_S,"getInstance", lua_ww_NetEngine_getInstance);
    tolua_endmodule(tolua_S);
    std::string typeName = typeid(NetEngine).name();
    g_luaType[typeName] = "NetEngine";
    g_typeCast["NetEngine"] = "NetEngine";
    return 1;
}

int lua_ww_NetClient_getState(lua_State* tolua_S)
{
    int argc = 0;
    NetClient* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"NetClient",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (NetClient*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_ww_NetClient_getState'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 0) 
    {
        if(!ok)
            return 0;
        int ret = (int)cobj->getState();
        tolua_pushnumber(tolua_S,(lua_Number)ret);
        return 1;
    }
    CCLOG("%s has wrong number of arguments: %d, was expecting %d \n", "getState",argc, 0);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_ww_NetClient_getState'.",&tolua_err);
#endif

    return 0;
}
int lua_ww_NetClient_isAlive(lua_State* tolua_S)
{
    int argc = 0;
    NetClient* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"NetClient",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (NetClient*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_ww_NetClient_isAlive'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 0) 
    {
        if(!ok)
            return 0;
        bool ret = cobj->isAlive();
        tolua_pushboolean(tolua_S,(bool)ret);
        return 1;
    }
    CCLOG("%s has wrong number of arguments: %d, was expecting %d \n", "isAlive",argc, 0);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_ww_NetClient_isAlive'.",&tolua_err);
#endif

    return 0;
}
int lua_ww_NetClient_clear(lua_State* tolua_S)
{
    int argc = 0;
    NetClient* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"NetClient",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (NetClient*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_ww_NetClient_clear'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 0) 
    {
        if(!ok)
            return 0;
        cobj->clear();
        return 0;
    }
    CCLOG("%s has wrong number of arguments: %d, was expecting %d \n", "clear",argc, 0);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_ww_NetClient_clear'.",&tolua_err);
#endif

    return 0;
}
int lua_ww_NetClient_send(lua_State* tolua_S)
{
    int argc = 0;
    NetClient* cobj = nullptr;
    bool ok  = true;
#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif

#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"NetClient",0,&tolua_err)) goto tolua_lerror;
#endif
    cobj = (NetClient*)tolua_tousertype(tolua_S,1,0);
#if COCOS2D_DEBUG >= 1
    if (!cobj)
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_ww_NetClient_send'", nullptr);
        return 0;
    }
#endif
    argc = lua_gettop(tolua_S)-1;
    do{
        if (argc == 1) {
            NetPacket* arg0;
            ok &= luaval_to_object<NetPacket>(tolua_S, 2, "NetPacket",&arg0);

            if (!ok) { break; }
            bool ret = cobj->send(arg0);
            tolua_pushboolean(tolua_S,(bool)ret);
            return 1;
        }
    }while(0);
    ok  = true;
    do{
        if (argc == 2) {
            const char* arg0;
            std::string arg0_tmp; ok &= luaval_to_std_string(tolua_S, 2, &arg0_tmp); arg0 = arg0_tmp.c_str();

            if (!ok) { break; }
            int arg1;
            ok &= luaval_to_int32(tolua_S, 3,(int *)&arg1);

            if (!ok) { break; }
            bool ret = cobj->send(arg0, arg1);
            tolua_pushboolean(tolua_S,(bool)ret);
            return 1;
        }
    }while(0);
    ok  = true;
    CCLOG("%s has wrong number of arguments: %d, was expecting %d \n", "send",argc, 2);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_ww_NetClient_send'.",&tolua_err);
#endif

    return 0;
}
int lua_ww_NetClient_isClosed(lua_State* tolua_S)
{
    int argc = 0;
    NetClient* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"NetClient",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (NetClient*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_ww_NetClient_isClosed'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 0) 
    {
        if(!ok)
            return 0;
        bool ret = cobj->isClosed();
        tolua_pushboolean(tolua_S,(bool)ret);
        return 1;
    }
    CCLOG("%s has wrong number of arguments: %d, was expecting %d \n", "isClosed",argc, 0);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_ww_NetClient_isClosed'.",&tolua_err);
#endif

    return 0;
}
int lua_ww_NetClient_setParms(lua_State* tolua_S)
{
    int argc = 0;
    NetClient* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"NetClient",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (NetClient*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_ww_NetClient_setParms'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 2) 
    {
        std::string arg0;
        unsigned short arg1;

        ok &= luaval_to_std_string(tolua_S, 2,&arg0);

        ok &= luaval_to_ushort(tolua_S, 3, &arg1);
        if(!ok)
            return 0;
        cobj->setParms(arg0, arg1);
        return 0;
    }
    if (argc == 3) 
    {
        std::string arg0;
        unsigned short arg1;
        int arg2;

        ok &= luaval_to_std_string(tolua_S, 2,&arg0);

        ok &= luaval_to_ushort(tolua_S, 3, &arg1);

        ok &= luaval_to_int32(tolua_S, 4,(int *)&arg2);
        if(!ok)
            return 0;
        cobj->setParms(arg0, arg1, arg2);
        return 0;
    }
    CCLOG("%s has wrong number of arguments: %d, was expecting %d \n", "setParms",argc, 2);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_ww_NetClient_setParms'.",&tolua_err);
#endif

    return 0;
}
int lua_ww_NetClient_isDead(lua_State* tolua_S)
{
    int argc = 0;
    NetClient* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"NetClient",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (NetClient*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_ww_NetClient_isDead'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 0) 
    {
        if(!ok)
            return 0;
        bool ret = cobj->isDead();
        tolua_pushboolean(tolua_S,(bool)ret);
        return 1;
    }
    CCLOG("%s has wrong number of arguments: %d, was expecting %d \n", "isDead",argc, 0);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_ww_NetClient_isDead'.",&tolua_err);
#endif

    return 0;
}
int lua_ww_NetClient_connect(lua_State* tolua_S)
{
    int argc = 0;
    NetClient* cobj = nullptr;
    bool ok  = true;
#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif

#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"NetClient",0,&tolua_err)) goto tolua_lerror;
#endif
    cobj = (NetClient*)tolua_tousertype(tolua_S,1,0);
#if COCOS2D_DEBUG >= 1
    if (!cobj)
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_ww_NetClient_connect'", nullptr);
        return 0;
    }
#endif
    argc = lua_gettop(tolua_S)-1;
    do{
        if (argc == 2) {
            std::string arg0;
            ok &= luaval_to_std_string(tolua_S, 2,&arg0);

            if (!ok) { break; }
            unsigned short arg1;
            ok &= luaval_to_ushort(tolua_S, 3, &arg1);

            if (!ok) { break; }
            bool ret = cobj->connect(arg0, arg1);
            tolua_pushboolean(tolua_S,(bool)ret);
            return 1;
        }
    }while(0);
    ok  = true;
    do{
        if (argc == 3) {
            std::string arg0;
            ok &= luaval_to_std_string(tolua_S, 2,&arg0);

            if (!ok) { break; }
            unsigned short arg1;
            ok &= luaval_to_ushort(tolua_S, 3, &arg1);

            if (!ok) { break; }
            int arg2;
            ok &= luaval_to_int32(tolua_S, 4,(int *)&arg2);

            if (!ok) { break; }
            bool ret = cobj->connect(arg0, arg1, arg2);
            tolua_pushboolean(tolua_S,(bool)ret);
            return 1;
        }
    }while(0);
    ok  = true;
    do{
        if (argc == 0) {
            bool ret = cobj->connect();
            tolua_pushboolean(tolua_S,(bool)ret);
            return 1;
        }
    }while(0);
    ok  = true;
    CCLOG("%s has wrong number of arguments: %d, was expecting %d \n", "connect",argc, 0);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_ww_NetClient_connect'.",&tolua_err);
#endif

    return 0;
}
int lua_ww_NetClient_close(lua_State* tolua_S)
{
    int argc = 0;
    NetClient* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"NetClient",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (NetClient*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_ww_NetClient_close'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 0) 
    {
        if(!ok)
            return 0;
        cobj->close();
        return 0;
    }
    CCLOG("%s has wrong number of arguments: %d, was expecting %d \n", "close",argc, 0);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_ww_NetClient_close'.",&tolua_err);
#endif

    return 0;
}
int lua_ww_NetClient_unregisterNetEventHandler(lua_State* tolua_S)
{
    int argc = 0;
    NetClient* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"NetClient",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (NetClient*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_ww_NetClient_unregisterNetEventHandler'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 0) 
    {
        if(!ok)
            return 0;
        cobj->unregisterNetEventHandler();
        return 0;
    }
    CCLOG("%s has wrong number of arguments: %d, was expecting %d \n", "unregisterNetEventHandler",argc, 0);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_ww_NetClient_unregisterNetEventHandler'.",&tolua_err);
#endif

    return 0;
}
int lua_ww_NetClient_create(lua_State* tolua_S)
{
    int argc = 0;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif

#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertable(tolua_S,1,"NetClient",0,&tolua_err)) goto tolua_lerror;
#endif

    argc = lua_gettop(tolua_S) - 1;

    if (argc == 0)
    {
        if(!ok)
            return 0;
        NetClient* ret = NetClient::create();
        object_to_luaval<NetClient>(tolua_S, "NetClient",(NetClient*)ret);
        return 1;
    }
    CCLOG("%s has wrong number of arguments: %d, was expecting %d\n ", "create",argc, 0);
    return 0;
#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_ww_NetClient_create'.",&tolua_err);
#endif
    return 0;
}
static int lua_ww_NetClient_finalize(lua_State* tolua_S)
{
    printf("luabindings: finalizing LUA object (NetClient)");
    return 0;
}

int lua_register_ww_NetClient(lua_State* tolua_S)
{
    tolua_usertype(tolua_S,"NetClient");
    tolua_cclass(tolua_S,"NetClient","NetClient","cc.Ref",nullptr);

    tolua_beginmodule(tolua_S,"NetClient");
        tolua_function(tolua_S,"getState",lua_ww_NetClient_getState);
        tolua_function(tolua_S,"isAlive",lua_ww_NetClient_isAlive);
        tolua_function(tolua_S,"clear",lua_ww_NetClient_clear);
        tolua_function(tolua_S,"send",lua_ww_NetClient_send);
        tolua_function(tolua_S,"isClosed",lua_ww_NetClient_isClosed);
        tolua_function(tolua_S,"setParms",lua_ww_NetClient_setParms);
        tolua_function(tolua_S,"isDead",lua_ww_NetClient_isDead);
        tolua_function(tolua_S,"connect",lua_ww_NetClient_connect);
        tolua_function(tolua_S,"close",lua_ww_NetClient_close);
        tolua_function(tolua_S,"unregisterNetEventHandler",lua_ww_NetClient_unregisterNetEventHandler);
        tolua_function(tolua_S,"create", lua_ww_NetClient_create);
    tolua_endmodule(tolua_S);
    std::string typeName = typeid(NetClient).name();
    g_luaType[typeName] = "NetClient";
    g_typeCast["NetClient"] = "NetClient";
    return 1;
}

int lua_ww_NetHttp_cancel(lua_State* tolua_S)
{
    int argc = 0;
    NetHttp* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"NetHttp",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (NetHttp*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_ww_NetHttp_cancel'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 1) 
    {
        std::string arg0;

        ok &= luaval_to_std_string(tolua_S, 2,&arg0);
        if(!ok)
            return 0;
        cobj->cancel(arg0);
        return 0;
    }
    CCLOG("%s has wrong number of arguments: %d, was expecting %d \n", "cancel",argc, 1);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_ww_NetHttp_cancel'.",&tolua_err);
#endif

    return 0;
}
int lua_ww_NetHttp_getInstance(lua_State* tolua_S)
{
    int argc = 0;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif

#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertable(tolua_S,1,"NetHttp",0,&tolua_err)) goto tolua_lerror;
#endif

    argc = lua_gettop(tolua_S) - 1;

    if (argc == 0)
    {
        if(!ok)
            return 0;
        NetHttp* ret = NetHttp::getInstance();
        object_to_luaval<NetHttp>(tolua_S, "NetHttp",(NetHttp*)ret);
        return 1;
    }
    CCLOG("%s has wrong number of arguments: %d, was expecting %d\n ", "getInstance",argc, 0);
    return 0;
#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_ww_NetHttp_getInstance'.",&tolua_err);
#endif
    return 0;
}
static int lua_ww_NetHttp_finalize(lua_State* tolua_S)
{
    printf("luabindings: finalizing LUA object (NetHttp)");
    return 0;
}

int lua_register_ww_NetHttp(lua_State* tolua_S)
{
    tolua_usertype(tolua_S,"NetHttp");
    tolua_cclass(tolua_S,"NetHttp","NetHttp","cc.Ref",nullptr);

    tolua_beginmodule(tolua_S,"NetHttp");
        tolua_function(tolua_S,"cancel",lua_ww_NetHttp_cancel);
        tolua_function(tolua_S,"getInstance", lua_ww_NetHttp_getInstance);
    tolua_endmodule(tolua_S);
    std::string typeName = typeid(NetHttp).name();
    g_luaType[typeName] = "NetHttp";
    g_typeCast["NetHttp"] = "NetHttp";
    return 1;
}

int lua_ww_FileIndexRow_getState(lua_State* tolua_S)
{
    int argc = 0;
    FileIndexRow* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"FileIndexRow",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (FileIndexRow*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_ww_FileIndexRow_getState'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 0) 
    {
        if(!ok)
            return 0;
        int ret = (int)cobj->getState();
        tolua_pushnumber(tolua_S,(lua_Number)ret);
        return 1;
    }
    CCLOG("%s has wrong number of arguments: %d, was expecting %d \n", "getState",argc, 0);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_ww_FileIndexRow_getState'.",&tolua_err);
#endif

    return 0;
}
int lua_ww_FileIndexRow_getName(lua_State* tolua_S)
{
    int argc = 0;
    FileIndexRow* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"FileIndexRow",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (FileIndexRow*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_ww_FileIndexRow_getName'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 0) 
    {
        if(!ok)
            return 0;
        std::string ret = cobj->getName();
        tolua_pushcppstring(tolua_S,ret);
        return 1;
    }
    CCLOG("%s has wrong number of arguments: %d, was expecting %d \n", "getName",argc, 0);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_ww_FileIndexRow_getName'.",&tolua_err);
#endif

    return 0;
}
int lua_ww_FileIndexRow_getType(lua_State* tolua_S)
{
    int argc = 0;
    FileIndexRow* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"FileIndexRow",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (FileIndexRow*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_ww_FileIndexRow_getType'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 0) 
    {
        if(!ok)
            return 0;
        int ret = (int)cobj->getType();
        tolua_pushnumber(tolua_S,(lua_Number)ret);
        return 1;
    }
    CCLOG("%s has wrong number of arguments: %d, was expecting %d \n", "getType",argc, 0);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_ww_FileIndexRow_getType'.",&tolua_err);
#endif

    return 0;
}
int lua_ww_FileIndexRow_getSize(lua_State* tolua_S)
{
    int argc = 0;
    FileIndexRow* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"FileIndexRow",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (FileIndexRow*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_ww_FileIndexRow_getSize'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 0) 
    {
        if(!ok)
            return 0;
        int ret = cobj->getSize();
        tolua_pushnumber(tolua_S,(lua_Number)ret);
        return 1;
    }
    CCLOG("%s has wrong number of arguments: %d, was expecting %d \n", "getSize",argc, 0);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_ww_FileIndexRow_getSize'.",&tolua_err);
#endif

    return 0;
}
int lua_ww_FileIndexRow_copyTo(lua_State* tolua_S)
{
    int argc = 0;
    FileIndexRow* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"FileIndexRow",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (FileIndexRow*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_ww_FileIndexRow_copyTo'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 1) 
    {
        FileIndexRow* arg0;

        ok &= luaval_to_object<FileIndexRow>(tolua_S, 2, "FileIndexRow",&arg0);
        if(!ok)
            return 0;
        cobj->copyTo(arg0);
        return 0;
    }
    CCLOG("%s has wrong number of arguments: %d, was expecting %d \n", "copyTo",argc, 1);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_ww_FileIndexRow_copyTo'.",&tolua_err);
#endif

    return 0;
}
int lua_ww_FileIndexRow_getFullName(lua_State* tolua_S)
{
    int argc = 0;
    FileIndexRow* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"FileIndexRow",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (FileIndexRow*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_ww_FileIndexRow_getFullName'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 0) 
    {
        if(!ok)
            return 0;
        std::string ret = cobj->getFullName();
        tolua_pushcppstring(tolua_S,ret);
        return 1;
    }
    CCLOG("%s has wrong number of arguments: %d, was expecting %d \n", "getFullName",argc, 0);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_ww_FileIndexRow_getFullName'.",&tolua_err);
#endif

    return 0;
}
int lua_ww_FileIndexRow_getInBasePkg(lua_State* tolua_S)
{
    int argc = 0;
    FileIndexRow* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"FileIndexRow",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (FileIndexRow*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_ww_FileIndexRow_getInBasePkg'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 0) 
    {
        if(!ok)
            return 0;
        bool ret = cobj->getInBasePkg();
        tolua_pushboolean(tolua_S,(bool)ret);
        return 1;
    }
    CCLOG("%s has wrong number of arguments: %d, was expecting %d \n", "getInBasePkg",argc, 0);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_ww_FileIndexRow_getInBasePkg'.",&tolua_err);
#endif

    return 0;
}
int lua_ww_FileIndexRow_getVersion(lua_State* tolua_S)
{
    int argc = 0;
    FileIndexRow* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"FileIndexRow",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (FileIndexRow*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_ww_FileIndexRow_getVersion'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 0) 
    {
        if(!ok)
            return 0;
        int ret = cobj->getVersion();
        tolua_pushnumber(tolua_S,(lua_Number)ret);
        return 1;
    }
    CCLOG("%s has wrong number of arguments: %d, was expecting %d \n", "getVersion",argc, 0);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_ww_FileIndexRow_getVersion'.",&tolua_err);
#endif

    return 0;
}
static int lua_ww_FileIndexRow_finalize(lua_State* tolua_S)
{
    printf("luabindings: finalizing LUA object (FileIndexRow)");
    return 0;
}

int lua_register_ww_FileIndexRow(lua_State* tolua_S)
{
    tolua_usertype(tolua_S,"FileIndexRow");
    tolua_cclass(tolua_S,"FileIndexRow","FileIndexRow","",nullptr);

    tolua_beginmodule(tolua_S,"FileIndexRow");
        tolua_function(tolua_S,"getState",lua_ww_FileIndexRow_getState);
        tolua_function(tolua_S,"getName",lua_ww_FileIndexRow_getName);
        tolua_function(tolua_S,"getType",lua_ww_FileIndexRow_getType);
        tolua_function(tolua_S,"getSize",lua_ww_FileIndexRow_getSize);
        tolua_function(tolua_S,"copyTo",lua_ww_FileIndexRow_copyTo);
        tolua_function(tolua_S,"getFullName",lua_ww_FileIndexRow_getFullName);
        tolua_function(tolua_S,"getInBasePkg",lua_ww_FileIndexRow_getInBasePkg);
        tolua_function(tolua_S,"getVersion",lua_ww_FileIndexRow_getVersion);
    tolua_endmodule(tolua_S);
    std::string typeName = typeid(FileIndexRow).name();
    g_luaType[typeName] = "FileIndexRow";
    g_typeCast["FileIndexRow"] = "FileIndexRow";
    return 1;
}

int lua_ww_FileIndex_getRow(lua_State* tolua_S)
{
    int argc = 0;
    FileIndex* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"FileIndex",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (FileIndex*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_ww_FileIndex_getRow'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 1) 
    {
        std::string arg0;

        ok &= luaval_to_std_string(tolua_S, 2,&arg0);
        if(!ok)
            return 0;
        FileIndexRow* ret = cobj->getRow(arg0);
        object_to_luaval<FileIndexRow>(tolua_S, "FileIndexRow",(FileIndexRow*)ret);
        return 1;
    }
    CCLOG("%s has wrong number of arguments: %d, was expecting %d \n", "getRow",argc, 1);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_ww_FileIndex_getRow'.",&tolua_err);
#endif

    return 0;
}
int lua_ww_FileIndex_getVersion(lua_State* tolua_S)
{
    int argc = 0;
    FileIndex* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"FileIndex",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (FileIndex*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_ww_FileIndex_getVersion'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 0) 
    {
        if(!ok)
            return 0;
        int ret = cobj->getVersion();
        tolua_pushnumber(tolua_S,(lua_Number)ret);
        return 1;
    }
    CCLOG("%s has wrong number of arguments: %d, was expecting %d \n", "getVersion",argc, 0);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_ww_FileIndex_getVersion'.",&tolua_err);
#endif

    return 0;
}
int lua_ww_FileIndex_getNames(lua_State* tolua_S)
{
    int argc = 0;
    FileIndex* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"FileIndex",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (FileIndex*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_ww_FileIndex_getNames'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 0) 
    {
        if(!ok)
            return 0;
        std::vector<std::string> ret = cobj->getNames();
        ccvector_std_string_to_luaval(tolua_S, ret);
        return 1;
    }
    CCLOG("%s has wrong number of arguments: %d, was expecting %d \n", "getNames",argc, 0);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_ww_FileIndex_getNames'.",&tolua_err);
#endif

    return 0;
}
static int lua_ww_FileIndex_finalize(lua_State* tolua_S)
{
    printf("luabindings: finalizing LUA object (FileIndex)");
    return 0;
}

int lua_register_ww_FileIndex(lua_State* tolua_S)
{
    tolua_usertype(tolua_S,"FileIndex");
    tolua_cclass(tolua_S,"FileIndex","FileIndex","sqlite.Connection",nullptr);

    tolua_beginmodule(tolua_S,"FileIndex");
        tolua_function(tolua_S,"getRow",lua_ww_FileIndex_getRow);
        tolua_function(tolua_S,"getVersion",lua_ww_FileIndex_getVersion);
        tolua_function(tolua_S,"getNames",lua_ww_FileIndex_getNames);
    tolua_endmodule(tolua_S);
    std::string typeName = typeid(FileIndex).name();
    g_luaType[typeName] = "FileIndex";
    g_typeCast["FileIndex"] = "FileIndex";
    return 1;
}

int lua_ww_FileManager_load(lua_State* tolua_S)
{
    int argc = 0;
    FileManager* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"FileManager",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (FileManager*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_ww_FileManager_load'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 1) 
    {
        std::vector<std::string> arg0;

        ok &= luaval_to_std_vector_string(tolua_S, 2, &arg0);
        if(!ok)
            return 0;
        bool ret = cobj->load(arg0);
        tolua_pushboolean(tolua_S,(bool)ret);
        return 1;
    }
    CCLOG("%s has wrong number of arguments: %d, was expecting %d \n", "load",argc, 1);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_ww_FileManager_load'.",&tolua_err);
#endif

    return 0;
}
int lua_ww_FileManager_loadIndex(lua_State* tolua_S)
{
    int argc = 0;
    FileManager* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"FileManager",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (FileManager*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_ww_FileManager_loadIndex'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 2) 
    {
        std::string arg0;
        std::string arg1;

        ok &= luaval_to_std_string(tolua_S, 2,&arg0);

        ok &= luaval_to_std_string(tolua_S, 3,&arg1);
        if(!ok)
            return 0;
        bool ret = cobj->loadIndex(arg0, arg1);
        tolua_pushboolean(tolua_S,(bool)ret);
        return 1;
    }
    CCLOG("%s has wrong number of arguments: %d, was expecting %d \n", "loadIndex",argc, 2);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_ww_FileManager_loadIndex'.",&tolua_err);
#endif

    return 0;
}
int lua_ww_FileManager_getIndex(lua_State* tolua_S)
{
    int argc = 0;
    FileManager* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"FileManager",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (FileManager*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_ww_FileManager_getIndex'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 0) 
    {
        if(!ok)
            return 0;
        FileIndex* ret = cobj->getIndex();
        object_to_luaval<FileIndex>(tolua_S, "FileIndex",(FileIndex*)ret);
        return 1;
    }
    CCLOG("%s has wrong number of arguments: %d, was expecting %d \n", "getIndex",argc, 0);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_ww_FileManager_getIndex'.",&tolua_err);
#endif

    return 0;
}
int lua_ww_FileManager_unload(lua_State* tolua_S)
{
    int argc = 0;
    FileManager* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"FileManager",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (FileManager*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_ww_FileManager_unload'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 1) 
    {
        std::vector<std::string> arg0;

        ok &= luaval_to_std_vector_string(tolua_S, 2, &arg0);
        if(!ok)
            return 0;
        bool ret = cobj->unload(arg0);
        tolua_pushboolean(tolua_S,(bool)ret);
        return 1;
    }
    CCLOG("%s has wrong number of arguments: %d, was expecting %d \n", "unload",argc, 1);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_ww_FileManager_unload'.",&tolua_err);
#endif

    return 0;
}
int lua_ww_FileManager_download(lua_State* tolua_S)
{
    int argc = 0;
    FileManager* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"FileManager",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (FileManager*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_ww_FileManager_download'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 1) 
    {
        std::vector<std::string> arg0;

        ok &= luaval_to_std_vector_string(tolua_S, 2, &arg0);
        if(!ok)
            return 0;
        cobj->download(arg0);
        return 0;
    }
    CCLOG("%s has wrong number of arguments: %d, was expecting %d \n", "download",argc, 1);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_ww_FileManager_download'.",&tolua_err);
#endif

    return 0;
}
int lua_ww_FileManager_getFileIndexRow(lua_State* tolua_S)
{
    int argc = 0;
    FileManager* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"FileManager",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (FileManager*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_ww_FileManager_getFileIndexRow'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 1) 
    {
        std::string arg0;

        ok &= luaval_to_std_string(tolua_S, 2,&arg0);
        if(!ok)
            return 0;
        FileIndexRow* ret = cobj->getFileIndexRow(arg0);
        object_to_luaval<FileIndexRow>(tolua_S, "FileIndexRow",(FileIndexRow*)ret);
        return 1;
    }
    CCLOG("%s has wrong number of arguments: %d, was expecting %d \n", "getFileIndexRow",argc, 1);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_ww_FileManager_getFileIndexRow'.",&tolua_err);
#endif

    return 0;
}
int lua_ww_FileManager_getInstance(lua_State* tolua_S)
{
    int argc = 0;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif

#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertable(tolua_S,1,"FileManager",0,&tolua_err)) goto tolua_lerror;
#endif

    argc = lua_gettop(tolua_S) - 1;

    if (argc == 0)
    {
        if(!ok)
            return 0;
        FileManager* ret = FileManager::getInstance();
        object_to_luaval<FileManager>(tolua_S, "FileManager",(FileManager*)ret);
        return 1;
    }
    CCLOG("%s has wrong number of arguments: %d, was expecting %d\n ", "getInstance",argc, 0);
    return 0;
#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_ww_FileManager_getInstance'.",&tolua_err);
#endif
    return 0;
}
static int lua_ww_FileManager_finalize(lua_State* tolua_S)
{
    printf("luabindings: finalizing LUA object (FileManager)");
    return 0;
}

int lua_register_ww_FileManager(lua_State* tolua_S)
{
    tolua_usertype(tolua_S,"FileManager");
    tolua_cclass(tolua_S,"FileManager","FileManager","",nullptr);

    tolua_beginmodule(tolua_S,"FileManager");
        tolua_function(tolua_S,"load",lua_ww_FileManager_load);
        tolua_function(tolua_S,"loadIndex",lua_ww_FileManager_loadIndex);
        tolua_function(tolua_S,"getIndex",lua_ww_FileManager_getIndex);
        tolua_function(tolua_S,"unload",lua_ww_FileManager_unload);
        tolua_function(tolua_S,"download",lua_ww_FileManager_download);
        tolua_function(tolua_S,"getFileIndexRow",lua_ww_FileManager_getFileIndexRow);
        tolua_function(tolua_S,"getInstance", lua_ww_FileManager_getInstance);
    tolua_endmodule(tolua_S);
    std::string typeName = typeid(FileManager).name();
    g_luaType[typeName] = "FileManager";
    g_typeCast["FileManager"] = "FileManager";
    return 1;
}

int lua_ww_Hosts_getMAC(lua_State* tolua_S)
{
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
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_ww_Hosts_getMAC'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 0) 
    {
        if(!ok)
            return 0;
        const std::string& ret = cobj->getMAC();
        tolua_pushcppstring(tolua_S,ret);
        return 1;
    }
    CCLOG("%s has wrong number of arguments: %d, was expecting %d \n", "getMAC",argc, 0);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_ww_Hosts_getMAC'.",&tolua_err);
#endif

    return 0;
}
int lua_ww_Hosts_base64Encode(lua_State* tolua_S)
{
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
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_ww_Hosts_base64Encode'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 1) 
    {
        std::string arg0;

        ok &= luaval_to_std_string(tolua_S, 2,&arg0);
        if(!ok)
            return 0;
        std::string ret = cobj->base64Encode(arg0);
        tolua_pushcppstring(tolua_S,ret);
        return 1;
    }
    CCLOG("%s has wrong number of arguments: %d, was expecting %d \n", "base64Encode",argc, 1);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_ww_Hosts_base64Encode'.",&tolua_err);
#endif

    return 0;
}
int lua_ww_Hosts_unregisterGameLoopEventHandler(lua_State* tolua_S)
{
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
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_ww_Hosts_unregisterGameLoopEventHandler'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 1) 
    {
        std::string arg0;

        ok &= luaval_to_std_string(tolua_S, 2,&arg0);
        if(!ok)
            return 0;
        bool ret = cobj->unregisterGameLoopEventHandler(arg0);
        tolua_pushboolean(tolua_S,(bool)ret);
        return 1;
    }
    CCLOG("%s has wrong number of arguments: %d, was expecting %d \n", "unregisterGameLoopEventHandler",argc, 1);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_ww_Hosts_unregisterGameLoopEventHandler'.",&tolua_err);
#endif

    return 0;
}
int lua_ww_Hosts_getTime(lua_State* tolua_S)
{
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
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_ww_Hosts_getTime'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 0) 
    {
        if(!ok)
            return 0;
        int ret = cobj->getTime();
        tolua_pushnumber(tolua_S,(lua_Number)ret);
        return 1;
    }
    CCLOG("%s has wrong number of arguments: %d, was expecting %d \n", "getTime",argc, 0);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_ww_Hosts_getTime'.",&tolua_err);
#endif

    return 0;
}
int lua_ww_Hosts_sdkType(lua_State* tolua_S)
{
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
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_ww_Hosts_sdkType'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 0) 
    {
        if(!ok)
            return 0;
        std::string ret = cobj->sdkType();
        tolua_pushcppstring(tolua_S,ret);
        return 1;
    }
    CCLOG("%s has wrong number of arguments: %d, was expecting %d \n", "sdkType",argc, 0);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_ww_Hosts_sdkType'.",&tolua_err);
#endif

    return 0;
}
int lua_ww_Hosts_base64Decode(lua_State* tolua_S)
{
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
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_ww_Hosts_base64Decode'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 1) 
    {
        std::string arg0;

        ok &= luaval_to_std_string(tolua_S, 2,&arg0);
        if(!ok)
            return 0;
        std::string ret = cobj->base64Decode(arg0);
        tolua_pushcppstring(tolua_S,ret);
        return 1;
    }
    CCLOG("%s has wrong number of arguments: %d, was expecting %d \n", "base64Decode",argc, 1);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_ww_Hosts_base64Decode'.",&tolua_err);
#endif

    return 0;
}
int lua_ww_Hosts_getUDID(lua_State* tolua_S)
{
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
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_ww_Hosts_getUDID'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 0) 
    {
        if(!ok)
            return 0;
        const std::string& ret = cobj->getUDID();
        tolua_pushcppstring(tolua_S,ret);
        return 1;
    }
    CCLOG("%s has wrong number of arguments: %d, was expecting %d \n", "getUDID",argc, 0);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_ww_Hosts_getUDID'.",&tolua_err);
#endif

    return 0;
}
int lua_ww_Hosts_addRef(lua_State* tolua_S)
{
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
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_ww_Hosts_addRef'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 1) 
    {
        cocos2d::Ref* arg0;

        ok &= luaval_to_object<cocos2d::Ref>(tolua_S, 2, "cc.Ref",&arg0);
        if(!ok)
            return 0;
        cobj->addRef(arg0);
        return 0;
    }
    CCLOG("%s has wrong number of arguments: %d, was expecting %d \n", "addRef",argc, 1);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_ww_Hosts_addRef'.",&tolua_err);
#endif

    return 0;
}
int lua_ww_Hosts_removeRef(lua_State* tolua_S)
{
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
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_ww_Hosts_removeRef'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 1) 
    {
        cocos2d::Ref* arg0;

        ok &= luaval_to_object<cocos2d::Ref>(tolua_S, 2, "cc.Ref",&arg0);
        if(!ok)
            return 0;
        cobj->removeRef(arg0);
        return 0;
    }
    CCLOG("%s has wrong number of arguments: %d, was expecting %d \n", "removeRef",argc, 1);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_ww_Hosts_removeRef'.",&tolua_err);
#endif

    return 0;
}
int lua_ww_Hosts_openUrl(lua_State* tolua_S)
{
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
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_ww_Hosts_openUrl'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 1) 
    {
        std::string arg0;

        ok &= luaval_to_std_string(tolua_S, 2,&arg0);
        if(!ok)
            return 0;
        cobj->openUrl(arg0);
        return 0;
    }
    CCLOG("%s has wrong number of arguments: %d, was expecting %d \n", "openUrl",argc, 1);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_ww_Hosts_openUrl'.",&tolua_err);
#endif

    return 0;
}
int lua_ww_Hosts_restart(lua_State* tolua_S)
{
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
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_ww_Hosts_restart'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 0) 
    {
        if(!ok)
            return 0;
        cobj->restart();
        return 0;
    }
    CCLOG("%s has wrong number of arguments: %d, was expecting %d \n", "restart",argc, 0);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_ww_Hosts_restart'.",&tolua_err);
#endif

    return 0;
}
int lua_ww_Hosts_notify(lua_State* tolua_S)
{
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
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_ww_Hosts_notify'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 3) 
    {
        int arg0;
        int arg1;
        std::string arg2;

        ok &= luaval_to_int32(tolua_S, 2,(int *)&arg0);

        ok &= luaval_to_int32(tolua_S, 3,(int *)&arg1);

        ok &= luaval_to_std_string(tolua_S, 4,&arg2);
        if(!ok)
            return 0;
        cobj->notify(arg0, arg1, arg2);
        return 0;
    }
    if (argc == 4) 
    {
        int arg0;
        int arg1;
        std::string arg2;
        std::string arg3;

        ok &= luaval_to_int32(tolua_S, 2,(int *)&arg0);

        ok &= luaval_to_int32(tolua_S, 3,(int *)&arg1);

        ok &= luaval_to_std_string(tolua_S, 4,&arg2);

        ok &= luaval_to_std_string(tolua_S, 5,&arg3);
        if(!ok)
            return 0;
        cobj->notify(arg0, arg1, arg2, arg3);
        return 0;
    }
    if (argc == 5) 
    {
        int arg0;
        int arg1;
        std::string arg2;
        std::string arg3;
        std::string arg4;

        ok &= luaval_to_int32(tolua_S, 2,(int *)&arg0);

        ok &= luaval_to_int32(tolua_S, 3,(int *)&arg1);

        ok &= luaval_to_std_string(tolua_S, 4,&arg2);

        ok &= luaval_to_std_string(tolua_S, 5,&arg3);

        ok &= luaval_to_std_string(tolua_S, 6,&arg4);
        if(!ok)
            return 0;
        cobj->notify(arg0, arg1, arg2, arg3, arg4);
        return 0;
    }
    CCLOG("%s has wrong number of arguments: %d, was expecting %d \n", "notify",argc, 3);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_ww_Hosts_notify'.",&tolua_err);
#endif

    return 0;
}
int lua_ww_Hosts_getInstance(lua_State* tolua_S)
{
    int argc = 0;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif

#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertable(tolua_S,1,"Hosts",0,&tolua_err)) goto tolua_lerror;
#endif

    argc = lua_gettop(tolua_S) - 1;

    if (argc == 0)
    {
        if(!ok)
            return 0;
        Hosts* ret = Hosts::getInstance();
        object_to_luaval<Hosts>(tolua_S, "Hosts",(Hosts*)ret);
        return 1;
    }
    CCLOG("%s has wrong number of arguments: %d, was expecting %d\n ", "getInstance",argc, 0);
    return 0;
#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_ww_Hosts_getInstance'.",&tolua_err);
#endif
    return 0;
}
static int lua_ww_Hosts_finalize(lua_State* tolua_S)
{
    printf("luabindings: finalizing LUA object (Hosts)");
    return 0;
}

int lua_register_ww_Hosts(lua_State* tolua_S)
{
    tolua_usertype(tolua_S,"Hosts");
    tolua_cclass(tolua_S,"Hosts","Hosts","cc.Node",nullptr);

    tolua_beginmodule(tolua_S,"Hosts");
        tolua_function(tolua_S,"getMAC",lua_ww_Hosts_getMAC);
        tolua_function(tolua_S,"base64Encode",lua_ww_Hosts_base64Encode);
        tolua_function(tolua_S,"unregisterGameLoopEventHandler",lua_ww_Hosts_unregisterGameLoopEventHandler);
        tolua_function(tolua_S,"getTime",lua_ww_Hosts_getTime);
        tolua_function(tolua_S,"sdkType",lua_ww_Hosts_sdkType);
        tolua_function(tolua_S,"base64Decode",lua_ww_Hosts_base64Decode);
        tolua_function(tolua_S,"getUDID",lua_ww_Hosts_getUDID);
        tolua_function(tolua_S,"addRef",lua_ww_Hosts_addRef);
        tolua_function(tolua_S,"removeRef",lua_ww_Hosts_removeRef);
        tolua_function(tolua_S,"openUrl",lua_ww_Hosts_openUrl);
        tolua_function(tolua_S,"restart",lua_ww_Hosts_restart);
        tolua_function(tolua_S,"notify",lua_ww_Hosts_notify);
        tolua_function(tolua_S,"getInstance", lua_ww_Hosts_getInstance);
    tolua_endmodule(tolua_S);
    std::string typeName = typeid(Hosts).name();
    g_luaType[typeName] = "Hosts";
    g_typeCast["Hosts"] = "Hosts";
    return 1;
}

int lua_ww_BuyInfo_getExchangeURL(lua_State* tolua_S)
{
    int argc = 0;
    BuyInfo* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"BuyInfo",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (BuyInfo*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_ww_BuyInfo_getExchangeURL'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 0) 
    {
        if(!ok)
            return 0;
        const std::string& ret = cobj->getExchangeURL();
        tolua_pushcppstring(tolua_S,ret);
        return 1;
    }
    CCLOG("%s has wrong number of arguments: %d, was expecting %d \n", "getExchangeURL",argc, 0);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_ww_BuyInfo_getExchangeURL'.",&tolua_err);
#endif

    return 0;
}
int lua_ww_BuyInfo_setTransactionBase64(lua_State* tolua_S)
{
    int argc = 0;
    BuyInfo* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"BuyInfo",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (BuyInfo*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_ww_BuyInfo_setTransactionBase64'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 1) 
    {
        std::string arg0;

        ok &= luaval_to_std_string(tolua_S, 2,&arg0);
        if(!ok)
            return 0;
        cobj->setTransactionBase64(arg0);
        return 0;
    }
    CCLOG("%s has wrong number of arguments: %d, was expecting %d \n", "setTransactionBase64",argc, 1);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_ww_BuyInfo_setTransactionBase64'.",&tolua_err);
#endif

    return 0;
}
int lua_ww_BuyInfo_setAccountId(lua_State* tolua_S)
{
    int argc = 0;
    BuyInfo* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"BuyInfo",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (BuyInfo*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_ww_BuyInfo_setAccountId'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 1) 
    {
        int arg0;

        ok &= luaval_to_int32(tolua_S, 2,(int *)&arg0);
        if(!ok)
            return 0;
        cobj->setAccountId(arg0);
        return 0;
    }
    CCLOG("%s has wrong number of arguments: %d, was expecting %d \n", "setAccountId",argc, 1);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_ww_BuyInfo_setAccountId'.",&tolua_err);
#endif

    return 0;
}
int lua_ww_BuyInfo_getAccountName(lua_State* tolua_S)
{
    int argc = 0;
    BuyInfo* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"BuyInfo",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (BuyInfo*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_ww_BuyInfo_getAccountName'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 0) 
    {
        if(!ok)
            return 0;
        const std::string& ret = cobj->getAccountName();
        tolua_pushcppstring(tolua_S,ret);
        return 1;
    }
    CCLOG("%s has wrong number of arguments: %d, was expecting %d \n", "getAccountName",argc, 0);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_ww_BuyInfo_getAccountName'.",&tolua_err);
#endif

    return 0;
}
int lua_ww_BuyInfo_getAccountId(lua_State* tolua_S)
{
    int argc = 0;
    BuyInfo* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"BuyInfo",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (BuyInfo*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_ww_BuyInfo_getAccountId'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 0) 
    {
        if(!ok)
            return 0;
        int ret = cobj->getAccountId();
        tolua_pushnumber(tolua_S,(lua_Number)ret);
        return 1;
    }
    CCLOG("%s has wrong number of arguments: %d, was expecting %d \n", "getAccountId",argc, 0);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_ww_BuyInfo_getAccountId'.",&tolua_err);
#endif

    return 0;
}
int lua_ww_BuyInfo_setTransactionId(lua_State* tolua_S)
{
    int argc = 0;
    BuyInfo* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"BuyInfo",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (BuyInfo*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_ww_BuyInfo_setTransactionId'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 1) 
    {
        std::string arg0;

        ok &= luaval_to_std_string(tolua_S, 2,&arg0);
        if(!ok)
            return 0;
        cobj->setTransactionId(arg0);
        return 0;
    }
    CCLOG("%s has wrong number of arguments: %d, was expecting %d \n", "setTransactionId",argc, 1);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_ww_BuyInfo_setTransactionId'.",&tolua_err);
#endif

    return 0;
}
int lua_ww_BuyInfo_setRegionId(lua_State* tolua_S)
{
    int argc = 0;
    BuyInfo* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"BuyInfo",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (BuyInfo*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_ww_BuyInfo_setRegionId'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 1) 
    {
        int arg0;

        ok &= luaval_to_int32(tolua_S, 2,(int *)&arg0);
        if(!ok)
            return 0;
        cobj->setRegionId(arg0);
        return 0;
    }
    CCLOG("%s has wrong number of arguments: %d, was expecting %d \n", "setRegionId",argc, 1);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_ww_BuyInfo_setRegionId'.",&tolua_err);
#endif

    return 0;
}
int lua_ww_BuyInfo_setAccountName(lua_State* tolua_S)
{
    int argc = 0;
    BuyInfo* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"BuyInfo",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (BuyInfo*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_ww_BuyInfo_setAccountName'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 1) 
    {
        std::string arg0;

        ok &= luaval_to_std_string(tolua_S, 2,&arg0);
        if(!ok)
            return 0;
        cobj->setAccountName(arg0);
        return 0;
    }
    CCLOG("%s has wrong number of arguments: %d, was expecting %d \n", "setAccountName",argc, 1);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_ww_BuyInfo_setAccountName'.",&tolua_err);
#endif

    return 0;
}
int lua_ww_BuyInfo_setExchangeURL(lua_State* tolua_S)
{
    int argc = 0;
    BuyInfo* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"BuyInfo",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (BuyInfo*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_ww_BuyInfo_setExchangeURL'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 1) 
    {
        std::string arg0;

        ok &= luaval_to_std_string(tolua_S, 2,&arg0);
        if(!ok)
            return 0;
        cobj->setExchangeURL(arg0);
        return 0;
    }
    CCLOG("%s has wrong number of arguments: %d, was expecting %d \n", "setExchangeURL",argc, 1);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_ww_BuyInfo_setExchangeURL'.",&tolua_err);
#endif

    return 0;
}
int lua_ww_BuyInfo_setVerifyURL(lua_State* tolua_S)
{
    int argc = 0;
    BuyInfo* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"BuyInfo",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (BuyInfo*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_ww_BuyInfo_setVerifyURL'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 1) 
    {
        std::string arg0;

        ok &= luaval_to_std_string(tolua_S, 2,&arg0);
        if(!ok)
            return 0;
        cobj->setVerifyURL(arg0);
        return 0;
    }
    CCLOG("%s has wrong number of arguments: %d, was expecting %d \n", "setVerifyURL",argc, 1);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_ww_BuyInfo_setVerifyURL'.",&tolua_err);
#endif

    return 0;
}
int lua_ww_BuyInfo_setCharId(lua_State* tolua_S)
{
    int argc = 0;
    BuyInfo* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"BuyInfo",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (BuyInfo*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_ww_BuyInfo_setCharId'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 1) 
    {
        int arg0;

        ok &= luaval_to_int32(tolua_S, 2,(int *)&arg0);
        if(!ok)
            return 0;
        cobj->setCharId(arg0);
        return 0;
    }
    CCLOG("%s has wrong number of arguments: %d, was expecting %d \n", "setCharId",argc, 1);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_ww_BuyInfo_setCharId'.",&tolua_err);
#endif

    return 0;
}
int lua_ww_BuyInfo_getTransactionId(lua_State* tolua_S)
{
    int argc = 0;
    BuyInfo* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"BuyInfo",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (BuyInfo*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_ww_BuyInfo_getTransactionId'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 0) 
    {
        if(!ok)
            return 0;
        const std::string& ret = cobj->getTransactionId();
        tolua_pushcppstring(tolua_S,ret);
        return 1;
    }
    CCLOG("%s has wrong number of arguments: %d, was expecting %d \n", "getTransactionId",argc, 0);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_ww_BuyInfo_getTransactionId'.",&tolua_err);
#endif

    return 0;
}
int lua_ww_BuyInfo_getRegionId(lua_State* tolua_S)
{
    int argc = 0;
    BuyInfo* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"BuyInfo",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (BuyInfo*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_ww_BuyInfo_getRegionId'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 0) 
    {
        if(!ok)
            return 0;
        int ret = cobj->getRegionId();
        tolua_pushnumber(tolua_S,(lua_Number)ret);
        return 1;
    }
    CCLOG("%s has wrong number of arguments: %d, was expecting %d \n", "getRegionId",argc, 0);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_ww_BuyInfo_getRegionId'.",&tolua_err);
#endif

    return 0;
}
int lua_ww_BuyInfo_getCharId(lua_State* tolua_S)
{
    int argc = 0;
    BuyInfo* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"BuyInfo",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (BuyInfo*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_ww_BuyInfo_getCharId'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 0) 
    {
        if(!ok)
            return 0;
        int ret = cobj->getCharId();
        tolua_pushnumber(tolua_S,(lua_Number)ret);
        return 1;
    }
    CCLOG("%s has wrong number of arguments: %d, was expecting %d \n", "getCharId",argc, 0);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_ww_BuyInfo_getCharId'.",&tolua_err);
#endif

    return 0;
}
int lua_ww_BuyInfo_getProductId(lua_State* tolua_S)
{
    int argc = 0;
    BuyInfo* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"BuyInfo",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (BuyInfo*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_ww_BuyInfo_getProductId'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 0) 
    {
        if(!ok)
            return 0;
        const std::string& ret = cobj->getProductId();
        tolua_pushcppstring(tolua_S,ret);
        return 1;
    }
    CCLOG("%s has wrong number of arguments: %d, was expecting %d \n", "getProductId",argc, 0);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_ww_BuyInfo_getProductId'.",&tolua_err);
#endif

    return 0;
}
int lua_ww_BuyInfo_setProductId(lua_State* tolua_S)
{
    int argc = 0;
    BuyInfo* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"BuyInfo",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (BuyInfo*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_ww_BuyInfo_setProductId'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 1) 
    {
        std::string arg0;

        ok &= luaval_to_std_string(tolua_S, 2,&arg0);
        if(!ok)
            return 0;
        cobj->setProductId(arg0);
        return 0;
    }
    CCLOG("%s has wrong number of arguments: %d, was expecting %d \n", "setProductId",argc, 1);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_ww_BuyInfo_setProductId'.",&tolua_err);
#endif

    return 0;
}
int lua_ww_BuyInfo_getVerifyURL(lua_State* tolua_S)
{
    int argc = 0;
    BuyInfo* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"BuyInfo",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (BuyInfo*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_ww_BuyInfo_getVerifyURL'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 0) 
    {
        if(!ok)
            return 0;
        const std::string& ret = cobj->getVerifyURL();
        tolua_pushcppstring(tolua_S,ret);
        return 1;
    }
    CCLOG("%s has wrong number of arguments: %d, was expecting %d \n", "getVerifyURL",argc, 0);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_ww_BuyInfo_getVerifyURL'.",&tolua_err);
#endif

    return 0;
}
int lua_ww_BuyInfo_getTransactionBase64(lua_State* tolua_S)
{
    int argc = 0;
    BuyInfo* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"BuyInfo",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (BuyInfo*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_ww_BuyInfo_getTransactionBase64'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 0) 
    {
        if(!ok)
            return 0;
        const std::string& ret = cobj->getTransactionBase64();
        tolua_pushcppstring(tolua_S,ret);
        return 1;
    }
    CCLOG("%s has wrong number of arguments: %d, was expecting %d \n", "getTransactionBase64",argc, 0);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_ww_BuyInfo_getTransactionBase64'.",&tolua_err);
#endif

    return 0;
}
int lua_ww_BuyInfo_create(lua_State* tolua_S)
{
    int argc = 0;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif

#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertable(tolua_S,1,"BuyInfo",0,&tolua_err)) goto tolua_lerror;
#endif

    argc = lua_gettop(tolua_S) - 1;

    if (argc == 0)
    {
        if(!ok)
            return 0;
        BuyInfo* ret = BuyInfo::create();
        object_to_luaval<BuyInfo>(tolua_S, "BuyInfo",(BuyInfo*)ret);
        return 1;
    }
    CCLOG("%s has wrong number of arguments: %d, was expecting %d\n ", "create",argc, 0);
    return 0;
#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_ww_BuyInfo_create'.",&tolua_err);
#endif
    return 0;
}
static int lua_ww_BuyInfo_finalize(lua_State* tolua_S)
{
    printf("luabindings: finalizing LUA object (BuyInfo)");
    return 0;
}

int lua_register_ww_BuyInfo(lua_State* tolua_S)
{
    tolua_usertype(tolua_S,"BuyInfo");
    tolua_cclass(tolua_S,"BuyInfo","BuyInfo","cc.Ref",nullptr);

    tolua_beginmodule(tolua_S,"BuyInfo");
        tolua_function(tolua_S,"getExchangeURL",lua_ww_BuyInfo_getExchangeURL);
        tolua_function(tolua_S,"setTransactionBase64",lua_ww_BuyInfo_setTransactionBase64);
        tolua_function(tolua_S,"setAccountId",lua_ww_BuyInfo_setAccountId);
        tolua_function(tolua_S,"getAccountName",lua_ww_BuyInfo_getAccountName);
        tolua_function(tolua_S,"getAccountId",lua_ww_BuyInfo_getAccountId);
        tolua_function(tolua_S,"setTransactionId",lua_ww_BuyInfo_setTransactionId);
        tolua_function(tolua_S,"setRegionId",lua_ww_BuyInfo_setRegionId);
        tolua_function(tolua_S,"setAccountName",lua_ww_BuyInfo_setAccountName);
        tolua_function(tolua_S,"setExchangeURL",lua_ww_BuyInfo_setExchangeURL);
        tolua_function(tolua_S,"setVerifyURL",lua_ww_BuyInfo_setVerifyURL);
        tolua_function(tolua_S,"setCharId",lua_ww_BuyInfo_setCharId);
        tolua_function(tolua_S,"getTransactionId",lua_ww_BuyInfo_getTransactionId);
        tolua_function(tolua_S,"getRegionId",lua_ww_BuyInfo_getRegionId);
        tolua_function(tolua_S,"getCharId",lua_ww_BuyInfo_getCharId);
        tolua_function(tolua_S,"getProductId",lua_ww_BuyInfo_getProductId);
        tolua_function(tolua_S,"setProductId",lua_ww_BuyInfo_setProductId);
        tolua_function(tolua_S,"getVerifyURL",lua_ww_BuyInfo_getVerifyURL);
        tolua_function(tolua_S,"getTransactionBase64",lua_ww_BuyInfo_getTransactionBase64);
        tolua_function(tolua_S,"create", lua_ww_BuyInfo_create);
    tolua_endmodule(tolua_S);
    std::string typeName = typeid(BuyInfo).name();
    g_luaType[typeName] = "BuyInfo";
    g_typeCast["BuyInfo"] = "BuyInfo";
    return 1;
}

int lua_ww_Product_setPrice(lua_State* tolua_S)
{
    int argc = 0;
    Product* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"Product",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (Product*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_ww_Product_setPrice'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 1) 
    {
        std::string arg0;

        ok &= luaval_to_std_string(tolua_S, 2,&arg0);
        if(!ok)
            return 0;
        cobj->setPrice(arg0);
        return 0;
    }
    CCLOG("%s has wrong number of arguments: %d, was expecting %d \n", "setPrice",argc, 1);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_ww_Product_setPrice'.",&tolua_err);
#endif

    return 0;
}
int lua_ww_Product_getDesc(lua_State* tolua_S)
{
    int argc = 0;
    Product* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"Product",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (Product*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_ww_Product_getDesc'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 0) 
    {
        if(!ok)
            return 0;
        const std::string& ret = cobj->getDesc();
        tolua_pushcppstring(tolua_S,ret);
        return 1;
    }
    CCLOG("%s has wrong number of arguments: %d, was expecting %d \n", "getDesc",argc, 0);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_ww_Product_getDesc'.",&tolua_err);
#endif

    return 0;
}
int lua_ww_Product_setPid(lua_State* tolua_S)
{
    int argc = 0;
    Product* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"Product",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (Product*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_ww_Product_setPid'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 1) 
    {
        std::string arg0;

        ok &= luaval_to_std_string(tolua_S, 2,&arg0);
        if(!ok)
            return 0;
        cobj->setPid(arg0);
        return 0;
    }
    CCLOG("%s has wrong number of arguments: %d, was expecting %d \n", "setPid",argc, 1);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_ww_Product_setPid'.",&tolua_err);
#endif

    return 0;
}
int lua_ww_Product_setTitle(lua_State* tolua_S)
{
    int argc = 0;
    Product* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"Product",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (Product*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_ww_Product_setTitle'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 1) 
    {
        std::string arg0;

        ok &= luaval_to_std_string(tolua_S, 2,&arg0);
        if(!ok)
            return 0;
        cobj->setTitle(arg0);
        return 0;
    }
    CCLOG("%s has wrong number of arguments: %d, was expecting %d \n", "setTitle",argc, 1);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_ww_Product_setTitle'.",&tolua_err);
#endif

    return 0;
}
int lua_ww_Product_getPid(lua_State* tolua_S)
{
    int argc = 0;
    Product* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"Product",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (Product*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_ww_Product_getPid'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 0) 
    {
        if(!ok)
            return 0;
        const std::string& ret = cobj->getPid();
        tolua_pushcppstring(tolua_S,ret);
        return 1;
    }
    CCLOG("%s has wrong number of arguments: %d, was expecting %d \n", "getPid",argc, 0);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_ww_Product_getPid'.",&tolua_err);
#endif

    return 0;
}
int lua_ww_Product_getTitle(lua_State* tolua_S)
{
    int argc = 0;
    Product* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"Product",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (Product*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_ww_Product_getTitle'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 0) 
    {
        if(!ok)
            return 0;
        const std::string& ret = cobj->getTitle();
        tolua_pushcppstring(tolua_S,ret);
        return 1;
    }
    CCLOG("%s has wrong number of arguments: %d, was expecting %d \n", "getTitle",argc, 0);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_ww_Product_getTitle'.",&tolua_err);
#endif

    return 0;
}
int lua_ww_Product_getPrice(lua_State* tolua_S)
{
    int argc = 0;
    Product* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"Product",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (Product*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_ww_Product_getPrice'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 0) 
    {
        if(!ok)
            return 0;
        const std::string& ret = cobj->getPrice();
        tolua_pushcppstring(tolua_S,ret);
        return 1;
    }
    CCLOG("%s has wrong number of arguments: %d, was expecting %d \n", "getPrice",argc, 0);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_ww_Product_getPrice'.",&tolua_err);
#endif

    return 0;
}
int lua_ww_Product_setDesc(lua_State* tolua_S)
{
    int argc = 0;
    Product* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"Product",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (Product*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_ww_Product_setDesc'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 1) 
    {
        std::string arg0;

        ok &= luaval_to_std_string(tolua_S, 2,&arg0);
        if(!ok)
            return 0;
        cobj->setDesc(arg0);
        return 0;
    }
    CCLOG("%s has wrong number of arguments: %d, was expecting %d \n", "setDesc",argc, 1);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_ww_Product_setDesc'.",&tolua_err);
#endif

    return 0;
}
int lua_ww_Product_create(lua_State* tolua_S)
{
    int argc = 0;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif

#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertable(tolua_S,1,"Product",0,&tolua_err)) goto tolua_lerror;
#endif

    argc = lua_gettop(tolua_S) - 1;

    if (argc == 0)
    {
        if(!ok)
            return 0;
        Product* ret = Product::create();
        object_to_luaval<Product>(tolua_S, "Product",(Product*)ret);
        return 1;
    }
    CCLOG("%s has wrong number of arguments: %d, was expecting %d\n ", "create",argc, 0);
    return 0;
#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_ww_Product_create'.",&tolua_err);
#endif
    return 0;
}
static int lua_ww_Product_finalize(lua_State* tolua_S)
{
    printf("luabindings: finalizing LUA object (Product)");
    return 0;
}

int lua_register_ww_Product(lua_State* tolua_S)
{
    tolua_usertype(tolua_S,"Product");
    tolua_cclass(tolua_S,"Product","Product","cc.Ref",nullptr);

    tolua_beginmodule(tolua_S,"Product");
        tolua_function(tolua_S,"setPrice",lua_ww_Product_setPrice);
        tolua_function(tolua_S,"getDesc",lua_ww_Product_getDesc);
        tolua_function(tolua_S,"setPid",lua_ww_Product_setPid);
        tolua_function(tolua_S,"setTitle",lua_ww_Product_setTitle);
        tolua_function(tolua_S,"getPid",lua_ww_Product_getPid);
        tolua_function(tolua_S,"getTitle",lua_ww_Product_getTitle);
        tolua_function(tolua_S,"getPrice",lua_ww_Product_getPrice);
        tolua_function(tolua_S,"setDesc",lua_ww_Product_setDesc);
        tolua_function(tolua_S,"create", lua_ww_Product_create);
    tolua_endmodule(tolua_S);
    std::string typeName = typeid(Product).name();
    g_luaType[typeName] = "Product";
    g_typeCast["Product"] = "Product";
    return 1;
}

int lua_ww_Products_insert(lua_State* tolua_S)
{
    int argc = 0;
    Products* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"Products",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (Products*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_ww_Products_insert'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 1) 
    {
        Product* arg0;

        ok &= luaval_to_object<Product>(tolua_S, 2, "Product",&arg0);
        if(!ok)
            return 0;
        cobj->insert(arg0);
        return 0;
    }
    CCLOG("%s has wrong number of arguments: %d, was expecting %d \n", "insert",argc, 1);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_ww_Products_insert'.",&tolua_err);
#endif

    return 0;
}
int lua_ww_Products_clear(lua_State* tolua_S)
{
    int argc = 0;
    Products* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"Products",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (Products*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_ww_Products_clear'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 0) 
    {
        if(!ok)
            return 0;
        cobj->clear();
        return 0;
    }
    CCLOG("%s has wrong number of arguments: %d, was expecting %d \n", "clear",argc, 0);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_ww_Products_clear'.",&tolua_err);
#endif

    return 0;
}
int lua_ww_Products_erase(lua_State* tolua_S)
{
    int argc = 0;
    Products* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"Products",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (Products*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_ww_Products_erase'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 1) 
    {
        std::string arg0;

        ok &= luaval_to_std_string(tolua_S, 2,&arg0);
        if(!ok)
            return 0;
        cobj->erase(arg0);
        return 0;
    }
    CCLOG("%s has wrong number of arguments: %d, was expecting %d \n", "erase",argc, 1);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_ww_Products_erase'.",&tolua_err);
#endif

    return 0;
}
int lua_ww_Products_at(lua_State* tolua_S)
{
    int argc = 0;
    Products* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"Products",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (Products*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_ww_Products_at'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 1) 
    {
        int arg0;

        ok &= luaval_to_int32(tolua_S, 2,(int *)&arg0);
        if(!ok)
            return 0;
        Product* ret = cobj->at(arg0);
        object_to_luaval<Product>(tolua_S, "Product",(Product*)ret);
        return 1;
    }
    CCLOG("%s has wrong number of arguments: %d, was expecting %d \n", "at",argc, 1);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_ww_Products_at'.",&tolua_err);
#endif

    return 0;
}
int lua_ww_Products_empty(lua_State* tolua_S)
{
    int argc = 0;
    Products* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"Products",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (Products*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_ww_Products_empty'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 0) 
    {
        if(!ok)
            return 0;
        bool ret = cobj->empty();
        tolua_pushboolean(tolua_S,(bool)ret);
        return 1;
    }
    CCLOG("%s has wrong number of arguments: %d, was expecting %d \n", "empty",argc, 0);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_ww_Products_empty'.",&tolua_err);
#endif

    return 0;
}
int lua_ww_Products_size(lua_State* tolua_S)
{
    int argc = 0;
    Products* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"Products",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (Products*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_ww_Products_size'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 0) 
    {
        if(!ok)
            return 0;
        int ret = cobj->size();
        tolua_pushnumber(tolua_S,(lua_Number)ret);
        return 1;
    }
    CCLOG("%s has wrong number of arguments: %d, was expecting %d \n", "size",argc, 0);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_ww_Products_size'.",&tolua_err);
#endif

    return 0;
}
static int lua_ww_Products_finalize(lua_State* tolua_S)
{
    printf("luabindings: finalizing LUA object (Products)");
    return 0;
}

int lua_register_ww_Products(lua_State* tolua_S)
{
    tolua_usertype(tolua_S,"Products");
    tolua_cclass(tolua_S,"Products","Products","",nullptr);

    tolua_beginmodule(tolua_S,"Products");
        tolua_function(tolua_S,"insert",lua_ww_Products_insert);
        tolua_function(tolua_S,"clear",lua_ww_Products_clear);
        tolua_function(tolua_S,"erase",lua_ww_Products_erase);
        tolua_function(tolua_S,"at",lua_ww_Products_at);
        tolua_function(tolua_S,"empty",lua_ww_Products_empty);
        tolua_function(tolua_S,"size",lua_ww_Products_size);
    tolua_endmodule(tolua_S);
    std::string typeName = typeid(Products).name();
    g_luaType[typeName] = "Products";
    g_typeCast["Products"] = "Products";
    return 1;
}

int lua_ww_Purchase_pull(lua_State* tolua_S)
{
    int argc = 0;
    Purchase* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"Purchase",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (Purchase*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_ww_Purchase_pull'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 1) 
    {
        std::vector<std::string> arg0;

        ok &= luaval_to_std_vector_string(tolua_S, 2, &arg0);
        if(!ok)
            return 0;
        bool ret = cobj->pull(arg0);
        tolua_pushboolean(tolua_S,(bool)ret);
        return 1;
    }
    CCLOG("%s has wrong number of arguments: %d, was expecting %d \n", "pull",argc, 1);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_ww_Purchase_pull'.",&tolua_err);
#endif

    return 0;
}
int lua_ww_Purchase_buy(lua_State* tolua_S)
{
    int argc = 0;
    Purchase* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"Purchase",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (Purchase*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_ww_Purchase_buy'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 1) 
    {
        std::string arg0;

        ok &= luaval_to_std_string(tolua_S, 2,&arg0);
        if(!ok)
            return 0;
        bool ret = cobj->buy(arg0);
        tolua_pushboolean(tolua_S,(bool)ret);
        return 1;
    }
    CCLOG("%s has wrong number of arguments: %d, was expecting %d \n", "buy",argc, 1);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_ww_Purchase_buy'.",&tolua_err);
#endif

    return 0;
}
int lua_ww_Purchase_exchange(lua_State* tolua_S)
{
    int argc = 0;
    Purchase* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"Purchase",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (Purchase*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_ww_Purchase_exchange'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 1) 
    {
        std::string arg0;

        ok &= luaval_to_std_string(tolua_S, 2,&arg0);
        if(!ok)
            return 0;
        cobj->exchange(arg0);
        return 0;
    }
    CCLOG("%s has wrong number of arguments: %d, was expecting %d \n", "exchange",argc, 1);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_ww_Purchase_exchange'.",&tolua_err);
#endif

    return 0;
}
int lua_ww_Purchase_getBuyInfo(lua_State* tolua_S)
{
    int argc = 0;
    Purchase* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"Purchase",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (Purchase*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_ww_Purchase_getBuyInfo'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 0) 
    {
        if(!ok)
            return 0;
        BuyInfo* ret = cobj->getBuyInfo();
        object_to_luaval<BuyInfo>(tolua_S, "BuyInfo",(BuyInfo*)ret);
        return 1;
    }
    CCLOG("%s has wrong number of arguments: %d, was expecting %d \n", "getBuyInfo",argc, 0);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_ww_Purchase_getBuyInfo'.",&tolua_err);
#endif

    return 0;
}
int lua_ww_Purchase_stop(lua_State* tolua_S)
{
    int argc = 0;
    Purchase* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"Purchase",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (Purchase*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_ww_Purchase_stop'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 0) 
    {
        if(!ok)
            return 0;
        cobj->stop();
        return 0;
    }
    CCLOG("%s has wrong number of arguments: %d, was expecting %d \n", "stop",argc, 0);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_ww_Purchase_stop'.",&tolua_err);
#endif

    return 0;
}
int lua_ww_Purchase_remove(lua_State* tolua_S)
{
    int argc = 0;
    Purchase* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"Purchase",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (Purchase*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_ww_Purchase_remove'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 1) 
    {
        std::string arg0;

        ok &= luaval_to_std_string(tolua_S, 2,&arg0);
        if(!ok)
            return 0;
        cobj->remove(arg0);
        return 0;
    }
    CCLOG("%s has wrong number of arguments: %d, was expecting %d \n", "remove",argc, 1);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_ww_Purchase_remove'.",&tolua_err);
#endif

    return 0;
}
int lua_ww_Purchase_start(lua_State* tolua_S)
{
    int argc = 0;
    Purchase* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"Purchase",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (Purchase*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_ww_Purchase_start'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 6) 
    {
        std::string arg0;
        int arg1;
        int arg2;
        int arg3;
        std::string arg4;
        std::string arg5;

        ok &= luaval_to_std_string(tolua_S, 2,&arg0);

        ok &= luaval_to_int32(tolua_S, 3,(int *)&arg1);

        ok &= luaval_to_int32(tolua_S, 4,(int *)&arg2);

        ok &= luaval_to_int32(tolua_S, 5,(int *)&arg3);

        ok &= luaval_to_std_string(tolua_S, 6,&arg4);

        ok &= luaval_to_std_string(tolua_S, 7,&arg5);
        if(!ok)
            return 0;
        cobj->start(arg0, arg1, arg2, arg3, arg4, arg5);
        return 0;
    }
    CCLOG("%s has wrong number of arguments: %d, was expecting %d \n", "start",argc, 6);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_ww_Purchase_start'.",&tolua_err);
#endif

    return 0;
}
int lua_ww_Purchase_add(lua_State* tolua_S)
{
    int argc = 0;
    Purchase* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"Purchase",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (Purchase*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_ww_Purchase_add'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 4) 
    {
        std::string arg0;
        std::string arg1;
        std::string arg2;
        std::string arg3;

        ok &= luaval_to_std_string(tolua_S, 2,&arg0);

        ok &= luaval_to_std_string(tolua_S, 3,&arg1);

        ok &= luaval_to_std_string(tolua_S, 4,&arg2);

        ok &= luaval_to_std_string(tolua_S, 5,&arg3);
        if(!ok)
            return 0;
        cobj->add(arg0, arg1, arg2, arg3);
        return 0;
    }
    CCLOG("%s has wrong number of arguments: %d, was expecting %d \n", "add",argc, 4);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_ww_Purchase_add'.",&tolua_err);
#endif

    return 0;
}
int lua_ww_Purchase_close(lua_State* tolua_S)
{
    int argc = 0;
    Purchase* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"Purchase",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (Purchase*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_ww_Purchase_close'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 1) 
    {
        std::string arg0;

        ok &= luaval_to_std_string(tolua_S, 2,&arg0);
        if(!ok)
            return 0;
        cobj->close(arg0);
        return 0;
    }
    CCLOG("%s has wrong number of arguments: %d, was expecting %d \n", "close",argc, 1);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_ww_Purchase_close'.",&tolua_err);
#endif

    return 0;
}
int lua_ww_Purchase_getProducts(lua_State* tolua_S)
{
    int argc = 0;
    Purchase* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"Purchase",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (Purchase*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_ww_Purchase_getProducts'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 0) 
    {
        if(!ok)
            return 0;
        Products* ret = cobj->getProducts();
        object_to_luaval<Products>(tolua_S, "Products",(Products*)ret);
        return 1;
    }
    CCLOG("%s has wrong number of arguments: %d, was expecting %d \n", "getProducts",argc, 0);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_ww_Purchase_getProducts'.",&tolua_err);
#endif

    return 0;
}
int lua_ww_Purchase_getInstance(lua_State* tolua_S)
{
    int argc = 0;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif

#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertable(tolua_S,1,"Purchase",0,&tolua_err)) goto tolua_lerror;
#endif

    argc = lua_gettop(tolua_S) - 1;

    if (argc == 0)
    {
        if(!ok)
            return 0;
        Purchase* ret = Purchase::getInstance();
        object_to_luaval<Purchase>(tolua_S, "Purchase",(Purchase*)ret);
        return 1;
    }
    CCLOG("%s has wrong number of arguments: %d, was expecting %d\n ", "getInstance",argc, 0);
    return 0;
#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_ww_Purchase_getInstance'.",&tolua_err);
#endif
    return 0;
}
static int lua_ww_Purchase_finalize(lua_State* tolua_S)
{
    printf("luabindings: finalizing LUA object (Purchase)");
    return 0;
}

int lua_register_ww_Purchase(lua_State* tolua_S)
{
    tolua_usertype(tolua_S,"Purchase");
    tolua_cclass(tolua_S,"Purchase","Purchase","",nullptr);

    tolua_beginmodule(tolua_S,"Purchase");
        tolua_function(tolua_S,"pull",lua_ww_Purchase_pull);
        tolua_function(tolua_S,"buy",lua_ww_Purchase_buy);
        tolua_function(tolua_S,"exchange",lua_ww_Purchase_exchange);
        tolua_function(tolua_S,"getBuyInfo",lua_ww_Purchase_getBuyInfo);
        tolua_function(tolua_S,"stop",lua_ww_Purchase_stop);
        tolua_function(tolua_S,"remove",lua_ww_Purchase_remove);
        tolua_function(tolua_S,"start",lua_ww_Purchase_start);
        tolua_function(tolua_S,"add",lua_ww_Purchase_add);
        tolua_function(tolua_S,"close",lua_ww_Purchase_close);
        tolua_function(tolua_S,"getProducts",lua_ww_Purchase_getProducts);
        tolua_function(tolua_S,"getInstance", lua_ww_Purchase_getInstance);
    tolua_endmodule(tolua_S);
    std::string typeName = typeid(Purchase).name();
    g_luaType[typeName] = "Purchase";
    g_typeCast["Purchase"] = "Purchase";
    return 1;
}

int lua_ww_Random_nextInt(lua_State* tolua_S)
{
    int argc = 0;
    Random* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"Random",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (Random*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_ww_Random_nextInt'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 1) 
    {
        int arg0;

        ok &= luaval_to_int32(tolua_S, 2,(int *)&arg0);
        if(!ok)
            return 0;
        int ret = cobj->nextInt(arg0);
        tolua_pushnumber(tolua_S,(lua_Number)ret);
        return 1;
    }
    if (argc == 2) 
    {
        int arg0;
        int arg1;

        ok &= luaval_to_int32(tolua_S, 2,(int *)&arg0);

        ok &= luaval_to_int32(tolua_S, 3,(int *)&arg1);
        if(!ok)
            return 0;
        int ret = cobj->nextInt(arg0, arg1);
        tolua_pushnumber(tolua_S,(lua_Number)ret);
        return 1;
    }
    CCLOG("%s has wrong number of arguments: %d, was expecting %d \n", "nextInt",argc, 1);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_ww_Random_nextInt'.",&tolua_err);
#endif

    return 0;
}
int lua_ww_Random_nextDouble(lua_State* tolua_S)
{
    int argc = 0;
    Random* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"Random",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (Random*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_ww_Random_nextDouble'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 0) 
    {
        if(!ok)
            return 0;
        double ret = cobj->nextDouble();
        tolua_pushnumber(tolua_S,(lua_Number)ret);
        return 1;
    }
    CCLOG("%s has wrong number of arguments: %d, was expecting %d \n", "nextDouble",argc, 0);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_ww_Random_nextDouble'.",&tolua_err);
#endif

    return 0;
}
int lua_ww_Random_create(lua_State* tolua_S)
{
    int argc = 0;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif

#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertable(tolua_S,1,"Random",0,&tolua_err)) goto tolua_lerror;
#endif

    argc = lua_gettop(tolua_S) - 1;

    if (argc == 1)
    {
        int arg0;
        ok &= luaval_to_int32(tolua_S, 2,(int *)&arg0);
        if(!ok)
            return 0;
        Random* ret = Random::create(arg0);
        object_to_luaval<Random>(tolua_S, "Random",(Random*)ret);
        return 1;
    }
    CCLOG("%s has wrong number of arguments: %d, was expecting %d\n ", "create",argc, 1);
    return 0;
#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_ww_Random_create'.",&tolua_err);
#endif
    return 0;
}
static int lua_ww_Random_finalize(lua_State* tolua_S)
{
    printf("luabindings: finalizing LUA object (Random)");
    return 0;
}

int lua_register_ww_Random(lua_State* tolua_S)
{
    tolua_usertype(tolua_S,"Random");
    tolua_cclass(tolua_S,"Random","Random","cc.Ref",nullptr);

    tolua_beginmodule(tolua_S,"Random");
        tolua_function(tolua_S,"nextInt",lua_ww_Random_nextInt);
        tolua_function(tolua_S,"nextDouble",lua_ww_Random_nextDouble);
        tolua_function(tolua_S,"create", lua_ww_Random_create);
    tolua_endmodule(tolua_S);
    std::string typeName = typeid(Random).name();
    g_luaType[typeName] = "Random";
    g_typeCast["Random"] = "Random";
    return 1;
}

int lua_ww_ChatSys_soundRecording(lua_State* tolua_S)
{
    int argc = 0;
    ChatSys* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"ChatSys",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (ChatSys*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_ww_ChatSys_soundRecording'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 0) 
    {
        if(!ok)
            return 0;
        cobj->soundRecording();
        return 0;
    }
    CCLOG("%s has wrong number of arguments: %d, was expecting %d \n", "soundRecording",argc, 0);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_ww_ChatSys_soundRecording'.",&tolua_err);
#endif

    return 0;
}
int lua_ww_ChatSys_soundRecordingFinished(lua_State* tolua_S)
{
    int argc = 0;
    ChatSys* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"ChatSys",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (ChatSys*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_ww_ChatSys_soundRecordingFinished'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 0) 
    {
        if(!ok)
            return 0;
        cobj->soundRecordingFinished();
        return 0;
    }
    CCLOG("%s has wrong number of arguments: %d, was expecting %d \n", "soundRecordingFinished",argc, 0);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_ww_ChatSys_soundRecordingFinished'.",&tolua_err);
#endif

    return 0;
}
int lua_ww_ChatSys_setChannelKey(lua_State* tolua_S)
{
    int argc = 0;
    ChatSys* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"ChatSys",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (ChatSys*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_ww_ChatSys_setChannelKey'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 2) 
    {
        int arg0;
        std::string arg1;

        ok &= luaval_to_int32(tolua_S, 2,(int *)&arg0);

        ok &= luaval_to_std_string(tolua_S, 3,&arg1);
        if(!ok)
            return 0;
        cobj->setChannelKey(arg0, arg1);
        return 0;
    }
    CCLOG("%s has wrong number of arguments: %d, was expecting %d \n", "setChannelKey",argc, 2);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_ww_ChatSys_setChannelKey'.",&tolua_err);
#endif

    return 0;
}
int lua_ww_ChatSys_playAudioWithFilePath(lua_State* tolua_S)
{
    int argc = 0;
    ChatSys* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"ChatSys",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (ChatSys*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_ww_ChatSys_playAudioWithFilePath'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 1) 
    {
        std::string arg0;

        ok &= luaval_to_std_string(tolua_S, 2,&arg0);
        if(!ok)
            return 0;
        cobj->playAudioWithFilePath(arg0);
        return 0;
    }
    CCLOG("%s has wrong number of arguments: %d, was expecting %d \n", "playAudioWithFilePath",argc, 1);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_ww_ChatSys_playAudioWithFilePath'.",&tolua_err);
#endif

    return 0;
}
int lua_ww_ChatSys_resetChannelKeys(lua_State* tolua_S)
{
    int argc = 0;
    ChatSys* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"ChatSys",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (ChatSys*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_ww_ChatSys_resetChannelKeys'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 0) 
    {
        if(!ok)
            return 0;
        cobj->resetChannelKeys();
        return 0;
    }
    CCLOG("%s has wrong number of arguments: %d, was expecting %d \n", "resetChannelKeys",argc, 0);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_ww_ChatSys_resetChannelKeys'.",&tolua_err);
#endif

    return 0;
}
int lua_ww_ChatSys_setLegionId(lua_State* tolua_S)
{
    int argc = 0;
    ChatSys* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"ChatSys",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (ChatSys*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_ww_ChatSys_setLegionId'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 1) 
    {
        int arg0;

        ok &= luaval_to_int32(tolua_S, 2,(int *)&arg0);
        if(!ok)
            return 0;
        cobj->setLegionId(arg0);
        return 0;
    }
    if (argc == 2) 
    {
        int arg0;
        bool arg1;

        ok &= luaval_to_int32(tolua_S, 2,(int *)&arg0);

        ok &= luaval_to_boolean(tolua_S, 3,&arg1);
        if(!ok)
            return 0;
        cobj->setLegionId(arg0, arg1);
        return 0;
    }
    CCLOG("%s has wrong number of arguments: %d, was expecting %d \n", "setLegionId",argc, 1);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_ww_ChatSys_setLegionId'.",&tolua_err);
#endif

    return 0;
}
int lua_ww_ChatSys_login_yaya(lua_State* tolua_S)
{
    int argc = 0;
    ChatSys* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"ChatSys",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (ChatSys*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_ww_ChatSys_login_yaya'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 1) 
    {
        std::string arg0;

        ok &= luaval_to_std_string(tolua_S, 2,&arg0);
        if(!ok)
            return 0;
        cobj->login_yaya(arg0);
        return 0;
    }
    CCLOG("%s has wrong number of arguments: %d, was expecting %d \n", "login_yaya",argc, 1);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_ww_ChatSys_login_yaya'.",&tolua_err);
#endif

    return 0;
}
int lua_ww_ChatSys_setGameServerId(lua_State* tolua_S)
{
    int argc = 0;
    ChatSys* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"ChatSys",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (ChatSys*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_ww_ChatSys_setGameServerId'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 1) 
    {
        std::string arg0;

        ok &= luaval_to_std_string(tolua_S, 2,&arg0);
        if(!ok)
            return 0;
        cobj->setGameServerId(arg0);
        return 0;
    }
    if (argc == 2) 
    {
        std::string arg0;
        bool arg1;

        ok &= luaval_to_std_string(tolua_S, 2,&arg0);

        ok &= luaval_to_boolean(tolua_S, 3,&arg1);
        if(!ok)
            return 0;
        cobj->setGameServerId(arg0, arg1);
        return 0;
    }
    CCLOG("%s has wrong number of arguments: %d, was expecting %d \n", "setGameServerId",argc, 1);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_ww_ChatSys_setGameServerId'.",&tolua_err);
#endif

    return 0;
}
int lua_ww_ChatSys_loginChannel(lua_State* tolua_S)
{
    int argc = 0;
    ChatSys* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"ChatSys",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (ChatSys*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_ww_ChatSys_loginChannel'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 0) 
    {
        if(!ok)
            return 0;
        cobj->loginChannel();
        return 0;
    }
    CCLOG("%s has wrong number of arguments: %d, was expecting %d \n", "loginChannel",argc, 0);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_ww_ChatSys_loginChannel'.",&tolua_err);
#endif

    return 0;
}
int lua_ww_ChatSys_playAudioWithUrl(lua_State* tolua_S)
{
    int argc = 0;
    ChatSys* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"ChatSys",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (ChatSys*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_ww_ChatSys_playAudioWithUrl'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 1) 
    {
        std::string arg0;

        ok &= luaval_to_std_string(tolua_S, 2,&arg0);
        if(!ok)
            return 0;
        cobj->playAudioWithUrl(arg0);
        return 0;
    }
    CCLOG("%s has wrong number of arguments: %d, was expecting %d \n", "playAudioWithUrl",argc, 1);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_ww_ChatSys_playAudioWithUrl'.",&tolua_err);
#endif

    return 0;
}
int lua_ww_ChatSys_soundRecordingStop(lua_State* tolua_S)
{
    int argc = 0;
    ChatSys* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"ChatSys",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (ChatSys*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_ww_ChatSys_soundRecordingStop'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 0) 
    {
        if(!ok)
            return 0;
        cobj->soundRecordingStop();
        return 0;
    }
    CCLOG("%s has wrong number of arguments: %d, was expecting %d \n", "soundRecordingStop",argc, 0);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_ww_ChatSys_soundRecordingStop'.",&tolua_err);
#endif

    return 0;
}
int lua_ww_ChatSys_sendVoiceAndText(lua_State* tolua_S)
{
    int argc = 0;
    ChatSys* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"ChatSys",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (ChatSys*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_ww_ChatSys_sendVoiceAndText'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 4) 
    {
        std::string arg0;
        int arg1;
        std::string arg2;
        std::string arg3;

        ok &= luaval_to_std_string(tolua_S, 2,&arg0);

        ok &= luaval_to_int32(tolua_S, 3,(int *)&arg1);

        ok &= luaval_to_std_string(tolua_S, 4,&arg2);

        ok &= luaval_to_std_string(tolua_S, 5,&arg3);
        if(!ok)
            return 0;
        cobj->sendVoiceAndText(arg0, arg1, arg2, arg3);
        return 0;
    }
    CCLOG("%s has wrong number of arguments: %d, was expecting %d \n", "sendVoiceAndText",argc, 4);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_ww_ChatSys_sendVoiceAndText'.",&tolua_err);
#endif

    return 0;
}
int lua_ww_ChatSys_getChannelKey(lua_State* tolua_S)
{
    int argc = 0;
    ChatSys* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"ChatSys",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (ChatSys*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_ww_ChatSys_getChannelKey'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 1) 
    {
        int arg0;

        ok &= luaval_to_int32(tolua_S, 2,(int *)&arg0);
        if(!ok)
            return 0;
        std::string ret = cobj->getChannelKey(arg0);
        tolua_pushcppstring(tolua_S,ret);
        return 1;
    }
    CCLOG("%s has wrong number of arguments: %d, was expecting %d \n", "getChannelKey",argc, 1);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_ww_ChatSys_getChannelKey'.",&tolua_err);
#endif

    return 0;
}
int lua_ww_ChatSys_getChannelId(lua_State* tolua_S)
{
    int argc = 0;
    ChatSys* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"ChatSys",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (ChatSys*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_ww_ChatSys_getChannelId'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 1) 
    {
        std::string arg0;

        ok &= luaval_to_std_string(tolua_S, 2,&arg0);
        if(!ok)
            return 0;
        int ret = cobj->getChannelId(arg0);
        tolua_pushnumber(tolua_S,(lua_Number)ret);
        return 1;
    }
    CCLOG("%s has wrong number of arguments: %d, was expecting %d \n", "getChannelId",argc, 1);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_ww_ChatSys_getChannelId'.",&tolua_err);
#endif

    return 0;
}
int lua_ww_ChatSys_setStageId(lua_State* tolua_S)
{
    int argc = 0;
    ChatSys* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"ChatSys",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (ChatSys*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_ww_ChatSys_setStageId'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 1) 
    {
        int arg0;

        ok &= luaval_to_int32(tolua_S, 2,(int *)&arg0);
        if(!ok)
            return 0;
        cobj->setStageId(arg0);
        return 0;
    }
    if (argc == 2) 
    {
        int arg0;
        bool arg1;

        ok &= luaval_to_int32(tolua_S, 2,(int *)&arg0);

        ok &= luaval_to_boolean(tolua_S, 3,&arg1);
        if(!ok)
            return 0;
        cobj->setStageId(arg0, arg1);
        return 0;
    }
    CCLOG("%s has wrong number of arguments: %d, was expecting %d \n", "setStageId",argc, 1);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_ww_ChatSys_setStageId'.",&tolua_err);
#endif

    return 0;
}
int lua_ww_ChatSys_sendMsg(lua_State* tolua_S)
{
    int argc = 0;
    ChatSys* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"ChatSys",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (ChatSys*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_ww_ChatSys_sendMsg'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 2) 
    {
        std::string arg0;
        std::string arg1;

        ok &= luaval_to_std_string(tolua_S, 2,&arg0);

        ok &= luaval_to_std_string(tolua_S, 3,&arg1);
        if(!ok)
            return 0;
        cobj->sendMsg(arg0, arg1);
        return 0;
    }
    CCLOG("%s has wrong number of arguments: %d, was expecting %d \n", "sendMsg",argc, 2);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_ww_ChatSys_sendMsg'.",&tolua_err);
#endif

    return 0;
}
int lua_ww_ChatSys_outIMGame(lua_State* tolua_S)
{
    int argc = 0;
    ChatSys* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"ChatSys",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (ChatSys*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_ww_ChatSys_outIMGame'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 0) 
    {
        if(!ok)
            return 0;
        cobj->outIMGame();
        return 0;
    }
    CCLOG("%s has wrong number of arguments: %d, was expecting %d \n", "outIMGame",argc, 0);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_ww_ChatSys_outIMGame'.",&tolua_err);
#endif

    return 0;
}
int lua_ww_ChatSys_getInstance(lua_State* tolua_S)
{
    int argc = 0;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif

#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertable(tolua_S,1,"ChatSys",0,&tolua_err)) goto tolua_lerror;
#endif

    argc = lua_gettop(tolua_S) - 1;

    if (argc == 0)
    {
        if(!ok)
            return 0;
        ChatSys* ret = ChatSys::getInstance();
        object_to_luaval<ChatSys>(tolua_S, "ChatSys",(ChatSys*)ret);
        return 1;
    }
    CCLOG("%s has wrong number of arguments: %d, was expecting %d\n ", "getInstance",argc, 0);
    return 0;
#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_ww_ChatSys_getInstance'.",&tolua_err);
#endif
    return 0;
}
static int lua_ww_ChatSys_finalize(lua_State* tolua_S)
{
    printf("luabindings: finalizing LUA object (ChatSys)");
    return 0;
}

int lua_register_ww_ChatSys(lua_State* tolua_S)
{
    tolua_usertype(tolua_S,"ChatSys");
    tolua_cclass(tolua_S,"ChatSys","ChatSys","",nullptr);

    tolua_beginmodule(tolua_S,"ChatSys");
        tolua_function(tolua_S,"soundRecording",lua_ww_ChatSys_soundRecording);
        tolua_function(tolua_S,"soundRecordingFinished",lua_ww_ChatSys_soundRecordingFinished);
        tolua_function(tolua_S,"setChannelKey",lua_ww_ChatSys_setChannelKey);
        tolua_function(tolua_S,"playAudioWithFilePath",lua_ww_ChatSys_playAudioWithFilePath);
        tolua_function(tolua_S,"resetChannelKeys",lua_ww_ChatSys_resetChannelKeys);
        tolua_function(tolua_S,"setLegionId",lua_ww_ChatSys_setLegionId);
        tolua_function(tolua_S,"login_yaya",lua_ww_ChatSys_login_yaya);
        tolua_function(tolua_S,"setGameServerId",lua_ww_ChatSys_setGameServerId);
        tolua_function(tolua_S,"loginChannel",lua_ww_ChatSys_loginChannel);
        tolua_function(tolua_S,"playAudioWithUrl",lua_ww_ChatSys_playAudioWithUrl);
        tolua_function(tolua_S,"soundRecordingStop",lua_ww_ChatSys_soundRecordingStop);
        tolua_function(tolua_S,"sendVoiceAndText",lua_ww_ChatSys_sendVoiceAndText);
        tolua_function(tolua_S,"getChannelKey",lua_ww_ChatSys_getChannelKey);
        tolua_function(tolua_S,"getChannelId",lua_ww_ChatSys_getChannelId);
        tolua_function(tolua_S,"setStageId",lua_ww_ChatSys_setStageId);
        tolua_function(tolua_S,"sendMsg",lua_ww_ChatSys_sendMsg);
        tolua_function(tolua_S,"outIMGame",lua_ww_ChatSys_outIMGame);
        tolua_function(tolua_S,"getInstance", lua_ww_ChatSys_getInstance);
    tolua_endmodule(tolua_S);
    std::string typeName = typeid(ChatSys).name();
    g_luaType[typeName] = "ChatSys";
    g_typeCast["ChatSys"] = "ChatSys";
    return 1;
}
TOLUA_API int register_all_ww(lua_State* tolua_S)
{
	tolua_open(tolua_S);
	
	tolua_module(tolua_S,"ww",0);
	tolua_beginmodule(tolua_S,"ww");

	lua_register_ww_FileIndex(tolua_S);
	lua_register_ww_Product(tolua_S);
	lua_register_ww_BuyInfo(tolua_S);
	lua_register_ww_ExtClippingNode(tolua_S);
	lua_register_ww_FileCache(tolua_S);
	lua_register_ww_FontManager(tolua_S);
	lua_register_ww_GameAnalyticsWrapper(tolua_S);
	lua_register_ww_ExtScrollView(tolua_S);
	lua_register_ww_FileIndexRow(tolua_S);
	lua_register_ww_NodeDumper(tolua_S);
	lua_register_ww_Purchase(tolua_S);
	lua_register_ww_RichTextEx(tolua_S);
	lua_register_ww_Lightning(tolua_S);
	lua_register_ww_Hosts(tolua_S);
	lua_register_ww_ChatSys(tolua_S);
	lua_register_ww_NetPacket(tolua_S);
	lua_register_ww_Random(tolua_S);
	lua_register_ww_FileManager(tolua_S);
	lua_register_ww_NetEngine(tolua_S);
	lua_register_ww_RichText(tolua_S);
	lua_register_ww_ExtButton(tolua_S);
	lua_register_ww_Products(tolua_S);
	lua_register_ww_RTTBlur(tolua_S);
	lua_register_ww_NetClient(tolua_S);
	lua_register_ww_NetHttp(tolua_S);

	tolua_endmodule(tolua_S);
	return 1;
}

