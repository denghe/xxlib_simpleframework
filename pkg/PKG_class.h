
namespace PKG
{
	struct PkgGenMd5
	{
		static constexpr char const* value = "aa4225edbd8eab2f73f1a1956c172de2";

    };

    // 一个请求( 当前限定为 service 与 db 间 ), 通常携带一个流水号. 这是基类
    struct Request;
    using Request_p = xx::Ptr<Request>;
    using Request_r = xx::Ref<Request>;

    // 一个回应( 当前限定为 service 与 db 间 ), 通常携带一个请求发过来的流水号. 这是基类
    struct Response;
    using Response_p = xx::Ptr<Response>;
    using Response_r = xx::Ref<Response>;

    // 成功
    struct Success;
    using Success_p = xx::Ptr<Success>;
    using Success_r = xx::Ref<Success>;

    // 失败
    struct Fail;
    using Fail_p = xx::Ptr<Fail>;
    using Fail_r = xx::Ref<Fail>;

    struct Property;
    using Property_p = xx::Ptr<Property>;
    using Property_r = xx::Ref<Property>;

    struct Property_long;
    using Property_long_p = xx::Ptr<Property_long>;
    using Property_long_r = xx::Ref<Property_long>;

    struct Property_double;
    using Property_double_p = xx::Ptr<Property_double>;
    using Property_double_r = xx::Ref<Property_double>;

    struct Property_string;
    using Property_string_p = xx::Ptr<Property_string>;
    using Property_string_r = xx::Ref<Property_string>;

    struct Properties;
    using Properties_p = xx::Ptr<Properties>;
    using Properties_r = xx::Ref<Properties>;

    struct UserInfo;
    using UserInfo_p = xx::Ptr<UserInfo>;
    using UserInfo_r = xx::Ref<UserInfo>;

namespace Client_Server
{
    // 请求进入服务器
    struct Join;
    using Join_p = xx::Ptr<Join>;
    using Join_r = xx::Ref<Join>;

    // 发消息
    struct Message;
    using Message_p = xx::Ptr<Message>;
    using Message_r = xx::Ref<Message>;

    // 主动退出
    struct Logout;
    using Logout_p = xx::Ptr<Logout>;
    using Logout_r = xx::Ref<Logout>;

}
namespace Server_Client
{
    // 进入成功, 返回用户信息
    struct JoinSuccess;
    using JoinSuccess_p = xx::Ptr<JoinSuccess>;
    using JoinSuccess_r = xx::Ref<JoinSuccess>;

    // 进入失败, 返回错误信息
    struct JoinFail;
    using JoinFail_p = xx::Ptr<JoinFail>;
    using JoinFail_r = xx::Ref<JoinFail>;

    // 推送文字消息
    struct PushJoin;
    using PushJoin_p = xx::Ptr<PushJoin>;
    using PushJoin_r = xx::Ref<PushJoin>;

    // 推送文字消息
    struct PushMessage;
    using PushMessage_p = xx::Ptr<PushMessage>;
    using PushMessage_r = xx::Ref<PushMessage>;

    // 推送退出消息
    struct PushLogout;
    using PushLogout_p = xx::Ptr<PushLogout>;
    using PushLogout_r = xx::Ref<PushLogout>;

}
namespace Manage_DB
{
    // 请求进入服务器
    struct Login;
    using Login_p = xx::Ptr<Login>;
    using Login_r = xx::Ref<Login>;

    // 注销式退出
    struct Logout;
    using Logout_p = xx::Ptr<Logout>;
    using Logout_r = xx::Ref<Logout>;

    // 拉管理员账号列表的 id 列表( 需要相关权限 )
    struct SelectManageAccountIds;
    using SelectManageAccountIds_p = xx::Ptr<SelectManageAccountIds>;
    using SelectManageAccountIds_r = xx::Ref<SelectManageAccountIds>;

    // 拉管理员账号列表( 需要相关权限 )
    struct SelectManageAccounts;
    using SelectManageAccounts_p = xx::Ptr<SelectManageAccounts>;
    using SelectManageAccounts_r = xx::Ref<SelectManageAccounts>;

}
namespace Manage
{
    // 账号
    struct Account;
    using Account_p = xx::Ptr<Account>;
    using Account_r = xx::Ref<Account>;

    // 身份
    struct Role;
    using Role_p = xx::Ptr<Role>;
    using Role_r = xx::Ref<Role>;

    // 权限
    struct Permission;
    using Permission_p = xx::Ptr<Permission>;
    using Permission_r = xx::Ref<Permission>;

    // 管理人员 & 身份 绑定
    struct BindAccountRole;
    using BindAccountRole_p = xx::Ptr<BindAccountRole>;
    using BindAccountRole_r = xx::Ref<BindAccountRole>;

    // 身份 & 权限 绑定
    struct BindRolePermission;
    using BindRolePermission_p = xx::Ptr<BindRolePermission>;
    using BindRolePermission_r = xx::Ref<BindRolePermission>;

}
namespace DB_Manage
{
    // 登录成功
    struct LoginSuccess;
    using LoginSuccess_p = xx::Ptr<LoginSuccess>;
    using LoginSuccess_r = xx::Ref<LoginSuccess>;

    // 登录失败
    struct LoginFail;
    using LoginFail_p = xx::Ptr<LoginFail>;
    using LoginFail_r = xx::Ref<LoginFail>;

    // 注销成功
    struct LogoutSuccess;
    using LogoutSuccess_p = xx::Ptr<LogoutSuccess>;
    using LogoutSuccess_r = xx::Ref<LogoutSuccess>;

    // 注销失败
    struct LogoutFail;
    using LogoutFail_p = xx::Ptr<LogoutFail>;
    using LogoutFail_r = xx::Ref<LogoutFail>;

    // 拉管理员账号列表的 id 列表 成功
    struct SelectManageAccountIdsSuccess;
    using SelectManageAccountIdsSuccess_p = xx::Ptr<SelectManageAccountIdsSuccess>;
    using SelectManageAccountIdsSuccess_r = xx::Ref<SelectManageAccountIdsSuccess>;

    // 拉管理员账号列表的 id 列表 失败
    struct SelectManageAccountIdsFail;
    using SelectManageAccountIdsFail_p = xx::Ptr<SelectManageAccountIdsFail>;
    using SelectManageAccountIdsFail_r = xx::Ref<SelectManageAccountIdsFail>;

    // 拉管理员账号列表 成功
    struct SelectManageAccountsSuccess;
    using SelectManageAccountsSuccess_p = xx::Ptr<SelectManageAccountsSuccess>;
    using SelectManageAccountsSuccess_r = xx::Ref<SelectManageAccountsSuccess>;

