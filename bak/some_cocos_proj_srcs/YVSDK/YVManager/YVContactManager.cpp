/*********************************************************************************
*Copyright(C), 2015 YUNVA Company
*FileName:  YVProtocol.h
*Author:  元谷
*Version:  1.0.3
*Date:  2015-5-7
*Description:  联系人管理（好友黑名单）
**********************************************************************************/

#include "YVContactManager.h"
#include "YVSDK.h"

using namespace YVSDK;

bool YVContactManager::init()
{
	YVMsgDispatcher*  msgDispatcher = YVPlatform::getSingletonPtr()->getMsgDispatcher();

	//好友列表
	msgDispatcher->registerMsg(IM_FRIEND_LIST_NOTIFY, this, &YVContactManager::friendsNotifyCallback);
	//黑名单列表
	msgDispatcher->registerMsg(IM_FRIEND_BLACKLIST_NOTIFY, this, &YVContactManager::blacksNotifyCallback);
	//最近联系人列表
	msgDispatcher->registerMsg(IM_FRIEND_NEARLIST_NOTIFY, this, &YVContactManager::nearsNotifyCallback);
	//添加好友成功回应
	msgDispatcher->registerMsg(IM_FRIEND_ADD_RESP, this, &YVContactManager::addFriendCallback);
	//删除好友回应
	msgDispatcher->registerMsg(IM_FRIEND_DEL_RESP, this, &YVContactManager::delFriendCallback);
	//黑名单操作
	msgDispatcher->registerMsg(IM_FRIEND_OPER_RESP, this, &YVContactManager::blackControlCallback);
	//好友添加结果
	msgDispatcher->registerMsg(IM_FRIEND_ACCEPT_RESP, this, &YVContactManager::addFriendRetCallback);
	//搜索好友结果
	msgDispatcher->registerMsg(IM_FRIEND_SEARCH_RESP, this, &YVContactManager::searchResponceCallback);
	//推荐好结果
	msgDispatcher->registerMsg(IM_FRIEND_RECOMMAND_RESP, this, &YVContactManager::recommendNotifyCallback);
	//查看用户信息
	msgDispatcher->registerMsg(IM_USER_GETINFO_RESP, this, &YVContactManager::getUserInfoCallBack);
	//被好友删除了
	msgDispatcher->registerMsg(IM_FRIEND_DEL_NOTIFY, this, &YVContactManager::delFriendNotify);
	//好友状态推送
	msgDispatcher->registerMsg(IM_FRIEND_STATUS_NOTIFY, this, &YVContactManager::friendStatusNotify);
	//请求加好友通知;
	msgDispatcher->registerMsg(IM_FRIEND_ADD_NOTIFY, this, &YVContactManager::begAddFriendNotify);
	return true;
}

bool YVContactManager::destory()
{
	YVMsgDispatcher*  msgDispatcher = YVPlatform::getSingletonPtr()->getMsgDispatcher();
	//好友列表
	msgDispatcher->unregisterMsg(IM_FRIEND_LIST_NOTIFY, this);
	//黑名单列表
	msgDispatcher->unregisterMsg(IM_FRIEND_BLACKLIST_NOTIFY, this);
	//最近联系人列表
	msgDispatcher->unregisterMsg(IM_FRIEND_NEARLIST_NOTIFY, this);
	//添加好友成功回应
	msgDispatcher->unregisterMsg(IM_FRIEND_ADD_RESP, this);
	//删除好友回应
	msgDispatcher->unregisterMsg(IM_FRIEND_DEL_RESP, this);
	//黑名单操作
	msgDispatcher->unregisterMsg(IM_FRIEND_OPER_RESP, this);
	//别人加好友后，你的同意或者拒绝
	msgDispatcher->unregisterMsg(IM_FRIEND_ACCEPT_RESP, this);
	//搜索好友结果
	msgDispatcher->unregisterMsg(IM_FRIEND_SEARCH_RESP, this);
	//推荐好结果
	msgDispatcher->unregisterMsg(IM_FRIEND_RECOMMAND_RESP, this);
	//查看用户信息
	msgDispatcher->unregisterMsg(IM_USER_GETINFO_RESP, this);
	//被好友删除了
	msgDispatcher->unregisterMsg(IM_FRIEND_DEL_NOTIFY, this);
	//好友状态推送
	msgDispatcher->unregisterMsg(IM_FRIEND_STATUS_NOTIFY, this);
	//请求好友通知
	msgDispatcher->unregisterMsg(IM_FRIEND_ADD_NOTIFY, this);
	//内存清理
	delAllBlackInfo();
	delAllFriendInfo();
	return true;
}

