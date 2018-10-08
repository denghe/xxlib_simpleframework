using System;
using System.Diagnostics;
using System.IO;
using System.Linq;
using System.Reflection;
using System.Runtime.InteropServices;
using System.Text;
using TemplateLibrary;

public static class GenCS_MultiKeyDict
{
    public static void Gen(Assembly asm, string outDir, string templateName)
    {
        var sb = new StringBuilder();
        sb.Append(@"using System;
using System.Collections.Generic;

namespace " + templateName + @"
{");

        var cs = asm._GetTypes<MultiKeyDict>();
        for (int i = 0; i < cs.Count; ++i)
        {
            var c = cs[i];
            var cn = c.Name._ToFirstUpper();
            var fs = c._GetFields();
            var keys = c._GetFields<Key>();
            Debug.Assert(keys != null && keys.Count > 0);
            var pk = keys.FirstOrDefault(k => k._GetKey() != null && k._GetKey() == true);
            if (pk == null)
            {
                pk = keys[0];
            }
            var pkn = pk.Name;
            var pkt = pk.FieldType;
            var pktn = pkt._GetTypeDecl_Csharp();

            // namespace c_ns {
            if (c.Namespace != null && (i == 0 || (i > 0 && cs[i - 1].Namespace != c.Namespace))) // namespace 去重
            {
                sb.Append(@"
namespace " + c.Namespace + @"
{");
            }


            sb.Append(@"
    public partial class " + c.Name + @"
    {
        public " + cn + @"_MultiKeyDict __owner;
");
            foreach (var f in keys)
            {
                var fn = f.Name;
                var ftn = f.FieldType._GetTypeDecl_Csharp();
                sb.Append(@"
        public int __index_" + fn + @";
        public bool MK_Set" + fn._ToFirstUpper() + @"(" + ftn + @" val)
        {
            if (" + fn + @" == val) return true;
            if (__owner.");
                if (f != keys[0])
                {
                    sb.Append("__dict_" + fn + ".");
                }
                sb.Append(@"ModifyKeyAt(__index_" + fn + @", val))
            {
                " + fn + @" = val;
                return true;
            }
            return false;
        }
");
            }
            sb.Append(@"
    }

    public class " + cn + @"_MultiKeyDict : xx.Dict<" + pktn + @", " + cn + @">
    {
");
            foreach (var f in keys)
            {
                var fn = f.Name;
                var ftn = f.FieldType._GetTypeDecl_Csharp();
                var dict = "__dict_" + fn;

                if (f == pk)                                // 首字段位于 xx.Dict 类本身
                {
                    dict = "this";
                }
                else
                {
                    sb.Append(@"
        public xx.Dict<" + ftn + @", int> " + dict + @" = new xx.Dict<" + ftn + @", int>();");
                }
                sb.Append(@"
        public int MK_FindBy" + fn._ToFirstUpper() + @"(" + ftn + @" key)
        {
            return " + dict + @".Find(key);
        }
        public " + cn + @" MK_GetBy" + fn._ToFirstUpper() + @"(" + ftn + @" key)
        {
            var idx = " + dict + @".Find(key);
            if (idx != -1)
            {
                return this.ValueAt(idx);
            }
            return default(" + cn + @");
        }
        public List<" + cn + @"> MK_GetBy" + fn._ToFirstUpper() + @"s(params " + ftn + @"[] keys)
        {
            if (keys == null) return null;
            var rtv = new List<" + cn + @">();
            var keysLength = keys.Length;
            for (int i = 0; i < keysLength; ++i)
            {
                var idx = " + dict + @".Find(keys[i]);
                if (idx != -1)
                {
                    rtv.Add(this.ValueAt(idx));
                }
            }
            return rtv;
        }
        public bool MK_ExistsBy" + fn._ToFirstUpper() + @"s(params " + ftn + @"[] keys)
        {
            if (keys == null) return false;
            var keysLength = keys.Length;
            for (int i = 0; i < keysLength; ++i)
            {
                if (" + dict + @".Find(keys[i]) != -1) return true;
            }
            return false;
        }
");
            }

            sb.Append(@"

        List<Action> rollbacks = new List<Action>();
        public bool MK_Add(" + cn + @" o)
        {
            var rtv = Add(o." + pkn + @", o);
            if (!rtv.success) return false;
            o.__index_" + pkn + @" = rtv.index;
            rollbacks.Add(() => { RemoveAt(o.__index_" + pkn + @"); });

");
            foreach (var f in keys)
            {
                if (f == keys[0]) continue;

                var fn = f.Name._ToFirstLower();             // field name 首字小写
                var ptn = f.FieldType._GetTypeDecl_Csharp();

                sb.Append(@"
            rtv = __dict_" + fn + @".Add(o." + fn + @", rtv.index);
            if (!rtv.success) goto LabRollback;
            o.__index_" + fn + @" = rtv.index;");

                if (f != keys._Last())
                {
                    sb.Append(@"
            rollbacks.Add(() => { __dict_" + fn + @".RemoveAt(o.__index_" + fn + @"); });
");
                }
            }
            sb.Append(@"

            rollbacks.Clear();
            o.__owner = this;
            return true;

            LabRollback:
            foreach (var a in rollbacks) { a(); }
            rollbacks.Clear();
            return false;
        }

        public bool MK_AddRange(IEnumerable<" + cn + @"> os)
        {
            foreach (var o in os)
            {
                if (!MK_Add(o)) return false;
            }
            return true;
        }

        public void MK_Remove(" + cn + @" o)
        {
            System.Diagnostics.Debug.Assert(o.__owner != null);");
            foreach (var f in keys)
            {
                var fn = f.Name._ToFirstLower();             // field name 首字小写
                sb.Append(@"
            " + (f == keys[0] ? "" : ("__dict_" + fn + @".")) + @"RemoveAt(o.__index_" + fn + @");");
            }
            sb.Append(@"
        }

        public void MK_ForEach(Func<" + cn + @", bool> handler)
        {
            for (int i = 0; i < count; ++i)
            {
                if (items[i].prev != -2 && !handler(items[i].value)) break;
            }
        }
        public void MK_ForEach(Action<" + cn + @"> handler)
        {
            for (int i = 0; i < count; ++i)
            {
                if (items[i].prev != -2)
                {
                    handler(items[i].value);
                }
            }
        }
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
        sb._WriteToFile(Path.Combine(outDir, templateName + "_mkdict.cs"));
    }

}
