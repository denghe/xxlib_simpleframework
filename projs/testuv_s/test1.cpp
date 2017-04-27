//#include <xx_mempool.h>
//#include <xx_bbuffer.h>
//
//namespace xx
//{
//	// 包队列。提供按字节数零散 pop 的功能
//	template<class OutType>
//	struct BBufferQueue
//	{
//		// todo: xx_queue
//
//		//std::deque<BBuffer> bufs;
//		size_t numPopBufs = 0;          // 已 pop buf 个数
//
//		size_t numPushLen = 0;          // 已 push 字节数 for 统计 / 限速啥的
//		size_t numPopLen = 0;           // 已 pop 字节数 for 统计 / 限速啥的
//
//		size_t bufIndex = 0;            // 待发 buf 索引( 减去 numPopBufs 才是 bufs 下标 )
//		size_t byteOffset = 0;          // 待发 buf 的待发内容起始索引
//
//		BufPool<bufLen> &bufPool;
//		BBufferQueue(BufPool<bufLen> &bufPool) : bufPool(bufPool) {}
//		~BBufferQueue()
//		{
//			Clear();
//		}
//
//
//		// 如果最后压的那个包 剩余空间还够，且非正在发送状态, 就取出并返回它继续填. 否则还是从池中分配
//		BufType Alloc(size_t maxDataLen)
//		{
//			assert(BufType::bufLen >= maxDataLen);
//			if (maxDataLen && bufs.size() + numPopBufs > bufIndex)
//			{
//				auto& p = bufs.back();
//				if (maxDataLen + p.dataLen() <= BufType::bufLen)
//				{
//					numPushLen -= p.dataLen();
//					auto r = std::move(bufs.back());
//					bufs.pop_back();
//					return r;
//				}
//			}
//			return bufPool.Alloc();
//		}
//		void Free(BufType &&p)
//		{
//			bufPool.Free(std::move(p));
//		}
//
//
//		template<typename ...PKGS>
//		void Push(PKGS&&...vs)
//		{
//			std::initializer_list<int>{ (
//				numPushLen += vs.dataLen(),
//				bufs.push_back(std::forward<PKGS>(vs)),
//				0)... };
//		}
//
//		// 下次调用时将清理上一次的内存 以确保数据 host 到发送成功. 也就是说只有发送成功之后才能再次调用.
//		// 如果发送失败, 要重试, outs 的值是可以反复使用的.
//		size_t PopTo(std::vector<OutType>& outs, size_t len)
//		{
//			outs.clear();
//			if (!len)
//			{
//				return 0;
//			}
//
//			if (bufIndex != numPopBufs)     // 清理内存
//			{
//				for (size_t i = 0; i < bufIndex - numPopBufs; i++)
//				{
//					bufPool.Free(std::move(bufs.front()));
//					bufs.pop_front();
//				}
//				numPopBufs = bufIndex;
//			}
//
//			auto maxIdx = min(64, bufs.size());    // 听说批量发送指令一般只支持最多 64 段数据
//			auto idx = bufIndex - numPopBufs;
//			if (idx >= maxIdx)
//			{
//				return 0;
//			}
//
//			auto bak_len = len;
//			while (len > 0 && idx < maxIdx)
//			{
//				auto& p = bufs[idx];
//				auto left = p.dataLen() - byteOffset;
//				if (left <= len)
//				{
//					len -= left;
//					++idx;
//					outs.push_back(BufQueue_MakeOutType<OutType>(p.buf() + byteOffset, left));
//					byteOffset = 0;
//				}
//				else
//				{
//					outs.push_back(BufQueue_MakeOutType<OutType>(p.buf() + byteOffset, len));
//					byteOffset += len;
//					len = 0;
//				}
//			}
//			bufIndex = idx + numPopBufs;
//			numPopLen += bak_len - len;
//			return bak_len - len;
//		}
//
//		void Clear()
//		{
//			for (auto& buf : bufs)
//			{
//				bufPool.Free(std::move(buf));
//			}
//			bufs.clear();
//			numPopBufs = numPopLen = bufIndex = byteOffset = 0;
//		}
//
//		bool Empty()
//		{
//			return bufs.empty();
//		}
//
//		size_t BytesCount()
//		{
//			return numPushLen - numPopLen;
//		}
//	};
//}
//
//int main()
//{
//
//	return 0;
//}
