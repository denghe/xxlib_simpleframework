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
        sb.Append(@"#pragma once
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
            if (c.Namespace != null && (i == 0 || (i > 0 && ss[i - 1].Namespace != c.Namespace))) // namespace 去重
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
                if (dv != "" && !ft._IsList() && !ft._IsUserClass() && !ft._IsString() && !ft._IsObject())  // 当前还无法正确处理 String 数据类型的默认值
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
            if (c.Namespace != null && ((i < ss.Count - 1 && ss[i + 1].Namespace != c.Namespace) || i == ss.Count - 1))
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
            var btn = c._HasBaseType() ? bt._GetSafeTypeDecl_Cpp(templateName) : "xx::Object";

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

                if (ft._IsExternal() && !ft._GetExternalSerializable() && !string.IsNullOrEmpty(ft._GetExternalCppDefaultValue()))
                {
                    sb.Append(" = " + ft._GetExternalCppDefaultValue() + ";");
                }
                else
                {
                    var v = f.GetValue(f.IsStatic ? null : o);
                    var dv = v._GetDefaultValueDecl_Cpp(templateName);
                    if (dv != "" && !ft._IsList() && !(ft._IsUserClass()) && !ft._IsString() && !ft._IsNullable() && !ft._IsObject())  // 当前还无法正确处理 String 数据类型的默认值
                    {
                        sb.Append(" = " + dv + ";");
                    }
                    else
                    {
                        sb.Append(";");
                    }
                }
            }

            sb.Append(@"

        typedef " + c.Name + @" ThisType;
        typedef " + btn + @" BaseType;
	    " + c.Name + @"(xx::MemPool* const& mempool) noexcept;
	    " + c.Name + @"(xx::BBuffer* const& bb);
		" + c.Name + @"(" + c.Name + @" const&) = delete;
		" + c.Name + @"& operator=(" + c.Name + @" const&) = delete;
        void ToString(xx::String& s) const noexcept override;
        void ToStringCore(xx::String& s) const noexcept override;
        void ToBBuffer(xx::BBuffer& bb) const noexcept override;
        int FromBBuffer(xx::BBuffer& bb) noexcept override;
        int FromBBufferCore(xx::BBuffer& bb) noexcept;
        void CopyTo(" + c.Name + @"* const& o) const noexcept;
        " + c.Name + @"* MakeCopy() const noexcept;
        " + c.Name + @"_p MakePtrCopy() const noexcept;
        inline static xx::Ptr<ThisType> defaultInstance;
    };");   // class }

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
            var ctn = c._GetSafeTypeDecl_Cpp(templateName);
            var fs = c._GetFields();

            sb.Append(@"
	template<>
	struct BytesFunc<" + ctn + @", void>
	{
		static inline void WriteTo(BBuffer& bb, " + ctn + @" const& in) noexcept
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
		static inline int ReadFrom(BBuffer& bb, " + ctn + @"& out) noexcept
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
		static inline void WriteTo(xx::String& s, " + ctn + @" const& in) noexcept
		{
			s.Append(""{ \""structTypeName\"":\""" + (string.IsNullOrEmpty(c.Namespace) ? c.Name : c.Namespace + "." + c.Name) + @"\""""");
            foreach (var f in fs)
            {
                sb.Append(@", "", \""" + f.Name + @"\"":"", in." + f.Name);
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
            var ctn = ct._GetSafeTypeDecl_Cpp(templateName);

            sb.Append(@"
	template<> struct TypeId<" + ctn + @"> { static const uint16_t value = " + typeId + @"; };");
        }

        sb.Append(@"
}
namespace " + templateName + @"
{");

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
            var btn = c._HasBaseType() ? bt._GetSafeTypeDecl_Cpp(templateName) : "xx::Object";
            var fs = c._GetFields();

            sb.Append(@"
	inline " + c.Name + @"::" + c.Name + @"(xx::MemPool* const& mempool) noexcept
        : " + btn + @"(mempool)");
            sb.Append(@"
	{
	}
	inline " + c.Name + @"::" + c.Name + @"(xx::BBuffer* const& bb)
        : " + btn + @"(bb)
	{
        if (int r = FromBBufferCore(*bb)) throw r;
	}
    inline void " + c.Name + @"::ToBBuffer(xx::BBuffer& bb) const noexcept
    {");

            if (c._HasBaseType())
            {
                sb.Append(@"
        this->BaseType::ToBBuffer(bb);");
            }
            fs = c._GetFields();
            foreach (var f in fs)
            {
                var ft = f.FieldType;
                if (ft._IsExternal() && !ft._GetExternalSerializable()) continue;
                if (f._Has<TemplateLibrary.NotSerialize>())
                {
                    sb.Append(@"
        bb.WriteDefaultValue<" + ft._GetSafeTypeDecl_Cpp(templateName) + ">();");
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
    inline int " + c.Name + @"::FromBBuffer(xx::BBuffer& bb) noexcept
    {");
            if (c._HasBaseType())
            {
                sb.Append(@"
        if (int r = this->BaseType::FromBBuffer(bb)) return r;");
            }
            sb.Append(@"
        return this->FromBBufferCore(bb);
    }
    inline int " + c.Name + @"::FromBBufferCore(xx::BBuffer& bb) noexcept
    {");
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
        if (int r = bb.Read(this->" + f.Name + @")) return r;");
            }
            sb.Append(@"
        return 0;
    }

    inline void " + c.Name + @"::ToString(xx::String& s) const noexcept
    {
        if (this->memHeader().flags)
        {
        	s.Append(""[ \""***** recursived *****\"" ]"");
        	return;
        }
        else this->memHeader().flags = 1;

        s.Append(""{ \""pkgTypeName\"":\""" + (string.IsNullOrEmpty(c.Namespace) ? c.Name : c.Namespace + "." + c.Name) + @"\"", \""pkgTypeId\"":"", xx::TypeId_v<ThisType>);
        ToStringCore(s);
        s.Append("" }"");
        
        this->memHeader().flags = 0;
    }
    inline void " + c.Name + @"::ToStringCore(xx::String& s) const noexcept
    {
        this->BaseType::ToStringCore(s);");
            foreach (var f in fs)
            {
                if (f.FieldType._IsExternal() && !f.FieldType._GetExternalSerializable()) continue;
                if (f.FieldType._IsString())
                {
                    sb.Append(@"
        if (this->" + f.Name + @") s.Append("", \""" + f.Name + @"\"":\"""", this->" + f.Name + @", ""\"""");
        else s.Append("", \""" + f.Name + @"\"":nil"");");
                }
                else
                {
                    sb.Append(@"
        s.Append("", \""" + f.Name + @"\"":"", this->" + f.Name + @");");
                }
            }
            sb.Append(@"
    }
    inline void " + c.Name + @"::CopyTo(" + c.Name + @"* const& o) const noexcept
    {");
            if (c._HasBaseType())
            {
                sb.Append(@"
        this->BaseType::CopyTo(o);");
            }
            foreach (var f in fs)
            {
                sb.Append(@"
        o->" + f.Name + @" = this->" + f.Name + @";");
            }
            sb.Append(@"
    }
    inline " + c.Name + @"* " + c.Name + @"::MakeCopy() const noexcept
    {
        auto o = mempool->MPCreate<" + c.Name + @">();
        this->CopyTo(o);
        return o;
    }
    inline " + c.Name + @"_p " + c.Name + @"::MakePtrCopy() const noexcept
    {
        return " + c.Name + @"_p(this->MakeCopy());
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


        sb.Append(@"
namespace " + templateName + @"
{
	inline void AllTypesRegister() noexcept
	{
        xx::MemPool::RegisterInternals();");
        foreach (var kv in typeIds.types)
        {
            var ct = kv.Key;
            if (ct._IsString() || ct._IsBBuffer() || ct._IsExternal() && !ct._GetExternalSerializable()) continue;
            var ctn = ct._GetSafeTypeDecl_Cpp(templateName);
            var bt = ct.BaseType;
            var btn = ct._HasBaseType() ? bt._GetSafeTypeDecl_Cpp(templateName) : "xx::Object";

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