    // 拉管理员账号列表 失败
    struct SelectManageAccountsFail;
    using SelectManageAccountsFail_p = xx::Ptr<SelectManageAccountsFail>;
    using SelectManageAccountsFail_r = xx::Ref<SelectManageAccountsFail>;

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
	    Response(xx::MemPool* mempool);
	    Response(xx::BBuffer *bb);
		Response(Response const&) = delete;
		Response& operator=(Response const&) = delete;
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
	    Request(xx::MemPool* mempool);
	    Request(xx::BBuffer *bb);
		Request(Request const&) = delete;
		Request& operator=(Request const&) = delete;
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
	    Success(xx::MemPool* mempool);
	    Success(xx::BBuffer *bb);
		Success(Success const&) = delete;
		Success& operator=(Success const&) = delete;
        virtual void ToString(xx::String &str) const override;
        virtual void ToStringCore(xx::String &str) const override;
        virtual void ToBBuffer(xx::BBuffer &bb) const override;
        virtual int FromBBuffer(xx::BBuffer &bb) override;
    };
    // 失败
    struct Fail : PKG::Response
    {
        xx::String_p reason;

        typedef Fail ThisType;
        typedef PKG::Response BaseType;
	    Fail(xx::MemPool* mempool);
	    Fail(xx::BBuffer *bb);
		Fail(Fail const&) = delete;
		Fail& operator=(Fail const&) = delete;
        virtual void ToString(xx::String &str) const override;
        virtual void ToStringCore(xx::String &str) const override;
        virtual void ToBBuffer(xx::BBuffer &bb) const override;
        virtual int FromBBuffer(xx::BBuffer &bb) override;
    };
    struct Property : xx::Object
    {
        xx::String_p name;

        typedef Property ThisType;
        typedef xx::Object BaseType;
	    Property(xx::MemPool* mempool);
	    Property(xx::BBuffer *bb);
		Property(Property const&) = delete;
		Property& operator=(Property const&) = delete;
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
        xx::List_p<int64_t> ids;

        typedef SelectManageAccounts ThisType;
        typedef PKG::Request BaseType;
	    SelectManageAccounts(xx::MemPool* mempool);
	    SelectManageAccounts(xx::BBuffer *bb);
		SelectManageAccounts(SelectManageAccounts const&) = delete;
		SelectManageAccounts& operator=(SelectManageAccounts const&) = delete;
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
        xx::String_p username;

        typedef Account ThisType;
        typedef xx::Object BaseType;
	    Account(xx::MemPool* mempool);
	    Account(xx::BBuffer *bb);
		Account(Account const&) = delete;
		Account& operator=(Account const&) = delete;
        virtual void ToString(xx::String &str) const override;
        virtual void ToStringCore(xx::String &str) const override;
        virtual void ToBBuffer(xx::BBuffer &bb) const override;
        virtual int FromBBuffer(xx::BBuffer &bb) override;
    };
    // 身份
    struct Role : xx::Object
    {
        int32_t id = 0;
        xx::String_p name;
        xx::String_p desc;

        typedef Role ThisType;
        typedef xx::Object BaseType;
	    Role(xx::MemPool* mempool);
	    Role(xx::BBuffer *bb);
		Role(Role const&) = delete;
		Role& operator=(Role const&) = delete;
        virtual void ToString(xx::String &str) const override;
        virtual void ToStringCore(xx::String &str) const override;
        virtual void ToBBuffer(xx::BBuffer &bb) const override;
        virtual int FromBBuffer(xx::BBuffer &bb) override;
    };
    // 权限
    struct Permission : xx::Object
    {
        int32_t id = 0;
        xx::String_p group;
        xx::String_p name;
        xx::String_p desc;

        typedef Permission ThisType;
        typedef xx::Object BaseType;
	    Permission(xx::MemPool* mempool);
	    Permission(xx::BBuffer *bb);
		Permission(Permission const&) = delete;
		Permission& operator=(Permission const&) = delete;
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
	    BindAccountRole(xx::MemPool* mempool);
	    BindAccountRole(xx::BBuffer *bb);
		BindAccountRole(BindAccountRole const&) = delete;
		BindAccountRole& operator=(BindAccountRole const&) = delete;
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
	    BindRolePermission(xx::MemPool* mempool);
	    BindRolePermission(xx::BBuffer *bb);
		BindRolePermission(BindRolePermission const&) = delete;
		BindRolePermission& operator=(BindRolePermission const&) = delete;
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
	    LoginFail(xx::MemPool* mempool);
	    LoginFail(xx::BBuffer *bb);
		LoginFail(LoginFail const&) = delete;
		LoginFail& operator=(LoginFail const&) = delete;
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
	    SelectManageAccountIds(xx::MemPool* mempool);
	    SelectManageAccountIds(xx::BBuffer *bb);
		SelectManageAccountIds(SelectManageAccountIds const&) = delete;
		SelectManageAccountIds& operator=(SelectManageAccountIds const&) = delete;
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
	    LogoutSuccess(xx::MemPool* mempool);
	    LogoutSuccess(xx::BBuffer *bb);
		LogoutSuccess(LogoutSuccess const&) = delete;
		LogoutSuccess& operator=(LogoutSuccess const&) = delete;
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
	    LogoutFail(xx::MemPool* mempool);
	    LogoutFail(xx::BBuffer *bb);
		LogoutFail(LogoutFail const&) = delete;
		LogoutFail& operator=(LogoutFail const&) = delete;
        virtual void ToString(xx::String &str) const override;
        virtual void ToStringCore(xx::String &str) const override;
        virtual void ToBBuffer(xx::BBuffer &bb) const override;
        virtual int FromBBuffer(xx::BBuffer &bb) override;
    };
    // 拉管理员账号列表的 id 列表 成功
    struct SelectManageAccountIdsSuccess : PKG::Success
    {
        xx::List_p<int64_t> ids;

        typedef SelectManageAccountIdsSuccess ThisType;
        typedef PKG::Success BaseType;
	    SelectManageAccountIdsSuccess(xx::MemPool* mempool);
	    SelectManageAccountIdsSuccess(xx::BBuffer *bb);
		SelectManageAccountIdsSuccess(SelectManageAccountIdsSuccess const&) = delete;
		SelectManageAccountIdsSuccess& operator=(SelectManageAccountIdsSuccess const&) = delete;
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
	    SelectManageAccountIdsFail(xx::MemPool* mempool);
	    SelectManageAccountIdsFail(xx::BBuffer *bb);
		SelectManageAccountIdsFail(SelectManageAccountIdsFail const&) = delete;
		SelectManageAccountIdsFail& operator=(SelectManageAccountIdsFail const&) = delete;
        virtual void ToString(xx::String &str) const override;
        virtual void ToStringCore(xx::String &str) const override;
        virtual void ToBBuffer(xx::BBuffer &bb) const override;
        virtual int FromBBuffer(xx::BBuffer &bb) override;
    };
    // 登录成功
    struct LoginSuccess : PKG::Success
    {
        int64_t id = 0;
        xx::String_p token;

        typedef LoginSuccess ThisType;
        typedef PKG::Success BaseType;
	    LoginSuccess(xx::MemPool* mempool);
	    LoginSuccess(xx::BBuffer *bb);
		LoginSuccess(LoginSuccess const&) = delete;
		LoginSuccess& operator=(LoginSuccess const&) = delete;
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
	    Logout(xx::MemPool* mempool);
	    Logout(xx::BBuffer *bb);
		Logout(Logout const&) = delete;
		Logout& operator=(Logout const&) = delete;
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
        xx::String_p reason;

