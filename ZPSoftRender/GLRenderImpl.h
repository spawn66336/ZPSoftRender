#ifndef ZP_GLRENDERIMPL
#define ZP_GLRENDERIMPL


#include "IRender.h"
#include "ZPMathDependency.h"

class Camera;

namespace Render
{

class GLRenderImpl : public IRender
{
public:
	GLRenderImpl(void);
	virtual ~GLRenderImpl(void);

	virtual void Init( const winHandle_t hwnd ); 
	virtual void Destroy(); 
	virtual void Resize(); 

	virtual void BeginDraw( Camera* pCam ); 
	virtual void EndDraw( void ); 
	virtual void SwapBuffers( void );

	virtual void EnableTexture2D( bool enable); 
	virtual void EnableLight( bool enable ) ; 
	virtual void EnableDepthTest( bool enable ); 

	virtual void SetClearColor( const Math::Vec4& color); 
	virtual void ClearBuffer( unsigned int flag); 

	virtual void ApplyMaterial( Resource::Material* pMaterial );
	virtual  void DrawElements( RenderPrimitive& renderPrimitive );
	 
	virtual void SetShadingModel( SHADE_MODEL type ) ; 

	virtual Light* CreateLight( const String& name ); 
	virtual bool DeleteLightByName( const String& name ); 
	virtual Light* FindLightByName( const String& name); 
	virtual void DeleteAllLights( void ); 

	virtual void SetProjectionMatrix( const Math::Matrix4& mat );
	virtual void PushMatrix(); 
	virtual void PopMatrix();  
	virtual void LoadMatrix( const Math::Matrix4 &mat); 
	virtual void LoadIdentity(); 
	virtual void MultMatrix( const Math::Matrix4 &mat);

protected:

	void _SetupPixelFormat( void ); 
	void _ApplyShadeModel( void );
	void _ApplyAllActiveLights( void );
public:
	void _DrawFrameOfAxes( void );
protected:

	winHandle_t m_hWnd;						///>Windows窗体句柄
	HDC				m_hDC;
	HGLRC			m_hRC;
	RECT				m_wndRect;					///>当前窗体大小
	Math::Matrix4 m_projectionMat;      ///>当前投影矩阵
	Math::Vec4	m_clearColor;				///>清除颜色
	bool				m_enableTexture2D;		///>二维纹理是否开启
	bool				m_enableDepthTest;		///>深度测试是否开启
	bool				m_enableLighting;			///>是否启用光照
	SHADE_MODEL	m_shadeModel;	///>着色模型

	typedef std::map<String,Light*> lightTable_t; 
	lightTable_t m_lights;						///>光源列表

	typedef std::list<unsigned int> commitTextureList_t;
	commitTextureList_t m_commitTexturesList;
};

}//namespace Render

#endif// ZP_GLRENDERIMPL