#include "xx_mempool.h"
#pragma once
namespace xx
{
	template<>
	struct TypeId<String>
	{
		static const uint16_t value = 1;
	};

	template<>
	struct TypeId<BBuffer>
	{
		static const uint16_t value = 2;
	};


	inline MemPool::MemPool() noexcept
	{
		headers.fill(nullptr);
		MPCreateTo(ptrStore);
		MPCreateTo(idxStore);
	}

	inline MemPool::~MemPool() noexcept
	{
		Release(idxStore);
		Release(ptrStore);
		for (auto header : headers)
		{
			while (header)
			{
				auto next = *(void**)header;
				free(header);
				header = next;
			}
		}
	}

	template<typename MHType>
	void* MemPool::Alloc(size_t siz) noexcept
	{
		// 根据长度(预留 MHType 的空间)拿到链表下标
		assert(siz);
		siz += sizeof(MHType);
		auto idx = Calc2n(siz);
		if (siz > (size_t(1) << idx)) siz = size_t(1) << ++idx;

		// 试从链表拿指针. 拿不到就分配.
		auto p = headers[idx];
		if (p) headers[idx] = *(void**)p;
		else p = malloc(siz);
		if (!p) return nullptr;

		// 填充版本号以及链表下标
		auto h = (MHType*)p;
		h->versionNumber = ++versionNumber;
		h->mpIndex() = (uint8_t)idx;
		return h + 1;
	}

	template<typename MHType>
	void MemPool::Free(void* p) noexcept
	{
		if (!p) return;
		auto h = (MHType*)p - 1;
		assert(h->mpIndex() > 0 && h->mpIndex() < headers.size());
		auto idx = h->mpIndex();
		if constexpr(sizeof(void*) < 8) h->versionNumber = 0;
		*(void**)h = headers[idx];
		headers[idx] = h;
	}

	inline void* MemPool::Realloc(void *p, size_t newSize, size_t dataLen) noexcept
	{
		if (!newSize)
		{
			Free(p);
			return nullptr;
		}
		if (!p) return Alloc(newSize);

		auto originalSize = (size_t(1) << *(size_t*)((void**)p - 1)) - sizeof(void*);
		if (originalSize >= newSize) return p;

		auto np = Alloc(newSize);
		memcpy(np, p, std::min(originalSize, dataLen));
		Free(p);
		return np;
	}




	template<typename T, typename ...Args>
	T* MemPool::Create(Args &&... args)
	{
		auto p = Alloc<MemHeader_Object>(sizeof(T));
		if (!p) return nullptr;

		// 继续填充 header
		auto h = (MemHeader_Object*)p - 1;
		h->objectData = 0;
		h->typeId = TypeId_v<T>;

		try
		{
			return new (p) T(std::forward<Args>(args)...);
		}
		catch (...)
		{
			Free<MemHeader_Object>(p);
			throw - 1;
		}
	}

	template<typename T, typename ...Args>
	Ptr<T> MemPool::CreatePtr(Args &&... args)
	{
		return Ptr<T>(Create<T>(std::forward<Args>(args)...));
	}


	template<typename T, typename ...Args>
	bool MemPool::CreateTo(T*& outPtr, Args &&... args)
	{
		outPtr = Create<T>(std::forward<Args>(args)...);
		return outPtr;
	}

	template<typename T, typename ...Args>
	bool MemPool::CreateTo(Ptr<T>& outPtr, Args &&... args)
	{
		outPtr = CreatePtr<T>(std::forward<Args>(args)...);
		return outPtr.pointer;
	}







	template<typename T, typename ...Args>
	T* MemPool::MPCreate(Args &&... args)
	{
		return Create<T>(this, std::forward<Args>(args)...);
	}

	template<typename T, typename ...Args>
	Ptr<T> MemPool::MPCreatePtr(Args &&... args)
	{
		return CreatePtr<T>(this, std::forward<Args>(args)...);
	}


	template<typename T, typename ...Args>
	bool MemPool::MPCreateTo(T*& outPtr, Args &&... args)
	{
		return CreateTo(outPtr, this, std::forward<Args>(args)...);
	}

	template<typename T, typename ...Args>
	bool MemPool::MPCreateTo(Ptr<T>& outPtr, Args &&... args)
	{
		return CreateTo(outPtr, this, std::forward<Args>(args)...);
	}






	inline void MemPool::Release(Object* o)
	{
		if (!o) return;
		o->~Object();
		Free<MemHeader_Object>(o);
	}

	template<typename T, typename U>
	inline void MemPool::SafeRelease(T*& o)
	{
		if (!o) return;
		o->~T();
		Free<MemHeader_Object>(o);
		o = nullptr;
	}



	inline size_t MemPool::Calc2n(size_t n) noexcept
	{
		assert(n);
#ifdef _MSC_VER
		unsigned long r = 0;
#if defined(_WIN64) || defined(_M_X64)
		_BitScanReverse64(&r, n);
# else
		_BitScanReverse(&r, n);
# endif
		return (size_t)r;
#else
#if defined(__LP64__) || __WORDSIZE == 64
		return int(63 - __builtin_clzl(n));
# else
		return int(31 - __builtin_clz(n));
# endif
#endif
	}

