
#include "Precompile.h"

const float SCROLL_DEACCEL_RATE = 0.9f;
const float SCROLL_DEACCEL_DIST = 1.0f;
const float BOUNCE_DURATION = 0.15f;
const float INSET_RATIO = 0.2f;
const float MOVE_INCH = 15.5f / 160.0f;
const float BOUNCE_BACK_FACTOR = 0.35f;

float ExtScrollView::convertDistanceFromPointToInch( float pointDis )
{
    auto glview = Director::getInstance()->getOpenGLView();
    float factor = ( glview->getScaleX() + glview->getScaleY() ) / 2;
    return pointDis * factor / Device::getDPI();
}


ExtScrollView::ExtScrollView()
: _zoomScale( 0.0f )
, _minZoomScale( 0.0f )
, _maxZoomScale( 0.0f )
, _direction( ScrollViewDirection::Both )
, _dragging( false )
, _container( nullptr )
, _touchMoved( false )
, _bounceable( true )
, _clippingToBounds( true )
, _touchLength( 0.0f )
, _minScale( 1.0f )
, _maxScale( 1.0f )
, _scissorRestored( false )
, _touchListener( nullptr )
, _canMovedOut( true )
{
}

ExtScrollView::~ExtScrollView()
{
}

ExtScrollView* ExtScrollView::create( Size size, Node * container/* = nullptr */ )
{
    if( auto p = new ExtScrollView() )
    {
        if( p->initWithViewSize( size, container ) )
        {
            p->autorelease();
            return p;
        }
        delete p;
    }
    return nullptr;
}

ExtScrollView* ExtScrollView::create()
{
    if( auto p = new ExtScrollView() )
    {
        if( p->init() )
        {
            p->autorelease();
            return p;
        }
        delete p;
    }
    return nullptr;
}

bool ExtScrollView::initWithViewSize( Size size, Node * container/* = nullptr */ )
{
    if( !Layer::init() ) return false;

    _container = container;
    if( !container )
    {
        _container = Layer::create();
        _container->ignoreAnchorPointForPosition( false );
        _container->setAnchorPoint( Vec2( 0.0f, 0.0f ) );
    }
    setViewSize( size );
    setEnabled( true );
    _touches.reserve( EventTouch::MAX_TOUCHES );

    //_container->setContentSize(Size::ZERO);
    _container->setPosition( Vec2( 0.0f, 0.0f ) );

    Node::addChild( _container );

    return true;
}

bool ExtScrollView::init()
{
    return initWithViewSize( Size( 200, 200 ), nullptr );
}

bool ExtScrollView::isNodeVisible( Node * node )
{
    auto offset = getContentOffset();
    auto size = getViewSize();
    auto scale = getZoomScale();
    Rect viewRect( -offset.x / scale, -offset.y / scale, size.width / scale, size.height / scale );
    return viewRect.intersectsRect( node->getBoundingBox() );
}

void ExtScrollView::pause( Ref * sender )
{
    _container->pause();

    auto& children = _container->getChildren();
    for( const auto &child : children )
    {
        child->pause();
    }
}

void ExtScrollView::resume( Ref * sender )
{
    auto& children = _container->getChildren();
    for( const auto &child : children )
    {
        child->resume();
    }
    _container->resume();
}

bool ExtScrollView::isEnabled() const
{
    return _touchListener != nullptr;
}

void ExtScrollView::setEnabled( bool enabled )
{
    if( enabled )
    {
        if( _touchListener ) return;

        _touchListener = EventListenerTouchOneByOne::create();
        _touchListener->onTouchBegan = [ this ]( Touch * t, Event * e ) { return touchBegan( t, e ); };
        _touchListener->onTouchMoved = [ this ]( Touch * t, Event * e ) { touchMoved( t, e ); };
        _touchListener->onTouchEnded = [ this ]( Touch * t, Event * e ) { touchEnded( t, e ); };
        _touchListener->onTouchCancelled = [ this ]( Touch * t, Event * e ) { touchCancelled( t, e ); };
		_touchListener->setSwallowTouches( true );
        _eventDispatcher->addEventListenerWithSceneGraphPriority( _touchListener, this );
    }
    else
    {
        if( !_touchListener ) return;
        _eventDispatcher->removeEventListener( _touchListener );
        _touchListener = nullptr;
        _dragging = false;
        _touchMoved = false;
        _touches.clear();
    }
}

