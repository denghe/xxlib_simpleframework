#ifndef _YVSDK_YVLISTERN_H_
#define _YVSDK_YVLISTERN_H_
#include <iostream>
#include <algorithm>
#include <list>
#include "YVType/YVType.h"
#include "YVProtocol/YVProtocol.h"

namespace YVSDK
{
	//========================================================================================
#define InitListern(Name, ArgcType)  \
public:  \
	typedef YVListern::YV##Name##Listern Name##Lister;  \
	void add##Name##Listern(Name##Lister* lister)  \
	{ \
	del##Name##Listern(lister);   \
	m_##Name##listerList.push_back(lister); \
} \
	void del##Name##Listern(Name##Lister* lister)  \
	{  \
	std::list<Name##Lister* >::iterator iter = m_##Name##listerList.begin(); \
	for (;iter != m_##Name##listerList.end(); ++iter) {if ((*iter) == lister)break;} \
	if (iter != m_##Name##listerList.end()) \
	{ \
	m_##Name##listerList.erase(iter); \
} \
} \
	void call##Name##Listern(ArgcType t) \
	{ \
	std::list<Name##Lister* >::iterator _listenerItr = m_##Name##listerList.begin(); \
	while (_listenerItr != m_##Name##listerList.end())  \
	{  \
	Name##Lister* lister = *_listenerItr; \
	++_listenerItr; \
	lister->on##Name##Listern(t); \
}  \
} \
private: \
	std::list<Name##Lister* > m_##Name##listerList;

	//========================================================================================

	class YVListern
	{
	public:

		//CP登录事件 
		class YVCPLoginListern
		{
		public:
			virtual void onCPLoginListern(CPLoginResponce*) = 0;
		};

		//呀呀帐号登录事件
		class YVYYLoginListern
		{
		public:
			virtual void onYYLoginListern(LoginResponse*) = 0;
		};

		//好友列表事件 
		class YVFriendListListern
		{
		public:
			virtual void onFriendListListern(FriendListNotify*) = 0;
		};
		
		//黑名单列表事件
		class YVBlackListListern
		{
		public:
			virtual void onBlackListListern(BlackListNotify*) = 0;
		};
	    
		//添加好友
		class YVAddFriendListern
		{
		public:
			virtual void onAddFriendListern(YVUInfoPtr) = 0;
		};

		//移除好友
		class YVDelFriendListern
		{
		public:
			virtual void onDelFriendListern(YVUInfoPtr) = 0;
		};

		//请求加对方为好友
		class YVBegAddFriendListern
		{
		public:
			virtual void onBegAddFriendListern(YVBegFriendNotifyPtr) = 0;
		};

		//添加好友的结果(一种是主动请求，一种是被请求)
		class YVAddFriendRetListern
		{
		public:
			virtual void onAddFriendRetListern(YVAddFriendRetPtr) = 0;
		};

		//添加黑色单
		class YVAddBlackListern
		{
		public:
			virtual void onAddBlackListern(YVUInfoPtr) = 0;
		};

		//移除黑名单
		class YVDelBlackListern
		{
		public:
			virtual void onDelBlackListern(YVUInfoPtr) = 0;
		};

		//刷新用户信息(可能是好友信息，也有可能是黑名单信息)
		class YVUpdateUserInfoListern
		{
		public:
			virtual void onUpdateUserInfoListern(YVUInfoPtr) = 0;
		};

		//好友的搜索结果
		class YVSearchFriendRetListern
		{
		public:
			virtual void onSearchFriendRetListern(SearchFriendRespond*) = 0;
		};

		//好友的推荐结果
		class YVRecommendFriendRetListern
		{
		public:
			virtual void onRecommendFriendRetListern(RecommendFriendRespond*) = 0;
		};

		//重连事件 
		class YVReConnectListern
		{
		public:
			virtual void onReConnectListern(ReconnectionNotify*) = 0;
		};

		//结束录音事件
		class YVStopRecordListern
		{
		public:
			virtual void onStopRecordListern(RecordStopNotify*) = 0;
		};

		//完成识别事件
		class YVFinishSpeechListern
		{
		public:
			virtual void onFinishSpeechListern(SpeechStopRespond*) = 0;
		};

		//结束播放事件 
		class YVFinishPlayListern
		{
		public:
			virtual void onFinishPlayListern(StartPlayVoiceRespond*) = 0;
		};

		//上传事件 
		class YVUpLoadFileListern
		{
		public:
			virtual void onUpLoadFileListern(YVFilePathPtr) = 0;
		};

		//下载事件 
		class YVDownLoadFileListern
		{
		public:
			virtual void onDownLoadFileListern(YVFilePathPtr) = 0;
		};

		//网络状态事件 
		class YVNetWorkSateListern
		{
		public:
			virtual void onNetWorkSateListern(NetWorkStateNotify*) = 0;
		};

		////录音音量事件
		class YVRecordVoiceListern
		{
		public:
			virtual void onRecordVoiceListern(RecordVoiceNotify*) = 0;
		};

		///频道历史消息事件
		class YVChannelHistoryChatListern
		{
		public:
			virtual void onChannelHistoryChatListern(YVMessageListPtr) = 0;
		};
		
		//收到频道消息事件 
		class YVChannelChatListern
		{
		public:
			virtual void onChannelChatListern(YVMessagePtr) = 0;
		};

		//发送消息频道状态事件 
		class YVChannelChatStateListern
		{
		public:
			virtual void onChannelChatStateListern(YVMessagePtr) = 0;
		};

		//好友历史消息
		class YVFriendHistoryChatListern
		{
		public:
			virtual void onFriendHistoryChatListern(YVMessageListPtr) = 0;
		};

		//好友实时消息
		class YVFriendChatListern
		{
		public:
			virtual void onFriendChatListern(YVMessagePtr) = 0;
		};

		//发送好友消息状态事件 
		class YVFriendChatStateListern
		{
		public:
			virtual void onFriendChatStateListern(YVMessagePtr) = 0;
		};


	};
};
#endif