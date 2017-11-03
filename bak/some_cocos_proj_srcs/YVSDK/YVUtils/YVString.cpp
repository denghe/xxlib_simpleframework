#include "YVString.h"
#include <sstream>
#include <stdlib.h>
#include <string.h>

namespace YVSDK
{
	std::string toString(uint64 num)
	{
		std::stringstream ss;
		ss << num;
		return ss.str();
	}

	uint64 toNumber(std::string& str)
	{
#if defined(_MSC_VER) && (_MSC_VER <= 1700)
		char* end = NULL;
		uint64 i = static_cast<uint64>(_strtoi64(str.c_str(), &end, 10));
		return i;
#else
		char* end = NULL;
		uint64 i = static_cast<uint64>(strtoll(str.c_str(), &end, 10));
		return i;
#endif
	}
}

