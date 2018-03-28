#pragma once
namespace xx
{

	template <class T>
	Queue<T>::Queue(MemPool* const& mempool, size_t capacity)
		: Object(mempool)
	{
		if (capacity < 8)
		{
			capacity = 8;
		}
		auto bufByteLen = MemPool::Round2n(capacity * sizeof(T) + sizeof(MemHeader)) - sizeof(MemHeader);
		buf = (T*)mempool->Alloc((size_t)bufByteLen);
		bufLen = size_t(bufByteLen / sizeof(T));
	}

	template <class T>
	Queue<T>::~Queue()
	{
		if (buf)
		{
			Clear();
			mempool->Free(buf);
			buf = nullptr;
		}
	}

	template <class T>
	size_t Queue<T>::Count() const
	{
		//......Head+++++++++++Tail.......
		//...............FR...............
		if (head <= tail) return tail - head;
		// ++++++Tail...........Head++++++
		else return tail + (bufLen - head);
	}

	template <class T>
	bool Queue<T>::Empty() const
	{
		return head == tail;
	}

	template <class T>
	void Queue<T>::Pop()
	{
		assert(head != tail);
		buf[head++].~T();
		if (head == bufLen)
		{
			head = 0;
		}
	}

	template <class T>
	void Queue<T>::PopLast()
	{
		assert(head != tail);
		buf[tail--].~T();
		if (tail == (size_t)-1)
		{
			tail = bufLen - 1;
		}
	}


	template <class T>
	template<typename...Args>
	T& Queue<T>::Emplace(Args&&...args)
	{
		auto idx = tail;
		new (buf + tail++) T(std::forward<Args>(args)...);
		if (tail == bufLen)			// cycle
		{
			tail = 0;
		}
		if (tail == head)			// no more space
		{
			idx = bufLen - 1;
			Reserve(bufLen * 2, true);
		}
		return buf[idx];
	}

	template <class T>
	void Queue<T>::Push(T const& v)
	{
		Emplace(v);
	}

	template <class T>
	void Queue<T>::Push(T&& v)
	{
		Emplace((T&&)v);
	}




	template <class T>
	void Queue<T>::Clear()
	{
		//........HT......................
		if (head == tail) return;

		//......Head+++++++++++Tail......
		if (head < tail)
		{
			for (auto i = head; i < tail; ++i) buf[i].~T();
		}
		// ++++++Tail...........Head++++++
		else
		{
			for (size_t i = 0; i < tail; ++i) buf[i].~T();
			for (auto i = head; i < bufLen; ++i) buf[i].~T();
		}
		//........HT......................
		head = tail = 0;
	}

	template <typename T>
	size_t Queue<T>::PopMulti(size_t const& count)
	{
		if (count <= 0) return 0;

		auto dataLen = Count();
		if (count >= dataLen)
		{
			Clear();
			return dataLen;
		}
		// count < dataLen

		//......Head+++++++++++Tail......
		if (head < tail)
		{
			//......Head+++++++++++count......
			for (auto i = head; i < head + count; ++i) buf[i].~T();
			head += count;
		}
		// ++++++Tail...........Head++++++
		else
		{
			auto frontDataLen = bufLen - head;
			//...Head+++
			if (count < frontDataLen)
			{
				for (auto i = head; i < head + count; ++i) buf[i].~T();
				head += count;
			}
			else
			{
				//...Head++++++
				for (auto i = head; i < bufLen; ++i) buf[i].~T();

				// <-Head
				head = count - frontDataLen;

				// ++++++Head...
				for (size_t i = 0; i < head; ++i) buf[i].~T();
			}
		}
		return count;
	}

	template <class T>
	void Queue<T>::Reserve(size_t const& capacity, bool afterPush)
	{
		assert(capacity > 0);
		if (capacity <= bufLen) return;

		auto newBufByteLen = MemPool::Round2n(capacity * sizeof(T) + sizeof(MemHeader)) - sizeof(MemHeader);
		auto newBuf = (T*)mempool->Alloc((size_t)newBufByteLen);
		auto newBufLen = size_t(newBufByteLen / sizeof(T));

		// afterPush: ++++++++++++++TH++++++++++++++++
		auto dataLen = afterPush ? bufLen : Count();

		//......Head+++++++++++Tail.......
		if (head < tail)
		{
			if constexpr(IsTrivial_v<T>)
			{
				memcpy(newBuf, buf + head, dataLen * sizeof(T));
			}
			else
			{
				for (size_t i = 0; i < dataLen; ++i)
				{
					new (newBuf + i) T((T&&)buf[head + i]);
					buf[head + i].~T();
				}
			}
		}
		// ++++++Tail...........Head+++++++
		// ++++++++++++++TH++++++++++++++++
		else
		{
			//...Head++++++
			auto frontDataLen = bufLen - head;
			if constexpr(IsTrivial_v<T>)
			{
				memcpy(newBuf, buf + head, frontDataLen * sizeof(T));
			}
			else
			{
				for (size_t i = 0; i < frontDataLen; ++i)
				{
					new (newBuf + i) T((T&&)buf[head + i]);
					buf[head + i].~T();
				}
			}

			// ++++++Tail...
			if constexpr(IsTrivial_v<T>)
			{
				memcpy(newBuf + frontDataLen, buf, tail * sizeof(T));
			}
			else
			{
				for (size_t i = 0; i < tail; ++i)
				{
					new (newBuf + frontDataLen + i) T((T&&)buf[i]);
					buf[i].~T();
				}
			}
		}

		// Head+++++++++++Tail.............
		head = 0;
		tail = dataLen;

		mempool->Free(buf);
		buf = newBuf;
		bufLen = newBufLen;
	}


	template<typename T>
	T const& Queue<T>::operator[](size_t const& idx) const
	{
		return At(idx);
	}

	template<typename T>
	T& Queue<T>::operator[](size_t const& idx)
	{
		return At(idx);
	}

	template<typename T>
	T const& Queue<T>::At(size_t const& idx) const
	{
		return const_cast<Queue<T>*>(this)->At(idx);
	}

	template<typename T>
	T const& Queue<T>::Top() const
	{
		assert(head != tail);
		return buf[head];
	}
	template<typename T>
	T& Queue<T>::Top()
	{
		assert(head != tail);
		return buf[head];
	}


	template<typename T>
	T& Queue<T>::At(size_t const& idx)
	{
		assert(idx < Count());
		if (head + idx < bufLen)
		{
			return buf[head + idx];
		}
		else
		{
			return buf[head + idx - bufLen];
		}
	}


	template<typename T>
	T const& Queue<T>::Last() const
	{
		assert(head != tail);
		return buf[tail - 1 == (size_t)-1 ? bufLen - 1 : tail - 1];
	}
	template<typename T>
	T& Queue<T>::Last()
	{
		assert(head != tail);
		return buf[tail - 1 == (size_t)-1 ? bufLen - 1 : tail - 1];
	}


	template <typename T>
	bool Queue<T>::TryPop(T& outVal)
	{
		if (head == tail)
		{
			return false;
		}
		outVal = std::move(buf[head]);
		Pop();
		return true;
	}

}
