#include "ZP3DEngine.h"
#include "GLRenderImpl.h"
#include "ZPMathDependency.h"
#include "Camera.h"
#include "MeshManager.h"
#include "MaterialManager.h"
#include "TextureManager.h"
#include "Light.h"
#include "RenderPipelineFactory.h"
#include "SoftRenderImpl.h"
#include "FrameStackAllocator.h"

ZP3DEngine::ZP3DEngine(void):
m_pRenderer(NULL),
m_pCamera(NULL),
m_pFrameListener(NULL)
{
	m_uiShadeMode = Render::WIREFRAME_MODEL;
}

ZP3DEngine::~ZP3DEngine(void)
{
}

void ZP3DEngine::Init( const winHandle_t hwnd )
{
	ZP_ASSERT( NULL == m_pRenderer ); 

	Render::FrameStackAllocator::CreateInstance();
	Render::RenderPipelineFactory::CreateInstance();
	Resource::TextureManager::CreateInstance();
	Resource::MaterialManager::CreateInstance();
	Resource::MeshManager::CreateInstance();

	//m_pRenderer = new Render::GLRenderImpl;
	m_pRenderer = new Render::SoftRenderImpl;
	m_pRenderer->Init( hwnd );
	m_pRenderer->SetClearColor( Math::Vec4( 0.0f,0.0f,0.0f,1.0f) );
	m_pRenderer->EnableLight( true );
	Render::Light* pLight = m_pRenderer->CreateLight( "Light0" );
	pLight->SetActive( true );
	pLight->SetDiffuse( Math::Vec4( 1.0f , 1.0f , 1.0f , 1.0f ) );
	Render::Light* pLight1 = m_pRenderer->CreateLight( "Light1" );
	pLight1->SetActive( true );
	pLight1->SetPosition( Math::Vec4( 0.0f , -100.0f , 0.0f , 1.0f ) );
	pLight1->SetDiffuse( Math::Vec4( 0.0f , 1.0f , 0.0f , 1.0f ) );
	Render::Light* pLight2 = m_pRenderer->CreateLight( "Light2" );
	pLight2->SetActive( true );
	pLight2->SetPosition( Math::Vec4( 0.0f , 100.0f , -100.0f , 1.0f ) );
	pLight2->SetDiffuse( Math::Vec4( 1.0f , 1.0f , 1.0f , 1.0f ) );
	
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
	Render::RenderPipelineFactory::DestroyInstance();
	Render::FrameStackAllocator::DestroyInstance();
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
	this->LoadImages();
	this->LoadModels(); 
}

void ZP3DEngine::RenderOneFrame( void )
{
	ZP_ASSERT( NULL != m_pRenderer );

	if( m_pFrameListener )
	{
		m_pFrameListener->FrameStarted();
	}

	m_pRenderer->SetShadingModel( (Render::SHADE_MODEL)m_uiShadeMode );
	m_pRenderer->BeginDraw( m_pCamera ); 
	
		Resource::MeshManager::GetInstance()->DrawActiveMesh( m_pRenderer , Math::Matrix4::IDENTITY );

	m_pRenderer->EndDraw();
	m_pRenderer->SwapBuffers();

	if( m_pFrameListener )
	{
		m_pFrameListener->FrameEnded();
	}
}

void ZP3DEngine::LoadModels( void )
{
	WIN32_FIND_DATAA findData;
	String strModelsPath = ".\\media\\models\\";
	String strWildCard = "*.dae";
	HANDLE hFindFile = ::FindFirstFileA( ( strModelsPath + strWildCard ).c_str() , &findData );

	//打开路径失败
	if( INVALID_HANDLE_VALUE == hFindFile )
	{
		return;
	}

	do{

		if( !( findData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY ) )
		{
			String strFileName = findData.cFileName;
			Resource::MeshManager::GetInstance()->CreateOrRetrieveMesh( strModelsPath + strFileName );
		}

	}while( TRUE == ::FindNextFileA( hFindFile , &findData ) );

	::FindClose( hFindFile );
	hFindFile = NULL;
}

void ZP3DEngine::LoadImages( void )
{
	WIN32_FIND_DATAA findData;
	String strImagesPath = ".\\media\\images\\";
	String strWildCard = "*";
	HANDLE hFindFile = ::FindFirstFileA( ( strImagesPath + strWildCard ).c_str() , &findData );

	//打开路径失败
	if( INVALID_HANDLE_VALUE == hFindFile )
	{
		return;
	}

	do{ 
		if( !( findData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY ) )
		{
			String strFileName = findData.cFileName; 
			Resource::TextureManager::GetInstance()->CreateOrRetrieveTexture2DFromDefaultLocation(  strFileName );
		} 
	}while( TRUE == ::FindNextFileA( hFindFile , &findData ) );

	::FindClose( hFindFile );
	hFindFile = NULL;
}

void ZP3DEngine::RegisterFrameListener( IFrameListener* listener )
{
	m_pFrameListener = listener;
}

void ZP3DEngine::SwitchMesh( void )
{
	Resource::MeshManager::GetInstance()->SetNextMeshActive();
}

void ZP3DEngine::SwitchShadeMode( void )
{
	m_uiShadeMode = ( m_uiShadeMode+1 )%5;
}


