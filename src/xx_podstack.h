#pragma once
#include "xx_defines.h"

namespace xx
{
	// 简单粗暴, 纯用于内存池的基础组件, 只包含必要元素
	template<typename T>
	struct PodStack
	{
		static_assert(std::is_pod<T>::value, "the T must be POD");

		T*			buf;
		uint32_t	bufLen;
		uint32_t	dataLen;
		explicit PodStack(uint32_t capacity = 0)
		{
			if (capacity == 0)
			{
				buf = nullptr;
				bufLen = 0;
			}
			else
			{
				auto bufByteLen = Round2n(capacity * sizeof(T));
				buf = (T*)malloc(bufByteLen);
				bufLen = uint32_t(bufByteLen / sizeof(T));
			}
			dataLen = 0;
		}
		PodStack(PodStack const &o) = delete;
		PodStack& operator=(PodStack const &o) = delete;
		~PodStack()
		{
			Clear(true);
		}
		void Reserve(uint32_t capacity)
		{
			if (capacity <= bufLen) return;
			auto newBufByteLen = Round2n(capacity * sizeof(T));
			buf = (T*)realloc(buf, newBufByteLen);
			bufLen = uint32_t(newBufByteLen / sizeof(T));
		}
		void Resize(uint32_t len)
		{
			Reserve(len);
			dataLen = len;
		}
		T const& operator[](uint32_t idx) const
		{
			assert(idx < dataLen);
			return buf[idx];
		}
		T& operator[](uint32_t idx)
		{
			assert(idx < dataLen);
			return buf[idx];
		}
		bool TryPop(T& output)
		{
			if (!dataLen) return false;
			output = buf[--dataLen];
			return true;
		}
		void Clear(bool freeBuf = false)
		{
			if (!buf) return;
			dataLen = 0;
			if (freeBuf)
			{
				::free(buf);
				buf = nullptr;
				bufLen = 0;
			}
		}
		void Push(T const& v)
		{
			Reserve(dataLen + 1);
			buf[dataLen++] = v;
		}
	};

}
