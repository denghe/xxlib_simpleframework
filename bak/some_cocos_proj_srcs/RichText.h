#ifndef _RICHTEXT_H_
#define _RICHTEXT_H_


/*

RichText 指令说明( 注意指令内部都不能加空格等排版符 ):

font config reference:
    @f[xxx]
        xxx 为 addFont 到 font manager 的字体配置索引( 含字体, 字号, 描边信息 )

font color: 
    @c[xxx]
        xxx 为 10进制 addColor 到 font manager 的颜色配置索引
        如果 xxx 以 非数字打头, 则 xxx 将被解析为 颜色配置别名
    @c[#xxxxxx]
        xxxxxx 为 6 字节定长 RGB 16进制码
     
offset:
    @o[xxx]
        设后续内容的显示 x 坐标（ 从行首算起的 ）

image:
    @i[xxx]
        xxx 为 frame name, 保持原始尺寸显示
    @I[xxx]
        xxx 为 file name, 保持原始尺寸显示
    @i[xxx,width,height]
    @I[xxx,width,height]
        width height 为限制显示的宽高, 如果某个值为 0, 则该值为 等比自适应. 两个值都为 0 等同 原始尺寸

*/



class RTBaseNode
{
public:
    float _x, _y, _w, _h;                       // 显示坐标, 宽高
    RTBaseNode * _p;                            // 父节点
    virtual void Render( Node * n ) = 0;        // 创建 c2dx 对象填充到 Node
    virtual Node * GetRenderer() = 0;           // 在 Render 之后调用这个可以直接引用到显示元素
    RTBaseNode();
    virtual ~RTBaseNode() {}
};

class WrapPanel : public RTBaseNode
{
    std::vector<RTBaseNode*> _currRowChilds;    // 当前行对象集, 换行时, 调整坐标, 清空
    std::vector<RTBaseNode*> _childs;           // 所有对象, Container 析构时回收
    float _rowH;                                // 当前行高
    RichTextLineVAligns _am;                              // 子的对齐模式
public:
    float _rowX, _maxW;                         // 行起始 x 坐标, 最大行宽
    float _rowSpacing;                          // 行间距

    WrapPanel( float w, float rowSpacing = 0, RichTextLineVAligns am = RichTextLineVAligns::Bottom );
    ~WrapPanel();
    std::vector<RTBaseNode*> & Childs();
    void NewLine( float rowSpacing = 0 );
    // 将子节点加入, 设置节点的 x, 再根据节点宽度来调整 _rowX ( 视情况换行 )
    RTBaseNode * AddChild( RTBaseNode * n );
    template<typename T>
    T * AddChild( T * n )
    {
        return static_cast<T*>( AddChild( static_cast<RTBaseNode*>( n ) ) );
    }
    static void SyncOffset( WrapPanel * c, float x, float y );
    void Render( Node * n );
    // 在当前行附加 w 宽度的空白。遇行尾则不执行
    WrapPanel & AddSpace( float w );
    // 将当前行输出x坐标直接设为一个偏移量
    WrapPanel & SetOffset( float offset );
    WrapPanel & AddImage( char const * frameName, char const * defaultFrameName = "", float w = 0, float h = 0 );
    WrapPanel & AddPicture( char const * fileName, float w = 0, float h = 0 );
    WrapPanel & AddRichText( std::string const & txt );
    WrapPanel & AddText( std::string const & txt, Color3B c = Color3B::WHITE, int fontIndex = 0 );
    WrapPanel & AddNode( Node * ctrl );
	
private:
    Node * GetRenderer();
    FontManager * _fm;
};


class RTNode : public RTBaseNode
{
public:
    // 可初始化实际显示宽高
    RTNode( Node * node );
    void Render( Node * n );
    Node * GetRenderer();
private:
    Node * _node;
};


class RTImage : public RTBaseNode
{
public:
    // 可初始化实际显示宽高
    RTImage( char const * frameName, char const * defaultFrameName = "", float w = 0, float h = 0 );
    void Render( Node * n );
    Sprite * GetRenderer();
private:
    Sprite * _sprite;
};


class RTPicture : public RTBaseNode
{
public:
    // 可初始化实际显示宽高
    RTPicture( char const * fileName, float w = 0, float h = 0 );
    void Render( Node * n );
    Sprite * GetRenderer();
private:
    Sprite * _sprite;
};


class RTText : public RTBaseNode
{
public:
    // 初始化显示文本, 颜色, 每个字的宽高
    RTText( std::string const & txt, Color3B color = Color3B::WHITE, int fontIndex = 0 );
    void Render( Node * n );
    Label * GetRenderer();
private:
    std::string _txt;
    int _fontIndex;
    Color3B _color;
    Label * _label;
};







class RichText : public Node
{
public:
    // 富文本 中 超连接/button 之类 的 点击回调
    typedef function<void( string const & )> RichTextTouchHandlerType;

    // 获取富文本拼接节点
    static RichText * create( string const & txt, float maxWidth, float rowSpacing = 0, RichTextLineVAligns am = RichTextLineVAligns::Bottom );

    void registerTouchEventHandler( RichTextTouchHandlerType handler);
    void registerTouchEventHandler( LUA_FUNCTION handler );
	void setText( std::string const & txt );
private:
	float _maxWidth, _rowSpacing;
	RichTextLineVAligns	_am;
    RichTextTouchHandlerType _handler;
};

#endif
