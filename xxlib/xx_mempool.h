#pragma once
namespace xx
{
	/***********************************************************************************/
	// 公共内存头相关
	/***********************************************************************************/

	// Alloc 出来的都带这个头
	struct MemHeader
	{
		union
		{
			uint64_t versionNumber;		// 用于野指针判定
			uint8_t vnArea[8];
		};
		uint8_t& mpIndex() { return vnArea[7]; }	// 内存池数组 下标
	};

	// Create 出来的都带这个头
	struct MemHeader_Object : MemHeader
	{
		union
		{
			uint64_t objectData;		// 方便清 0
			struct
			{
				// 受 Ptr 类影响, Ptr 构造时 +1, 析构时 -1, 减到 0 时 Release
				uint32_t refs;

				// MemPool 创建时填充类型ID
				uint16_t typeId;

				// 当前用于 ToString 防重入( 或是存空格缩进值 )
				uint16_t flags;
			};
		};
	};


	/***********************************************************************************/
	// TypeId 映射
	/***********************************************************************************/

	template<typename T>
	struct TypeId
	{
		static const uint16_t value = 0;
	};

	template<typename T>
	constexpr uint16_t TypeId_v = TypeId<T>::value;


	// 一些预声明

	template<typename T>
	class Ptr;

	template<typename T>
	class Ref;

	template<typename T>
	class Unique;

	template<typename T>
	class Weak;

	class BBuffer;
	class Object;
	class String;
	template <typename TK, typename TV>
	class Dict;
	template <typename TK>
	class HashSet;


	/***********************************************************************************/
	// 粗暴快速的非线程安全内存池
	/***********************************************************************************/

	class MemPool
	{
		static_assert(sizeof(size_t) <= sizeof(void*));

		// 数组长度涵盖所有 2^n 对齐分配长度规则
		std::array<void*, sizeof(void*) * 8> headers;

		// 循环自增版本号( 每次创建时先 ++ 再填充 )
		decltype(MemHeader::versionNumber) versionNumber = 0;

	public:
		MemPool() noexcept;
		~MemPool() noexcept;
		MemPool(MemPool const&) = delete;
		MemPool& operator=(MemPool const&) = delete;

		template<typename MHType = MemHeader>
		void* Alloc(size_t siz) noexcept;

		template<typename MHType = MemHeader>
		void Free(void* const& p) noexcept;

		// 可用于创建 lua state
		void* Realloc(void* const& p, size_t const& newSize, size_t const& dataLen = -1) noexcept;

		// 正常 create 系列

		template<typename T, typename...Args>
		T* Create(Args&&...args) noexcept;

		template<typename T, typename...Args>
		Ptr<T> CreatePtr(Args&&...args) noexcept;

		template<typename T, typename...Args>
		Unique<T> CreateUnique(Args&&...args) noexcept;

		template<typename T, typename...Args>
		T* CreateTo(T*& outPtr, Args&&...args) noexcept;

		template<typename T, typename...Args>
		T* CreateTo(Ptr<T>& outPtr, Args&&...args) noexcept;


		// 首参为 MemPool* 传 this 系列, 省一个参数

		template<typename T, typename...Args>
		T* MPCreate(Args&&...args) noexcept;

		template<typename T, typename...Args>
		Ptr<T> MPCreatePtr(Args&&...args) noexcept;

		template<typename T, typename...Args>
		Unique<T> MPCreateUnique(Args&&...args) noexcept;

		template<typename T, typename...Args>
		bool MPCreateTo(T*& outPtr, Args&&...args) noexcept;

		template<typename T, typename...Args>
		bool MPCreateTo(Ptr<T>& outPtr, Args&&...args) noexcept;


		// 简化一下常用的类型创建. 相当于 mp.MPCreatePtr<xx::String>( args )
		template<typename ...Args>
		Ptr<String> Str(Args&&...args) noexcept;


		void Release(Object* const& o) noexcept;

		/***********************************************************************************/
		// 工具函数

		static size_t Calc2n(size_t const& n) noexcept;

		// 将长度值按 2^n 来对齐
		static size_t Round2n(size_t const& n) noexcept;


		/***********************************************************************************/
		// typeId & 序列化 相关

		// 放置 type 对应的 parent 的 type id. 1 : Object
		inline static std::array<uint16_t, std::numeric_limits<uint16_t>::max()> pids;

		typedef void*(*Creator)(MemPool* const&, BBuffer* const&, size_t const&);

		// 存 typeId 到序列化构造函数的映射
		inline static std::array<Creator, 1 << (sizeof(uint16_t) * 8)> creators;

		// 注册 String, BBuffer 的 typeId 映射( 需要在程序最开始时执行 )
		static void RegisterInternals() noexcept;

