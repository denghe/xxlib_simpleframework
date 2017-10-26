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

		template<typename O>
		Ptr(Ptr<O>&& o)
			: pointer(o.pointer)
		{
			static_assert(std::is_base_of<T, O>::value, "");
			o.pointer = nullptr;
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

		//Ptr& operator=(Ptr&& o)
		//{
		//	std::swap(pointer, o.pointer);
		//	return *this;
		//}

		template<typename O>
		Ptr& operator=(Ptr<O>&& o)
		{
			static_assert(std::is_base_of<T, O>::value, "");
			if (pointer) pointer->Release();
			pointer = o.pointer;
			o.pointer = nullptr;
			return *this;
		}

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

		T const* operator->() const
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

		//operator T const* const& () const
		//{
		//	return pointer;
		//}
		//operator T* const&()
		//{
		//	return pointer;
		//}

		operator Ref<T> ()
		{
			return pointer;
		}

		template<typename ...Args>
		Ptr<T>& Create(MemPool& mp, Args &&... args);
		//{
		//	if (!pointer)
		//	{
		//		mp.CreateTo(pointer, std::forward<Args>(args)...);
		//	}
		//	return pointer;
		//}

		template<typename BT, typename U = std::enable_if_t<std::is_base_of_v<BT, T>>>
		Ptr<BT>&& Move()
		{
			return std::move(*(Ptr<BT>*)this);
		}
		Ptr<T>&& Move()
		{
			return std::move(*this);
		}
		template<typename BT, typename U = std::enable_if_t<std::is_base_of_v<BT, T>>>
		Ptr<BT> Copy() const
		{
			return (BT*)pointer;
		}
		Ptr<T> Copy() const
		{
			return pointer;
		}
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

	using Object_p = Ptr<Object>;
}
