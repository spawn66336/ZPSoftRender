#include "FrameBuffer.h"
#include "RenderList.h"

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

//
//void FrameBuffer::DrawTriangle2DSolid( 
//	const Math::Vec2& p0 , 
//	const Math::Vec2& p1 , 
//	const Math::Vec2& p2 , void* pPixel )
//{
//	this->DrawTriangle2DSolid( 
//				static_cast<int>(p0.x) , static_cast<int>(p0.y),
//				static_cast<int>(p1.x) , static_cast<int>(p1.y),
//				static_cast<int>(p2.x) , static_cast<int>(p2.y),
//				pPixel
//			);
//}
//
//void FrameBuffer::DrawTriangle2DSolid( 
//	const int x0 , const int y0 , 
//	const int x1 , const int y1 , 
//	const int x2 , const int y2 , 
//	void* pPixel )
//{
//
//	int iPx0 = x0 , iPy0 = y0 , 
//		 iPx1 = x1 , iPy1 = y1 , 
//		 iPx2 = x2 , iPy2 = y2;
//
//	//若待渲染三角面 在屏幕外
//	if( 0 != Calc2DPointOutCode( x0 , y0 ) &&
//		0 != Calc2DPointOutCode( x1 , y1 ) &&
//		0 != Calc2DPointOutCode( x2 , y2 ) )
//	{
//		return;
//	}
//
//	{ //对顶点按y由小到大排序
//
//		if( iPy0 > iPy1 )
//		{
//			std::swap( iPx0 , iPx1 );
//			std::swap( iPy0 , iPy1 );
//		}
//
//		if( iPy0 > iPy2 )
//		{
//			std::swap( iPx0 , iPx2 );
//			std::swap( iPy0 , iPy2 );
//		}
//
//		if( iPy1 > iPy2 )
//		{
//			std::swap( iPx1 , iPx2 );
//			std::swap( iPy1 , iPy2 );
//		} 
//	}
//
//	//平底三角形
//	if( iPy1 == iPy2 )
//	{
//		//保持顶点1 在 顶点2 的左边
//		if( iPx1 > iPx2 )
//		{
//			std::swap( iPx1 , iPx2 );
//			std::swap( iPy1 , iPy2 );
//		} 
//
//		DrawTriangle2DFlatBaseSolid( iPx0 , iPy0 , iPx1 , iPy1 , iPx2 , iPy2 , pPixel );
//		return;
//	}
//
//	//平顶三角形
//	if( iPy0 == iPy1 )
//	{
//		//保持顶点0 在 顶点1 的左边
//		if( iPx0 > iPx1 )
//		{
//			std::swap( iPx0 , iPx1 );
//			std::swap( iPy0 , iPy1 );
//		}
//		DrawTriangle2DFlatTopSolid( iPx0 , iPy0 , iPx1 , iPy1 , iPx2 , iPy2 , pPixel );
//		return;
//	}
//	 
//
//	Real fDxNum = static_cast<Real>( ( iPx2 - iPx0 )*( iPy1 - iPy0 ) );
//	Real fDxDet = static_cast<Real>( iPy2 - iPy0 ); 
//
//	int iPMidx = static_cast<int>( static_cast<Real>(iPx0) + fDxNum/fDxDet + 0.5f ); 
//	int iPMidy = iPy1;
//	 
//	DrawTriangle2DFlatBaseSolid( iPx0 , iPy0 , iPx1 , iPy1 , iPMidx , iPMidy , pPixel  );
//	DrawTriangle2DFlatTopSolid( iPx1 , iPy1 , iPMidx , iPMidy , iPx2 , iPy2 , pPixel  ); 
//}
//
//
//void FrameBuffer::DrawVLine( const int x , const int y0 , const int y1 , void* pPixel )
//{
//	if( x < 0 || x >= (int)Width() )
//		return;
//	int iYStart = y0;
//	int iYEnd = y1;
//	if( iYStart > iYEnd )
//	{
//		std::swap( iYStart , iYEnd );
//	}
//
//	if( iYStart < 0 )
//	{
//		iYStart = 0;
//	}else if( iYStart >= (int)Height() )
//	{
//		return;
//	}
//
//	for( int y = iYStart ; y <= iYEnd ; y++ )
//	{
//		WritePixel( x , y , pPixel );
//	}
//}
//
//void FrameBuffer::DrawHLine( const int y , const int x0 , const int x1 , void* pPixel )
//{
//	if( y < 0 || y >= (int)Height() )
//		return;
//	
//	int iXStart = x0;
//	int iXEnd = x1;
//	if( iXStart > iXEnd )
//	{
//		std::swap( iXStart , iXEnd );
//	}
//
//	if( iXStart < 0 )
//	{
//		iXStart = 0;
//	}else if( iXStart >= (int)Width() )
//	{
//		return;
//	}
//
//	for( int x = iXStart ; x <= iXEnd ; x++ )
//	{
//		WritePixel( x , y , pPixel );
//	}
//}
//
//void FrameBuffer::DrawHLine( 
//	const int y , const int x0 , const int x1 , const Math::Vec4& colorStart , const Math::Vec4& colorEnd )
//{
//	if( y < 0 || y >= (int)Height() )
//		return;
//
//	int iXStart = x0;
//	int iXEnd = x1;
//	Math::Vec4 v4ColorStart = colorStart , v4ColorEnd = colorEnd;
//
//	if( iXStart > iXEnd )
//	{
//		std::swap( iXStart , iXEnd );
//		Math::SwapT( v4ColorStart , v4ColorEnd );
//	}
//
//	if( iXStart < 0 )
//	{
//		iXStart = 0;
//	}else if( iXStart >= (int)Width() )
//	{
//		return;
//	}
//
//	Real fDT = 1.0f / static_cast<Real>( iXEnd - iXStart );
//	Real fT = 0.0f;
//
//	for( int x = iXStart ; x <= iXEnd ; x++ )
//	{
//		Math::Vec4 v4CurrColor = v4ColorStart*( 1.0f - fT ) + v4ColorEnd*fT;
//		Math::BGRA8888_t uiPixel = Math::MathUtil::ColorVecToRGBA8888( v4CurrColor );
//		WritePixel( x , y , &uiPixel );
//		fT += fDT;
//	}
//}
//
//void FrameBuffer::DrawLineMidPoint( const Math::Vec2& p0 , const Math::Vec2& p1 , void* pPixel )
//{
//	int dx,dy,d;
//	int x0 = (int)p0.x;
//	int y0 = (int)p0.y;
//	int x1 = (int)p1.x;
//	int y1 = (int)p1.y;
//	int x,y;
//
//	if( x0 > x1)               /* 保证x0<x1,即x0为x1左边的点。*/
//	{
//		std::swap(x0,x1); 
//		std::swap(y0,y1);
//	}
//
//	if( 
//		Calc2DPointOutCode( x0 , y0 ) != 0 && 
//		Calc2DPointOutCode( x1 , y1 ) != 0 
//		)
//		return;
//	  
//	dx = x1 - x0;
//	dy = y1 - y0;
//	 
//
//	if( dx == 0)                /* 斜率为无穷大，画直竖线 */
//	{
//		DrawVLine(x0,y0,y1,pPixel);
//		return;
//	}
//
//	if( dy == 0)                /*斜率为零，画水平线 */
//	{
//		DrawHLine(y0,x0,x1,pPixel);
//		return;
//	}
//
//	x = x0;
//	y = y0;
//	WritePixel(x,y,pPixel);
//
//	if((dx>=dy)&&(dy>0))           /* when 0<k<=1 */
//	{
//		d = (dy*2) - dx;
//		while(x<x1)
//		{
//			if(d > 0)
//			{
//				d += (dy - dx)*2;       /* 选择NE点 */
//				x++;
//				y++;
//			}
//			else
//			{
//				d += dy*2;        /* 选择N点 */
//				x++;
//			}
//
//			WritePixel(x,y,pPixel);
//		}
//		return;
//	}
//
//	if( (dy>dx)&&(dy>0))           /* when k>1 */
//	{
//		d = dy - (dx*2);
//		while(y<y1)
//		{
//			if(d < 0)
//			{
//				d += (dy - dx)*2; /* 选择 NE 点 */
//				x++;
//				y++;
//			}
//			else
//			{
//				d += (-dx)*2;     /* 选择N点 */
//				y++;
//			}
//
//			WritePixel(x,y,pPixel);
//		}
//		return;
//	}
//
//	if((dx>= Math::MathUtil::Abs(dy))&&(dy<0))           /* when -1=<k<0 */
//	{
//		d = (dy*2) + dx;           /* d = 2a-b */
//		while(x<x1)
//		{
//			if(d < 0)
//			{
//				d += (dy+dx) * 2;   /* 选择SE点 */
//				x++;
//				y--;
//			}
//			else
//			{
//				d += dy*2;          /* 选择S点 */
//				x++;
//			}
//
//			WritePixel(x,y,pPixel);
//		}
//		return;
//	}
//
//	if( (Math::MathUtil::Abs(dy)>dx)&&(dy<0))           /* when k<-1 */
//	{
//		d = dy + (dx*2);          /* d = a - 2b */
//		while(y>y1)
//		{
//			if(d > 0)
//			{
//				d += (dy + dx)*2; /* 选择 SE 点 */
//				x++;
//				y--;
//			}
//			else
//			{
//				d += (dx)*2;     /* 选择S点 */
//				y--;
//			}
//
//			WritePixel(x,y,pPixel);
//		}
//		return;
//	}
//}
//
//void FrameBuffer::DrawTriangle2DFlatBaseSolid( 
//	const int x0 , const int y0 , 
//	const int x1 , const int y1 , 
//	const int x2 , const int y2 , 
//	void* pPixel )
//{
//	ZP_ASSERT( y1 == y2 );
//	ZP_ASSERT( y0 <= y1 ); 
//
//	int iPx0 = x0 , iPy0 = y0 , iPx1 = x1 , iPy1 = y1 , iPx2 = x2 , iPy2 = y2;
//
//	if( iPx1 > iPx2 )
//	{
//		std::swap( iPx1 , iPx2 );
//		std::swap( iPy1 , iPy2 );
//	}
//	 
//	float fDxLeft = static_cast<float>(iPx1-iPx0)/static_cast<float>(iPy1-iPy0);
//	float fDxRight = static_cast<float>(iPx2-iPx0)/static_cast<float>(iPy2-iPy0);
//	float fXStart =static_cast<float>(iPx0);
//	float fXEnd = static_cast<float>(iPx0);
//
//	for( int y = iPy0 ; y <= iPy1 ; y++ )
//	{
//		DrawHLine( y , static_cast<int>(fXStart) , static_cast<int>(fXEnd) , pPixel );
//		fXStart += fDxLeft;
//		fXEnd   += fDxRight;
//	}
//}
//
//void FrameBuffer::DrawTriangle2DFlatTopSolid( 
//	const int x0 , const int y0 , 
//	const int x1 , const int y1 , 
//	const int x2 , const int y2 , 
//	void* pPixel )
//{
//	ZP_ASSERT( y0 == y1 );
//	ZP_ASSERT( y2 >= y0 );
//	/*ZP_ASSERT( x0 <= x1 );*/
//
//	int iPx0 = x0 , iPy0 = y0 , iPx1 = x1 , iPy1 = y1 , iPx2 = x2 , iPy2 = y2;
//
//	if( iPx0 > iPx1 )
//	{
//		std::swap( iPx0 , iPx1 );
//		std::swap( iPy0 , iPy1 );
//	}
//
//	float fDxLeft = static_cast<float>(iPx2-iPx0)/static_cast<float>(iPy2-iPy0);
//	float fDxRight = static_cast<float>(iPx2-iPx1)/static_cast<float>(iPy2-iPy1);
//	float fXStart =static_cast<float>(iPx0);
//	float fXEnd = static_cast<float>(iPx1);
//
//	for( int y = iPy0 ; y <= iPy2 ; y++ )
//	{
//		DrawHLine( y , static_cast<int>(fXStart) , static_cast<int>(fXEnd) , pPixel );
//		fXStart += fDxLeft;
//		fXEnd   += fDxRight;
//	}
//}
//
//unsigned int FrameBuffer::Calc2DPointOutCode( const int x , const int y )
//{
//	unsigned int uiOutCode = 0;
//
//	if( x < 0 ) uiOutCode |= OCODE_2D_LEFT;
//	if( x >= (int)Width() ) uiOutCode |= OCODE_2D_RIGHT;
//	if( y < 0 ) uiOutCode |= OCODE_2D_TOP;
//	if( y >= (int)Height() ) uiOutCode |= OCODE_2D_BOTTOM;
//
//	return uiOutCode;
//}
// 
//
//void FrameBuffer::DrawTriangle2DFlatBase( 
//	const int x0 , const int y0 , 
//	const int x1 , const int y1 , 
//	const int x2 , const int y2 , 
//	const Math::Vec4& color0 , const Math::Vec4& color1 , const Math::Vec4& color2 )
//{
//	ZP_ASSERT( y1 == y2 );
//	ZP_ASSERT( y0 <= y1 ); 
//
//	int iPx0 = x0 , iPy0 = y0 , iPx1 = x1 , iPy1 = y1 , iPx2 = x2 , iPy2 = y2;
//	Math::Vec4 v4Color0 = color0 , v4Color1 = color1 , v4Color2 = color2;
//
//	if( iPx1 > iPx2 )
//	{
//		std::swap( iPx1 , iPx2 );
//		std::swap( iPy1 , iPy2 );
//		Math::SwapT( v4Color1 , v4Color2 );
//	}
//
//	float fDxLeft = static_cast<float>(iPx1-iPx0)/static_cast<float>(iPy1-iPy0);
//	float fDxRight = static_cast<float>(iPx2-iPx0)/static_cast<float>(iPy2-iPy0);
//	Math::Vec4 v4DColorLeft = ( v4Color1 - v4Color0 ) / static_cast<float>(iPy1-iPy0);
//	Math::Vec4 v4DColorRight = ( v4Color2 - v4Color0 ) / static_cast<float>(iPy2-iPy0);
//	float fXStart =static_cast<float>(iPx0);
//	float fXEnd = static_cast<float>(iPx0);
//	Math::Vec4 v4ColorStart = v4Color0;
//	Math::Vec4 v4ColorEnd = v4Color0;
//
//	for( int y = iPy0 ; y <= iPy1 ; y++ )
//	{
//		DrawHLine( y , static_cast<int>(fXStart) , static_cast<int>(fXEnd) , v4ColorStart , v4ColorEnd );
//
//		v4ColorStart += v4DColorLeft;
//		v4ColorEnd += v4DColorRight;
//		fXStart += fDxLeft;
//		fXEnd   += fDxRight;
//	}
//}
//
//void FrameBuffer::DrawTriangle2DFlatTop( 
//	const int x0 , const int y0 , 
//	const int x1 , const int y1 , 
//	const int x2 , const int y2 , 
//	const Math::Vec4& color0 , const Math::Vec4& color1 , const Math::Vec4& color2 )
//{
//	ZP_ASSERT( y0 == y1 );
//	ZP_ASSERT( y2 >= y0 );
//	/*ZP_ASSERT( x0 <= x1 );*/
//
//	int iPx0 = x0 , iPy0 = y0 , iPx1 = x1 , iPy1 = y1 , iPx2 = x2 , iPy2 = y2;
//	Math::Vec4 v4Color0 = color0 , v4Color1 = color1 , v4Color2 = color2;
//
//	if( iPx0 > iPx1 )
//	{
//		std::swap( iPx0 , iPx1 );
//		std::swap( iPy0 , iPy1 );
//		Math::SwapT( v4Color0 , v4Color1 );
//	}
//
//	float fDxLeft = static_cast<float>(iPx2-iPx0)/static_cast<float>(iPy2-iPy0);
//	float fDxRight = static_cast<float>(iPx2-iPx1)/static_cast<float>(iPy2-iPy1);
//	Math::Vec4 v4DColorLeft = ( v4Color2 - v4Color0 ) / static_cast<float>(iPy2-iPy0);
//	Math::Vec4 v4DColorRight = ( v4Color2 - v4Color1 ) / static_cast<float>(iPy2-iPy1);
//	float fXStart =static_cast<float>(iPx0);
//	float fXEnd = static_cast<float>(iPx1);
//	Math::Vec4 v4ColorStart = v4Color0;
//	Math::Vec4 v4ColorEnd = v4Color1;
//
//	for( int y = iPy0 ; y <= iPy2 ; y++ )
//	{
//		DrawHLine( y , static_cast<int>(fXStart) , static_cast<int>(fXEnd) , v4ColorStart , v4ColorEnd );
//		v4ColorStart += v4DColorLeft;
//		v4ColorEnd += v4DColorRight;
//		fXStart += fDxLeft;
//		fXEnd   += fDxRight;
//	}
//}
// 
//void FrameBuffer::DrawTriangle2D( const RVertex& v0 , const RVertex& v1 , const RVertex& v2 )
//{
//
//	int iPx0 = static_cast<int>( v0.m_v3Pos.x ) , iPy0 = static_cast<int>( v0.m_v3Pos.y ) , 
//		 iPx1 = static_cast<int>( v1.m_v3Pos.x ) , iPy1  = static_cast<int>( v1.m_v3Pos.y ) , 
//		 iPx2 = static_cast<int>( v2.m_v3Pos.x ) , iPy2  = static_cast<int>( v2.m_v3Pos.y );
//
//	Math::Vec4 v4Color0 = v0.m_v4Color , v4Color1 = v1.m_v4Color , v4Color2 = v2.m_v4Color;
//
//	//若待渲染三角面 在屏幕外
//	if( 0 != Calc2DPointOutCode( iPx0 , iPy0 ) &&
//		0 != Calc2DPointOutCode( iPx1 , iPy1 ) &&
//		0 != Calc2DPointOutCode( iPx2 , iPy2 ) )
//	{
//		return;
//	}
//
//	{ //对顶点按y由小到大排序
//
//		if( iPy0 > iPy1 )
//		{
//			std::swap( iPx0 , iPx1 );
//			std::swap( iPy0 , iPy1 );
//			Math::SwapT( v4Color0 , v4Color1 );
//		}
//
//		if( iPy0 > iPy2 )
//		{
//			std::swap( iPx0 , iPx2 );
//			std::swap( iPy0 , iPy2 );
//			Math::SwapT( v4Color0 , v4Color2 );
//		}
//
//		if( iPy1 > iPy2 )
//		{
//			std::swap( iPx1 , iPx2 );
//			std::swap( iPy1 , iPy2 );
//			Math::SwapT( v4Color1 , v4Color2 );
//		} 
//	}
//
//	//平底三角形
//	if( iPy1 == iPy2 )
//	{
//		//保持顶点1 在 顶点2 的左边
//		if( iPx1 > iPx2 )
//		{
//			std::swap( iPx1 , iPx2 );
//			std::swap( iPy1 , iPy2 );
//			Math::SwapT( v4Color1 , v4Color2 );
//		} 
//
//		DrawTriangle2DFlatBase( iPx0 , iPy0 , iPx1 , iPy1 , iPx2 , iPy2 , v4Color0 , v4Color1 , v4Color2 );
//		return;
//	}
//
//	//平顶三角形
//	if( iPy0 == iPy1 )
//	{
//		//保持顶点0 在 顶点1 的左边
//		if( iPx0 > iPx1 )
//		{
//			std::swap( iPx0 , iPx1 );
//			std::swap( iPy0 , iPy1 );
//			Math::SwapT( v4Color0 , v4Color1 );
//		}
//		DrawTriangle2DFlatTop( iPx0 , iPy0 , iPx1 , iPy1 , iPx2 , iPy2 , v4Color0 , v4Color1 , v4Color2 );
//		return;
//	}
//
//
//	Real fDxNum = static_cast<Real>( iPx2 - iPx0 );
//	Real fDxDet = static_cast<Real>( iPy2 - iPy0 ); 
//
//	int iPMidx = static_cast<int>( static_cast<Real>(iPx0) + ( static_cast<Real>(iPy1 - iPy0)*fDxNum ) /fDxDet + 0.5f ); 
//	int iPMidy = iPy1;
//
//	Math::Vec4 v4MidColor =  v4Color0 + static_cast<Real>(iPy1 - iPy0)*( ( v4Color2 - v4Color0 ) /fDxDet );
//
//	DrawTriangle2DFlatBase( iPx0 , iPy0 , iPx1 , iPy1 , iPMidx , iPMidy , v4Color0 , v4Color1 , v4MidColor );
//	DrawTriangle2DFlatTop( iPx1 , iPy1 , iPMidx , iPMidy , iPx2 , iPy2 , v4Color1 , v4MidColor , v4Color2  ); 
//}



 



}//namespace Render