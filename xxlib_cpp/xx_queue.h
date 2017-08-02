#pragma once
#include "xx_defines.h"        // Round2n
#include "xx_mempool.h"
#include <assert.h>

namespace xx
{
	// todo: 从 C# 版同步新增函数
	// todo: 移动后空指针状如果继续用的话 并未经过测试

	// ring buffer FIFO 队列
	// 能随机检索...能批量 pop
	//...............FR...............					// Head == Tail
	//......Head+++++++++++Tail.......					// DataLen = Tail - Head
	//++++++Tail...........Head+++++++					// DataLen = BufLen - Head + Tail
	template <class T>
	struct Queue : public MPObject
	{
		typedef T ChildType;
		static const bool memmoveAble = std::is_pod<T>::value || IsMemHeaderBox_v<T>;

		T*			buf;
		uint32_t    bufLen;
		uint32_t	head = 0, tail = 0;					// FR..............................

		explicit Queue(uint32_t capacity = 8);
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

		T const& operator[](uint32_t const& idx) const;	// [0] = [ head ]
		T& operator[](uint32_t const& idx);
		T const& At(uint32_t const& idx) const;			// []
		T& At(uint32_t const& idx);

		uint32_t Count() const;
		bool Empty() const;
		void Clear();
		void Reserve(uint32_t const& capacity, bool afterPush = false);

		template<typename...PTS>
		T& Emplace(PTS&&...ps);							// [ tail++ ] = T( ps )
		void Push(T const& v);
		void Push(T&& v);

		bool TryPop(T& outVal);

		T const& Top() const;							// [ head ]
		T& Top();
		void Pop();										// ++head
		uint32_t PopMulti(uint32_t const& count);		// head += count

		T const& Last() const;							// [ tail-1 ]
		T& Last();
		void PopLast();									// --tail
	};


	/*************************************************************************/
	// 值类型使用形态包装
	/*************************************************************************/

	template<typename T>
	using Queue_v = MemHeaderBox<Queue<T>>;

	template<typename T>
	using Queue_p = Ptr<Queue<T>>;

	template<typename T>
	struct MemmoveSupport<Queue_v<T>>
	{
		static const bool value = true;
	};




	template <class T>
	Queue<T>::Queue(uint32_t capacity)
	{
		if (capacity < 8) capacity = 8;
		auto bufByteLen = Round2n(capacity * sizeof(T) + sizeof(MemHeader_VersionNumber)) - sizeof(MemHeader_VersionNumber);
		buf = (T*)mempool().Alloc((uint32_t)bufByteLen);
		bufLen = uint32_t(bufByteLen / sizeof(T));
	}

	template <class T>
	Queue<T>::~Queue()
	{
		if (buf)
		{
			Clear();
			mempool().Free(buf);
			buf = nullptr;
		}
	}

	template <class T>
	uint32_t Queue<T>::Count() const
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
		if (tail == (uint32_t)-1)
		{
			tail = bufLen - 1;
		}
	}


	template <class T>
	template<typename...PTS>
	T& Queue<T>::Emplace(PTS&&...ps)
	{
		auto idx = tail;
		new (buf + tail++) T(std::forward<PTS>(ps)...);
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

		if (!memmoveAble)
		{
			//......Head+++++++++++Tail......
			if (head < tail)
			{
				for (auto i = head; i < tail; ++i) buf[i].~T();
			}
			// ++++++Tail...........Head++++++
			else
			{
				for (uint32_t i = 0; i < tail; ++i) buf[i].~T();
				for (auto i = head; i < bufLen; ++i) buf[i].~T();
			}
		}
		//........HT......................
		head = tail = 0;
	}

	template <typename T>
	uint32_t Queue<T>::PopMulti(uint32_t const& count)
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
			if (!memmoveAble)
			{
				//......Head+++++++++++count......
				for (auto i = head; i < head + count; ++i) buf[i].~T();
			}
			head += count;
		}
		// ++++++Tail...........Head++++++
		else
		{
			auto frontDataLen = bufLen - head;
			//...Head+++
			if (count < frontDataLen)
			{
				if (!memmoveAble)
				{
					for (auto i = head; i < head + count; ++i) buf[i].~T();
				}
				head += count;
			}
			else
			{
				//...Head++++++
				if (!memmoveAble)
				{
					for (auto i = head; i < bufLen; ++i) buf[i].~T();
				}

				// <-Head
				head = count - frontDataLen;

				// ++++++Head...
				if (!memmoveAble)
				{
					for (uint32_t i = 0; i < head; ++i) buf[i].~T();
				}
			}
		}
		return count;
	}

	template <class T>
	void Queue<T>::Reserve(uint32_t const& capacity, bool afterPush)
	{
		assert(capacity > 0);
		if (capacity <= bufLen) return;

		auto newBufByteLen = Round2n(capacity * sizeof(T) + sizeof(MemHeader_VersionNumber)) - sizeof(MemHeader_VersionNumber);
		auto newBuf = (T*)mempool().Alloc((uint32_t)newBufByteLen);
		auto newBufLen = uint32_t(newBufByteLen / sizeof(T));

		// afterPush: ++++++++++++++TH++++++++++++++++
		auto dataLen = afterPush ? bufLen : Count();

		//......Head+++++++++++Tail.......
		if (head < tail)
		{
			if (memmoveAble)
			{
				std::memcpy(newBuf, buf + head, dataLen * sizeof(T));
			}
			else
			{
				for (uint32_t i = 0; i < dataLen; ++i)
				{
					new (newBuf + i) T((T&&)buf[head + i]);
					//Delete(srcIdx + i);
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
			if (memmoveAble)
			{
				std::memcpy(newBuf, buf + head, frontDataLen * sizeof(T));
			}
			else
			{
				for (uint32_t i = 0; i < frontDataLen; ++i)
				{
					new (newBuf + i) T((T&&)buf[head + i]);
					buf[head + i].~T();
				}
			}

			// ++++++Tail...
			if (memmoveAble)
			{
				std::memcpy(newBuf + frontDataLen, buf, tail * sizeof(T));
			}
			else
			{
				for (uint32_t i = 0; i < tail; ++i)
				{
					new (newBuf + frontDataLen + i) T((T&&)buf[i]);
					buf[i].~T();
				}
			}
		}

		// Head+++++++++++Tail.............
		head = 0;
		tail = dataLen;

		mempool().Free(buf);
		buf = newBuf;
		bufLen = newBufLen;
	}


	template<typename T>
	T const& Queue<T>::operator[](uint32_t const& idx) const
	{
		return At(idx);
	}

	template<typename T>
	T& Queue<T>::operator[](uint32_t const& idx)
	{
		return At(idx);
	}

	template<typename T>
	T const& Queue<T>::At(uint32_t const& idx) const
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
	T& Queue<T>::At(uint32_t const& idx)
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
		return buf[tail - 1 == (uint32_t)-1 ? bufLen - 1 : tail - 1];
	}
	template<typename T>
	T& Queue<T>::Last()
	{
		assert(head != tail);
		return buf[tail - 1 == (uint32_t)-1 ? bufLen - 1 : tail - 1];
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
