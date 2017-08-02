#pragma once
#include "xx_mempool.h"
#include <memory>

namespace xx
{
	// 经由 Dict 简化而来的双向链表
	template <typename T>
	struct Links : public MPObject
	{
		typedef T ValueType;
		struct Node
		{
			int             prev;
			int             next;
			T               value;					// 同时也用于存空内存单元下标( 避免使用 next 以便于 RemoveAt 后 next 还能读 方便 foreach 中删 )
		};

		int                 freeList;               // 自由空间链表头( *(int*)&nodes[index].value 指向下一个未使用单元 )
		int                 freeCount;              // 自由空间链长
		int                 count;                  // 已使用空间数
		int					header;					// 存第1个节点的下标
		int					tail;					// 尾巴下标
		int                 nodesLen;				// 节点数组长度
		Node               *nodes;                  // 节点数组

		explicit Links(int capacity = 16)
		{
			freeList = -1;
			freeCount = 0;
			count = 0;
			header = -1;
			tail = -1;
			auto nodesByteLen = Round2n(capacity * sizeof(Node) + sizeof(MemHeader_VersionNumber)) - sizeof(MemHeader_VersionNumber);
			nodesLen = (int)(nodesByteLen / sizeof(Node));
			nodes = (Node*)mempool().Alloc(nodesLen * sizeof(Node));
		}
		~Links()
		{
			if (nodes)
			{
				DeleteNodes();
				mempool().Free(nodes);
				nodes = nullptr;
			}
		}
		// todo: 右值复制构造

		Links(Links const &o) = delete;
		Links& operator=(Links const &o) = delete;

	public:

		// 追加到尾部
		template<typename ...VPS>
		int EmplaceBack(VPS &&... vps)
		{
			int index = EmplaceCore(std::forward<VPS>(vps)...);

			nodes[index].prev = tail;
			nodes[index].next = -1;
			if (tail != -1)
			{
				nodes[tail].next = index;
			}
			tail = index;
			if (header == -1) header = index;

			return index;
		}

		// 插入到头部
		template<typename ...VPS>
		int EmplaceFront(VPS &&... vps)
		{
			int index = EmplaceCore(std::forward<VPS>(vps)...);

			nodes[index].prev = -1;
			nodes[index].next = header;
			if (header != -1)
			{
				nodes[header].prev = index;
			}
			header = index;
			if (tail == -1) tail = index;

			return index;
		}

		int PushBack(T && v)
		{
			return EmplaceBack((T&&)v);
		}
		int PushBack(T const& v)
		{
			return EmplaceBack(v);
		}

		int PushFront(T && v)
		{
			return EmplaceFront((T&&)v);
		}
		int PushFront(T const& v)
		{
			return EmplaceFront(v);
		}

		// 将 v 插入到 tarIdx 的后面( tarIdx 为 -1 则插入到最前面 ), 返回内存块地址索引
		template<typename VT>
		int InsertAt(int tarIdx, VT && v)
		{
			assert(tarIdx >= 0 && tarIdx < count && nodes[tarIdx].prev != -2);
			assert(header == tarIdx && nodes[tarIdx].prev == -1 || header != tarIdx);
			assert(tail == tarIdx && nodes[tarIdx].next == -1 || tail != tarIdx);
			assert(header == tail && tarIdx == header && Count() == 1 || header != tail);

			if (tarIdx == -1) return PushFront(std::forward<VT>(v));
			else if (tarIdx == tail) return PushBack(std::forward<VT>(v));
			else
			{
				int index = EmplaceCore(std::forward<VT>(v));
				nodes[index].prev = tarIdx;
				nodes[index].next = nodes[tarIdx].next;
				if (nodes[tarIdx].next != -1)
				{
					nodes[nodes[tarIdx].next].prev = index;
				}
				nodes[tarIdx].next = index;
				return index;
			}
		}

		// todo: Insert Range?


		void RemoveAt(int index)
		{
			assert(index >= 0 && index < count && nodes[index].prev != -2);
			assert(header == index && nodes[index].prev == -1 || header != index);
			assert(tail == index && nodes[index].next == -1 || tail != index);
			assert(header == tail && index == header && Count() == 1 || header != tail);

			if (nodes[index].next != -1)
			{
				nodes[nodes[index].next].prev = nodes[index].prev;
			}
			if (index == header) header = nodes[index].next;

			if (nodes[index].prev != -1)
			{
				nodes[nodes[index].prev].next = nodes[index].next;
			}
			if (index == tail) tail = nodes[index].prev;

			nodes[index].value.T::~T();
			*(int*)&nodes[index].value = freeList;     // 当前节点已被移出链表, 令其 value 存自由节点链表头下标
			freeList = index;
			freeCount++;

		}

		// 遍历查找移除
		int Remove(T const& v)
		{
			for (int i = header; i != -1; i = nodes[i].next)
			{
				if (EqualsTo(nodes[i].value, v))
				{
					RemoveAt(i);
					return i;
				}
			}
			return -1;
		}