bool YVContactManager::addFriend(uint32 uid, std::string greet)
{
	YVPlatform* platform = YVPlatform::getSingletonPtr();
	YVMsgDispatcher* msgDispatcher = platform->getMsgDispatcher();

	if (platform->getLoginUserInfo()->userid == uid)
	{
		return false;
	}

	AddFriendRequest r;
	r.userid = uid;
	r.greet = greet;
	return msgDispatcher->send(&r);
}

bool YVContactManager::delFriend(uint32 uid, bool isRmoveSelf)
{
	YVPlatform* platform = YVPlatform::getSingletonPtr();
	YVMsgDispatcher* msgDispatcher = platform->getMsgDispatcher();

	DelFriendRequest r;
	r.del_friend = uid;
	r.act = isRmoveSelf ? df_remove_from_list : df_exit_in_list;
	return  msgDispatcher->send(&r);
}

bool YVContactManager::addBlack(uint32 uid)
{
	YVPlatform* platform = YVPlatform::getSingletonPtr();
	YVMsgDispatcher* msgDispatcher = platform->getMsgDispatcher();

	
	BlackControlRequest r;
	r.operId = uid;
	r.userId = platform->getLoginUserInfo()->userid;
	r.act = oper_add_blacklist;

	return 	msgDispatcher->send(&r);
}

bool YVContactManager::delBlack(uint32 uid)
{
	YVPlatform* platform = YVPlatform::getSingletonPtr();
	YVMsgDispatcher* msgDispatcher = platform->getMsgDispatcher();


	BlackControlRequest r;
	r.operId = uid;
	r.userId = platform->getLoginUserInfo()->userid;
	r.act = oper_del_blacklist;

	return 	msgDispatcher->send(&r);
}

bool YVContactManager::opposeFriend(uint32 uid)
{
	YVPlatform* platform = YVPlatform::getSingletonPtr();
	YVMsgDispatcher* msgDispatcher = platform->getMsgDispatcher();

	FriendAcceptRequest q;
	q.affirm = af_refuse;
	q.userid = uid;

	return 	msgDispatcher->send(&q);
}

bool YVContactManager::agreeFriend(uint32 uid)
{
	YVPlatform* platform = YVPlatform::getSingletonPtr();
	YVMsgDispatcher* msgDispatcher = platform->getMsgDispatcher();
	
	FriendAcceptRequest q;
	q.affirm = af_agree_add;
	q.userid = uid;
	
	return 	msgDispatcher->send(&q);
}

bool YVContactManager::getUserInfoSync(uint32 uid)
{
	YVPlatform* platform = YVPlatform::getSingletonPtr();
	YVMsgDispatcher* msgDispatcher = platform->getMsgDispatcher();

	GetUserInfoRequest q;
	q.userid = uid;

	return 	msgDispatcher->send(&q);
}

bool YVContactManager::searchFiend(std::string& keyWord, int start, int count)
{
	YVPlatform* platform = YVPlatform::getSingletonPtr();
	YVMsgDispatcher* msgDispatcher = platform->getMsgDispatcher();

	SearchFriendRequest r;
	r.count = count;
	r.start = start;
	r.keyword.append(keyWord);

	return 	msgDispatcher->send(&r);
}

bool YVContactManager::recommendFriend(int start, int count)
{
	YVPlatform* platform = YVPlatform::getSingletonPtr();
	YVMsgDispatcher* msgDispatcher = platform->getMsgDispatcher();

	RecommendFriendRequest r;
	r.count = count;
	r.start = start;

	return 	msgDispatcher->send(&r);
}

