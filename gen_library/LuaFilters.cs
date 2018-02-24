using System;
using System.Collections.Generic;
using System.Linq;
using System.Reflection;
using System.Text;
using System.Threading.Tasks;

namespace TemplateLibrary
{
    // 白名单
    public class LuaFilters
    {
        HashSet<Type> depends = new HashSet<Type>();
        System.Collections.Generic.List<Type> ts;

        public LuaFilters(Assembly asm)
        {
            // 扫出命名空间白名单
            var nss = new HashSet<string>();
            ts = asm._GetTypes();
            var ifaces = ts._GetInterfaces<TemplateLibrary.LuaFilter>();
            foreach (var iface in ifaces)
            {
                var lfs = iface.GetCustomAttributes<TemplateLibrary.LuaFilter>();
                foreach (var lf in lfs)
                {
                    nss.Add(lf.value);
                }
            }
            if (nss.Count == 0) return;

            // 记录　命名空间白名单　下所有类
            foreach (var t in ts)
            {
                if (nss.Contains(t.Namespace)) depends.Add(t);
            }

            // 不断扫基类和属性依赖直到扫不出来新 type 为止
            int n = 0;
            do
            {
                n = depends.Count;
                AddBases();
                AddFields();
            }
            while (depends.Count > n);
        }

        // 补充记录　所有基类, 以及基类的基类....
        void AddBases()
        {
            foreach (var t in ts)
            {
                if (depends.Contains(t))
                {
                    if (t._HasBaseType())
                        AddBaseType(t.BaseType);
                    if (t._IsList())
                        AddBaseType(t.GenericTypeArguments[0]);
                }
            }
        }
        void AddBaseType(Type t)
        {
            if (!depends.Add(t))
                return;
            if (t._HasBaseType())
                AddBaseType(t.BaseType);
            if (t._IsList())
                AddBaseType(t.GenericTypeArguments[0]);
        }



        // 补充记录　所有 field 相关类, 以及这些类的 field 相关类...
        void AddFields()
        {
            foreach (var t in ts)
            {
                if (depends.Contains(t))
                {
                    if (t._IsList()) AddFields(t.GenericTypeArguments[0]);
                    else
                    {
                        var fs = t._GetFieldsConsts();
                        foreach (var f in fs)
                        {
                            AddFields(f.FieldType);
                        }
                    }
                }
            }
        }

        void AddFields(Type t)
        {
            if (!depends.Add(t)) return;
            if (t._IsList()) AddFields(t.GenericTypeArguments[0]);
            else
            {
                var fs = t._GetFieldsConsts();
                foreach (var f in fs)
                {
                    AddFields(f.FieldType);
                }
            }
        }


        // 判断一个类型是否位于白名单之中( 为简化起见，　系统原生类型也算位于白名单之中 )
        public bool Contains(Type t)
        {
            if (depends.Count == 0) return true;
            if (t.Namespace == nameof(System)) return true;
            return depends.Contains(t);
        }
    }
}
