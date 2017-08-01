#pragma once
#include "xx_mempool.h"
#include <cstring>

namespace xx
{
	struct BBuffer;

	// 类似 std vector / .net List 的简化容器
	// reservedHeaderLen 为分配 buf 内存后在前面空出一段内存不用, 也不初始化, 扩容不复制( 为附加头部数据创造便利 )
	template<typename T, uint32_t reservedHeaderLen = 0>
	struct List : public MPObject
	{
		typedef T ChildType;
		T*			buf;
		uint32_t    bufLen;
		uint32_t    dataLen;

		explicit List(uint32_t const& capacity = 0)
		{
			if (capacity == 0)
			{
				buf = nullptr;
				bufLen = 0;
			}
			else
			{
				auto bufByteLen = Round2n((reservedHeaderLen + capacity) * sizeof(T) + sizeof(MemHeader_VersionNumber)) - sizeof(MemHeader_VersionNumber);
				buf = (T*)mempool().Alloc((uint32_t)bufByteLen) + reservedHeaderLen;
				bufLen = uint32_t(bufByteLen / sizeof(T)) - reservedHeaderLen;
			}
			dataLen = 0;
		}
		~List()
		{
			Clear(true);
		}
		List(List const&o) = delete;
		List(List &&o)
			: buf(o.buf)
			, bufLen(o.bufLen)
			, dataLen(o.dataLen)
		{
			o.buf = nullptr;
			o.bufLen = 0;
			o.dataLen = 0;
		}
		List& operator=(List const&o) = delete;
		List(BBuffer* bb);


		void Reserve(uint32_t const& capacity)
		{
			if (capacity <= bufLen) return;

			auto newBufByteLen = Round2n(reservedHeaderLen + capacity * sizeof(T) + sizeof(MemHeader_VersionNumber)) - sizeof(MemHeader_VersionNumber);
			auto newBuf = (T*)mempool().Alloc((uint32_t)newBufByteLen) + reservedHeaderLen;

			if (std::is_trivial<T>::value || MemmoveSupport_v<T>)
			{
				memcpy(newBuf, buf, dataLen * sizeof(T));
			}
			else
			{
				for (uint32_t i = 0; i < dataLen; ++i)
				{
					new (&newBuf[i]) T((T&&)buf[i]);
					buf[i].~T();
				}
			}
			// memcpy(newBuf - reservedHeaderLen, buf - reservedHeaderLen, reservedHeaderLen * sizeof(T));

			if (buf) mempool().Free(buf - reservedHeaderLen);
			buf = newBuf;
			bufLen = uint32_t(newBufByteLen / sizeof(T)) - reservedHeaderLen;
		}

	private:
		template<typename U = T, typename...Args>
		void PlacementNew(std::enable_if_t< IsMemHeaderBox_v<U> > *p, Args &&... args)
		{
			new (p) U(mempool(), std::forward<Args>(args)...);
		}
		template<typename U = T, typename...Args>
		void PlacementNew(std::enable_if_t< !IsMemHeaderBox_v<U> > *p, Args &&... args)
		{
			new (p) U(std::forward<Args>(args)...);
		}

	public:

		// 返回扩容前的长度
		uint32_t Resize(uint32_t const& len)
		{
			if (len == dataLen) return dataLen;
			else if (len < dataLen && !std::is_trivial<T>::value)
			{
				for (uint32_t i = len; i < dataLen; ++i)
				{
					buf[i].~T();
				}
			}
			else // len > dataLen
			{
				Reserve(len);
				if (!std::is_pod<T>::value)
				{
					for (uint32_t i = dataLen; i < len; ++i)
					{
						PlacementNew(buf + i);		//new (buf + i) T();
					}
				}
			}
			auto rtv = dataLen;
			dataLen = len;
			return rtv;
		}

		T const& operator[](uint32_t const& idx) const
		{
			assert(idx < dataLen);
			return buf[idx];
		}
		T& operator[](uint32_t const& idx)
		{
			assert(idx < dataLen);
			return buf[idx];
		}

