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
        sb.Append(@"using System;");

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
            sb.Append(e._GetDesc_Csharp(4) + @"
    public enum " + e.Name + @" : " + e._GetEnumUnderlyingTypeName_Csharp() + @"
    {");

            // desc
            // xxxxxx = val
            var fs = e._GetEnumFields();
            foreach (var f in fs)
            {
                sb.Append(f._GetDesc_Csharp(8) + @"
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


        var cs = ts._GetClasss();
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
            sb.Append(c._GetDesc_Csharp(4) + @"
    public partial class " + c.Name + @"
    {");

            // consts( static ) / fields
            var fs = c._GetFieldsConsts();
            foreach (var f in fs)
            {
                var ft = f.FieldType;
                var ftn = ft._GetTypeDecl_Csharp();
                if (f._Has<TemplateLibrary.Property>())
                {
                    if (f.IsStatic) throw new Exception("static con't be property");
                    sb.Append(f._GetDesc_Csharp(8) + @"
        public " + ftn + " " + f.Name + " { get; set; }");
                }
                else
                {
                    sb.Append(f._GetDesc_Csharp(8) + @"
        public " + (f.IsStatic ? "const " : "") + ftn + " " + f.Name);
                }
                var v = f.GetValue(f.IsStatic ? null : o);
                var dv = v._GetDefaultValueDecl_Csharp();
                if (dv != "")
                {
                    sb.Append(" = " + dv + ";");
                }
                else
                {
                    if (!f._Has<TemplateLibrary.Property>()) sb.Append(";");
                }
            }

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

        // template namespace /
        sb.Append(@"
}
");

        sb._WriteToFile(Path.Combine(outDir, templateName + "_class.cs"));
    }
}
