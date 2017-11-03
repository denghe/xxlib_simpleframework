#include "YVFileManager.h"
#include "YVSDK.h"
#include <time.h>
#include <sstream>

using namespace  YVSDK;

YVFilePathPtr YVFileManager::getYVPathByUrl(std::string& url)
{
	for (FileMap::iterator it = m_files.begin();
		it != m_files.end(); ++it)
	{
		if (it->second->getUrlPath() == url)
		{
			return it->second;
		}
	}

	return newYVPath(NULL, url.c_str());
}

YVFilePathPtr YVFileManager::getYVPathByLocal(std::string& path)
{
	for (FileMap::iterator it = m_files.begin();
		it != m_files.end(); ++it)
	{
		if (it->second->getLocalPath() == path)
		{
			return it->second;
		}
	}

	return newYVPath(path.c_str(), NULL);
}

YVFilePathPtr YVFileManager::getYVPathById(uint64 id)
{
	for (FileMap::iterator it = m_files.begin();
		it != m_files.end(); ++it)
	{
		if (it->second->getPathId() == id)
		{
			return it->second;
		}
	}

	return NULL;
}

YVFilePathPtr YVFileManager::newYVPath(const char* localpath, const char* url)
{
	_YVFilePath*  _path = new _YVFilePath(localpath, url);
	YVFilePathPtr path = _path;
	m_files.insert(std::make_pair(path->getPathId(), path));
	return path;
}

YVFilePathPtr YVFileManager::getYVPathByRand(std::string extren)
{

	//用时间戳做文件名
	time_t now;
	int timeStamp = time(&now);
	char timeStr[12] = { 0 };
	sprintf(timeStr, "%d", timeStamp);

	//加入绝对路径
	YVConfigManager* platform = YVPlatform::getSingletonPtr();
	std::string pathStr;
	if (platform->tempPath.length() == 0)
	{
		printf("error:the yvsdk tmppath length is zero");
	}

	pathStr.append(platform->tempPath);
	pathStr.append(timeStr);

	//防冲突措施(客户端应当够用了)
	static  uint32 _id = 0;
	std::stringstream ss;
	ss << "_" << _id ;
	pathStr.append(ss.str());

	//加入文件后缀名
	pathStr.append(".");
	pathStr.append(extren);

	YVFilePathPtr  _path = new _YVFilePath(pathStr.c_str(), NULL);
	m_files.insert(std::make_pair(_path->getPathId(), _path));
	return _path;
}