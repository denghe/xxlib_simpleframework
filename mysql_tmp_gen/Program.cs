using System.IO;
using System.Text;
using System;
using MySql.Data.MySqlClient;
using xx;

public static class Program
{
    // 扫目标库中的表结构, 生成模板用的声明代码段. 生成前需要手工改下面这些参数.
    // 生成物仅供参考, 需要自行复制粘贴部分或全部到具体模板中.

    public const string dbIP = "192.168.1.254";
    public const string dbPort = "3306";
    public const string dbUserId = "root";
    public const string dbPassword = "1";
    public const string dbDatabase = "test";

    public const string outputPath_tables = "../../../../db/DBTables.cs";
    public const string outputPath_funcs = "../../../../db/DBFuncs.cs";

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

        GenDbTables(ts);
        GenDbFuncs(ts);
    }

    public static void GenDbTables(List<MYSQLGEN.DbTable> ts)
    {
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
        sb._WriteToFile(outputPath_tables);
    }

    public static void GenDbFuncs(List<MYSQLGEN.DbTable> ts)
    {
        var sb = new StringBuilder();
        var sb2 = new StringBuilder();
        var sb3 = new StringBuilder();
        var sb4 = new StringBuilder();
        sb.Append(@"#pragma warning disable 0169, 0414
using TemplateLibrary;

[MySql]
partial interface MySqlFuncs
{");
        ts.ForEach(t =>
        {
            // 在 sb2 中拼接 `col1`, `col2`, `col3`, ...
            sb2.Clear();
            {
                t.childs.ForEach(c =>
                {
                    sb2.Append(@"`" + c.COLUMN_NAME + @"`, ");
                });
                if (sb2.Length > 0) sb2.Length -= 2;
            }

            sb.Append(@"
    [Desc(@""查 " + t.TABLE_NAME + @" 表所有数据"")]
    [Sql(@""
select " + sb2 + @"
  from `" + t.TABLE_NAME + @"`"")]
    List<Tables." + t.TABLE_NAME + @"> " + t.TABLE_NAME + @"__SelectAll();
");

            // 在 sb2 中拼接 `col1`, `col2`, `col3`, ... 跳过只读字段
            sb2.Clear();
            {
                t.childs.ForEach(c =>
                {
                    if (c._IsReadonly()) return;
                    sb2.Append(@"`" + c.COLUMN_NAME + @"`, ");
                });
                if (sb2.Length > 0) sb2.Length -= 2;
            }

            // 在 sb3 中拼接 {0}, {1}, {2}, ... 不算只读字段
            sb3.Clear();
            {
                var n = t._GetWritableColumnCount();
                for (int i = 0; i < n; ++i)
                {
                    sb3.Append(@"{" + i + @"}, ");
                }
                if (sb3.Length > 0) sb3.Length -= 2;
            }

            // 在 sb4 中拼接 函数参数部分, 不算只读字段
            sb4.Clear();
            {
                t.childs.ForEach(c =>
                {
                    if (c._IsReadonly()) return;
                    sb4.Append(c._GetCsharpDataType() + " " + c.COLUMN_NAME + ", ");
                });
                if (sb4.Length > 0) sb4.Length -= 2;
            }

            sb.Append(@"
    [Desc(@""往 " + t.TABLE_NAME + @" 表插入单条数据"")]
    [Sql(@""
insert into `" + t.TABLE_NAME + @"` (" + sb2 + @")
values (" + sb3 + @")"")]
    void " + t.TABLE_NAME + @"__Insert(" + sb4 + @");
");

            sb.Append(@"
    [Desc(@""往 " + t.TABLE_NAME + @" 表插入单条数据对象"")]
    [Sql(@""
insert into `" + t.TABLE_NAME + @"` (" + sb2 + @")
values {0}"")]
    void " + t.TABLE_NAME + @"__Insert([SkipReadOnly] Tables." + t.TABLE_NAME + @" row);
");

            sb.Append(@"
    [Desc(@""往 " + t.TABLE_NAME + @" 表插入多条数据对象"")]
    [Sql(@""
insert into `" + t.TABLE_NAME + @"` (" + sb2 + @")
values {0}"")]
    void " + t.TABLE_NAME + @"__Insert([SkipReadOnly] List<Tables." + t.TABLE_NAME + @"> rows);
");

            var pks = t._GetPrimaryKeys();
            if (pks.dataLen > 0)
            {
                // 在 sb3 中拼接 where 部分的条件
                sb3.Clear();
                {
                    int i = 0;
                    pks.ForEach(c =>
                    {
                        sb3.Append(@"`" + c.COLUMN_NAME + $"` = {i++} and ");
                    });
                    if (sb3.Length > 0) sb3.Length -= 5;
                }

                // 在 sb4 中拼接 函数参数部分
                sb4.Clear();
                {
                    pks.ForEach(c =>
                    {
                        sb4.Append(c._GetCsharpDataType() + " " + c.COLUMN_NAME + ", ");
                    });
                    if (sb4.Length > 0) sb4.Length -= 2;
                }

                sb.Append(@"
    [Desc(@""根据主键查 " + t.TABLE_NAME + @" 表指定数据行"")]
    [Sql(@""
select " + sb2 + @"
  from `" + t.TABLE_NAME + @"`
 where " + sb3 + @""")]
    Tables." + t.TABLE_NAME + @" " + t.TABLE_NAME + @"__SelectByPrimaryKey(" + sb4 + @");
");
            }

            // todo: 为外键子表生成查询?

        });
        sb.Append(@"
}
");
        sb._WriteToFile(outputPath_funcs);
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

    public static bool _IsPrimaryKey(this MYSQLGEN.DbColumn c)
    {
        return c.COLUMN_KEY == "PRI";
    }
    public static string _IfPrimaryKey(this MYSQLGEN.DbColumn c, string sTrue, string sFalse = "")
    {
        return c._IsPrimaryKey() ? sTrue : sFalse;
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

    public static List<MYSQLGEN.DbColumn> _GetPrimaryKeys(this MYSQLGEN.DbTable t)
    {
        var rtv = new List<MYSQLGEN.DbColumn>();
        t.childs.ForEach(c =>
        {
            if (c._IsPrimaryKey())
            {
                rtv.Add(c);
            }
        });
        return rtv;
    }

    public static bool _HasPrimaryKey(this MYSQLGEN.DbTable t)
    {
        return t._GetPrimaryKeys().dataLen > 0;
    }
    public static string _IfHasPrimaryKey(this MYSQLGEN.DbTable t, string sTrue, string sFalse = "")
    {
        return t._HasPrimaryKey() ? sTrue : sFalse;
    }

    public static int _GetWritableColumnCount(this MYSQLGEN.DbTable t)
    {
        int rtv = 0;
        t.childs.ForEach(c =>
        {
            if (!c._IsReadonly()) ++rtv;
        });
        return rtv;
    }

    // todo: GetName 系列, 以防止名称与 c# 关键字相冲, 以及去掉特殊字符
}
