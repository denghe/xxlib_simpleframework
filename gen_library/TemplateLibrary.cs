
// 下面是包序列化相关备注

/********************************************************/
// 类型
/********************************************************/

// enum                                     // 枚举
// class / struct                           // 自定义数据类型

/********************************************************/
// 标记
/********************************************************/

// Title( "字段名" )                        // 用于导出为 excel 时的列名生成
// Desc( "注释" )                           // 用于类 及类成员
// Limit(最大长)                            // 用于 string, bbuffer, List 容器等还原时的长度安全限制
// External()                               // 对标记为这个的 枚举或结构体，属于外部引用，不再填充或生成. 其本身所处命名空间与实际对应, 不受 模板大命名空间影响
// NaN, Infinity                            // 标记当反列化 浮点 时, 如果值是 nan / infinity, 就替换成指定 Value

// Column, Sql, MySql, ...                  // 数据库相关

/********************************************************/
// 可用 数据类型:
/********************************************************/

// byte  
// ushort
// uint  
// ulong 
// sbyte 
// short 
// int   
// long  
// double
// float 
// bool

// string
// BBuffer   byte[]
// object    xx::Object / IObject
// List<T>   可嵌套

// Ref<T>    说明 T 是弱引用类型. T 是用户类或上面4种. 不可嵌套.

// DateTime  暂不可用

// todo: LUA 下 Ref<T> 的表现形式, 还需要进一步设计


namespace TemplateLibrary
{
    /// <summary>
    /// 模拟 xx.List
    /// </summary>
    public class List<T> { }

    /// <summary>
    /// 模拟 xx.Ref 弱引用数据类型
    /// </summary>
    public class Ref<T> { }

    /// <summary>
    /// 模拟 xx.BBuffer
    /// </summary>
    public class BBuffer { }

    /// <summary>
    /// 模拟 System.DateTime
    /// </summary>
    public class DateTime { }

    /// <summary>
    /// 标记一个枚举的内容用于 生成时的 项目分类( 与项目对应的 根命名空间 白名单 ), 以便做分类转发之类的功能
    /// 该标记最多只可以出现一次
    /// </summary>
    [System.AttributeUsage(System.AttributeTargets.Enum)]
    public class CategoryNamespaces : System.Attribute
    {
    }

    /// <summary>
    /// 用来做类型到 typeId 的固定映射生成. 对应的 interface 的成员长相为  Type _id { get; }
    /// </summary>
    [System.AttributeUsage(System.AttributeTargets.Interface)]
    public class TypeIdMappings : System.Attribute
    {
    }

    /// <summary>
    /// 标记一个类成员不参与序列化
    /// </summary>
    [System.AttributeUsage(System.AttributeTargets.Field)]
    public class NotSerialize : System.Attribute
    {
    }

    /// <summary>
    /// 标记一个类成员不参与序列化
    /// </summary>
    [System.AttributeUsage(System.AttributeTargets.Field)]
    public class CustomSerialize : System.Attribute
    {
    }

    /// <summary>
    /// Lua 生成物之命名空间过滤( 白名单 ), 用到 ILuaFilter 接口上
    /// </summary>
    [System.AttributeUsage(System.AttributeTargets.Interface, AllowMultiple = true)]
    public class LuaFilter : System.Attribute
    {
        public LuaFilter(string v) { value = v; }
        public string value;
    }

    /// <summary>
    /// 备注。可用于类/枚举/函数 及其 成员
    /// </summary>
    public class Desc : System.Attribute
    {
        public Desc(string v) { value = v; }
        public string value;
    }

    /// <summary>
    /// 用于导出为 excel 时的列名生成
    /// </summary>
    public class Title : System.Attribute
    {
        public Title(string v) { value = v; }
        public string value;
    }


    /// <summary>
    /// 外部扩展。命名空间根据类所在实际命名空间获取，去除根模板名。参数如果传 false 则表示该类不支持序列化，无法用于收发
    /// </summary>
    [System.AttributeUsage(System.AttributeTargets.Enum | System.AttributeTargets.Class | System.AttributeTargets.Struct)]
    public class External : System.Attribute
    {
        public External(bool serializable = true
            , string cppDefaultValue = "nullptr"
            , string csharpDefaultValue = "null"
            , string luaDefaultValue = "null")
        {
            this.serializable = serializable;
            this.cppDefaultValue = cppDefaultValue;
            this.csharpDefaultValue = csharpDefaultValue;
            this.luaDefaultValue = luaDefaultValue;
        }
        public bool serializable;
        public string cppDefaultValue;
        public string csharpDefaultValue;
        public string luaDefaultValue;
    }

    /// <summary>
    /// 存储数据用的结构体. 生成时将不作为 "包" 对待
    /// </summary>
    [System.AttributeUsage(System.AttributeTargets.Class | System.AttributeTargets.Struct)]
    public class IsNotPackage : System.Attribute
    {
    }


    ///// <summary>
    ///// 针对指针类型成员, 于构造函数中创建默认实例
    ///// </summary>
    //[System.AttributeUsage(System.AttributeTargets.Field)]
    //public class CreateInstance : System.Attribute
    //{
    //}


