#ifndef ZP_SOFTRENDERIMPL
#define ZP_SOFTRENDERIMPL

#include "IRender.h"

namespace Render
{ 
	class RenderContext;
	class RenderPipeline;

	class SoftRenderImpl : public IRender
	{
	public:
		SoftRenderImpl(void);
		virtual ~SoftRenderImpl(void);

		virtual void Init( const winHandle_t hwnd );
		virtual void Destroy();
		virtual void Resize();
		virtual void BeginDraw( Camera* pCam ) ;
		virtual void EndDraw( void );
		virtual void SwapBuffers( void );
		virtual void SetClearColor( const Math::Vec4& color);
		virtual void ClearBuffer(unsigned int flag);
		virtual void ApplyMaterial( Resource::Material* pMaterial );
		virtual  void DrawElements( RenderPrimitive& renderPrimitive );
		virtual void EnableTexture2D( bool enable);
		virtual void EnableLight( bool enable );
		virtual void EnableDepthTest( bool enable );
		virtual void SetShadingModel( SHADE_MODEL type );

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

		void PrintRenderInfo( void );

	protected:

		RenderContext* m_pRenderContext;
		SHADE_MODEL m_shadeModel;			///>着色模型
		RenderPipeline* m_pRenderPipe;			///>渲染管线

	};

}//namespace Render

#endif//ZP_SOFTRENDERIMPL