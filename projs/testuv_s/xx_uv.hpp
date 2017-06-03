#pragma once
namespace xx
{

	inline UV::UV()
		: listeners(mempool())
		, clients(mempool())
	{
		loop = uv_default_loop();
	}

	inline UV::~UV()
	{
		// todo release container items
	}

	inline void UV::Run()
	{
		uv_run(loop, UV_RUN_DEFAULT);
	}



	template<typename ListenerType>
	ListenerType* UV::CreateListener(int port, int backlog)
	{
		return mempool().Create<ListenerType>(this, port, backlog);
	}

	template<typename ClientType>
	ClientType* UV::CreateClient()
	{
		return mempool().Create<ClientType>(this);
	}






	inline UVListener::UVListener(UV* uv, int port, int backlog)
		: uv(uv)
		, uv_listeners_index(uv->listeners->dataLen)
		, peers(mempool())
	{
		struct sockaddr_in addr;
		uv_ip4_addr("0.0.0.0", port, &addr);

		if (uv_tcp_init(uv->loop, &tcpServer)) throw - 1;
		if (uv_tcp_bind(&tcpServer, (const struct sockaddr*) &addr, 0)) throw - 2;
		if (uv_listen((uv_stream_t*)&tcpServer, backlog, OnConnect)) throw - 3;

		uv->listeners->Add(this);
	}

	inline UVListener::~UVListener()
	{
		// todo release peers?

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






	inline UVServerPeer::UVServerPeer(UVListener* listener)
		: uv(listener->uv)
		, listener(listener)
		, listener_peers_index(listener->peers->dataLen)
		, bbReceive(mempool())
		, bbReceiveLeft(mempool())
		, bbReceivePackage(mempool())
	{
		if (uv_tcp_init(uv->loop, (uv_tcp_t*)&stream)) throw - 1;

		listener->peers->Add(this);

		if (uv_accept((uv_stream_t*)&listener->tcpServer, (uv_stream_t*)&stream))
		{
			uv_close((uv_handle_t*)&stream, CloseCB);
			return;
		}

		if (uv_read_start((uv_stream_t*)&stream, AllocCB, ReadCB))
		{
			uv_close((uv_handle_t*)&stream, CloseCB);
			return;
		}
	}

	inline UVServerPeer::~UVServerPeer()
	{
		bbReceivePackage->buf = nullptr;
		bbReceivePackage->bufLen = 0;
		bbReceivePackage->dataLen = 0;
		bbReceivePackage->offset = 0;

		XX_LIST_SWAP_REMOVE(listener->peers, this, listener_peers_index);
	}

	inline void UVServerPeer::OnReceive()
	{
		// 先实现定长 2 字节包头的版本

		// 如果 bbReceiveLeft 没数据, 则直接在 bbReceive 上进行包完整性判断. 如果内含完整包, 不断触发 OnReceivePackage 处理之, 最后将剩下的数据移到 bbReceiveLeft
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
		// 如果 bbReceiveLeft 有数据, 则试从 bbReceive 补齐一个包的数据触发 OnReceivePackage 后清数据, 跳到 bbReceiveLeft 没数据的流程
		else
		{
			uint16_t dataLen = 0;

			// 判断头长度. 如果不够长, 看看能不能补足
			if (bbReceiveLeft->offset + sizeof(dataLen) > bbReceiveLeft->dataLen)
			{
				// 还差多少凑齐包头
				auto left = bbReceiveLeft->offset + sizeof(dataLen) - bbReceiveLeft->dataLen;

				// 如果剩余数据长度无法补足, 移动剩余数据到头部后追加刚收到的数据后退出
				if (bbReceive->offset + left > bbReceive->dataLen) goto LabEnd;
				else
				{
					bbReceiveLeft->Write(bbReceive->buf[bbReceive->offset++]);		// 这里只可能差1字节补足包头
				}
			}

			// 读包头, 得到长度
			dataLen = bbReceiveLeft->buf[bbReceiveLeft->offset] + (bbReceiveLeft->buf[bbReceiveLeft->offset + 1] << 8);
			bbReceiveLeft->offset += 2;
			auto offset_bak = bbReceiveLeft->offset;

			// 判断数据区长度. 如果不够长, 看看能不能补足
			if (bbReceiveLeft->offset + dataLen > bbReceiveLeft->dataLen)
			{
				// 还差多少凑齐数据区
				auto left = bbReceiveLeft->offset + dataLen - bbReceiveLeft->dataLen;

				// 如果剩余数据长度无法补足, 移动剩余数据到头部后追加刚收到的数据后退出
				if (bbReceive->offset + left > bbReceive->dataLen) goto LabEnd;
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
			bbReceiveLeft->offset = 0;
			if (bbReceive->dataLen > bbReceive->offset) goto LabBegin;

		LabEnd:
			// 移动 bbReceiveLeft 剩余数据到头部
			auto left = bbReceiveLeft->dataLen - bbReceiveLeft->offset;
			memmove(bbReceiveLeft->buf, bbReceiveLeft->buf + bbReceiveLeft->offset, left);
			bbReceiveLeft->dataLen = left;
			bbReceiveLeft->offset = 0;
			// 追加 bbReceive 剩余数据到 bbReceiveLeft
			bbReceiveLeft->WriteBuf(bbReceive->buf + bbReceive->offset, bbReceive->dataLen - bbReceive->offset);
		}
	}

	void UVServerPeer::AllocCB(uv_handle_t* handle, size_t suggested_size, uv_buf_t* buf)
	{
		auto self = container_of(handle, UVServerPeer, stream);
		self->bbReceive->Reserve((uint32_t)suggested_size);
		buf->base = self->bbReceive->buf;
		buf->len = self->bbReceive->bufLen;
	}

	void UVServerPeer::CloseCB(uv_handle_t* handle)
	{
		auto self = container_of(handle, UVServerPeer, stream);
		self->Release();
	}

	void UVServerPeer::ReadCB(uv_stream_t* handle, ssize_t nread, const uv_buf_t* buf)
	{
		auto self = container_of(handle, UVServerPeer, stream);
		if (nread < 0)
		{
			/* Error or EOF */
			auto rtv = uv_shutdown(&self->sreq, handle, ShutdownCB);
			assert(!rtv);
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

	void UVServerPeer::ShutdownCB(uv_shutdown_t* req, int status)
	{
		uv_close((uv_handle_t*)req->handle, CloseCB);
	}







	inline UVClient::UVClient(UV* uv)
	{

	}
	inline UVClient::~UVClient()
	{

	}
	bool UVClient::SetAddress(/*, addr */)
	{
		return false;
	}
	bool UVClient::Connect()
	{
		return false;
	}

}
