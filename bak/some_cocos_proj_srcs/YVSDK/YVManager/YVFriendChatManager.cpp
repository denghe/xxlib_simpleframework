/*********************************************************************************
*Copyright(C), 2015 YUNVA Company
*FileName:  RespondFactory.cpp
*Author:  元谷
*Version:  1.0.0
*Date:  2015-5-7
*Description:  好友聊天的管理类
**********************************************************************************/

#include "YVFriendChatManager.h"
#include "YVSDK.h"
#include <assert.h>

using namespace YVSDK;
bool YVFriendChatManager::init()
{
	m_sendMsgCache = new _YVMessageList();

	YVMsgDispatcher*  msgDispatcher = YVPlatform::getSingletonPtr()->getMsgDispatcher();
	
	//好友消息返回
	msgDispatcher->registerMsg(IM_CHAT_FRIEND_NOTIFY, this,
		&YVFriendChatManager::friendMessageNotifyCallback);
	
	//云消息拉取
	msgDispatcher->registerMsg(IM_CLOUDMSG_LIMIT_NOTIFY, this, 
		&YVFriendChatManager::cloundMsgNotifyCallback);
	
	//消息发送状态通知//
	msgDispatcher->registerMsg(IM_CHATT_FRIEND_RESP, this,
		&YVFriendChatManager::friendMessageStateCallback);

	YVPlatform* platform = YVPlatform::getSingletonPtr();
	platform->addFinishSpeechListern(this);

	return true;
}

bool YVFriendChatManager::destory()
{
	YVMsgDispatcher*  msgDispatcher = YVPlatform::getSingletonPtr()->getMsgDispatcher();
	
	//好友消息返回
	msgDispatcher->unregisterMsg(IM_CHAT_FRIEND_NOTIFY, this);
	//云消息拉取
	msgDispatcher->unregisterMsg(IM_CLOUDMSG_LIMIT_NOTIFY, this);
	//消息发送状态通知//
	msgDispatcher->unregisterMsg(IM_CHATT_FRIEND_RESP, this);

	YVPlatform* platform = YVPlatform::getSingletonPtr();
	platform->delFinishSpeechListern(this);

	return true;
}

bool YVFriendChatManager::getFriendChatHistoryData(uint32 uid, int index)
{
	//这里候已经拉到顶了，呵呵
	if (index == 1) return true;
	YVPlatform* platform = YVPlatform::getSingletonPtr();
	YVMsgDispatcher* msgDispatcher = platform->getMsgDispatcher();

	FriendChatMessageMap::iterator it = m_historyCache.find(uid);
	if (it != m_historyCache.end())
	{
		it->second->clear();
	}
	CloundMsgRequest q;
	q.limit = platform->friendHistoryChatNum;
	q.end = index == 0? index: index -1;
	q.source = CLOUDMSG_FRIEND;
	q.id = uid;     

	return msgDispatcher->send(&q);
}

void YVFriendChatManager::friendMessageNotifyCallback(YaYaRespondBase* respond)
{
	FriendChatNotify* r = static_cast<FriendChatNotify*>(respond);
	YVMessagePtr msg = NULL;
	switch (r->type)
	{
	case chat_msgtype_text:
		msg = new _YVTextMessage(r->data);
		break;
	case chat_msgtype_audio:
		msg = new _YVVoiceMessage(r->data, true, r->audiotime, r->attach);
		break;
	case chat_msgtype_image:
		break;
	default:
		break;
	}

	YVPlatform* platform = YVPlatform::getSingletonPtr();
	msg->state = YVMessageStatusUnread;
	msg->recvId = platform->getLoginUserInfo()->userid;
	msg->sendId = r->userid;
	msg->sendTime = r->sendtime;
	msg->index = r->index;
	insertMessage(r->userid, msg);


// 	YVUInfoPtr uinfo =  platform->getUInfoById(r->userid);
// 	if (uinfo == NULL)
// 	{
// 		uinfo = new _YVUInfo();
// 		uinfo->header = platform->getYVPathByUrl(r->headicon);
// 		uinfo->nickname.append(r->name);
// 		uinfo->userid = r->userid;
// 
// 		platform->updateUInfo(uinfo);
// 	}
	
}

