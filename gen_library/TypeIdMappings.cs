using System;
using System.Collections.Generic;
using System.Linq;
using System.Reflection;
using System.Text;
using System.Threading.Tasks;

namespace TemplateLibrary
{
    public class TypeIds
    {
        /// <summary>
        /// 存的是 type id 映射接口中填的值
        /// </summary>
        public Dictionary<Type, ushort> dict = new Dictionary<Type, ushort>();

        /// <summary>
        /// 
        /// </summary>
        public ushort maxId = 0;

        /// <summary>
        /// 是否存在映射接口
        /// </summary>
        public bool typeIdMappingsExists = false;

        /// <summary>
        /// 映射接口并未完整的包含所有数据类型
        /// </summary>
        public bool hasNewMappings = false;

        /// <summary>
        /// 存的是从所有类中扫出来的相关 type
        /// </summary>
        public Dictionary<Type, ushort> types = new Dictionary<Type, ushort>();

        public TypeIds(Assembly asm)
        {
            dict.Add(typeof(string), 1);
            dict.Add(typeof(BBuffer), 2);
            maxId = 2;

            // 读接口中配置的映射id
            var ts = asm._GetTypes();
            var ifaces = ts._GetInterfaces<TemplateLibrary.TypeIdMappings>();
            foreach (var iface in ifaces)
            {
                var ps = iface._GetProperties();
                foreach (var p in ps)
                {
                    typeIdMappingsExists = true;
                    var t = p.PropertyType;
                    var id = ushort.Parse(p.Name.Substring(1));
                    if (dict.ContainsValue(id))
                    {
                        throw new Exception("typeid 映射接口中的 typeid: " + id + " 已存在.");
                    }
                    if (dict.ContainsKey(t))
                    {
                        throw new Exception("typeid 映射接口中的 type: " + t + " 已存在.");
                    }
                    dict.Add(t, id);
                    if (id > maxId) maxId = id;
                }
            }

            // 遍历实际的类型, 逐个映射id
            types.Add(typeof(string), 1);
            types.Add(typeof(BBuffer), 2);
            var cs = ts._GetClasss();
            foreach (var c in cs)
            {
                if (!types.ContainsKey(c)) types.Add(c, GetTypeId(c));

                if (c._HasBaseType())
                {
                    var bt = c.BaseType;
                    if (!types.ContainsKey(bt)) types.Add(bt, GetTypeId(bt));
                }

                var fs = c._GetFields();
                foreach (var f in fs)
                {
                    var ft = f.FieldType;
                    if (ft._IsUserClass() || ft.IsGenericType)
                    {
                        if (!types.ContainsKey(ft)) types.Add(ft, GetTypeId(ft));
                    }
                }
            }
        }

        public ushort GetTypeId(Type t)
        {
            if (dict.ContainsKey(t)) return dict[t];
            hasNewMappings = true;
            ++maxId;
            dict.Add(t, maxId);
            return maxId;
        }
    }
}
