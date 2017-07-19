#pragma warning disable 0169, 0414
using TemplateLibrary;

namespace Game1
{
    [Desc("游戏阶段列表")]
    enum Stages : byte
    {
        [Desc("等人点准备以开始游戏")]
        WaitReady,
        [Desc("洗牌")]
        Shuffle,
        [Desc("发牌")]
        Deal,
        [Desc("下底注")]
        BaseBet,
        [Desc("看牌")]
        Watch,
        [Desc("依次操作")]
        Foreach,
        [Desc("最终比牌")]
        Match,
        [Desc("局终报告")]
        Report
    }

    [Desc("参与游戏的玩家")]
    class Player
    {
        [Desc("账号id")]
        int accountId;

        [Desc("临时票据"), NotSerialize]
        string token;

        [Desc("当前金币数")]
        long coin;

        [Desc("当前位于的桌")]
        Desk desk;

        [Desc("自己位于容器中的下标")]
        int desk_players_index;

        [Desc("本局游戏是否参与")]
        bool joined;

        [Desc("手牌"), CustomSerialize]
        List<Poker.Card> cards;

        [Desc("是否看过")]
        bool shown;

        [Desc("已下注金额")]
        long stake;

        [Desc("开牌目标")]
        int openTarget;

        [Desc("步骤自增编号")]
        int stepId;

        // todo: more 
    }

    [Desc("基本单位: 桌")]
    class Desk
    {
        [Desc("本桌游戏的流水号()")]
        int id;

        [Desc("在座的所有玩家( 包括参与者与观众 )")]
        List<Player> players;

        [Desc("初始 52 张牌"), NotSerialize]
        List<Poker.Card> cards;

        [Desc("已下注金额")]
        long totalStake;

        // todo: 基于 游戏阶段列表 的所有 玩家游标, 计时器啥的

        // todo: 游戏结算相关配置
    }

}
