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
using xx;
");

        // template namespace
        sb.Append(@"
namespace " + templateName + @"
{");

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
        #region conn, cmd, sb, recordsAffecteds
        public MySql.Data.MySqlClient.MySqlConnection conn;
        public MySql.Data.MySqlClient.MySqlCommand cmd;
        public System.Text.StringBuilder sb = new System.Text.StringBuilder();
        private List<int> recordsAffecteds = new List<int>();

        public List<int> RecordsAffecteds
        {
            get { return recordsAffecteds; }
        }

        public int RecordsAffected
        {
            get { return recordsAffecteds[0]; }
        }

        public " + iface.Name + @"(MySql.Data.MySqlClient.MySqlConnection conn)
        {
            this.conn = conn;
            this.cmd = conn.CreateCommand();
        }
        #endregion
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
                var w = "";
                var w2 = "";


                // todo: 实现一个非泛型版以方便使用
                //                sb.Append(@"
                //" + f._GetDesc()._GetComment_CSharp(8) + @"
                //        " + "public " + rtn + " " + f.Name + w2);

                //                if (ps.Length > 0)
                //                {
                //                    sb.Append(@"
                //        ");
                //                }

                //                sb.Append(@"(");
                //                foreach (var p in ps)
                //                {
                //                    if (p != ps[0])
                //                    {
                //                        sb.Append(",");
                //                    }

                //                    sb.Append(p._GetDesc()._GetComment_CSharp(12) + @"
                //            " + p.ParameterType._GetTypeDecl_Csharp() + " " + p.Name);

                //                    if (p.HasDefaultValue)
                //                    {
                //                        sb.Append(" = " + p._GetDefaultValueDecl_Csharp());
                //                    }
                //                }

                //                if (ps.Length > 0)
                //                {
                //                    sb.Append(@"
                //            , ");
                //                }
                //                sb.Append(@"MySql.Data.MySqlClient.MySqlTransaction tran_ = null)
                //            {
                //                " + (rtn == "void" ? "return " : "") + @" " + f.Name + @"_<>();
                //            }");


                // 生成泛型版返回值
                // 多结果集
                if (rt._IsTuple())
                {
                    rtn = "Tuple <";
                    for (int i = 0; i < rt.GenericTypeArguments.Length; ++i)
                    {
                        var crt = rt.GenericTypeArguments[i];
                        var crtn = crt._GetTypeDecl_Csharp();
                        if (i > 0)
                        {
                            rtn += ", ";
                        }
                        // 单结果集
                        if (crt._IsList())
                        {
                            var cct = crt.GenericTypeArguments[0];
                            var cctn = cct._GetTypeDecl_Csharp();
                            if (cct._IsUserClass())
                            {
                                rtn += $"List<T{i}>";

                                if (w == "")
                                {
                                    w = $" where T{i} : " + cctn + ", new()";
                                    w2 = $"<T{i}";
                                }
                                else
                                {
                                    w += $" where T{i} : " + cctn + ", new()";
                                    w2 += $", T{i}";
                                }
                            }
                            else
                            {
                                rtn += $"List<{cctn}>";
                            }
                        }
                        // 单行 / 单个结果
                        else
                        {
                            if (crt._IsUserClass())
                            {
                                rtn += $"T{i}";

                                if (w == "")
                                {
                                    w = $" where T{i} : " + crtn + ", new()";
                                    w2 = $"<T{i}";
                                }
                                else
                                {
                                    w += $" where T{i} : " + crtn + ", new()";
                                    w2 += $", T{i}";
                                }
                            }
                            else
                            {
                                rtn += crtn;
                            }
                        }
                    }
                    rtn += ">";
                    if (w2 != "") w2 += ">";
                }
                // 单结果集
                else if (rt._IsList())
                {
                    var ct = rt.GenericTypeArguments[0];
                    if (ct._IsUserClass())
                    {
                        rtn = "List<T>";
                        w = $" where T : " + ct._GetTypeDecl_Csharp() + ", new()";
                        w2 = "<T>";
                    }
                }
                // 单行 / 单个结果
                else
                {
                    if (rt._IsUserClass())
                    {
                        rtn = "T";
                        w = $" where T : " + rt._GetTypeDecl_Csharp() + ", new()";
                        w2 = "<T>";
                    }
                }

                var rtn_bak = rtn;


                sb.Append(@"
" + f._GetDesc()._GetComment_CSharp(8) + @"
        " + "public " + rtn + " " + f.Name + w2);

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
                sb.Append(@"MySql.Data.MySqlClient.MySqlTransaction tran_ = null)" + w + @"
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
                                if (pt._IsList() || pt._IsUserClass())
                                {
                                    sb.Append(@"
            " + pn + @".MySqlAppend(sb, " + (p._Has<SkipReadOnly>() ? "true" : "false") + ");");
                                }
                                else if (pt._IsString())
                                {
                                    sb.Append(@"
            sb.Append(" + pn + @" == null ? ""null"" : (""'"" + " + pn + @".Replace(""'"", ""''"") + ""'""));");
                                }
                                else if (pt.IsEnum)
                                {
                                    sb.Append(@"
            sb.Append(" + "(" + pt._GetEnumUnderlyingTypeName_Csharp() + ")" + pn + @");");
                                }
                                else
                                {
                                    sb.Append(@"
            sb.Append(" + pn + @");");
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




                var sb_bak = sb;
                sb = new StringBuilder();
                var GenRtv = new Action(() => {
                

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
                        if (rt._IsList())
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
                var rtv" + ii + @" = new List<T" + jj + @">();
                while (r.Read())
                {
                    rtv" + ii + @".Add(new T" + jj + @"
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
                T" + jj + @" rtv" + ii + @" = null;
                if (r.Read())
                {
                    rtv" + ii + @" = new T" + jj + @"
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
                else if (rt._IsList())
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
            var rtv = new List<T>();
            using (var r = cmd.ExecuteReader())
            {
                recordsAffecteds.Add(r.RecordsAffected);
                while (r.Read())
                {
                    rtv.Add(new T
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

                });

                GenRtv();
                var sb2 = sb;
                sb = sb_bak;
                sb.Append(sb2);
                rtn = rtn_bak;



                // 代码基本同上, 生成一个 dynamic o 成员展开传参的重载版本
                if (false && ps.Length > 0)
                {

                    sb.Append(@"
" + f._GetDesc()._GetComment_CSharp(8) + @"
        " + "public " + rtn + " " + f.Name + "_d" + w2 + "(dynamic o, MySql.Data.MySqlClient.MySqlTransaction tran_ = null)" + w + @"
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
            sb.Append(o." + pn + @");");
                                }
                                else
                                {
                                    if (pt._IsList() || pt._IsUserClass())
                                    {
                                        sb.Append(@"
            o." + pn + @".MySqlAppend(sb, " + (p._Has<SkipReadOnly>() ? "true" : "false") + ");");
                                    }
                                    else if (pt._IsString())
                                    {
                                        sb.Append(@"
            sb.Append(o." + pn + @" == null ? ""null"" : (""'"" + o." + pn + @".Replace(""'"", ""''"") + ""'""));");
                                    }
                                    else if (pt.IsEnum)
                                    {
                                        sb.Append(@"
            sb.Append(" + "(" + pt._GetEnumUnderlyingTypeName_Csharp() + ")o." + pn + @");");
                                    }
                                    else
                                    {
                                        sb.Append(@"
            sb.Append(o." + pn + @");");
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

                    sb.Append(sb2);

                }

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
