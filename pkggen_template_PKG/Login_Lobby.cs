#pragma warning disable 0169, 0414
using TemplateLibrary;

// 登录 -> 大厅
namespace Login_Lobby
{
    [Desc("登录通知大厅有玩家要进来")]
    class Join : Global.Request
    {
        int accountId;
    }
}
