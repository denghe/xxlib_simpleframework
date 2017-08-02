#pragma once
#include <functional>
#include "xx_bytesutils.h"
#include "xx_mempool.h"
#include "xx_string.h"
#include "xx_list.h"
#include "xx_dict.h"

namespace xx
{

	/*************************************************************************/
	// BBufferRWSwitcher( GCC 需要将这样的声明写在类外面 )
	/*************************************************************************/

	template<typename T, typename ENABLE = void>
	struct BBufferRWSwitcher
	{
		static void Write(BBuffer* bb, T const& v);
		static int Read(BBuffer* bb, T& v);
	};

	// 非 MPObject 任何形态
	template<typename T>
	struct BBufferRWSwitcher<T, std::enable_if_t< !IsMPObject_v<T> >>
	{
		static void Write(BBuffer* bb, T const& v);
		static int Read(BBuffer* bb, T& v);
	};

	// MPObject* || MPtr || Ptr ( 能当指针来处理的 )
	template<typename T>
	struct BBufferRWSwitcher<T, std::enable_if_t< IsMPObjectPointer_v<T> || IsMPtr<T>::value || IsPtr<T>::value >>
	{
		static void Write(BBuffer* bb, T const& v);
		static int Read(BBuffer* bb, T& v);
	};

	// MemHeaderBox
	template<typename T>
	struct BBufferRWSwitcher<T, std::enable_if_t< IsMemHeaderBox_v<T> >>
	{
		static void Write(BBuffer* bb, T const& v);
		static int Read(BBuffer* bb, T& v);
	};

	// todo: 这里并未支持 MPObjectStruct 参与序列化, 因为实际上不会那么用

	/*************************************************************************/
	// BBuffer 本体
	/*************************************************************************/

	struct BBuffer : public List<char, 16>
	{
		typedef List<char, 16> BaseType;
		uint32_t offset = 0;					// 读指针偏移量
		uint32_t offsetRoot = 0;				// offset值写入修正
		uint32_t dataLenBak = 0;				// WritePackage 时用于备份当前数据写入偏移
		uint32_t readLengthLimit = 0;			// 主用于传递给容器类进行长度合法校验

		BBuffer(BBuffer const&o) = delete;
		BBuffer& operator=(BBuffer const&o) = delete;
		explicit BBuffer(uint32_t capacity = 0) : BaseType(capacity) {}
		BBuffer(BBuffer* bb) : BaseType(bb) {}
		~BBuffer()
		{
			mempool().SafeRelease(ptrStore);
			mempool().SafeRelease(idxStore);
		}

		/*************************************************************************/
		// 路由为统一接口系列
		/*************************************************************************/

		template<typename T>
		void Write(T const& v)
		{
			BBufferRWSwitcher<T>::Write(this, v);
		}
		template<typename T>
		int Read(T &v)
		{
			return BBufferRWSwitcher<T>::Read(this, v);
		}


		/*************************************************************************/
		// 传统 pod 读写系列( 通过 bytesutils 里的重载 / 特化 实现 )
		/*************************************************************************/

		template<typename ...TS>
		void WritePods(TS const& ...vs)
		{
			this->Reserve(this->dataLen + BBCalc(vs...));
			this->dataLen += BBWriteTo(this->buf + this->dataLen, vs...);
			assert(this->dataLen <= this->bufLen);
		}

		template<typename T, typename ...TS>
		int ReadPods(T &v, TS&...vs)
		{
			return BBReadFrom(this->buf, this->dataLen, this->offset, v, vs...);
		}

		int ReadPods() { return 0; }


		/*************************************************************************/
		//  MPObject 对象读写系列
		/*************************************************************************/

		Dict<void*, uint32_t>*						ptrStore = nullptr;		// 临时记录 key: 指针, value: offset
		Dict<uint32_t, std::pair<void*, uint16_t>>*	idxStore = nullptr;		// 临时记录 key: 读offset, value: pair<ptr, typeId>

