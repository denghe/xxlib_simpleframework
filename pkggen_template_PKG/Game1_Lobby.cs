#pragma warning disable 0169, 0414
using TemplateLibrary;

// 游戏1 -> 大厅
namespace Game1_Lobby
{
    [Desc("游戏1 返回给 大厅 给玩家分配的 token")]
    class JoinSuccess : Global.Response
    {
        [Desc("该 token 由 Game1 生成, 下发给 client, 并带超时时间. 在规定时间内, 如果 client 没拿着 token 连上来, 则过期, 重新通知大厅 缺人, 等大厅裁决")]
        string token;
    }

    [Desc("游戏1 返回给 大厅 进入失败( 满员?? 无资格?? 内部错误 )")]
    class JoinFail : Global.Response
    {
        string reason;
    }

    [Desc("client 并没有在规定时间拿着 token 来连, 或是玩家逻辑断线 / 一局完 / 强行退出")]
    class Quit
    {
        int accountId;
        string reason;
    }
}