	inline size_t MemPool::Round2n(size_t n) noexcept
	{
		auto rtv = size_t(1) << Calc2n(n);
		if (rtv == n) return n;
		else return rtv << 1;
	}






	inline void MemPool::RegisterInternal() noexcept
	{
		Register<String, Object>();
		Register<BBuffer, Object>();
	}


	template<typename T, typename PT>
	void MemPool::Register() noexcept
	{
		// 存父 pid
		assert(!pids[TypeId_v<T>]);
		pids[TypeId_v<T>] = TypeId_v<PT>;

		// 在执行构造函数之前拿到指针 塞入 bb. 构造函数执行失败时从 bb 移除
		creators[TypeId_v<T>] = [](MemPool* mp, BBuffer* bb, size_t ptrOffset) ->void*
		{
			// 插入字典占位, 分配到实际指针后替换
			auto addResult = mp->idxStore->Add(ptrOffset, std::make_pair(nullptr, TypeId_v<T>));

			// 拿内存
			auto p = mp->Alloc<MemHeader_Object>(sizeof(T));
			if (!p) return nullptr;

			// 继续填充 header
			auto h = (MemHeader_Object*)p - 1;
			h->objectData = 0;
			h->typeId = TypeId_v<T>;

			// 将字典中的 value 替换成真实指针
			mp->idxStore->ValueAt(addResult.index).first = p;
			try
			{
				// 调构造函数
				return new (p) T(bb);
			}
			catch (...)
			{
				// 从字典移除
				mp->idxStore->RemoveAt(addResult.index);
				mp->Free<MemHeader_Object>(p);
				return nullptr;
			}
		};
	}

	inline bool MemPool::IsBaseOf(uint32_t baseTypeId, uint32_t typeId) noexcept
	{
		for (; typeId != baseTypeId; typeId = pids[typeId])
		{
			if (!typeId || typeId == pids[typeId]) return false;
		}
		return true;
	}

	template<typename BT>
	bool MemPool::IsBaseOf(uint32_t typeId) noexcept
	{
		return IsBaseOf(TypeId<BT>::value, typeId);
	}

	template<typename BT, typename T>
	bool MemPool::IsBaseOf() noexcept
	{
		return IsBaseOf(TypeId<BT>::value, TypeId<T>::value);
	}

	template<typename T>
	T* MemPool::TryCast(Object* p) noexcept
	{
		return IsBaseOf(TypeId<T>::value, p->typeId()) ? (T*)p : nullptr;
	}







	inline Object::Object(MemPool* mempool) noexcept
		: mempool(mempool)
	{}

	inline Object::Object(BBuffer* bb) noexcept
		: mempool(bb->mempool)
	{
	}

	inline Object::~Object() noexcept {}

	inline void Object::Release()
	{
		mempool->Release(this);
	}

	inline MemHeader_Object& Object::memHeader() noexcept { return *((MemHeader_Object*)this - 1); }
	inline MemHeader_Object& Object::memHeader() const noexcept { return *((MemHeader_Object*)this - 1); }

	inline void Object::ToString(String &s) const {}
	inline void Object::ToStringCore(String &s) const {}
	inline void Object::ToBBuffer(BBuffer &bb) const {}
	inline int Object::FromBBuffer(BBuffer &bb) { return 0; }











	template<typename T>
	Ptr<T>::Ptr() noexcept
		: pointer(nullptr)
	{}


	template<typename T>
	template<typename O>
	Ptr<T>::Ptr(O* const& pointer) noexcept
		: pointer(pointer)
	{
		static_assert(std::is_base_of_v<T, O>);
		if (pointer)
		{
			++pointer->memHeader().refs;
		}
	}
	template<typename T>
	template<typename O>
	Ptr<T>& Ptr<T>::operator=(O* const& o) noexcept
	{
		static_assert(std::is_base_of_v<T, O>);
		Clear();
		pointer = o;
		if (pointer)
		{
			++pointer->memHeader().refs;
		}
		return *this;
	}


	template<typename T>
	Ptr<T>::Ptr(Ptr const& o) noexcept
		: Ptr<T>(o.pointer)
	{
		if (pointer)
		{
			++pointer->memHeader().refs;
		}
	}
	template<typename T>
	template<typename O>
	Ptr<T>::Ptr(Ptr<O> const& o) noexcept
		: Ptr<T>(o.pointer)
	{
		if (pointer)
		{
			++pointer->memHeader().refs;
		}
	}

	template<typename T>
	Ptr<T>& Ptr<T>::operator=(Ptr const& o) noexcept
	{
		return operator=(o.pointer);
	}
	template<typename T>
	Ptr<T>& Ptr<T>::operator=(Ptr&& o) noexcept
	{
		std::swap(this->pointer, o.pointer);
		return *this;
	}


