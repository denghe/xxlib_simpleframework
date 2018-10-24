namespace xx
{
	template<typename T>
	inline int UvTcpUdpBase::Send(T const& pkg) noexcept
	{
		//assert(pkg);
		bbSend.Clear();
		bbSend.Reserve(5);
		bbSend.dataLen = 5;
		if constexpr (std::is_same<xx::BBuffer, T>::value)
		{
			bbSend.WriteBuf(pkg);
		}
		else
		{
			bbSend.WriteRoot(pkg);
		}
		auto dataLen = bbSend.dataLen - 5;
		if (dataLen <= std::numeric_limits<uint16_t>::max())
		{
			auto p = bbSend.buf + 2;
			p[0] = 0;
			p[1] = (uint8_t)dataLen;
			p[2] = (uint8_t)(dataLen >> 8);
			return SendBytes(p, (int)(dataLen + 3));
		}
		else
		{
			auto p = bbSend.buf;
			p[0] = 0b00000100;
			p[1] = (uint8_t)dataLen;
			p[2] = (uint8_t)(dataLen >> 8);
			p[3] = (uint8_t)(dataLen >> 16);
			p[4] = (uint8_t)(dataLen >> 24);
			return SendBytes(p, (int)(dataLen + 5));
		}
	}

	template<typename T>
	inline uint32_t UvTcpUdpBase::SendRequest(T const& pkg, std::function<void(uint32_t, BBuffer*)>&& cb, int const& interval) noexcept
	{
		assert(loop.rpcMgr);
		bbSend.Clear();
		bbSend.Reserve(5);
		bbSend.dataLen = 5;
		auto serial = loop.rpcMgr->Register(std::move(cb), interval);	// 注册回调并得到流水号
		bbSend.Write(serial);											// 在包前写入流水号
		if constexpr (std::is_same<xx::BBuffer, T>::value)
		{
			bbSend.WriteBuf(pkg);
		}
		else
		{
			bbSend.WriteRoot(pkg);
		}
		auto dataLen = bbSend.dataLen - 5;
		auto r = 0;
		if (dataLen <= std::numeric_limits<uint16_t>::max())
		{
			auto p = bbSend.buf + 2;
			p[0] = 0b00000001;											// 这里标记包头为 Request 类型
			p[1] = (uint8_t)dataLen;
			p[2] = (uint8_t)(dataLen >> 8);
			r = SendBytes(p, (int)(dataLen + 3));
		}
		else
		{
			auto p = bbSend.buf;
			p[0] = 0b00000101;											// 这里标记包头为 Big + Request 类型
			p[1] = (uint8_t)dataLen;
			p[2] = (uint8_t)(dataLen >> 8);
			p[3] = (uint8_t)(dataLen >> 16);
			p[4] = (uint8_t)(dataLen >> 24);
			r = SendBytes(p, (int)(dataLen + 5));
		}
		if (r)	// 发送失败立即发起超时回调
		{
			loop.rpcMgr->Callback(serial, nullptr);
			return 0;
		}
		return serial;													// 返回流水号
	}

	template<typename T>
	inline int UvTcpUdpBase::SendResponse(uint32_t const& serial, T const& pkg) noexcept
	{
		bbSend.Clear();
		bbSend.Reserve(5);
		bbSend.dataLen = 5;
		bbSend.Write(serial);											// 在包前写入流水号
		if constexpr (std::is_same<xx::BBuffer, T>::value)
		{
			bbSend.WriteBuf(pkg);
		}
		else
		{
			bbSend.WriteRoot(pkg);
		}
		auto dataLen = bbSend.dataLen - 5;
		if (dataLen <= std::numeric_limits<uint16_t>::max())
		{
			auto p = bbSend.buf + 2;
			p[0] = 0b00000010;											// 这里标记包头为 Response 类型
			p[1] = (uint8_t)dataLen;
			p[2] = (uint8_t)(dataLen >> 8);
			return SendBytes(p, (int)(dataLen + 3));
		}
		else
		{
			auto p = bbSend.buf;
			p[0] = 0b00000110;											// 这里标记包头为 Big + Response 类型
			p[1] = (uint8_t)dataLen;
			p[2] = (uint8_t)(dataLen >> 8);
			p[3] = (uint8_t)(dataLen >> 16);
			p[4] = (uint8_t)(dataLen >> 24);
			return SendBytes(p, (int)(dataLen + 5));
		}
	}


	template<typename T>
	inline int UvTcpUdpBase::SendRouting(char const* const& serviceAddr, size_t const& serviceAddrLen, T const& pkg) noexcept
	{
		bbSend.Clear();
		bbSend.Reserve(5);
		bbSend.dataLen = 5;
		bbSend.WriteBuf(serviceAddr, serviceAddrLen);					// 在包前写入地址
		if constexpr (std::is_same<xx::BBuffer, T>::value)
		{
			bbSend.WriteBuf(pkg);
		}
		else
		{
			bbSend.WriteRoot(pkg);
		}
		auto dataLen = bbSend.dataLen - 5;
		if (dataLen <= std::numeric_limits<uint16_t>::max())
		{
			auto p = bbSend.buf + 2;
			p[0] = (uint8_t)(0b00001000 | ((serviceAddrLen - 1) << 4));	// 拼接为 XXXX1000 的含长度信息的路由包头
			p[1] = (uint8_t)dataLen;
			p[2] = (uint8_t)(dataLen >> 8);
			return SendBytes(p, (int)(dataLen + 3));
		}
		else
		{
			auto p = bbSend.buf;
			p[0] = (uint8_t)(0b00001100 | ((serviceAddrLen - 1) << 4));	// 拼接为 XXXX1100 的含长度信息的 Big 路由包头
			p[1] = (uint8_t)dataLen;
			p[2] = (uint8_t)(dataLen >> 8);
			p[3] = (uint8_t)(dataLen >> 16);
			p[4] = (uint8_t)(dataLen >> 24);
			return SendBytes(p, (int)(dataLen + 5));
		}
	}

	template<typename T>
	inline uint32_t UvTcpUdpBase::SendRoutingRequest(char const* const& serviceAddr, size_t const& serviceAddrLen, T const& pkg, std::function<void(uint32_t, BBuffer*)>&& cb, int const& interval) noexcept
	{
		bbSend.Clear();
		bbSend.Reserve(5);
		bbSend.dataLen = 5;
		bbSend.WriteBuf(serviceAddr, serviceAddrLen);					// 在包前写入地址
		auto serial = loop.rpcMgr->Register(std::move(cb), interval);	// 注册回调并得到流水号
		bbSend.Write(serial);											// 在包前写入流水号
		if constexpr (std::is_same<xx::BBuffer, T>::value)
		{
			bbSend.WriteBuf(pkg);
		}
		else
		{
			bbSend.WriteRoot(pkg);
		}
		auto dataLen = bbSend.dataLen - 5;
		auto r = 0;
		if (dataLen <= std::numeric_limits<uint16_t>::max())
		{
			auto p = bbSend.buf + 2;
			p[0] = (uint8_t)(0b00001001 | ((serviceAddrLen - 1) << 4));	// 这里标记包头为 addrLen + Routing + Request 类型
			p[1] = (uint8_t)dataLen;
			p[2] = (uint8_t)(dataLen >> 8);
			r = SendBytes(p, (int)(dataLen + 3));
		}
		else
		{
			auto p = bbSend.buf;
			p[0] = (uint8_t)(0b00001101 | ((serviceAddrLen - 1) << 4));	// 这里标记包头为 addrLen + Routing + Big + Request 类型
			p[1] = (uint8_t)dataLen;
			p[2] = (uint8_t)(dataLen >> 8);
			p[3] = (uint8_t)(dataLen >> 16);
			p[4] = (uint8_t)(dataLen >> 24);
			r = SendBytes(p, (int)(dataLen + 5));
		}
		if (r)	// 发送失败立即发起超时回调
		{
			cb(serial, nullptr);
			return 0;
		}
		return serial;													// 返回流水号
	}

	template<typename T>
	inline int UvTcpUdpBase::SendRoutingResponse(char const* const& serviceAddr, size_t const& serviceAddrLen, uint32_t const& serial, T const& pkg) noexcept
	{
		bbSend.Clear();
		bbSend.Reserve(5);
		bbSend.dataLen = 5;
		bbSend.WriteBuf(serviceAddr, serviceAddrLen);					// 在包前写入地址
		bbSend.Write(serial);											// 在包前写入流水号
		if constexpr (std::is_same<xx::BBuffer, T>::value)
		{
			bbSend.WriteBuf(pkg);
		}
		else
		{
			bbSend.WriteRoot(pkg);
		}
		auto dataLen = bbSend.dataLen - 5;
		if (dataLen <= std::numeric_limits<uint16_t>::max())
		{
			auto p = bbSend.buf + 2;
			p[0] = (uint8_t)(0b00001010 | ((serviceAddrLen - 1) << 4));	// 这里标记包头为 addrLen + Routing + Response 类型
			p[1] = (uint8_t)dataLen;
			p[2] = (uint8_t)(dataLen >> 8);
			return SendBytes(p, (int)(dataLen + 3));
		}
		else
		{
			auto p = bbSend.buf;
			p[0] = (uint8_t)(0b00001110 | ((serviceAddrLen - 1) << 4));	// 这里标记包头为 addrLen + Routing + Big + Response 类型
			p[1] = (uint8_t)dataLen;
			p[2] = (uint8_t)(dataLen >> 8);
			p[3] = (uint8_t)(dataLen >> 16);
			p[4] = (uint8_t)(dataLen >> 24);
			return SendBytes(p, (int)(dataLen + 5));
		}
	}

	template<typename T>
	inline uint32_t UvTcpUdpBase::SendRequestEx(T const& pkg, std::function<void(Object_p&)>&& cb, int const& interval) noexcept
	{
		auto serial = SendRequest(pkg, [this, cb = std::move(cb)](uint32_t ser, BBuffer* bb)
		{
			if (rpcSerials) rpcSerials->Remove(ser);
			Object_p inPkg;		// 如果 超时或 read 异常, inPkg 设空值
			if (bb)
			{
				if (bb->ReadRoot(inPkg))
				{
					inPkg.Reset();
				}
			}
			cb(inPkg);	// call 原始 lambda
		}, interval);

		if (!serial) return 0;

		if (!rpcSerials)
		{
			rpcSerials.MPCreate(mempool);
		}
		rpcSerials->Add(serial);

		return serial;
	}

	template<typename T>
	inline void UvHttpPeer::SendHttpResponse(T const& o) noexcept
	{
		s.Assign(o);
		SendHttpResponse();
	}
}
