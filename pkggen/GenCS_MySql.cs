using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.IO;
using System.Linq;
using System.Reflection;
using System.Text;
using TemplateLibrary;

public static class GenCS_MySql
{
    public static void Gen(Assembly asm, string outDir, string templateName)
    {
        var sb = new StringBuilder();
        var ts = asm._GetTypes();

        // usings
        sb.Append(@"using System;
using System.Collections.Generic;
using System.Text;
using MySql.Data.MySqlClient;
using System.Diagnostics;
");

        // template namespace
        sb.Append(@"
namespace " + templateName + @"
{
    public static class MySqlAppendExt
    {
        public static void MySqlAppend<T>(this StringBuilder sb, T v)
        {
            sb.Append(v);
        }

        public static void MySqlAppend<T>(this StringBuilder sb, T? o) where T : struct
        {
            if (o.HasValue)
            {
                sb.MySqlAppend(o.Value);
            }
            else
            {
                sb.Append(""null"");
            }
        }

        public static void MySqlAppend(this StringBuilder sb, string v)
        {
            sb.Append(""'"" + v.Replace(""'"", ""''"") + ""'"");
        }

        public static void MySqlAppend(this StringBuilder sb, DateTime o)
        {
            sb.Append(""'"" + o.ToString(""yyyy-MM-dd HH:mm:ss"") + ""'"");
        }

        public static void MySqlAppend<T>(this StringBuilder sb, List<T> os) { Debug.Assert(os.Count > 0); for (int i = 0; i < os.Count; ++i) { sb.MySqlAppend(os[i]); sb.Append("", ""); }; if (os.Count > 0) sb.Length -= 2; }
        public static void MySqlAppend(this StringBuilder sb, List<DateTime> os) { Debug.Assert(os.Count > 0); sb.Append(""( ""); for (int i = 0; i < os.Count; ++i) { sb.MySqlAppend(os[i]); sb.Append("", ""); }; sb.Length -= 2; sb.Append("" )""); }
        public static void MySqlAppend(this StringBuilder sb, List<byte> os) { Debug.Assert(os.Count > 0); sb.Append(""( ""); for (int i = 0; i < os.Count; ++i) { sb.Append(os[i]); sb.Append("", ""); }; sb.Length -= 2; sb.Append("" )""); }
        public static void MySqlAppend(this StringBuilder sb, List<ushort> os) { Debug.Assert(os.Count > 0); sb.Append(""( ""); for (int i = 0; i < os.Count; ++i) { sb.Append(os[i]); sb.Append("", ""); }; sb.Length -= 2; sb.Append("" )""); }
        public static void MySqlAppend(this StringBuilder sb, List<uint> os) { Debug.Assert(os.Count > 0); sb.Append(""( ""); for (int i = 0; i < os.Count; ++i) { sb.Append(os[i]); sb.Append("", ""); }; sb.Length -= 2; sb.Append("" )""); }
        public static void MySqlAppend(this StringBuilder sb, List<ulong> os) { Debug.Assert(os.Count > 0); sb.Append(""( ""); for (int i = 0; i < os.Count; ++i) { sb.Append(os[i]); sb.Append("", ""); }; sb.Length -= 2; sb.Append("" )""); }
        public static void MySqlAppend(this StringBuilder sb, List<sbyte> os) { Debug.Assert(os.Count > 0); sb.Append(""( ""); for (int i = 0; i < os.Count; ++i) { sb.Append(os[i]); sb.Append("", ""); }; sb.Length -= 2; sb.Append("" )""); }
        public static void MySqlAppend(this StringBuilder sb, List<short> os) { Debug.Assert(os.Count > 0); sb.Append(""( ""); for (int i = 0; i < os.Count; ++i) { sb.Append(os[i]); sb.Append("", ""); }; sb.Length -= 2; sb.Append("" )""); }
        public static void MySqlAppend(this StringBuilder sb, List<int> os) { Debug.Assert(os.Count > 0); sb.Append(""( ""); for (int i = 0; i < os.Count; ++i) { sb.Append(os[i]); sb.Append("", ""); }; sb.Length -= 2; sb.Append("" )""); }
        public static void MySqlAppend(this StringBuilder sb, List<long> os) { Debug.Assert(os.Count > 0); sb.Append(""( ""); for (int i = 0; i < os.Count; ++i) { sb.Append(os[i]); sb.Append("", ""); }; sb.Length -= 2; sb.Append("" )""); }
        public static void MySqlAppend(this StringBuilder sb, List<double> os) { Debug.Assert(os.Count > 0); sb.Append(""( ""); for (int i = 0; i < os.Count; ++i) { sb.Append(os[i]); sb.Append("", ""); }; sb.Length -= 2; sb.Append("" )""); }
        public static void MySqlAppend(this StringBuilder sb, List<float> os) { Debug.Assert(os.Count > 0); sb.Append(""( ""); for (int i = 0; i < os.Count; ++i) { sb.Append(os[i]); sb.Append("", ""); }; sb.Length -= 2; sb.Append("" )""); }
        public static void MySqlAppend(this StringBuilder sb, List<bool> os) { Debug.Assert(os.Count > 0); sb.Append(""( ""); for (int i = 0; i < os.Count; ++i) { sb.Append(os[i]); sb.Append("", ""); }; sb.Length -= 2; sb.Append("" )""); }
");

        foreach (var e in ts._GetEnums())
        {
            var etn = e._GetTypeDecl_Csharp();
            var eutn = e._GetEnumUnderlyingTypeName_Csharp();
            sb.Append(@"
        public static void MySqlAppend(this StringBuilder sb, List<" + etn + @"> os) { Debug.Assert(os.Count > 0); sb.Append(""( ""); for (int i = 0; i < os.Count; ++i) { sb.Append("", ""); sb.Append((" + eutn + @")os[i]); }; sb.Length -= 2; sb.Append("" )""); }");
        }

        sb.Append(@"

        // todo: Nullable<Enum>, BBuffer
");

        foreach (var c in ts._GetClasss())
        {
            var ctn = c._GetTypeDecl_Csharp();

            var fs = c._GetFields<Column>();



            sb.Append(@"
        public static void MySqlAppend(this StringBuilder sb, List<" + ctn + @"> os, bool ignoreReadOnly = false) { Debug.Assert(os.Count > 0); for (int i = 0; i < os.Count; ++i) { sb.MySqlAppend(os[i], ignoreReadOnly); sb.Append("", ""); }; if (os.Count > 0) sb.Length -= 2; }
        public static void MySqlAppend(this StringBuilder sb, " + ctn + @" o, bool ignoreReadOnly = false)
        {
            sb.Append(""("");");



            foreach (var m in fs)
            {

                if (m._IsReadOnly())
                {
                    sb.Append(@"
            if (!ignoreReadOnly)
            {");
                }

                var mt = m.FieldType;
                var mtn = mt._GetTypeDecl_Csharp();

                string fnfix = "", typecnv = "";
                if (mt.IsEnum)
                {
                    typecnv = "(" + mt._GetEnumUnderlyingTypeName_Csharp() + ")";
                }
                else if (!mt._IsNumeric())
                {
                    fnfix = "MySql";
                }
                sb.Append(@"
            sb." + fnfix + "Append(" + typecnv + "o." + m.Name + ");");

                sb.Append(@"
            sb.Append("", "");");
                if (m._IsReadOnly())
                {
                    sb.Append(@"
            }");
                }
            }


            sb.Append(@"
            sb.Length -= 2;
            sb.Append("")"");
        }");
        }
        sb.Append(@"
    }
");


        // todo: [MySql]

        // 扫带有 [MySql] 标志的 interface , 生成相应的函数
        var ifaces = ts._GetInterfaces<MySql>();
        foreach (var iface in ifaces)
        {
            // namespace xxx {
            if (iface.Namespace != null)
            {
                sb.Append(@"
namespace " + iface.Namespace + @"
{");
            }

            // class {
            sb.Append(iface._GetDesc()._GetComment_CSharp(4) + @"
    public class " + iface.Name + @"
    {
        public MySqlConnection conn;
        public MySqlCommand cmd;
        public StringBuilder sb = new StringBuilder();
        private List<int> recordsAffecteds = new List<int>();

        public List<int> RecordsAffecteds
        {
            get { return recordsAffecteds; }
        }

        public int RecordsAffected
        {
            get { return recordsAffecteds[0]; }
        }

        public " + iface.Name + @"(MySqlConnection conn)
        {
            // conn.Open();
            this.conn = conn;
            this.cmd = conn.CreateCommand();
        }
");

            // todo: 如果参数都是简单数据类型( 不含 List / Sql拼接段 啥的 ), 生成 SqlCommand + Parameter 复用

            // members = default vals;
            var fs = iface._GetMethods();
            foreach (var f in fs)
            {
                if (!f._Has<Sql>())
                    throw new Exception("no [Sql] attribute on " + iface.Name + "." + f.Name);

                var ps = f.GetParameters();
                var rt = f.ReturnType;
                var rtn = rt._GetTypeDecl_Csharp();

                sb.Append(@"
" + f._GetDesc()._GetComment_CSharp(8) + @"
        " + "public " + rtn + " " + f.Name + @"");

                if (ps.Length > 0)
                {
                    sb.Append(@"
        ");
                }

                sb.Append(@"(");
                foreach (var p in ps)
                {
                    if (p != ps[0])
                    {
                        sb.Append(",");
                    }

                    sb.Append(p._GetDesc()._GetComment_CSharp(12) + @"
            " + p.ParameterType._GetTypeDecl_Csharp() + " " + p.Name);

                    if (p.HasDefaultValue)
                    {
                        sb.Append(" = " + p._GetDefaultValueDecl_Csharp());
                    }
                }

                if (ps.Length > 0)
                {
                    sb.Append(@"
            , ");
                }
                sb.Append(@"MySqlTransaction tran_ = null)
        {
            cmd.Transaction = tran_;");

                if (ps.Length > 0)
                {
                    sb.Append(@"
            recordsAffecteds.Clear();
            sb.Clear();");

                    // 扫描出 f.Sql 的参数部分并与 f.Parameters 相对应
                    var sqls = f._GetSql()._SpliteSql();
                    foreach (var o in sqls)
                    {
                        if (o is string)
                        {
                            sb.Append(@"
            sb.Append(@""" + ((string)o).Replace("\"", "\"\"") + @""");");
                        }
                        else
                        {
                            var p = ps[(int)o];
                            var pn = p.Name;
                            var pt = p.ParameterType;
                            if (p._Has<Literal>() || pt._IsNumeric())
                            {
                                sb.Append(@"
            sb.Append(" + pn + @");");
                            }
                            else
                            {
                                if (pt._IsUserClass())
                                {
                                    sb.Append(@"
            sb.MySqlAppend(" + pn + @", " + (p._Has<SkipReadOnly>() ? "true" : "false") + ");");
                                }
                                else
                                {
                                    sb.Append(@"
            sb.MySqlAppend(" + pn + @");");
                                }

                            }
                        }
                    }

                    sb.Append(@"
            cmd.CommandText = sb.ToString();
");
                }
                else
                {
                    sb.Append(@"
            cmd.CommandText = @""" + f._GetSql().Replace("\"", "\"\"") + @""";");
                }

                // 根据函数返回值类型，生成不同的代码段。

                // 多结果集
                if (rt._IsTuple())
                {
                    // 为了尽量不改下面的代码, 造个 rt 出来
                    var rrt = rt;
                    var rrtn = rtn;

                    sb.Append(@"
            using (var r = cmd.ExecuteReader())
            {
                recordsAffecteds.Add(r.RecordsAffected);");

                    for (int jj = 0; jj < rrt.GenericTypeArguments.Length; ++jj)
                    {







                        int ii = jj + 1;
                        rt = rrt.GenericTypeArguments[jj];
                        rtn = rt._GetTypeDecl_Csharp();
                        if (jj > 0)
                        {
                            sb.Append(@"
                r.NextResult();");
                        }
                        sb.Append(@"
                recordsAffecteds.Add(r.RecordsAffected);");
                        // 单结果集
                        if (rt.IsGenericType && rt.Name == "List`1")
                        {
                            var ct = rt.GenericTypeArguments[0];
                            var ctn = ct._GetTypeDecl_Csharp();

                            if (ct.IsValueType)
                            {
                                sb.Append(@"
                var rtv" + ii + @" = new List<" + ctn + @">();
                while (r.Read())
                {
                    rtv" + ii + @".Add(" + (ct._IsSqlNullable() ? ("r.IsDBNull(0) ? null : (" + ctn + ")") : "") + ct._GetDataReaderFuncName() + @"(0));
                }");
                            }
                            else
                            {
                                sb.Append(@"
                var rtv" + ii + @" = new " + rtn + @"();
                while (r.Read())
                {
                    rtv" + ii + @".Add(new " + ctn + @"
                    {");
                                var ctfs = ct._GetFields<Column>();
                                if (ctfs.Count() == 0)
                                    throw new Exception("the class's fields can't be empty");

                                for (int i = 0; i < ctfs.Count; ++i)
                                {
                                    var m = ctfs[i];
                                    var mt = m.FieldType;
                                    var mtn = mt._GetTypeDecl_Csharp();
                                    var mn = m.Name;


                                    var getfn = mt._GetDataReaderFuncName();

                                    if (i > 0)
                                    {
                                        sb.Append(",");
                                    }
                                    sb.Append(@"
                        " + mn + @" = " + (mt._IsSqlNullable() ? ("r.IsDBNull(" + i + ") ? null : (" + mtn + ")") : "") + getfn + @"(" + i + @")");
                                }
                                sb.Append(@"
                    });
                }");
                            }
                        }
                        // 单行 / 单个结果
                        else
                        {
                            if (rt._IsUserClass())
                            {
                                var rtfs = rt._GetFields<Column>();
                                if (rtfs.Count() == 0)
                                    throw new Exception("the class's fields can't be empty");


                                sb.Append(@"
                " + rtn + @" rtv" + ii + @" = null;
                if (r.Read())
                {
                    rtv" + ii + @" = new " + rtn + @"
                    {");
                                for (int i = 0; i < rtfs.Count; ++i)
                                {
                                    var m = rtfs[i];
                                    var mt = m.FieldType;
                                    var mtn = mt._GetTypeDecl_Csharp();
                                    var mn = m.Name;

                                    var getfn = mt._GetDataReaderFuncName();

                                    if (i > 0)
                                    {
                                        sb.Append(", ");
                                    }
                                    sb.Append(@"
                    " + mn + @" = " + (mt._IsSqlNullable() ? ("r.IsDBNull(" + i + ") ? null : (" + mtn + ")") : "") + getfn + @"(" + i + @")");
                                }
                                sb.Append(@"
                    };
                }");
                            }
                            else
                            {
                                if (rt._IsVoid())
                                {
                                    Debug.Assert(false);
                                }
                                else
                                {
                                    sb.Append(@"
                var rtv" + ii + @" = default(" + rtn + @");
                if (r.Read())
                {
                    rtv" + ii + @" = " + (rt._IsSqlNullable() ? ("r.IsDBNull(0) ? null : (" + rtn + ")") : "") + rt._GetDataReaderFuncName() + @"(0);
                }");

                                }
                            }
                        }









                    }
                    sb.Append(@"
                return new " + rrtn + @"(");
                    for (int jj = 0; jj < rrt.GenericTypeArguments.Length; ++jj)
                    {
                        if (jj > 0) sb.Append(", ");
                        sb.Append("rtv" + (jj + 1));
                    }
                    sb.Append(@");
            }");
                }
                // 单结果集
                else if (rt.IsGenericType && rt.Name == "List`1")
                {
                    var ct = rt.GenericTypeArguments[0];
                    var ctn = ct._GetTypeDecl_Csharp();

                    if (ct.IsValueType)
                    {
                        sb.Append(@"
            var rtv = new List<" + ctn + @">();
            using (var r = cmd.ExecuteReader())
            {
                recordsAffecteds.Add(r.RecordsAffected);
                while (r.Read())
                {
                    rtv.Add(" + (ct._IsSqlNullable() ? ("r.IsDBNull(0) ? null : (" + ctn + ")") : "") + ct._GetDataReaderFuncName() + @"(0));
                }
            }
            return rtv;");
                    }
                    else
                    {
                        sb.Append(@"
            var rtv = new " + rtn + @"();
            using (var r = cmd.ExecuteReader())
            {
                recordsAffecteds.Add(r.RecordsAffected);
                while (r.Read())
                {
                    rtv.Add(new " + ctn + @"
                    {");
                        var ctfs = ct._GetFields<Column>();
                        if (ctfs.Count() == 0)
                            throw new Exception("the class's fields can't be empty");

                        for (int i = 0; i < ctfs.Count; ++i)
                        {
                            var m = ctfs[i];
                            var mt = m.FieldType;
                            var mtn = mt._GetTypeDecl_Csharp();
                            var mn = m.Name;

                            var getfn = mt._GetDataReaderFuncName();

                            if (i > 0)
                            {
                                sb.Append(",");
                            }
                            sb.Append(@"
                        " + mn + @" = " + (mt._IsSqlNullable() ? ("r.IsDBNull(" + i + ") ? null : (" + mtn + ")") : "") + getfn + @"(" + i + @")");
                        }
                        sb.Append(@"
                    });
                }
            }
            return rtv;");
                    }
                }
                // 单行 / 单个结果
                else
                {
                    if (rt._IsUserClass())
                    {
                        var rtfs = rt._GetFields<Column>();
                        if (rtfs.Count() == 0)
                            throw new Exception("the class's fields can't be empty");


                        sb.Append(@"
            using (var r = cmd.ExecuteReader())
            {
                recordsAffecteds.Add(r.RecordsAffected);
                if (!r.Read())
                {
                    return null;
                }
                return new " + rtn + @"
                {");
                        for (int i = 0; i < rtfs.Count; ++i)
                        {
                            var m = rtfs[i];
                            var mt = m.FieldType;
                            var mtn = mt._GetTypeDecl_Csharp();
                            var mn = m.Name;

                            var getfn = mt._GetDataReaderFuncName();

                            if (i > 0)
                            {
                                sb.Append(", ");
                            }
                            sb.Append(@"
                    " + mn + @" = " + (mt._IsSqlNullable() ? ("r.IsDBNull(" + i + ") ? null : (" + mtn + ")") : "") + getfn + @"(" + i + @")");
                        }
                        sb.Append(@"
                };
            }");
                    }
                    else
                    {
                        if (rt._IsVoid())
                        {
                            // 无返回值
                            sb.Append(@"
            recordsAffecteds.Add(cmd.ExecuteNonQuery());");
                        }
                        else
                        {
                            sb.Append(@"
            using (var r = cmd.ExecuteReader())
            {
                recordsAffecteds.Add(r.RecordsAffected);
                if (!r.Read())
                {
                    return default(" + rtn + @");
                }
                return " + (rt._IsSqlNullable() ? ("r.IsDBNull(0) ? null : (" + rtn + ")") : "") + rt._GetDataReaderFuncName() + @"(0);
            }");

                        }
                    }
                }

                // func }
                sb.Append(@"
        }");
            }


            // class }
            sb.Append(@"
    }");

            // namespace xxx }
            if (iface.Namespace != null)
            {
                sb.Append(@"
}");
            }
        }

        // template namespace /
        sb.Append(@"
}
");

        sb._WriteToFile(Path.Combine(outDir, templateName + "_mysql.cs"));
    }
}
