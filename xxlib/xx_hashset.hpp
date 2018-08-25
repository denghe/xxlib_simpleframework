#pragma once
namespace xx
{
	template <typename TK>
	HashSet<TK>::HashSet(MemPool* const& mempool, int const& capacity) noexcept
		: Object(mempool)
	{
		freeList = -1;
		freeCount = 0;
		count = 0;
		bucketsLen = (int)GetPrime(capacity, sizeof(Node));
		buckets = (int*)mempool->Alloc(bucketsLen * sizeof(int));
		memset(buckets, -1, bucketsLen * sizeof(int));  // -1 代表 "空"
		nodes = (Node*)mempool->Alloc(bucketsLen * sizeof(Node));
	}

	template <typename TK>
	HashSet<TK>::~HashSet() noexcept
	{
		DeleteKs();
		mempool->Free(buckets);
		mempool->Free(nodes);
	}

	template <typename TK>
	template<typename K>
	bool HashSet<TK>::Add(K&& k) noexcept
	{
		assert(bucketsLen);

		// hash 按桶数取模 定位到具体 链表, 扫找
		auto hashCode = HashFunc<TK>::GetHashCode(k);
		auto targetBucket = hashCode % bucketsLen;
		for (int i = buckets[targetBucket]; i >= 0; i = nodes[i].next)
		{
			if (nodes[i].hashCode == hashCode && EqualsFunc<TK>::EqualsTo(nodes[i].key, k))
			{
				return false;
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
			nodes[buckets[targetBucket]].prev = index;
		}
		buckets[targetBucket] = index;

		// 写 key
		new (&nodes[index].key) TK(std::forward<K>(k));
		nodes[index].prev = -1;

		return true;
	}

	// 只支持没数据时扩容或空间用尽扩容( 如果不这样限制, 扩容时的 遍历损耗 略大 )
	template <typename TK>
	void HashSet<TK>::Reserve(int capacity) noexcept
	{
		assert(buckets);
		assert(count == 0 || count == bucketsLen);          // 确保扩容函数使用情型

		// 得到空间利用率最高的扩容长度并直接修改 bucketsLen( count 为当前数据长 )
		if (capacity == 0)
		{
			capacity = count * 2;                           // 2倍扩容
		}
		if (capacity <= bucketsLen) return;
		bucketsLen = (int)GetPrime(capacity, sizeof(Node));

		// 桶扩容并全部初始化( 后面会重新映射一次 )
		mempool->Free(buckets);
		buckets = (int*)mempool->Alloc(bucketsLen * sizeof(int));
		memset(buckets, -1, bucketsLen * sizeof(int));

		// 节点扩容( 保留老数据 )
		if constexpr(IsTrivial_v<TK>)
		{
			nodes = (Node*)mempool->Realloc(nodes, bucketsLen * sizeof(Node));
		}
		else
		{
			auto newItems = (Node*)mempool->Alloc(bucketsLen * sizeof(Node));
			for (int i = 0; i < count; ++i)
			{
				new (&newItems[i].key) TK((TK&&)nodes[i].key);
				nodes[i].key.TK::~TK();
			}
			mempool->Free(nodes);
			nodes = newItems;
		}

		// 遍历所有节点, 重构桶及链表( 扩容情况下没有节点空洞 )
		for (int i = 0; i < count; i++)
		{
			auto index = nodes[i].hashCode % bucketsLen;
			if (buckets[index] >= 0)
			{
				nodes[buckets[index]].prev = i;
			}
			nodes[i].prev = -1;
			nodes[i].next = buckets[index];
			buckets[index] = i;
		}
	}

	template <typename TK>
	bool HashSet<TK>::Exists(TK const& k) const noexcept
	{
		assert(buckets);
		auto hashCode = HashFunc<TK>::GetHashCode(k);
		for (int i = buckets[hashCode % bucketsLen]; i >= 0; i = nodes[i].next)
		{
			if (nodes[i].hashCode == hashCode && EqualsFunc<TK>::EqualsTo(nodes[i].key, k))
			{
				return true;
			}
		}
		return false;
	}

	template <typename TK>
	bool HashSet<TK>::Remove(TK const& k) noexcept
	{
		assert(buckets);

		auto hashCode = HashFunc<TK>::GetHashCode(k);
		for (int i = buckets[hashCode % bucketsLen]; i >= 0; i = nodes[i].next)
		{
			if (nodes[i].hashCode == hashCode && EqualsFunc<TK>::EqualsTo(nodes[i].key, k))
			{
				if (nodes[i].prev < 0)
				{
					buckets[nodes[i].hashCode % bucketsLen] = nodes[i].next;
				}
				else
				{
					nodes[nodes[i].prev].next = nodes[i].next;
				}
				if (nodes[i].next >= 0)       // 如果存在当前节点的下一个节点, 令其 prev 指向 上一个节点
				{
					nodes[nodes[i].next].prev = nodes[i].prev;
				}

				nodes[i].next = freeList;     // 当前节点已被移出链表, 令其 next 指向  自由节点链表头( next 有两种功用 )
				freeList = i;
				freeCount++;

				nodes[i].key.~TK();
				nodes[i].prev = -2;           // foreach 时的无效标志

				return true;
			}
		}
		return false;
	}

	template <typename TK>
	void HashSet<TK>::Clear() noexcept
	{
		assert(buckets);
		if (!count) return;
		DeleteKs();
		memset(buckets, -1, bucketsLen * sizeof(int));
		freeList = -1;
		freeCount = 0;
		count = 0;
	}

	template <typename TK>
	void HashSet<TK>::DeleteKs() noexcept
	{
		assert(buckets);
		for (int i = 0; i < count; ++i)
		{
			if (nodes[i].prev != -2)
			{
				nodes[i].key.~TK();
				nodes[i].prev = -2;
			}
		}
	}

	template <typename TK>
	int HashSet<TK>::Count() const noexcept
	{
		assert(buckets && count >= freeCount);
		return count - freeCount;
	}

	template <typename TK>
	bool HashSet<TK>::Empty() const noexcept
	{
		return Count() == 0;
	}

}

