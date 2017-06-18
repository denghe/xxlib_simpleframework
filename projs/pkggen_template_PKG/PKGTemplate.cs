#pragma warning disable 0169, 0414
using TemplateLibrary;

[CategoryNamespaces]
enum ServiceTypes
{
    Loader, Discovery, Navgate, Proxy, Login, Lobby, Room, DB, DBCommit
}

namespace Global
{
    [Desc("服务信息描述")]
    class Service
    {
        ServiceTypes type;
        ushort instanceId;
        string ip;
        int port;
    }

    [Desc("暂定用于包之附加描述")]
    class UserToken
    {
        ulong token;
    }
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
        List<Global.Service> services;
    }

    [Desc("注册结果--失败: 重复的实例id")]
    class RegisterResult_DupeInstanceId
    {
    }

    // more errors
}

namespace Proxy
{
    // todo
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
