#include "SoftRenderImpl.h"
#include "RenderContext.h"
#include "RenderPipelineFactory.h"
#include "RenderPipeline.h"

namespace Render
{ 

SoftRenderImpl::SoftRenderImpl(void):
m_pRenderContext(NULL),
m_pRenderPipe(NULL),
m_shadeModel( WIREFRAME_MODEL )
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
	this->SetClearColor( Math::Vec4( 0.0f , 0.0f , 0.0f ,1.0f ) );
	this->ClearBuffer( COLOR_BUFFER_FLAG | DEPTH_BUFFER_FLAG );
}

void SoftRenderImpl::EndDraw( void )
{ 
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



}//namespace Render