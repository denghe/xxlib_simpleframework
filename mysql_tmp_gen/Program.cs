using System.Reflection;
using System.IO;
using System.Windows.Forms;
using System.Text;
using System;
using MySql.Data.MySqlClient;
using xx;

public static class Program
{
    // 扫目标库中的表结构, 生成模板用的声明代码段. 生成前需要手工改下面这些参数.

    public const string dbIP = "192.168.1.249";
    public const string dbPort = "3306";
    public const string dbUserId = "root";
    public const string dbPassword = "root";
    public const string dbDatabase = "game_dev2";

    public const string outputPath = "../../../../pkggen_template_PKG/DB_Tables.cs";

    static void Main(string[] args)
    {
        List<MYSQLGEN.DbTable> ts = null;
        CallMySqlFuncs(fs =>
        {
            ts = fs.GetTables<MYSQLGEN.DbTable>(dbDatabase);
            ts.ForEach(t =>
            {
                t.childs = fs.GetColumns<MYSQLGEN.DbColumn>(dbDatabase, t.TABLE_NAME);
                t.createScript = fs.GetCreateScript<MYSQLGEN.Tables.show_create_table>(dbDatabase, t.TABLE_NAME).CreateTable;
                t.childs.ForEach(c => c.parent = t);
            });
        });

        var sb = new StringBuilder();
        sb.Append(@"
#pragma warning disable 0169, 0414
using TemplateLibrary;

namespace Tables
{");
        ts.ForEach(t =>
        {
            sb.Append(@"
    /*
" + t.createScript + @"
    */
    class " + t.TABLE_NAME + @"
    {");
            t.childs.ForEach(c =>
            {
                sb.Append(@"
        [Column] " + c.DATA_TYPE._GetCsharpDataType() + @" " + c.COLUMN_NAME + @";");  // todo: type, readonly?
            });
            sb.Append(@"
    }");

        });
        sb.Append(@"
}
");

        sb._WriteToFile(outputPath);
    }












    /// <summary>
    /// 以 utf8 格式写文本到文件, 可选择是否附加 bom 头.
    /// </summary>
    public static void _Write(this string fn, StringBuilder sb, bool useBOM = true)
    {
        try
        {
            File.Delete(fn);
        }
        catch { }
        using (var fs = File.OpenWrite(fn))
        {
            if (useBOM)
                fs.Write(_bom, 0, _bom.Length);
            var buf = Encoding.UTF8.GetBytes(sb.ToString());
            fs.Write(buf, 0, buf.Length);
            fs.Close();
        }
    }

    public static byte[] _bom = { 0xEF, 0xBB, 0xBF };

    /// <summary>
    /// 以 utf8 格式写文本到文件, 可选择是否附加 bom 头.
    /// </summary>
    public static void _WriteToFile(this StringBuilder sb, string fn, bool useBOM = true)
    {
        fn._Write(sb, useBOM);
    }


    /// <summary>
    /// 得到 mysql 连接串
    /// </summary>
    public static string dbConnStr
    {
        get
        {
            return $"server={dbIP};user id={dbUserId};password={dbPassword};database={dbDatabase};port={dbPort};charset=utf8;sslmode=none";
        }
    }

    /// <summary>
    /// 快速执行 sql 的简易封装( 针对 MySqlFuncs ). 返回异常.
    /// </summary>
    public static void CallMySqlFuncs(Action<MYSQLGEN.MySqlFuncs> a)
    {
        using (var conn = new MySqlConnection(dbConnStr))
        {
            conn.Open();
            var fs = new MYSQLGEN.MySqlFuncs(conn);
            a(fs);
        }
    }

    public static string _GetCsharpDataType(this string DATA_TYPE)
    {
        switch (DATA_TYPE)
        {
            case "int": return "int";
            case "bigint": return "long";
            case "varchar": return "string";
            case "float": return "float";
            case "double": return "double";
            case "text": return "string";
            default: throw new Exception("unhandled.");
        }
    }
}
