#pragma warning disable 0169, 0414
using TemplateLibrary;

// WEB 向 test_cpp3 发的包
namespace WEB_testcpp3
{
    [Desc("指令基类. 路由用户上下文. 校验身份权限.")]
    class AuthInfo
    {
        int id;
    }

    class Cmd1 : AuthInfo
    {
    }

    class Cmd2 : AuthInfo
    {
    }
}