        typedef PushLogout ThisType;
        typedef xx::Object BaseType;
	    PushLogout(xx::MemPool* mempool);
	    PushLogout(xx::BBuffer *bb);
		PushLogout(PushLogout const&) = delete;
		PushLogout& operator=(PushLogout const&) = delete;
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
        xx::List_p<PKG::Manage::Account_p> rows;

        typedef SelectManageAccountsSuccess ThisType;
        typedef PKG::Success BaseType;
	    SelectManageAccountsSuccess(xx::MemPool* mempool);
	    SelectManageAccountsSuccess(xx::BBuffer *bb);
		SelectManageAccountsSuccess(SelectManageAccountsSuccess const&) = delete;
		SelectManageAccountsSuccess& operator=(SelectManageAccountsSuccess const&) = delete;
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
        xx::String_p text;

        typedef PushMessage ThisType;
        typedef xx::Object BaseType;
	    PushMessage(xx::MemPool* mempool);
	    PushMessage(xx::BBuffer *bb);
		PushMessage(PushMessage const&) = delete;
		PushMessage& operator=(PushMessage const&) = delete;
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
	    PushJoin(xx::MemPool* mempool);
	    PushJoin(xx::BBuffer *bb);
		PushJoin(PushJoin const&) = delete;
		PushJoin& operator=(PushJoin const&) = delete;
        virtual void ToString(xx::String &str) const override;
        virtual void ToStringCore(xx::String &str) const override;
        virtual void ToBBuffer(xx::BBuffer &bb) const override;
        virtual int FromBBuffer(xx::BBuffer &bb) override;
    };
    // 进入失败, 返回错误信息
    struct JoinFail : PKG::Response
    {
        xx::String_p reason;

        typedef JoinFail ThisType;
        typedef PKG::Response BaseType;
	    JoinFail(xx::MemPool* mempool);
	    JoinFail(xx::BBuffer *bb);
		JoinFail(JoinFail const&) = delete;
		JoinFail& operator=(JoinFail const&) = delete;
        virtual void ToString(xx::String &str) const override;
        virtual void ToStringCore(xx::String &str) const override;
        virtual void ToBBuffer(xx::BBuffer &bb) const override;
        virtual int FromBBuffer(xx::BBuffer &bb) override;
    };
    // 进入成功, 返回用户信息
    struct JoinSuccess : PKG::Response
    {
        PKG::UserInfo_p self;
        xx::List_p<PKG::UserInfo_p> users;

        typedef JoinSuccess ThisType;
        typedef PKG::Response BaseType;
	    JoinSuccess(xx::MemPool* mempool);
	    JoinSuccess(xx::BBuffer *bb);
		JoinSuccess(JoinSuccess const&) = delete;
		JoinSuccess& operator=(JoinSuccess const&) = delete;
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
        PKG::UserInfo_p ui;

        typedef Logout ThisType;
        typedef xx::Object BaseType;
	    Logout(xx::MemPool* mempool);
	    Logout(xx::BBuffer *bb);
		Logout(Logout const&) = delete;
		Logout& operator=(Logout const&) = delete;
        virtual void ToString(xx::String &str) const override;
        virtual void ToStringCore(xx::String &str) const override;
        virtual void ToBBuffer(xx::BBuffer &bb) const override;
        virtual int FromBBuffer(xx::BBuffer &bb) override;
    };
    // 发消息
    struct Message : xx::Object
    {
        xx::String_p text;

        typedef Message ThisType;
        typedef xx::Object BaseType;
	    Message(xx::MemPool* mempool);
	    Message(xx::BBuffer *bb);
		Message(Message const&) = delete;
		Message& operator=(Message const&) = delete;
        virtual void ToString(xx::String &str) const override;
        virtual void ToStringCore(xx::String &str) const override;
        virtual void ToBBuffer(xx::BBuffer &bb) const override;
        virtual int FromBBuffer(xx::BBuffer &bb) override;
    };
    // 请求进入服务器
    struct Join : PKG::Request
    {
        xx::String_p username;
        xx::String_p password;

