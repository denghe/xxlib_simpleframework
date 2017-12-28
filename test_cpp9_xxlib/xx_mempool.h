#pragma once
namespace xx
{
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


	/***********************************************************************************/
	// 经由 MemPool 分配的内存都带有这样一个头部
	/***********************************************************************************/

	union MemHeader
	{
		uint64_t data;
		struct
		{
			uint32_t versionNumber;		// 自增版本号, 用于野指针判定
			uint8_t mpIndex;			// 内存池数组 下标
			uint8_t flags;				// 见机使用
			uint16_t typeId;			// 类型编号
		};
	};


	template<typename T>
	class Ptr;
	class BBuffer;
	class Object;

	/***********************************************************************************/
	// 粗暴快速的非线程安全内存池
	/***********************************************************************************/
	
	class MemPool
	{
		static_assert(sizeof(size_t) <= sizeof(void*));

		// 数组长度涵盖所有 2^n 对齐分配长度规则
		std::array<void*, sizeof(void*) * 8> headers;

		// 循环自增版本号( 每次创建时先 ++ 再填充 )
		uint32_t versionNumber = 0;

	public:
		MemPool() noexcept;
		~MemPool() noexcept;
		MemPool(MemPool const&) = delete;
		MemPool& operator=(MemPool const&) = delete;

		void* Alloc(size_t siz) noexcept;

		void Free(void* p) noexcept;

		// 可用于创建 lua state
		void* Realloc(void *p, size_t newSize, size_t dataLen = -1) noexcept;

		template<typename T>
		T* Alloc() noexcept;

		inline void SafeFree(void*& p) noexcept;

		template<typename T, typename ...Args>
		T* Create(Args &&... args);

		template<typename T, typename ...Args>
		Ptr<T> CreatePtr(Args &&... args);

		template<typename T, typename ...Args>
		bool CreateTo(T*& outPtr, Args &&... args);

		//template<typename T, typename ...Args>
		//bool CreateTo(Ref<T>& outPtr, Args &&... args);

		template<typename T, typename ...Args>
		bool CreateTo(Ptr<T>& outPtr, Args &&... args);

		template<typename T>
		void Release(T* p) noexcept;

		template<typename T>
		void SafeRelease(T*& p) noexcept;


		/***********************************************************************************/
		// 工具函数
		/***********************************************************************************/

		static size_t Calc2n(size_t n) noexcept;

		// 将长度值按 2^n 来对齐
		static size_t Round2n(size_t n) noexcept;


		/***********************************************************************************/
		// typeId & 序列化 相关
		/***********************************************************************************/

		// 放置 type 对应的 parent 的 type id. 1 : Object
		inline static std::array<uint16_t, std::numeric_limits<uint16_t>::max()> pids;

		typedef void*(*creator)(MemPool*, BBuffer*, size_t);

		// 存 typeId 到序列化构造函数的映射
		inline static std::array<creator, 1 << (sizeof(uint16_t) * 8)> creators;

		// 注册类型的父子关系. 顺便生成创建函数. Object 不需要注册. T 需要提供相应构造函数 for 反序列化
		template<typename T, typename PT>
		static void Register() noexcept;

		// 根据 typeid 判断父子关系
		static bool IsBaseOf(uint32_t baseTypeId, uint32_t typeId) noexcept;

		// 根据 类型 判断父子关系
		template<typename BT>
		static bool IsBaseOf(uint32_t typeId) noexcept;

		// 根据 类型 判断父子关系
		template<typename BT, typename T>
		static bool IsBaseOf() noexcept;

		// 试将指针 p 转成 T* 类型. 取代 dynamic_cast
		template<typename T>
		static T* TryCast(Object* p) noexcept;

	};



	class Object
	{
	public:
		MemPool * mempool;
		Object(MemPool* mempool) noexcept;
		virtual ~Object() noexcept;

		inline MemHeader& memHeader() noexcept;
		inline MemHeader& memHeader() const noexcept;

		// todo: more interfaces
	};


	// std::unique_ptr like
	template<typename T>
	class Ptr
	{
	public:
		static_assert(std::is_base_of_v<Object, T>);
		typename T ChildType;
		T* pointer;

		Ptr() noexcept;
		Ptr(T* const& pointer) noexcept;
		Ptr(Ptr&& o) noexcept;

		template<typename O>
		Ptr(Ptr<O>&& o) noexcept;

		~Ptr();
		Ptr(Ptr const&) = delete;
		Ptr& operator=(Ptr const&) = delete;

		template<typename O>
		Ptr& operator=(Ptr<O>&& o) noexcept;
		Ptr& operator=(T* const& o) noexcept;

		T const* operator->() const noexcept;
		T* & operator->() noexcept;
		T& operator*() noexcept;
		T const& operator*() const noexcept;

		operator bool() const noexcept;
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

}
