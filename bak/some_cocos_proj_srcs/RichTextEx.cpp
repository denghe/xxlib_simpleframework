#include "Precompile.h"
using namespace RichTextHelper;

// 单个渲染对象及属性
struct Item
{
    float                       w;                  // 宽( 算上 scale 之后的实际的 )，用于对齐计算
    float                       h;                  // 高( 同上 )，用于对齐计算
    VerticalAligns              av;                 // 纵对齐方式
    bool                        ul;                 // 是否开启下划线
    float                       ult;                // 下划线宽
    Color3B                     ulc;                // 下划线颜色
    float                       uly;                // 下划线偏移量
    bool                        btn;                // 是否开启点击( 点击区域理论上讲应该是和当前行高相同，宽度等同于显示宽度 )
    string                      btnKey;             // 点击后传给回调的参数
    Node*                       node;
};

// 块对象容器（通常情况下，如果没有 align 指令的出现，一行只有一块）
struct Block : vector < Item >
{
    HorizontalAligns            ah;                 // 横对齐方式
    float                       x;                  // 这个值被用于剩余长度存储
};

// 一行由多块组成，有多少块取决于 align(横) 指令在当前行出现了多少次
struct Row : vector < Block >
{
    float                       lh;                 // 实际行高, 换行后恢复默认行高
};

// 段落( Paragraph, Header ) 是一个长方型显示区块
struct Paragraph
{
    RichTextEx*                 rt;                 // 最终容器( 方便取回调函数啥的 )
    Node*                       container;          // 绘制容器
    // 下面所有变量，都是指 "当前" 状态
    Row                         row;                // 行( 换行时清空复用 )
    float                       lh, lw;             // 默认行高，行宽( 这个值是不变的 )
    float                       y;                  // 光标 y 轴位置( x 在 block 里 要根据对齐方式换算 )
    int                         f;                  // 字体索引 于 font manager
    Color3B                     c;                  // 颜色
    bool                        ul;                 // 是否开启下划线
    float                       ult;                // 下划线宽
    Color3B                     ulc;                // 下划线颜色
    float                       uly;                // 下划线偏移量
    bool                        btn;                // 是否开启点击( 点击区域理论上讲应该是和当前行高相同，宽度等同于显示宽度 )
    string                      btnKey;             // 点击后传给回调的参数
    VerticalAligns              av;                 // 纵向对齐方式
    HorizontalAligns            ah;                 // 横向对齐方式
    vector<Rect>                toucheAreas;        // 点击区域

    Paragraph( Node* container, float lineWidth, RichTextEx* rt );
    void newItem( Node* node, float w, float h );
    void newBlock();
    void newLine();
    Block& currBlock();
    float space();
};

Block& Paragraph::currBlock()
{
    return row[ row.size() - 1 ];
}

float Paragraph::space()
{
    return currBlock().x;
}

Paragraph::Paragraph( Node* container, float lineWidth, RichTextEx* rt )
{
    auto fm = FontManager::getInstance();
    this->rt = rt;
    this->container = container;
    this->lw = lineWidth;
    this->lh = fm->getCharSize( 0, ' ' ).height;
    this->y = 0;
    this->f = 0;
    this->c = fm->getDefaultColor();
    this->ul = false;
    this->ult = 2;
    this->ulc = Color3B::RED;
    this->uly = 3;
    this->btn = false;
    this->av = VerticalAligns::Middle;
    this->ah = HorizontalAligns::Left;
    this->row.lh = lh;
    newBlock();
}

void Paragraph::newBlock()
{
    Block b;
    b.ah = this->ah;
    b.x = this->lw;
    row.push_back( b );
}

void Paragraph::newItem( Node* node, float w, float h )
{
    if( currBlock().x < w ) newLine();

    Item o;
    o.w = w;
    o.h = h;
    o.av = av;
    o.ul = ul;
    o.ult = ult;
    o.ulc = ulc;
    o.uly = uly;
    o.btn = btn;
    o.btnKey = btnKey;
    o.node = node;
    o.av = av;
    if( row.lh < o.h ) row.lh = o.h;

    auto& b = currBlock();
    b.push_back( o );
    if( b.ah == HorizontalAligns::Left )
    {
        o.node->setPositionX( lw - b.x );
        b.x -= o.w;
    }
    else
    {
        b.x -= o.w;
        float posX = ( b.ah == HorizontalAligns::Right ) ? b.x : ( b.x / 2 );     // 算出左边距
        for( auto& o : b )
        {
            o.node->setPositionX( posX );
            posX += o.w;                                    // 逐个调整
        }
    }
}

