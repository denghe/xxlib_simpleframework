#include "Precompile.h"
#if __WIN
#include <io.h>
#include <codecvt>
#endif
#if __IOS
#include "SkipBackup.h"
#endif



bool Utils::fileCopy( string const & src, string const & dest )
{
    auto data = FileUtils::getInstance()->getDataFromFile( src );
    if( !data.getSize() ) return false;
    return fileWriteBytes( dest, (char*)data.getBytes(), data.getSize() );
}

bool Utils::fileExists( string const & fn )
{
    return FileUtils::getInstance()->isFileExist( fn );
}

bool Utils::fileDelete( string const & fn )
{
    return remove( fn.c_str() ) == 0;
}

bool Utils::fileRename( string const & src, string const & dest )
{
    return rename( src.c_str(), dest.c_str() ) == 0;
}

bool Utils::fileWriteBytes( string const & fn, char const * buf, int len )
{
    auto f = fopen( fn.c_str(), "wb" );
    if( !f ) return false;
    auto ws = fwrite( buf, 1, len, f );
    if( ws != len )
    {
        fclose( f );
        return false;
    }
    return fclose( f ) == 0;
}

int Utils::fileMoveSeekEnd( string const & fn )
{
    auto f = fopen( fn.c_str(), "r" );
    fseek( f, 0, SEEK_END );
    auto len = ftell( f );
    fclose( f );
    return len;
}

int Utils::fileMoveSeekEnd( FILE* f )
{
    fseek( f, 0, SEEK_END );
    return ftell( f );
}

int Utils::fileGetSize( FILE* f )
{
    auto bak = ftell( f );
    fseek( f, 0, SEEK_END );
    auto size = ftell( f );
    fseek( f, bak, SEEK_SET );
    return size;
}

bool Utils::fileCut( FILE* f, uint size )
{
#ifdef __WIN
    fseek( f, size, SEEK_SET );
    int fd = _fileno( f );
    HANDLE hfile = (HANDLE)_get_osfhandle( fd );
    return SetEndOfFile( hfile ) == TRUE;
#else
    return ftruncate( fileno( f ), size ) == 0;
#endif
}

std::string Utils::fileGetFullPath( string const & fn )
{
    return FileUtils::getInstance()->fullPathForFilename( fn );
}

std::string Utils::filePathCutFileName( string const & fn )
{
    auto pos = fn.find_last_of( "/\\" );
    return string( fn.data(), pos + 1 );
}

cocos2d::Data Utils::fileGetData( string const & fn )
{
    return FileUtils::getInstance()->getDataFromFile( fn );
}

std::string Utils::fileGetText( string const & fn )
{
    return FileUtils::getInstance()->getStringFromFile( fn );
}

void Utils::fileSkipBackup( string const & path )
{
#if __IOS
    skipBackup( path.c_str() );
#endif
}

std::string Utils::getTextureFileNameFromPlistDict( ValueMap const & dict )
{
    // sprite frame plist
    auto it = dict.find( "metadata" );
    if( it != dict.end() )
    {
        auto& metadataDict = it->second.asValueMap();
        return metadataDict.at( "textureFileName" ).asString();
    }
    // maybe is particle plist
    it = dict.find( "textureFileName" );
    if( it != dict.end() )
    {
        return it->second.asString();
    }
    return "";
}

std::string Utils::getTextureFileNameFromPlistFile( string const & fn )
{
    auto fu = FileUtils::getInstance();
    auto fullPath = fu->fullPathForFilename( fn );
    return fu->fullPathFromRelativeFile(
        getTextureFileNameFromPlistDict(
            fu->getValueMapFromFile(
                fullPath
            )
        ), fullPath
    );
}

