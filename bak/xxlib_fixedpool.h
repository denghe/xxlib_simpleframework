#pragma once
#include "xxlib_list.h"

namespace xxlib
{
	template<typename T>
	struct FixedPool : public List<T>
	{
		static_assert(std::is_pod<T>::value && sizeof(T) >= sizeof(int), "");
		typedef List<T> BaseType;

		int         freeList;					// 自由空间链表头( next 指向下一个未使用单元 )
		int         freeCount;					// 已 Free 数( 如果 Alloc 是从 freeList 分配, 该值将 -1 )

	protected:
		friend MemPool;

		explicit FixedPool(int capacity = 16)
			: BaseType((int)Round2n(capacity))
			, freeList(-1)
			, freeCount(0)
		{
		}

		FixedPool(FixedPool const &o) = delete;
		FixedPool operator=(FixedPool const &o) = delete;

	public:
		int Alloc()
		{
			int index;
			if (freeCount > 0)                  // 如果 自由节点链表 不空, 取一个来当容器
			{
				index = freeList;
				freeList = *(int*)&this->At(index);
				freeCount--;
			}
			else
			{
				if (this->dataLen == this->bufLen)          // 所有空节点都用光了, 扩容
				{
					Reserve(this->dataLen + 1);
				}
				index = this->dataLen;
				this->dataLen++;
			}
			return index;
		}

		void Free(int index)
		{
			*(int*)&this->At(index) = freeList;
			freeList = index;
			freeCount++;
		}

		int Size() const
		{
			return this->dataLen - freeCount;
		}
	};
}
