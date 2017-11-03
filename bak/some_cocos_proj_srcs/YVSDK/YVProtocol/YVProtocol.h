#ifndef _YVSDK_YVPROTOCOL_H_
#define _YVSDK_YVPROTOCOL_H_
/*********************************************************************************
*Copyright(C), 2015 YUNVA Company
*FileName:  YVProtocol.h
*Author:  元谷
*Version:  1.0.0
*Date:  2015-5-5
*Description:  定义协议的调用结构体
**********************************************************************************/

#include <string>
#include <iostream>
#include <vector>
#include "IMSDK.h"
#include "YVIMCmdDef.h"
#include "YVUtils/yvpacket_overloaded.h"
#include <time.h>
#include "YVType/YVType.h"

namespace YVSDK
{
	//==========================智能析构指针==========================================
	class YVRef
	{
	public:
		YVRef(){ _refCount = 1; }
		virtual ~YVRef(){};
		void addRef(){ ++_refCount; };
		void release(){ if ((--_refCount) <= 0)delete this; }
	private:
		unsigned int _refCount;
	};
	//==================================协议基类=====================================

	//请求的基类
	struct YaYaRequestBase :public  YVRef
	{
		virtual ~YaYaRequestBase(){};
		CmdChannel m_requestChannel;
		uint32 m_requestCmd;
		YaYaRequestBase(CmdChannel channel, uint32 cmd)
		{
			m_requestChannel = channel;
			m_requestCmd = cmd;
		}

		virtual YV_PARSER encode() = 0;
	};

	//响应基类
	struct YaYaRespondBase :public YVRef
	{
		virtual ~YaYaRespondBase(){};
		virtual void decode(YV_PARSER parser) = 0;
	};

	//=============================登录模块==================================

	//云娃登录请求 IM_LOGIN_REQ
	struct LoginRequest :YaYaRequestBase
	{
		uint32 userid;                        //用户ID
		std::string	 pwd;                     //用户密码
		std::string	 pgameServiceID;          //游戏服务ID
		std::vector<std::string>  wildCard;   //通配符
		uint8 readstatus;                     //消息需要确认么

		LoginRequest()
			:YaYaRequestBase(IM_LOGIN, IM_LOGIN_REQ)
		{

		}

		YV_PARSER encode()
		{
			YV_PARSER parser = yvpacket_get_parser();
			parser_set_uint32(parser, x11000::userid, userid);
			parser_set_cstring(parser, x11000::pwd, pwd.c_str());
			parser_set_cstring(parser, x11000::pgameServiceID, pgameServiceID.c_str());
			parser_set_uint8(parser, x11002::readstatus, readstatus);

			for (std::vector<std::string>::iterator it = wildCard.begin(); it < wildCard.end(); ++it)
			{
				parser_set_cstring(parser, x11000::wildCard, it->c_str());
			}

			return parser;
		}
	};

	//云娃登陆请求响应 IM_LOGIN_RESP
	struct LoginResponse :YaYaRespondBase
	{
		uint32 result; //返回结果 不为0即为失败
		std::string msg; //错误描述
		std::string nickname; //用户昵称
		uint32 userId; //用户ID
		std::string iconurl; //用户图像地址
		void decode(YV_PARSER parser)
		{
			result = parser_get_uint32(parser, x11001::result);
			msg = parser_get_string(parser, x11001::msg);
			nickname = parser_get_string(parser, x11001::nickname);
			userId = parser_get_uint32(parser, x11001::userId);
			iconurl = parser_get_string(parser, x11001::iconurl);
		}
	};

	//CP账号登陆请求
	struct CPLoginRequest : public YaYaRequestBase
	{
		CPLoginRequest()
		:YaYaRequestBase(IM_LOGIN, IM_THIRD_LOGIN_REQ)
		{

		}

		std::string tt;  //cp登录凭证
		std::string pgameServiceID; //游戏服务ID
		std::vector<std::string>  wildCard; //通配符
		uint8 readstatus;    //消息需要确认么
		YV_PARSER encode()
		{
			YV_PARSER parser = yvpacket_get_parser();
			parser_set_cstring(parser, x11002::tt, tt.c_str());
			parser_set_cstring(parser, x11002::pgameServiceID, pgameServiceID.c_str());
			parser_set_uint8(parser, x11002::readstatus, readstatus);

			for (std::vector<std::string>::iterator it = wildCard.begin(); it < wildCard.end(); ++it)
			{
				parser_set_cstring(parser, x11002::wildCard, it->c_str());
			}
			return parser;
		}
	};

	struct CPLoginResponce : public YaYaRespondBase
	{
		uint32 result; //返回结果 不为0即为失败
		std::string msg; //错误描述
		uint32 userid;//云娃ID
		std::string nickName;//用户昵称
		std::string iconUrl; //用户图像地址
		uint32 thirdUserId; //第三方用户ID
		std::string thirdUserName; //第三方用户名
		void decode(YV_PARSER parser)
		{
			this->result = parser_get_uint32(parser, x11003::result);
			this->msg = parser_get_string(parser, x11003::msg);
			this->userid = parser_get_uint32(parser, x11003::userid);
			this->nickName = parser_get_uint32(parser, x11003::nickName);
			this->iconUrl = parser_get_string(parser, x11003::iconUrl);
			this->thirdUserName = parser_get_string(parser, x11003::thirdUserId);
			this->thirdUserName = parser_get_string(parser, x11003::thirdUserName);
		}
	};

	//注销登录 IM_LOGOUT_REQ
	struct CPLogoutRequest :YaYaRequestBase
	{
		CPLogoutRequest()
		:YaYaRequestBase(IM_LOGIN, IM_LOGOUT_REQ)
		{

		}

		YV_PARSER encode()
		{
			YV_PARSER parser = yvpacket_get_parser();
			return parser;
		}
	};

	//重连通知 IM_RECONNECTION_NOTIFY
	struct ReconnectionNotify : YaYaRespondBase
	{
		uint32	userid;;

		void decode(YV_PARSER parser)
		{
			userid = parser_get_uint32(parser, x11013::userid);
		}
	};

	//用户信息
	struct YaYaUserInfo :YVRef
	{
		std::string nickname; //用户昵称
		uint32 userid; //用户ID
		std::string iconurl; //用户图像地址
		uint8  online; //是否在线
		std::string userlevel; //用户等级
		std::string viplevel; //vip等级
		std::string ext; //扩展字段
		uint8  shieldmsg; //是否屏蔽聊天消息
		uint8  sex; //性别
		std::string group; //所在组名称
		std::string note; //备注
	};

	//主动请求添加好友 IM_FRIEND_ADD_REQ
	struct AddFriendRequest :YaYaRequestBase
	{
		AddFriendRequest()
		:YaYaRequestBase(IM_FRIEND, IM_FRIEND_ADD_REQ)
		{

		}

		uint32 userid; //用户ID
		std::string greet; //问候语

		YV_PARSER encode()
		{
			YV_PARSER parser = yvpacket_get_parser();
			parser_set_uint32(parser, x12000::userid, userid);
			parser_set_cstring(parser, x12000::greet, greet.c_str());
			return parser;
		}
	};

