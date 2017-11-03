#include "Precompile.h"
#include "RTTBlur.h"
#include "base/CCDirector.h"
#include "renderer/CCGLProgram.h"
#include "renderer/CCGLProgramState.h"
#include "renderer/ccShaders.h"

RTTBlur::RTTBlur()
    : _rtt(nullptr)
    , _dirty(DT_ALL)
    , _blur_radius(1.0f)
    , _sample_num(1.0f)
    , _fmt(Texture2D::PixelFormat::RGBA8888)
    , _auto_redraw(false)
{
    this->setScale(1.0f);
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID || CC_TARGET_PLATFORM == CC_PLATFORM_WP8)
    // listen the event that renderer was recreated on Android/WP8
    _backgroundListener = EventListenerCustom::create( EVENT_RENDERER_RECREATED,
                                                       [ this ]( EventCustom* )
    {
        this->_dirty = DT_ALL;
    });
    Director::getInstance()->getEventDispatcher()->addEventListenerWithFixedPriority(_backgroundListener, -1);
#endif

}

RTTBlur::~RTTBlur()
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID || CC_TARGET_PLATFORM == CC_PLATFORM_WP8)
    // listen the event that renderer was recreated on Android/WP8
    Director::getInstance()->getEventDispatcher()->removeEventListener(_backgroundListener);
#endif
}

RTTBlur *RTTBlur::create()
{
    auto sz = Director::getInstance()->getVisibleSize();
    return RTTBlur::create(sz.width, sz.height);
}

RTTBlur *RTTBlur::create(float w, float h, Texture2D::PixelFormat fmt)
{
    auto rtv = new RTTBlur();
    if (rtv && rtv->init(w, h, fmt))
    {
        rtv->autorelease();
        return rtv;
    }
    else
    {
        delete rtv;
        return nullptr;
    }
}

bool RTTBlur::init(float w, float h, Texture2D::PixelFormat fmt)
{
    _fmt = fmt;

    this->scheduleUpdate();
    this->setContentSize(Size(w, h));

    return true;
}

string gen_vsh(const Size &sz)
{
    char buf[1024] = {0};
    string s;
    s.reserve(4096);

    s.append("#ifdef GL_ES                                               \n");
    s.append("precision mediump float;                                   \n");
    s.append("#endif                                                     \n");
    s.append("                                                           \n");
    s.append("varying vec4 v_fragmentColor;                              \n");
    s.append("varying vec2 v_texCoord;                                   \n");
    s.append("                                                           \n");
    s.append("vec3 blur(vec2);                                           \n");
    s.append("                                                           \n");
    s.append("void main(void)                                            \n");
    s.append("{                                                          \n");
    s.append("	vec3 col = blur(v_texCoord);                             \n");
    s.append("	gl_FragColor = vec4(col, 1.0) * v_fragmentColor;         \n");
    s.append("}                                                          \n");
    s.append("                                                           \n");
    s.append("vec3 blur(vec2 p)                                          \n");
    s.append("{                                                          \n");

#define T "    c += texture2D(CC_Texture0, p + vec2(%0.6f, %0.6f)).rgb * %0.6f; \n"

    //s.append("    vec3 c = texture2D(CC_Texture0, p).rgb * 0.473688;     \n");
    //snprintf(buf, sizeof(buf) - 1, T, -4.0f / sz.width, 0.0f, 0.026315);    s.append(buf);
    //snprintf(buf, sizeof(buf) - 1, T, -2.0f / sz.width, 0.0f, 0.105263);    s.append(buf);
    //snprintf(buf, sizeof(buf) - 1, T,  2.0f / sz.width, 0.0f, 0.105263);    s.append(buf);
    //snprintf(buf, sizeof(buf) - 1, T,  4.0f / sz.width, 0.0f, 0.026315);    s.append(buf);

    //snprintf(buf, sizeof(buf) - 1, T, 0.0f, -4.0f / sz.height, 0.026315);   s.append(buf);
    //snprintf(buf, sizeof(buf) - 1, T, 0.0f, -2.0f / sz.height, 0.105263);   s.append(buf);
    //snprintf(buf, sizeof(buf) - 1, T, 0.0f,  2.0f / sz.height, 0.105263);   s.append(buf);
    //snprintf(buf, sizeof(buf) - 1, T, 0.0f,  4.0f / sz.height, 0.026315);   s.append(buf);

    s.append("    vec3 c = texture2D(CC_Texture0, p).rgb * 0.666668;     \n");
    snprintf(buf, sizeof(buf) - 1, T, -2.0f / sz.width, 0.0f, 0.083333);    s.append(buf);
    snprintf(buf, sizeof(buf) - 1, T,  2.0f / sz.width, 0.0f, 0.083333);    s.append(buf);

    snprintf(buf, sizeof(buf) - 1, T, 0.0f, -2.0f / sz.height, 0.083333);   s.append(buf);
    snprintf(buf, sizeof(buf) - 1, T, 0.0f,  2.0f / sz.height, 0.083333);   s.append(buf);
   
    s.append("    return c;\n");
    s.append("}");

    return s;
}

