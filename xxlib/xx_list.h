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

		explicit List(MemPool* const& mempool, size_t const& capacity = 0) noexcept;
		List(List &&o) noexcept;
		~List() noexcept;
		List(List const&o) = delete;
		List& operator=(List const&o) = delete;

		void Reserve(size_t const& capacity) noexcept;
		size_t Resize(size_t const& len) noexcept;			// 如果不想初始化扩容的单元, 直接 Reserve + 改 dataLen

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
		void SwapRemoveAt(size_t const& idx) noexcept;		// 和最后一个元素做交换删除. 通常上一句为 list[list.dataLen - 1]->idx = o->idx;
		void Remove(T const& v) noexcept;

		template<typename...Args>
		T& Emplace(Args&&...args) noexcept;	// 用 T 的一到多个构造函数的参数来追加构造一个 v

		template<typename...Args>
		T& EmplaceAt(size_t const& idx, Args&&...args) noexcept;	// 用参数直接构造一个到指定位置

		template<typename ...TS>
		void Add(TS&&...vs) noexcept;		// 与 Emplace 不同的是, 这个仅支持1个参数的构造函数, 可同时追加多个

		void AddRange(T const* const& items, size_t const& count) noexcept;

		size_t Find(T const& v) noexcept;			// 如果找到就返回索引. 找不到将返回 size_t(-1)
		size_t Find(std::function<bool(T const&)> cond) noexcept;
		bool Exists(std::function<bool(T const&)> cond) noexcept;
		bool TryFill(T& out, std::function<bool(T const&)> cond) noexcept;

		void ForEachRevert(std::function<void(T&)> handler) noexcept;	// 倒着扫. 便于 SwapRemoveAt.

		// 支持 for( auto c : list ) 语法
		struct Iter
		{
			T *ptr;
			bool operator!=(Iter const& other) noexcept { return ptr != other.ptr; }
			Iter& operator++() noexcept { ++ptr; return *this; }
			T& operator*() noexcept { return *ptr; }
		};
		Iter begin() noexcept { return Iter{ buf }; }
		Iter end() noexcept { return Iter{ buf + dataLen }; }
		Iter begin() const noexcept { return Iter{ buf }; }
		Iter end() const noexcept { return Iter{ buf + dataLen }; }


		// Object 接口支持
		List(BBuffer* const& bb);
		void ToBBuffer(BBuffer& bb) const noexcept override;
		int FromBBuffer(BBuffer& bb) noexcept override;

		void ToString(String& s) const noexcept override;
	};


	template<typename T>
	using List_p = Ptr<List<T>>;

	template<typename T>
	using List_r = Ref<List<T>>;

	template<typename T>
	using List_u = Unique<List<T>>;

	template<typename T>
	using List_w = Weak<List<T>>;
}