	//主动添加好友回应 IM_FRIEND_ADD_RESP
	struct AddFriendRepond :YaYaRespondBase
	{
		e_addfriend_affirm affirm; //返回结果 e_addfriend_affirm
		uint32 userid; //用户ID
		std::string name; //用户名称
		std::string url; //头像地址
		std::string greet; //问候语

		void decode(YV_PARSER parser)
		{
			affirm = (e_addfriend_affirm)parser_get_uint32(parser, x12001::affirm);
			userid = parser_get_uint32(parser, x12001::userid);
			name = parser_get_string(parser, x12001::name);
			url = parser_get_string(parser, x12001::url);
			greet = parser_get_string(parser, x12001::greet);
		}
	};

	//被动好友请求通知  IM_FRIEND_ADD_NOTIFY  
	struct  AddFriendNotify :YaYaRespondBase
	{
		uint32 userid;  //用户ID
		std::string name;  //用户名称
		std::string greet;  //问候语
		std::string sign;	//签名
		std::string url;  //头像地址
		void decode(YV_PARSER parser)
		{
			userid = parser_get_uint32(parser, x12002::userid);
			name = parser_get_string(parser, x12002::name);
			greet = parser_get_string(parser, x12002::greet);
			sign = parser_get_string(parser, x12002::sign);
			url = parser_get_string(parser, x12002::url);
		}
	};

	//被动同意添加好友 IM_FRIEND_ADD_ACCEPT
	struct  FriendAcceptRequest :YaYaRequestBase
	{
		FriendAcceptRequest()
		:YaYaRequestBase(IM_FRIEND, IM_FRIEND_ADD_ACCEPT)
		{

		}

		uint32  userid; //用户ID
		e_addfriend_affirm	affirm;	//是否同意 e_addfriend_affirm
		std::string	greet;	//问候语
		YV_PARSER encode()
		{
			YV_PARSER parser = yvpacket_get_parser();
			parser_set_uint32(parser, x12003::userid, userid);
			parser_set_uint8(parser, x12003::affirm, affirm);
			parser_set_cstring(parser, x12003::greet, greet.c_str());
			return parser;
		}
	};

	//被动添加好友结果回应 IM_FRIEND_ACCEPT_RESP
	struct  FriendAcceptRespond :YaYaRespondBase
	{
		uint32  userid; //用户ID
		e_addfriend_affirm	affirm;	//是否同意 e_addfriend_affirm
		std::string	greet;	//问候语
		void decode(YV_PARSER parser)
		{
			userid = parser_get_uint32(parser, x12004::userid);
			affirm = (e_addfriend_affirm)parser_get_uint8(parser, x12004::affirm);
			greet = parser_get_string(parser, x12004::greet);
		}
	};

	//删除好友请求 IM_FRIEND_DEL_REQ
	struct DelFriendRequest :YaYaRequestBase
	{
		DelFriendRequest()
		:YaYaRequestBase(IM_FRIEND, IM_FRIEND_DEL_REQ)
		{

		}

		uint32	del_friend;  //删除好友id
		e_delfriend  act;  //动作 e_delfriend

		YV_PARSER encode()
		{
			YV_PARSER parser = yvpacket_get_parser();
			parser_set_uint32(parser, x12005::del_friend, del_friend);
			parser_set_uint8(parser, x12005::act, act);

			return parser;
		}
	};

	//删除好友返回 IM_FRIEND_DEL_RESP
	struct DelFriendRespond :YaYaRespondBase
	{
		uint32		result;
		std::string		msg;
		uint32	    del_friend; //删除好友id
		e_delfriend       act;  //动作 e_delfriend

		void decode(YV_PARSER parser)
		{
			result = parser_get_uint32(parser, x12006::result);
			del_friend = parser_get_uint32(parser, x12006::del_friend);
			act = (e_delfriend)parser_get_uint8(parser, x12006::act);
			msg = parser_get_string(parser, x12006::msg);
		}
	};

	//删除好友通知 IM_FRIEND_DEL_NOTIFY
	struct DelFriendNotify :YaYaRespondBase
	{
		uint32		del_friend;
		e_delfriend       del_fromlist;  //动作 e_delfriend

		void decode(YV_PARSER parser)
		{
			del_friend = parser_get_uint32(parser, x12007::del_friend);
			del_fromlist = (e_delfriend)parser_get_uint8(parser, x12007::del_fromlist);
		}
	};

	//搜索好友请求 IM_FRIEND_SEARCH_REQ 
	struct SearchFriendRequest :YaYaRequestBase
	{
		SearchFriendRequest()
		:YaYaRequestBase(IM_FRIEND, IM_FRIEND_SEARCH_REQ)
		{

		}

		std::string keyword; //搜索关键字
		uint32 start; //搜索起始位置
		uint32 count; //返回结果总数

		YV_PARSER encode()
		{
			YV_PARSER parser = yvpacket_get_parser();
			parser_set_cstring(parser, x12018::keyworld, keyword.c_str());
			parser_set_uint32(parser, x12018::start, start);
			parser_set_uint32(parser, x12018::count, count);
			return parser;
		}
	};

	//推荐好友 IM_FRIEND_RECOMMAND_REQ 
	struct RecommendFriendRequest :YaYaRequestBase
	{
		RecommendFriendRequest()
		:YaYaRequestBase(IM_FRIEND, IM_FRIEND_RECOMMAND_REQ)
		{

		}
		uint32 start; //结果列表的起始位置
		uint32 count; //返回结果条目
		YV_PARSER encode()
		{
			YV_PARSER parser = yvpacket_get_parser();
			parser_set_uint32(parser, x12008::start, start);
			parser_set_uint32(parser, x12008::count, count);
			return parser;
		}
	};

	//获取个人信息
	struct GetUserInfoRequest :YaYaRequestBase
	{
		GetUserInfoRequest()
		:YaYaRequestBase(IM_FRIEND, IM_USER_GETINFO_REQ)
		{

		}
		uint32 userid;
		YV_PARSER encode()
		{
			YV_PARSER parser = yvpacket_get_parser();
			parser_set_uint32(parser, x12020::userid, userid);
			return parser;
		}
	};

	//获取个人信息回应 IM_USER_GETINFO_RESP 
	struct GetUserInfoFriendRespond :YaYaRespondBase
	{
		uint32  userid;
		uint8   sex;     //性别
		std::string  nickname;     //昵称
		std::string  headicon;     //图像地址
		std::string  userlevel;     //用户等级
		std::string  viplevel;     //用户VIP等级
		std::string  ext;     //扩展字段

		void decode(YV_PARSER parser)
		{
			userid = parser_get_uint32(parser, x12021::userid);
			sex = parser_get_uint8(parser, x12021::sex);
			nickname = parser_get_string(parser, x12021::nickname);
			headicon = parser_get_string(parser, x12021::headicon);
			userlevel = parser_get_string(parser, x12021::userlevel);
			viplevel = parser_get_string(parser, x12021::viplevel);
			ext = parser_get_string(parser, x12021::ext);
		}
	};

