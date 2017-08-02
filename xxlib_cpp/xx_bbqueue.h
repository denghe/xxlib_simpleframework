#pragma once
#include "xx_mempool.h"
#include "xx_bbuffer.h"
#include "xx_queue.h"

namespace xx
{
	// 包队列。提供按字节数零散 pop 的功能
	// 包直接使用 BBuffer 来实现, 指针方式使用, 走引用计数删除
	struct BBQueue : protected Queue<BBuffer*>
	{
		typedef Queue<BBuffer*> BaseType;
		uint32_t numPopBufs = 0;          // 已 pop buf 个数

		uint32_t numPushLen = 0;          // 已 push 字节数 for 统计 / 限速啥的
		uint32_t numPopLen = 0;           // 已 pop 字节数 for 统计 / 限速啥的

		uint32_t bufIndex = 0;            // 待发 buf 索引( 减去 numPopBufs 才是 bufs 下标 )
		uint32_t byteOffset = 0;          // 待发 buf 的待发内容起始索引

		BBQueue(BBQueue const& o) = delete;
		BBQueue& operator=(BBQueue const& o) = delete;

		explicit BBQueue(uint32_t capacity = 8)
			: BaseType(capacity)
		{
			mempool().CreateTo(ptrStore);
			mempool().CreateTo(idxStore);
		}

		BBQueue(BBQueue &&o)
			: BaseType((BaseType&&)o)
		{
		}

		~BBQueue()
		{
			Clear();
			mempool().SafeRelease(ptrStore);
			mempool().SafeRelease(idxStore);
		}

		// 公用, 移到新创建的 BB 中, Push 时移回.
		Dict<void*, uint32_t>*						ptrStore = nullptr;
		Dict<uint32_t, std::pair<void*, uint16_t>>*	idxStore = nullptr;

		// 创建一个 BBuffer 返回( 将填充公用 ptrStore & idxStore )
		BBuffer* CreateBB(uint32_t const& capacity = 0)
		{
			auto bb = mempool().Create<BBuffer>(capacity);
			std::swap(bb->ptrStore, ptrStore);
			std::swap(bb->idxStore, idxStore);
			return bb;
		}

		// 如果队列尾包存在，且 非正在发送状态 且 引用计数为 1( 非群发 ), 就返回它用于继续填充. 否则用当前内存池新建一个返回.
		BBuffer* PopLastBB(uint32_t const& capacity = 0)
		{
			if (Count() + numPopBufs > bufIndex && Last()->refCount() == 1)
			{
				numPushLen -= Last()->dataLen;
				auto bb = Last();
				PopLast();
				std::swap(bb->ptrStore, ptrStore);
				std::swap(bb->idxStore, idxStore);
				return bb;
			}
			return CreateBB(capacity);
		}

		// 将待发数据 bb 压入队列托管( 将同步相应的统计数值, PopTo 后将自动 Release ), 之后不可以再继续操作 bb
		void Push(BBuffer* const& bb)
		{
			numPushLen += bb->dataLen;
			std::swap(bb->ptrStore, ptrStore);
			std::swap(bb->idxStore, idxStore);
			this->BaseType::Push(bb);
		}

		// 弹出指定字节长度到指定容器( [ { len, bufPtr,  }, ... ] 格式 ), 返回实际弹出字节数
		// 下次调用时将清理上一次的内存 以确保数据持有到发送成功. 也就是说只有发送成功之后才能再次调用该函数.
		// 如果发送失败, 要重试, outBufs 的值是可以反复使用的.
		template<typename T, uint32_t maxBufsCount = 64>
		uint32_t PopTo(List<T>& outBufs, uint32_t len)
		{
			outBufs.Clear();
			if (!len) return 0;

			if (bufIndex != numPopBufs)							// 清理内存
			{
				for (uint32_t i = 0; i < bufIndex - numPopBufs; i++)
				{
					Top()->Release();
					Pop();
				}
				numPopBufs = bufIndex;
			}

			auto maxIdx = MIN(maxBufsCount, Count());		// 听说批量发送指令一般只支持最多 64 段数据
			auto idx = uint32_t(bufIndex - numPopBufs);
			if (idx >= maxIdx) return 0;						// 没有数据要发

			auto bak_len = len;
			while (len > 0 && idx < maxIdx)
			{
				auto& bb = At(idx);
				auto left = bb->dataLen - byteOffset;
				if (left <= len)
				{
					len -= left;
					++idx;
					outBufs.Add(BufMaker<T>::Make(bb->buf + byteOffset, left));
					byteOffset = 0;
				}
				else
				{
					outBufs.Add(BufMaker<T>::Make(bb->buf + byteOffset, len));
					byteOffset += len;
					len = 0;
				}
			}
			bufIndex = idx + numPopBufs;
			numPopLen += bak_len - len;
			return bak_len - len;
		}

		// 清光所有数据, 释放所有 bb
		void Clear()
		{
			while (!Empty())
			{
				Top()->Release();
				Pop();
			}
			numPopBufs = 0;
			numPopLen = 0;
			bufIndex = 0;
			byteOffset = 0;
		}

		// 获取当前还有多少字节的数据待发
		uint32_t BytesCount()
		{
			return numPushLen - numPopLen;
		}
	};




	/*************************************************************************/
	// 值类型使用形态包装
	/*************************************************************************/

	using BBQueue_v = MemHeaderBox<BBQueue>;
	using BBQueue_p = Ptr<BBQueue>;

	template<>
	struct MemmoveSupport<BBQueue_v>
	{
		static const bool value = true;
	};

}
