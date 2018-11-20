#pragma once
namespace xx
{
	// ring buffer FIFO 队列
	// 能随机检索...能批量 pop
	//...............FR...............					// Head == Tail
	//......Head+++++++++++Tail.......					// DataLen = Tail - Head
	//++++++Tail...........Head+++++++					// DataLen = BufLen - Head + Tail
	template <class T>
	class Queue : public Object
	{
	public:
		typedef T ChildType;

		T*			buf;
		size_t		bufLen;
		size_t		head = 0, tail = 0;					// FR..............................

		explicit Queue(MemPool* const& mempool, size_t capacity = 8) noexcept;
		Queue(Queue && o) noexcept;
		~Queue() noexcept;

		Queue(Queue const& o) = delete;
		Queue& operator=(Queue const& o) = delete;

		T const& operator[](size_t const& idx) const noexcept;	// [0] = [ head ]
		T& operator[](size_t const& idx) noexcept;
		T const& At(size_t const& idx) const noexcept;			// []
		T& At(size_t const& idx) noexcept;

		size_t Count() const noexcept;
		bool Empty() const noexcept;
		void Clear() noexcept;
		void Reserve(size_t const& capacity, bool const& afterPush = false) noexcept;

		template<typename...Args>
		T& Emplace(Args&&...ps) noexcept;						// [ tail++ ] = T( ps )

		template<typename ...TS>
		void Push(TS&& ...vs) noexcept;

		bool TryPop(T& outVal) noexcept;

		T const& Top() const noexcept;							// [ head ]
		T& Top() noexcept;
		void Pop() noexcept;									// ++head
		size_t PopMulti(size_t const& count) noexcept;			// head += count

		T const& Last() const noexcept;							// [ tail-1 ]
		T& Last() noexcept;
		void PopLast() noexcept;								// --tail
	};


	template<typename T>
	using Queue_p = Ptr<Queue<T>>;

	template<typename T>
	using Queue_r = Ref<Queue<T>>;

	template<typename T>
	using Queue_u = Unique<Queue<T>>;

	template<typename T>
	using Queue_w = Weak<Queue<T>>;
}
