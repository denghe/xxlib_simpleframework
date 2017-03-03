#pragma once
#include <cassert>
#include <tuple>
#include <type_traits>
#include <utility>
#include <array>
#include <initializer_list>
#include "xx_mempool.h"
#include "lua.hpp"

#include <iostream>

namespace xx
{
	// todo: 考虑用宏来将各种检查进行分级. 视情况开启.

	// 可放入 LUA 的数据类型有: float, double, int64, 各式 string, 以及 T, T*
	// 其中 T 又分为 一般结构体 以及 MPtr<T> ( T 为 MPObject 派生类 )
	// T* 分为一般指针 或 MPObject* 派生类指针
	// String* 空指针于 lua 中当前用 nil 来表达

	/************************************************************************************/
	// Lua_Container / Lua_SetGlobalContainer
	/************************************************************************************/

	// 在此设置全局容器名
	constexpr auto Lua_Container = "objs";

	// 创建全局容器
	inline void Lua_SetGlobalContainer(lua_State* L)
	{
		lua_newtable(L);										// t
		lua_setglobal(L, Lua_Container);						//
	}



	/************************************************************************************/
	// Lua_PushMetatables
	/************************************************************************************/

	// 批量创建出所有元表放置到 L( 一直占据 stack 空间, 以便于从根 L  )
	template<typename MP, typename Tuple, size_t... Indexs>
	void Lua_PushMetatables(lua_State* L, std::index_sequence<Indexs...> const& idxs)
	{
		// isMPObject = true: idx 为正. false: idx 为负, idx 用来判断父子关系
		auto func = [&](bool isMPObject, int idx)
		{
			if (idx == 0) return;								// MPObject 类型本身不注册 mt

			lua_newtable(L);									// mt
			lua_pushinteger(L, isMPObject ? idx : -idx);		// mt, idx/-idx
			lua_rawseti(L, -2, 1);								// mt
			lua_pushstring(L, "__index");						// mt, __index
			lua_pushvalue(L, -2);								// mt, __index, mt
			lua_rawset(L, -3);									// mt
		};
		std::initializer_list<int>{ (
			func(IsMPObject<std::tuple_element_t<Indexs, Tuple>>::value, Indexs)
			, 0)... };
	}

	// 批量创建出所有元表放置到 L( 一直占据 stack 空间, 以便于从根 L  )
	template<typename MP>
	inline void Lua_PushMetatables(lua_State* L)
	{
		assert(0 == lua_gettop(L));
		Lua_PushMetatables<MP, typename MP::Tuple>(L, std::make_index_sequence<MP::typesSize>());
	}



	/************************************************************************************/
	// Lua_NewState
	/************************************************************************************/

	// 创建并返回一个 lua_State*, 以内存池为内存分配方式, 默认 openLibs
	template<typename MP>
	inline lua_State* Lua_NewState(MP& mp, bool openLibs = true, bool setGlobalContainer = true, bool pushMetatables = true)
	{
		auto L = lua_newstate([](void *ud, void *ptr, size_t osize, size_t nsize)
		{
			return ((MemPoolBase*)ud)->Realloc(ptr, nsize, osize);
		}, &mp);
		// 之后可以用 lua_getallocf 函数来得到 mp

		if (openLibs) luaL_openlibs(L);

		if (setGlobalContainer)
		{
			Lua_SetGlobalContainer(L);
		}

		if (pushMetatables)
		{
			Lua_PushMetatables<MP>(L);
		}

		return L;
	}


	/************************************************************************************/
	// Lua_CopyItemsFromParentMetatable
	/************************************************************************************/

	// 从指定 index 处 clone 元素到 -1 表
	template<typename MP, typename PT>
	inline void Lua_CopyItemsFromParentMetatable(lua_State* L)
	{
		lua_pushnil(L);
		while (lua_next(L, TupleIndexOf<PT, typename MP::Tuple>::value) != 0)
		{
			lua_pushvalue(L, -2);
			lua_insert(L, -2);
			lua_rawset(L, -4);
		}
	}


	/************************************************************************************/
	// Lua_RegisterCoroutine
	/************************************************************************************/

	// 为状态机( 基类是 MPObject )建协程放至 LUA_REGISTRYINDEX 并返回指针
	// 主要供状态机构造函数处调用. o 传入 this
	inline lua_State* Lua_RegisterCoroutine(lua_State* L, void* key)
	{
		auto co = lua_newthread(L);							// key, co
		lua_rawsetp(L, LUA_REGISTRYINDEX, key);				//
		return co;
	}