	struct SearchRetInfo
	{
		uint32 yunvaId;
		std::string userId; //用户ID
		std::string nickName; //用户昵称
		std::string iconUrl; //用户图像地址
		std::string level; //用户等级
		std::string vip; //用户VIP等级
		std::string Ext; //扩展字段
	};

	//搜索好友回应 IM_FRIEND_SEARCH_RESP  
	struct SearchFriendRespond :YaYaRespondBase
	{
		uint32		result;
		std::string msg;
		std::vector<SearchRetInfo> searchRetInfo;

		void decode(YV_PARSER parser)
		{
			result = parser_get_uint32(parser, x12019::result);
			msg = parser_get_string(parser, x12019::msg);
			int index = 0;
			while (true)
			{
				if (parser_is_empty(parser, x12019::userinfo, index))
					return;

				YV_PARSER Object = yvpacket_get_parser_object(parser);
				parser_get_object(parser, x12019::userinfo, Object, index);

				SearchRetInfo userInfo;
				userInfo.yunvaId = parser_get_uint32(Object, xSearchInfo::yunvaId);
				userInfo.userId = parser_get_string(Object, xSearchInfo::userId);
				userInfo.nickName = parser_get_string(Object, xSearchInfo::nickName);
				userInfo.iconUrl = parser_get_string(Object, xSearchInfo::iconUrl);
				userInfo.level = parser_get_string(Object, xSearchInfo::level);
				userInfo.vip = parser_get_string(Object, xSearchInfo::vip);
				userInfo.Ext = parser_get_string(Object, xSearchInfo::Ext);
				searchRetInfo.push_back(userInfo);
				++index;
			}
		}
	};

	//推荐好友回应 IM_FRIEND_RECOMMAND_RESP 
	struct RecommendFriendRespond :YaYaRespondBase
	{
		uint32	result; //结果信息
		std::string		msg; //错误信息
		std::vector<SearchRetInfo> userInfos; //用户信息
		void decode(YV_PARSER parser)
		{
			result = parser_get_uint32(parser, x12009::result);
			msg = parser_get_string(parser, x12009::msg);
			int index = 0;
			while (true)
			{
				if (parser_is_empty(parser, x12009::userinfo, index))
					return;

				YV_PARSER Object = yvpacket_get_parser_object(parser);
				parser_get_object(parser, x12009::userinfo, Object, index);

				SearchRetInfo userInfo;
				userInfo.yunvaId = parser_get_uint32(Object, xSearchInfo::yunvaId);
				userInfo.userId = parser_get_string(Object, xSearchInfo::userId);
				userInfo.nickName = parser_get_string(Object, xSearchInfo::nickName);
				userInfo.iconUrl = parser_get_string(Object, xSearchInfo::iconUrl);
				userInfo.level = parser_get_string(Object, xSearchInfo::level);
				userInfo.vip = parser_get_string(Object, xSearchInfo::vip);
				userInfo.Ext = parser_get_string(Object, xSearchInfo::Ext);
				userInfos.push_back(userInfo);

				++index;
			}
		}
	};

	//好友操作请求(黑名单) IM_FRIEND_OPER_REQ	
	struct BlackControlRequest :YaYaRequestBase
	{
		BlackControlRequest()
		:YaYaRequestBase(IM_FRIEND, IM_FRIEND_OPER_REQ)
		{

		}
		uint32	userId;	//用户ID
		uint32	operId;	//要操作的黑名的用户的id
		e_oper_friend_act	act;	//动作   oper_friend_act

		YV_PARSER encode()
		{
			YV_PARSER parser = yvpacket_get_parser();
			parser_set_uint8(parser, x12010::act, act);
			parser_set_uint32(parser, x12010::userId, userId);
			parser_set_uint32(parser, x12010::operId, operId);
			return parser;
		}
	};

	//好友操作回应(黑名单) IM_FRIEND_OPER_RESP	
	struct BlackControlRespond :YaYaRespondBase
	{
		uint32	userId;	//用户ID
		uint32	operId;	//要操作的黑名的用户的id
		e_oper_friend_act	act;	//动作   oper_friend_act
		e_friend_status	oper_state; //对方状态

		void decode(YV_PARSER parser)
		{
			userId = parser_get_uint32(parser, x12011::userId);
			operId = parser_get_uint32(parser, x12011::operId);
			act = (e_oper_friend_act)parser_get_uint8(parser, x12011::act);
			oper_state = (e_friend_status)parser_get_uint8(parser, x12011::oper_state);
		}
	};

	//好友列表推送 IM_FRIEND_LIST_NOTIFY
	struct FriendListNotify :YaYaRespondBase
	{

		~FriendListNotify()
		{
			for (std::vector<YaYaUserInfo*>::iterator it = userInfos.begin();
				it != userInfos.end(); ++it)
			{
				(*it)->release();
			}
		}

		std::vector<YaYaUserInfo*> userInfos;
		void decode(YV_PARSER parser)
		{
			int index = 0;
			while (true)
			{
				if (parser_is_empty(parser, x12012::userinfo, index))
					return;

				YV_PARSER Object = yvpacket_get_parser_object(parser);
				parser_get_object(parser, x12012::userinfo, Object, index);

				YaYaUserInfo* userInfo = new YaYaUserInfo();
				userInfo->nickname = parser_get_string(Object, xUserInfo::nickname);
				userInfo->userid = parser_get_uint32(Object, xUserInfo::userid);
				userInfo->iconurl = parser_get_string(Object, xUserInfo::iconurl);
				userInfo->online = parser_get_uint8(Object, xUserInfo::online);
				userInfo->userlevel = parser_get_string(Object, xUserInfo::userlevel);
				userInfo->viplevel = parser_get_string(Object, xUserInfo::viplevel);
				userInfo->ext = parser_get_string(Object, xUserInfo::ext);
				userInfo->sex = parser_get_uint8(Object, xUserInfo::sex);
				userInfo->group = parser_get_string(Object, xUserInfo::group);
				userInfo->note = parser_get_string(Object, xUserInfo::remark);
				userInfo->shieldmsg = parser_get_uint8(Object, xUserInfo::shieldmsg);
				userInfos.push_back(userInfo);
				++index;
			}
		}
	};

	//黑名单列表推送 IM_FRIEND_BLACKLIST_NOTIFY
	struct BlackListNotify :YaYaRespondBase
	{
		~BlackListNotify()
		{
			for (std::vector<YaYaUserInfo*>::iterator it = userInfos.begin();
				it != userInfos.end(); ++it)
			{
				(*it)->release();
			}
		}
		std::vector<YaYaUserInfo*> userInfos;
		void decode(YV_PARSER parser)
		{
			int index = 0;
			while (true)
			{
				if (parser_is_empty(parser, x12013::userinfo, index))
					return;

				YV_PARSER Object = yvpacket_get_parser_object(parser);
				parser_get_object(parser, x12013::userinfo, Object, index);

				YaYaUserInfo* userInfo = new YaYaUserInfo();
				userInfo->nickname = parser_get_string(Object, xUserInfo::nickname);
				userInfo->userid = parser_get_uint32(Object, xUserInfo::userid);
				userInfo->iconurl = parser_get_string(Object, xUserInfo::iconurl);
				userInfo->online = parser_get_uint8(Object, xUserInfo::online);
				userInfo->userlevel = parser_get_string(Object, xUserInfo::userlevel);
				userInfo->viplevel = parser_get_string(Object, xUserInfo::viplevel);
				userInfo->ext = parser_get_string(Object, xUserInfo::ext);
				userInfo->sex = parser_get_uint8(Object, xUserInfo::sex);
				userInfo->group = parser_get_string(Object, xUserInfo::group);
				userInfo->note = parser_get_string(Object, xUserInfo::remark);
				userInfos.push_back(userInfo);
				++index;
			}
		}
	};

