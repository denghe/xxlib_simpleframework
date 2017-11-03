#ifndef _FONTMANAGER_H_
#define _FONTMANAGER_H_

// 字体管理器( 用于存储所有 字体配置 以及创建 Label, 富文本 )
class FontManager
{
public:
    MAKE_INSTANCE_H( FontManager );
    FontManager();
    ~FontManager();

    // 添加一个字体配置, 返回 configIndex
    int addFont( string const & fontName, int fontSize = 36, int outLineSize = 0, Color4B outlineColor = Color4B::BLACK );

    // 添加一个颜色配置( 可取别名 ), 返回 colorIndex
    int addColor( Color3B c, string const & nickname = "" );

    // 返回一个默认色
    Color3B getDefaultColor();

    // 返回指定索引的颜色
    Color3B getColor( int colorIndex );

    // 返回指定别名的颜色
    Color3B getColor( string const & colorName );

    // 根据配置编号, 创建一个 Label
    Label* createLabel( int configIndex, string const & txt = "", int lineWidth = 0 );

    // 创建一个基于平台默认系统字体的 Label
    static Label* createSystemFontLabel( string const & txt, int fontSize, int lineWidth = 0 );

    // 获取 utf8 整字符 于 某配置编号 的显示尺寸, outLen 填充当前位于 c 的 utf8 整字的长度
    cocos2d::Size getCharSize( int configIndex, char const * c, int & outLen );

    // 获取 ASCII 字符 于 某配置编号 的显示尺寸
    cocos2d::Size getCharSize( int configIndex, char c );

    // 上面某些函数的 找不到就返回 false 的版本( 不生成到 lua )
    bool tryGetCharSize( int configIndex, char const * c, int & outLen, cocos2d::Size& sz );
    bool tryGetCharSize( int configIndex, char c, cocos2d::Size& sz );
    bool tryGetColor( int colorIndex, Color3B& c );
    bool tryGetColor( string const & colorName, Color3B& c );
    int getColorCount();
    int getFontCount();


    // 清容器
    void clear();
private:
    struct FontConfig
    {
        TTFConfig   ttfConfig;
        int         outLineSize;
        Color4B     outlineColor;
        int         widths[ 128 ];      // 文字渲染像素宽表( widths[0] 用来存 全角字符宽, 1 ~ 127 是变宽的 )
        int         height;             // 文字渲染像素高
    };
    vector<FontConfig> _fonts;
    vector<Color3B> _colors;
    unordered_map<string, Color3B> _namedColors;

    DELETE_COPY_ASSIGN( FontManager );
};


#endif
