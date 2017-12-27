#pragma once
#include <cassert>
#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <array>
#include <algorithm>
#include <type_traits>
#ifdef _WIN32
#include <intrin.h>     // _BitScanReverse  64
#endif

namespace xx
{

	// 经由 MemPool 分配的内存都带有这样一个头部
	struct MemHeader
	{
		union
		{
			// 自增版本号, 作为识别当前指针是否有效的重要标识
			uint64_t versionNumber;
			uint8_t bytes[8];
		};

		// 内存池数组 下标, 指向 versionNumber 的最高位字节. 
		// 当版本号用于存放指针链表值时, 不可能比这个值大, 不影响变野判定. 
		// 当前不支持大尾机
		uint8_t& mpIndex() { return bytes[7]; }
	};


	template<typename T>
	class Ptr;


	// 粗暴快速的非线程安全内存池
	class MemPool
	{
		static_assert(sizeof(size_t) <= sizeof(void*));

		// 数组长度涵盖所有 2^n 对齐分配长度规则
		std::array<void*, sizeof(void*) * 8> headers;

		// 自增版本号( 每次创建时先 ++ 再填充 )
		uint64_t versionNumber = 0;

	public:
		MemPool() noexcept
		{
			headers.fill(nullptr);
		}

		~MemPool() noexcept
		{
			for (auto header : headers)
			{
				while (header)
				{
					auto next = *(void**)header;
					free(header);
					header = next;
				}
			}
		}
		MemPool(MemPool const&) = delete;
		MemPool& operator=(MemPool const&) = delete;

		inline void* Alloc(size_t siz) noexcept
		{
			// 根据长度(预留 MemHeader)拿到链表下标
			assert(siz);
			siz += sizeof(MemHeader);
			auto idx = Calc2n(siz);
			if (siz > (size_t(1) << idx)) siz = size_t(1) << ++idx;

			// 试从链表拿指针. 拿不到就分配.
			auto p = headers[idx];
			if (p) headers[idx] = *(void**)p;
			else p = malloc(siz);
			if (!p) return nullptr;

			// 填充版本号以及链表下标
			auto h = (MemHeader*)p;
			h->versionNumber = ++versionNumber;
			h->mpIndex() = (uint8_t)idx;
			return h + 1;
		}

		inline void Free(void* p) noexcept
		{
			if (!p) return;
			auto h = (MemHeader*)p - 1;
			// 基于x64地址不会用到最高位的判断原则来检查重复释放
			assert(h->mpIndex() > 0 && h->mpIndex() < headers.size());
			auto idx = h->mpIndex();
			if constexpr(sizeof(void*) < 8) h->versionNumber = 0;
			*(void**)h = headers[idx];
			headers[idx] = h;
		}

		// 可用于创建 lua state
		inline void* Realloc(void *p, size_t newSize, size_t dataLen = -1) noexcept
		{
			if (!newSize)
			{
				Free(p);
				return nullptr;
			}
			if (!p) return Alloc(newSize);

			auto originalSize = (size_t(1) << *(size_t*)((void**)p - 1)) - sizeof(void*);
			if (originalSize >= newSize) return p;

			auto np = Alloc(newSize);
			memcpy(np, p, std::min(originalSize, dataLen));
			Free(p);
			return np;
		}

		/**************************************************************************************************/
		// utils
		/**************************************************************************************************/

		inline static size_t Calc2n(size_t n) noexcept
		{
			assert(n);
#ifdef _MSC_VER
			unsigned long r = 0;
#if defined(_WIN64) || defined(_M_X64)
			_BitScanReverse64(&r, n);
# else
			_BitScanReverse(&r, n);
# endif
			return (size_t)r;
#else
#if defined(__LP64__) || __WORDSIZE == 64
			return int(63 - __builtin_clzl(n));
# else
			return int(31 - __builtin_clz(n));
# endif
#endif
		}

		// 将长度值按 2^n 来对齐
		inline static size_t Round2n(size_t n) noexcept
		{
			auto rtv = size_t(1) << Calc2n(n);
			if (rtv == n) return n;
			else return rtv << 1;
		}


