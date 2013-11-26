#include "D3DRenderImpl.h"
#include "Camera.h"
#include "Light.h" 
#include <D3dx9math.h>
#include "Material.h"
#include "Texture2D.h"
#include "D3DIncludeCallback.h"
#include "ClipMapTerrain.h"
#include "MaterialManager.h"
#include "TextureManager.h"


namespace Render
{


D3DRenderImpl::D3DRenderImpl(void): 
m_bActive(true),
m_pD3D9(NULL),
m_pD3D9Device(NULL), 
m_pDefaultVBDecl(NULL),
m_pCurrMaterial(NULL),
#ifdef ZP_CLIPMAP_TERRAIN_DEMO
m_pTerrainMaterial(NULL),
#endif
m_pColorRT(NULL),
m_pDepthStencilSurface(NULL),
m_pColorRTSurface(NULL),
m_pSavedColorRT(NULL),
m_pSavedDepthStencilSurface(NULL),
m_pD3DHelperVB(NULL),
m_pD3DHelperIB(NULL),
m_pVS(NULL),
m_pPS(NULL),
m_pD3DVSConstantTab(NULL),
m_pD3DPSConstantTab(NULL),
m_pEffectPool(NULL),
m_pGlobalConstEffect(NULL), 
m_uiFVF(0),
m_hWnd(NULL),
m_fAspect(1.0f),
m_uiClearFlag(0),
m_bEnableLighting(false),
m_bEnableTexture(true),
m_bEnableDepthTest(true),
m_shadeModel(WIREFRAME_MODEL)
{
	memset( &m_material , 0 , sizeof( m_material ) );
	memset( &m_d3dParams , 0 , sizeof( m_d3dParams ) );
}


D3DRenderImpl::~D3DRenderImpl(void)
{
	DeleteAllLights();
}

void D3DRenderImpl::Init( const winHandle_t hwnd )
{
	m_hWnd = hwnd;

	m_m4ProjMat = Math::Matrix4::IDENTITY;
	m_m4ViewMat = Math::Matrix4::IDENTITY;
	m_m4WorldMat = Math::Matrix4::IDENTITY;

	//初始化COM
	HRESULT hRes = CoInitialize(NULL); 
	ZP_ASSERT( TRUE == SUCCEEDED( hRes ) );

	//创建D3D9对象
	m_pD3D9 = Direct3DCreate9( D3D_SDK_VERSION );
	ZP_ASSERT( NULL != m_pD3D9 );
	 
	 
	memset( &m_d3dParams , 0 , sizeof(m_d3dParams) );

	RECT wndRect;
	::GetClientRect( hwnd , &wndRect );

	m_viewPort.Width = wndRect.right - wndRect.left;
	m_viewPort.Height = wndRect.bottom - wndRect.top; 
	m_fAspect = static_cast<Real>( m_viewPort.Width ) / static_cast<Real>( m_viewPort.Height );  

	//d3d初始化为窗口模式
	m_d3dParams.Windowed = TRUE;
	//由于是非全屏模式，将后缓冲区宽高设为0则
	//后缓冲区会根据窗口大小自动调整
	m_d3dParams.BackBufferWidth = wndRect.right - wndRect.left;
	m_d3dParams.BackBufferHeight = wndRect.bottom - wndRect.top;
	m_d3dParams.BackBufferFormat = D3DFMT_UNKNOWN;  
	m_d3dParams.SwapEffect = D3DSWAPEFFECT_DISCARD;
	//启用深度与模板缓冲区
	m_d3dParams.EnableAutoDepthStencil = TRUE;
	m_d3dParams.AutoDepthStencilFormat = D3DFMT_D16; 
	 
	UINT AdapterToUse=D3DADAPTER_DEFAULT; 
	D3DDEVTYPE DeviceType=D3DDEVTYPE_HAL; 
	UINT AdapterCount = m_pD3D9->GetAdapterCount();

#ifdef ZP_SUPPORT_PERFHUD 
	for (UINT Adapter=0;Adapter< AdapterCount ;Adapter++) 
	{ 
		D3DADAPTER_IDENTIFIER9 Identifier; 
		HRESULT Res; 

		Res = m_pD3D9->GetAdapterIdentifier(Adapter,0,&Identifier); 
		if (strstr(Identifier.Description,"PerfHUD") != 0) 
		{ 
			AdapterToUse=Adapter; 
			DeviceType=D3DDEVTYPE_REF; 
			::MessageBoxA(NULL , Identifier.Description , NULL , MB_OK );
			break; 
		} 
	} 
#endif 

#ifdef ZP_DEBUG_D3D
	hRes = m_pD3D9->CreateDevice( 
		AdapterToUse , 
		D3DDEVTYPE_REF , 
		m_hWnd , 
		D3DCREATE_HARDWARE_VERTEXPROCESSING, 
		&m_d3dParams ,
		&m_pD3D9Device
		);
#else
	//创建D3D设备
	hRes = m_pD3D9->CreateDevice( 
				AdapterToUse , 
				DeviceType , 
				m_hWnd , 
				D3DCREATE_HARDWARE_VERTEXPROCESSING, 
				&m_d3dParams ,
				&m_pD3D9Device
		);
#endif

	ZP_ASSERT( TRUE == SUCCEEDED( hRes ) );  
	ZP_ASSERT( NULL != m_pD3D9Device );

#ifdef ZP_CLIPMAP_TERRAIN_DEMO
	Terrain::ClipMapTerrain::GetInstance()->Init( m_pD3D9Device , ".\\dem\\ASTGTM2_N35E138_dem.mipmap", 6 , Math::Vec3() , 4.0f ); 
	m_pTerrainMaterial = Resource::MaterialManager::GetInstance()->CreateOrRetrieveMaterial("TerrainMat");
	Resource::Texture2D* pTerrainColorTex = Resource::TextureManager::GetInstance()->CreateOrRetrieveTexture2DFromDefaultLocation("terrain_color.png");
	m_pTerrainMaterial->SetTexture(  pTerrainColorTex , DIFFUSE_CH );
#endif

	if( FAILED(hRes) )
	{
		::MessageBoxA(NULL , "设备创建失败!" , NULL , MB_OK );
	}

	//初始化视口
	m_pD3D9Device->GetViewport( &m_viewPort);
	 
	//初始化渲染目标
	_InitRT();
	 
	//初始化字体
	_InitFont();

	//初始化效果
	_InitEffect();

	//创建顶点缓冲区
	_InitVB();

	//初始化管线
	m_renderPipe.Init( m_pD3D9Device , m_pEffectPool );
}

void D3DRenderImpl::Destroy()
{ 


#ifdef ZP_CLIPMAP_TERRAIN_DEMO
	Terrain::ClipMapTerrain::GetInstance()->Destroy();
#endif

	//销毁管线
	m_renderPipe.Destroy();

	//销毁顶点缓冲区
	_DestroyVB();

	//销毁效果
	_DestroyEffect();

	//销毁字体
	_DestroyFont();

	//销毁纹理 
	_DestroyTextureCache();

	//销毁渲染目标
	_DestroyRT();
	 
	//释放D3D9设备
	 ZP_SAFE_RELEASE( m_pD3D9Device ); 
	 ZP_SAFE_RELEASE( m_pD3D9 ); 
	//销毁COM
	CoUninitialize();
}

void D3DRenderImpl::Resize()
{
	RECT rect;
	::GetClientRect( m_hWnd , &rect );
	m_viewPort.Width = rect.right - rect.left;
	m_viewPort.Height = rect.bottom - rect.top; 
	m_fAspect = static_cast<Real>( m_viewPort.Width ) / static_cast<Real>( m_viewPort.Height );  

	if( m_viewPort.Width == 0 ||
		m_viewPort.Height == 0 )
	{
		return;
	} 

	////若窗口尺寸大小没变则不用调整
	if( m_viewPort.Width == m_d3dParams.BackBufferWidth &&
		m_viewPort.Height == m_d3dParams.BackBufferHeight )
	{
		return;
	}

	if( NULL != m_pD3D9Device )
	{
		_OnDeviceLost();
		m_d3dParams.BackBufferWidth = m_viewPort.Width;
		m_d3dParams.BackBufferHeight = m_viewPort.Height;
		m_d3dParams.BackBufferFormat = D3DFMT_UNKNOWN;
		HRESULT hRes = m_pD3D9Device->Reset( &m_d3dParams ); 
		if( SUCCEEDED( hRes ) )
		{
			_OnDeviceReset();
		}else{
			m_bActive = false;
		}
	}
}

void D3DRenderImpl::BeginDraw( Camera* pCam )
{     
	_ApplyRenderState(); 
	_ApplyShadeModel();

	m_pD3D9Device->Clear( 
		0 , NULL , 
		D3DCLEAR_TARGET|D3DCLEAR_ZBUFFER , 
		D3DCOLOR_XRGB( 0 , 0 , 0 ) , 1.0f , 0 );  
	 
	_ApplyAllLights(); 
	 
	m_pD3D9Device->BeginScene();   
	_SetupMatrices( pCam ); 
}

void D3DRenderImpl::EndDraw( void )
{    

#ifdef ZP_CLIPMAP_TERRAIN_DEMO

	Resource::Texture2D* pTerrainTex = m_pTerrainMaterial->GetTexture( DIFFUSE_CH );  
	_CommitTexture( pTerrainTex , DIFFUSE_CH );

	Terrain::ClipMapTerrain::GetInstance()->GetRenderOps( m_terrainTmpRenderOps );
	auto itOp = m_terrainTmpRenderOps.begin();
	while( itOp != m_terrainTmpRenderOps.end() )
	{
		m_renderPipe.PushRenderOp( NULL , *itOp );
		++itOp;
	}
	m_terrainTmpRenderOps.clear();
#endif
	if( NULL != m_pGlobalConstEffect )
	{
		m_pGlobalConstEffect->SetMatrix( "m4View" , (D3DXMATRIX*)&m_m4ViewMat  );
		m_pGlobalConstEffect->SetMatrix( "m4Proj" , (D3DXMATRIX*)&m_m4ProjMat ); 
		//m_pGlobalConstEffect->SetMatrix( "m4World" , (D3DXMATRIX*)m_m4WorldMat.m  );
	}

	m_renderPipe.Render();
	m_renderPipe.Reset();

	_DrawHelper();

	String strShadeModel;
	switch( m_shadeModel )
	{
	case 	WIREFRAME_MODEL:
		strShadeModel = "线框渲染";
		break;
	case FLAT_MODEL:
		strShadeModel = "Flat渲染";
		break;
	case GOURAUD_MODEL:
		strShadeModel = "Gouraud渲染";
		break;
	case PHONG_MODEL:
		strShadeModel = "Phong渲染";
		break;
	case NORMMAP_MODEL:
		strShadeModel = "凹凸渲染";
		break;
	default:
		break;
	}
	_DrawText( 0 , 0 , strShadeModel.c_str() );
 
	m_pD3D9Device->EndScene();
	  
}

void D3DRenderImpl::SwapBuffers( void )
{  
	HRESULT hRes = m_pD3D9Device->Present( NULL , NULL , NULL , NULL );

	if( FAILED( hRes ) )
	{
		m_bActive = false; 
	}
}

void D3DRenderImpl::SetClearColor( const Math::Vec4& color )
{
	m_v4ClearColor = color;
}

void D3DRenderImpl::ClearBuffer( unsigned int flag )
{
	m_uiClearFlag = flag;
}

void D3DRenderImpl::ApplyMaterial( Resource::Material* pMaterial )
{
	m_pCurrMaterial = pMaterial;

	if( NULL == pMaterial )
	{
		return;
	}

	HRESULT hRes = 0; 
	Resource::Texture2D* pDiffuseTex = pMaterial->GetTexture( DIFFUSE_CH );  
	_CommitTexture( pDiffuseTex , DIFFUSE_CH );

	Resource::Texture2D* pNormTex = pMaterial->GetTexture( BUMPMAP_CH ); 
	_CommitTexture( pNormTex , BUMPMAP_CH );
	 
}


void D3DRenderImpl::UnapplyMaterial( Resource::Material* pMaterial )
{
	m_pCurrMaterial = NULL;
}


void D3DRenderImpl::_CommitTexture( Resource::Texture2D* pTexture  , const int tex_ch )
{

	HRESULT hRes = 0; 

	if(NULL != pTexture )
	{ 

		IDirect3DTexture9* pD3DTexture = NULL;
		unsigned int uiTexHandle = pTexture->GetTextureHandle();
		pD3DTexture = (IDirect3DTexture9*)uiTexHandle;

		if( 0 != uiTexHandle )
		{
			textureList_t::iterator itTex = m_textureCache.find( uiTexHandle );
			if( itTex == m_textureCache.end() )
			{
				pD3DTexture = NULL;
				uiTexHandle = 0;
			}
		}

		if( NULL == pD3DTexture )
		{ 
			hRes = m_pD3D9Device->CreateTexture( 
				pTexture->Width() , pTexture->Height() , 1 ,  
				D3DUSAGE_DYNAMIC ,  D3DFMT_A8R8G8B8  , D3DPOOL_DEFAULT , &pD3DTexture , NULL );

			if( FAILED( hRes ) )
			{
				return;
			}
			  
			D3DLOCKED_RECT lockedRect; 
			memset( &lockedRect , 0 , sizeof( lockedRect ) );
			hRes = pD3DTexture->LockRect( 0 , &lockedRect , NULL , D3DLOCK_DISCARD );
			if( SUCCEEDED( hRes ) )
			{ 
					unsigned char* pDiffBuf = (unsigned char*)lockedRect.pBits;
					for( int y = 0 ; y < pTexture->Height() ; y++ )
					{
						for( int x = 0 ; x < pTexture->Width() ; x++ )
						{
							int offset = lockedRect.Pitch*y + x*4;

							pDiffBuf[offset]	 =  pTexture->Pixels()[offset+2];
							pDiffBuf[offset+1] =  pTexture->Pixels()[offset+1];
							pDiffBuf[offset+2] =  pTexture->Pixels()[offset];
							pDiffBuf[offset+3] =  pTexture->Pixels()[offset+3];
						}//for( int x = 0 ; x < pTexture->Width() ; x++ )
					}//for( int y = 0 ; y < pTexture->Height() ; y++ )

					pD3DTexture->UnlockRect( 0 );  
					uiTexHandle = *( reinterpret_cast<unsigned int*>(&pD3DTexture) );
					pTexture->SetTextureHandle( uiTexHandle ); 
					m_textureCache.insert( std::make_pair( uiTexHandle , pD3DTexture ) ); 
			}else{
				pD3DTexture->Release();
				pD3DTexture = NULL;
			}

		}//if( NULL == pD3DTexture )
	 
			//将在资源中记录纹理对象，今后用于管线渲染
			pTexture->SetUserPointer( pD3DTexture );
  
	}//	if(NULL != pTexture )
}


void D3DRenderImpl::DrawElements( RenderPrimitive& renderPrimitive )
{ 
	D3DRenderOperation* pOp = m_RenderOpCache.CreateRenderOperation( m_pCurrSubMesh );
	_PrepareRenderOperation( renderPrimitive , pOp );
	pOp->m_worldMat = m_m4WorldMat;
	m_renderPipe.PushRenderOp( m_pCurrMaterial , pOp );  
}

void D3DRenderImpl::EnableTexture2D( bool enable )
{
	m_bEnableTexture = enable;
}

void D3DRenderImpl::EnableLight( bool enable )
{
	m_bEnableLighting = enable;
}

void D3DRenderImpl::EnableDepthTest( bool enable )
{
	m_bEnableDepthTest = enable;
}

void D3DRenderImpl::SetShadingModel( SHADE_MODEL type )
{
	m_shadeModel = type;
}

Light* D3DRenderImpl::CreateLight( const String& name )
{
	Light* pLight = FindLightByName( name );

	if( pLight )
	{
		return pLight;
	}

	pLight = new Light;
	pLight->SetName( name ); 
	m_lights.insert( std::make_pair( name , pLight ) );
	return pLight;
}

bool D3DRenderImpl::DeleteLightByName( const String& name )
{
	lightTable_t::iterator itFindLight = m_lights.find( name );

	//未找到此名称的光源
	if( itFindLight == m_lights.end() )
	{
		return false;
	}

	ZP_SAFE_DELETE( itFindLight->second );
	m_lights.erase( itFindLight );
	return true;
}

Light* D3DRenderImpl::FindLightByName( const String& name )
{
	lightTable_t::iterator itFindLight = m_lights.find( name );

	//未找到此名称的光源
	if( itFindLight == m_lights.end() )
	{
		return NULL;
	}
	return itFindLight->second;
}

void D3DRenderImpl::DeleteAllLights( void )
{
	lightTable_t::iterator it = m_lights.begin();
	while( it != m_lights.end() )
	{
		ZP_SAFE_DELETE( it->second );
		it++;
	}
	m_lights.clear();
}

void D3DRenderImpl::SetProjectionMatrix( const Math::Matrix4& mat )
{
	m_m4ProjMat = mat;
}

void D3DRenderImpl::PushMatrix()
{
	m_worldMatrixStack.push( m_m4WorldMat );
}

void D3DRenderImpl::PopMatrix()
{
	if( m_worldMatrixStack.size() > 0 )
	{
		m_m4WorldMat = m_worldMatrixStack.top();
		m_worldMatrixStack.pop();
	}
}

void D3DRenderImpl::LoadMatrix( const Math::Matrix4 &mat )
{
	m_m4WorldMat = mat;
}

void D3DRenderImpl::LoadIdentity()
{
	m_m4WorldMat = Math::Matrix4::IDENTITY;
}

void D3DRenderImpl::MultMatrix( const Math::Matrix4 &mat )
{
	m_m4WorldMat = mat * m_m4WorldMat;
}

void D3DRenderImpl::_InitVB( void )
{
	D3DVERTEXELEMENT9 declElems[6] ={
		{ 0 , 0 , D3DDECLTYPE_FLOAT3 , D3DDECLMETHOD_DEFAULT , D3DDECLUSAGE_POSITION , 0 },
		{ 0 , sizeof(float)*3 , D3DDECLTYPE_FLOAT3 , D3DDECLMETHOD_DEFAULT , D3DDECLUSAGE_NORMAL , 0 },
		{ 0 , sizeof(float)*6 , D3DDECLTYPE_FLOAT2 , D3DDECLMETHOD_DEFAULT , D3DDECLUSAGE_TEXCOORD , 0 },
		{ 0 , sizeof(float)*8 , D3DDECLTYPE_FLOAT3 , D3DDECLMETHOD_DEFAULT , D3DDECLUSAGE_TEXCOORD , 1 },
		{ 0 , sizeof(float)*11 , D3DDECLTYPE_FLOAT3 , D3DDECLMETHOD_DEFAULT , D3DDECLUSAGE_TEXCOORD , 2 },
		D3DDECL_END()
	};
	m_pD3D9Device->CreateVertexDeclaration( declElems  ,&m_pDefaultVBDecl ); 
	_InitHelperVB();

}

void D3DRenderImpl::_DestroyVB( void )
{
	_DestroyHelperVB(); 
	ZP_SAFE_RELEASE( m_pDefaultVBDecl ); 
}

void D3DRenderImpl::_InitHelperVB( void )
{
	const int MAX_VERTEX_NUM = 6; 
	const unsigned int uiVBLockSize = MAX_VERTEX_NUM*sizeof(d3dRenderVert2_t);
	 
	//创建足够大的顶点缓冲区
	HRESULT hRes = m_pD3D9Device->CreateVertexBuffer( 
		uiVBLockSize , D3DUSAGE_WRITEONLY , 	D3DFVF_XYZ|D3DFVF_DIFFUSE , D3DPOOL_DEFAULT , &m_pD3DHelperVB , NULL );
	ZP_ASSERT( TRUE == SUCCEEDED( hRes ) );

	d3dRenderVert2_t* pVertBuf = NULL;
	m_pD3DHelperVB->Lock( 0 , uiVBLockSize , (void**)( &pVertBuf ) , 0 ); 

			//X轴
			pVertBuf[0].pos = Math::Vec3( 0.0f , 0.0f , 0.0f );
			pVertBuf[0].diffuse =  D3DCOLOR_XRGB( 255 , 0 , 0 );
			pVertBuf[1].pos = Math::Vec3( 100.0f , 0.0f , 0.0f );
			pVertBuf[1].diffuse =  D3DCOLOR_XRGB( 255 , 0 , 0 );

			//Y轴
			pVertBuf[2].pos = Math::Vec3( 0.0f , 0.0f , 0.0f );
			pVertBuf[2].diffuse =  D3DCOLOR_XRGB( 0 , 255 , 0 );
			pVertBuf[3].pos = Math::Vec3( 0.0f , 100.0f , 0.0f );
			pVertBuf[3].diffuse =  D3DCOLOR_XRGB( 0 , 255 , 0 );

			//Z轴
			pVertBuf[4].pos = Math::Vec3( 0.0f , 0.0f , 0.0f );
			pVertBuf[4].diffuse =  D3DCOLOR_XRGB( 0 , 0 , 255 );
			pVertBuf[5].pos = Math::Vec3( 0.0f , 0.0f , 100.0f );
			pVertBuf[5].diffuse =  D3DCOLOR_XRGB( 0 , 0 , 255 );

	m_pD3DHelperVB->Unlock(); 
}

void D3DRenderImpl::_DestroyHelperVB( void )
{
	ZP_SAFE_RELEASE( m_pD3DHelperVB ); 
	ZP_SAFE_RELEASE( m_pD3DHelperIB ); 
}

 

void D3DRenderImpl::_PrepareRenderOperation( RenderPrimitive& renderPrimitive , D3DRenderOperation* pOp )
{
	//若此RenderOp已经初始化则无需构建
	if( pOp->m_pVB )
	{
		return;
	}

	LPDIRECT3DVERTEXBUFFER9 pNewVB = NULL;		//用于模型的顶点缓冲区
	LPDIRECT3DINDEXBUFFER9  pNewIB = NULL;			//用于模型的索引缓冲区

	unsigned int uiVertCount = renderPrimitive.VertexBuf().Count();
	unsigned int uiIndexCount = renderPrimitive.IndicesCount(); 
	 
	//拷贝顶点
	unsigned int uiVBLockSize = uiVertCount*sizeof(d3dRenderVert_t); 

	//创建顶点缓冲区
	HRESULT hRes = m_pD3D9Device->CreateVertexBuffer( 
		uiVBLockSize , D3DUSAGE_WRITEONLY , 0 , D3DPOOL_DEFAULT , &pNewVB , NULL );
	ZP_ASSERT( TRUE == SUCCEEDED( hRes ) );
	 

	void* pBuf = NULL;
	hRes = pNewVB->Lock( 0 , uiVBLockSize , &pBuf  , 0 );
	ZP_ASSERT( TRUE == SUCCEEDED( hRes ) );
	ZP_ASSERT( NULL != pBuf );

	for( int i = 0; i < (int)uiVertCount ; i++ )
	{
		((d3dRenderVert_t*)pBuf)[i].pos = renderPrimitive.VertexBuf().Pointer()[i].m_pos;
		((d3dRenderVert_t*)pBuf)[i].norm = renderPrimitive.VertexBuf().Pointer()[i].m_norm; 
		((d3dRenderVert_t*)pBuf)[i].tex = renderPrimitive.VertexBuf().Pointer()[i].m_texcoord;
		((d3dRenderVert_t*)pBuf)[i].binorm = renderPrimitive.VertexBuf().Pointer()[i].m_binormal;
		((d3dRenderVert_t*)pBuf)[i].tangent = renderPrimitive.VertexBuf().Pointer()[i].m_tangent;
	}

	pNewVB->Unlock();
	pBuf = NULL;

	//拷贝索引
	unsigned int uiIBLockSize = sizeof(unsigned short)*renderPrimitive.IndicesCount();

	//创建索引缓冲区
	hRes = m_pD3D9Device->CreateIndexBuffer( 
		uiIBLockSize , D3DUSAGE_WRITEONLY , D3DFMT_INDEX16 , D3DPOOL_DEFAULT , &pNewIB , NULL );
	ZP_ASSERT( TRUE == SUCCEEDED( hRes ) );

	hRes = pNewIB->Lock( 0 , uiIBLockSize , &pBuf , 0  );
	ZP_ASSERT( TRUE == SUCCEEDED( hRes ) );
	ZP_ASSERT( NULL != pBuf );

	for( int i = 0 ; i < renderPrimitive.IndicesCount() ; i++ )
	{
		( (unsigned short*)pBuf )[i] = (unsigned short)renderPrimitive.Indices()[i];
	}

	pNewIB->Unlock();
	pBuf = NULL;


	IDirect3DVertexDeclaration9* pDecl = NULL;
	D3DVERTEXELEMENT9 declElems[] ={
		{ 0 , 0 , D3DDECLTYPE_FLOAT3 , D3DDECLMETHOD_DEFAULT , D3DDECLUSAGE_POSITION , 0 },
		{ 0 , sizeof(float)*3 , D3DDECLTYPE_FLOAT3 , D3DDECLMETHOD_DEFAULT , D3DDECLUSAGE_NORMAL , 0 },
		{ 0 , sizeof(float)*6 , D3DDECLTYPE_FLOAT2 , D3DDECLMETHOD_DEFAULT , D3DDECLUSAGE_TEXCOORD , 0 },
		{ 0 , sizeof(float)*8 , D3DDECLTYPE_FLOAT3 , D3DDECLMETHOD_DEFAULT , D3DDECLUSAGE_TEXCOORD , 1 },
		{ 0 , sizeof(float)*11 , D3DDECLTYPE_FLOAT3 , D3DDECLMETHOD_DEFAULT , D3DDECLUSAGE_TEXCOORD , 2 },
		D3DDECL_END()
	};
	hRes = m_pD3D9Device->CreateVertexDeclaration( declElems  ,&pDecl );
	ZP_ASSERT( TRUE == SUCCEEDED( hRes ) );

	//填充渲染操作
	pOp->m_streamIndex = 0;
	pOp->m_pVertexDecl = pDecl;
	pOp->m_pVB = pNewVB;
	pOp->m_pIB = pNewIB;
	pOp->m_primitiveType = D3DPT_TRIANGLELIST;
	pOp->m_stride = sizeof(d3dRenderVert_t);
	pOp->m_vertexCount = uiVertCount;
	pOp->m_primCount = uiIndexCount/3;
	pOp->m_worldMat = m_m4WorldMat;
}


void D3DRenderImpl::_PrepareRender( RenderPrimitive& renderPrimitive )
{
	//unsigned int uiVertCount = renderPrimitive.VertexBuf().Count();
	//unsigned int uiIndexCount = renderPrimitive.IndicesCount(); 
	////拷贝顶点
	//unsigned int uiVBLockSize = uiVertCount*sizeof(d3dRenderVert_t); 
	//void* pBuf = NULL;
	//HRESULT hRes = m_pD3DVB->Lock( 0 , uiVBLockSize , &pBuf  , 0 );
	//ZP_ASSERT( TRUE == SUCCEEDED( hRes ) );
	//ZP_ASSERT( NULL != pBuf );
	// 
	//for( int i = 0; i < (int)uiVertCount ; i++ )
	//{
	//	((d3dRenderVert_t*)pBuf)[i].pos = renderPrimitive.VertexBuf().Pointer()[i].m_pos;
	//	((d3dRenderVert_t*)pBuf)[i].norm = renderPrimitive.VertexBuf().Pointer()[i].m_norm; 
	//	((d3dRenderVert_t*)pBuf)[i].tex = renderPrimitive.VertexBuf().Pointer()[i].m_texcoord;
	//	((d3dRenderVert_t*)pBuf)[i].binorm = renderPrimitive.VertexBuf().Pointer()[i].m_binormal;
	//	((d3dRenderVert_t*)pBuf)[i].tangent = renderPrimitive.VertexBuf().Pointer()[i].m_tangent;
	//}

	//m_pD3DVB->Unlock();
	//pBuf = NULL;

	////拷贝索引
	//unsigned int uiIBLockSize = sizeof(unsigned short)*renderPrimitive.IndicesCount();

	//hRes = m_pD3DIB->Lock( 0 , uiIBLockSize , &pBuf , 0  );
	//ZP_ASSERT( TRUE == SUCCEEDED( hRes ) );
	//ZP_ASSERT( NULL != pBuf );

	//for( int i = 0 ; i < renderPrimitive.IndicesCount() ; i++ )
	//{
	//	( (unsigned short*)pBuf )[i] = (unsigned short)renderPrimitive.Indices()[i];
	//}

	//m_pD3DIB->Unlock();
	//pBuf = NULL;
}

void D3DRenderImpl::_DrawHelper( void )
{
	_UnapplyShaders();

	D3DXMATRIX worldMat;
	D3DXMatrixIdentity( &worldMat ); 
	m_pD3D9Device->SetRenderState( D3DRS_LIGHTING , FALSE);
	m_pD3D9Device->SetTexture(0,0);
	m_pD3D9Device->SetTransform( D3DTS_WORLD , &worldMat );
	m_pD3D9Device->SetStreamSource( 0 , m_pD3DHelperVB , 0 , sizeof(d3dRenderVert2_t) );
	m_pD3D9Device->SetFVF( D3DFVF_XYZ|D3DFVF_DIFFUSE ); 
	m_pD3D9Device->DrawPrimitive( D3DPT_LINELIST , 0 , 3 ); 
	 
}

void D3DRenderImpl::_SetupMatrices( Camera* pCam )
{  
	if( NULL == pCam )
	{
		return;
	} 
	m_m4ViewMat = pCam->GetCameraMatrix(); 
	m_pD3D9Device->SetTransform(  D3DTS_VIEW , (D3DMATRIX*)( m_m4ViewMat.m ) );

	m_m4ProjMat = Math::Matrix4::MakeD3DProjectionMatrix( 60.0f , m_fAspect , 2.0f , 9999000.0f );  
	m_pD3D9Device->SetTransform(  D3DTS_PROJECTION ,  (D3DMATRIX*)( &m_m4ProjMat.m ) );   
}

void D3DRenderImpl::_OnDeviceReset( void )
{ 
	_ResizeRT( m_viewPort.Width , m_viewPort.Height );
	m_pFont->OnResetDevice();
	_InitEffect();
	_InitVB();
	m_renderPipe.Init( m_pD3D9Device , m_pEffectPool );
#ifdef ZP_CLIPMAP_TERRAIN_DEMO
	Terrain::ClipMapTerrain::GetInstance()->OnResetDevice();
#endif
	//m_renderPipe.OnResetDevice();

}

void D3DRenderImpl::_OnDeviceLost( void )
{ 
#ifdef ZP_CLIPMAP_TERRAIN_DEMO
	Terrain::ClipMapTerrain::GetInstance()->OnLostDevice();
#endif
	m_RenderOpCache.Clear();
	m_renderPipe.Destroy();
	//m_renderPipe.OnLostDevice();
	_DestroyVB();   
	_DestroyEffect();
	m_pFont->OnLostDevice();
	_DestroyTextureCache();
	_DestroyRT();
}

void D3DRenderImpl::_ApplyRenderState( void )
{
	m_pD3D9Device->SetRenderState( D3DRS_CULLMODE, D3DCULL_CW );
	m_pD3D9Device->SetRenderState( D3DRS_LIGHTING, m_bEnableLighting ); 
	m_pD3D9Device->SetRenderState( D3DRS_ZENABLE, m_bEnableDepthTest );  
}

void D3DRenderImpl::_DestroyTextureCache( void )
{
	textureList_t::iterator itTex = m_textureCache.begin();
	while( itTex != m_textureCache.end() )
	{
		(*itTex).second->Release();
		(*itTex).second = NULL;
		itTex++;
	}
	m_textureCache.clear();

	
}

void D3DRenderImpl::_ApplyShadeModel( void )
{
	unsigned int uiShadeModel = D3DSHADE_PHONG; 
	unsigned int uiFillMode = D3DFILL_SOLID;
	switch( m_shadeModel )
	{
	case 	WIREFRAME_MODEL:
		uiFillMode = D3DFILL_WIREFRAME;
		break;
	case FLAT_MODEL:
		uiShadeModel = D3DSHADE_FLAT;
		break;
	case GOURAUD_MODEL:
		uiShadeModel = D3DSHADE_GOURAUD;
		break;
	case PHONG_MODEL:
		uiShadeModel = D3DSHADE_PHONG;
		break;
	case NORMMAP_MODEL:
		uiShadeModel = D3DSHADE_PHONG;
		break;
	default:
		break;
	}

	m_pD3D9Device->SetRenderState( D3DRS_FILLMODE , uiFillMode );
	m_pD3D9Device->SetRenderState( D3DRS_SHADEMODE , uiShadeModel );
}

void D3DRenderImpl::_ApplyAllLights( void )
{
	if( m_bEnableLighting )
	{ 

		int iLightIndx = 0; 
		lightTable_t::iterator itLight = m_lights.begin();
		while( itLight != m_lights.end() )
		{
			Light* pLight = (*itLight).second;
			  
			if( pLight->IsActive() )
			{
				memset( &m_currLight , 0 , sizeof(m_currLight) ); 
				m_currLight.Type = D3DLIGHT_POINT;
				memcpy( &m_currLight.Ambient , &( pLight->Ambient() ) , sizeof(D3DCOLORVALUE) );
				memcpy( &m_currLight.Diffuse , &( pLight->Diffuse() ) , sizeof(D3DCOLORVALUE) );
				memcpy( &m_currLight.Specular , &( pLight->Specular() ) , sizeof(D3DCOLORVALUE) );
				memcpy( &m_currLight.Position , &( pLight->Position()) , sizeof(D3DVECTOR) );
				m_currLight.Attenuation0 = 1.0f; 
				m_currLight.Range = 1000.0f; 

				m_pD3D9Device->SetLight( iLightIndx, &m_currLight );  
				m_pD3D9Device->LightEnable( iLightIndx, TRUE ); 
			}else{
				m_pD3D9Device->LightEnable( iLightIndx , FALSE );
			}

			iLightIndx++;
			++itLight;
		}
	}
}

bool D3DRenderImpl::_LoadVertexShader( const String& filename , IDirect3DVertexShader9 ** ppVS )
{ 
	String strVSProfile = "vs_3_0";
	String strSupportVSProfiles = D3DXGetVertexShaderProfile( m_pD3D9Device );

	//若当前设备不支持所需要的顶点着色器
	if( String::npos == strSupportVSProfiles.find( strVSProfile ) )
	{
		return false;
	}

	LPD3DXBUFFER pShaderData = NULL;
	LPD3DXBUFFER pErrMsg = NULL;

	HRESULT hRes = D3DXCompileShaderFromFileA( 
		filename.c_str() , NULL , NULL , "main" , strVSProfile.c_str() , 0 , &pShaderData , &pErrMsg , &m_pD3DVSConstantTab  );

	if( TRUE == FAILED(hRes) )
	{
		String strErrMsg = (char*)pErrMsg->GetBufferPointer();
		return false;
	}

	 hRes = m_pD3D9Device->CreateVertexShader( 
		(DWORD*)pShaderData->GetBufferPointer() , ppVS );

	if( TRUE == FAILED(hRes) )
	{
		ZP_SAFE_RELEASE( pErrMsg );
		ZP_SAFE_RELEASE( pShaderData );
		return false;
	}

	ZP_SAFE_RELEASE( pErrMsg );
	ZP_SAFE_RELEASE( pShaderData );
	return true;
}

bool D3DRenderImpl::_LoadPixelShader( const String& filename , IDirect3DPixelShader9 ** ppPS )
{
	String strPSProfile = "ps_3_0";
	String strSupportPSProfiles = D3DXGetPixelShaderProfile( m_pD3D9Device );

	//若当前设备不支持所需要的像素着色器
	if( String::npos == strSupportPSProfiles.find( strPSProfile ) )
	{
		return false;
	}

	LPD3DXBUFFER pShaderData = NULL;
	LPD3DXBUFFER pErrMsg = NULL;

	HRESULT hRes = D3DXCompileShaderFromFileA( 
		filename.c_str() , NULL , NULL , "main" , strPSProfile.c_str() , 0 , &pShaderData , &pErrMsg , &m_pD3DPSConstantTab  );

	if( TRUE == FAILED(hRes) )
	{
		String strErrMsg = (char*)pErrMsg->GetBufferPointer();
		return false;
	}

	 hRes = m_pD3D9Device->CreatePixelShader(
		(DWORD*)pShaderData->GetBufferPointer() , ppPS );

	if( TRUE == FAILED(hRes) )
	{
		ZP_SAFE_RELEASE( pErrMsg );
		ZP_SAFE_RELEASE( pShaderData );
		return false;
	}

	ZP_SAFE_RELEASE( pErrMsg );
	ZP_SAFE_RELEASE( pShaderData );
	return true;
}

void D3DRenderImpl::_ApplyShaders( void )
{ 
	m_pD3DVSConstantTab->SetMatrix( m_pD3D9Device , "m4World" , (D3DXMATRIX*)m_m4WorldMat.m  );
	m_pD3DVSConstantTab->SetMatrix( m_pD3D9Device , "m4View" , (D3DXMATRIX*)m_m4ViewMat.m  );
	m_pD3DVSConstantTab->SetMatrix( m_pD3D9Device , "m4Proj" , (D3DXMATRIX*)m_m4ProjMat.m );
	m_pD3D9Device->SetVertexShader( m_pVS );
	m_pD3DPSConstantTab->SetInt( m_pD3D9Device , "diffuseTex" , 0 );
	m_pD3D9Device->SetPixelShader( m_pPS );
}

void D3DRenderImpl::_UnapplyShaders( void )
{
	m_pD3D9Device->SetVertexShader( NULL );
	m_pD3D9Device->SetPixelShader( NULL );
}

 
void D3DRenderImpl::_InitEffect( void )
{
	DWORD dwFlags = 
#ifdef ZP_DEBUG
		D3DXSHADER_FORCE_VS_SOFTWARE_NOOPT|
		D3DXSHADER_FORCE_PS_SOFTWARE_NOOPT|
		D3DXSHADER_NO_PRESHADER
#else
		0
#endif
		; 

	//创建全局变量共享的Pool
	HRESULT hRes = 
	D3DXCreateEffectPool( &m_pEffectPool );
	ZP_ASSERT( SUCCEEDED( hRes ) );

	ID3DXBuffer* pErrorBuf = NULL; 
	D3DIncludeCallback d3dIncludeCallback;

	DWORD dwShaderFlags = 0;

#ifdef ZP_DEBUG_D3D
	dwShaderFlags = D3DXSHADER_DEBUG|D3DXSHADER_SKIPOPTIMIZATION;
#endif

	hRes = 
		D3DXCreateEffectFromFileA( 
		m_pD3D9Device , 
		"globalconst.fx" , 
		NULL , 
		&d3dIncludeCallback , 
		dwShaderFlags ,
		m_pEffectPool ,
		&m_pGlobalConstEffect ,
		&pErrorBuf
		);

	ZP_ASSERT( SUCCEEDED( hRes ) );
}

void D3DRenderImpl::_DestroyEffect( void )
{ 
	ZP_SAFE_RELEASE( m_pGlobalConstEffect );
	ZP_SAFE_RELEASE( m_pEffectPool ); 
}

void D3DRenderImpl::_InitFont( void )
{
	D3DXFONT_DESCA fontDesc;
	memset( &fontDesc , 0 , sizeof( fontDesc ) );
	strcpy(fontDesc.FaceName, "Arial");
	fontDesc.Height = 20;
	fontDesc.CharSet = GB2312_CHARSET;

	D3DXCreateFontIndirectA( m_pD3D9Device , &fontDesc , &m_pFont );
	ZP_ASSERT( NULL != m_pFont );

}

void D3DRenderImpl::_DestroyFont( void )
{
	ZP_SAFE_RELEASE( m_pFont );
}

void D3DRenderImpl::_DrawText( const int x , const int y , const String& str )
{
	RECT  rect;
	rect.left = x;
	rect.top = y;
	rect.right = x + 800;
	rect.bottom = y + 600;
	m_pFont->DrawTextA( NULL , str.c_str() , -1 , &rect , DT_TOP | DT_LEFT , D3DCOLOR_RGBA( 0, 255 , 0 , 255 ) );
}

void D3DRenderImpl::_InitRT( void )
{
	//创建颜色缓冲区
	HRESULT hRes = 
		m_pD3D9Device->CreateTexture( 
		m_d3dParams.BackBufferWidth , m_d3dParams.BackBufferHeight , 1 , 
		D3DUSAGE_RENDERTARGET , 
		D3DFMT_A8R8G8B8 , 
		D3DPOOL_DEFAULT , 
		&m_pColorRT , NULL );
	ZP_ASSERT( TRUE == SUCCEEDED( hRes ) );
	//获取颜色缓冲区的Surface
	hRes = m_pColorRT->GetSurfaceLevel( 0 , &m_pColorRTSurface );

	hRes = m_pD3D9Device->CreateDepthStencilSurface( 
		m_d3dParams.BackBufferWidth , m_d3dParams.BackBufferHeight ,
		D3DFMT_D24X8 , D3DMULTISAMPLE_NONE , 0 , FALSE , 
		&m_pDepthStencilSurface , NULL
		);
}

void D3DRenderImpl::_DestroyRT( void )
{
	//销毁渲染目标
	ZP_SAFE_RELEASE( m_pDepthStencilSurface );
	ZP_SAFE_RELEASE( m_pColorRTSurface );
	ZP_SAFE_RELEASE( m_pColorRT );
}

void D3DRenderImpl::_ResizeRT( const int w , const int h )
{
	//重建渲染目标
	HRESULT hRes = 
		m_pD3D9Device->CreateTexture( 
		w , h , 1 , 
		D3DUSAGE_RENDERTARGET , 
		D3DFMT_A8R8G8B8 , 
		D3DPOOL_DEFAULT , 
		&m_pColorRT , NULL );
	ZP_ASSERT( TRUE == SUCCEEDED( hRes ) );
	//获取颜色缓冲区的Surface
	m_pColorRT->GetSurfaceLevel( 0 , &m_pColorRTSurface );

	hRes = m_pD3D9Device->CreateDepthStencilSurface( 
		w , h ,
		D3DFMT_D24X8 , D3DMULTISAMPLE_NONE , 0 , FALSE , 
		&m_pDepthStencilSurface , NULL
		);
}

void D3DRenderImpl::_ApplyRT( void )
{
	m_pD3D9Device->GetRenderTarget( 0 , &m_pSavedColorRT );
	m_pD3D9Device->GetDepthStencilSurface( &m_pSavedDepthStencilSurface );
	m_pD3D9Device->SetRenderTarget( 0 , m_pColorRTSurface );
	m_pD3D9Device->SetDepthStencilSurface( m_pDepthStencilSurface );
	 
	m_pD3D9Device->Clear( 
		0 , NULL , 
		D3DCLEAR_TARGET|D3DCLEAR_ZBUFFER  , 
		D3DCOLOR_XRGB( 0 , 0 , 0 ) , 1.0f , 0 );  
}

void D3DRenderImpl::_UnapplyRT( void )
{ 
	m_pD3D9Device->SetDepthStencilSurface( m_pSavedDepthStencilSurface );
	m_pD3D9Device->SetRenderTarget( 0 , m_pSavedColorRT );
	ZP_SAFE_RELEASE( m_pSavedColorRT ); 
	ZP_SAFE_RELEASE( m_pSavedDepthStencilSurface );
}

bool D3DRenderImpl::IsActive( void ) 
{
	HRESULT hRes = m_pD3D9Device->TestCooperativeLevel();

	if( SUCCEEDED( hRes ) )
	{
		return true;
	}

	if( hRes == D3DERR_DEVICENOTRESET )
	{
		_OnDeviceLost();  
		m_d3dParams.BackBufferWidth = m_viewPort.Width;
		m_d3dParams.BackBufferHeight = m_viewPort.Height;
		m_d3dParams.BackBufferFormat = D3DFMT_UNKNOWN;
		HRESULT hRes = m_pD3D9Device->Reset( &m_d3dParams ); 
		if( FAILED( hRes ) )
		{
			return false;
		}
		_OnDeviceReset();
		return true;
	}
	return false;

}


 

}//namespace Render