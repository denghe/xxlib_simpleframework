#include "Precompile.h"

ExtClippingNode::ExtClippingNode( void )
    : _clippingEnabled( false )
    , _isTouchMoved( false )
{
}

ExtClippingNode* ExtClippingNode::create( const Rect& clippingRegion )
{
    ExtClippingNode* node = new ExtClippingNode();
    node->setClippingRegion( clippingRegion );
    node->autorelease();
    return node;
}

ExtClippingNode* ExtClippingNode::create( void )
{
    ExtClippingNode* node = new ExtClippingNode();
    node->autorelease();
    return node;
}

const cocos2d::Rect ExtClippingNode::getClippingRegion( void )
{
    return _clippingRegion;
}

void ExtClippingNode::setClippingRegion( const Rect &clippingRegion )
{
    _clippingRegion = clippingRegion;
    setContentSize( clippingRegion.size );
    _clippingEnabled = true;
}

void ExtClippingNode::visit( Renderer *renderer, const Mat4 &parentTransform, uint32_t parentFlags )
{
    if( !_visible )
        return;

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





void ExtClippingNode::beforeDraw()
{
    _beforeDrawCommand.init( _globalZOrder );
    _beforeDrawCommand.func = [ this ] 
    {
        onBeforeDraw(); 
    };
    Director::getInstance()->getRenderer()->addCommand( &_beforeDrawCommand );
}

/**
* clip this view so that outside of the visible bounds can be hidden.
*/
void ExtClippingNode::onBeforeDraw()
{
    if( _clippingEnabled )
    {
        _scissorRestored = false;
        auto glview = Director::getInstance()->getOpenGLView();

        // Моід x y w h
        Vec4 tmp;
        tmp.x = _clippingRegion.origin.x;
        tmp.y = _clippingRegion.origin.y;
        tmp.z = _positionZ;
        tmp.w = 1;
        _modelViewTransform.transformVector( &tmp );
        auto x = tmp.x;
        auto y = tmp.y;

        tmp.x = _clippingRegion.origin.x + _clippingRegion.size.width;
        tmp.y = _clippingRegion.origin.y + _clippingRegion.size.height;
        tmp.z = _positionZ;
        tmp.w = 1;
        _modelViewTransform.transformVector( &tmp );
        auto w = tmp.x - x;
        auto h = tmp.y - y;

        if( glview->isScissorEnabled() )
        {
            _scissorRestored = true;
            _parentScissorRect = glview->getScissorRect();

            //set the intersection of _parentScissorRect and frame as the new scissor rect
            if( _clippingRegion.intersectsRect( _parentScissorRect ) )
            {
                float x_ = MAX( x, _parentScissorRect.origin.x );
                float y_ = MAX( y, _parentScissorRect.origin.y );
                float xx_ = MIN( x + w, _parentScissorRect.origin.x + _parentScissorRect.size.width );
                float yy_ = MIN( y + h, _parentScissorRect.origin.y + _parentScissorRect.size.height );
                glview->setScissorInPoints( x_, y_, xx_ - x_, yy_ - y_ );
            }
        }
        else
        {
            glEnable( GL_SCISSOR_TEST );


            Director::getInstance()->getOpenGLView()->setScissorInPoints( x, y, w, h );
        }
    }
}

void ExtClippingNode::afterDraw()
{
    _afterDrawCommand.init( _globalZOrder );
    _afterDrawCommand.func = [ this ]
    {
        onAfterDraw();
    };
    Director::getInstance()->getRenderer()->addCommand( &_afterDrawCommand );
}

/**
* retract what's done in beforeDraw so that there's no side effect to
* other nodes.
*/
void ExtClippingNode::onAfterDraw()
{
    if( _clippingEnabled )
    {
        if( _scissorRestored )
        {//restore the parent's scissor rect
            auto glview = Director::getInstance()->getOpenGLView();

            glview->setScissorInPoints( _parentScissorRect.origin.x, _parentScissorRect.origin.y, _parentScissorRect.size.width, _parentScissorRect.size.height );
        }
        else
        {
            glDisable( GL_SCISSOR_TEST );
        }
    }
}






bool ExtClippingNode::isTouchMoved() const
{
    return _isTouchMoved;
}
void ExtClippingNode::setTouchMoved( bool v )
{
    _isTouchMoved = v;
}

