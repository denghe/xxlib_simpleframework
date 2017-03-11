#pragma once
#include "xx_mpobject.h"
#include "xx_string.h"
#include <array>
#include <vector>
#include <cstring>

namespace xx
{
	// 内存池基类. 提供类型无关的内存分配 / 回收功能
	struct MemPoolBase
	{
		// 单链表. 指向下一指针的内存区域避开版本号区域不写
		struct PtrStack
		{
			void* header = nullptr;

			bool TryPop(void*& output)
			{
				if (!header) return false;
				output = header;
				header = *(void**)((char*)header + sizeof(MemHeader_VersionNumber));
				return true;
			}

			void Push(void* const& v)
			{
				*(void**)((char*)v + sizeof(MemHeader_VersionNumber)) = header;
				header = v;
			}
		};

		// 数组长度涵盖所有 2^n 对齐分配长度规则
		std::array<PtrStack, sizeof(size_t) * 8> ptrstacks;

		// 自增版本号( 每次创建时先 ++ 再填充 )
		uint64_t versionNumber = 0;

		// 池内存回收
		~MemPoolBase()
		{
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
			memcpy(np, p, std::min(oldSize, dataLen));
			Free(p);
			return np;
		}

		/***********************************************************************************/
		// 内存分配( Create / Release 系列 ). 仅针对派生自 MPObject 的对象
		/***********************************************************************************/

		// 该操作将会在头部填充 MemHeader_MPObject
		template<typename T, typename ...Args>
		T* CreateWithoutTypeId(Args &&... args) noexcept
		{
			static_assert(std::is_base_of<MPObject, T>::value, "the T must be inerit of MPObject.");

			// 下列代码 复制自 Alloc 函数小改
			auto siz = sizeof(T) + sizeof(MemHeader_MPObject);
			auto idx = Calc2n(siz);
			if (siz > (size_t(1) << idx)) siz = size_t(1) << ++idx;

			void* rtv;
			if (!ptrstacks[idx].TryPop(rtv)) rtv = malloc(siz);

			auto p = (MemHeader_MPObject*)rtv;
			p->versionNumber = (++versionNumber) | ((uint64_t)idx << 56);
			p->mempoolbase = this;
			p->refCount = 1;
			p->typeId = -1;// (decltype(p->typeId))TupleIndexOf<T, Tuple>::value;

			auto t = (T*)(p + 1);
			try
			{
				new (t) T(std::forward<Args>(args)...);
			}
			catch (...)
			{
				this->Release(t);
				return nullptr;
			}
			return t;
		}

		// 释放由 Create 创建的类
		inline void Release(MPObject* p) noexcept
		{
			if (!p) return;
			assert(p->versionNumber());
			if (p->refCount() == 0 || --p->refCount()) return;
			p->~MPObject();

			auto h = (MemHeader_MPObject*)p - 1;								// 指到内存头
			assert(h->versionNumber);											// 理论上讲 free 的时候其版本号不应该是 0. 否则就涉嫌重复 Free
			ptrstacks[h->mpIndex].Push(h);											// 入池
			h->versionNumber = 0;												// 清空版本号
		}

		inline void InitMemHeader(MPObject* p) noexcept
		{

		}

		/***********************************************************************************/
		// helpers
		/***********************************************************************************/

		template<typename T, typename ...Args>
		MPtr<T> CreateMPtrWithoutTypeId(Args &&... args) noexcept
		{
			return CreateWithoutTypeId<T>(std::forward<Args>(args)...);
		}

		template<typename T, typename ...Args>
		void CreateToWithoutTypeId(T*& outPtr, Args &&... args) noexcept
		{
			outPtr = CreateWithoutTypeId<T>(std::forward<Args>(args)...);
		}
		template<typename T, typename ...Args>
		void CreateToWithoutTypeId(MPtr<T>& outPtr, Args &&... args) noexcept
		{
			outPtr = CreateMPtrWithoutTypeId<T>(std::forward<Args>(args)...);
		}

	};


	/***********************************************************************************/
	// 一些函数要用到 MemPool 的功能故实现写在这里
	/***********************************************************************************/

	inline void MPObject::Release() noexcept
	{
		mempoolbase().Release(this);
	}

	inline MemHeader_MPObject::MemHeader_MPObject(MemPoolBase& mempoolbase)
		: mempoolbase(&mempoolbase)
	{
		this->versionNumber = 0;
		this->refCount = 0;
		this->typeId = 0;
		this->tsFlags = 0;
	}

}
