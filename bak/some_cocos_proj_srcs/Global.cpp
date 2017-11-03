#include "Precompile.h"
/* curl */
#include <curl/curl.h>
#include <curl/easy.h>

lua_State * Global::L = nullptr;
LuaStack * Global::LS = nullptr;
string Global::WritablePath;
string Global::ResourcePath;
vector<string> Global::SearchPaths;

void Global::init( LuaStack * LS, lua_State * L, string && writablePath, string && resourcePath, vector<string> && SearchPaths )
{
    Global::L = L;
    Global::LS = LS;
    Global::WritablePath = ( writablePath );
    Global::ResourcePath = ( resourcePath );
    Global::SearchPaths  = ( SearchPaths );

    curl_global_init( CURL_GLOBAL_ALL );
    FileCache::initInstance();
    NetEngine::initInstance();
    NetHttp::initInstance();
    CommandQueue::initInstance();
    FileManager::initInstance();
    FontManager::initInstance();
    //...
    Destructor::initInstance();
    Hosts::initInstance();
    Purchase::initInstance();
    GameAnalyticsWrapper::initInstance();
	ChatSys::initInstance();

#ifdef CHANNEL_UC
    UCSdk::initInstance();
#endif

#ifdef CHANNEL_VIVO
    VivoSdk::initInstance();
#endif

}

// 上下两个函数中的语句顺序相反

void Global::free()
{
#ifdef CHANNEL_VIVO
    VivoSdk::freeInstance();
#endif

#ifdef CHANNEL_UC
    UCSdk::freeInstance();
#endif

    ChatSys::freeInstance();
    GameAnalyticsWrapper::freeInstance();
    Purchase::freeInstance();
    // cocos2d::Node::freeShader();
    Hosts::freeInstance();
    Destructor::freeInstance();
    //...
    FontManager::freeInstance();
    FileManager::freeInstance();
    CommandQueue::freeInstance();
    NetHttp::freeInstance();
    NetEngine::freeInstance();
    FileCache::freeInstance();
    curl_global_cleanup();

    L = nullptr;
    LS = nullptr;
}


