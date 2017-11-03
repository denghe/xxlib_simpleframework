#include "Precompile.h"
#include "Lightning.h"
#include <vector>
using namespace std;

#define WW_MAX(a, b) ((a) > (b) ? (a) : (b))

float randomFloat()
{
    return ((float)rand() / (float)RAND_MAX) * 2.0f - 1.0f;
}

struct VertexDefine
{
    Vec3 pos;
    Vec2 uv;
};

Lightning::Lightning()
	: m_Time(0.0f)
	, m_Width(10.0f)
	, m_Step(20.0f)
	, m_TextureLength(256.0f)
    , m_spd(-1.0f)
    , m_fps(24)
    , m_center(Vec3::ZERO)
    , m_need_rebuild_mesh(true)
    , m_need_reset_cmd(true)
    , m_Texture(nullptr)
    , m_RenderMesh(nullptr)
    , m_BlendFunc(BlendFunc::ADDITIVE)
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID || CC_TARGET_PLATFORM == CC_PLATFORM_WP8)
    , _backgroundListener(nullptr)
#endif
{
}

Lightning::~Lightning(void)
{
    CC_SAFE_RELEASE(m_Texture);
    CC_SAFE_RELEASE(m_RenderMesh);

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID || CC_TARGET_PLATFORM == CC_PLATFORM_WP8)
    // listen the event that renderer was recreated on Android/WP8
    Director::getInstance()->getEventDispatcher()->removeEventListener(_backgroundListener);
#endif
}

bool Lightning::init()
{
    CC_SAFE_RELEASE(m_Texture);
    m_Texture = Director::getInstance()->getTextureCache()->addImage("common/Lightning.png");
    m_Texture->retain();

    Texture2D::TexParams p;
    p.magFilter = GL_LINEAR;
    p.minFilter = GL_LINEAR;
    p.wrapS = GL_REPEAT;
    p.wrapT = GL_REPEAT;
    m_Texture->setTexParameters(p);

    const char *VSH = "                                               \n\
        // Attributes                                                 \n\
        attribute vec3 a_position;                                    \n\
        attribute vec3 a_normal;                                      \n\
        attribute vec2 a_texCoord;                                    \n\
                                                                      \n\
        // Varyings                                                   \n\
        //varying vec2 v_texCoord;                                    \n\
        #ifdef GL_ES                                                  \n\
        varying mediump vec2 v_texCoord;                              \n\
        #else                                                         \n\
        varying vec2 v_texCoord;                                      \n\
        #endif                                                        \n\
                                                                      \n\
        void main()                                                   \n\
        {                                                             \n\
            gl_Position = CC_MVPMatrix * vec4(a_position, 1.0);       \n\
            v_texCoord = a_texCoord;                                  \n\
        }";

    const char *FSH = "                                               \n\
        // Varyings                                                   \n\
        #ifdef GL_ES                                                  \n\
        varying mediump vec2 v_texCoord;                              \n\
        #else                                                         \n\
        varying vec2 v_texCoord;                                      \n\
        #endif                                                        \n\
                                                                      \n\
        uniform sampler2D u_texture;                                  \n\
        uniform	vec4      u_color;                                    \n\
        uniform vec2      u_diff;                                     \n\
                                                                      \n\
        void main()                                                   \n\
        {                                                             \n\
	        vec2 uv = v_texCoord;                                     \n\
	        uv.x += u_diff.x;                                         \n\
	        uv.y = (uv.y + u_diff.y) * 0.25;                          \n\
                                                                      \n\
            gl_FragColor = texture2D(u_texture, uv.xy) * u_color;     \n\
        }";
    

    setGLProgram(GLProgram::createWithByteArrays(VSH, FSH));

    auto programState = getGLProgramState();

	//将贴图设置给Shader中的变量值u_texture
    programState->setUniformTexture("u_texture", m_Texture);

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID || CC_TARGET_PLATFORM == CC_PLATFORM_WP8)
    // listen the event that renderer was recreated on Android/WP8
    _backgroundListener = EventListenerCustom::create( EVENT_RENDERER_RECREATED,
                                                       [ VSH, FSH, this ]( EventCustom* )
    {
        auto glProgram = this->getGLProgram();
        glProgram->reset();
        glProgram->initWithByteArrays( VSH, FSH );
        glProgram->link();
        glProgram->updateUniforms();

        auto programState = this->getGLProgramState();
	    //将贴图设置给Shader中的变量值u_texture
        programState->setUniformTexture("u_texture", m_Texture);

    });
    Director::getInstance()->getEventDispatcher()->addEventListenerWithFixedPriority(_backgroundListener, -1);
