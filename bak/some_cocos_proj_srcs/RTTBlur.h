#ifndef _RTTBLUR_H_
#define _RTTBLUR_H_

#include "2d/CCNode.h"
#include "2d/CCRenderTexture.h"
#include "renderer/CCTexture2D.h"

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID || CC_TARGET_PLATFORM == CC_PLATFORM_WP8)
#include "base/CCEventListenerCustom.h"
#endif

USING_NS_CC;

class RTTBlur : public Node
{
public:
    RTTBlur();
    virtual ~RTTBlur();

    static RTTBlur *create();
    static RTTBlur *create(float w, float h, Texture2D::PixelFormat fmt = Texture2D::PixelFormat::RGB565);

    bool init(float w, float h, Texture2D::PixelFormat fmt);

    virtual void update(float dt) override;
    virtual void setContentSize(const cocos2d::Size& contentSize) override;

    void setRTTNodes(Vector<Node *> &nodes)
    {
        _rtt_nodes = std::move(nodes);
        _dirty |= DT_REDRAW;
    }

    void addRTTNode(Node *node)
    {
        _rtt_nodes.pushBack(node);
        _dirty |= DT_REDRAW;
    }

    void removeRTTNode(Node *node)
    {
        _rtt_nodes.eraseObject(node);
        _dirty |= DT_REDRAW;
    }

    void clearRTTNodes()
    {
        _rtt_nodes.clear();
        _dirty |= DT_REDRAW;
    }

    float getBlurRadius()
    {
        return _blur_radius;
    }

    void setBlurRadius(float r)
    {
        if (_blur_radius == r) return;
        _blur_radius = r;
        _dirty |= DT_BLUR_RADIUS;
    }

    float getSampleNum()
    {
        return _sample_num;
    }

    void setSampleNum(float n)
    {
        if (_sample_num == n) return;
        _sample_num = n;
        _dirty |= DT_SAMPLE_NUM;
    }

    Texture2D::PixelFormat getPixelFormat()
    {
        return _fmt;
    }

    void setPixelFormat(Texture2D::PixelFormat fmt)
    {
        if (_fmt == fmt) return;
        _fmt = fmt;
        _dirty |= DT_REBUILD_TEX | DT_REDRAW;
    }

    void enabledAutoRedraw(bool e)
    {
        _auto_redraw = e;
    }

    bool isAutoRedraw()
    {
        return _auto_redraw;
    }


protected:

    enum DirtyType
    {
        DT_REBUILD_TEX  = 0x0001,
        DT_REDRAW       = 0x0002,
        DT_SHARDER      = 0x0010,
        DT_BLUR_RADIUS  = 0x0020,
        DT_SAMPLE_NUM   = 0x0040,
        DT_ALL          = 0xFFFFFFFF,
    };

    RenderTexture  *_rtt;
    float           _blur_radius;
    float           _sample_num;
    int             _dirty;
    Texture2D::PixelFormat _fmt;
    bool            _auto_redraw;
    
    Vector<Node *>  _rtt_nodes;

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID || CC_TARGET_PLATFORM == CC_PLATFORM_WP8)
    EventListenerCustom *_backgroundListener;
#endif
};

#endif//_RTTBLUR_H_
