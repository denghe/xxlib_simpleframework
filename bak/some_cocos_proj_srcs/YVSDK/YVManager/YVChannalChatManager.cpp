/*********************************************************************************
*Copyright(C), 2015 YUNVA Company
*FileName:  RespondFactory.cpp
*Author:  元谷
*Version:  1.0.0
*Date:  2015-5-8
*Description:  频道聊天实现类
**********************************************************************************/

#include "YVChannalChatManager.h"
#include "YVUtils/YVMsgDispatcher.h"
#include "YVSDK.h"

#include <assert.h>

using namespace YVSDK;

bool YVChannalChatManager::init()
{
	m_historyCache = new _YVMessageList();
	m_sendMsgCache = new _YVMessageList();
	YVMsgDispatcher*  msgDispatcher = YVPlatform::getSingletonPtr()->getMsgDispatcher();

	//频道消息通知//
	msgDispatcher->registerMsg(IM_CHANNEL_MESSAGE_NOTIFY, this,
		&YVChannalChatManager::channelMessageNotifyCallback);
	
	//频道历史消息通知//
	msgDispatcher->registerMsg(IM_CHANNEL_HISTORY_MSG_RESP, this, 
		&YVChannalChatManager::channelMessageHistoryCallback);

	//消息发送状态通知//
	msgDispatcher->registerMsg(IM_CHANNEL_SENDMSG_RESP, this,
		&YVChannalChatManager::channelMessageStateCallback);

	YVPlatform* platform = YVPlatform::getSingletonPtr();
	platform->addFinishSpeechListern(this);
	return true;
}

bool YVChannalChatManager::destory()
{
	YVMsgDispatcher*  msgDispatcher = YVPlatform::getSingletonPtr()->getMsgDispatcher();

	msgDispatcher->unregisterMsg(IM_CHANNEL_MESSAGE_NOTIFY, this);

	msgDispatcher->unregisterMsg(IM_CHANNEL_HISTORY_MSG_RESP, this);

	msgDispatcher->unregisterMsg(IM_CHANNEL_SENDMSG_RESP, this);

	YVPlatform* platform = YVPlatform::getSingletonPtr();
	platform->delFinishSpeechListern(this);
	return true;
}

void YVChannalChatManager::channelMessageNotifyCallback(YaYaRespondBase* respond)
{
	ChannelMessageNotify* r = static_cast<ChannelMessageNotify*>(respond);

	int id =  getChannelIdByKey(r->wildcard);
	assert(id >= 0);    //协议异常//
	
	//文本消息//
	YVMessagePtr msg = NULL;
	if (r->message_type == chat_msgtype_text)
	{
		msg = new _YVTextMessage(r->message_body);
	}
	else if (r->message_type == chat_msgtype_audio)
	{
		msg = new _YVVoiceMessage(r->message_body, true, r->voiceDuration, r->attach);
	}

	msg->recvId = getChannelIdByKey(r->wildcard);
	msg->sendId = r->user_id;
	
	YVPlatform* platform = YVPlatform::getSingletonPtr();
	YVUInfoPtr uinfo = platform->getUInfoById(msg->sendId);
	if (uinfo == NULL)
	{
		_YVUInfo* info = new _YVUInfo;
		info->userid = r->user_id;
		info->nickname.append(r->nickname);
		uinfo = info;
		platform->updateUInfo(uinfo);
	}

	insertMsg(id, msg);
}

void YVChannalChatManager::channelMessageHistoryCallback(YaYaRespondBase* respond)
{
	ChannelHistoryNotify* r = static_cast<ChannelHistoryNotify*>(respond);

	std::vector<YaYaChannelHistoryMsgInfo>& xHistoryMsg = r->xHistoryMsg;

	m_historyCache->clear();
	YVPlatform* platform = YVPlatform::getSingletonPtr();
	for (std::vector<YaYaChannelHistoryMsgInfo>::iterator it = xHistoryMsg.begin();
		it != xHistoryMsg.end(); ++it)
	{
		YaYaChannelHistoryMsgInfo& info = *it;
		int id = getChannelIdByKey(info.wildcard);
		assert(id >= 0);    //协议异常//
		m_historyCache->setChatWithID(id);
	
		//文本消息//
		YVMessagePtr msg = NULL;
		if (info.message_type == chat_msgtype_text)
		{
			msg = new _YVTextMessage(info.message_body);
		}
		else if (info.message_type == chat_msgtype_audio)
		{
			msg = new _YVVoiceMessage(info.message_body, true, info.voiceDuration, info.attach);
		}
		msg->recvId = getChannelIdByKey(info.wildcard);
		msg->sendId = info.user_id;
		
		YVUInfoPtr uinfo = platform->getUInfoById(msg->sendId);
		if (uinfo == NULL)
		{
			_YVUInfo* _uinfo = new _YVUInfo;
			_uinfo->userid = msg->sendId;
			_uinfo->nickname.append(info.nickname);
			uinfo = _uinfo;
			platform->updateUInfo(uinfo);
		}

		m_historyCache->insertMessage(msg);
	}
	//调用历史消息回调//
	callChannelHistoryChatListern(m_historyCache);
}

