#include "RenderContext.h"
#include "Light.h" 
#include "IRender.h"

namespace Render
{

RenderContext::RenderContext(void):
m_hWnd(NULL),
m_hDC(NULL),
m_aspect(1.0f),
m_enableTexture2D(false),	
m_enableDepthTest(false),		
m_enableLighting(false),
m_pCurrMaterial( NULL ),
m_colorFrameBuf( 0 , 0 , 4 ),
m_zFrameBuf( 0 , 0 , 4 )
{
}


RenderContext::~RenderContext(void)
{
	DeleteAllLights();
}

void RenderContext::Init( const winHandle_t hwnd )
{
	ZP_ASSERT( NULL != hwnd );
	m_hWnd = hwnd;
	this->Resize();
	m_hDC = ::GetDC( m_hWnd );
}

void RenderContext::Destroy( void )
{
	::ReleaseDC( m_hWnd , m_hDC );
	m_hDC = NULL;
	m_hWnd = NULL;
}

void RenderContext::Resize( void )
{
	::GetClientRect( m_hWnd , &m_wndRect );
	int iWndWidth = m_wndRect.right - m_wndRect.left;
	int iWndHeight = m_wndRect.bottom - m_wndRect.top;
	 
	if( iWndWidth > MAX_SCREEN_WIDTH )
		iWndWidth = MAX_SCREEN_WIDTH;
	if( iWndHeight > MAX_SCREEN_HEIGHT )
		iWndHeight = MAX_SCREEN_HEIGHT;

	m_aspect = ( (Real)iWndWidth ) / ( (Real)iWndHeight );

	m_colorFrameBuf.Resize( iWndWidth , iWndHeight );
	m_zFrameBuf.Resize( iWndWidth , iWndHeight );

	//更新透视坐标到屏幕坐标变换矩阵
	Real alpha = 0.5f * (Real)iWndWidth - 0.5f;
	Real beta   = 0.5f * (Real)iWndHeight - 0.5f;

	m_currProjToScreenMat = Math::Matrix4( 
			alpha , 0.0f    , 0.0f  , 0.0f ,
			0.0f   , -beta  , 0.0f  , 0.0f ,
			0.0f   , 0.0f    , 1.0f  , 0.0f ,
			alpha , beta   , 0.0f  , 1.0f 
		);
}

void RenderContext::EnableTexture2D( bool enable )
{
	m_enableTexture2D = enable;
}

void RenderContext::EnableLight( bool enable )
{
	m_enableLighting = enable;
}

void RenderContext::EnableDepthTest( bool enable )
{
	m_enableDepthTest = enable;
}

Light* RenderContext::CreateLight( const String& name )
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

bool RenderContext::DeleteLightByName( const String& name )
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

Light* RenderContext::FindLightByName( const String& name )
{
	lightTable_t::iterator itFindLight = m_lights.find( name );

	//未找到此名称的光源
	if( itFindLight == m_lights.end() )
	{
		return NULL;
	}
	return itFindLight->second;
}

void RenderContext::DeleteAllLights( void )
{
	lightTable_t::iterator it = m_lights.begin();
	while( it != m_lights.end() )
	{
		ZP_SAFE_DELETE( it->second );
		it++;
	}
	m_lights.clear();
}

void RenderContext::SetProjectionMatrix( const Math::Matrix4& mat )
{
	m_currCamToProjMat = mat;
}

void RenderContext::PushMatrix()
{
	m_modelViewMatrixStack.push( m_currLocalToCamMat );
}

void RenderContext::PopMatrix()
{
	if( m_modelViewMatrixStack.size() )
	{
		m_currLocalToCamMat = m_modelViewMatrixStack.top();
		m_modelViewMatrixStack.pop();
	}
}

void RenderContext::LoadMatrix( const Math::Matrix4 &mat )
{
	m_currLocalToCamMat = mat;
}

void RenderContext::LoadIdentity()
{
	m_currLocalToCamMat = Math::Matrix4::IDENTITY;
}

void RenderContext::MultMatrix( const Math::Matrix4 &mat )
{
	m_currLocalToCamMat = mat * m_currLocalToCamMat;
}

void RenderContext::SetClearColor( const Math::Vec4& color )
{
	m_clearColor = color;
}

void RenderContext::ClearBuffer( unsigned int flag )
{
	if( flag & COLOR_BUFFER_FLAG )
	{
		Math::BGRA8888_t rgba = Math::MathUtil::ColorVecToBGRA8888( m_clearColor );
		m_colorFrameBuf.Clear( &rgba );
	}

	if( flag & DEPTH_BUFFER_FLAG )
	{
		unsigned int depth = 0;
		m_zFrameBuf.Clear( &depth );
	}
}

void RenderContext::SwapBuffers( void )
{ 
	int iResult = 0;  
	::SetStretchBltMode( m_hDC , COLORONCOLOR );
	BITMAPINFO bitmapInfo;
	m_colorFrameBuf.FillBitmapInfo( bitmapInfo ); 

	iResult = ::SetDIBitsToDevice( 
								m_hDC , 0 , 0 , m_colorFrameBuf.Width() , m_colorFrameBuf.Height() ,
								0 , 0 , 0 , m_colorFrameBuf.Height() , 
								m_colorFrameBuf.Pixels() , &bitmapInfo , DIB_RGB_COLORS 
								); 

	DWORD dwLastErr = ::GetLastError();
	 
}

void RenderContext::SetCurrMaterial(  Resource::Material* pMat )
{
	m_pCurrMaterial = pMat;
}

Resource::Material* RenderContext::GetCurrMaterial( void ) const
{
	return m_pCurrMaterial;
}

void RenderContext::SetPerspectiveProjection( const Real fov , const Real n , const Real f )
{
	Real d = 1.0f * Math::MathUtil::Tan( Math::MathUtil::DegreesToRadians( fov/2.0f ) );


	m_currCamToProjMat = Math::Matrix4(
			d ,		 0.0f , 0.0f , 0.0f ,
			0.0f , d*m_aspect , 0.0f , 0.0f ,
			0.0f , 0.0f , -n/( f - n ) , 1.0f ,
			0.0f , 0.0f , n*f/( f - n ) , 0.0f 
		);
}

void RenderContext::SetWorldToCameraMatrix( const Math::Matrix4& mat )
{
	m_currWorldToCamMat = mat;
}

}//namespace Render