	//好友状态推送 IM_FRIEND_STATUS_NOTIFY 
	struct FriendStatusNotify :YaYaRespondBase
	{
		uint32 userid;
		e_friend_status  status;

		void decode(YV_PARSER parser)
		{
			userid = parser_get_uint32(parser, x12015::userid);
			status = (e_friend_status)parser_get_uint8(parser, x12015::status);
		}
	};

	//设置好友信息 IM_FRIEND_INFOSET_REQ
	struct FriendInfoSetRequest :YaYaRequestBase
	{
		FriendInfoSetRequest()
		:YaYaRequestBase(IM_FRIEND, IM_FRIEND_INFOSET_REQ)
		{

		}
		uint32 friendId; //好友ID
		std::string group; //好友所在组
		std::string note; //好友备注

		YV_PARSER encode()
		{
			YV_PARSER parser = yvpacket_get_parser();
			parser_set_cstring(parser, x12016::group, group.c_str());
			parser_set_uint32(parser, x12016::friendId, friendId);
			parser_set_cstring(parser, x12016::note, note.c_str());
			return parser;
		}
	};

	//云消息的父类
	struct YaYaxNotify
	{
		uint32 unread;
	};
	//好友离线结构
	struct YaYaP2PChatNotify :YaYaxNotify
	{
		uint32 userid; //用户ID
		std::string name; //用户名称
		std::string signature; //用户签名
		std::string headicon; //图像地址
		uint32 sendtime; //消息发送时间
		e_chat_msgtype  type; //消息类型 e_chat_msgtype
		std::string data; //若为文本类型，则是消息内容，其他则是文件地址
		std::string imageurl; //若为图片，则是小图像地址
		uint32 audiotime; //若为音频文件, 则为文件播放时长(秒)
		std::string attach; //若为音频文件，则是附加文本(没有附带文本时为空)
		std::string ext1; //扩展字段
	};

	//群离线结构
	struct YaYaGroupChatNotify :YaYaxNotify
	{
		uint32 groupid; //群ID
		uint32 sendid; //发送者ID
		uint32 time; //消息发送时间
		std::string groupicon; //群图像地址
		std::string groupname; //群名称
		e_chat_msgtype  type; //消息类型 e_chat_msgtype
		std::string data; //若为文本类型，则是消息内容，其他则是文件地址
		uint32 audiotime; //若为音频文件, 则为文件播放时长(秒)
	};

	//云消息推送 IM_CLOUDMSG_NOTIFY 
	struct CloundMsgNotify :YaYaRespondBase
	{
		std::string source; //来源(SYSTEM 系统消息 PUSH 推送消息 userId 好友消息)
		uint32 id; //若是好友消息, 则为好友ID
		uint32 beginid;  //开始索引
		uint32 endid; //结束索引
		uint32 time; //结束索引时间
		uint32 unread; //未读消息数
		YaYaxNotify*  packet;  //结束索引内容  xP2PChatMsg,  xGroupChatMsg

		CloundMsgNotify()
		{
			packet = NULL;
		}
		~CloundMsgNotify()
		{
			if (packet != NULL)
			{
				delete packet;
			}
		}

		void decode(YV_PARSER parser)
		{
			this->source = parser_get_string(parser, x15002::source);
			this->id = parser_get_uint32(parser, x15002::id);
			this->beginid = parser_get_uint32(parser, x15002::beginid);
			this->endid = parser_get_uint32(parser, x15002::endid);
			this->time = parser_get_uint32(parser, x15002::time);
			this->unread = parser_get_uint32(parser, x15002::unread);
			/*	if (this->unread == 0)
				{
				packet = NULL;
				return;
				}
				*/
			YV_PARSER yayaParser = yvpacket_get_parser_object(parser);
			parser_get_object(parser, x15002::packet, yayaParser);
			if (this->source == CLOUDMSG_FRIEND)
			{
				YaYaP2PChatNotify* p2pmsg = new YaYaP2PChatNotify();
				p2pmsg->unread = this->unread;
				p2pmsg->userid = parser_get_uint32(yayaParser, xP2PChatMsg::userid);
				p2pmsg->name = parser_get_string(yayaParser, xP2PChatMsg::name);
				p2pmsg->signature = parser_get_string(yayaParser, xP2PChatMsg::signature);
				p2pmsg->headicon = parser_get_string(yayaParser, xP2PChatMsg::headurl);
				p2pmsg->sendtime = parser_get_uint32(yayaParser, xP2PChatMsg::sendtime);
				p2pmsg->type = (e_chat_msgtype)parser_get_uint8(yayaParser, xP2PChatMsg::type);
				p2pmsg->data = parser_get_string(yayaParser, xP2PChatMsg::data);
				p2pmsg->imageurl = parser_get_string(yayaParser, xP2PChatMsg::imageurl);
				p2pmsg->audiotime = parser_get_uint32(yayaParser, xP2PChatMsg::audiotime);
				p2pmsg->attach = parser_get_string(yayaParser, xP2PChatMsg::attach);
				p2pmsg->ext1 = parser_get_string(yayaParser, xP2PChatMsg::ext1);

				packet = p2pmsg;
			}
			else if (this->source == CLOUDMSG_GROUP)
			{
				YaYaGroupChatNotify* groupmsg = new YaYaGroupChatNotify();

				groupmsg->groupid = parser_get_uint32(yayaParser, xGroupChatMsg::groupid);
				groupmsg->sendid = parser_get_uint32(yayaParser, xGroupChatMsg::sendid);
				groupmsg->groupicon = parser_get_string(yayaParser, xGroupChatMsg::groupicon);
				groupmsg->groupname = parser_get_string(yayaParser, xGroupChatMsg::groupname);
				groupmsg->time = parser_get_uint32(yayaParser, xGroupChatMsg::time);
				groupmsg->type = (e_chat_msgtype)parser_get_uint8(yayaParser, xGroupChatMsg::type);
				groupmsg->data = parser_get_string(yayaParser, xGroupChatMsg::data);
				groupmsg->audiotime = parser_get_uint32(yayaParser, xGroupChatMsg::audiotime);

				packet = groupmsg;
			}
			else
			{
				//CCLOG("not has this Notify logic:%s", this->source.c_str());
			}
		}
	};

	//请求云消息IM_CLOUDMSG_LIMIT_REQ 
	struct CloundMsgRequest : YaYaRequestBase
	{
		std::string source; //来源(userId 好友消息)
		uint32 id;     //若是好友消息, 则为好友ID
		uint32 end;    //获取到位置（endid）
		int limit;     //获取条数 