		// 只支持没数据时扩容或空间用尽扩容( 如果不这样限制, 扩容时的 遍历损耗 略大 )
		void Reserve(int capacity = 0)
		{
			assert(count == 0 || count == nodesLen);          // 确保扩容函数使用情型
			if (capacity == 0) capacity = count * 2;            // 2倍扩容
			if (capacity <= nodesLen) return;
			auto nodesByteLen = Round2n(capacity * sizeof(Node) + sizeof(MemHeader_VersionNumber)) - sizeof(MemHeader_VersionNumber);	// 规避写 versionNumber 的区域
			nodesLen = (int)(nodesByteLen / sizeof(Node));

			if (std::is_trivial<T>::value || MemmoveSupport_v<T>)
			{
				nodes = (Node*)mempool().Realloc(nodes, nodesByteLen);
			}
			else
			{
				auto newNodes = (Node*)mempool().Alloc(nodesByteLen);
				for (int i = 0; i < count; ++i)
				{
					new (&newNodes[i].value) T((T&&)nodes[i].value);
					nodes[i].value.T::~T();
				}
				mempool().Free(nodes);
				nodes = newNodes;
			}
		}

		int Find(T const &v) const
		{
			for (int i = header; i != -1; i = nodes[i].next)
			{
				if (EqualsTo(nodes[i].value, v))
				{
					return i;
				}
			}
			return -1;
		}

		// 可传入一个资源回收函数来搞事
		void Clear()
		{
			if (!count) return;
			DeleteNodes();
			header = -1;
			tail = -1;
			freeList = -1;
			freeCount = 0;
			count = 0;
		}

	public:

		int Count() const
		{
			return int(count - freeCount);
		}

		bool Empty()
		{
			return count == 0;
		}

		// todo: SetAt?

		T const& At(int index) const
		{
			return const_cast<Links*>(this)->IndexAtValue(index);
		}
		Node const& NodeAt(int index) const
		{
			return const_cast<Links*>(this)->At(index);
		}

#ifndef NDEBUG
		bool IndexExists(int index) const
		{
			return index >= 0 && index < count && nodes[index].prev != -2;
		}
#endif


		/*******************************************************************************/
		// 只是为了能 for( auto &c : 
		/*******************************************************************************/

		// 注意: 如果要在 for 的过程中 RemoveAt + Push, 直接用 iter 并不安全.
		// 另: 如果 freeCount 为 0, 则表示内存无空洞. 无序遍历的话可直接扫数组. 以 count 为个数限定
		/*
		if (!list->freeCount)
		{
			// 乱序但扫得最快. 适合扫找之类.
			for( int i = 0; i < list->count; ++i )
			{
				list->nodes[i].....
			}
		}
		else
		{
			// 顺序, 删 + 增 安全
			auto i = list->header;
			while (i != -1)
			{
				auto nexti = list->nodes[i].next;	// 先读出 next 以避免当前节点失效
				// remove at ? add ? ...
				i = nexti;							// 继续扫
			}
		}
		// 允许 RemoveAt(i) 但不可以立即再 Push/Insert 啥的
		for (auto& i : *list) list->nodes[i]........
		*/


		struct Iter
		{
			Links& pl;
			int i;
			bool operator!=(Iter const& other) { return i != other.i; }
			Iter& operator++()
			{
				if (i != -1) i = pl.nodes[i].next;
				return *this;
			}
			int& operator*() { return i; }
		};
		Iter begin()
		{
			return Iter{ *this, header };
		}
		Iter end()
		{
			return Iter{ *this, -1 };
		}




		/*************************************************************************/
		// 实现 ToString 接口
		/*************************************************************************/

		virtual void ToString(String &str) const override;



	protected:

		template<typename ...VPS>
		int EmplaceCore(VPS &&... vps)
		{
			int index;
			if (freeCount > 0)                          // 如果 自由节点链表 不空, 取一个来当容器
			{                                           // 这些节点来自 Remove 操作. value 指向下一个
				index = freeList;
				freeList = *(int*)&nodes[index].value;
				freeCount--;
			}
			else
			{
				if (count == nodesLen)					// 所有空节点都用光了, Resize
				{
					Reserve();
				}
				index = count;							// 指向 Resize 后面的空间起点
				count++;
			}

			new (&nodes[index].value) T(std::forward<VPS>(vps)...);
			return index;
		}

		void DeleteNodes()                                    // 用于 析构, Clear
		{
			for (int i = header; i != -1; i = nodes[i].next)
			{
				nodes[i].value.T::~T();
#ifndef NDEBUG
				nodes[i].prev = -2;
#endif
			}
		}

	};


	/*************************************************************************/
	// 实现值类型使用类型声明
	/*************************************************************************/

	template <typename T>
	using Links_v = MemHeaderBox<Links<T>>;
	template <typename T>
	using Links_p = Ptr<Links<T>>;

	template <typename T>
	struct MemmoveSupport<Links_v<T>>
	{
		static const bool value = true;
	};
}
