#include "FrameBuffer.h"

namespace Render
{


FrameBuffer::FrameBuffer(void):
m_uiWidth(0),
m_uiHeight(0),
m_uiBytesPerPixel(0),
m_uiCapacity(0),
m_pPixels(NULL)
{
}

FrameBuffer::FrameBuffer( const unsigned int uiWidth , const unsigned int uiHeight , const unsigned int uiBytesPerPixel ):
m_uiWidth(uiWidth),
m_uiHeight(uiHeight),
m_uiBytesPerPixel(uiBytesPerPixel),
m_uiCapacity(0),
m_pPixels(NULL)
{
	Assign();
}


FrameBuffer::~FrameBuffer(void)
{
	m_uiWidth = 0;
	m_uiHeight = 0;
	m_uiBytesPerPixel = 0;
	m_uiCapacity = 0;
	ZP_SAFE_DELETE_BUFFER( m_pPixels );
}

void FrameBuffer::Resize( const int uiWidth , const int uiHeight )
{
	m_uiWidth = uiWidth;
	m_uiHeight = uiHeight;
	Assign();
}

void FrameBuffer::Assign( void )
{
	unsigned int newSize = Width() * Height() * BytesPerPixel();
	if( newSize <= this->Capacity() )
	{
		return;
	} 
	
	ZP_SAFE_DELETE_BUFFER( m_pPixels );
	m_pPixels = new unsigned char[ newSize ];
	ZP_ASSERT( NULL != m_pPixels );
	m_uiCapacity = newSize; 
}

void FrameBuffer::FillBitmapInfo( BITMAPINFO& bitmapInfo )
{
	memset( &bitmapInfo , 0 , sizeof(bitmapInfo) );
	bitmapInfo.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	bitmapInfo.bmiHeader.biWidth = Width();
	bitmapInfo.bmiHeader.biHeight = -((int)Height());
	bitmapInfo.bmiHeader.biPlanes = 1;
	bitmapInfo.bmiHeader.biBitCount = BytesPerPixel()*8;
	bitmapInfo.bmiHeader.biCompression = BI_RGB;
	bitmapInfo.bmiHeader.biSizeImage = 0;
	bitmapInfo.bmiHeader.biClrUsed = 0;
	bitmapInfo.bmiHeader.biClrImportant = 0;
}



}//namespace Render