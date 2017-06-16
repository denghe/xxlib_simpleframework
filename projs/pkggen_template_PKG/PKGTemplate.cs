#pragma warning disable 0169, 0414
using TemplateLibrary;

[Namespaces]
enum ServiceTypes
{
    Loader, Agent, Navgate, Proxy, Login, Lobby, Room, DB, DBCommit
}

class Service
{
    ServiceTypes type;
    ushort instanceId;
    string ip;
    int port;
}

/* 
    启动流程

    Loader 最先启动. 接受管理后台数据, 启动相应 service, 并监视其执行状态
    之后 agent 启动.  主要作用是 当 service 启动时, 就到 agent 来注册, 同时得到 已启动 service 的清单( 含实例type, id, ip, port 之类 )
    service 启动并注册后, 根据清单来查找自己感兴趣的 service, 并与之建立连接.
    连接过程是双向的. 不管谁依赖谁, 都是后启动的去找先启动的 service 建立连接.


    通信流程
 
    client 先从 navgate 得到要连接哪个 proxy( 当前只做1个 )的信息, 然后去就去连指定 proxy
    
    client 发送相应指令到 proxy. proxy 负责转发到相应 service, 并转发处理结果( 应答 / 推送均可 )
    对于断线重连的情况, proxy 应该告知 client, 连接到原有 proxy 上, 并与 context 重连.

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

*/

//[Desc("投递地址( client -> proxy )")]
//class PostAddress
//{
//    ServiceTypes type;
//    ushort instanceId;
//}

//[Desc("返回地址( proxy -> service )")]
//class BackAddress
//{
//    ServiceTypes type;
//    ushort instanceId;
//}

namespace Agent
{
    namespace CS
    {
        [Desc("服务启动后用该指令告知 agent")]
        class Register
        {
            [Desc("实例id")]
            ushort instanceId;

            [Desc("端口号")]
            int port;

            //ulong versionNumber;
        }
    }
    namespace SC
    {
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
}

namespace Proxy
{

}








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