		CloundMsgRequest()
			:YaYaRequestBase(IM_CLOUND, IM_CLOUDMSG_LIMIT_REQ)
		{

		}
		YV_PARSER encode()
		{
			YV_PARSER parser = yvpacket_get_parser();
			parser_set_cstring(parser, x15003::source, source.c_str());
			parser_set_uint32(parser, x15003::id, id);
			parser_set_uint32(parser, x15003::end, end);
			parser_set_integer(parser, x15003::limit, limit);
			return parser;
		}
	};

	//请求云消息回应 IM_CLOUDMSG_LIMIT_RESP 
	struct CloundMsgRepond :YaYaRespondBase
	{
		uint32 result;  //结果信息     
		std::string msg;     //错误信息
		std::string session; //请求Session

		void decode(YV_PARSER parser)
		{
			result = parser_get_uint32(parser, x15004::result);
			msg = parser_get_string(parser, x15004::msg);
			session = parser_get_string(parser, x15004::session);
		}
	};

	//消息状态推送 IM_CHATT_FRIEND_RESP 
	struct FriendMsgStateNotify :YaYaRespondBase
	{
		uint32 result;
		uint32 userid;  //好友ID
		std::string  msg;
		std::string flag;  //消息标记
		uint32 index;
		void decode(YV_PARSER parser)
		{
			index = parser_get_uint32(parser, CLOUDMSG_ID);
			result = parser_get_uint32(parser, x14004::result);
			userid = parser_get_uint32(parser, x14004::userid);
			flag = parser_get_string(parser, x14004::flag);
			msg = parser_get_string(parser, x14004::msg);
		}
	};


	//云消息推送 IM_CLOUDMSG_LIMIT_NOTIFY
	struct CloundMsgLimitNotify :YaYaRespondBase
	{
		std::string source; //来源(SYSTEM 系统消息 PUSH 推送消息 userId 好友消息)
		uint32 id; //若是好友消息, 则为好友ID
		uint32 count;  //消息数
		uint32 indexId;  //当前消息索引
		uint32 ptime; //当前消息时间

		YaYaxNotify*  packet;  //结束索引内容  xP2PChatMsg,  xGroupChatMsg

		CloundMsgLimitNotify()
		{
			packet = NULL;
		}

		~CloundMsgLimitNotify()
		{
			if (packet != NULL)
			{
				delete packet;
			}
		}

		void decode(YV_PARSER parser)
		{
			this->source = parser_get_string(parser, x15005::source);
			this->id = parser_get_uint32(parser, x15005::id);
			this->count = parser_get_uint32(parser, x15005::count);
			this->indexId = parser_get_uint32(parser, x15005::indexId);
			this->ptime = parser_get_uint32(parser, x15005::ptime);


			YV_PARSER yayaParser = yvpacket_get_parser_object(parser);
			parser_get_object(parser, x15002::packet, yayaParser);
			if (this->source == CLOUDMSG_FRIEND)
			{
				YaYaP2PChatNotify* p2pmsg = new YaYaP2PChatNotify();

				p2pmsg->userid = parser_get_uint32(yayaParser, xP2PChatMsg::userid);
				p2pmsg->name = parser_get_string(yayaParser, xP2PChatMsg::name);
				p2pmsg->signature = parser_get_string(yayaParser, xP2PChatMsg::signature);
				p2pmsg->headicon = parser_get_string(yayaParser, xP2PChatMsg::headurl);
				p2pmsg->sendtime = parser_get_uint32(yayaParser, xP2PChatMsg::sendtime);
				p2pmsg->type = (e_chat_msgtype)parser_get_uint8(yayaParser, xP2PChatMsg::type);
				p2pmsg->data = parser_get_string(yayaParser, xP2PChatMsg::data);
				p2pmsg->imageurl = parser_get_string(yayaParser, xP2PChatMsg::imageurl);
				p2pmsg->audiotime = parser_get_uint32(yayaParser, xP2PChatMsg::audiotime);
				p2pmsg->attach = parser_get_string(yayaParser, xP2PChatMsg::attach);
				p2pmsg->ext1 = parser_get_string(yayaParser, xP2PChatMsg::ext1);

				packet = p2pmsg;
			}
			else if (this->source == CLOUDMSG_GROUP)
			{
				YaYaGroupChatNotify* groupmsg = new YaYaGroupChatNotify();

				groupmsg->groupid = parser_get_uint32(yayaParser, xGroupChatMsg::groupid);
				groupmsg->sendid = parser_get_uint32(yayaParser, xGroupChatMsg::sendid);
				groupmsg->groupicon = parser_get_string(yayaParser, xGroupChatMsg::groupicon);
				groupmsg->groupname = parser_get_string(yayaParser, xGroupChatMsg::groupname);
				groupmsg->time = parser_get_uint32(yayaParser, xGroupChatMsg::time);
				groupmsg->type = (e_chat_msgtype)parser_get_uint8(yayaParser, xGroupChatMsg::type);
				groupmsg->data = parser_get_string(yayaParser, xGroupChatMsg::data);
				groupmsg->audiotime = parser_get_uint32(yayaParser, xGroupChatMsg::audiotime);

				packet = groupmsg;
			}
			else
			{
				//CCLOG("not has this Notify logic:%s", this->source.c_str());
			}
		}
	};

	//云消息确认已读  IM_CLOUDMSG_READ_STATUS
	struct CloundMsgReadStatusRequest : YaYaRequestBase
	{
		CloundMsgReadStatusRequest()
		:YaYaRequestBase(IM_CLOUND, IM_CLOUDMSG_READ_STATUS)
		{

		}
		uint32  id;      //对应 CLOUDMSG_ID::110
		std::string	source;  //对应 CLOUDMSG_SOURCE::111

		YV_PARSER encode()
		{
			YV_PARSER parser = yvpacket_get_parser();
			parser_set_cstring(parser, x15007::source, source.c_str());
			parser_set_uint32(parser, x15007::id, id);
			return parser;
		}
	};


	//好友聊天-文本 IM_CHAT_FRIEND_TEXT_REQ
	struct FriendTextChatRequest :YaYaRequestBase
	{
		FriendTextChatRequest()
		:YaYaRequestBase(IM_CHAT, IM_CHAT_FRIEND_TEXT_REQ)
		{

		}

		uint32 userid; //好友ID
		std::string data; //消息内容
		std::string ext; //扩展字段
		std::string flag;

		YV_PARSER encode()
		{
			YV_PARSER parser = yvpacket_get_parser();
			parser_set_cstring(parser, x14000::data, data.c_str());
			parser_set_uint32(parser, x14000::userid, userid);
			parser_set_cstring(parser, x14000::ext, ext.c_str());
			parser_set_cstring(parser, x14000::flag, flag.c_str());
			return parser;
		}
	};

	//好友聊天-图像 IM_CHAT_FRIEND_IMAGE_REQ
	struct FriendImageChatRequest :YaYaRequestBase
	{
		FriendImageChatRequest()
		:YaYaRequestBase(IM_CHAT, IM_CHAT_FRIEND_IMAGE_REQ)
		{

		}
		uint32 userid; //好友ID
		std::string image; //图片路径
		std::string ext; //扩展字段
		std::string flag;

