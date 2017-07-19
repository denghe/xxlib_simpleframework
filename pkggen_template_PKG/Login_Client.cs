#pragma warning disable 0169, 0414
using TemplateLibrary;

// 登录 -> 客户端
namespace Login_Client
{
    [Desc("登录成功, 返回导航地址和 token")]
    class LoginSuccess
    {
        int accountId;
        string token;
        Global.IPAddress serviceAddress;
        [Desc("目标地址可能是大厅也可能是游戏")]
        Global.ServiceTypes serviceType;
    }
    [Desc("登录失败( 含原因 )")]
    class LoginFail
    {
        int errorCode;      // todo: 枚举化
        string reason;
    }
}
