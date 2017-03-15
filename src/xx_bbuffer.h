#pragma once
#include "xx_list.h"

namespace xx
{
	struct BBuffer : public List<char, false, 16>
	{
		typedef List<char, false, 16> BaseType;
		uint32_t offset = 0;												// 读指针偏移量

		explicit BBuffer(uint32_t capacity = 0) : BaseType(capacity) {}
		BBuffer(BBuffer const&o) = delete;
		BBuffer& operator=(BBuffer const&o) = delete;

		/*************************************************************************/
		// 传统 pod 读写系列( 通过 bytesutils 里的重载 / 特化 实现 )
		/*************************************************************************/

		template<typename ...TS>
		void Write(TS const & ...vs)
		{
			this->Reserve(this->dataLen + BBCalc(vs...));
			this->dataLen += BBWriteTo(this->buf + this->dataLen, vs...);
			assert(this->dataLen <= this->bufLen);
		}

		template<typename T>
		int Read(T &v)
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
			this->Reserve(this->dataLen + dataLen);
			std::memcpy(this->buf + this->dataLen, buf, dataLen);
			this->dataLen += dataLen;
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

}