	template<typename T>
	template<typename O>
	Ptr<T>& Ptr<T>::operator=(Ptr<O> const& o) noexcept
	{
		return operator=(o.pointer);
	}

	template<typename T>
	Ptr<T>::Ptr(Ptr&& o) noexcept
		: pointer(o.pointer)
	{
		o.pointer = nullptr;
	}
	template<typename T>
	template<typename O>
	Ptr<T>::Ptr(Ptr<O>&& o) noexcept
		: pointer(o.pointer)
	{
		static_assert(std::is_base_of_v<T, O>);
		o.pointer = nullptr;
	}
	template<typename T>
	template<typename O>
	Ptr<T>& Ptr<T>::operator=(Ptr<O>&& o) noexcept
	{
		static_assert(std::is_base_of_v<T, O>);
		std::swap(pointer, o.pointer);
		return *this;
	}
	template<typename T>
	template<typename O>
	void Ptr<T>::Assign(Ptr<O>&& o) noexcept
	{
		static_assert(std::is_base_of_v<T, O>);
		Clear();
		pointer = o.pointer;
		o.pointer = nullptr;
	}


	template<typename T>
	void Ptr<T>::Clear()
	{
		if (pointer)
		{
			if (--pointer->memHeader().refs == 0)
			{
				auto mp = pointer->mempool;
				pointer->~T();
				mp->Free<MemHeader_Object>(pointer);
			}
			pointer = nullptr;
		}
	}

	template<typename T>
	Ptr<T>::~Ptr()
	{
		Clear();
	}


	template<typename T>
	template<typename O>
	bool Ptr<T>::operator==(Ptr<O> const& o) const noexcept
	{
		return pointer == o.pointer;
	}


	template<typename T>
	T const* Ptr<T>::operator->() const noexcept
	{
		return pointer;
	}

	template<typename T>
	T* & Ptr<T>::operator->() noexcept
	{
		return pointer;
	}

	template<typename T>
	T& Ptr<T>::operator*() noexcept
	{
		return *pointer;
	}

	template<typename T>
	T const& Ptr<T>::operator*() const noexcept
	{
		return *pointer;
	}

	template<typename T>
	Ptr<T>::operator bool() const noexcept
	{
		return pointer != nullptr;
	}








	template<typename T>
	Ref<T>::Ref() noexcept
		: pointer(nullptr)
		, versionNumber(0)
	{}

	template<typename T>
	template<typename O>
	Ref<T>::Ref(O* const& o) noexcept
		: pointer(o)
	{
		static_assert(std::is_base_of_v<T, O>);
		versionNumber = o ? o->memHeader().versionNumber : 0;
	}

	template<typename T>
	template<typename O>
	Ref<T>::Ref(Ptr<O> const& o) noexcept
		: pointer(o.pointer)
	{
		static_assert(std::is_base_of_v<T, O>);
		versionNumber = o ? o->memHeader().versionNumber : 0;
	}

	template<typename T>
	Ref<T>::Ref(Ref const& o) noexcept
	{
		operator=(o.Lock());
	}
	template<typename T>
	template<typename O>
	Ref<T>::Ref(Ref<O> const& o) noexcept
	{
		static_assert(std::is_base_of_v<T, O>);
		operator=(o.Lock());
	}

	template<typename T>
	template<typename O>
	Ref<T>& Ref<T>::operator=(Ptr<O> const& o) noexcept
	{
		static_assert(std::is_base_of_v<T, O>);
		pointer = o.pointer;
		versionNumber = o ? o->memHeader().versionNumber : 0;
		return *this;
	}

	template<typename T>
	Ref<T>& Ref<T>::operator=(Ref const& o) noexcept
	{
		return operator=(o.Lock());
	}
	template<typename T>
	template<typename O>
	Ref<T>& Ref<T>::operator=(Ref<O> const& o) noexcept
	{
		return operator=(o.Lock());
	}

	template<typename T>
	template<typename O>
	Ptr<O> Ref<T>::Lock() const noexcept
	{
		static_assert(std::is_base_of_v<O, T>);
		if (pointer && pointer->memHeader().versionNumber == versionNumber)
		{
			return Ptr<O>(pointer);
		}
		return Ptr<O>();
	}

	template<typename T>
	Ref<T>::operator bool() const noexcept
	{
		return pointer && pointer->memHeader().versionNumber == versionNumber;
	}









	/***********************************************************************************/
	// std::cout 扩展
	/***********************************************************************************/

	inline std::ostream& operator<<(std::ostream& os, const Object& o)
	{
		String s(o.mempool);
		o.ToString(s);
		os << s;
		return os;
	}

	template<typename T>
	std::ostream& operator<<(std::ostream& os, Ptr<T> const& o)
	{
		if (!o) return os << "nil";
		return os << *o;
	}

	template<typename T>
	std::ostream& operator<<(std::ostream& os, Ref<T> const& o)
	{
		return os << o.Lock();
	}

}