bool YVFriendChatManager::sendFriendText(uint32 chatWithUid, std::string text)
{
	YVPlatform* platform = YVPlatform::getSingletonPtr();
	YVMsgDispatcher*  msgDispatcher = platform->getMsgDispatcher();
	
	//消息存入缓存//
	YVMessagePtr msg = new _YVTextMessage(text);
	msg->recvId = chatWithUid;
	msg->sendId = platform->getLoginUserInfo()->userid;
	msg->state = YVMessageStatusSending;
	msg->sendTime = time(0);
	m_sendMsgCache->insertMessage(msg);
	//缓存消息
	insertMessage(chatWithUid, msg);

	//发送消息
	FriendTextChatRequest q;
	q.userid = chatWithUid;
	q.data = text;
	uint64 msgID = msg->id;
	q.flag.append(toString(msgID));
	
	return msgDispatcher->send(&q);
}

bool YVFriendChatManager::sendFriendVoice(uint32 chatWithUid, YVFilePathPtr voicePath,
	uint32 voiceTime)
{
	YVPlatform* platform = YVPlatform::getSingletonPtr();
	YVMsgDispatcher*  msgDispatcher = platform->getMsgDispatcher();

	//消息存入缓存//
	YVMessagePtr msg = new _YVVoiceMessage(voicePath, voiceTime, "");
	msg->recvId = chatWithUid;
	msg->sendId = platform->getLoginUserInfo()->userid;
	msg->state = YVMessageStatusSending;
	msg->sendTime = time(0);
	//缓存消息
	insertMessage(chatWithUid, msg);
	//放至消息发送列表里
	m_sendMsgCache->insertMessage(msg);


	//发送前需要识别语音
	if (platform->speechWhenSend)
	{
		return platform->speechVoice(voicePath);
	}
	else
	{
		FriendVoiceChatRequest q;
		q.userid = chatWithUid;

		q.file = voicePath->getLocalPath();
		q.time = voiceTime;
		//q.txt = text;
		uint64 msgID = msg->id;
		q.flag.append(toString(msgID));
		
		return msgDispatcher->send(&q);
	}

}

bool YVFriendChatManager::sendFriendImage(uint32 chatWithUid, YVFilePathPtr path)
{
	YVPlatform* platform = YVPlatform::getSingletonPtr();
	YVMsgDispatcher* msgDispatcher = platform->getMsgDispatcher();


	////消息存入缓存//
	//YVMessageBase* msg = new YVImageMessage(text);
	//msg->recvId = chatWithUid;
	//msg->sendId = playerManager->getUserInfo()->userid;
	//msg->state = YVMessageStatusSending;
	////缓存消息
	//insertMessage(chatWithUid, msg);


	FriendImageChatRequest q;
	q.userid = chatWithUid;
	q.image = path->getLocalPath();
	
	return msgDispatcher->send(&q);
}

