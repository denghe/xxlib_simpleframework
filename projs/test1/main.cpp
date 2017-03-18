#include "xx_bbuffer.h"
#include "xx_dict.h"
#include "xx_luahelper.h"
#include <iostream>

typedef xx::MemPool<> MP;


int main()
{
	MP mp;
	xx::String_v s(mp);
	xx::BBuffer_v bb(mp);
	xx::Dict_v<void*, uint32_t> pd(mp);
	xx::Dict_v<uint32_t, std::pair<int, xx::LuaTypes>> od(mp);

	auto L = xx::Lua_NewState(mp);
	auto rtv = luaL_dostring(L, R"##(
t = {}
t[1] = "asdfqwer"
t.i = 123
t.s = t[1]
t.t = { t, 1, 2, t[1] }
t.t.s = t.s
t.t[t] = t
)##");
	assert(!rtv);

	// 将要序列化的值放到 L 顶, 序列化后移除
	lua_getglobal(L, "t");
	auto r = xx::Lua_ToBBuffer(*pd, *bb, L, lua_gettop(L));
	assert(!r);
	lua_pop(L, 1);

	// 打印序列化数据
	bb->ToString(*s);
	std::cout << s->C_str() << std::endl;

	// 反序列化, 压入 L 顶, 放到 global t2
	r = xx::Lua_PushFromBBuffer(*od, *bb, L);
	assert(!r);
	lua_setglobal(L, "t2");
	
	// 在 lua 中校验输出
	rtv = luaL_dostring(L, R"##(
print( t2 )
print( t2[1] )
print( t2.i )
print( t2.s )
print( t2.t )
print( t2.t[1] )
print( t2.t[2] )
print( t2.t[3] )
print( t2.t[4] )
print( t2.t.s )
print( t2.t[t2] )
)##");
	assert(!rtv);
	lua_close(L);
	return 0;
}


