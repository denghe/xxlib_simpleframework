#include "Precompile.h"

namespace RichTextHelper
{
    string _cmdTexts[] =
    {
        "Paragraph",
        "ParagraphEnd",
        "UnderLine",
        "UnderLineEnd",
        "Button",
        "ButtonEnd",
        "Font",
        "Color",
        "Offset",
        "Image",
        "Align",
        "Space",
        "LineHeight",
        "Variant"
    };

    unordered_map<string, RTCmds> _cmdsMap;
    void initCmdsMap()
    {
        _cmdsMap.insert( make_pair( "p", RTCmds::Paragraph ) );
        _cmdsMap.insert( make_pair( "paragraph", RTCmds::Paragraph ) );

        _cmdsMap.insert( make_pair( "/p", RTCmds::ParagraphEnd ) );
        _cmdsMap.insert( make_pair( "/paragraph", RTCmds::ParagraphEnd ) );

        _cmdsMap.insert( make_pair( "u", RTCmds::UnderLine ) );
        _cmdsMap.insert( make_pair( "ul", RTCmds::UnderLine ) );
        _cmdsMap.insert( make_pair( "underline", RTCmds::UnderLine ) );

        _cmdsMap.insert( make_pair( "/u", RTCmds::UnderLineEnd ) );
        _cmdsMap.insert( make_pair( "/ul", RTCmds::UnderLineEnd ) );
        _cmdsMap.insert( make_pair( "/underline", RTCmds::UnderLineEnd ) );

        _cmdsMap.insert( make_pair( "b", RTCmds::Button ) );
        _cmdsMap.insert( make_pair( "btn", RTCmds::Button ) );
        _cmdsMap.insert( make_pair( "button", RTCmds::Button ) );

        _cmdsMap.insert( make_pair( "/b", RTCmds::ButtonEnd ) );
        _cmdsMap.insert( make_pair( "/btn", RTCmds::ButtonEnd ) );
        _cmdsMap.insert( make_pair( "/button", RTCmds::ButtonEnd ) );

        _cmdsMap.insert( make_pair( "f", RTCmds::Font ) );
        _cmdsMap.insert( make_pair( "font", RTCmds::Font ) );

        _cmdsMap.insert( make_pair( "c", RTCmds::Color ) );
        _cmdsMap.insert( make_pair( "color", RTCmds::Color ) );

        _cmdsMap.insert( make_pair( "o", RTCmds::Offset ) );
        _cmdsMap.insert( make_pair( "offset", RTCmds::Offset ) );

        _cmdsMap.insert( make_pair( "i", RTCmds::Image ) );
        _cmdsMap.insert( make_pair( "img", RTCmds::Image ) );
        _cmdsMap.insert( make_pair( "image", RTCmds::Image ) );

        _cmdsMap.insert( make_pair( "a", RTCmds::Align ) );
        _cmdsMap.insert( make_pair( "align", RTCmds::Align ) );

        _cmdsMap.insert( make_pair( "s", RTCmds::Space ) );
        _cmdsMap.insert( make_pair( "space", RTCmds::Space ) );

        _cmdsMap.insert( make_pair( "h", RTCmds::Height ) );
        _cmdsMap.insert( make_pair( "height", RTCmds::Height ) );
        _cmdsMap.insert( make_pair( "lh", RTCmds::Height ) );
        _cmdsMap.insert( make_pair( "lineheight", RTCmds::Height ) );

        _cmdsMap.insert( make_pair( "v", RTCmds::Variant ) );
        _cmdsMap.insert( make_pair( "var", RTCmds::Variant ) );
        _cmdsMap.insert( make_pair( "variant", RTCmds::Variant ) );
    }



    // 将 0 ~ 9, a ~ f, A ~ F 的 ASCII 映射成相应的 int 值
    char const _hex_int[ 256 ] =
    {
        -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
        -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
        -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
        +0, +1, +2, +3, +4, +5, +6, +7, +8, +9, -1, -1, -1, -1, -1, -1,
        -1, 10, 11, 12, 13, 14, 15, -1, -1, -1, -1, -1, -1, -1, -1, -1,
        -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
        -1, 10, 11, 12, 13, 14, 15, -1, -1, -1, -1, -1, -1, -1, -1, -1,
        -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
        -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
        -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
        -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
        -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
        -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
        -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
        -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
        -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
    };