void ExtScrollView::setContentOffset( Vec2 offset, bool animated/* = false*/ )
{
    if( animated )   //animate scrolling
    {
        setContentOffsetInDuration( offset, BOUNCE_DURATION );
    }
    else             //set the container position directly
    {
		if( !_bounceable || !_canMovedOut )
        {
            const Vec2 minOffset = getMinContainerOffset();
            const Vec2 maxOffset = getMaxContainerOffset();

            offset.x = MAX( minOffset.x, MIN( maxOffset.x, offset.x ) );
            offset.y = MAX( minOffset.y, MIN( maxOffset.y, offset.y ) );
        }
        _container->setPosition( offset );
    }
}

void ExtScrollView::setContentOffsetInDuration( Vec2 offset, float dt )
{
    FiniteTimeAction *scroll, *expire;

    scroll = MoveTo::create( dt, offset );
    expire = CallFuncN::create( [ this ]( Node* n ) { stoppedAnimatedScroll( n ); } );
    _container->runAction( Sequence::create( scroll, expire, nullptr ) );
    schedule( schedule_selector( ExtScrollView::performedAnimatedScroll ) );
}

Vec2 ExtScrollView::getContentOffset()
{
    return _container->getPosition();
}

void ExtScrollView::setZoomScale( float s )
{
    CCLOG( "*************************setZoomScale %f", s );
    if( _container->getScale() == s ) return;

    Vec2 oldCenter, newCenter, center;
    if( _touchLength == 0.0f )
    {
        center = Vec2( _viewSize.width*0.5f, _viewSize.height*0.5f );
        center = convertToWorldSpace( center );
    }
    else
    {
        center = _touchPoint;
    }

    oldCenter = _container->convertToNodeSpace( center );
    _container->setScale( MAX( _minScale, MIN( _maxScale, s ) ) );
    newCenter = _container->convertToWorldSpace( oldCenter );

    auto offset = center - newCenter;
    setContentOffset( _container->getPosition() + offset );
    CCLOG( "*************************setZoomScale %f", _container->getScale() );
}

float ExtScrollView::getZoomScale()
{
    return _container->getScale();
}

void ExtScrollView::setZoomScale( float s, bool animated )
{
    if( animated )
    {
        setZoomScaleInDuration( s, BOUNCE_DURATION );
    }
    else
    {
        setZoomScale( s );
    }
}

void ExtScrollView::setZoomScaleInDuration( float s, float dt )
{
    if( dt > 0 )
    {
        if( _container->getScale() != s )
        {
            auto a = ActionTween::create( dt, "zoomScale", _container->getScale(), s );
            runAction( a );
        }
    }
    else
    {
        setZoomScale( s );
    }
}

void ExtScrollView::updateTweenAction( float value, const std::string& key )
{
    this->setZoomScale( value );
}
void ExtScrollView::setViewSize( Size size )
{
    _viewSize = size;
    Node::setContentSize( size );
}

Node * ExtScrollView::getContainer()
{
    return _container;
}

void ExtScrollView::setContainer( Node * pContainer )
{
    if( nullptr == pContainer ) return;

    removeAllChildrenWithCleanup( true );
    _container = pContainer;

    _container->ignoreAnchorPointForPosition( false );
    _container->setAnchorPoint( Vec2( 0.0f, 0.0f ) );

    Node::addChild( _container );

    setViewSize( _viewSize );
}

