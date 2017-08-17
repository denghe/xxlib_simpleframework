#include <xx_mempool.h>
#include <xx_bbuffer.h>

namespace PKG
{
    // 一个请求( 当前限定为 service 与 db 间 ), 通常携带一个流水号. 这是基类
    struct Request;
    // 一个回应( 当前限定为 service 与 db 间 ), 通常携带一个请求发过来的流水号. 这是基类
    struct Response;
    // 成功
    struct Success;
    // 失败
    struct Fail;
    struct Property;
    struct Property_long;
    struct Property_double;
    struct Property_string;
    struct Properties;
    struct UserInfo;
namespace Client_Server
{
    // 请求进入服务器
    struct Join;
    // 发消息
    struct Message;
    // 主动退出
    struct Logout;
}
namespace Server_Client
{
    // 进入成功, 返回用户信息
    struct JoinSuccess;
    // 进入失败, 返回错误信息
    struct JoinFail;
    // 推送文字消息
    struct PushJoin;
    // 推送文字消息
    struct PushMessage;
    // 推送退出消息
    struct PushLogout;
}
namespace Manage_DB
{
    // 请求进入服务器
    struct Login;
    // 注销式退出
    struct Logout;
    // 拉管理员账号列表的 id 列表( 需要相关权限 )
    struct SelectManageAccountIds;
    // 拉管理员账号列表( 需要相关权限 )
    struct SelectManageAccounts;
}
namespace Manage
{
    // 账号
    struct Account;
    // 身份
    struct Role;
    // 权限
    struct Permission;
    // 管理人员 & 身份 绑定
    struct BindAccountRole;
    // 身份 & 权限 绑定
    struct BindRolePermission;
}
namespace DB_Manage
{
    // 登录成功
    struct LoginSuccess;
    // 登录失败
    struct LoginFail;
    // 注销成功
    struct LogoutSuccess;
    // 注销失败
    struct LogoutFail;
    // 拉管理员账号列表的 id 列表 成功
    struct SelectManageAccountIdsSuccess;
    // 拉管理员账号列表的 id 列表 失败
    struct SelectManageAccountIdsFail;
    // 拉管理员账号列表 成功
    struct SelectManageAccountsSuccess;
    // 拉管理员账号列表 失败
    struct SelectManageAccountsFail;
}
namespace Manage
{
    // 账号获取时所用的排序规则
    enum class AccountSorts : int32_t
    {
        id_asc = 0,
        id_desc = 1,
        username_asc = 2,
        username_desc = 3,
    };
}
    // 一个回应( 当前限定为 service 与 db 间 ), 通常携带一个请求发过来的流水号. 这是基类
    struct Response : xx::Object
    {
        int32_t requestSerial = 0;

        typedef Response ThisType;
        typedef xx::Object BaseType;
	    Response();
	    Response(xx::BBuffer *bb);
	    ~Response();
        virtual void ToString(xx::String &str) const override;
        virtual void ToStringCore(xx::String &str) const override;
        virtual void ToBBuffer(xx::BBuffer &bb) const override;
        virtual int FromBBuffer(xx::BBuffer &bb) override;
    };
    // 一个请求( 当前限定为 service 与 db 间 ), 通常携带一个流水号. 这是基类
    struct Request : xx::Object
    {
        int32_t serial = 0;

        typedef Request ThisType;
        typedef xx::Object BaseType;
	    Request();
	    Request(xx::BBuffer *bb);
	    ~Request();
        virtual void ToString(xx::String &str) const override;
        virtual void ToStringCore(xx::String &str) const override;
        virtual void ToBBuffer(xx::BBuffer &bb) const override;
        virtual int FromBBuffer(xx::BBuffer &bb) override;
    };
    // 成功
    struct Success : PKG::Response
    {

        typedef Success ThisType;
        typedef PKG::Response BaseType;
	    Success();
	    Success(xx::BBuffer *bb);
	    ~Success();
        virtual void ToString(xx::String &str) const override;
        virtual void ToStringCore(xx::String &str) const override;
        virtual void ToBBuffer(xx::BBuffer &bb) const override;
        virtual int FromBBuffer(xx::BBuffer &bb) override;
    };
    // 失败
    struct Fail : PKG::Response
    {
        xx::String* reason = nullptr;

        typedef Fail ThisType;
        typedef PKG::Response BaseType;
	    Fail();
	    Fail(xx::BBuffer *bb);
	    ~Fail();
        virtual void ToString(xx::String &str) const override;
        virtual void ToStringCore(xx::String &str) const override;
        virtual void ToBBuffer(xx::BBuffer &bb) const override;
        virtual int FromBBuffer(xx::BBuffer &bb) override;
    };
    struct Property : xx::Object
    {
        xx::String* name = nullptr;

        typedef Property ThisType;
        typedef xx::Object BaseType;
	    Property();
	    Property(xx::BBuffer *bb);
	    ~Property();
        virtual void ToString(xx::String &str) const override;
        virtual void ToStringCore(xx::String &str) const override;
        virtual void ToBBuffer(xx::BBuffer &bb) const override;
        virtual int FromBBuffer(xx::BBuffer &bb) override;
    };
namespace Manage_DB
{
    // 拉管理员账号列表( 需要相关权限 )
    struct SelectManageAccounts : PKG::Request
    {
        // 要拉的 id 列表
        xx::List<int64_t>* ids = nullptr;

        typedef SelectManageAccounts ThisType;
        typedef PKG::Request BaseType;
	    SelectManageAccounts();
	    SelectManageAccounts(xx::BBuffer *bb);
	    ~SelectManageAccounts();
        virtual void ToString(xx::String &str) const override;
        virtual void ToStringCore(xx::String &str) const override;
        virtual void ToBBuffer(xx::BBuffer &bb) const override;
        virtual int FromBBuffer(xx::BBuffer &bb) override;
    };
}
namespace Manage
{
    // 账号
    struct Account : xx::Object
    {
        int64_t id = 0;
        xx::String* username = nullptr;

        typedef Account ThisType;
        typedef xx::Object BaseType;
	    Account();
	    Account(xx::BBuffer *bb);
	    ~Account();
        virtual void ToString(xx::String &str) const override;
        virtual void ToStringCore(xx::String &str) const override;
        virtual void ToBBuffer(xx::BBuffer &bb) const override;
        virtual int FromBBuffer(xx::BBuffer &bb) override;
    };
    // 身份
    struct Role : xx::Object
    {
        int32_t id = 0;
        xx::String* name = nullptr;
        xx::String* desc = nullptr;

        typedef Role ThisType;
        typedef xx::Object BaseType;
	    Role();
	    Role(xx::BBuffer *bb);
	    ~Role();
        virtual void ToString(xx::String &str) const override;
        virtual void ToStringCore(xx::String &str) const override;
        virtual void ToBBuffer(xx::BBuffer &bb) const override;
        virtual int FromBBuffer(xx::BBuffer &bb) override;
    };
    // 权限
    struct Permission : xx::Object
    {
        int32_t id = 0;
        xx::String* group = nullptr;
        xx::String* name = nullptr;
        xx::String* desc = nullptr;

        typedef Permission ThisType;
        typedef xx::Object BaseType;
	    Permission();
	    Permission(xx::BBuffer *bb);
	    ~Permission();
        virtual void ToString(xx::String &str) const override;
        virtual void ToStringCore(xx::String &str) const override;
        virtual void ToBBuffer(xx::BBuffer &bb) const override;
        virtual int FromBBuffer(xx::BBuffer &bb) override;
    };
    // 管理人员 & 身份 绑定
    struct BindAccountRole : xx::Object
    {
        int32_t account_id = 0;
        int32_t role_id = 0;

        typedef BindAccountRole ThisType;
        typedef xx::Object BaseType;
	    BindAccountRole();
	    BindAccountRole(xx::BBuffer *bb);
	    ~BindAccountRole();
        virtual void ToString(xx::String &str) const override;
        virtual void ToStringCore(xx::String &str) const override;
        virtual void ToBBuffer(xx::BBuffer &bb) const override;
        virtual int FromBBuffer(xx::BBuffer &bb) override;
    };
    // 身份 & 权限 绑定
    struct BindRolePermission : xx::Object
    {
        int32_t role_id = 0;
        int32_t permission_id = 0;

        typedef BindRolePermission ThisType;
        typedef xx::Object BaseType;
	    BindRolePermission();
	    BindRolePermission(xx::BBuffer *bb);
	    ~BindRolePermission();
        virtual void ToString(xx::String &str) const override;
        virtual void ToStringCore(xx::String &str) const override;
        virtual void ToBBuffer(xx::BBuffer &bb) const override;
        virtual int FromBBuffer(xx::BBuffer &bb) override;
    };
}
namespace DB_Manage
{
    // 登录失败
    struct LoginFail : PKG::Fail
    {

        typedef LoginFail ThisType;
        typedef PKG::Fail BaseType;
	    LoginFail();
	    LoginFail(xx::BBuffer *bb);
	    ~LoginFail();
        virtual void ToString(xx::String &str) const override;
        virtual void ToStringCore(xx::String &str) const override;
        virtual void ToBBuffer(xx::BBuffer &bb) const override;
        virtual int FromBBuffer(xx::BBuffer &bb) override;
    };
}
namespace Manage_DB
{
    // 拉管理员账号列表的 id 列表( 需要相关权限 )
    struct SelectManageAccountIds : PKG::Request
    {
        // 限制获取的 id 总个数
        int32_t limit = 0;
        // 字段排序规则( 当前先只支持对一个字段排序 )
        PKG::Manage::AccountSorts sort = (PKG::Manage::AccountSorts)0;