//vector<string> Utils::getFileNamesByAtlas( string const & data )
//{
//    vector<string> fns;
//    auto pb = data.c_str();
//    auto pe = pb + data.size();
//    auto pf = find( pb, pe, '\n' );
//    assert( pf != pe );
//    string fn;
//    fn.assign( pb, pf );
//    fns.push_back( fn );                    // 取第１个文件名
//    do
//    {
//        pf = find( pf + 1, pe, '\n' );
//        if( *( ++pf ) != '\n' ) continue;   // 找出一个空行
//        pb = pf + 1;
//        pf = find( pb, pe, '\n' );          // 找到空行后面一行（文件名）的行尾
//        fn.assign( pb, pf );
//        fns.push_back( fn );
//    } while( pf < pe );
//    return fns;
//}

void Utils::simpleReplace( string &s, char src, char dest )
{
    auto p = (char*)s.c_str();
    for( size_t i = 0; i < s.size(); ++i )      // 替换 src 为 dest
    {
        if( p[ i ] == src ) p[ i ] = dest;
    }
}

std::string Utils::simpleReplace( string &s, char src, string const & dest )
{
    string rtv;
    rtv.reserve( s.size() );
    auto p = (char*)s.c_str();
    for( size_t i = 0; i < s.size(); ++i )      // 替换 src 为 dest
    {
        if( p[ i ] == src )
        {
            rtv.append( dest );
        }
        else
        {
            rtv.append( p + i, 1 );
        }
    }
    return rtv;
}

bool Utils::createDirectory( char const * tarPath )
{
#if( CC_TARGET_PLATFORM != CC_PLATFORM_WIN32 && CC_TARGET_PLATFORM != CC_PLATFORM_WP8 )
    mode_t processMask = umask( 0 );
    int ret = mkdir( tarPath, S_IRWXU | S_IRWXG | S_IRWXO );
    umask( processMask );
    if( ret != 0 && ( errno != EEXIST ) )
    {
        return false;
    }

    return true;
#else
    static std::wstring_convert<std::codecvt_utf8<wchar_t>> conv;

    
    BOOL ret = CreateDirectoryW( conv.from_bytes( tarPath ).c_str(), NULL );
    if( !ret && ERROR_ALREADY_EXISTS != GetLastError() )
    {
        return false;
    }
    return true;
#endif
}

bool Utils::ensureDirectory( string fullPath )
{
    if( fullPath.empty() ) return true;
    auto p = (char*)fullPath.c_str();                       // 取指针以方便使用
    auto len = fullPath.size();                             // 取长度
    auto &lc = p[ len - 1 ];
    if( lc == '\\' || lc == '/' ) p[ len-- ] = '\0';        // 如果尾部有 / , 去掉, len--
    bool isFirst = true;
    for( size_t i = 0; i < len; ++i )                       // 开始扫 /, 临时替换为 \0, 产生一级级的 完整目录名
    {
        auto &c = p[ i ];
        if( c == '\\' || c == '/' )
        {
            if( isFirst )                                   // 跳过第1个 /, 解决 类似 /xxxx/xxxx   c:\xxxx\xxx 的问题( 会产生空串或 c: 这种非目录 )
            {
                isFirst = false;
                continue;
            }
            auto &nc = p[ i + 1 ];
            if( nc == '\\' || nc == '/' ) ++i;              // 万一有 // 之类的, 跳过

            c = '\0';
            if( !createDirectory( p ) ) return false;
            c = '/';
        }
    }
    return createDirectory( p );
}

vector<string> Utils::split( string const & s, string const & delim )
{
    vector<string> ss;
    size_t last = 0;
    size_t index = s.find_first_of( delim, last );
    while( index != string::npos )
    {
        ss.push_back( s.substr( last, index - last ) );
        last = index + 1;
        index = s.find_first_of( delim, last );
    }
    if( index - last > 0 )
    {
        ss.push_back( s.substr( last, index - last ) );
    }
    return ss;
}

uint Utils::getHashCode( char const * buf, int len )
{
    uint seed = 131, hash = 0;
    for( int i = 0; i < len; ++i )
        hash = hash * seed + (uint8)buf[ i ];
    return hash;
}

