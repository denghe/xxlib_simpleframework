#pragma once
#include "xx_defines.h"        // Round2n
#include "xx_mempool.h"
#include <assert.h>

namespace xx
{
	// todo: is_pod, is_mptr
	// todo: 从 C# 版同步新增函数

	// ring buffer FIFO 队列
	// 能随机检索...能批量 pop
	//...............FR...............					// Head == Tail
	//......Head+++++++++++Tail.......					// DataLen = Tail - Head
	//++++++Tail...........Head+++++++					// DataLen = BufLen - Head + Tail
	template <class T>
	struct Queue : public MPObject
	{
		typedef T ChildType;
		T*			buf;
		uint32_t    bufLen;
		uint32_t	head = 0, tail = 0;					// FR..............................

		explicit Queue(uint32_t const& capacity = 8);
		~Queue();

		Queue(Queue const& o) = delete;
		Queue& operator=(Queue const& o) = delete;

		T const& operator[](uint32_t const& idx) const;	// [0] = Head()
		T& operator[](uint32_t const& idx);
		T const& At(uint32_t const& idx) const;				// []
		T& At(uint32_t const& idx);

		uint32_t Count() const;
		bool Empty() const;
		void Clear();
		void Reserve(uint32_t const& capacity, bool afterPush = false);

		template<typename...PTS>
		T& Emplace(PTS&&...ps);							// [ Tail++ ] = T( ps )
		void Enqueue(T const& v);
		void Enqueue(T&& v);
		T Dequeue();									// 会复制，不推荐（替代方案: 使用 Peek + Pop 或 TryDequeue）
		bool TryDequeue(T& outVal);
		T const& Peek() const;
		void Pop();										// ++Head
		uint32_t PopMulti(uint32_t const& count);		// Head += count
	};






	template <class T>
	Queue<T>::Queue(uint32_t const& capacity)
	{
		if (capacity == 0)
		{
			buf = nullptr;
			bufLen = 0;
		}
		else
		{
			auto bufByteLen = Round2n(capacity * sizeof(T) + sizeof(MemHeader_VersionNumber)) - sizeof(MemHeader_VersionNumber);
			buf = (T*)mempoolbase().Alloc((uint32_t)bufByteLen);
			bufLen = uint32_t(bufByteLen / sizeof(T));
		}
	}

	template <class T>
	Queue<T>::~Queue()
	{
		Clear();
		if (buf)
		{
			mempoolbase().Free(buf);
			buf = nullptr;
			bufLen = 0;
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
	template<typename...PTS>
	T& Queue<T>::Emplace(PTS&&...ps)
	{
		auto idx = tail;
		new (buf + tail) T(std::forward<PTS>(ps)...);
		if (tail == bufLen)			// cycle
		{
			tail = 0;
		}
		if (tail == head)			// no more space
		{
			idx = bufLen;
			Reserve(bufLen * 2, true);
		}
		return array[idx];
	}

	template <class T>
	void Queue<T>::Enqueue(T const& v)
	{
		Emplace(v);
	}

	template <class T>
	void Queue<T>::Enqueue(T&& v)
	{
		Emplace((T&&)v);
	}


	template <class T>
	void Queue<T>::Clear()
	{
		//........HT......................
		if (head == tail) return;

		if (!std::is_pod<T>::value && !IsMPtr<T>::value)
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

		auto dataLen = Size();
		if (count >= dataLen)
		{
			Clear();
			return dataLen;
		}
		// count < dataLen

		//......Head+++++++++++Tail......
		if (head < tail)
		{
			if (!std::is_pod<T>::value && !IsMPtr<T>::value)
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
				if (!std::is_pod<T>::value && !IsMPtr<T>::value)
				{
					for (auto i = head; i < head + count; ++i) buf[i].~T();
				}
				head += count;
			}
			else
			{
				//...Head++++++
				if (!std::is_pod<T>::value && !IsMPtr<T>::value)
				{
					for (auto i = head; i < bufLen; ++i) buf[i].~T();
				}

				// <-Head
				head = count - frontDataLen;

				// ++++++Head...
				if (!std::is_pod<T>::value && !IsMPtr<T>::value)
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
		auto newBuf = (T*)mempoolbase().Alloc((uint32_t)newBufByteLen);
		auto newBufLen = uint32_t(newBufByteLen / sizeof(T));

		// afterPush: ++++++++++++++TH++++++++++++++++
		auto dataLen = afterPush ? bufLen : Size();

		//......Head+++++++++++Tail.......
		if (head < tail)
		{
			if (std::is_pod<T>::value || IsMPtr<T>::value)
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
			if (std::is_pod<T>::value || IsMPtr<T>::value)
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
			if (std::is_pod<T>::value || IsMPtr<T>::value)
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
	T const& Queue<T>::Peek() const
	{
		assert(head != tail);
		return At(head);
	}

	template<typename T>
	T& Queue<T>::At(uint32_t const& idx)
	{
		assert(idx < Size());
		if (head + idx < bufLen)
		{
			return buf[head + idx];
		}
		else
		{
			return buf[head + idx - bufLen];
		}
	}


	template <typename T>
	T Queue<T>::Dequeue()
	{
		assert(head != tail);
		T rtv((T&&)array[head]);
		Pop();
		return rtv;
	}

	template <typename T>
	bool Queue<T>::TryDequeue(T& outVal)
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



#include <iostream>
int main()
{
	xx::Queue<int> q;


	return 0;
}
