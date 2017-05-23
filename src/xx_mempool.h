#pragma once
#include "xx_mpobject.h"
#include <array>

namespace xx
{

	/*
	// 示例:
	template<> struct TypeId<T> { static const uint16_t value = 2; };
	template<> struct TypeId<T> { static const uint16_t value = 3; };
	template<> struct TypeId<T> { static const uint16_t value = 4; };
	...

	MemPool::Register< T, PT >();
	MemPool::Register< T, PT >();
	MemPool::Register< T, PT >();
	...

	MemPool mp;
	auto o = mp.Create<T>(.....);

	*/


	struct BBuffer;

	// 整套库的核心内存分配组件. 按 2^N 尺寸划分内存分配行为, 将 free 的指针放入 stack 缓存复用
	// 对于分配出来的内存, 自增 版本号 将填充在 指针 -8 区( Alloc ). 用于判断指针是否已失效
	// MPObject 对象使用 Create / Release 来创建和析构
	struct MemPool
	{
		// 单链表. 指向下一指针的内存区域避开版本号区域不写
		struct PtrStack
		{
			void* header = nullptr;

			inline bool TryPop(void*& output)
			{
				if (!header) return false;
				output = header;
				header = *(void**)((char*)header + sizeof(MemHeader_VersionNumber));
				return true;
			}

			inline void Push(void* const& v)
			{
				*(void**)((char*)v + sizeof(MemHeader_VersionNumber)) = header;
				header = v;
			}
		};

		// 数组长度涵盖所有 2^n 对齐分配长度规则
		std::array<PtrStack, sizeof(size_t) * 8> ptrstacks;

		// 自增版本号( 每次创建时先 ++ 再填充 )
		uint64_t versionNumber = 0;


		MemPool(MemPool const&) = delete;
		MemPool& operator=(MemPool const &) = delete;
		MemPool() {}
		~MemPool()
		{
			// 池内存回收
			void* p;
			for (auto& stack : ptrstacks)
			{
				while (stack.TryPop(p)) std::free(p);
			}
		}

		/***********************************************************************************/
		// 内存分配( malloc / free 系列. 主要供一些容器类的代码使用 )
		/***********************************************************************************/


		// 该操作将会在头部区域填充 MemHeader_VersionNumber 并跳过, 返回偏移后的指针
		// 最大化内存利用率的 size 计算: Round2n(capacity * sizeof(T) + sizeof(MemHeader_VersionNumber)) - sizeof(MemHeader_VersionNumber)
		inline void* Alloc(size_t siz)
		{
			assert(siz);
			siz += sizeof(MemHeader_VersionNumber);								// 空出放置 MemHeader_VersionNumber 的地儿
			auto idx = Calc2n(siz);
			if (siz > (size_t(1) << idx)) siz = size_t(1) << ++idx;

			void* p;
			if (!ptrstacks[idx].TryPop(p)) p = std::malloc(siz);

			auto h = (MemHeader_VersionNumber*)p;								// 指到内存头
			h->versionNumber = (++versionNumber) | ((uint64_t)idx << 56);		// 将数组下标附在最高位上, Free 要用
			return h + 1;														// 指向 header 后面的区域返回
		}

		inline void Free(void* p)
		{
			if (!p) return;
			auto h = (MemHeader_VersionNumber*)p - 1;							// 指到内存头
			assert(h->versionNumber && h->mpIndex < ptrstacks.size());				// 理论上讲 free 的时候其版本号不应该是 0. 否则就涉嫌重复 Free
			ptrstacks[h->mpIndex].Push(h);											// 入池
			h->versionNumber = 0;												// 清空版本号
		}

		// dataLen 表示要复制多少字节数到新的内存. 并不代表 p 的原始长度
		inline void* Realloc(void *p, size_t newSize, size_t dataLen = -1)
		{
			if (!newSize)
			{
				Free(p);
				return nullptr;
			}
			if (!p) return Alloc(newSize);

			auto h = (MemHeader_VersionNumber*)p - 1;
			assert(h->versionNumber && h->mpIndex < ptrstacks.size());
			auto oldSize = (size_t(1) << h->mpIndex) - sizeof(MemHeader_VersionNumber);
			if (oldSize >= newSize) return p;

			auto np = Alloc(newSize);
			memcpy(np, p, MIN(oldSize, dataLen));
			Free(p);
			return np;
		}

		/***********************************************************************************/
		// 内存分配( Create / Release 系列 ). 仅针对派生自 MPObject 的对象
		/***********************************************************************************/

