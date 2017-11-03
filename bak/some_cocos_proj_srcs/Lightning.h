#ifndef _LIGHTNING_H_
#define _LIGHTNING_H_

#include "cocos2d.h"


//闪电链显示类
class Lightning : public cocos2d::Layer
{
public:
    typedef std::vector<Vec3>  PointVec;

public:
	//构造
	Lightning();
	//析构
	~Lightning(void);

	//初始化
    virtual bool init();  
	//创建
    CREATE_FUNC(Lightning);

    //重载绘制处理函数
	virtual void draw(Renderer* renderer, const Mat4 &transform, uint32_t flags);

    bool setPoints( const std::vector<Vec3> &pts);

	//设置宽度
	void setWidth( float f )
	{
		m_Width = f;
	}

	float getWidth(void) const
	{
		return m_Width;
	}

	//设置单个面片网格长，越小曲线越平滑，数值过于小可能带来效率问题
	void setStep( float f )
	{
		m_Step = f;
	}

	float getStep(void) const
	{
		return m_Step;
	}

    void setSpeed( float f )
	{
		m_spd = f;
	}

	float getSpeed(void) const
	{
		return m_spd;
	}

    // 单张纹理长度，调整这个数值可以避免纹理过度拉伸或挤压
	void setTextureLength( float f )
	{
		m_TextureLength = f;
	}
	float getTextureLength(void) const
	{
		return m_TextureLength;
	}

    Vec3 getCenter(void);

private:
    void updateCenter();

	//初始化
	void init( const std::string& materialPath );
	//更新
	void updateMesh(void);

private:
	float						  m_Time;			// 秒
	float						  m_Width;			// 宽
	float						  m_Step;			// 单个面片网格长，越小曲线越平滑，数值过于小可能带来效率问题
    float						  m_TextureLength;	// 单张纹理长度，调整这个数值可以避免纹理过度拉伸或挤压
    float						  m_spd;
    int                           m_fps;

    Vec3                          m_center;            // 包围盒 中心点
    bool                          m_need_rebuild_mesh; // 是否 需要 重建 mesh 数据， 当 step 和 points 变化时，设置true
    bool                          m_need_reset_cmd;

	Texture2D                    *m_Texture;			//纹理
	Mesh						 *m_RenderMesh;		//渲染模型
	BlendFunc					  m_BlendFunc;		//渲染状态

    PointVec                      m_points;            // 点容器
    std::vector<float>            m_vertexs;    //
    std::vector<unsigned short>   m_indinces;   //
	MeshCommand 				  m_pMeshCommand;	//渲染队列

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID || CC_TARGET_PLATFORM == CC_PLATFORM_WP8)
    EventListenerCustom *_backgroundListener;
#endif
};

#endif	//#define _LIGHTNING_H_