#pragma execution_character_set("utf-8")
#pragma once
#include "xx_lua_helpers.h"

namespace xx
{
	struct LuaUvTcpClient : public UvTcpClient
	{
		LuaUvTcpClient(lua_State* L)
			: UvTcpClient(*LuaGetUvLoop(L))
			, messages(LuaGetUvLoop(L)->mempool)
		{
		}
		LuaUvTcpClient(LuaUvTcpClient const&) = delete;
		LuaUvTcpClient& operator=(LuaUvTcpClient const&) = delete;

		// 潜规则类名变量
		inline static const char * const name = "UvTcpClient";

		// 向 lua 映射全局的 UvTcpClient 表/元表
		inline static void LuaRegister(lua_State *L)
		{
			luaL_Reg funcs[] =
			{
				{ "__gc", __gc },
			{ "__tostring", __tostring },
			{ "Create", Create_ },								// 创建类实例

			{ "Send", Send_ },									// 发一般包
			{ "SendResponse", SendResponse_ },					// 发请求包
			{ "SendRequest", SendRequest_ },					// 发应答包

			{ "SetAddress", SetAddress_ },						// 设置目标 ip & port
			{ "SetAddress6", SetAddress6_ },					// 设置目标 ipv6 & port
			{ "Connect", Connect_ },							// 开始连接目标 ip & port. 需要先 SetAddress. 兼容 v6.
			{ "ConnectEx", ConnectEx_ },						// Disconnect + SetAddress + Connect
			{ "Connect6Ex", Connect6Ex_ },						// Disconnect + SetAddress6 + Connect
			{ "Disconnect", Disconnect_ },						// Disconnect( 并不会自动清掉之前的消息, 同时可能还会产生一条 OnDisconnect 相关的 message )

			{ "GetState", GetState_ },							// 注册连接事件处理函数

			{ "OnConnect", OnConnect_ },						// 注册连接事件处理函数
			{ "OnDisconnect", OnDisconnect_ },					// 注册连接断开事件处理函数
			{ "OnReceive", OnReceive_ },						// 注册一般包处理函数
			{ "OnReceiveRequest", OnReceiveRequest_ },			// 注册请求包处理函数


			{ "PopMessage", PopMessage_ },						// 弹出一个消息, lua 端框架代码继续处理
			{ "ClearMessages", ClearMessages_ },				// 清掉之前的消息

			{ nullptr, nullptr }
			};

			lua_createtable(L, 0, _countof(funcs));				// mt
			luaL_setfuncs(L, funcs, 0);							// mt

			/* 设成自查找 for metatable */
			lua_pushvalue(L, -1);								// mt, mt
			lua_setfield(L, -2, "__index");						// mt

			/* 设置保护元表 */
			lua_pushvalue(L, -1);								// mt, mt
			lua_setfield(L, -2, "__metatable");					// mt

			lua_setglobal(L, name);								// 

			/* 在注册表中创建 funcId 到 lua function 的映射表 */
			lua_pushlightuserdata(L, (void*)name);				// lud
			lua_createtable(L, 128, 0);							// lud, cbfuncs
			lua_rawset(L, LUA_REGISTRYINDEX);					// 
		}

		// 注册函数的自增 id
		int luaFuncIndex = 0;


		// PopMessage 的数据元素. 存了 数据, 类型, 序列号, lua 回调 id
		// 发送类型 使用 -1 表示 回调超时
		struct Message : xx::Object
		{
			// -1 为 回调超时, -2 为连接失败, 3 为连接成功. 0 为收到普通包, 1 为收到请求包, 2 为收到回应包, -3 为连接断开
			int typeId;

			// 请求 / 回应包 相关序列号
			uint32_t serial;

			// 回调函数id
			int funcId;

			// 数据
			xx::BBuffer bb;

			Message(xx::MemPool* mp)
				: xx::Object(mp)
				, bb(mp, 0)		// buf = nullptr
			{
			}
		};

