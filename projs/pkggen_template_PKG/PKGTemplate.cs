#pragma warning disable 0169, 0414
using TemplateLibrary;

[CategoryNamespaces]
enum ServiceTypes
{
    Loader, Discovery, Navgate, Proxy, Login, Lobby, Room, DB, DBCommit
}

class Service
{
    ServiceTypes type;
    ushort instanceId;
    string ip;
    int port;
}

namespace Discovery
{
    [Desc("服务启动后用该指令告知 Discovery")]
    class Register
    {
        [Desc("实例id")]
        ushort instanceId;

        [Desc("端口号")]
        int port;

        //ulong versionNumber;
    }

    [Desc("注册结果--成功")]
    class RegisterResult_Success
    {
        [Desc("现有服务实例列表")]
        List<Service> services;
    }

    [Desc("注册结果--失败: 重复的实例id")]
    class RegisterResult_DupeInstanceId
    {
    }

    // more errors
}

namespace Proxy
{
    [Desc("附在待转发的包前面, 表明包是 谁 发过来的")]
    class UserToken
    {
        ulong token;
    }
}



/* 

    基本结构( 核心 )

    Manage, Loader, Discovery, Navgate, Proxy, ...



    物理分布

    Manage 是服务器管理后台( 1 份 ), 借助 Loader, 可针对每台机器 进行 service 开关配置

    Loader 每台机器开机自启动运行一份, 连接到 Manage
    其他服务 受 Loader 控制, 根据 Manage 的配置, 于相应服务器动态开启

    Loader 同时也监视 具体 service 实例的运行健康状态

    Discovery 为发现服务, 接受各 service 注册并拉已注册 service 清单( 含实例type, id, ip, port 之类 )
    这样子 service 就可以在接下来 根据清单来查找自己感兴趣的 service, 并与之建立连接.
    连接过程是双向的. 不管谁依赖谁, 都是后启动的去找先启动的 service 建立连接.

    Discovery 服务需要先于其他 service 启动( Manage, Loader 除外 )



    网络连接对应关系

    Manage 与 Loader 一对多
    Discovery 与 services 一对多
    Navgate 与 Proxy 一对多
    Proxy 与 services 多对多



    通信流程与需求
 
    client 先从 navgate( 可能有多个, udp, 首包即含复杂校验信息 ) 得到要连接哪个 proxy 的信息, 然后去就去连指定 proxy
    
    client 发送相应指令到 proxy. proxy 负责转发到相应 service, 并转发处理结果( 应答 / 推送均可 )
    对于断线重连的情况, proxy 应该告知 client, 连接到原有 proxy 上, 并与 context 重连.
    proxy 上面理论上讲有一段 待发送 & 已发送的 数据包队列, 其生命周期手工配置, 用于断线重连补发
    针对补发的需求, 向 client 发送的包, 汇聚到一个队列, 有 "包编号", 以便于在队列中定位

    proxy 同时也应该有 上 & 下行 限速功能, 用以保护有限的总带宽
    对于可并行下发的流量( 例如 视频流 和 游戏指令 ), 理论上讲需要做分 channel 优先级限速( 切片组合, 先确保关键性数据 channel 的包占据流量大头 )

    proxy 同时也应该有 多级转发, 多点接入, 动态切换, 以分摊 / 转移流量, 做到防 ddos 的效果

    proxy context 中, 针对每类指令, 应建立 client 到 service 的双向映射, 以便于双向转发.
    当前以 生成器 扫描根命名空间 的方式来区分 service 类型, 公共结构都不加命名空间.
    生成器 需要生成与上述规则对应的 根命名空间枚举 以及 ServiceTypeId< 包 >.value = 根命名空间下标 设置代码
    考虑令某特殊的 Enum 名字用来枚举这些 根命名空间, 以便命名空间可以任意使用
    
    对于一种类型有多个 service 的情况, 存在一个 Redirect 的设计, 即首次转发都是发给同类型 service 的 "交换机"
    client context, 存在 "某类型 service 发送目标" 的映射. 如果首次发送, 该映射并未建立. 遂先向 "交换机" 发送导航请求
    "交换机" 根据某种理由( 负载均衡? 断线重连? 目标服务已死? ), 向 proxy 回复 redirect
    proxy 在收到 redirect 之后, 将该类型 service 在 context 中 建立映射, 并继续发包.
    这可能导致数据包滞留. 但也要等到映射建立之后才发送后续.

    client 发向 proxy 的包, 可以不带任何地址信息. proxy 通过查询 context 来定位到目标 service 的连接并转发
    转发时需要附上该 client 的唯一标识, 以便 service 发送数据过来的时候, 定位到 client 的连接并转发
    service 在收到包之后, 也需要对 "从哪个 proxy 收到的 client xxx 的数据" 做相应的映射
    这个映射可能会发生改变( 当 service 中的 context 与 proxy 中的 context 生命周期不一致时 ), 后续都是往当前 proxy 发送
    
    也存在一种 proxy 死掉的可能性( 代码不稳定? 虚拟主机死机 掉线啥的 ), 考虑采取先暂存一段时间, 待目标确认接收成功后 再移除


    
    开房流程

    玩家位于大厅时, 如果通过匹配, 定位到了目标战斗 service, 则此时需要将玩家 "引" 过去. 这里 proxy 并不针对此类 service 做 "映射",
    而是大厅告知 client 它需要连接的 service 的 address 信息, 在 client 发到 proxy 的包( 用于与目标服务器通信 )中, 需要携带该 address 信息.
    同时, 大厅也告知目标 service, 要连进来的 client 的 唯一标识. ( 这个过程有可能先于 大厅告知 client 发生, 此举可以顺便探测目标 service 的健康 )

    该做法相比同类型映射方案, 可以更灵活. 比如可能存在这样的需求: 大屏客户端 逻辑上"多开" , 同时打 "多桌" 游戏
    但相应的, 也有暴露 服务端 逻辑流程的风险, 恶意 client 可能直接伪造目标地址以实施某些因 开发过程中的疏忽造成的漏洞攻击
    故 此种做法, 只针对部分需要并行的同类型服务运用

*/






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