        typedef SelectManageAccountIds ThisType;
        typedef PKG::Request BaseType;
	    SelectManageAccountIds();
	    SelectManageAccountIds(xx::BBuffer *bb);
	    ~SelectManageAccountIds();
        virtual void ToString(xx::String &str) const override;
        virtual void ToStringCore(xx::String &str) const override;
        virtual void ToBBuffer(xx::BBuffer &bb) const override;
        virtual int FromBBuffer(xx::BBuffer &bb) override;
    };
}
namespace DB_Manage
{
    // 注销成功
    struct LogoutSuccess : PKG::Success
    {

        typedef LogoutSuccess ThisType;
        typedef PKG::Success BaseType;
	    LogoutSuccess();
	    LogoutSuccess(xx::BBuffer *bb);
	    ~LogoutSuccess();
        virtual void ToString(xx::String &str) const override;
        virtual void ToStringCore(xx::String &str) const override;
        virtual void ToBBuffer(xx::BBuffer &bb) const override;
        virtual int FromBBuffer(xx::BBuffer &bb) override;
    };
    // 注销失败
    struct LogoutFail : PKG::Fail
    {

        typedef LogoutFail ThisType;
        typedef PKG::Fail BaseType;
	    LogoutFail();
	    LogoutFail(xx::BBuffer *bb);
	    ~LogoutFail();
        virtual void ToString(xx::String &str) const override;
        virtual void ToStringCore(xx::String &str) const override;
        virtual void ToBBuffer(xx::BBuffer &bb) const override;
        virtual int FromBBuffer(xx::BBuffer &bb) override;
    };
    // 拉管理员账号列表的 id 列表 成功
    struct SelectManageAccountIdsSuccess : PKG::Success
    {
        xx::List<int64_t>* ids = nullptr;

        typedef SelectManageAccountIdsSuccess ThisType;
        typedef PKG::Success BaseType;
	    SelectManageAccountIdsSuccess();
	    SelectManageAccountIdsSuccess(xx::BBuffer *bb);
	    ~SelectManageAccountIdsSuccess();
        virtual void ToString(xx::String &str) const override;
        virtual void ToStringCore(xx::String &str) const override;
        virtual void ToBBuffer(xx::BBuffer &bb) const override;
        virtual int FromBBuffer(xx::BBuffer &bb) override;
    };
    // 拉管理员账号列表的 id 列表 失败
    struct SelectManageAccountIdsFail : PKG::Fail
    {

        typedef SelectManageAccountIdsFail ThisType;
        typedef PKG::Fail BaseType;
	    SelectManageAccountIdsFail();
	    SelectManageAccountIdsFail(xx::BBuffer *bb);
	    ~SelectManageAccountIdsFail();
        virtual void ToString(xx::String &str) const override;
        virtual void ToStringCore(xx::String &str) const override;
        virtual void ToBBuffer(xx::BBuffer &bb) const override;
        virtual int FromBBuffer(xx::BBuffer &bb) override;
    };
    // 登录成功
    struct LoginSuccess : PKG::Success
    {
        int64_t id = 0;
        xx::String* token = nullptr;

        typedef LoginSuccess ThisType;
        typedef PKG::Success BaseType;
	    LoginSuccess();
	    LoginSuccess(xx::BBuffer *bb);
	    ~LoginSuccess();
        virtual void ToString(xx::String &str) const override;
        virtual void ToStringCore(xx::String &str) const override;
        virtual void ToBBuffer(xx::BBuffer &bb) const override;
        virtual int FromBBuffer(xx::BBuffer &bb) override;
    };
}
namespace Manage_DB
{
    // 注销式退出
    struct Logout : PKG::Request
    {

        typedef Logout ThisType;
        typedef PKG::Request BaseType;
	    Logout();
	    Logout(xx::BBuffer *bb);
	    ~Logout();
        virtual void ToString(xx::String &str) const override;
        virtual void ToStringCore(xx::String &str) const override;
        virtual void ToBBuffer(xx::BBuffer &bb) const override;
        virtual int FromBBuffer(xx::BBuffer &bb) override;
    };
}
namespace Server_Client
{
    // 推送退出消息
    struct PushLogout : xx::Object
    {
        int64_t id = 0;
        // 退出原因( 主动? 掉线?
        xx::String* reason = nullptr;

        typedef PushLogout ThisType;
        typedef xx::Object BaseType;
	    PushLogout();
	    PushLogout(xx::BBuffer *bb);
	    ~PushLogout();
        virtual void ToString(xx::String &str) const override;
        virtual void ToStringCore(xx::String &str) const override;
        virtual void ToBBuffer(xx::BBuffer &bb) const override;
        virtual int FromBBuffer(xx::BBuffer &bb) override;
    };
}
namespace DB_Manage
{
    // 拉管理员账号列表 成功
    struct SelectManageAccountsSuccess : PKG::Success
    {
        xx::List<PKG::Manage::Account*>* rows = nullptr;

        typedef SelectManageAccountsSuccess ThisType;
        typedef PKG::Success BaseType;
	    SelectManageAccountsSuccess();
	    SelectManageAccountsSuccess(xx::BBuffer *bb);
	    ~SelectManageAccountsSuccess();
        virtual void ToString(xx::String &str) const override;
        virtual void ToStringCore(xx::String &str) const override;
        virtual void ToBBuffer(xx::BBuffer &bb) const override;
        virtual int FromBBuffer(xx::BBuffer &bb) override;
    };
}
namespace Server_Client
{
    // 推送文字消息
    struct PushMessage : xx::Object
    {
        int64_t id = 0;
        xx::String* text = nullptr;

        typedef PushMessage ThisType;
        typedef xx::Object BaseType;
	    PushMessage();
	    PushMessage(xx::BBuffer *bb);
	    ~PushMessage();
        virtual void ToString(xx::String &str) const override;
        virtual void ToStringCore(xx::String &str) const override;
        virtual void ToBBuffer(xx::BBuffer &bb) const override;
        virtual int FromBBuffer(xx::BBuffer &bb) override;
    };
    // 推送文字消息
    struct PushJoin : xx::Object
    {
        int64_t id = 0;

        typedef PushJoin ThisType;
        typedef xx::Object BaseType;
	    PushJoin();
	    PushJoin(xx::BBuffer *bb);
	    ~PushJoin();
        virtual void ToString(xx::String &str) const override;
        virtual void ToStringCore(xx::String &str) const override;
        virtual void ToBBuffer(xx::BBuffer &bb) const override;
        virtual int FromBBuffer(xx::BBuffer &bb) override;
    };
    // 进入失败, 返回错误信息
    struct JoinFail : PKG::Response
    {
        xx::String* reason = nullptr;

        typedef JoinFail ThisType;
        typedef PKG::Response BaseType;
	    JoinFail();
	    JoinFail(xx::BBuffer *bb);
	    ~JoinFail();
        virtual void ToString(xx::String &str) const override;
        virtual void ToStringCore(xx::String &str) const override;
        virtual void ToBBuffer(xx::BBuffer &bb) const override;
        virtual int FromBBuffer(xx::BBuffer &bb) override;
    };
    // 进入成功, 返回用户信息
    struct JoinSuccess : PKG::Response
    {
        PKG::UserInfo* self = nullptr;
        xx::List<PKG::UserInfo*>* users = nullptr;

        typedef JoinSuccess ThisType;
        typedef PKG::Response BaseType;
	    JoinSuccess();
	    JoinSuccess(xx::BBuffer *bb);
	    ~JoinSuccess();
        virtual void ToString(xx::String &str) const override;
        virtual void ToStringCore(xx::String &str) const override;
        virtual void ToBBuffer(xx::BBuffer &bb) const override;
        virtual int FromBBuffer(xx::BBuffer &bb) override;
    };
}
namespace Client_Server
{
    // 主动退出
    struct Logout : xx::Object
    {

        typedef Logout ThisType;
        typedef xx::Object BaseType;
	    Logout();
	    Logout(xx::BBuffer *bb);
	    ~Logout();
        virtual void ToString(xx::String &str) const override;
        virtual void ToStringCore(xx::String &str) const override;
        virtual void ToBBuffer(xx::BBuffer &bb) const override;
        virtual int FromBBuffer(xx::BBuffer &bb) override;
    };
    // 发消息
    struct Message : xx::Object
    {
        xx::String* text = nullptr;

        typedef Message ThisType;
        typedef xx::Object BaseType;
	    Message();
	    Message(xx::BBuffer *bb);
	    ~Message();
        virtual void ToString(xx::String &str) const override;
        virtual void ToStringCore(xx::String &str) const override;
        virtual void ToBBuffer(xx::BBuffer &bb) const override;
        virtual int FromBBuffer(xx::BBuffer &bb) override;
    };
    // 请求进入服务器
    struct Join : PKG::Request
    {
        xx::String* username = nullptr;
        xx::String* password = nullptr;

        typedef Join ThisType;
        typedef PKG::Request BaseType;
	    Join();
	    Join(xx::BBuffer *bb);
	    ~Join();
        virtual void ToString(xx::String &str) const override;
        virtual void ToStringCore(xx::String &str) const override;
        virtual void ToBBuffer(xx::BBuffer &bb) const override;
        virtual int FromBBuffer(xx::BBuffer &bb) override;
    };
}
    struct UserInfo : xx::Object
    {
        int64_t id = 0;
        xx::List<PKG::Property*>* props = nullptr;

        typedef UserInfo ThisType;
        typedef xx::Object BaseType;
	    UserInfo();
	    UserInfo(xx::BBuffer *bb);
	    ~UserInfo();
        virtual void ToString(xx::String &str) const override;
        virtual void ToStringCore(xx::String &str) const override;
        virtual void ToBBuffer(xx::BBuffer &bb) const override;
        virtual int FromBBuffer(xx::BBuffer &bb) override;
    };
    struct Properties : PKG::Property
    {
        xx::List<PKG::Property*>* value = nullptr;