		// 收到的所有网络包, 都堆积在此. 靠 PopMessage 压到 lua
		xx::Queue<xx::Ptr<Message>> messages;

		/**************************************************************************************************/
		// 下面是各种映射函数的具体实现
		/**************************************************************************************************/

		// 析构
		inline static int __gc(lua_State* L)
		{
			auto& self = *(LuaUvTcpClient**)lua_touserdata(L, -1);
			self->~LuaUvTcpClient();
			return 0;
		}

		// 创建, 无参数
		inline static int Create_(lua_State* L)
		{
			auto& self = *(LuaUvTcpClient**)lua_newuserdata(L, sizeof(void*));	// ..., ud
			lua_getglobal(L, name);					// ..., ud, mt

			auto mp = LuaGetMemPool(L);
			if (!(self = mp->Create<LuaUvTcpClient>(L)))
			{
				lua_pop(L, 2);						// ...
				return 0;
			}

			lua_setmetatable(L, -2);				// ..., ud

			self->OnConnect = [self](int status) 
			{
				decltype(auto) m = self->messages.Emplace();
				m.MPCreate(self->mempool);
				m->typeId = status ? -2 : 3;
				m->serial = 0;
				m->funcId = 0;
			};

			self->OnDisconnect = [self] 
			{
				decltype(auto) m = self->messages.Emplace();
				m.MPCreate(self->mempool);
				m->typeId = -3;
				m->serial = 0;
				m->funcId = 0;
			};

			self->OnReceivePackage = [self](xx::BBuffer& bb)
			{
				decltype(auto) m = self->messages.Emplace();
				m.MPCreate(self->mempool);
				m->typeId = 0;
				m->serial = 0;
				m->funcId = self->onReceiveFuncId;
				m->bb.WriteBuf(bb.buf + bb.offset, bb.readLengthLimit - bb.offset);
			};

			self->OnReceiveRequest = [self](uint32_t serial, xx::BBuffer& bb)
			{
				decltype(auto) m = self->messages.Emplace();
				m.MPCreate(self->mempool);
				m->typeId = 1;
				m->serial = serial;
				m->funcId = self->onReceiveRequestFuncId;
				m->bb.WriteBuf(bb.buf + bb.offset, bb.readLengthLimit - bb.offset);
			};

			return 1;
		}

		// 从首参得到自己
		inline static LuaUvTcpClient& GetSelf(lua_State* L, int top)
		{
			if (lua_gettop(L) < top)
			{
				luaL_error(L, "less arg nums. expect %d+", top);
			}
			auto p = LuaGetPointer<LuaUvTcpClient>(L, 1);
			if (!p)
			{
				luaL_error(L, "first arg isn't UvTcpClient( forget \":\" ? )");
			}
			return *p;
		}

		// 适配 lua tostring
		inline static int __tostring(lua_State* L)
		{
			auto& self = GetSelf(L, 1);
			auto s = self.mempool->Str();
			s->Append("{ \"state\":\"");
			switch (self.state)
			{
			case UvTcpStates::Disconnected:
				s->Append("0: Disconnected");
				break;
			case UvTcpStates::Connecting:
				s->Append("1: Connecting");
				break;
			case UvTcpStates::Connected:
				s->Append("2: Connected");
				break;
			case UvTcpStates::Disconnecting:
				s->Append("3: Disconnecting");
				break;
			default:
				s->Append("Unknown(", (int)self.state, ")");
				break;
			}
			s->Append("\" }");
			lua_pushlstring(L, s->buf, s->dataLen);
			return 1;
		}



		// 发送一般包. 返回 非0 则发送失败
		// 参数: self, bb
		inline static int Send_(lua_State* L)
		{
			auto& self = GetSelf(L, 2);
			auto bb = LuaGetPointer<LuaBBuffer>(L, 1);
			if (!bb)
			{
				luaL_error(L, "arg[2] isn't BBuffer( forget \":\" ? )");
			}

			int r = self.Send(*(xx::BBuffer*)bb);
			lua_pushinteger(L, r);
			return 1;
		}



