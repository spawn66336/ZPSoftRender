#ifndef ZP_RENDERCONTEXT
#define ZP_RENDERCONTEXT

#include "ZPDependency.h"
#include "ZPMathDependency.h"
#include "FrameBuffer.h"

namespace Resource
{
	class Material;
}

namespace Render
{
	class Light;

	const  int MAX_SCREEN_WIDTH = 800;
	const  int MAX_SCREEN_HEIGHT = 600;

	class RenderContext
	{
	public:
		RenderContext(void);
		virtual ~RenderContext(void);

		virtual void Init( const winHandle_t hwnd );
		virtual void Destroy( void );
		virtual void Resize( void );

		virtual void SetClearColor( const Math::Vec4& color);
		virtual void ClearBuffer( unsigned int flag);
		virtual void SwapBuffers( void );

		virtual void EnableTexture2D( bool enable);
		virtual void EnableLight( bool enable );
		virtual void EnableDepthTest( bool enable );

		virtual bool IsTexture2DEnable( void ) const { return m_enableTexture2D; }
		virtual bool IsLightingEnable( void ) const { return m_enableLighting; }
		virtual bool IsDepthTestEnable( void ) const { return m_enableDepthTest; }

		virtual void SetCurrMaterial(  Resource::Material* pMat );
		virtual Resource::Material* GetCurrMaterial( void ) const;

		virtual Light* CreateLight( const String& name );
		virtual bool DeleteLightByName( const String& name );
		virtual Light* FindLightByName( const String& name);
		virtual void DeleteAllLights( void );

		virtual void SetPerspectiveProjection( const Real fov , const Real near , const Real far );
		virtual void SetProjectionMatrix( const Math::Matrix4& mat );
		virtual void PushMatrix();
		virtual void PopMatrix();
		virtual void LoadMatrix( const Math::Matrix4 &mat);
		virtual void LoadIdentity();
		virtual void MultMatrix( const Math::Matrix4 &mat);

		virtual const Math::Matrix4& GetCurrModelViewMatrix( void ) const { return m_currLocalToCamMat; }
		virtual const Math::Matrix4& GetCurrProjectionMatrix( void ) const { return m_currCamToProjMat; }
		virtual const Math::Matrix4& GetCurrProjectionToScreenMatrix( void ) const { return m_currProjToScreenMat; }

		FrameBuffer& GetColorFrameBuffer( void ){ return m_colorFrameBuf; }
		FrameBuffer& GetZFrameBuffer( void ){ return m_zFrameBuf; }

	protected:
		winHandle_t m_hWnd;						///>Windows������ 
		HDC				m_hDC;						///>ͼ���豸������
		RECT				m_wndRect;					///>��ǰ�����С
		Real				m_aspect;						///>�ӿڿ�߱�
		Math::Matrix4 m_currCamToProjMat;      ///>��ǰͶӰ����
		Math::Matrix4 m_currLocalToCamMat; ///>��ǰģ����ͼ����
		Math::Matrix4 m_currProjToScreenMat; 
		Math::Vec4	m_clearColor;				///>�����ɫ
		bool				m_enableTexture2D;		///>��ά�����Ƿ���
		bool				m_enableDepthTest;		///>��Ȳ����Ƿ���
		bool				m_enableLighting;			///>�Ƿ����ù���

		Resource::Material* m_pCurrMaterial; ///>��ǰ����

		typedef std::map<String,Light*> lightTable_t; 
		lightTable_t m_lights;						///>��Դ�б�
		
		typedef std::stack<Math::Matrix4> matrixStack_t;
		matrixStack_t m_modelViewMatrixStack;	///>ģ����ͼ����ջ

		FrameBuffer m_colorFrameBuf;
		FrameBuffer m_zFrameBuf;
	};

}//namespace Render

#endif //ZP_RENDERCONTEXT