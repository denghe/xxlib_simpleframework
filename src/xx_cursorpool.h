#pragma once
namespace xx
{
	// 循环利用池. 通常用于管理函数返回 list / buf 之类的, 但有可能并发多个调用该函数
	template<typename T, size_t Size>
	struct CursorPool
	{
		static_assert(Size > 0 && !(Size & (Size - 1)));	// 2^n ensure
		std::array<T, Size> pool;
		size_t cursor = 0;
		// 每执行一次, 以 cursor 为下标返回 pool 中对象的引用. cursor 为循环增加
		T& GetOne()
		{
			auto& rtv = pool[cursor];
			cursor = (cursor + 1) & (Size - 1);
			return rtv;
		}
	};

	/*
	// 示例:

	MP mp;
	CursorPool<xx::List<int>*, 4> p;
	for (auto& o : p.pool)
	{
		o = mp.CreateWithoutTypeId<xx::List<int>>();
	}
	auto list = p.GetOne()
	auto list = p.GetOne()
	auto list = p.GetOne()

	for (auto& o : fp.pool)
	{
		o->Release();
	}

	
	CursorPool<std::vector<xxxx>, 8> p;

	auto& vec = p.GetOne()
	auto& vec = p.GetOne()
	auto& vec = p.GetOne()

	*/

}
