#include "RenderContext.h"
#include "Light.h" 
#include "IRender.h"

namespace Render
{

RenderContext::RenderContext(void):
m_hWnd(NULL),
m_hDC(NULL),
m_enableTexture2D(false),	
m_enableDepthTest(false),		
m_enableLighting(false),
m_pCurrMaterial( NULL ),
m_colorFrameBuf( 0 , 0 , 4 ),
m_zFrameBuf( 0 , 0 , 1 )
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

	m_colorFrameBuf.Resize( iWndWidth , iWndHeight );
	m_zFrameBuf.Resize( iWndWidth , iWndHeight );
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
	m_projectionMat = mat;
}

void RenderContext::PushMatrix()
{
	m_modelViewMatrixStack.push( m_currModelViewMat );
}

void RenderContext::PopMatrix()
{
	if( m_modelViewMatrixStack.size() )
	{
		m_currModelViewMat = m_modelViewMatrixStack.top();
		m_modelViewMatrixStack.pop();
	}
}

void RenderContext::LoadMatrix( const Math::Matrix4 &mat )
{
	m_currModelViewMat = mat;
}

void RenderContext::LoadIdentity()
{
	m_currModelViewMat = Math::Matrix4::IDENTITY;
}

void RenderContext::MultMatrix( const Math::Matrix4 &mat )
{
	m_currModelViewMat = mat * m_currModelViewMat;
}

void RenderContext::SetClearColor( const Math::Vec4& color )
{
	m_clearColor = color;
}

void RenderContext::ClearBuffer( unsigned int flag )
{
	if( flag & COLOR_BUFFER_FLAG )
	{
		Math::BGRA8888_t rgba = Math::MathUtil::ColorVecToRGBA8888( m_clearColor );
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

}//namespace Render