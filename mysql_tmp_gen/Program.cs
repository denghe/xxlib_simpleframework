using System.IO;
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

    public const string outputPath = "../../../../pkggen_template_PKG/DBTables.cs";

    static void Main(string[] args)
    {
        // 查询并填充表结构体
        List<MYSQLGEN.DbTable> ts = null;
        CallMySqlFuncs(fs =>
        {
            ts = fs.GetTables<MYSQLGEN.DbTable>(dbDatabase);
            ts.ForEach(t =>
            {
                // 查询并填充表结构体中的字段
                t.childs = fs.GetColumns<MYSQLGEN.DbColumn>(dbDatabase, t.TABLE_NAME);
                t.childs.ForEach(c => c.parent = t);

                // 查询并填充表结构体中的创建脚本
                t.createScript = fs.GetCreateScript<MYSQLGEN.Tables.show_create_table>(dbDatabase, t.TABLE_NAME).CreateTable;
            });
        });


        var sb = new StringBuilder();
        sb.Append(@"#pragma warning disable 0169, 0414
using TemplateLibrary;

namespace Tables
{");
        ts.ForEach(t =>
        {
            sb.Append(@"
    /*
" + t._GetCreateScript() + @"
    */
    [Desc(@""" + t._GetDesc() + @""")]
    class " + t.TABLE_NAME + @"
    {");
            t.childs.ForEach(c =>
            {
                //c.COLUMN_DEFAULT
                sb.Append(@"
        [Desc(@""" + c._GetDesc() + @""")]
        [Column" + c._IfReadonly("(true)") + @"] " + c._GetCsharpDataType() + @" " + c.COLUMN_NAME + @";");
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



    public static bool _IsUnsigned(this MYSQLGEN.DbColumn c)
    {
        return c.COLUMN_TYPE.EndsWith("unsigned");
    }
    public static string _IfUnsigned(this MYSQLGEN.DbColumn c, string sTrue, string sFalse = "")
    {
        return c._IsUnsigned() ? sTrue : sFalse;
    }


    public static bool _IsNullable(this MYSQLGEN.DbColumn c)
    {
        return c.IS_NULLABLE == "YES";
    }
    public static string _IfNullable(this MYSQLGEN.DbColumn c, string sTrue, string sFalse = "")
    {
        return c._IsNullable() ? sTrue : sFalse;
    }


    public static bool _IsAutoIncrement(this MYSQLGEN.DbColumn c)
    {
        return c.EXTRA == "auto_increment";
    }
    public static string _IfAutoIncrement(this MYSQLGEN.DbColumn c, string sTrue, string sFalse = "")
    {
        return c._IsAutoIncrement() ? sTrue : sFalse;
    }


    public static bool _IsReadonly(this MYSQLGEN.DbColumn c)
    {
        return c._IsAutoIncrement();    // todo: more check
    }
    public static string _IfReadonly(this MYSQLGEN.DbColumn c, string sTrue, string sFalse = "")
    {
        return c._IsReadonly() ? sTrue : sFalse;
    }


    public static string _GetCsharpDataType(this MYSQLGEN.DbColumn c)
    {
        switch (c.DATA_TYPE)
        {
            case "int": return c._IfUnsigned("u") + "int" + c._IfNullable("?");
            case "bigint": return c._IfUnsigned("u") + "long" + c._IfNullable("?");
            case "varchar": return "string";
            case "float": return "float" + c._IfNullable("?");
            case "double": return "double" + c._IfNullable("?");
            case "text": return "string";
            // todo: more
            default: throw new Exception("unhandled.");
        }
    }
    public static string _GetDesc(this MYSQLGEN.DbColumn c)
    {
        // todo: 处理内含的 /* */
        return c.COLUMN_COMMENT.Replace("\r\n", "\n").Replace("\n", "\r\n").Replace("\"", "\"\"");
    }

    public static string _GetDesc(this MYSQLGEN.DbTable t)
    {
        // todo: 处理内含的 /* */
        return t.TABLE_COMMENT.Replace("\r\n", "\n").Replace("\n", "\r\n").Replace("\"", "\"\"");
    }

    public static string _GetCreateScript(this MYSQLGEN.DbTable t)
    {
        // todo: 处理内含的 /* */
        return t.createScript.Replace("\r\n", "\n").Replace("\n", "\r\n");
    }

    // todo: GetName 系列, 以防止名称与 c# 关键字相冲, 以及去掉特殊字符
}