		YV_PARSER encode()
		{
			YV_PARSER parser = yvpacket_get_parser();
			parser_set_cstring(parser, x14001::image, image.c_str());
			parser_set_uint32(parser, x14001::userid, userid);
			parser_set_cstring(parser, x14001::ext, ext.c_str());
			parser_set_cstring(parser, x14001::flag, flag.c_str());
			return parser;
		}
	};

	//好友聊天 - 语音 IM_CHATI_FRIEND_AUDIO_REQ
	struct FriendVoiceChatRequest :YaYaRequestBase
	{
		FriendVoiceChatRequest()
		:YaYaRequestBase(IM_CHAT, IM_CHATI_FRIEND_AUDIO_REQ)
		{

		}

		uint32 userid; //好友ID
		std::string file; //语音文件路径
		uint32 time; //文件播放时长(秒)
		std::string txt; //附带文本(可选)
		std::string ext; //扩展字段
		std::string flag;

		YV_PARSER encode()
		{
			YV_PARSER parser = yvpacket_get_parser();
			parser_set_cstring(parser, x14002::file, file.c_str());
			parser_set_uint32(parser, x14002::userid, userid);
			parser_set_uint32(parser, x14002::time, time);
			parser_set_cstring(parser, x14002::txt, txt.c_str());
			parser_set_cstring(parser, x14002::ext, ext.c_str());
			parser_set_cstring(parser, x14002::flag, flag.c_str());
			return parser;
		}
	};

	//好友聊天通知 IM_CHAT_FRIEND_NOTIFY
	struct FriendChatNotify :YaYaRespondBase
	{
		uint32 userid; //好友ID
		std::string name; //好友名称
		std::string signature; //好友签名
		std::string headicon; //图像地址
		uint32 sendtime; //发送时间
		e_chat_msgtype  type;//类型 e_chat_msgtype
		std::string data; //若为文本类型，则是消息内容，若为音频，则是文件地址，若为图像，则是大图像地址
		std::string imageurl; //若为音频，则是小图像地址
		uint32 audiotime; //若为音频文件, 则是文件播放时长(秒)
		std::string attach; //若为音频文件，则是附加文本(没有附带文本时为空)
		std::string ext1; //扩展字段
		uint32 index; 
		void decode(YV_PARSER parser)
		{
			userid = parser_get_uint32(parser, x14003::userid);
			name = parser_get_string(parser, x14003::name);
			signature = parser_get_string(parser, x14003::signature);
			headicon = parser_get_string(parser, x14003::headurl);
			sendtime = parser_get_uint32(parser, x14003::sendtime);
			type = (e_chat_msgtype)parser_get_uint8(parser, x14003::type);
			data = parser_get_string(parser, x14003::data);
			imageurl = parser_get_string(parser, x14003::imageurl);
			audiotime = parser_get_uint32(parser, x14003::audiotime);
			attach = parser_get_string(parser, x14003::attach);
			ext1 = parser_get_string(parser, x14003::ext1);
			index = parser_get_uint32(parser, CLOUDMSG_ID);
		}
	};
	/****************************频道开始***************************/
    // 频道登录
    struct ChannelLoginRequest : YaYaRequestBase
    {
        ChannelLoginRequest()
            : YaYaRequestBase(IM_CHANNEL, IM_CHANNEL_LOGIN_REQ)
        {
        }

        std::string pgameServiceID;
        std::vector<std::string> wildCard;

		YV_PARSER encode()
		{
			YV_PARSER parser = yvpacket_get_parser();
            parser_set_cstring(parser, x16007::pgameServiceID, pgameServiceID.c_str());

            for (auto &w : wildCard)
			{
				parser_set_cstring(parser, x16007::wildCard, w.c_str());
			}

			return parser;
		}
    };

	//发送频道文字消息请求 IM_CHANNEL_TEXTMSG_REQ
	struct ChannelTextRequest :YaYaRequestBase
	{
		ChannelTextRequest()
		:YaYaRequestBase(IM_CHANNEL, IM_CHANNEL_TEXTMSG_REQ)
		{

		}

		std::string	textMsg; //发送内容
		std::string	wildCard; //通配符
		std::string	expand; //扩展字段
		YV_PARSER encode()
		{
			YV_PARSER parser = yvpacket_get_parser();
			parser_set_cstring(parser, x16002::textMsg, textMsg.c_str());
			parser_set_cstring(parser, x16002::wildCard, wildCard.c_str());
			parser_set_cstring(parser, x16002::expand, expand.c_str());

			return parser;
		}
	};

	//发送频道语音消息 IM_CHANNEL_VOICEMSG_REQ	
	struct ChannelVoiceRequest :YaYaRequestBase
	{
		ChannelVoiceRequest()
		:YaYaRequestBase(IM_CHANNEL, IM_CHANNEL_VOICEMSG_REQ)
		{

		}

		std::string			voiceFilePath;     //录音文件路径名
		uint32			voiceDurationTime; //录音时长  单位秒
		std::string			wildCard;          // 游戏通道字符串
		std::string          txt;               //附带文本(可选)
		std::string			expand;            //扩展字段
		YV_PARSER encode()
		{
			YV_PARSER parser = yvpacket_get_parser();
			parser_set_cstring(parser, x16003::voiceFilePath, voiceFilePath.c_str());
			parser_set_cstring(parser, x16003::wildCard, wildCard.c_str());
			parser_set_cstring(parser, x16003::txt, txt.c_str());
			parser_set_cstring(parser, x16003::expand, expand.c_str());
			parser_set_uint32(parser, x16003::voiceDurationTime, voiceDurationTime);
			return parser;
		}
	};
	//频道发送结果通知 IM_CHANNEL_SENDMSG_RESP 
	struct ChannelMessageStateNotify :YaYaRespondBase
	{
		uint32 result;
		std::string	wildCard;	
		std::string	expand;   //透传字段
		bool  shield;		  //是否有敏感字， 1：存在，0不存在
		
		void decode(YV_PARSER parser)
		{
			result = parser_get_uint32(parser, x16010::result);
			wildCard = parser_get_string(parser, x16010::wildCard);
			expand = parser_get_string(parser, x16010::expand);
			shield = parser_get_uint8(parser, x16010::shield) == 0?false:true;
		}
	};

	//频道收到消息通知  IM_CHANNEL_MESSAGE_NOTIFY
	struct ChannelMessageNotify :YaYaRespondBase
	{
		uint32	user_id;//用户ID
		std::string	message_body; //消息
		std::string	nickname; //昵称
		std::string	ext1; //扩展1
		std::string	ext2; //扩展2
		uint8	channel; //游戏通道
		std::string	wildcard;//游戏通道字符串
		e_chat_msgtype	message_type;// type= 1 语音  type= 2 文本
		uint32  voiceDuration; //type= 1 语音时 该字段为语音时长
		std::string  attach;//语音消息的附带文本(可选)

