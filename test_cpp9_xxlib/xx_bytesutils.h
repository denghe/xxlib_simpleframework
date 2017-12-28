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
		// 返回 Reserve 长度参考
		static size_t Calc(T const &in)
		{
			assert(false);
			return 0;
		}
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



	/**************************************************************************************************/
	// 将 BytesFunc 转为函数
	/**************************************************************************************************/

	template<typename T>
	size_t BBCalc(T const &in)
	{
		return BytesFunc<T>::Calc(in);
	}

	template<typename T>
	void BBWriteTo(BBuffer& bb, T const &in)
	{
		return BytesFunc<T>::WriteTo(bb, in);
	}

	template<typename T>
	int BBReadFrom(BBuffer& bb, T &out)
	{
		return BytesFunc<T>::ReadFrom(bb, out);
	}


	///**************************************************************************************************/
	//// 变参模板支持
	///**************************************************************************************************/

	//template<typename T, typename...TS>
	//uint32_t BBCalc(T const &in, TS const& ... ins)
	//{
	//	return BBCalc(in) + BBCalc(ins...);
	//}
	//template<typename T, typename...TS>
	//uint32_t BBWriteTo(char *dstBuf, T const &in, TS const& ... ins)
	//{
	//	uint32_t offset = BBWriteTo(dstBuf, in);
	//	return offset + BBWriteTo(dstBuf + offset, ins...);
	//}
	//template<typename T, typename...TS>
	//int BBReadFrom(char const *srcBuf, uint32_t const &dataLen, uint32_t &offset, T &out, TS &...outs)
	//{
	//	auto rtv = BBReadFrom(srcBuf, dataLen, offset, out);
	//	if (rtv) return rtv;
	//	return BBReadFrom(srcBuf, dataLen, offset, outs...);
	//}


}