        typedef Properties ThisType;
        typedef PKG::Property BaseType;
	    Properties();
	    Properties(xx::BBuffer *bb);
	    ~Properties();
        virtual void ToString(xx::String &str) const override;
        virtual void ToStringCore(xx::String &str) const override;
        virtual void ToBBuffer(xx::BBuffer &bb) const override;
        virtual int FromBBuffer(xx::BBuffer &bb) override;
    };
    struct Property_string : PKG::Property
    {
        xx::String* value = nullptr;

        typedef Property_string ThisType;
        typedef PKG::Property BaseType;
	    Property_string();
	    Property_string(xx::BBuffer *bb);
	    ~Property_string();
        virtual void ToString(xx::String &str) const override;
        virtual void ToStringCore(xx::String &str) const override;
        virtual void ToBBuffer(xx::BBuffer &bb) const override;
        virtual int FromBBuffer(xx::BBuffer &bb) override;
    };
    struct Property_double : PKG::Property
    {
        double value = 0;

        typedef Property_double ThisType;
        typedef PKG::Property BaseType;
	    Property_double();
	    Property_double(xx::BBuffer *bb);
	    ~Property_double();
        virtual void ToString(xx::String &str) const override;
        virtual void ToStringCore(xx::String &str) const override;
        virtual void ToBBuffer(xx::BBuffer &bb) const override;
        virtual int FromBBuffer(xx::BBuffer &bb) override;
    };
    struct Property_long : PKG::Property
    {
        int64_t value = 0;

        typedef Property_long ThisType;
        typedef PKG::Property BaseType;
	    Property_long();
	    Property_long(xx::BBuffer *bb);
	    ~Property_long();
        virtual void ToString(xx::String &str) const override;
        virtual void ToStringCore(xx::String &str) const override;
        virtual void ToBBuffer(xx::BBuffer &bb) const override;
        virtual int FromBBuffer(xx::BBuffer &bb) override;
    };
namespace Manage_DB
{
    // 请求进入服务器
    struct Login : PKG::Request
    {
        xx::String* username = nullptr;
        xx::String* password = nullptr;

        typedef Login ThisType;
        typedef PKG::Request BaseType;
	    Login();
	    Login(xx::BBuffer *bb);
	    ~Login();
        virtual void ToString(xx::String &str) const override;
        virtual void ToStringCore(xx::String &str) const override;
        virtual void ToBBuffer(xx::BBuffer &bb) const override;
        virtual int FromBBuffer(xx::BBuffer &bb) override;
    };
}
namespace DB_Manage
{
    // 拉管理员账号列表 失败
    struct SelectManageAccountsFail : PKG::Fail
    {

        typedef SelectManageAccountsFail ThisType;
        typedef PKG::Fail BaseType;
	    SelectManageAccountsFail();
	    SelectManageAccountsFail(xx::BBuffer *bb);
	    ~SelectManageAccountsFail();
        virtual void ToString(xx::String &str) const override;
        virtual void ToStringCore(xx::String &str) const override;
        virtual void ToBBuffer(xx::BBuffer &bb) const override;
        virtual int FromBBuffer(xx::BBuffer &bb) override;
    };
}
	inline Request::Request()
	{
	}
	inline Request::Request(xx::BBuffer *bb)
	{
	    int rtv = 0;
        if (rtv = bb->Read(serial)) throw rtv;
	}
	inline Request::~Request()
	{
	}

    inline void Request::ToString(xx::String &str) const
    {
        if (tsFlags())
        {
        	str.Append("[ \"***** recursived *****\" ]");
        	return;
        }
        else tsFlags() = 1;

        str.Append("{ \"type\" : \"Request\"");
        ToStringCore(str);
        str.Append(" }");
        
        tsFlags() = 0;
    }
    inline void Request::ToStringCore(xx::String &str) const
    {
        this->BaseType::ToStringCore(str);
        str.Append(", \"serial\" : ", this->serial);
    }


    inline void Request::ToBBuffer(xx::BBuffer &bb) const
    {
        bb.Write(this->serial);
    }

    inline int Request::FromBBuffer(xx::BBuffer &bb)
    {
        int rtv = 0;
        if (rtv = bb.Read(this->serial)) return rtv;
        return rtv;
    }

	inline Response::Response()
	{
	}
	inline Response::Response(xx::BBuffer *bb)
	{
	    int rtv = 0;
        if (rtv = bb->Read(requestSerial)) throw rtv;
	}
	inline Response::~Response()
	{
	}

    inline void Response::ToString(xx::String &str) const
    {
        if (tsFlags())
        {
        	str.Append("[ \"***** recursived *****\" ]");
        	return;
        }
        else tsFlags() = 1;

        str.Append("{ \"type\" : \"Response\"");
        ToStringCore(str);
        str.Append(" }");
        
        tsFlags() = 0;
    }
    inline void Response::ToStringCore(xx::String &str) const
    {
        this->BaseType::ToStringCore(str);
        str.Append(", \"requestSerial\" : ", this->requestSerial);
    }


    inline void Response::ToBBuffer(xx::BBuffer &bb) const
    {
        bb.Write(this->requestSerial);
    }

    inline int Response::FromBBuffer(xx::BBuffer &bb)
    {
        int rtv = 0;
        if (rtv = bb.Read(this->requestSerial)) return rtv;
        return rtv;
    }

	inline Success::Success()
        : PKG::Response()
	{
	}
	inline Success::Success(xx::BBuffer *bb)
        : PKG::Response(bb)
	{
	}
	inline Success::~Success()
	{
	}

    inline void Success::ToString(xx::String &str) const
    {
        if (tsFlags())
        {
        	str.Append("[ \"***** recursived *****\" ]");
        	return;
        }
        else tsFlags() = 1;

        str.Append("{ \"type\" : \"Success\"");
        ToStringCore(str);
        str.Append(" }");
        
        tsFlags() = 0;
    }
    inline void Success::ToStringCore(xx::String &str) const
    {
        this->BaseType::ToStringCore(str);
    }


    inline void Success::ToBBuffer(xx::BBuffer &bb) const
    {
        this->BaseType::ToBBuffer(bb);
    }

    inline int Success::FromBBuffer(xx::BBuffer &bb)
    {
        int rtv = 0;
        if (rtv = this->BaseType::FromBBuffer(bb)) return rtv;
        return rtv;
    }

	inline Fail::Fail()
        : PKG::Response()
	{
	}
	inline Fail::Fail(xx::BBuffer *bb)
        : PKG::Response(bb)
	{
	    int rtv = 0;
        bb->readLengthLimit = 0;
        if (rtv = bb->Read(reason)) throw rtv;
	}
	inline Fail::~Fail()
	{
        mempool().SafeRelease(reason);
	}

    inline void Fail::ToString(xx::String &str) const
    {
        if (tsFlags())
        {
        	str.Append("[ \"***** recursived *****\" ]");
        	return;
        }
        else tsFlags() = 1;

        str.Append("{ \"type\" : \"Fail\"");
        ToStringCore(str);
        str.Append(" }");
        
        tsFlags() = 0;
    }
    inline void Fail::ToStringCore(xx::String &str) const
    {
        this->BaseType::ToStringCore(str);
        str.Append(", \"reason\" : ", this->reason);
    }


    inline void Fail::ToBBuffer(xx::BBuffer &bb) const
    {
        this->BaseType::ToBBuffer(bb);
        bb.Write(this->reason);
    }

    inline int Fail::FromBBuffer(xx::BBuffer &bb)
    {
        int rtv = 0;
        if (rtv = this->BaseType::FromBBuffer(bb)) return rtv;
        bb.readLengthLimit = 0;
        if (rtv = bb.Read(this->reason)) return rtv;
        return rtv;
    }

	inline Property::Property()
	{
	}
	inline Property::Property(xx::BBuffer *bb)
	{
	    int rtv = 0;
        bb->readLengthLimit = 0;
        if (rtv = bb->Read(name)) throw rtv;
	}
	inline Property::~Property()
	{
        mempool().SafeRelease(name);
	}

    inline void Property::ToString(xx::String &str) const
    {
        if (tsFlags())
        {
        	str.Append("[ \"***** recursived *****\" ]");
        	return;
        }
        else tsFlags() = 1;

        str.Append("{ \"type\" : \"Property\"");
        ToStringCore(str);
        str.Append(" }");
        
        tsFlags() = 0;
    }
    inline void Property::ToStringCore(xx::String &str) const
    {
        this->BaseType::ToStringCore(str);
        str.Append(", \"name\" : ", this->name);
    }


    inline void Property::ToBBuffer(xx::BBuffer &bb) const
    {
        bb.Write(this->name);
    }

    inline int Property::FromBBuffer(xx::BBuffer &bb)
    {
        int rtv = 0;
        bb.readLengthLimit = 0;
        if (rtv = bb.Read(this->name)) return rtv;
        return rtv;
    }

	inline Property_long::Property_long()
        : PKG::Property()
	{
	}
	inline Property_long::Property_long(xx::BBuffer *bb)
        : PKG::Property(bb)
	{
	    int rtv = 0;
        if (rtv = bb->Read(value)) throw rtv;
	}
	inline Property_long::~Property_long()
	{
	}

    inline void Property_long::ToString(xx::String &str) const
    {
        if (tsFlags())
        {
        	str.Append("[ \"***** recursived *****\" ]");
        	return;
        }
        else tsFlags() = 1;

        str.Append("{ \"type\" : \"Property_long\"");
        ToStringCore(str);
        str.Append(" }");
        
        tsFlags() = 0;
    }
    inline void Property_long::ToStringCore(xx::String &str) const
    {
        this->BaseType::ToStringCore(str);
        str.Append(", \"value\" : ", this->value);
    }


    inline void Property_long::ToBBuffer(xx::BBuffer &bb) const
    {
        this->BaseType::ToBBuffer(bb);
        bb.Write(this->value);
    }

    inline int Property_long::FromBBuffer(xx::BBuffer &bb)
    {
        int rtv = 0;
        if (rtv = this->BaseType::FromBBuffer(bb)) return rtv;
        if (rtv = bb.Read(this->value)) return rtv;
        return rtv;
    }

