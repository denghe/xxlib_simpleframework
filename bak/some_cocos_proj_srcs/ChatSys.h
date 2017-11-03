#pragma once

#include <string>


using std::string;

class YVListernImpl;

class ChatSys 
{
    friend class YVListernImpl;
	// no for lua
public:
	ChatSys();
	~ChatSys();

	// for lua manual binding
public:
	// 注册yunva 回调
	void registerToChatEventHandler(LUA_FUNCTION func) {
		_chat_cb = func;
	}

	bool hasCB() { return !!_chat_cb; }
	LUA_FUNCTION getCB() { return _chat_cb; }

private:
	LUA_FUNCTION _chat_cb;
   

private:
    enum CallbackTypes : int
    {
        CT_Login = 1,
        CT_ModifyChannelKey,
        CT_RecvChannelMsg,
        CT_FinishSpeech,
        CT_FinishPlay,
        CT_SendChannelState,
    };

    void loginCB(int err, string err_msg);
    void modifyChannelCB(int err, string err_msg);
    void recvChannelMessageCB(int err, int msg_type, string msg_body, int channel_id, string url);
    void finishSpeechCB(int err, string err_msg, string voice_path, int duration, string txt);
    void finishPlayCB();
    void sendChannelStateCB(int err);

	// for lua auto binding
public:
	MAKE_INSTANCE_H(ChatSys);

    void resetChannelKeys();
    void setGameServerId(string id, bool imm = true);
    void setLegionId(int id, bool imm = true);
    void setStageId(int id, bool imm = true);

	//登陆
	void login_yaya(string tt);
	//添加频道
	void loginChannel();
	//退出聊天系统
	void outIMGame();
	//发送文本消息
	void sendMsg(string str_send_msg, string wildcard);
	//发送语音消息
	void sendVoiceAndText(string voicePath, int voiceDuration, string wildcard, string txt);
	//开始录音
	void soundRecording();
	//完成录音
	void soundRecordingFinished();
	//取消录音
	void soundRecordingStop();
	//播放本地声音
	void playAudioWithFilePath(string filePath);
	//播放网络声音
	void playAudioWithUrl(string url);

    int getChannelId(string key);
    string getChannelKey(int idx);
    void setChannelKey(int idx, string key);

private:
    void updateChannelKeys();

private:
    YVListernImpl *_lsner;
    string         _pgameServiceID;
    vector<string> _channel_keys;
};

