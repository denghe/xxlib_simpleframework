/*********************************************************************************
*Copyright(C), 2015 YUNVA Company
*FileName:  RespondFactory.cpp
*Author:  元谷
*Version:  1.0.0
*Date:  2015-5-5
*Description:  用户管理类
**********************************************************************************/

#include "YVPlayerManager.h"
#include "YVSDK.h"
#include <assert.h>

using namespace YVSDK;

bool YVPlayerManager::init()
{
	m_loginUserInfo = NULL;
	YVMsgDispatcher*  msgDispatcher = YVPlatform::getSingletonPtr()->getMsgDispatcher();
	
	msgDispatcher->registerMsg(IM_LOGIN_RESP, this, &YVPlayerManager::loginResponceCallback);
	msgDispatcher->registerMsg(IM_THIRD_LOGIN_RESP, this, &YVPlayerManager::cpLoginResponce);

	return true;
}

bool YVPlayerManager::destory()
{
	YVMsgDispatcher*  msgDispatcher = YVPlatform::getSingletonPtr()->getMsgDispatcher();

	msgDispatcher->unregisterMsg(IM_LOGIN_RESP, this);
	msgDispatcher->unregisterMsg(IM_THIRD_LOGIN_RESP, this);

	return true;
}

bool YVPlayerManager::cpLogin(std::string ttStr)
{
	YVPlatform* platform = YVPlatform::getSingletonPtr();
	YVMsgDispatcher* msgDispatcher = platform->getMsgDispatcher();

	CPLoginRequest r;
	r.tt.append(ttStr);
	r.wildCard = platform->channelKey;
	r.readstatus = platform->readstatus;
	r.pgameServiceID.append(platform->serverid);

	return msgDispatcher->send(&r);
}

bool YVPlayerManager::yyLogin(int userId, std::string passWord)
{
	YVPlatform* platform = YVPlatform::getSingletonPtr();
	YVMsgDispatcher* msgDispatcher = platform->getMsgDispatcher();

	LoginRequest r;
	r.userid = userId;
	r.pwd = passWord;
	r.wildCard = platform->channelKey;
	r.readstatus = platform->readstatus;
	r.pgameServiceID.append(platform->serverid);

	return msgDispatcher->send(&r);
}

void YVPlayerManager::loginResponceCallback(YaYaRespondBase* respond)
{
	LoginResponse* r = static_cast<LoginResponse*>(respond);
	if (r->result == 0)
	{
		m_loginUserInfo = new _YVUInfo();
		m_loginUserInfo->header = YVPlatform::getSingletonPtr()->getYVPathByUrl(r->iconurl);
		m_loginUserInfo->nickname.append(r->nickname);
		m_loginUserInfo->userid = r->userId;
	}
	callYYLoginListern(r);
}

void YVPlayerManager::cpLoginResponce(YaYaRespondBase* respond)
{
	CPLoginResponce* r = static_cast<CPLoginResponce*>(respond);
	if (r->result == 0)
	{
		m_loginUserInfo = new _YVUInfo(); 
		m_loginUserInfo->header = YVPlatform::getSingletonPtr()->getYVPathByUrl(r->iconUrl);
		m_loginUserInfo->nickname.append(r->thirdUserName);
		m_loginUserInfo->userid = r->userid;
		
		//将用户数据更新到好友信息缓存中;
		YVPlatform* platform = YVPlatform::getSingletonPtr();
		platform->updateUInfo(m_loginUserInfo);
	}
	callCPLoginListern(r);
}

const YVUInfoPtr YVPlayerManager::getLoginUserInfo()
{
	return m_loginUserInfo;
}