	inline Property_double::Property_double()
        : PKG::Property()
	{
	}
	inline Property_double::Property_double(xx::BBuffer *bb)
        : PKG::Property(bb)
	{
	    int rtv = 0;
        if (rtv = bb->Read(value)) throw rtv;
	}
	inline Property_double::~Property_double()
	{
	}

    inline void Property_double::ToString(xx::String &str) const
    {
        if (tsFlags())
        {
        	str.Append("[ \"***** recursived *****\" ]");
        	return;
        }
        else tsFlags() = 1;

        str.Append("{ \"type\" : \"Property_double\"");
        ToStringCore(str);
        str.Append(" }");
        
        tsFlags() = 0;
    }
    inline void Property_double::ToStringCore(xx::String &str) const
    {
        this->BaseType::ToStringCore(str);
        str.Append(", \"value\" : ", this->value);
    }


    inline void Property_double::ToBBuffer(xx::BBuffer &bb) const
    {
        this->BaseType::ToBBuffer(bb);
        bb.Write(this->value);
    }

    inline int Property_double::FromBBuffer(xx::BBuffer &bb)
    {
        int rtv = 0;
        if (rtv = this->BaseType::FromBBuffer(bb)) return rtv;
        if (rtv = bb.Read(this->value)) return rtv;
        return rtv;
    }

	inline Property_string::Property_string()
        : PKG::Property()
	{
	}
	inline Property_string::Property_string(xx::BBuffer *bb)
        : PKG::Property(bb)
	{
	    int rtv = 0;
        bb->readLengthLimit = 0;
        if (rtv = bb->Read(value)) throw rtv;
	}
	inline Property_string::~Property_string()
	{
        mempool().SafeRelease(value);
	}

    inline void Property_string::ToString(xx::String &str) const
    {
        if (tsFlags())
        {
        	str.Append("[ \"***** recursived *****\" ]");
        	return;
        }
        else tsFlags() = 1;

        str.Append("{ \"type\" : \"Property_string\"");
        ToStringCore(str);
        str.Append(" }");
        
        tsFlags() = 0;
    }
    inline void Property_string::ToStringCore(xx::String &str) const
    {
        this->BaseType::ToStringCore(str);
        str.Append(", \"value\" : ", this->value);
    }


    inline void Property_string::ToBBuffer(xx::BBuffer &bb) const
    {
        this->BaseType::ToBBuffer(bb);
        bb.Write(this->value);
    }

    inline int Property_string::FromBBuffer(xx::BBuffer &bb)
    {
        int rtv = 0;
        if (rtv = this->BaseType::FromBBuffer(bb)) return rtv;
        bb.readLengthLimit = 0;
        if (rtv = bb.Read(this->value)) return rtv;
        return rtv;
    }

	inline Properties::Properties()
        : PKG::Property()
	{
	}
	inline Properties::Properties(xx::BBuffer *bb)
        : PKG::Property(bb)
	{
	    int rtv = 0;
        bb->readLengthLimit = 0;
        if (rtv = bb->Read(value)) throw rtv;
	}
	inline Properties::~Properties()
	{
        mempool().SafeRelease(value);
	}

    inline void Properties::ToString(xx::String &str) const
    {
        if (tsFlags())
        {
        	str.Append("[ \"***** recursived *****\" ]");
        	return;
        }
        else tsFlags() = 1;

        str.Append("{ \"type\" : \"Properties\"");
        ToStringCore(str);
        str.Append(" }");
        
        tsFlags() = 0;
    }
    inline void Properties::ToStringCore(xx::String &str) const
    {
        this->BaseType::ToStringCore(str);
        str.Append(", \"value\" : ", this->value);
    }


    inline void Properties::ToBBuffer(xx::BBuffer &bb) const
    {
        this->BaseType::ToBBuffer(bb);
        bb.Write(this->value);
    }

    inline int Properties::FromBBuffer(xx::BBuffer &bb)
    {
        int rtv = 0;
        if (rtv = this->BaseType::FromBBuffer(bb)) return rtv;
        bb.readLengthLimit = 0;
        if (rtv = bb.Read(this->value)) return rtv;
        return rtv;
    }

	inline UserInfo::UserInfo()
	{
	}
	inline UserInfo::UserInfo(xx::BBuffer *bb)
	{
	    int rtv = 0;
        if (rtv = bb->Read(id)) throw rtv;
        bb->readLengthLimit = 0;
        if (rtv = bb->Read(props)) throw rtv;
	}
	inline UserInfo::~UserInfo()
	{
        mempool().SafeRelease(props);
	}

    inline void UserInfo::ToString(xx::String &str) const
    {
        if (tsFlags())
        {
        	str.Append("[ \"***** recursived *****\" ]");
        	return;
        }
        else tsFlags() = 1;

        str.Append("{ \"type\" : \"UserInfo\"");
        ToStringCore(str);
        str.Append(" }");
        
        tsFlags() = 0;
    }
    inline void UserInfo::ToStringCore(xx::String &str) const
    {
        this->BaseType::ToStringCore(str);
        str.Append(", \"id\" : ", this->id);
        str.Append(", \"props\" : ", this->props);
    }


    inline void UserInfo::ToBBuffer(xx::BBuffer &bb) const
    {
        bb.Write(this->id);
        bb.Write(this->props);
    }

    inline int UserInfo::FromBBuffer(xx::BBuffer &bb)
    {
        int rtv = 0;
        if (rtv = bb.Read(this->id)) return rtv;
        bb.readLengthLimit = 0;
        if (rtv = bb.Read(this->props)) return rtv;
        return rtv;
    }

namespace Client_Server
{
	inline Join::Join()
        : PKG::Request()
	{
	}
	inline Join::Join(xx::BBuffer *bb)
        : PKG::Request(bb)
	{
	    int rtv = 0;
        bb->readLengthLimit = 16;
        if (rtv = bb->Read(username)) throw rtv;
        bb->readLengthLimit = 16;
        if (rtv = bb->Read(password)) throw rtv;
	}
	inline Join::~Join()
	{
        mempool().SafeRelease(username);
        mempool().SafeRelease(password);
	}

    inline void Join::ToString(xx::String &str) const
    {
        if (tsFlags())
        {
        	str.Append("[ \"***** recursived *****\" ]");
        	return;
        }
        else tsFlags() = 1;

        str.Append("{ \"type\" : \"Join\"");
        ToStringCore(str);
        str.Append(" }");
        
        tsFlags() = 0;
    }
    inline void Join::ToStringCore(xx::String &str) const
    {
        this->BaseType::ToStringCore(str);
        str.Append(", \"username\" : ", this->username);
        str.Append(", \"password\" : ", this->password);
    }


    inline void Join::ToBBuffer(xx::BBuffer &bb) const
    {
        this->BaseType::ToBBuffer(bb);
        bb.Write(this->username);
        bb.Write(this->password);
    }

    inline int Join::FromBBuffer(xx::BBuffer &bb)
    {
        int rtv = 0;
        if (rtv = this->BaseType::FromBBuffer(bb)) return rtv;
        bb.readLengthLimit = 16;
        if (rtv = bb.Read(this->username)) return rtv;
        bb.readLengthLimit = 16;
        if (rtv = bb.Read(this->password)) return rtv;
        return rtv;
    }

	inline Message::Message()
	{
	}
	inline Message::Message(xx::BBuffer *bb)
	{
	    int rtv = 0;
        bb->readLengthLimit = 256;
        if (rtv = bb->Read(text)) throw rtv;
	}
	inline Message::~Message()
	{
        mempool().SafeRelease(text);
	}

    inline void Message::ToString(xx::String &str) const
    {
        if (tsFlags())
        {
        	str.Append("[ \"***** recursived *****\" ]");
        	return;
        }
        else tsFlags() = 1;

        str.Append("{ \"type\" : \"Message\"");
        ToStringCore(str);
        str.Append(" }");
        
        tsFlags() = 0;
    }
    inline void Message::ToStringCore(xx::String &str) const
    {
        this->BaseType::ToStringCore(str);
        str.Append(", \"text\" : ", this->text);
    }


    inline void Message::ToBBuffer(xx::BBuffer &bb) const
    {
        bb.Write(this->text);
    }

    inline int Message::FromBBuffer(xx::BBuffer &bb)
    {
        int rtv = 0;
        bb.readLengthLimit = 256;
        if (rtv = bb.Read(this->text)) return rtv;
        return rtv;
    }

	inline Logout::Logout()
	{
	}
	inline Logout::Logout(xx::BBuffer *bb)
	{
	}
	inline Logout::~Logout()
	{
	}

    inline void Logout::ToString(xx::String &str) const
    {
        if (tsFlags())
        {
        	str.Append("[ \"***** recursived *****\" ]");
        	return;
        }
        else tsFlags() = 1;

        str.Append("{ \"type\" : \"Logout\"");
        ToStringCore(str);
        str.Append(" }");
        
        tsFlags() = 0;
    }
    inline void Logout::ToStringCore(xx::String &str) const
    {
        this->BaseType::ToStringCore(str);
    }


    inline void Logout::ToBBuffer(xx::BBuffer &bb) const
    {
    }

    inline int Logout::FromBBuffer(xx::BBuffer &bb)
    {
        int rtv = 0;
        return rtv;
    }

}
namespace Server_Client
{
	inline JoinSuccess::JoinSuccess()
        : PKG::Response()
	{
	}
	inline JoinSuccess::JoinSuccess(xx::BBuffer *bb)
        : PKG::Response(bb)
	{
	    int rtv = 0;
        if (rtv = bb->Read(self)) throw rtv;
        bb->readLengthLimit = 0;
        if (rtv = bb->Read(users)) throw rtv;
	}
	inline JoinSuccess::~JoinSuccess()
	{
        mempool().SafeRelease(self);
        mempool().SafeRelease(users);
	}

