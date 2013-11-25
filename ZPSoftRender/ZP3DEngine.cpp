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
#include "D3DRenderImpl.h"
#include "ClipMapTerrain.h"

ZP3DEngine::ZP3DEngine(void):
m_bIsMoving(false),
m_pRenderer(NULL),
m_pCamera(NULL),
m_pFrameListener(NULL)
{
	m_uiShadeModel = Render::NORMMAP_MODEL;
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

	//初始化地形
#ifdef ZP_CLIPMAP_TERRAIN_DEMO
		Terrain::ClipMapTerrain::CreateInstance(); 
#endif
	m_meshMatrix = Math::Matrix4::IDENTITY;

#	if defined( ZP_GL_RENDERER ) 
	m_pRenderer = new Render::GLRenderImpl;
#	elif defined( ZP_SOFT_RENDERER )
	m_pRenderer = new Render::SoftRenderImpl;
#  elif defined( ZP_DX_RENDERER )
	m_pRenderer = new Render::D3DRenderImpl;
#endif

	m_pRenderer->Init( hwnd );
	m_pRenderer->SetClearColor( Math::Vec4( 0.0f,0.0f,0.0f,1.0f) );
	m_pRenderer->EnableLight( true );
	Render::Light* pLight = m_pRenderer->CreateLight( "Light0" );

	if( NULL != pLight )
	{ 
		pLight->SetActive( true );
		pLight->SetDiffuse( Math::Vec4( 1.0f , 1.0f , 1.0f , 1.0f ) );
		pLight->SetPosition( Math::Vec4( -100.0f , -100.0f , -100.0f , 1.0f ) ); 
	}
	
	Render::Light* pLight1 = m_pRenderer->CreateLight( "Light1" );

	if( NULL != pLight1 )
	{ 
		pLight1->SetActive( true );
		pLight1->SetDiffuse( Math::Vec4( 1.0f , 1.0f , 1.0f , 1.0f ) );
		pLight1->SetPosition( Math::Vec4( 100.0f , 100.0f , -100.0f , 1.0f ) ); 
	}
	
	m_pCamera = new Camera;
#ifdef ZP_CLIPMAP_TERRAIN_DEMO
	 m_pCamera->SetPos( Math::Vec3( 1800.0f*4.0f , 0.0f , 1800.0f*4.0f ));
#endif
}

void ZP3DEngine::Destroy( void )
{
	ZP_ASSERT( NULL != m_pRenderer );
	m_pRenderer->Destroy();
	ZP_SAFE_DELETE( m_pRenderer ); 
	ZP_SAFE_DELETE( m_pCamera );

	//销毁地形
#ifdef ZP_CLIPMAP_TERRAIN_DEMO 
	Terrain::ClipMapTerrain::DestroyInstance();
#endif

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

	//更新地形
#ifdef ZP_CLIPMAP_TERRAIN_DEMO
	Terrain::ClipMapTerrain::GetInstance()->Update( m_pCamera->GetPos() );
	Math::Vec3 v3FinalCamPos;
	Terrain::ClipMapTerrain::GetInstance()->CameraCollision( m_pCamera->GetPos() , v3FinalCamPos );
	m_pCamera->SetPos( v3FinalCamPos );
#endif

	if( m_pRenderer->IsActive() )
	{ 
			//摄像机在移动时为了不卡顿切换至线框渲染
			if( m_bIsMoving )
			{
				m_pRenderer->SetShadingModel( Render::WIREFRAME_MODEL );
			}else{
				m_pRenderer->SetShadingModel( (Render::SHADE_MODEL)m_uiShadeModel );
			}

			m_pRenderer->BeginDraw( m_pCamera ); 
	
				Resource::MeshManager::GetInstance()->DrawActiveMesh( m_pRenderer , m_meshMatrix );

			m_pRenderer->EndDraw(); 

	m_pRenderer->SwapBuffers();

	}//if( m_pRenderer->IsActive() )

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


void ZP3DEngine::ResetMesh( void )
{
	m_meshMatrix = Math::Matrix4::IDENTITY;
}


void ZP3DEngine::SwitchMesh( void )
{
	Resource::MeshManager::GetInstance()->SetNextMeshActive();
}

void ZP3DEngine::SwitchShadeModel( void )
{
	m_uiShadeModel = ( m_uiShadeModel+1 )%5;
}

void ZP3DEngine::SetShadeModel( const Render::SHADE_MODEL model )
{
	m_uiShadeModel = model;
}

void ZP3DEngine::RotateMeshWithXAxis( const Real theta )
{
	m_meshMatrix = m_meshMatrix * Math::Matrix4::MakeRotateWithAxisMatrix( 
		Math::Vec3( 1.0f , 0.0f ,0.0f ) , theta );
}

void ZP3DEngine::RotateMeshWithYAxis( const Real theta )
{
	m_meshMatrix = m_meshMatrix * Math::Matrix4::MakeRotateWithAxisMatrix( 
		Math::Vec3( 0.0f , 1.0f ,0.0f ) , theta );
}

void ZP3DEngine::RotateMeshWithZAxis( const Real theta )
{
	m_meshMatrix = m_meshMatrix * Math::Matrix4::MakeRotateWithAxisMatrix( 
		Math::Vec3( 0.0f , 0.0f , 1.0f ) , theta );
}

void ZP3DEngine::SetMovingFlag( const bool flag /*= true */ )
{
	m_bIsMoving = flag;
}

#ifdef ZP_CLIPMAP_TERRAIN_DEMO
void ZP3DEngine::SwitchShowMask( void )
{
	static unsigned int uiShowMasks[] = {
		TERRAIN_MASK_SHOW_ALL ,
		TERRAIN_MASK_SHOW_OUTER_TILES ,
		TERRAIN_MASK_SHOW_CENTER_TILE ,
		TERRAIN_MASK_SHOW_GAP_TILES ,
		TERRAIN_MASK_SHOW_L_FIXED_TILE ,
		TERRAIN_MASK_SHOW_FIXED_RING
	};
	static unsigned int uiMaskIndex = 0; 
	uiMaskIndex = ( uiMaskIndex+1 )%6;
	Terrain::ClipMapTerrain::GetInstance()->SetMask( uiShowMasks[uiMaskIndex] );
} 
#endif


