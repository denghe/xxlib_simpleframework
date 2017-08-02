#pragma once
namespace xx
{

	inline UV::UV()
		: listeners(mempool())
		, clientPeers(mempool())
		, timers(mempool())
		, asyncs(mempool())
	{
		//loop = uv_default_loop();
		if (auto r = uv_loop_init(&loop)) throw r;
		uv_idle_init(&loop, &idler);
	}

	inline UV::~UV()
	{
		for (int i = (int)asyncs->dataLen - 1; i >= 0; --i)
		{
			asyncs->At(i)->Release();
		}
		asyncs->Clear();

		for (int i = (int)timers->dataLen - 1; i >= 0; --i)
		{
			timers->At(i)->Release();
		}
		timers->Clear();

		for (int i = (int)listeners->dataLen - 1; i >= 0; --i)
		{
			listeners->At(i)->Release();	// todo: 传递 release 原因?
		}
		listeners->Clear();

		for (int i = (int)clientPeers->dataLen - 1; i >= 0; --i)
		{
			clientPeers->At(i)->Release();	// todo: 传递 release 原因?
		}
		clientPeers->Clear();

		uv_loop_close(&loop);
	}

	inline int UV::EnableIdle()
	{
		return uv_idle_start(&idler, IdleCB);
	}

	inline void UV::DisableIdle()
	{
		uv_close((uv_handle_t*)&idler, nullptr);
	}

	inline void UV::OnIdle()
	{
	}

	inline void UV::Run()
	{
		uv_run(&loop, UV_RUN_DEFAULT);
	}

	inline void UV::Stop()
	{
		uv_stop(&loop);
	}

	template<typename ListenerType, typename ...Args>
	ListenerType* UV::CreateListener(int port, int backlog, Args &&... args)
	{
		static_assert(std::is_base_of<UVListener, ListenerType>::value, "the ListenerType must inherit of UVListener.");
		return mempool().Create<ListenerType>(this, port, backlog, std::forward<Args>(args)...);
	}

	template<typename ClientPeerType, typename ...Args>
	ClientPeerType* UV::CreateClientPeer(Args &&... args)
	{
		static_assert(std::is_base_of<UVClientPeer, ClientPeerType>::value, "the ClientPeerType must inherit of UVClientPeer.");
		return mempool().Create<ClientPeerType>(this, std::forward<Args>(args)...);
	}

	template<typename TimerType, typename ...Args>
	TimerType* UV::CreateTimer(Args &&... args)
	{
		static_assert(std::is_base_of<UVTimer, TimerType>::value, "the TimerType must inherit of UVTimer.");
		return mempool().Create<TimerType>(this, std::forward<Args>(args)...);
	}

	template<typename AsyncType, typename ...Args>
	AsyncType* UV::CreateAsync(Args &&... args)
	{
		static_assert(std::is_base_of<UVAsync, AsyncType>::value, "the AsyncType must inherit of UVAsync.");
		return mempool().Create<AsyncType>(this, std::forward<Args>(args)...);
	}

	inline void UV::IdleCB(uv_idle_t* handle)
	{
		auto self = container_of(handle, UV, idler);
		self->OnIdle();
	}





	inline UVListener::UVListener(UV* uv, int port, int backlog)
		: uv(uv)
		, uv_listeners_index(uv->listeners->dataLen)
		, peers(mempool())
	{
		sockaddr_in addr;
		uv_ip4_addr("0.0.0.0", port, &addr);

		if (auto rtv = uv_tcp_init(&uv->loop, &tcpServer))
		{
			throw rtv;
		}
		if (auto rtv = uv_tcp_bind(&tcpServer, (sockaddr const*)&addr, 0))
		{
			uv_close((uv_handle_t*)&tcpServer, nullptr);	// rollback
			throw rtv;
		}
		if (auto rtv = uv_listen((uv_stream_t*)&tcpServer, backlog, OnConnect))
		{
			uv_close((uv_handle_t*)&tcpServer, nullptr);	// rollback
			throw rtv;
		}

		uv->listeners->Add(this);
	}

	inline UVListener::~UVListener()
	{
		for (int i = (int)peers->dataLen - 1; i >= 0; --i)
		{
			peers->At(i)->Release();	// todo: 传递 release 原因?
		}
		peers->Clear();

		XX_LIST_SWAP_REMOVE(uv->listeners, this, uv_listeners_index);
	}

