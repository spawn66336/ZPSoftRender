#include "D3DRenderImpl.h"
#include "Camera.h"
#include "Light.h" 
#include <D3dx9math.h>


namespace Render
{


D3DRenderImpl::D3DRenderImpl(void): 
m_pD3D9(NULL),
m_pD3D9Device(NULL),
m_pD3DVB(NULL),
m_pD3DIB(NULL),
m_pD3DHelperVB(NULL),
m_pD3DHelperIB(NULL),
m_uiFVF(0),
m_hWnd(NULL),
m_fAspect(1.0f),
m_uiClearFlag(0)
{
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

	//d3d初始化为窗口模式
	m_d3dParams.Windowed = TRUE;
	//由于是非全屏模式，将后缓冲区宽高设为0则
	//后缓冲区会根据窗口大小自动调整
	m_d3dParams.BackBufferWidth = 800;
	m_d3dParams.BackBufferHeight = 600;
	m_d3dParams.BackBufferFormat = D3DFMT_UNKNOWN;  
	m_d3dParams.SwapEffect = D3DSWAPEFFECT_DISCARD;
	//启用深度与模板缓冲区
	m_d3dParams.EnableAutoDepthStencil = TRUE;
	m_d3dParams.AutoDepthStencilFormat = D3DFMT_D16; 


	//创建D3D设备
	hRes = m_pD3D9->CreateDevice( 
		D3DADAPTER_DEFAULT , 
		D3DDEVTYPE_HAL , 
		m_hWnd , 
		D3DCREATE_SOFTWARE_VERTEXPROCESSING ,
		&m_d3dParams ,
		&m_pD3D9Device
		);

	ZP_ASSERT( TRUE == SUCCEEDED( hRes ) );  
	ZP_ASSERT( NULL != m_pD3D9Device );

	//初始化视口
	m_pD3D9Device->GetViewport( &m_viewPort);
	
	//创建顶点缓冲区
	_InitVB();
}

void D3DRenderImpl::Destroy()
{ 
	//销毁顶点缓冲区
	_DestroyVB();

	//释放D3D9设备
	if( m_pD3D9Device )
	{
		m_pD3D9Device->Release();
		m_pD3D9Device = NULL;
	}

	if( m_pD3D9 )
	{
		m_pD3D9->Release();
		m_pD3D9 = NULL;
	}
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
}

void D3DRenderImpl::BeginDraw( Camera* pCam )
{    
	m_pD3D9Device->SetRenderState( D3DRS_CULLMODE, D3DCULL_NONE );
	m_pD3D9Device->SetRenderState( D3DRS_LIGHTING, FALSE ); 
	m_pD3D9Device->SetRenderState( D3DRS_ZENABLE, TRUE ); 

	m_pD3D9Device->Clear( 
		0 , NULL , 
		D3DCLEAR_TARGET|D3DCLEAR_ZBUFFER , 
		D3DCOLOR_XRGB( 0 , 0 , 0 ) , 1.0f , 0 );  

	m_pD3D9Device->BeginScene();

	_SetupMatrices( pCam );

	_DrawHelper();
}

void D3DRenderImpl::EndDraw( void )
{  
	m_pD3D9Device->EndScene();
}

void D3DRenderImpl::SwapBuffers( void )
{  
	m_pD3D9Device->Present( NULL , NULL , NULL , NULL );
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

}

void D3DRenderImpl::DrawElements( RenderPrimitive& renderPrimitive )
{
	unsigned int uiVertCount = renderPrimitive.VertexBuf().Count();
	unsigned int uiIndexCount = renderPrimitive.IndicesCount();
	unsigned int uiFaceCount = uiIndexCount / 3;

	_PrepareRender( renderPrimitive );
	 
	m_pD3D9Device->SetTransform( D3DTS_WORLD ,  (D3DMATRIX*)( m_m4WorldMat.m ) );
	m_pD3D9Device->SetStreamSource( 0 , m_pD3DVB , 0 , sizeof(d3dRenderVert_t)  );
	m_pD3D9Device->SetFVF( m_uiFVF );
	m_pD3D9Device->SetIndices( m_pD3DIB ); 
	m_pD3D9Device->DrawIndexedPrimitive( D3DPT_TRIANGLELIST , 0 , 0 , uiVertCount  , 0 , uiFaceCount );
	
}

void D3DRenderImpl::EnableTexture2D( bool enable )
{

}

void D3DRenderImpl::EnableLight( bool enable )
{

}

void D3DRenderImpl::EnableDepthTest( bool enable )
{
	
}

void D3DRenderImpl::SetShadingModel( SHADE_MODEL type )
{

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
	const int MAX_VERTEX_NUM = 1024*256;
	const int MAX_INDICES_NUM =  1024*256;
	m_uiFVF = 
		D3DFVF_XYZ|
		D3DFVF_NORMAL|
		D3DFVF_DIFFUSE|
		D3DFVF_TEX1|D3DFVF_TEXCOORDSIZE2(0);

	//创建足够大的顶点缓冲区
	HRESULT hRes = m_pD3D9Device->CreateVertexBuffer( 
		MAX_VERTEX_NUM*sizeof(d3dRenderVert_t) , 0 , m_uiFVF , D3DPOOL_DEFAULT , &m_pD3DVB , NULL );
	ZP_ASSERT( TRUE == SUCCEEDED( hRes ) );
	
	//创建足够大的索引缓冲区
	hRes = m_pD3D9Device->CreateIndexBuffer( 
		MAX_INDICES_NUM*sizeof(unsigned short) , 0 , D3DFMT_INDEX16 , D3DPOOL_DEFAULT , &m_pD3DIB , NULL );
	ZP_ASSERT( TRUE == SUCCEEDED( hRes ) );

	_InitHelperVB();

}

void D3DRenderImpl::_DestroyVB( void )
{
	_DestroyHelperVB();
	 
	if( NULL != m_pD3DVB )
	{
		m_pD3DVB->Release();
		m_pD3DVB = NULL;
	}

	if( NULL != m_pD3DIB )
	{
		m_pD3DIB->Release();
		m_pD3DIB = NULL;
	}
}

void D3DRenderImpl::_InitHelperVB( void )
{
	const int MAX_VERTEX_NUM = 6; 
	const unsigned int uiVBLockSize = MAX_VERTEX_NUM*sizeof(d3dRenderVert2_t);
	 

	//创建足够大的顶点缓冲区
	HRESULT hRes = m_pD3D9Device->CreateVertexBuffer( 
		uiVBLockSize , 0 , 	D3DFVF_XYZ|D3DFVF_DIFFUSE , D3DPOOL_DEFAULT , &m_pD3DHelperVB , NULL );
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
	if( NULL != m_pD3DHelperVB )
	{
		m_pD3DHelperVB->Release();
		m_pD3DHelperVB = NULL;
	}

	if( NULL != m_pD3DHelperIB )
	{
		m_pD3DHelperIB->Release();
		m_pD3DHelperIB = NULL;
	}
}


void D3DRenderImpl::_PrepareRender( RenderPrimitive& renderPrimitive )
{
	unsigned int uiVertCount = renderPrimitive.VertexBuf().Count();
	unsigned int uiIndexCount = renderPrimitive.IndicesCount(); 
	//拷贝顶点
	unsigned int uiVBLockSize = uiVertCount*sizeof(d3dRenderVert_t); 
	void* pBuf = NULL;
	HRESULT hRes = m_pD3DVB->Lock( 0 , uiVBLockSize , &pBuf  , 0 );
	ZP_ASSERT( TRUE == SUCCEEDED( hRes ) );
	ZP_ASSERT( NULL != pBuf );
	 
	for( int i = 0; i < uiVertCount ; i++ )
	{
		((d3dRenderVert_t*)pBuf)[i].pos = renderPrimitive.VertexBuf().Pointer()[i].m_pos;
		((d3dRenderVert_t*)pBuf)[i].norm = renderPrimitive.VertexBuf().Pointer()[i].m_norm;
		((d3dRenderVert_t*)pBuf)[i].diffuse = 0xffffffff;
		((d3dRenderVert_t*)pBuf)[i].tex = renderPrimitive.VertexBuf().Pointer()[i].m_texcoord;
	}

	m_pD3DVB->Unlock();
	pBuf = NULL;

	//拷贝索引
	unsigned int uiIBLockSize = sizeof(unsigned short)*renderPrimitive.IndicesCount();
	hRes = m_pD3DIB->Lock( 0 , uiIBLockSize , &pBuf , 0  );
	ZP_ASSERT( TRUE == SUCCEEDED( hRes ) );
	ZP_ASSERT( NULL != pBuf );

	for( int iIndx = 0 ; iIndx < renderPrimitive.IndicesCount() ; iIndx++ )
	{
		( (unsigned short*)pBuf )[iIndx] = (unsigned short)renderPrimitive.Indices()[iIndx];
	}

	m_pD3DIB->Unlock();
	pBuf = NULL;
}

void D3DRenderImpl::_DrawHelper( void )
{
	D3DXMATRIX worldMat;
	D3DXMatrixIdentity( &worldMat );
	m_pD3D9Device->SetTransform( D3DTS_WORLD , &worldMat );
	m_pD3D9Device->SetStreamSource( 0 , m_pD3DHelperVB , 0 , sizeof(d3dRenderVert2_t) );
	m_pD3D9Device->SetFVF( D3DFVF_XYZ|D3DFVF_DIFFUSE ); 
	m_pD3D9Device->DrawPrimitive( D3DPT_LINELIST , 0 , 3 ); 
}

void D3DRenderImpl::_SetupMatrices( Camera* pCam )
{  
	Math::Matrix4 m4View = pCam->GetCameraMatrix(); 
	m_pD3D9Device->SetTransform(  D3DTS_VIEW , (D3DMATRIX*)( m4View.m ) );

	m_m4ProjMat = 
		Math::Matrix4::MakeD3DProjectionMatrix( 60.0f , m_fAspect , 2.0f , 1000.0f );  
	m_pD3D9Device->SetTransform(  D3DTS_PROJECTION ,  (D3DMATRIX*)( &m_m4ProjMat.m ) );   
}



}//namespace Render