    // 试将 string( buf, len ) 字串 完全匹配 转为 float, 失败返回 false
    bool tryParse( char const* buf, int len, float& outValue )
    {
        string tmp( buf, len );
        char* p;
        outValue = std::strtod( tmp.c_str(), &p );  // 貌似 vs2012 strtof 不能用
        return p == tmp.c_str() + len;
    }

    // 试将 string( buf, len ) 字串 完全匹配 转为 double, 失败返回 false
    bool tryParse( char const* buf, int len, double& outValue )
    {
        string tmp( buf, len );
        char* p;
        outValue = strtod( tmp.c_str(), &p );
        return p == tmp.c_str() + len;
    }

    // 试将 string( buf, len ) 字串 完全匹配 转为 int, 失败返回 false
    bool tryParse( char const* buf, int len, int& outValue )
    {
        string tmp( buf, len );
        char* p;
        outValue = strtol( tmp.c_str(), &p, 10 );
        return p == tmp.c_str() + len;
    }

    // 试将 string( buf, len ) 字串 完全匹配 转为 string, 失败返回 false
    bool tryParse( char const* buf, int len, string& outValue )
    {
        if( len == 0 ) return false;
        outValue.assign( buf, len );
        return true;
    }

    // 试将 string( buf, len ) 字串 完全匹配 转为 Color3B, 失败返回 false
    bool tryParse( char const* buf, int len, Color3B& outValue )    // 将 6 字节长的 16 进制转为颜色
    {
        if( len != 6 ) return false;
        auto b0 = _hex_int[ buf[ 0 ] ];
        auto b1 = _hex_int[ buf[ 1 ] ];
        auto b2 = _hex_int[ buf[ 2 ] ];
        auto b3 = _hex_int[ buf[ 3 ] ];
        auto b4 = _hex_int[ buf[ 4 ] ];
        auto b5 = _hex_int[ buf[ 5 ] ];
        if( b0 == -1 || b1 == -1 ||
            b2 == -1 || b3 == -1 ||
            b4 == -1 || b5 == -1 ) return false;
        outValue.r = ( b0 << 4 ) + b1;
        outValue.g = ( b2 << 4 ) + b3;
        outValue.b = ( b4 << 4 ) + b5;
        return true;
    }

    // 将关键字所有字母转成小写( 不会伤 / 号 )
    void toLower( char* s, int len )
    {
        auto p = (size_t*)s;
        int count = len / sizeof( size_t );
        size_t mask = (size_t)0x2020202020202020;
        for( int i = 0; i < count; ++i ) p[ i ] |= mask;
        for( int i = count * sizeof( size_t ); i < len; ++i ) s[ i ] |= 0x20;
    }

    // 扫出 空格，制表，换行符 个数
    int scanSpaces( char const* s, int len )
    {
        int i = 0;
        for( ; i < len; ++i )
        {
            auto c = s[ i ];
            if( c != ' ' &&
                c != '\r' &&
                c != '\t' &&
                c != '\n' ) return i;
        }
        return i;
    }

    // 扫出 指令 长度( 遇到 非 大小写英文字母 或 / 时结束 )
    int scanCommand( char const* s, int len )
    {
        int i = 0;
        for( ; i < len; ++i )
        {
            auto c = s[ i ];
            if( !( (c >= 'a' && c <= 'z')
                || (c >= 'A' && c <= 'Z')
                || c == '/' ) ) break;
        }
        return i;
    }

    // 扫出 参数 长度( 遇到 逗号 ] 空格等 时结束 )
    int scanParameter( char const* s, int len )
    {
        int i = 0;
        for( ; i < len; ++i )
        {
            auto c = s[ i ];
            if( c == '>' ||
                c == ',' ||
                c == ' ' ||
                c == '\r' ||
                c == '\t' ||
                c == '\n' ) return i;
        }
        return i;
    }