    inline void JoinSuccess::ToString(xx::String &str) const
    {
        if (tsFlags())
        {
        	str.Append("[ \"***** recursived *****\" ]");
        	return;
        }
        else tsFlags() = 1;

        str.Append("{ \"type\" : \"JoinSuccess\"");
        ToStringCore(str);
        str.Append(" }");
        
        tsFlags() = 0;
    }
    inline void JoinSuccess::ToStringCore(xx::String &str) const
    {
        this->BaseType::ToStringCore(str);
        str.Append(", \"self\" : ", this->self);
        str.Append(", \"users\" : ", this->users);
    }


    inline void JoinSuccess::ToBBuffer(xx::BBuffer &bb) const
    {
        this->BaseType::ToBBuffer(bb);
        bb.Write(this->self);
        bb.Write(this->users);
    }

    inline int JoinSuccess::FromBBuffer(xx::BBuffer &bb)
    {
        int rtv = 0;
        if (rtv = this->BaseType::FromBBuffer(bb)) return rtv;
        if (rtv = bb.Read(this->self)) return rtv;
        bb.readLengthLimit = 0;
        if (rtv = bb.Read(this->users)) return rtv;
        return rtv;
    }

	inline JoinFail::JoinFail()
        : PKG::Response()
	{
	}
	inline JoinFail::JoinFail(xx::BBuffer *bb)
        : PKG::Response(bb)
	{
	    int rtv = 0;
        bb->readLengthLimit = 0;
        if (rtv = bb->Read(reason)) throw rtv;
	}
	inline JoinFail::~JoinFail()
	{
        mempool().SafeRelease(reason);
	}

    inline void JoinFail::ToString(xx::String &str) const
    {
        if (tsFlags())
        {
        	str.Append("[ \"***** recursived *****\" ]");
        	return;
        }
        else tsFlags() = 1;

        str.Append("{ \"type\" : \"JoinFail\"");
        ToStringCore(str);
        str.Append(" }");
        
        tsFlags() = 0;
    }
    inline void JoinFail::ToStringCore(xx::String &str) const
    {
        this->BaseType::ToStringCore(str);
        str.Append(", \"reason\" : ", this->reason);
    }


    inline void JoinFail::ToBBuffer(xx::BBuffer &bb) const
    {
        this->BaseType::ToBBuffer(bb);
        bb.Write(this->reason);
    }

    inline int JoinFail::FromBBuffer(xx::BBuffer &bb)
    {
        int rtv = 0;
        if (rtv = this->BaseType::FromBBuffer(bb)) return rtv;
        bb.readLengthLimit = 0;
        if (rtv = bb.Read(this->reason)) return rtv;
        return rtv;
    }

	inline PushJoin::PushJoin()
	{
	}
	inline PushJoin::PushJoin(xx::BBuffer *bb)
	{
	    int rtv = 0;
        if (rtv = bb->Read(id)) throw rtv;
	}
	inline PushJoin::~PushJoin()
	{
	}

    inline void PushJoin::ToString(xx::String &str) const
    {
        if (tsFlags())
        {
        	str.Append("[ \"***** recursived *****\" ]");
        	return;
        }
        else tsFlags() = 1;

        str.Append("{ \"type\" : \"PushJoin\"");
        ToStringCore(str);
        str.Append(" }");
        
        tsFlags() = 0;
    }
    inline void PushJoin::ToStringCore(xx::String &str) const
    {
        this->BaseType::ToStringCore(str);
        str.Append(", \"id\" : ", this->id);
    }


    inline void PushJoin::ToBBuffer(xx::BBuffer &bb) const
    {
        bb.Write(this->id);
    }

    inline int PushJoin::FromBBuffer(xx::BBuffer &bb)
    {
        int rtv = 0;
        if (rtv = bb.Read(this->id)) return rtv;
        return rtv;
    }

	inline PushMessage::PushMessage()
	{
	}
	inline PushMessage::PushMessage(xx::BBuffer *bb)
	{
	    int rtv = 0;
        if (rtv = bb->Read(id)) throw rtv;
        bb->readLengthLimit = 0;
        if (rtv = bb->Read(text)) throw rtv;
	}
	inline PushMessage::~PushMessage()
	{
        mempool().SafeRelease(text);
	}

    inline void PushMessage::ToString(xx::String &str) const
    {
        if (tsFlags())
        {
        	str.Append("[ \"***** recursived *****\" ]");
        	return;
        }
        else tsFlags() = 1;

        str.Append("{ \"type\" : \"PushMessage\"");
        ToStringCore(str);
        str.Append(" }");
        
        tsFlags() = 0;
    }
    inline void PushMessage::ToStringCore(xx::String &str) const
    {
        this->BaseType::ToStringCore(str);
        str.Append(", \"id\" : ", this->id);
        str.Append(", \"text\" : ", this->text);
    }


    inline void PushMessage::ToBBuffer(xx::BBuffer &bb) const
    {
        bb.Write(this->id);
        bb.Write(this->text);
    }

    inline int PushMessage::FromBBuffer(xx::BBuffer &bb)
    {
        int rtv = 0;
        if (rtv = bb.Read(this->id)) return rtv;
        bb.readLengthLimit = 0;
        if (rtv = bb.Read(this->text)) return rtv;
        return rtv;
    }

	inline PushLogout::PushLogout()
	{
	}
	inline PushLogout::PushLogout(xx::BBuffer *bb)
	{
	    int rtv = 0;
        if (rtv = bb->Read(id)) throw rtv;
        bb->readLengthLimit = 0;
        if (rtv = bb->Read(reason)) throw rtv;
	}
	inline PushLogout::~PushLogout()
	{
        mempool().SafeRelease(reason);
	}

    inline void PushLogout::ToString(xx::String &str) const
    {
        if (tsFlags())
        {
        	str.Append("[ \"***** recursived *****\" ]");
        	return;
        }
        else tsFlags() = 1;

        str.Append("{ \"type\" : \"PushLogout\"");
        ToStringCore(str);
        str.Append(" }");
        
        tsFlags() = 0;
    }
    inline void PushLogout::ToStringCore(xx::String &str) const
    {
        this->BaseType::ToStringCore(str);
        str.Append(", \"id\" : ", this->id);
        str.Append(", \"reason\" : ", this->reason);
    }


    inline void PushLogout::ToBBuffer(xx::BBuffer &bb) const
    {
        bb.Write(this->id);
        bb.Write(this->reason);
    }

    inline int PushLogout::FromBBuffer(xx::BBuffer &bb)
    {
        int rtv = 0;
        if (rtv = bb.Read(this->id)) return rtv;
        bb.readLengthLimit = 0;
        if (rtv = bb.Read(this->reason)) return rtv;
        return rtv;
    }

}
namespace Manage_DB
{
	inline Login::Login()
        : PKG::Request()
	{
	}
	inline Login::Login(xx::BBuffer *bb)
        : PKG::Request(bb)
	{
	    int rtv = 0;
        bb->readLengthLimit = 50;
        if (rtv = bb->Read(username)) throw rtv;
        bb->readLengthLimit = 50;
        if (rtv = bb->Read(password)) throw rtv;
	}
	inline Login::~Login()
	{
        mempool().SafeRelease(username);
        mempool().SafeRelease(password);
	}

    inline void Login::ToString(xx::String &str) const
    {
        if (tsFlags())
        {
        	str.Append("[ \"***** recursived *****\" ]");
        	return;
        }
        else tsFlags() = 1;

        str.Append("{ \"type\" : \"Login\"");
        ToStringCore(str);
        str.Append(" }");
        
        tsFlags() = 0;
    }
    inline void Login::ToStringCore(xx::String &str) const
    {
        this->BaseType::ToStringCore(str);
        str.Append(", \"username\" : ", this->username);
        str.Append(", \"password\" : ", this->password);
    }


    inline void Login::ToBBuffer(xx::BBuffer &bb) const
    {
        this->BaseType::ToBBuffer(bb);
        bb.Write(this->username);
        bb.Write(this->password);
    }

    inline int Login::FromBBuffer(xx::BBuffer &bb)
    {
        int rtv = 0;
        if (rtv = this->BaseType::FromBBuffer(bb)) return rtv;
        bb.readLengthLimit = 50;
        if (rtv = bb.Read(this->username)) return rtv;
        bb.readLengthLimit = 50;
        if (rtv = bb.Read(this->password)) return rtv;
        return rtv;
    }

	inline Logout::Logout()
        : PKG::Request()
	{
	}
	inline Logout::Logout(xx::BBuffer *bb)
        : PKG::Request(bb)
	{
	}
	inline Logout::~Logout()
	{
	}

    inline void Logout::ToString(xx::String &str) const
    {
        if (tsFlags())
        {
        	str.Append("[ \"***** recursived *****\" ]");
        	return;
        }
        else tsFlags() = 1;

        str.Append("{ \"type\" : \"Logout\"");
        ToStringCore(str);
        str.Append(" }");
        
        tsFlags() = 0;
    }
    inline void Logout::ToStringCore(xx::String &str) const
    {
        this->BaseType::ToStringCore(str);
    }


    inline void Logout::ToBBuffer(xx::BBuffer &bb) const
    {
        this->BaseType::ToBBuffer(bb);
    }

    inline int Logout::FromBBuffer(xx::BBuffer &bb)
    {
        int rtv = 0;
        if (rtv = this->BaseType::FromBBuffer(bb)) return rtv;
        return rtv;
    }

	inline SelectManageAccountIds::SelectManageAccountIds()
        : PKG::Request()
	{
	}
	inline SelectManageAccountIds::SelectManageAccountIds(xx::BBuffer *bb)
        : PKG::Request(bb)
	{
	    int rtv = 0;
        if (rtv = bb->Read(limit)) throw rtv;
        if (rtv = bb->Read(sort)) throw rtv;
	}
	inline SelectManageAccountIds::~SelectManageAccountIds()
	{
	}

