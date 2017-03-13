#pragma once
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
	// todo: Lua_BindFunc 之非类成员函数版. 考虑为 Lua_SetGlobal 增加函数指针重载

	// todo: 考虑一下结构体中的子结构体的 lua userdata 包装( 就是说 ud 的说明部分可能需要移到 uservalue 中去, 或是用 uservalue 来延长 container 的生命周期? )

	// todo: 改进初始 metatable 创建, 默认可按类的父子关系, 自动设为继承关系


	// 可放入 LUA 的数据类型有: float, double, int64, 各式 string, 以及 T, T*
	// 其中 T 又分为 一般结构体 以及 MPtr<T> ( T 为 MPObject 派生类 )
	// T* 分为一般指针 或 MPObject* 派生类指针
	// String* 空指针于 lua 中当前用 nil 来表达
	// 不支持复杂结构体创建为 ud( 可以有构造函数但不要有析构函数. 最好就是个 pod )
	// 只支持单继承




	/************************************************************************************/
	// Lua_GetMainThread
	/************************************************************************************/

	inline lua_State* Lua_GetMainThread(lua_State* L)
	{
		lua_rawgeti(L, LUA_REGISTRYINDEX, LUA_RIDX_MAINTHREAD);
		auto rtv = lua_tothread(L, -1);
		lua_pop(L, 1);
		return rtv;
	}


	/************************************************************************************/
	// Lua_GetMainThread
	/************************************************************************************/

	// 获取内存池. 仅限于用 Lua_NewState 创建的 L 可用
	template<typename MP>
	MP& Lua_GetMemPool(lua_State* L)
	{
		MP* mp;
		lua_getallocf(L, (void**)&mp);
		return *mp;
	}



	/************************************************************************************/
	// Lua_CloneParentMetatables
	/************************************************************************************/

	// 针对所有 mt, 级联复制父的元素到自身, 避免逐级向上查找( 每多一级查询似乎就会慢 1/5, 一直叠加 )
	template<typename MP>
	inline void Lua_CloneParentMetatables(MP& mp, lua_State* L)
	{
		lua_rawgeti(L, LUA_REGISTRYINDEX, LUA_RIDX_GLOBALS);	// _G
		for (int tidx = MP::typesSize - 1; tidx >= 0; --tidx)
		{
			auto pidx = mp.pids[tidx];
			if (pidx == tidx) continue;
			lua_rawgeti(L, -1, tidx);							// _G, mt
			lua_rawgeti(L, -2, pidx);							// _G, mt, pmt
			lua_pushnil(L);										// _G, mt, pmt, nil
			while (lua_next(L, -2) != 0)						// _G, mt, pmt, k, v
			{
				lua_pushvalue(L, -2);							// _G, mt, pmt, k, v, k
				auto dt = lua_rawget(L, -5);					// _G, mt, pmt, k, v, ?
				if (dt == LUA_TNIL)
				{
					lua_pop(L, 1);								// _G, mt, pmt, k, v
					lua_pushvalue(L, -2);						// _G, mt, pmt, k, v, k
					lua_insert(L, -2);							// _G, mt, pmt, k, k, v
					lua_rawset(L, -5);							// _G, mt, pmt, k
				}
				else
				{
					lua_pop(L, 2);								// _G, mt, pmt, k
				}
			}													// _G, mt, pmt
			lua_pop(L, 2);										// _G
		}
		lua_pop(L, 1);											//
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

	inline int Lua_Resume(lua_State* co, xx::String* err, int narg = 0)
	{
		assert(co);
		int status = lua_resume(co, nullptr, narg);
		if (status == LUA_YIELD)
		{
			return 0;	// todo: 暂存函数的返回值? 
		}
		else if (status == LUA_ERRRUN && lua_isstring(co, -1))
		{
			if (err) err->Assign(lua_tostring(co, -1));
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
	// Lua_PushMetatable
	/************************************************************************************/

	// 从 _G[ TypeId ] 取出元表压入栈顶
	template<typename MP, typename T>
	void Lua_PushMetatable(lua_State* L)
	{
		lua_rawgeti(L, LUA_REGISTRYINDEX, LUA_RIDX_GLOBALS);			// _G
		lua_rawgeti(L, -1, TupleIndexOf<T, typename MP::Tuple>::value);	// _G, mt
		lua_replace(L, -2);												// mt
	}



	/************************************************************************************/
	// Lua_UD
	/************************************************************************************/
	// auto ud = (Lua_UD*)lua_touserdata(L, -1);
	// auto t = (T*)(ud + 1)

	// 压入 lua 的 userdata 的数据形态
	enum class Lua_UDTypes : uint8_t
	{
		Pointer,					// 指针( 可能是 MPObject 派生 )
		MPtr,						// MPtr<T>( 必然是 MPObject 派生 )
		Struct						// 结构体( 一定不是 MPObject 派生 )
	};

	// 压入 lua 的 userdata 的数据头. 应该是占 8 字节. 后面紧跟数据区
	template<typename T>
	struct Lua_UD
	{
		int typeIndex;				// MP 中的类型索引( 主用于判断继承关系, 做 dynamic_cast )
		Lua_UDTypes udType;			// 数据形态
		bool isMPObject;			// 是否从 MPObject 派生( 作为指针数据形态的一个补充说明 )
		T data;
	};


	/************************************************************************************/
	// Lua<T, cond>
	/************************************************************************************/

	template<typename MP, typename T, typename ENABLE = void>
	struct Lua
	{
		// 将 v 压入栈顶( L 得是根 )
		static void Push(lua_State* L, T const& v);

		// 从 idx 读出数据填到 v
		static void To(lua_State* L, T& v, int idx);

		// 试从 idx 读出数据填到 v. 成功返回 true
		static bool TryTo(lua_State* L, T& v, int idx);
	};

	// string
	template<typename MP>
	struct Lua<MP, std::string, void>
	{
		static inline void Push(lua_State* L, std::string const& v)
		{
			lua_pushlstring(L, v.data(), v.size());
		}
		static inline void To(lua_State* L, std::string& v, int idx)
		{
			size_t len;
			auto s = lua_tolstring(L, idx, &len);
			v.assign(s, len);
		}
		static inline bool TryTo(lua_State* L, std::string& v, int idx)
		{
			if (!lua_isstring(L, idx)) return false;
			To(L, v, idx);
			return true;
		}
	};

	// xx::String*
	template<typename MP>
	struct Lua<MP, String*, void>
	{
		static inline void Push(lua_State* L, String* const& v)
		{
			if (v) lua_pushlstring(L, v->buf, v->dataLen);
			else lua_pushnil(L);
		}
		static inline void To(lua_State* L, String*& v, int idx)
		{
			if (v) v->Release();
			if (lua_isnil(L, idx))
			{
				v = nullptr;
				return;
			}
			size_t len;
			auto s = lua_tolstring(L, idx, &len);
			Lua_GetMemPool<MP>(L).CreateTo(v, s, (uint32_t)len);
		}
		static inline bool TryTo(lua_State* L, String*& v, int idx)
		{
			if (!lua_isstring(L, idx) && !lua_isnil(L, idx)) return false;
			To(L, v, idx);
			return true;
		}
	};

	// char*
	template<typename MP>
	struct Lua<MP, char const*, void>
	{
		static inline void Push(lua_State* L, char const* const& v)
		{
			if (v) lua_pushstring(L, v);
			else lua_pushnil(L);
		}
		static inline void To(lua_State* L, char const*& v, int idx)
		{
			if (lua_isnil(L, idx)) v = nullptr;
			else v = lua_tostring(L, idx);
		}
		static inline bool TryTo(lua_State* L, char const*& v, int idx)
		{
			if (!lua_isstring(L, idx) && !lua_isnil(L, idx)) return false;
			To(L, v, idx);
			return true;
		}
	};

	// fp
	template<typename MP, typename T>
	struct Lua<MP, T, std::enable_if_t<std::is_floating_point<T>::value>>
	{
		static inline void Push(lua_State* L, T const& v)
		{
			lua_pushnumber(L, (lua_Number)v);
		}
		static inline void To(lua_State* L, T& v, int idx)
		{
			v = (T)lua_tonumber(L, idx);
		}
		static inline bool TryTo(lua_State* L, T& v, int idx)
		{
			if (!lua_isnumber(L, idx)) return false;
			To(L, v, idx);
			return true;
		}
	};

	// int
	template<typename MP, typename T>
	struct Lua<MP, T, std::enable_if_t<std::is_integral<T>::value || std::is_enum<T>::value>>
	{
		static inline void Push(lua_State* L, T const& v)
		{
			lua_pushinteger(L, (lua_Integer)v);
		}
		static inline void To(lua_State* L, T& v, int idx)
		{
			v = (T)lua_tointeger(L, idx);
		}
		static inline bool TryTo(lua_State* L, T& v, int idx)
		{
			if (!lua_isinteger(L, idx)) return false;
			To(L, v, idx);
			return true;
		}
	};

	// T*
	template<typename MP, typename T>
	struct Lua<MP, T, std::enable_if_t<std::is_pointer<T>::value>>
	{
		typedef std::remove_pointer_t<T> TT;

		static inline void Push(lua_State* L, T const& v)
		{
			auto ud = (Lua_UD<T>*)lua_newuserdata(L, sizeof(Lua_UD<T>));	// ud
			Lua_PushMetatable<MP, TT>(L);									// ud, mt
			lua_setmetatable(L, -2);										// ud
			if (std::is_base_of<MPObject, TT>::value && v)
			{
				ud->typeIndex = (decltype(ud->typeIndex))((MPObject*)v)->typeId();
			}
			else
			{
				ud->typeIndex = TupleIndexOf_v<TT, typename MP::Tuple>;
			}
			ud->udType = Lua_UDTypes::Pointer;
			ud->isMPObject = IsMPObject<TT>::value;
			new (&ud->data) T(v);
		}
		static inline void To(lua_State* L, T& v, int idx)
		{
			v = ((Lua_UD<T>*)lua_touserdata(L, idx))->data;
		}
		static inline bool TryTo(lua_State* L, T& v, int idx)
		{
			if (!lua_isuserdata(L, idx)) return false;
			auto ud = (Lua_UD<T>*)lua_touserdata(L, idx);
			auto tid = TupleIndexOf<TT, typename MP::Tuple>::value;
			if (!Lua_GetMemPool<MP>(L).IsBaseOf(tid, ud->typeIndex)) return false;
			switch (ud->udType)
			{
			case Lua_UDTypes::Pointer:
				v = ud->data;
				return true;
			case Lua_UDTypes::MPtr:
				v = ((MPtr<TT>*)&ud->data)->Ensure();
				return true;
			case Lua_UDTypes::Struct:
				v = (TT*)&ud->data;			// 理论上讲这个值是危险的. 如果被 lua 回收就没了. 需要立即使用
				return true;
			}
			return false;
		}
	};

	// T
	template<typename MP, typename T>
	struct Lua<MP, T, std::enable_if_t<std::is_class<T>::value && !IsMPtr<T>::value>>
	{
		// todo: 右值版, 结构体析构函数
		static inline void Push(lua_State* L, T const& v)
		{
			auto ud = (Lua_UD<T>*)lua_newuserdata(L, sizeof(Lua_UD<T>));	// ud
			Lua_PushMetatable<MP, T>(L);									// ud, mt
			lua_setmetatable(L, -2);										// ud
			ud->typeIndex = TupleIndexOf<T, typename MP::Tuple>::value;
			ud->udType = Lua_UDTypes::Struct;
			ud->isMPObject = false;
			new (&ud->data) T(v);
		}
		static inline void To(lua_State* L, T& v, int idx)
		{
			v = ((Lua_UD<T>*)lua_touserdata(L, idx))->data;
		}
		static inline bool TryTo(lua_State* L, T& v, int idx)
		{
			if (!lua_isuserdata(L, idx)) return false;
			auto ud = (Lua_UD<T>*)lua_touserdata(L, idx);
			if (!Lua_GetMemPool<MP>(L).IsBaseOf(TupleIndexOf<T, typename MP::Tuple>::value, ud->typeIndex)) return false;
			switch (ud->udType)
			{
			case Lua_UDTypes::Pointer:
				if (ud->isMPObject) return false;
				else
				{
					v = **(T**)&ud->data;
					return true;
				}
			case Lua_UDTypes::MPtr:
				return false;				// MPObject 不支持以值方式使用
			case Lua_UDTypes::Struct:
				v = ud->data;
				return true;
			}
		}
	};

	// MPtr<T>
	template<typename MP, typename T>
	struct Lua<MP, T, std::enable_if_t<IsMPtr<T>::value>>
	{
		typedef typename T::ChildType TT;

		static inline void Push(lua_State* L, T const& v)
		{
			auto ud = (Lua_UD<T>*)lua_newuserdata(L, sizeof(Lua_UD<T>));	// ud
			Lua_PushMetatable<MP, TT>(L);									// ud, mt
			lua_setmetatable(L, -2);										// ud
			if (std::is_base_of<MPObject, TT>::value && v)
			{
				ud->typeIndex = (decltype(ud->typeIndex))((MPObject*)v.pointer)->typeId();
			}
			else
			{
				ud->typeIndex = TupleIndexOf_v<TT, typename MP::Tuple>;
			}
			ud->udType = Lua_UDTypes::MPtr;
			ud->isMPObject = true;
			new (&ud->data) T(v);
		}
		static inline void To(lua_State* L, T& v, int idx)
		{
			v = ((Lua_UD<T>*)lua_touserdata(L, idx))->data;
		}
		static inline bool TryTo(lua_State* L, T& v, int idx)
		{
			if (!lua_isuserdata(L, idx)) return false;
			auto ud = (Lua_UD<T>*)lua_touserdata(L, idx);
			if (!Lua_GetMemPool<MP>(L).IsBaseOf(TupleIndexOf<TT, typename MP::Tuple>::value, ud->typeIndex)) return false;
			switch (ud->udType)
			{
			case Lua_UDTypes::Pointer:
				v = *(TT**)&ud->data;
				return true;
			case Lua_UDTypes::MPtr:
				v = ud->data;
				return true;
			case Lua_UDTypes::Struct:
				return false;
			}
		}
	};

	/************************************************************************************/
	// Lua_SetGlobal
	/************************************************************************************/

	// 在全局以下标方式压入值
	template<typename MP, typename T>
	void Lua_SetGlobal(lua_State* L, lua_Integer const& key, T const& v)
	{
		lua_rawgeti(L, LUA_REGISTRYINDEX, LUA_RIDX_GLOBALS);	// _G
		Lua<MP, T>::Push(L, v);									// _G, v
		lua_rawseti(L, -2, key);								// _G
		lua_pop(L, 1);											//
	}

	// 在全局以字串 key 方式压入值
	template<typename MP, typename T>
	void Lua_SetGlobal(lua_State* L, char const* const& key, T const& v)
	{
		Lua<MP, T>::Push(L, v);									// v
		lua_setglobal(L, key);									//
	}



	/************************************************************************************/
	// Lua_SetGlobalNil
	/************************************************************************************/

	// 从全局清除 key( integer ) 项
	inline void Lua_SetGlobalNil(lua_State* L, lua_Integer const& key)
	{
		if (!L) return;
		lua_rawgeti(L, LUA_REGISTRYINDEX, LUA_RIDX_GLOBALS);	// _G
		lua_pushnil(L);											// _G, nil
		lua_rawseti(L, -2, key);								// _G
		lua_pop(L, 1);											//
	}

	// 从全局清除 key( string ) 项
	inline void Lua_SetGlobalNil(lua_State* L, char const* key)
	{
		if (!L) return;
		lua_pushnil(L);
		lua_setglobal(L, key);
	}


	/************************************************************************************/
	// Lua_TupleFiller
	/************************************************************************************/

	template<typename MP, typename Tuple, std::size_t N>
	struct Lua_TupleFiller
	{
		static bool Fill(lua_State* L, Tuple& t)
		{
			auto rtv = Lua<MP, std::tuple_element_t<N - 1, Tuple>>::TryTo(L, std::get<N - 1>(t), -(int)(std::tuple_size<Tuple>::value - N + 1));
			if (!rtv) return false;
			return Lua_TupleFiller<MP, Tuple, N - 1>::Fill(L, t);
		}
	};
	template<typename MP, typename Tuple>
	struct Lua_TupleFiller <MP, Tuple, 1 >
	{
		static bool Fill(lua_State* L, Tuple& t)
		{
			return Lua<MP, std::tuple_element_t<0, Tuple>>::TryTo(L, std::get<0>(t), -(int)(std::tuple_size<Tuple>::value));
		}
	};

	template<typename MP, typename Tuple>
	bool Lua_FillTuple(lua_State* L, Tuple& t)
	{
		return Lua_TupleFiller<MP, Tuple, std::tuple_size<Tuple>::value>::Fill(L, t);
	}

	/************************************************************************************/
	// Lua_CallFunc
	/************************************************************************************/

	// 已知问题: 标记为 YIELD 方式执行的函数, 将忽略直接返回值.
	// 有参数 有返回值
	template<typename MP, bool YIELD, typename T, typename R, typename ...Args>
	int Lua_CallFunc(std::enable_if_t<sizeof...(Args) && !std::is_void<R>::value, lua_State*> L, T* o, R(T::* f)(Args...))
	{
		std::tuple<Args...> t;
		if (Lua_FillTuple<MP>(L, t))
		{
			auto rtv = FuncTupleCaller(o, f, t, std::make_index_sequence<sizeof...(Args)>());
			if (YIELD) return lua_yield(L, 0);
			Lua<MP, R>::Push(L, rtv);
			return 1;
		}
		return Lua_Error(L, "error!!! bad arg data type? type cast fail?");
	}
	// 无参数 有返回值
	template<typename MP, bool YIELD, typename T, typename R, typename ...Args>
	int Lua_CallFunc(std::enable_if_t<!sizeof...(Args) && !std::is_void<R>::value, lua_State*> L, T* o, R(T::* f)(Args...))
	{
		auto rtv = (o->*f)();
		if (YIELD) return lua_yield(L, 0);
		Lua<MP, R>::Push(L, rtv);
		return 1;
	}
	// 有参数 无返回值
	template<typename MP, bool YIELD, typename T, typename R, typename ...Args>
	int Lua_CallFunc(std::enable_if_t<sizeof...(Args) && std::is_void<R>::value, lua_State*> L, T* o, R(T::* f)(Args...))
	{
		std::tuple<Args...> t;
		if (Lua_FillTuple<MP>(L, t))
		{
			FuncTupleCaller(o, f, t, std::make_index_sequence<sizeof...(Args)>());
			if (YIELD) return lua_yield(L, 0);
			else return 0;
		}
		return Lua_Error(L, "error!!! bad arg data type? type cast fail?");
	}
	// 无参数 无返回值
	template<typename MP, bool YIELD, typename T, typename R, typename ...Args>
	int Lua_CallFunc(std::enable_if_t<!sizeof...(Args) && std::is_void<R>::value, lua_State*> L, T* o, R(T::* f)(Args...))
	{
		(o->*f)();
		if (YIELD) return lua_yield(L, 0);
		else return 0;
	}





	/************************************************************************************/
	// Lua_SetGlobalFunc_Log
	/************************************************************************************/

	inline void Lua_SetGlobalFunc_Log(lua_State* L)
	{
		lua_pushcclosure(L, [](lua_State* L)
		{
			// todo: 遍历并获取所有入参, 记录到某处. 当前主用于打断点
			return 0;
		}, 0);
		lua_setglobal(L, "Log");
	}



	/************************************************************************************/
	// Lua_BindFunc_Ensure
	/************************************************************************************/

	// 生成 ud.Ensure 函数
	template<typename MP>
	void Lua_BindFunc_Ensure(lua_State* L)
	{
		lua_pushstring(L, "Ensure");
		lua_pushcclosure(L, [](lua_State* L)
		{
			auto top = lua_gettop(L);
			if (top != 1)
			{
				return Lua_Error(L, "error!!! func args num wrong.");
			}
			MPObject* self = nullptr;
			auto b = xx::Lua<MP, MPObject*>::TryTo(L, self, 1) && self;
			lua_pushboolean(L, b);
			return 1;
		}, 0);
		lua_rawset(L, -3);
	}

	/************************************************************************************/
	// Lua_BindFunc_Release
	/************************************************************************************/

	// 生成 ud.Release 函数
	template<typename MP>
	void Lua_BindFunc_Release(lua_State* L)
	{
		lua_pushstring(L, "Release");
		lua_pushcclosure(L, [](lua_State* L)
		{
			auto top = lua_gettop(L);
			if (top != 1)
			{
				return Lua_Error(L, "error!!! func args num wrong.");
			}
			MPObject* self = nullptr;
			auto b = xx::Lua<MP, MPObject*>::TryTo(L, self, 1) && self;
			if (b) self->Release();
			return 0;
		}, 0);
		lua_rawset(L, -3);
	}




	/************************************************************************************/
	// Lua_NewState
	/************************************************************************************/

	// 创建并返回一个 lua_State*, 以内存池为内存分配方式, 默认 openLibs 以及创建 mt
	// 可以用 lua_getallocf 函数来得到 mp 指针
	template<typename MP>
	inline lua_State* Lua_NewState(MP& mp, bool openLibs = true, bool registerMetatables = true)
	{
		auto L = lua_newstate([](void *ud, void *ptr, size_t osize, size_t nsize)
		{
			return ((MemPoolBase*)ud)->Realloc(ptr, nsize, osize);
		}, &mp);

		if (openLibs) luaL_openlibs(L);
		if (registerMetatables)
		{
			lua_rawgeti(L, LUA_REGISTRYINDEX, LUA_RIDX_GLOBALS);	// _G

			// 先造出所有类型的 空mt ( __index 指向自己 )
			for (int i = 0; i < MP::typesSize; ++i)
			{
				lua_newtable(L);									// _G, mt
				lua_pushstring(L, "__index");						// _G, mt, __index
				lua_pushvalue(L, -2);								// _G, mt, __index, mt
				lua_rawset(L, -3);									// _G, mt
				lua_rawseti(L, -2, i);								// _G
			}
			assert(lua_gettop(L) == 1);

			// 遍历设其 mt 指向父 mt
			for (int i = 0; i < MP::typesSize; ++i)
			{
				if (i == mp.pids[i]) continue;
				lua_rawgeti(L, -1, i);								// _G, mt
				lua_rawgeti(L, -2, mp.pids[i]);						// _G, mt, pmt
				lua_setmetatable(L, -2);							// _G, mt
				lua_pop(L, 1);										// _G
			}
			assert(lua_gettop(L) == 1);

			lua_rawgeti(L, -1, 0);									// _G, MPObject's mt
			Lua_BindFunc_Ensure<MP>(L);								// _G, MPObject's mt
			Lua_BindFunc_Release<MP>(L);							// _G, MPObject's mt

			lua_pop(L, 2);											//
			assert(lua_gettop(L) == 0);
		}
		return L;
	}

}

/************************************************************************************/
// xxLua_BindFunc
/************************************************************************************/

// 函数绑定
#define xxLua_BindFunc(MPTYPE, LUA, T, F, YIELD)										\
lua_pushstring(LUA, #F);																\
lua_pushcclosure(LUA, [](lua_State* L)													\
{																						\
	auto top = lua_gettop(L);															\
	auto numArgs = xx::GetFuncArgsCount(&T::F);											\
	if (top != numArgs + 1)																\
	{																					\
		return xx::Lua_Error(L, "error!!! wrong num args.");							\
	}																					\
	T* self = nullptr;																	\
	if (!xx::Lua<MPTYPE, T*>::TryTo(L, self, 1))										\
	{																					\
		return xx::Lua_Error(L, "error!!! self is nil or bad data type!");				\
	}																					\
	return xx::Lua_CallFunc<MPTYPE, YIELD>(L, self, &T::F);								\
}, 0);																					\
lua_rawset(LUA, -3);


/************************************************************************************/
// xxLua_BindField
/************************************************************************************/

// 成员变量绑定
#define xxLua_BindField(MPTYPE, LUA, T, F, writeable)									\
lua_pushstring(LUA, #F);																\
lua_pushcclosure(LUA, [](lua_State* L)													\
{																						\
	auto top = lua_gettop(L);															\
	if (!top)																			\
	{																					\
		return xx::Lua_Error(L, "error!!! forget : ?");									\
	}																					\
	T* self = nullptr;																	\
	if (!xx::Lua<MPTYPE, T*>::TryTo(L, self, 1))										\
	{																					\
		return xx::Lua_Error(L, "error!!! self is nil or bad data type!");				\
	}																					\
	if (top == 1)																		\
	{																					\
		xx::Lua<MPTYPE, decltype(self->F)>::Push(L, self->F);							\
		return 1;																		\
	}																					\
	if (top == 2)																		\
	{																					\
		if (!writeable)																	\
		{																				\
			return xx::Lua_Error(L, "error!!! readonly!");								\
		}																				\
		else if (!xx::Lua<MPTYPE, decltype(self->F)>::TryTo(L, self->F, 2))				\
		{																				\
			return xx::Lua_Error(L, "error!!! bad data type!");							\
		}																				\
		return 0;																		\
	}																					\
	return xx::Lua_Error(L, "error!!! too many args!");									\
}, 0);																					\
lua_rawset(LUA, -3);