	inline void UVListener::OnConnect(uv_stream_t* server, int status)
	{
		auto self = container_of(server, UVListener, tcpServer);
		if (status)
		{
			// todo: connect error log
			return;
		}
		self->OnCreatePeer();
	}






	inline UVPeer::UVPeer()
		: bbReceive(mempool())
		, bbReceiveLeft(mempool())
		, bbReceivePackage(mempool())
		, sendBufs(mempool())
		, writeBufs(mempool())
		, tmpStr(mempool())
		, recvPkgs(mempool())
	{
	}

	inline UVPeer::~UVPeer()
	{
		// 还需要进一步了解这样做的副作用( 已知会导致回调发生, 但此时正在析构, 应该拦截 )
		if (!uv_is_closing((uv_handle_t*)&stream))
		{
			uv_close((uv_handle_t*)&stream, nullptr);
		}

		bbReceivePackage->buf = nullptr;
		bbReceivePackage->bufLen = 0;
		bbReceivePackage->dataLen = 0;
		bbReceivePackage->offset = 0;

		if (recvPkgs->dataLen) bbReceivePackage->ReleasePackages(*recvPkgs);
	}

	inline void UVPeer::AllocCB(uv_handle_t* handle, size_t suggested_size, uv_buf_t* buf)
	{
		auto self = container_of(handle, UVPeer, stream);
		if (suggested_size > self->bbReceive->bufLen)
		{
			self->bbReceive->Reserve((uint32_t)suggested_size);
		}
		buf->base = self->bbReceive->buf;
		buf->len = self->bbReceive->bufLen;
	}

	inline void UVPeer::ShutdownCB(uv_shutdown_t* req, int status)
	{
		if (!uv_is_closing((uv_handle_t*)req->handle))
		{
			uv_close((uv_handle_t*)req->handle, CloseCB);
		}
		else
		{
			CloseCB((uv_handle_t*)req->handle);
		}
	}

	inline void UVPeer::CloseCB(uv_handle_t* handle)
	{
		auto self = container_of(handle, UVPeer, stream);
		self->state = UVPeerStates::Closed;
		self->Clear();
		self->OnDisconnect();
	}

	inline void UVPeer::ReadCB(uv_stream_t* handle, ssize_t nread, const uv_buf_t* buf)
	{
		auto self = container_of(handle, UVPeer, stream);
		if (nread < 0)
		{
			/* Error or EOF */
			self->Disconnect(false);
			return;
		}
		if (nread == 0)
		{
			/* Everything OK, but nothing read. */
			return;
		}
		assert(buf->base == self->bbReceive->buf && buf->len == self->bbReceive->bufLen);
		self->bbReceive->dataLen = (uint32_t)nread;
		self->bbReceive->offset = 0;
		self->OnReceive();
	}

	inline void UVPeer::SendCB(uv_write_t *req, int status)
	{
		auto self = container_of(req, UVPeer, writer);
		self->sending = false;
		if (status)
		{
			//std::cout << "Send error " << uv_strerror(status) << std::endl;
			self->Disconnect();	// todo: 传原因?
		}
		else
		{
			self->Send();  // 继续发, 直到发光	// todo: 如果返回错误, 存 last error?
		}
	}