void ExtScrollView::relocateContainer( bool animated )
{
    Vec2 oldPoint, min, max;
    float newX, newY;

    min = getMinContainerOffset();
    max = getMaxContainerOffset();

    oldPoint = _container->getPosition();

    newX = oldPoint.x;
    newY = oldPoint.y;
    if( _direction == ScrollViewDirection::Both || _direction == ScrollViewDirection::Horizontal )
    {
        newX = MAX( newX, min.x );
        newX = MIN( newX, max.x );
    }

    if( _direction == ScrollViewDirection::Both || _direction == ScrollViewDirection::Vertical )
    {
        newY = MIN( newY, max.y );
        newY = MAX( newY, min.y );
    }

    if( newY != oldPoint.y || newX != oldPoint.x )
    {
        setContentOffset( Vec2( newX, newY ), animated );
    }

    // todo: 这里应该判断是否还有后续行为 诸如 弹, 惯性滑 以避免在动画发生之前抛出事件
    if( _actionEventHandler )
    {
        _actionEventHandler( this, ScrollViewEvents::Stoped );
    }
}

Vec2 ExtScrollView::getMaxContainerOffset()
{
    return Vec2( 0.0f, 0.0f );
}

Vec2 ExtScrollView::getMinContainerOffset()
{
    return Vec2( _viewSize.width - _container->getContentSize().width*_container->getScaleX(),
                  _viewSize.height - _container->getContentSize().height*_container->getScaleY() );
}

void ExtScrollView::deaccelerateScrolling( float dt )
{
    if( _dragging )
    {
        unschedule( schedule_selector( ExtScrollView::deaccelerateScrolling ) );
        return;
    }

    float newX, newY;
    Vec2 maxInset, minInset;

    _container->setPosition( _container->getPosition() + _scrollDistance );

    if( _bounceable )
    {
        maxInset = _maxInset;
        minInset = _minInset;
    }
    else
    {
        maxInset = getMaxContainerOffset();
        minInset = getMinContainerOffset();
    }
    newX = _container->getPosition().x;
    newY = _container->getPosition().y;

    _scrollDistance = _scrollDistance * SCROLL_DEACCEL_RATE;
    setContentOffset( Vec2( newX, newY ) );

    if( ( fabsf( _scrollDistance.x ) <= SCROLL_DEACCEL_DIST &&
        fabsf( _scrollDistance.y ) <= SCROLL_DEACCEL_DIST ) ||
        newY >= maxInset.y || newY <= minInset.y ||
        newX >= maxInset.x || newX <= minInset.x )
    {
        unschedule( schedule_selector( ExtScrollView::deaccelerateScrolling ) );
        relocateContainer( true );
    }
}

void ExtScrollView::stoppedAnimatedScroll( Node * node )
{
    unschedule( schedule_selector( ExtScrollView::performedAnimatedScroll ) );
    if( _actionEventHandler )
    {
        _actionEventHandler( this, ScrollViewEvents::Stoped );
    }
}

void ExtScrollView::performedAnimatedScroll( float dt )
{
    if( _dragging )
    {
        unschedule( schedule_selector( ExtScrollView::performedAnimatedScroll ) );
        return;
    }
    if( _actionEventHandler )
    {
        _actionEventHandler( this, ScrollViewEvents::Moving );
    }

}


const Size& ExtScrollView::getContentSize() const
{
    return _container->getContentSize();
}

void ExtScrollView::setContentSize( const Size & size )
{
    if( _container )
    {
        _container->setContentSize( size );
        updateInset();
    }
}

void ExtScrollView::updateInset()
{
    if( _container )
    {
        _maxInset = getMaxContainerOffset();
        _maxInset = Vec2( _maxInset.x + _viewSize.width * INSET_RATIO,
                           _maxInset.y + _viewSize.height * INSET_RATIO );
        _minInset = getMinContainerOffset();
        _minInset = Vec2( _minInset.x - _viewSize.width * INSET_RATIO,
                           _minInset.y - _viewSize.height * INSET_RATIO );
    }
}

/**
* make sure all children go to the container
*/
void ExtScrollView::addChild( Node * child, int zOrder, int tag )
{
    if( _container != child )
    {
        _container->addChild( child, zOrder, tag );
    }
    else
    {
        Layer::addChild( child, zOrder, tag );
    }
}

