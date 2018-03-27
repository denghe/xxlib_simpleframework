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
        sb.Append(@"#pragma once
#include """ + templateName + @"_class.h""
#include ""xx_sqlite.h""

namespace " + templateName + @"
{");

        // 扫带有 [SQLite] 标志的 interface , 生成相应的函数
        var ifaces = ts._GetInterfaces<SQLite>();
        foreach (var iface in ifaces)
        {
            // namespace xxx {
            if (iface.Namespace != null)
            {
                sb.Append(@"
namespace " + iface.Namespace + @"
{");
            }

            // struct {
            sb.Append(iface._GetDesc()._GetComment_Cpp(4) + @"
    class " + iface.Name + @" : xx::Object
    {
    public:
		xx::SQLite& sqlite;
		xx::SQLiteString s;

		inline " + iface.Name + @"(xx::SQLite& sqlite)
            : xx::Object(sqlite.mempool)
            , sqlite(sqlite)
            , s(sqlite.mempool)
        {
        }");
            //int recordsAffected // todo

            // 如果参数都是简单数据类型( 不含 List / Sql拼接段 啥的 ), 生成 SqlCommand + Parameter 复用

            // members = default vals;
            var fs = iface._GetMethods();
            foreach (var f in fs)
            {
                var fn = f.Name;
                if (!f._Has<Sql>())
                    throw new Exception("no [Sql] attribute on " + iface.Name + "." + fn);

                var ps = f.GetParameters();
                var rt = f.ReturnType;
                var rtn = rt._GetSafeTypeDecl_Cpp(templateName);

                var sb2 = new StringBuilder();
                sb2.Append(@"


        xx::SQLiteQuery_p query_" + fn + @";" + f._GetDesc()._GetComment_Cpp(8) + @"
        inline " + rtn + " " + fn);

                if (ps.Length > 0)
                {
                    sb2.Append(@"
        ");
                }

                sb2.Append(@"(");
                foreach (var p in ps)
                {
                    if (p != ps[0])
                    {
                        sb2.Append(",");
                    }
                    var pt = p.ParameterType;

                    sb2.Append(p._GetDesc()._GetComment_Cpp(12) + @"
            " + pt._GetSafeTypeDecl_Cpp(templateName) + " const& " + p.Name);

                    if (p.HasDefaultValue)
                    {
                        sb2.Append(" = " + p._GetDefaultValueDecl_Cpp(templateName));    // todo: string 支持
                    }
                }

                if (ps.Length > 0)
                {
                    sb2.Append(@"
        ");
                }
                sb2.Append(@")
        {
");
                // recordsAffecteds.Clear();

                var sqls = f._GetSql()._SpliteSql();

                // 看看是否要生成拼字串的代码
                bool needSqlAppend = false;

                // 合并 sql 成 1 个串 for NewQuery
                var sqlcombine = new StringBuilder();
                foreach (var o in sqls)
                {
                    if (o is string)
                    {
                        sqlcombine.Append(o);
                    }
                    else if (ps[(int)o].ParameterType._IsList()
                        || ps[(int)o]._Has<Literal>())
                    {
                        needSqlAppend = true;
                    }
                    else
                    {
                        sqlcombine.Append("?");
                    }
                }

                if (needSqlAppend)
                {
                    sb2.Append(@"
            this->s->Clear();");
                    foreach (var o in sqls)
                    {
                        if (o is string)
                        {
                            sb2.Append(@"
            this->s->Append(R""=-=(" + o + @")=-="");");
                        }
                        else
                        {
                            var p = ps[(int)o];
                            var pn = p.Name;
                            var pt = p.ParameterType;
                            sb2.Append(@"
            this->s->" + (p._Has<Literal>() ? "" : "SQL") + "Append(" + pn + @");");
                        }
                    }
                    sb2.Append(@"
            this->query_" + fn + @" = sqlite.CreateQuery(s->C_str(), s->dataLen);");
                }
                else
                {
                    sb2.Append(@"
			if (!this->query_" + fn + @")
			{
				this->query_" + fn + @" = this->sqlite.CreateQuery(R""=-=(" + sqlcombine + @")=-="");
			}");
                }
                if (rtn == "void")
                {
                }
                else
                {
                    sb2.Append(@"
            " + rtn + @" rtv;");
                    if (rt._IsList()) sb2.Append(@"
            this->mempool.MPCreateTo(rtv);");
                }

                if (ps.Where(p => !p.ParameterType._IsList() && !p._Has<Literal>()).Count() > 0)
                {
                    sb2.Append(@"
            this->query_" + fn + @"->SetParameters(");
                    foreach (var o in sqls)
                    {
                        if (o is string) continue;
                        var p = ps[(int)o];
                        if (p.ParameterType._IsList()) continue;
                        sb2.Append(p.Name + ", ");
                    }
                    sb2.Length -= 2;
                    sb2.Append(@");");
                }

                // 根据函数返回值类型，生成不同的代码段。

                // 多结果集 SQLite 不支持
                if (rt._IsTuple())
                {
                    throw new NotSupportedException();
                }
                // 单结果集
                else if (rt.IsGenericType && rt.Name == "List`1")
                {
                    var ct = rt.GenericTypeArguments[0];
                    var ctn = ct._GetTypeDecl_Cpp(templateName);

                    // 用户类
                    if (ct._IsUserClass())
                    {
                        sb2.Append(@"
			this->query_" + fn + @"->Execute([&](xx::SQLiteReader& sr)
            {
				auto& r = rtv->Emplace(this->mempool);");
                        var ctfs = ct._GetFields();
                        if (ctfs.Count() == 0)
                            throw new Exception("the class's fields can't be empty");

                        for (int i = 0; i < ctfs.Count; ++i)
                        {
                            var m = ctfs[i];
                            var mt = m.FieldType;
                            var mn = m.Name;
                            var getfn = mt._GetDataReaderFuncName_Cpp(i);

                            sb2.Append(@"
                " + (mt._IsSqlNullable() && !m._Has<NotNull>() ? "if (!sr.IsDBNull(0)) " : "") + "r->" + mn + @" = " + getfn + @";");
                        }
                    }
                    // 简单类型
                    else
                    {
                        sb2.Append(@"
            this->query_" + fn + @"->Execute([&](xx::SQLiteReader& sr)
            {
				" + (ct._IsSqlNullable() ? @"if (sr.IsDBNull(0)) rtv->Emplace();
                else " : "") + @"rtv->Add(" + ct._GetDataReaderFuncName_Cpp(0) + @");");
                    }
                    sb2.Append(@"
            });
			return rtv;");
                }
                // 单行 / 单个结果
                else
                {
                    if (rt._IsVoid())
                    {
                        // 无返回值
                        sb2.Append(@"
            this->query_" + fn + @"->Execute();");
                    }
                    // 用户类
                    else if (rt._IsUserClass())
                    {
                        var rtfs = rt._GetFields();
                        if (rtfs.Count() == 0)
                            throw new Exception("the class's fields can't be empty");

                        sb2.Append(@"
			this->query_" + fn + @"->Execute([&](xx::SQLiteReader& sr)
            {
                assert(!rtv);
                this->mempool.MPCreateTo(rtv);");
                        for (int i = 0; i < rtfs.Count; ++i)
                        {
                            var m = rtfs[i];
                            var mt = m.FieldType;
                            var mn = m.Name;
                            var getfn = mt._GetDataReaderFuncName_Cpp(i);

                            sb2.Append(@"
                " + (mt._IsSqlNullable() && !m._Has<NotNull>() ? "if (!sr.IsDBNull(0)) " : "") + "rtv->" + mn + @" = " + getfn + @";");
                        }
                        sb2.Append(@"
            });
            return rtv;");
                    }
                    // 简单类型
                    else
                    {
                        sb2.Append(@"
            this->query_" + fn + @"->Execute([&](xx::SQLiteReader& sr)
            {");
                        sb2.Append(@"
                " + (rt._IsSqlNullable() ? "if (!sr.IsDBNull(0)) " : "") + "rtv = " + rt._GetDataReaderFuncName_Cpp(0) + @";");
                        sb2.Append(@"
            });
            return rtv;");
                    }
                }

                // func }
                sb2.Append(@"
        }");

                sb.Append(sb2);

                // if 有 string 参数, 再生成一份 char const* 版
                if (ps.Any(p => p.ParameterType._IsString()))
                {
                    sb.Append(sb2.Replace("xx::SQLiteQuery_p", "// xx::SQLiteQuery_p").Replace("xx::String_p", "char const*"));
                }

            }


            // class }
            sb.Append(@"
    };");

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