        typedef Join ThisType;
        typedef PKG::Request BaseType;
	    Join(xx::MemPool* mempool);
	    Join(xx::BBuffer *bb);
		Join(Join const&) = delete;
		Join& operator=(Join const&) = delete;
        virtual void ToString(xx::String &str) const override;
        virtual void ToStringCore(xx::String &str) const override;
        virtual void ToBBuffer(xx::BBuffer &bb) const override;
        virtual int FromBBuffer(xx::BBuffer &bb) override;
    };
}
    struct UserInfo : xx::Object
    {
        int64_t id = 0;
        xx::List_p<PKG::Property_p> props;

        typedef UserInfo ThisType;
        typedef xx::Object BaseType;
	    UserInfo(xx::MemPool* mempool);
	    UserInfo(xx::BBuffer *bb);
		UserInfo(UserInfo const&) = delete;
		UserInfo& operator=(UserInfo const&) = delete;
        virtual void ToString(xx::String &str) const override;
        virtual void ToStringCore(xx::String &str) const override;
        virtual void ToBBuffer(xx::BBuffer &bb) const override;
        virtual int FromBBuffer(xx::BBuffer &bb) override;
    };
    struct Properties : PKG::Property
    {
        xx::List_p<PKG::Property_p> value;

        typedef Properties ThisType;
        typedef PKG::Property BaseType;
	    Properties(xx::MemPool* mempool);
	    Properties(xx::BBuffer *bb);
		Properties(Properties const&) = delete;
		Properties& operator=(Properties const&) = delete;
        virtual void ToString(xx::String &str) const override;
        virtual void ToStringCore(xx::String &str) const override;
        virtual void ToBBuffer(xx::BBuffer &bb) const override;
        virtual int FromBBuffer(xx::BBuffer &bb) override;
    };
    struct Property_string : PKG::Property
    {
        xx::String_p value;

        typedef Property_string ThisType;
        typedef PKG::Property BaseType;
	    Property_string(xx::MemPool* mempool);
	    Property_string(xx::BBuffer *bb);
		Property_string(Property_string const&) = delete;
		Property_string& operator=(Property_string const&) = delete;
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
	    Property_double(xx::MemPool* mempool);
	    Property_double(xx::BBuffer *bb);
		Property_double(Property_double const&) = delete;
		Property_double& operator=(Property_double const&) = delete;
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
	    Property_long(xx::MemPool* mempool);
	    Property_long(xx::BBuffer *bb);
		Property_long(Property_long const&) = delete;
		Property_long& operator=(Property_long const&) = delete;
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
        xx::String_p username;
        xx::String_p password;

        typedef Login ThisType;
        typedef PKG::Request BaseType;
	    Login(xx::MemPool* mempool);
	    Login(xx::BBuffer *bb);
		Login(Login const&) = delete;
		Login& operator=(Login const&) = delete;
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
	    SelectManageAccountsFail(xx::MemPool* mempool);
	    SelectManageAccountsFail(xx::BBuffer *bb);
		SelectManageAccountsFail(SelectManageAccountsFail const&) = delete;
		SelectManageAccountsFail& operator=(SelectManageAccountsFail const&) = delete;
        virtual void ToString(xx::String &str) const override;
        virtual void ToStringCore(xx::String &str) const override;
        virtual void ToBBuffer(xx::BBuffer &bb) const override;
        virtual int FromBBuffer(xx::BBuffer &bb) override;
    };
}
	inline Request::Request(xx::MemPool* mempool)
        : xx::Object(mempool)
	{
	}
	inline Request::Request(xx::BBuffer *bb)
        : xx::Object(bb)
	{
	    int rtv = 0;
        if (rtv = bb->Read(serial)) throw rtv;
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

    inline void Request::ToString(xx::String &str) const
    {
        if (memHeader().flags)
        {
        	str.Append("[ \"***** recursived *****\" ]");
        	return;
        }
        else memHeader().flags = 1;

        str.Append("{ \"type\" : \"Request\"");
        ToStringCore(str);
        str.Append(" }");
        
        memHeader().flags = 0;
    }
    inline void Request::ToStringCore(xx::String &str) const
    {
        this->BaseType::ToStringCore(str);
        str.Append(", \"serial\" : ", this->serial);
    }


	inline Response::Response(xx::MemPool* mempool)
        : xx::Object(mempool)
	{
	}
	inline Response::Response(xx::BBuffer *bb)
        : xx::Object(bb)
	{
	    int rtv = 0;
        if (rtv = bb->Read(requestSerial)) throw rtv;
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

    inline void Response::ToString(xx::String &str) const
    {
        if (memHeader().flags)
        {
        	str.Append("[ \"***** recursived *****\" ]");
        	return;
        }
        else memHeader().flags = 1;

        str.Append("{ \"type\" : \"Response\"");
        ToStringCore(str);
        str.Append(" }");
        
        memHeader().flags = 0;
    }
    inline void Response::ToStringCore(xx::String &str) const
    {
        this->BaseType::ToStringCore(str);
        str.Append(", \"requestSerial\" : ", this->requestSerial);
    }


	inline Success::Success(xx::MemPool* mempool)
        : PKG::Response(mempool)
	{
	}
	inline Success::Success(xx::BBuffer *bb)
        : PKG::Response(bb)
	{
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

    inline void Success::ToString(xx::String &str) const
    {
        if (memHeader().flags)
        {
        	str.Append("[ \"***** recursived *****\" ]");
        	return;
        }
        else memHeader().flags = 1;

        str.Append("{ \"type\" : \"Success\"");
        ToStringCore(str);
        str.Append(" }");
        
        memHeader().flags = 0;
    }
    inline void Success::ToStringCore(xx::String &str) const
    {
        this->BaseType::ToStringCore(str);
    }


	inline Fail::Fail(xx::MemPool* mempool)
        : PKG::Response(mempool)
	{
	}
	inline Fail::Fail(xx::BBuffer *bb)
        : PKG::Response(bb)
	{
	    int rtv = 0;
        bb->readLengthLimit = 0;
        if (rtv = bb->Read(reason)) throw rtv;
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

    inline void Fail::ToString(xx::String &str) const
    {
        if (memHeader().flags)
        {
        	str.Append("[ \"***** recursived *****\" ]");
        	return;
        }
        else memHeader().flags = 1;

        str.Append("{ \"type\" : \"Fail\"");
        ToStringCore(str);
        str.Append(" }");
        
        memHeader().flags = 0;
    }
    inline void Fail::ToStringCore(xx::String &str) const
    {
        this->BaseType::ToStringCore(str);
        str.Append(", \"reason\" : ", this->reason);
    }


	inline Property::Property(xx::MemPool* mempool)
        : xx::Object(mempool)
	{
	}
	inline Property::Property(xx::BBuffer *bb)
        : xx::Object(bb)
	{
	    int rtv = 0;
        bb->readLengthLimit = 0;
        if (rtv = bb->Read(name)) throw rtv;
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

    inline void Property::ToString(xx::String &str) const
    {
        if (memHeader().flags)
        {
        	str.Append("[ \"***** recursived *****\" ]");
        	return;
        }
        else memHeader().flags = 1;

        str.Append("{ \"type\" : \"Property\"");
        ToStringCore(str);
        str.Append(" }");
        
        memHeader().flags = 0;
    }
    inline void Property::ToStringCore(xx::String &str) const
    {
        this->BaseType::ToStringCore(str);
        str.Append(", \"name\" : ", this->name);
    }


	inline Property_long::Property_long(xx::MemPool* mempool)
        : PKG::Property(mempool)
	{
	}
	inline Property_long::Property_long(xx::BBuffer *bb)
        : PKG::Property(bb)
	{
	    int rtv = 0;
        if (rtv = bb->Read(value)) throw rtv;
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

    inline void Property_long::ToString(xx::String &str) const
    {
        if (memHeader().flags)
        {
        	str.Append("[ \"***** recursived *****\" ]");
        	return;
        }
        else memHeader().flags = 1;

        str.Append("{ \"type\" : \"Property_long\"");
        ToStringCore(str);
        str.Append(" }");
        
        memHeader().flags = 0;
    }
    inline void Property_long::ToStringCore(xx::String &str) const
    {
        this->BaseType::ToStringCore(str);
        str.Append(", \"value\" : ", this->value);
    }


	inline Property_double::Property_double(xx::MemPool* mempool)
        : PKG::Property(mempool)
	{
	}
	inline Property_double::Property_double(xx::BBuffer *bb)
        : PKG::Property(bb)
	{
	    int rtv = 0;
        if (rtv = bb->Read(value)) throw rtv;
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

    inline void Property_double::ToString(xx::String &str) const
    {
        if (memHeader().flags)
        {
        	str.Append("[ \"***** recursived *****\" ]");
        	return;
        }
        else memHeader().flags = 1;

        str.Append("{ \"type\" : \"Property_double\"");
        ToStringCore(str);
        str.Append(" }");
        
        memHeader().flags = 0;
    }
    inline void Property_double::ToStringCore(xx::String &str) const
    {
        this->BaseType::ToStringCore(str);
        str.Append(", \"value\" : ", this->value);
    }


	inline Property_string::Property_string(xx::MemPool* mempool)
        : PKG::Property(mempool)
	{
	}
	inline Property_string::Property_string(xx::BBuffer *bb)
        : PKG::Property(bb)
	{
	    int rtv = 0;
        bb->readLengthLimit = 0;
        if (rtv = bb->Read(value)) throw rtv;
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

    inline void Property_string::ToString(xx::String &str) const
    {
        if (memHeader().flags)
        {
        	str.Append("[ \"***** recursived *****\" ]");
        	return;
        }
        else memHeader().flags = 1;

        str.Append("{ \"type\" : \"Property_string\"");
        ToStringCore(str);
        str.Append(" }");
        
        memHeader().flags = 0;
    }
    inline void Property_string::ToStringCore(xx::String &str) const
    {
        this->BaseType::ToStringCore(str);
        str.Append(", \"value\" : ", this->value);
    }


	inline Properties::Properties(xx::MemPool* mempool)
        : PKG::Property(mempool)
	{
	}
	inline Properties::Properties(xx::BBuffer *bb)
        : PKG::Property(bb)
	{
	    int rtv = 0;
        bb->readLengthLimit = 0;
        if (rtv = bb->Read(value)) throw rtv;
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

    inline void Properties::ToString(xx::String &str) const
    {
        if (memHeader().flags)
        {
        	str.Append("[ \"***** recursived *****\" ]");
        	return;
        }
        else memHeader().flags = 1;

        str.Append("{ \"type\" : \"Properties\"");
        ToStringCore(str);
        str.Append(" }");
        
        memHeader().flags = 0;
    }
    inline void Properties::ToStringCore(xx::String &str) const
    {
        this->BaseType::ToStringCore(str);
        str.Append(", \"value\" : ", this->value);
    }


	inline UserInfo::UserInfo(xx::MemPool* mempool)
        : xx::Object(mempool)
	{
	}
	inline UserInfo::UserInfo(xx::BBuffer *bb)
        : xx::Object(bb)
	{
	    int rtv = 0;
        if (rtv = bb->Read(id)) throw rtv;
        bb->readLengthLimit = 0;
        if (rtv = bb->Read(props)) throw rtv;
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

    inline void UserInfo::ToString(xx::String &str) const
    {
        if (memHeader().flags)
        {
        	str.Append("[ \"***** recursived *****\" ]");
        	return;
        }
        else memHeader().flags = 1;

        str.Append("{ \"type\" : \"UserInfo\"");
        ToStringCore(str);
        str.Append(" }");
        
        memHeader().flags = 0;
    }
    inline void UserInfo::ToStringCore(xx::String &str) const
    {
        this->BaseType::ToStringCore(str);
        str.Append(", \"id\" : ", this->id);
        str.Append(", \"props\" : ", this->props);
    }


namespace Client_Server
{
	inline Join::Join(xx::MemPool* mempool)
        : PKG::Request(mempool)
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

    inline void Join::ToString(xx::String &str) const
    {
        if (memHeader().flags)
        {
        	str.Append("[ \"***** recursived *****\" ]");
        	return;
        }
        else memHeader().flags = 1;

        str.Append("{ \"type\" : \"Join\"");
        ToStringCore(str);
        str.Append(" }");
        
        memHeader().flags = 0;
    }
    inline void Join::ToStringCore(xx::String &str) const
    {
        this->BaseType::ToStringCore(str);
        str.Append(", \"username\" : ", this->username);
        str.Append(", \"password\" : ", this->password);
    }


	inline Message::Message(xx::MemPool* mempool)
        : xx::Object(mempool)
	{
	}
	inline Message::Message(xx::BBuffer *bb)
        : xx::Object(bb)
	{
	    int rtv = 0;
        bb->readLengthLimit = 256;
        if (rtv = bb->Read(text)) throw rtv;
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

    inline void Message::ToString(xx::String &str) const
    {
        if (memHeader().flags)
        {
        	str.Append("[ \"***** recursived *****\" ]");
        	return;
        }
        else memHeader().flags = 1;

        str.Append("{ \"type\" : \"Message\"");
        ToStringCore(str);
        str.Append(" }");
        
        memHeader().flags = 0;
    }
    inline void Message::ToStringCore(xx::String &str) const
    {
        this->BaseType::ToStringCore(str);
        str.Append(", \"text\" : ", this->text);
    }


	inline Logout::Logout(xx::MemPool* mempool)
        : xx::Object(mempool)
	{
	}
	inline Logout::Logout(xx::BBuffer *bb)
        : xx::Object(bb)
	{
	    int rtv = 0;
        if (rtv = bb->Read(ui)) throw rtv;
	}
    inline void Logout::ToBBuffer(xx::BBuffer &bb) const
    {
        bb.Write(this->ui);
    }
    inline int Logout::FromBBuffer(xx::BBuffer &bb)
    {
        int rtv = 0;
        if (rtv = bb.Read(this->ui)) return rtv;
        return rtv;
    }

    inline void Logout::ToString(xx::String &str) const
    {
        if (memHeader().flags)
        {
        	str.Append("[ \"***** recursived *****\" ]");
        	return;
        }
        else memHeader().flags = 1;

        str.Append("{ \"type\" : \"Logout\"");
        ToStringCore(str);
        str.Append(" }");
        
        memHeader().flags = 0;
    }
    inline void Logout::ToStringCore(xx::String &str) const
    {
        this->BaseType::ToStringCore(str);
        str.Append(", \"ui\" : ", this->ui);
    }


}
namespace Server_Client
{
	inline JoinSuccess::JoinSuccess(xx::MemPool* mempool)
        : PKG::Response(mempool)
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

    inline void JoinSuccess::ToString(xx::String &str) const
    {
        if (memHeader().flags)
        {
        	str.Append("[ \"***** recursived *****\" ]");
        	return;
        }
        else memHeader().flags = 1;

        str.Append("{ \"type\" : \"JoinSuccess\"");
        ToStringCore(str);
        str.Append(" }");
        
        memHeader().flags = 0;
    }
    inline void JoinSuccess::ToStringCore(xx::String &str) const
    {
        this->BaseType::ToStringCore(str);
        str.Append(", \"self\" : ", this->self);
        str.Append(", \"users\" : ", this->users);
    }


	inline JoinFail::JoinFail(xx::MemPool* mempool)
        : PKG::Response(mempool)
	{
	}
	inline JoinFail::JoinFail(xx::BBuffer *bb)
        : PKG::Response(bb)
	{
	    int rtv = 0;
        bb->readLengthLimit = 0;
        if (rtv = bb->Read(reason)) throw rtv;
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

    inline void JoinFail::ToString(xx::String &str) const
    {
        if (memHeader().flags)
        {
        	str.Append("[ \"***** recursived *****\" ]");
        	return;
        }
        else memHeader().flags = 1;

        str.Append("{ \"type\" : \"JoinFail\"");
        ToStringCore(str);
        str.Append(" }");
        
        memHeader().flags = 0;
    }
    inline void JoinFail::ToStringCore(xx::String &str) const
    {
        this->BaseType::ToStringCore(str);
        str.Append(", \"reason\" : ", this->reason);
    }


	inline PushJoin::PushJoin(xx::MemPool* mempool)
        : xx::Object(mempool)
	{
	}
	inline PushJoin::PushJoin(xx::BBuffer *bb)
        : xx::Object(bb)
	{
	    int rtv = 0;
        if (rtv = bb->Read(id)) throw rtv;
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

    inline void PushJoin::ToString(xx::String &str) const
    {
        if (memHeader().flags)
        {
        	str.Append("[ \"***** recursived *****\" ]");
        	return;
        }
        else memHeader().flags = 1;

        str.Append("{ \"type\" : \"PushJoin\"");
        ToStringCore(str);
        str.Append(" }");
        
        memHeader().flags = 0;
    }
    inline void PushJoin::ToStringCore(xx::String &str) const
    {
        this->BaseType::ToStringCore(str);
        str.Append(", \"id\" : ", this->id);
    }


	inline PushMessage::PushMessage(xx::MemPool* mempool)
        : xx::Object(mempool)
	{
	}
	inline PushMessage::PushMessage(xx::BBuffer *bb)
        : xx::Object(bb)
	{
	    int rtv = 0;
        if (rtv = bb->Read(id)) throw rtv;
        bb->readLengthLimit = 0;
        if (rtv = bb->Read(text)) throw rtv;
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

    inline void PushMessage::ToString(xx::String &str) const
    {
        if (memHeader().flags)
        {
        	str.Append("[ \"***** recursived *****\" ]");
        	return;
        }
        else memHeader().flags = 1;

        str.Append("{ \"type\" : \"PushMessage\"");
        ToStringCore(str);
        str.Append(" }");
        
        memHeader().flags = 0;
    }
    inline void PushMessage::ToStringCore(xx::String &str) const
    {
        this->BaseType::ToStringCore(str);
        str.Append(", \"id\" : ", this->id);
        str.Append(", \"text\" : ", this->text);
    }


	inline PushLogout::PushLogout(xx::MemPool* mempool)
        : xx::Object(mempool)
	{
	}
	inline PushLogout::PushLogout(xx::BBuffer *bb)
        : xx::Object(bb)
	{
	    int rtv = 0;
        if (rtv = bb->Read(id)) throw rtv;
        bb->readLengthLimit = 0;
        if (rtv = bb->Read(reason)) throw rtv;
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

    inline void PushLogout::ToString(xx::String &str) const
    {
        if (memHeader().flags)
        {
        	str.Append("[ \"***** recursived *****\" ]");
        	return;
        }
        else memHeader().flags = 1;

        str.Append("{ \"type\" : \"PushLogout\"");
        ToStringCore(str);
        str.Append(" }");
        
        memHeader().flags = 0;
    }
    inline void PushLogout::ToStringCore(xx::String &str) const
    {
        this->BaseType::ToStringCore(str);
        str.Append(", \"id\" : ", this->id);
        str.Append(", \"reason\" : ", this->reason);
    }


}
namespace Manage_DB
{
	inline Login::Login(xx::MemPool* mempool)
        : PKG::Request(mempool)
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

    inline void Login::ToString(xx::String &str) const
    {
        if (memHeader().flags)
        {
        	str.Append("[ \"***** recursived *****\" ]");
        	return;
        }
        else memHeader().flags = 1;

        str.Append("{ \"type\" : \"Login\"");
        ToStringCore(str);
        str.Append(" }");
        
        memHeader().flags = 0;
    }
    inline void Login::ToStringCore(xx::String &str) const
    {
        this->BaseType::ToStringCore(str);
        str.Append(", \"username\" : ", this->username);
        str.Append(", \"password\" : ", this->password);
    }


	inline Logout::Logout(xx::MemPool* mempool)
        : PKG::Request(mempool)
	{
	}
	inline Logout::Logout(xx::BBuffer *bb)
        : PKG::Request(bb)
	{
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

    inline void Logout::ToString(xx::String &str) const
    {
        if (memHeader().flags)
        {
        	str.Append("[ \"***** recursived *****\" ]");
        	return;
        }
        else memHeader().flags = 1;

        str.Append("{ \"type\" : \"Logout\"");
        ToStringCore(str);
        str.Append(" }");
        
        memHeader().flags = 0;
    }
    inline void Logout::ToStringCore(xx::String &str) const
    {
        this->BaseType::ToStringCore(str);
    }


	inline SelectManageAccountIds::SelectManageAccountIds(xx::MemPool* mempool)
        : PKG::Request(mempool)
	{
	}
	inline SelectManageAccountIds::SelectManageAccountIds(xx::BBuffer *bb)
        : PKG::Request(bb)
	{
	    int rtv = 0;
        if (rtv = bb->Read(limit)) throw rtv;
        if (rtv = bb->Read(sort)) throw rtv;
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

    inline void SelectManageAccountIds::ToString(xx::String &str) const
    {
        if (memHeader().flags)
        {
        	str.Append("[ \"***** recursived *****\" ]");
        	return;
        }
        else memHeader().flags = 1;

        str.Append("{ \"type\" : \"SelectManageAccountIds\"");
        ToStringCore(str);
        str.Append(" }");
        
        memHeader().flags = 0;
    }
    inline void SelectManageAccountIds::ToStringCore(xx::String &str) const
    {
        this->BaseType::ToStringCore(str);
        str.Append(", \"limit\" : ", this->limit);
        str.Append(", \"sort\" : ", this->sort);
    }


	inline SelectManageAccounts::SelectManageAccounts(xx::MemPool* mempool)
        : PKG::Request(mempool)
	{
	}
	inline SelectManageAccounts::SelectManageAccounts(xx::BBuffer *bb)
        : PKG::Request(bb)
	{
	    int rtv = 0;
        bb->readLengthLimit = 50;
        if (rtv = bb->Read(ids)) throw rtv;
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

    inline void SelectManageAccounts::ToString(xx::String &str) const
    {
        if (memHeader().flags)
        {
        	str.Append("[ \"***** recursived *****\" ]");
        	return;
        }
        else memHeader().flags = 1;

        str.Append("{ \"type\" : \"SelectManageAccounts\"");
        ToStringCore(str);
        str.Append(" }");
        
        memHeader().flags = 0;
    }
    inline void SelectManageAccounts::ToStringCore(xx::String &str) const
    {
        this->BaseType::ToStringCore(str);
        str.Append(", \"ids\" : ", this->ids);
    }


}
namespace Manage
{
	inline Account::Account(xx::MemPool* mempool)
        : xx::Object(mempool)
	{
	}
	inline Account::Account(xx::BBuffer *bb)
        : xx::Object(bb)
	{
	    int rtv = 0;
        if (rtv = bb->Read(id)) throw rtv;
        bb->readLengthLimit = 0;
        if (rtv = bb->Read(username)) throw rtv;
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

    inline void Account::ToString(xx::String &str) const
    {
        if (memHeader().flags)
        {
        	str.Append("[ \"***** recursived *****\" ]");
        	return;
        }
        else memHeader().flags = 1;

        str.Append("{ \"type\" : \"Account\"");
        ToStringCore(str);
        str.Append(" }");
        
        memHeader().flags = 0;
    }
    inline void Account::ToStringCore(xx::String &str) const
    {
        this->BaseType::ToStringCore(str);
        str.Append(", \"id\" : ", this->id);
        str.Append(", \"username\" : ", this->username);
    }


	inline Role::Role(xx::MemPool* mempool)
        : xx::Object(mempool)
	{
	}
	inline Role::Role(xx::BBuffer *bb)
        : xx::Object(bb)
	{
	    int rtv = 0;
        if (rtv = bb->Read(id)) throw rtv;
        bb->readLengthLimit = 0;
        if (rtv = bb->Read(name)) throw rtv;
        bb->readLengthLimit = 0;
        if (rtv = bb->Read(desc)) throw rtv;
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

    inline void Role::ToString(xx::String &str) const
    {
        if (memHeader().flags)
        {
        	str.Append("[ \"***** recursived *****\" ]");
        	return;
        }
        else memHeader().flags = 1;

        str.Append("{ \"type\" : \"Role\"");
        ToStringCore(str);
        str.Append(" }");
        
        memHeader().flags = 0;
    }
    inline void Role::ToStringCore(xx::String &str) const
    {
        this->BaseType::ToStringCore(str);
        str.Append(", \"id\" : ", this->id);
        str.Append(", \"name\" : ", this->name);
        str.Append(", \"desc\" : ", this->desc);
    }


	inline Permission::Permission(xx::MemPool* mempool)
        : xx::Object(mempool)
	{
	}
	inline Permission::Permission(xx::BBuffer *bb)
        : xx::Object(bb)
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

    inline void Permission::ToString(xx::String &str) const
    {
        if (memHeader().flags)
        {
        	str.Append("[ \"***** recursived *****\" ]");
        	return;
        }
        else memHeader().flags = 1;

        str.Append("{ \"type\" : \"Permission\"");
        ToStringCore(str);
        str.Append(" }");
        
        memHeader().flags = 0;
    }
    inline void Permission::ToStringCore(xx::String &str) const
    {
        this->BaseType::ToStringCore(str);
        str.Append(", \"id\" : ", this->id);
        str.Append(", \"group\" : ", this->group);
        str.Append(", \"name\" : ", this->name);
        str.Append(", \"desc\" : ", this->desc);
    }


	inline BindAccountRole::BindAccountRole(xx::MemPool* mempool)
        : xx::Object(mempool)
	{
	}
	inline BindAccountRole::BindAccountRole(xx::BBuffer *bb)
        : xx::Object(bb)
	{
	    int rtv = 0;
        if (rtv = bb->Read(account_id)) throw rtv;
        if (rtv = bb->Read(role_id)) throw rtv;
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

    inline void BindAccountRole::ToString(xx::String &str) const
    {
        if (memHeader().flags)
        {
        	str.Append("[ \"***** recursived *****\" ]");
        	return;
        }
        else memHeader().flags = 1;

        str.Append("{ \"type\" : \"BindAccountRole\"");
        ToStringCore(str);
        str.Append(" }");
        
        memHeader().flags = 0;
    }
    inline void BindAccountRole::ToStringCore(xx::String &str) const
    {
        this->BaseType::ToStringCore(str);
        str.Append(", \"account_id\" : ", this->account_id);
        str.Append(", \"role_id\" : ", this->role_id);
    }


	inline BindRolePermission::BindRolePermission(xx::MemPool* mempool)
        : xx::Object(mempool)
	{
	}
	inline BindRolePermission::BindRolePermission(xx::BBuffer *bb)
        : xx::Object(bb)
	{
	    int rtv = 0;
        if (rtv = bb->Read(role_id)) throw rtv;
        if (rtv = bb->Read(permission_id)) throw rtv;
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

    inline void BindRolePermission::ToString(xx::String &str) const
    {
        if (memHeader().flags)
        {
        	str.Append("[ \"***** recursived *****\" ]");
        	return;
        }
        else memHeader().flags = 1;

        str.Append("{ \"type\" : \"BindRolePermission\"");
        ToStringCore(str);
        str.Append(" }");
        
        memHeader().flags = 0;
    }
    inline void BindRolePermission::ToStringCore(xx::String &str) const
    {
        this->BaseType::ToStringCore(str);
        str.Append(", \"role_id\" : ", this->role_id);
        str.Append(", \"permission_id\" : ", this->permission_id);
    }


}
namespace DB_Manage
{
	inline LoginSuccess::LoginSuccess(xx::MemPool* mempool)
        : PKG::Success(mempool)
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

    inline void LoginSuccess::ToString(xx::String &str) const
    {
        if (memHeader().flags)
        {
        	str.Append("[ \"***** recursived *****\" ]");
        	return;
        }
        else memHeader().flags = 1;

        str.Append("{ \"type\" : \"LoginSuccess\"");
        ToStringCore(str);
        str.Append(" }");
        
        memHeader().flags = 0;
    }
    inline void LoginSuccess::ToStringCore(xx::String &str) const
    {
        this->BaseType::ToStringCore(str);
        str.Append(", \"id\" : ", this->id);
        str.Append(", \"token\" : ", this->token);
    }


	inline LoginFail::LoginFail(xx::MemPool* mempool)
        : PKG::Fail(mempool)
	{
	}
	inline LoginFail::LoginFail(xx::BBuffer *bb)
        : PKG::Fail(bb)
	{
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

    inline void LoginFail::ToString(xx::String &str) const
    {
        if (memHeader().flags)
        {
        	str.Append("[ \"***** recursived *****\" ]");
        	return;
        }
        else memHeader().flags = 1;

        str.Append("{ \"type\" : \"LoginFail\"");
        ToStringCore(str);
        str.Append(" }");
        
        memHeader().flags = 0;
    }
    inline void LoginFail::ToStringCore(xx::String &str) const
    {
        this->BaseType::ToStringCore(str);
    }


	inline LogoutSuccess::LogoutSuccess(xx::MemPool* mempool)
        : PKG::Success(mempool)
	{
	}
	inline LogoutSuccess::LogoutSuccess(xx::BBuffer *bb)
        : PKG::Success(bb)
	{
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

    inline void LogoutSuccess::ToString(xx::String &str) const
    {
        if (memHeader().flags)
        {
        	str.Append("[ \"***** recursived *****\" ]");
        	return;
        }
        else memHeader().flags = 1;

        str.Append("{ \"type\" : \"LogoutSuccess\"");
        ToStringCore(str);
        str.Append(" }");
        
        memHeader().flags = 0;
    }
    inline void LogoutSuccess::ToStringCore(xx::String &str) const
    {
        this->BaseType::ToStringCore(str);
    }


	inline LogoutFail::LogoutFail(xx::MemPool* mempool)
        : PKG::Fail(mempool)
	{
	}
	inline LogoutFail::LogoutFail(xx::BBuffer *bb)
        : PKG::Fail(bb)
	{
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

    inline void LogoutFail::ToString(xx::String &str) const
    {
        if (memHeader().flags)
        {
        	str.Append("[ \"***** recursived *****\" ]");
        	return;
        }
        else memHeader().flags = 1;

        str.Append("{ \"type\" : \"LogoutFail\"");
        ToStringCore(str);
        str.Append(" }");
        
        memHeader().flags = 0;
    }
    inline void LogoutFail::ToStringCore(xx::String &str) const
    {
        this->BaseType::ToStringCore(str);
    }


	inline SelectManageAccountIdsSuccess::SelectManageAccountIdsSuccess(xx::MemPool* mempool)
        : PKG::Success(mempool)
	{
	}
	inline SelectManageAccountIdsSuccess::SelectManageAccountIdsSuccess(xx::BBuffer *bb)
        : PKG::Success(bb)
	{
	    int rtv = 0;
        bb->readLengthLimit = 0;
        if (rtv = bb->Read(ids)) throw rtv;
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

    inline void SelectManageAccountIdsSuccess::ToString(xx::String &str) const
    {
        if (memHeader().flags)
        {
        	str.Append("[ \"***** recursived *****\" ]");
        	return;
        }
        else memHeader().flags = 1;

        str.Append("{ \"type\" : \"SelectManageAccountIdsSuccess\"");
        ToStringCore(str);
        str.Append(" }");
        
        memHeader().flags = 0;
    }
    inline void SelectManageAccountIdsSuccess::ToStringCore(xx::String &str) const
    {
        this->BaseType::ToStringCore(str);
        str.Append(", \"ids\" : ", this->ids);
    }


	inline SelectManageAccountIdsFail::SelectManageAccountIdsFail(xx::MemPool* mempool)
        : PKG::Fail(mempool)
	{
	}
	inline SelectManageAccountIdsFail::SelectManageAccountIdsFail(xx::BBuffer *bb)
        : PKG::Fail(bb)
	{
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

    inline void SelectManageAccountIdsFail::ToString(xx::String &str) const
    {
        if (memHeader().flags)
        {
        	str.Append("[ \"***** recursived *****\" ]");
        	return;
        }
        else memHeader().flags = 1;

        str.Append("{ \"type\" : \"SelectManageAccountIdsFail\"");
        ToStringCore(str);
        str.Append(" }");
        
        memHeader().flags = 0;
    }
    inline void SelectManageAccountIdsFail::ToStringCore(xx::String &str) const
    {
        this->BaseType::ToStringCore(str);
    }


	inline SelectManageAccountsSuccess::SelectManageAccountsSuccess(xx::MemPool* mempool)
        : PKG::Success(mempool)
	{
	}
	inline SelectManageAccountsSuccess::SelectManageAccountsSuccess(xx::BBuffer *bb)
        : PKG::Success(bb)
	{
	    int rtv = 0;
        bb->readLengthLimit = 0;
        if (rtv = bb->Read(rows)) throw rtv;
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

    inline void SelectManageAccountsSuccess::ToString(xx::String &str) const
    {
        if (memHeader().flags)
        {
        	str.Append("[ \"***** recursived *****\" ]");
        	return;
        }
        else memHeader().flags = 1;

        str.Append("{ \"type\" : \"SelectManageAccountsSuccess\"");
        ToStringCore(str);
        str.Append(" }");
        
        memHeader().flags = 0;
    }
    inline void SelectManageAccountsSuccess::ToStringCore(xx::String &str) const
    {
        this->BaseType::ToStringCore(str);
        str.Append(", \"rows\" : ", this->rows);
    }


	inline SelectManageAccountsFail::SelectManageAccountsFail(xx::MemPool* mempool)
        : PKG::Fail(mempool)
	{
	}
	inline SelectManageAccountsFail::SelectManageAccountsFail(xx::BBuffer *bb)
        : PKG::Fail(bb)
	{
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

    inline void SelectManageAccountsFail::ToString(xx::String &str) const
    {
        if (memHeader().flags)
        {
        	str.Append("[ \"***** recursived *****\" ]");
        	return;
        }
        else memHeader().flags = 1;

        str.Append("{ \"type\" : \"SelectManageAccountsFail\"");
        ToStringCore(str);
        str.Append(" }");
        
        memHeader().flags = 0;
    }
    inline void SelectManageAccountsFail::ToStringCore(xx::String &str) const
    {
        this->BaseType::ToStringCore(str);
    }


}
}
namespace xx
{
	template<> struct TypeId<PKG::Request> { static const uint16_t value = 3; };
	template<> struct TypeId<PKG::Response> { static const uint16_t value = 4; };
	template<> struct TypeId<PKG::Success> { static const uint16_t value = 5; };
	template<> struct TypeId<PKG::Fail> { static const uint16_t value = 6; };
	template<> struct TypeId<PKG::Property> { static const uint16_t value = 7; };
	template<> struct TypeId<PKG::Property_long> { static const uint16_t value = 8; };
	template<> struct TypeId<PKG::Property_double> { static const uint16_t value = 9; };
	template<> struct TypeId<PKG::Property_string> { static const uint16_t value = 10; };
	template<> struct TypeId<PKG::Properties> { static const uint16_t value = 11; };
	template<> struct TypeId<xx::List<PKG::Property_p>> { static const uint16_t value = 12; };
	template<> struct TypeId<PKG::UserInfo> { static const uint16_t value = 13; };
	template<> struct TypeId<PKG::Client_Server::Join> { static const uint16_t value = 14; };
	template<> struct TypeId<PKG::Client_Server::Message> { static const uint16_t value = 15; };
	template<> struct TypeId<PKG::Client_Server::Logout> { static const uint16_t value = 16; };
	template<> struct TypeId<PKG::Server_Client::JoinSuccess> { static const uint16_t value = 17; };
	template<> struct TypeId<xx::List<PKG::UserInfo_p>> { static const uint16_t value = 18; };
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
	template<> struct TypeId<xx::List<PKG::Manage::Account_p>> { static const uint16_t value = 40; };
	template<> struct TypeId<PKG::DB_Manage::SelectManageAccountsFail> { static const uint16_t value = 41; };
}
namespace PKG
{
	inline void AllTypesRegister()
	{
	    xx::MemPool::Register<PKG::Request, xx::Object>();
	    xx::MemPool::Register<PKG::Response, xx::Object>();
	    xx::MemPool::Register<PKG::Success, PKG::Response>();
	    xx::MemPool::Register<PKG::Fail, PKG::Response>();
	    xx::MemPool::Register<PKG::Property, xx::Object>();
	    xx::MemPool::Register<PKG::Property_long, PKG::Property>();
	    xx::MemPool::Register<PKG::Property_double, PKG::Property>();
	    xx::MemPool::Register<PKG::Property_string, PKG::Property>();
	    xx::MemPool::Register<PKG::Properties, PKG::Property>();
	    xx::MemPool::Register<xx::List<PKG::Property_p>, xx::Object>();
	    xx::MemPool::Register<PKG::UserInfo, xx::Object>();
	    xx::MemPool::Register<PKG::Client_Server::Join, PKG::Request>();
	    xx::MemPool::Register<PKG::Client_Server::Message, xx::Object>();
	    xx::MemPool::Register<PKG::Client_Server::Logout, xx::Object>();
	    xx::MemPool::Register<PKG::Server_Client::JoinSuccess, PKG::Response>();
	    xx::MemPool::Register<xx::List<PKG::UserInfo_p>, xx::Object>();
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
	    xx::MemPool::Register<xx::List<PKG::Manage::Account_p>, xx::Object>();
	    xx::MemPool::Register<PKG::DB_Manage::SelectManageAccountsFail, PKG::Fail>();
	}
}
