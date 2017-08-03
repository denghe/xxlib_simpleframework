#pragma once
#include <cstdint>

namespace xx
{
	// 经由 MemPool 分配的内存都带有这样一个头部
	struct MemHeader_VersionNumber
	{
		// 自增版本号, 作为识别当前指针是否有效的重要标识
		union
		{
			uint64_t versionNumber;
			uint8_t vnArea[8];			// vnArea[7] 用于存放 内存池数组 下标( 指向 versionNumber 的最高位字节 )
		};
		uint8_t& ptrStackIndex() { return vnArea[7]; }
	};

	struct MemPool;

	// 经由 MemPool 分配的 针对 MPObject 派生类的内存都带有这样一个头部
	struct MemHeader_MPObject : public MemHeader_VersionNumber
	{
		// 这个用于希望将 MPObject 对象以非指针方式使用时, 放在变量前面以提供内存头空间时使用( 实现在 xx_mempoolbase.h 属 )
		MemHeader_MPObject(MemPool& mempool, uint16_t const& typeId)
			: mempool(&mempool)
			, typeId(typeId)
		{
			this->versionNumber = 0;
			this->refCount = (uint32_t)-1;											// 防 Release
		}
		MemHeader_MPObject(MemHeader_MPObject&& o)
			: mempool(o.mempool)
			, refCount(o.refCount)
			, typeId(o.typeId)
			, tsFlags(o.tsFlags)
		{
			this->versionNumber = o.versionNumber;
			o.versionNumber = 0;
			o.mempool = nullptr;
			o.refCount = 0;
			o.typeId = 0;
			o.tsFlags = 0;
		}

		// 便于局部代码访问内存池( 使用 MP::Get(this) 来引用到内存池. MP 是内存池的 type )
		MemPool *mempool;

		// 减到 0 就真正 Dispose
		uint32_t refCount = 1;

		// MemPool 创建时填充类型ID
		uint16_t typeId = 0;

		// 当前可用于 ToString 防重入( 或是存空格缩进值 )
		uint16_t tsFlags = 0;
	};



	/************************************************************************************/
	// 值类型使用相关
	/************************************************************************************/

	// 为 MPObject 附加内存头以便提供值类型的物理结构( 这种模式下 mh 除了填充内存池指针以下, 不再填充版本号, 类型等信息 )
	template<typename T>
	struct Dock
	{
		MemHeader_MPObject mh;
		T instance;

		template<typename ...Args>
		Dock(MemPool& mempool, Args&& ... args)
			: mh(mempool, TypeId_v<T>)
			, instance(std::forward<Args>(args)...)
		{
		}

		Dock(Dock const&) = delete;
		Dock& operator=(Dock const&) = delete;
		Dock(Dock&& o)
			: mh(std::move(o.mh))
			, instance(std::move(o.instance))
		{
		}

		T const* operator->() const
		{
			return &instance;
		}
		T* operator->()
		{
			return &instance;
		}

		T& operator*()
		{
			return instance;
		}
		T const& operator*() const
		{
			return instance;
		}

		operator T const* () const
		{
			return &instance;
		}
		operator T* ()
		{
			return &instance;
		}
	};


	template<typename T>
	struct IsDock
	{
		static const bool value = false;
	};

	template<typename T>
	struct IsDock<Dock<T>>
	{
		static const bool value = true;
	};
	template<typename T>
	constexpr bool IsDock_v = IsDock<T>::value;


	// 标记在容器中可 memmove

	template<typename T>
	struct MemmoveSupport<Dock<T>>
	{
		static const bool value = MemmoveSupport_v<T>;
	};
}
