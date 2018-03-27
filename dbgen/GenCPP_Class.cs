using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Reflection;
using System.Text;

public static class GenCPP_Class
{
    public static void Gen(Assembly asm, string outDir, string templateName)
    {
        var sb = new StringBuilder();

        // usings
        sb.Append(@"#pragma once
#include ""xx.h""
#include <optional>

");

        // template namespace
        sb.Append(@"
namespace " + templateName + @"
{");

        var ts = asm._GetTypes();

        // predefines

        var cs = ts._GetClasssStructs();
        for (int i = 0; i < cs.Count; ++i)
        {
            var c = cs[i];

            // namespace e_ns {
            if (c.Namespace != null && (i == 0 || (i > 0 && cs[i - 1].Namespace != c.Namespace))) // namespace 去重
            {
                sb.Append(@"
namespace " + c.Namespace + @"
{");
            }

            // 预声明
            sb.Append(c._GetDesc()._GetComment_Cpp(4) + @"
    " + (c.IsValueType ? "struct" : "class") + @" " + c.Name + @";
    using " + c.Name + @"_p = xx::Ptr<" + c.Name + @">;
	using " + c.Name + @"_r = xx::Ref<" + c.Name + @">;
");

            // namespace }
            if (c.Namespace != null && ((i < cs.Count - 1 && cs[i + 1].Namespace != c.Namespace) || i == cs.Count - 1))
            {
                sb.Append(@"
}");
            }
        }

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
            // enum class xxxxxxxxx : underlyingType
            sb.Append(e._GetDesc()._GetComment_Cpp(4) + @"
    enum class " + e.Name + @" : " + e._GetEnumUnderlyingTypeName_Cpp() + @"
    {");

            // desc
            // xxxxxx = val
            var fs = e._GetEnumFields();
            foreach (var f in fs)
            {
                sb.Append(f._GetDesc()._GetComment_Cpp(8) + @"
        " + f.Name + " = " + f._GetEnumValue(e) + ",");
            }

            // enum /
            sb.Append(@"
    };");

            // namespace }
            if (e.Namespace != null && ((i < es.Count - 1 && es[i + 1].Namespace != e.Namespace) || i == es.Count - 1))
            {
                sb.Append(@"
}");
            }
        }

        var ss = ts._GetStructs()._SortByInheritRelation();
        for (int i = 0; i < ss.Count; ++i)
        {
            var c = ss[i];
            var o = asm.CreateInstance(c.FullName);

            // namespace e_ns {
            if (c.Namespace != null && (i == 0 || (i > 0 && es[i - 1].Namespace != c.Namespace))) // namespace 去重
            {
                sb.Append(@"
namespace " + c.Namespace + @"
{");
            }

            // desc
            // struct xxxxxxxxx
            sb.Append(c._GetDesc()._GetComment_Cpp(4) + @"
    struct " + c.Name + @"
    {");

            // desc
            // T xxxxxx = val
            // consts( static ) / fields
            var fs = c._GetFieldsConsts();
            foreach (var f in fs)
            {
                var ft = f.FieldType;
                var ftn = ft._GetSafeTypeDecl_Cpp(templateName);
                sb.Append(f._GetDesc()._GetComment_Cpp(8) + @"
        " + (f.IsStatic ? "constexpr " : "") + ftn + " " + f.Name);

                // 当前还无法正确处理 String 数据类型的默认值

                var v = f.GetValue(f.IsStatic ? null : o);
                var dv = v._GetDefaultValueDecl_Cpp(templateName);
                if (dv != "" && !ft._IsUserClass() && !ft._IsString())  // 当前还无法正确处理 String 数据类型的默认值
                {
                    sb.Append(" = " + dv + ";");
                }
                else
                {
                    sb.Append(";");
                }
            }

            // struct /
            sb.Append(@"
    };");

            // namespace }
            if (c.Namespace != null && ((i < es.Count - 1 && es[i + 1].Namespace != c.Namespace) || i == es.Count - 1))
            {
                sb.Append(@"
}");
            }
        }



        cs = ts._GetClasss()._SortByInheritRelation();

        // 预声明
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

            // 定位到基类
            var bt = c.BaseType;
            var btn = c._HasBaseType() ? bt._GetTypeDecl_Cpp(templateName).CutLast() : "xx::Object";

            // desc
            // T xxxxxxxxx = defaultValue
            // constexpr T xxxxxxxxx = defaultValue

            sb.Append(c._GetDesc()._GetComment_Cpp(4) + @"
    class " + c.Name + @" : public " + btn + @"
    {
    public:");

            // consts( static ) / fields
            var fs = c._GetFieldsConsts();
            foreach (var f in fs)
            {
                var ft = f.FieldType;
                var ftn = ft._GetSafeTypeDecl_Cpp(templateName);
                sb.Append(f._GetDesc()._GetComment_Cpp(8) + @"
        " + (f.IsStatic ? "constexpr " : "") + ftn + " " + f.Name);

                var v = f.GetValue(f.IsStatic ? null : o);
                var dv = v._GetDefaultValueDecl_Cpp(templateName);
                if (dv != "" && !ft._IsUserClass() && !ft._IsString())  // 当前还无法正确处理 String 数据类型的默认值
                {
                    sb.Append(" = " + dv + ";");
                }
                else
                {
                    sb.Append(";");
                }
            }

            sb.Append(@"

        typedef " + c.Name + @" ThisType;
        typedef " + btn + @" BaseType;
	    " + c.Name + @"();
		" + c.Name + @"(" + c.Name + @" const&) = delete;
		" + c.Name + @"& operator=(" + c.Name + @" const&) = delete;
        virtual void ToString(xx::String &s) const override;
        virtual void ToStringCore(xx::String &s) const override;
    };

");   // class }

            // namespace }
            if (c.Namespace != null && ((i < cs.Count - 1 && cs[i + 1].Namespace != c.Namespace) || i == cs.Count - 1))
            {
                sb.Append(@"
}");
            }

        }

        cs = ts._GetClasss();   //._SortByInheritRelation();
        // 实现
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

            // 定位到基类
            var bt = c.BaseType;
            var btn = c._HasBaseType() ? bt._GetTypeDecl_Cpp(templateName).CutLast() : "xx::Object";

            sb.Append(@"
	inline " + c.Name + @"::" + c.Name + @"()");
            bool dot = false;
            if (c._HasBaseType())
            {
                sb.Append(@"
        " + (dot ? "." : ":") + " " + btn + @"()");
                dot = true;
            }
            sb.Append(@"
	{
	}

    inline void " + c.Name + @"::ToString(xx::String &s) const
    {
        if (memHeader().flags)
        {
        	s.Append(""[ \""***** recursived *****\"" ]"");
        	return;
        }
        else memHeader().flags = 1;

        s.Append(""{ \""type\"" : \""" + c.Name + @"\"""");
        ToStringCore(s);
        s.Append("" }"");
        
        memHeader().flags = 0;
    }
    inline void " + c.Name + @"::ToStringCore(xx::String &s) const
    {
        this->BaseType::ToStringCore(s);");
            var fs = c._GetFields();
            foreach (var f in fs)
            {
                if (f.FieldType._IsString())
                {
                    sb.Append(@"
        if (this->" + f.Name + @") str.Append("", \""" + f.Name + @"\"" : \"""", this->" + f.Name + @", ""\"""");
        else str.Append("", \""" + f.Name + @"\"" : nil"");");
                }
                else
                {
                    sb.Append(@"
        str.Append("", \""" + f.Name + @"\"" : "", this->" + f.Name + @");");
                }
            }
            sb.Append(@"
    }");

            // namespace }
            if (c.Namespace != null && ((i < cs.Count - 1 && cs[i + 1].Namespace != c.Namespace) || i == cs.Count - 1))
            {
                sb.Append(@"
}");
            }

        }

        sb.Append(@"
}");

        // 泛型接口适配
        cs = ts._GetStructs();
        if (cs.Count > 0)
        {
            sb.Append(@"
namespace xx
{");
            foreach (var c in cs)
            {
                var ctn = c._GetTypeDecl_Cpp(templateName);
                var fs = c._GetFields();

                sb.Append(@"
	template<>
	struct StrFunc<" + ctn + @", void>
	{
		static inline void WriteTo(xx::String& s, " + ctn + @" const &in)
		{
			s.Append(""{ \""type\"" : \""" + c.Name + @"\""""");
                foreach (var f in fs)
                {
                    sb.Append(@", "", \""" + f.Name + @"\"" : "", in." + f.Name);
                }
                sb.Append(@", "" }"");
        }
    };");

            }

            sb.Append(@"
}
");
        }

        sb._WriteToFile(Path.Combine(outDir, templateName + "_class.h"));
    }
}
