#pragma once

#ifndef __cplusplus
#error need cpp compiler
#endif

#include "lua_mempool.h"
extern "C"
{
#include "../lua/lua.h"
#include "../lua/lauxlib.h"
#include "../lua/lualib.h"
}

void SetUtf8Console();