		// 发送请求包. 返回 0 则发送失败( 不会触发回调 ), 非 0 则为刚生成的 序列号
		// 参数: self, bb, cbfunc, interval
		inline static int SendResponse_(lua_State* L)
		{
			auto& self = GetSelf(L, 3);
			auto bb = LuaGetPointer<LuaBBuffer>(L, 1);
			if (!bb)
			{
				luaL_error(L, "arg[2] isn't BBuffer");
			}
			if (!lua_isfunction(L, 3))
			{
				luaL_error(L, "arg[3] isn't lua function");
			}
			int timeoutMS = 0;
			if (lua_gettop(L) >= 4)
			{
				if (lua_isnumber(L, 4))
				{
					timeoutMS = (int)lua_tointeger(L, 4);
					if (timeoutMS < 0)
					{
						luaL_error(L, "arg[4] isn't valid timeoutMS");
					}
				}
				else
				{
					luaL_error(L, "arg[4] isn't timeoutMS number.");
				}
			}

			// 存回调
			int funcId = ++self.luaFuncIndex;
			StoreFunc(L, funcId, 3);

			uint32_t serial = self.SendRequest(*(xx::BBuffer*)bb, [self = &self, funcId] (uint32_t ser, BBuffer* bb)
			{
				if (!self) return;
				if (self->rpcSerials) self->rpcSerials->Remove(ser);
				if (bb)
				{
					decltype(auto) m = self->messages.Emplace();
					m.MPCreate(self->mempool);
					m->typeId = 2;
					m->serial = ser;
					m->funcId = funcId;
					m->bb.WriteBuf(bb->buf + bb->offset, bb->readLengthLimit - bb->offset);
				}
				else
				{
					decltype(auto) m = self->messages.Emplace();
					m.MPCreate(self->mempool);
					m->typeId = -1;
					m->serial = 0;
					m->funcId = funcId;
				}
			}
			, timeoutMS);

			if (serial)
			{
				if (!self.rpcSerials)
				{
					self.rpcSerials.MPCreate(self.mempool);
				}
				self.rpcSerials->Add(serial);
			}

			lua_pushinteger(L, (int)serial);
			return 1;
		}



		// 发送回应包. 返回 非0 则发送失败
		// 参数: self, bb, serial
		inline static int SendRequest_(lua_State* L)
		{
			auto& self = GetSelf(L, 3);
			auto bb = LuaGetPointer<LuaBBuffer>(L, 1);
			if (!bb)
			{
				luaL_error(L, "arg[2] isn't BBuffer( forget \":\" ? )");
			}

			uint32_t serial = 0;
			if (!lua_isnumber(L, 3))
			{
				luaL_error(L, "arg[3] isn't serial( forget \":\" ? )");
			}
			serial = (uint32_t)lua_tointeger(L, 3);

			int r = self.SendResponse(serial, *(xx::BBuffer*)bb);
			lua_pushinteger(L, r);
			return 1;
		}

		inline static int SetAddress_(lua_State* L)
		{
			auto& self = GetSelf(L, 3);
			if (!lua_isstring(L, 2))
			{
				luaL_error(L, "arg[2] isn't ip string");
			}
			if (!lua_isnumber(L, 3))
			{
				luaL_error(L, "arg[3] isn't port number");
			}
			auto ip = lua_tostring(L, 2);
			auto port = (int)lua_tointeger(L, 3);
			int r = self.SetAddress(ip, port);
			lua_pushinteger(L, r);
			return 1;
		}

		inline static int SetAddress6_(lua_State* L)
		{
			auto& self = GetSelf(L, 3);
			if (!lua_isstring(L, 2))
			{
				luaL_error(L, "arg[2] isn't ipv6 string");
			}
			if (!lua_isnumber(L, 3))
			{
				luaL_error(L, "arg[3] isn't port number");
			}
			auto ip = lua_tostring(L, 2);
			auto port = (int)lua_tointeger(L, 3);
			int r = self.SetAddress6(ip, port);
			lua_pushinteger(L, r);
			return 1;
		}

