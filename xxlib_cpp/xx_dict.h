#pragma once
#include "xx_hashutils.h"
#include "xx_mempool.h"

namespace xx
{
	// Dict.Add 的操作结果
	struct DictAddResult
	{
		bool success;
		int index;
	};


	// 翻抄自 .net 的 Dictionary 的代码
	// 内存优化原则: 内存集中以 亲和 cpu cache( hashCode, next 与 key value prev 分离 )
	template <typename TK, typename TV>
	struct Dict : public MPObject
	{
		typedef TK KeyType;
		typedef TV ValueType;

		struct Node
		{
			unsigned int    hashCode;
			int             next;
		};
		struct Data
		{
			TK              key;
			TV              value;
			int             prev;
		};

		int                 freeList;               // 自由空间链表头( next 指向下一个未使用单元 )
		int                 freeCount;              // 自由空间链长
		int                 count;                  // 已使用空间数
		int                 bucketsLen;             // 桶数组长( 质数 )
		int                *buckets;                // 桶数组
		Node               *nodes;                  // 节点数组
		Data               *items;                  // 数据数组( 与节点数组同步下标 )

		explicit Dict(int capacity = 16)
		{
			freeList = -1;
			freeCount = 0;
			count = 0;
			bucketsLen = (int)GetPrime(capacity, sizeof(Data));
			buckets = (int*)mempool().Alloc(bucketsLen * sizeof(int));
			memset(buckets, -1, bucketsLen * sizeof(int));  // -1 代表 "空"
			nodes = (Node*)mempool().Alloc(bucketsLen * sizeof(Node));
			items = (Data*)mempool().Alloc(bucketsLen * sizeof(Data));
		}
		~Dict()
		{
			DeleteKVs();
			mempool().Free(buckets);
			mempool().Free(nodes);
			mempool().Free(items);
		}

		Dict(Dict const &o) = delete;
		Dict& operator=(Dict const &o) = delete;

		template<typename K, typename ...VPS>
		DictAddResult Emplace(bool override, K &&key, VPS &&... vps)
		{
			assert(bucketsLen);

			// hash 按桶数取模 定位到具体 链表, 扫找
			auto hashCode = GetHashCode(key);
			auto targetBucket = hashCode % bucketsLen;
			for (int i = buckets[targetBucket]; i >= 0; i = nodes[i].next)
			{
				if (nodes[i].hashCode == hashCode && EqualsTo(items[i].key, key))
				{
					if (override)                       // 允许覆盖 value
					{
						items[i].value.~TV();
						new (&items[i].value) TV(std::forward<VPS>(vps)...);
						return DictAddResult{ true, i };
					}
					return DictAddResult{ false, i };
				}
			}
			// 没找到则新增
			int index;
			if (freeCount > 0)                          // 如果 自由节点链表 不空, 取一个来当容器
			{                                           // 这些节点来自 Remove 操作. next 指向下一个
				index = freeList;
				freeList = nodes[index].next;
				freeCount--;
			}
			else
			{
				if (count == bucketsLen)               // 所有空节点都用光了, Resize
				{
					Reserve();
					targetBucket = hashCode % bucketsLen;
				}
				index = count;                         // 指向 Resize 后面的空间起点
				count++;
			}

			// 执行到此处时, freeList 要么来自 自由节点链表, 要么为 Reserve 后新增的空间起点.
			nodes[index].hashCode = hashCode;
			nodes[index].next = buckets[targetBucket];

			// 如果当前 bucket 中有 index, 则令其 prev 等于即将添加的 index
			if (buckets[targetBucket] >= 0)
			{
				items[buckets[targetBucket]].prev = index;
			}
			buckets[targetBucket] = index;

			// 移动复制构造写 key, value
			new (&items[index].key) TK(std::forward<K>(key));
			new (&items[index].value) TV(std::forward<VPS>(vps)...);
			items[index].prev = -1;

			return DictAddResult{ true, index };
		}

