using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Reflection;
using System.Text;


public static class GenCS_Class
{
    public static void Gen(Assembly asm, string outDir, string templateName)
    {
        var sb = new StringBuilder();

        // usings
        sb.Append(@"using System;
using xx;");

        // template namespace
        sb.Append(@"
namespace " + templateName + @"
{");

        var ts = asm._GetTypes();
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
    public partial struct " + c.Name + @" : IBBuffer
    {");
            }
            else
            {
                sb.Append(c._GetDesc()._GetComment_CSharp(4) + @"
    public partial class " + c.Name + @" : " + (c._HasBaseType() ? c.BaseType._GetTypeDecl_Csharp() : "IBBuffer") + @"
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

        public " + (c.IsValueType ? "" : (c._HasBaseType() ? "override " : "virtual ")) + @"ushort GetPackageId()
        {
            return TypeIdMaps<" + c.Name + @">.typeId;
        }

        public " + (c.IsValueType ? "" : (c._HasBaseType() ? "override" : "virtual")) + @" void ToBBuffer(BBuffer bb)
        {");

            if (!c.IsValueType && c._HasBaseType())
            {
                sb.Append(@"
            base.ToBBuffer(bb);");
            }
            fs = c._GetFields();
            foreach (var f in fs)
            {
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
                else if (ft.IsValueType && !ft.IsPrimitive)
                {
                    sb.Append(@"
            ((IBBuffer)this." + f.Name + ").ToBBuffer(bb);");
                }
                else
                {
                    sb.Append(@"
            bb.Write(this." + f.Name + ");");
                }
            }

            sb.Append(@"
        }

        public " + (c.IsValueType ? "" : (c._HasBaseType() ? "override" : "virtual")) + @" void FromBBuffer(BBuffer bb)
        {");
            if (!c.IsValueType && c._HasBaseType())
            {
                sb.Append(@"
            base.FromBBuffer(bb);");
            }
            fs = c._GetFields();
            foreach (var f in fs)
            {
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
                else if (ft.IsValueType && !ft.IsPrimitive)
                {
                    sb.Append(@"
            ((IBBuffer)this." + f.Name + ").FromBBuffer(bb);");
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
");

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
            // BBuffer.Register<string>(1);
            BBuffer.Register<BBuffer>(2);");

        foreach (var kv in typeIds.types)
        {
            if (kv.Key == typeof(string) || kv.Key == typeof(TemplateLibrary.BBuffer)) continue;
            var ct = kv.Key;
            var ctn = ct._GetTypeDecl_Cpp(templateName).CutLast();
            var typeId = kv.Value;

            sb.Append(@"
            BBuffer.Register<" + ct._GetTypeDecl_Csharp() + @">(" + typeId++ + ");");
        }

        sb.Append(@"

            BBuffer.handlers = new Action<IBBuffer>[" + typeIds.maxId + @"];
        }
    }");

        // template namespace /
        sb.Append(@"
}
");


        sb._WriteToFile(Path.Combine(outDir, templateName + "_class.cs"));
    }
}
