#pragma once
namespace xx
{

	// 经由 MemPool 分配的内存都带有这样一个头部
	struct MemHeader
	{
		union
		{
			uint64_t versionNumber;		// 自增版本号, 用于野指针判定
			uint8_t bytes[8];
		};

		// 内存池数组 下标, 位于 versionNumber 的最高位字节( 当前不支持大尾机 )
		uint8_t& mpIndex() noexcept;
	};


	template<typename T>
	class Ptr;


	// 粗暴快速的非线程安全内存池
	class MemPool
	{
		static_assert(sizeof(size_t) <= sizeof(void*));

		// 数组长度涵盖所有 2^n 对齐分配长度规则
		std::array<void*, sizeof(void*) * 8> headers;

		// 自增版本号( 每次创建时先 ++ 再填充 )
		uint64_t versionNumber = 0;

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

		template<typename T>
		void Release(T* p) noexcept;

		template<typename T>
		void SafeRelease(T*& p) noexcept;


		static size_t Calc2n(size_t n) noexcept;

		// 将长度值按 2^n 来对齐
		static size_t Round2n(size_t n) noexcept;
	};


	class Object
	{
	public:
		MemPool * mempool;
		Object(MemPool* mempool) noexcept;
		virtual ~Object() noexcept;

		inline MemHeader& memHeader() noexcept;
		inline MemHeader& memHeader() const noexcept;

		inline uint64_t const& versionNumber() const noexcept;
		inline uint64_t pureVersionNumber() const noexcept;

		// todo: more interfaces
	};


	// std::unique_ptr like
	template<typename T>
	class Ptr
	{
	public:
		static_assert(std::is_base_of_v<Object, T>);
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

}
