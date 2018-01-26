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

		explicit Queue(MemPool* const& mempool, size_t capacity = 8);
		~Queue();

		Queue(Queue const& o) = delete;
		Queue(Queue && o)
			: buf(o.buf)
			, bufLen(o.bufLen)
			, head(o.head)
			, tail(o.tail)
		{
			o.buf = nullptr;
			o.bufLen = o.head = o.tail = 0;
		}
		Queue& operator=(Queue const& o) = delete;

		T const& operator[](size_t const& idx) const;	// [0] = [ head ]
		T& operator[](size_t const& idx);
		T const& At(size_t const& idx) const;			// []
		T& At(size_t const& idx);

		size_t Count() const;
		bool Empty() const;
		void Clear();
		void Reserve(size_t const& capacity, bool afterPush = false);

		template<typename...PTS>
		T& Emplace(PTS&&...ps);							// [ tail++ ] = T( ps )
		void Push(T const& v);
		void Push(T&& v);

		bool TryPop(T& outVal);

		T const& Top() const;							// [ head ]
		T& Top();
		void Pop();										// ++head
		size_t PopMulti(size_t const& count);			// head += count

		T const& Last() const;							// [ tail-1 ]
		T& Last();
		void PopLast();									// --tail
	};


	template<typename T>
	using Queue_p = Ptr<Queue<T>>;

	template<typename T>
	using Queue_r = Ref<Queue<T>>;
}
