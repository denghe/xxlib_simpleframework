#pragma once
#include "xxlib_defines.h"
#include <cstdint>

namespace xxlib
{
	struct MemPool;

	/************************************************************************/
	// 经由 MemPool 分配的内存( 不管什么使用性质 )都带有这样一个头部( 位于返回的指针的左方区域, 直接由 MemPool 初始化 )
	/************************************************************************/

	struct MPObjectHeader
	{
		// 自增版本号, 作为识别当前指针是否有效的重要标识
		union
		{
			uint64_t versionNumber;
			struct
			{
				uint8_t vnArea[7];
				const uint8_t mpIndex;		// 分配 / 回收时的内存池数组 下标( 指向 versionNumber 的最高位字节 )
			};
		};
	};

	/************************************************************************/
	// 使用 MemPool 之 Create / Release 时附加的头部( 增强版, 同样直接由 MemPool 初始化 )
	/************************************************************************/

	struct MPObjectHeaderEx : public MPObjectHeader
	{
		// 保存上下文用到的内存池指针( 基础设计, 必用 )
		MemPool *mempool;

		// 减到 0 就真正 Dispose
		uint32_t refCount = 1;

		// MemPool 创建时填充类型ID
		uint16_t typeId = 0;

		// 当前可用于 ToString 防重入( 或是存空格缩进值 )
		uint16_t tsFlags = 0;
	};


	/************************************************************************/
	// 支持 MemPool 的类都应该实现该基类
	/************************************************************************/

	struct String;
	struct BBuffer;
	struct MPObject
	{
	protected:						// protected 下面须具备这些元素
		friend MemPool;
		MPObject() {}				// 默认构造
		MPObject(MemPool&) {}		// for 反序列化
		// 复制构造, op=			// = delete
		virtual ~MPObject() {}		// 析构如果有, 也要位于 protected

	public:

		// 加持
		inline void AddRef()
		{
			++refCount();
		}

		// 减持 或 析构+回收变野
		void Release();

		/*
		if (!tsFlags()) return; else tsFlags() = 1;
		// str.append( .......... )........
		tsFlags() = 0;
		*/
		virtual void ToString(String &str) const;

		/*
		this->BaseType::ToBBuffer(bb);
		bb.Write(.............);
		*/
		virtual void ToBBuffer(BBuffer &bb) const {}

		/*
		if (auto rtv = this->BaseType::FromBBuffer(bb)) return rtv;
		return bb.Read(.............);
		*/
		virtual int FromBBuffer(BBuffer &bb) { return 0; }

		// todo: virtual GetHashCode? Equals?

		/************************************************************************/
		// 下面是访问头部数据的各种 helpers
		/************************************************************************/

		inline MPObjectHeaderEx& mpHeader() { return *(MPObjectHeaderEx*)((char*)this - sizeof(MPObjectHeaderEx)); }
		inline MPObjectHeaderEx& mpHeader() const { return *(MPObjectHeaderEx*)((char*)this - sizeof(MPObjectHeaderEx)); }
		inline MemPool& mempool() { return *mpHeader().mempool; }
		inline uint64_t const& versionNumber() const { return mpHeader().versionNumber; }
		inline uint64_t pureVersionNumber() const { return versionNumber() & 0x00FFFFFFFFFFFFFFu; }
		inline uint32_t& refCount() { return mpHeader().refCount; }
		inline uint32_t const& refCount() const { return mpHeader().refCount; }
		inline uint16_t& tsFlags() { return mpHeader().tsFlags; }
		inline uint16_t const& tsFlags() const { return mpHeader().tsFlags; }
		inline uint16_t const& typeId() const { return mpHeader().typeId; }

		/************************************************************************/
		// 令 Cout 操作方便点
		/************************************************************************/
		template<typename...TS>
		void Cout(TS const&...ps);	// 实现在 xxlib_cout.h
	};


	/************************************************************************/
	// 与 MemPool 的版本号相配合的智能指针之模板派生类
	/************************************************************************/

	template<typename T>
	struct MPtr
	{
		T* pointer = nullptr;
		uint64_t versionNumber = 0;

		MPtr() {}
		MPtr(MPtr const &o) = default;
		MPtr& operator=(MPtr const& o) = default;
		MPtr(T* p)
			: pointer(p)
			, versionNumber(p ? RefVersionNumber(p) : 0)
		{}
		MPtr& operator=(T* p)
		{
			pointer = p;
			versionNumber = p ? RefVersionNumber(p) : 0;
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
			if (pointer && RefVersionNumber(pointer) == versionNumber) return pointer;
			return nullptr;
		}
		operator bool() const
		{
			return Ensure() != nullptr;
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

	protected:
		template<typename U = MPObject>
		static uint64_t const& RefVersionNumber(std::enable_if_t<std::is_base_of<U, T>::value, U>* p)
		{
			return p->versionNumber();
		}
		template<typename U = MPObject>
		static uint64_t const& RefVersionNumber(std::enable_if_t<!std::is_base_of<U, T>::value>* p)
		{
			return ((MPObjectHeader*)p - 1)->versionNumber;
		}
	};


	/************************************************************************/
	// 下面是一些 helper / 标记特化
	/************************************************************************/

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
	struct IsMPObject
	{
		static const bool value = false;
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
