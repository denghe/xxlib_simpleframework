#ifndef __LUA_ANYSDK_BINDINGS_H_
#define __LUA_ANYSDK_BINDINGS_H_

#ifdef __cplusplus
extern "C" {
#endif
#include "tolua++.h"
#ifdef __cplusplus
}
#endif

#define TOLUA_DISABLE_tolua_anysdk_AgentManager_loadALLPlugin00
#define TOLUA_DISABLE_tolua_anysdk_AgentManager_unloadALLPlugin00


TOLUA_API int tolua_anysdk_open(lua_State* tolua_S);

#endif // __LUACOCOSTUDIO_H_
