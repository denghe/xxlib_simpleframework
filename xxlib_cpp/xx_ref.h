#pragma once
#include "xx_defines.h"
#include "xx_memheader.h"

namespace xx
{
	struct Object;

	// 与 MemPool 的版本号相配合的智能指针之模板派生类
	template<typename T>
	struct Ref
	{
		typedef T ChildType;
		T* pointer = nullptr;
		uint64_t versionNumber = 0;

		Ref() {}
		Ref(Ref const &o) = default;
		Ref& operator=(Ref const& o) = default;
		Ref(T* p)
			: pointer(p)
			, versionNumber(p ? ((MemHeader_Object*)p - 1)->versionNumber : 0)
		{}
		Ref& operator=(T* p)
		{
			pointer = p;
			versionNumber = p ? ((MemHeader_Object*)p - 1)->versionNumber : 0;
			return *this;
		}

		template<typename IT>
		Ref(Ref<IT> const &p)
		{
			operator=(static_cast<T*>(p.Ensure()));
		}
		template<typename IT>
		Ref& operator=(Ref<IT> const& p)
		{
			operator=(static_cast<T*>(p.Ensure()));
		}

		bool operator==(Ref const &o) const
		{
			return Ensure() == o.Ensure();
		}
		T* Ensure() const
		{
			if (pointer && ((MemHeader_Object*)pointer - 1)->versionNumber == versionNumber) return pointer;
			return nullptr;
		}
		operator bool() const
		{
			return Ensure() != nullptr;
		}

		operator T const* () const
		{
			return Ensure();
		}
		operator T* ()
		{
			return Ensure();
		}

		// 这样直接用是不安全的
		T* operator->() const
		{
			return (T*)pointer;
		}
		T& operator*()
		{
			return *(T*)pointer;
		}
		T const& operator*() const
		{
			return *(T*)pointer;
		}

	};


	/***********************************************************************************/
	// type_traits
	/***********************************************************************************/

	template<typename T>
	struct IsRef
	{
		static const bool value = false;
	};

	template<typename T>
	struct IsRef<Ref<T>>
	{
		static const bool value = true;
	};
	template<typename T>
	constexpr bool IsRef_v = IsRef<T>::value;


	template<typename T>
	struct MemmoveSupport<Ref<T>>
	{
		static const bool value = true;
	};

	template<typename T>
	Ref<T> MakeRef(T* p)
	{
		return Ref<T>(p);
	}

}
