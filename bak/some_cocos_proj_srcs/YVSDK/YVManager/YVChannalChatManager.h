#ifndef _CHANNALCHATMANAGER_H_
#define _CHANNALCHATMANAGER_H_

#include <vector>
#include "YVListern/YVListern.h"
#include "YVType/YVType.h"

namespace YVSDK
{
#define CHANNALLINITMSGNUM 10

	struct YaYaRespondBase;

	class YVChannalChatManager
		:public YVListern::YVFinishSpeechListern
	{
	public:
		bool init();
		bool destory();

        //登录频道
        bool loginChannel(std::string &pgameServiceID, std::vector<std::string> &wildCard);
		
		//发送文本消息
		bool sendChannalText(int channelKeyId, std::string text);
		
		//发送语音消息
		bool sendChannalVoice(int channelKeyId, YVFilePathPtr voicePath,
			uint32 voiceTime);
		bool sendChannalVoice(int channelKeyId, YVFilePathPtr voicePath,
			uint32 voiceTime, std::string text);

		//获取历史消息
		bool getChannalHistoryData(int channelKeyId, int index);

		//获取内存中的数据。
		YVMessageListPtr getCacheChannalChatData(int channelKeyId);

		void channelMessageNotifyCallback(YaYaRespondBase*);
		void channelMessageHistoryCallback(YaYaRespondBase*);
		void channelMessageStateCallback(YaYaRespondBase*);
		
		InitListern(ChannelHistoryChat, YVMessageListPtr);
		InitListern(ChannelChat, YVMessagePtr);
		InitListern(ChannelChatState, YVMessagePtr);

	protected:
		//结束识别接口
		void onFinishSpeechListern(SpeechStopRespond*);
		std::string& getChannelKeyById(int id);
		int  getChannelIdByKey(std::string key);
		void insertMsg(int channelKeyId, YVMessagePtr, bool isCallChannelChatListern = true);
		
		ChannelMessageMap m_channelMssages;           //频道消息备份
		YVMessageListPtr m_historyCache;            //频道历史消息缓存
		YVMessageListPtr m_sendMsgCache;            //发送中的消息备份
	};
}
#endif