	inline void UVPeer::OnReceive()
	{
		// 先实现定长 2 字节包头的版本

		// 如果 bbReceiveLeft 没数据, 则直接在 bbReceive 上进行包完整性判断. 
		// 如果内含完整包, 不断触发 OnReceivePackage 处理之, 最后将剩下的数据移到 bbReceiveLeft
		if (!bbReceiveLeft->dataLen)
		{
			// 开始处理
		LabBegin:
			uint16_t dataLen = 0;

			// 判断头长度. 如果不够长, 将剩余数据追加到 bbReceiveLeft 后退出
			if (bbReceive->dataLen < bbReceive->offset + sizeof(dataLen))
			{
				bbReceiveLeft->Write(bbReceive->buf[bbReceive->offset++]);		// 这里只可能是1字节
				return;
			}

			// 读出头
			dataLen = bbReceive->buf[bbReceive->offset] + (bbReceive->buf[bbReceive->offset + 1] << 8);
			bbReceive->offset += 2;

			// 如果数据区长度足够, 来一发 OnReceivePackage 并重复解析头 + 数据的过程
			if (bbReceive->offset + dataLen <= bbReceive->dataLen)
			{
				bbReceivePackage->buf = bbReceive->buf + bbReceive->offset;
				bbReceivePackage->bufLen = dataLen;
				bbReceivePackage->dataLen = dataLen;
				bbReceivePackage->offset = 0;

				OnReceivePackage(*bbReceivePackage);

				// 跳过已处理过的数据段并继续解析流程
				bbReceive->offset += dataLen;
				if (bbReceive->dataLen > bbReceive->offset) goto LabBegin;
			}
			// 否则将剩余数据追加到 bbReceiveLeft 后退出
			else
			{
				bbReceiveLeft->WriteBuf(bbReceive->buf + bbReceive->offset, bbReceive->dataLen - bbReceive->offset);
			}
		}
		// 如果 bbReceiveLeft 有数据, 则试从 bbReceive 补齐一个包的数据
		// 触发 OnReceivePackage 后清数据, 跳到 bbReceiveLeft 没数据的流程
		else
		{
			bbReceiveLeft->offset = 0;
			uint16_t dataLen = 0;

			// 判断头长度. 如果不够长, 看看能不能补足
			if (bbReceiveLeft->offset + sizeof(dataLen) > bbReceiveLeft->dataLen)
			{
				// 还差多少凑齐包头
				auto left = bbReceiveLeft->offset + sizeof(dataLen) - bbReceiveLeft->dataLen;

				// 如果剩余数据长度无法补足, 追加刚收到的数据后退出
				if (bbReceive->offset + left > bbReceive->dataLen)
				{
					bbReceiveLeft->Write(bbReceive->buf[bbReceive->offset]);	// 这里只可能差1字节补足包头( 下同 )
					return;
				}
				else
				{
					bbReceiveLeft->Write(bbReceive->buf[bbReceive->offset++]);
				}
			}

			// 读包头, 得到长度
			dataLen = bbReceiveLeft->buf[bbReceiveLeft->offset] + (bbReceiveLeft->buf[bbReceiveLeft->offset + 1] << 8);
			bbReceiveLeft->offset += 2;

			// 判断数据区长度. 如果不够长, 看看能不能补足
			if (bbReceiveLeft->offset + dataLen > bbReceiveLeft->dataLen)
			{
				// 还差多少凑齐数据区
				auto left = bbReceiveLeft->offset + dataLen - bbReceiveLeft->dataLen;

				// 如果剩余数据长度无法补足, 移动剩余数据到头部后追加刚收到的数据后退出
				if (bbReceive->offset + left > bbReceive->dataLen)
				{
					bbReceiveLeft->WriteBuf(bbReceive->buf + bbReceive->offset, bbReceive->dataLen - bbReceive->offset);
					return;
				}
				// 否则只补齐当前包的数据
				else
				{
					bbReceiveLeft->WriteBuf(bbReceive->buf + bbReceive->offset, left);
					bbReceive->offset += left;
				}
			}

			// 数据区长度足够, 来一发 OnReceivePackage
			bbReceivePackage->buf = bbReceiveLeft->buf + bbReceiveLeft->offset;
			bbReceivePackage->bufLen = dataLen;
			bbReceivePackage->dataLen = dataLen;
			bbReceivePackage->offset = 0;

			OnReceivePackage(*bbReceivePackage);

			// 清除 bbReceiveLeft 中的数据, 如果还有剩余数据, 跳到 bbReceive 处理代码段继续. 
			bbReceiveLeft->dataLen = 0;
			if (bbReceive->dataLen > bbReceive->offset) goto LabBegin;
		}
	}

	inline int UVPeer::Send()
	{
		assert(!sending);
		if (state != UVPeerStates::Connected) return -1;
		auto len = sendBufs->PopTo(*writeBufs, 65536);	// todo: 先写死. 这个值理论上讲可配
		if (len)
		{
			if (auto rtv = uv_write(&writer, (uv_stream_t*)&stream, writeBufs->buf, writeBufs->dataLen, SendCB)) return rtv;
			sending = true;
		}
		return 0;
	}

	inline void UVPeer::Clear()
	{
		bbReceiveLeft->Clear();
		sendBufs->Clear();
	}

	inline BBuffer* UVPeer::GetSendBB(int const& capacity)
	{
		return sendBufs->PopLastBB(capacity);
	}

