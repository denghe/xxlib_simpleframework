#ifndef _YVSDK_YVUINFOMANAGER_H_
#define _YVSDK_YVUINFOMANAGER_H_
#include <string>
#include <vector>
#include <map>

#include "YVType/YVType.h"
#include "YVUtils/YVUtil.h"

namespace YVSDK
{
	class YVUInfoManager
	{
	public:
		YVUInfoPtr getUInfoById(uint32 id);
		void updateUInfo(YVUInfoPtr);
	private:
		typedef std::map<uint32, YVUInfoPtr> UInfoMap;
		UInfoMap m_uinfos;
	};
}

#endif