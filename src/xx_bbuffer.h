#pragma once
#include "xx_bytesutils.h"
#include "xx_mempool.h"
#include "xx_list.h"
#include "xx_dict.h"

namespace xx
{
	struct BBuffer : public List<char, false, 16>
	{
		typedef List<char, false, 16> BaseType;
		uint32_t offset = 0;													// 读指针偏移量

		explicit BBuffer(uint32_t capacity = 0) : BaseType(capacity) {}
		BBuffer(BBuffer const&o) = delete;
		BBuffer& operator=(BBuffer const&o) = delete;

		/*************************************************************************/
		// 传统 pod 读写系列( 通过 bytesutils 里的重载 / 特化 实现 )
		/*************************************************************************/

		template<typename T>
		void Write(T const& v)
		{
			this->Reserve(this->dataLen + BBCalc(v));
			this->dataLen += BBWriteTo(this->buf + this->dataLen, v);
			assert(this->dataLen <= this->bufLen);
		}
		template<typename T>
		int Read(T &v)
		{
			return BBReadFrom(buf, dataLen, offset, v);
		}

		template<typename ...TS>
		void WriteMulti(TS const& ...vs)
		{
			this->Reserve(this->dataLen + BBCalc(vs...));
			this->dataLen += BBWriteTo(this->buf + this->dataLen, vs...);
			assert(this->dataLen <= this->bufLen);
		}
		template<typename T, typename ...TS>
		int ReadMulti(T &v, TS&...vs)
		{
			if (auto rtv = Read<T>(v)) return rtv;
			return Read(vs...);
		}
		int Read() { return 0; }


		/*************************************************************************/
		//  MPObject 对象读写系列
		/*************************************************************************/

		// todo: 要实现这部分需要解决运行时向 MemPool 注册类型的问题

		//Dict_v<void*, uint32_t>*						ptrStore = nullptr;		// 临时记录 key: 指针, value: offset
		//Dict_v<uint32_t, std::pair<void*, uint16_t>>*	idxStore = nullptr;		// 临时记录 key: 读offset, value: pair<ptr, typeId>

		//// todo: Begin End WriteRef ReadRef
		//void BeginWrite()
		//{
		//	if (!ptrStore) this->mempool()->CreateTo(ptrStore, 16);
		//}
		//void EndWrite()
		//{
		//	assert(ptrStore);
		//	ptrStore->Clear();
		//}
		//void BeginRead()
		//{
		//	if (!idxStore) mempool().CreateTo(idxStore, 16);
		//}
		//void EndRead()
		//{
		//	assert(idxStore);
		//	idxStore->Clear();
		//}

		//void Write(MPObject* const& v)
		//{
		//	assert(ptrStore);
		//}
		//int Read(MPObject* &v)
		//{
		//	return 0;
		//}

		/*************************************************************************/
		//  其他工具函数
		/*************************************************************************/

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

	};


	/*************************************************************************/
	// 实现值类型使用类型声明
	/*************************************************************************/

	using BBuffer_v = MemHeaderBox<BBuffer>;

}
