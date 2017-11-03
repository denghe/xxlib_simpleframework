#ifndef YunVaSDKUtils_h__
#define YunVaSDKUtils_h__

/************************************************************************/
/* 云娃语音接入单元                                                     */
/************************************************************************/
#include <map>
#include <vector>
#include "YVType/YVType.h"
#include "YVListern/YVListern.h"

namespace YVSDK
{
	/*语音识别类型*/
	enum SpeechLanguageType
	{
		speech_zn_type = 1, //中文
		speech_ct_type = 2, //粤语
		speech_en_type = 3, //英语
	};

	class YVToolManager
	{
	public:
		bool init();
		bool destory();

	
		/*录音时长,单位为秒,默认是60秒*/
		bool setRecordTime(uint32 timeNum);

		/*开始录音*/
		bool startRecord();

		/*结束录音*/
		bool stopRecord();

		/*播放录音*/
		bool playRecord(YVFilePathPtr);

		/*结束播放*/
		bool stopPlay();

		/*设置语音识别类型*/
		bool setSpeechType(SpeechLanguageType type);

		/*语音识别*/
		bool speechVoice(YVFilePathPtr);

		/*请求上传文件*/
		bool upLoadFile(YVFilePathPtr);

		/*请求下载文件*/
		bool downLoadFile(YVFilePathPtr path);

		//设置设备信息
		bool setDeviceInfo(std::string imsi, std::string imei, std::string mac,
			std::string appVersion, std::string networkType);

		InitListern(StopRecord, RecordStopNotify*);
		InitListern(FinishSpeech, SpeechStopRespond*);
		InitListern(FinishPlay, StartPlayVoiceRespond*);
		InitListern(UpLoadFile, YVFilePathPtr);
		InitListern(DownLoadFile, YVFilePathPtr);
		InitListern(RecordVoice, RecordVoiceNotify*);
	public:
		/*停止录音返回*/
		void stopRecordRespond(YaYaRespondBase* respond);

		/*播放录音完成返回*/
		void finishPlayRespond(YaYaRespondBase* respond);

		/*语音识别完成返回*/
		void finishSpeechRespond(YaYaRespondBase* respond);

		/*上传文件请求返回*/
		void upLoadFileRespond(YaYaRespondBase* respond);

		/*下载文件请求返回*/
		void downLoadFileRespond(YaYaRespondBase* respond);

		/*录音音量通知*/
		void recordVoiceRespond(YaYaRespondBase* respond);
	public:
		bool isPlaying(){ return _isPlaying; }
	private:
		void registerMsgCallBack();
		void unRegisterMsgCallBack();

		bool _isSDKInit;  //SDK是否初始化完成
		bool _isLoginSuccess; //CP登录是否成功
		bool _isRecording; //正在录音
		bool _isPlaying;  //是否正在播放
		bool _isUsedSchedule;
		bool _lockInit;

		std::vector<YVFilePathPtr>  _recordingPath;
		std::vector<YVFilePathPtr>  _speechingPath;
		std::vector<YVFilePathPtr>  _downloadingPath;
		std::vector<YVFilePathPtr>  _playingPath;
	};
}
#endif // YunVaSDKUtils_h__

