#pragma once
namespace xx
{
	inline BBuffer::BBuffer(MemPool* const& mempool, size_t const& capacity) noexcept
		: BaseType(mempool, capacity)
	{}

	inline BBuffer::BBuffer(MemPool* const& mempool, std::pair<char const*, size_t> const& buff) noexcept
		: BaseType(mempool, buff.second)
	{
		memcpy(buf, buff.first, buff.second);
		dataLen = buff.second;
	}

	template<typename ...TS>
	void BBuffer::Write(TS const & ...vs) noexcept
	{
		std::initializer_list<int> n{ (BytesFunc<TS>::WriteTo(*this, vs), 0)... };
		assert(this->dataLen <= this->bufLen);
	}

	template<typename T, typename ...TS>
	int BBuffer::ReadCore(T& v, TS&...vs) noexcept
	{
		if (auto r = BytesFunc<T>::ReadFrom(*this, v)) return r;
		return ReadCore(vs...);
	}

	template<typename T>
	int BBuffer::ReadCore(T& v) noexcept
	{
		return BytesFunc<T>::ReadFrom(*this, v);
	}

	template<typename ...TS>
	int BBuffer::Read(TS&...vs) noexcept
	{
		return ReadCore(vs...);
	}


	/*************************************************************************/
	//  Object 对象读写系列
	/*************************************************************************/


	inline void BBuffer::BeginWrite() noexcept
	{
		mempool->ptrStore->Clear();
		offsetRoot = dataLen;
	}

	inline void BBuffer::BeginRead() noexcept
	{
		mempool->idxStore->Clear();
		offsetRoot = offset;
	}

	template<typename T>
	void BBuffer::WriteRoot(T const& v) noexcept
	{
		BeginWrite();
		Write(v);
	}

	template<typename T>
	int BBuffer::ReadRoot(T& v) noexcept
	{
		BeginRead();
		return Read(v);
	}

	template<typename T>
	void BBuffer::WritePtr(T* const& v) noexcept
	{
		if (!v)
		{
			Write((uint8_t)0);
			return;
		}
		assert(v->memHeader().typeId);	// forget Register TypeId ? 
		Write(v->memHeader().typeId);

		auto rtv = mempool->ptrStore->Add((void*)v, dataLen - offsetRoot);
		Write(mempool->ptrStore->ValueAt(rtv.index));
		if (rtv.success)
		{
			v->ToBBuffer(*this);
		}
	}

	template<typename T>
	int BBuffer::ReadPtr(T*& v) noexcept
	{
		// get typeid
		uint16_t tid;
		if (auto rtv = Read(tid)) return rtv;

		// isnull ?
		if (tid == 0)
		{
			v = nullptr;
			return 0;
		}

		// simple validate tid( bad data or forget register typeid ? )
		if (!MemPool::creators[tid]) return -5;

		// get offset
		size_t ptr_offset = 0, bb_offset_bak = offset - offsetRoot;
		if (auto rtv = Read(ptr_offset)) return rtv;

		// fill or ref
		if (ptr_offset == bb_offset_bak)
		{
			// ensure inherit
			if (!mempool->IsBaseOf(TypeId<T>::value, tid)) return -2;

			// try get creator func
			auto& f = MemPool::creators[tid];

			// try create & read from bb
			v = (T*)f(mempool, this, ptr_offset);
			if (v == nullptr) return -3;
		}
		else
		{
			// try get ptr from dict
			std::pair<void*, uint16_t> val;
			if (!mempool->idxStore->TryGetValue(ptr_offset, val)) return -4;

			// inherit validate
			if (!mempool->IsBaseOf(TypeId<T>::value, val.second)) return -2;

			// set val
			v = (T*)val.first;
		}
		return 0;
	}


	/*************************************************************************/
	//  其他工具函数
	/*************************************************************************/

	inline void BBuffer::Clear() noexcept
	{
		BaseType::Clear();
		offset = 0;
	}

