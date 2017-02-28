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
				const uint8_t mpIndex;		// 分配 / 回收时的内存池数组 下标( 指向 versionNumber 的最高位字节 )
			};
		};

		// 通过一个指针来访问头部
		static MemHeader_VersionNumber& Visit(void* p)
		{
			return *((MemHeader_VersionNumber*)p - 1);
		}
	};

	struct MemPoolBase;

	// 经由 MemPool 分配的 针对 MPObject 派生类的内存都带有这样一个头部
	struct MemHeader_MPObject : public MemHeader_VersionNumber
	{
		// 便于局部代码访问内存池( 使用 MP::Get(this) 来引用到内存池. MP 是内存池的 type )
		MemPoolBase *mempool;

		// 减到 0 就真正 Dispose
		uint32_t refCount = 1;

		// MemPool 创建时填充类型ID
		uint32_t typeId = 0;

		// 通过一个指针来访问头部
		static MemHeader_MPObject& Visit(void* p)
		{
			return *((MemHeader_MPObject*)p - 1);
		}
	};
}
