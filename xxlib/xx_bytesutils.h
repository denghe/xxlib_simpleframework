#pragma once
namespace xx
{
	/**************************************************************************************************/
	// 一些处理变长读写的基础函数
	/**************************************************************************************************/

	// negative -> ZigZag positive.  效果：负数变正，正数 *= 2
	uint16_t ZigZagEncode(int16_t in) noexcept;
	uint32_t ZigZagEncode(int32_t in) noexcept;
	uint64_t ZigZagEncode(int64_t in) noexcept;

	// ZigZag positive -> negative
	int16_t ZigZagDecode(uint16_t in) noexcept;
	int32_t ZigZagDecode(uint32_t in) noexcept;
	int64_t ZigZagDecode(uint64_t in) noexcept;

	size_t VarWrite7(char *dstBuf, uint16_t in) noexcept;
	size_t VarWrite7(char *dstBuf, uint32_t in) noexcept;
	size_t VarWrite7(char *dstBuf, uint64_t in) noexcept;

	int VarRead7(char const *srcBuf, size_t dataLen, size_t& offset, uint32_t &out) noexcept;
	int VarRead7(char const *srcBuf, size_t dataLen, size_t& offset, uint16_t &out) noexcept;
	int VarRead7(char const *srcBuf, size_t dataLen, size_t& offset, uint64_t &out) noexcept;


	/**************************************************************************************************/
	// 类型--操作适配模板区
	/**************************************************************************************************/

	class BBuffer;

	// 基础适配模板
	template<typename T, typename ENABLE = void>
	struct BytesFunc
	{
		static void WriteTo(BBuffer& bb, T const &in)
		{
			assert(false);
		}
		static int ReadFrom(BBuffer& bb, T &out)
		{
			assert(false);
			return 0;
		}
	};

}