void Paragraph::newLine()
{
    for( auto& b : row )
    {
        for( auto& o : b )
        {
            switch( o.av )
            {
            case VerticalAligns::Top:
                o.node->setPositionY( y );
                break;
                default:
            case VerticalAligns::Middle:
                o.node->setPositionY( y - row.lh / 2 + o.h / 2 );
                break;
            case VerticalAligns::Bottom:
                o.node->setPositionY( y - row.lh + o.h / 2 );
                break;
            }
            container->addChild( o.node );

            // 用一个 sprite 调 scaleX Y 来仿长度和线宽 用于绘制下划线
            // plan.A: 每个 node 配一个 拉伸到等同于 o.w 来显示在 o.x, y + lh - uly
            if( o.ul )
            {
                auto spr = Sprite::create( "ul.png" );  // 4 * 4               todo: 这个图还没有
                spr->setAnchorPoint( Vec2::ANCHOR_BOTTOM_LEFT );
                spr->setScaleX( o.w / 4 );
                spr->setScaleY( o.ult / 4 );
                spr->setColor( o.ulc );
                spr->setPosition( o.node->getPositionX(), y - row.lh + uly );
                container->addChild( spr );
            }
            // plan.B: 合并显示 todo

            // 点击区域处理
            // plan.A: 每个 node 都设 touch listener
            if( o.btn )
            {
                auto listener = EventListenerTouchOneByOne::create();
                listener->setSwallowTouches( true );        // todo: 这个要不要令 rich text 接收一个入参以确认自己是否位于 ScrollView 之类控件 须考虑
                auto node = o.node;
                auto key = btnKey;
                auto rt = this->rt;
                listener->onTouchBegan = [ node, key, rt ]( Touch * t, Event * e )
                {
                    if( !node->getBoundingBox().containsPoint(
                        node->getParent()->convertToNodeSpace( t->getLocation() )
                        ) ) return false;
                    rt->_touchHandler( key );
                    return true;
                };
                node->getEventDispatcher()->addEventListenerWithSceneGraphPriority( listener, node );
            }
            // plan.B: 记录所有区域，只在 container 里注册 todo
        }
    }
    y -= row.lh;
    row.clear();
    row.lh = lh;                    // 恢复默认行高
    ah = HorizontalAligns::Left;    // 恢复左对齐
    newBlock();
}

struct Obj
{
    Obj( RTCmds cmd
         , int ip1 = 0, int ip2 = 0
         , float fp1 = 0, float fp2 = 0
         , string sp1 = ""
         , Obj* parent = nullptr )
         : cmd( cmd )
         , ip1( ip1 )
         , ip2( ip2 )
         , fp1( fp1 )
         , fp2( fp2 )
         , sp1( sp1 )
         , parent( parent )
    {
    }
    ~Obj()
    {
        for( auto &o : objs ) delete o;
    }
    RTCmds cmd;
    int ip1;
    int ip2;
    float fp1;
    float fp2;
    string sp1;
    vector<Obj*> objs;
    Obj* parent;
};

