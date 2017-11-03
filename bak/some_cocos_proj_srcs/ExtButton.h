#ifndef __EXTBUTTONEX_H__
#define __EXTBUTTONEX_H__


class ExtButton;

// 实现单击限定相关类
struct hasher
{
    inline size_t operator()( const pair<int, ExtButton*> & v ) const
    {
        return v.first;
    }
};
struct equalto
{
    inline bool operator()( const pair<int, ExtButton*> & _Left, const pair<int, ExtButton*> & _Right ) const
    {
        return ( _Left.first == _Right.first );
    }
};

/*
    简单功能的 button.
    支持传入 背景, 前景节点. 背景必传, 其 ContentSize 即为点击范围
    支持 启/禁用, 高亮/普通 点击事件 处理函数 bind
    */
class ExtButton : public Layer
{
public:
    // touch 事件 处理函数 相关
    // 示例: xxxx->registerTouchEventHandler( [ this, xxxxxx ]( ExtButton * sender, ButtonTouchEvents e, float x, float y ) {  ... } );
    typedef function<void( ExtButton *, ButtonTouchEvents, float, float )> TouchEventHandlerType;
    void registerTouchEventHandler( LUA_FUNCTION h );
    void registerTouchEventHandler( TouchEventHandlerType h );
    void unregisterTouchEventHandler();

    // high light 处理函数 相关
    // 示例: xxxx->registerHighlightHandler( [ this, xxxxxx ]( ExtButton * sender, bool b ) {  ... } );
    typedef function<void( ExtButton *, bool )> HighlightHandlerType;
    void registerHighlightHandler( LUA_FUNCTION h );
    void registerHighlightHandler( HighlightHandlerType h );
    void unregisterHighlightHandler();

    // enable 处理函数 相关
    // 示例: xxxx->registerEnableHandler( [ this, xxxxxx ]( ExtButton * sender, bool b ) {  ... } );
    typedef function<void( ExtButton *, bool )> EnableHandlerType;
    void registerEnableHandler( LUA_FUNCTION h );
    void registerEnableHandler( EnableHandlerType h );
    void unregisterEnableHandler();

    // 创建. 背景必须传入. 并且其 content size 将作为 touch 范围依据. 当 btn 用于 scroll view 内时, swallow 须传 false( 可击穿 )
    // mutexNumber 为互斥编号. 相同编号的 btn 同时发起 touch 时, 只有一个会响应 
    static ExtButton* create( Node * bg, Node * fg = nullptr, bool swallow = true, int mutexNumber = 1 );

    // 覆盖实现, 需要在这步把全局防 multi touch 标记防御性清除( 比如 touch down 的回调中 visible false )
    virtual void setVisible( bool visible ) override;
    virtual void onExit() override;
	virtual void onEnter() override;


    // 背景节点
protected:
    Node * _bg;
public:
    Node * getBg() const;
    void setBg( Node * v );

    // 前景节点
protected:
    Node * _fg;
public:
    Node * getFg() const;
    void setFg( Node * v );

    // 启用/禁用 touch 时放大效果
protected:
    bool _zoomOnTouchDown;
public:
    bool isZoomOnTouchDown() const;
    void setZoomOnTouchDown( bool b );

    // 启用/禁用 touch
public:
    bool isEnabled() const;
    void setEnabled( bool b );

    // 高亮/非高亮
protected:
    bool _highlighted;
public:
    bool isHighlighted() const;
    void setHighlighted( bool b );

    // 是否已按下
protected:
    bool _pushed;
public:
    bool isPushed() const;

    // 是否防击穿
protected:
    bool  _swallow;
public:
    bool isSwallow() const;
    void setSwallow( bool b );


    // button state
protected:
    ButtonStates _state;
public:
    ButtonStates const & getState() const;

    friend Scheduler;
protected:
    ExtButton();
    virtual ~ExtButton();
    DELETE_COPY_ASSIGN( ExtButton );

    // for schedule frame callback
    virtual void update( float df ) override;

    // for create
    bool init( Node * bg, Node * fg, bool swallow = true, int mutexNumber = 0 );

    // 内部布局相关函数
    void needsLayout( void );

    // for EventListenerTouchOneByOne touchListener
    bool handleTouchBegan( Touch * t, Event * e );
    void handleTouchMoved( Touch * t, Event * e );
    void handleTouchEnded( Touch * t, Event * e );
    void handleTouchCancelled( Touch * t, Event * e );

    // 上面几个函数发起 touch handler 调用的公用函数
    void callTouchEventHandler( ButtonTouchEvents e, Touch *t );
	
	// 将点距转为英寸
	inline static float convertDistanceFromPointToInch( float pointDis )
	{
		auto glview = Director::getInstance()->getOpenGLView();
		float factor = ( glview->getScaleX() + glview->getScaleY() ) / 2;
		return pointDis * factor / Device::getDPI();
	}

    // 本 frame 已执行过 touch up inside 代码的标记 ( 防双击 ), 每 frame 靠 update 函数重置
    bool  _currFrameTouched;

    // touch 监听器 相关
    EventListenerTouchOneByOne * _touchListener;
    EventDispatcher * _dispatcher;

    // 各种 handler lambda 容器
    TouchEventHandlerType _touchEventHandler;
    HighlightHandlerType _highlightHandler;
    EnableHandlerType _enableHandler;

    // 实现单击限定
    int _mutexNumber;
    static unordered_set<pair<int, ExtButton*>, hasher, equalto> _mutexNumbers;
public:
    void clearTouchLock();


protected:
    // 记录第 1 次点击，用于吞噬非第 1 次点击
    Touch* _firstTouch;

};

#endif
