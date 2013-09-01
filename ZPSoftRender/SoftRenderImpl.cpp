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
	//���ջ�������ڴ�
	FrameStackAllocator::GetInstance()->Clear();

	this->SetClearColor( Math::Vec4( 0.0f , 0.0f , 0.0f ,1.0f ) );
	this->ClearBuffer( COLOR_BUFFER_FLAG | DEPTH_BUFFER_FLAG ); 

	m_pRenderContext->SetPerspectiveProjection( pCam->GetFov() , 8.0f , 9999.0f );
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
		strShadingModel = "�߿���Ⱦ";
		break;
	case FLAT_MODEL:
		strShadingModel = "Flat ��ɫ";
		break;
	case GOURAUD_MODEL:
		strShadingModel = "Gouraud ��ɫ";
		break;
	case PHONG_MODEL:
		strShadingModel = "Phong ��ɫ";
		break;
	case NORMMAP_MODEL:
		strShadingModel = "������ͼ��ɫ";
		break;
	default:
		break;
	}

	int x = 10 , y = 10;
	int dy = 20;
	m_pRenderContext->DrawText( x , y , strShadingModel );
	y += dy;
	m_pRenderContext->DrawText( x , y , "�� 'C' ���л�ģ��");
	y += dy;
	m_pRenderContext->DrawText( x , y , "�� 'L' ���л���ɫģʽ");
	y += dy;
	m_pRenderContext->DrawText( x , y , "�� 'R' ����λ�����");
	y += dy;
	m_pRenderContext->DrawText( x , y , "�� 'Q' 'E' �������ҡ��");
	y += dy;
	m_pRenderContext->DrawText( x , y , "WSAD + ��� ���������");
	y += dy;
	m_pRenderContext->DrawText( x , y , "����� ����ģ����ת");

	String strFaceCount;
	std::ostringstream   ostr;   
	ostr<<"��Ⱦ����:"<<
		m_pRenderContext->GetCurrRenderFaceCount();
	strFaceCount = ostr.str();

	y += 2*dy;
	m_pRenderContext->DrawText( x , y , strFaceCount );

	String strVertCount;
	ostr.str("");
	ostr<<"��Ⱦ������:"<<
		m_pRenderContext->GetCurrRenderVertexCount();
	strVertCount = ostr.str();

	y += dy;
	m_pRenderContext->DrawText( x , y , strVertCount );

	String strFMS;
	ostr.str("");
	ostr<<"��ǰ֡��ʱ:"<<
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