Obj* analyze( Analyzer& a )
{
    auto o = new Obj( RTCmds::Paragraph );
    a.setFont = [ &]( int fontIndex )
    {
        o->objs.push_back( new Obj( RTCmds::Font, fontIndex ) );
        return true;
    };
    a.setColor = [ &]( Color3B c )
    {
        o->objs.push_back( new Obj( RTCmds::Color, *(int*)&c ) );
    };
    a.setOffset = [ &]( float offset )
    {
        o->objs.push_back( new Obj( RTCmds::Offset, 0, 0, offset ) );
    };
    a.setAlign = [ &]( HorizontalAligns ah, VerticalAligns av )
    {
        o->objs.push_back( new Obj( RTCmds::Align, (int)ah, (int)av ) );
    };
    a.setLineHeight = [ &]( float lh, bool isRatio )
    {
        o->objs.push_back( new Obj( RTCmds::Height, isRatio ? 1 : 0, 0, lh ) );
    };
    a.addText = [ &]( string const & txt )
    {
        o->objs.push_back( new Obj( RTCmds::Text, 0, 0, 0, 0, txt ) );
    };
    a.addNewLine = [ &]
    {
        o->objs.push_back( new Obj( RTCmds::NewLine ) );
    };
    a.addImage = [ &]( string fn, bool isFileName, float w, float h )
    {
        o->objs.push_back( new Obj( RTCmds::Image, isFileName ? 1 : 0, 0, w, h, fn ) );
    };
    a.addSpace = [ &]( float w, float h )
    {
        o->objs.push_back( new Obj( RTCmds::Space, 0, 0, w, h ) );
    };
    a.pushParagraph = [ &]( float lw )
    {
        auto n = new Obj( RTCmds::Paragraph, 0, 0, lw, 0, "", o );
        o->objs.push_back( n );
        o = n;
        return true;
    };
    a.popParagraph = [ &]
    {
        o = o->parent;
    };
    a.pushButton = [ &]( string const& key )
    {
        o->objs.push_back( new Obj( RTCmds::Button, 0, 0, 0, 0, key ) );
    };
    a.popButton = [ &]
    {
        o->objs.push_back( new Obj( RTCmds::ButtonEnd ) );
    };
    a.pushUnderLine = [ &]( Color3B c, float thickness, float y )
    {
        o->objs.push_back( new Obj( RTCmds::UnderLine, *(int*)&c, 0, thickness, y ) );
    };
    a.popUnderLine = [ &]
    {
        o->objs.push_back( new Obj( RTCmds::UnderLineEnd ) );
    };
    if( !a.analyze() )
    {
        delete o;
        return nullptr;
    }
    return o;
}