		// 注册类型的父子关系. 顺便生成创建函数. Object 不需要注册. T 需要提供相应构造函数 for 反序列化
		template<typename T, typename PT>
		static void Register() noexcept;


		// 根据 typeid 判断父子关系
		static bool IsBaseOf(uint32_t const& baseTypeId, uint32_t typeId) noexcept;

		// 根据 类型 判断父子关系
		template<typename BT>
		static bool IsBaseOf(uint32_t const& typeId) noexcept;

		// 根据 类型 判断父子关系
		template<typename BT, typename T>
		static bool IsBaseOf() noexcept;

		// 试将指针 p 转成 T* 类型. 取代 dynamic_cast
		template<typename T>
		static T* TryCast(Object* const& p) noexcept;

		// BBuffer 序列化 & 反序列化时会用到
		Dict<void*, size_t>* ptrStore;
		Dict<size_t, std::pair<void*, uint16_t>>* idxStore;

		// 域名解析去重会用到
		HashSet<xx::Ptr<xx::String>>* strs;
	};




	/***********************************************************************************/
	// 所有非 MemPool 类的基类
	/***********************************************************************************/

	class Object
	{
	public:
		MemPool * mempool;
		Object(MemPool* const& mempool) noexcept;
		Object(BBuffer* const& bb) noexcept;
		virtual ~Object() noexcept;
		void Release() noexcept;

		// 可通过在 局部变量 备份 versionNumber 的方式, 于某函数调用后 check 目标是否已 Release
		bool IsReleased(decltype(MemHeader_Object::versionNumber) const& vn) const noexcept;

		MemHeader_Object& memHeader() noexcept;
		MemHeader_Object& memHeader() const noexcept;

		virtual void ToString(String& s) const noexcept;
		virtual void ToStringCore(String& s) const noexcept;

		virtual void ToBBuffer(BBuffer& bb) const noexcept;
		virtual int FromBBuffer(BBuffer& bb) noexcept;
	};




	/***********************************************************************************/
	// std::shared_ptr like
	/***********************************************************************************/

	template<typename T>
	class Ref;

	template<typename T>
	class Ptr
	{
	public:
		typedef T ChildType;
		T* pointer;

		Ptr() noexcept;

		template<typename O>
		Ptr(O* const& pointer) noexcept;

		template<typename O>
		Ptr(Ptr<O> const& o) noexcept;

		template<typename O>
		Ptr(Ptr<O>&& o) noexcept;

		Ptr(Ptr<T> const& o) noexcept;
		Ptr(Ptr<T>&& o) noexcept;


		template<typename O>
		Ptr& operator=(O* const& o) noexcept;

		template<typename O>
		Ptr& operator=(Ptr<O> const& o) noexcept;

		template<typename O>
		Ptr& operator=(Ptr<O>&& o) noexcept;

		template<typename O>
		Ptr& operator=(Ref<O> const& o) noexcept;

		Ptr& operator=(Ptr const& o) noexcept;
		Ptr& operator=(Ptr&& o) noexcept;


		// 提供到基类 Ptr 的隐式转换
		template<typename O>
		operator Ptr<O>&() const noexcept;


		// 提供到基类 * 的隐式转换
		template<typename O>
		operator O*() const noexcept;

		template<typename O>
		operator O const*() const noexcept;


		// cleanup
		void Reset() noexcept;

		// std like
		template<typename O>
		void Reset(O* const& o) noexcept;


		// 提供一些快捷读取 memHeader 区域的函数
		decltype(MemHeader_Object::refs) GetRefs() const noexcept;
		decltype(MemHeader_Object::typeId) GetTypeId() const noexcept;


		Ref<T> MakeRef() const noexcept;


		// 比指针硬转科学丁点, 能防手误. 使用 assert 检测实际父子关系
		template<typename O>
		Ptr<O> const& As() const noexcept;
		template<typename O>
		Ptr<O>& As() noexcept;


		// T can be try cast to O ?
		template<typename O>
		bool Is() const noexcept;

		// T is typeId's parent or equals?
		bool Is(uint16_t const& typeId) const noexcept;


		~Ptr() noexcept;

		template<typename O>
		bool operator==(Ptr<O> const& o) const noexcept;
		template<typename O>
		bool operator!=(Ptr<O> const& o) const noexcept;

		template<typename O>
		bool operator==(Ref<O> const& o) const noexcept;
		template<typename O>
		bool operator!=(Ref<O> const& o) const noexcept;


		template<typename O>
		bool operator==(O* const& o) const noexcept;
		template<typename O>
		bool operator!=(O* const& o) const noexcept;


		operator bool() const noexcept;
		// unsafe funcs
		T* operator->() const noexcept;
		T& operator*() const noexcept;