		void BeginWrite()
		{
			if (!ptrStore) this->mempool().CreateTo(ptrStore, 16);
			//else ptrStore->Clear();
			offsetRoot = dataLen;
		}
		void EndWrite()
		{
			assert(ptrStore);
			ptrStore->Clear();
		}
		void BeginRead()
		{
			if (!idxStore) this->mempool().CreateTo(idxStore, 16);
			//else idxStore->Clear();
			offsetRoot = offset;
		}
		void EndRead()
		{
			assert(idxStore);
			idxStore->Clear();
		}

		// 一键爽 write
		template<typename T>
		void WriteRoot(T const& v)
		{
			BeginWrite();
			Write(v);
			EndWrite();
		}
		// 一键爽 read
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
			if (auto rtv = ReadPods(tid)) return rtv;

			// isnull ?
			if (tid == 0)
			{
				v = nullptr;
				return 0;
			}

			// get offset
			uint32_t ptr_offset = 0, bb_offset_bak = offset - offsetRoot;
			if (auto rtv = ReadPods(ptr_offset)) return rtv;

			// fill or ref
			if (ptr_offset == bb_offset_bak)
			{
				// ensure inherit
				if (!mempool().IsBaseOf(TypeId<T>::value, tid)) return -2;

				// try get creator func
				auto f = MemPool::creators()[tid];
				assert(f);

				// try create & read from bb
				v = (T*)f(&mempool(), this, ptr_offset);
				if (v == nullptr) return -3;
			}
			else
			{
				// try get ptr from dict
				typename std::remove_pointer_t<decltype(idxStore)>::ValueType val;
				if (!idxStore->TryGetValue(ptr_offset, val)) return -4;

				// inherit validate
				if (!mempool().IsBaseOf(TypeId<T>::value, std::get<1>(val))) return -2;

				// set val
				v = (T*)std::get<0>(val);
			}
			return 0;
		}

		template<typename T>
		void WritePtr(MPtr<T> const& v)
		{
			Write(v.Ensure());
		}
		template<typename T>
		int ReadPtr(MPtr<T> &v)
		{
			T* t;
			auto rtv = ReadPtr(t);
			v = t;
			return rtv;
		}
		template<typename T>
		void WritePtr(Ptr<T> const& v)
		{
			Write(v.pointer);
		}
		template<typename T>
		int ReadPtr(Ptr<T> &v)
		{
			T* t;
			auto rtv = ReadPtr(t);
			v = t;
			return rtv;
		}


		template<typename T>
		void WriteBox(MemHeaderBox<T> const& v)
		{
			v->ToBBuffer(*this);
		}
		template<typename T>
		int ReadBox(MemHeaderBox<T> &v)
		{
			return v->FromBBuffer(*this);
		}


		/*************************************************************************/
		//  其他工具函数
		/*************************************************************************/

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
		int ReadAt(uint32_t const& pos, TS&...vs)
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
		int ReadJump(uint32_t const& len, TS&...vs)
		{
			if (offset + len > dataLen) return -1;
			auto bak = offset;
			if (auto rtv = Read(vs...)) return rtv;
			offset = bak + len;
			return 0;
		}

		// 直接追加写入一段 buf ( 并不记录长度 )
		void WriteBuf(char const* buf, uint32_t const& len)
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
		uint32_t WriteSpace(uint32_t const& len)
		{
			auto rtv = this->dataLen;
			this->Reserve(this->dataLen + len);
			this->dataLen += len;
			return rtv;
		}

		// 在 pos 位置写入一段 buf ( 并不记录长度 ). dataLen 可能撑大.
		void WriteBufAt(uint32_t const& pos, char const* buf, uint32_t const& len)
		{
			assert(pos < this->dataLen);
			auto bak = this->dataLen;		// 备份原始数据长度, 开始追加. 追加完之后, 对比原始数据长度. 如果没超出, 还要还原.
			this->dataLen = pos;
			WriteBuf(buf, len);
			if (this->dataLen < bak) this->dataLen = bak;
		}

