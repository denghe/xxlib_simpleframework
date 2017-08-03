using System.Reflection;
using System.IO;
using System.Windows.Forms;

// todo: 针对 "受影响行数" 之类的查询执行后附加结果, 考虑存放在 函数接口对应的类中

public static class Program
{
    public const string templatePrefix = "dbgen_template_";
    public const string outputPath = "../../../../db";

    static void Main(string[] args)
    {
        // 扫所有添加过引用的, 名字前缀为 templatePrefix 的 dll, 执行相应的生成
        foreach (var fn in Directory.GetFiles(Application.StartupPath, templatePrefix + "*.dll"))
        {
            var asm = Assembly.LoadFile(fn);
            var shortfn = new FileInfo(fn).Name;
            shortfn = shortfn.Substring(0, shortfn.LastIndexOf('.'));
            var path = Path.Combine(Application.StartupPath, outputPath);
            var tn = shortfn.Substring(templatePrefix.Length);
            GenCS_Class.Gen(asm, path, tn);
            GenCS_MsSql.Gen(asm, path, tn);
            GenCS_MultiKeyDict.Gen(asm, path, tn);
            GenCPP_Class.Gen(asm, path, tn);
            GenCPP_SQLite.Gen(asm, path, tn);
        }
    }

}
