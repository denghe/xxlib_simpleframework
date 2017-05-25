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
            sb.Append(e._GetDesc(4) + @"
    enum class " + e.Name + @" : " + e._GetEnumUnderlyingTypeName_Cpp() + @"
    {");

            // desc
            // xxxxxx = val
            var fs = e._GetEnumFields();
            foreach (var f in fs)
            {
                sb.Append(f._GetDesc(8) + @"
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

        // todo: 生成 预声明 + 声明 + 实现 分离的三部分

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

            // 定位到基类
            var bt = c.BaseType;
            var btn = c._HasBaseType() ? bt._GetTypeDecl_Cpp(templateName).CutLast() : "xx::MPObject";      // todo: C++ 版

            // desc
            // public T xxxxxxxxx = defaultValue
            // public const T xxxxxxxxx = defaultValue

            sb.Append(c._GetDesc(4) + @"
    struct " + c.Name + @" : " + btn + @"
    {
        typedef " + btn + " BaseType;");

            // consts( static ) / fields
            var fs = c._GetFieldsConsts();
            foreach (var f in fs)
            {
                var ft = f.FieldType;
                var ftn = ft._GetTypeDecl_Cpp(templateName);      // todo: C++ 版
                sb.Append(f._GetDesc(8) + @"
        " + (f.IsStatic ? "static const " : "") + ftn + " " + f.Name);  // todo: 生成函数包装

                var v = f.GetValue(f.IsStatic ? null : o);
                var dv = v._GetCSharpDefaultValueDecl();    // todo: C++ 版
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
            //    if (ft.IsClass && ft._IsString())
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
                if (ft.IsClass) // todo: 跳过 _v
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
            //    if (ft.IsClass && ft._IsString())
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
                if (ft.IsClass && !ft._IsString())
                {
                    sb.Append(@"
            mempool().SafeRelease(" + f.Name + ");");
                }
            }
            sb.Append(@"
	    }

        // todo: inline virtual void ToString(xx::String &str) const override

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
            var btn = ct._HasBaseType() ? bt._GetTypeDecl_Cpp(templateName).CutLast() : "xx::MPObject";      // todo: C++ 版

            sb.Append(@"
	    xx::MemPool::Register<" + ctn + @", " + btn + @">();");
        }
        sb.Append(@"
	}
}
");

        sb._WriteToFile(Path.Combine(outDir, templateName + "_class.h"));
    }
}