		template<typename O = T, typename...Args>
		Ptr<T>& Create(MemPool* const& mp, Args&&...args) noexcept;

		template<typename O = T, typename...Args>
		Ptr<T>& MPCreate(MemPool* const& mp, Args&&...args) noexcept;
	};

	template<typename T>
	struct IsPtr
	{
		static const bool value = false;
	};

	template<typename T>
	struct IsPtr<Ptr<T>>
	{
		static const bool value = true;
	};

	template<typename T>
	constexpr bool IsPtr_v = IsPtr<T>::value;


	typedef Ptr<Object> Object_p;




	/***********************************************************************************/
	// weak_ptr like ( 只能和 Ptr 搭配 )
	/***********************************************************************************/

	template<typename T>
	class Ref
	{
	public:
		typedef T ChildType;
		T* pointer;
		decltype(MemHeader::versionNumber) versionNumber;

		Ref() noexcept;
		~Ref() noexcept;	// 用于处理值有效但 refs == 0 的对象析构. 常见于反序列化类成员仅含弱引用部分

		//template<typename O>
		//Ref(O* const& o) noexcept;

		//template<typename O>
		//Ref(O* const& o, decltype(MemHeader::versionNumber) versionNumber) noexcept;

		template<typename O>
		Ref(Ptr<O> const& o) noexcept;

		Ref(Ref const& o) noexcept;
		Ref(Ref&& o) noexcept;

		template<typename O>
		Ref(Ref<O> const& o) noexcept;

		template<typename O>
		Ref& operator=(Ptr<O> const& o) noexcept;

		Ref& operator=(Ref const& o) noexcept;

		template<typename O>
		Ref& operator=(Ref<O> const& o) noexcept;

		template<typename O>
		Ref& operator=(O* const& o) noexcept;

		template<typename O>
		bool operator==(O* const& o) const noexcept;
		template<typename O>
		bool operator!=(O* const& o) const noexcept;

		template<typename O>
		bool operator==(Ptr<O> const& o) const noexcept;
		template<typename O>
		bool operator!=(Ptr<O> const& o) const noexcept;

		template<typename O>
		bool operator==(Ref<O> const& o) const noexcept;
		template<typename O>
		bool operator!=(Ref<O> const& o) const noexcept;




		Ptr<T>& Lock() const noexcept;

		void Reset() noexcept;

		operator bool() const noexcept;

		// unsafe funcs
		T* operator->() const noexcept;
		T& operator*() const noexcept;
	};


	template<typename T>
	struct IsRef
	{
		static const bool value = false;
	};

	template<typename T>
	struct IsRef<Ref<T>>
	{
		static const bool value = true;
	};

	template<typename T>
	constexpr bool IsRef_v = IsRef<T>::value;

	typedef Ref<Object> Object_r;













	/***********************************************************************************/
	// std::unique_ptr like
	/***********************************************************************************/

	template<typename T>
	class Weak;

	template<typename T>
	class Unique
	{
	public:
		typedef T ChildType;
		T* pointer;

		Unique() noexcept;

		template<typename O>
		Unique(Unique<O> const& o) = delete;
		Unique(Unique<T> const& o) = delete;

		template<typename O>
		Unique(O* const& pointer) noexcept;

		template<typename O>
		Unique(Unique<O>&& o) noexcept;
		
		Unique(Unique<T>&& o) noexcept;



		template<typename O>
		Unique& operator=(Unique<O> const& o) = delete;
		Unique& operator=(Unique const& o) = delete;


		template<typename O>
		Unique& operator=(O* const& o) noexcept;

		template<typename O>
		Unique& operator=(Unique<O>&& o) noexcept;

		Unique& operator=(Unique&& o) noexcept;



		// 提供到基类 Unique 的隐式转换
		template<typename O>
		operator Unique<O>&() const noexcept;


		// 提供到基类 * 的隐式转换
		template<typename O>
		operator O*() const noexcept;

		template<typename O>
		operator O const*() const noexcept;


		// cleanup
		void Reset() noexcept;

		// std like
		template<typename O>
		void Reset(O* const& o) noexcept;


		// 提供一些快捷读取 memHeader 区域的函数
		decltype(MemHeader_Object::refs) GetRefs() const noexcept;
		decltype(MemHeader_Object::typeId) GetTypeId() const noexcept;


		Weak<T> MakeWeak() const noexcept;


		// 比指针硬转科学丁点, 能防手误. 使用 assert 检测实际父子关系
		template<typename O>
		Unique<O> const& As() const noexcept;
		template<typename O>
		Unique<O>& As() noexcept;


		// T can be try cast to O ?
		template<typename O>
		bool Is() const noexcept;

