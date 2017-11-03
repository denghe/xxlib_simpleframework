#ifndef __MISCNODE_CCCLIPPING_REGION_NODE_H__
#define __MISCNODE_CCCLIPPING_REGION_NODE_H__

// 方形裁剪节点
class ExtClippingNode : public cocos2d::Node
{
public:
    static ExtClippingNode* create( const cocos2d::Rect& clippingRegion );
    static ExtClippingNode* create( void );

    // 取得裁剪范围
    const cocos2d::Rect getClippingRegion( void );

    // 设置裁剪范围
    void setClippingRegion( const cocos2d::Rect& clippingRegion );

    // 内部渲染调用函数, 不生成到 LUA
    virtual void visit( Renderer *renderer, const Mat4 &parentTransform, uint32_t parentFlags ) override;

    // 如果裁剪节点用于 拖拽容器, touch move 时设置该值为 true 可阻止容器内 button 被点击
    bool isTouchMoved() const;
    void setTouchMoved( bool v );

protected:
    ExtClippingNode( void );

    /**
    * clip this view so that outside of the visible bounds can be hidden.
    */
    void beforeDraw();
    void onBeforeDraw();
    /**
    * retract what's done in beforeDraw so that there's no side effect to
    * other nodes.
    */
    void afterDraw();
    void onAfterDraw();


    cocos2d::Rect _clippingRegion;
    /**
    * scissor rect for parent, just for restoring GL_SCISSOR_BOX
    */
    cocos2d::Rect _parentScissorRect;
    bool _scissorRestored;
    bool _clippingEnabled;

    CustomCommand _beforeDrawCommand;
    CustomCommand _afterDrawCommand;

    bool _isTouchMoved;
};


#endif

