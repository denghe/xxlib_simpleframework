#ifndef _YVSDK_YVUINFO_H_
#define _YVSDK_YVUINFO_H_
#include <map>
#include <string>
#include "../YVUtils/YVUtil.h"
#include "YVFilePath.h"

namespace YVSDK
{
	enum YVRelation
	{
		FriendRelation,    //好友关系
		BlackRelation,     //黑名单
		StrangerRelation,  //陌生人
	};

	//用户扩展信息类型
	enum  YVUExtInfoType
	{
		
	};

	//用户信息类实现
	class _YVUInfo
	{
	public:
		uint32   userid;        //用户id
		std::string nickname;   //用户昵称
		YVFilePathPtr header;       //用户图像地址
	private:
		std::map<YVUExtInfoType, std::string> m_uinfos;   //这里存除基本信息的扩展信息
	};

	//申明为知能指针
	WISDOM_PTR(_YVUInfo, YVUInfoPtr);

	typedef  std::map<uint32, YVUInfoPtr> YVUInfoMap;

}
#endif