    inline void SelectManageAccountIds::ToString(xx::String &str) const
    {
        if (tsFlags())
        {
        	str.Append("[ \"***** recursived *****\" ]");
        	return;
        }
        else tsFlags() = 1;

        str.Append("{ \"type\" : \"SelectManageAccountIds\"");
        ToStringCore(str);
        str.Append(" }");
        
        tsFlags() = 0;
    }
    inline void SelectManageAccountIds::ToStringCore(xx::String &str) const
    {
        this->BaseType::ToStringCore(str);
        str.Append(", \"limit\" : ", this->limit);
        str.Append(", \"sort\" : ", this->sort);
    }


    inline void SelectManageAccountIds::ToBBuffer(xx::BBuffer &bb) const
    {
        this->BaseType::ToBBuffer(bb);
        bb.Write(this->limit);
        bb.Write(this->sort);
    }

    inline int SelectManageAccountIds::FromBBuffer(xx::BBuffer &bb)
    {
        int rtv = 0;
        if (rtv = this->BaseType::FromBBuffer(bb)) return rtv;
        if (rtv = bb.Read(this->limit)) return rtv;
        if (rtv = bb.Read(this->sort)) return rtv;
        return rtv;
    }

	inline SelectManageAccounts::SelectManageAccounts()
        : PKG::Request()
	{
	}
	inline SelectManageAccounts::SelectManageAccounts(xx::BBuffer *bb)
        : PKG::Request(bb)
	{
	    int rtv = 0;
        bb->readLengthLimit = 50;
        if (rtv = bb->Read(ids)) throw rtv;
	}
	inline SelectManageAccounts::~SelectManageAccounts()
	{
        mempool().SafeRelease(ids);
	}

    inline void SelectManageAccounts::ToString(xx::String &str) const
    {
        if (tsFlags())
        {
        	str.Append("[ \"***** recursived *****\" ]");
        	return;
        }
        else tsFlags() = 1;

        str.Append("{ \"type\" : \"SelectManageAccounts\"");
        ToStringCore(str);
        str.Append(" }");
        
        tsFlags() = 0;
    }
    inline void SelectManageAccounts::ToStringCore(xx::String &str) const
    {
        this->BaseType::ToStringCore(str);
        str.Append(", \"ids\" : ", this->ids);
    }


    inline void SelectManageAccounts::ToBBuffer(xx::BBuffer &bb) const
    {
        this->BaseType::ToBBuffer(bb);
        bb.Write(this->ids);
    }

    inline int SelectManageAccounts::FromBBuffer(xx::BBuffer &bb)
    {
        int rtv = 0;
        if (rtv = this->BaseType::FromBBuffer(bb)) return rtv;
        bb.readLengthLimit = 50;
        if (rtv = bb.Read(this->ids)) return rtv;
        return rtv;
    }

}
namespace Manage
{
	inline Account::Account()
	{
	}
	inline Account::Account(xx::BBuffer *bb)
	{
	    int rtv = 0;
        if (rtv = bb->Read(id)) throw rtv;
        bb->readLengthLimit = 0;
        if (rtv = bb->Read(username)) throw rtv;
	}
	inline Account::~Account()
	{
        mempool().SafeRelease(username);
	}

    inline void Account::ToString(xx::String &str) const
    {
        if (tsFlags())
        {
        	str.Append("[ \"***** recursived *****\" ]");
        	return;
        }
        else tsFlags() = 1;

        str.Append("{ \"type\" : \"Account\"");
        ToStringCore(str);
        str.Append(" }");
        
        tsFlags() = 0;
    }
    inline void Account::ToStringCore(xx::String &str) const
    {
        this->BaseType::ToStringCore(str);
        str.Append(", \"id\" : ", this->id);
        str.Append(", \"username\" : ", this->username);
    }


    inline void Account::ToBBuffer(xx::BBuffer &bb) const
    {
        bb.Write(this->id);
        bb.Write(this->username);
    }

    inline int Account::FromBBuffer(xx::BBuffer &bb)
    {
        int rtv = 0;
        if (rtv = bb.Read(this->id)) return rtv;
        bb.readLengthLimit = 0;
        if (rtv = bb.Read(this->username)) return rtv;
        return rtv;
    }

	inline Role::Role()
	{
	}
	inline Role::Role(xx::BBuffer *bb)
	{
	    int rtv = 0;
        if (rtv = bb->Read(id)) throw rtv;
        bb->readLengthLimit = 0;
        if (rtv = bb->Read(name)) throw rtv;
        bb->readLengthLimit = 0;
        if (rtv = bb->Read(desc)) throw rtv;
	}
	inline Role::~Role()
	{
        mempool().SafeRelease(name);
        mempool().SafeRelease(desc);
	}

    inline void Role::ToString(xx::String &str) const
    {
        if (tsFlags())
        {
        	str.Append("[ \"***** recursived *****\" ]");
        	return;
        }
        else tsFlags() = 1;

        str.Append("{ \"type\" : \"Role\"");
        ToStringCore(str);
        str.Append(" }");
        
        tsFlags() = 0;
    }
    inline void Role::ToStringCore(xx::String &str) const
    {
        this->BaseType::ToStringCore(str);
        str.Append(", \"id\" : ", this->id);
        str.Append(", \"name\" : ", this->name);
        str.Append(", \"desc\" : ", this->desc);
    }


    inline void Role::ToBBuffer(xx::BBuffer &bb) const
    {
        bb.Write(this->id);
        bb.Write(this->name);
        bb.Write(this->desc);
    }

    inline int Role::FromBBuffer(xx::BBuffer &bb)
    {
        int rtv = 0;
        if (rtv = bb.Read(this->id)) return rtv;
        bb.readLengthLimit = 0;
        if (rtv = bb.Read(this->name)) return rtv;
        bb.readLengthLimit = 0;
        if (rtv = bb.Read(this->desc)) return rtv;
        return rtv;
    }

	inline Permission::Permission()
	{
	}
	inline Permission::Permission(xx::BBuffer *bb)
	{
	    int rtv = 0;
        if (rtv = bb->Read(id)) throw rtv;
        bb->readLengthLimit = 0;
        if (rtv = bb->Read(group)) throw rtv;
        bb->readLengthLimit = 0;
        if (rtv = bb->Read(name)) throw rtv;
        bb->readLengthLimit = 0;
        if (rtv = bb->Read(desc)) throw rtv;
	}
	inline Permission::~Permission()
	{
        mempool().SafeRelease(group);
        mempool().SafeRelease(name);
        mempool().SafeRelease(desc);
	}

    inline void Permission::ToString(xx::String &str) const
    {
        if (tsFlags())
        {
        	str.Append("[ \"***** recursived *****\" ]");
        	return;
        }
        else tsFlags() = 1;

        str.Append("{ \"type\" : \"Permission\"");
        ToStringCore(str);
        str.Append(" }");
        
        tsFlags() = 0;
    }
    inline void Permission::ToStringCore(xx::String &str) const
    {
        this->BaseType::ToStringCore(str);
        str.Append(", \"id\" : ", this->id);
        str.Append(", \"group\" : ", this->group);
        str.Append(", \"name\" : ", this->name);
        str.Append(", \"desc\" : ", this->desc);
    }


    inline void Permission::ToBBuffer(xx::BBuffer &bb) const
    {
        bb.Write(this->id);
        bb.Write(this->group);
        bb.Write(this->name);
        bb.Write(this->desc);
    }

    inline int Permission::FromBBuffer(xx::BBuffer &bb)
    {
        int rtv = 0;
        if (rtv = bb.Read(this->id)) return rtv;
        bb.readLengthLimit = 0;
        if (rtv = bb.Read(this->group)) return rtv;
        bb.readLengthLimit = 0;
        if (rtv = bb.Read(this->name)) return rtv;
        bb.readLengthLimit = 0;
        if (rtv = bb.Read(this->desc)) return rtv;
        return rtv;
    }

	inline BindAccountRole::BindAccountRole()
	{
	}
	inline BindAccountRole::BindAccountRole(xx::BBuffer *bb)
	{
	    int rtv = 0;
        if (rtv = bb->Read(account_id)) throw rtv;
        if (rtv = bb->Read(role_id)) throw rtv;
	}
	inline BindAccountRole::~BindAccountRole()
	{
	}

    inline void BindAccountRole::ToString(xx::String &str) const
    {
        if (tsFlags())
        {
        	str.Append("[ \"***** recursived *****\" ]");
        	return;
        }
        else tsFlags() = 1;

        str.Append("{ \"type\" : \"BindAccountRole\"");
        ToStringCore(str);
        str.Append(" }");
        
        tsFlags() = 0;
    }
    inline void BindAccountRole::ToStringCore(xx::String &str) const
    {
        this->BaseType::ToStringCore(str);
        str.Append(", \"account_id\" : ", this->account_id);
        str.Append(", \"role_id\" : ", this->role_id);
    }


    inline void BindAccountRole::ToBBuffer(xx::BBuffer &bb) const
    {
        bb.Write(this->account_id);
        bb.Write(this->role_id);
    }

    inline int BindAccountRole::FromBBuffer(xx::BBuffer &bb)
    {
        int rtv = 0;
        if (rtv = bb.Read(this->account_id)) return rtv;
        if (rtv = bb.Read(this->role_id)) return rtv;
        return rtv;
    }

	inline BindRolePermission::BindRolePermission()
	{
	}
	inline BindRolePermission::BindRolePermission(xx::BBuffer *bb)
	{
	    int rtv = 0;
        if (rtv = bb->Read(role_id)) throw rtv;
        if (rtv = bb->Read(permission_id)) throw rtv;
	}
	inline BindRolePermission::~BindRolePermission()
	{
	}