	/************************************************************************************/
	// Lua_ReleaseCoroutine
	/************************************************************************************/

	// 移除一个协程 ( 位于 LUA_REGISTRYINDEX 表 )
	inline void Lua_UnregisterCoroutine(lua_State* L, void* key)
	{
		if (!L) return;										// 如果是 scene 析构( L 已死 )就会导致这个情况
		lua_pushnil(L);
		lua_rawsetp(L, LUA_REGISTRYINDEX, key);
	}


	/************************************************************************************/
	// Lua_Resume
	/************************************************************************************/

	inline int Lua_Resume(lua_State* co, std::string& err)
	{
		assert(co);
		int status = lua_resume(co, nullptr, 0);
		if (status == LUA_YIELD)
		{
			return 0;	// todo: 暂存函数的返回值? 
		}
		else if (status == LUA_ERRRUN && lua_isstring(co, -1))
		{
			err = lua_tostring(co, -1);
			lua_pop(co, -1);
			return -1;
		}
		else
		{
			return 1;	// LUA_OK
		}
	}

	/************************************************************************************/
	// LuaError
	/************************************************************************************/

	// 简化出错函数调用. 用法: return LuaError("...");
	inline int Lua_Error(lua_State* L, char const* errmsg)
	{
		lua_pushstring(L, errmsg);
		lua_error(L);
		return 0;
	}


	/************************************************************************************/
	// Set XXXXXXX Nil
	/************************************************************************************/

	// 从全局容器中清除 key 项
	inline static void SetContainerNil(lua_State* L, lua_Integer const& key)
	{
		if (!L) return;
		auto rtv = lua_getglobal(L, Lua_Container);			// objs
		assert(rtv == LUA_TTABLE);
		lua_pushnil(L);										// objs, nil
		lua_rawseti(L, -2, key);							// objs
		lua_pop(L, 1);										//
	}

	// 从全局清除 key 项
	inline static void SetGlobalNil(lua_State* L, char const* key)
	{
		if (!L) return;
		lua_pushnil(L);
		lua_setglobal(L, key);
	}




	/************************************************************************************/
	// Lua<T, cond>
	/************************************************************************************/

	template<typename MP, typename T, typename ENABLE = void>
	struct Lua
	{
		// 将 T 类型对应的 mt 压栈( 用于填充 mt 时放至栈顶 或 Bind 时压入 upvalue )( L 得是根 )
		static void PushMetatable(lua_State* L);

		// 将 v 压入栈顶( L 得是根 )
		static void Push(lua_State* L, T const& v);

		// 从 idx 读出数据填到 v
		static void To(MP& mp, lua_State* L, T& v, int idx);

		// 试从 idx 读出数据填到 v. 成功返回 true
		static bool TryTo(MP& mp, lua_State* L, T& v, int idx);

		// 往全局容器填一个值( L 得是根 )
		static void SetContainer(lua_State* L, lua_Integer const& key, T const& v);

		// 往全局填一个值( L 得是根 )
		static void SetGlobal(lua_State* L, char const* const& key, T const& v);

		// 将 v 压入栈顶( L 非根. 针对返回值 结构体 T, 从 upvalue 拿 mt )
		static void PushRtv(lua_State* L, T const& v);
	};

	// void
	template<typename MP>
	struct Lua<MP, void, void>
	{
		static inline void PushMetatable(lua_State* L)
		{
			lua_pushnil(L);
		}
	};

	// string
	template<typename MP>
	struct Lua<MP, std::string, void>
	{
		static inline void PushMetatable(lua_State* L)
		{
			lua_pushnil(L);
		}
		static inline void Push(lua_State* L, std::string const& v)
		{
			lua_pushlstring(L, v.data(), v.size());
		}
		static inline void To(MP& mp, lua_State* L, std::string& v, int idx)
		{
			size_t len;
			auto s = lua_tolstring(L, idx, &len);
			v.assign(s, len);
		}
		static inline bool TryTo(MP& mp, lua_State* L, std::string& v, int idx)
		{
			if (!lua_isstring(L, idx)) return false;
			To(mp, L, v, idx);
			return true;
		}
		static inline void SetContainer(lua_State* L, lua_Integer const& key, std::string const& v)
		{
			auto rtv = lua_getglobal(L, Lua_Container);						// objs
			assert(rtv == LUA_TTABLE);
			Push(L, v);
			lua_pop(L, 1);													//
		}
		static inline void SetGlobal(lua_State* L, char const* const& key, std::string const& v)
		{
			Push(L, v);
			lua_pop(L, 1);													//
		}
		static inline void PushRtv(lua_State* L, std::string const& v)
		{
			lua_pushlstring(L, v.data(), v.size());
		}
	};

