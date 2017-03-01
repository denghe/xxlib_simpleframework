#pragma once
#include "xx_podstack.h"
#include "xx_mpobject.h"
#include <array>
#include <vector>
#include <cassert>
#include <cstring>

namespace xx
{
	// 内存池基类. 提供类型无关的内存分配 / 回收功能
	struct MemPoolBase
	{
		// stacks 数组
		std::array<PodStack<void*>, sizeof(size_t) * 8> stacks;

		// 自增版本号( 每次创建时先 ++ 再填充 )
		uint64_t versionNumber = 0;

		// 池内存回收
		~MemPoolBase()
		{
			void* p;
			for (auto& stack : stacks)
			{
				while (stack.TryPop(p)) std::free(p);
			}
		}

		/***********************************************************************************/
		// 内存分配( malloc / free 系列. 主要供一些容器类的代码使用 )
		/***********************************************************************************/

		// 该操作将会在头部区域填充 MemHeader_VersionNumber 并跳过, 返回偏移后的指针
		// 最大化内存利用率的 size 计算: Round2n(capacity * sizeof(T) + 8) - 8
		inline void* Alloc(size_t siz)
		{
			siz += sizeof(MemHeader_VersionNumber);								// 空出放置 MemHeader_VersionNumber 的地儿
			auto idx = Calc2n(siz);
			if (siz > (size_t(1) << idx)) siz = size_t(1) << ++idx;

			void* rtv;
			if (!stacks[idx].TryPop(rtv)) rtv = malloc(siz);

			auto p = (MemHeader_VersionNumber*)rtv;
			p->versionNumber = (++versionNumber) | ((uint64_t)idx << 56);		// 将数组下标附在最高位上, Free 要用
			return p + 1;														// 指向 header 后面的区域返回
		}

		inline void Free(void* p)
		{
			if (!p) return;
			auto& h = MemHeader_VersionNumber::Visit(p);						// 指到内存头
			assert(h.versionNumber);											// 理论上讲 free 的时候其版本号不应该是 0. 否则就涉嫌重复 Free
			stacks[h.mpIndex].Push(&h);											// 入池
			h.versionNumber = 0;												// 清空版本号
		}

		// dataLen 表示要复制多少字节数到新的内存. 并不代表 p 的原始长度
		inline void* Realloc(void *p, size_t newSize, size_t dataLen = std::numeric_limits<size_t>::max())
		{
			assert(!p || (p && dataLen));
			auto rtv = Alloc(newSize);
			if (p)
			{
				auto oldSize = (size_t(1) << ((MemHeader_VersionNumber*)p - 1)->mpIndex) - sizeof(MemHeader_VersionNumber);
				memcpy(rtv, p, std::min(oldSize, dataLen));
				Free(p);
			}
			return rtv;
		}

		/***********************************************************************************/
		// 内存分配( Create / Release 系列 ). 仅针对派生自 MPObject 的对象
		/***********************************************************************************/

		// Create<T> 函数在 MemPool 类中

		// 释放由 Create 创建的类
		inline void Release(MPObject* p)
		{
			if (!p) return;
			assert(p->versionNumber());
			if (p->refCount() == 0 || --p->refCount()) return;
			p->~MPObject();

			auto& h = MemHeader_MPObject::Visit(p);								// 指到内存头
			assert(h.versionNumber);											// 理论上讲 free 的时候其版本号不应该是 0. 否则就涉嫌重复 Free
			stacks[h.mpIndex].Push(&h);											// 入池
			h.versionNumber = 0;												// 清空版本号
		}

	};


	/***********************************************************************************/
	// 这个函数要用到 MemPool 的功能故实现写在这里
	/***********************************************************************************/
	inline void MPObject::Release()
	{
		mempoolbase().Release(this);
	}
}
