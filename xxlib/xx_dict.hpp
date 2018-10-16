#pragma once
namespace xx
{
	template <typename TK, typename TV>
	Dict<TK, TV>::Dict(MemPool* const& mempool, int const& capacity)
		: Object(mempool)
	{
		freeList = -1;
		freeCount = 0;
		count = 0;
		bucketsLen = (int)GetPrime(capacity, sizeof(Data));
		buckets = (int*)mempool->Alloc(bucketsLen * sizeof(int));
		memset(buckets, -1, bucketsLen * sizeof(int));  // -1 代表 "空"
		nodes = (Node*)mempool->Alloc(bucketsLen * sizeof(Node));
		items = (Data*)mempool->Alloc(bucketsLen * sizeof(Data));
	}

	template <typename TK, typename TV>
	Dict<TK, TV>::~Dict()
	{
		DeleteKVs();
		mempool->Free(buckets);
		mempool->Free(nodes);
		mempool->Free(items);
	}

	template <typename TK, typename TV>
	template<typename K, typename V>
	DictAddResult Dict<TK, TV>::Add(K&& k, V&& v, bool const& override) noexcept
	{
		assert(bucketsLen);

		// hash 按桶数取模 定位到具体 链表, 扫找
		auto hashCode = HashFunc<TK>::GetHashCode(k);
		auto targetBucket = hashCode % bucketsLen;
		for (int i = buckets[targetBucket]; i >= 0; i = nodes[i].next)
		{
			if (nodes[i].hashCode == hashCode && EqualsFunc<TK>::EqualsTo(items[i].key, k))
			{
				if (override)                       // 允许覆盖 value
				{
					items[i].value.~TV();
					new (&items[i].value) TV(std::forward<V>(v));
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
		new (&items[index].key) TK(std::forward<K>(k));
		new (&items[index].value) TV(std::forward<V>(v));
		items[index].prev = -1;

		return DictAddResult{ true, index };
	}

	// 只支持没数据时扩容或空间用尽扩容( 如果不这样限制, 扩容时的 遍历损耗 略大 )
	template <typename TK, typename TV>
	void Dict<TK, TV>::Reserve(int capacity) noexcept
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
		mempool->Free(buckets);
		buckets = (int*)mempool->Alloc(bucketsLen * sizeof(int));
		memset(buckets, -1, bucketsLen * sizeof(int));

		// 节点数组扩容( 保留老数据 )
		nodes = (Node*)mempool->Realloc(nodes, bucketsLen * sizeof(Node));

		// item 数组扩容
		if constexpr(IsTrivial_v<TK> && IsTrivial_v<TV>)
		{
			items = (Data*)mempool->Realloc(items, bucketsLen * sizeof(Data));
		}
		else
		{
			auto newItems = (Data*)mempool->Alloc(bucketsLen * sizeof(Data));
			for (int i = 0; i < count; ++i)
			{
				new (&newItems[i].key) TK((TK&&)items[i].key);
				items[i].key.TK::~TK();
				new (&newItems[i].value) TV((TV&&)items[i].value);
				items[i].value.TV::~TV();
			}
			mempool->Free(items);
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

	template <typename TK, typename TV>
	int Dict<TK, TV>::Find(TK const& k) const noexcept
	{
		assert(buckets);
		auto hashCode = HashFunc<TK>::GetHashCode(k);
		for (int i = buckets[hashCode % bucketsLen]; i >= 0; i = nodes[i].next)
		{
			if (nodes[i].hashCode == hashCode && EqualsFunc<TK>::EqualsTo(items[i].key, k))
			{
				return i;
			}
		}
		return -1;
	}

	template <typename TK, typename TV>
	void Dict<TK, TV>::RemoveAt(int const& idx) noexcept
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
	template <typename TK, typename TV>
	void Dict<TK, TV>::Clear(std::function<void(Data&)> killer) noexcept
	{
		if (killer)
		{
			for (decltype(auto) data : *this) killer(data);
		}

		assert(buckets);
		if (!count) return;
		DeleteKVs();
		memset(buckets, -1, bucketsLen * sizeof(int));
		freeList = -1;
		freeCount = 0;
		count = 0;
	}

	template <typename TK, typename TV>
	template<typename K>
	TV& Dict<TK, TV>::operator[](K &&k) noexcept
	{
		assert(buckets);
		int idx = Find(k);
		if (idx < 0)
		{
			idx = Add(std::forward<K>(k), TV(), true).index;
		}
		return items[idx].value;
	}

	template <typename TK, typename TV>
	void Dict<TK, TV>::Remove(TK const &k) noexcept
	{
		assert(buckets);
		auto idx = Find(k);
		if (idx != -1)
		{
			RemoveAt(idx);
		}
	}

	template <typename TK, typename TV>
	void Dict<TK, TV>::DeleteKVs() noexcept
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

	template <typename TK, typename TV>
	uint32_t Dict<TK, TV>::Count() const noexcept
	{
		assert(buckets);
		return uint32_t(count - freeCount);
	}

	template <typename TK, typename TV>
	bool Dict<TK, TV>::Empty() noexcept
	{
		assert(buckets);
		return Count() == 0;
	}

	template <typename TK, typename TV>
	bool Dict<TK, TV>::TryGetValue(TK const& key, TV& outV) noexcept
	{
		int idx = Find(key);
		if (idx >= 0)
		{
			outV = items[idx].value;
			return true;
		}
		return false;
	}

	template <typename TK, typename TV>
	template<typename K>
	TV& Dict<TK, TV>::At(K&& key) noexcept
	{
		return operator[](std::forward<K>(key));
	}

	template <typename TK, typename TV>
	TV& Dict<TK, TV>::ValueAt(int const& idx) noexcept
	{
		assert(buckets);
		assert(idx >= 0 && idx < count && items[idx].prev != -2);
		return items[idx].value;
	}

	template <typename TK, typename TV>
	TK const& Dict<TK, TV>::KeyAt(int const& idx) const noexcept
	{
		assert(idx >= 0 && idx < count && items[idx].prev != -2);
		return items[idx].key;
	}

	template <typename TK, typename TV>
	TV const& Dict<TK, TV>::ValueAt(int const& idx) const noexcept
	{
		return const_cast<Dict*>(this)->ValueAt(idx);
	}

//	template <typename TK, typename TV>
//	Dict<TK, TV>::Data const& Dict<TK, TV>::DataAt(int idx) const noexcept
//	{
//		return const_cast<Dict*>(this)->At(idx);
//	}

	template <typename TK, typename TV>
	bool Dict<TK, TV>::IndexExists(int const& idx) const noexcept
	{
		return idx >= 0 && idx < count && items[idx].prev != -2;
	}


	template <typename TK, typename TV>
	template<typename K>
	bool Dict<TK, TV>::Update(TK const& oldKey, K&& newKey) noexcept
	{
		int idx = Find(oldKey);
		if (idx == -1) return false;
		return UpdateAt(idx, std::forward<K>(newKey));
	}

	template <typename TK, typename TV>
	template<typename K>
	bool Dict<TK, TV>::UpdateAt(int const& idx, K&& newKey) noexcept
	{
		assert(idx >= 0 && idx < count && items[idx].prev != -2);
		auto& node = nodes[idx];
		auto& item = items[idx];

		// 如果 hash 相等可以直接改 key 并退出函数
		auto newHashCode = HashFunc<TK>::GetHashCode(newKey);
		if (node.hashCode == newHashCode)
		{
			item.key = std::forward<K>(newKey);
			return true;
		}

		// 位于相同 bucket, 直接改 has & key 并退出函数
		auto targetBucket = node.hashCode % (uint32_t)bucketsLen;
		auto newTargetBucket = newHashCode % (uint32_t)bucketsLen;
		if (targetBucket == newTargetBucket)
		{
			node.hashCode = newHashCode;
			item.key = std::forward<K>(newKey);
			return true;
		}

		// 检查 newKey 是否已存在
		for (int i = buckets[newTargetBucket]; i >= 0; i = nodes[i].next)
		{
			if (nodes[i].hashCode == newHashCode && EqualsFunc<TK>::EqualsTo(items[i].key, newKey))
			{
				return false;
			}
		}

		// 简化的 RemoveAt
		if (item.prev < 0)
		{
			buckets[targetBucket] = node.next;
		}
		else
		{
			nodes[item.prev].next = node.next;
		}
		if (node.next >= 0)
		{
			items[node.next].prev = item.prev;
		}

		// 简化的 Add 后半段
		node.hashCode = newHashCode;
		node.next = buckets[newTargetBucket];

		if (buckets[newTargetBucket] >= 0)
		{
			items[buckets[newTargetBucket]].prev = idx;
		}
		buckets[newTargetBucket] = idx;

		item.key = std::forward<K>(newKey);
		item.prev = -1;

		return true;
	}

}