	// xx::String*
	template<typename MP>
	struct Lua<MP, String*, void>
	{
		static inline void PushMetatable(lua_State* L)
		{
			lua_pushnil(L);
		}
		static inline void Push(lua_State* L, String* const& v)
		{
			if (v) lua_pushlstring(L, v->buf, v->dataLen);
			else lua_pushnil(L);
			
		}
		static inline void To(MP& mp, lua_State* L, String*& v, int idx)
		{
			if (v) v->Release();
			if (lua_isnil(L, idx))
			{
				v = nullptr;
				return;
			}
			size_t len;
			auto s = lua_tolstring(L, idx, &len);
			v = mp.Create<String>(s, len);
		}
		static inline bool TryTo(MP& mp, lua_State* L, String*& v, int idx)
		{
			if (!lua_isstring(L, idx) && !lua_isnil(L, idx)) return false;
			To(mp, L, v, idx);
			return true;
		}
		static inline void SetContainer(lua_State* L, lua_Integer const& key, String* const& v)
		{
			auto rtv = lua_getglobal(L, Lua_Container);						// objs
			assert(rtv == LUA_TTABLE);
			Push(L, v);
			lua_pop(L, 1);													//
		}
		static inline void SetGlobal(lua_State* L, char const* const& key, String* const& v)
		{
			Push(L, v);
			lua_pop(L, 1);													//
		}
		static inline void PushRtv(lua_State* L, String* const& v)
		{
			if(v) lua_pushlstring(L, v->buf, v->dataLen);
			else lua_pushnil(L);
		}
	};

	// char*
	template<typename MP>
	struct Lua<MP, char const*, void>
	{
		static inline void PushMetatable(lua_State* L)
		{
			lua_pushnil(L);
		}
		static inline void Push(lua_State* L, char const* const& v)
		{
			if(v) lua_pushstring(L, v);
			else lua_pushnil(L);
		}
		static inline void To(MP& mp, lua_State* L, char const*& v, int idx)
		{
			if (lua_isnil(L, idx)) v = nullptr;
			else v = lua_tostring(L, idx);
		}
		static inline bool TryTo(MP& mp, lua_State* L, char const*& v, int idx)
		{
			if (!lua_isstring(L, idx) && !lua_isnil(L, idx)) return false;
			To(mp, L, v, idx);
			return true;
		}
		static inline void SetContainer(lua_State* L, lua_Integer const& key, char const* const& v)
		{
			auto rtv = lua_getglobal(L, Lua_Container);						// objs
			assert(rtv == LUA_TTABLE);
			Push(L, v);
			lua_pop(L, 1);													//
		}
		static inline void SetGlobal(lua_State* L, char const* const& key, char const* const& v)
		{
			Push(L, v);
			lua_pop(L, 1);													//
		}
		static inline void PushRtv(lua_State* L, char const* const& v)
		{
			lua_pushstring(L, v);
		}
	};

	// fp
	template<typename MP, typename T>
	struct Lua<MP, T, std::enable_if_t<std::is_floating_point<T>::value>>
	{
		static inline void PushMetatable(lua_State* L)
		{
			lua_pushnil(L);
		}
		static inline void Push(lua_State* L, T const& v)
		{
			lua_pushnumber(L, (lua_Number)v);
		}
		static inline void To(MP& mp, lua_State* L, T& v, int idx)
		{
			v = (T)lua_tonumber(L, idx);
		}
		static inline bool TryTo(MP& mp, lua_State* L, T& v, int idx)
		{
			if (!lua_isnumber(L, idx)) return false;
			To(mp, L, v, idx);
			return true;
		}
		static inline void SetContainer(lua_State* L, lua_Integer const& key, T const& v)
		{
			auto rtv = lua_getglobal(L, Lua_Container);						// objs
			assert(rtv == LUA_TTABLE);
			Push(L, v);
			lua_pop(L, 1);													//
		}
		static inline void SetGlobal(lua_State* L, char const* const& key, T const& v)
		{
			Push(L, v);
			lua_pop(L, 1);													//
		}
		static inline void PushRtv(lua_State* L, T const& v)
		{
			lua_pushnumber(L, (lua_Number)v);
		}
	};

