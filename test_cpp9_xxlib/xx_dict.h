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

	private:
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
		Dict(Dict const &o) = delete;
		Dict& operator=(Dict const &o) = delete;


		// 只支持没数据时扩容或空间用尽扩容( 如果不这样限制, 扩容时的 遍历损耗 略大 )
		void Reserve(int capacity = 0);

		int Find(TK const &key) const noexcept;
		void Remove(TK const &key) noexcept;
		void RemoveAt(int idx) noexcept;

		template<typename K>
		TV& operator[](K &&key);

		// 可传入一个资源回收函数来搞事
		void Clear(std::function<void(Data&)> killer = nullptr) noexcept;

	private:
		template<typename K, typename V>
		DictAddResult AddCore(bool override, K &&key, V &&v);
	public:

		DictAddResult Add(TK const &k, TV const &v, bool override = false);
		DictAddResult Add(TK const &k, TV &&v, bool override = false);
		DictAddResult Add(TK &&k, TV const &v, bool override = false);
		DictAddResult Add(TK &&k, TV &&v, bool override = false);

		uint32_t Count() const noexcept;
		bool Empty() noexcept;
		bool TryGetValue(TK const &key, TV &outV) noexcept;

		template<typename K>
		TV& At(K &&key);

		TK const& KeyAt(int idx) const noexcept;
		TV& ValueAt(int idx) noexcept;
		TV const& ValueAt(int idx) const noexcept;
		Data const& DataAt(int idx) const noexcept;

		bool IndexExists(int idx) const noexcept;


		// for( auto &c :  支持
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
		Iter end() { return Iter{ *this, count }; }

	protected:
		// 用于 析构, Clear
		void DeleteKVs() noexcept;
	};


	template <typename TK, typename TV>
	using Dict_p = Ptr<Dict<TK, TV>>;

	template <typename TK, typename TV>
	using Dict_r = Ref<Dict<TK, TV>>;

}
