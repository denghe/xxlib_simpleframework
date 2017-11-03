/*********************************************************************************
*Copyright(C), 2015 YUNVA Company
*FileName:  YVProtocol.h
*Author:  元谷
*Version:  1.0.0
*Date:  2015-5-5
*Description: 工具类的实现, 录音，上传下载等
**********************************************************************************/

#include "YVToolManager.h"
#include "YVSDK.h"
#include <assert.h>
using namespace YVSDK;

bool YVToolManager::init()
{
	_isPlaying = false;
	_isSDKInit = false;
	_isUsedSchedule = false;
	_isLoginSuccess = false;
	_isRecording = false;
	_lockInit = false;

	registerMsgCallBack();
	return true;
}

bool YVToolManager::destory()
{
	unRegisterMsgCallBack();
	return true;
}

void YVToolManager::registerMsgCallBack()
{
	YVMsgDispatcher*  msgDispatcher = YVPlatform::getSingletonPtr()->getMsgDispatcher();

	/*绑定相关消息处理*/
	msgDispatcher->registerMsg(IM_RECORD_STOP_RESP, this, &YVToolManager::stopRecordRespond);
	msgDispatcher->registerMsg(IM_RECORD_FINISHPLAY_RESP, this, &YVToolManager::finishPlayRespond);
	msgDispatcher->registerMsg(IM_SPEECH_STOP_RESP, this, &YVToolManager::finishSpeechRespond);
	msgDispatcher->registerMsg(IM_UPLOAD_FILE_RESP, this, &YVToolManager::upLoadFileRespond);
	msgDispatcher->registerMsg(IM_DOWNLOAD_FILE_RESP, this, &YVToolManager::downLoadFileRespond);
	msgDispatcher->registerMsg(IM_RECORD_VOLUME_NOTIFY, this, &YVToolManager::recordVoiceRespond);
}

void YVToolManager::unRegisterMsgCallBack()
{
	YVMsgDispatcher*  msgDispatcher = YVPlatform::getSingletonPtr()->getMsgDispatcher();
	msgDispatcher->unregisterMsg(IM_RECORD_STOP_RESP, this);
	msgDispatcher->unregisterMsg(IM_RECORD_FINISHPLAY_RESP, this);
	msgDispatcher->unregisterMsg(IM_SPEECH_STOP_RESP, this);
	msgDispatcher->unregisterMsg(IM_UPLOAD_FILE_RESP, this);
	msgDispatcher->unregisterMsg(IM_DOWNLOAD_FILE_RESP, this);
	msgDispatcher->unregisterMsg(IM_RECORD_VOLUME_NOTIFY, this);
}

bool YVToolManager::setRecordTime(unsigned int timeNum)
{
	YVPlatform* platform = YVPlatform::getSingletonPtr();
	YVMsgDispatcher* msgDispatcher = platform->getMsgDispatcher();

	SetRecordRequest q;
	q.times = timeNum;

	return msgDispatcher->send(&q);
}

bool YVToolManager::startRecord()
{
	if (_isRecording) return false;
	_isRecording = true;

	//获取一个随机的文件名
	YVFilePathPtr path = YVPlatform::getSingletonPtr()->getYVPathByRand("amr");
	YVPlatform* platform = YVPlatform::getSingletonPtr();
	YVMsgDispatcher* msgDispatcher = platform->getMsgDispatcher();
	StartRecordRequest q;
	q.strFilePath.append(path->getLocalPath());
	//将id记录下来
	uint64 fileId = path->getPathId();
	q.ext.append(toString(fileId));
	_recordingPath.push_back(path);
	return msgDispatcher->send(&q);
}

bool YVToolManager::stopRecord()
{
	if (!_isRecording) return false;
	YVPlatform* platform = YVPlatform::getSingletonPtr();
	YVMsgDispatcher* msgDispatcher = platform->getMsgDispatcher();

	StopRecordRequest q;
	_isRecording = false;

	return msgDispatcher->send(&q);
}

bool YVToolManager::playRecord(YVFilePathPtr path)
{
	if (path == NULL) return false;
	if (_isPlaying) return false;
	_isPlaying = true;

	YVPlatform* platform = YVPlatform::getSingletonPtr();
	YVMsgDispatcher* msgDispatcher = platform->getMsgDispatcher();

	_playingPath.push_back(path);

	StartPlayVoiceRequest q;

	//这个地方，如果是使用url播放的话，底层库要提供接口，将下载好的地址，附上到本地址上。
	q.strfilepath.append(path->getLocalPath());
	q.strUrl.append(path->getUrlPath());
	//将id记录下来
	uint64 fileId = path->getPathId();
	q.ext.append(toString(fileId));
	return msgDispatcher->send(&q);
}

bool YVToolManager::stopPlay()
{
	if (!_isPlaying) return false;
	YVPlatform* platform = YVPlatform::getSingletonPtr();
	YVMsgDispatcher* msgDispatcher = platform->getMsgDispatcher();
	StopPlayVoiceRequest q;
	return msgDispatcher->send(&q);
}

bool YVToolManager::setSpeechType(SpeechLanguageType type)
{
	YVPlatform* platform = YVPlatform::getSingletonPtr();
	YVMsgDispatcher* msgDispatcher = platform->getMsgDispatcher();
	
	SpeechSetRequest q;
	q.language = (yvimspeech_language)type;

	return msgDispatcher->send(&q);
}