void YVContactManager::friendsNotifyCallback(YaYaRespondBase* request)
{
	FriendListNotify* r = (FriendListNotify*)request;
	for (std::vector<YaYaUserInfo*>::iterator it = r->userInfos.begin();
		it != r->userInfos.end(); ++it)
	{
		YaYaUserInfo* _netUserInfo = (*it);   //从网络层接收到的用户信息
		
		YVUInfoPtr userInfo = new _YVUInfo();
		userInfo->userid = _netUserInfo->userid;
		userInfo->nickname.append(_netUserInfo->nickname);
		
		userInfo->header = YVPlatform::getSingletonPtr()->getYVPathByUrl(_netUserInfo->iconurl);
		/*userInfo->online = _netUserInfo->online;
		userInfo->userlevel.append(_netUserInfo->userlevel);
		userInfo->viplevel.append(_netUserInfo->viplevel);
		userInfo->shieldmsg = _netUserInfo->shieldmsg;
		userInfo->sex = _netUserInfo->sex;
		userInfo->ext.append(_netUserInfo->ext);
		userInfo->group.append(_netUserInfo->group);
		userInfo->note.append(_netUserInfo->note);*/
		
		addFriendInfo(userInfo);
	}
	callFriendListListern(r);
}

void YVContactManager::blacksNotifyCallback(YaYaRespondBase* request)
{
	//m_blackInfos.clear();
	BlackListNotify* r = (BlackListNotify*)request;

	for (std::vector<YaYaUserInfo*>::iterator it = r->userInfos.begin();
		it != r->userInfos.end(); ++it)
	{
		YaYaUserInfo* _netUserInfo = (*it);   //从网络层接收到的用户信息

		YVUInfoPtr userInfo = new _YVUInfo();
		userInfo->userid = _netUserInfo->userid;
		userInfo->nickname.append(_netUserInfo->nickname);

		userInfo->header = YVPlatform::getSingletonPtr()->getYVPathByUrl(_netUserInfo->iconurl);
		/*userInfo->headerPath = YVFilePath::getUrlPath(_netUserInfo->iconurl);
		userInfo->online = _netUserInfo->online;
		userInfo->userlevel.append(_netUserInfo->userlevel);
		userInfo->viplevel.append(_netUserInfo->viplevel);
		userInfo->shieldmsg = _netUserInfo->shieldmsg;
		userInfo->sex = _netUserInfo->sex;
		userInfo->ext.append(_netUserInfo->ext);
		userInfo->group.append(_netUserInfo->group);
		userInfo->note.append(_netUserInfo->note);*/

		addBlackInfo(userInfo);
	}
	callBlackListListern(r);
}

void YVContactManager::nearsNotifyCallback(YaYaRespondBase* respond)
{
	m_nearInfos.clear();
}

void YVContactManager::addFriendRetCallback(YaYaRespondBase* respond)
{
	FriendAcceptRespond* r = (FriendAcceptRespond*)respond;
	YVAddFriendRetPtr ret = new _YVAddFriendRet;
	ret->way = ApplyAddFriend;  //主动请求加的好友

	if (r->affirm == af_agree || r->affirm == af_agree_add)
	{
		//判断是否是黑名单成员//
		YVUInfoPtr userInfo = getBlackInfo(r->userid);
		if (userInfo != NULL)
		{
			addFriendInfo(userInfo);
			delBlackInfo(r->userid);
		}
		else
		{
			//没有好友数据，网络请求之;
			userInfo = new _YVUInfo();
			userInfo->userid = r->userid;
			addFriendInfo(userInfo);
			getUserInfoSync(r->userid);
		}
		ret->ret = (r->affirm == af_agree_add ? BothAddFriend : OnlyAddFriend);
		ret->uinfo = userInfo;
	}
	else
	{
		YVUInfoPtr userInfo = new _YVUInfo();
		userInfo->userid = r->userid;
		getUserInfoSync(r->userid);
		ret->ret = RefuseAddFriend;
		ret->uinfo = userInfo;
	}
	callAddFriendRetListern(ret);
}

