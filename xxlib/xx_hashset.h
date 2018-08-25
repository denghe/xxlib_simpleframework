#pragma once
namespace xx
{
	// 从 Dict 精简而来

	template <typename TK>
	class HashSet : public Object
	{
	public:
		typedef TK KeyType;

		struct Node
		{
			unsigned int    hashCode;
			int             next;
			TK              key;
			int             prev;
		};

	private:
		int                 freeList;               // 自由空间链表头( next 指向下一个未使用单元 )
		int                 freeCount;              // 自由空间链长
		int                 count;                  // 已使用空间数
		int                 bucketsLen;             // 桶数组长( 质数 )
		int                *buckets;                // 桶数组
		Node               *nodes;                  // 节点数组
	public:

		explicit HashSet(MemPool* const& mempool, int const& capacity = 16) noexcept;
		~HashSet() noexcept;
		HashSet(HashSet const &o) = delete;
		HashSet& operator=(HashSet const &o) = delete;

		// 只支持没数据时扩容或空间用尽扩容( 如果不这样限制, 扩容时的 遍历损耗 略大 )
		void Reserve(int capacity = 0) noexcept;

		// 返回 true: 添加成功. false: 已存在相同的值
		template<typename K>
		bool Add(K&& k) noexcept;

		// 如果存在就返回 true
		bool Exists(TK const& k) const noexcept;

		// 如果找到就移除并返回 true. 未找到返回 false
		bool Remove(TK const& k) noexcept;

		// 移除所有数据
		void Clear() noexcept;

		// 返回成员个数
		int Count() const noexcept;

		// return Count() == 0
		bool Empty() const noexcept;

		// for( auto &c :  支持
		struct Iter
		{
			HashSet* self;
			int i;
			bool operator!=(Iter const& other) noexcept { return i != other.i; }
			Iter& operator++() noexcept
			{
				while (++i < self->count)
				{
					if (self->nodes[i].prev != -2) break;
				}
				return *this;
			}
			TK& operator*() noexcept { return self->nodes[i].key; }
		};
		Iter begin() noexcept
		{
			if (count != freeCount)
			{
				for (int i = 0; i < count; ++i)
				{
					if (nodes[i].prev != -2) return Iter{ this, i };
				}
			}
			return end();
		}
		Iter end() noexcept { return Iter{ this, count }; }

	protected:
		// 用于 析构, Clear
		void DeleteKs() noexcept;
	};


	template <typename TK>
	using HashSet_p = Ptr<HashSet<TK>>;

	template <typename TK>
	using HashSet_r = Ref<HashSet<TK>>;

}