void ExtScrollView::beforeDraw()
{
    _beforeDrawCommand.init( _globalZOrder );
    _beforeDrawCommand.func = [ this ] { onBeforeDraw(); };
    Director::getInstance()->getRenderer()->addCommand( &_beforeDrawCommand );
}

/**
* clip this view so that outside of the visible bounds can be hidden.
*/
void ExtScrollView::onBeforeDraw()
{
    if( _clippingToBounds )
    {
        _scissorRestored = false;
        Rect frame = getViewRect();
        auto glview = Director::getInstance()->getOpenGLView();
        if( glview->isScissorEnabled() )
        {
            _scissorRestored = true;
            _parentScissorRect = glview->getScissorRect();
            //set the intersection of _parentScissorRect and frame as the new scissor rect
            if( frame.intersectsRect( _parentScissorRect ) )
            {
                float x = MAX( frame.origin.x, _parentScissorRect.origin.x );
                float y = MAX( frame.origin.y, _parentScissorRect.origin.y );
                float xx = MIN( frame.origin.x + frame.size.width, _parentScissorRect.origin.x + _parentScissorRect.size.width );
                float yy = MIN( frame.origin.y + frame.size.height, _parentScissorRect.origin.y + _parentScissorRect.size.height );
                glview->setScissorInPoints( x, y, xx - x, yy - y );
            }
        }
        else
        {
            glEnable( GL_SCISSOR_TEST );
            glview->setScissorInPoints( frame.origin.x, frame.origin.y, frame.size.width, frame.size.height );
        }
    }
}

void ExtScrollView::afterDraw()
{
    _afterDrawCommand.init( _globalZOrder );
    _afterDrawCommand.func = [ this ] { onAfterDraw(); };
    Director::getInstance()->getRenderer()->addCommand( &_afterDrawCommand );
}

/**
* retract what's done in beforeDraw so that there's no side effect to
* other nodes.
*/
void ExtScrollView::onAfterDraw()
{
    if( _clippingToBounds )
    {
        if( _scissorRestored )
        {//restore the parent's scissor rect
            Director::getInstance()->getOpenGLView()->setScissorInPoints( _parentScissorRect.origin.x, _parentScissorRect.origin.y, _parentScissorRect.size.width, _parentScissorRect.size.height );
        }
        else
        {
            glDisable( GL_SCISSOR_TEST );
        }
    }
}

void ExtScrollView::visit( Renderer *renderer, const Mat4 &parentTransform, uint32_t parentFlags )
{
    // quick return if not visible
    if( !isVisible() )
    {
        return;
    }

    uint32_t flags = processParentFlags( parentTransform, parentFlags );

    // IMPORTANT:
    // To ease the migration to v3.0, we still support the kmGL stack,
    // but it is deprecated and your code should not rely on it
    Director* director = Director::getInstance();
    CCASSERT( nullptr != director, "Director is null when seting matrix stack" );
    director->pushMatrix( MATRIX_STACK_TYPE::MATRIX_STACK_MODELVIEW );
    director->loadMatrix( MATRIX_STACK_TYPE::MATRIX_STACK_MODELVIEW, _modelViewTransform );

    this->beforeDraw();

    if( !_children.empty() )
    {
        int i = 0;

        // draw children zOrder < 0
        for( ; i < _children.size(); i++ )
        {
            Node *child = _children.at( i );
            if( child->getLocalZOrder() < 0 )
            {
                child->visit( renderer, _modelViewTransform, flags );
            }
            else
            {
                break;
            }
        }

        // this draw
        this->draw( renderer, _modelViewTransform, flags );

        // draw children zOrder >= 0
        for( ; i < _children.size(); i++ )
        {
            Node *child = _children.at( i );
            child->visit( renderer, _modelViewTransform, flags );
        }

    }
    else
    {
        this->draw( renderer, _modelViewTransform, flags );
    }

    this->afterDraw();

    director->popMatrix( MATRIX_STACK_TYPE::MATRIX_STACK_MODELVIEW );
}