	// int
	template<typename MP, typename T>
	struct Lua<MP, T, std::enable_if_t<std::is_integral<T>::value || std::is_enum<T>::value>>
	{
		static inline void PushMetatable(lua_State* L)
		{
			lua_pushnil(L);
		}
		static inline void Push(lua_State* L, T const& v)
		{
			lua_pushinteger(L, (lua_Integer)v);
		}
		static inline void To(MP& mp, lua_State* L, T& v, int idx)
		{
			v = (T)lua_tointeger(L, idx);
		}
		static inline bool TryTo(MP& mp, lua_State* L, T& v, int idx)
		{
			if (!lua_isinteger(L, idx)) return false;
			To(mp, L, v, idx);
			return true;
		}
		static inline void SetContainer(lua_State* L, lua_Integer const& key, T const& v)
		{
			auto rtv = lua_getglobal(L, Lua_Container);						// objs
			assert(rtv == LUA_TTABLE);
			Push(L, v);
			lua_pop(L, 1);													//
		}
		static inline void SetGlobal(lua_State* L, char const* const& key, T const& v)
		{
			Push(L, v);
			lua_pop(L, 1);													//
		}
		static inline void PushRtv(lua_State* L, T const& v)
		{
			lua_pushinteger(L, (lua_Integer)v);
		}
	};

	// T*
	template<typename MP, typename T>
	struct Lua<MP, T, std::enable_if_t<std::is_pointer<T>::value>>
	{
		typedef std::remove_pointer_t<T> TT;

		static inline void PushMetatable(lua_State* L)
		{
			lua_pushvalue(L, TupleIndexOf<TT, typename MP::Tuple>::value);
		}
		static inline void Push(lua_State* L, T const& v)
		{
			lua_pushlightuserdata(L, v);								// lud
			PushMetatable(L);											// lud, mt
			lua_setmetatable(L, -2);									// lud
		}
		static inline void To(MP& mp, lua_State* L, T& v, int idx)
		{
			v = (T)lua_touserdata(L, idx);
		}
		static inline bool TryTo(MP& mp, lua_State* L, T& v, int idx)
		{
			if (!lua_islightuserdata(L, idx)) return false;
			auto rtv = lua_getmetatable(L, idx);						// ... mt?
			if (!rtv) return false;
			rtv = lua_rawgeti(L, -1, 1);								// ... mt, idx
			if (rtv != LUA_TNUMBER) goto LabFail;
			auto idx = lua_tointeger(L, -1);
			if (idx == 0) goto LabFail;
			else if (idx > 0)											// 如果是 MPObject 派生, 直接取指针本体的 typeId 用于判断
			{
				if (idx >= MP::typesSize) goto LabFail;
				v = (T)lua_touserdata(L, idx);							// 这里采取宽松的判断策略 . 如果 v 为空, 则认为必然能转
				if (v && !mp.IsBaseOf(TupleIndexOf<TT, typename MP::Tuple>::value, ((MPObject*)v)->typeId())) goto LabFail;
				goto LabSuccess;
			}
			else
			{
				idx = -idx;
				if (idx >= MP::typesSize) goto LabFail;
				v = (T)lua_touserdata(L, idx);
				if (v && !mp.IsBaseOf(TupleIndexOf<TT, typename MP::Tuple>::value, idx)) goto LabFail;
				goto LabSuccess;
			}
		LabSuccess:
			lua_pop(L, 2);
			return true;
		LabFail:
			lua_pop(L, 2);
			v = nullptr;
			return false;
		}
		static inline void SetContainer(lua_State* L, lua_Integer const& key, T const& v)
		{
			auto rtv = lua_getglobal(L, Lua_Container);					// objs
			assert(rtv == LUA_TTABLE);
			Push(L, v);													// objs, lud
			lua_rawseti(L, -2, key);									// objs
			lua_pop(L, 1);												//
		}
		static inline void SetGlobal(lua_State* L, char const* const& key, T const& v)
		{
			Push(L, v);													// lud
			lua_setglobal(L, key);										//
		}
		static inline void PushRtv(lua_State* L, T const& v)
		{
			lua_pushlightuserdata(L, v);								// lud
			lua_getupvalue(L, -lua_gettop(L) - 1, 1);					// lud, mt
			lua_setmetatable(L, -2);									// lud
		}
	};

