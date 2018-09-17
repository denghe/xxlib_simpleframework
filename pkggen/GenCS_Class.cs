using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Reflection;
using System.Text;


public static class GenCS_Class
{
    public static void Gen(Assembly asm, string outDir, string templateName, string md5)
    {
        var sb = new StringBuilder();

        // usings
        sb.Append(@"using System;
using xx;");

        // template namespace
        sb.Append(@"
namespace " + templateName + @"
{
    public static class PkgGenMd5
    {
        public const string value = """ + md5 + @"""; 
    }
");

        var ts = asm._GetTypes();

        // 找出所有成员含 Column 标记的类
        var sqlcs = ts._GetClasss().Where(a => a._GetFields().Any(b => b._Has<TemplateLibrary.Column>())).ToList();

        var es = ts._GetEnums();
        for (int i = 0; i < es.Count; ++i)
        {
            var e = es[i];

            // namespace e_ns {
            if (e.Namespace != null && (i == 0 || (i > 0 && es[i - 1].Namespace != e.Namespace))) // namespace 去重
            {
                sb.Append(@"
namespace " + e.Namespace + @"
{");
            }

            // desc
            // public enum xxxxxxxxx : underlyingType
            sb.Append(e._GetDesc()._GetComment_CSharp(4) + @"
    public enum " + e.Name + @" : " + e._GetEnumUnderlyingTypeName_Csharp() + @"
    {");

            // desc
            // xxxxxx = val
            var fs = e._GetEnumFields();
            foreach (var f in fs)
            {
                sb.Append(f._GetDesc()._GetComment_CSharp(8) + @"
        " + f.Name + " = " + f._GetEnumValue(e) + ",");
            }

            // enum /
            sb.Append(@"
    }");

            // namespace }
            if (e.Namespace != null && ((i < es.Count - 1 && es[i + 1].Namespace != e.Namespace) || i == es.Count - 1))
            {
                sb.Append(@"
}");
            }

        }


        var cs = ts._GetClasssStructs();
        for (int i = 0; i < cs.Count; ++i)
        {
            var c = cs[i];
            var o = asm.CreateInstance(c.FullName);

            // namespace c_ns {
            if (c.Namespace != null && (i == 0 || (i > 0 && cs[i - 1].Namespace != c.Namespace))) // namespace 去重
            {
                sb.Append(@"
namespace " + c.Namespace + @"
{");
            }

            // desc
            // public T xxxxxxxxx = defaultValue
            // public const T xxxxxxxxx = defaultValue
            if (c.IsValueType)
            {
                sb.Append(c._GetDesc()._GetComment_CSharp(4) + @"
    public partial struct " + c.Name + @" : IObject
    {");
            }
            else
            {
                sb.Append(c._GetDesc()._GetComment_CSharp(4) + @"
    public partial class " + c.Name + @" : " + c.BaseType._GetTypeDecl_Csharp() + @"
    {");
            }

            // consts( static ) / fields
            var fs = c._GetFieldsConsts();
            foreach (var f in fs)
            {
                var ft = f.FieldType;
                var ftn = ft._GetTypeDecl_Csharp();
                sb.Append(f._GetDesc()._GetComment_CSharp(8) + @"
        public " + (f.IsStatic ? "const " : "") + ftn + " " + f.Name);

                var v = f.GetValue(f.IsStatic ? null : o);
                var dv = v._GetDefaultValueDecl_Csharp();
                if (dv != "")
                {
                    sb.Append(" = " + dv + ";");
                }
                //else if (f._Has<TemplateLibrary.CreateInstance>())
                //{
                //    sb.Append(" = new " + ftn + "();");
                //}
                else
                {
                    sb.Append(";");
                }
            }

            sb.Append(@"

        public" + (c.IsValueType ? "" : " override") + @" ushort GetPackageId()
        {
            return TypeId<" + c.Name + @">.value;
        }

        public" + (c.IsValueType ? "" : " override") + @" void ToBBuffer(BBuffer bb)
        {");

            if (!c.IsValueType && c._HasBaseType())
            {
                sb.Append(@"
            base.ToBBuffer(bb);");
            }
            fs = c._GetFields();
            foreach (var f in fs)
            {
                if (f.FieldType._IsExternal() && !f.FieldType._GetExternalSerializable()) continue;
                var ft = f.FieldType;
                if (f._Has<TemplateLibrary.NotSerialize>())
                {
                    sb.Append(@"
            bb.Write(default(" + f.FieldType._GetTypeDecl_Csharp() + "));");
                }
                else if (f._Has<TemplateLibrary.CustomSerialize>())
                {
                    sb.Append(@"
            bb.CustomWrite(bb, this, """ + f.Name + @""");");
                }
                else if (ft.IsEnum)
                {
                    sb.Append(@"
            bb.Write((" + ft._GetEnumUnderlyingTypeName_Csharp() + ")this." + f.Name + ");");
                }
                else if (!ft._IsNullable() && ft.IsValueType && !ft.IsPrimitive)
                {
                    sb.Append(@"
            ((IObject)this." + f.Name + ").ToBBuffer(bb);");
                }
                else
                {
                    sb.Append(@"
            bb.Write(this." + f.Name + ");");
                }
            }

            sb.Append(@"
        }

        public" + (c.IsValueType ? "" : " override") + @" void FromBBuffer(BBuffer bb)
        {");
            if (!c.IsValueType && c._HasBaseType())
            {
                sb.Append(@"
            base.FromBBuffer(bb);");
            }
            fs = c._GetFields();
            foreach (var f in fs)
            {
                if (f.FieldType._IsExternal() && !f.FieldType._GetExternalSerializable()) continue;
                var ft = f.FieldType;

                if (ft.IsEnum)
                {
                    var ftn = ft._GetTypeDecl_Csharp();
                    var etn = ft._GetEnumUnderlyingTypeName_Csharp();
                    sb.Append(@"
            {
                " + etn + @" tmp = 0;
                bb.Read(ref tmp);
                this." + f.Name + @" = (" + ftn + @")tmp;
            }");
                }
                else if (!ft._IsNullable() && ft.IsValueType && !ft.IsPrimitive)
                {
                    sb.Append(@"
            ((IObject)this." + f.Name + ").FromBBuffer(bb);");
                }
                else
                {
                    if (ft._IsContainer())
                    {
                        sb.Append(@"
            bb.readLengthLimit = " + f._GetLimit() + ";");
                    }
                    sb.Append(@"
            bb.Read(ref this." + f.Name + ");");
                }
            }
            sb.Append(@"
        }
        public" + (c.IsValueType ? "" : " override") + @" void ToString(System.Text.StringBuilder s)
        {");

            if (!c.IsValueType)
            {
                sb.Append(@"
            if (__toStringing)
            {
        	    s.Append(""[ \""***** recursived *****\"" ]"");
        	    return;
            }
            else __toStringing = true;
");
            }
            sb.Append(@"
            s.Append(""{ \""pkgTypeName\"":\""" + (string.IsNullOrEmpty(c.Namespace) ? c.Name : c.Namespace + "." + c.Name) + @"\"", \""pkgTypeId\"":"" + GetPackageId());
            ToStringCore(s);
            s.Append("" }"");");
            if (!c.IsValueType)
            {
                sb.Append(@"

            __toStringing = false;");
            }

            sb.Append(@"
        }
        public" + (c.IsValueType ? "" : " override") + @" void ToStringCore(System.Text.StringBuilder s)
        {" + (c._HasBaseType() ? @"
            base.ToStringCore(s);" : ""));
            foreach (var f in fs)
            {
                var ft = f.FieldType;
                if (ft._IsExternal() && !ft._GetExternalSerializable()) continue;
                if (ft._IsString())
                {
                    sb.Append(@"
            if (" + f.Name + @" != null) s.Append("", \""" + f.Name + @"\"":\"""" + " + f.Name + @" + ""\"""");
            else s.Append("", \""" + f.Name + @"\"":nil"");");
                }
                else if (ft._IsNullable())
                {
                    sb.Append(@"
            s.Append("", \""" + f.Name + @"\"":"" + (" + f.Name + @".HasValue ? " + f.Name + @".Value.ToString() : ""nil""));");
                }
                else if (ft._IsUserClass() || ft._IsString() || ft._IsList())  // todo: 进一步判断所有可能为空的
                {
                    sb.Append(@"
            s.Append("", \""" + f.Name + @"\"":"" + (" + f.Name + @" == null ? ""nil"" : " + f.Name + @".ToString()));");
                }
                else
                {
                    sb.Append(@"
            s.Append("", \""" + f.Name + @"\"":"" + " + f.Name + @");");
                }
            }
            sb.Append(@"
        }
        public override string ToString()
        {
            var sb = new System.Text.StringBuilder();
            ToString(sb);
            return sb.ToString();
        }");

            if (!c.IsValueType)
            {
                sb.Append(@"
        public override void MySqlAppend(System.Text.StringBuilder sb, bool ignoreReadOnly)
        {");
                if (sqlcs.Contains(c))
                {
                    sb.Append(@"
            sb.Append(""("");");

                    foreach (var m in fs)
                    {
                        if (!m._Has<TemplateLibrary.Column>()) continue;
                        if (m._IsReadOnly())
                        {
                            sb.Append(@"
            if (!ignoreReadOnly)
            {");
                        }

                        var mt = m.FieldType;
                        var mtn = mt._GetTypeDecl_Csharp();

                        if (mt._IsUserClass())
                        {
                            throw new Exception("column is Object, unsupported now.");
                        }
                        else if (mt._IsString())
                        {
                            sb.Append(@"
            sb.Append(this." + m.Name + @" == null ? ""null"" : (""'"" + this." + m.Name + @".Replace(""'"", ""''"") + ""'""));");
                        }
                        else if (mt.IsEnum)
                        {
                            sb.Append(@"
            sb.Append(" + "(" + mt._GetEnumUnderlyingTypeName_Csharp() + ")this." + m.Name + @");");
                        }
                        else if (mt._IsNullable())
                        {
                            sb.Append(@"
            sb.Append(this." + m.Name + @".HasValue ? this." + m.Name + @".Value.ToString() : ""null"");");
                        }
                        else
                        {
                            sb.Append(@"
            sb.Append(this." + m.Name + @");");
                        }

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
");
                }
                else
                {
                    if (c._HasBaseType())
                    {
                        sb.Append(@"
            base.MySqlAppend(sb, ignoreReadOnly);");
                    }
                }
                sb.Append(@"
        }");
            }

            // todo: ToString support


            // class /
            sb.Append(@"
    }");

            // namespace }
            if (c.Namespace != null && ((i < cs.Count - 1 && cs[i + 1].Namespace != c.Namespace) || i == cs.Count - 1))
            {
                sb.Append(@"
}");
            }

        }

        // 遍历所有 type 及成员数据类型 生成  BBuffer.Register< T >( typeId ) 函数组. 0 不能占. string 占掉 1. BBuffer 占掉 2.
        // 在基础命名空间中造一个静态类 AllTypes 静态方法 Register

        var typeIds = new TemplateLibrary.TypeIds(asm);
        sb.Append(@"
    public static class AllTypes
    {
        public static void Register()
        {
            xx.Object.RegisterInternals();");

        foreach (var kv in typeIds.types)
        {
            var ct = kv.Key;
            if (ct._IsString() || ct._IsBBuffer() || ct._IsExternal() && !ct._GetExternalSerializable()) continue;
            var ctn = ct._GetTypeDecl_Cpp(templateName);
            var typeId = kv.Value;

            sb.Append(@"
            xx.Object.Register<" + ct._GetTypeDecl_Csharp() + @">(" + typeId++ + ");");
        }

        sb.Append(@"
        }
    }");

        // template namespace /
        sb.Append(@"
}
");


        sb._WriteToFile(Path.Combine(outDir, templateName + "_class.cs"));
    }
}