bool ExtScrollView::touchBegan( Touch* touch, Event* event )
{
    if( !isVisible() )
    {
        return false;
    }

    // 不在范围的点击忽略
    // 不管产生次多少点击，都记录进 _touches, 全吞，防透到下面，三个手指以上只取前 2 

    auto frame = getViewRect();
    if( !frame.containsPoint( touch->getLocation() ) )      //if( _touches.size() > 2 || _touchMoved ||  )
    {
        return false;
    }


    if( std::find( _touches.begin(), _touches.end(), touch ) == _touches.end() )
    {
        _touches.push_back( touch );
    }

    if( _touches.size() == 1 )
    { // scrolling
        _touchPoint = convertTouchToNodeSpace( touch );
        _touchMoved = false;
        _dragging = true; //dragging started
        _scrollDistance = Vec2( 0.0f, 0.0f );
        _touchLength = 0.0f;
    }
    else if( _touches.size() >= 2 )
    {
        _touchPoint = ( convertTouchToNodeSpace( _touches[ 0 ] ).getMidpoint(
            convertTouchToNodeSpace( _touches[ 1 ] ) ) );

        _touchLength = _container->convertTouchToNodeSpace( _touches[ 0 ] ).getDistance(
            _container->convertTouchToNodeSpace( _touches[ 1 ] ) );

        _dragging = false;
    }

    if( _actionEventHandler )
    {
        _actionEventHandler( this, ScrollViewEvents::Begin );
    }
    return true;
}

void ExtScrollView::touchMoved( Touch* touch, Event* event )
{
    if( !isVisible() )
    {
        return;
    }

    if( std::find( _touches.begin(), _touches.end(), touch ) != _touches.end() )
    {
        if( _touches.size() == 1 && _dragging )
        { // scrolling
            Vec2 moveDistance, newPoint;
            Rect  frame;
            float newX, newY;

            frame = getViewRect();

            newPoint = convertTouchToNodeSpace( _touches[ 0 ] );
            moveDistance = newPoint - _touchPoint;

            float dis = 0.0f;
            if( _direction == ScrollViewDirection::Vertical )
            {
                dis = moveDistance.y;
                float pos = _container->getPosition().y;
                if( !( getMinContainerOffset().y <= pos && pos <= getMaxContainerOffset().y ) )
                {
                    moveDistance.y *= BOUNCE_BACK_FACTOR;
                }
            }
            else if( _direction == ScrollViewDirection::Horizontal )
            {
                dis = moveDistance.x;
                float pos = _container->getPosition().x;
                if( !( getMinContainerOffset().x <= pos && pos <= getMaxContainerOffset().x ) )
                {
                    moveDistance.x *= BOUNCE_BACK_FACTOR;
                }
            }
            else
            {
                dis = sqrtf( moveDistance.x*moveDistance.x + moveDistance.y*moveDistance.y );

                float pos = _container->getPosition().y;
                if( !( getMinContainerOffset().y <= pos && pos <= getMaxContainerOffset().y ) )
                {
                    moveDistance.y *= BOUNCE_BACK_FACTOR;
                }

                pos = _container->getPosition().x;
                if( !( getMinContainerOffset().x <= pos && pos <= getMaxContainerOffset().x ) )
                {
                    moveDistance.x *= BOUNCE_BACK_FACTOR;
                }
            }

            if( !_touchMoved && fabs( convertDistanceFromPointToInch( dis ) ) < MOVE_INCH )
            {
                //CCLOG("Invalid movement, distance = [%f, %f], disInch = %f", moveDistance.x, moveDistance.y);
                return;
            }

            if( !_touchMoved )
            {
                moveDistance = Vec2::ZERO;
            }

            _touchPoint = newPoint;
            _touchMoved = true;

            if( _dragging )
            {
                switch( _direction )
                {
                case ScrollViewDirection::Vertical:
                    moveDistance = Vec2( 0.0f, moveDistance.y );
                    break;
                case ScrollViewDirection::Horizontal:
                    moveDistance = Vec2( moveDistance.x, 0.0f );
                    break;
                default:
                    break;
                }

                newX = _container->getPosition().x + moveDistance.x;
                newY = _container->getPosition().y + moveDistance.y;

                _scrollDistance = moveDistance;
                setContentOffset( Vec2( newX, newY ) );
                if( _actionEventHandler )
                {
                    _actionEventHandler( this, ScrollViewEvents::Draging );
                }
            }
        }
        else if( _touches.size() >= 2 && !_dragging )
        {
            const float len = _container->convertTouchToNodeSpace( _touches[ 0 ] ).getDistance(
                _container->convertTouchToNodeSpace( _touches[ 1 ] ) );
            setZoomScale( getZoomScale()*len / _touchLength );
        }
    }
}