    /// <summary>
    /// 标记当反列化 float / double 时, 如果值是 nan, 就设成 v
    /// </summary>
    [System.AttributeUsage(System.AttributeTargets.Field)]
    public class NaN : System.Attribute
    {
        public string value;
        public NaN(float v)
        {
            if (float.IsNaN(v) || float.IsInfinity(v))
            {
                throw new System.Exception("?????????");
            }
            value = v.ToString() + "f";
        }
        public NaN(double v)
        {
            if (double.IsNaN(v) || double.IsInfinity(v))
            {
                throw new System.Exception("?????????");
            }
            value = v.ToString();
        }
        public NaN()
        {
            value = "";
        }
    }

    /// <summary>
    /// 标记当反列化 float / double 时, 如果值是 infinity, 就设成 v
    /// </summary>
    [System.AttributeUsage(System.AttributeTargets.Field)]
    public class Infinity : System.Attribute
    {
        public string value;
        public Infinity(float v)
        {
            if (float.IsNaN(v) || float.IsInfinity(v))
            {
                throw new System.Exception("?????????");
            }
            value = v.ToString() + "f";
        }
        public Infinity(double v)
        {
            if (double.IsNaN(v) || double.IsInfinity(v))
            {
                throw new System.Exception("?????????");
            }
            value = v.ToString();
        }
        public Infinity()
        {
            value = "";
        }
    }

    /// <summary>
    /// 针对最外层级的 List, BBuffer, string 做长度限制。单个长度值为定长
    /// </summary>
    [System.AttributeUsage(System.AttributeTargets.Field | System.AttributeTargets.ReturnValue | System.AttributeTargets.ReturnValue)]
    public class Limit : System.Attribute
    {
        public Limit(int value)
        {
            this.value = value;
        }
        public int value;
    }






    /// <summary>
    /// 标记一个类为 "多主键" 类. 需要进一步标记类成员的 "主" "副" key
    /// </summary>
    [System.AttributeUsage(System.AttributeTargets.Class | System.AttributeTargets.Struct)]
    public class MultiKeyDict : System.Attribute
    {
    }

    /// <summary>
    /// 标记一个 "多主键" 类的成员, 是否为 key. 传入 true 的为主 key.
    /// </summary>
    [System.AttributeUsage(System.AttributeTargets.Field)]
    public class Key : System.Attribute
    {
        public bool value;
        public Key(bool value = false)
        {
            this.value = value;
        }
    }


    /// <summary>
    /// 标记一个类成员是一个数据库字段( 填充时仅考虑其出现的顺序而不管其名称 )
    /// </summary>
    [System.AttributeUsage(System.AttributeTargets.Field)]
    public class Column : System.Attribute
    {
        public bool readOnly;
        public Column(bool readOnly = false)
        {
            this.readOnly = readOnly;
        }
    }

    /// <summary>
    /// 标记一个类成员在数据库中不可空( 主要是指 String, BBuffer 这种指针类型 )
    /// </summary>
    [System.AttributeUsage(System.AttributeTargets.Field)]
    public class NotNull : System.Attribute
    {
    }


    /// <summary>
    /// 标记一个类成员以 "Property" 方式生成( 例如 int xxxx { get; set; }
    /// </summary>
    [System.AttributeUsage(System.AttributeTargets.Field)]
    public class Property : System.Attribute
    {
    }

    /// <summary>
    /// 标记一个类成员是在生成的时候跳过某些readOnly字段
    /// </summary>
    [System.AttributeUsage(System.AttributeTargets.Parameter)]
    public class SkipReadOnly : System.Attribute
    {
    }

    /// <summary>
    /// 标记 interface 对应 mysql 的连接对象
    /// </summary>
    [System.AttributeUsage(System.AttributeTargets.Interface)]
    public class MySql : System.Attribute
    {
    }

    /// <summary>
    /// 标记 interface 对应 microsoft sql server 的连接对象
    /// </summary>
    [System.AttributeUsage(System.AttributeTargets.Interface)]
    public class MsSql : System.Attribute
    {
    }

    /// <summary>
    /// 标记 interface 对应 sqlite 的连接对象
    /// </summary>
    [System.AttributeUsage(System.AttributeTargets.Interface)]
    public class SQLite : System.Attribute
    {
    }

    /// <summary>
    /// 标记 TSQL 语句的内容. 用于参数时表示 "不转义" 的字串直接拼接
    /// </summary>
    [System.AttributeUsage(System.AttributeTargets.Method)]
    public class Sql : System.Attribute
    {
        public string value;
        public Sql(string v) { value = v; }
    }

    /// <summary>
    /// 标记函数参数为不转义的字符串, 或是不纳入"传参" 的部分
    /// </summary>
    [System.AttributeUsage(System.AttributeTargets.Parameter)]
    public class Literal : System.Attribute
    {
    }

    /// <summary>
    /// 对应一次返回多个 SELECT 的查询结果
    /// </summary>
    public class Tuple<T1, T2> { }
    public class Tuple<T1, T2, T3> { }
    public class Tuple<T1, T2, T3, T4> { }
    public class Tuple<T1, T2, T3, T4, T5> { }
    public class Tuple<T1, T2, T3, T4, T5, T6> { }
    public class Tuple<T1, T2, T3, T4, T5, T6, T7> { }

}