void RTTBlur::update(float dt) 
{
    if (!_dirty) return;

    if (_dirty & DT_REBUILD_TEX)
    {
        if (_rtt) _rtt->removeFromParentAndCleanup(true);
        _rtt = RenderTexture::create(_contentSize.width, _contentSize.height, _fmt);
        _rtt->setKeepMatrix(true);
        _rtt->getSprite()->getTexture()->setAntiAliasTexParameters();
        _rtt->getSprite()->setColor(this->getColor());
        this->addChild(_rtt);
    }

    if (_dirty & DT_SHARDER)
    {
        const char *fsh = "\n\
#ifdef GL_ES                                               \n\
precision mediump float;                                   \n\
#endif                                                     \n\
                                                           \n\
varying vec4 v_fragmentColor;                              \n\
varying vec2 v_texCoord;                                   \n\
                                                           \n\
uniform vec2 resolution;                                   \n\
uniform float blurRadius;                                  \n\
uniform float sampleNum;                                   \n\
                                                           \n\
vec3 blur(vec2);                                           \n\
                                                           \n\
void main(void)                                            \n\
{                                                          \n\
	vec3 col = blur(v_texCoord);                           \n\
	gl_FragColor = vec4(col, 1.0) * v_fragmentColor;       \n\
}                                                          \n\
                                                           \n\
vec3 blur(vec2 p)                                          \n\
{                                                          \n\
    if (blurRadius > 0.0 && sampleNum > 1.0)               \n\
    {                                                      \n\
        vec3 col = vec3(0);                                \n\
        vec2 unit = 1.0 / resolution.xy;                   \n\
                                                           \n\
        float r = blurRadius;                              \n\
        float sampleStep = r / sampleNum;                  \n\
                                                           \n\
        float count = 0.0;                                 \n\
                                                           \n\
        for(float x = -r; x < r; x += sampleStep)          \n\
        {                                                  \n\
            for(float y = -r; y < r; y += sampleStep)      \n\
            {                                              \n\
                float weight = (r - abs(x)) * (r - abs(y));\n\
//                if (weight > 0)                            \n\
                col += texture2D(CC_Texture0, p + vec2(x * unit.x, y * unit.y)).rgb * weight; \n\
                count += weight;                           \n\
            }                                              \n\
        }                                                  \n\
                                                           \n\
        return col / count;                                \n\
    }                                                      \n\
                                                           \n\
    return texture2D(CC_Texture0, p).rgb;                  \n\
}";

        //_rtt->getSprite()->setGLProgram(
        //    GLProgram::createWithByteArrays(ccPositionTextureColor_noMVP_vert, gen_vsh(_contentSize).c_str()) );
    }

    //if (_dirty & DT_REBUILD_TEX)
    //{
    //    _rtt->getSprite()->getGLProgramState()->setUniformVec2("resolution", this->getContentSize());
    //}

    //if (_dirty & DT_BLUR_RADIUS)
    //{
    //    _rtt->getSprite()->getGLProgramState()->setUniformFloat("blurRadius", _blur_radius);
    //}

    //if (_dirty & DT_SAMPLE_NUM)
    //{
    //    _rtt->getSprite()->getGLProgramState()->setUniformFloat("sampleNum",  _sample_num);
    //}

    if (_auto_redraw ||
        _dirty & DT_REDRAW)
    {
        _rtt->begin();

        for (auto &n : _rtt_nodes)
        {
            n->visit();
        }

        _rtt->end();
    }

    _dirty = 0;
}

void RTTBlur::setContentSize(const Size& sz)
{
    Node::setContentSize(sz);
    _dirty |= DT_REBUILD_TEX | DT_REDRAW;
}