    inline void BindRolePermission::ToString(xx::String &str) const
    {
        if (tsFlags())
        {
        	str.Append("[ \"***** recursived *****\" ]");
        	return;
        }
        else tsFlags() = 1;

        str.Append("{ \"type\" : \"BindRolePermission\"");
        ToStringCore(str);
        str.Append(" }");
        
        tsFlags() = 0;
    }
    inline void BindRolePermission::ToStringCore(xx::String &str) const
    {
        this->BaseType::ToStringCore(str);
        str.Append(", \"role_id\" : ", this->role_id);
        str.Append(", \"permission_id\" : ", this->permission_id);
    }


    inline void BindRolePermission::ToBBuffer(xx::BBuffer &bb) const
    {
        bb.Write(this->role_id);
        bb.Write(this->permission_id);
    }

    inline int BindRolePermission::FromBBuffer(xx::BBuffer &bb)
    {
        int rtv = 0;
        if (rtv = bb.Read(this->role_id)) return rtv;
        if (rtv = bb.Read(this->permission_id)) return rtv;
        return rtv;
    }

}
namespace DB_Manage
{
	inline LoginSuccess::LoginSuccess()
        : PKG::Success()
	{
	}
	inline LoginSuccess::LoginSuccess(xx::BBuffer *bb)
        : PKG::Success(bb)
	{
	    int rtv = 0;
        if (rtv = bb->Read(id)) throw rtv;
        bb->readLengthLimit = 0;
        if (rtv = bb->Read(token)) throw rtv;
	}
	inline LoginSuccess::~LoginSuccess()
	{
        mempool().SafeRelease(token);
	}

    inline void LoginSuccess::ToString(xx::String &str) const
    {
        if (tsFlags())
        {
        	str.Append("[ \"***** recursived *****\" ]");
        	return;
        }
        else tsFlags() = 1;

        str.Append("{ \"type\" : \"LoginSuccess\"");
        ToStringCore(str);
        str.Append(" }");
        
        tsFlags() = 0;
    }
    inline void LoginSuccess::ToStringCore(xx::String &str) const
    {
        this->BaseType::ToStringCore(str);
        str.Append(", \"id\" : ", this->id);
        str.Append(", \"token\" : ", this->token);
    }


    inline void LoginSuccess::ToBBuffer(xx::BBuffer &bb) const
    {
        this->BaseType::ToBBuffer(bb);
        bb.Write(this->id);
        bb.Write(this->token);
    }

    inline int LoginSuccess::FromBBuffer(xx::BBuffer &bb)
    {
        int rtv = 0;
        if (rtv = this->BaseType::FromBBuffer(bb)) return rtv;
        if (rtv = bb.Read(this->id)) return rtv;
        bb.readLengthLimit = 0;
        if (rtv = bb.Read(this->token)) return rtv;
        return rtv;
    }

	inline LoginFail::LoginFail()
        : PKG::Fail()
	{
	}
	inline LoginFail::LoginFail(xx::BBuffer *bb)
        : PKG::Fail(bb)
	{
	}
	inline LoginFail::~LoginFail()
	{
	}

    inline void LoginFail::ToString(xx::String &str) const
    {
        if (tsFlags())
        {
        	str.Append("[ \"***** recursived *****\" ]");
        	return;
        }
        else tsFlags() = 1;

        str.Append("{ \"type\" : \"LoginFail\"");
        ToStringCore(str);
        str.Append(" }");
        
        tsFlags() = 0;
    }
    inline void LoginFail::ToStringCore(xx::String &str) const
    {
        this->BaseType::ToStringCore(str);
    }


    inline void LoginFail::ToBBuffer(xx::BBuffer &bb) const
    {
        this->BaseType::ToBBuffer(bb);
    }

    inline int LoginFail::FromBBuffer(xx::BBuffer &bb)
    {
        int rtv = 0;
        if (rtv = this->BaseType::FromBBuffer(bb)) return rtv;
        return rtv;
    }

	inline LogoutSuccess::LogoutSuccess()
        : PKG::Success()
	{
	}
	inline LogoutSuccess::LogoutSuccess(xx::BBuffer *bb)
        : PKG::Success(bb)
	{
	}
	inline LogoutSuccess::~LogoutSuccess()
	{
	}

    inline void LogoutSuccess::ToString(xx::String &str) const
    {
        if (tsFlags())
        {
        	str.Append("[ \"***** recursived *****\" ]");
        	return;
        }
        else tsFlags() = 1;

        str.Append("{ \"type\" : \"LogoutSuccess\"");
        ToStringCore(str);
        str.Append(" }");
        
        tsFlags() = 0;
    }
    inline void LogoutSuccess::ToStringCore(xx::String &str) const
    {
        this->BaseType::ToStringCore(str);
    }


    inline void LogoutSuccess::ToBBuffer(xx::BBuffer &bb) const
    {
        this->BaseType::ToBBuffer(bb);
    }

    inline int LogoutSuccess::FromBBuffer(xx::BBuffer &bb)
    {
        int rtv = 0;
        if (rtv = this->BaseType::FromBBuffer(bb)) return rtv;
        return rtv;
    }

	inline LogoutFail::LogoutFail()
        : PKG::Fail()
	{
	}
	inline LogoutFail::LogoutFail(xx::BBuffer *bb)
        : PKG::Fail(bb)
	{
	}
	inline LogoutFail::~LogoutFail()
	{
	}

    inline void LogoutFail::ToString(xx::String &str) const
    {
        if (tsFlags())
        {
        	str.Append("[ \"***** recursived *****\" ]");
        	return;
        }
        else tsFlags() = 1;

        str.Append("{ \"type\" : \"LogoutFail\"");
        ToStringCore(str);
        str.Append(" }");
        
        tsFlags() = 0;
    }
    inline void LogoutFail::ToStringCore(xx::String &str) const
    {
        this->BaseType::ToStringCore(str);
    }


    inline void LogoutFail::ToBBuffer(xx::BBuffer &bb) const
    {
        this->BaseType::ToBBuffer(bb);
    }

    inline int LogoutFail::FromBBuffer(xx::BBuffer &bb)
    {
        int rtv = 0;
        if (rtv = this->BaseType::FromBBuffer(bb)) return rtv;
        return rtv;
    }

	inline SelectManageAccountIdsSuccess::SelectManageAccountIdsSuccess()
        : PKG::Success()
	{
	}
	inline SelectManageAccountIdsSuccess::SelectManageAccountIdsSuccess(xx::BBuffer *bb)
        : PKG::Success(bb)
	{
	    int rtv = 0;
        bb->readLengthLimit = 0;
        if (rtv = bb->Read(ids)) throw rtv;
	}
	inline SelectManageAccountIdsSuccess::~SelectManageAccountIdsSuccess()
	{
        mempool().SafeRelease(ids);
	}

    inline void SelectManageAccountIdsSuccess::ToString(xx::String &str) const
    {
        if (tsFlags())
        {
        	str.Append("[ \"***** recursived *****\" ]");
        	return;
        }
        else tsFlags() = 1;

        str.Append("{ \"type\" : \"SelectManageAccountIdsSuccess\"");
        ToStringCore(str);
        str.Append(" }");
        
        tsFlags() = 0;
    }
    inline void SelectManageAccountIdsSuccess::ToStringCore(xx::String &str) const
    {
        this->BaseType::ToStringCore(str);
        str.Append(", \"ids\" : ", this->ids);
    }


    inline void SelectManageAccountIdsSuccess::ToBBuffer(xx::BBuffer &bb) const
    {
        this->BaseType::ToBBuffer(bb);
        bb.Write(this->ids);
    }

    inline int SelectManageAccountIdsSuccess::FromBBuffer(xx::BBuffer &bb)
    {
        int rtv = 0;
        if (rtv = this->BaseType::FromBBuffer(bb)) return rtv;
        bb.readLengthLimit = 0;
        if (rtv = bb.Read(this->ids)) return rtv;
        return rtv;
    }

	inline SelectManageAccountIdsFail::SelectManageAccountIdsFail()
        : PKG::Fail()
	{
	}
	inline SelectManageAccountIdsFail::SelectManageAccountIdsFail(xx::BBuffer *bb)
        : PKG::Fail(bb)
	{
	}
	inline SelectManageAccountIdsFail::~SelectManageAccountIdsFail()
	{
	}

    inline void SelectManageAccountIdsFail::ToString(xx::String &str) const
    {
        if (tsFlags())
        {
        	str.Append("[ \"***** recursived *****\" ]");
        	return;
        }
        else tsFlags() = 1;

        str.Append("{ \"type\" : \"SelectManageAccountIdsFail\"");
        ToStringCore(str);
        str.Append(" }");
        
        tsFlags() = 0;
    }
    inline void SelectManageAccountIdsFail::ToStringCore(xx::String &str) const
    {
        this->BaseType::ToStringCore(str);
    }


    inline void SelectManageAccountIdsFail::ToBBuffer(xx::BBuffer &bb) const
    {
        this->BaseType::ToBBuffer(bb);
    }

    inline int SelectManageAccountIdsFail::FromBBuffer(xx::BBuffer &bb)
    {
        int rtv = 0;
        if (rtv = this->BaseType::FromBBuffer(bb)) return rtv;
        return rtv;
    }

	inline SelectManageAccountsSuccess::SelectManageAccountsSuccess()
        : PKG::Success()
	{
	}
	inline SelectManageAccountsSuccess::SelectManageAccountsSuccess(xx::BBuffer *bb)
        : PKG::Success(bb)
	{
	    int rtv = 0;
        bb->readLengthLimit = 0;
        if (rtv = bb->Read(rows)) throw rtv;
	}
	inline SelectManageAccountsSuccess::~SelectManageAccountsSuccess()
	{
        mempool().SafeRelease(rows);
	}

    inline void SelectManageAccountsSuccess::ToString(xx::String &str) const
    {
        if (tsFlags())
        {
        	str.Append("[ \"***** recursived *****\" ]");
        	return;
        }
        else tsFlags() = 1;

        str.Append("{ \"type\" : \"SelectManageAccountsSuccess\"");
        ToStringCore(str);
        str.Append(" }");
        
        tsFlags() = 0;
    }
    inline void SelectManageAccountsSuccess::ToStringCore(xx::String &str) const
    {
        this->BaseType::ToStringCore(str);
        str.Append(", \"rows\" : ", this->rows);
    }


