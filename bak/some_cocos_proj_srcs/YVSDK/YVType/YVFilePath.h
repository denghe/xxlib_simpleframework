#ifndef _YVSDK_YVPATH_H_
#define _YVSDK_YVPATH_H_
#include <string>
#include "YVDef.h"
#include "YVUtils/YVUtil.h"

namespace YVSDK
{

	enum YVFileState
	{
		UnkownState ,

		DownLoadingState,                //下载当中
		UpdateLoadingState ,              //上传当中

		DownLoadErroSate ,                //下载失败
		UpdateLoadErrorState,

		OnlyLocalState  ,                  //仅存在本地状态
		OnlyNetWorkState,                //仅存在网络状态

		BothExistState ,                  //网络及本地状态存在
	};

	class _YVFilePath
	{
	public:
		_YVFilePath(const char* path, const char* url);

		std::string& getLocalPath();
		std::string& getUrlPath();

		void setLocalPath(std::string& path);
		void setUrlPath(std::string& url);
		YVFileState getState();
		uint64 getPathId();

		void setState(YVFileState);
	private:
		YVFileState m_state;
		uint64 m_id;
		std::string m_path;
		std::string m_url;
	};

	//申明为知能指针
	WISDOM_PTR(_YVFilePath, YVFilePathPtr);
}
#endif