bool Utils::ensureTouch( Node * o, Touch * touch )
{
    if( !o->isVisible() ) return false;
    auto n = o->getParent();
    CC_ASSERT( n );
    while( n )
    {
        if( !n->isVisible() ) return false;
        auto sv = dynamic_cast<ExtScrollView *>( n );
        if( sv )
        {
            if( sv->isTouchMoved() ) return false;                                      // 如果 sv 正在移动，点击不生效
            auto p = sv->getParent()->convertToNodeSpace( touch->getLocation() );
            if( !sv->getBoundingBox().containsPoint( p ) ) return false;                // 点击点 如果没有位于 sv 显示区域内，点击不生效
        }
        else
        {
            auto crn = dynamic_cast<ExtClippingNode*>( n );
            if( crn )
            {
                if( crn->isTouchMoved() ) return false;                                 // 如果 正在移动，点击不生效 ( 正在移动标志 为用户自己代码设置 )
                auto p = crn->getParent()->convertToNodeSpace( touch->getLocation() );
                if( !crn->getBoundingBox().containsPoint( p ) ) return false;           // 点击点 如果没有位于 sv 显示区域内，点击不生效
            }
            else
            {
                auto btn = dynamic_cast<ExtButton *>( n );
                if( btn )
                {
                    if( !btn->isEnabled() ) return false;                               // 如果 btn 未Enable，点击不生效
                }
            }
        }
        n = n->getParent();
    }
    return true;
}

bool Utils::isTouchInside( Node * o, Touch * touch )
{
    auto touchLocation = touch->getLocation();  // Get the touch position
    touchLocation = o->getParent()->convertToNodeSpace( touchLocation );
    auto bBox = o->getBoundingBox();
    return bBox.containsPoint( touchLocation );
}









Utils::ScopeGuard::ScopeGuard( FT f )
: _f( f )
{
}

Utils::ScopeGuard::~ScopeGuard()
{
    if( _f ) _f();
}

void Utils::ScopeGuard::runAndCancel()
{
    if( _f )
    {
        _f();
        _f = nullptr;
    }
}

void Utils::ScopeGuard::run()
{
    if( _f )
    {
        _f();
    }
}

void Utils::ScopeGuard::cancel()
{
    _f = nullptr;
}

void Utils::ScopeGuard::set( FT f )
{
    _f = f;
}










int Utils::fillCore( char * buf, char v )
{
    buf[ 0 ] = v;
    return 1;
}

int Utils::fillCore( char * buf, int v )
{
    return ::sprintf( buf, "%d", v );
}

int Utils::fillCore( char * buf, uint v )
{
    return ::sprintf( buf, "%u", v );
}

int Utils::fillCore( char * buf, float v )
{
    return ::sprintf( buf, "%.1f", v );
}

int Utils::fillCore( char * buf, double v )
{
    return ::sprintf( buf, "%.1f", v );
}

int Utils::fillCore( char * buf, string const & s )
{
    memcpy( buf, s.c_str(), s.size() );
    return s.size();
}






/*
UCS-4编码  | UTF-8字节流
-----------+-----------------------------------------------------------------
U+00000000 | U+0000007F 0xxxxxxx
U+00000080 | U+000007FF 110xxxxx 10xxxxxx
U+00000800 | U+0000FFFF 1110xxxx 10xxxxxx 10xxxxxx
U+00010000 | U+001FFFFF 11110xxx 10xxxxxx 10xxxxxx 10xxxxxx
U+00200000 | U+03FFFFFF 111110xx 10xxxxxx 10xxxxxx 10xxxxxx 10xxxxxx
U+04000000 | U+7FFFFFFF 1111110x 10xxxxxx 10xxxxxx 10xxxxxx 10xxxxxx 10xxxxxx
*/

