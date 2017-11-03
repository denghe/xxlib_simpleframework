#include "Precompile.h"

enum
{
    kZoomActionTag = 0xCCCB1111,
};

const float MOVE_INCH = 15.5f / 160.0f;

ExtButton::ExtButton()
    : _fg( nullptr )
    , _bg( nullptr )
    , _pushed( false )
    , _highlighted( false )
    , _zoomOnTouchDown( true )
    , _swallow( true )
    , _state( ButtonStates::Normal )
    , _touchListener( nullptr )
    , _dispatcher( nullptr )
    , _mutexNumber( 0 )
    , _firstTouch( nullptr )
{
}

ExtButton::~ExtButton()
{
    clearTouchLock();
}

void ExtButton::update( float df )
{
    _currFrameTouched = false;                                                // 防双击相关: 每 frame 开始时, 清除标记
}

void ExtButton::setEnabled( bool b )
{
    if( _enableHandler ) _enableHandler( this, b );
    // needsLayout();
    if( b )
    {
        if( _touchListener ) return;
        _state = ButtonStates::Normal;
        _touchListener = EventListenerTouchOneByOne::create();
        _touchListener->setSwallowTouches( _swallow );
        _touchListener->onTouchBegan = [ this ]( Touch * t, Event * e ) { return handleTouchBegan( t, e ); };
        _touchListener->onTouchMoved = [ this ]( Touch * t, Event * e ) { handleTouchMoved( t, e ); };
        _touchListener->onTouchEnded = [ this ]( Touch * t, Event * e ) { handleTouchEnded( t, e ); };
        _touchListener->onTouchCancelled = [ this ]( Touch * t, Event * e ) { handleTouchCancelled( t, e ); };
        _dispatcher->addEventListenerWithSceneGraphPriority( _touchListener, this );
    }
    else
    {
        clearTouchLock();
        if( _touchListener == nullptr ) return;
        _state = ButtonStates::Disabled;
        _eventDispatcher->removeEventListener( _touchListener );
        _touchListener = nullptr;
    }
}

void ExtButton::setHighlighted( bool b )
{
    _state = b ? ButtonStates::HighLighted : ButtonStates::Normal;
    _highlighted = b;
    if( _highlightHandler ) _highlightHandler( this, b );

    if( auto action = getActionByTag( kZoomActionTag ) )
    {
        stopAction( action );
    }
    // needsLayout();
    if( _zoomOnTouchDown )
    {
        float scaleValue = ( _highlighted && _touchListener ) ? 1.1f : 1.0f;
        auto zoomAction = ScaleTo::create( 0.05f, scaleValue );
        zoomAction->setTag( kZoomActionTag );
        runAction( zoomAction );
    }
}

void ExtButton::callTouchEventHandler( ButtonTouchEvents te, Touch * pTouch )
{
    if( _touchEventHandler )
    {
        auto loc = pTouch->getLocation();
        _touchEventHandler( this, te, loc.x, loc.y );
    }
}

bool ExtButton::handleTouchBegan( Touch * pTouch, Event * pEvent )
{
    // 如果不是第 1 次点击，handle 并退出
    if( _firstTouch ) return true;

    if( !Utils::isTouchInside( this, pTouch ) || !Utils::ensureTouch( this, pTouch ) )
    {
        return false;
    }

    auto result = _mutexNumbers.insert( make_pair( _mutexNumber, this ) );
    if( !result.second )
    {
        CCLOG( "touch locked. lock num = %d, tag = %d", result.first->first, result.first->second->getTag() );
        return false;
    }

    _firstTouch = pTouch;

    _pushed = true;
    setHighlighted( true );

    callTouchEventHandler( ButtonTouchEvents::Down, pTouch );
    return true;
}

