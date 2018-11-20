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
		MPCreateTo(strs);
	}

	inline MemPool::~MemPool() noexcept
	{
		Release(strs);
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
	void MemPool::Free(void* const& p) noexcept
	{
		if (!p) return;
		auto h = (MHType*)p - 1;
		assert(h->mpIndex() > 0 && h->mpIndex() < headers.size());
		auto idx = h->mpIndex();
		if constexpr (sizeof(void*) < 8) h->versionNumber = 0;
		*(void**)h = headers[idx];
		headers[idx] = h;
	}

	inline void* MemPool::Realloc(void* const& p, size_t const& newSize, size_t const& dataLen) noexcept
	{
		if (!newSize)
		{
			Free(p);
			return nullptr;
		}
		if (!p) return Alloc(newSize);

		auto h = (MemHeader*)p - 1;
		auto originalSize = (size_t(1) << h->mpIndex()) - sizeof(MemHeader);
		if (originalSize >= newSize) return p;

		auto np = Alloc(newSize);
		memcpy(np, p, std::min(originalSize, dataLen));
		Free(p);
		return np;
	}




	template<typename T, typename...Args>
	T* MemPool::Create(Args&&...args) noexcept
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
			return nullptr;
		}
	}

	template<typename T, typename...Args>
	Ptr<T> MemPool::CreatePtr(Args&&...args) noexcept
	{
		return Ptr<T>(Create<T>(std::forward<Args>(args)...));
	}

	template<typename T, typename...Args>
	Unique<T> MemPool::CreateUnique(Args&&...args) noexcept
	{
		return Unique<T>(Create<T>(std::forward<Args>(args)...));
	}


	template<typename T, typename...Args>
	T* MemPool::CreateTo(T*& outPtr, Args&&...args) noexcept
	{
		outPtr = Create<T>(std::forward<Args>(args)...);
		return outPtr;
	}

	template<typename T, typename...Args>
	T* MemPool::CreateTo(Ptr<T>& outPtr, Args&&...args) noexcept
	{
		outPtr = CreatePtr<T>(std::forward<Args>(args)...);
		return outPtr.pointer;
	}







	template<typename T, typename...Args>
	T* MemPool::MPCreate(Args&&...args) noexcept
	{
		return Create<T>(this, std::forward<Args>(args)...);
	}

	template<typename T, typename...Args>
	Ptr<T> MemPool::MPCreatePtr(Args&&...args) noexcept
	{
		return CreatePtr<T>(this, std::forward<Args>(args)...);
	}

	template<typename T, typename...Args>
	Unique<T> MemPool::MPCreateUnique(Args&&...args) noexcept
	{
		return CreateUnique<T>(this, std::forward<Args>(args)...);
	}


	template<typename T, typename...Args>
	bool MemPool::MPCreateTo(T*& outPtr, Args&&...args) noexcept
	{
		return CreateTo(outPtr, this, std::forward<Args>(args)...);
	}

	template<typename T, typename...Args>
	bool MemPool::MPCreateTo(Ptr<T>& outPtr, Args&&...args) noexcept
	{
		return CreateTo(outPtr, this, std::forward<Args>(args)...);
	}





	template<typename...Args>
	Ptr<String> MemPool::Str(Args&&...args) noexcept
	{
		return CreatePtr<xx::String>(this, std::forward<Args>(args)...);
	}





	// 从 Free 复制小改. 能防递归 Release
	inline void MemPool::Release(Object* const& p) noexcept
	{
		if (!p) return;
		auto h = (MemHeader_Object*)p - 1;
		if (!h->versionNumber) return;								// 防 Release 重入
		assert(!h->refs);											// 防 Ptr<> 方式使用时过早的被 Release
		assert(h->mpIndex() > 0 && h->mpIndex() < headers.size());	// 防长度部分被破坏
		auto idx = h->mpIndex();
		h->versionNumber = 0;
		p->~Object();
		*(void**)h = headers[idx];
		headers[idx] = h;
	}



	inline size_t MemPool::Calc2n(size_t const& n) noexcept
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

	inline size_t MemPool::Round2n(size_t const& n) noexcept
	{
		auto rtv = size_t(1) << Calc2n(n);
		if (rtv == n) return n;
		else return rtv << 1;
	}






	inline void MemPool::RegisterInternals() noexcept
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
		creators[TypeId_v<T>] = [](MemPool* const& mp, BBuffer* const& bb, size_t const& ptrOffset) noexcept ->void*
		{
			// 如果把 TypeId_v<T> 直接放入 std::make_pair, 其值将永远是 0. 故先取出来.
			auto typeId = TypeId_v<T>;

			// 插入字典占位, 分配到实际指针后替换
			auto addResult = mp->idxStore->Add(ptrOffset, std::make_pair(nullptr, typeId));

			// 拿内存
			auto p = mp->Alloc<MemHeader_Object>(sizeof(T));
			if (!p) return nullptr;

			// 继续填充 header
			auto h = (MemHeader_Object*)p - 1;
			h->objectData = 0;
			h->typeId = typeId;

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
				mp->template Free<MemHeader_Object>(p);
				return nullptr;
			}
		};
	}

	inline bool MemPool::IsBaseOf(uint32_t const& baseTypeId, uint32_t typeId) noexcept
	{
		for (; typeId != baseTypeId; typeId = pids[typeId])
		{
			if (!typeId || typeId == pids[typeId]) return false;
		}
		return true;
	}

	template<typename BT>
	bool MemPool::IsBaseOf(uint32_t const& typeId) noexcept
	{
		return IsBaseOf(TypeId<BT>::value, typeId);
	}

	template<typename BT, typename T>
	bool MemPool::IsBaseOf() noexcept
	{
		return IsBaseOf(TypeId<BT>::value, TypeId<T>::value);
	}

	template<typename T>
	T* MemPool::TryCast(Object* const& p) noexcept
	{
		return IsBaseOf(TypeId<T>::value, p->memHeader().typeId) ? (T*)p : nullptr;
	}







	inline Object::Object(MemPool* const& mempool) noexcept
		: mempool(mempool)
	{}

	inline Object::Object(BBuffer* const& bb) noexcept
		: mempool(bb->mempool)
	{
	}

	inline Object::~Object() noexcept {}

	inline void Object::Release() noexcept
	{
		mempool->Release(this);
	}
	inline bool Object::IsReleased(decltype(MemHeader_Object::versionNumber) const& vn) const noexcept
	{
		return vn != memHeader().versionNumber;
	}

	inline MemHeader_Object& Object::memHeader() noexcept { return *((MemHeader_Object*)this - 1); }
	inline MemHeader_Object& Object::memHeader() const noexcept { return *((MemHeader_Object*)this - 1); }

	inline void Object::ToString(String& s) const noexcept {}
	inline void Object::ToStringCore(String& s) const noexcept {}
	inline void Object::ToBBuffer(BBuffer& bb) const noexcept {}
	inline int Object::FromBBuffer(BBuffer& bb) noexcept { return 0; }









	template<typename T>
	Ptr<T>::Ptr() noexcept
		: pointer(nullptr)
	{}

	template<typename T>
	template<typename O>
	Ptr<T>::Ptr(O* const& pointer) noexcept
		: pointer((T*)pointer)
	{
		static_assert(std::is_base_of_v<T, O>);
		if (pointer)
		{
			++pointer->memHeader().refs;
		}
	}
	template<typename T>
	template<typename O>
	Ptr<T>::Ptr(Ptr<O> const& o) noexcept
		: pointer((T*)o.pointer)
	{
		static_assert(std::is_base_of_v<T, O>);
		if (o.pointer)
		{
			++o.pointer->memHeader().refs;
		}
	}

	template<typename T>
	template<typename O>
	Ptr<T>::Ptr(Ptr<O>&& o) noexcept
		: pointer((T*)o.pointer)
	{
		static_assert(std::is_base_of_v<T, O>);
		o.pointer = nullptr;
	}


	template<typename T>
	Ptr<T>::Ptr(Ptr<T> const& o) noexcept
		: pointer((T*)o.pointer)
	{
		if (o.pointer)
		{
			++o.pointer->memHeader().refs;
		}
	}

	template<typename T>
	Ptr<T>::Ptr(Ptr<T>&& o) noexcept
		: pointer(o.pointer)
	{
		o.pointer = nullptr;
	}





	template<typename T>
	template<typename O>
	Ptr<T>& Ptr<T>::operator=(O* const& o) noexcept
	{
		static_assert(std::is_base_of_v<T, O>);
		Reset();
		if (o)
		{
			pointer = (T*)o;
			++o->memHeader().refs;
		}
		return *this;
	}

	template<typename T>
	template<typename O>
	Ptr<T>& Ptr<T>::operator=(Ptr<O> const& o) noexcept
	{
		static_assert(std::is_base_of_v<T, O>);
		return operator=<O>(o.pointer);
	}

	template<typename T>
	template<typename O>
	Ptr<T>& Ptr<T>::operator=(Ref<O> const& o) noexcept
	{
		static_assert(std::is_base_of_v<T, O>);
		return operator=<O>(o.Lock());
	}


	template<typename T>
	template<typename O>
	Ptr<T>& Ptr<T>::operator=(Ptr<O>&& o) noexcept
	{
		static_assert(std::is_base_of_v<T, O>);
		Reset();
		pointer = (T*)o.pointer;
		o.pointer = nullptr;
		return *this;
	}

	template<typename T>
	Ptr<T>& Ptr<T>::operator=(Ptr<T> const& o) noexcept
	{
		return operator=<T>(o.pointer);
	}

	template<typename T>
	Ptr<T>& Ptr<T>::operator=(Ptr<T>&& o) noexcept
	{
		return operator=<T>(std::move(o));
	}

	template<typename T>
	template<typename O>
	Ptr<T>::operator Ptr<O>&() const noexcept
	{
		static_assert(std::is_base_of_v<O, T>);
		return *(Ptr<O>*)this;
	}

	template<typename T>
	template<typename O>
	Ptr<T>::operator O*() const noexcept
	{
		static_assert(std::is_base_of_v<O, T>);
		return (O*)pointer;
	}
	template<typename T>
	template<typename O>
	Ptr<T>::operator O const*() const noexcept
	{
		static_assert(std::is_base_of_v<O, T>);
		return (O const*)pointer;
	}



	template<typename T>
	void Ptr<T>::Reset() noexcept
	{
		if (pointer)
		{
			if (--pointer->memHeader().refs == 0)
			{
				pointer->Release();
			}
			pointer = nullptr;
		}
	}


	template<typename T>
	template<typename O>
	void Ptr<T>::Reset(O* const& o) noexcept
	{
		static_assert(std::is_base_of_v<T, O>);
		Reset();
		if (o)
		{
			pointer = (T*)o;
			++o->memHeader().refs;
		}
	}


	template<typename T>
	decltype(MemHeader_Object::refs) Ptr<T>::GetRefs() const noexcept
	{
		if (pointer) return pointer->memHeader().refs;
		else return 0;
	}
	template<typename T>
	decltype(MemHeader_Object::typeId) Ptr<T>::GetTypeId() const noexcept
	{
		if (pointer) return pointer->memHeader().typeId;
		else return 0;
	}


	template<typename T>
	Ref<T> Ptr<T>::MakeRef() const noexcept
	{
		Ref<T> r;
		r = pointer;
		return r;
	}


	template<typename T>
	template<typename O>
	Ptr<O> const& Ptr<T>::As() const noexcept
	{
		assert(Is<O>());
		return *(Ptr<O>*)this;
	}
	template<typename T>
	template<typename O>
	Ptr<O>& Ptr<T>::As() noexcept
	{
		assert(Is<O>());
		return *(Ptr<O>*)this;
	}

	template<typename T>
	template<typename O>
	bool Ptr<T>::Is() const noexcept
	{
		return !this || MemPool::IsBaseOf<O>(pointer->memHeader().typeId);
	}
	template<typename T>
	bool Ptr<T>::Is(uint16_t const& typeId) const noexcept
	{
		return !this || MemPool::IsBaseOf(typeId, pointer->memHeader().typeId);
	}


	template<typename T>
	Ptr<T>::~Ptr() noexcept
	{
		Reset();
	}


	template<typename T>
	template<typename O>
	bool Ptr<T>::operator==(Ptr<O> const& o) const noexcept
	{
		return pointer == o.pointer;
	}
	template<typename T>
	template<typename O>
	bool Ptr<T>::operator!=(Ptr<O> const& o) const noexcept
	{
		return pointer != o.pointer;
	}

	template<typename T>
	template<typename O>
	bool Ptr<T>::operator==(Ref<O> const& o) const noexcept
	{
		return pointer == o.Lock().pointer;
	}
	template<typename T>
	template<typename O>
	bool Ptr<T>::operator!=(Ref<O> const& o) const noexcept
	{
		return pointer != o.Lock().pointer;
	}

	template<typename T>
	template<typename O>
	bool Ptr<T>::operator==(O* const& o) const noexcept
	{
		return pointer == (T*)o;
	}
	template<typename T>
	template<typename O>
	bool Ptr<T>::operator!=(O* const& o) const noexcept
	{
		return pointer != (T*)o;
	}


	template<typename T>
	T* Ptr<T>::operator->() const noexcept
	{
		return (T*)pointer;
	}

	template<typename T>
	T& Ptr<T>::operator*() const noexcept
	{
		return *(T*)pointer;
	}

	template<typename T>
	Ptr<T>::operator bool() const noexcept
	{
		return pointer != nullptr;
	}



	template<typename T>
	template<typename O, typename ...Args>
	Ptr<T>& Ptr<T>::Create(MemPool* const& mp, Args&&...args) noexcept
	{
		Reset(mp->Create<O>(std::forward<Args>(args)...));
		return *this;
	}

	template<typename T>
	template<typename O, typename ...Args>
	Ptr<T>& Ptr<T>::MPCreate(MemPool* const& mp, Args&&...args) noexcept
	{
		Reset(mp->MPCreate<O>(std::forward<Args>(args)...));
		return *this;
	}





	template<typename T>
	Ref<T>::Ref() noexcept
		: pointer(nullptr)
		, versionNumber(0)
	{}

	template<typename T>
	Ref<T>::~Ref() noexcept
	{
		if (*this && !pointer->memHeader().refs)
		{
			pointer->Release();
		}
	}

	//template<typename T>
	//template<typename O>
	//Ref<T>::Ref(O* const& o) noexcept
	//	: pointer(o)
	//{
	//	static_assert(std::is_base_of_v<T, O>);
	//	versionNumber = o ? o->memHeader().versionNumber : 0;
	//}

	//template<typename T>
	//template<typename O>
	//Ref<T>::Ref(O* const& o, decltype(MemHeader::versionNumber) versionNumber) noexcept
	//	: pointer(o)
	//	, versionNumber(versionNumber)
	//{
	//	static_assert(std::is_base_of_v<T, O>);
	//}

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
		: pointer(o.pointer)
		, versionNumber(o.versionNumber)
	{
	}

	template<typename T>
	Ref<T>::Ref(Ref&& o) noexcept
		: pointer(o.pointer)
		, versionNumber(o.versionNumber)
	{
		o.pointer = nullptr;
		o.versionNumber = 0;
	}

	template<typename T>
	template<typename O>
	Ref<T>::Ref(Ref<O> const& o) noexcept
		: pointer(o.pointer)
		, versionNumber(o.versionNumber)
	{
		static_assert(std::is_base_of_v<T, O>);
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
		pointer = o.pointer;
		versionNumber = o.versionNumber;
		return *this;
	}
	template<typename T>
	template<typename O>
	Ref<T>& Ref<T>::operator=(Ref<O> const& o) noexcept
	{
		pointer = o.pointer;
		versionNumber = o.versionNumber;
		return *this;
	}




	template<typename T>
	template<typename O>
	bool Ref<T>::operator==(Ptr<O> const& o) const noexcept
	{
		return o.pointer == Lock().pointer;
	}
	template<typename T>
	template<typename O>
	bool Ref<T>::operator!=(Ptr<O> const& o) const noexcept
	{
		return o.pointer != Lock().pointer;
	}

	template<typename T>
	template<typename O>
	bool Ref<T>::operator==(Ref<O> const& o) const noexcept
	{
		return Lock().pointer == o.Lock().pointer;
	}
	template<typename T>
	template<typename O>
	bool Ref<T>::operator!=(Ref<O> const& o) const noexcept
	{
		return Lock().pointer != o.Lock().pointer;
	}

	template<typename T>
	template<typename O>
	Ref<T>& Ref<T>::operator=(O* const& o) noexcept
	{
		static_assert(std::is_base_of_v<T, O>);
		pointer = o;
		versionNumber = o ? o->memHeader().versionNumber : 0;
		return *this;
	}

	template<typename T>
	template<typename O>
	bool Ref<T>::operator==(O* const& o) const noexcept
	{
		return Lock().pointer == o;
	}
	template<typename T>
	template<typename O>
	bool Ref<T>::operator!=(O* const& o) const noexcept
	{
		return Lock().pointer != o;
	}



	template<typename T>
	Ptr<T>& Ref<T>::Lock() const noexcept
	{
		if (!(pointer && pointer->memHeader().versionNumber == versionNumber))
		{
			((Ref<T>*)this)->Reset();
		}
		return *(Ptr<T>*)this;
	}


	template<typename T>
	void Ref<T>::Reset() noexcept
	{
		pointer = nullptr;
		versionNumber = 0;
	}

	template<typename T>
	Ref<T>::operator bool() const noexcept
	{
		return pointer && pointer->memHeader().versionNumber == versionNumber;
	}

	template<typename T>
	T* Ref<T>::operator->() const noexcept
	{
		return (T*)pointer;
	}

	template<typename T>
	T& Ref<T>::operator*() const noexcept
	{
		return *(T*)pointer;
	}




















	template<typename T>
	Unique<T>::Unique() noexcept
		: pointer(nullptr)
	{}

	template<typename T>
	template<typename O>
	Unique<T>::Unique(O* const& pointer) noexcept
		: pointer((T*)pointer)
	{
		static_assert(std::is_base_of_v<T, O>);
		if (pointer)
		{
			++pointer->memHeader().refs;
		}
	}

	template<typename T>
	template<typename O>
	Unique<T>::Unique(Unique<O>&& o) noexcept
		: pointer((T*)o.pointer)
	{
		static_assert(std::is_base_of_v<T, O>);
		o.pointer = nullptr;
	}

	template<typename T>
	Unique<T>::Unique(Unique<T>&& o) noexcept
		: pointer(o.pointer)
	{
		o.pointer = nullptr;
	}





	template<typename T>
	template<typename O>
	Unique<T>& Unique<T>::operator=(O* const& o) noexcept
	{
		static_assert(std::is_base_of_v<T, O>);
		Reset();
		if (o)
		{
			pointer = (T*)o;
			++o->memHeader().refs;
		}
		return *this;
	}

	template<typename T>
	template<typename O>
	Unique<T>& Unique<T>::operator=(Unique<O>&& o) noexcept
	{
		static_assert(std::is_base_of_v<T, O>);
		Reset();
		pointer = (T*)o.pointer;
		o.pointer = nullptr;
		return *this;
	}

	template<typename T>
	Unique<T>& Unique<T>::operator=(Unique<T>&& o) noexcept
	{
		return operator=<T>(std::move(o));
	}

	template<typename T>
	template<typename O>
	Unique<T>::operator Unique<O>&() const noexcept
	{
		static_assert(std::is_base_of_v<O, T>);
		return *(Unique<O>*)this;
	}

	template<typename T>
	template<typename O>
	Unique<T>::operator O*() const noexcept
	{
		static_assert(std::is_base_of_v<O, T>);
		return (O*)pointer;
	}
	template<typename T>
	template<typename O>
	Unique<T>::operator O const*() const noexcept
	{
		static_assert(std::is_base_of_v<O, T>);
		return (O const*)pointer;
	}



	template<typename T>
	void Unique<T>::Reset() noexcept
	{
		if (pointer)
		{
			if (--pointer->memHeader().refs == 0)
			{
				pointer->Release();
			}
			pointer = nullptr;
		}
	}


	template<typename T>
	template<typename O>
	void Unique<T>::Reset(O* const& o) noexcept
	{
		static_assert(std::is_base_of_v<T, O>);
		Reset();
		if (o)
		{
			pointer = (T*)o;
			++o->memHeader().refs;
		}
	}


	template<typename T>
	decltype(MemHeader_Object::refs) Unique<T>::GetRefs() const noexcept
	{
		if (pointer) return pointer->memHeader().refs;
		else return 0;
	}
	template<typename T>
	decltype(MemHeader_Object::typeId) Unique<T>::GetTypeId() const noexcept
	{
		if (pointer) return pointer->memHeader().typeId;
		else return 0;
	}


	template<typename T>
	Weak<T> Unique<T>::MakeWeak() const noexcept
	{
		return Weak<T>(pointer);
	}


	template<typename T>
	template<typename O>
	Unique<O> const& Unique<T>::As() const noexcept
	{
		assert(Is<O>());
		return *(Unique<O>*)this;
	}
	template<typename T>
	template<typename O>
	Unique<O>& Unique<T>::As() noexcept
	{
		assert(Is<O>());
		return *(Unique<O>*)this;
	}

	template<typename T>
	template<typename O>
	bool Unique<T>::Is() const noexcept
	{
		return !this || MemPool::IsBaseOf<O>(pointer->memHeader().typeId);
	}
	template<typename T>
	bool Unique<T>::Is(uint16_t const& typeId) const noexcept
	{
		return !this || MemPool::IsBaseOf(typeId, pointer->memHeader().typeId);
	}


	template<typename T>
	Unique<T>::~Unique() noexcept
	{
		Reset();
	}


	template<typename T>
	template<typename O>
	bool Unique<T>::operator==(Unique<O> const& o) const noexcept
	{
		return pointer == o.pointer;
	}
	template<typename T>
	template<typename O>
	bool Unique<T>::operator!=(Unique<O> const& o) const noexcept
	{
		return pointer != o.pointer;
	}

	template<typename T>
	template<typename O>
	bool Unique<T>::operator==(Weak<O> const& o) const noexcept
	{
		o.operator bool();
		return pointer == o.pointer;
	}
	template<typename T>
	template<typename O>
	bool Unique<T>::operator!=(Weak<O> const& o) const noexcept
	{
		o.operator bool();
		return pointer != o.pointer;
	}

	template<typename T>
	template<typename O>
	bool Unique<T>::operator==(O* const& o) const noexcept
	{
		return pointer == (T*)o;
	}
	template<typename T>
	template<typename O>
	bool Unique<T>::operator!=(O* const& o) const noexcept
	{
		return pointer != (T*)o;
	}


	template<typename T>
	T* Unique<T>::operator->() const noexcept
	{
		return (T*)pointer;
	}

	template<typename T>
	T& Unique<T>::operator*() const noexcept
	{
		return *(T*)pointer;
	}

	template<typename T>
	Unique<T>::operator bool() const noexcept
	{
		return pointer != nullptr;
	}



	template<typename T>
	template<typename O, typename ...Args>
	Unique<T>& Unique<T>::Create(MemPool* const& mp, Args&&...args) noexcept
	{
		Reset(mp->Create<O>(std::forward<Args>(args)...));
		return *this;
	}

	template<typename T>
	template<typename O, typename ...Args>
	Unique<T>& Unique<T>::MPCreate(MemPool* const& mp, Args&&...args) noexcept
	{
		Reset(mp->MPCreate<O>(std::forward<Args>(args)...));
		return *this;
	}












	template<typename T>
	Weak<T>::Weak() noexcept
		: pointer(nullptr)
		, versionNumber(0)
	{}

	template<typename T>
	template<typename O>
	Weak<T>::Weak(O* const& o) noexcept
		: pointer(o)
	{
		static_assert(std::is_base_of_v<T, O>);
		versionNumber = o ? o->memHeader().versionNumber : 0;
	}

	template<typename T>
	Weak<T>::Weak(T* const& o) noexcept
		: pointer(o)
	{
		versionNumber = o ? o->memHeader().versionNumber : 0;
	}

	template<typename T>
	template<typename O>
	Weak<T>::Weak(O* const& o, decltype(MemHeader::versionNumber) versionNumber) noexcept
		: pointer(o)
		, versionNumber(versionNumber)
	{
		static_assert(std::is_base_of_v<T, O>);
	}

	template<typename T>
	Weak<T>::Weak(Weak const& o) noexcept
		: pointer(o.pointer)
		, versionNumber(o.versionNumber)
	{
	}

	template<typename T>
	Weak<T>::Weak(Weak&& o) noexcept
		: pointer(o.pointer)
		, versionNumber(o.versionNumber)
	{
		o.pointer = nullptr;
		o.versionNumber = 0;
	}

	template<typename T>
	template<typename O>
	Weak<T>::Weak(Weak<O> const& o) noexcept
		: pointer(o.pointer)
		, versionNumber(o.versionNumber)
	{
		static_assert(std::is_base_of_v<T, O>);
	}


	template<typename T>
	Weak<T>& Weak<T>::operator=(Weak const& o) noexcept
	{
		pointer = o.pointer;
		versionNumber = o.versionNumber;
		return *this;
	}
	template<typename T>
	template<typename O>
	Weak<T>& Weak<T>::operator=(Weak<O> const& o) noexcept
	{
		pointer = o.pointer;
		versionNumber = o.versionNumber;
		return *this;
	}


	template<typename T>
	template<typename O>
	bool Weak<T>::operator==(Unique<O> const& o) const noexcept
	{
		operator bool();
		return pointer == o.pointer;
	}
	template<typename T>
	template<typename O>
	bool Weak<T>::operator!=(Unique<O> const& o) const noexcept
	{
		operator bool();
		return pointer != o.pointer;
	}


	template<typename T>
	template<typename O>
	bool Weak<T>::operator==(Weak<O> const& o) const noexcept
	{
		operator bool();
		o.operator bool();
		return pointer == o.pointer;
	}
	template<typename T>
	template<typename O>
	bool Weak<T>::operator!=(Weak<O> const& o) const noexcept
	{
		operator bool();
		o.operator bool();
		return pointer != o.pointer;
	}

	template<typename T>
	template<typename O>
	bool Weak<T>::operator==(O* const& o) const noexcept
	{
		operator bool();
		return pointer == o;
	}
	template<typename T>
	template<typename O>
	bool Weak<T>::operator!=(O* const& o) const noexcept
	{
		operator bool();
		return pointer != o;
	}






	template<typename T>
	template<typename O>
	Weak<O> const& Weak<T>::As() const noexcept
	{
		assert(Is<O>());
		return *(Weak<O>*)this;
	}
	template<typename T>
	template<typename O>
	Weak<O>& Weak<T>::As() noexcept
	{
		assert(Is<O>());
		return *(Weak<O>*)this;
	}

	template<typename T>
	template<typename O>
	bool Weak<T>::Is() const noexcept
	{
		return !this || MemPool::IsBaseOf<O>(pointer->memHeader().typeId);
	}
	template<typename T>
	bool Weak<T>::Is(uint16_t const& typeId) const noexcept
	{
		return !this || MemPool::IsBaseOf(typeId, pointer->memHeader().typeId);
	}





	template<typename T>
	template<typename O>
	Weak<T>& Weak<T>::operator=(O* const& o) noexcept
	{
		static_assert(std::is_base_of_v<T, O>);
		pointer = o;
		versionNumber = o ? o->memHeader().versionNumber : 0;
		return *this;
	}

	template<typename T>
	void Weak<T>::Reset() noexcept
	{
		pointer = nullptr;
		versionNumber = 0;
	}

	template<typename T>
	Weak<T>::operator bool() const noexcept
	{
		if (pointer)
		{
			if (pointer->memHeader().versionNumber == versionNumber)
			{
				return true;
			}
			else
			{
				((Weak<T>*)this)->Reset();
				return false;
			}
		}
		else
		{
			return false;
		}
	}

	template<typename T>
	T* Weak<T>::operator->() const noexcept
	{
		return (T*)pointer;
	}

	template<typename T>
	T& Weak<T>::operator*() const noexcept
	{
		return *(T*)pointer;
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
		if (!o) return os << "nil";
		return os << *o;
	}

}
