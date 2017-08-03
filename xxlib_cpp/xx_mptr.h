#pragma once
#include "xx_defines.h"
#include "xx_memheader.h"

namespace xx
{
	struct MPObject;

	// 与 MemPool 的版本号相配合的智能指针之模板派生类
	template<typename T>
	struct MPtr
	{
		typedef T ChildType;
		T* pointer = nullptr;
		uint64_t versionNumber = 0;

		MPtr() {}
		MPtr(MPtr const &o) = default;
		MPtr& operator=(MPtr const& o) = default;
		MPtr(T* p)
			: pointer(p)
			, versionNumber(p ? ((MemHeader_MPObject*)p - 1)->versionNumber : 0)
		{}
		MPtr& operator=(T* p)
		{
			pointer = p;
			versionNumber = p ? ((MemHeader_MPObject*)p - 1)->versionNumber : 0;
			return *this;
		}

		template<typename IT>
		MPtr(MPtr<IT> const &p)
		{
			operator=(static_cast<T*>(p.Ensure()));
		}
		template<typename IT>
		MPtr& operator=(MPtr<IT> const& p)
		{
			operator=(static_cast<T*>(p.Ensure()));
		}

		bool operator==(MPtr const &o) const
		{
			return Ensure() == o.Ensure();
		}
		T* Ensure() const
		{
			if (pointer && ((MemHeader_MPObject*)pointer - 1)->versionNumber == versionNumber) return pointer;
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
	struct IsMPtr
	{
		static const bool value = false;
	};

	template<typename T>
	struct IsMPtr<MPtr<T>>
	{
		static const bool value = true;
	};
	template<typename T>
	constexpr bool IsMPtr_v = IsMPtr<T>::value;


	template<typename T>
	struct MemmoveSupport<MPtr<T>>
	{
		static const bool value = true;
	};

	template<typename T>
	MPtr<T> MakeMPtr(T* p)
	{
		return MPtr<T>(p);
	}

}
