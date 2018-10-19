#pragma once
#include "xxlib_defines.h"
#include "xxlib_mpobject.h"
#include "xxlib_list.h"
#include "xxlib_dict.h"
#include "xxlib_string.h"
#include <cstdint>

namespace xxlib
{
	struct BBuffer;

	/*******************************************************************************************************/
	// 类型id 映射( for 序列化指针 )
	/*******************************************************************************************************/

	// 0 用于表示空指针
	template<> struct TypeId<MPObject> { static const uint16_t value = 1; };
	template<> struct TypeId<String> { static const uint16_t value = 2; };
	template<> struct TypeId<BBuffer> { static const uint16_t value = 3; };
	// 当前还不支持 Dict 序列化.  PoolQueue ??
	constexpr uint16_t customTypeIdBegin = 4;


	// 凡是需要序列化的类, 都需要映射为 typeId. 值不要和上面的重复
	// struct TTTTTTT; namespace xxlib { template<> struct TypeId< TTTTTTT > { static const uint16_t value = customTypeIdBegin + __COUNTER__; }; }

	// 也可以用下面这个宏来
#define MapTypeId( typeName ) namespace xxlib { template<> struct TypeId<typeName> { static const uint16_t value = customTypeIdBegin + __COUNTER__; }; }



	// 一些 gcc 要求必须声明在外面的路由类
	template<bool>
	struct ByteBufferWriteSwitcher;
	template<>
	struct ByteBufferWriteSwitcher<false>;
	template<>
	struct ByteBufferWriteSwitcher<true>;


	/*******************************************************************************************************/
	// 支持指针的序列化器
	/*******************************************************************************************************/

	struct BBuffer : public List<char, false, 16>
	{
		typedef List<char, false, 16> BaseType;
		uint32_t									offset = 0;				// 读指针偏移量
		Dict<void*, uint32_t>*						ptrStore = nullptr;		// 临时记录 key: 指针, value: offset
		Dict<uint32_t, std::pair<void*, uint32_t>>*	idxStore = nullptr;		// 临时记录 key: 读offset, value: pair<ptr, typeId>

	protected:
		friend MemPool;
		BBuffer(MemPool&mp) : BaseType(mp) {}
		explicit BBuffer(uint32_t capacity = 0) : BaseType(capacity) {}
		~BBuffer()
		{
			// 这两货是不参与序列化的
			if (ptrStore)
			{
				ptrStore->Release();
				ptrStore = nullptr;
			}
			if (idxStore)
			{
				idxStore->Release();
				idxStore = nullptr;
			}
		}
		BBuffer(BBuffer const&o) = delete;
		BBuffer& operator=(BBuffer const&o) = delete;

	public:

		/*************************************************************************/
		// 传统 pod 读写系列( 通过 bytesutils 里的重载 / 特化 实现 )
		/*************************************************************************/

		friend ByteBufferWriteSwitcher<true>;
		friend ByteBufferWriteSwitcher<false>;

		template<typename ...TS>
		void Write(TS const & ...vs)
		{
			ByteBufferWriteSwitcher<ExistsMPObject<TS...>::value>::Exec(*this, vs...);
		}

		template<typename T>
		int Read(std::enable_if_t< IsMPObject<T>::value, T> &v)
		{
			return ReadPtr(v);
		}
		template<typename T>
		int Read(std::enable_if_t<!IsMPObject<T>::value, T> &v)
		{
			return BBReadFrom(buf, dataLen, offset, v);
		}

		template<typename T, typename ...TS>
		int Read(T &v, TS&...vs)
		{
			if (auto rtv = Read<T>(v)) return rtv;
			return Read(vs...);
		}
		int Read() { return 0; }


		// 直接追加写入一段 buf ( 并不记录长度 )
		void WriteBuf(char const* buf, uint32_t const& dataLen)
		{
			Reserve(this->dataLen + dataLen);
			memcpy(this->buf + this->dataLen, buf, dataLen);
			this->dataLen += dataLen;
		}

	protected:
		/*************************************************************************/
		// 指针序列化支持: WritePtr 系列
		/*************************************************************************/

		// MPObject 指针
		void WritePtr(MPObject const* v)
		{
			assert(ptrStore);
			if (WriteNull(v)) return;
			auto typeId = v->typeId();
			assert(typeId);
			Write((uint32_t)typeId);
			if (WriteOffset(v)) return;

			v->ToBBuffer(*this);
		}

		// MPtr<MPObject> 智指
		template<typename T>
		void WritePtr(MPtr<T> const &p)
		{
			WritePtr(p.Ensure());
		}

	public:
		// 一次如果只针对一个指针类做序列化, 这个函数会提供一些便利
		template<typename T>
		void WriteOnce(T const& v)
		{
			BeginWrite();
			Write(v);
			EndWrite();
		}
	private:

		template<typename T>
		bool WriteNull(T* p)
		{
			if (p) return false;
			Write((uint32_t)0);
			return true;
		}

		// 写入 offset 值, 返回是否不再需要继续填充内容( 已序列化过, 只需要填充 offset 即可 )
		template<typename T>
		bool WriteOffset(T* p)
		{
			auto rtv = ptrStore->Add((void*)p, dataLen);
			Write(ptrStore->ValueAt(rtv.index));
			return !rtv.success;
		}

		/*************************************************************************/
		// ReadPtr 系列
		/*************************************************************************/

