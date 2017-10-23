using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Reflection;
using System.Text;


public static class GenLUA_Class
{
    public static void Gen(Assembly asm, string outDir, string templateName)
    {
        var sb = new StringBuilder();

        var ts = asm._GetTypes();
        var es = ts._GetEnums();
        for (int i = 0; i < es.Count; ++i)
        {
            var e = es[i];
            var en = e.FullName.Replace(".", "_");
            sb.Append(e._GetDesc()._GetComment_Lua(0) + @"
" + en + @" = {");

            var fs = e._GetEnumFields();
            foreach (var f in fs)
            {
                sb.Append(f._GetDesc()._GetComment_Lua(4) + @"
    " + f.Name + " = " + f._GetEnumValue(e) + ",");
            }
            sb.Length--;

            // enum /
            sb.Append(@"
}");
        }


        var cs = ts._GetClasssStructs();
        for (int i = 0; i < cs.Count; ++i)
        {
            var c = cs[i];
            var o = asm.CreateInstance(c.FullName);

            if (c.IsValueType)
            {
                sb.Append(c._GetDesc()._GetComment_Lua(4) + @"
    public partial struct " + c.Name + @" : IBBuffer
    {");
            }
            else
            {
                sb.Append(c._GetDesc()._GetComment_Lua(4) + @"
    public partial class " + c.Name + @" : " + (c._HasBaseType() ? c.BaseType._GetTypeDecl_Csharp() : "IBBuffer") + @"
    {");
            }

            // consts( static ) / fields
            var fs = c._GetFieldsConsts();
            foreach (var f in fs)
            {
                var ft = f.FieldType;
                var ftn = ft._GetTypeDecl_Csharp();
                sb.Append(f._GetDesc()._GetComment_Lua(8) + @"
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

        }

        // 遍历所有 type 及成员数据类型 生成  BBuffer.Register< T >( typeId ) 函数组. 0 不能占. string 占掉 1. BBuffer 占掉 2.
        // 在基础命名空间中造一个静态类 AllTypes 静态方法 Register

        var types = new Dictionary<Type, ushort>();
        types.Add(typeof(string), 1);
        types.Add(typeof(TemplateLibrary.BBuffer), 2);
        ushort typeId = 3;
        cs = ts._GetClasss();
        foreach (var c in cs)
        {
            if (!types.ContainsKey(c)) types.Add(c, typeId++);

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
                    if (!types.ContainsKey(ft)) types.Add(ft, typeId++);
                }
            }
        }

        sb.Append(@"
    public static class AllTypes
    {
        public static void Register()
        {
            // BBuffer.Register<string>(1);
            BBuffer.Register<BBuffer>(2);");

        foreach (var kv in types)
        {
            if (kv.Key == typeof(string) || kv.Key == typeof(TemplateLibrary.BBuffer)) continue;
            var ct = kv.Key;
            var ctn = ct._GetTypeDecl_Cpp(templateName).CutLast();
            typeId = (ushort)kv.Value;

            sb.Append(@"
            BBuffer.Register<" + ct._GetTypeDecl_Csharp() + @">(" + typeId++ + ");");
        }

        sb.Append(@"

            BBuffer.handlers = new Action<IBBuffer>[" + typeId + @"];
        }
    }");

        // template namespace /
        sb.Append(@"
}
");


        sb._WriteToFile(Path.Combine(outDir, templateName + "_class.lua"));
    }
}
