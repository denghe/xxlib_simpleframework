#pragma once
namespace xx
{
	class BBuffer : public List<char>
	{
	public:
		typedef List<char> BaseType;
		size_t										offset = 0;				// 读指针偏移量
		size_t										offsetRoot = 0;			// offset值写入修正
		size_t										dataLenBak = 0;			// WritePackage 时用于备份当前数据写入偏移
		size_t										readLengthLimit = 0;	// 主用于传递给容器类进行长度合法校验
		Dict_p<void*, size_t>						ptrStore;				// 临时记录 key: 指针, value: offset
		Dict_p<size_t, std::pair<void*, uint16_t>>	idxStore;				// 临时记录 key: 读offset, value: pair<ptr, typeId>

		BBuffer(BBuffer const&o) = delete;
		BBuffer& operator=(BBuffer const&o) = delete;

		explicit BBuffer(MemPool* mempool, size_t capacity = 0)
			: BaseType(mempool, capacity)
		{}

		BBuffer(MemPool* mempool, std::pair<char const*, int> const& buff)
			: BaseType(mempool, buff.second)
		{
			memcpy(buf, buff.first, buff.second);
			dataLen = buff.second;
		}

		template<typename T>
		void Write(T const& v)
		{
			Reserve(this->dataLen + BBCalc(v));
			BBWriteTo(*this, v);
			assert(this->dataLen <= this->bufLen);
		}

		template<typename T>
		int Read(T &v)
		{
			return BBReadFrom(*this, v);
		}


		/*************************************************************************/
		//  Object 对象读写系列
		/*************************************************************************/


		void BeginWrite()
		{
			if (!ptrStore) this->mempool->CreateTo(ptrStore, 16);
			else ptrStore->Clear();
			offsetRoot = dataLen;
		}

		void EndWrite()
		{
			assert(ptrStore);
			ptrStore->Clear();
		}

		void BeginRead()
		{
			if (!idxStore) this->mempool->CreateTo(idxStore, 16);
			else idxStore->Clear();
			offsetRoot = offset;
		}

		void EndRead()
		{
			assert(idxStore);
			idxStore->Clear();
		}

		template<typename T>
		void WriteRoot(T const& v)
		{
			BeginWrite();
			Write(v);
			EndWrite();
		}

		template<typename T>
		int ReadRoot(T &v)
		{
			BeginRead();
			auto rtv = Read(v);
			EndRead();
			return rtv;
		}

		template<typename T>
		void WritePtr(T* const& v)
		{
			assert(ptrStore);
			if (!v)
			{
				WritePods((uint8_t)0);
				return;
			}
			WritePods(v->typeId());

			auto rtv = ptrStore->Add((void*)v, dataLen - offsetRoot);
			WritePods(ptrStore->ValueAt(rtv.index));
			if (rtv.success)
			{
				v->ToBBuffer(*this);
			}
		}

