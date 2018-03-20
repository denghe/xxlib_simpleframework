#pragma once
namespace xx
{
	class BBuffer : public List<char>
	{
	public:
		typedef List<char> BaseType;
		size_t										offset = 0;				// 读指针偏移量
		size_t										offsetRoot = 0;			// offset值写入修正
		size_t										dataLenBak = 0;			// WritePackage 时用于备份当前数据写入偏移
		size_t										readLengthLimit = 0;	// 主用于传递给容器类进行长度合法校验

		BBuffer(BBuffer const&o) = delete;
		BBuffer& operator=(BBuffer const&o) = delete;

		explicit BBuffer(MemPool* const& mempool, size_t const& capacity = 0);
		BBuffer(MemPool* const& mempool, std::pair<char const*, size_t> const& buff);


		template<typename ...TS>
		void Write(TS const & ...vs);

		template<typename ...TS>
		int Read(TS &...vs);

		template<typename T, typename ...TS>
		int ReadCore(T& v, TS &...vs);
		template<typename T>
		int ReadCore(T& v);


		/*************************************************************************/
		//  Object 对象读写系列
		/*************************************************************************/


		void BeginWrite();
		void BeginRead();

		template<typename T>
		void WriteRoot(T const& v);

		template<typename T>
		int ReadRoot(T& v);

		template<typename T>
		void WritePtr(T* const& v);

		template<typename T>
		int ReadPtr(T* &v);


		/*************************************************************************/
		//  其他工具函数
		/*************************************************************************/

		void Clear();

		// 根据数据类型往当前位置写入默认值
		template<typename T>
		void WriteDefaultValue();

		// 自定义 Write 函数
		std::function<void(BBuffer& bb, void* owner, size_t fieldOffset)> CustomWrite;


		// 读指定位置的数据( 不影响 offset )
		template<typename ...TS>
		int ReadAt(size_t const& pos, TS&...vs);

		// 从当前位置读数据但事后 offset 偏移指定长度
		template<typename ...TS>
		int ReadJump(size_t const& len, TS&...vs);

		// 直接追加写入一段 buf ( 并不记录长度 )
		void WriteBuf(char const* buf, size_t const& len);
		void WriteBuf(BBuffer const& bb);
		void WriteBuf(BBuffer const* const& bb);

		// 追加一个指定长度的空间, 返回当前 dataLen
		size_t WriteSpace(size_t const& len);

		// 在 pos 位置写入一段 buf ( 并不记录长度 ). dataLen 可能撑大.
		void WriteBufAt(size_t const& pos, char const* buf, size_t const& len);

		// 在 pos 位置做 Write 操作. dataLen 可能撑大.
		template<typename ...TS>
		void WriteAt(size_t const& pos, TS const&...vs);


		/*************************************************************************/
		//  包相关
		/*************************************************************************/

		// todo: 大包支持

		// 开始写一个包
		void BeginWritePackage(uint8_t const& pkgTypeId = 0, uint32_t const& serial = 0);

		// 结束写一个包, 返回长度是否在包头表达范围内( 如果 true 则会填充包头, false 则回滚长度 )
		bool EndWritePackage();

		// 一键爽 write 定长 字节长度 + root数据. 如果超过 长度最大计数, 将回滚 dataLen 并返回 false
		template<typename T>
		bool WritePackage(uint8_t const& pkgTypeId = 0, uint32_t const& serial = 0);

		// 在已知数据长度的情况下, 直接以包头格式写入长度. 成功返回 true( 只针对 pkgTypeId == 0 的情况 )
		bool WritePackageLength(uint16_t const& len);

		// 尝试一次性反序列化一到多个包, 将结果填充到 outPkgs, 返回 0 或 错误码
		// 注意: 注意其元素的 引用计数, 通通为 1( 即便是递归互引 )
		// 注意: 即便返回错误码, outPkgs 中也可能存在残留数据
		int ReadPackages(List<Object_p>& outPkgs);

		// 队列版并不清除原有数据, 乃是追加. 如果出错, 也不会回滚.
		int ReadPackages(Queue<Object_p>& outPkgs);

		// 试读出单个包( 等同于 ReadRoot )
		template<typename T>
		int ReadPackage(T& outPkg);


		// Object 接口实现

		BBuffer(BBuffer* bb);
		void ToBBuffer(BBuffer &bb) const override;
		int FromBBuffer(BBuffer &bb) override;

		void ToString(String &s) const override;
	};


	using BBuffer_p = Ptr<BBuffer>;

	using BBuffer_r = Ref<BBuffer>;

}
