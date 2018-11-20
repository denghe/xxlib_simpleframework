#pragma once
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
	class Dict : public Object
	{
	public:
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
		
	//private:
		int                 freeList;               // 自由空间链表头( next 指向下一个未使用单元 )
		int                 freeCount;              // 自由空间链长
		int                 count;                  // 已使用空间数
		int                 bucketsLen;             // 桶数组长( 质数 )
		int                *buckets;                // 桶数组
		Node               *nodes;                  // 节点数组
		Data               *items;                  // 数据数组( 与节点数组同步下标 )
	public:

		explicit Dict(MemPool* const& mempool, int const& capacity = 16);
		~Dict();
		Dict(Dict const& o) = delete;
		Dict& operator=(Dict const& o) = delete;


		// 只支持没数据时扩容或空间用尽扩容( 如果不这样限制, 扩容时的 遍历损耗 略大 )
		void Reserve(int capacity = 0) noexcept;

		// 根据 key 返回下标. -1 表示没找到.
		int Find(TK const& key) const noexcept;

		// 根据 key 移除一条数据
		void Remove(TK const& key) noexcept;

		// 根据 下标 移除一条数据( unsafe )
		void RemoveAt(int const& idx) noexcept;

		// 规则同 std. 如果 key 不存在, 将创建 key, TV默认值 的元素出来. C++ 不方便像 C# 那样直接返回 default(TV). 无法返回临时变量的引用
		template<typename K>
		TV& operator[](K&& key) noexcept;

		// 可传入一个资源回收函数来搞事
		void Clear(std::function<void(Data&)> killer = nullptr) noexcept;

		// 放入数据. 如果放入失败, 将返回 false 以及已存在的数据的下标
		template<typename K, typename V>
		DictAddResult Add(K&& k, V&& v, bool const& override = false) noexcept;

		// 取数据记录数
		uint32_t Count() const noexcept;

		// 是否没有数据
		bool Empty() noexcept;

		// 试着填充数据到 outV. 如果不存在, 就返回 false
		bool TryGetValue(TK const& key, TV& outV) noexcept;

		// 同 operator[]
		template<typename K>
		TV& At(K&& key) noexcept;


		// 下标直读系列( unsafe )

		// 读下标所在 key
		TK const& KeyAt(int const& idx) const noexcept;

		// 读下标所在 value
		TV& ValueAt(int const& idx) noexcept;
		TV const& ValueAt(int const& idx) const noexcept;

		//Data const& DataAt(int idx) const noexcept;

		// 简单 check 某下标是否有效.
		bool IndexExists(int const& idx) const noexcept;


		// 修改 key 值系列
		template<typename K>
		bool Update(TK const& oldKey, K&& newKey) noexcept;

		template<typename K>
		bool UpdateAt(int const& idx, K&& newKey) noexcept;


		// for( auto &c :  支持
		struct Iter
		{
			Dict& hs;
			int i;
			bool operator!=(Iter const& other) noexcept { return i != other.i; }
			Iter& operator++() noexcept
			{
				while (++i < hs.count)
				{
					if (hs.items[i].prev != -2) break;
				}
				return *this;
			}
			Data& operator*() { return hs.items[i]; }
		};
		Iter begin() noexcept
		{
			if (Empty()) return end();
			for (int i = 0; i < count; ++i)
			{
				if (items[i].prev != -2) return Iter{ *this, i };
			}
			return end();
		}
		Iter end() noexcept { return Iter{ *this, count }; }

	protected:
		// 用于 析构, Clear
		void DeleteKVs() noexcept;
	};


	template <typename TK, typename TV>
	using Dict_p = Ptr<Dict<TK, TV>>;

	template <typename TK, typename TV>
	using Dict_r = Ref<Dict<TK, TV>>;

	template <typename TK, typename TV>
	using Dict_u = Unique<Dict<TK, TV>>;

	template <typename TK, typename TV>
	using Dict_w = Weak<Dict<TK, TV>>;

}