    inline void SelectManageAccountsSuccess::ToBBuffer(xx::BBuffer &bb) const
    {
        this->BaseType::ToBBuffer(bb);
        bb.Write(this->rows);
    }

    inline int SelectManageAccountsSuccess::FromBBuffer(xx::BBuffer &bb)
    {
        int rtv = 0;
        if (rtv = this->BaseType::FromBBuffer(bb)) return rtv;
        bb.readLengthLimit = 0;
        if (rtv = bb.Read(this->rows)) return rtv;
        return rtv;
    }

	inline SelectManageAccountsFail::SelectManageAccountsFail()
        : PKG::Fail()
	{
	}
	inline SelectManageAccountsFail::SelectManageAccountsFail(xx::BBuffer *bb)
        : PKG::Fail(bb)
	{
	}
	inline SelectManageAccountsFail::~SelectManageAccountsFail()
	{
	}

    inline void SelectManageAccountsFail::ToString(xx::String &str) const
    {
        if (tsFlags())
        {
        	str.Append("[ \"***** recursived *****\" ]");
        	return;
        }
        else tsFlags() = 1;

        str.Append("{ \"type\" : \"SelectManageAccountsFail\"");
        ToStringCore(str);
        str.Append(" }");
        
        tsFlags() = 0;
    }
    inline void SelectManageAccountsFail::ToStringCore(xx::String &str) const
    {
        this->BaseType::ToStringCore(str);
    }


    inline void SelectManageAccountsFail::ToBBuffer(xx::BBuffer &bb) const
    {
        this->BaseType::ToBBuffer(bb);
    }

    inline int SelectManageAccountsFail::FromBBuffer(xx::BBuffer &bb)
    {
        int rtv = 0;
        if (rtv = this->BaseType::FromBBuffer(bb)) return rtv;
        return rtv;
    }

}
}
namespace xx
{
	template<> struct TypeId<xx::String> { static const uint16_t value = 1; };
	template<> struct TypeId<xx::BBuffer> { static const uint16_t value = 2; };
	template<> struct TypeId<PKG::Request> { static const uint16_t value = 3; };
	template<> struct TypeId<PKG::Response> { static const uint16_t value = 4; };
	template<> struct TypeId<PKG::Success> { static const uint16_t value = 5; };
	template<> struct TypeId<PKG::Fail> { static const uint16_t value = 6; };
	template<> struct TypeId<PKG::Property> { static const uint16_t value = 7; };
	template<> struct TypeId<PKG::Property_long> { static const uint16_t value = 8; };
	template<> struct TypeId<PKG::Property_double> { static const uint16_t value = 9; };
	template<> struct TypeId<PKG::Property_string> { static const uint16_t value = 10; };
	template<> struct TypeId<PKG::Properties> { static const uint16_t value = 11; };
	template<> struct TypeId<xx::List<PKG::Property*>> { static const uint16_t value = 12; };
	template<> struct TypeId<PKG::UserInfo> { static const uint16_t value = 13; };
	template<> struct TypeId<PKG::Client_Server::Join> { static const uint16_t value = 14; };
	template<> struct TypeId<PKG::Client_Server::Message> { static const uint16_t value = 15; };
	template<> struct TypeId<PKG::Client_Server::Logout> { static const uint16_t value = 16; };
	template<> struct TypeId<PKG::Server_Client::JoinSuccess> { static const uint16_t value = 17; };
	template<> struct TypeId<xx::List<PKG::UserInfo*>> { static const uint16_t value = 18; };
	template<> struct TypeId<PKG::Server_Client::JoinFail> { static const uint16_t value = 19; };
	template<> struct TypeId<PKG::Server_Client::PushJoin> { static const uint16_t value = 20; };
	template<> struct TypeId<PKG::Server_Client::PushMessage> { static const uint16_t value = 21; };
	template<> struct TypeId<PKG::Server_Client::PushLogout> { static const uint16_t value = 22; };
	template<> struct TypeId<PKG::Manage_DB::Login> { static const uint16_t value = 23; };
	template<> struct TypeId<PKG::Manage_DB::Logout> { static const uint16_t value = 24; };
	template<> struct TypeId<PKG::Manage_DB::SelectManageAccountIds> { static const uint16_t value = 25; };
	template<> struct TypeId<PKG::Manage_DB::SelectManageAccounts> { static const uint16_t value = 26; };
	template<> struct TypeId<xx::List<int64_t>> { static const uint16_t value = 27; };
	template<> struct TypeId<PKG::Manage::Account> { static const uint16_t value = 28; };
	template<> struct TypeId<PKG::Manage::Role> { static const uint16_t value = 29; };
	template<> struct TypeId<PKG::Manage::Permission> { static const uint16_t value = 30; };
	template<> struct TypeId<PKG::Manage::BindAccountRole> { static const uint16_t value = 31; };
	template<> struct TypeId<PKG::Manage::BindRolePermission> { static const uint16_t value = 32; };
	template<> struct TypeId<PKG::DB_Manage::LoginSuccess> { static const uint16_t value = 33; };
	template<> struct TypeId<PKG::DB_Manage::LoginFail> { static const uint16_t value = 34; };
	template<> struct TypeId<PKG::DB_Manage::LogoutSuccess> { static const uint16_t value = 35; };
	template<> struct TypeId<PKG::DB_Manage::LogoutFail> { static const uint16_t value = 36; };
	template<> struct TypeId<PKG::DB_Manage::SelectManageAccountIdsSuccess> { static const uint16_t value = 37; };
	template<> struct TypeId<PKG::DB_Manage::SelectManageAccountIdsFail> { static const uint16_t value = 38; };
	template<> struct TypeId<PKG::DB_Manage::SelectManageAccountsSuccess> { static const uint16_t value = 39; };
	template<> struct TypeId<xx::List<PKG::Manage::Account*>> { static const uint16_t value = 40; };
	template<> struct TypeId<PKG::DB_Manage::SelectManageAccountsFail> { static const uint16_t value = 41; };
}
namespace PKG
{
	inline void AllTypesRegister()
	{
	    xx::MemPool::Register<xx::String, xx::Object>();
	    xx::MemPool::Register<xx::BBuffer, xx::Object>();
	    xx::MemPool::Register<PKG::Request, xx::Object>();
	    xx::MemPool::Register<PKG::Response, xx::Object>();
	    xx::MemPool::Register<PKG::Success, PKG::Response>();
	    xx::MemPool::Register<PKG::Fail, PKG::Response>();
	    xx::MemPool::Register<PKG::Property, xx::Object>();
	    xx::MemPool::Register<PKG::Property_long, PKG::Property>();
	    xx::MemPool::Register<PKG::Property_double, PKG::Property>();
	    xx::MemPool::Register<PKG::Property_string, PKG::Property>();
	    xx::MemPool::Register<PKG::Properties, PKG::Property>();
	    xx::MemPool::Register<xx::List<PKG::Property*>, xx::Object>();
	    xx::MemPool::Register<PKG::UserInfo, xx::Object>();
	    xx::MemPool::Register<PKG::Client_Server::Join, PKG::Request>();
	    xx::MemPool::Register<PKG::Client_Server::Message, xx::Object>();
	    xx::MemPool::Register<PKG::Client_Server::Logout, xx::Object>();
	    xx::MemPool::Register<PKG::Server_Client::JoinSuccess, PKG::Response>();
	    xx::MemPool::Register<xx::List<PKG::UserInfo*>, xx::Object>();
	    xx::MemPool::Register<PKG::Server_Client::JoinFail, PKG::Response>();
	    xx::MemPool::Register<PKG::Server_Client::PushJoin, xx::Object>();
	    xx::MemPool::Register<PKG::Server_Client::PushMessage, xx::Object>();
	    xx::MemPool::Register<PKG::Server_Client::PushLogout, xx::Object>();
	    xx::MemPool::Register<PKG::Manage_DB::Login, PKG::Request>();
	    xx::MemPool::Register<PKG::Manage_DB::Logout, PKG::Request>();
	    xx::MemPool::Register<PKG::Manage_DB::SelectManageAccountIds, PKG::Request>();
	    xx::MemPool::Register<PKG::Manage_DB::SelectManageAccounts, PKG::Request>();
	    xx::MemPool::Register<xx::List<int64_t>, xx::Object>();
	    xx::MemPool::Register<PKG::Manage::Account, xx::Object>();
	    xx::MemPool::Register<PKG::Manage::Role, xx::Object>();
	    xx::MemPool::Register<PKG::Manage::Permission, xx::Object>();
	    xx::MemPool::Register<PKG::Manage::BindAccountRole, xx::Object>();
	    xx::MemPool::Register<PKG::Manage::BindRolePermission, xx::Object>();
	    xx::MemPool::Register<PKG::DB_Manage::LoginSuccess, PKG::Success>();
	    xx::MemPool::Register<PKG::DB_Manage::LoginFail, PKG::Fail>();
	    xx::MemPool::Register<PKG::DB_Manage::LogoutSuccess, PKG::Success>();
	    xx::MemPool::Register<PKG::DB_Manage::LogoutFail, PKG::Fail>();
	    xx::MemPool::Register<PKG::DB_Manage::SelectManageAccountIdsSuccess, PKG::Success>();
	    xx::MemPool::Register<PKG::DB_Manage::SelectManageAccountIdsFail, PKG::Fail>();
	    xx::MemPool::Register<PKG::DB_Manage::SelectManageAccountsSuccess, PKG::Success>();
	    xx::MemPool::Register<xx::List<PKG::Manage::Account*>, xx::Object>();
	    xx::MemPool::Register<PKG::DB_Manage::SelectManageAccountsFail, PKG::Fail>();
	}
}
