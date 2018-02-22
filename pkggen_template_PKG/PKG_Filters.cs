#pragma warning disable 0169, 0414
using TemplateLibrary;

[LuaFilter(nameof(Server_Client))]
[LuaFilter(nameof(Client_Server))]
interface ILuaFilter
{
}