	inline int UVPeer::Send(BBuffer* const& bb)
	{
		sendBufs->Push(bb);			// 压入, 接管并移交上下文字典		//if (sendBufs->BytesCount() + bb.dataLen > sendBufLimit) return false;
		if (state != UVPeerStates::Connected) return -1;
		if (!sending) return Send();
		return 0;
	}

	inline int UVPeer::Disconnect(bool const& immediately)
	{
		if (state == UVPeerStates::Disconnecting || state == UVPeerStates::Disconnected || state == UVPeerStates::Closed) return -1;
		state = UVPeerStates::Disconnecting;

		// todo: save reason ?
		if (immediately														// 立即断开
			|| !sending && ((uv_stream_t*)&stream)->write_queue_size == 0	// 没数据正在发
			|| uv_shutdown(&sreq, (uv_stream_t*)&stream, ShutdownCB))		// shutdown 失败
		{
			if (!uv_is_closing((uv_handle_t*)&stream))						// 非 正在关
			{
				uv_close((uv_handle_t*)&stream, CloseCB);
			}
			else
			{
				state = UVPeerStates::Closed;
				Clear();
				OnDisconnect();
			}
		}
		return 0;
	}

	inline int UVPeer::SetNoDelay(bool const& enable)
	{
		return uv_tcp_nodelay(&stream, enable ? 1 : 0);
	}

	inline int UVPeer::SetKeepAlive(bool const& enable, uint32_t const& delay)
	{
		return uv_tcp_keepalive(&stream, enable ? 1 : 0, delay);
	}

	inline String& UVPeer::GetPeerName()
	{
		sockaddr_in saddr;
		int len = sizeof(saddr);
		if (auto rtv = uv_tcp_getpeername(&stream, (sockaddr*)&saddr, &len))
		{
			tmpStr->Clear();
		}
		else
		{
			tmpStr->Reserve(16);
			rtv = uv_inet_ntop(AF_INET, &saddr.sin_addr, tmpStr->buf, tmpStr->bufLen);
			tmpStr->dataLen = (uint32_t)strlen(tmpStr->buf);
			tmpStr->Append(':', ntohs(saddr.sin_port));
		}
		return *tmpStr;
	}

	inline void UVPeer::ReleaseRecvPkgs()
	{
		bbReceivePackage->ReleasePackages(*recvPkgs);
	}

	template<typename T>
	int UVPeer::SendCore(T const& pkg)
	{
		auto bb = GetSendBB();
		auto b = bb->WritePackage(pkg);
		if (!b) return -1;
		return Send(bb);
	}
	template<typename T, typename ...TS>
	int UVPeer::SendCore(T const& pkg, TS const& ... pkgs)
	{
		if (auto rtv = SendCore(pkg)) return rtv;
		return SendCore(pkgs...);
	}
	template<typename ...TS>
	int UVPeer::SendPackages(TS const& ... pkgs)
	{
		return SendCore(pkgs...);
	}

	template<typename T>
	void UVPeer::SendCombineCore(BBuffer& bb, T const& pkg)
	{
		bb.WriteRoot(pkg);
	}
	template<typename T, typename ...TS>
	void UVPeer::SendCombineCore(BBuffer& bb, T const& pkg, TS const& ... pkgs)
	{
		bb.WriteRoot(pkg);
		SendCombineCore(bb, pkgs...);
	}
	template<typename ...TS>
	int UVPeer::SendCombine(TS const& ... pkgs)
	{
		auto bb = GetSendBB();
		bb->BeginWritePackage();
		SendCombineCore(*bb, pkgs...);
		if (bb->EndWritePackage()) return -1;
		return Send(bb);
	}






	inline UVServerPeer::UVServerPeer(UVListener* listener)
		: UVPeer()
	{
		state = UVPeerStates::Connected;
		this->uv = listener->uv;
		this->listener = listener;
		if (auto rtv = uv_tcp_init(&uv->loop, (uv_tcp_t*)&stream))
		{
			throw rtv;
		}
		listener_peers_index = listener->peers->dataLen;
		if (auto rtv = uv_accept((uv_stream_t*)&listener->tcpServer, (uv_stream_t*)&stream))
		{
			uv_close((uv_handle_t*)&stream, nullptr);	// rollback
			throw rtv;
		}
		if (auto rtv = uv_read_start((uv_stream_t*)&stream, AllocCB, ReadCB))
		{
			uv_close((uv_handle_t*)&stream, nullptr);	// rollback
			throw rtv;
		}
		listener->peers->Add(this);
	}
	inline UVServerPeer::~UVServerPeer()
	{
		XX_LIST_SWAP_REMOVE(listener->peers, this, listener_peers_index);
	}