    // 扫出 文本, 内容填充到 outBuf 文本长度填充到 outLen, 返回 扫描长度
    // 如果出错将返回 负值索引 （遇到 <指令 或 \n 时截止， << 转义被视作文本, \r 忽略 )
    int scanText( char const* s, int len, char* outBuf, int& outLen )
    {
        outLen = 0;
        int i = 0;
        for( ; i < len; ++i )
        {
            auto c = s[ i ];
            if( c == '\n' ) return i;
            if( c == '<' )
            {
                if( i == len - 1 ) return -i;
                if( s[ i + 1 ] != '<' ) return i;
                else ++i;
            }
            else if( c == '\r' )
            {
                // 跳过
            }
            else
            {
                outBuf[ outLen++ ] = c;
            }
        }
        return i;
    }

    // 跳过空格
    void jumpSpaces( Analyzer& ctx )
    {
        if( int rtv = scanSpaces( ctx.s, ctx.len ) )
        {
            ctx.s += rtv;
            ctx.len -= rtv;
        }
    }

    // 跳过右尖扩号( 尖扩号 左侧 可能有空格 ), 格式不对将报错并返回 false
    bool jumpRightAngleBrackets( Analyzer& ctx )
    {
        jumpSpaces( ctx );
        if( ctx.s[ 0 ] != '>' )
        {
            ctx.setError_MissRightAngleBrackets();
            return false;
        }
        ++ctx.s;
        --ctx.len;
        return true;
    }

    // 跳过逗号( 逗号 前后 可能有空格 ), 格式不对将报错并返回 false
    bool jumpComma( Analyzer& ctx )
    {
        jumpSpaces( ctx );
        if( ctx.s[ 0 ] != ',' )
        {
            ctx.setError_MissComma();
            return false;
        }
        ++ctx.s;
        --ctx.len;
        jumpSpaces( ctx );
        return true;
    }

    // 取参数长, 0 长将报错并返回 false
    bool getParmLen( Analyzer& ctx, int& outLen )
    {
        outLen = scanParameter( ctx.s, ctx.len );
        if( outLen == 0 )
        {
            ctx.setError_MissParameter();
            return false;
        }
        return true;
    }

    // 取 int 参数. 格式不对将报错并返回 false
    bool getParm( Analyzer& ctx, int& outVal )
    {
        int parmLen;
        if( !getParmLen( ctx, parmLen ) ) return false;
        if( !tryParse( ctx.s, parmLen, outVal ) )
        {
            ctx.setError_WrongParameterFormat( "Integer" );
            return false;
        }
        ctx.s += parmLen;
        ctx.len -= parmLen;
        return true;
    }

    // 取 float 参数. 格式不对将报错并返回 false
    bool getParm( Analyzer& ctx, float& outVal )
    {
        int parmLen;
        if( !getParmLen( ctx, parmLen ) ) return false;
        if( !tryParse( ctx.s, parmLen, outVal ) )
        {
            ctx.setError_WrongParameterFormat( "Float" );
            return false;
        }
        ctx.s += parmLen;
        ctx.len -= parmLen;
        return true;
    }

    // 取 string 参数. 格式不对将报错并返回 false
    bool getParm( Analyzer& ctx, string& outVal )
    {
        int parmLen;
        if( !getParmLen( ctx, parmLen ) ) return false;
        if( !tryParse( ctx.s, parmLen, outVal ) )
        {
            ctx.setError_WrongParameterFormat( "String" );
            return false;
        }
        ctx.s += parmLen;
        ctx.len -= parmLen;
        return true;
    }

    // 取 Color3B 参数. 格式不对将报错并返回 false
    bool getParm( Analyzer& ctx, Color3B& outVal )
    {
        int parmLen;
        if( !getParmLen( ctx, parmLen ) ) return false;
        if( !tryParse( ctx.s, parmLen, outVal ) )
        {
            ctx.setError_WrongParameterFormat( "Color" );
            return false;
        }
        ctx.s += parmLen;
        ctx.len -= parmLen;
        return true;
    }