		T const& At(uint32_t const& idx) const
		{
			assert(idx < dataLen);
			return buf[idx];
		}
		T& At(uint32_t const& idx)
		{
			assert(idx < dataLen);
			return buf[idx];
		}

		uint32_t Count() const
		{
			return dataLen;
		}

		T& Top()
		{
			assert(dataLen > 0);
			return buf[dataLen - 1];
		}
		void Pop()
		{
			assert(dataLen > 0);
			--dataLen;
			buf[dataLen].~T();
		}
		T const& Top() const
		{
			assert(dataLen > 0);
			return buf[dataLen - 1];
		}
		bool TryPop(T& output)
		{
			if (!dataLen) return false;
			output = (T&&)buf[--dataLen];
			buf[dataLen].~T();
			return true;
		}

		void Clear(bool const& freeBuf = false)
		{
			if (!buf) return;
			if (dataLen)
			{
				for (uint32_t i = dataLen - 1; i != (uint32_t)-1; --i)
				{
					buf[i].~T();
				}
				dataLen = 0;
			}
			if (freeBuf)
			{
				mempool().Free(buf - reservedHeaderLen);
				buf = nullptr;
				bufLen = 0;
			}
		}

		// 移除指定索引的元素. 为紧凑排列, 可能产生内存移动
		void RemoveAt(uint32_t const& idx)
		{
			assert(idx < dataLen);
			--dataLen;
			if (std::is_trivial<T>::value || MemmoveSupport_v<T>)
			{
				buf[idx].~T();
				memmove(buf + idx, buf + idx + 1, (dataLen - idx) * sizeof(T));
			}
			else
			{
				for (uint32_t i = idx; i < dataLen; ++i)
				{
					buf[i] = (T&&)buf[i + 1];
				}
				buf[dataLen].~T();
			}
		}

		// 遍历查找并移除
		void Remove(T const& v)
		{
			for (uint32_t i = 0; i < dataLen; ++i)
			{
				if (EqualsTo(v, buf[i]))
				{
					RemoveAt(i);
					return;
				}
			}
		}

	protected:

		// 不越界检查
		void FastAdd(T const& v)
		{
			new (buf + dataLen) T(v);
			++dataLen;
		}

		// 不越界检查( 右值版 )
		void FastAdd(T&& v)
		{
			new (buf + dataLen) T((T&&)v);
			++dataLen;
		}

	public:

		// 移动 / 复制添加一到多个
		template<typename...VS>
		void AddMulti(VS&&...vs)
		{
			static_assert(sizeof...(vs), "lost args ??");
			Reserve(dataLen + sizeof...(vs));
			std::initializer_list<int>{ (FastAdd(std::forward<VS>(vs)), 0)... };
		}

		// 追加一个 item( 右值版 )
		void Add(T&& v)
		{
			Emplace((T&&)v);
		}
		// 追加一个 item
		void Add(T const& v)
		{
			Emplace(v);
		}

		// 用参数直接构造一个
		template<typename...Args>
		T& Emplace(Args &&... args)
		{
			Reserve(dataLen + 1);
			auto& p = buf[dataLen++];
			/*PlacementNew(&p, std::forward<Args>(args)...);	*/
			new (&p) T(std::forward<Args>(args)...);
			return p;
		}

		// 首参传 mempool() 的 Emplace
		template<typename...Args>
		T& EmplaceMP(Args &&... args)
		{
			return Emplace(mempool(), std::forward<Args>(args)...);
		}


		void InsertAt(uint32_t const& idx, T&& v)
		{
			EmplaceAt(idx, (T&&)v);
		}
		void InsertAt(uint32_t const& idx, T const& v)
		{
			EmplaceAt(idx, v);
		}