void ExtButton::handleTouchMoved( Touch * pTouch, Event * pEvent )
{
    // 如果不是第 1 次点击，直接退出
    if( _firstTouch != pTouch ) return;

	// 容差处理 
	auto dis = pTouch->getStartLocation().distance( pTouch->getLocation() );
	auto dis_inch = convertDistanceFromPointToInch( dis );
	if ( dis_inch <= MOVE_INCH ) return;

    if( !_pushed )
    {
        if( _highlighted )
        {
            setHighlighted( false );
        }
        return;
    }

    bool isTouchMoveInside = Utils::isTouchInside( this, pTouch ) && Utils::ensureTouch( this, pTouch );
    if( isTouchMoveInside )
    {
        if( _highlighted )
        {
            callTouchEventHandler( ButtonTouchEvents::DragInside, pTouch );
        }
        else
        {
            setHighlighted( true );
            callTouchEventHandler( ButtonTouchEvents::DragEnter, pTouch );
        }
    }
    else
    {
        if( _highlighted )
        {
            setHighlighted( false );
            callTouchEventHandler( ButtonTouchEvents::DragExit, pTouch );
        }
        else
        {
            callTouchEventHandler( ButtonTouchEvents::DragOutside, pTouch );
        }
    }
}

void ExtButton::handleTouchEnded( Touch * pTouch, Event * pEvent )
{
    // 如果不是第 1 次点击
    if( _firstTouch != pTouch ) return;
    _firstTouch = nullptr;

    clearTouchLock();

    if( _currFrameTouched ) return;                           // 防双击相关: 每 frame 开始时, 清除标记
    _currFrameTouched = true;

    _pushed = false;
    setHighlighted( false );

    if( Utils::isTouchInside( this, pTouch ) && Utils::ensureTouch( this, pTouch ) )
    {
        callTouchEventHandler( ButtonTouchEvents::UpInside, pTouch );
    }
    else
    {
        callTouchEventHandler( ButtonTouchEvents::UpOutside, pTouch );
    }
}

void ExtButton::handleTouchCancelled( Touch * pTouch, Event * pEvent )
{
    clearTouchLock();

    _pushed = false;
    setHighlighted( false );
    callTouchEventHandler( ButtonTouchEvents::Cancel, pTouch );
}

void ExtButton::setBg( Node * bg )
{
    if( _bg == bg ) return;
    if( _bg ) _bg->removeFromParentAndCleanup( true );
    _bg = bg;
    addChild( bg, 1 );
    needsLayout();
}

Node * ExtButton::getBg() const
{
    return _bg;
}


void ExtButton::setFg( Node * fg )
{
    if( _fg == fg ) return;
    if( _fg ) _fg->removeFromParentAndCleanup( true );
    _fg = fg;
    addChild( fg, 2 );
}


Node * ExtButton::getFg() const
{
    return _fg;
}


void ExtButton::needsLayout()
{
    if( _bg )
    {
        // _bg->setGray( getState() == ControlStateDisabled );
        auto siz = _bg->getContentSize();
        auto scaleX = _bg->getScaleX();
        auto scaleY = _bg->getScaleY();
        setContentSize( Size( siz.width * scaleX, siz.height * scaleY ) );
        _bg->setPosition( Vec2( getContentSize().width / 2, getContentSize().height / 2 ) );
    }
    if( _fg )
    {
        _fg->setPosition( Vec2( getContentSize().width / 2, getContentSize().height / 2 ) );
    }
}

bool ExtButton::init( Node * bg, Node * fg, bool swallow, int mutexNumber )
{
    if( !Layer::init() ) return false;

    _dispatcher = Director::getInstance()->getEventDispatcher();
    _swallow = swallow;
    _mutexNumber = mutexNumber;
    setEnabled( true ); // 初始化 touch 监听器

    CCAssert( bg != nullptr, "background node must not be nil." );

    // Set the default anchor point
    ignoreAnchorPointForPosition( false );
    setAnchorPoint( Vec2( 0.5f, 0.5f ) );

    setFg( fg );
    setBg( bg );    // 内含 needsLayout 函数的调用

    //// Set the default color and opacity ( 功用不明 先干掉 )
    //setColor( Color3B( 255.0f, 255.0f, 255.0f ) );
    //setOpacity( 255.0f );
    //setOpacityModifyRGB( true );

    return true;
}

