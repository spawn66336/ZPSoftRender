#include "RenderContext.h"
#include "Light.h" 
#include "IRender.h"

namespace Render
{

RenderContext::RenderContext(void):
m_hWnd(NULL),
m_hDC(NULL),
m_hBackDC( NULL ),
m_hOldBitmap( NULL ),
m_hBackBuffer(NULL),
m_aspect(1.0f),
m_enableTexture2D(false),	
m_enableDepthTest(false),		
m_enableLighting(false),
m_pCurrMaterial( NULL ),
m_colorFrameBuf( 0 , 0 , 4 ),
m_zFrameBuf( 0 , 0 , 4 ),
m_iCurrRenderVertexCount(0),
m_iCurrRenderFaceCount(0),
m_iCurrFrameMS(0),
m_iFrameCount(0),
m_iTotalTicks(0),
m_uiTickTag(0),
m_fFPS(0.0f)
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
	m_hDC = ::GetDC( m_hWnd ); 
	m_hBackDC = ::CreateCompatibleDC( m_hDC );
	this->Resize();
}

void RenderContext::Destroy( void )
{
	if( NULL != m_hBackBuffer )
	{
		::DeleteObject( m_hBackBuffer );
		m_hBackBuffer = NULL;
	}

	if( NULL != m_hBackDC )
	{
		::DeleteDC( m_hBackDC );
		m_hBackDC = NULL;
	}

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

	if( NULL != m_hBackBuffer )
	{
		::DeleteObject( m_hBackBuffer );
		m_hBackBuffer = NULL;
	}

	m_hBackBuffer = 
		::CreateCompatibleBitmap( m_hDC , 
		static_cast<int>( m_colorFrameBuf.Width() ) ,  
		static_cast<int>( m_colorFrameBuf.Height() ) );


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
	::BitBlt( m_hDC , 0 , 0 , m_colorFrameBuf.Width() , m_colorFrameBuf.Height() , 
			   m_hBackDC , 0 , 0, SRCCOPY );

	SelectObject( m_hBackDC , m_hOldBitmap ); 

	m_iCurrFrameMS = static_cast<int>( ::GetTickCount() - m_uiTickTag );
	m_iTotalTicks += m_iCurrFrameMS;
	IncFrameCounter();

	if( m_iFrameCount >= 10 )
	{
		m_fFPS = 1000.0f / ( static_cast<Real>( m_iTotalTicks ) / static_cast<Real>( m_iFrameCount ) );

		if( m_fFPS > 500.0f )
		{
			m_fFPS = 500.0f;
		}

		m_iTotalTicks = 0;
		m_iFrameCount = 0;
	}
	
}


void RenderContext::CopyColorBufferToBackBuffer( void )
{ 
	int iResult = 0;   
	 
	::SetStretchBltMode( m_hBackDC , COLORONCOLOR );
	BITMAPINFO bitmapInfo;
	m_colorFrameBuf.FillBitmapInfo( bitmapInfo ); 

	iResult = ::SetDIBitsToDevice( 
			m_hBackDC , 0 , 0 , m_colorFrameBuf.Width() , m_colorFrameBuf.Height() ,
			0 , 0 , 0 , m_colorFrameBuf.Height() , 
			m_colorFrameBuf.Pixels() , &bitmapInfo , DIB_RGB_COLORS 
		); 
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
	Real d = 1.0f * Math::MathUtil::Tan( Math::MathUtil::DegreesToRadians( fov * 0.5f ) );

	m_frustum.Set( fov , m_aspect , n , f );

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

void RenderContext::DrawText( const int x , const int y , const String& str )
{ 
	::SetBkColor( m_hBackDC , RGB(0,0,0) );
	::SetTextColor( m_hBackDC , RGB(150,150,150 ) );
	::TextOutA( m_hBackDC , x , y , str.c_str() , str.length() );
}

void RenderContext::PrepareBackBuffer( void )
{
	m_hOldBitmap =  static_cast<HBITMAP>( ::SelectObject( m_hBackDC , m_hBackBuffer ) );
	m_uiTickTag = ::GetTickCount();
}

void RenderContext::ClearAllCounters( void )
{ 
	m_iCurrRenderVertexCount = 0;
	m_iCurrRenderFaceCount = 0;
}

 

}//namespace Render