void YVFriendChatManager::cloundMsgNotifyCallback(YaYaRespondBase* respond)
{
	CloundMsgLimitNotify* cmsg = static_cast<CloundMsgLimitNotify*>(respond);
	if (cmsg->packet == NULL)
	{
		return;
	}
	//当为聊天消息时//
	if (cmsg->source == CLOUDMSG_FRIEND)
	{
		YaYaP2PChatNotify* p2pmsg = static_cast<YaYaP2PChatNotify*> (cmsg->packet);
		
		YVMessagePtr msg = NULL;
		switch (p2pmsg->type)
		{
		case chat_msgtype_text:
			msg = new _YVTextMessage(p2pmsg->data);
			break;
		case chat_msgtype_audio:
			msg = new _YVVoiceMessage(p2pmsg->data, true, p2pmsg->audiotime, p2pmsg->attach);
			break;
		case chat_msgtype_image:
			break;
		default:
			break;
		}

		YVPlatform* platform = YVPlatform::getSingletonPtr();
		msg->sendTime = p2pmsg->sendtime;
		msg->state = p2pmsg->unread == 0? YVMessageStatusUnread:YVMessageStatusCreated;
		if (platform->getLoginUserInfo()->userid == p2pmsg->userid)
		{
			msg->sendId = p2pmsg->userid;
			msg->recvId = cmsg->id;
		}
		else
		{
			msg->sendId = cmsg->id;
			msg->recvId = platform->getLoginUserInfo()->userid;
		}
		msg->index = cmsg->indexId;
		insertMessage(cmsg->id, msg ,false);
	
		//上屏回调
		FriendChatMessageMap::iterator it = m_historyCache.find(cmsg->id);
		if (it == m_historyCache.end())
		{
			YVMessageListPtr msgList = new _YVMessageList();	
			msgList->setChatWithID(cmsg->id);
			m_historyCache.insert(std::make_pair(cmsg->id, msgList));
			it = m_historyCache.find(cmsg->id);
		}
		it->second->insertMessage(msg);
		//收完全了数据，上屏显示回调
		uint32 buffSize = it->second->getMessageList().size();
		if (buffSize == cmsg->count)
		{
			callFriendHistoryChatListern(it->second);
			it->second->clear();
		}
	}
}

void YVFriendChatManager::friendMessageStateCallback(YaYaRespondBase* respond)
{
	FriendMsgStateNotify* r = static_cast<FriendMsgStateNotify*>(respond);
	uint64 id = 0;
	id = toNumber(r->flag);
	YVMessagePtr msg = m_sendMsgCache->getMessageById(id);
	assert (msg != NULL);
	if (msg == NULL) return;
	msg->state = r->result == 0 ? YVMessageStatusCreated : YVMessageStatusSendingFailed;
	callFriendChatStateListern(msg);
	m_sendMsgCache->delMessageById(id);
}

void YVFriendChatManager::insertMessage(uint32 chatWithId, YVMessagePtr messageBase, bool isCallFriendChatListern)
{
	if (messageBase == NULL) return;
	YVPlatform* platform = YVPlatform::getSingletonPtr();
	FriendChatMessageMap::iterator it = m_friendChatMap.find(chatWithId);
	if (it == m_friendChatMap.end())
	{
		YVPlatform* platform = YVPlatform::getSingletonPtr();
		YVMessageListPtr msgList = new _YVMessageList();
		msgList->setChatWithID(chatWithId);
		msgList->setMaxNum(platform->friendChatCacheNum);
		m_friendChatMap.insert(std::make_pair(chatWithId, msgList));
		it = m_friendChatMap.find(chatWithId);
	}
	it->second->insertMessage(messageBase);
	if (isCallFriendChatListern)
	{
		callFriendChatListern(messageBase);
	}
	
}

YVMessageListPtr YVFriendChatManager::getFriendChatListById(uint32 uid)
{
	FriendChatMessageMap::iterator it = m_friendChatMap.find(uid);
	if (it != m_friendChatMap.end())
	{
		return (it->second);
	}
	return NULL;
}

void YVFriendChatManager::onFinishSpeechListern(SpeechStopRespond* r)
{
	std::vector<YVMessagePtr>& sendingmsgs = m_sendMsgCache->getMessageList();
	for (std::vector<YVMessagePtr>::iterator it = sendingmsgs.begin();
		it != sendingmsgs.end(); ++it)
	{
		YVMessagePtr msg = *it;
		if (msg->type != YVMessageTypeAudio) continue;
		YVVoiceMessagePtr voiceMsg = msg;
		if (voiceMsg->voicePath != r->filePath) continue;

		voiceMsg->attach.clear();
		voiceMsg->attach.append(r->result);

		FriendVoiceChatRequest q;
		q.userid = voiceMsg->recvId;

		q.file = r->filePath->getLocalPath();
		q.time = voiceMsg->voiceTimes;
		q.txt = r->result;
		
		uint64 msgID = msg->id;
		q.flag.append(toString(msgID));


		//发送消息//
		YVPlatform* platform = YVPlatform::getSingletonPtr();
		YVMsgDispatcher*  msgDispatcher = platform->getMsgDispatcher();
		msgDispatcher->send(&q);
	}
}