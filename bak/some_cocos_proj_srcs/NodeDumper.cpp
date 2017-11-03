#include "Precompile.h"


NodeDumper::NodeDumper( string const & rootName /*= "n0"*/, int bufSize /*= 16384 */ )
: _bufIdx( 0 )
, _bufSize( bufSize )
, _rootName( rootName )
{
    _buf = new char[ bufSize ];
}

NodeDumper::~NodeDumper()
{
    delete[] _buf;
}

char* NodeDumper::getBuffer()
{
    _buf[ _bufIdx ] = '\0';
    return _buf;
}

void NodeDumper::clear()
{
    _bufIdx = 0;
}

void NodeDumper::dump( Node* node, string const & parentName /*= ""*/, string const & name /*= "" */, string const & addToParentCode1 /*= ""*/, string const & addToParentCode2 /*= ""*/ )
{
    if( !node ) return;
    auto currName = name;
    if( !name.size() ) currName = _rootName;
    do
    {
        // 按类型优先级试做类型转换
        // todo: more if( dumpXxxxx
        if( dumpExtButton( node, "", currName, addToParentCode1, addToParentCode2 ) ) break;
        if( dumpSprite( node, "", currName, addToParentCode1, addToParentCode2 ) ) break;
        if( dumpLabel( node, "", currName, addToParentCode1, addToParentCode2 ) ) break;
        if( dumpLayer( node, "", currName, addToParentCode1, addToParentCode2 ) ) break;
        dumpNode( node, "", currName, addToParentCode1, addToParentCode2 );
    } while( 0 );
}

void NodeDumper::dumpChilds( Node* node, string const & name, string const & addToParentCode1 /*= ""*/, string const & addToParentCode2 /*= ""*/ )
{
    char buf[ 32 ];
    int cidx = -1;
    for( const auto& cn : node->getChildren() )
    {
        ++cidx;
        buf[ Utils::fill( buf, cidx ) ] = 0;
        auto child_name = name + "_" + buf;
        dump( cn, name, child_name, addToParentCode1, addToParentCode2 );
    }
}

bool NodeDumper::dumpExtButton( Node* node, string const & parentName, string const & name, string const & addToParentCode1 /*= ""*/, string const & addToParentCode2 /*= ""*/, bool inherit /*= false*/ )
{
    auto o = dynamic_cast<ExtButton *>( node );
    if( !o ) return false;
    auto bg = o->getBg();
    dump( bg, name, name + "_bg" );
    auto fg = o->getFg();
    if( fg )
    {
        dump( fg, name, name + "_fg" );
        _bufIdx += Utils::fill( _buf + _bufIdx, "auto ", name, " = ExtButton::create( ", name + "_bg", ", ", name + "_fg", " );\r\n" );
    }
    else
    {
        _bufIdx += Utils::fill( _buf + _bufIdx, "auto ", name, " = ExtButton::create( ", name + "_bg );\r\n" );
    }
    return true;
}

bool NodeDumper::dumpSprite( Node* node, string const & parentName, string const & name, string const & addToParentCode1 /*= ""*/, string const & addToParentCode2 /*= ""*/, bool inherit /*= false*/ )
{
    auto o = dynamic_cast<Sprite *>( node );
    if( !o ) return false;
    auto sf = o->getSpriteFrame();
    auto& re = sf->getRect();
    auto te = sf->getTexture();
    auto fn = te->getFilePath();
    bool skipContentSize = true;
    if( fn.size() )
    {
        _bufIdx += Utils::fill( _buf + _bufIdx, "auto ", name, " = cocos2d::Sprite::create( \"", fn, "\"", ", Rect( " );
        _bufIdx += Utils::fill( _buf + _bufIdx, re.origin.x, ", ", re.origin.y, ", ", re.size.width, ", ", re.size.height, " ) );\r\n" );
        skipContentSize = true;
    }
    else
    {
        _bufIdx += Utils::fill( _buf + _bufIdx, "auto ", name, " = cocos2d::Sprite::create();\r\n" );
        // set a special picture ?
        skipContentSize = false;
    }
    dumpNode( node, parentName, name, addToParentCode1, addToParentCode2, true, skipContentSize );
    return true;
}