	inline UVClientPeer::UVClientPeer(UV* uv)
		: UVPeer()
	{
		state = UVPeerStates::Closed;
		this->uv = uv;
		uv_clientPeers_index = uv->clientPeers->dataLen;
		uv->clientPeers->Add(this);
	}

	inline UVClientPeer::~UVClientPeer()
	{
		XX_LIST_SWAP_REMOVE(uv->clientPeers, this, uv_clientPeers_index);
	}

	inline int UVClientPeer::SetAddress(char const* ip, int port)
	{
		return uv_ip4_addr(ip, port, &tarAddr);
	}

	inline int UVClientPeer::Connect()
	{
		if (state == UVPeerStates::Closed)
		{
			if (auto rtv = uv_tcp_init(&uv->loop, (uv_tcp_t*)&stream)) return rtv;
		}
		else if (state != UVPeerStates::Disconnected) return -1;
		state = UVPeerStates::Connecting;
		if (auto rtv = uv_tcp_connect(&conn, &stream, (sockaddr*)&tarAddr, ConnectCB))
		{
			state = UVPeerStates::Disconnected;
			return rtv;
		}
		return 0;
	}

	inline void UVClientPeer::ConnectCB(uv_connect_t* conn, int status)
	{
		auto self = container_of(conn, UVClientPeer, conn);
		if (!self->versionNumber()) return;			// 拦截因主动析构导致的回调( 当前还不确定有没有其他 CB 也需要拦截 )

		self->lastStatus = status;
		if (status < 0)
		{
			self->state = UVPeerStates::Disconnected;
			self->OnConnect();
		}
		else
		{
			self->state = UVPeerStates::Connected;
			if (uv_read_start((uv_stream_t*)&self->stream, self->AllocCB, self->ReadCB))
			{
				self->state = UVPeerStates::Disconnecting;
				uv_close((uv_handle_t*)&self->stream, self->CloseCB);
			}
			self->OnConnect();
		}
	}








	inline UVTimer::UVTimer(UV* uv)
		: uv(uv)
	{
		uv_timers_index = uv->timers->dataLen;
		if (auto rtv = uv_timer_init(&uv->loop, &timer_req))
		{
			throw rtv;
		}
		uv->timers->Add(this);
	}

	inline UVTimer::~UVTimer()
	{
		uv_close((uv_handle_t*)&timer_req, nullptr);
		XX_LIST_SWAP_REMOVE(uv->timers, this, uv_timers_index);
	}

	inline int UVTimer::Start(uint64_t const& timeoutMS, uint64_t const& repeatIntervalMS)
	{
		return uv_timer_start(&timer_req, TimerCB, timeoutMS, repeatIntervalMS);
	}

	inline void UVTimer::SetRepeat(uint64_t const& repeatIntervalMS)
	{
		uv_timer_set_repeat(&timer_req, repeatIntervalMS);
	}

	inline int UVTimer::Again()
	{
		return uv_timer_again(&timer_req);
	}

	inline int UVTimer::Stop()
	{
		return uv_timer_stop(&timer_req);
	}

	inline void UVTimer::TimerCB(uv_timer_t* handle)
	{
		auto self = container_of(handle, UVTimer, timer_req);
		self->OnFire();
	}








	inline UVAsync::UVAsync(UV* uv)
		: uv(uv)
	{
		uv_asyncs_index = uv->asyncs->dataLen;
		if (auto rtv = uv_async_init(&uv->loop, &async_req, AsyncCB))
		{
			throw rtv;
		}
		uv->asyncs->Add(this);
	}

	inline UVAsync::~UVAsync()
	{
		uv_close((uv_handle_t*)&async_req, nullptr);
		XX_LIST_SWAP_REMOVE(uv->asyncs, this, uv_asyncs_index);
	}

	inline void UVAsync::Fire()
	{
		uv_async_send(&async_req);
	}

	inline void UVAsync::AsyncCB(uv_async_t* handle)
	{
		auto self = container_of(handle, UVAsync, async_req);
		self->OnFire();
	}
}