bool YVToolManager::speechVoice(YVFilePathPtr path)
{
	YVPlatform* platform = YVPlatform::getSingletonPtr();
	YVMsgDispatcher* msgDispatcher = platform->getMsgDispatcher();

	assert((path->getState() == BothExistState || path->getState() == OnlyLocalState));
	_speechingPath.push_back(path);
	//这里缺少了一个云识别
	SpeechStartRequest q;
	q.strfilepath.append(path->getLocalPath());
	uint64 pathId = path->getPathId();
	//将id记录下来
	q.ext.append(toString(pathId));
	return msgDispatcher->send(&q);
}

bool  YVToolManager::downLoadFile(YVFilePathPtr file)
{
	//检测此文件是否正在下载或者已经缓存至本地
	if (file == NULL || file->getState() == DownLoadingState
		|| file->getState() == OnlyLocalState)
	{
		return true;
	}
	if (file->getUrlPath().length()==0)
		return true;
	//变化状态
	file->setState(DownLoadingState);
	YVPlatform* platform = YVPlatform::getSingletonPtr();
	YVMsgDispatcher* msgDispatcher = platform->getMsgDispatcher();

	DownLoadFileRequest q;
	q.url.append(file->getUrlPath());
	q.filename.append(file->getLocalPath());
	uint64 fileId = file->getPathId();
	q.fileid.append(toString(fileId));

	_downloadingPath.push_back(file);

	return msgDispatcher->send(&q);
}

bool  YVToolManager::setDeviceInfo(std::string imsi, std::string imei, std::string mac,
	std::string appVersion, std::string networkType)
{
	YVPlatform* platform = YVPlatform::getSingletonPtr();
	YVMsgDispatcher* msgDispatcher = platform->getMsgDispatcher();

	SetDeviceInfoReqeust q;
	q.imsi.append(imsi);
	q.imei.append(imei);
	q.mac.append(mac);
	q.appVersion.append(appVersion);
	q.networkType.append(networkType);

	return msgDispatcher->send(&q);
}

void YVToolManager::stopRecordRespond(YaYaRespondBase* respond)
{
	_isRecording = false;
	RecordStopNotify* r = static_cast<RecordStopNotify*>(respond);
	//根据录音的回调获取出录音时候的文件路径
	uint64 id = 0;
	id = toNumber(r->ext);
	
	for (std::vector<YVFilePathPtr>::iterator it = _recordingPath.begin();
		it != _recordingPath.end(); ++it)
	{
		if ((*it)->getPathId() == id)
		{
			r->strfilepath = (*it);
			callStopRecordListern(r);
			_recordingPath.erase(it);
			break;
		}
	}
}

void YVToolManager::finishPlayRespond(YaYaRespondBase* respond)
{
	StartPlayVoiceRespond* r = static_cast<StartPlayVoiceRespond*>(respond);
	_isPlaying = false;
	
	uint64 id = 0;
	id = toNumber(r->ext);
	
	for (std::vector<YVFilePathPtr>::iterator it = _playingPath.begin();
		it != _playingPath.end(); ++it	)
	{
		if ((*it)->getPathId() == id)
		{
			r->filePath = (*it);

			callFinishPlayListern(r);
			_playingPath.erase(it);
			break;
		}
	}
}

void YVToolManager::finishSpeechRespond(YaYaRespondBase* respond)
{
	SpeechStopRespond* r = static_cast<SpeechStopRespond*>(respond);	
	
	uint64 id = 0;
	id = toNumber(r->ext);
	
	for (std::vector<YVFilePathPtr>::iterator it = _speechingPath.begin();
		it != _speechingPath.end(); ++it)
	{
		if ( (*it)->getPathId() == id)
		{
			r->filePath = (*it);
			_speechingPath.erase(it);
			callFinishSpeechListern(r);
			break;
		}
	}
}

void YVToolManager::upLoadFileRespond(YaYaRespondBase* respond)
{
	/*UpLoadFileRespond* r = static_cast<UpLoadFileRespond*>(respond);

	assert (r->fileid.length() <= sizeof(uint64));
	uint64 id = 0;
	memcpy(&id, r->fileid.c_str(), r->fileid.length());
	YVFilePathPtr path = YVFilePath::getPathById(id);
	if (path == NULL) return;
	path->state == r->result == 0 ? BothExistState : UpdateLoadErrorState;
	path->url.clear();
	path->url.append(r->fileid);
	callUpLoadFileListern(path);
	path->release();*/
}

void YVToolManager::downLoadFileRespond(YaYaRespondBase* respond)
{
	DownLoadFileRespond* r = static_cast<DownLoadFileRespond*>(respond);
	uint64 id = 0;
	id = toNumber(r->fileid);
	
	for (std::vector<YVFilePathPtr>::iterator it = _downloadingPath.begin();
		it != _downloadingPath.end(); ++it)
	{
		if ((*it)->getPathId() == id)
		{
			YVFilePathPtr path = (*it);
			if (path == NULL) return;
			path->setState(r->result == 0 ? BothExistState : DownLoadErroSate);
			callDownLoadFileListern(path);

			_downloadingPath.erase(it);
			break;
		}
	}
}

void YVToolManager::recordVoiceRespond(YaYaRespondBase* respond)
{
	RecordVoiceNotify* r = static_cast<RecordVoiceNotify*>(respond);
	callRecordVoiceListern(r);
}
