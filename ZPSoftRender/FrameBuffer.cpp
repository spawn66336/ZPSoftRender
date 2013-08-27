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

void FrameBuffer::DrawLineDDA( const Math::Vec2& p0 , const Math::Vec2& p1 , void* pPixel )
{
	Math::Vec2 v2PStart;
	Math::Vec2 v2PEnd;

	if( p0.x < p1.x )
	{
		v2PStart = p0;
		v2PEnd = p1;
	}else{
		v2PStart = p1;
		v2PEnd = p0;
	}

	 int iStartX = static_cast<int>(v2PStart.x);
	 int iEndX = static_cast<int>(v2PEnd.x);

	if( iStartX < 0 )
		iStartX = 0;
	else if( iStartX > (int)Width()-1 )
		iStartX = Width()-1;

	if( iEndX < 0 )
		iEndX = 0;
	else if( iEndX > (int)Width()-1 )
		iEndX = Width()-1;

	Math::Vec2 v2StartToEnd = v2PEnd - v2PStart;
	Real k = v2StartToEnd.y / v2StartToEnd.x;
	Real y = v2PStart.y;

	for( int x = iStartX ; x <= iEndX ; x++ )
	{
		WritePixel( x , static_cast<int>(y) , pPixel );
		y+=k;
	}

}


void FrameBuffer::DrawTriangle2DSolid( 
	const Math::Vec2& p0 , 
	const Math::Vec2& p1 , 
	const Math::Vec2& p2 , void* pPixel )
{

}


void FrameBuffer::DrawVLine( const int x , const int y0 , const int y1 , void* pPixel )
{
	if( x < 0 || x >= (int)Width() )
		return;
	int iYStart = y0;
	int iYEnd = y1;
	if( iYStart > iYEnd )
	{
		std::swap( iYStart , iYEnd );
	}
	for( int y = iYStart ; y <= iYEnd ; y++ )
	{
		WritePixel( x , y , pPixel );
	}
}

void FrameBuffer::DrawHLine( const int y , const int x0 , const int x1 , void* pPixel )
{
	if( y < 0 || y >= (int)Height() )
		return;
	
	int iXStart = x0;
	int iXEnd = x1;
	if( iXStart > iXEnd )
	{
		std::swap( iXStart , iXEnd );
	}
	for( int x = iXStart ; x <= iXEnd ; x++ )
	{
		WritePixel( x , y , pPixel );
	}
}

void FrameBuffer::DrawLineMidPoint( const Math::Vec2& p0 , const Math::Vec2& p1 , void* pPixel )
{
	int dx,dy,d;
	int x0 = (int)p0.x;
	int y0 = (int)p0.y;
	int x1 = (int)p1.x;
	int y1 = (int)p1.y;
	unsigned int x,y;

	if( x0 > x1)               /* 保证x0<x1,即x0为x1左边的点。*/
	{
		std::swap(x0,x1); 
		std::swap(y0,y1);
	}

	dx = x1 - x0;
	dy = y1 - y0;

	if( dx == 0)                /* 斜率为无穷大，画直竖线 */
	{
		DrawVLine(x0,y0,y1,pPixel);
		return;
	}

	if( dy == 0)                /*斜率为零，画水平线 */
	{
		DrawHLine(y0,x0,x1,pPixel);
		return;
	}

	x = x0;
	y = y0;
	WritePixel(x,y,pPixel);

	if((dx>=dy)&&(dy>0))           /* when 0<k<=1 */
	{
		d = (dy*2) - dx;
		while(x<x1)
		{
			if(d > 0)
			{
				d += (dy - dx)*2;       /* 选择NE点 */
				x++;
				y++;
			}
			else
			{
				d += dy*2;        /* 选择N点 */
				x++;
			}

			WritePixel(x,y,pPixel);
		}
		return;
	}

	if( (dy>dx)&&(dy>0))           /* when k>1 */
	{
		d = dy - (dx*2);
		while(y<y1)
		{
			if(d < 0)
			{
				d += (dy - dx)*2; /* 选择 NE 点 */
				x++;
				y++;
			}
			else
			{
				d += (-dx)*2;     /* 选择N点 */
				y++;
			}

			WritePixel(x,y,pPixel);
		}
		return;
	}

	if((dx>= Math::MathUtil::Abs(dy))&&(dy<0))           /* when -1=<k<0 */
	{
		d = (dy*2) + dx;           /* d = 2a-b */
		while(x<x1)
		{
			if(d < 0)
			{
				d += (dy+dx) * 2;   /* 选择SE点 */
				x++;
				y--;
			}
			else
			{
				d += dy*2;          /* 选择S点 */
				x++;
			}

			WritePixel(x,y,pPixel);
		}
		return;
	}

	if( (Math::MathUtil::Abs(dy)>dx)&&(dy<0))           /* when k<-1 */
	{
		d = dy + (dx*2);          /* d = a - 2b */
		while(y>y1)
		{
			if(d > 0)
			{
				d += (dy + dx)*2; /* 选择 SE 点 */
				x++;
				y--;
			}
			else
			{
				d += (dx)*2;     /* 选择S点 */
				y--;
			}

			WritePixel(x,y,pPixel);
		}
		return;
	}
}

void FrameBuffer::DrawTriangle2DFlatBaseSolid( const int x0 , const int y0 , const int x1 , const int y1 , const int x2 , const int y2 , void* pPixel )
{

}

void FrameBuffer::DrawTriangle2DFlatTopSolid( const int x0 , const int y0 , const int x1 , const int y1 , const int x2 , const int y2 , void* pPixel )
{

}



}//namespace Render