void YVChannalChatManager::channelMessageStateCallback(YaYaRespondBase* respond)
{
	ChannelMessageStateNotify* r = static_cast<ChannelMessageStateNotify*>(respond);
	uint64 id = 0;
	id = toNumber(r->expand);
	YVMessagePtr msg = m_sendMsgCache->getMessageById(id);
	if (msg == NULL) return;
	msg->state = r->result == 0 ? YVMessageStatusCreated : YVMessageStatusSendingFailed;
	callChannelChatStateListern(msg);
	m_sendMsgCache->delMessageById(id);
}

bool YVChannalChatManager::loginChannel(std::string &pgameSerivceID, std::vector<std::string> &wildCard)
{
	YVPlatform* platform = YVPlatform::getSingletonPtr();
	YVMsgDispatcher*  msgDispatcher = platform->getMsgDispatcher();

    //构造消息
	ChannelLoginRequest q;
    q.pgameServiceID = pgameSerivceID;
    q.wildCard = wildCard;
	
	//发送消息//
	return msgDispatcher->send(&q);
}

bool YVChannalChatManager::sendChannalText(int channelKeyId, std::string text)
{
	if (text.length() == 0)
	{
		return false;
	}

	YVPlatform* platform = YVPlatform::getSingletonPtr();
	YVMsgDispatcher*  msgDispatcher = platform->getMsgDispatcher();
	//消息存入缓存//
	YVMessagePtr msg = new _YVTextMessage(text);
	msg->recvId = channelKeyId;
	msg->sendId = platform->getLoginUserInfo()->userid;
	msg->state = YVMessageStatusSending;
	msg->sendTime = time(0);
	//缓存消息
	insertMsg(channelKeyId, msg);
	//放至消息发送列表里
	m_sendMsgCache->insertMessage(msg);

	//构造消息
	ChannelTextRequest q;
	q.textMsg = text;
	q.wildCard.append(getChannelKeyById(channelKeyId));
	q.expand.append(toString(msg->id));
	
	//发送消息//
	return msgDispatcher->send(&q);
}

void YVChannalChatManager::insertMsg(int channelKeyId, YVMessagePtr msg, bool isCallChannelChatListern)
{
	ChannelMessageMap::iterator it = m_channelMssages.find(channelKeyId);
	if (it == m_channelMssages.end())
	{
		YVPlatform* platform = YVPlatform::getSingletonPtr();
		YVMessageListPtr messageList = new _YVMessageList();
		messageList->setMaxNum(platform->channelChatCacheNum);
		messageList->setChatWithID((uint32)channelKeyId);
		m_channelMssages.insert(std::make_pair((uint32)channelKeyId, messageList));
		it = m_channelMssages.find((uint32)channelKeyId);
	}
	it->second->insertMessage(msg);
	//调用消息回调//
	if (isCallChannelChatListern)
	{
		callChannelChatListern(msg);
	}
	
}

//录音发送//
bool YVChannalChatManager::sendChannalVoice(int channelKeyId, YVFilePathPtr voicePath,
	uint32 voiceTime)
{
	if (!(voicePath->getState() == OnlyLocalState
		|| voicePath->getState() == BothExistState))
	{
		return false;
	}

	YVPlatform* platform = YVPlatform::getSingletonPtr();
	YVMsgDispatcher*  msgDispatcher = platform->getMsgDispatcher();
	
	//消息存入缓存//
	YVMessagePtr msg = new _YVVoiceMessage(voicePath, voiceTime, "");
	msg->recvId = channelKeyId;
	msg->sendId = platform->getLoginUserInfo()->userid;
	msg->state = YVMessageStatusSending;
	msg->sendTime = time(0);
	//缓存消息
	insertMsg(channelKeyId, msg);
	//放至消息发送列表里
	m_sendMsgCache->insertMessage(msg);

	
	//发送前需要识别语音
	if (platform->speechWhenSend)
	{
		return platform->speechVoice(voicePath);
	}
	else
	{
		ChannelVoiceRequest q;
		q.txt = "";
		q.voiceDurationTime = voiceTime;
		q.wildCard.append(getChannelKeyById(channelKeyId));
		q.voiceFilePath = voicePath->getLocalPath();
		//q.txt = text;
		q.expand.append(toString(msg->id));

		//发送消息//
		return msgDispatcher->send(&q);
	}
}