#endif

    return true;
}

bool Lightning::setPoints( const std::vector<Vec3> &pts)
{
    CCAssert( pts.size() >= 2, "Lightning must be have two(or more) Points!");
    if (pts.size() < 2) return false;

    m_points = std::move(pts);

    updateCenter();

    m_need_rebuild_mesh = true;

    return true;
}

void Lightning::updateMesh()
{
    if (!m_need_rebuild_mesh) return;
    if (m_points.size() < 2)  return;
        
    /// 第一次扩展点
    /*
    From:           p3
                   /
        p1--------/
                  p2

    to:
                    p3
                   /
        p1--------/  p3'
                  p2 /
        p1'---------/
                    p2'
    */
 
    PointVec lines[2];
    Vec3  dir, pos, tangent, camDir(0,0,1);
    Vec3  cPos[2], dirs[2];
    float halfWidth = m_Width * 0.5f;

    unsigned int pts_cnt = m_points.size();
    for (unsigned int i = 0; i < pts_cnt; i++)
    {
        auto &ep = m_points[i];

        if (i == 0)
        {
            tangent = m_points[1] - m_points[0];
        }
        else if (i == pts_cnt - 1)
        {
            tangent = m_points[i] - m_points[i - 1];
        }
        else
        {
            tangent = m_points[i + 1] - m_points[i - 1];
        }

        Vec3::cross(tangent, camDir, &dir);
        dir.normalize();
        dir.scale(halfWidth);

        cPos[0] = ep - dir;
        lines[0].emplace_back(cPos[0]);

        cPos[1] = ep + dir;
        lines[1].emplace_back(cPos[1]);
    }

    /// 线性插值，为后面的vertex 做准备
#define SET_VERTEX(p, u, v) \
    m_vertexs.emplace_back(p.x); \
    m_vertexs.emplace_back(p.y); \
    m_vertexs.emplace_back(p.z); \
    m_vertexs.emplace_back(u /*uv.u*/); \
    m_vertexs.emplace_back(v /*uv.v*/); \
    vertexCnt++

    int   vertexCnt = 0;
    float dists[2] = { 0.0f, 0.0f };
    float uvStep;
    float uvOffset = 0.0f;
    float vertexSteps[2] = { 0.0f, 0.0f };
    float interpolationCnt[2] = {0, 0};
    float cnt;

    m_vertexs.clear();
    m_vertexs.reserve(m_points.size() * m_TextureLength / m_Step * 2 * (3/*pos*/ + 2/*uv*/));

    cPos[0] = lines[0][0];
    cPos[1] = lines[1][0];

    SET_VERTEX(cPos[0], uvOffset, 0.0f);
    SET_VERTEX(cPos[1], uvOffset, 1.0f);

    for (unsigned int i = 1; i < m_points.size(); i++)
    {
        cPos [0] = lines[0][i - 1];
        cPos [1] = lines[1][i - 1];

        dirs [0] = lines[0][i] - cPos[0]; 
        dirs [1] = lines[1][i] - cPos[1]; 

        dists[0] = dirs [0].length(); 
        dists[1] = dirs [1].length(); 

        interpolationCnt[0] = floor(dists[0] / m_Step + 0.5f);
        interpolationCnt[1] = floor(dists[1] / m_Step + 0.5f);

        cnt = WW_MAX(interpolationCnt[0], interpolationCnt[1]);

        vertexSteps[0] = dists[0] / cnt;
        vertexSteps[1] = dists[1] / cnt;

        dirs[0].normalize(); 
        dirs[1].normalize(); 

        dirs[0].scale(vertexSteps[0]);
        dirs[1].scale(vertexSteps[1]);

        uvStep = WW_MAX(vertexSteps[0], vertexSteps[1]) / m_TextureLength;

        for (int n = 0; n < cnt; n++)
        {
            cPos[0] += dirs[0];
            cPos[1] += dirs[1];

            uvOffset += uvStep;

            SET_VERTEX(cPos[0], uvOffset, 0.0f);
            SET_VERTEX(cPos[1], uvOffset, 1.0f);
        }
    }

    m_indinces.clear();
    m_indinces.reserve( (vertexCnt - 2) * 2 * 3);
    unsigned short baseIdx;

    for (int i = 0; i < vertexCnt / 2 - 1; i++)
    {
        baseIdx = i << 1;
        m_indinces.emplace_back( baseIdx + 0 );
        m_indinces.emplace_back( baseIdx + 2 );
        m_indinces.emplace_back( baseIdx + 3 );
        m_indinces.emplace_back( baseIdx + 0 );
        m_indinces.emplace_back( baseIdx + 1 );
        m_indinces.emplace_back( baseIdx + 3 );
    }

    MeshVertexAttrib mva;
    std::vector<MeshVertexAttrib> MeshVertexAttribs;

    mva.size = 3;
    mva.type = GL_FLOAT;
    mva.vertexAttrib = GLProgram::VERTEX_ATTRIB_POSITION;
    mva.attribSizeBytes = sizeof(float) * 3;
    MeshVertexAttribs.push_back( mva );

    mva.size = 2;
    mva.type = GL_FLOAT;
    mva.vertexAttrib = GLProgram::VERTEX_ATTRIB_TEX_COORD;
    mva.attribSizeBytes = sizeof(float) * 2;
    MeshVertexAttribs.push_back( mva );

	CC_SAFE_RELEASE_NULL(m_RenderMesh);

    m_RenderMesh = Mesh::create(m_vertexs, m_vertexs.size(), m_indinces, MeshVertexAttribs);
	m_RenderMesh->retain();
	
    long soffset = 0;
    for (auto k = 0; k < m_RenderMesh->getMeshVertexAttribCount(); k++) {
        auto meshattribute = m_RenderMesh->getMeshVertexAttribute(k);
        getGLProgramState()->setVertexAttribPointer(s_attributeNames[meshattribute.vertexAttrib],
                                             meshattribute.size,
                                             meshattribute.type,
                                             GL_FALSE,
											 m_RenderMesh->getVertexSizeInBytes(),
                                             (GLvoid*)soffset);
        soffset += meshattribute.attribSizeBytes;
    }

    m_need_rebuild_mesh = false;
    m_need_reset_cmd = true;
}

