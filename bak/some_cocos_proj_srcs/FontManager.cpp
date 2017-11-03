#include "Precompile.h"

MAKE_INSTANCE_CPP( FontManager );

FontManager::FontManager()
{
}

FontManager::~FontManager()
{
}

int FontManager::addFont( string const & fontName, int fontSize, int outLineSize /*= 0*/, Color4B outlineColor /*= Color4B::BLACK */ )
{
    int rtv = (int)_fonts.size();

    // 如果传入 fontName 为 "", 则表示使用当前平台系统默认字体名. 当前只兼容三个平台: windows, ios, android
    string fn;
    Label* lbl;
    FontConfig cfg;

    if( fontName.empty() )
    {
#ifdef __WIN
        fn = "Simsun";
#elif __APPLE__
        fn = "Helvetica";
#else
        fn = "DroidSans";
#endif
        lbl = Label::createWithSystemFont( "", fn, fontSize );
        cfg.ttfConfig.fontSize = fontSize;
        cfg.ttfConfig.fontFilePath = fontName;
    }
    else
    {
        TTFConfig ttfcfg( fontName.c_str(), fontSize );
        cfg.ttfConfig = ttfcfg;
        lbl = Label::createWithTTF( ttfcfg, "" );
    }

    if( !lbl )
    {
        CCLOG( "label create failed. maybe font file can't open?" );
        return -1;
    }
    if( outLineSize )
    {
        lbl->enableOutline( outlineColor, outLineSize );
    }

    string c( "\xE6\x88\x91" );
    lbl->setString( c );
    cfg.outLineSize = outLineSize;
    cfg.outlineColor = outlineColor;
    cfg.widths[ 0 ] = lbl->getContentSize().width;
    c.resize( 1 );
    for( int i = 1; i <= 127; ++i )
    {
        c[ 0 ] = (char)i;
        lbl->setString( c );
        cfg.widths[ i ] = lbl->getContentSize().width;
    }
    cfg.height = lbl->getContentSize().height;
    _fonts.push_back( cfg );

    return rtv;
}

Label* FontManager::createLabel( int configIndex, string const & txt, int lineWidth )
{
    if( configIndex >= (int)_fonts.size() )
    {
        configIndex = 0;
        CCLOG( "FontManager::createLabel( int configIndex out of range. size = %d", (int)_fonts.size() );
    }
    auto &cfg = _fonts[ configIndex ];
    Label* rtv;
    if( cfg.ttfConfig.fontFilePath.empty() )
    {
#ifdef __WIN
        string fn = "Simsun";
#elif __APPLE__
        string fn = "Helvetica";
#else
        string fn = "DroidSans";
#endif
        rtv = Label::createWithSystemFont( txt, fn, cfg.ttfConfig.fontSize );
        if( rtv && lineWidth ) rtv->setMaxLineWidth( lineWidth );
    }
    else
    {
        rtv = Label::createWithTTF( cfg.ttfConfig, txt, TextHAlignment::LEFT, lineWidth );
    }
    
    if( rtv == nullptr ) return rtv;
    if( cfg.outLineSize )
    {
        rtv->enableOutline( cfg.outlineColor, cfg.outLineSize );
    }
    //rtv->setAnchorPoint( Vec2::ANCHOR_MIDDLE );
    return rtv;
}

cocos2d::Size FontManager::getCharSize( int configIndex, char const * c, int & outLen )
{
    CC_ASSERT( configIndex >= 0 && configIndex < (int)_fonts.size() );
    Size rtv;
    auto& cfg = _fonts[ configIndex ];
    rtv.height = cfg.height;
    ushort key;
    outLen = Utils::fillWChar( &key, c );
    rtv.width = cfg.widths[ key > 127 ? 0 : key ];  // 大于 127 的（包括汉字）均视为等宽，放置于 0 单元
    return rtv;
}

cocos2d::Size FontManager::getCharSize( int configIndex, char c )
{
    CC_ASSERT( configIndex >= 0 && configIndex < (int)_fonts.size() );
    Size rtv;
    auto& cfg = _fonts[ configIndex ];
    rtv.height = cfg.height;
    rtv.width = cfg.widths[ (byte)c > 127 ? 0 : c ];
    return rtv;
}

bool FontManager::tryGetCharSize( int configIndex, char const * c, int & outLen, cocos2d::Size& sz )
{
    if( configIndex >= 0 && configIndex < (int)_fonts.size() ) return false;
    sz = getCharSize( configIndex, c, outLen );
    return true;
}

bool FontManager::tryGetCharSize( int configIndex, char c, cocos2d::Size& sz )
{
    if( configIndex >= 0 && configIndex < (int)_fonts.size() ) return false;
    sz = getCharSize( configIndex, c );
    return true;
}



int FontManager::addColor( Color3B c, string const & nickname /*= "" */ )
{
    int idx = (int)_colors.size();
    _colors.push_back( c );
    if( nickname.size() )
    {
        _namedColors.insert( make_pair( nickname, c ) );
    }
    return idx;
}


void FontManager::clear()
{
    _fonts.clear();
    _colors.clear();
    _namedColors.clear();
}

cocos2d::Color3B FontManager::getDefaultColor()
{
    if( _colors.size() ) return _colors[ 0 ];
    return Color3B::WHITE;
}

cocos2d::Color3B FontManager::getColor( int colorIndex )
{
    if( colorIndex >= (int)_colors.size() )
    {
        CCLOG( "getColor colorIndex out of range: _colors.size() = %d, colorIndex = %d", (int)_colors.size(), colorIndex );
        return Color3B::WHITE;
    }
    return _colors[ colorIndex ];
}

cocos2d::Color3B FontManager::getColor( string const & colorName )
{
    if( !colorName.size() ) return getDefaultColor();
    auto it = _namedColors.find( colorName );
    if( it == _namedColors.end() ) return getDefaultColor();
    return it->second;
}

Label* FontManager::createSystemFontLabel( string const & txt, int fontSize, int lineWidth /*= 0 */ )
{
#ifdef __WIN
    string fn = "Simsun";
#elif __APPLE__
    string fn = "Helvetica";
#else
    string fn = "DroidSans";
#endif
    auto rtv = Label::createWithSystemFont( txt, fn, fontSize );
    if( !rtv ) return nullptr;
    if( lineWidth ) rtv->setMaxLineWidth( lineWidth );
    return rtv;
}

bool FontManager::tryGetColor( int colorIndex, Color3B& c )
{
    if( colorIndex >= (int)_colors.size() ) return false;
    c = _colors[ colorIndex ];
    return true;
}

bool FontManager::tryGetColor( string const & colorName, Color3B& c )
{
    if( !colorName.size() ) return false;
    auto it = _namedColors.find( colorName );
    if( it == _namedColors.end() ) return false;
    c = it->second;
    return true;
}

int FontManager::getColorCount()
{
    return (int)_colors.size();
}

int FontManager::getFontCount()
{
    return (int)_fonts.size();
}
