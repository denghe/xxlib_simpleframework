#ifndef _YVSDK_YVBEGADDFRIENDRET_H_
#define _YVSDK_YVBEGADDFRIENDRET_H_
#include "YVUtils/YVUtil.h"
#include "YVType/YVUInfo.h"

namespace YVSDK
{

	//成为好友的方式 
	enum YVToFriendWay
	{
		BegAddFriend,        //主动请求添加
		ApplyAddFriend,      //他人请求添加好友

	};

	//加好友的结果
	enum  YVToFriendRet
	{
		RefuseAddFriend,   //拒绝加为好友
		BothAddFriend,     //同意加为好友,双方互为好友
		OnlyAddFriend,     //仅自己加为了好友
	};

	//请求加好友或者被加好友的结果
	class _YVAddFriendRet
	{
	public:
		YVToFriendRet ret;
		YVToFriendWay way;    //来源
		YVUInfoPtr uinfo;     
	};
	WISDOM_PTR(_YVAddFriendRet, YVAddFriendRetPtr);
	
	//请求加好友的通知
	class _YVBegFriendNotify
	{
	public:
		YVUInfoPtr uinfo;
		std::string greet;
	};
	WISDOM_PTR(_YVBegFriendNotify, YVBegFriendNotifyPtr);
}
#endif