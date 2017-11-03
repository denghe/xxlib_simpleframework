#ifndef _PRECOMPILE_H_
#define _PRECOMPILE_H_

/* c2dx */
#include "cocos2d.h"

/* c2dx old gui extensions */
#include "extensions/cocos-ext.h"

/* c2dx network extensions */
#include "network/HttpClient.h"
#include "network/HttpRequest.h"
#include "network/HttpResponse.h"

/* c2dx lua engine */
#include "CCLuaEngine.h"

/* c2dx audio engine */
#include "SimpleAudioEngine.h"

/* c2dx spine extension */
#include "spine/spine-cocos2dx.h"

/* sqlite */
#include "sqlite3.h"

/* unzip */
#include <unzip/unzip.h>

// c2dx lua
extern "C"
{
    #include "lua.h"
    #include "lualib.h"
    #include "lauxlib.h"
}
#include "lua/tolua/tolua++.h"
#include "tolua_fix.h"

/* project entry */
#include "AppDelegate.h"

/************************************************************************/
/* current project headers begin here                                   */
/************************************************************************/

// for easy use
using namespace std;
using namespace ::cocos2d;
using namespace ::cocos2d::extension;
using namespace CocosDenshion;

// Hardware, Compiler, STL defines, includes
#include "PreIncludes.h"

// for easy use defines
#include "Defines.h"

// all enums is here
#include "Enums.h"

// all singleton instance, init func is here
#include "Global.h"

/************************/
/* following misc utils */
/************************/

// encrypt
#include "md5.h"

// queue like
#include "ThreadSafeQueue.h"
#include "ThreadSafeQueue.hpp"
#include "CommandQueue.h"
#include "LRUCache.h"
#include "FileCache.h"
#include "Destructor.h"
#include "GenericEvent.h"

// utils funcs
#include "Utils.h"

// for performance test
#include "Stopwatch.h"

// sqlite helper
#include "Sqlite.h"

// lua helper funcs: push, executeFunction
#include "LuaHelper.h"

// dump current scene or node to create code string
#include "NodeDumper.h"

// Game Analytics
#include "GameAnalyticsWrapper.h"

/**********************/
/* custom ui controls */
/**********************/

// bg, fg node simple button
#include "ExtButton.h"

// rectangle cut node
#include "ExtClippingNode.h"

// my scroll view
#include "ExtScrollView.h"

// font solution manager
#include "FontManager.h"

// RichText
#include "RichText.h"                                   // todo: refactor
#include "RichTextHelper.h"
#include "RichTextEx.h"

/***********/
/* effects */
/***********/

#include "Lightning.h"
#include "RTTBlur.h"

/************/
/* Channels */
/************/

#ifdef CHANNEL_UC
#include "ucsdk/ww_ucsdk.h"
#include "ucsdk/lua_wwuc_auto.hpp"
#include "ucsdk/lua_wwuc_manual.hpp"
#endif

#ifdef CHANNEL_VIVO
#include "vivo/VivoSdk.h"
#include "vivo/lua_wwvivo_auto.hpp"
#include "vivo/lua_wwvivo_manual.hpp"
#endif

/**********/
/* others */
/**********/

// lua bind
#include "LuaExtensions.h"          // for cjson
#include "lua_ww_auto.hpp"
#include "lua_ww_manual.hpp"

// tcp network etc.
#include "BufferHelper.h"
#include "NetPacket.h"
#include "ListBuffer.h"
#include "NetUtils.h"
#include "NetEngine.h"
#include "NetClient.h"

// http request
#include "NetHttp.h"

// file etc.
#include "FileIndex.h"
#include "FileDownloader.h"
#include "FileManager.h"

// hosts functions
#include "Hosts.h"

// in app purchase
#include "Purchase.h"

#include "Random.h"

#include "ChatSys.h"

#endif