void YVContactManager::addFriendCallback(YaYaRespondBase* respond)
{
	AddFriendRepond* r = (AddFriendRepond*)respond;
	YVAddFriendRetPtr ret = new _YVAddFriendRet;
	ret->way = BegAddFriend;
	YVUInfoPtr userInfo = getBlackInfo(r->userid);
	if (userInfo == NULL)
	{
		userInfo = new _YVUInfo();
		userInfo->userid = r->userid;
		userInfo->nickname = r->name;
		if (r->url.length() > 0)
		{
			userInfo->header = YVPlatform::getSingletonPtr()->getYVPathByUrl(r->url);
		}
		getUserInfoSync(r->userid);
	}

	//拒绝加好友
	if (r->affirm == af_refuse)
	{
		ret->ret = RefuseAddFriend;
	}
	else
	{
		ret->ret = (r->affirm == af_agree_add) ? BothAddFriend : OnlyAddFriend;
		addFriendInfo(userInfo);
		callAddFriendListern(userInfo);
		delBlackInfo(r->userid);
	}
	ret->uinfo = userInfo;
	callAddFriendRetListern(ret);
}

void YVContactManager::delFriendCallback(YaYaRespondBase* request)
{
	DelFriendRespond* r = (DelFriendRespond*)request;
	delFriendInfo(r->del_friend);
}

void YVContactManager::blackControlCallback(YaYaRespondBase* request)
{
	BlackControlRespond* r = (BlackControlRespond*)request;

	if (r->act == oper_add_blacklist)
	{
		//查找好友列表中的数据
		YVUInfoPtr userinfo = getFriendInfo(r->operId);
		if (userinfo != NULL)
		{
			addBlackInfo(userinfo);
			delFriendInfo(r->operId);
		}
		else
		{
			//好友列表里没有黑名单信息
			userinfo = new _YVUInfo();;
			userinfo->userid = r->operId;
			//userinfo->online = r->oper_state;
			addBlackInfo(userinfo);
			//获取黑名单信息
			getUserInfoSync(r->operId);
		}
	}
	else if (r->act == oper_del_blacklist)
	{
		delBlackInfo(r->operId);
	}
}

void YVContactManager::searchResponceCallback(YaYaRespondBase* respond)
{
	SearchFriendRespond* r = static_cast<SearchFriendRespond*>(respond);
	callSearchFriendRetListern(r);
}

void YVContactManager::recommendNotifyCallback(YaYaRespondBase* respond)
{
	RecommendFriendRespond* r = static_cast<RecommendFriendRespond*>(respond);
	callRecommendFriendRetListern(r);
}

void YVContactManager::begAddFriendNotify(YaYaRespondBase* respond)
{
	AddFriendNotify* r = static_cast<AddFriendNotify*>(respond);
	YVUInfoPtr uinfo = new _YVUInfo;
	if (r->url.length() > 0)
	{
		uinfo->header = YVPlatform::getSingletonPtr()->getYVPathByUrl(r->url);
	}
	uinfo->nickname.append(r->name);
	uinfo->userid = r->userid;
	YVBegFriendNotifyPtr notify = new _YVBegFriendNotify;
	notify->greet.append(r->greet);
	notify->uinfo = uinfo;
	callBegAddFriendListern(notify);
}

bool YVContactManager::addFriendInfo(YVUInfoPtr info)
{
	YVUInfoMap::iterator it = m_friendInfos.find(info->userid);
	if (it != m_friendInfos.end())
	{
		return false;
	}
	YVPlatform::getSingletonPtr()->updateUInfo(info);
	m_friendInfos.insert(std::make_pair(info->userid, info));
	return true;
}

bool YVContactManager::delFriendInfo(uint32 uid)
{
	YVUInfoMap::iterator it = m_friendInfos.find(uid);
	if (it == m_friendInfos.end())
	{
		return false;
	}
	YVUInfoPtr info =  it->second;
	m_friendInfos.erase(it);
	callDelFriendListern(info);
	return true;
}

