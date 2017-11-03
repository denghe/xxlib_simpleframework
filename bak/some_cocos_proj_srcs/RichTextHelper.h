#ifndef _RICHTEXTHELPER_H_
#define _RICHTEXTHELPER_H_

namespace RichTextHelper
{
    enum class RTCmds
    {
        Paragraph,
        ParagraphEnd,

        UnderLine,
        UnderLineEnd,

        Button,
        ButtonEnd,

        Font,
        Color,
        Offset,
        Image,
        Align,
        Space,
        Height,
        Variant,

        // 下面的并非正儿八经的命令类型 但可用于标记 富文本局部类型
        Text,
        NewLine
    };

    enum class HorizontalAligns
    {
        Left, Center, Right, Unknown
    };
    enum class VerticalAligns
    {
        Top, Middle, Bottom, Unknown
    };

    class Analyzer
    {
    public:
        Analyzer();
        Analyzer( string const& richText, int maxLen = 16384, int maxStackSize = 32 );
        Analyzer& assign( string const& richText, int maxLen = 16384, int maxStackSize = 32 );
        bool analyze();

        function<void()>                                                setError_MissLeftAngleBrackets;
        function<void()>                                                setError_MissRightAngleBrackets;
        function<void()>                                                setError_MissComma;
        function<void()>                                                setError_MissCommand;
        function<void()>                                                setError_MissParameter;
        function<void( int fontIdx )>                                   setError_MissFontIndex;
        
        function<void( string const& cmdName )>                         setError_WrongCommandName;
        function<void( string const& format )>                          setError_WrongParameterFormat;
        function<void( string const& cmdName )>                         setError_MissEndCommand;
        function<void()>                                                setError_LengthOverflow;
        function<void()>                                                setError_StackOverflow;

        function<void( int colorIdx )>                                  setError_GetColorByIndex;
        function<void( string const& colorName )>                       setError_GetColorByName;
        function<void( string const& key )>                             setError_GetVariant;


        function<bool( string const& colorName, Color3B& c )>           getColorByName;
        function<bool( int colorIndex, Color3B& c )>                    getColorByIndex;
        function<bool( string const& key, string& val )>                getVariant;

        function<bool( int fontIndex )>                                 setFont;
        function<void( Color3B c )>                                     setColor;
        function<void( float offset )>                                  setOffset;
        function<void( HorizontalAligns ah, VerticalAligns av )>        setAlign;
        function<void( float lh, bool isRatio )>                        setLineHeight;

        function<void( string const & txt )>                            addText;
        function<void()>                                                addNewLine;
        function<void( string fn, bool isFileName, float w, float h )>  addImage;
        function<void( float w, float h )>                              addSpace;

        function<bool( float lw )>                                      pushParagraph;
        function<void()>                                                popParagraph;
        function<void( string const& key )>                             pushButton;
        function<void()>                                                popButton;
        function<void( Color3B c, float thickness, float y )>           pushUnderLine;
        function<void()>                                                popUnderLine;

    //private:
        stack<RTCmds> cmds;
        string richText;
        char const* s;              // 指向 richText.c_str() 的当前处理点
        int len;                    // 存当前处理的剩余长度
        int maxLen;                 // 剩余长度限制( 因 Variant 功能会解析时替换文本内容，造成递归须判断 )
        int maxStackSize;
        int so();                   // 取 s 于 richText 的 offset
    };

}

#endif