// 返回指定半角数字符于 s 的实际长度（如果 s 是 buf， s[ rtv ] = 0 即可实现裁剪输出）
int Utils::getCutIndex( char const * s, int numHalfChars )
{
    int i = 0, num = 0;
    while( unsigned char c = s[ i ] )
    {
        if( c <= 0x7F )                     // 0xxxxxxx
        {
            ++i;
            if( ++num >= numHalfChars ) break;
        }
        else
        {
            if( ( c >> 5 ) == 0x06 )        // 110xxxxx
            {
                i += 2;
            }
            else if( ( c >> 4 ) == 0x0E )   // 1110xxxx
            {
                i += 3;
            }
            else if( ( c >> 3 ) == 0x1E )   // 11110xxx
            {
                i += 4;
            }
            else if( ( c >> 2 ) == 0x3E )   // 111110xx
            {
                i += 5;
            }
            else if( ( c >> 1 ) == 0x7E )   // 1111110x
            {
                i += 6;
            }
            else if( ( c >> 6 ) == 0x7E )   // 10xxxxxx
            {
                CC_ASSERT( false );
            }
            else if( c >= 0x80 )            // 1xxxxxxx
            {
                CC_ASSERT( false );
            }
            if( num + 2 >= numHalfChars ) break;
            num += 2;
        }
    }
    return i;
}

// 统计 utf8 串的字数( 宽字符算 1 长 )
int Utils::getWCharCount( char const * s )
{
    int n = 0;
    while( unsigned char c = *s++ ) if( ( 0xC0 & c ) != 0x80 ) ++n;
    return n;
}

// 统计 utf8 串中 宽字符 的个数
int Utils::getOnlyWCharCount( char const * s )
{
    int n = 0;
    while( unsigned char c = *s++ ) if( c > 0xC0 ) ++n;
    return n;
}

// 统计 utf8 串的字数( 宽字符算 2 长 )
int Utils::getCharCount( char const * s )
{
    int i = 0, num = 0;
    while( unsigned char c = s[ i ] )
    {
        if( c <= 0x7F )                     // 0xxxxxxx
        {
            ++i;
            ++num;
        }
        else
        {
            if( ( c >> 5 ) == 0x06 )        // 110xxxxx
            {
                i += 2;
            }
            else if( ( c >> 4 ) == 0x0E )   // 1110xxxx
            {
                i += 3;
            }
            else                            // 当前编码不可能出现更长的 utf8 码故不处理
            {
                CC_ASSERT( false );
            }
            //else if( ( c >> 3 ) == 0x1E )   // 11110xxx
            //{
            //    i += 4;
            //}
            //else if( ( c >> 2 ) == 0x3E )   // 111110xx
            //{
            //    i += 5;
            //}
            //else if( ( c >> 1 ) == 0x7E )   // 1111110x
            //{
            //    i += 6;
            //}
            //else if( ( c >> 6 ) == 0x7E )   // 10xxxxxx
            //{
            //    CC_ASSERT( false );
            //}
            //else if( c >= 0x80 )            // 1xxxxxxx
            //{
            //    CC_ASSERT( false );
            //}
            num += 2;
        }
    }
    return num;
}


