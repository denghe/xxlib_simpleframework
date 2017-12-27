#pragma once
#include "xx_object.h"
#include <cstring>
#include <functional>

namespace xx
{
	struct BBuffer;

	// std::vector / .net List like
	template<typename T>
	class List : public Object
	{
	public:
		typedef T ChildType;
		T*			buf;
		size_t		bufLen;
		size_t		dataLen;

		explicit List(MemPool* mempool, size_t const& capacity = 0)
			: Object(mempool)
		{
			if (capacity == 0)
			{
				buf = nullptr;
				bufLen = 0;
			}
			else
			{
				// 充分利用 2^n 空间
				auto bufByteLen = MemPool::Round2n(capacity * sizeof(T) + sizeof(MemHeader)) - sizeof(MemHeader);
				buf = (T*)mempool->Alloc(bufByteLen);
				if (!buf) throw - 1;
				bufLen = bufByteLen / sizeof(T);
			}
			dataLen = 0;
		}

		~List()
		{
			Clear(true);
		}

		List(List &&o)
			: buf(o.buf)
			, bufLen(o.bufLen)
			, dataLen(o.dataLen)
		{
			o.buf = nullptr;
			o.bufLen = 0;
			o.dataLen = 0;
		}

		List(List const&o) = delete;
		List& operator=(List const&o) = delete;


		void Reserve(size_t const& capacity)
		{
			if (capacity <= bufLen) return;

			auto newBufByteLen = MemPool::Round2n(capacity * sizeof(T) + sizeof(MemHeader)) - sizeof(MemHeader);
			auto newBuf = (T*)mempool->Alloc(newBufByteLen);
			if (!newBuf) throw - 1;

			if constexpr(std::is_trivial<T>::value || IsPtr_v<T>)
			{
				memcpy(newBuf, buf, dataLen * sizeof(T));
			}
			else
			{
				for (size_t i = 0; i < dataLen; ++i)
				{
					new (&newBuf[i]) T((T&&)buf[i]);
					buf[i].~T();
				}
			}

			if (buf) mempool->Free(buf);
			buf = newBuf;
			bufLen = size_t(newBufByteLen / sizeof(T));
		}

		// 如果不想初始化扩容的单元, 直接 Reserve + 改 dataLen
		void Resize(size_t const& len)
		{
			if (len == dataLen) return;
			else if (len < dataLen)
			{
				for (size_t i = len; i < dataLen; ++i)
				{
					buf[i].~T();
				}
			}
			else // len > dataLen
			{
				Reserve(len);
				for (size_t i = dataLen; i < len; ++i)
				{
					new (buf + i) T();
				}
			}
			auto rtv = dataLen;
			dataLen = len;
			return rtv;
		}

		T const& operator[](size_t const& idx) const noexcept
		{
			assert(idx < dataLen);
			return buf[idx];
		}

		T& operator[](size_t const& idx) noexcept
		{
			assert(idx < dataLen);
			return buf[idx];
		}

		T const& At(size_t const& idx) const noexcept
		{
			assert(idx < dataLen);
			return buf[idx];
		}

		T& At(size_t const& idx) noexcept
		{
			assert(idx < dataLen);
			return buf[idx];
		}

		T& Top() noexcept
		{
			assert(dataLen > 0);
			return buf[dataLen - 1];
		}

		void Pop() noexcept
		{
			assert(dataLen > 0);
			--dataLen;
			buf[dataLen].~T();
		}

		T const& Top() const noexcept
		{
			assert(dataLen > 0);
			return buf[dataLen - 1];
		}

		bool TryPop(T& output) noexcept
		{
			if (!dataLen) return false;
			output = (T&&)buf[--dataLen];
			buf[dataLen].~T();
			return true;
		}

		void Clear(bool const& freeBuf = false) noexcept
		{
			if (!buf) return;
			if (dataLen)
			{
				for (size_t i = dataLen - 1; i != (size_t)-1; --i)
				{
					buf[i].~T();
				}
				dataLen = 0;
			}
			if (freeBuf)
			{
				mempool->Free(buf);
				buf = nullptr;
				bufLen = 0;
			}
		}

		void RemoveAt(size_t const& idx) noexcept
		{
			assert(idx < dataLen);
			--dataLen;
			if constexpr(std::is_trivial<T>::value || IsPtr_v<T>)
			{
				buf[idx].~T();
				memmove(buf + idx, buf + idx + 1, (dataLen - idx) * sizeof(T));
			}
			else
			{
				for (size_t i = idx; i < dataLen; ++i)
				{
					buf[i] = (T&&)buf[i + 1];
				}
				buf[dataLen].~T();
			}
		}