		// 该操作将会在头部填充 MemHeader_MPObject
		template<typename T, typename ...Args>
		T* Create(Args &&... args) noexcept
		{
			static_assert(std::is_base_of<MPObject, T>::value, "the T must be inerit of MPObject.");

			// 下列代码 复制自 Alloc 函数小改
			auto siz = sizeof(T) + sizeof(MemHeader_MPObject);
			auto idx = Calc2n(siz);
			if (siz > (size_t(1) << idx)) siz = size_t(1) << ++idx;

			void* rtv;
			if (!ptrstacks[idx].TryPop(rtv)) rtv = malloc(siz);
			if (!rtv) return nullptr;

			auto p = (MemHeader_MPObject*)rtv;
			p->versionNumber = (++versionNumber) | ((uint64_t)idx << 56);
			p->mempool = this;
			p->refCount = 1;
			p->typeId = TypeId<T>::value;
			p->tsFlags = 0;

			auto t = (T*)(p + 1);
			try
			{
				new (t) T(std::forward<Args>(args)...);
			}
			catch (...)
			{
				ptrstacks[idx].Push(p);											// 入池
				p->versionNumber = 0;											// 清空版本号
				return nullptr;
			}
			return t;
		}

		// 释放由 Create 创建的类
		inline void Release(MPObject* p) noexcept
		{
			if (!p || p->refCount() > 0x7FFFFFFF) return;						// 如果空指针 或是用 MemHeaderBox 包裹则不执行 Release 操作
			assert(p->versionNumber() && p->refCount());						// 理论上讲 free 的时候其版本号不应该是 0, 引用计数不该是 0. 否则就是 Release 次数过多
			if (--p->refCount()) return;
			p->~MPObject();

			auto h = (MemHeader_MPObject*)p - 1;								// 指到内存头
			ptrstacks[h->mpIndex].Push(h);										// 入池
			h->versionNumber = 0;												// 清空版本号
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
		bool CreateTo(T*& outPtr, Args &&... args)
		{
			outPtr = Create<T>(std::forward<Args>(args)...);
			return outPtr != nullptr;
		}
		template<typename T, typename ...Args>
		bool CreateTo(MPtr<T>& outPtr, Args &&... args)
		{
			outPtr = CreateMPtr<T>(std::forward<Args>(args)...);
			return outPtr.pointer != nullptr;
		}









		/***********************************************************************************/
		// 类型处理相关
		/***********************************************************************************/

		// 放置 type 对应的 parent 的 type id. 1 : MPObject
		inline static std::array<uint16_t, std::numeric_limits<uint16_t>::max()>& pids()
		{
			static std::array<uint16_t, std::numeric_limits<uint16_t>::max()> _pids;
			return _pids;
		}

		// 存 typeId 到序列化构造函数的映射
		inline static std::array<void*(*)(MemPool*, BBuffer*, uint32_t), 1 << sizeof(uint16_t) * 8>& creators()
		{
			static std::array<void*(*)(MemPool*, BBuffer*, uint32_t), 1 << sizeof(uint16_t) * 8> _creators;
			return _creators;
		}

		// 注册类型的父子关系. 顺便生成创建函数. MPObject 不需要注册. T 需要提供相应构造函数 for 反序列化
		template<typename T, typename PT>
		static void Register();							// 实现在 xx_buffer.h 尾部
		//{
		//	assert(!pids()[TypeId<T>::value]);
		//	pids()[TypeId<T>::value] = TypeId<PT>::value;
		//	creators()[TypeId<T>::value] = [](MemPool* mp, BBuffer* bb) 
		//	{
		//		// todo: 需要在这里单独实现 Create, 以便在执行构造函数之前拿到指针 塞入 bb. 构造函数执行失败时从 bb 移除
		//		return (MPObject*)mp->Create<T>(bb);
		//	};
		//}

		// 根据 typeid 判断父子关系
		inline static bool IsBaseOf(uint32_t baseTypeId, uint32_t typeId)
		{
			for (; typeId != baseTypeId; typeId = pids()[typeId])
			{
				if (!typeId || typeId == pids()[typeId]) return false;
			}
			return true;
		}

		// 根据 类型 判断父子关系
		template<typename BT>
		static bool IsBaseOf(uint32_t typeId)
		{
			return IsBaseOf(TupleIndexOf<BT, Tuple>::value, typeId);
		}

		// 根据 类型 判断父子关系
		template<typename BT, typename T>
		static bool IsBaseOf()
		{
			return IsBaseOf(TupleIndexOf<BT, Tuple>::value, TupleIndexOf<T, Tuple>::value);
		}

		// 试将指针 p 转成 T* 类型. 取代 dynamic_cast
		template<typename T>
		static T* TryCast(MPObject* p)
		{
			return IsBaseOf(TupleIndexOf<T, Tuple>::value, p->typeId()) ? (T*)p : nullptr;
		}

	};


	/***********************************************************************************/
	// 一些函数要用到 MemPool 的功能故实现写在这里
	/***********************************************************************************/

	inline void MPObject::Release() noexcept
	{
		mempool().Release(this);
	}
}
