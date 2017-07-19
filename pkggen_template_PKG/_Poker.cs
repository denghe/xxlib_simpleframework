#pragma warning disable 0169, 0414
using TemplateLibrary;

// 扑克相关结构体
namespace Poker
{
    [Desc("花色")]
    enum Sets : byte
    {
        [Desc("黑桃")]
        Spades = 1,
        [Desc("红心")]
        Hearts,
        [Desc("樱桃/梅花")]
        Clubs,
        [Desc("方片/方块")]
        Diamonds
    }

    [Desc("点数")]
    enum Points : byte
    {
        Ace = 1,
        _2,
        _3,
        _4,
        _5,
        _6,
        _7,
        _8,
        _9,
        _10,
        Jack,
        Queen,
        King,
        [Desc("小王/小鬼")]
        Joker,
        [Desc("大王/大鬼")]
        JokerEx
    }

    [Desc("牌")]
    struct Card
    {
        Sets set;
        Points point;
    }

    [Desc("某玩家手上的牌")]
    class Suit
    {
        int accountId;
        List<Card> cards;
    }
}
