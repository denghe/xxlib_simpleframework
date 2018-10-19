#pragma once
#include "xxlib_podstack.h"
#include "xxlib_mpobject.h"
#include <array>
#include <cassert>
#include <cstring>

namespace xxlib
{
	// todo: 将 MemPool 改造成 template<typename ... Types> 形态, 从而避免繁杂的 register 过程. 用所谓"保险的"类特化 <T, ENABLE = void> 写法, 来改造一些基础工具函数


	// 整套库的核心内存分配组件. 按 2^N 尺寸划分内存分配行为, 将 free 的指针放入 stack 缓存复用
	// 对于分配出来的内存, 自增 版本号 将填充在 指针 -8 区. 用于判断指针是否已失效
	// MPObject 对象使用 Create / Release 来创建和析构
	struct MemPool
	{
		static const uint32_t stacksCount = sizeof(size_t) * 8;
		PodStack<void*>* stacks;
		uint64_t versionNumber;

		// 超出阈值的内存请求将失败
		explicit MemPool()
			: stacks(new PodStack<void*>[stacksCount])
			, versionNumber(0)
		{
		}
		MemPool(MemPool const &o) = delete;
		MemPool& operator=(MemPool const &o) = delete;
		MemPool(MemPool &&o)
		{
			operator=((MemPool &&)o);
		}
		MemPool& operator=(MemPool &&o)
		{
			std::swap(stacks, o.stacks);
			std::swap(versionNumber, o.versionNumber);
			return *this;
		}
		~MemPool()
		{
			ClearStack();
			delete[] stacks;
			stacks = nullptr;

			if (typeId2CreateFuncs)
			{
				delete typeId2CreateFuncs;
				typeId2CreateFuncs = nullptr;
			}
			if (typeId2ParentTypeIds)
			{
				delete typeId2ParentTypeIds;
				typeId2ParentTypeIds = nullptr;
			}
		}

		inline void ClearStack()
		{
			for (uint32_t i = 0; i < stacksCount; ++i)
			{
				auto& stack = stacks[i];
				for (uint32_t i = 0; i < stack.dataLen; ++i)
				{
					::free(stack[i]);
				}
				stack.Clear();
			}
		}

		inline bool Empty() const
		{
			for (uint32_t i = 0; i < stacksCount; ++i)
			{
				auto& stack = stacks[i];
				if (stack.dataLen) return false;
			}
			return true;
		}

		/**********************************************************************/
		// 直接拿内存的
		/**********************************************************************/

		// 该操作将会在头部区域填充 MPObjectHeader 并跳过, 返回偏移后的指针
		// 示例: auto siz = Round2n(capacity * sizeof(T) + 8) - 8;	 xxx = (T*) mempool().Alloc( siz );   len = (uint32_t)(siz / sizeof(T));
		
		inline void* Alloc(size_t siz)
		{
			siz += sizeof(MPObjectHeader);		// 空出放置 MPObjectHeader 的地儿
			auto idx = Calc2n(siz);
			if (siz > (size_t(1) << idx)) siz = size_t(1) << ++idx;
			if (idx >= stacksCount) return nullptr;

			void* rtv;
			if (!stacks[idx].TryPop(rtv)) rtv = (MPObjectHeader*)malloc(siz);

			auto p = (MPObjectHeader*)rtv;
			p->versionNumber = (++versionNumber) | ((uint64_t)idx << 56);		// 将数组下标附在最高位上, Free 要用
			return p + 1;						// 指向 header 后面的区域返回
		}

		inline void Free(void* p)
		{
			if (!p) return;
			auto h = (MPObjectHeader*)p - 1;	// 还原出原始指针
			assert(h->versionNumber);			// 理论上讲 free 的时候其版本号不应该是 0. 否则就涉嫌重复 Free
			stacks[h->mpIndex].Push(h);			// 入池
			h->versionNumber = 0;				// 清空版本号
		}