bool NodeDumper::dumpLabel( Node* node, string const & parentName, string const & name, string const & addToParentCode1 /*= ""*/, string const & addToParentCode2 /*= ""*/, bool inherit /*= false*/ )
{
    auto o = dynamic_cast<Label *>( node );
    if( !o ) return false;
    // todo
    return true;
}

bool NodeDumper::dumpLayer( Node* node, string const & parentName, string const & name, string const & addToParentCode1 /*= ""*/, string const & addToParentCode2 /*= ""*/, bool inherit /*= false*/ )
{
    auto o = dynamic_cast<Layer *>( node );
    if( !o ) return false;
    if( !inherit ) _bufIdx += Utils::fill( _buf + _bufIdx, "auto ", name, " = cocos2d::Layer::create();\r\n" );

    dumpNode( node, parentName, name, addToParentCode1, addToParentCode2, true );

    if( !inherit ) dumpChilds( node, name, name + "->addChild( ", " );" );
    return true;
}

void NodeDumper::dumpNode( Node* node, string const & parentName, string const & name, string const & addToParentCode1 /*= ""*/, string const & addToParentCode2 /*= ""*/, bool inherit /*= false*/, bool skipContentSize /*= false*/ )
{
    if( !node ) return;

    if( !inherit ) _bufIdx += Utils::fill( _buf + _bufIdx, "auto ", name, " = cocos2d::Node::create();\r\n" );

    _bufIdx += Utils::fill( _buf + _bufIdx, name, "->setLocalZOrder( ", node->getLocalZOrder(), " );\r\n" );
    _bufIdx += Utils::fill( _buf + _bufIdx, name, "->setScaleX( ", node->getScaleX(), " );\r\n" );
    _bufIdx += Utils::fill( _buf + _bufIdx, name, "->setScaleY( ", node->getScaleY(), " );\r\n" );
    _bufIdx += Utils::fill( _buf + _bufIdx, name, "->setPosition( cocos2d::Vec2( ", node->getPosition().x, ", ", node->getPosition().y, " ) );\r\n" );
    _bufIdx += Utils::fill( _buf + _bufIdx, name, "->setAnchorPoint( cocos2d::Vec2( ", node->getAnchorPoint().x, ", ", node->getAnchorPoint().y, " ) );\r\n" );
    _bufIdx += Utils::fill( _buf + _bufIdx, name, "->setContentSize( cocos2d::Size( ", node->getContentSize().width, ", ", node->getContentSize().height, " ) );\r\n" );
    _bufIdx += Utils::fill( _buf + _bufIdx, name, "->setVisible( ", node->isVisible(), " );\r\n" );
    _bufIdx += Utils::fill( _buf + _bufIdx, name, "->setRotation( ", node->getRotation(), " );\r\n" );
    if( addToParentCode1.size() ) _bufIdx += Utils::fill( _buf + _bufIdx, addToParentCode1, name, addToParentCode2, "\r\n" );
    _bufIdx += Utils::fill( _buf + _bufIdx, name, "->setOrderOfArrival( ", node->getOrderOfArrival(), " );\r\n" );
    _bufIdx += Utils::fill( _buf + _bufIdx, name, "->ignoreAnchorPointForPosition( ", node->isIgnoreAnchorPointForPosition(), " );\r\n" );
    // g/s etTag
    // g/s etUserData
    // g/s etUserObject
    _bufIdx += Utils::fill( _buf + _bufIdx, name, "->setOpacity( ", node->getOpacity(), " );\r\n" );
    _bufIdx += Utils::fill( _buf + _bufIdx, name, "->setCascadeOpacityEnabled( ", node->isCascadeOpacityEnabled(), " );\r\n" );
    _bufIdx += Utils::fill( _buf + _bufIdx, name, "->setColor( Color3B( ", node->getColor().r, ", ", node->getColor().g, ", ", node->getColor().b, " ) );\r\n" );
    _bufIdx += Utils::fill( _buf + _bufIdx, name, "->setCascadeColorEnabled( ", node->isCascadeColorEnabled(), " );\r\n" );
    // g/s etOpacityModifyRGB

    if( !inherit ) dumpChilds( node, name, name + "->addChild( ", " );" );
}

void NodeDumper::saveToFile( string const & fn )
{
    auto fullPath = Global::WritablePath + fn;
    Utils::fileWriteBytes( fullPath, _buf, _bufIdx );
}