		// T is typeId's parent or equals?
		bool Is(uint16_t const& typeId) const noexcept;


		~Unique() noexcept;

		template<typename O>
		bool operator==(Unique<O> const& o) const noexcept;
		template<typename O>
		bool operator!=(Unique<O> const& o) const noexcept;

		template<typename O>
		bool operator==(Weak<O> const& o) const noexcept;
		template<typename O>
		bool operator!=(Weak<O> const& o) const noexcept;


		template<typename O>
		bool operator==(O* const& o) const noexcept;
		template<typename O>
		bool operator!=(O* const& o) const noexcept;


		operator bool() const noexcept;
		// unsafe funcs
		T* operator->() const noexcept;
		T& operator*() const noexcept;


		template<typename O = T, typename...Args>
		Unique<T>& Create(MemPool* const& mp, Args&&...args) noexcept;

		template<typename O = T, typename...Args>
		Unique<T>& MPCreate(MemPool* const& mp, Args&&...args) noexcept;
	};

	template<typename T>
	struct IsUnique
	{
		static const bool value = false;
	};

	template<typename T>
	struct IsUnique<Unique<T>>
	{
		static const bool value = true;
	};

	template<typename T>
	constexpr bool IsUnique_v = IsUnique<T>::value;


	typedef Unique<Object> Object_u;













	/***********************************************************************************/
	// weak_ptr like ( 只能和 裸指针/Unique 搭配, 与 Ref 的区别在于这个不析构 )
	/***********************************************************************************/

	template<typename T>
	class Weak
	{
	public:
		typedef T ChildType;
		T* pointer;
		decltype(MemHeader::versionNumber) versionNumber;

		Weak() noexcept;

		template<typename O>
		Weak(O* const& o) noexcept;
		Weak(T* const& o) noexcept;

		template<typename O>
		Weak(O* const& o, decltype(MemHeader::versionNumber) versionNumber) noexcept;

		Weak(Weak const& o) noexcept;
		Weak(Weak&& o) noexcept;

		template<typename O>
		Weak(Weak<O> const& o) noexcept;

		Weak& operator=(Weak const& o) noexcept;

		template<typename O>
		Weak& operator=(Weak<O> const& o) noexcept;

		template<typename O>
		Weak& operator=(O* const& o) noexcept;



		// 比指针硬转科学丁点, 能防手误. 使用 assert 检测实际父子关系
		template<typename O>
		Weak<O> const& As() const noexcept;
		template<typename O>
		Weak<O>& As() noexcept;


		// T can be try cast to O ?
		template<typename O>
		bool Is() const noexcept;

		// T is typeId's parent or equals?
		bool Is(uint16_t const& typeId) const noexcept;



		// 下面所有比较都会先 operator bool() 再进行

		template<typename O>
		bool operator==(Unique<O> const& o) const noexcept;
		template<typename O>
		bool operator!=(Unique<O> const& o) const noexcept;

		template<typename O>
		bool operator==(Weak<O> const& o) const noexcept;
		template<typename O>
		bool operator!=(Weak<O> const& o) const noexcept;

		template<typename O>
		bool operator==(O* const& o) const noexcept;
		template<typename O>
		bool operator!=(O* const& o) const noexcept;

		// pointer & versionNumber 清 0
		void Reset() noexcept;

		// 重要函数: 有效性检测, 如果已失效, 会自动 Reset
		operator bool() const noexcept;

		// unsafe funcs
		T* operator->() const noexcept;
		T& operator*() const noexcept;
	};









	template<typename T>
	struct IsWeak
	{
		static const bool value = false;
	};

	template<typename T>
	struct IsWeak<Weak<T>>
	{
		static const bool value = true;
	};

	template<typename T>
	constexpr bool IsWeak_v = IsWeak<T>::value;


	typedef Weak<Object> Object_w;












	// 增强版, 用于替代 is_trivial 的判断以实现针对 Ptr<> Ref<> 的 memcpy 操作. 未来可以继续在此加料
	template<typename T>
	constexpr bool IsTrivial_v = std::is_trivial<T>::value || IsPtr_v<T> || IsRef_v<T> || IsUnique_v<T> || IsWeak_v<T>;



	// 检查类型是否存在一个 mp* 参数的构造函数
	template <typename T>
	struct CtorTakesMemPool
	{
		template<typename U> static int32_t SFINAE(decltype(U((MemPool*)0))*);
		template<typename U> static int8_t SFINAE(...);

		static const bool value = std::is_base_of<Object, T>::value && sizeof(SFINAE<T>(nullptr)) == sizeof(int32_t);
	};

	template<typename T>
	constexpr bool CtorTakesMemPool_v = CtorTakesMemPool<T>::value;
}
