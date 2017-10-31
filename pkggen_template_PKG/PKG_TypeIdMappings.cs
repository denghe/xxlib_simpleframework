#pragma warning disable 0169, 0414
using TemplateLibrary;

[TypeIdMappings]
interface ITypeIdMappings
{
    Request _3 { get; }

    Response _4 { get; }

    Success _5 { get; }

    Fail _6 { get; }

    Property _7 { get; }

    Property_long _8 { get; }

    Property_double _9 { get; }

    Property_string _10 { get; }

    Properties _11 { get; }

    List<Property> _12 { get; }

    UserInfo _13 { get; }

    Client_Server.Join _14 { get; }

    Client_Server.Message _15 { get; }

    Client_Server.Logout _16 { get; }

    Server_Client.JoinSuccess _17 { get; }

    List<UserInfo> _18 { get; }

    Server_Client.JoinFail _19 { get; }

    Server_Client.PushJoin _20 { get; }

    Server_Client.PushMessage _21 { get; }

    Server_Client.PushLogout _22 { get; }

    Manage_DB.Login _23 { get; }

    Manage_DB.Logout _24 { get; }

    Manage_DB.SelectManageAccountIds _25 { get; }

    Manage_DB.SelectManageAccounts _26 { get; }

    List<long> _27 { get; }

    Manage.Account _28 { get; }

    Manage.Role _29 { get; }

    Manage.Permission _30 { get; }

    Manage.BindAccountRole _31 { get; }

    Manage.BindRolePermission _32 { get; }

    DB_Manage.LoginSuccess _33 { get; }

    DB_Manage.LoginFail _34 { get; }

    DB_Manage.LogoutSuccess _35 { get; }

    DB_Manage.LogoutFail _36 { get; }

    DB_Manage.SelectManageAccountIdsSuccess _37 { get; }

    DB_Manage.SelectManageAccountIdsFail _38 { get; }

    DB_Manage.SelectManageAccountsSuccess _39 { get; }

    List<Manage.Account> _40 { get; }

    DB_Manage.SelectManageAccountsFail _41 { get; }

}