bool YVChannalChatManager::sendChannalVoice(int channelKeyId, YVFilePathPtr voicePath,
	uint32 voiceTime, std::string text)
{
	if (!(voicePath->getState() == OnlyLocalState
		|| voicePath->getState() == BothExistState))
	{
		return false;
	}

	YVPlatform* platform = YVPlatform::getSingletonPtr();
	YVMsgDispatcher*  msgDispatcher = platform->getMsgDispatcher();
	
	//消息存入缓存//
	YVMessagePtr msg = new _YVVoiceMessage(voicePath, voiceTime, text);
	msg->recvId = channelKeyId;
	msg->sendId = platform->getLoginUserInfo()->userid;
	msg->state = YVMessageStatusSending;
	msg->sendTime = time(0);
	//缓存消息
	insertMsg(channelKeyId, msg);
	//放至消息发送列表里
	m_sendMsgCache->insertMessage(msg);

	
	//发送前需要识别语音
	if (platform->speechWhenSend)
	{
		return platform->speechVoice(voicePath);
	}
	else
	{
		ChannelVoiceRequest q;
		q.txt = "";
		q.voiceDurationTime = voiceTime;
		q.wildCard.append(getChannelKeyById(channelKeyId));
		q.voiceFilePath = voicePath->getLocalPath();
		q.txt = text;
		q.expand.append(toString(msg->id));

		//发送消息//
		return msgDispatcher->send(&q);
	}
}

bool YVChannalChatManager::getChannalHistoryData(int channelKeyId, int index)
{
	YVPlatform* platform = YVPlatform::getSingletonPtr();

	//获取一屏的数量历史消息的数量// 
	uint32  count = platform->channelHistoryChatNum;
	m_historyCache->clear();
	m_historyCache->setMaxNum(count);

	ChannelHistoryRequest q;
	q.count = 0 - count;
	q.index = index;
	q.wildCard.append(getChannelKeyById(channelKeyId));

	YVMsgDispatcher*  msgDispatcher = YVPlatform::getSingletonPtr()->getMsgDispatcher();
	return msgDispatcher->send(&q);
}

std::string& YVChannalChatManager::getChannelKeyById(int id)
{
	YVPlatform* platform = YVPlatform::getSingletonPtr();

	assert(id <  (int)platform->channelKey.size() && id >= 0);
	return  platform->channelKey.at(id);
}

int  YVChannalChatManager::getChannelIdByKey(std::string key)
{
	YVPlatform* platform = YVPlatform::getSingletonPtr();
	int id = 0;
	for (std::vector<std::string>::iterator it = platform->channelKey.begin();
		it != platform->channelKey.end(); ++it)
	{
		if (*it == key)
		{
			return id;
		}
		++id;
	}
	return -1;
}

YVMessageListPtr YVChannalChatManager::getCacheChannalChatData(int channelKeyId)
{
	ChannelMessageMap::iterator it = m_channelMssages.find(channelKeyId);
	if (it == m_channelMssages.end())
	{
		return NULL;
	}
	return (it->second);
}

void YVChannalChatManager::onFinishSpeechListern(SpeechStopRespond* r)
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
		ChannelVoiceRequest q;
		q.txt = "";
		q.voiceDurationTime = voiceMsg->voiceTimes;
		q.wildCard.append(getChannelKeyById(voiceMsg->recvId));
		q.voiceFilePath = r->filePath->getLocalPath();
		q.txt = r->result;
		q.expand.append(toString(msg->id));

		//发送消息//
		YVPlatform* platform = YVPlatform::getSingletonPtr();
		YVMsgDispatcher*  msgDispatcher = platform->getMsgDispatcher();
		msgDispatcher->send(&q);
	}
}