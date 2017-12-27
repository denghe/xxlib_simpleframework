#pragma once
#include "xx_mempool.h"
#include <type_traits>

namespace xx
{
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
}
