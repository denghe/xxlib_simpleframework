#pragma once
#include "xx_mptr.h"

namespace xx
{
	struct String;


	// 支持 MemPool 的类都应该从该基类派生
	struct MPObject
	{
		virtual ~MPObject() {}

		// 加持
		inline void AddRef()
		{
			++refCount();
		}

		/*
		if (!tsFlags()) return; else tsFlags() = 1;
		// str.append( .......... )........
		tsFlags() = 0;
		*/
		virtual void ToString(String &str) const;

		// 减持 或 析构 + 回收变野( 代码的实现在 xx_mempoolbase.h 的尾部 )
		void Release();

		/***********************************************************************************/
		// 下面是访问头部数据的各种 helpers
		/***********************************************************************************/

		inline MemHeader_MPObject& memHeader() { return *((MemHeader_MPObject*)this - 1); }
		inline MemHeader_MPObject& memHeader() const { return *((MemHeader_MPObject*)this - 1); }

		inline uint64_t const& versionNumber() const { return memHeader().versionNumber; }
		inline uint64_t pureVersionNumber() const { return versionNumber() & 0x00FFFFFFFFFFFFFFu; }

		inline uint32_t& refCount() { return memHeader().refCount; }
		inline uint32_t const& refCount() const { return memHeader().refCount; }

		inline uint16_t const& typeId() const { return memHeader().typeId; }

		inline uint16_t& tsFlags() { return memHeader().tsFlags; }
		inline uint16_t const& tsFlags() const { return memHeader().tsFlags; }

		inline MemPoolBase const& mempoolbase() const { return *(memHeader().mempoolbase); }
		inline MemPoolBase& mempoolbase() { return *(memHeader().mempoolbase); }

		template<typename T> T& mempool()
		{
			static_assert(std::is_base_of<MemPoolBase, T>::value, "the T must be a MemPool<...>");
			return *(T*)(memHeader().mempoolbase);
		}
	};


	/***********************************************************************************/
	// type_traits
	/***********************************************************************************/

	template<typename T>
	struct IsMPObject
	{
		static const bool value = !std::is_void<T>::value && std::is_base_of<MPObject, T>::value;
	};
	template<typename T>
	struct IsMPObject<T*>
	{
		static const bool value = !std::is_void<T>::value && std::is_base_of<MPObject, T>::value;
	};
	template<typename T>
	struct IsMPObject<MPtr<T>>
	{
		static const bool value = true;
	};
	template<typename T>
	constexpr bool IsMPObject_v = IsMPObject<T>::value;


	// 扫类型列表中是否含有 MPObject* 或 MPtr<MPObject> 类型
	template<typename ...Types>
	struct ExistsMPObject
	{
		template<class Tuple, size_t N> struct TupleScaner {
			static constexpr bool Exec()
			{
				return IsMPObject< std::tuple_element_t<N - 1, Tuple> >::value ? true : TupleScaner<Tuple, N - 1>::Exec();
			}
		};
		template<class Tuple> struct TupleScaner<Tuple, 1> {
			static constexpr bool Exec()
			{
				return IsMPObject< std::tuple_element_t<0, Tuple> >::value;
			}
		};
		static constexpr bool value = TupleScaner<std::tuple<Types...>, sizeof...(Types)>::Exec();
	};

}
