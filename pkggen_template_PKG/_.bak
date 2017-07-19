#pragma warning disable 0169, 0414
using TemplateLibrary;

// 各种公共类结构 / 枚举
namespace Global
{
    // 虽然用作项目根命名空间枚举, 但其本身也是一个正常的 enum
    [CategoryNamespaces]
    enum ServiceTypes
    {
        Loader, Discovery, Navgate, Proxy, Login, Lobby, Room, DB, DBCommit
    }

    [Desc("服务地址信息")]
    class IPAddress
    {
        // 当前这里并不关注多线IP属于什么运营商, 具体用什么靠实际ping延迟

        [Desc("是否为内网 IP( 内部通信主用 ). false 通常出现在 proxy 服务上, 用以汇报自己的多个多线外网 ip")]
        bool lan;

        [Desc("IPV4 或 V6 的地址")]
        string ip;

        [Desc("端口")]
        int port;
    }

    [Desc("服务信息描述")]
    class Service
    {
        [Desc("实例类型")]
        ServiceTypes type;

        [Desc("实例级别( 如果是根级别, 该值通常为 0. 子级别非 0 )")]
        int level;

        [Desc("实例id( 注册时如果传 0, 则表示由 Discovery 服务来分配一个 id. 已得到过就传之前的. 类似 DHCP 自动分配 IP )")]
        int instanceId;

        [Desc("IP集合( 大多数内部服务只有1个内网 IP:PORT. 网关可能有多个, 比如多线机房 )")]
        List<IPAddress> addrs;

        [Desc("时间戳( 用以和 Discovery 校对时间 )")]
        ulong timestamp;
    }
}

// 注册( 带 订阅指定服务的注册通知 ), 心跳
namespace Discovery
{
    [Desc("服务启动后用该指令告知 Discovery 自己的信息")]
    class Register : Global.Service
    {
        [Desc("订阅感兴趣的服务的注册行为")]
        List<Global.ServiceTypes> subscribeServiceTypes;
    }

    [Desc("注册结果--成功")]
    class Register_Success
    {
        [Desc("订阅服务类型中已注册的服务列表")]
        List<Global.Service> services;
    }

    [Desc("注册结果--失败: 重复的实例id")]
    class Register_DupeInstanceId
    {
        [Desc("重复项的明细")]
        Global.Service tar;
    }

    [Desc("注册结果--失败: 重复的IP:PORT( 可能出现于多IP服务配错 )")]
    class Register_DupeIPPort
    {
        [Desc("重复项的明细")]
        Global.Service tar;
    }

    // todo: more errors

    [Desc("后续订阅服务推送( 这个并不优化为批量推送. 注册一个推送一个 )")]
    class Push
    {
        [Desc("订阅服务类型中已注册的服务")]
        Global.Service service;
    }



    [Desc("保持活跃")]
    class KeepAlive
    {
        ulong timestamp;
    }

    [Desc("保持活跃结果--成功")]
    class KeepAlive_Success
    {
    }

    [Desc("保持活跃结果--失败( 时间差异大 )")]
    class KeepAlive_BadTimestamp
    {
    }
    [Desc("保持活跃结果--失败( 已超时? )")]
    class KeepAlive_Timeout
    {
    }

    // todo: more errors

}



namespace Proxy
{
    // todo
}



//namespace Desk
//{
//    class Join
//    {
//        [Desc("Desk 实例id ( 地址信息 )")]
//        int instanceId;

//        // ...... token? ....
//    }
//}




//enum Color
//{
//    Red, Blue
//}

//struct Pos
//{
//    float x, y;
//}

//class SceneObj
//{
//    [Desc("所在场景")]
//    Scene scene;
//}

//class Monster : SceneObj
//{
//    [Desc("位于 scene monster 容器中的下标 for 交换快删")]
//    int scene_monsters_index;
//    [CreateInstance]
//    string name;
//    Color color = Color.Blue;
//    Pos pos;
//}

//class Deamon : Monster
//{
//}

//class Butcher : Monster
//{
//}

//class Scene
//{
//    [CreateInstance]
//    List<Monster> monsters;
//}
