#ifndef _RICHTEXTEX_H_
#define _RICHTEXTEX_H_

/*

指令说明: ( 非字母参数与指令间可以不空格。 <>内侧，逗号两侧 都可以加空格，文本区的\r将被忽略 )

font config reference:
    <f xxx>
        xxx 为 addFont 到 font manager 的字体配置索引( 含字体, 字号, 描边信息 )

font color: 
    <c xxx>
        xxx 为 10进制 addColor 到 font manager 的颜色配置索引
        如果 xxx 以 非数字打头, 则 xxx 将被解析为 颜色配置别名
    <c #xxxxxx>
        xxxxxx 为 6 字节定长 RGB 16进制码
     
offset:
    <o xxx>
        设后续内容的显示 x 坐标（ 从行首算起的 ）

image:
    <i xxx>
        xxx 为 frame name, 保持原始尺寸显示
        如果 xxx 以 #开头，则为文件名
    <i xxx, width, height>
        width height 为限制显示的宽高, 如果某个值为 0, 则该值为 等比自适应. 两个值都为 0 等同 原始尺寸

under line:
    <u a,b,c>
        a 为颜色
        b 为线宽
        c 为至底向上的显示偏移量
        显示下划线。参数可倒序缺省
    </u>
        关闭下划线

button:
    <b xxx>
        xxx 为 clickHandler 调用参数( string 类型 )
    </b>
        button 区域结束标记

align:  
    <a xx>
        xx 为 L ( left ) R ( right ) C ( center )
        从当前位置开始 直到 换行符 或 串尾, 将恢复 左对齐

        xx 为 T ( top ) M ( middle ) B ( bottom )
        设 "从当前位置开始 直到 另一个 <a 纵向>" 的 "纵向" 对齐模式

        如果遇到另一个 <a 横向>， 将不换行直接开始从相应的坐标绘制，之前的绘制内容可能被覆盖
        可以不分大小写，同时指定纵横两种对齐方式，例如 <a CM>

space:
    <s xxx, yyy>
        制造 xxx * yyy 像素的空间. yyy 可省( 可用于制造 margin 效果 )

height:
    <h xxx>
        xxx 如果不传或为0 则恢复默认以当前字体高作为行高。否则以 xxx 作为行高

paragraph:
    <p xxx>
        段落起始标志. 一个段 即一个显示 区块， 即矩形
        xxx 为矩形宽度，不传 或 传0 则该值为 当前行剩余宽度
    </p>
        段落结束标志

variant:
    <v xxx>
        xxx 为 variantHandler 调用参数（ string 类型 ), 其返回值将作为 富文本 嵌入在标记处

*/


class RichTextEx : public Node
{
public:
    // 初始化 Analyzer
    void init_analyzer();

    // button 的 点击/touch 处理函数类型
    typedef function<void( string const& key )> TouchHandlerType;

    // 变量 的 替换处理函数类型
    typedef function<bool( string const& key, string& val )> VariantHandlerType;

    // create 出错处理函数类型
    typedef function<void( string const& err )> ErrorHandlerType;

    // 创建富文本对象
    static RichTextEx * create( string const & txt
                                , float lineWidth
                                , TouchHandlerType touchHandler = nullptr
                                , VariantHandlerType variantHandler = nullptr
                                , ErrorHandlerType errorHandler = nullptr );

    // 后期修改 handler
    void registerTouchEventHandler( TouchHandlerType handler );
    void registerVariantEventHandler( VariantHandlerType handler );
    void registerErrorEventHandler( ErrorHandlerType handler );

    // 重设内容, 失败返回 false, 错误将触发 _errorHandler 调用
    bool assign( std::string const & txt );

    // for lua
    static RichTextEx * createLua( string const & txt
                                , float lineWidth
                                , LUA_FUNCTION touchHandler = 0
                                , LUA_FUNCTION variantHandler = 0
                                , LUA_FUNCTION errorHandler = 0 );
    void registerTouchEventHandler( LUA_FUNCTION handler );
    void registerVariantEventHandler( LUA_FUNCTION handler );
    void registerErrorEventHandler( LUA_FUNCTION handler );

//private:
    TouchHandlerType _touchHandler;
    VariantHandlerType _variantHandler;
    ErrorHandlerType _errorHandler;
    int _lineWidth;
    RichTextHelper::Analyzer _a;
    string _err;

    //bool fillNode( RichTextHelper::Analyzer& a, Node*& container, float lineWidth );
};

#endif