void ExtScrollView::touchEnded( Touch* touch, Event* event )
{
    if( !isVisible() )
    {
        return;
    }

    auto touchIter = std::find( _touches.begin(), _touches.end(), touch );

    if( touchIter != _touches.end() )
    {
        if( _touches.size() == 1 && _touchMoved )
        {
            schedule( schedule_selector( ExtScrollView::deaccelerateScrolling ) );
        }
        _touches.erase( touchIter );
    }

    auto moved = _touchMoved;

    if( _touches.size() == 0 )
    {
        _dragging = false;
        _touchMoved = false;
        if( !moved && _actionEventHandler )
        {
            _actionEventHandler( this, ScrollViewEvents::Stoped );
        }
    }

   
}

void ExtScrollView::touchCancelled( Touch* touch, Event* event )
{
    if( !isVisible() )
    {
        return;
    }

    auto touchIter = std::find( _touches.begin(), _touches.end(), touch );
    _touches.erase( touchIter );

    if( _touches.size() == 0 )
    {
        _dragging = false;
        _touchMoved = false;
    }
}

Rect ExtScrollView::getViewRect()
{
    Vec2 screenPos = convertToWorldSpace( Vec2::ZERO );

    float scaleX = getScaleX();
    float scaleY = getScaleY();

    for( Node *p = _parent; p != nullptr; p = p->getParent() )
    {
        scaleX *= p->getScaleX();
        scaleY *= p->getScaleY();
    }

    // Support negative scaling. Not doing so causes intersectsRect calls
    // (eg: to check if the touch was within the bounds) to return false.
    // Note, Node::getScale will assert if X and Y scales are different.
    if( scaleX < 0.f )
    {
        screenPos.x += _viewSize.width*scaleX;
        scaleX = -scaleX;
    }
    if( scaleY < 0.f )
    {
        screenPos.y += _viewSize.height*scaleY;
        scaleY = -scaleY;
    }

    return Rect( screenPos.x, screenPos.y, _viewSize.width*scaleX, _viewSize.height*scaleY );
}

bool ExtScrollView::isDragging() const
{
    return _dragging;
}

bool ExtScrollView::isTouchMoved() const
{
    return _touchMoved;
}

void ExtScrollView::setBounceable( bool bBounceable )
{
    _bounceable = bBounceable;
}

bool ExtScrollView::isBounceable() const
{
    return _bounceable;
}

cocos2d::Size ExtScrollView::getViewSize() const
{
    return _viewSize;
}

ScrollViewDirection ExtScrollView::getDirection() const
{
    return _direction;
}

void ExtScrollView::setDirection( ScrollViewDirection eDirection )
{
    _direction = eDirection;
}



void ExtScrollView::registerActionEventHandler( LUA_FUNCTION h )
{
    registerActionEventHandler( [ this, h ]( ExtScrollView * s, ScrollViewEvents e )
    {
        LuaHelper::push( s, "ExtScrollView" );
        LuaHelper::push( (int)e );
        LuaHelper::executeFunction( h, 2 );
    } );
}

void ExtScrollView::registerActionEventHandler( ActionEventHandlerType h )
{
    _actionEventHandler = h;
}

void ExtScrollView::unregisterActionEventHandler()
{
    _actionEventHandler = nullptr;
}

void ExtScrollView::setZoomScaleLimit( float minScale, float maxScale )
{
    _minScale = minScale;
    _maxScale = maxScale;
}
