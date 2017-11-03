#ifndef __FRIENDMANAGER_H_
#define __FRIENDMANAGER_H_

#include <string>
#include <vector>
#include "YVType/YVType.h"
#include "YVListern/YVListern.h"

namespace YVSDK
{
	struct YaYaRespondBase;
	class YVContactManager
	{
	public:
		bool init();
		bool destory();
		//请求加好友//
		bool addFriend(uint32 uid, std::string greet = "");
		//删除好友isRmoveSelf是否要求对方也删除你//
		bool delFriend(uint32 uid, bool isRmoveSelf = true);
		//拉黑好友//
		bool addBlack(uint32 uid);
		//移出黑名表//
		bool delBlack(uint32 uid);
		//同意加好友//
		bool agreeFriend(uint32 uid);
		//拒绝加好友//
		bool opposeFriend(uint32 uid);
		//查找好友//
		bool searchFiend(std::string& keyWord, int start, int count);
		//获取推荐好友//
		bool recommendFriend(int start, int count);
		//从网络上拉取获取用户的信息//
		bool getUserInfoSync(uint32 uid);

		YVUInfoPtr getFriendInfo(uint32 uid);
		YVUInfoPtr getBlackInfo(uint32 uid);

		YVUInfoMap& getAllFriendInfo();
		YVUInfoMap& getAllBlackInfo();
	public:
		void friendsNotifyCallback(YaYaRespondBase*);
		void blacksNotifyCallback(YaYaRespondBase*);
		void nearsNotifyCallback(YaYaRespondBase*);
		void addFriendCallback(YaYaRespondBase*);
		void delFriendCallback(YaYaRespondBase*);
		void blackControlCallback(YaYaRespondBase*);
		//主动同意加好友//
		void addFriendRetCallback(YaYaRespondBase*);
		void searchResponceCallback(YaYaRespondBase*);
		void recommendNotifyCallback(YaYaRespondBase*);
		//获取好友资料返回//
		void getUserInfoCallBack(YaYaRespondBase*);
		//好友删除你//
		void delFriendNotify(YaYaRespondBase*);
		//好友状态 //
		void friendStatusNotify(YaYaRespondBase*);
		//请求加好友//
		void begAddFriendNotify(YaYaRespondBase*);

	public:
		InitListern(AddFriend, YVUInfoPtr);
		InitListern(DelFriend, YVUInfoPtr);
		InitListern(DelBlack, YVUInfoPtr);
		InitListern(AddBlack, YVUInfoPtr);
		InitListern(UpdateUserInfo, YVUInfoPtr);
		InitListern(RecommendFriendRet, RecommendFriendRespond*);
		InitListern(SearchFriendRet, SearchFriendRespond*);
		
		InitListern(BegAddFriend, YVBegFriendNotifyPtr);
		InitListern(AddFriendRet, YVAddFriendRetPtr);

		InitListern(FriendList, FriendListNotify*);
		InitListern(BlackList, BlackListNotify*);

	private:
		bool addFriendInfo(YVUInfoPtr info);
		bool delFriendInfo(uint32 uid);
		bool addBlackInfo(YVUInfoPtr info);
		bool delBlackInfo(uint32 uid);

		void delAllBlackInfo();
		void delAllFriendInfo();

		YVUInfoMap m_friendInfos;
		YVUInfoMap m_blackInfos;
		YVUInfoMap m_nearInfos;
	};
}
#endif