		inline void* Realloc(void *p, size_t newSize, size_t dataLen = std::numeric_limits<size_t>::max())
		{
			auto rtv = Alloc(newSize);
			auto oldSize = (size_t(1) << ((MPObjectHeader*)p - 1)->mpIndex) - sizeof(MPObjectHeader);
			memcpy(rtv, p, std::min(oldSize, dataLen));
			Free(p);
			return rtv;
		}


		/**********************************************************************/
		// 派生自 MPObject 的对象创建
		/**********************************************************************/

		// 该操作将会在头部填充 MPObjectHeaderEx
		template<typename T, typename ...Args>
		std::enable_if_t<std::is_base_of<MPObject, T>::value, T*> Create(Args &&... args)
		{
			// 下列代码 复制自 Alloc 函数小改
			auto siz = sizeof(T) + sizeof(MPObjectHeaderEx);
			auto idx = Calc2n(siz);
			if (siz > (size_t(1) << idx)) siz = size_t(1) << ++idx;
			if (idx >= stacksCount) return nullptr;

			void* rtv;
			if (!stacks[idx].TryPop(rtv)) rtv = malloc(siz);

			auto p = (MPObjectHeaderEx*)rtv;
			p->versionNumber = (++versionNumber) | ((uint64_t)idx << 56);
			p->mempool = this;
			p->refCount = 1;
			p->typeId = TypeId<T>::value;
			p->tsFlags = 0;
			return new (p + 1) T(std::forward<Args>(args)...);
		}

		template<typename ...TS>
		void Release(TS &...ps)
		{
			std::initializer_list<int>{ (ReleaseCore(ps), 0)... };
		}

	protected:
		void ReleaseCore(MPObject* p)
		{
			if (p) p->Release();
		}
		template<typename T>
		void ReleaseCore(MPtr<T>& p)
		{
			if (p) p->Release();
		}

		friend MPObject;
		// 对应上面的 Create
		inline void FreeMPObject(void* p)
		{
			if (!p) return;

			auto h = (MPObjectHeaderEx*)p - 1;	// 还原出原始指针
			assert(h->versionNumber);			// 理论上讲 free 的时候其版本号不应该是 0. 否则就涉嫌重复 Free
			stacks[h->mpIndex].Push(h);			// 入池
			h->versionNumber = 0;				// 清空版本号
		}


		/**********************************************************************/
		// 并非派生自 MPObject 的对象创建
		/**********************************************************************/


		template<typename T>
		std::enable_if_t<!std::is_base_of<MPObject, T>::value> ReleaseCore(T* p)
		{
			if (!p) return;
			p->T::~T();
			Free(p);
		}
	public:

		// 该操作将会在头部填充 MPObjectHeader
		template<typename T, typename ...Args>
		std::enable_if_t<!std::is_base_of<MPObject, T>::value, T*> Create(Args &&... args)
		{
			return new (Alloc(sizeof(T))) T(std::forward<Args>(args)...);
		}

		// 一些方便使用的重载

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


		/**********************************************************************/
		// 创建函数注册相关( 理论上讲可以静态化. 以后再说 )
		/**********************************************************************/

		// 潜规则, 声明一个数组, 存 typeId 到创建函数的映射
		std::array<void*(*)(MemPool*), 1 << sizeof(uint16_t) * 8>* typeId2CreateFuncs = nullptr;

		// 以 当前 typeId 为下标, 内容存 父类 typeId, 以此判断继承关系. 
		std::array<uint16_t, 1 << sizeof(uint16_t) * 8>* typeId2ParentTypeIds = nullptr;

	protected:

		template<size_t Index, typename Tuple>
		void RegisterTypeIdsCore(Tuple const* const& t)
		{
			typedef std::tuple_element_t<Index, Tuple> T;
			(*typeId2CreateFuncs)[TypeId<T>::value] = [](MemPool* mp) { return (void*)mp->Create<T>(*(MemPool*)nullptr); };	// T 需要提供 MemPool& 构造函数以便于接下来反序列化填充
		}
		template<size_t... Indexs, typename Tuple>
		void RegisterTypeIdsCore(std::index_sequence<Indexs...>, Tuple const* const& t)
		{
			std::initializer_list<int>{ (RegisterTypeIdsCore<Indexs, Tuple>(t), 0)... };
		}


		template<size_t... Indexs, typename Tuple, typename CT>
		void FillParentTypeId(std::index_sequence<Indexs...> const& idxs, Tuple const* const& t, CT* ct)
		{
			uint16_t parentTypeId = 0;
			std::initializer_list<int>{ ((
				std::is_base_of<std::tuple_element_t<Indexs, Tuple>, CT>::value && !std::is_same<std::tuple_element_t<Indexs, Tuple>, CT>::value
				? (parentTypeId = TypeId<std::tuple_element_t<Indexs, Tuple>>::value) : 0
				), 0)... };
			(*typeId2ParentTypeIds)[TypeId<CT>::value] = parentTypeId;
		}
		template<size_t... Indexs, typename Tuple>
		void FillParentTypeId(std::index_sequence<Indexs...> const& idxs, Tuple const* const& t)
		{
			std::initializer_list<int>{ (FillParentTypeId(idxs, t, (std::tuple_element_t<Indexs, Tuple>*)nullptr), 0)... };
		}

	public:
		// 向 mempool 注册 typeId 到类型创建函数的映射 以及父子关系查询表
		template<typename ...Types>
		void RegisterTypes()
		{
			if (!typeId2CreateFuncs)
			{
				typeId2CreateFuncs = new std::remove_pointer_t<decltype(typeId2CreateFuncs)>();
			}
			if (!typeId2ParentTypeIds)
			{
				typeId2ParentTypeIds = new std::remove_pointer_t<decltype(typeId2ParentTypeIds)>();
			}
			typeId2CreateFuncs->fill(0);
			typeId2ParentTypeIds->fill(0);

			RegisterTypeIdsCore(std::index_sequence_for<MPObject, Types...>(), (std::tuple<MPObject, Types...>*)nullptr);
			FillParentTypeId(std::index_sequence_for<MPObject, Types...>(), (std::tuple<MPObject, Types...>*)nullptr);
		}

		// 根据 type id 来创建类型实例( 这要求目标类型带有默认构造函数 )
		void* CreatePtrByTypeId(uint16_t typeId)
		{
			return (*typeId2CreateFuncs)[typeId] ? (*typeId2CreateFuncs)[typeId](this) : nullptr;
		}

		// 根据 typeid 判断父子关系
		bool IsBaseOf(uint16_t baseTypeId, uint16_t typeId)
		{
			for (; typeId != baseTypeId; typeId = (*typeId2ParentTypeIds)[typeId])
			{
				if (!typeId) return false;
			}
			return true;
		}

		// 根据 类型 判断父子关系( 须在同一 mempool 注册 )
		template<typename BT, typename T>
		bool IsBaseOf()
		{
			return IsBaseOf(TypeId<BT>::value, TypeId<T>::value);
		}

		// 判断 目标类型 是否从基类指针派生( 某基类指针是否能转换成目标类型 ) 以取代 dynamic_cast
		template<typename T>
		bool IsInheritOf(MPObject* base)
		{
			return IsBaseOf(base->typeId(), TypeId<T>::value);
		}
	};


	/**********************************************************************/
	// 这个函数要用到 MemPool 的功能故实现写在这里
	/**********************************************************************/
	inline void MPObject::Release()
	{
		assert(versionNumber());
		if (refCount() == 0 || --refCount()) return;
		this->~MPObject();
		// 因为 Free 接受的是仅含有 MPObjectHeader 偏移的 offset 故调整
		mempool().FreeMPObject(this);
	}
}