	// T
	template<typename MP, typename T>
	struct Lua<MP, T, std::enable_if_t<std::is_class<T>::value && !IsMPtr<T>::value>>
	{
		static inline void PushMetatable(lua_State* L)
		{
			lua_pushvalue(L, TupleIndexOf<T, typename MP::Tuple>::value);
		}
		// todo: 右值版
		static inline void Push(lua_State* L, T const& v)
		{
			auto p = (T*)lua_newuserdata(L, sizeof(v));					// ud
			new (p) T(v);
			PushMetatable(L);											// ud, mt
			lua_setmetatable(L, -2);									// ud
		}
		static inline void To(MP& mp, lua_State* L, T& v, int idx)
		{
			v = *(T*)lua_touserdata(L, idx);
		}
		static inline bool TryTo(MP& mp, lua_State* L, T& v, int idx)
		{
			if (!lua_isuserdata(L, idx)) return false;
			auto rtv = lua_getmetatable(L, idx);						// ... mt?
			if (!rtv) return false;
			rtv = lua_rawgeti(L, -1, 1);								// ... mt, idx
			if (rtv != LUA_TNUMBER) goto LabFail;
			auto idx = lua_tointeger(L, -1);
			if (idx >= 0) goto LabFail;
			idx = -idx;
			if (idx >= MP::typesSize) goto LabFail;
			if (!mp.IsBaseOf(TupleIndexOf<T, typename MP::Tuple>::value, idx)) goto LabFail;
			v = *(T*)lua_touserdata(L, idx);
			lua_pop(L, 2);
			return true;
		LabFail:
			lua_pop(L, 2);
			return false;
		}
		static inline void SetContainer(lua_State* L, lua_Integer const& key, T const& v)
		{
			auto rtv = lua_getglobal(L, Lua_Container);					// objs
			assert(rtv == LUA_TTABLE);
			Push(L, v);													// objs, ud
			lua_rawseti(L, -2, key);									// objs
			lua_pop(L, 1);												//
		}
		static inline void SetGlobal(lua_State* L, char const* const& key, T const& v)
		{
			Push(L, v);													// ud
			lua_setglobal(L, key);										//
		}
		static inline void PushRtv(lua_State* L, T&& v)
		{
			auto p = (T*)lua_newuserdata(L, sizeof(v));					// ud
			new (p) T((T&&)v);
			lua_getupvalue(L, -lua_gettop(L) - 1, 1);					// ud, mt
			lua_setmetatable(L, -2);									// ud
		}
	};

	// MPtr<T>
	template<typename MP, typename T>
	struct Lua<MP, T, std::enable_if_t<IsMPtr<T>::value>>
	{
		typedef typename T::ChildType TT;

		static inline void PushMetatable(lua_State* L)
		{
			lua_pushvalue(L, TupleIndexOf<TT, typename MP::Tuple>::value);
		}
		static inline void Push(lua_State* L, T const& v)
		{
			auto p = lua_newuserdata(L, sizeof(v));						// ud
			new (p) T(v);
			PushMetatable(L);											// lud, mt
			lua_setmetatable(L, -2);									// ud
		}
		static inline void To(MP& mp, lua_State* L, T& v, int idx)
		{
			v = *(T*)lua_touserdata(L, idx);
		}
		static inline bool TryTo(MP& mp, lua_State* L, T& v, int idx)
		{
			if (!lua_isuserdata(L, idx)) return false;
			auto rtv = lua_getmetatable(L, idx);						// ... mt?
			if (!rtv) return false;
			rtv = lua_rawgeti(L, -1, 1);								// ... mt, idx
			if (rtv != LUA_TNUMBER) goto LabFail;
			auto idx = lua_tointeger(L, -1);
			if (idx <= 0 || idx >= MP::typesSize) goto LabFail;
			auto& p = *(T*)lua_touserdata(L, idx);
			if (p && !mp.IsBaseOf(TupleIndexOf<TT, typename MP::Tuple>::value, p->typeId())) goto LabFail;
			v = p;
			lua_pop(L, 2);
			return true;
		LabFail:
			lua_pop(L, 2);
			return false;
		}
		static inline void SetContainer(lua_State* L, lua_Integer const& key, T const& v)
		{
			auto rtv = lua_getglobal(L, Lua_Container);					// objs
			assert(rtv == LUA_TTABLE);
			Push(L, v);													// objs, lud
			lua_rawseti(L, -2, key);									// objs
			lua_pop(L, 1);												//
		}
		static inline void SetGlobal(lua_State* L, char const* const& key, T const& v)
		{
			Push(L, v);													// lud
			lua_setglobal(L, key);										//
		}
		static inline void PushRtv(lua_State* L, T const& v)
		{
			auto p = lua_newuserdata(L, sizeof(v));						// ud
			new (p) T(v);
			lua_getupvalue(L, -lua_gettop(L) - 1, 1);					// ud, mt
			lua_setmetatable(L, -2);									// ud
		}
	};



