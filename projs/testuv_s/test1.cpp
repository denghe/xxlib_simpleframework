#include <xx_mempool.h>
#include <xx_bbuffer.h>
#include <xx_queue.h>

namespace xx
{
	// 包队列。提供按字节数零散 pop 的功能
	// 包直接使用 BBuffer 来实现, 指针方式使用, 走引用计数删除
	struct BBufferQueue : protected Queue<BBuffer*>
	{
		typedef Queue<BBuffer*> BaseType;
		size_t numPopBufs = 0;          // 已 pop buf 个数

		size_t numPushLen = 0;          // 已 push 字节数 for 统计 / 限速啥的
		size_t numPopLen = 0;           // 已 pop 字节数 for 统计 / 限速啥的

		size_t bufIndex = 0;            // 待发 buf 索引( 减去 numPopBufs 才是 bufs 下标 )
		size_t byteOffset = 0;          // 待发 buf 的待发内容起始索引

		BBufferQueue(BBufferQueue const& o) = delete;
		BBufferQueue& operator=(BBufferQueue const& o) = delete;

		explicit BBufferQueue(uint32_t capacity = 8)
			: BaseType(capacity)
		{}

		BBufferQueue(BBufferQueue &&o)
			: BaseType((BaseType&&)o)
		{}

		~BBufferQueue()
		{
			Clear();
		}

		// todo: Add

		// 如果队列尾包( 存在的话 )，非正在发送状态 且 引用计数为 1, 就返回它( 可用于继续填充 ). 否则返回空.
		// 使用完这个函数之后, 需要修正 numPushLen 的值. 示例:
		/*
		auto buf = q->TryRefLastBB();
		auto bak_dataLen = 0;
		if (buf)
		{
			bak_dataLen = buf->dataLen;
		}
		else
		{
			buf = mp.Create<BBuffer>();
		}

		// fill buf

		if (bak_dataLen)
		{
			q->numPushLen += buf->dataLen - bak_dataLen;
		}
		else
		{
			q->Add(buf);
		}
		*/
		BBuffer* const& TryRefLastBB()
		{
			if (this->Count() + numPopBufs > bufIndex && this->Last()->refCount() == 1)
			{
				return this->Last();
			}
			return nullptr;
		}

		// 弹出指定字节长度到指定容器( [ { buf, len }, ... ] 格式 )
		// 下次调用时将清理上一次的内存 以确保数据持有到发送成功. 也就是说只有发送成功之后才能再次调用该函数.
		// 如果发送失败, 要重试, outBufs 的值是可以反复使用的.
		template<typename T, uint32_t maxBufsCount = 64>
		size_t PopBytesTo(List<T>* const& outBufs, size_t len)
		{
			//outBufs->Clear();
			//if (!len)
			//{
			//	return 0;
			//}

			//if (bufIndex != numPopBufs)     // 清理内存
			//{
			//	for (size_t i = 0; i < bufIndex - numPopBufs; i++)
			//	{
			//		bufPool.Free(std::move(bufs.front()));
			//		bufs.pop_front();
			//	}
			//	numPopBufs = bufIndex;
			//}

			//auto maxIdx = min(maxBufsCount, bufs.size());    // 听说批量发送指令一般只支持最多 64 段数据
			//auto idx = bufIndex - numPopBufs;
			//if (idx >= maxIdx)
			//{
			//	return 0;
			//}

			//auto bak_len = len;
			//while (len > 0 && idx < maxIdx)
			//{
			//	auto& p = bufs[idx];
			//	auto left = p.dataLen() - byteOffset;
			//	if (left <= len)
			//	{
			//		len -= left;
			//		++idx;
			//		outBufs->Add({ p.buf() + byteOffset, left });
			//		byteOffset = 0;
			//	}
			//	else
			//	{
			//		outBufs->Add({ p.buf() + byteOffset, len });
			//		byteOffset += len;
			//		len = 0;
			//	}
			//}
			//bufIndex = idx + numPopBufs;
			//numPopLen += bak_len - len;
			//return bak_len - len;
		}

		size_t BytesCount()
		{
			return numPushLen - numPopLen;
		}
	};
}

int main()
{

	return 0;
}
