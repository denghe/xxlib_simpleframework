#include "YVFilePath.h"
#include "YVPlatform.h"

using namespace  YVSDK;

_YVFilePath::_YVFilePath(const char* path, const char* url)
{
	static uint64 sid = 1;
	m_id = sid++;
	if (sid == 0) sid = 1;
	
	if (path != NULL)
	{
		m_path.append(path);
		setState(OnlyLocalState);
	}

	if (url != NULL)
	{
		m_url.append(url);
		
		if (path == NULL)
		{
			std::string urlstr = url;
			std::string fileName;
			unsigned found = urlstr.find_last_of("/");
			//不是一个http协议地址
			assert(found > 0);
			std::string& tempath = YVPlatform::getSingletonPtr()->tempPath;
			fileName.append(tempath);
			fileName.append(urlstr.substr(found + 1));
			m_path.append(fileName);
		}

		setState(OnlyNetWorkState);
	}

	if (url != NULL && path != NULL)
	{
		setState(BothExistState);
	}
}

std::string&  _YVFilePath::getLocalPath()
{
	return m_path;
}

std::string& _YVFilePath::getUrlPath()
{
	return m_url;
}

void _YVFilePath::setLocalPath(std::string& path)
{
	m_path.clear();
	m_path.append(path);

	if (m_state == OnlyNetWorkState)
	{
		setState(BothExistState);
	}
	else
	{
		setState(OnlyLocalState);
	}

}

void _YVFilePath::setUrlPath(std::string& url)
{
	m_url.clear();
	m_url.append(url);

	if (m_state == OnlyLocalState)
	{
		setState(BothExistState);
	}
	else
	{
		setState(OnlyNetWorkState);
	}
}

uint64 _YVFilePath::getPathId()
{
	return m_id;
}

YVFileState _YVFilePath::getState()
{
	return m_state;
}

void _YVFilePath::setState(YVFileState state)
{
	m_state = state;
}