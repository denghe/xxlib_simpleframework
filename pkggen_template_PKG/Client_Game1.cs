#pragma warning disable 0169, 0414
using TemplateLibrary;

// todo: 金花的具体逻辑用到的数据结构


// 客户端 -> 游戏1
namespace Client_Game1
{
    [Desc("进入大厅")]
    class Join
    {
        [Desc("账号id")]
        int accountId;

        [Desc("临时票据")]
        [Limit(64)]
        string token;
    }

    [Desc("主动退出")]
    class Quit
    {
    }

    // todo: 金花的具体流程用到的包

}
