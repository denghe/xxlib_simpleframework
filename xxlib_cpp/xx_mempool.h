#pragma once
#include "xx_mpobject.h"
#include "xx_ptr.h"
#include <array>
#include <string.h>	// for linux memcpy

namespace xx
{
	// todo: 用宏的方式 开/关 基于 interlocked_compare_exchange 改链表的内存 请求/释放 操作, 以提供部分情况下所需线程安全的特性
	// todo2: 提供 1 producer 1 consumer 安全的队列 / 无序bag, 理论上讲似乎和上面这种内存搞法差不多

	/*
	// 示例:
	template<> struct TypeId<T> { static const uint16_t value = 1; };
	template<> struct TypeId<T> { static const uint16_t value = 2; };
	template<> struct TypeId<T> { static const uint16_t value = 3; };
	...
	注意生成器默认规则为 String 和 BBuffer 占掉 typeid 的 1, 2, 而 0 留给 mpobject

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
#ifndef NDEBUG
			uint32_t count = 0;
#endif

			inline bool TryPop(void*& output)
			{
				if (!header) return false;
				output = header;
				header = *(void**)((char*)header + sizeof(MemHeader_VersionNumber));
#ifndef NDEBUG
				--count;
#endif
				return true;
			}

			inline void Push(void* const& v)
			{
				*(void**)((char*)v + sizeof(MemHeader_VersionNumber)) = header;
				header = v;
#ifndef NDEBUG
				++count;
#endif
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
			h->versionNumber = ++versionNumber;
			h->ptrStackIndex() = (uint8_t)idx;									// 将数组下标附在最高位上, Free 要用
			return h + 1;														// 指向 header 后面的区域返回
		}

		inline void Free(void* p)
		{
			if (!p) return;
			auto h = (MemHeader_VersionNumber*)p - 1;							// 指到内存头
			assert(h->versionNumber && h->ptrStackIndex() < ptrstacks.size());	// 理论上讲 free 的时候其版本号不应该是 0. 否则就涉嫌重复 Free
			ptrstacks[h->ptrStackIndex()].Push(h);								// 入池
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
			assert(h->versionNumber && h->ptrStackIndex() < ptrstacks.size());
			auto oldSize = (size_t(1) << h->ptrStackIndex()) - sizeof(MemHeader_VersionNumber);
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


		// 下面这组设计中为 package debug 状态 释放所设计, 可临时关闭 assert, 
		// 以便于 package 类包的释放特性: 不会在释放时创建新对象, 可能递归 / 互引用但引用计数为 1
#ifndef NDEBUG
		bool enableRefCountAssert = true;
#endif
		inline void EnableRefCountAssert()
		{
#ifndef NDEBUG
			enableRefCountAssert = true;
#endif
		}
		inline void DisableRefCountAssert()
		{
#ifndef NDEBUG
			enableRefCountAssert = false;
#endif
		}


		// 释放由 Create 创建的类
		inline void Release(MPObject* p) noexcept
		{
			if (!p || p->refCount() > 0x7FFFFFFF) return;						// 如果空指针 或是用 Dock 包裹则不执行 Release 操作
			assert(p->versionNumber());											// 版本号不应该是 0,
#ifndef NDEBUG
			if (enableRefCountAssert)
			{
				assert(p->refCount());											// 引用计数不该是 0. 否则就是 Release 次数过多
			}
#endif
			if (--p->refCount()) return;
			auto stackIdx = p->memHeader().ptrStackIndex();						// 提前清空版本号以提供析构过程中针对当前对象的 Ensure() 返回空
			p->memHeader().versionNumber = 0;
			p->~MPObject();
			ptrstacks[stackIdx].Push((MemHeader_MPObject*)p - 1);				// 入池
		}


		// 释放由 Create 创建的类之 safe 版( 会清 0 )
		template<typename T>
		inline void SafeRelease(T* &p) noexcept
		{
			Release(p);
			p = nullptr;
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
		Ptr<T> CreatePtr(Args &&... args)
		{
			return Create<T>(std::forward<Args>(args)...);
		}

		// 适合构造函数中创建成员

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

		template<typename T, typename ...Args>
		bool CreateTo(Ptr<T>& outPtr, Args &&... args)
		{
			outPtr = Create<T>(std::forward<Args>(args)...);
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
			return IsBaseOf(TypeId<BT>::value, typeId);
		}

		// 根据 类型 判断父子关系
		template<typename BT, typename T>
		static bool IsBaseOf()
		{
			return IsBaseOf(TypeId<BT>::value, TypeId<T>::value);
		}

		// 试将指针 p 转成 T* 类型. 取代 dynamic_cast
		template<typename T>
		static T* TryCast(MPObject* p)
		{
			return IsBaseOf(TypeId<T>::value, p->typeId()) ? (T*)p : nullptr;
		}


		// 取代 std::cout, 用起来方便一些. 实现在 xx_string.h
		template<typename ...Args>
		void Cout(Args const& ... args);
	};


	/***********************************************************************************/
	// 一些函数要用到 MemPool 的功能故实现写在这里
	/***********************************************************************************/

	inline void MPObject::Release() noexcept
	{
		mempool().Release(this);
	}

	template<typename T>
	template<typename ...Args>
	Ptr<T>::Ptr(MemPool& mp, Args &&... args)
	{
		if (!mp.CreateTo(pointer, std::forward<Args>(args)...)) throw;
	}

	template<typename T>
	template<typename ...Args>
	Ptr<T>& Ptr<T>::Create(MemPool& mp, Args &&... args)
	{
		if (!pointer)
		{
			mp.CreateTo(pointer, std::forward<Args>(args)...);
		}
		return *this;
	}
}

#include "xx_string.h"
#include "xx_bbuffer.h"




/*

// todo: 对于线程安全的改造, 只是这样还不得行. 还要令 versionNumber 变为 atomic 类型( mempool 中的 与 内存头 中的 )
// 这样的话就不方便用一个变量来运行时切换, 似乎只有用宏来切换

// 线程安全的单链表
struct SafePtrStack
{
	std::atomic<void*> header = nullptr;

	inline bool TryPop(void*& output)
	{
		if (!header) return false;
		for (;;)
		{
			output = header;
			if (header.compare_exchange_weak(output, *(void**)((char*)output + sizeof(MemHeader_VersionNumber)))) break;
		}
		return true;
	}

	inline void Push(void* const& v)
	{
		auto& next = *(void**)((char*)v + sizeof(MemHeader_VersionNumber));
		for (;;)
		{
			next = header;
			if (header.compare_exchange_weak(next, v)) break;
		}
	}
};

*/