    void handleText( Analyzer& ctx, char const* s, int len )
    {
        ctx.addText( string( s, len ) );
    }

    bool handleParagraph( Analyzer& ctx, int stackSize )
    {
        float LW = 0;     // 行宽
        if( *ctx.s != '>' )
        {
            if( !getParm( ctx, LW ) ) return false;
        }

        if( !jumpRightAngleBrackets( ctx ) ) return false;

        ctx.cmds.push( RTCmds::Paragraph );
        if( !ctx.pushParagraph( LW ) ) return false;
        return true;
    }

    bool handleParagraphEnd( Analyzer& ctx, int stackSize )
    {
        if( !jumpRightAngleBrackets( ctx ) ) return false;

        if( (int)ctx.cmds.size() < stackSize
            || (ctx.cmds.size() && ctx.cmds.top() != RTCmds::Paragraph) )
        {
            ctx.setError_MissEndCommand( _cmdTexts[ (int)ctx.cmds.top() ] );
            return false;
        }
        ctx.cmds.pop();
        ctx.popParagraph();
        return true;
    }

    bool handleUnderLine( Analyzer& ctx, int stackSize )
    {
        Color3B c;
        if( *ctx.s == '>' )                             // 不带任何参数, 取默认值
        {
            if( !ctx.getColorByIndex( 0, c ) )
            {
                ctx.setError_GetColorByIndex( 0 );
                return false;
            }
            ++ctx.s;
            --ctx.len;                                  // 跳过 >
            ctx.cmds.push( RTCmds::UnderLine );
            ctx.pushUnderLine( c, 0, 0 );
            return true;
        }

        if( *ctx.s == '#' )                             // # 打头接 6 字节长的 16进制颜色码
        {
            ++ctx.s;                                    // 跳过 #
            --ctx.len;
            if( !getParm( ctx, c ) ) return false;
        }
        else if( *ctx.s >= '0' &&  *ctx.s <= '9' )      // 颜色编号
        {
            int cidx;
            if( !getParm( ctx, cidx ) ) return false;
            if( !ctx.getColorByIndex( cidx, c ) )
            {
                ctx.setError_GetColorByIndex( cidx );
                return false;
            }
        }
        else                                            // 颜色别名
        {
            string cn;
            if( !getParm( ctx, cn ) ) return false;
            if( !ctx.getColorByName( cn, c ) )
            {
                ctx.setError_GetColorByName( cn );
                return false;
            }
        }

        float thickness = 1;                                // 线粗
        jumpSpaces( ctx );
        if( *ctx.s == ',' )
        {
            ++ctx.s;
            --ctx.len;
            jumpSpaces( ctx );
            if( !getParm( ctx, thickness ) ) return false;
        }

        float ypos = 0;                                 // 从下到上的显示偏移量
        jumpSpaces( ctx );
        if( *ctx.s == ',' )
        {
            ++ctx.s;
            --ctx.len;
            jumpSpaces( ctx );
            if( !getParm( ctx, ypos ) ) return false;
        }

        if( !jumpRightAngleBrackets( ctx ) ) return false;

        ctx.cmds.push( RTCmds::UnderLine );
        ctx.pushUnderLine( c, thickness, ypos );
        return true;
    }

    bool handleUnderLineEnd( Analyzer& ctx, int stackSize )
    {
        if( !jumpRightAngleBrackets( ctx ) ) return false;

        if( (int)ctx.cmds.size() < stackSize
            || (ctx.cmds.size() && ctx.cmds.top() != RTCmds::UnderLine) )
        {
            ctx.setError_MissEndCommand( _cmdTexts[ (int)ctx.cmds.top() ] );
            return false;
        }
        ctx.cmds.pop();
        ctx.popUnderLine();
        return true;
    }

    bool handleButton( Analyzer& ctx, int stackSize )
    {
        string key;             // button click 回调 要传的参数
        if( !getParm( ctx, key ) ) return false;

        if( !jumpRightAngleBrackets( ctx ) ) return false;

        ctx.cmds.push( RTCmds::Button );
        ctx.pushButton( key );
        return true;
    }

