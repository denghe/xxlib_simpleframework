#pragma warning disable 0169, 0414
using TemplateLibrary;

// 大厅 -> 游戏1
namespace Lobby_Game1
{
    [Desc("大厅通知游戏有玩家要进来")]
    class Join : Global.Request
    {
        int accountId; // todo: more id? id list? more args?
    }
}
