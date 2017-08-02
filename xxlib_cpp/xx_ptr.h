#pragma once
#include "xx_defines.h"

namespace xx
{
	struct MemPool;

	// like unique ptr, for release pointer when leave scope
	template<typename T>
	struct Ptr
	{
		T* pointer;

		Ptr()
			: pointer(nullptr)
		{
		}

		Ptr(T* const& pointer)
			: pointer(pointer)
		{
		}

		template<typename ...Args>
		Ptr(MemPool& mp, Args &&... args);
		//{
		//	if (!mp.CreateTo(pointer, std::forward<Args>(args)...)) throw;
		//}

		~Ptr()
		{
			if (pointer)
			{
				pointer->Release();
				pointer = nullptr;
			}
		}

		Ptr(Ptr&& o)
			: pointer(o.pointer)
		{
			o.pointer = nullptr;
		}

		Ptr(Ptr const&) = delete;
		Ptr& operator=(Ptr const&) = delete;

		Ptr& operator=(T* const& o)
		{
			if (pointer)
			{
				pointer->Release();
			}
			pointer = o;
			return *this;
		}

		T const* const& operator->() const
		{
			return pointer;
		}

		T* & operator->()
		{
			return pointer;
		}

		T& operator*()
		{
			return *pointer;
		}

		T const& operator*() const
		{
			return *pointer;
		}

		operator bool() const
		{
			return pointer != nullptr;
		}

		operator T const* const& () const
		{
			return pointer;
		}
		operator T* &()
		{
			return pointer;
		}

		template<typename ...Args>
		T* & Create(MemPool& mp, Args &&... args);
		//{
		//	if (!pointer)
		//	{
		//		mp.CreateTo(pointer, std::forward<Args>(args)...);
		//	}
		//	return pointer;
		//}
	};


	template<typename T>
	struct MemmoveSupport<Ptr<T>>
	{
		static const bool value = true;
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


	template<typename T>
	Ptr<T> MakePtr(T* p)
	{
		return Ptr<T>(p);
	}
}