    bool handleButtonEnd( Analyzer& ctx, int stackSize )
    {
        if( !jumpRightAngleBrackets( ctx ) ) return false;

        if( (int)ctx.cmds.size() < stackSize
            || (ctx.cmds.size() && ctx.cmds.top() != RTCmds::Button) )
        {
            ctx.setError_MissEndCommand( _cmdTexts[ (int)ctx.cmds.top() ] );
            return false;
        }
        ctx.cmds.pop();
        ctx.popButton();
        return true;
    }

    bool handleFont( Analyzer& ctx, int stackSize )
    {
        int fontIndex = 0;  // 字体编号
        if( !getParm( ctx, fontIndex ) ) return false;

        if( !jumpRightAngleBrackets( ctx ) ) return false;

        if( !ctx.setFont( fontIndex ) )
        {
            ctx.setError_MissFontIndex( fontIndex );
            return false;
        }
        return true;
    }

    bool handleColor( Analyzer& ctx, int stackSize )
    {
        Color3B c;                                      // 颜色有 3 种表达方式
        if( *ctx.s == '#' )                             // # 打头接 6 字节长的 16进制颜色码
        {
            ++ctx.s;                                    // 跳过 #
            --ctx.len;
            if( !getParm( ctx, c ) ) return false;
        }
        else if( *ctx.s >= '0' &&  *ctx.s <= '9' )      // 颜色编号
        {
            int cidx;
            if( !getParm( ctx, cidx ) ) return false;
            if( !ctx.getColorByIndex( cidx, c ) )
            {
                ctx.setError_GetColorByIndex( cidx );
                return false;
            }
        }
        else                                            // 颜色别名
        {
            string cn;
            if( !getParm( ctx, cn ) ) return false;
            if( !ctx.getColorByName( cn, c ) )
            {
                ctx.setError_GetColorByName( cn );
                return false;
            }
        }

        if( !jumpRightAngleBrackets( ctx ) ) return false;

        ctx.setColor( c );
        return true;
    }

    bool handleOffset( Analyzer& ctx, int stackSize )
    {
        float offset = 0;     // 当前行 光标的 x 坐标
        if( !getParm( ctx, offset ) ) return false;

        if( !jumpRightAngleBrackets( ctx ) ) return false;

        ctx.setOffset( offset );
        return true;
    }

    bool handleImage( Analyzer& ctx, int stackSize )
    {
        string fn;                  // sprite frame name 或 file name
        bool isFileName = false;
        if( *ctx.s == '#' )         // # 打头的是 file name
        {
            isFileName = true;
            ++ctx.s;                // 跳过 #
            --ctx.len;
            if( !getParm( ctx, fn ) ) return false;
        }
        else                        // 或者 sprite frame name
        {
            if( !getParm( ctx, fn ) ) return false;
        }

        float width = 0;                // 宽
        jumpSpaces( ctx );
        if( *ctx.s == ',' )
        {
            ++ctx.s;
            --ctx.len;
            jumpSpaces( ctx );
            if( !getParm( ctx, width ) ) return false;
        }

        float height = 0;               // 高
        jumpSpaces( ctx );
        if( *ctx.s == ',' )
        {
            ++ctx.s;
            --ctx.len;
            jumpSpaces( ctx );
            if( !getParm( ctx, height ) ) return false;
        }

        if( !jumpRightAngleBrackets( ctx ) ) return false;

        ctx.addImage( fn, isFileName, width, height );
        return true;
    }