	/************************************************************************************/
	// Lua_TupleFiller
	/************************************************************************************/

	template<typename MP, typename Tuple, std::size_t N>
	struct Lua_TupleFiller
	{
		static bool Fill(MP& mp, lua_State* L, Tuple& t)
		{
			auto rtv = Lua<MP, std::tuple_element_t<N - 1, Tuple>>::TryTo(mp, L, std::get<N - 1>(t), -(int)(std::tuple_size<Tuple>::value - N + 1));
			if (!rtv) return false;
			return Lua_TupleFiller<MP, Tuple, N - 1>::Fill(mp, L, t);
		}
	};
	template<typename MP, typename Tuple>
	struct Lua_TupleFiller <MP, Tuple, 1 >
	{
		static bool Fill(MP& mp, lua_State* L, Tuple& t)
		{
			return Lua<MP, std::tuple_element_t<0, Tuple>>::TryTo(mp, L, std::get<0>(t), -(int)(std::tuple_size<Tuple>::value));
		}
	};

	template<typename MP, typename Tuple>
	bool Lua_FillTuple(MP& mp, lua_State* L, Tuple& t)
	{
		return Lua_TupleFiller<MP, Tuple, std::tuple_size<Tuple>::value>::Fill(mp, L, t);
	}

	/************************************************************************************/
	// Lua_CallFunc
	/************************************************************************************/

	// 已知问题: 标记为 YIELD 方式执行的函数, 将忽略直接返回值.
	// 有参数 有返回值
	template<typename MP, bool YIELD, typename T, typename R, typename ...Args>
	int Lua_CallFunc(MP& mp, std::enable_if_t<sizeof...(Args) && !std::is_void<R>::value, lua_State*> L, T* o, R(T::* f)(Args...))
	{
		std::tuple<Args...> t;
		if (Lua_FillTuple(mp, L, t))
		{
			auto rtv = FuncTupleCaller(o, f, t, std::make_index_sequence<sizeof...(Args)>());
			if (YIELD) return lua_yield(L, 0);
			Lua<MP, R>::PushRtv(L, rtv);
			return 1;
		}
		return 0;
	}
	// 无参数 有返回值
	template<typename MP, bool YIELD, typename T, typename R, typename ...Args>
	int Lua_CallFunc(MP& mp, std::enable_if_t<!sizeof...(Args) && !std::is_void<R>::value, lua_State*> L, T* o, R(T::* f)(Args...))
	{
		auto rtv = (o->*f)();
		if (YIELD) return lua_yield(L, 0);
		Lua<MP, R>::PushRtv(L, rtv);
		return 1;
	}
	// 有参数 无返回值
	template<typename MP, bool YIELD, typename T, typename R, typename ...Args>
	int Lua_CallFunc(MP& mp, std::enable_if_t<sizeof...(Args) && std::is_void<R>::value, lua_State*> L, T* o, R(T::* f)(Args...))
	{
		std::tuple<Args...> t;
		if (Lua_FillTuple(mp, L, t))
		{
			FuncTupleCaller(o, f, t, std::make_index_sequence<sizeof...(Args)>());
			if (YIELD) return lua_yield(L, 0);
			else return 0;
		}
		return 0;
	}
	// 无参数 无返回值
	template<typename MP, bool YIELD, typename T, typename R, typename ...Args>
	int Lua_CallFunc(MP& mp, std::enable_if_t<!sizeof...(Args) && std::is_void<R>::value, lua_State*> L, T* o, R(T::* f)(Args...))
	{
		(o->*f)();
		if (YIELD) return lua_yield(L, 0);
		else return 0;
	}


