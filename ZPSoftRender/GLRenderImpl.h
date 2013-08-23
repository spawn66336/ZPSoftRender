#ifndef ZP_GLRENDERIMPL
#define ZP_GLRENDERIMPL


#include "IRender.h"
#include "ZPMathDependency.h"

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

	virtual void BeginDraw( void ); 
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

protected:

	winHandle_t m_hWnd;						///>Windows������
	HDC				m_hDC;
	HGLRC			m_hRC;
	RECT				m_wndRect;					///>��ǰ�����С
	Math::Matrix4 m_projectionMat;      ///>��ǰͶӰ����
	Math::Vec4	m_clearColor;				///>�����ɫ
	bool				m_enableTexture2D;		///>��ά�����Ƿ���
	bool				m_enableDepthTest;		///>��Ȳ����Ƿ���
	bool				m_enableLighting;			///>�Ƿ����ù���
	SHADE_MODEL	m_shadeModel;	///>��ɫģ��

	typedef std::map<String,Light*> lightTable_t; 
	lightTable_t m_lights;						///>��Դ�б�

	typedef std::list<unsigned int> commitTextureList_t;
	commitTextureList_t m_commitTexturesList;
};

}//namespace Render

#endif// ZP_GLRENDERIMPL