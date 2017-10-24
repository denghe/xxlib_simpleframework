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
            var en = e._GetTypeDecl_Lua(templateName);
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

        // 遍历所有 type 及成员数据类型 生成  typeId. 0 不能占. string 占掉 1. BBuffer 占掉 2.

        var types = new Dictionary<Type, ushort>();
        types.Add(typeof(string), 1);
        types.Add(typeof(TemplateLibrary.BBuffer), 2);
        ushort typeId = 3;
        var cs = ts._GetClasss();
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


        foreach (var kv in types)
        {
            if (kv.Key == typeof(string) || kv.Key == typeof(TemplateLibrary.BBuffer)) continue;
            var c = kv.Key;
            typeId = (ushort)kv.Value;
            var cn = c._GetTypeDecl_Lua(templateName);
            var o = asm.CreateInstance(c.FullName);

            sb.Append(c._GetDesc()._GetComment_Lua(0) + @"
" + cn + @" = {
    typeName = """ + cn + @""",
    typeId = " + typeId + @",
    Create = function()
        local o = {}
        o.__proto = " + cn + @"
        o.__index = o
        o.__newindex = o
");
            var fs = c._GetFieldsConsts();
            foreach (var f in fs)
            {
                var ft = f.FieldType;
                var v = f.GetValue(f.IsStatic ? null : o);
                var dv = v._GetDefaultValueDecl_Lua(templateName);
                sb.Append(f._GetDesc()._GetComment_Lua(8));
                if (dv != "")
                {
                    sb.Append(@"
        o." + f.Name + @" = " + dv);
                }
                else
                {
                    sb.Append(@"
        o." + f.Name + " = BBuffer.null");
                }
                sb.Append(" -- " + ft._GetTypeDecl_Lua(templateName));
            }

            if (c._HasBaseType())
            {
                var bt = c.BaseType._GetTypeDecl_Lua(templateName);
                sb.Append(@"
        setmetatable( o, " + bt + @".Create() )");
            }
            sb.Append(@"
        return o
    end,
    FromBBuffer = function( bb, o )");
            if (c._HasBaseType())
            {
                var bt = c.BaseType._GetTypeDecl_Lua(templateName);
                sb.Append(@"
        local p = getmetatable( o )
        rawget( p, ""__proto"" ).FromBBuffer( bb, p )");
            }
            foreach (var f in fs)
            {
                var ft = f.FieldType;
                var ftn = ft._IsNumeric() ? ft.Name : "Object";
                sb.Append(@"
        rawset( o, """ + f.Name + @""", bb:Read" + ftn + @"() )");
            }
            if (c._IsList())
            {
                var fn = "ReadObject";
                var ct = c.GenericTypeArguments[0];
                if (!ct._IsUserClass())
                {
                    fn = "Read" + ct.Name;
                }
                sb.Append(@"
		local len = bb:ReadUInt32()
		for i = 1, len do
			table.insert( o, bb:" + fn + @"() )
		end");
            }
            sb.Append(@"
    end,
    ToBBuffer = function( bb, o )");
            if (c._HasBaseType())
            {
                var bt = c.BaseType._GetTypeDecl_Lua(templateName);
                sb.Append(@"
        local p = getmetatable( o )
        rawget( p, ""__proto"" ).ToBBuffer( bb, p )");
            }
            foreach (var f in fs)
            {
                var ft = f.FieldType;
                var ftn = ft._IsNumeric() ? ft.Name : "Object";
                sb.Append(@"
        bb:Write" + ftn + @"( rawget( o, """ + f.Name + @""" ) )");
            }
            if (c._IsList())
            {
                var fn = "WriteObject";
                var ct = c.GenericTypeArguments[0];
                if (!ct._IsUserClass())
                {
                    fn = "Write" + ct.Name;
                }
                sb.Append(@"
		bb:WriteUInt32( #o )
		for k, v in ipairs( o ) do
			bb:" + fn + @"( v )
		end");
            }
            sb.Append(@"
    end
}
BBuffer.Register( " + cn + @" )");
        }

        sb._WriteToFile(Path.Combine(outDir, templateName + "_class.lua"));
    }
}
