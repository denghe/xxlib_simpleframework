/*********************************************************************************
*Copyright(C), 2015 YUNVA Company
*FileName:  RespondFactory.cpp
*Author:  元谷
*Version:  1.0.0
*Date:  2015-5-5
*Description:  定义协议的调用结构体
**********************************************************************************/

#include "YVRespondFactory.h"
#include "YVProtocol/YVProtocol.h"
namespace YVSDK
{
	RespondFactory* RespondFactory::m_respondFactoryPtr = NULL;

	//云娃帐号登录回应
	AutoRegisterRespond(IM_LOGIN_RESP, LoginResponse);
	//CP帐号登录回应
	AutoRegisterRespond(IM_THIRD_LOGIN_RESP, CPLoginResponce);

	AutoRegisterRespond(IM_FRIEND_ADD_RESP, AddFriendRepond);
	AutoRegisterRespond(IM_FRIEND_ADD_NOTIFY, AddFriendNotify);
	AutoRegisterRespond(IM_FRIEND_ACCEPT_RESP, FriendAcceptRespond);
	//被好友删除通知（好友要求你从他的好友列表里移除）
	AutoRegisterRespond(IM_FRIEND_DEL_NOTIFY, DelFriendNotify);
	AutoRegisterRespond(IM_FRIEND_RECOMMAND_RESP, RecommendFriendRespond);
	AutoRegisterRespond(IM_FRIEND_SEARCH_RESP, SearchFriendRespond);
	AutoRegisterRespond(IM_FRIEND_OPER_RESP, BlackControlRespond);
	//好友删除回应
	AutoRegisterRespond(IM_FRIEND_DEL_RESP, DelFriendRespond);
	//好友列表推送
	AutoRegisterRespond(IM_FRIEND_LIST_NOTIFY, FriendListNotify);
	//黑名单推送
	AutoRegisterRespond(IM_FRIEND_BLACKLIST_NOTIFY, BlackListNotify);
	//好友状态通知
	AutoRegisterRespond(IM_FRIEND_STATUS_NOTIFY, FriendStatusNotify);
	//AutoRegisterRespond(IM_CLOUDMSG_NOTIFY, CloundMsgNotify);
	//好友消息通知
	AutoRegisterRespond(IM_CHAT_FRIEND_NOTIFY, FriendChatNotify);
	//频道消息通知
	AutoRegisterRespond(IM_CHANNEL_MESSAGE_NOTIFY, ChannelMessageNotify);
	//频道历史消息
	AutoRegisterRespond(IM_CHANNEL_HISTORY_MSG_RESP, ChannelHistoryNotify);
	//录音结束地址通知
	AutoRegisterRespond(IM_RECORD_STOP_RESP, RecordStopNotify);
	//播放语音完成	
	AutoRegisterRespond(IM_RECORD_FINISHPLAY_RESP, StartPlayVoiceRespond);
	//停止语音识别回应
	AutoRegisterRespond(IM_SPEECH_STOP_RESP, SpeechStopRespond);
	//下载文件回应
	AutoRegisterRespond(IM_DOWNLOAD_FILE_RESP, DownLoadFileRespond);
	//云消息推送
	AutoRegisterRespond(IM_CLOUDMSG_NOTIFY, CloundMsgNotify);
	//云消息请求返回
	AutoRegisterRespond(IM_CLOUDMSG_LIMIT_RESP, CloundMsgRepond);
	//云消息限制推送返回
	AutoRegisterRespond(IM_CLOUDMSG_LIMIT_NOTIFY, CloundMsgLimitNotify);
	//用户资料信息获取 
	AutoRegisterRespond(IM_USER_GETINFO_RESP, GetUserInfoFriendRespond);
	//重连实现
	AutoRegisterRespond(IM_RECONNECTION_NOTIFY, ReconnectionNotify);
	//频道消息状态
	AutoRegisterRespond(IM_CHANNEL_SENDMSG_RESP, ChannelMessageStateNotify);
	//好友消息状态
	AutoRegisterRespond(IM_CHATT_FRIEND_RESP, FriendMsgStateNotify);
};