		void decode(YV_PARSER parser)
		{
			user_id = parser_get_uint32(parser, x16004::user_id);
			message_body = parser_get_string(parser, x16004::message_body);
			nickname = parser_get_string(parser, x16004::nickname);
			ext1 = parser_get_string(parser, x16004::ext1);
			ext2 = parser_get_string(parser, x16004::ext2);

			channel = parser_get_uint8(parser, x16004::channel);
			wildcard = parser_get_string(parser, x16004::wildcard);

			message_type = (e_chat_msgtype)parser_get_uint32(parser, x16004::message_type);

			voiceDuration = parser_get_uint32(parser, x16004::voiceDuration);
			attach = parser_get_string(parser, x16004::attach);
		}
	};

	//频道历史消息请求 IM_CHANNEL_HISTORY_MSG_REQ
	struct ChannelHistoryRequest :YaYaRequestBase
	{
		ChannelHistoryRequest()
		:YaYaRequestBase(IM_CHANNEL, IM_CHANNEL_HISTORY_MSG_REQ)
		{

		}

		uint32	index;				//消息索引	(当前最大索引号,索引为0请求最后count条记录)
		uint32	count; 				//请求条数	正数为index向后请求 负数为index向前请求 (时间排序)
		std::string	wildCard;           //通配符

		YV_PARSER encode()
		{
			YV_PARSER parser = yvpacket_get_parser();
			parser_set_uint32(parser, x16005::index, index);
			parser_set_uint32(parser, x16005::count, count);
			parser_set_cstring(parser, x16005::wildcard, wildCard.c_str());

			return parser;
		}
	};

	////room消息列表
	struct YaYaChannelHistoryMsgInfo
	{
		uint32	index;			//消息索引
		std::string	ctime;			//消息时间 2015-02 10:50:13
		uint32	user_id;			//用户ID
		std::string	message_body;			//消息
		std::string	nickname;			//昵称
		std::string	ext1;			//扩展1
		std::string	ext2;			//扩展2
		uint8	channel;			//游戏通道
		std::string	wildcard;			//游戏通道字符串
		e_chat_msgtype	message_type; 			//type= 1 语音  type= 2 文本
		uint32  voiceDuration;			//type= 1 语音时 该字段为语音时长
		std::string  attach;			//语音消息的附带文本(可选)	
	};

	//频道历史消息返回 IM_CHANNEL_HISTORY_MSG_RESP
	struct ChannelHistoryNotify :YaYaRespondBase
	{
		std::vector<YaYaChannelHistoryMsgInfo> xHistoryMsg;

		void decode(YV_PARSER parser)
		{
			int index = 0;
			while (true)
			{
				if (parser_is_empty(parser, x16006::xHistoryMsg, index))
					return;

				YV_PARSER Object = yvpacket_get_parser_object(parser);
				parser_get_object(parser, x16006::xHistoryMsg, Object, index);

				YaYaChannelHistoryMsgInfo info;
				info.index = parser_get_uint32(Object, xHistoryMsgInfo::index);
				info.ctime = parser_get_string(Object, xHistoryMsgInfo::ctime);
				info.user_id = parser_get_uint32(Object, xHistoryMsgInfo::user_id);
				info.message_body = parser_get_string(Object, xHistoryMsgInfo::message_body);
				info.nickname = parser_get_string(Object, xHistoryMsgInfo::nickname);
				info.ext1 = parser_get_string(Object, xHistoryMsgInfo::ext1);
				info.ext2 = parser_get_string(Object, xHistoryMsgInfo::ext2);
				info.channel = parser_get_uint8(Object, xHistoryMsgInfo::channel);
				info.wildcard = parser_get_string(Object, xHistoryMsgInfo::wildcard);
				info.message_type = (e_chat_msgtype)parser_get_uint32(Object, xHistoryMsgInfo::message_type);
				info.voiceDuration = parser_get_uint32(Object, xHistoryMsgInfo::voiceDuration); 			//type= 1 语音  type= 2 文本
				info.attach = parser_get_string(Object, xHistoryMsgInfo::voiceDuration); 			//type= 1 语音时 该字段为语音时长

				xHistoryMsg.push_back(info);
				++index;
			}
		}
	};

	/****************************频道结束***************************/

	/****************************工具开始***************************/
	//网络状态通知IM_NET_STATE_NOTIFY
	struct NetWorkStateNotify :YaYaRespondBase
	{
		yv_net state;
		void decode(YV_PARSER parser)
		{
			state = (yv_net)parser_get_uint8(parser, x11016::state);
		}
	};

	//设置录音时长 IM_RECORD_SETINFO_REQ
	struct SetRecordRequest :YaYaRequestBase
	{
		uint32 times;
		SetRecordRequest()
			:YaYaRequestBase(IM_TOOLS, IM_RECORD_SETINFO_REQ)
		{

		}

		YV_PARSER encode()
		{
			YV_PARSER parser = yvpacket_get_parser();
			parser_set_uint32(parser, x19014::times, times);
			return parser;
		}
	};

	//开始录音(最长60秒)  IM_RECORD_STRART_REQ	
	struct StartRecordRequest :YaYaRequestBase
	{
		std::string strFilePath;
		std::string  ext;
		StartRecordRequest()
			:YaYaRequestBase(IM_TOOLS, IM_RECORD_STRART_REQ)
		{

		}

		YV_PARSER encode()
		{
			YV_PARSER parser = yvpacket_get_parser();
			parser_set_cstring(parser, x19000::strfilepath, strFilePath.c_str());
			parser_set_cstring(parser, x19000::ext, ext.c_str());
			return parser;
		}
	};

	//结束录音(最长60秒)  IM_RECORD_STOP_REQ
	struct StopRecordRequest :YaYaRequestBase
	{
		StopRecordRequest()
		:YaYaRequestBase(IM_TOOLS, IM_RECORD_STOP_REQ)
		{
		}

		YV_PARSER encode()
		{
			YV_PARSER parser = yvpacket_get_parser();
			return parser;
		}
	};

	//停止录音返回  回调返回录音文件路径名  IM_RECORD_STOP_RESP
	struct RecordStopNotify :YaYaRespondBase
	{
		RecordStopNotify(){ strfilepath = NULL; }
		~RecordStopNotify(){  }
		
		uint32	time; //录音时长
		YVFilePathPtr strfilepath; //录音保存文件路径名
		std::string ext;
		
		void decode(YV_PARSER parser)
		{
			time = parser_get_uint32(parser, x19002::time);
			std::string path = parser_get_string(parser, x19002::strfilepath);
			ext = parser_get_string(parser, x19002::ext);
			//strfilepath = YVFilePath::getLocalPath(path);
		}
	};

	//录音音量大小通知 IM_RECORD_VOLUME_NOTIFY
	struct RecordVoiceNotify
		:YaYaRespondBase
	{
		std::string ext;  //扩展标记
		uint8       volume;  //音量大小(0-100)
		void decode(YV_PARSER parser)
		{
			volume = parser_get_uint8(parser, x19015::volume);
			ext = parser_get_string(parser, x19015::ext);
		}
	};

