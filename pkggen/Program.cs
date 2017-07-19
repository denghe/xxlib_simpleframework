using System.Reflection;
using System.IO;
using System.Windows.Forms;



public static class Program
{
    public const string templatePrefix = "pkggen_template_";
    public const string outputPath_cpp = "../../../../pkg_cpp";
    public const string outputPath_csharp = "../../../../pkg";

    static void Main(string[] args)
    {
        // 扫所有添加过引用的, 名字前缀为 templatePrefix 的 dll, 执行相应的生成
        foreach (var fn in Directory.GetFiles(Application.StartupPath, templatePrefix + "*.dll"))
        {
            var asm = Assembly.LoadFile(fn);
            var shortfn = new FileInfo(fn).Name;
            shortfn = shortfn.Substring(0, shortfn.LastIndexOf('.'));

            {
                var path = Path.Combine(Application.StartupPath, outputPath_cpp);
                var tn = shortfn.Substring(templatePrefix.Length);
                GenCPP_Class.Gen(asm, path, tn);
            }

            {
                var path = Path.Combine(Application.StartupPath, outputPath_csharp);
                var tn = shortfn.Substring(templatePrefix.Length);
                GenCS_Class.Gen(asm, path, tn);
            }
        }
    }

}