    bool handleAlign( Analyzer& ctx, int stackSize )
    {
        string s;
        if( !getParm( ctx, s ) ) return false;

        auto ha = HorizontalAligns::Unknown;
        auto va = VerticalAligns::Unknown;
        auto setVal = [ &]( int i )
        {
            if( s[ i ] == 'l' ) { ha = HorizontalAligns::Left; return true; }
            else if( s[ i ] == 'c' ) { ha = HorizontalAligns::Center;  return true; }
            else if( s[ i ] == 'r' ) { ha = HorizontalAligns::Right;  return true; }
            else if( s[ i ] == 't' ) { va = VerticalAligns::Top; return true; }
            else if( s[ i ] == 'm' ) { va = VerticalAligns::Middle;  return true; }
            else if( s[ i ] == 'b' ) { va = VerticalAligns::Bottom;  return true; }
            else return false;
        };
        toLower( (char*)s.c_str(), (int)s.size() );
        if( ( s.size() == 1 && !setVal( 0 ) ) ||
            ( s.size() == 2 && ( s[ 0 ] == s[ 1 ]
            || (( s[ 0 ] == 'l' || s[ 0 ] == 'c' || s[ 0 ] == 'r' ) && ( s[ 1 ] == 'l' || s[ 1 ] == 'c' || s[ 1 ] == 'r' ))
            || (( s[ 0 ] == 't' || s[ 0 ] == 'm' || s[ 0 ] == 'b' ) && ( s[ 1 ] == 't' || s[ 1 ] == 'm' || s[ 1 ] == 'b' ))
            || !setVal( 0 ) || !setVal( 1 ) ) ) )
        {
            ctx.setError_WrongParameterFormat( "L/R/C, T/M/B" );
            return false;
        }
        if( !jumpRightAngleBrackets( ctx ) ) return false;

        ctx.setAlign( ha, va );
        return true;
    }

    bool handleSpace( Analyzer& ctx, int stackSize )
    {
        float width = 0, height = 0;    // 空间宽高
        if( !getParm( ctx, width ) ) return false;
        jumpSpaces( ctx );
        if( *ctx.s == ',' )             // 如果有第2个参数
        {
            ++ctx.s;
            --ctx.len;
            jumpSpaces( ctx );
            if( !getParm( ctx, height ) ) return false;
        }
        if( !jumpRightAngleBrackets( ctx ) ) return false;

        ctx.addSpace( width, height );
        return true;
    }

    bool handleLineHeight( Analyzer& ctx, int stackSize )
    {
        if( *ctx.s == '>' )                             // 不带任何参数, 默认值为 0
        {
            ++ctx.s;
            --ctx.len;                                  // 跳过 >
            ctx.setLineHeight( 0, false );
            return true;
        }

        bool isRatio = false;       // 是否为倍数
        if( *ctx.s == '*' )         // * 打头的是倍数
        {
            isRatio = true;
            ++ctx.s;                // 跳过 *
            --ctx.len;
        }

        float LH = 0;               // 行高
        if( !getParm( ctx, LH ) ) return false;

        if( !jumpRightAngleBrackets( ctx ) ) return false;

        ctx.setLineHeight( LH, isRatio );
        return true;
    }

    bool handleVariant( Analyzer& ctx, int stackSize )
    {
        string key;
        if( !getParm( ctx, key ) ) return false;

        if( !jumpRightAngleBrackets( ctx ) ) return false;

        // 将取到的文本的内容插入到当前处理点( ] 后面 ) 以便继续处理
        string val;
        if( !ctx.getVariant( key, val ) )
        {
            ctx.setError_GetVariant( key );
            return false;
        }
        // insert 可能导致字串内存重分配 于是 s 不再合法 故先得到 offset
        if( val.size() )
        {
            auto currIdx = ctx.so();
            ctx.richText.insert( currIdx, val );
            ctx.s = ctx.richText.c_str() + currIdx;
            ctx.len += (int)val.size();
        }
        return true;
    }



    // 构成一个指针数组，方便按 enum 调用
    typedef bool( *HandleType )( Analyzer&, int );
    HandleType _handles[] =
    {
        handleParagraph,
        handleParagraphEnd,
        handleUnderLine,
        handleUnderLineEnd,
        handleButton,
        handleButtonEnd,
        handleFont,
        handleColor,
        handleOffset,
        handleImage,
        handleAlign,
        handleSpace,
        handleLineHeight,
        handleVariant
    };











    Analyzer::Analyzer()
    {
        if( !_cmdsMap.size() ) initCmdsMap();
    }