	// 根据数据类型往当前位置写入默认值
	template<typename T>
	void BBuffer::WriteDefaultValue() noexcept
	{
		Write(T());
	}

	// 读指定位置的数据( 不影响 offset )
	template<typename ...TS>
	int BBuffer::ReadAt(size_t const& pos, TS&...vs) noexcept
	{
		if (pos >= dataLen) return -1;
		auto bak = offset;
		offset = pos;
		if (auto rtv = Read(vs...)) return rtv;
		offset = bak;
		return 0;
	}

	// 从当前位置读数据但事后 offset 偏移指定长度
	template<typename ...TS>
	int BBuffer::ReadJump(size_t const& len, TS&...vs) noexcept
	{
		if (offset + len > dataLen) return -1;
		auto bak = offset;
		if (auto rtv = Read(vs...)) return rtv;
		offset = bak + len;
		return 0;
	}

	// 直接追加写入一段 buf ( 并不记录长度 )
	inline void BBuffer::WriteBuf(char const* const& buf, size_t const& len) noexcept
	{
		this->Reserve(this->dataLen + len);
		std::memcpy(this->buf + this->dataLen, buf, len);
		this->dataLen += len;
	}
	inline void BBuffer::WriteBuf(BBuffer const& bb) noexcept
	{
		WriteBuf(bb.buf, bb.dataLen);
	}
	inline void BBuffer::WriteBuf(BBuffer const* const& bb) noexcept
	{
		WriteBuf(bb->buf, bb->dataLen);
	}

	// 追加一个指定长度的空间, 返回当前 dataLen
	inline size_t BBuffer::WriteSpace(size_t const& len) noexcept
	{
		auto rtv = this->dataLen;
		this->Reserve(this->dataLen + len);
		this->dataLen += len;
		return rtv;
	}

	// 在 pos 位置写入一段 buf ( 并不记录长度 ). dataLen 可能撑大.
	inline void BBuffer::WriteBufAt(size_t const& pos, char const* const& buf, size_t const& len) noexcept
	{
		assert(pos < this->dataLen);
		auto bak = this->dataLen;		// 备份原始数据长度, 开始追加. 追加完之后, 对比原始数据长度. 如果没超出, 还要还原.
		this->dataLen = pos;
		WriteBuf(buf, len);
		if (this->dataLen < bak) this->dataLen = bak;
	}

	// 在 pos 位置做 Write 操作. dataLen 可能撑大.
	template<typename ...TS>
	void BBuffer::WriteAt(size_t const& pos, TS const&...vs) noexcept
	{
		assert(pos < this->dataLen);
		auto bak = this->dataLen;
		this->dataLen = pos;
		Write(vs...);
		if (this->dataLen < bak) this->dataLen = bak;
	}



	inline BBuffer::BBuffer(BBuffer* const& bb)
		: BaseType(bb)
	{}

	inline void BBuffer::ToBBuffer(BBuffer &bb) const noexcept
	{
		bb.Write(dataLen);
		bb.WriteBuf(*this);
	}

	inline int BBuffer::FromBBuffer(BBuffer &bb) noexcept
	{
		uint32_t len = 0;
		if (auto rtv = bb.Read(len)) return rtv;
		if (bb.readLengthLimit != 0 && len > bb.readLengthLimit) return -1;
		if (bb.offset + len > bb.dataLen) return -2;
		this->Resize(len);
		if (len == 0) return 0;
		memcpy(this->buf, bb.buf + bb.offset, len);
		bb.offset += len;
		return 0;
	}

	inline void BBuffer::ToString(String &s) const noexcept
	{
		s.Append("{ \"len\":", dataLen, ", \"offset\":", offset, ", \"data\":[ ");
		for (size_t i = 0; i < dataLen; i++)
		{
			s.Append((int)(uint8_t)buf[i], ", ");
		}
		if (dataLen) s.dataLen -= 2;
		s.Append(" ] }");
	}
}