		/**************************************************************************************************/
		// exts for easy use
		/**************************************************************************************************/

		template<typename T>
		T* Alloc() noexcept
		{
			static_assert(std::is_pod_v<T>);
			return (T*)Alloc(sizeof(T));
		}

		inline void SafeFree(void*& p) noexcept
		{
			Free(p);
			p = nullptr;
		}

		template<typename T, typename ...Args>
		T* Create(Args &&... args);


		template<typename T, typename ...Args>
		Ptr<T> CreatePtr(Args &&... args);


		template<typename T>
		void Release(T* p) noexcept
		{
			p->~T();
			Free(p);
		}

		template<typename T>
		void SafeRelease(T*& p) noexcept
		{
			Release(p);
			p = nullptr;
		}
	};


	class Object
	{
	public:
		MemPool * mempool;
		Object(MemPool* mempool) : mempool(mempool) {}
		virtual ~Object() {}

		inline MemHeader& memHeader() { return *((MemHeader*)this - 1); }
		inline MemHeader& memHeader() const { return *((MemHeader*)this - 1); }

		inline uint64_t const& versionNumber() const { return memHeader().versionNumber; }
		inline uint64_t pureVersionNumber() const { return versionNumber() & 0x00FFFFFFFFFFFFFFu; }

		// todo: more interfaces
	};


	// std::unique_ptr like
	template<typename T>
	class Ptr
	{
	public:
		static_assert(std::is_base_of_v<Object, T>);
		T* pointer;

		Ptr() noexcept : pointer(nullptr) {}
		Ptr(T* const& pointer) noexcept : pointer(pointer) {}

		template<typename O>
		Ptr(Ptr<O>&& o) noexcept : pointer(o.pointer)
		{
			static_assert(std::is_base_of_v<T, O>);
			o.pointer = nullptr;
		}

		Ptr(Ptr&& o) noexcept : pointer(o.pointer)
		{
			o.pointer = nullptr;
		}

		Ptr(Ptr const&) = delete;

		~Ptr()
		{
			if (pointer)
			{
				pointer->mempool->Release(pointer);
				pointer = nullptr;
			}
		}

		template<typename O>
		Ptr& operator=(Ptr<O>&& o) noexcept
		{
			static_assert(std::is_base_of_v<T, O>);
			if (pointer) pointer->mempool->Release(pointer);
			pointer = o.pointer;
			o.pointer = nullptr;
			return *this;
		}

		Ptr& operator=(Ptr const&) = delete;

		Ptr& operator=(T* const& o) noexcept
		{
			if (pointer)
			{
				pointer->mempool->Release(pointer);
			}
			pointer = o;
			return *this;
		}

		T const* operator->() const noexcept
		{
			return pointer;
		}

		T* & operator->() noexcept
		{
			return pointer;
		}

		T& operator*() noexcept
		{
			return *pointer;
		}

		T const& operator*() const noexcept
		{
			return *pointer;
		}

		operator bool() const noexcept
		{
			return pointer != nullptr;
		}
	};

	template<typename T>
	struct IsPtr
	{
		static const bool value = false;
	};

	template<typename T>
	struct IsPtr<Ptr<T>>
	{
		static const bool value = true;
	};

	template<typename T>
	constexpr bool IsPtr_v = IsPtr<T>::value;



	template<typename T, typename ...Args>
	T* MemPool::Create(Args &&... args)
	{
		auto p = (void**)Alloc(sizeof(T));
		try
		{
			if constexpr(std::is_base_of_v<Object, T>)
			{
				return new (p) T(this, std::forward<Args>(args)...);
			}
			else
			{
				return new (p) T(std::forward<Args>(args)...);
			}
		}
		catch (...)
		{
			Free(p);
			throw - 1;
		}
	}


	template<typename T, typename ...Args>
	Ptr<T> MemPool::CreatePtr(Args &&... args)
	{
		return Create<T, Args...>(std::forward<Args>(args)...);
	}
}
