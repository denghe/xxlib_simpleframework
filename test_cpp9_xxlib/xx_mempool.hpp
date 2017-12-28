#pragma once
namespace xx
{

	inline MemPool::MemPool() noexcept
	{
		headers.fill(nullptr);
	}

	inline MemPool::~MemPool() noexcept
	{
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

	inline void* MemPool::Alloc(size_t siz) noexcept
	{
		// 根据长度(预留 MemHeader)拿到链表下标
		assert(siz);
		siz += sizeof(MemHeader);
		auto idx = Calc2n(siz);
		if (siz > (size_t(1) << idx)) siz = size_t(1) << ++idx;

		// 试从链表拿指针. 拿不到就分配.
		auto p = headers[idx];
		if (p) headers[idx] = *(void**)p;
		else p = malloc(siz);
		if (!p) return nullptr;

		// 填充版本号以及链表下标
		auto h = (MemHeader*)p;
		h->data = 0;
		h->versionNumber = ++versionNumber;
		h->mpIndex = (uint8_t)idx;
		return h + 1;
	}

	inline void MemPool::Free(void* p) noexcept
	{
		if (!p) return;
		auto h = (MemHeader*)p - 1;
		// 基于x64地址不会用到最高位的判断原则来检查重复释放
		assert(h->mpIndex > 0 && h->mpIndex < headers.size());
		auto idx = h->mpIndex;
		if constexpr(sizeof(void*) < 8) h->data = 0;
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

	template<typename T>
	T* MemPool::Alloc() noexcept
	{
		static_assert(std::is_pod_v<T>);
		return (T*)Alloc(sizeof(T));
	}

	inline void MemPool::SafeFree(void*& p) noexcept
	{
		Free(p);
		p = nullptr;
	}

	template<typename T, typename ...Args>
	T* MemPool::Create(Args &&... args)
	{
		auto p = (void**)Alloc(sizeof(T));
		((MemHeader*)p - 1)->typeId = TypeId_v<T>;
		try
		{
			if constexpr(std::is_base_of_v<Object, T>)
			{
				return new (p) T(this, std::forward<Args>(args)...);
			}
			else
			{
				return new (p) T(std::forward<Args>(args)...);
			}
		}
		catch (...)
		{
			Free(p);
			throw - 1;
		}
	}

	template<typename T, typename ...Args>
	Ptr<T> MemPool::CreatePtr(Args &&... args)
	{
		return Create<T, Args...>(std::forward<Args>(args)...);
	}

	template<typename T, typename ...Args>
	bool MemPool::CreateTo(T*& outPtr, Args &&... args)
	{
		outPtr = Create<T>(std::forward<Args>(args)...);
		return outPtr != nullptr;
	}

	//template<typename T, typename ...Args>
	//bool MemPool::CreateTo(Ref<T>& outPtr, Args &&... args)
	//{
	//	outPtr = CreateRef<T>(std::forward<Args>(args)...);
	//	return outPtr.pointer != nullptr;
	//}

	template<typename T, typename ...Args>
	bool MemPool::CreateTo(Ptr<T>& outPtr, Args &&... args)
	{
		outPtr = Create<T>(std::forward<Args>(args)...);
		return outPtr.pointer != nullptr;
	}


	template<typename T>
	void MemPool::Release(T* p) noexcept
	{
		p->~T();
		Free(p);
	}

	template<typename T>
	void MemPool::SafeRelease(T*& p) noexcept
	{
		Release(p);
		p = nullptr;
	}







	template<typename T, typename PT>
	void MemPool::Register() noexcept
	{
		// 存父 pid
		assert(!pids[TypeId_v<T>]);
		pids[TypeId_v<T>] = TypeId_v<PT>;

		// 在执行构造函数之前拿到指针 塞入 bb. 构造函数执行失败时从 bb 移除
		creators[TypeId_v<T>] = [](MemPool* mp, BBuffer* bb, uint32_t ptrOffset) ->void*
		{
			// 插入字典占位, 分配到实际指针后替换
			auto addResult = bb->idxStore->Add(ptrOffset, std::make_pair(nullptr, TypeId_v<T>));

			// copy from Alloc
			auto siz = sizeof(T) + sizeof(MemHeader);
			auto idx = Calc2n(siz);
			if (siz > (size_t(1) << idx)) siz = size_t(1) << ++idx;

			auto p = headers[idx];
			if (p) headers[idx] = *(void**)p;
			else p = malloc(siz);
			if (!p) return nullptr;

			auto h = (MemHeader*)p;
			h->data = 0;
			h->versionNumber = ++versionNumber;
			h->mpIndex = (uint8_t)idx;
			h->typeId = TypeId_v<T>;

			auto t = (T*)(p + 1);
			bb->idxStore->ValueAt(addResult.index).first = t;	// 将字典中的 value 替换成真实指针
			try
			{
				return new (t) T(mp, bb);
			}
			catch (...)
			{
				bb->idxStore->RemoveAt(addResult.index);		// 从字典移除

				// copy from Free
				if constexpr(sizeof(void*) < 8) h->data = 0;
				*(void**)h = mp->headers[idx];
				mp->headers[idx] = h;
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







	Object::Object(MemPool* mempool) noexcept
		: mempool(mempool)
	{}
	Object::~Object() noexcept {}

	inline MemHeader& Object::memHeader() noexcept { return *((MemHeader*)this - 1); }
	inline MemHeader& Object::memHeader() const noexcept { return *((MemHeader*)this - 1); }





	template<typename T>
	Ptr<T>::Ptr() noexcept
		: pointer(nullptr)
	{}

	template<typename T>
	Ptr<T>::Ptr(T* const& pointer) noexcept
		: pointer(pointer)
	{}

	template<typename T>
	template<typename O>
	Ptr<T>::Ptr(Ptr<O>&& o) noexcept
		: pointer(o.pointer)
	{
		static_assert(std::is_base_of_v<T, O>);
		o.pointer = nullptr;
	}

	template<typename T>
	Ptr<T>::Ptr(Ptr<T>&& o) noexcept
		: pointer(o.pointer)
	{
		o.pointer = nullptr;
	}


	template<typename T>
	Ptr<T>::~Ptr()
	{
		if (pointer)
		{
			pointer->mempool->Release(pointer);
			pointer = nullptr;
		}
	}

	template<typename T>
	template<typename O>
	Ptr<T>& Ptr<T>::operator=(Ptr<O>&& o) noexcept
	{
		static_assert(std::is_base_of_v<T, O>);
		if (pointer)
		{
			pointer->mempool->Release(pointer);
		}
		pointer = o.pointer;
		o.pointer = nullptr;
		return *this;
	}


	template<typename T>
	Ptr<T>& Ptr<T>::operator=(T* const& o) noexcept
	{
		if (pointer)
		{
			pointer->mempool->Release(pointer);
		}
		pointer = o;
		return *this;
	}

	template<typename T>
	T const* Ptr<T>::operator->() const noexcept { return pointer; }

	template<typename T>
	T* & Ptr<T>::operator->() noexcept { return pointer; }

	template<typename T>
	T& Ptr<T>::operator*() noexcept { return *pointer; }

	template<typename T>
	T const& Ptr<T>::operator*() const noexcept { return *pointer; }

	template<typename T>
	Ptr<T>::operator bool() const noexcept { return pointer != nullptr; }

}