    Analyzer::Analyzer( string const& richText, int maxLen, int maxStackSize )
    {
        if( !_cmdsMap.size() ) initCmdsMap();
        assign( richText, maxLen, maxStackSize );
    }

    Analyzer& Analyzer::assign( string const& richText, int maxLen, int maxStackSize )
    {
        this->richText = richText;
        this->s = this->richText.c_str();
        this->len = ( int )this->richText.size();
        this->maxLen = maxLen;
        this->maxStackSize = maxStackSize;
        while( this->cmds.size() ) this->cmds.pop();
        return *this;
    }

    int Analyzer::so()
    {
        return int( s - richText.c_str() );
    }

    bool Analyzer::analyze()
    {
        string tmpStr, cmdStr;
        tmpStr.resize( len );
        auto outBuf = (char*)tmpStr.c_str();
        int outLen = 0;
        int rtv = 0;

        auto stackSize = (int)cmds.size();                              // 记录堆栈深度, 正常出函数的时候要 check, 且中间的 pop 类操作也不能小于该值
        if( stackSize > maxStackSize )
        {
            setError_StackOverflow();                                   // 超出了设定的安全堆栈深度
            return false;
        }
        do
        {
            rtv = scanText( s, len, outBuf, outLen );                   // 扫文本 直到出现 <( << 不算 ) 或 \n
            if( rtv < 0 )
            {
                s += -rtv + 1;
                len -= -rtv + 1;                                        // 把指针移到出错点 方便显示啥的
                setError_MissCommand();
                return false;
            }
            if( outLen )                                                // 成功扫出文本，处理并跳过长度
            {
                handleText( *this, outBuf, outLen );
                s += rtv;
                len -= rtv;
            }
            if( !len ) return true;                                     // 如果 offset 到达结尾，返回 正确处理完毕

            if( *s == '\n' )                                            // 如果 scanText 时遇到了换行符, 换行并继续扫 文本
            {
                ++s;
                --len;                                                  // 跳过 \n
                addNewLine();
                continue;                                               // 继续扫文本
            }

            ++s;
            --len;                                                      // 跳过 <
            jumpSpaces( *this );                                        // 跳过 < 后面可能出现的空格

            if( !len )
            {
                setError_MissCommand();                                 // 如果跳过空格之后遇到文本结束，那一定是缺指令
                return false;
            }

            rtv = scanCommand( s, len );                                // 扫指令 直到出现 空格或>
            if( rtv == 0 )
            {
                setError_MissCommand();                                 // 扫到 0 长 即 没扫出指令，有可能是遇到 > 或非指令字符集打头
                return false;
            }
            cmdStr.assign( s, rtv );                                    // 用取到的指令构造一个 string 方便去 map 查
            toLower( (char*)cmdStr.c_str(), (int)cmdStr.size() );       // 转小写

            auto it = _cmdsMap.find( cmdStr );
            if( it == _cmdsMap.end() )
            {
                setError_WrongCommandName( cmdStr );                    // 字典中没找到
                return false;
            }
            s += rtv;                                                   // 移动指针，同步长度，指向指令后面的区域
            len -= rtv;

            jumpSpaces( *this );                                        // 跳过 指令 后面可能出现的空格
            if( !len )
            {
                setError_MissRightAngleBrackets();                      // 如果跳过空格之后遇到文本结束，那一定是缺 >
                return false;
            }

            if( !_handles[ (int)it->second ]( *this, stackSize ) )      // 处理指令, 不成功返回 false
            {
                return false;
            }

            if( s > richText.c_str() + maxLen )                         // 防 variant replace 死循环
            {
                setError_LengthOverflow();
                return false;
            }

            if( (int)cmds.size() < stackSize )                          // 被用于递归时，解析完 “子” 后跳出
            {
                return true;
            }

        } while( len );

        if( (int)cmds.size() > stackSize )
        {
            setError_MissEndCommand( _cmdTexts[ (int)cmds.top() ] );
            return false;
        }
        return true;
    }
}