		inline static int Connect_(lua_State* L)
		{
			auto& self = GetSelf(L, 1);
			int timeoutMS = 0;
			if (lua_gettop(L) > 1)
			{
				if (lua_isnumber(L, 2))
				{
					timeoutMS = (int)lua_tointeger(L, 2);
					if (timeoutMS < 0)
					{
						luaL_error(L, "arg[2] isn't valid timeoutMS");
					}
				}
				else
				{
					luaL_error(L, "arg[2] isn't timeoutMS number.");
				}
			}
			int r = self.Connect(timeoutMS);
			lua_pushinteger(L, r);
			return 1;
		}

		inline static int ConnectEx_(lua_State* L)
		{
			auto& self = GetSelf(L, 3);
			if (!lua_isstring(L, 2))
			{
				luaL_error(L, "arg[2] isn't ip string");
			}
			if (!lua_isnumber(L, 3))
			{
				luaL_error(L, "arg[3] isn't port number");
			}
			int timeoutMS = 0;
			if (lua_gettop(L) >= 4)
			{
				if (lua_isnumber(L, 4))
				{
					timeoutMS = (int)lua_tointeger(L, 4);
					if (timeoutMS < 0)
					{
						luaL_error(L, "arg[4] isn't valid timeoutMS");
					}
				}
				else
				{
					luaL_error(L, "arg[4] isn't timeoutMS number.");
				}
			}
			auto ip = lua_tostring(L, 2);
			auto port = (int)lua_tointeger(L, 3);
			int r = self.ConnectEx(ip, port, timeoutMS);
			lua_pushinteger(L, r);
			return 1;
		}

		inline static int Connect6Ex_(lua_State* L)
		{
			auto& self = GetSelf(L, 3);
			if (!lua_isstring(L, 2))
			{
				luaL_error(L, "arg[2] isn't ipv6 string");
			}
			if (!lua_isnumber(L, 3))
			{
				luaL_error(L, "arg[3] isn't port number");
			}
			int timeoutMS = 0;
			if (lua_gettop(L) >= 4)
			{
				if (lua_isnumber(L, 4))
				{
					timeoutMS = (int)lua_tointeger(L, 4);
					if (timeoutMS < 0)
					{
						luaL_error(L, "arg[4] isn't valid timeoutMS");
					}
				}
				else
				{
					luaL_error(L, "arg[4] isn't timeoutMS number.");
				}
			}
			auto ip = lua_tostring(L, 2);
			auto port = (int)lua_tointeger(L, 3);
			int r = self.ConnectEx(ip, port, timeoutMS);
			lua_pushinteger(L, r);
			return 1;
		}

		inline static int Disconnect_(lua_State* L)
		{
			auto& self = GetSelf(L, 1);
			self.Disconnect();
			return 0;
		}

		inline static int GetState_(lua_State* L)
		{
			auto& self = GetSelf(L, 1);
			lua_pushinteger(L, (int)self.state);
			return 1;
		}




		inline int OnXxxx(lua_State* L, int& tarHandlerId)
		{
			// 删旧
			if (tarHandlerId != -1)
			{
				RemoveFunc(L, tarHandlerId);
			}

			if (lua_isnil(L, 2))
			{
				tarHandlerId = -1;
				return 0;
			}

			if (!lua_isfunction(L, 2))
			{
				luaL_error(L, "arg[2] isn't lua function");
			}

			// 加新
			tarHandlerId = ++luaFuncIndex;
			StoreFunc(L, tarHandlerId, 2);

			return 0;
		}


		int onConnectFuncId = -1;
		inline static int OnConnect_(lua_State* L)
		{
			auto& self = GetSelf(L, 2);
			return self.OnXxxx(L, self.onConnectFuncId);
		}
				
