
// 下面是包序列化相关备注

/********************************************************/
// 关键字
/********************************************************/

// enum                                     // 枚举
// class                                    // 自定义数据类型

/********************************************************/
// 标记
/********************************************************/

// Title( "字段名" )                        // 用于导出为 excel 时的列名生成
// Desc( "注释" )                           // 用于类 及类成员
// Limit(最大长)                            // 用于 string, bbuffer, List 容器等还原时的长度安全限制
// IsNotPackage                             // 用于标记某个 类或结构体 生成时不作为 "包" 对待( 可能只是其他包的子成员 )
// External( "path / namespace 啥的 " )     // 对标记为这个的 枚举或结构体，属于外部引用，不再填充或生成
// NaN, Infinity                            // 标记当反列化 浮点 时, 如果值是 nan / infinity, 就替换成指定 Value

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
// DateTime
// BBuffer                       // 2进制流

// T[]
// List<T>


namespace TemplateLibrary
{
    /// <summary>
    /// 模拟 xx.List
    /// </summary>
    public class List<T> { }

    /// <summary>
    /// 模拟 xx.BBuffer
    /// </summary>
    public class BBuffer { }

    /// <summary>
    /// 模拟 System.DateTime
    /// </summary>
    public class DateTime { }

    ///// <summary>
    ///// 模拟 xx.String_v
    ///// </summary>
    //public class String_v { }

    ///// <summary>
    ///// 模拟 xx.List_v
    ///// </summary>
    //public class List_v<T> { }


    /// <summary>
    /// 标记一个枚举的内容用于 生成时的 项目分类( 与项目对应的 根命名空间 白名单 ), 以便做分类转发之类的功能
    /// 该标记最多只可以出现一次
    /// </summary>
    [System.AttributeUsage(System.AttributeTargets.Enum)]
    public class CategoryNamespaces : System.Attribute
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
    /// 外部扩展。内容通常是 namespace。以 . 间隔的。
    /// </summary>
    [System.AttributeUsage(System.AttributeTargets.Enum | System.AttributeTargets.Class | System.AttributeTargets.Struct)]
    public class External : System.Attribute
    {
        public External() { }
        public External(string v) { value = v; }
        public string value;
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
    public class Ignore : System.Attribute
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
    /// 标记函数参数为不转义的字符串
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

