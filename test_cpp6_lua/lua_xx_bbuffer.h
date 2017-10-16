#pragma once
#include "xx_bytesutils.h"
namespace xx::Lua_BBuffer
{
	// todo: 修复 xx_luahelper.h
	// todo: lua_State 通过自定义 alloc 的方式将 mempool 指针传入
	// todo: 将下面这些函数做成 metatable, 映射到全局, 叫 BBuffer. 提供 new 和 gc 接口函数
	// todo: BBuffer.new() 将通过 lua 定位到 mp, 从 lua 请求 userdata 空间, 创建 xx::BBuffer_v 对象, 将 BBuffer 设成元表

	// todo: new, gc

	inline static int write_i8(lua_State* L)
	{
		return 0;
	}
	inline static int write_i16(lua_State* L)
	{
		return 0;
	}
	inline static int write_i32(lua_State* L)
	{
		return 0;
	}
	inline static int write_i64(lua_State* L)
	{
		return 0;
	}

	inline static int write_u8(lua_State* L)
	{
		return 0;
	}
	inline static int write_u16(lua_State* L)
	{
		return 0;
	}
	inline static int write_u32(lua_State* L)
	{
		return 0;
	}
	inline static int write_u64(lua_State* L)
	{
		return 0;
	}

	inline static int write_float(lua_State* L)
	{
		return 0;
	}
	inline static int write_double(lua_State* L)
	{
		return 0;
	}

	inline static int write_string(lua_State* L)
	{
		return 0;
	}

};
