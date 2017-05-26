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
        sb.Append(@"#include <xx_mempool.h>
#include <xx_bbuffer.h>
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

            // desc
            // enum class xxxxxxxxx : underlyingType
            sb.Append(c._GetDesc_Cpp(4) + @"
    struct " + c.Name + @";");

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
            sb.Append(e._GetDesc_Cpp(4) + @"
    enum class " + e.Name + @" : " + e._GetEnumUnderlyingTypeName_Cpp() + @"
    {");

            // desc
            // xxxxxx = val
            var fs = e._GetEnumFields();
            foreach (var f in fs)
            {
                sb.Append(f._GetDesc_Cpp(8) + @"
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

        var ss = ts._GetStructs();
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
            sb.Append(c._GetDesc_Cpp(4) + @"
    struct " + c.Name + @"
    {");

            // desc
            // T xxxxxx = val
            // consts( static ) / fields
            var fs = c._GetFieldsConsts();
            foreach (var f in fs)
            {
                var ft = f.FieldType;
                var ftn = ft._GetTypeDecl_Cpp(templateName);
                sb.Append(f._GetDesc_Cpp(8) + @"
        " + (f.IsStatic ? "constexpr " : "") + ftn + " " + f.Name);

                var v = f.GetValue(f.IsStatic ? null : o);
                var dv = v._GetDefaultValueDecl_Cpp(templateName);
                if (dv != "")
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



        cs = ts._GetClasss();
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
            var btn = c._HasBaseType() ? bt._GetTypeDecl_Cpp(templateName).CutLast() : "xx::MPObject";

            // desc
            // T xxxxxxxxx = defaultValue
            // constexpr T xxxxxxxxx = defaultValue

            sb.Append(c._GetDesc_Cpp(4) + @"
    struct " + c.Name + @" : " + btn + @"
    {
        typedef " + btn + " BaseType;");

            // consts( static ) / fields
            var fs = c._GetFieldsConsts();
            foreach (var f in fs)
            {
                var ft = f.FieldType;
                var ftn = ft._GetTypeDecl_Cpp(templateName);
                sb.Append(f._GetDesc_Cpp(8) + @"
        " + (f.IsStatic ? "constexpr " : "") + ftn + " " + f.Name);

                var v = f.GetValue(f.IsStatic ? null : o);
                var dv = v._GetDefaultValueDecl_Cpp(templateName);
                if (dv != "")
                {
                    sb.Append(" = " + dv + ";");
                }
                else
                {
                    sb.Append(";");
                }
            }

            sb.Append(@"
#pragma region ctor, interface impls

	    " + c.Name + @"()");
            var isFirst = true;
            if (c._HasBaseType())
            {
                sb.Append(@"
            : BaseType()");
                isFirst = false;
            }
            //// 其他 _v 成员初始化
            //fs = c._GetFields();
            //foreach (var f in fs)
            //{
            //    var ft = f.FieldType;
            //    if (ft.IsClass)
            //    {
            //        sb.Append(@"
            //" + (isFirst ? ":" : ",") + @" " + f.Name + "(mempool())");
            //        isFirst = false;
            //    }
            //}
            sb.Append(@"
	    {");
            fs = c._GetFields();
            foreach (var f in fs)
            {
                var ft = f.FieldType;
                if (ft.IsClass && f._Has<TemplateLibrary.CreateInstance>()) // todo: 跳过 _v
                {
                    sb.Append(@"
            mempool().CreateTo(" + f.Name + ");");
                }
            }
            sb.Append(@"
	    }
	    " + c.Name + @"(xx::BBuffer *bb)");
            isFirst = true;
            if (c._HasBaseType())
            {
                sb.Append(@"
            : BaseType(bb)");
                isFirst = false;
            }
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
	    {");
            fs = c._GetFields();
            foreach (var f in fs)
            {
                var ft = f.FieldType;
                if (ft.IsClass)   // todo: 跳过 _v
                {
                    sb.Append(@"
            if (auto rtv = bb->Read(" + f.Name + ")) throw rtv;");
                }
            }
            sb.Append(@"
	    }
	    ~" + c.Name + @"()
	    {");
            fs = c._GetFields();
            foreach (var f in fs)
            {
                var ft = f.FieldType;
                if (ft.IsClass)
                {
                    sb.Append(@"
            mempool().SafeRelease(" + f.Name + ");");
                }
            }
            sb.Append(@"
	    }

        inline virtual void ToString(xx::String &str) const override
        {
        	if (tsFlags())
        	{
        		str.Append(""[ \""recursived\"" ]"");
        		return;
        	}
        	else tsFlags() = 1;

            str.Append(""{ \""type\"" : \""" + c.Name + @"\"""");
            ToStringCore(str);
            str.Append("" }"");
        
        	tsFlags() = 0;
        }
        inline virtual void ToStringCore(xx::String &str) const override
        {
            this->BaseType::ToStringCore(str);");
            foreach (var f in fs)
            {
                sb.Append(@"
            str.Append("", \""" + f.Name + @"\"" : "", this->" + f.Name + @");");
            }
            sb.Append(@"
        }


        inline virtual void ToBBuffer(xx::BBuffer &bb) const override
        {");

            if (c._HasBaseType())
            {
                sb.Append(@"
            this->BaseType::ToBBuffer(bb);");
            }
            fs = c._GetFields();
            foreach (var f in fs)
            {
                sb.Append(@"
            bb.Write(this->" + f.Name + ");");
            }

            sb.Append(@"
        }

        inline virtual int FromBBuffer(xx::BBuffer &bb) override
        {");
            if (c._HasBaseType())
            {
                sb.Append(@"
            this->BaseType::FromBBuffer(bb);");
            }
            fs = c._GetFields();
            foreach (var f in fs)
            {
                sb.Append(@"
            bb.Read(this->" + f.Name + ");");
            }
            sb.Append(@"
            return 0;
        }
");

            // class /
            sb.Append(@"
#pragma endregion
    };");

            // namespace }
            if (c.Namespace != null && ((i < cs.Count - 1 && cs[i + 1].Namespace != c.Namespace) || i == cs.Count - 1))
            {
                sb.Append(@"
}");
            }

        }

        sb.Append(@"
}");

        // 遍历所有 type 及成员数据类型 生成  BBuffer.Register< T >( typeId ) 函数组. 0 不能占. BBuffer 占掉 1.
        // 在基础命名空间中造一个静态类 AllTypes 静态方法 Register

        // todo: 似乎需要统计引用次数来倒排( cpp 需要 )

        var types = new Dictionary<Type, ushort>();
        ushort typeId = 2;
        foreach (var c in cs)
        {
            if (types.ContainsKey(c)) continue;
            types.Add(c, typeId++);

            if (c._HasBaseType())
            {
                var bt = c.BaseType;
                if (!types.ContainsKey(bt)) types.Add(bt, typeId++);
            }

            var fs = c._GetFields();
            foreach (var f in fs)
            {
                var ft = f.FieldType;
                if (ft._IsUserClass() || ft.IsGenericType)
                {
                    if (types.ContainsKey(ft)) continue;
                    types.Add(ft, typeId++);
                }
            }
        }

        sb.Append(@"
namespace xx
{");
        cs = ts._GetStructs();
        foreach (var c in cs)
        {
            var ctn = c._GetTypeDecl_Cpp(templateName);
            var fs = c._GetFields();

            sb.Append(c._GetDesc_Cpp(4) + @"
	template<>
	struct BytesFunc<" + ctn + @", void>
	{
		static inline uint32_t Calc(" + ctn + @" const &in)
		{
			return BBCalc(");
            foreach (var f in fs)
            {
                sb.Append((f == fs[0] ? "" : ", ") + "in." + f.Name);
            }
            sb.Append(@");
		}
		static inline uint32_t WriteTo(char *dstBuf, " + ctn + @" const &in)
		{
			return BBWriteTo(dstBuf");
            foreach (var f in fs)
            {
                sb.Append(@", " + "in." + f.Name);
            }
            sb.Append(@");
		}
		static inline int ReadFrom(char const *srcBuf, uint32_t const &dataLen, uint32_t &offset, " + ctn + @" &out)
		{
			return BBReadFrom(srcBuf, dataLen, offset");
            foreach (var f in fs)
            {
                sb.Append(@", " + "out." + f.Name);
            }
            sb.Append(@");
		}
	};
	template<>
	struct StrFunc<" + ctn + @", void>
	{
		static inline uint32_t Calc(" + ctn + @" const &in)
		{
			return StrCalc(");
            foreach (var f in fs)
            {
                sb.Append((f == fs[0] ? "" : ", ") + "in." + f.Name);
            }
            // todo: 这里先简单粗略的计算一下 json 格式消耗附加值
            sb.Append(@" + " + fs.Count * 5 + 10 + @");
		}
		static inline uint32_t WriteTo(char *dstBuf, " + ctn + @" const &in)
		{
			return StrWriteTo(dstBuf, ""{ \""type\"" : \""" + c.Name + @"\""""");
            foreach (var f in fs)
            {
                sb.Append(@", "", \""" + f.Name + @"\"" : "", in." + f.Name);
            }
            sb.Append(@", "" }"");
        }
    };");

        }

        foreach (var kv in types)
        {
            var ct = kv.Key;
            var ctn = ct._GetTypeDecl_Cpp(templateName).CutLast();
            typeId = kv.Value;

            sb.Append(@"
	template<> struct TypeId<" + ctn + @"> { static const uint16_t value = " + typeId + @"; };");
        }
        sb.Append(@"
}
namespace " + templateName + @"
{
	inline void RegisterTypes()
	{");
        foreach (var kv in types)
        {
            var ct = kv.Key;
            var ctn = ct._GetTypeDecl_Cpp(templateName).CutLast();
            var bt = ct.BaseType;
            var btn = ct._HasBaseType() ? bt._GetTypeDecl_Cpp(templateName).CutLast() : "xx::MPObject";

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