		// MPObject* 指针类型: 创建实例后用 FromBBuffer 反序列化, Read 出错则清掉数据填充 nullptr
		template<typename T>
		int ReadPtr(T* & v)
		{
			// get typeid
			uint32_t tid;
			if (auto rtv = Read(tid)) return rtv;

			// isnull ?
			if (tid == 0)
			{
				v = nullptr;
				return 0;
			}

			// get offset
			uint32_t ptr_offset = 0, bb_offset_bak = offset;
			if (auto rtv = Read(ptr_offset)) return rtv;

			// fill or ref
			if (ptr_offset == bb_offset_bak)
			{
				// ensure inherit
				if (!mempool().IsBaseOf(TypeId<T>::value, tid)) return -2;

				// create instance
				v = (T*)mempool().CreatePtrByTypeId((uint16_t)tid);
				if (v == nullptr) return -3;

				// save to dict
				auto addResult = idxStore->Add(ptr_offset, std::make_pair(v, tid));
				if (!addResult.success) return -5;

				// try to fill data
				if (auto rtv = v->FromBBuffer(*this))
				{
					// fail: release resources
					idxStore->RemoveAt(addResult.index);
					mempool().Release(v);						// 这里会调用析构, 故析构需要适应还原了一半的数据长相
					v = nullptr;
					return rtv;
				}
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

		// 上述情况的 MPtr 包一层版( 主要区别为类型id 为负
		template<typename T>
		int ReadPtr(MPtr<T> & v)
		{
			T* p;
			if (auto rtv = ReadPtr(p))
			{
				v = nullptr;
				return rtv;
			}
			v = p;
			return 0;
		}

		// todo: 对于这种限制长度的功能需求, 考虑做面一个数组映射表, 即运行时往这个数组填充限制值, 容器自己去读. 不需要限制就改成 0. 从而避免多带参数
		// 另一个思路是 FromBBuffer 加参数. 不过每次传多余的参数略伤. 参数可以是一个限制结构体, 提供多层次的限定? 这部分具体还要再想下
		//int ReadPtrLimit(T* &v, uint32_t minLen, uint32_t maxLen)		// 这种函数可以在内部去改这个数组的值, 从而不需要多写具体反序列化重载.



	public:
#
		// 一次如果只针对一个指针类做反序列化, 这个函数会提供一些便利
		template<typename T>
		int ReadOnce(T& v)
		{
			BeginRead();
			auto rtv = Read(v);
			EndRead();
			return rtv;
		}


		/*************************************************************************/
		// 指针序列化支持: Begin End 系列
		/*************************************************************************/

		void BeginWrite()
		{
			if (!ptrStore) mempool().CreateTo(ptrStore, 16);
		}
		void EndWrite()
		{
			assert(ptrStore);
			ptrStore->Clear();
		}
		void BeginRead()
		{
			if (!idxStore) mempool().CreateTo(idxStore, 16);
		}
		void EndRead()
		{
			assert(idxStore);
			idxStore->Clear();
		}

		/*************************************************************************/
		// 实现序列化接口以序列化自己
		/*************************************************************************/

		virtual void ToBBuffer(BBuffer &bb) const override 
		{
			bb.Write(dataLen);
			bb.WriteBuf(buf, dataLen);
		}
		virtual int FromBBuffer(BBuffer &bb) override
		{
			uint32_t len = 0;
			if (auto rtv = bb.Read(len)) return rtv;
			if (bb.offset + len > bb.dataLen) return -1;
			WriteBuf(bb.buf + bb.offset, len);
			bb.offset += len;
			return 0;
		}


		/*************************************************************************/
		// 实现 ToString 接口
		/*************************************************************************/

		// 先简单来点
		virtual void ToString(String &str) const override
		{
			str.Append("{ \"typeName\" : \"BBuffer\", \"typeId\" : ", typeId(), ", \"refCount\" : ", refCount(), ", \"versionNumber\" : ", pureVersionNumber(), ", \"len\" : ", dataLen, ", \"data\" : [ ");
			for (size_t i = 0; i < dataLen; i++)
			{
				str.Append((int)(uint8_t)buf[i], ", ");
			}
			if (dataLen) str.dataLen -= 2;
			str.Append(" ] }");
		}

	};




	template<>
	struct ByteBufferWriteSwitcher<false>
	{
		template<typename ...TS>
		static void Exec(BBuffer& bb, TS const& ...vs)
		{
			bb.Reserve(bb.dataLen + BBCalc(vs...));
			bb.dataLen += BBWriteTo(bb.buf + bb.dataLen, vs...);
			assert(bb.dataLen <= bb.bufLen);
		}
	};

	template<>
	struct ByteBufferWriteSwitcher<true>
	{
		template<typename T>
		static void WriteTo(BBuffer& bb, std::enable_if_t< IsMPObject<T>::value, T> const& v)
		{
			bb.WritePtr(v);
		}
		template<typename T>
		static void WriteTo(BBuffer& bb, std::enable_if_t<!IsMPObject<T>::value, T> const& v)
		{
			bb.Reserve(bb.dataLen + BBCalc(v));
			bb.dataLen += BBWriteTo(bb.buf + bb.dataLen, v);
			assert(bb.dataLen <= bb.bufLen);
		}
		template<typename ...TS>
		static void Exec(BBuffer& bb, TS const& ...vs)
		{
			std::initializer_list<int>{ (WriteTo<TS>(bb, vs), 0)... };
		}
	};

}
