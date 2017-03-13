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
			struct
			{
				uint8_t vnArea[7];
				uint8_t mpIndex;		// 分配 / 回收时的内存池数组 下标( 指向 versionNumber 的最高位字节 )
			};
		};
	};

	struct MemPoolBase;

	// 经由 MemPool 分配的 针对 MPObject 派生类的内存都带有这样一个头部
	struct MemHeader_MPObject : public MemHeader_VersionNumber
	{
		// 这个用于希望将 MPObject 对象以非指针方式使用时, 放在变量前面以提供内存头空间时使用( 实现在 xx_mempoolbase.h 属 )
		MemHeader_MPObject(MemPoolBase& mempoolbase);

		// 便于局部代码访问内存池( 使用 MP::Get(this) 来引用到内存池. MP 是内存池的 type )
		MemPoolBase *mempoolbase;

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
	struct MPStruct
	{
		MemHeader_MPObject mh;
		T instance;
		template<typename ...Args>
		MPStruct(MemPoolBase& mempoolbase, Args&& ... args)
			: mh(mempoolbase)
			, instance(std::forward<Args>(args)...)
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
	};


	template<typename T>
	struct IsMPStruct
	{
		static const bool value = false;
	};

	template<typename T>
	struct IsMPStruct<MPStruct<T>>
	{
		static const bool value = true;
	};
	template<typename T>
	constexpr bool IsMPStruct_v = IsMPStruct<T>::value;

}
