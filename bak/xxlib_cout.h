#pragma once
#include "xxlib_string.h"
#include <iostream>

namespace xxlib
{
	// 一组简单的输出函数
	template<typename...TS>
	void Cout(MemPool& mp, TS const&...ps)
	{
		auto str = mp.Create<String>(16384);
		str->Append(ps...);
		std::cout << str->C_str();
		std::flush(std::cout);
		mp.Release(str);
	}

	template<typename...TS>
	void CoutLine(MemPool& mp, TS const&...ps)
	{
		Cout(mp, ps..., '\n');
	}

	template<typename...TS>
	void MPObject::Cout(TS const&...ps)
	{
		::xxlib::Cout(mempool(), ps...);
	}
}

