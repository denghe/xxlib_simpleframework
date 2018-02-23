#pragma warning disable 0169, 0414
using TemplateLibrary;

[TypeIdMappings]
interface ITypeIdMappings
{
    DB_Manage.MsgResult _11 { get; }

    Manage_DB.Msg _12 { get; }

    Login_Client.LoginSuccess _4 { get; }

    Client_Login.Login _5 { get; }

    DB_Login.AuthSuccess _6 { get; }

    Login_DB.Auth _7 { get; }

    Generic.ServiceInfo _3 { get; }

    Generic.Error _8 { get; }

    Generic.Ping _9 { get; }

    Generic.Pong _10 { get; }

}
