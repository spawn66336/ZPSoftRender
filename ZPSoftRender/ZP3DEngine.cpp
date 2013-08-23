#include "ZP3DEngine.h"
#include "GLRenderImpl.h"
#include "ZPMathDependency.h"
#include "Camera.h"
#include "MeshManager.h"
#include "MaterialManager.h"
#include "TextureManager.h"

ZP3DEngine::ZP3DEngine(void):
m_pRenderer(NULL)
{
}

ZP3DEngine::~ZP3DEngine(void)
{
}

void ZP3DEngine::Init( const winHandle_t hwnd )
{
	ZP_ASSERT( NULL == m_pRenderer ); 

	Resource::TextureManager::CreateInstance();
	Resource::MaterialManager::CreateInstance();
	Resource::MeshManager::CreateInstance();

	m_pRenderer = new Render::GLRenderImpl;
	m_pRenderer->Init( hwnd );
	m_pRenderer->SetClearColor( Math::Vec4( 0.0f,0.0f,0.0f,1.0f) );

	m_pCamera = new Camera;
}

void ZP3DEngine::Destroy( void )
{
	ZP_ASSERT( NULL != m_pRenderer );
	m_pRenderer->Destroy();
	ZP_SAFE_DELETE( m_pRenderer ); 
	ZP_SAFE_DELETE( m_pCamera );

	Resource::MeshManager::DestroyInstance();
	Resource::MaterialManager::DestroyInstance();
	Resource::TextureManager::DestroyInstance();
}

void ZP3DEngine::Resize( void )
{
	ZP_ASSERT( NULL != m_pRenderer );

	m_pRenderer->Resize();
}

Camera* ZP3DEngine::GetCamera( void )
{
	return m_pCamera;
}

void ZP3DEngine::LoadResources( void )
{

}

void ZP3DEngine::RenderOneFrame( void )
{
	ZP_ASSERT( NULL != m_pRenderer );

	m_pRenderer->BeginDraw();
	m_pRenderer->LoadMatrix( m_pCamera->GetCameraMatrix() );
	
		Resource::MeshManager::GetInstance()->DrawActiveMesh( m_pRenderer , Math::Matrix4::IDENTITY );

	m_pRenderer->EndDraw();
	m_pRenderer->SwapBuffers();
}


