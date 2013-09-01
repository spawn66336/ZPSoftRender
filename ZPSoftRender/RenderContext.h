#ifndef ZP_RENDERCONTEXT
#define ZP_RENDERCONTEXT

#include "ZPDependency.h"
#include "ZPMathDependency.h"
#include "FrameBuffer.h"
#include "Frustum.h"

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

		typedef std::map<String,Light*> lightTable_t; 

	public:
		RenderContext(void);
		virtual ~RenderContext(void);

		virtual void Init( const winHandle_t hwnd );
		virtual void Destroy( void );
		virtual void Resize( void );

		virtual void SetClearColor( const Math::Vec4& color);
		virtual void ClearBuffer( unsigned int flag);

		virtual void PrepareBackBuffer( void );
		virtual void CopyColorBufferToBackBuffer( void );
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
		virtual lightTable_t& GetLightList( void ){ return m_lights; }

		virtual void SetPerspectiveProjection( const Real fov , const Real n , const Real f );
		virtual void SetProjectionMatrix( const Math::Matrix4& mat );
		virtual void SetWorldToCameraMatrix( const Math::Matrix4& mat );
		virtual void PushMatrix();
		virtual void PopMatrix();
		virtual void LoadMatrix( const Math::Matrix4 &mat);
		virtual void LoadIdentity();
		virtual void MultMatrix( const Math::Matrix4 &mat);

		virtual const Math::Frustum& GetFrustum( void ) const { return m_frustum; }

		virtual void DrawText( const int x , const int y , const String& str );

		virtual const Math::Matrix4& GetCurrModelViewMatrix( void ) const { return m_currLocalToCamMat; }
		virtual const Math::Matrix4& GetCurrWorldToCamMatrix( void ) const { return m_currWorldToCamMat; }
		virtual const Math::Matrix4& GetCurrProjectionMatrix( void ) const { return m_currCamToProjMat; }
		virtual const Math::Matrix4& GetCurrProjectionToScreenMatrix( void ) const { return m_currProjToScreenMat; }

		FrameBuffer& GetColorFrameBuffer( void ){ return m_colorFrameBuf; }
		FrameBuffer& GetDepthFrameBuffer( void ){ return m_zFrameBuf; }

		//�������к�̨������
		void ClearAllCounters( void );

		inline int GetCurrRenderFaceCount( void ) const { return m_iCurrRenderFaceCount; }
		inline void SetCurrRenderFaceCount( const int count ) { m_iCurrRenderFaceCount = count; }
		inline void IncCurrRenderFaceCount( void ){ m_iCurrRenderFaceCount++; }

		inline int GetCurrRenderVertexCount( void ) const { return m_iCurrRenderVertexCount; }
		inline void SetCurrRenderVertexCount( const int count ){ m_iCurrRenderVertexCount = count; }
		inline void IncCurrRenderVertexCount( void ){ m_iCurrRenderVertexCount++; }

		inline Real GetFPS( void ) const { return m_fFPS; }
		inline int GetCurrFrameMS( void ) const { return m_iCurrFrameMS; }
		inline void IncFrameCounter( void ) { m_iFrameCount++; }
		inline void ClearFrameCounter( void ) { m_iFrameCount = 0; }

	protected:
		winHandle_t m_hWnd;						///>Windows������ 
		HDC				m_hDC;						///>ͼ���豸������
		HDC				m_hBackDC;					///>�󻺳����豸������
		RECT				m_wndRect;					///>��ǰ�����С
		HBITMAP     m_hOldBitmap;				///>���ڱ��ݺ󻺳����ľ�Bitmap
		HBITMAP		m_hBackBuffer;			///>����ͬ����ɫ��������GDI���ֻ��Ƶĺ󻺳��� 

		Real				m_aspect;						///>�ӿڿ�߱�
		Math::Matrix4 m_currCamToProjMat;      ///>��ǰͶӰ����
		Math::Matrix4 m_currLocalToCamMat;		///>��ǰģ����ͼ����
		Math::Matrix4 m_currWorldToCamMat;   ///>��ǰ��������ϵ���������ϵ����
		Math::Matrix4 m_currProjToScreenMat; 
		Math::Vec4	m_clearColor;				///>�����ɫ
		bool				m_enableTexture2D;		///>��ά�����Ƿ���
		bool				m_enableDepthTest;		///>��Ȳ����Ƿ���
		bool				m_enableLighting;			///>�Ƿ����ù���

		Resource::Material* m_pCurrMaterial; ///>��ǰ���� 
		lightTable_t m_lights;						///>��Դ�б�
		
		typedef std::stack<Math::Matrix4> matrixStack_t;
		matrixStack_t m_modelViewMatrixStack;	///>ģ����ͼ����ջ
		
		Math::Frustum  m_frustum;				///>��׵�� 
		FrameBuffer m_colorFrameBuf;			///>��ɫ������
		FrameBuffer m_zFrameBuf;				///>��Ȼ�����


		/***********������������****************/

		int m_iCurrRenderVertexCount;		///>��֡��Ⱦ�Ķ�����
		int m_iCurrRenderFaceCount;			///>��ǰ��Ⱦ������ 
		int m_iFrameCount;							///>֡������
		int m_iTotalTicks;
		unsigned int m_uiTickTag;
		int m_iCurrFrameMS;						///>��ǰ֡����ʱ�䣨�Ժ���ƣ�
		Real m_fFPS;										///>ÿ��֡��

		/************************************/
	};

}//namespace Render

#endif //ZP_RENDERCONTEXT