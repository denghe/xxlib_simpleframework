#pragma once
namespace xx
{
	// 关于 String 跨语言的序列化注意事项: c# server 将会对 string 类型做合并, 导致发出的包如果遇到相同串内容, 就会指向同一个指针, 
	// 于是 c++ 在收到包之后, 解出来也就只有一个实例了. 此时如果对任意引用到该串的指针做内容修改操作, 后果不堪设想.
	// 对于这个问题, 的确可以在 c++ 解包处针对 string 特化 read 流程, 创建出多个副本. 但如果该内容被再次发送时, string 就没有合并效果了. 比较纠结
	// 通常性的建议是, 包纯用于数据收发时, 尽量使用内容复制的方式来读取解开后的数据, 除非非常有把握.
	// 用于数据建模时, 如果有修改字串的需求, 可以用 MPCreate 直接新建, 以免牵连修改. 新建构造复制也不会慢多少.

	class BBuffer : public List<char>
	{
	public:
		typedef List<char> BaseType;
		size_t										offset = 0;				// 读指针偏移量
		size_t										offsetRoot = 0;			// offset值写入修正
		size_t										readLengthLimit = 0;	// 主用于传递给容器类进行长度合法校验

		BBuffer(BBuffer const& o) = delete;
		BBuffer& operator=(BBuffer const& o) = delete;

		explicit BBuffer(MemPool* const& mempool, size_t const& capacity = 0) noexcept;
		BBuffer(MemPool* const& mempool, std::pair<char const*, size_t> const& buff) noexcept;


		template<typename ...TS>
		void Write(TS const& ...vs) noexcept;

		template<typename ...TS>
		int Read(TS&...vs) noexcept;

		template<typename T, typename ...TS>
		int ReadCore(T& v, TS&...vs) noexcept;
		template<typename T>
		int ReadCore(T& v) noexcept;


		/*************************************************************************/
		//  Object 对象读写系列
		/*************************************************************************/


		void BeginWrite() noexcept;
		void BeginRead() noexcept;

		template<typename T>
		void WriteRoot(T const& v) noexcept;

		template<typename T>
		int ReadRoot(T& v) noexcept;

		template<typename T>
		void WritePtr(T* const& v) noexcept;

		template<typename T>
		int ReadPtr(T*& v) noexcept;


		/*************************************************************************/
		//  其他工具函数
		/*************************************************************************/

		void Clear() noexcept;

		// 根据数据类型往当前位置写入默认值
		template<typename T>
		void WriteDefaultValue() noexcept;

		// 自定义 Write 函数
		std::function<void(BBuffer& bb, void* owner, size_t fieldOffset)> CustomWrite;


		// 读指定位置的数据( 不影响 offset )
		template<typename ...TS>
		int ReadAt(size_t const& pos, TS&...vs) noexcept;

		// 从当前位置读数据但事后 offset 偏移指定长度
		template<typename ...TS>
		int ReadJump(size_t const& len, TS&...vs) noexcept;

		// 直接追加写入一段 buf ( 并不记录长度 )
		void WriteBuf(char const* const& buf, size_t const& len) noexcept;
		void WriteBuf(BBuffer const& bb) noexcept;
		void WriteBuf(BBuffer const* const& bb) noexcept;

		// 追加一个指定长度的空间, 返回当前 dataLen
		size_t WriteSpace(size_t const& len) noexcept;

		// 在 pos 位置写入一段 buf ( 并不记录长度 ). dataLen 可能撑大.
		void WriteBufAt(size_t const& pos, char const* const& buf, size_t const& len) noexcept;

		// 在 pos 位置做 Write 操作. dataLen 可能撑大.
		template<typename ...TS>
		void WriteAt(size_t const& pos, TS const&...vs) noexcept;



		// Object 接口实现

		BBuffer(BBuffer* const& bb);
		void ToBBuffer(BBuffer& bb) const noexcept override;
		int FromBBuffer(BBuffer& bb) noexcept override;

		void ToString(String& s) const noexcept override;
	};


	using BBuffer_p = Ptr<BBuffer>;

	using BBuffer_r = Ref<BBuffer>;

	using BBuffer_u = Unique<BBuffer>;

	using BBuffer_w = Weak<BBuffer>;

}
