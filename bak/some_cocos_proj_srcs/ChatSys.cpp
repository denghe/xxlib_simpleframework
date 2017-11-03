#include "Precompile.h"
#include "ChatSys.h"


#include "YVSDK.h"

using YVSDK::YVListern;
using YVSDK::YVPlatform;
using YVSDK::CPLoginResponce;
using YVSDK::ReconnectionNotify;
using YVSDK::RecordStopNotify;
using YVSDK::SpeechStopRespond;
using YVSDK::StartPlayVoiceRespond;
using YVSDK::UpLoadFileRespond;
using YVSDK::DownLoadFileRespond;
using YVSDK::RecordVoiceNotify;
using YVSDK::YVFilePathPtr;
using YVSDK::YVMessagePtr;
using YVSDK::YVTextMessagePtr;
using YVSDK::YVImageMessagePtr;
using YVSDK::YVVoiceMessagePtr;

#define sYVPlatform (*(YVPlatform::getSingletonPtr()))
#define RECORD_MAX_TIME (30)

struct VoiceMsg
{
    YVSDK::YVFilePathPtr vfn;
    YVSDK::uint32 time;
    string msg;
};

class YVListernImpl
: public YVListern::YVCPLoginListern
, public YVListern::YVStopRecordListern
, public YVListern::YVFinishPlayListern
, public YVListern::YVFinishSpeechListern
, public YVListern::YVChannelChatListern
, public YVListern::YVChannelChatStateListern
{
    friend class ChatSys;

public:
    YVListernImpl()
    : _canceled_record(false)
    {
	    sYVPlatform.addFinishPlayListern(this);
	    sYVPlatform.addFinishSpeechListern(this);
	    sYVPlatform.addCPLoginListern(this);
	    sYVPlatform.addStopRecordListern(this);
        sYVPlatform.addChannelChatStateListern(this);
        sYVPlatform.addChannelChatListern(this);
    }

    ~YVListernImpl()
    {
	    sYVPlatform.delFinishPlayListern(this);
	    sYVPlatform.delFinishSpeechListern(this);
	    sYVPlatform.delCPLoginListern(this);
	    sYVPlatform.delStopRecordListern(this);
        sYVPlatform.delChannelChatStateListern(this);
        sYVPlatform.delChannelChatListern(this);
    }

    virtual void onCPLoginListern           (CPLoginResponce*);
	virtual void onStopRecordListern        (RecordStopNotify*);
	virtual void onFinishSpeechListern      (SpeechStopRespond*);
	virtual void onFinishPlayListern        (StartPlayVoiceRespond*);
	virtual void onChannelChatListern       (YVMessagePtr);
	virtual void onChannelChatStateListern  (YVMessagePtr);

private:
    bool _canceled_record;
    map<string, VoiceMsg> _cachedMsg;
};


void YVListernImpl::onCPLoginListern(CPLoginResponce* r )
{
    CCLOG("CPLogin: %u, %s", r->result, r->msg.c_str());

    if (r->result == 0) sYVPlatform.setRecordTime( RECORD_MAX_TIME );

    ChatSys::getInstance()->loginCB(r->result, r->msg);
}

void YVListernImpl::onStopRecordListern(RecordStopNotify* r)
{
    CCLOG("onStopRecordListern: %u, path: %s", r->time ,r->strfilepath->getLocalPath().c_str());

    if (_canceled_record) return;

    VoiceMsg msg = {r->strfilepath, r->time, ""};

    _cachedMsg.emplace(make_pair(r->strfilepath->getLocalPath(), msg));

    CCLOG("Start to speech voice");
    sYVPlatform.speechVoice(r->strfilepath);
}

void YVListernImpl::onFinishSpeechListern(SpeechStopRespond* r)
{
    CCLOG("onFinishSpeechListern: %u, %s", r->err_id ,r->err_msg.c_str());
    CCLOG("onFinishSpeechListern: %s", r->result.c_str());

    auto it = _cachedMsg.find(r->filePath->getLocalPath());
    if (it == _cachedMsg.end()) return;

    if (r->err_id == 0) (*it).second.msg = r->result;

    ChatSys::getInstance()->finishSpeechCB(r->err_id, 
                                           r->err_msg, 
                                           r->filePath->getLocalPath(), 
                                           (*it).second.time,
                                           r->result);
}

void YVListernImpl::onFinishPlayListern(StartPlayVoiceRespond* r)
{
    CCLOG("<Finish Play>");
    ChatSys::getInstance()->finishPlayCB();
}