bool fillNode( RichTextEx* rt, Obj* o, Node* container, float lineWidth, int fontIndex )
{
    auto fm = FontManager::getInstance();
    auto p = new Paragraph( container, lineWidth, rt );    // 本级上下文
    p->f = fontIndex;
    Utils::ScopeGuard sg( [ &] { delete p; } );

    for( auto& co : o->objs )
    {
        switch( co->cmd )
        {
        case RTCmds::Paragraph:
        {
            auto lw = co->fp1;
            if( lw == 0 ) lw = p->currBlock().x;

            auto n = Node::create();
            n->setAnchorPoint( Vec2::ANCHOR_BOTTOM_LEFT );
            n->setContentSize( Size( lw, 0 ) );
            if( !fillNode( rt, co, n, lw, p->f ) )
                return false;
            auto sz = n->getContentSize();
            p->newItem( n, lw, sz.height );
        } break;
        case RTCmds::ParagraphEnd:
        {
            rt->_err = "should not appear: ParagraphEnd";
            return false;
        }
        case RTCmds::UnderLine:
        {
            p->ul = true;
            p->ulc = *(Color3B*)&co->ip1;
            p->ult = co->fp1;
            p->uly = co->fp2;
        } break;
        case RTCmds::UnderLineEnd:
        {
            p->ul = false;
        } break;
        case RTCmds::Button:
        {
            p->btn = true;
            p->btnKey = co->sp1;
        } break;
        case RTCmds::ButtonEnd:
        {
            p->btn = false;
        } break;
        case RTCmds::Font:
        {
            auto fontIndex = co->ip1;
            if( fontIndex < 0 || fontIndex >= fm->getFontCount() )
            {
                rt->_err = "can't find font index:" + Utils::toString( fontIndex );
                return false;
            }
            p->f = fontIndex;
        } break;
        case RTCmds::Color:
        {
            p->c = *(Color3B*)&co->ip1;
        } break;
        case RTCmds::Offset:
        {
            auto offset = co->fp1;
            Block b;
            p->ah = b.ah = HorizontalAligns::Left;
            b.x = p->lw - offset;
            p->row.push_back( b );
        } break;
        case RTCmds::Image:
        {
            bool isFileName = co->ip1 == 1;
            auto& fn = co->sp1;
            auto w = co->fp1;
            auto h = co->fp2;

            Sprite* s = nullptr;
            if( isFileName )
            {
                s = Sprite::create( fn );
            }
            else
            {
                auto sf = SpriteFrameCache::getInstance()->getSpriteFrameByName( fn );
                if( !sf )
                {
                    rt->_err = "can't find sprite frame name :" + fn;
                    return false;
                }
                s = Sprite::createWithSpriteFrame( sf );
            }
            s->setAnchorPoint( Vec2::ANCHOR_TOP_LEFT );
            auto sz = s->getContentSize();
            if( w )
            {
                s->setScaleX( w / sz.width );
            }
            else
            {
                w = sz.width;
            }
            if( h )
            {
                s->setScaleY( h / sz.height );
            }
            else
            {
                h = sz.height;
            }
            p->newItem( s, w, h );
        } break;
        case RTCmds::Align:
        {
            auto ah = (HorizontalAligns)co->ip1;
            auto av = (VerticalAligns)co->ip2;

            if( av != VerticalAligns::Unknown )
            {
                p->av = av;
            }
            if( ah != HorizontalAligns::Unknown )
            {
                // 如果当前行已经有内容 且传入 ah 与之 ah 不同，则创建新 block， 相同则忽略
                if( p->currBlock().size() )
                {
                    if( p->ah != ah )
                    {
                        p->ah = ah;
                        p->newBlock();
                    }
                }
                // 如果没有内容 那直接改 ah
                else
                {
                    p->ah = ah;
                    p->currBlock().ah = ah;
                }
            }
        } break;
        case RTCmds::Space:
        {
            auto w = co->fp1;
            auto h = co->fp2;

            Node* n = Node::create();
            n->setAnchorPoint( Vec2::ANCHOR_BOTTOM_LEFT );
            n->setContentSize( Size( w, h ) );
            p->newItem( n, w, h );
        } break;
        case RTCmds::Height:
        {
            auto lh = co->fp1;
            bool isRatio = co->ip1 == 1;

            if( lh == 0 )   // 默认值为当前字体行高
            {
                p->lh = fm->getCharSize( p->f, ' ' ).height;
            }
            else if( isRatio )
            {
                p->lh = fm->getCharSize( p->f, ' ' ).height * lh;
            }
            else
            {
                p->lh = lh;
            }
            if( lh >= p->row.lh )  // 比当前行实际行高大，直接改
            {
                p->row.lh = lh;
            }
            else                    // 试压缩实际行高到当前行最大 h
            {
                float maxLH = 0;    // 找出最大值
                for( auto&b : p->row )
                {
                    for( auto&o : b )
                    {
                        if( o.h > maxLH ) maxLH = o.h;
                    }
                }
                p->row.lh = maxLH;
            }
        } break;
        case RTCmds::Variant:
        {
            rt->_err = "should not appear: Variant";
            return false;
        }
        case RTCmds::Text:
        {
            auto& txt = co->sp1;

            string currText;                    // 累积文本( 遇到状态改变时将渲染为单个 Label )
            int currTextWidth = 0;              // 累积文本的宽度
            currText.reserve( txt.size() );
            auto str = txt.c_str();
            auto strEnd = str + txt.size();

            auto newLabel = [ &]                // 绘制 currText 为 Label 并放入 p
            {
                Size sz;                        // 实际渲染尺寸
                auto lbl = fm->createLabel( p->f, currText );
                lbl->setAnchorPoint( Vec2::ANCHOR_TOP_LEFT );
                lbl->ignoreAnchorPointForPosition( false );
                sz = lbl->getContentSize();
                lbl->setColor( p->c );
                // maybe need set lbl ignorexxxxx anchorpoint ....
                p->newItem( lbl, sz.width, sz.height );

                currTextWidth = 0;              // 清累计串
                currText.clear();
            };

            while( str < strEnd )
            {
                int utf8len;
                auto cw = fm->getCharSize( p->f, str, utf8len ).width;      // 取 wchar 显示宽度
                if( ( currTextWidth + cw <= p->currBlock().x )              // 当前文本 加上 char 宽 如果没超宽, 则继续追加
                    || (currTextWidth == 0 && p->currBlock().x == p->lw) )  // 或者当前 block 无内容且为第1个字，也追加（ 以实现 如果 lw 小于 文字宽，至少显示 1 个字 的效果 ）
                {
                    currTextWidth += cw;
                    currText.append( str, utf8len );
                    str += utf8len;
                }
                else
                {
                    if (currTextWidth) newLabel();
                    p->newLine();
                }
            }
            if( currText.size() )   // 还有剩的
            {
                newLabel();
            }
        } break;
        case RTCmds::NewLine:
        {
            p->newLine();
        } break;
        }
    }

    p->newLine();
    container->setContentSize( Size( p->lw, -p->y ) );

    return true;
}




