#include "SoftRenderImpl.h"
#include "RenderContext.h"
#include "RenderPipelineFactory.h"
#include "RenderPipeline.h"
#include "FrameStackAllocator.h"
#include "Camera.h"

namespace Render
{ 

SoftRenderImpl::SoftRenderImpl(void):
m_pRenderContext(NULL),
m_pRenderPipe(NULL),
m_shadeModel( NORMMAP_MODEL )
{
	m_pRenderContext = new RenderContext;
} 

SoftRenderImpl::~SoftRenderImpl(void)
{
	ZP_SAFE_DELETE( m_pRenderContext );
}

void SoftRenderImpl::Init( const winHandle_t hwnd )
{
	m_pRenderContext->Init( hwnd );
	ZP_ASSERT( NULL == m_pRenderPipe );
	m_pRenderPipe = RenderPipelineFactory::GetInstance()->Create( m_shadeModel );
	ZP_ASSERT( NULL != m_pRenderPipe );
	m_pRenderPipe->SetRenderContext( m_pRenderContext );
}

void SoftRenderImpl::Destroy()
{
	ZP_SAFE_DELETE( m_pRenderPipe );
	m_pRenderContext->Destroy();
}

void SoftRenderImpl::Resize()
{
	m_pRenderContext->Resize();
}

void SoftRenderImpl::BeginDraw( Camera* pCam )
{
	//清空栈分配器内存
	FrameStackAllocator::GetInstance()->Clear();

	this->SetClearColor( Math::Vec4( 0.0f , 0.0f , 0.0f ,1.0f ) );
	this->ClearBuffer( COLOR_BUFFER_FLAG | DEPTH_BUFFER_FLAG ); 

	m_pRenderContext->SetPerspectiveProjection( pCam->GetFov() , 2.0f , 9999.0f );
	m_pRenderContext->SetWorldToCameraMatrix( pCam->GetCameraMatrix() );
	m_pRenderContext->LoadIdentity();
	m_pRenderContext->LoadMatrix( pCam->GetCameraMatrix() );
	m_pRenderContext->PrepareBackBuffer();
	m_pRenderContext->ClearAllCounters();
}

void SoftRenderImpl::EndDraw( void )
{   
	m_pRenderContext->CopyColorBufferToBackBuffer();
	PrintRenderInfo();
}

void SoftRenderImpl::SwapBuffers( void )
{
	m_pRenderContext->SwapBuffers();

}

void SoftRenderImpl::SetClearColor( const Math::Vec4& color )
{
	m_pRenderContext->SetClearColor( color );
}

void SoftRenderImpl::ClearBuffer( unsigned int flag )
{
	m_pRenderContext->ClearBuffer( flag );
}

void SoftRenderImpl::ApplyMaterial( Resource::Material* pMaterial )
{
	m_pRenderContext->SetCurrMaterial( pMaterial );
}

void SoftRenderImpl::DrawElements( RenderPrimitive& renderPrimitive )
{
	m_pRenderPipe->DrawElements( renderPrimitive );
}

void SoftRenderImpl::EnableTexture2D( bool enable )
{
	m_pRenderContext->EnableTexture2D( enable );
}

void SoftRenderImpl::EnableLight( bool enable )
{
	m_pRenderContext->EnableLight( enable );
}

void SoftRenderImpl::EnableDepthTest( bool enable )
{
	m_pRenderContext->EnableDepthTest( enable );
}

void SoftRenderImpl::SetShadingModel( SHADE_MODEL type )
{
	if( type != m_shadeModel )
	{
		ZP_SAFE_DELETE( m_pRenderPipe );
		m_pRenderPipe = RenderPipelineFactory::GetInstance()->Create( type );
		ZP_ASSERT( NULL != m_pRenderPipe );
		m_pRenderPipe->SetRenderContext( m_pRenderContext );
		m_shadeModel = type;
	}
}

Light* SoftRenderImpl::CreateLight( const String& name )
{
	return m_pRenderContext->CreateLight( name );
}

bool SoftRenderImpl::DeleteLightByName( const String& name )
{
	return m_pRenderContext->DeleteLightByName( name );
}

Light* SoftRenderImpl::FindLightByName( const String& name )
{
	return m_pRenderContext->FindLightByName( name );
}

void SoftRenderImpl::DeleteAllLights( void )
{
	m_pRenderContext->DeleteAllLights();
}

void SoftRenderImpl::SetProjectionMatrix( const Math::Matrix4& mat )
{
	m_pRenderContext->SetProjectionMatrix( mat );
}

void SoftRenderImpl::PushMatrix()
{
	m_pRenderContext->PushMatrix();
}

void SoftRenderImpl::PopMatrix()
{
	m_pRenderContext->PopMatrix();
}

void SoftRenderImpl::LoadMatrix( const Math::Matrix4 &mat )
{
	m_pRenderContext->LoadMatrix( mat );
}

void SoftRenderImpl::LoadIdentity()
{
	m_pRenderContext->LoadIdentity();
}

void SoftRenderImpl::MultMatrix( const Math::Matrix4 &mat )
{
	m_pRenderContext->MultMatrix( mat );
}

void SoftRenderImpl::PrintRenderInfo( void )
{
	String strShadingModel = "";

	switch( m_shadeModel )
	{
	case WIREFRAME_MODEL:
		strShadingModel = "线框渲染";
		break;
	case FLAT_MODEL:
		strShadingModel = "Flat 着色";
		break;
	case GOURAUD_MODEL:
		strShadingModel = "Gouraud 着色";
		break;
	case PHONG_MODEL:
		strShadingModel = "Phong 着色";
		break;
	case NORMMAP_MODEL:
		strShadingModel = "法线贴图着色";
		break;
	default:
		break;
	}

	int x = 10 , y = 10;
	int dy = 20;
	m_pRenderContext->DrawText( x , y , strShadingModel );
	y += dy;
	m_pRenderContext->DrawText( x , y , "按 'C' 键切换模型");
	y += dy;
	m_pRenderContext->DrawText( x , y , "按 'L' 键切换着色模式");
	y += dy;
	m_pRenderContext->DrawText( x , y , "按 'R' 键复位摄像机");
	y += dy;
	m_pRenderContext->DrawText( x , y , "按 'Q' 'E' 键摄像机摇摆");
	y += dy;
	m_pRenderContext->DrawText( x , y , "WSAD + 鼠标 控制摄像机");
	y += dy;
	m_pRenderContext->DrawText( x , y , "方向键 控制模型旋转");

	String strFaceCount;
	std::ostringstream   ostr;   
	ostr<<"渲染面数:"<<
		m_pRenderContext->GetCurrRenderFaceCount();
	strFaceCount = ostr.str();

	y += 2*dy;
	m_pRenderContext->DrawText( x , y , strFaceCount );

	String strVertCount;
	ostr.str("");
	ostr<<"渲染顶点数:"<<
		m_pRenderContext->GetCurrRenderVertexCount();
	strVertCount = ostr.str();

	y += dy;
	m_pRenderContext->DrawText( x , y , strVertCount );

	String strFMS;
	ostr.str("");
	ostr<<"当前帧用时:"<<
		m_pRenderContext->GetCurrFrameMS();
	strFMS = ostr.str();
	y += dy;
	m_pRenderContext->DrawText( x , y , strFMS );

	String strFPS;
	ostr.str("");
	ostr<<"FPS:"<<
		m_pRenderContext->GetFPS();
	strFPS = ostr.str();
	y += dy;
	m_pRenderContext->DrawText( x , y , strFPS );


}



}//namespace Render