void YVListernImpl::onChannelChatListern(YVMessagePtr msg)
{
    switch (msg->type)
    {
    case YVSDK::YVMessageTypeText:
        {
            YVTextMessagePtr tm = msg;
            char *out = 0;
            base64Decode((uint8_t *)tm->text.c_str(), (uint32_t)tm->text.size(), (uint8_t **)&out);
            ChatSys::getInstance()->recvChannelMessageCB(tm->state, 
                                       tm->type,
                                       out,
                                       tm->recvId,
                                       "");
            ::free(out);
        }
        break;
    case YVSDK::YVMessageTypeAudio:
        {
            YVVoiceMessagePtr vm = msg;
            auto url = vm->voicePath->getUrlPath();
            if (url.empty()) url = vm->voicePath->getLocalPath();

            char *out = 0;
            base64Decode((uint8_t *)vm->attach.c_str(), (uint32_t)vm->attach.size(), (uint8_t **)&out);
            ChatSys::getInstance()->recvChannelMessageCB(vm->state, 
                                                         vm->type, 
                                                         out, 
                                                         vm->recvId,
                                                         url);
            ::free(out);
        }
        break;
    default:
        return;
    }
}

void YVListernImpl::onChannelChatStateListern(YVMessagePtr msg)
{
    ChatSys::getInstance()->sendChannelStateCB(msg->state);
}


MAKE_INSTANCE_CPP(ChatSys);

ChatSys::ChatSys()
: _chat_cb(0)
, _lsner(nullptr)
{
    YVPlatform::createSingleton();

    Director::getInstance()->getScheduler()->schedule([](float dt) {
        sYVPlatform.updateSdk(dt);
    }, this, 0, false, "YunvaSDK");

    _lsner = new YVListernImpl();

    // 定位到 FileUtils 实例以方便使用
    auto &fs = *FileUtils::getInstance();
    // 取可写目录
    auto waPath = fs.getWritablePath();
    waPath += "chat";
    // 确保可写目录的存在
    Utils::ensureDirectory( waPath );
    // 标记该目录不备份到 iCloud
    Utils::fileSkipBackup( waPath );

    sYVPlatform.setConfig(YVSDK::ConfigTempPath, waPath.c_str());

    sYVPlatform.setConfig(YVSDK::ConfigAppid, 1000012);
    sYVPlatform.setConfig(YVSDK::ConfigIsTest, false);
    sYVPlatform.setConfig(YVSDK::ConfigSpeechWhenSend, false);

    sYVPlatform.init();

    resetChannelKeys();
}


ChatSys::~ChatSys()
{
    delete _lsner;

    sYVPlatform.destory();

    YVPlatform::destorySingleton();
}

void ChatSys::resetChannelKeys()
{
    _channel_keys.clear();
    _channel_keys.push_back("world");
    _channel_keys.push_back("no_stage");
    _channel_keys.push_back("no_legion");
    this->updateChannelKeys();
}

void ChatSys::setGameServerId(string id, bool imm)
{
    _pgameServiceID = id;
    sYVPlatform.setConfig(YVSDK::ConfigServerId, id.c_str());

    if (imm) loginChannel();
}

void ChatSys::setLegionId(int id, bool imm)
{
    if (id)
    {
        char buf[64];
        sprintf(buf, "legion_%u", id);
        _channel_keys[2] = buf;
    }
    else
    {
        _channel_keys[2] = "no_legion";
    }

    this->updateChannelKeys();
    if (imm) loginChannel();
}

void ChatSys::setStageId(int id, bool imm)
{
    if (id)
    {
        char buf[64];
        sprintf(buf, "stage_%u", id);
        _channel_keys[1] = buf;
    }
    else
    {
        _channel_keys[1] = "no_stage";
    }

    this->updateChannelKeys();
    if (imm) loginChannel();
}


void ChatSys::login_yaya(string tt)
{
    sYVPlatform.cpLogin(tt);
}

//添加频道
void ChatSys::loginChannel()
{
    sYVPlatform.loginChannel(_pgameServiceID, _channel_keys);
}

//退出聊天系统
void ChatSys::outIMGame()
{

}

//发送文本消息
void ChatSys::sendMsg(string str_send_msg, string wildcard)
{
    int cid = getChannelId(wildcard);
    if (cid < 0) return;

    char *out=0;
    base64Encode((uint8_t *)str_send_msg.c_str(), (uint32_t)str_send_msg.size() + 1, &out);
    sYVPlatform.sendChannalText(cid, out);
    ::free(out);
}

