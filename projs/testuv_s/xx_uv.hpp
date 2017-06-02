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
		XX_LIST_SWAP_REMOVE(listener->peers, this, listener_peers_index);
	}

	void UVServerPeer::AllocCB(uv_handle_t* handle, size_t suggested_size, uv_buf_t* buf)
	{
		auto self = container_of(handle, UVServerPeer, stream);
		buf->base = (char*)self->mempool().Alloc(suggested_size);
		buf->len = (decltype(buf->len))suggested_size;
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
			assert(nread == UV_EOF);	/* Error or EOF */

			self->mempool().Free(buf->base);
			auto rtv = uv_shutdown(&self->sreq, handle, ShutdownCB);
			assert(!rtv);
			return;
		}
		if (nread == 0)
		{
			//self->mempool().Free(buf->base);	/* Everything OK, but nothing read. */
			return;
		}
		self->OnReceive(buf->base, nread);
		//self->mempool().Free(buf->base);		// 如果读操作会反复使用同一个 buf 则不能释放
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