	/************************************************************************************/
	// Lua_BindFunc_Ensure
	/************************************************************************************/

	template<typename T>
	int Lua_BindFunc_Ensure_Impl(lua_State* L)
	{
		auto top = lua_gettop(L);
		if (top != 1)
		{
			return Lua_Error(L, "error!!! func args num wrong.");
		}
		bool hasValue = false;
		if (!lua_isuserdata(L, 1))
		{
			return Lua_Error(L, "error!!! self is not MPtr<T>.");
		}
		auto rtv = lua_getmetatable(L, 1);							// ... mt?
		if (!rtv)
		{
			lua_pop(L, 1);
			return Lua_Error(L, "error!!! self is no mt.");
		}
		rtv = lua_rawgeti(L, -1, 1);								// ... mt, idx
		if (rtv != LUA_TNUMBER)
		{
			lua_pop(L, 2);
			return Lua_Error(L, "error!!! self's mt is no [1].");
		}
		auto idx = (int)lua_tointeger(L, -1);
		if (idx <= 0 || idx >= MP::typesSize)
		{
			lua_pop(L, 2);
			return Lua_Error(L, "error!!! self's mt[1] out of range.");
		}
		auto& p = *(MPtr<T>*)lua_touserdata(L, 1);
		hasValue = (bool)p;
		lua_pop(L, 2);
		lua_pushboolean(L, hasValue);
		return 1;
	}

	// 生成确认 ud 是否合法的 Ensure 指令
	template<typename T>
	inline void Lua_BindFunc_Ensure(lua_State* L)
	{
		static_assert(std::is_base_of<MPObject, T>::value, "only MPObject* struct have Ensure func.");
		lua_pushstring(L, "Ensure");
		lua_pushcfunction(L, Lua_BindFunc_Ensure_Impl<T>);
		lua_rawset(L, -3);
	}



	// 将 self 转为 T* 的形态返回. 为空则表示转换失败或指针变野
	// 根据 lua_islightuserdata 结合 is_base_of<MPObject, T> 来判断到底是哪种情况
	template<typename MP, typename T, typename ENABLE = void>
	struct LuaSelf;

	template<typename MP, typename T>
	struct LuaSelf<MP, T, std::enable_if_t< std::is_base_of<MPObject, T>::value>>
	{
		static T* Get(MP& mp, lua_State* L)
		{
			// todo: type check
			if (lua_islightuserdata(L, 1))
			{
				return (T*)lua_touserdata(L, 1);
			}
			auto& ptr = *(MPtr<T>*)lua_touserdata(L, 1);
			if (ptr) return ptr.pointer;
			return nullptr;
		}
	};
	template<typename MP, typename T>
	struct LuaSelf<MP, T, std::enable_if_t<!std::is_base_of<MPObject, T>::value>>
	{
		static T* Get(MP& mp, lua_State* L)
		{
			// todo: type check
			return (T*)lua_touserdata(L, 1);
		}
	};

}



/************************************************************************************/
// xxLua_BindFunc
/************************************************************************************/