		// 只支持没数据时扩容或空间用尽扩容( 如果不这样限制, 扩容时的 遍历损耗 略大 )
		void Reserve(int capacity = 0)
		{
			assert(buckets);
			assert(count == 0 || count == bucketsLen);          // 确保扩容函数使用情型

			// 得到空间利用率最高的扩容长度并直接修改 bucketsLen( count 为当前数据长 )
			if (capacity == 0)
			{
				capacity = count * 2;                           // 2倍扩容
			}
			if (capacity <= bucketsLen) return;
			bucketsLen = (int)GetPrime(capacity, sizeof(Data));

			// 桶扩容并全部初始化( 后面会重新映射一次 )
			mempool().Free(buckets);
			buckets = (int*)mempool().Alloc(bucketsLen * sizeof(int));
			memset(buckets, -1, bucketsLen * sizeof(int));

			// 节点数组扩容( 保留老数据 )
			nodes = (Node*)mempool().Realloc(nodes, bucketsLen * sizeof(Node));

			// item 数组扩容
			if ((std::is_trivial<TK>::value || MemmoveSupport_v<TK>)
				&& (std::is_trivial<TV>::value || MemmoveSupport_v<TV>))
			{
				items = (Data*)mempool().Realloc(items, bucketsLen * sizeof(Data));
			}
			else
			{
				auto newItems = (Data*)mempool().Alloc(bucketsLen * sizeof(Data));
				for (int i = 0; i < count; ++i)
				{
					new (&newItems[i].key) TK((TK&&)items[i].key);
					items[i].key.TK::~TK();
					new (&newItems[i].value) TV((TV&&)items[i].value);
					items[i].value.TV::~TV();
				}
				mempool().Free(items);
				items = newItems;
			}

			// 遍历所有节点, 重构桶及链表( 扩容情况下没有节点空洞 )
			for (int i = 0; i < count; i++)
			{
				auto index = nodes[i].hashCode % bucketsLen;
				if (buckets[index] >= 0)
				{
					items[buckets[index]].prev = i;
				}
				items[i].prev = -1;
				nodes[i].next = buckets[index];
				buckets[index] = i;
			}
		}

		int Find(TK const &key) const
		{
			assert(buckets);
			auto hashCode = GetHashCode(key);
			for (int i = buckets[hashCode % bucketsLen]; i >= 0; i = nodes[i].next)
			{
				if (nodes[i].hashCode == hashCode && EqualsTo(items[i].key, key))
				{
					return i;
				}
			}
			return -1;
		}

		void RemoveAt(int idx)
		{
			assert(buckets);
			assert(idx >= 0 && idx < count && items[idx].prev != -2);
			if (items[idx].prev < 0)
			{
				buckets[nodes[idx].hashCode % bucketsLen] = nodes[idx].next;
			}
			else
			{
				nodes[items[idx].prev].next = nodes[idx].next;
			}
			if (nodes[idx].next >= 0)       // 如果存在当前节点的下一个节点, 令其 prev 指向 上一个节点
			{
				items[nodes[idx].next].prev = items[idx].prev;
			}

			nodes[idx].next = freeList;     // 当前节点已被移出链表, 令其 next 指向  自由节点链表头( next 有两种功用 )
			freeList = idx;
			freeCount++;

			items[idx].key.~TK();
			items[idx].value.~TV();
			items[idx].prev = -2;           // foreach 时的无效标志
		}

		// 可传入一个资源回收函数来搞事
		void Clear(std::function<void(Data&)> killer = nullptr)
		{
			if (killer)
			{
				for (auto& data : *this) killer(data);
			}

			assert(buckets);
			if (!count) return;
			DeleteKVs();
			memset(buckets, -1, bucketsLen * sizeof(int));
			freeList = -1;
			freeCount = 0;
			count = 0;
		}

		template<typename K>
		TV& operator[](K &&key)
		{
			assert(buckets);
			int idx = Find(key);
			if (idx < 0)
			{
				idx = Emplace(true, std::forward<K>(key), TV()).index;
			}
			return items[idx].value;
		}

