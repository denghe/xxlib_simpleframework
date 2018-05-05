#pragma warning disable 0169, 0414
using TemplateLibrary;

// 引用到外部类型

namespace xx
{
    // 坐标
    [External]
    struct Pos { }

    // 随机数发生器
    [External]
    class Random { }

}

// 玩家网络连接（仅服务器端）
[External(false)]
class ClientPeer { }

// 显示用精灵（仅客户端）
[External(false)]
class Sprite { }

// 显示用动画（仅客户端）
[External(false)]
class Animation { }
