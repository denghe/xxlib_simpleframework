using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Reflection;
using System.Text;

public static class GenCPP_Class
{
    public static void Gen(Assembly asm, string outDir, string templateName, string md5)
    {
        var sb = new StringBuilder();

        // template namespace
        sb.Append(@"
#include ""xx.h""

namespace " + templateName + @"
{
	struct PkgGenMd5
	{
		static constexpr char const* value = """ + md5 + @""";
    };
");
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
namespace " + c.Namespace.Replace(".", "::") + @"
{");
            }

            // desc
            // enum class xxxxxxxxx : underlyingType
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
namespace " + e.Namespace.Replace(".", "::") + @"
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
namespace " + c.Namespace.Replace(".", "::") + @"
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

                var v = f.GetValue(f.IsStatic ? null : o);
                var dv = v._GetDefaultValueDecl_Cpp(templateName);
                if (dv != "" && !ft._IsList() && !ft._IsUserClass() && !ft._IsString())  // 当前还无法正确处理 String 数据类型的默认值
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
namespace " + c.Namespace.Replace(".", "::") + @"
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
                if (dv != "" && !ft._IsList() && !ft._IsUserClass() && !ft._IsString())  // 当前还无法正确处理 String 数据类型的默认值
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
	    " + c.Name + @"(xx::MemPool* mempool);
	    " + c.Name + @"(xx::BBuffer *bb);
		" + c.Name + @"(" + c.Name + @" const&) = delete;
		" + c.Name + @"& operator=(" + c.Name + @" const&) = delete;
        void ToString(xx::String &str) const override;
        void ToStringCore(xx::String &str) const override;
        void ToBBuffer(xx::BBuffer &bb) const override;
        int FromBBuffer(xx::BBuffer &bb) override;
    };");   // class }

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
namespace " + c.Namespace.Replace(".", "::") + @"
{");
            }

            // 定位到基类
            var bt = c.BaseType;
            var btn = c._HasBaseType() ? bt._GetTypeDecl_Cpp(templateName).CutLast() : "xx::Object";

            sb.Append(@"
	inline " + c.Name + @"::" + c.Name + @"(xx::MemPool* mempool)
        : " + btn + @"(mempool)");
            sb.Append(@"
	{");
            var fs = c._GetFields();
            //    foreach (var f in fs)
            //    {
            //        var ft = f.FieldType;
            //        if (ft.IsClass && f._Has<TemplateLibrary.CreateInstance>())
            //        {
            //            sb.Append(@"
            //mempool().CreateTo(" + f.Name + ");");
            //        }
            //    }
            sb.Append(@"
	}
	inline " + c.Name + @"::" + c.Name + @"(xx::BBuffer *bb)
        : " + btn + @"(bb->mempool)");
            //// 其他 _v 成员初始化
            //fs = c._GetFields();
            //foreach (var f in fs)
            //{
            //    var ft = f.FieldType;
            //    if (ft.IsClass)
            //    {
            //        sb.Append(@"
            //" + (isFirst ? ":" : ",") + @" " + f.Name + "(mempool(), bb)");
            //        isFirst = false;
            //    }
            //}
            sb.Append(@"
	{
        if (int rtv = FromBBuffer(*bb)) throw rtv;
	}
    inline void " + c.Name + @"::ToBBuffer(xx::BBuffer &bb) const
    {");

            if (c._HasBaseType())
            {
                sb.Append(@"
        this->BaseType::ToBBuffer(bb);");
            }
            fs = c._GetFields();
            foreach (var f in fs)
            {
                if (f.FieldType._IsExternal() && !f.FieldType._GetExternalSerializable()) continue;
                if (f._Has<TemplateLibrary.NotSerialize>())
                {
                    sb.Append(@"
        bb.WriteDefaultValue<" + f.FieldType._GetSafeTypeDecl_Cpp(templateName) + ">();");
                }
                else if (f._Has<TemplateLibrary.CustomSerialize>())
                {
                    sb.Append(@"
        bb.CustomWrite(bb, (void*)this, _offsetof(ThisType, " + f.Name + "));");
                }
                else
                {
                    sb.Append(@"
        bb.Write(this->" + f.Name + ");");
                }
            }

            sb.Append(@"
    }
    inline int " + c.Name + @"::FromBBuffer(xx::BBuffer &bb)
    {
        int rtv = 0;");
            if (c._HasBaseType())
            {
                sb.Append(@"
        if ((rtv = this->BaseType::FromBBuffer(bb))) return rtv;");
            }
            fs = c._GetFields();
            foreach (var f in fs)
            {
                if (f.FieldType._IsExternal() && !f.FieldType._GetExternalSerializable()) continue;
                if (f.FieldType._IsContainer())
                {
                    sb.Append(@"
        bb.readLengthLimit = " + f._GetLimit() + ";");
                }

                sb.Append(@"
        if ((rtv = bb.Read(this->" + f.Name + @"))) return rtv;");
            }
            sb.Append(@"
        return rtv;
    }

    inline void " + c.Name + @"::ToString(xx::String &str) const
    {
        if (memHeader().flags)
        {
        	str.Append(""[ \""***** recursived *****\"" ]"");
        	return;
        }
        else memHeader().flags = 1;

        str.Append(""{ \""type\"" : \""" + c.Name + @"\"""");
        ToStringCore(str);
        str.Append("" }"");
        
        memHeader().flags = 0;
    }
    inline void " + c.Name + @"::ToStringCore(xx::String &str) const
    {
        this->BaseType::ToStringCore(str);");
            foreach (var f in fs)
            {
                if (f.FieldType._IsExternal() && !f.FieldType._GetExternalSerializable()) continue;
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
    }

");

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
        sb.Append(@"
namespace xx
{");
        cs = ts._GetStructs();
        foreach (var c in cs)
        {
            var ctn = c._GetTypeDecl_Cpp(templateName);
            var fs = c._GetFields();

            sb.Append(@"
	template<>
	struct BytesFunc<" + ctn + @", void>
	{
		static inline void WriteTo(BBuffer& bb, " + ctn + @" const &in)
		{
			bb.Write(");
            foreach (var f in fs)
            {
                if (f._Has<TemplateLibrary.NotSerialize>())
                {
                    // todo: write 默认值
                }
                else
                {
                    sb.Append((f == fs.First() ? "" : @", ") + "in." + f.Name);
                }
            }
            sb.Append(@");
		}
		static inline int ReadFrom(BBuffer& bb, " + ctn + @" &out)
		{
			return bb.Read(");
            foreach (var f in fs)
            {
                sb.Append((f == fs.First() ? "" : @", ") + "out." + f.Name);
            }
            sb.Append(@");
		}
	};
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


        // 遍历所有 type 及成员数据类型 生成  BBuffer.Register< T >( typeId ) 函数组. 0 不能占. String 占掉 1. BBuffer 占掉 2. ( 这两个不生成 )
        // 在基础命名空间中造一个静态类 AllTypes 静态方法 Register

        var typeIds = new TemplateLibrary.TypeIds(asm);
        foreach (var kv in typeIds.types)
        {
            var ct = kv.Key;
            if (ct._IsString() || ct._IsBBuffer() || ct._IsExternal() && !ct._GetExternalSerializable()) continue;
            var typeId = kv.Value;
            string ctn;
            if (ct._IsList()) ctn = ct._GetSafeTypeDecl_Cpp(templateName, true);
            else ctn = ct._GetTypeDecl_Cpp(templateName).CutLast();

            sb.Append(@"
	template<> struct TypeId<" + ctn + @"> { static const uint16_t value = " + typeId + @"; };");
        }


        sb.Append(@"
}
namespace " + templateName + @"
{
	inline void AllTypesRegister()
	{");
        foreach (var kv in typeIds.types)
        {
            var ct = kv.Key;
            if (ct._IsString() || ct._IsBBuffer() || ct._IsExternal() && !ct._GetExternalSerializable()) continue;
            string ctn;
            if (ct._IsList()) ctn = ct._GetSafeTypeDecl_Cpp(templateName, true);
            else ctn = ct._GetTypeDecl_Cpp(templateName).CutLast();
            var bt = ct.BaseType;
            var btn = ct._HasBaseType() ? bt._GetTypeDecl_Cpp(templateName).CutLast() : "xx::Object";

            sb.Append(@"
	    xx::MemPool::Register<" + ctn + @", " + btn + @">();");
        }
        sb.Append(@"
	}
}
");

        // todo: 为 structs 生成序列化 / ToString 的适配

        sb._WriteToFile(Path.Combine(outDir, templateName + "_class.h"));
    }
}