bool RichTextEx::assign( std::string const & txt )
{
    removeAllChildren();

    // 一些前置 check
    if( !FontManager::getInstance()->getColorCount() )
    {
        _err = "FontManager can't find default color!";
        return false;
    }
    if( !FontManager::getInstance()->getFontCount() )
    {
        _err = "FontManager can't find default font!";
        return false;
    }

    _a.assign( txt );
    auto o = analyze( _a );
    if( !o )
    {
        if(_errorHandler) _errorHandler( _err );
        return false;
    }
    auto rtv = fillNode( this, o, this, _lineWidth, 0 );
    delete o;
    if( !rtv )
    {
        if(_errorHandler) _errorHandler( _err );
    }
    else
    {
        // 算实际宽度: foreach 子，找出右侧边最大值，即 x + (1 - xAnchor) * w 的最大值
        float n = 0;
        for( auto c : this->getChildren() )
        {
            auto r = c->getPositionX() + ( 1.0f - c->getAnchorPoint().x ) * c->getContentSize().width;
            if( r > n ) n = r;
        }
        this->setContentSize( Size(n, this->getContentSize().height) );
    }
    return rtv;
}


RichTextEx * RichTextEx::create( string const & txt
                                 , float lineWidth
                                 , TouchHandlerType touchHandler
                                 , VariantHandlerType variantHandler
                                 , ErrorHandlerType errorHandler )
{
    auto ret = new RichTextEx();
    if( ret && ret->init() )
    {
        ret->setAnchorPoint( Vec2::ANCHOR_BOTTOM_LEFT );
        ret->setContentSize( Size( lineWidth, 0 ) );
        ret->autorelease();
        ret->_lineWidth = lineWidth;
        ret->_touchHandler = touchHandler;
        ret->_variantHandler = variantHandler;
        ret->_errorHandler = errorHandler;
        ret->init_analyzer();

        if( ret->_touchHandler && ret->_variantHandler && ret->_errorHandler )
        {
            ret->assign( txt );
        }
    }
    else
    {
        CC_SAFE_DELETE( ret );
    }
    return ret;
}

RichTextEx * RichTextEx::createLua( string const & txt
                                 , float lineWidth
                                 , LUA_FUNCTION touchHandler
                                 , LUA_FUNCTION variantHandler
                                 , LUA_FUNCTION errorHandler )
{
    auto ret = new RichTextEx();
    if( ret && ret->init() )
    {
        ret->setAnchorPoint( Vec2::ANCHOR_BOTTOM_LEFT );
        ret->setContentSize( Size( lineWidth, 0 ) );
        ret->autorelease();
        ret->_lineWidth = lineWidth;
        ret->registerTouchEventHandler( touchHandler );
        ret->registerVariantEventHandler( variantHandler );
        ret->registerErrorEventHandler( errorHandler );
        ret->init_analyzer();

        ret->assign( txt );
    }
    else
    {
        CC_SAFE_DELETE( ret );
    }
    return ret;
}


void RichTextEx::registerTouchEventHandler( TouchHandlerType handler )
{
    _touchHandler = handler;
}