	//播放录音请求	IM_RECORD_STARTPLAY_REQ	
	struct StartPlayVoiceRequest :YaYaRequestBase
	{
		StartPlayVoiceRequest()
		:YaYaRequestBase(IM_TOOLS, IM_RECORD_STARTPLAY_REQ)
		{
		}
		std::string	strUrl;      // 录音url	
		std::string	strfilepath;  //录音文件
		std::string ext;
		YV_PARSER encode()
		{
			YV_PARSER parser = yvpacket_get_parser();
			parser_set_cstring(parser, x19003::strUrl, strUrl.c_str());
			parser_set_cstring(parser, x19003::strfilepath, strfilepath.c_str());
			parser_set_cstring(parser, x19003::ext, ext.c_str());
			return parser;
		}
	};

	//播放语音完成	IM_RECORD_FINISHPLAY_RESP
	struct StartPlayVoiceRespond :YaYaRespondBase
	{
		uint32    result; //播放完成为 0， 失败为1
		std::string    describe; //描述
		YVFilePathPtr filePath;

		std::string  ext;    
		void decode(YV_PARSER parser)
		{
			result = parser_get_uint32(parser, x19004::result);
			describe = parser_get_string(parser, x19004::describe);
			ext = parser_get_string(parser, x19004::ext);
		}
	};

	//停止播放语音  IM_RECORD_STOPPLAY_REQ
	struct StopPlayVoiceRequest :YaYaRequestBase
	{
		StopPlayVoiceRequest()
		:YaYaRequestBase(IM_TOOLS, IM_RECORD_STOPPLAY_REQ)
		{
		}

		YV_PARSER encode()
		{
			YV_PARSER parser = yvpacket_get_parser();
			return parser;
		}
	};

	//语音识别设置 IM_SPEECH_SETLANGUAGE_REQ
	struct SpeechSetRequest :YaYaRequestBase
	{
		SpeechSetRequest()
		:YaYaRequestBase(IM_TOOLS, IM_SPEECH_SETLANGUAGE_REQ)
		{
		}
		yvimspeech_language language;
		YV_PARSER encode()
		{
			YV_PARSER parser = yvpacket_get_parser();
			parser_set_uint8(parser, x19008::inlanguage, language);
			return parser;
		}
	};

	//开始语音识别 IM_SPEECH_START_REQ	
	struct SpeechStartRequest :YaYaRequestBase
	{
		SpeechStartRequest()
		:YaYaRequestBase(IM_TOOLS, IM_SPEECH_START_REQ)
		{
		}
		std::string strfilepath;
		std::string ext;
		//yvspeech   type;
		std::string    url;
		YV_PARSER encode()
		{
			YV_PARSER parser = yvpacket_get_parser();
			parser_set_cstring(parser, x19006::strfilepath, strfilepath.c_str());
			parser_set_cstring(parser, x19006::ext, ext.c_str());
			//parser_set_uint8(parser, x19006::type, type);
			//parser_set_cstring(parser, x19006::url, url.c_str());
			return parser;
		}
	};

	//停止语音识别回应 IM_SPEECH_STOP_RESP	
	struct SpeechStopRespond :YaYaRespondBase
	{
		uint32		err_id;   //0为成功
		std::string		err_msg;  //返回的错误描述
		std::string		result;   //结果
		std::string ext;

		YVFilePathPtr filePath;

		//std::string url;    //识别时使用了上传功能，这个会返回url
		void decode(YV_PARSER parser)
		{
			err_id = parser_get_uint32(parser, x19009::err_id);
			err_msg = parser_get_string(parser, x19009::err_msg);
			result = parser_get_string(parser, x19009::result);
			ext = parser_get_string(parser, x19009::ext);
			//url = parser_get_string(parser, x19009::url);
		}
	};

	//请求上传文件  IM_UPLOAD_FILE_REQ 
	struct UpLoadFileRequest :YaYaRequestBase
	{
		UpLoadFileRequest()
		:YaYaRequestBase(IM_TOOLS, IM_UPLOAD_FILE_REQ)
		{
		}
		std::string filename;
		std::string fileid;
		YV_PARSER encode()
		{
			YV_PARSER parser = yvpacket_get_parser();
			parser_set_cstring(parser, x19010::filename, filename.c_str());
			parser_set_cstring(parser, x19010::fileid, fileid.c_str());
			return parser;
		}
	};

	//上传文件请求回应 IM_UPLOAD_FILE_RESP
	struct UpLoadFileRespond :YaYaRespondBase
	{
		uint32	result;
		std::string  msg;        //错误描述
		std::string  fileid;     //文件ID
		std::string  fileurl;    //返回文件地址
		uint32  percent;    //完成百分比

		void decode(YV_PARSER parser)
		{
			result = parser_get_uint32(parser, x19011::result);
			msg = parser_get_string(parser, x19011::msg);
			fileid = parser_get_string(parser, x19011::fileid);
			fileurl = parser_get_string(parser, x19011::fileurl);
			percent = parser_get_uint32(parser, x19011::percent);
		}
	};

	//请求下载文件  IM_DOWNLOAD_FILE_REQ 
	struct DownLoadFileRequest :YaYaRequestBase
	{
		DownLoadFileRequest()
		:YaYaRequestBase(IM_TOOLS, IM_DOWNLOAD_FILE_REQ)
		{
		}
		std::string url;
		std::string filename;
		std::string fileid;
		YV_PARSER encode()
		{
			YV_PARSER parser = yvpacket_get_parser();
			parser_set_cstring(parser, x19012::url, url.c_str());
			parser_set_cstring(parser, x19012::filename, filename.c_str());
			parser_set_cstring(parser, x19012::fileid, fileid.c_str());
			return parser;
		}
	};

	//下载文件回应 IM_DOWNLOAD_FILE_RESP 
	struct DownLoadFileRespond :YaYaRespondBase
	{
		uint32	result;     //返回码 0：成功，其他失败
		std::string  msg;        //错误描述
		
		std::string  fileid;     //文件ID
		uint32  percent;    //完成百分比
		YVFilePathPtr  path;   //文件路径

		void decode(YV_PARSER parser)
		{
			result = parser_get_uint32(parser, x19013::result);
			msg = parser_get_string(parser, x19013::msg);
			std::string filename = parser_get_string(parser, x19013::filename);
			//path = YVFilePath::getLocalPath(filename);
			fileid = parser_get_string(parser, x19013::fileid);
			percent = parser_get_uint32(parser, x19013::percent);
		}
	};

	//设备号等操作IM_DEVICE_SETINFO
	struct SetDeviceInfoReqeust : YaYaRequestBase
	{
		SetDeviceInfoReqeust()
		:YaYaRequestBase(IM_LOGIN, IM_DEVICE_SETINFO)
		{
		}
		std::string imsi;
		std::string imei;
		std::string mac;
		std::string appVersion;
		std::string networkType;

		YV_PARSER encode()
		{
			YV_PARSER parser = yvpacket_get_parser();
			parser_set_cstring(parser, x11012::imsi, imsi.c_str());
			parser_set_cstring(parser, x11012::imei, imei.c_str());
			parser_set_cstring(parser, x11012::mac, mac.c_str());
			parser_set_cstring(parser, x11012::appVersion, appVersion.c_str());
			parser_set_cstring(parser, x11012::networkType, networkType.c_str());
			return parser;
		}
	};
	/****************************工具结束***************************/
};
#endif