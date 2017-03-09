#pragma once
#include "xx_mempoolbase.h"

namespace xx
{

	/*
	// 示例:

	// headers ...

	// 重点: type 排列顺序 须满足 子在前父在后. 彼此无关的无所谓
	typedef xx::MemPool<C,B,A......> MP;

	// impls ...

	MP mp;
	auto a = mp.Create<A>(.....);
	*/


	// 整套库的核心内存分配组件. 按 2^N 尺寸划分内存分配行为, 将 free 的指针放入 stack 缓存复用
	// 对于分配出来的内存, 自增 版本号 将填充在 指针 -8 区( Alloc ). 用于判断指针是否已失效
	// MPObject 对象使用 Create / Release 来创建和析构
	template<typename ... Types>
	struct MemPool : MemPoolBase
	{
		// 将 Types 声明为 Tuple 以享用 std 提供的 tuple 元组系列操作
		typedef std::tuple<MPObject, Types...> Tuple;

		// 类型个数
		static const int typesSize = std::tuple_size<Tuple>::value;

		// 放置 type 对应的 parent 的 type id. 0: MPObject
		std::array<uint32_t, 1 + sizeof...(Types)> pids;


		MemPool(MemPool const&) = delete;
		MemPool& operator=(MemPool const &) = delete;
		MemPool()
		{
			FillPids(std::make_index_sequence<typesSize>());
		}

		/***********************************************************************************/
		// 内存分配( Create / Release 系列 ). 仅针对派生自 MPObject 的对象
		/***********************************************************************************/

		// 该操作将会在头部填充 MemHeader_MPObject
		template<typename T, typename ...Args>
		T* Create(Args &&... args)
		{
			static_assert(std::is_base_of<MPObject, T>::value, "the T must be inerit of MPObject.");

			// 下列代码 复制自 Alloc 函数小改
			auto siz = sizeof(T) + sizeof(MemHeader_MPObject);
			auto idx = Calc2n(siz);
			if (siz > (size_t(1) << idx)) siz = size_t(1) << ++idx;

			void* rtv;
			if (!stacks[idx].TryPop(rtv)) rtv = malloc(siz);

			auto p = (MemHeader_MPObject*)rtv;
			p->versionNumber = (++versionNumber) | ((uint64_t)idx << 56);
			p->mempoolbase = this;
			p->refCount = 1;
			p->typeId = (decltype(p->typeId))TupleIndexOf<T, Tuple>::value;
			p->tsFlags = 0;
			return new (p + 1) T(std::forward<Args>(args)...);
		}

		/***********************************************************************************/
		// helpers
		/***********************************************************************************/

		template<typename T, typename ...Args>
		MPtr<T> CreateMPtr(Args &&... args)
		{
			return Create<T>(std::forward<Args>(args)...);
		}

		template<typename T, typename ...Args>
		void CreateTo(T*& outPtr, Args &&... args)
		{
			outPtr = Create<T>(std::forward<Args>(args)...);
		}
		template<typename T, typename ...Args>
		void CreateTo(MPtr<T>& outPtr, Args &&... args)
		{
			outPtr = CreateMPtr<T>(std::forward<Args>(args)...);
		}



		// 根据 typeid 判断父子关系
		bool IsBaseOf(uint32_t baseTypeId, uint32_t typeId)
		{
			for (; typeId != baseTypeId; typeId = pids[typeId])
			{
				if (!typeId) return false;
			}
			return true;
		}

		// 根据 类型 判断父子关系
		template<typename BT, typename T>
		bool IsBaseOf()
		{
			return IsBaseOf(TupleIndexOf<BT, Tuple>::value, TupleIndexOf<T, Tuple>::value);
		}

		// 试将指针 p 转成 T* 类型. 取代 dynamic_cast
		template<typename T>
		T* TryCast(MPObject* p)
		{
			return IsBaseOf(TupleIndexOf<T, Tuple>::value, p->typeId()) ? (T*)p : nullptr;
		}

	protected:
		/***********************************************************************************/
		// pids 填充
		/***********************************************************************************/

		template<size_t... Indexs>
		void FillPids(std::index_sequence<Indexs...> const& idxs)
		{
			std::initializer_list<int>{ (FillPids(idxs, (std::tuple_element_t<Indexs, Tuple>*)nullptr), 0)... };
		}

		template<size_t... Indexs, typename T>
		void FillPids(std::index_sequence<Indexs...> const& idxs, T* const& ct)
		{
			uint16_t pid = 0;
			std::initializer_list<int>{ ((
				!std::is_same_v<std::tuple_element_t<Indexs, Tuple>, T>
				&& !pid 
				&& std::is_base_of_v<std::tuple_element_t<Indexs, Tuple>, T>
				? (pid = Indexs) : 0
				), 0)... };
			pids[TupleIndexOf<T, Tuple>::value] = pid;
			//std::cout << "tid = " << TupleIndexOf<T, Tuple>::value << ", t = " << typeid(T).name() << ", pid = " << pid << std::endl;
		}
	};
}
