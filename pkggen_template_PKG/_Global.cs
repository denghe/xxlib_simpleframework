#pragma warning disable 0169, 0414
using TemplateLibrary;

// 各种公共类结构 / 枚举
namespace Global
{
    [CategoryNamespaces]
    enum ServiceTypes : byte
    {
        DB, Login, Lobby, Game1
    }

    [Desc("服务地址信息( 用于各服务器下发导航信息")]
    class IPAddress
    {
        [Limit(64), Desc("IPV4 或 V6 的地址")]
        string ip;

        [Desc("端口")]
        int port;
    }

    [Desc("一个请求( 当前限定为 service 与 db 间 ), 通常携带一个流水号. 这是基类")]
    class Request
    {
        int serial;
    }

    [Desc("一个回应( 当前限定为 service 与 db 间 ), 通常携带一个请求发过来的流水号. 这是基类")]
    class Response
    {
        int requestSerial;
    }

    [Desc("当前用于 service 连接到 db 时简单描述自己是谁, 连接成功后的首包就需要发送.")]
    class ServiceInfo
    {
        ServiceTypes serviceType;

        [Desc("实例id. 对同一种类型的多个服务实例有意义, 方便做映射")]
        int instanceId;
    }

    [Desc("用户信息")]
    class UserInfo
    {
        long coin;
    }

    [Desc("游戏小厅信息")]
    class RoomInfo
    {
        int id;
    }

    [Desc("游戏小厅信息派生: 扎金花")]
    class RoomInfo_扎金花 : RoomInfo
    {
        long 进场基础金币要求;
        long 底注多少金币;
        long 封顶加注额度;
        int 几轮后可直接比牌;
        int 最多加注几轮;
    }

    //class Property
    //{
    //    string name;
    //}
    //class Property_int : Property
    //{
    //    int value;
    //}
    //class Property_string : Property
    //{
    //    string value;
    //}
    //class Properties : Property
    //{
    //    List<Property> value;
    //}
    // ...

}
