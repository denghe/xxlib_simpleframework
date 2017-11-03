#ifndef _YVSDK_YVPLATFORM_H_
#define _YVSDK_YVPLATFORM_H_
#include "YVManager/YVManager.h"


namespace YVSDK
{
	class YVMsgDispatcher;
	
	class YVPlatform
		:public YVUInfoManager,
		public YVFileManager,
		public YVConfigManager,
		public YVPlayerManager,
		public YVContactManager,
		public YVChannalChatManager,
		public YVToolManager,
		public YVFriendChatManager
	{
	public:
		static YVPlatform* getSingletonPtr();
		static void createSingleton();
		static void destorySingleton();

        ~YVPlatform();
		bool init();
		bool destory();

		//请使用定时器调用它，建议每帧调用一次
		void updateSdk(float dt);
		//获取事件派发器
		YVMsgDispatcher* getMsgDispatcher();
	private:
		YVPlatform();

		YVMsgDispatcher* m_msgDispatcher;
		static YVPlatform* s_YVPlatformPtr;
		bool _isInit;
	};
}
#endif