Vec3 Lightning::getCenter(void)
{
    return m_center;
}

void Lightning::updateCenter()
{
    Vec3 ptMax, ptMin;
    for (auto &p : m_points)
    {
        if (p.x > ptMax.x) ptMax.x = p.x;
        if (p.y > ptMax.y) ptMax.y = p.y;
        if (p.z > ptMax.z) ptMax.z = p.z;

        if (p.x < ptMin.x) ptMin.x = p.x;
        if (p.y < ptMin.y) ptMin.y = p.y;
        if (p.z < ptMin.z) ptMin.z = p.z;
	}
    m_center = (ptMax + ptMin) / 2;
}

void Lightning::draw(Renderer* renderer, const Mat4 &transform, uint32_t flags)
{
    updateMesh();

    auto *director = Director::getInstance();

    GLProgramState *programstate = getGLProgramState();

    if (m_need_reset_cmd)
    {
        auto textureID = m_Texture->getName();

        m_pMeshCommand.init(_globalZOrder, 
            textureID, 
            programstate, 
            m_BlendFunc, 
            m_RenderMesh->getVertexBuffer(),
            m_RenderMesh->getIndexBuffer(), 
            (GLenum)m_RenderMesh->getPrimitiveType(), 
            (GLenum)m_RenderMesh->getIndexFormat(), 
            m_RenderMesh->getIndexCount(), 
            transform);

        m_pMeshCommand.genMaterialID(textureID, programstate, m_RenderMesh, m_BlendFunc);

        m_pMeshCommand.setCullFaceEnabled(false);
        m_pMeshCommand.setDepthTestEnabled(false);

        m_need_reset_cmd = false;
    }

    m_Time += director->getDeltaTime();

    programstate->setUniformVec2("u_diff", Vec2(m_Time * m_spd, int(m_Time * m_fps) % 4) );

    Color4F color(getDisplayedColor());
    color.a = getDisplayedOpacity() / 255.0f;
    m_pMeshCommand.setDisplayColor(Vec4(color.r, color.g, color.b, color.a));

    director->getRenderer()->addCommand(&m_pMeshCommand);

    Node::draw(renderer,transform,flags);
}