void RichTextEx::registerTouchEventHandler( LUA_FUNCTION handler )
{
    if( !handler )
    {
        registerTouchEventHandler( nullptr );
        return;
    }

    TouchHandlerType f = [ handler ]( string const & key )
    {
        LuaHelper::push( key );
        LuaHelper::executeFunction( handler, 1 );
    };
    registerTouchEventHandler( f );
}


void RichTextEx::registerVariantEventHandler( VariantHandlerType handler )
{
    _variantHandler = handler;
}

void RichTextEx::registerVariantEventHandler( LUA_FUNCTION handler )
{
    if( !handler )
    {
        registerVariantEventHandler( nullptr );
        return;
    }
    VariantHandlerType f = [ handler ]( string const & key, string& val )
    {
        LuaHelper::push( key );
        LuaHelper::executeFunction( handler, 1, val );
        return val.size() > 0;
    };
    registerVariantEventHandler( f );
}


void RichTextEx::registerErrorEventHandler( ErrorHandlerType handler )
{
    _errorHandler = handler;
}

void RichTextEx::registerErrorEventHandler( LUA_FUNCTION handler )
{
    if( !handler )
    {
        registerErrorEventHandler( nullptr );
        return;
    }
    ErrorHandlerType f = [ handler ]( string const & err )
    {
        LuaHelper::push( err );
        LuaHelper::executeFunction( handler, 1 );
    };
    registerErrorEventHandler( f );
}


void RichTextEx::init_analyzer()
{
    auto fm = FontManager::getInstance();

    _a.setError_MissLeftAngleBrackets = [ &] { _err = string( "miss < at " ) + Utils::toString( _a.so() ); };
    _a.setError_MissRightAngleBrackets = [ &] { _err = string( "miss > at " ) + Utils::toString( _a.so() ); };
    _a.setError_MissComma = [ &] { _err = string( "miss , at" ) + Utils::toString( _a.so() ); };
    _a.setError_MissCommand = [ &] { _err = string( "miss command at " ) + Utils::toString( _a.so() ); };
    _a.setError_MissParameter = [ &] { _err = string( "miss parameter at" ) + Utils::toString( _a.so() ); };
    _a.setError_MissFontIndex = [ &]( int fontIdx ) { _err = string( "miss font index " ) + Utils::toString( fontIdx ) + string( " at " ) + Utils::toString( _a.so() ); };

    _a.setError_WrongCommandName = [ &]( string const& cmdName ) { _err = string( "wrong command name at " ) + Utils::toString( _a.so() ); };;
    _a.setError_WrongParameterFormat = [ &]( string const& format ) { _err = string( "wrong parameter format: " ) + format + string( " at " ) + Utils::toString( _a.so() ); };
    _a.setError_MissEndCommand = [ &]( string const& cmdName ) { _err = string( "miss </" ) + cmdName + string( "> at " ) + Utils::toString( _a.so() ); };
    _a.setError_LengthOverflow = [ &] { _err = string( "text length overflow: " ) + Utils::toString( _a.so() ) + string( ", limit: " ) + Utils::toString( _a.maxLen ); };
    _a.setError_StackOverflow = [ &] { _err = string( "stack overflow, limit: " ) + Utils::toString( _a.maxStackSize ); };

    _a.setError_GetColorByIndex = [ &]( int colorIdx ) { _err = string( "get color by index: " ) + Utils::toString( colorIdx ) + string( " was not found at " ) + Utils::toString( _a.so() ); };
    _a.setError_GetColorByName = [ &]( string const& colorName ) { _err = string( "get color by name: " ) + colorName + string( " was not found at " ) + Utils::toString( _a.so() ); };
    _a.setError_GetVariant = [ &]( string const& key ) { _err = string( "get variant by key: " ) + key + string( " was not found at " ) + Utils::toString( _a.so() ); };

    _a.getVariant = [ = ]( string const& key, string& val ) { return _variantHandler( key, val ); };
    _a.getColorByIndex = [ = ]( int colorIndex, Color3B& c ) { return fm->tryGetColor( colorIndex, c ); };
    _a.getColorByName = [ = ]( string const& colorName, Color3B& c ) { return fm->tryGetColor( colorName, c ); };
}