bool YVContactManager::addBlackInfo(YVUInfoPtr info)
{
	if (info == NULL) return false;
	YVPlatform::getSingletonPtr()->updateUInfo(info);
	YVUInfoMap::iterator it = m_blackInfos.find(info->userid);
	if (it != m_blackInfos.end())
	{
		return false;
	}
	m_blackInfos.insert(std::make_pair(info->userid, info));	
	callAddBlackListern(info);
	return true;
}

bool YVContactManager::delBlackInfo(uint32 uid)
{
	YVUInfoMap::iterator it = m_blackInfos.find(uid);
	if (it == m_blackInfos.end())
	{
		return false;
	}
	YVUInfoPtr info = it->second;
	m_blackInfos.erase(it);
	callDelBlackListern(info);
	return true;
}

YVUInfoPtr YVContactManager::getFriendInfo(uint32 uid)
{
	YVUInfoMap::iterator it = m_friendInfos.find(uid);
	if (it == m_friendInfos.end())
	{
		return NULL;
	}
	return it->second;
}

YVUInfoPtr YVContactManager::getBlackInfo(uint32 uid)
{
	YVUInfoMap::iterator it = m_blackInfos.find(uid);
	if (it == m_blackInfos.end())
	{
		return NULL;
	}
	return it->second;
}

//获取好友资料返回
void YVContactManager::getUserInfoCallBack(YaYaRespondBase* respond)
{
	GetUserInfoFriendRespond* r = static_cast<GetUserInfoFriendRespond*>(respond);
	
	//更新黑名单数据
	YVUInfoPtr blackInfo = getBlackInfo(r->userid);
	if (blackInfo != NULL)
	{
		
		blackInfo->nickname = r->nickname;
		blackInfo->header = YVPlatform::getSingletonPtr()->getYVPathByUrl(r->headicon);
		/*blackInfo->userlevel = r->userlevel;
		blackInfo->viplevel = r->viplevel;
		blackInfo->sex = r->sex;*/
		//更新数据
		callUpdateUserInfoListern(blackInfo);
		return;
	}

	//更新好友数据
	YVUInfoPtr friendInfo = getFriendInfo(r->userid);
	if (friendInfo != NULL)
	{
		
		friendInfo->nickname = r->nickname;
		friendInfo->header = YVPlatform::getSingletonPtr()->getYVPathByUrl(r->headicon);
		/*friendInfo->sex = r->sex;
		if (friendInfo->headerPath) friendInfo->headerPath->release();
		friendInfo->headerPath = YVFilePath::getUrlPath(r->headicon);
		friendInfo->userlevel = r->userlevel;
		friendInfo->viplevel = r->viplevel;*/
		callUpdateUserInfoListern(friendInfo);
		return;
	}

	//非好友非黑名单成员
	YVUInfoPtr userinfo = new _YVUInfo();
	userinfo->userid = r->userid;	
	userinfo->nickname = r->nickname;     //昵称
	userinfo->header = YVPlatform::getSingletonPtr()->getYVPathByUrl(r->headicon);   //图像地址
	//userinfo->sex = r->sex;     //性别
	//userinfo->userlevel = r->userlevel;     //用户等级
	//userinfo->viplevel = r->viplevel;     //用户VIP等级
	//userinfo->ext = r->ext;     //扩展字段

	callUpdateUserInfoListern(userinfo);
	YVPlatform::getSingletonPtr()->updateUInfo(userinfo);
}

void YVContactManager::delFriendNotify(YaYaRespondBase* respond)
{
	DelFriendNotify* r = static_cast<DelFriendNotify*>(respond);
	if (r->del_fromlist == df_remove_from_list)
	{
		delFriendInfo(r->del_friend);
	}
}

void YVContactManager::friendStatusNotify(YaYaRespondBase* respond)
{
	//FriendStatusNotify* r = static_cast<FriendStatusNotify*>(respond);
	
}

YVUInfoMap& YVContactManager::getAllFriendInfo()
{
	return m_friendInfos;
}

YVUInfoMap& YVContactManager::getAllBlackInfo()
{
	return m_blackInfos;
}

void YVContactManager::delAllBlackInfo()
{
	m_blackInfos.clear();
}

void YVContactManager::delAllFriendInfo()
{
	m_friendInfos.clear();
}