		void Remove(TK const &key)
		{
			assert(buckets);
			auto idx = Find(key);
			if (idx != -1)
			{
				RemoveAt(idx);
			}
		}

	protected:

		void DeleteKVs()                                    // 用于 析构, Clear
		{
			assert(buckets);
			for (int i = 0; i < count; ++i)
			{
				if (items[i].prev != -2)
				{
					items[i].key.~TK();
					items[i].value.~TV();
					items[i].prev = -2;
				}
			}
		}

	public:

		DictAddResult Add(TK const &k, TV const &v, bool override = false)
		{
			return Emplace(override, k, v);
		}
		DictAddResult Add(TK const &k, TV &&v, bool override = false)
		{
			return Emplace(override, k, (TV&&)v);
		}
		DictAddResult Add(TK &&k, TV const &v, bool override = false)
		{
			return Emplace(override, (TK&&)k, v);
		}
		DictAddResult Add(TK &&k, TV &&v, bool override = false)
		{
			return Emplace(override, (TK&&)k, (TV&&)v);
		}

		uint32_t Count() const
		{
			assert(buckets);
			return uint32_t(count - freeCount);
		}

		bool Empty()
		{
			assert(buckets);
			return count == 0;
		}

		bool TryGetValue(TK const &key, TV &outV)
		{
			int idx = Find(key);
			if (idx >= 0)
			{
				outV = items[idx].value;
				return true;
			}
			return false;
		}

		template<typename K>
		TV& At(K &&key)
		{
			return operator[](std::forward<K>(key));
		}

		//TK& KeyAt(int idx)
		//{
		//	assert(buckets);
		//	assert(idx >= 0 && idx < count && items[idx].prev != -2);
		//	return items[idx].key;
		//}
		TV& ValueAt(int idx)
		{
			assert(buckets);
			assert(idx >= 0 && idx < count && items[idx].prev != -2);
			return items[idx].value;
		}
		//Data& DataAt(int idx)
		//{
		//	assert(buckets);
		//	assert(idx >= 0 && idx < count && items[idx].prev != -2);
		//	return items[idx];
		//}

		TK const& KeyAt(int idx) const
		{
			return const_cast<Dict*>(this)->IndexAtKey(idx);
		}
		TV const& ValueAt(int idx) const
		{
			return const_cast<Dict*>(this)->IndexAtValue(idx);
		}
		Data const& DataAt(int idx) const
		{
			return const_cast<Dict*>(this)->At(idx);
		}

		bool IndexExists(int idx) const
		{
			return idx >= 0 && idx < count && items[idx].prev != -2;
		}


		/*******************************************************************************/
		// 只是为了能 for( auto &c : 
		/*******************************************************************************/

		struct Iter
		{
			Dict& hs;
			int i;
			bool operator!=(Iter const& other) { return i != other.i; }
			Iter& operator++()
			{
				while (++i < hs.count)
				{
					if (hs.items[i].prev != -2) break;
				}
				return *this;
			}
			Data& operator*() { return hs.items[i]; }
		};
		Iter begin()
		{
			if (Empty()) return end();
			for (int i = 0; i < count; ++i)
			{
				if (items[i].prev != -2) return Iter{ *this, i };
			}
			return end();
		}
		Iter end()
		{
			return Iter{ *this, count };
		}


		// todo
		///*************************************************************************/
		//// 实现 ToString 接口
		///*************************************************************************/

		//virtual void ToString(String &str) const override;

	};


	/*************************************************************************/
	// 实现值类型使用类型声明
	/*************************************************************************/

	template <typename TK, typename TV>
	using Dict_v = MemHeaderBox<Dict<TK, TV>>;

	template <typename TK, typename TV>
	using Dict_p = Ptr<Dict<TK, TV>>;


	template <typename TK, typename TV>
	struct MemmoveSupport<Dict_v<TK, TV>>
	{
		static const bool value = true;
	};
}
