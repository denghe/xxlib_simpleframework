#pragma warning disable 0169, 0414
using TemplateLibrary;

[MySql]
partial interface MySqlFuncs
{
    [Sql(@"select {n}")]
    int? TestSql(int? n);
}
