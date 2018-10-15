#pragma warning disable 0169, 0414
using TemplateLibrary;

// 本接口用于限制 LUA 的生成物命名空间范围

[LuaFilter(nameof(WEB_testcpp3))]
[LuaFilter(nameof(Generic))]
interface ILuaFilter
{
}