// 函数绑定
#define xxLua_BindFunc(MPTYPE, LUA, T, F, YIELD)										\
lua_pushstring(LUA, #F);																\
xx::Lua<MPTYPE, decltype(xx::GetFuncReturnType(&T::F))>::PushMetatable(LUA);			\
lua_pushcclosure(LUA, [](lua_State* L)													\
{																						\
	auto top = lua_gettop(L);															\
	auto numArgs = xx::GetFuncArgsCount(&T::F);											\
	if (top != numArgs + 1)																\
	{																					\
		return xx::Lua_Error(L, "error!!! wrong num args.");							\
	}																					\
	MPTYPE* mp;																			\
	lua_getallocf(L, (void**)&mp);														\
	auto self = xx::LuaSelf<MPTYPE, T>::Get(*mp, L);									\
	if (!self)																			\
	{																					\
		return xx::Lua_Error(L, "error!!! self is nil or bad data type!");				\
	}																					\
	return xx::Lua_CallFunc<MPTYPE, YIELD>(*mp, L, self, &T::F);						\
}, 1);																					\
lua_rawset(LUA, -3);


/************************************************************************************/
// xxLua_BindField
/************************************************************************************/

// 成员变量绑定
#define xxLua_BindField(MP, LUA, T, F, writeable)										\
lua_pushstring(LUA, #F);																\
xx::Lua<decltype(MP), decltype(xx::GetFieldType(&T::F))>::PushMetatable(LUA);			\
lua_pushcclosure(LUA, [](lua_State* L)													\
{																						\
	auto top = lua_gettop(L);															\
	if (!top)																			\
	{																					\
		return xx::Lua_Error(L, "error!!! forget : ?");									\
	}																					\
	auto self = xx::LuaSelf<decltype(MP), T>::Get(MP, L);								\
	if (!self)																			\
	{																					\
		return xx::Lua_Error(L, "error!!! self is nil or bad data type!");				\
	}																					\
	if (top == 1)																		\
	{																					\
		xx::Lua<decltype(MP), decltype(self->F)>::PushRtv(MP, L, self->F);				\
		return 1;																		\
	}																					\
	if (top == 2)																		\
	{																					\
		if (!writeable)																	\
		{																				\
			return xx::Lua_Error(L, "error!!! readonly!");								\
		}																				\
		else if (!xx::Lua<decltype(MP), decltype(self->F)>::TryTo(MP, L, self->F, 2))	\
		{																				\
			return xx::Lua_Error(L, "error!!! bad data type!");							\
		}																				\
		return 0;																		\
	}																					\
	return xx::Lua_Error(L, "error!!! too many args!");									\
}, 1);																					\
lua_rawset(LUA, -3);






// 这个函数移到 Lua<T>::SetContainer

///************************************************************************************/
//// Lua_ContainerPush
///************************************************************************************/

//// 将 MPtr<T> 放至全局容器( key 为 pureVersionNumber ). 主要用于 coroutine 中用 objs[ pvn ] 造出 self
//template<typename MP, typename T>
//inline void Lua_ContainerPush(MP& mp, lua_State* L, MPtr<T> const& o)
//{
//	auto rtv = lua_getglobal(L, Lua_Container);						// objs
//	assert(rtv == LUA_TTABLE);
//	auto p = lua_newuserdata(L, sizeof(o));							// objs, ud
//	new (p) MPtr<T>(o);
//	lua_pushvalue(L, TupleIndexOf<T, typename MP::Tuple>::value);	// objs, ud, mt
//	lua_setmetatable(L, -2);
//	lua_rawseti(L, -2, o->pureVersionNumber());						// objs
//	lua_pop(L, 1);													//
//}

//// 将 o 转为 MPtr<T> 放至全局容器( key 为 pureVersionNumber ). 主要用于 coroutine 中用 objs[ pvn ] 造出 self
//template<typename MP, typename T>
//inline void Lua_ContainerPush(MP& mp, lua_State* L, T const* o)
//{
//	static_assert(std::is_base_of<MPObject, T>::value, "the T must be inherit of MPObject");
//	Lua_ContainerPush(mp, L, MPtr<T>(o));
//}


///************************************************************************************/
//// Lua_ContainerPop
///************************************************************************************/

//// 从全局容器杀对象( key 为 pureVersionNumber )
//// 主要供状态机析构函数处调用. o 传入 this
//inline void Lua_ContainerPop(lua_State* L, MPObject const* o)
//{
//	assert(o);
//	if (!L) return;
//	auto rtv = lua_getglobal(L, Lua_Container);			// objs
//	assert(rtv == LUA_TTABLE);
//	lua_pushnil(L);										// objs, nil
//	lua_rawseti(L, -2, o->pureVersionNumber());			// objs
//	lua_pop(L, 1);										//
//}

//template<typename T>
//inline void Lua_ContainerPop(lua_State* L, MPtr<T> const& o)
//{
//	Lua_ContainerPop(L, o->pointer);
//}


// 这个函数移到 Lua<T>::SetGlobal

//template<typename MP, typename T>
//inline void Lua_SetGlobal(MP& mp, lua_State* L, T* o, char const* name)
//{
//	lua_pushlightuserdata(L, o);						// lud
//	lua_pushvalue(L, Lua_Metatable<T>::index);			// lud, mt
//	lua_setmetatable(L, -2);							// lud
//	lua_setglobal(L, name);
//}