		template<typename T>
		int ReadPtr(T* &v)
		{
			assert(idxStore);

			// get typeid
			uint16_t tid;
			if (auto rtv = Read(tid)) return rtv;

			// isnull ?
			if (tid == 0)
			{
				v = nullptr;
				return 0;
			}

			// get offset
			size_t ptr_offset = 0, bb_offset_bak = offset - offsetRoot;
			if (auto rtv = Read(ptr_offset)) return rtv;

			// fill or ref
			if (ptr_offset == bb_offset_bak)
			{
				// ensure inherit
				if (!mempool->IsBaseOf(TypeId<T>::value, tid)) return -2;

				// try get creator func
				auto f = MemPool::creators[tid];
				assert(f);

				// try create & read from bb
				v = (T*)f(mempool, this, ptr_offset);
				if (v == nullptr) return -3;
			}
			else
			{
				// try get ptr from dict
				std::pair<void*, uint16_t> val;
				if (!idxStore->TryGetValue(ptr_offset, val)) return -4;

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

		void Clear()
		{
			BaseType::Clear();
			offset = 0;
		}

		// 根据数据类型往当前位置写入默认值
		template<typename T>
		void WriteDefaultValue()
		{
			Write(T());
		}

		// 自定义 Write 函数
		std::function<void(BBuffer& bb, void* owner, size_t fieldOffset)> CustomWrite;


		// 读指定位置的数据( 不影响 offset )
		template<typename ...TS>
		int ReadAt(size_t const& pos, TS&...vs)
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
		int ReadJump(size_t const& len, TS&...vs)
		{
			if (offset + len > dataLen) return -1;
			auto bak = offset;
			if (auto rtv = Read(vs...)) return rtv;
			offset = bak + len;
			return 0;
		}

		// 直接追加写入一段 buf ( 并不记录长度 )
		void WriteBuf(char const* buf, size_t const& len)
		{
			this->Reserve(this->dataLen + len);
			std::memcpy(this->buf + this->dataLen, buf, len);
			this->dataLen += len;
		}
		void WriteBuf(BBuffer const& bb)
		{
			WriteBuf(bb.buf, bb.dataLen);
		}
		void WriteBuf(BBuffer const* const& bb)
		{
			WriteBuf(bb->buf, bb->dataLen);
		}

		// 追加一个指定长度的空间, 返回当前 dataLen
		size_t WriteSpace(size_t const& len)
		{
			auto rtv = this->dataLen;
			this->Reserve(this->dataLen + len);
			this->dataLen += len;
			return rtv;
		}

		// 在 pos 位置写入一段 buf ( 并不记录长度 ). dataLen 可能撑大.
		void WriteBufAt(size_t const& pos, char const* buf, size_t const& len)
		{
			assert(pos < this->dataLen);
			auto bak = this->dataLen;		// 备份原始数据长度, 开始追加. 追加完之后, 对比原始数据长度. 如果没超出, 还要还原.
			this->dataLen = pos;
			WriteBuf(buf, len);
			if (this->dataLen < bak) this->dataLen = bak;
		}

		// 在 pos 位置做 Write 操作. dataLen 可能撑大.
		template<typename ...TS>
		void WriteAt(size_t const& pos, TS const&...vs)
		{
			assert(pos < this->dataLen);
			auto bak = this->dataLen;
			this->dataLen = pos;
			Write(vs...);
			if (this->dataLen < bak) this->dataLen = bak;
		}


		/*************************************************************************/
		//  包相关
		/*************************************************************************/

		// todo: 3字节包头支持

		// 开始写一个包
		void BeginWritePackage(uint8_t const& pkgTypeId = 0, uint32_t const& serial = 0)
		{
			dataLenBak = dataLen;
			Reserve(dataLen + 3);
			buf[dataLen] = pkgTypeId;
			dataLen += 3;
			if (pkgTypeId == 1 || pkgTypeId == 2) Write(serial);
		}

		// 结束写一个包, 返回长度是否在包头表达范围内( 如果 true 则会填充包头, false 则回滚长度 )
		bool EndWritePackage()
		{
			auto pkgLen = dataLen - dataLenBak - 3;
			if (pkgLen > std::numeric_limits<uint16_t>::max())
			{
				dataLen = dataLenBak;
				return false;
			}
			memcpy(buf + dataLenBak + 1, &pkgLen, 2);
			return true;
		}

		// 一键爽 write 定长 字节长度 + root数据. 如果超过 长度最大计数, 将回滚 dataLen 并返回 false
		template<typename T>
		bool WritePackage(uint8_t const& pkgTypeId = 0, uint32_t const& serial = 0)
		{
			BeginWritePackage(pkgTypeId, serial);
			WriteRoot(v);
			return EndWritePackage();
		}

		// 在已知数据长度的情况下, 直接以包头格式写入长度. 成功返回 true( 只针对 pkgTypeId == 0 的情况 )
		bool WritePackageLength(uint16_t const& len)
		{
			if (len > std::numeric_limits<uint16_t>::max()) return false;
			Reserve(dataLen + 8 + len);
			buf[dataLen] = 0;
			memcpy(buf + dataLen + 1, &len, 2);
			dataLen += 3;
			return true;
		}

		// 尝试一次性反序列化一到多个包, 将结果填充到 outPkgs, 返回 0 或 错误码
		// 注意: 注意其元素的 引用计数, 通通为 1( 即便是递归互引 )
		// 注意: 即便返回错误码, outPkgs 中也可能存在残留数据
		int ReadPackages(List<Object_p>& outPkgs)
		{
			outPkgs.Clear();
			while (offset < dataLen)
			{
				Object_p ibb;
				if (auto rtv = ReadRoot(ibb)) return rtv;
				if (!ibb) return -2;
				outPkgs.Add(std::move(ibb));
			}
			return 0;
		}

		// 队列版并不清除原有数据, 乃是追加. 如果出错, 也不会回滚.
		int ReadPackages(Queue<Object_p>& outPkgs)
		{
			while (offset < dataLen)
			{
				Object_p ibb;
				if (auto rtv = ReadRoot(ibb)) return rtv;
				if (!ibb) return -2;
				outPkgs.Emplace(std::move(ibb));
			}
			return 0;
		}


	};


	using BBuffer_p = Ptr<BBuffer>;

	using BBuffer_r = Ref<BBuffer>;

}
