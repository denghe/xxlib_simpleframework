#ifndef _YVSDK_YVSTRING_H_
#define _YVSDK_YVSTRING_H_
#include "YVType/YVDef.h"
#include <string>

namespace YVSDK
{
	std::string toString(uint64 num);
	uint64 toNumber(std::string& str);

}
#endif