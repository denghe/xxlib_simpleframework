#ifndef __EXTExtScrollView_H__
#define __EXTExtScrollView_H__

 
// 复制/精简/改造自官方版 ScrollView
class ExtScrollView : public Layer, public ActionTweenDelegate
{
public:
    // size 为 contentSize 和 viewSize. container 如果不传 将自动创建
    static ExtScrollView* create( cocos2d::Size size, Node* container = NULL );

    // 将点距转为英寸
    static float convertDistanceFromPointToInch( float pointDis );

    // 设置内容偏移量x,y. animated: 是否动画移动
    void setContentOffset( cocos2d::Vec2 offset, bool animated = false );

    // 得到内容偏移量x,y
    cocos2d::Vec2 getContentOffset();

    // 设置内容偏移量x,y. dt 为动画移动耗时
    void setContentOffsetInDuration( cocos2d::Vec2 offset, float dt );

    // 设置缩放比
    void setZoomScale( float s );

    // 设置缩放比 animated: 是否动画缩放
    void setZoomScale( float s, bool animated );

    // 得到缩放比
    float getZoomScale();

    // 设置缩放比 dt: 动画缩放耗时
    void setZoomScaleInDuration( float s, float dt );

    // 得到内容偏移量最小尺寸( 以方便 setContentOffsetXxxxx 时界定值域 )
    cocos2d::Vec2 getMinContainerOffset();

    // 得到内容偏移量最大尺寸( 以方便 setContentOffsetXxxxx 时界定值域 )
    cocos2d::Vec2 getMaxContainerOffset();

    // 判断一个 node 是否被完全显示出来了
    bool isNodeVisible( Node * node );

    // 启用/禁用 touch
    void setEnabled( bool enabled );

    // 返回是否已启用 touch( 默认为 true )
    bool isEnabled() const;

    // 手指是否处于按压状态
    bool isDragging() const;

    // 手指是否滑动过
    bool isTouchMoved() const;

    // 启用/禁用 弹回效果
    void setBounceable( bool bBounceable );

    // 返回是否开启了弹回效果( 默认为 true )
    bool isBounceable() const;

    // 设置 视图 尺寸
    void setViewSize( cocos2d::Size size );
    // 获取 视图 尺寸
    cocos2d::Size getViewSize() const;

    // 设 2 手指 zoom 缩放限定
    void setZoomScaleLimit( float minScale, float maxScale );

    // 得到 内容容器
    Node * getContainer();

    // 设置 内容容器
    void setContainer( Node * pContainer );

    // 得到 移动方向
    ScrollViewDirection getDirection() const;
    
    // 设置移动方向
    void setDirection( ScrollViewDirection eDirection );

    // 
    void updateInset();

    // 是否裁剪显示( 默认为 true )
    bool isClippingToBounds() { return _clippingToBounds; }

    // 设置是否裁剪显示
    void setClippingToBounds( bool bClippingToBounds ) { _clippingToBounds = bClippingToBounds; }

    // touch 事件 处理函数 相关
    // 示例: xxxx->registerActionEventHandler( [ this, xxxxxx ]( ExtScrollView * sender, ScrollViewEvents e ) {  ... } );
    typedef function<void( ExtScrollView *, ScrollViewEvents )> ActionEventHandlerType;
    void registerActionEventHandler( LUA_FUNCTION h );
    void registerActionEventHandler( ActionEventHandlerType h );
    void unregisterActionEventHandler();

    // Provided to make scroll view compatible with SWLayer's pause method
    void pause( Ref * sender );

    // Provided to make scroll view compatible with SWLayer's resume method
    void resume( Ref * sender );

    // Overrides
    virtual void setContentSize( const cocos2d::Size & size ) override;
    virtual const cocos2d::Size& getContentSize() const override;
    virtual void visit( Renderer *renderer, const Mat4 &parentTransform, uint32_t parentFlags ) override;
    virtual void addChild( Node * child, int zOrder, int tag ) override;

    // CCActionTweenDelegate
    void updateTweenAction(float value, const std::string& key);

	// 用于判断container是否可以被移出
	bool _canMovedOut;                              
	inline bool getCanMovedOut() const { return _canMovedOut; }
	inline void setCanMovedOut( bool val ) { _canMovedOut = val; }
	
protected:

    ExtScrollView();
    virtual ~ExtScrollView();
    static ExtScrollView* create();
    bool init();
    bool initWithViewSize( cocos2d::Size size, Node* container = NULL );

    bool touchBegan( Touch * t, Event * e );
    void touchMoved( Touch * t, Event * e );
    void touchEnded( Touch * t, Event * e );
    void touchCancelled( Touch * t, Event * e );

    // Relocates the container at the proper offset, in bounds of max/min offsets.
    void relocateContainer( bool animated );

    // implements auto-scrolling behavior. change SCROLL_DEACCEL_RATE as needed to choose
    // deacceleration speed. it must be less than 1.0f.
    void deaccelerateScrolling( float dt );

	// This method makes sure auto scrolling causes delegate to invoke its method
    void performedAnimatedScroll( float dt );
    // Expire animated scroll delegate calls
    void stoppedAnimatedScroll( Node * node );
    // clip this view so that outside of the visible bounds can be hidden.
    void beforeDraw();
    void onBeforeDraw();

    // retract what's done in beforeDraw so that there's no side effect to other nodes.
    void afterDraw();
    void onAfterDraw();
    
    // Zoom handling
    void handleZoom();

    cocos2d::Rect getViewRect();

    
    float _zoomScale;                               // current zoom scale
    float _minZoomScale;                            // min zoom scale
    float _maxZoomScale;                            // max zoom scale
    ScrollViewDirection _direction;
    bool _dragging;                                 // If YES, the view is being dragged.
    cocos2d::Vec2 _contentOffset;                  // Content offset. Note that left-bottom point is the origin
    Node * _container;                              // Container holds scroll view contents, Sets the scrollable container object of the scroll view
    bool _touchMoved;                               // Determiens whether user touch is moved after begin phase.
    cocos2d::Vec2 _maxInset;                       // max inset point to limit scrolling by touch
    cocos2d::Vec2 _minInset;                       // min inset point to limit scrolling by touch
    bool _bounceable;                               // Determines whether the scroll view is allowed to bounce or not.
    bool _clippingToBounds;
    cocos2d::Vec2 _scrollDistance;                 // scroll speed
    cocos2d::Vec2 _touchPoint;                     // Touch point
    float _touchLength;                             // length between two fingers
    vector<Touch*> _touches;                        // Touch objects to detect multitouch
    cocos2d::Size _viewSize;                        // size to clip. Node boundingBox uses contentSize directly.
    float _minScale, _maxScale;                     // max and min scale
    cocos2d::Rect _parentScissorRect;               // scissor rect for parent, just for restoring GL_SCISSOR_BOX
    bool _scissorRestored;                          // 用于 onBeforeDraw 中记录是否成功 scissor 以便于 afterDraw 中恢复
    EventListenerTouchOneByOne * _touchListener;    // Touch listener
	

    CustomCommand _beforeDrawCommand;
    CustomCommand _afterDrawCommand;

    ActionEventHandlerType _actionEventHandler;     // 行为处理函数
};


#endif