//发送语音消息
void ChatSys::sendVoiceAndText(string voicePath, int voiceDuration, string wildcard, string txt)
{
    char *out=0;
    base64Encode((uint8_t *)txt.c_str(), (uint32_t)txt.size() + 1, &out);
    sYVPlatform.sendChannalVoice(getChannelId(wildcard),
        new YVSDK::_YVFilePath(voicePath.c_str(), 0), voiceDuration, out);
    ::free(out);
}

//开始录音
void ChatSys::soundRecording()
{
    _lsner->_canceled_record = false;
    sYVPlatform.startRecord();
}

//完成录音
void ChatSys::soundRecordingFinished()
{
    sYVPlatform.stopRecord();
}

//取消录音
void ChatSys::soundRecordingStop()
{
    _lsner->_canceled_record = true;
    sYVPlatform.stopRecord();
}

//播放本地声音
void ChatSys::playAudioWithFilePath(string filePath)
{
    sYVPlatform.playRecord(new YVSDK::_YVFilePath(filePath.c_str(), nullptr));
}

//播放网络声音
void ChatSys::playAudioWithUrl(string url)
{
    sYVPlatform.playRecord(new YVSDK::_YVFilePath(nullptr, url.c_str()));
}

void ChatSys::loginCB(int err, string err_msg)
{
	if (!hasCB()) return;
    LuaHelper::push(CallbackTypes::CT_Login);
	LuaHelper::push(err);
	LuaHelper::push(err_msg);
	LuaHelper::executeFunction(this->getCB(), 3);
}

void ChatSys::modifyChannelCB(int err, string err_msg)
{
	if (!hasCB()) return;
    LuaHelper::push(CallbackTypes::CT_ModifyChannelKey);
	LuaHelper::push(err);
	LuaHelper::push(err_msg);
	LuaHelper::executeFunction(this->getCB(), 3);
}

void ChatSys::recvChannelMessageCB(int err, 
                                   int msg_type,
	                               string msg_body, 
                                   int channel_id,
                                   string url)
{
	if (!hasCB()) return;
    LuaHelper::push(CallbackTypes::CT_RecvChannelMsg);
	LuaHelper::push(err);
	LuaHelper::push(msg_type);
	LuaHelper::push(msg_body);
    LuaHelper::push(getChannelKey(channel_id));
	LuaHelper::push(url);
	LuaHelper::executeFunction(this->getCB(), 6);
}

void ChatSys::finishSpeechCB(int err, string err_msg, string voice_path, int duration, string txt)
{
	if (!hasCB()) return;

    LuaHelper::push(CallbackTypes::CT_FinishSpeech);
	LuaHelper::push(err);
	LuaHelper::push(err_msg);
	LuaHelper::push(voice_path);
	LuaHelper::push(duration);
	LuaHelper::push(txt);
	LuaHelper::executeFunction(this->getCB(), 6);
}

void ChatSys::finishPlayCB()
{
	if (!hasCB()) return;
    LuaHelper::push(CallbackTypes::CT_FinishPlay);
	LuaHelper::executeFunction(this->getCB(), 1);
}

void ChatSys::sendChannelStateCB(int err)
{
	if (!hasCB()) return;
    LuaHelper::push(CallbackTypes::CT_SendChannelState);
	LuaHelper::push(err);
	LuaHelper::executeFunction(this->getCB(), 2);
}

int ChatSys::getChannelId(string key)
{
    for (int i=0; i<(int)_channel_keys.size(); i++)
    {
        if (_channel_keys[i] == key) return i;
    }

    CCLOG("ChatSys::getChannelId, Not Registered Key: %s", key.c_str());
    return -1;
}

string ChatSys::getChannelKey(int id)
{
    if (id >=0 && id <= (int)_channel_keys.size())
        return _channel_keys[id];

    CCLOG("ChatSys::getChannelKey, Invalid ChannelId: %d", id);
    return "";
}

void ChatSys::setChannelKey(int id, string key)
{
    if (id >=0 && id <= (int)_channel_keys.size())
        _channel_keys[id] = key;
}

void ChatSys::updateChannelKeys()
{
    sYVPlatform.clearChannelKeys();
    for (auto &it : _channel_keys)
    {
        sYVPlatform.setConfig(YVSDK::ConfigChannelKey, it.c_str(), nullptr);
    }
}