		// 和最后一个元素做交换删除
		void SwapRemoveAt(size_t const& idx)
		{
			if (idx + 1 < dataLen)
			{
				std::swap(buf[idx], buf[dataLen - 1]);
			}
			dataLen--;
			buf[dataLen].~T();
		}

		// 遍历查找并移除
		void Remove(T const& v) noexcept
		{
			for (size_t i = 0; i < dataLen; ++i)
			{
				if (EqualsTo(v, buf[i]))
				{
					RemoveAt(i);
					return;
				}
			}
		}

		template<typename...Args>
		T& Emplace(Args &&... args)
		{
			Reserve(dataLen + 1);
			auto& p = buf[dataLen++];
			if constexpr(std::is_base_of_v<Object, T>)
			{
				new (&p) T(mempool, std::forward<Args>(args)...);
			}
			else
			{
				new (&p) T(std::forward<Args>(args)...);
			}
			return p;
		}

		// 用参数直接构造一个到指定位置
		template<typename...Args>
		T& EmplaceAt(size_t idx, Args&&...args)
		{
			Reserve(dataLen + 1);
			if (idx < dataLen)
			{
				if constexpr(std::is_trivial<T>::value || IsPtr_v<T>)
				{
					memmove(buf + idx + 1, buf + idx, (dataLen - idx) * sizeof(T));
				}
				else
				{
					new (buf + dataLen) T((T&&)buf[dataLen - 1]);
					for (size_t i = dataLen - 1; i > idx; --i)
					{
						buf[i] = (T&&)buf[i - 1];
					}
					buf[idx].~T();
				}
			}
			else idx = dataLen;
			++dataLen;
			new (buf + idx) T(std::forward<Args>(args)...);
			return buf[idx];
		}


		void Add(T&& v)
		{
			Emplace((T&&)v);
		}

		void Add(T const& v)
		{
			Emplace(v);
		}

		void InsertAt(size_t const& idx, T&& v)
		{
			EmplaceAt(idx, (T&&)v);
		}

		void InsertAt(size_t const& idx, T const& v)
		{
			EmplaceAt(idx, v);
		}

		void AddRange(T const* items, size_t count)
		{
			Reserve(dataLen + count);
			if constexpr(std::is_trivial<T>::value || IsPtr_v<T>)
			{
				std::memcpy(buf, items, count * sizeof(T));
			}
			else
			{
				for (size_t i = 0; i < count; ++i)
				{
					new (&buf[dataLen + i]) T((T&&)items[i]);
				}
			}
			dataLen += count;
		}

		// 如果找到就返回索引. 找不到将返回 size_t(-1)
		size_t Find(T const& v) noexcept
		{
			for (size_t i = 0; i < dataLen; ++i)
			{
				if (EqualsTo(v, buf[i])) return i;
			}
			return size_t(-1);
		}

		// 如果找到就返回索引. 找不到将返回 size_t(-1)
		size_t Find(std::function<bool(T&)> cond)
		{
			for (size_t i = 0; i < dataLen; ++i)
			{
				if (cond(buf[i])) return i;
			}
			return size_t(-1);
		}

		bool Exists(std::function<bool(T&)> cond)
		{
			return Find(cond) != size_t(-1);
		}

		bool TryFill(T& out, std::function<bool(T&)> cond)
		{
			auto idx = Find(cond);
			if (idx == size_t(-1)) return false;
			out = buf[idx];
			return true;
		}


		// handler 返回 false 则 foreach 终止
		void ForEach(std::function<bool(T&)> handler)
		{
			for (size_t i = 0; i < dataLen; ++i)
			{
				if (handler(buf[i])) return;
			}
		}

		// 倒着扫. 便于 SwapRemoveAt.
		void ForEachRevert(std::function<bool(T&)> handler)
		{
			for (size_t i = dataLen - 1; i != (size_t)-1; --i)
			{
				if (handler(buf[i])) return;
			}
		}


		// for easy use
		template<typename...Args>
		static Ptr<List<T>> CreatePtr(MemPool* mempool, Args&&...args)
		{
			return mempool->Create<List<T>>(std::forward<Args>(args)...);
		}


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
	};


	template<typename T>
	using List_p = Ptr<List<T>>;
}
