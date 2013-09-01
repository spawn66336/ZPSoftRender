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

		//清理所有后台计数器
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
		winHandle_t m_hWnd;						///>Windows窗体句柄 
		HDC				m_hDC;						///>图形设备描述符
		HDC				m_hBackDC;					///>后缓冲区设备描述符
		RECT				m_wndRect;					///>当前窗体大小
		HBITMAP     m_hOldBitmap;				///>用于备份后缓冲区的旧Bitmap
		HBITMAP		m_hBackBuffer;			///>用于同步颜色缓冲区与GDI文字绘制的后缓冲区 

		Real				m_aspect;						///>视口宽高比
		Math::Matrix4 m_currCamToProjMat;      ///>当前投影矩阵
		Math::Matrix4 m_currLocalToCamMat;		///>当前模型视图矩阵
		Math::Matrix4 m_currWorldToCamMat;   ///>当前世界坐标系到相机坐标系矩阵
		Math::Matrix4 m_currProjToScreenMat; 
		Math::Vec4	m_clearColor;				///>清除颜色
		bool				m_enableTexture2D;		///>二维纹理是否开启
		bool				m_enableDepthTest;		///>深度测试是否开启
		bool				m_enableLighting;			///>是否启用光照

		Resource::Material* m_pCurrMaterial; ///>当前材质 
		lightTable_t m_lights;						///>光源列表
		
		typedef std::stack<Math::Matrix4> matrixStack_t;
		matrixStack_t m_modelViewMatrixStack;	///>模型视图矩阵栈
		
		Math::Frustum  m_frustum;				///>视椎体 
		FrameBuffer m_colorFrameBuf;			///>颜色缓冲区
		FrameBuffer m_zFrameBuf;				///>深度缓冲区


		/***********技术参数数据****************/

		int m_iCurrRenderVertexCount;		///>本帧渲染的顶点数
		int m_iCurrRenderFaceCount;			///>当前渲染的面数 
		int m_iFrameCount;							///>帧计数器
		int m_iTotalTicks;
		unsigned int m_uiTickTag;
		int m_iCurrFrameMS;						///>当前帧所花时间（以毫秒计）
		Real m_fFPS;										///>每秒帧数

		/************************************/
	};

}//namespace Render

#endif //ZP_RENDERCONTEXT