ExtButton * ExtButton::create( Node* bg, Node * fg, bool swallow, int mutexNumber )
{
    ExtButton * pRet = new ExtButton();

    if( !pRet->init( bg, fg, swallow, mutexNumber ) )
    {
        delete pRet;
        return nullptr;
    }
    pRet->autorelease();
    return pRet;
}




void ExtButton::registerTouchEventHandler( LUA_FUNCTION h )
{
    registerTouchEventHandler( [ this, h ]( ExtButton * s, ButtonTouchEvents et, float x, float y )
    {
        LuaHelper::push( s, "ExtButton" );
        LuaHelper::push( (int)et );
        LuaHelper::push( x );
        LuaHelper::push( y );
        LuaHelper::executeFunction( h, 4 );
    } );
}

void ExtButton::registerTouchEventHandler( TouchEventHandlerType h )
{
    _touchEventHandler = h;
}

void ExtButton::unregisterTouchEventHandler()
{
    _touchEventHandler = nullptr;
}




void ExtButton::registerHighlightHandler( LUA_FUNCTION h )
{
    registerHighlightHandler( [ this, h ]( ExtButton * s, bool b )
    {
        LuaHelper::push( this, "ExtButton" );
        LuaHelper::push( b );
        LuaHelper::executeFunction( h, 2 );
    } );
}

void ExtButton::registerHighlightHandler( HighlightHandlerType h )
{
    _highlightHandler = h;
}

void ExtButton::unregisterHighlightHandler()
{
    _highlightHandler = nullptr;
}




void ExtButton::registerEnableHandler( LUA_FUNCTION h )
{
    registerEnableHandler( [ this, h ]( ExtButton * s, bool b )
    {
        LuaHelper::push( this, "ExtButton" );
        LuaHelper::push( b );
        LuaHelper::executeFunction( h, 2 );
    } );
}

void ExtButton::registerEnableHandler( EnableHandlerType h )
{
    _enableHandler = h;
}

void ExtButton::unregisterEnableHandler()
{
    _enableHandler = nullptr;
}




bool ExtButton::isZoomOnTouchDown() const
{
    return _zoomOnTouchDown;
}

void ExtButton::setZoomOnTouchDown( bool b )
{
    _zoomOnTouchDown = b;
}

bool ExtButton::isEnabled() const
{
    return _touchListener != nullptr;
}

bool ExtButton::isHighlighted() const
{
    return _highlighted;
}

bool ExtButton::isPushed() const
{
    return _pushed;
}

ButtonStates const & ExtButton::getState() const
{
    return _state;
}

bool ExtButton::isSwallow() const
{
    return _swallow;
}

void ExtButton::setSwallow( bool b )
{
    _swallow = b;
    if( _touchListener ) _touchListener->setSwallowTouches( b );
}

void ExtButton::setVisible( bool visible )
{
    clearTouchLock();
    Node::setVisible( visible );
}

void ExtButton::onExit()
{
    clearTouchLock();
    unscheduleUpdate();             // 防双击相关: 反注册 update
    Layer::onExit();
}

void ExtButton::clearTouchLock()
{
    if( _mutexNumber )_mutexNumbers.erase( make_pair( _mutexNumber, this ) );

    // 保险起见
    _firstTouch = nullptr;
}

void ExtButton::onEnter()
{
    Layer::onEnter();
    scheduleUpdate();    // 防双击相关: 令回调每 frame 的结束时 都 call 当前对象的 update 函数. 
}



unordered_set<pair<int, ExtButton*>, hasher, equalto> ExtButton::_mutexNumbers;