		int onDisconnectFuncId = -1;
		inline static int OnDisconnect_(lua_State* L)
		{
			auto& self = GetSelf(L, 2);
			return self.OnXxxx(L, self.onDisconnectFuncId);
		}

		int onReceiveFuncId = -1;
		inline static int OnReceive_(lua_State* L)
		{
			auto& self = GetSelf(L, 2);
			return self.OnXxxx(L, self.onReceiveFuncId);
		}
				
		int onReceiveRequestFuncId = -1;
		inline static int OnReceiveRequest_(lua_State* L)
		{
			auto& self = GetSelf(L, 2);
			return self.OnXxxx(L, self.onReceiveRequestFuncId);
		}




		// 将 lua cb func 放入位于注册表中的容器
		inline static void StoreFunc(lua_State* L, int funcId, int funcIndex)
		{
			lua_pushlightuserdata(L, (void*)name);		// ..., ud
			lua_rawget(L, LUA_REGISTRYINDEX);			// ..., t
			lua_pushinteger(L, funcId);					// ..., t, funcId
			lua_pushvalue(L, funcIndex);				// ..., t, funcId, cb
			lua_rawset(L, -3);							// ..., t
			lua_pop(L, 1);								// ...
		}

		// 从注册表根据 funcId 定位并压函数 到 L
		inline static void PushFunc(lua_State* L, int funcId)
		{
			lua_pushlightuserdata(L, (void*)name);		// ..., ud
			lua_rawget(L, LUA_REGISTRYINDEX);			// ..., t
			lua_pushinteger(L, funcId);					// ..., t, funcId
			lua_rawget(L, -1);							// ..., t, cb
			lua_insert(L, -2);							// ..., cb, t
			lua_pop(L, 1);								// ..., cb
		}

		// 从注册表移除函数
		inline static void RemoveFunc(lua_State* L, int funcId)
		{
			lua_pushlightuserdata(L, (void*)name);		// ..., ud
			lua_rawget(L, LUA_REGISTRYINDEX);			// ..., t
			lua_pushinteger(L, funcId);					// ..., t, funcId
			lua_pushnil(L);								// ..., t, funcId, nil
			lua_rawset(L, -3);							// ..., t
			lua_pop(L, 1);								// ...
		}


		inline static void PushBBuffer(lua_State* L, xx::BBuffer& bb)
		{
			// 压 数据参数
			LuaBBuffer::Create(L);						// ..., bb
			auto& Lbb = *(LuaBBuffer**)lua_touserdata(L, -1);

			// 直接置换内部内存
			std::swap(Lbb->buf, bb.buf);
			std::swap(Lbb->bufLen, bb.bufLen);
			std::swap(Lbb->dataLen, bb.dataLen);
		}


		// 弹出事件数据. 返回格式视类型不同而不同
		inline static int PopMessage_(lua_State* L)
		{
			auto& self = GetSelf(L, 1);
			xx::Ptr<Message> m;
			if (!self.messages.TryPop(m)) return 0;

			lua_pushinteger(L, m->typeId);
			switch (m->typeId)
			{
			case -2:						// 连接失败
				PushFunc(L, self.onConnectFuncId);
				return 2;
			case -1:						// 请求回调超时
				PushFunc(L, m->funcId);
				return 2;
			case 0:							// 收到普通包
				PushBBuffer(L, m->bb);
				PushFunc(L, self.onReceiveFuncId);
				return 2;
			case 1:							// 收到请求包
				PushBBuffer(L, m->bb);
				lua_pushinteger(L, m->serial);
				return 3;
			case 2:							// 收到回应包
				PushBBuffer(L, m->bb);
				PushFunc(L, m->funcId);
				return 3;
			case 3:							// 连接成功
				PushFunc(L, self.onConnectFuncId);
				return 2;
			}
			return 1;
		}		

		inline static int ClearMessages_(lua_State* L)
		{
			auto& self = GetSelf(L, 1);
			self.messages.Clear();
			return 0;
		}

	};
}