		// 用参数直接构造一个到指定位置
		template<typename...Args>
		T& EmplaceAt(uint32_t idx, Args&&...args)
		{
			Reserve(dataLen + 1);
			if (idx < dataLen)
			{
				if (std::is_trivial<T>::value || MemmoveSupport_v<T>)
				{
					memmove(buf + idx + 1, buf + idx, (dataLen - idx) * sizeof(T));
				}
				else
				{
					new (buf + dataLen) T((T&&)buf[dataLen - 1]);
					for (uint32_t i = dataLen - 1; i > idx; --i)
					{
						buf[i] = (T&&)buf[i - 1];
					}
					buf[idx].~T();
				}
			}
			else idx = dataLen;
			++dataLen;
			/*PlacementNew(buf + idx, std::forward<Args>(args)...);*/
			new (buf + idx) T(std::forward<Args>(args)...);
			return buf[idx];
		}

		// 首参传 mempool() 的 Emplace
		template<typename...Args>
		T& EmplaceAtMP(uint32_t idx, Args &&... args)
		{
			return EmplaceAt(idx, mempool(), std::forward<Args>(args)...);
		}


		void AddRange(T const* items, uint32_t count)
		{
			Reserve(dataLen + count);
			if (std::is_trivial<T>::value || MemmoveSupport_v<T>)
			{
				std::memcpy(buf, items, count * sizeof(T));
			}
			else
			{
				for (uint32_t i = 0; i < count; ++i)
				{
					new (&buf[dataLen + i]) T((T&&)items[i]);
				}
			}
			dataLen += count;
		}



		// 如果找到就返回索引. 找不到将返回 uint32_t(-1)
		uint32_t Find(T const& v)
		{
			for (uint32_t i = 0; i < dataLen; ++i)
			{
				if (EqualsTo(v, buf[i])) return i;
			}
			return uint32_t(-1);
		}

		// 如果找到就返回索引. 找不到将返回 uint32_t(-1)
		uint32_t Find(std::function<bool(T&)> cond)
		{
			for (uint32_t i = 0; i < dataLen; ++i)
			{
				if (cond(buf[i])) return i;
			}
			return uint32_t(-1);
		}

		// 返回是否存在
		bool Exists(std::function<bool(T&)> cond)
		{
			return Find(cond) != uint32_t(-1);
		}

		bool TryFill(T& out, std::function<bool(T&)> cond)
		{
			auto idx = Find(cond);
			if (idx == uint32_t(-1)) return false;
			out = buf[idx];
			return true;
		}


		// handler 返回 false 则 foreach 终止
		void ForEach(std::function<bool(T&)> handler)
		{
			for (uint32_t i = 0; i < dataLen; ++i)
			{
				if (handler(buf[i])) return;
			}
		}


		template<typename U = T>
		std::enable_if_t<IsMPObject_v<U>> ReleaseWithItems()
		{
			auto& mp = mempool();
			for (uint32_t i = 0; i < dataLen; ++i)
			{
				mp.Release(buf[i]);
			}
			mp.Release(this);
		}



		// 只是为了能 for( auto c : 
		struct Iter
		{
			T *ptr;
			bool operator!=(Iter const& other) { return ptr != other.ptr; }
			Iter& operator++()
			{
				++ptr;
				return *this;
			}
			T& operator*() { return *ptr; }
		};
		Iter begin()
		{
			return Iter{ buf };
		}
		Iter end()
		{
			return Iter{ buf + dataLen };
		}
		Iter begin() const
		{
			return Iter{ buf };
		}
		Iter end() const
		{
			return Iter{ buf + dataLen };
		}



		/*************************************************************************/
		// 实现 ToString 接口
		/*************************************************************************/

		// 实现代码在 xx_string.h

		virtual void ToString(String &str) const override;

		/*************************************************************************/
		// 实现 BBuffer 接口
		/*************************************************************************/

		// 实现代码在 xx_bbuffer.h

		virtual void ToBBuffer(BBuffer &bb) const override;

		virtual int FromBBuffer(BBuffer &bb) override;
	};


	/*************************************************************************/
	// 值类型使用形态包装
	/*************************************************************************/

	template<typename T, uint32_t reservedHeaderLen = 0>
	using List_v = MemHeaderBox<List<T, reservedHeaderLen>>;

	// 标记在容器中可 memmove

	template<typename T, uint32_t reservedHeaderLen>
	struct MemmoveSupport<MemHeaderBox<List<T, reservedHeaderLen>>>
	{
		static const bool value = true;
	};
}