/*


#include <stdlib.h>
#include <string.h>
#include "serialize.h"

#ifdef __cplusplus
extern "C" {
#endif


#include "lualib.h"
#include "lauxlib.h"




void
serialize_init(serialize_data* ser_data, unsigned int support_flag, const char* init_buff, size_t init_size)
{
	if(!ser_data) return;
	ser_data->size = 0;

	ser_data->data = 0;
	ser_data->error = 0;
	ser_data->error_info = 0;
	ser_data->table_ref = 0;

	ser_data->init_buff = init_buff;
	ser_data->init_size = init_size;
	ser_data->support_flag = support_flag;

	ser_data->max_size = init_size;
	ser_data->data = (char*)init_buff;
}

void
serialize_clean(serialize_data* ser_data)
{
	if(!ser_data)
	{
		return;
	}
	if(ser_data->data != ser_data->init_buff)
	{
		free(ser_data->data);
	}
	ser_data->size = 0;
	ser_data->error = 0;
	ser_data->error_info = 0;
	ser_data->table_ref = 0;
	ser_data->max_size = ser_data->init_size;
	ser_data->data = (char*)ser_data->init_buff;
}

static int check_serialize_pack(serialize_data* ser_data, lua_State *L, size_t add_size)
{
	size_t new_size = ser_data->size + add_size;

	if(ser_data->max_size > new_size)
	{
		return 0;
	}
	if(ser_data->init_size >= new_size)
	{
		if(ser_data->data != ser_data->init_buff)
		{
			memcpy((char*)ser_data->init_buff, ser_data->data, ser_data->size);
			free(ser_data->data);
		}
		ser_data->data = (char*)ser_data->init_buff;
		ser_data->max_size = ser_data->init_size;
		return 0;
	}

	new_size += 1024;

	if(ser_data->data == ser_data->init_buff)
	{
		ser_data->data = (char*)malloc(new_size);
		memcpy(ser_data->data, ser_data->init_buff, ser_data->size);
	}
	else
	{
		ser_data->data = (char*)realloc(ser_data->data, new_size);
	}

	if(!ser_data->data)
	{
		ser_data->max_size = 0;
		ser_data->size = 0;
		ser_data->error_info = (int)ser_data->size;
		return -1;
	}
	ser_data->max_size = new_size;
	return 0;
}

static char serialize_pack_bytes(serialize_data* ser_data, lua_State*L, const void* data, size_t size)
{
	if(ser_data->error)
	{
		return 0;
	}
	if(check_serialize_pack(ser_data, L, size))
	{
		ser_data->error = err_no_memory;
		return 0;
	}
	memcpy(ser_data->data + ser_data->size, data, size);
	ser_data->size += size;
	return 1;
}

static void serialize_pack_op(serialize_data* ser_data, lua_State*L, char op)
{
	serialize_pack_bytes(ser_data, L, &op, sizeof(char));
}

static void pack_nil(serialize_data* ser_data, lua_State *L, int pos)
{
	serialize_pack_op(ser_data, L, ser_nil);
}

static void pack_boolean(serialize_data* ser_data, lua_State *L, int pos)
{
	char op;
	op = lua_toboolean(L, pos) ? ser_bool_true : ser_bool_false;
	serialize_pack_op(ser_data, L, op);
}


static void pack_integer(serialize_data* ser_data, lua_State *L, int pos)
{
	int							i32;
	short int					i16;
	char						i8;
	lua_Integer abs_value;
	lua_Integer value;
	value = lua_tointeger(L, pos);
	abs_value = value;
	if(value < 0)
	{
		abs_value = -value;
	}
	if(abs_value < 128)
	{
		serialize_pack_op(ser_data, L, ser_int8);
		i8 = (char)value;
		serialize_pack_bytes(ser_data, L, &i8, sizeof(i8));
	}
	else if(abs_value < 32768)
	{
		serialize_pack_op(ser_data, L, ser_int16);
		i16 = (short int)value;
		serialize_pack_bytes(ser_data, L, &i16, sizeof(i16));
	}
	else if(abs_value <= 2147483647)
	{
		serialize_pack_op(ser_data, L, ser_int32);
		i32 = (int)value;
		serialize_pack_bytes(ser_data, L, &i32, sizeof(i32));
	}
	else
	{
		serialize_pack_op(ser_data, L, ser_int64);
		serialize_pack_bytes(ser_data, L, &value, sizeof(value));
	}
}

static void pack_double(serialize_data* ser_data, lua_State *L, int pos)
{
	lua_Number number = lua_tonumber(L, pos);
	serialize_pack_op(ser_data, L, ser_double);
	serialize_pack_bytes(ser_data, L, &number, sizeof(lua_Number));
}


static void pack_string(serialize_data* ser_data, lua_State *L, int pos)
{
	size_t size;
	unsigned short int u16;
	unsigned char u8;

	const char* data = lua_tolstring(L, pos, &size);
	if(!data)
	{
		ser_data->error = err_null_string;
		return;
	}
	if(size < (1UL << 7))
	{
		serialize_pack_op(ser_data, L, ser_string8);
		u8 = (unsigned char)size;
		serialize_pack_bytes(ser_data, L, &u8, sizeof(u8));
	}
	else if(size < (1UL << 15))
	{
		serialize_pack_op(ser_data, L, ser_string16);
		u16 = (unsigned short int)size;
		serialize_pack_bytes(ser_data, L, &u16, sizeof(u16));
	}
	else
	{
		serialize_pack_op(ser_data, L, ser_string32);
		serialize_pack_bytes(ser_data, L, &size, sizeof(size));
	}
	serialize_pack_bytes(ser_data, L, data, size);
}

static void pack_userdata(serialize_data* ser_data, lua_State *L,  int pos)
{
	void* ud;
	const char* data;
	size_t size;
	lua_Integer ser_size;
	unsigned char u8;
	data = 0;

	if(ser_data->support_flag & USER_DATA_FLAG)
	{
		ud = lua_touserdata(L, pos);
		if(ud && lua_getmetatable(L, pos))
		{
			lua_getfield(L, -1, "__serialize");
			if(!lua_isnil(L, -1))
			{
				ser_size = luaL_checkinteger(L, -1) ;
				lua_getfield(L, -2, "__name");
				if(!lua_isnil(L, -1))
				{
					data = lua_tolstring(L, -1, &size);
					if(data)
					{
						serialize_pack_op(ser_data, L, ser_user_data);
						u8 = (unsigned char)size + 1;
						serialize_pack_bytes(ser_data, L, &u8, sizeof(u8));
						serialize_pack_bytes(ser_data, L, data, u8);
						u8 = (unsigned char)ser_size;
						serialize_pack_bytes(ser_data, L, &u8, sizeof(u8));
						serialize_pack_bytes(ser_data, L, (void*)ud, u8);
					}
				}
				lua_pop(L, 1);
			}
			lua_pop(L, 2);
		}
	}
	if(!data)
	{
		serialize_pack_op(ser_data, L, ser_nil);
	}
}

static void pack_lightuserdata(serialize_data* ser_data, lua_State *L,  int pos)
{
	void* ud;
	if(ser_data->support_flag & USER_DATA_FLAG)
	{
		ud = lua_touserdata(L, pos);
		serialize_pack_op(ser_data, L, ser_lightuser_data);
		serialize_pack_bytes(ser_data, L, (const void*)&ud, sizeof(void*));
	}
	else
	{
		serialize_pack_op(ser_data, L, ser_nil);
	}
}

static void pack_obj(serialize_data* ser_data, lua_State *L, int pos, int table_ref_pos);

static void pack_table(serialize_data* ser_data, lua_State * L, int pos, int table_ref_pos)
{
	int type;
	int index;

	// check the table is in table_ref
	lua_checkstack(L, 1);
	lua_pushvalue(L, pos);
	lua_gettable(L, table_ref_pos);
	type = lua_type(L, -1);
	if(lua_isinteger(L, -1))
	{
		// exist the table
		index = (int)lua_tointeger(L, -1);
		lua_pop(L, 1);
		serialize_pack_op(ser_data, L, ser_table_ref);
		serialize_pack_bytes(ser_data, L, &index, sizeof(index));
	}
	else if(type == LUA_TNIL)
	{
		// no exist the table
		lua_pop(L, 1);
		lua_checkstack(L, 3);
		// record to table_ref[t] = ++table_ref_index
		lua_pushvalue(L, pos);
		ser_data->table_ref += 1;
		lua_pushinteger(L, ser_data->table_ref);
		lua_settable(L, table_ref_pos);

		lua_pushnil(L);
		if(pos < 0)
		{
			pos -= 1;
		}
		serialize_pack_op(ser_data, L, ser_table_begin);
		while(lua_next(L, pos) != 0)
		{
			pack_obj(ser_data, L, -2, table_ref_pos);
			pack_obj(ser_data, L, -1, table_ref_pos);
			lua_pop(L, 1);
		}
		serialize_pack_op(ser_data, L, ser_table_end);
	}
	else
	{
		lua_pop(L, 1);
		ser_data->error = err_ref_table_nofit_type;
		ser_data->error_info = (short)type;
	}
}

static void pack_obj(serialize_data* ser_data, lua_State *L, int pos, int table_ref_pos)
{
	if(!ser_data || ser_data->error)
	{
		return;
	}
	int type = lua_type(L, pos);
	switch(type)
	{
	case LUA_TNIL:
		pack_nil(ser_data, L, pos);
		break;
	case LUA_TBOOLEAN:
		pack_boolean(ser_data, L, pos);
		break;
	case LUA_TNUMBER:
		if( lua_isinteger(L, pos) )pack_integer(ser_data, L, pos);
		else pack_double(ser_data, L, pos);
		break;
	case LUA_TSTRING:
		pack_string(ser_data, L, pos);
		break;
	case LUA_TTABLE:
		pack_table(ser_data, L, pos, table_ref_pos);
		break;
	case LUA_TUSERDATA:
		pack_userdata(ser_data, L, pos);
		break;
	case LUA_TLIGHTUSERDATA:
		pack_lightuserdata(ser_data, L, pos);
		break;
	default:
		pack_nil(ser_data, L, pos);
		break;
	}
}

int
serialize_pack(serialize_data* ser_data, lua_State* L, int argStartIndex)
{
	int i, top;

	serialize_clean(ser_data);

	top = lua_gettop(L);
	lua_checkstack(L, 1);
	// a table to check cycle ref
	lua_newtable(L);
	for(i = argStartIndex; i <= top; ++i)
	{
		pack_obj(ser_data, L, i, top + 1);
	}
	return ser_data->error;
}


static void unpack_nil(deserialize_data* deser_data, lua_State *L)
{
	lua_pushnil(L);
}

static void unpack_integer8(deserialize_data* deser_data, lua_State *L, size_t* bytes_pos)
{
	char dvalue;
	if(deser_data->size < (*bytes_pos + sizeof(dvalue)))
	{
		deser_data->error = err_unpack_int8_no_buff;
		deser_data->error_info = (short)*bytes_pos;
		return;
	}
	memcpy(&dvalue, deser_data->data + (*bytes_pos), sizeof(dvalue));
	lua_pushinteger(L, (lua_Integer)dvalue);
	*bytes_pos += sizeof(dvalue);
}

static void unpack_integer16(deserialize_data* deser_data, lua_State *L, size_t* bytes_pos)
{
	short int dvalue;
	if(deser_data->size < (*bytes_pos + sizeof(dvalue)))
	{
		deser_data->error = err_unpack_int16_no_buff;
		deser_data->error_info = (short)*bytes_pos;
		return;
	}
	memcpy(&dvalue, deser_data->data + (*bytes_pos), sizeof(dvalue));
	lua_pushinteger(L, (lua_Integer)dvalue);
	*bytes_pos += sizeof(dvalue);
}
static void unpack_integer32(deserialize_data* deser_data, lua_State *L, size_t* bytes_pos)
{
	int dvalue;
	if(deser_data->size < (*bytes_pos + sizeof(dvalue)))
	{
		deser_data->error = err_unpack_int32_no_buff;
		deser_data->error_info = (short)*bytes_pos;
		return;
	}
	memcpy(&dvalue, deser_data->data + (*bytes_pos), sizeof(dvalue));
	lua_pushinteger(L, (lua_Integer)dvalue);
	*bytes_pos += sizeof(dvalue);
}

static void unpack_integer64(deserialize_data* deser_data, lua_State *L, size_t* bytes_pos)
{
	lua_Integer dvalue;
	if(deser_data->size < (*bytes_pos + sizeof(dvalue)))
	{
		deser_data->error = err_unpack_int64_no_buff;
		return;
	}
	memcpy(&dvalue, deser_data->data + (*bytes_pos), sizeof(dvalue));
	lua_pushinteger(L, dvalue);
	*bytes_pos += sizeof(dvalue);
}

static void unpack_double(deserialize_data* deser_data, lua_State *L, size_t* bytes_pos)
{
	lua_Number dvalue;
	if(deser_data->size < (*bytes_pos + sizeof(lua_Number)))
	{
		deser_data->error = err_unpack_double_no_buff;
		deser_data->error_info = (short)*bytes_pos;
		return;
	}
	memcpy(&dvalue, deser_data->data + (*bytes_pos), sizeof(lua_Number));
	lua_pushnumber(L, dvalue);
	*bytes_pos += sizeof(lua_Number);
}

static void unpack_string8(deserialize_data* deser_data, lua_State *L, size_t* bytes_pos)
{
	unsigned char size;
	if(deser_data->size < (*bytes_pos + sizeof(size)))
	{
		deser_data->error = err_unpack_string8_no_size_buff;
		deser_data->error_info = (short)*bytes_pos;
		return;
	}
	memcpy(&size, deser_data->data + (*bytes_pos), sizeof(size));
	*bytes_pos += sizeof(size);
	if(deser_data->size < (*bytes_pos + (size_t)size))
	{
		deser_data->error = err_unpack_string8_no_data_buff;
		deser_data->error_info = (short)*bytes_pos;
		return;
	}
	lua_pushlstring(L, deser_data->data + (*bytes_pos), (size_t)size);
	*bytes_pos += (size_t) size;
}

static void unpack_string16(deserialize_data* deser_data, lua_State *L, size_t* bytes_pos)
{
	unsigned short int size;
	if(deser_data->size < (*bytes_pos + sizeof(size)))
	{
		deser_data->error = err_unpack_string16_no_size_buff;
		deser_data->error_info = (short)*bytes_pos;
		return;
	}

	memcpy(&size, deser_data->data + (*bytes_pos), sizeof(size));
	*bytes_pos += sizeof(size);
	if(deser_data->size < (*bytes_pos + (size_t)size))
	{
		deser_data->error = err_unpack_string16_no_data_buff;
		deser_data->error_info = (short)*bytes_pos;
		return;
	}
	lua_pushlstring(L, deser_data->data + (*bytes_pos), (size_t)size);
	*bytes_pos += (size_t) size;
}

static void unpack_string32(deserialize_data* deser_data, lua_State *L, size_t* bytes_pos)
{
	unsigned int size;
	if(deser_data->size < (*bytes_pos + sizeof(size)))
	{
		deser_data->error = err_unpack_string32_no_size_buff;
		deser_data->error_info = (short)*bytes_pos;
		return;
	}
	memcpy(&size, deser_data->data + (*bytes_pos), sizeof(size));
	*bytes_pos += sizeof(size);
	if(deser_data->size < (*bytes_pos + (size_t)size))
	{
		deser_data->error = err_unpack_string32_no_data_buff;
		deser_data->error_info = (short)*bytes_pos;
		return;
	}
	lua_pushlstring(L, deser_data->data + (*bytes_pos), (size_t)size);
	*bytes_pos += (size_t) size;
}

static void unpack_boolean(deserialize_data* deser_data, lua_State *L, int bvalue)
{
	lua_pushboolean(L, bvalue);
}


static void unpack_table_ref(deserialize_data* deser_data, lua_State *L, size_t* bytes_pos, int table_ref_pos)
{
	int index;
	int type;
	if(deser_data->size < (*bytes_pos + sizeof(index)))
	{
		deser_data->error = err_unpack_table_ref_no_index;
		deser_data->error_info = (short)*bytes_pos;
		return;
	}
	memcpy(&index, deser_data->data + (*bytes_pos), sizeof(index));
	*bytes_pos += sizeof(index);
	// get the table
	lua_rawgeti(L, table_ref_pos, index);
	type = lua_type(L, -1);
	if(type != LUA_TTABLE)
	{
		deser_data->error = err_unpack_table_ref_no_table;
		deser_data->error_info = (short)index;
	}
}

static int  unpack_obj(deserialize_data* deser_data, lua_State *L, size_t* bytes_pos, int table_ref_pos);

static void unpack_table(deserialize_data* deser_data, lua_State *L, size_t* bytes_pos, int table_ref_pos)
{
	int key_type = 0;
	int value_type = 0;
	lua_checkstack(L, 2);
	lua_newtable(L);
	lua_pushvalue(L, -1);
	lua_rawseti(L, table_ref_pos, lua_rawlen (L, table_ref_pos) + 1);

	key_type =unpack_obj(deser_data, L, bytes_pos, table_ref_pos);
	while(!deser_data->error && key_type != ser_table_end)
	{
		if(deser_data->error)
		{
			break;
		}
		if(key_type == ser_none)
		{
			deser_data->error = err_unpack_table_no_key_buff;
			break;
		}
		value_type = unpack_obj(deser_data, L, bytes_pos, table_ref_pos);
		if(deser_data->error)
		{
			break;
		}
		if(value_type == ser_none || value_type == ser_table_end)
		{
			deser_data->error = err_unpack_table_no_value_buff;
			break;
		}
		lua_rawset(L, -3);
		key_type =unpack_obj(deser_data, L, bytes_pos, table_ref_pos);
	}
}


static void unpack_userdata(deserialize_data* deser_data, lua_State *L, size_t* bytes_pos)
{
	void* ud;
	unsigned char u8;
	if(deser_data->size < (*bytes_pos + sizeof(ud)))
	{
		deser_data->error = err_unpack_userdata_no_buff;
		return;
	}

	memcpy(&u8, deser_data->data + (*bytes_pos), sizeof(u8));
	*bytes_pos += sizeof(u8);
	luaL_getmetatable(L, deser_data->data + (*bytes_pos));
	*bytes_pos += (size_t)u8;
	memcpy(&u8, deser_data->data + (*bytes_pos), sizeof(u8));
	*bytes_pos += sizeof(u8);

	if(!lua_isnil(L, -1))
	{
		ud = lua_newuserdata(L, (size_t)u8);
		if(ud)
		{
			lua_pushnil(L);
			memcpy(ud, deser_data->data + (*bytes_pos), (size_t)u8);
			lua_copy(L, -3, -1);
			lua_remove(L, -3);
			lua_setmetatable(L, -2);
		}
		else
		{
			lua_pop(L, 2);
			lua_pushnil(L);
		}
	}
	*bytes_pos += (size_t)u8;
}

static void unpack_lightuserdata(deserialize_data* deser_data, lua_State *L, size_t* bytes_pos)
{
	void* ud;
	if(deser_data->size < (*bytes_pos + sizeof(ud)))
	{
		deser_data->error = err_unpack_lightuserdata_no_buff;
		return;
	}
	memcpy(&ud, deser_data->data + (*bytes_pos), sizeof(ud));
	lua_pushlightuserdata(L, ud);
	*bytes_pos += sizeof(ud);
}


static int  unpack_obj(deserialize_data* deser_data, lua_State *L, size_t* bytes_pos, int table_ref_pos)
{
	if(!deser_data || deser_data->error)
	{
		return ser_none;
	}
	if((*bytes_pos) >= deser_data->size)
	{
		return ser_none;
	}
	lua_checkstack(L, 1);
	int ser_type = (int)deser_data->data[(*bytes_pos)];
	*bytes_pos += 1;
	switch(ser_type)
	{
	case ser_nil:
		unpack_nil(deser_data, L);
		break;
	case ser_int8:
		unpack_integer8(deser_data, L, bytes_pos);
		break;
	case ser_int16:
		unpack_integer16(deser_data, L, bytes_pos);
		break;
	case ser_int32:
		unpack_integer32(deser_data, L, bytes_pos);
		break;
	case ser_int64:
		unpack_integer64(deser_data, L, bytes_pos);
		break;
	case ser_double:
		unpack_double(deser_data, L, bytes_pos);
		break;
	case ser_string8:
		unpack_string8(deser_data, L, bytes_pos);
		break;
	case ser_string16:
		unpack_string16(deser_data, L, bytes_pos);
		break;
	case ser_string32:
		unpack_string32(deser_data, L, bytes_pos);
		break;
	case ser_bool_false:
		unpack_boolean(deser_data, L, 0);
		break;
	case ser_bool_true:
		unpack_boolean(deser_data, L, 1);
		break;
	case ser_table_ref:
		unpack_table_ref(deser_data, L, bytes_pos, table_ref_pos);
		break;
	case ser_table_begin:
		unpack_table(deser_data, L, bytes_pos, table_ref_pos);
		break;
	case ser_table_end:
		break;
	case ser_user_data:
		unpack_userdata(deser_data, L, bytes_pos);
		break;
	case ser_lightuser_data:
		unpack_lightuserdata(deser_data, L, bytes_pos);
		break;
	default:
		deser_data->error = err_unpack_error_op_type;
		deser_data->error_info = (short)ser_type;
		break;
	}
	return ser_type;
}


int
deserialize_unpack(deserialize_data* deser_data, lua_State* L, const char* data, size_t size, int resCount)
{
	int count = 0;
	int table_ref_pos;
	size_t bytes_pos = 0;
	int res;

	deser_data->data = (char*)data;
	deser_data->size = size;
	deser_data->error = err_none;
	deser_data->error_info = 0;


	// table_ref {table1, table2, table3, table4 } at top 1
	lua_checkstack(L, 1);
	lua_newtable(L);
	table_ref_pos = lua_gettop(L);
	while(bytes_pos < deser_data->size)
	{
		res = unpack_obj(deser_data, L, &bytes_pos, table_ref_pos);
		if(deser_data->error)
		{
			count = 0;
			break;
		}
		if(res == ser_none || res == ser_table_end)
		{
			deser_data->error = err_unpack_error;
			deser_data->error_info = res;;
			break;
		}
		++count;
		if(resCount >0 && count >= resCount)
		{
			break;
		}
	}

	if(deser_data->error)
	{
		return 0;
	}
	lua_remove(L, table_ref_pos);
	return count;
}


#ifdef __cplusplus
}
#endif

*/