// 将 wchar（只支持到 ucs2）转为 "\uXXXX" 的 16进制 字串 形态 写入 buf
void Utils::fillU16( char * buf, ushort c )
{
    static char dict[] = { '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'a', 'b', 'c', 'd', 'e', 'f' };
    buf[ 0 ] = '\\';
    buf[ 1 ] = 'u';
    buf[ 2 ] = dict[ ( c >> 12 ) & 0x0F ];
    buf[ 3 ] = dict[ ( c >> 8 ) & 0x0F ];
    buf[ 4 ] = dict[ ( c >> 4 ) & 0x0F ];
    buf[ 5 ] = dict[ ( c >> 0 ) & 0x0F ];
}

// 将 s: utf8 只支持到 ucs2）串转义为 json 字串填进 buf ( " -> \"  ,  宽字符-> \uXXXX ), 返回转换后的长
int Utils::fillToJson( char * buf, int bufLen, char const * s, int sLen )
{
    if( !s ) return 0;
    int bufIdx = 0;
    ushort wc = 0;
    for( int sIdx = 0; sIdx < sLen; ++sIdx )
    {
        byte c = s[ sIdx ];
        if( c < 0x80 )                              // 10000000
        {
            if( c == '\\' )
            {
                assert( bufIdx + 2 <= bufLen );
                buf[ bufIdx + 0 ] = '\\';
                buf[ bufIdx + 1 ] = '\\';
                bufIdx += 2;
            }
            else if( c == '\"' )
            {
                assert( bufIdx + 2 <= bufLen );
                buf[ bufIdx + 0 ] = '\\';
                buf[ bufIdx + 1 ] = '\"';
                bufIdx += 2;
            }
            else
            {
                assert( bufIdx + 1 <= bufLen );
                buf[ bufIdx++ ] = c;
            }
        }
        else
        {
            assert( bufIdx + 6 <= bufLen );
            if( ( c & 0xE0 ) == 0xE0 )              // 1110 xxxx
            {
                wc = ( ( c & 0x0F ) << 12 ) | ( ( s[ sIdx + 1 ] & 0x3F ) << 6 ) | ( s[ sIdx + 2 ] & 0x3F );
                sIdx += 2;
            }
            else if( ( c & 0xC0 ) == 0xC0 )         // 110x xxxx
            {
                wc = ( ( c & 0x1F ) << 6 ) | ( s[ ++sIdx ] & 0x3F );
            }
            assert( sIdx <= sLen );
            fillU16( buf + bufIdx, wc );
            bufIdx += 6;
        }
    }
    return bufIdx;
}


int Utils::fillWChar( ushort * buf, char const * s )
{
    auto p = (byte*)s;
    if( p[ 0 ] < 0x80 )
    {
        *buf = p[ 0 ];
        return 1;
    }
    else if( ( p[ 0 ] & 0xE0 ) == 0xE0 )
    {
        *buf = ( ( p[ 0 ] & 0x0F ) << 12 ) | ( ( ( p[ 1 ] ) & 0x3F ) << 6 ) | ( ( p[ 2 ] ) & 0x3F );
        return 3;
    }
    else if( ( p[ 0 ] & 0xC0 ) == 0xC0 )
    {
        *buf = ( ( p[ 0 ] & 0x1F ) << 6 ) | ( p[ 1 ] & 0x3F );
        return 2;
    }
    else return 1;
}


int Utils::fillUtf8( char * buf, ushort c )
{
    if( c < 0x80 )
    {
        buf[ 0 ] = (char)c;
        return 1;
    }
    else if( c < 0x800 )
    {
        buf[ 0 ] = ( ( c >> 6 ) & 0x1F ) | 0xC0;
        buf[ 1 ] = ( c & 0x3F ) | 0x80;
        return 2;
    }
    else
    {
        buf[ 0 ] = ( ( c >> 12 ) & 0x0F ) | 0xE0;
        buf[ 1 ] = ( ( c >> 6 ) & 0x3F ) | 0x80;
        buf[ 2 ] = ( c & 0x3F ) | 0x80;
        return 3;
    }
}
























Sprite * Utils::createSwitchButtonSprite( string const & fileName )
{
    auto t = Director::getInstance()->getTextureCache()->addImage( fileName );
    if( !t ) return nullptr;
    auto ts = t->getContentSize();
    auto s = Sprite::createWithTexture( t, Rect( 0, 0, ts.width / 2, ts.height ) );
    if( !s ) return nullptr;
    auto listener = EventListenerTouchOneByOne::create();
    listener->setSwallowTouches( true );
    listener->onTouchBegan = [ s ]( Touch * t, Event * e )
    {
        if( !s->getBoundingBox().containsPoint(
            s->getParent()->convertToNodeSpace( t->getLocation() )
            ) ) return false;
        auto r = s->getTextureRect();
        if( r.origin.x ) r.origin.x = 0;
        else r.origin.x = r.size.width;
        s->setTextureRect( r );
        return false;
    };
    s->getEventDispatcher()->addEventListenerWithSceneGraphPriority( listener, s );
    return s;
}

Node * Utils::drawRect( Node * n )
{
    auto wh = n->getContentSize();
    vector<Vec2> ps;
    auto p = n->getPosition();  // 逐个算出每个顶点的坐标

    // 左下
    p.x -= wh.width / 2;
    p.y -= wh.height / 2;
    ps.push_back( p );

    // 左上
    p.y += wh.height;
    ps.push_back( p );

    // 右上
    p.x += wh.width;
    ps.push_back( p );

    // 右下
    p.y -= wh.height;
    ps.push_back( p );

    // 画框
    auto dn = DrawNode::create();
    dn->drawPolygon( ps.data(), ps.size(), Color4F( 1, 0, 0, 0 ), 1, Color4F( 0, 0, 1, 1 ) );

    // 根据 anchor 相关来调偏移
    auto ap = n->getAnchorPoint();
    if( n->isIgnoreAnchorPointForPosition() ) ap = Vec2::ANCHOR_BOTTOM_LEFT;
    dn->setPosition( wh.width / 2 - wh.width * ap.x, wh.height / 2 - wh.height * ap.y );

    auto rtv = Node::create();
    rtv->addChild( dn );
    return rtv;
}


Node * Utils::drawRichTextRect( Node * n )
{
    auto wh = n->getContentSize();
    vector<Vec2> ps;
    auto p = n->getPosition();  // 逐个算出每个顶点的坐标

    // 左下
    p.x -= wh.width / 2;
    p.y -= wh.height / 2;
    ps.push_back( p );

    // 左上
    p.y += wh.height;
    ps.push_back( p );

    // 右上
    p.x += wh.width;
    ps.push_back( p );

    // 右下
    p.y -= wh.height;
    ps.push_back( p );

    // 画框
    auto dn = DrawNode::create();
    dn->drawPolygon( ps.data(), ps.size(), Color4F( 1, 0, 0, 0 ), 1, Color4F( 0, 0, 1, 1 ) );

    // 根据 anchor 相关来调偏移
    auto ap = n->getAnchorPoint();
    if( n->isIgnoreAnchorPointForPosition() ) ap = Vec2::ANCHOR_BOTTOM_LEFT;
    dn->setPosition( wh.width / 2 - wh.width * ap.x, wh.height / 2 - wh.height * ap.y - wh.height );

    auto rtv = Node::create();
    rtv->addChild( dn );
    return rtv;
}


Sprite * Utils::createDragableSprite( string const & fileName )
{
    auto s = Sprite::create( fileName );
    if( !s ) return nullptr;
    auto listener = EventListenerTouchOneByOne::create();
    listener->setSwallowTouches( true );
    auto p = new Vec2();
    listener->onDispose = [ p ] { delete p; };
    listener->onTouchBegan = [ s, p ]( Touch * t, Event * e )
    {
        if( !s->getBoundingBox().containsPoint(
            s->getParent()->convertToNodeSpace( t->getLocation() ) ) ) return false;
        *p = t->getLocation();
        return true;
    };
    listener->onTouchMoved = [ s, p ]( Touch * t, Event * e )
    {
        auto loc = t->getLocation();
        auto pos = s->getPosition();
        pos.x += loc.x - p->x;
        pos.y += loc.y - p->y;
        s->setPosition( pos );
        *p = loc;
    };
    s->getEventDispatcher()->addEventListenerWithSceneGraphPriority( listener, s );
    return s;
}

void Utils::trim( string& s )
{
    static string whitespaces( " \t\f\v\n\r" );

    size_t found = s.find_last_not_of( whitespaces );
    if( found != string::npos )
    {
        s.erase( found + 1 );

        // trim left
        found = s.find_first_not_of( whitespaces );
        if( found != string::npos )
            s.erase( 0, found );
    }
    else
        s.clear();
}