		// 在 pos 位置做 Write 操作. dataLen 可能撑大.
		template<typename ...TS>
		void WriteAt(uint32_t const& pos, TS const&...vs)
		{
			assert(pos < this->dataLen);
			auto bak = this->dataLen;
			this->dataLen = pos;
			Write(vs...);
			if (this->dataLen < bak) this->dataLen = bak;
		}


		// 开始写一个包( 特指 2 字节长度头的, 保留出包头区域 )
		template<typename SizeType = uint16_t>
		void BeginWritePackage()
		{
			dataLenBak = dataLen;
			Reserve(dataLen + sizeof(SizeType));
			dataLen += sizeof(SizeType);
		}

		// 结束写一个包, 返回长度是否在包头表达范围内( 如果 true 则会填充包头, false 则回滚长度 )
		template<typename SizeType = uint16_t>
		bool EndWritePackage()
		{
			auto pkgLen = dataLen - dataLenBak - sizeof(SizeType);
			if (pkgLen > std::numeric_limits<SizeType>::max())
			{
				dataLen = dataLenBak;
				return false;
			}
			memcpy(buf + dataLenBak, &pkgLen, sizeof(SizeType));
			return true;
		}

		// 一键爽 write 定长 字节长度 + root数据. 如果超过 长度最大计数, 将回滚 dataLen 并返回 false
		template<typename SizeType = uint16_t, typename T>
		bool WritePackage(T const& v)
		{
			BeginWritePackage<SizeType>();
			WriteRoot(v);
			return EndWritePackage<SizeType>();
		}

		//// 在已知数据长度的情况下, 直接以包头格式写入长度. 成功返回 true
		//template<typename SizeType = uint16_t, typename T>
		//bool WritePackageLength(T const& len)
		//{
		//	if (len > std::numeric_limits<SizeType>::max()) return false;
		//	Reserve(dataLen + sizeof(SizeType) + len);
		//	auto tmp = (SizeType)len;
		//	memcpy(buf + dataLen, &tmp, sizeof(SizeType));
		//	dataLen += sizeof(SizeType);
		//	return true;
		//}

		// 尝试一次性反序列化一到多个包, 将结果填充到 outPkgs, 返回包个数 或 错误码
		// 注意: 注意其元素的 引用计数, 通通为 1( 即便是递归互引 )
		// 注意: 即便返回错误码, outPkgs 中也可能存在残留数据
		int ReadPackages(List<MPObject*>& outPkgs)
		{
			if (outPkgs.dataLen) ReleasePackages(outPkgs);
			while (offset < dataLen)
			{
				MPObject* ibb = nullptr;
				if (auto rtv = ReadRoot(ibb)) return rtv;
				if (ibb == nullptr) return -2;
				outPkgs.Add(ibb);
			}
			return outPkgs.dataLen;
		}

		// 释放经由 ReadPackages 填充的 packages 的内存
		void ReleasePackages(List<MPObject*>& recvPkgs)
		{
			mempool().DisableRefCountAssert();
			for (auto& o : recvPkgs)
			{
				o->Release();
			}
			mempool().EnableRefCountAssert();
			recvPkgs.Clear();
		}

		/*************************************************************************/
		// 实现 ToString 接口
		/*************************************************************************/

		// 先简单来点
		virtual void ToString(String &str) const override
		{
			str.Append("{ \"len\" : ", dataLen, ", \"data\" : [ ");
			for (size_t i = 0; i < dataLen; i++)
			{
				str.Append((int)(uint8_t)buf[i], ", ");
			}
			if (dataLen) str.dataLen -= 2;
			str.Append(" ] }");
		}

		/*************************************************************************/
		// 实现 BBuffer 接口
		/*************************************************************************/

