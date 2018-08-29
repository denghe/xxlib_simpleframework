#pragma warning disable 0169, 0414
using TemplateLibrary;

[MySql]
partial interface MySqlFuncs
{
    [Desc("模拟从表查数据填充到 Foo. 顺序和数量一致应该与 Foo 中标有 Column 的类成员一致")]
    [Sql(@"select {0}, {1}, {2}")]
    Foo SelectFoo(int id, int? age, string info);
}
