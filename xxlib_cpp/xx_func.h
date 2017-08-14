#pragma once
#include <type_traits>

namespace xx
{
	// 抄自 zeno 的代码 重构成自己的风格

	template <typename Signature, size_t StorageSize = 64>
	struct Func;

	template <typename R, typename... Args, size_t StorageSize>
	struct Func<R(Args...), StorageSize>
	{
		using FuncType = R(*)(Args...);
		using StorageType = typename std::aligned_storage<StorageSize, sizeof(size_t)>::type;
		using CallFunc = R(*)(void*, FuncType, Args...);
		using AllocFunc = void(*)(void*, void*);

	private:
		union DataType
		{
			StorageType storage;
			FuncType func;
		} data;

		CallFunc call;
		AllocFunc alloc;

	public:
		Func() noexcept
			: call(nullptr)
			, alloc(nullptr)
		{
			data.func = nullptr;
		}

		Func(Func const& o) noexcept
			: call(o.call)
			, alloc(o.alloc)
		{
			data.func = o.data.func;
		}

		template<typename Function>
		Func(Function && f) noexcept
			: Func()
		{
			using UnRefType = typename std::remove_reference<Function>::type;
			static_assert(sizeof(UnRefType) < StorageSize, "StorageSize is too small");
			static_assert(std::is_move_constructible<UnRefType>::value, "should be of movable type");

			call = [](void* f, FuncType, Args...args)->R 
			{
				return static_cast<UnRefType*>(f)->operator()(args...);
			};

			alloc = [](void* p, void* f) 
			{
				if (f)
				{
					new (p) UnRefType(std::move(*static_cast<UnRefType*>(f)));
				}
				else
				{
					static_cast<UnRefType*>(p)->~UnRefType();
				}
			};
			alloc(&data.storage, &f);
		}

		template<typename Ret, typename...Params>
		Func(Ret(*func)(Params...)) noexcept
			: Func()
		{
			data.func = func;
			call = [](void*, FuncType ptr, Args...args)->R
			{
				return static_cast<Ret(*)(Params...)>(ptr)(args...);
			};
		}

		Func(Func && o) noexcept
			: Func()
		{
			MoveFrom(o);
		}

		~Func()
		{
			if (alloc) alloc(&data.storage, nullptr);
		}

		Func& operator=(Func const& o) noexcept
		{
			data.func = o.data.func;
			call = o.call;
			alloc = o.alloc;
			return *this;
		}

		Func& operator=(Func && o) noexcept
		{
			MoveFrom(o);
			return *this;
		}

		Func& operator=(std::nullptr_t) noexcept
		{
			if (alloc) alloc(&data.storage, nullptr);
			data.func = nullptr;
			call = nullptr;
			alloc = nullptr;
			return *this;
		}

		R operator()(Args...args)
		{
			if (!call) throw - 1;
			return call(&data.storage, data.func, args...);
		}

		operator bool() const
		{
			return call != nullptr;
		}

	private:
		void MoveFrom(Func &o) noexcept
		{
			if (this == &o) return;

			if (alloc)
			{
				alloc(&data.storage, nullptr);
				alloc = nullptr;
			}
			else
			{
				data.func = nullptr;
			}

			call = o.call;
			o.call = nullptr;

			if (o.alloc)
			{
				alloc = o.alloc;
				alloc(&data.storage, &o.data.storage);
			}
			else
			{
				data.func = o.data.func;
			}
		}

	};

}