		virtual void ToBBuffer(BBuffer &bb) const override;

		virtual int FromBBuffer(BBuffer &bb) override;
	};

	/*************************************************************************/
	// BBufferRWSwitcher
	/*************************************************************************/

	// 非 MPObject 任何形态

	template<typename T>
	void BBufferRWSwitcher<T, std::enable_if_t< !IsMPObject_v<T> >>::Write(BBuffer* bb, T const& v)
	{
		bb->WritePods(v);
	}
	template<typename T>
	int BBufferRWSwitcher<T, std::enable_if_t< !IsMPObject_v<T> >>::Read(BBuffer* bb, T& v)
	{
		return bb->ReadPods(v);
	}

	// MPObject* || MPtr || Ptr ( 能当指针来处理的 )

	template<typename T>
	void BBufferRWSwitcher<T, std::enable_if_t< IsMPObjectPointer_v<T> || IsMPtr<T>::value || IsPtr<T>::value >>::Write(BBuffer* bb, T const& v)
	{
		bb->WritePtr(v);
	}
	template<typename T>
	int BBufferRWSwitcher<T, std::enable_if_t< IsMPObjectPointer_v<T> || IsMPtr<T>::value || IsPtr<T>::value >>::Read(BBuffer* bb, T& v)
	{
		return bb->ReadPtr(v);
	}

	// MemHeaderBox

	template<typename T>
	void BBufferRWSwitcher<T, std::enable_if_t< IsMemHeaderBox_v<T> >>::Write(BBuffer* bb, T const& v)
	{
		bb->WriteBox(v);
	}
	template<typename T>
	int BBufferRWSwitcher<T, std::enable_if_t< IsMemHeaderBox_v<T> >>::Read(BBuffer* bb, T& v)
	{
		return bb->ReadBox(v);
	}

	/*************************************************************************/
	// 实现值类型使用类型声明
	/*************************************************************************/

	using BBuffer_v = MemHeaderBox<BBuffer>;
	using BBuffer_p = Ptr<BBuffer>;
	template<>
	struct MemmoveSupport<BBuffer_v>
	{
		static const bool value = true;
	};

	/*************************************************************************/
	// 实现各种序列化接口
	/*************************************************************************/

	template<typename T, typename PT>
	void MemPool::Register()
	{
		// 存父 pid
		assert(!pids()[TypeId<T>::value]);
		pids()[TypeId<T>::value] = TypeId<PT>::value;

		// 在执行构造函数之前拿到指针 塞入 bb. 构造函数执行失败时从 bb 移除
		creators()[TypeId<T>::value] = [](MemPool* mp, BBuffer* bb, uint32_t ptrOffset) ->void*
		{
			// 插入字典占位, 分配到实际指针后替换
			auto addResult = bb->idxStore->Add(ptrOffset, std::make_pair(nullptr, TypeId<T>::value));

			// 下列代码 复制自 Create 函数小改
			auto siz = sizeof(T) + sizeof(MemHeader_MPObject);
			auto idx = Calc2n(siz);
			if (siz > (size_t(1) << idx)) siz = size_t(1) << ++idx;

			void* rtv;
			if (!mp->ptrstacks[idx].TryPop(rtv)) rtv = malloc(siz);
			if (!rtv) return nullptr;

			auto p = (MemHeader_MPObject*)rtv;
			p->versionNumber = (++mp->versionNumber) | ((uint64_t)idx << 56);
			p->mempool = mp;
			p->refCount = 1;
			p->typeId = TypeId<T>::value;
			p->tsFlags = 0;

			auto t = (T*)(p + 1);
			bb->idxStore->ValueAt(addResult.index).first = t;	// 替换成真实字典
			try
			{
				new (t) T(bb);
			}
			catch (...)
			{
				bb->idxStore->RemoveAt(addResult.index);		// 从字典移除( 理论上讲可以不管, 会层层失败出去最后 clear )
				mp->ptrstacks[idx].Push(p);
				p->versionNumber = 0;
				return nullptr;
			}
			return t;
		};
	};


	void BBuffer::ToBBuffer(BBuffer &bb) const
	{
		bb.Reserve(bb.dataLen + 5 + this->dataLen);
		bb.Write(this->dataLen);
		if (!this->dataLen) return;
		memcpy(bb.buf + bb.dataLen, this->buf, this->dataLen);
		bb.dataLen += this->dataLen;
	}

	int BBuffer::FromBBuffer(BBuffer &bb)
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


	// List 序列化 路由类
	template<typename T, uint32_t reservedHeaderLen, typename ENABLE = void>
	struct ListBBSwitcher
	{
		static void ToBBuffer(List<T, reservedHeaderLen> const* list, BBuffer &bb);
		static int FromBBuffer(List<T, reservedHeaderLen>* list, BBuffer &bb);
		static void CreateFromBBuffer(List<T, reservedHeaderLen>* list, BBuffer &bb);
	};

	// 适配 1 字节长度的 数值 或枚举 或 float( 这些类型直接 memcpy )
	template<typename T, uint32_t reservedHeaderLen>
	struct ListBBSwitcher<T, reservedHeaderLen, std::enable_if_t< sizeof(T) == 1 || std::is_same<float, typename std::decay<T>::type>::value >>
	{
		static void ToBBuffer(List<T, reservedHeaderLen> const* list, BBuffer &bb)
		{
			bb.Reserve(bb.dataLen + 5 + list->dataLen);
			bb.Write(list->dataLen);
			if (!list->dataLen) return;
			memcpy(bb.buf + bb.dataLen, list->buf, list->dataLen);
			bb.dataLen += list->dataLen;
		}
		static int FromBBuffer(List<T, reservedHeaderLen>* list, BBuffer &bb)
		{
			uint32_t len = 0;
			if (auto rtv = bb.Read(len)) return rtv;
			if (bb.readLengthLimit != 0 && len > bb.readLengthLimit) return -1;
			if (bb.offset + len > bb.dataLen) return -2;
			list->Resize(len);
			if (len == 0) return 0;
			memcpy(list->buf, bb.buf + bb.offset, len);
			bb.offset += len;
			return 0;
		}
		static void CreateFromBBuffer(List<T, reservedHeaderLen>* list, BBuffer &bb)
		{
			uint32_t len = 0;
			if (auto rtv = bb.ReadPods(len)) throw rtv;
			if (bb.readLengthLimit != 0 && len > bb.readLengthLimit) throw - 1;
			if (bb.offset + len * sizeof(T) > bb.dataLen) throw - 2;
			if (len == 0) return;
			list->Reserve(len);
			memcpy(list->buf, bb.buf + bb.offset, len * sizeof(T));
			bb.offset += len * sizeof(T);
			list->dataLen = len;
		}
	};

	// 适配非 MPObject* / MPtr ( 只能 foreach 一个个搞, 含 MemHeaderBox )
	template<typename T, uint32_t reservedHeaderLen>
	struct ListBBSwitcher<T, reservedHeaderLen, std::enable_if_t< !(sizeof(T) == 1 || std::is_same<float, typename std::decay<T>::type>::value) && !(IsMPtr_v<T> || (std::is_pointer<T>::value && IsMPObject_v<T>)) >>
	{
		static void ToBBuffer(List<T, reservedHeaderLen> const* list, BBuffer &bb)
		{
			bb.Write(list->dataLen);
			if (!list->dataLen) return;
			for (uint32_t i = 0; i < list->dataLen; ++i)
			{
				bb.Write(list->At(i));
			}
		}
		static int FromBBuffer(List<T, reservedHeaderLen>* list, BBuffer &bb)
		{
			uint32_t len = 0;
			if (auto rtv = bb.Read(len)) return rtv;
			if (bb.readLengthLimit != 0 && len > bb.readLengthLimit) return -1;
			list->Resize(len);
			if (len == 0) return 0;
			for (uint32_t i = 0; i < len; ++i)
			{
				if (auto rtv = bb.Read(list->At(i)))
				{
					list->Resize(i);
					return rtv;
				}
			}
			return 0;
		}
		static void CreateFromBBuffer(List<T, reservedHeaderLen>* list, BBuffer &bb)
		{
			uint32_t len = 0;
			if (auto rtv = bb.Read(len)) throw rtv;
			if (bb.readLengthLimit != 0 && len > bb.readLengthLimit) throw - 1;
			list->Resize(len);
			if (len == 0) return;
			for (uint32_t i = 0; i < len; ++i)
			{
				if (auto rtv = bb.Read(list->At(i)))
				{
					list->Clear(true);
					throw rtv;
				}
			}
		}
	};

	// 适配 MPObject* / MPtr, 反序列化失败后会逐个 Release 已成功创建的 items
	template<typename T, uint32_t reservedHeaderLen>
	struct ListBBSwitcher<T, reservedHeaderLen, std::enable_if_t< IsMPtr_v<T> || (std::is_pointer<T>::value && IsMPObject_v<T>) >>
	{
		static void ToBBuffer(List<T, reservedHeaderLen> const* list, BBuffer &bb)
		{
			bb.Write(list->dataLen);
			if (!list->dataLen) return;
			for (uint32_t i = 0; i < list->dataLen; ++i)
			{
				bb.Write(list->At(i));
			}
		}
		static int FromBBuffer(List<T, reservedHeaderLen>* list, BBuffer &bb)
		{
			uint32_t len = 0;
			if (auto rtv = bb.Read(len)) return rtv;
			if (bb.readLengthLimit != 0 && len > bb.readLengthLimit) return -1;
			list->Resize(len);
			if (len == 0) return 0;
			for (uint32_t i = 0; i < len; ++i)
			{
				if (auto rtv = bb.Read(list->At(i)))
				{
					for (uint32_t j = i - 1; j != (uint32_t)-1; --j)
					{
						if (list->At(j)) list->At(j)->Release();
					}
					list->Resize(0);
					return rtv;
				}
			}
			return 0;
		}
		static void CreateFromBBuffer(List<T, reservedHeaderLen>* list, BBuffer &bb)
		{
			uint32_t len = 0;
			if (auto rtv = bb.Read(len)) throw rtv;
			if (bb.readLengthLimit != 0 && len > bb.readLengthLimit) throw - 1;
			list->Resize(len);
			if (len == 0) return;
			for (uint32_t i = 0; i < len; ++i)
			{
				if (auto rtv = bb.Read(list->At(i)))
				{
					for (uint32_t j = i - 1; j != (uint32_t)-1; --j)
					{
						if (list->At(j)) list->At(j)->Release();
					}
					list->Clear(true);
					throw rtv;
				}
			}
		}
	};

	template<typename T, uint32_t reservedHeaderLen>
	List<T, reservedHeaderLen>::List(BBuffer* bb)
		: buf(nullptr)
		, bufLen(0)
		, dataLen(0)
	{
		ListBBSwitcher<T, reservedHeaderLen>::CreateFromBBuffer(this, *bb);
	}

	template<typename T, uint32_t reservedHeaderLen>
	void List<T, reservedHeaderLen>::ToBBuffer(BBuffer &bb) const
	{
		ListBBSwitcher<T, reservedHeaderLen>::ToBBuffer(this, bb);
	}

	template<typename T, uint32_t reservedHeaderLen>
	int List<T, reservedHeaderLen>::FromBBuffer(BBuffer &bb)
	{
		return ListBBSwitcher<T, reservedHeaderLen>::FromBBuffer(this, bb);
	}


	inline String::String(BBuffer* bb)
		: BaseType(bb)
	{
	}
}
