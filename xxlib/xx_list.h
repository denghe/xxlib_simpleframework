#pragma once
namespace xx
{
	// std::vector / .net List like
	template<typename T>
	class List : public Object
	{
	public:
		typedef T ChildType;
		T*			buf;
		size_t		bufLen;
		size_t		dataLen;

		explicit List(MemPool* const& mempool, size_t const& capacity = 0);
		List(List &&o);
		~List();
		List(List const&o) = delete;
		List& operator=(List const&o) = delete;

		void Reserve(size_t const& capacity);
		size_t Resize(size_t const& len);			// 如果不想初始化扩容的单元, 直接 Reserve + 改 dataLen

		T const& operator[](size_t const& idx) const noexcept;
		T& operator[](size_t const& idx) noexcept;
		T const& At(size_t const& idx) const noexcept;
		T& At(size_t const& idx) noexcept;

		T& Top() noexcept;
		void Pop() noexcept;
		T const& Top() const noexcept;
		bool TryPop(T& output) noexcept;

		void Clear(bool const& freeBuf = false) noexcept;
		void RemoveAt(size_t const& idx) noexcept;
		void SwapRemoveAt(size_t const& idx);		// 和最后一个元素做交换删除
		void Remove(T const& v) noexcept;

		template<typename...Args>
		T& Emplace(Args &&... args);

		template<typename...Args>
		T& EmplaceAt(size_t idx, Args&&...args);	// 用参数直接构造一个到指定位置

		void Add(T&& v);
		void Add(T const& v);

		void InsertAt(size_t const& idx, T&& v);
		void InsertAt(size_t const& idx, T const& v);

		void AddRange(T const* items, size_t count);

		size_t Find(T const& v) noexcept;			// 如果找到就返回索引. 找不到将返回 size_t(-1)
		size_t Find(std::function<bool(T&)> cond);
		bool Exists(std::function<bool(T&)> cond);
		bool TryFill(T& out, std::function<bool(T&)> cond);

		void ForEachRevert(std::function<void(T&)> handler);	// 倒着扫. 便于 SwapRemoveAt.

		// 支持 for( auto c : list ) 语法
		struct Iter
		{
			T *ptr;
			bool operator!=(Iter const& other) noexcept { return ptr != other.ptr; }
			Iter& operator++() noexcept { ++ptr; return *this; }
			T& operator*() noexcept { return *ptr; }
		};
		Iter begin() { return Iter{ buf }; }
		Iter end() { return Iter{ buf + dataLen }; }
		Iter begin() const { return Iter{ buf }; }
		Iter end() const { return Iter{ buf + dataLen }; }


		// Object 接口支持
		List(BBuffer* bb);
		void ToBBuffer(BBuffer &bb) const override;
		int FromBBuffer(BBuffer &bb) override;

		void ToString(String &s) const override;
	};


	template<typename T>
	using List_p = Ptr<List<T>>;

	template<typename T>
	using List_r = Ref<List<T>>;
}
