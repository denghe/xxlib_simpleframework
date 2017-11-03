#ifndef _YVSDK_YVFILEMANAGER_H_
#define _YVSDK_YVFILEMANAGER_H_
#include <string>
#include <vector>
#include <map>

#include "YVType/YVType.h"
#include "YVUtils/YVUtil.h"

namespace YVSDK
{
	class YVFileManager
	{
	public:
		YVFilePathPtr getYVPathByUrl(std::string& url);
		YVFilePathPtr getYVPathByLocal(std::string& path);
		YVFilePathPtr getYVPathById(uint64 id);
		YVFilePathPtr getYVPathByRand(std::string extren);
	private:
		YVFilePathPtr newYVPath(const char* localpath, const char* url);
		typedef std::map<uint64, YVFilePathPtr> FileMap;
		FileMap m_files;
	};
}
#endif