using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.IO;
using System.Linq;
using System.Reflection;
using System.Text;
using TemplateLibrary;

public static class GenCPP_SQLite
{
    public static void Gen(Assembly asm, string outDir, string templateName)
    {
        var sb = new StringBuilder();
        var ts = asm._GetTypes();

        // usings
        sb.Append(@"using System;
using System.Collections.Generic;
using System.Text;
using System.Data.SqlClient;
using System.Diagnostics;
");

        // template namespace
        sb.Append(@"
namespace " + templateName + @"
{
    public static class MsSqlAppendExt
    {
        public static void MsSqlAppend<T>(this StringBuilder sb, T v)
        {
            sb.Append(v);
        }

        public static void MsSqlAppend<T>(this StringBuilder sb, T? o) where T : struct
        {
            if (o.HasValue)
            {
                sb.MsSqlAppend(o.Value);
            }
            else
            {
                sb.Append(""null"");
            }
        }

        public static void MsSqlAppend(this StringBuilder sb, string v)
        {
            sb.Append(""'"" + v.Replace(""'"", ""''"") + ""'"");
        }

        public static void MsSqlAppend(this StringBuilder sb, DateTime o)
        {
            sb.Append(""'"" + o.ToString(""yyyy-MM-dd HH:mm:ss"") + ""'"");
        }

        public static void MsSqlAppend<T>(this StringBuilder sb, List<T> os) { Debug.Assert(os.Count > 0); for (int i = 0; i < os.Count; ++i) { sb.MsSqlAppend(os[i]); sb.Append("", ""); }; if (os.Count > 0) sb.Length -= 2; }
        public static void MsSqlAppend(this StringBuilder sb, List<DateTime> os) { Debug.Assert(os.Count > 0); sb.Append(""( ""); for (int i = 0; i < os.Count; ++i) { sb.MsSqlAppend(os[i]); sb.Append("", ""); }; sb.Length -= 2; sb.Append("" )""); }
        public static void MsSqlAppend(this StringBuilder sb, List<byte> os) { Debug.Assert(os.Count > 0); sb.Append(""( ""); for (int i = 0; i < os.Count; ++i) { sb.Append(os[i]); sb.Append("", ""); }; sb.Length -= 2; sb.Append("" )""); }
        public static void MsSqlAppend(this StringBuilder sb, List<ushort> os) { Debug.Assert(os.Count > 0); sb.Append(""( ""); for (int i = 0; i < os.Count; ++i) { sb.Append(os[i]); sb.Append("", ""); }; sb.Length -= 2; sb.Append("" )""); }
        public static void MsSqlAppend(this StringBuilder sb, List<uint> os) { Debug.Assert(os.Count > 0); sb.Append(""( ""); for (int i = 0; i < os.Count; ++i) { sb.Append(os[i]); sb.Append("", ""); }; sb.Length -= 2; sb.Append("" )""); }
        public static void MsSqlAppend(this StringBuilder sb, List<ulong> os) { Debug.Assert(os.Count > 0); sb.Append(""( ""); for (int i = 0; i < os.Count; ++i) { sb.Append(os[i]); sb.Append("", ""); }; sb.Length -= 2; sb.Append("" )""); }
        public static void MsSqlAppend(this StringBuilder sb, List<sbyte> os) { Debug.Assert(os.Count > 0); sb.Append(""( ""); for (int i = 0; i < os.Count; ++i) { sb.Append(os[i]); sb.Append("", ""); }; sb.Length -= 2; sb.Append("" )""); }
        public static void MsSqlAppend(this StringBuilder sb, List<short> os) { Debug.Assert(os.Count > 0); sb.Append(""( ""); for (int i = 0; i < os.Count; ++i) { sb.Append(os[i]); sb.Append("", ""); }; sb.Length -= 2; sb.Append("" )""); }
        public static void MsSqlAppend(this StringBuilder sb, List<int> os) { Debug.Assert(os.Count > 0); sb.Append(""( ""); for (int i = 0; i < os.Count; ++i) { sb.Append(os[i]); sb.Append("", ""); }; sb.Length -= 2; sb.Append("" )""); }
        public static void MsSqlAppend(this StringBuilder sb, List<long> os) { Debug.Assert(os.Count > 0); sb.Append(""( ""); for (int i = 0; i < os.Count; ++i) { sb.Append(os[i]); sb.Append("", ""); }; sb.Length -= 2; sb.Append("" )""); }
        public static void MsSqlAppend(this StringBuilder sb, List<double> os) { Debug.Assert(os.Count > 0); sb.Append(""( ""); for (int i = 0; i < os.Count; ++i) { sb.Append(os[i]); sb.Append("", ""); }; sb.Length -= 2; sb.Append("" )""); }
        public static void MsSqlAppend(this StringBuilder sb, List<float> os) { Debug.Assert(os.Count > 0); sb.Append(""( ""); for (int i = 0; i < os.Count; ++i) { sb.Append(os[i]); sb.Append("", ""); }; sb.Length -= 2; sb.Append("" )""); }
        public static void MsSqlAppend(this StringBuilder sb, List<bool> os) { Debug.Assert(os.Count > 0); sb.Append(""( ""); for (int i = 0; i < os.Count; ++i) { sb.Append(os[i]); sb.Append("", ""); }; sb.Length -= 2; sb.Append("" )""); }
");

        foreach (var e in ts._GetEnums())
        {
            var etn = e._GetTypeDecl_Csharp();
            var eutn = e._GetEnumUnderlyingTypeName_Csharp();
            sb.Append(@"
        public static void MsSqlAppend(this StringBuilder sb, List<" + etn + @"> os) { Debug.Assert(os.Count > 0); sb.Append(""( ""); for (int i = 0; i < os.Count; ++i) { sb.Append("", ""); sb.Append((" + eutn + @")os[i]); }; sb.Length -= 2; sb.Append("" )""); }");
        }

        sb.Append(@"

        // todo: Nullable<Enum>, BBuffer
");

        foreach (var c in ts._GetClasss())
        {
            var ctn = c._GetTypeDecl_Csharp();

            var fs = c._GetFields<Column>();



            sb.Append(@"
        public static void MsSqlAppend(this StringBuilder sb, List<" + ctn + @"> os, bool ignoreReadOnly = false) { Debug.Assert(os.Count > 0); for (int i = 0; i < os.Count; ++i) { sb.MsSqlAppend(os[i], ignoreReadOnly); sb.Append("", ""); }; if (os.Count > 0) sb.Length -= 2; }
        public static void MsSqlAppend(this StringBuilder sb, " + ctn + @" o, bool ignoreReadOnly = false)
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
                    fnfix = "MsSql";
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

        // 扫带有 [MsSql] 标志的 interface , 生成相应的函数
        var ifaces = ts._GetInterfaces<MsSql>();
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
            sb.Append(iface._GetDesc_Csharp(4) + @"
    public class " + iface.Name + @"
    {
        private SqlConnection conn;
        private SqlCommand cmd;
        private StringBuilder sb = new StringBuilder();
        private List<int> recordsAffecteds = new List<int>();

        public List<int> RecordsAffecteds
        {
            get { return recordsAffecteds; }
        }

        public int RecordsAffected
        {
            get { return recordsAffecteds[0]; }
        }

        public " + iface.Name + @"(SqlConnection conn)
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
" + f._GetDesc_Csharp(8) + @"
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

                    sb.Append(p._GetDesc_Csharp(12) + @"
            " + p.ParameterType._GetTypeDecl_Csharp() + " " + p.Name);

                    if (p.HasDefaultValue)
                    {
                        sb.Append(" = " + p._GetDefaultValueDecl_Csharp());
                    }
                }

                if (ps.Length > 0)
                {
                    sb.Append(@"
        ");
                }
                sb.Append(@")
        {");

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
                                if (pt._IsUserClass() && p._Has<Ignore>())
                                {
                                    sb.Append(@"
            sb.MsSqlAppend(" + pn + @", true);");
                                }
                                else
                                {

                                    sb.Append(@"
            sb.MsSqlAppend(" + pn + @");");
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
                                var nullable = (ct.Namespace == "System" && ct.Name == "Nullable`1");
                                sb.Append(@"
                var rtv" + ii + @" = new List<" + ctn + @">();
                while (r.Read())
                {
                    rtv" + ii + @".Add(" + (nullable ? ("r.IsDBNull(0) ? null : (" + ctn + ")") : "") + ct._GetDataReaderFuncName() + @"(0));
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
                                    var mn = m.Name;

                                    var getfn = m.FieldType._GetDataReaderFuncName();

                                    if (i > 0)
                                    {
                                        sb.Append(",");
                                    }
                                    sb.Append(@"
                        " + mn + @" = " + (ct._IsNullable() ? ("r.IsDBNull(" + i + ") ? null : (" + ctn + ")") : "") + getfn + @"(" + i + @")");
                                }
                                sb.Append(@"
                    });
                }");
                            }
                        }
                        // 单行 / 单个结果
                        else
                        {
                            if (rt._IsSqlNullable())
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
                                    var mn = m.Name;

                                    var getfn = mt._GetDataReaderFuncName();

                                    if (i > 0)
                                    {
                                        sb.Append(", ");
                                    }
                                    sb.Append(@"
                    " + mn + @" = " + (mt._IsSqlNullable() ? ("r.IsDBNull(" + i + ") ? null : (" + mt._GetTypeDecl_Csharp() + ")") : "") + getfn + @"(" + i + @")");
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
                        var nullable = (ct.Namespace == "System" && ct.Name == "Nullable`1");
                        sb.Append(@"
            var rtv = new List<" + ctn + @">();
            using (var r = cmd.ExecuteReader())
            {
                recordsAffecteds.Add(r.RecordsAffected);
                while (r.Read())
                {
                    rtv.Add(" + (nullable ? ("r.IsDBNull(0) ? null : (" + ctn + ")") : "") + ct._GetDataReaderFuncName() + @"(0));
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
                            var mn = m.Name;

                            var getfn = m.FieldType._GetDataReaderFuncName();

                            if (i > 0)
                            {
                                sb.Append(",");
                            }
                            sb.Append(@"
                        " + mn + @" = " + (ct._IsNullable() ? ("r.IsDBNull(" + i + ") ? null : (" + ctn + ")") : "") + getfn + @"(" + i + @")");
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
                    if (rt._IsSqlNullable())
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
                            var mn = m.Name;

                            var getfn = mt._GetDataReaderFuncName();

                            if (i > 0)
                            {
                                sb.Append(", ");
                            }
                            sb.Append(@"
                    " + mn + @" = " + (mt._IsSqlNullable() ? ("r.IsDBNull(" + i + ") ? null : (" + mt._GetTypeDecl_Csharp() + ")") : "") + getfn + @"(" + i + @")");
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

        sb._WriteToFile(Path.Combine(outDir, templateName + "_sqlite.h"));
    }
}
