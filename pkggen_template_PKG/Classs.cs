#pragma warning disable 0169, 0414
using TemplateLibrary;

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


[Desc("成功")]
class Success : Response
{
}

[Desc("失败")]
class Fail : Response
{
    string reason;
}



// 下面是一些动态属性相关结构

class Property
{
    string name;
}
class Property_long : Property
{
    long value;
}
class Property_double : Property
{
    double value;
}
class Property_string : Property
{
    string value;
}
class Properties : Property
{
    List<Property> value;
}


// todo: 改名为 PlayerInfo
class UserInfo
{
    long id;
    List<Property> props;
}

