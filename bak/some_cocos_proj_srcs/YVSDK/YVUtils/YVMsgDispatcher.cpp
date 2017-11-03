/*********************************************************************************
*Copyright(C), 2015 YUNVA Company
*FileName:  YVProtocol.h
*Author:  元谷
*Version:  1.0.0
*Date:  2015-5-5
*Description: 事件消息调度类实现
**********************************************************************************/

#include "YVProtocol/YVProtocol.h"
#include "YVMsgDispatcher.h"
#include "YVRespondFactory.h"
#include "YVSyncQueue.h"
#include "YVSDK.h"
//#include "cocos2d.h"
using namespace YVSDK;

YVMsgDispatcher* YVMsgDispatcher::s_msgDispatcher = NULL;
YaYaMsgMap* YVMsgDispatcher::m_msgCallBackMap = NULL;

//线程之间传递的消息结构
struct MsgData
{
	unsigned int cmdid;
	YVSDK::YaYaRespondBase* r;
};

static CYVSyncQueue<MsgData> s_msgQueue;

static  void _stdcall YaYaStaticCallBack(CmdChannel type, unsigned int cmdid, YV_PARSER parser, unsigned long context)
{
	MsgData data;
	data.cmdid = cmdid;
	data.r = RespondFactory::getSingletonPtr()->getRepsond(cmdid);
	if (data.r == NULL)
	{
		printf("RespondFactory not find: 0x%x \n", cmdid);
		return;
	}
	//CCLOG("RespondFactory not find: 0x%x \n", cmdid);
	data.r->decode(parser);
	s_msgQueue.AddElement(data);
}

YVMsgDispatcher::YVMsgDispatcher()
{
    //: Shrimps
    //  加入断言检查，限制 YVMsgDispatcher 只能创建一次
    assert(!s_msgDispatcher && !m_msgCallBackMap);
    //.

	_isSDKInit = false;
	m_msgCallBackMap = new YaYaMsgMap();
};

YVMsgDispatcher::~YVMsgDispatcher()
{
	for (YaYaMsgMap::iterator it = m_msgCallBackMap->begin(); it != m_msgCallBackMap->end(); ++it)
	{
		for (YaYaMsgCallFuncList::iterator its = it->second.begin(); its != it->second.end(); ++its)
		{
			delete *its;
		}
		it->second.clear();
	}
	m_msgCallBackMap->clear();
	delete m_msgCallBackMap;
    m_msgCallBackMap = nullptr;
};

void YVMsgDispatcher::dispatchMsg(float dt)
{
	static MsgData msgData;
	if (!s_msgQueue.PopElement(msgData))
	{
		return;
	}
	unsigned int cmdid = msgData.cmdid;
	YaYaMsgMap* msgCallBackMap = YVMsgDispatcher::m_msgCallBackMap;
	YaYaMsgMap::iterator it = msgCallBackMap->find(cmdid);

	if (it != msgCallBackMap->end())
	{
		printf("CallBackFunc: 0x%x \n", cmdid);
		YaYaMsgCallFuncList& funcList = it->second;
		for (YaYaMsgCallFuncList::iterator its = funcList.begin();
			its != funcList.end(); ++its)
		{
			(*its)->call(msgData.r);
		}
	}
	else
	{
		printf("CallBackFunc not find: 0x%x \n", cmdid);
	}
	msgData.r->release();
}

bool YVMsgDispatcher::send(YaYaRequestBase* request)
{
	if (!(_isSDKInit || initSDK()))
	{
		printf("YVSDK not Init.");
		return false;
	}

	auto ret = YVIM_SendCmd(request->m_requestChannel, request->m_requestCmd, request->encode());

	if (ret == 0)
	{
		printf("YVIM_SendCmd: 0x%x Success.\n", request->m_requestCmd);
		return true;
	}

	printf("YVIM_SendCmd: 0x%xFail.\n", request->m_requestCmd);
	return false;
}

bool YVMsgDispatcher::initSDK()
{
	YVPlatform* platform  = YVPlatform::getSingletonPtr();

	assert(platform->appid != 0);
	assert(platform->tempPath.length() > 0);

	//我发现这个东西很耗时，如果初使化时候调用怕会让游戏启动时间加长
	int ret = YVIM_Init(YaYaStaticCallBack, 
		0, 
		platform->appid,
		platform->tempPath.c_str(),
		platform->isTest);
	if (ret != 0)
	{
		return false;
	}
	_isSDKInit = true;
	return true;
}

void YVMsgDispatcher::releseSDK()
{
	if (_isSDKInit)
	{
		YVIM_Release();
		_isSDKInit = false;
	}
}