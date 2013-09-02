#include "RasterProcessor.h"
#include "FrameBuffer.h"
#include "RenderList.h"
#include "PixelShader.h"

namespace Render
{ 

	RasterProcessor::RasterProcessor(void)
	{

	}
	 
	RasterProcessor::~RasterProcessor(void)
	{

	}

	unsigned int RasterProcessor::Calc2DPointOutCode( const int x , const int y )
	{
		unsigned int uiOutCode = 0;

		if( x < 0 ) uiOutCode |= OCODE_2D_LEFT;
		if( x >= (int)BufferWidth() ) uiOutCode |= OCODE_2D_RIGHT;
		if( y < 0 ) uiOutCode |= OCODE_2D_TOP;
		if( y >= (int)BufferHeight() ) uiOutCode |= OCODE_2D_BOTTOM;

		return uiOutCode;
	}
	 

	void RasterProcessor::DrawLineMidPoint( const Math::Vec2& p0 , const Math::Vec2& p1 , const Math::Vec4& color )
	{
		int dx,dy,d;
		int x0 = (int)p0.x;
		int y0 = (int)p0.y;
		int x1 = (int)p1.x;
		int y1 = (int)p1.y;
		int x,y;

		if( x0 > x1)               /* 保证x0<x1,即x0为x1左边的点。*/
		{
			std::swap(x0,x1); 
			std::swap(y0,y1);
		}

		if( 
			Calc2DPointOutCode( x0 , y0 ) != 0 && 
			Calc2DPointOutCode( x1 , y1 ) != 0 
			)
		{
			return;
		}

		Math::BGRA8888_t uiPixel = Math::MathUtil::ColorVecToBGRA8888( color );

		dx = x1 - x0;
		dy = y1 - y0; 

		if( dx == 0)                /* 斜率为无穷大，画直竖线 */
		{
			DrawVLine( x0 , y0 , y1 , uiPixel );
			return;
		}

		if( dy == 0)                /*斜率为零，画水平线 */
		{
			DrawHLine( y0 , x0 , x1 , uiPixel );
			return;
		}

		x = x0;
		y = y0; 
		WritePixel( x , y , uiPixel );

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

				WritePixel( x , y , uiPixel );
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

				WritePixel( x , y , uiPixel );
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

				WritePixel( x , y , uiPixel );
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

				WritePixel( x , y , uiPixel );
			}
			return;
		}
	}

	void RasterProcessor::DrawVLine( const int x , const int y0 , const int y1 , Math::BGRA8888_t color )
	{
		if( x < 0 || x >= (int)BufferWidth() )
			return;
		int iYStart = y0;
		int iYEnd = y1;
		if( iYStart > iYEnd )
		{
			Math::SwapT( iYStart , iYEnd );
		}

		if( iYStart < 0 )
		{
			iYStart = 0;
		}else if( iYStart >= (int)BufferHeight() )
		{
			return;
		}

		for( int y = iYStart ; y <= iYEnd ; y++ )
		{
			WritePixel( x , y , color );
		}
	}

	void RasterProcessor::DrawHLine( const int y , const int x0 , const int x1 , Math::BGRA8888_t color )
	{
		if( y < 0 || y >= (int)BufferHeight() )
			return;

		int iXStart = x0;
		int iXEnd = x1;

		if( iXStart > iXEnd )
		{
			Math::SwapT( iXStart , iXEnd );
		}

		if( iXStart < 0 )
		{
			iXStart = 0;
		}else if( iXStart >= (int)BufferWidth() )
		{
			return;
		} 

		for( int x = iXStart ; x <= iXEnd ; x++ )
		{
			WritePixel( x , y , color );
		}
	}


	void RasterProcessor::DrawScanLine( const int y , const int xs , const int xe , 
		const RVertex& v0 , const RVertex& v1 , PixelShader& shader )
	{

		if( xe < xs )
			return;
		////必须保证传入的 起始位置要小于等于终止位置
		//ZP_ASSERT( xs <= xe );

		if( y < 0 || y >= static_cast<int>( BufferHeight() ) )
		{
			return;
		}

		Real dt = 1.0f / static_cast<Real>( xe - xs  );
		//Real dt = 1.0f / ( v1.m_v3Pos.x - v0.m_v3Pos.x );
		Real t = 0.0f;

		for( int x = xs ; x <= xe ; x++ )
		{ 
			RVertex rvIVert = v0 * ( 1.0f - t ) + v1 * t;
			if( ZTest( x , y , rvIVert.m_invZ  ) )
			{
				Math::BGRA8888_t uiPixel = Math::MathUtil::ColorVecToBGRA8888( shader.Run( rvIVert ) ); 
				WritePixel( x , y , uiPixel );
			} 
			t += dt;
		}
	}

	void RasterProcessor::DrawTriangle2D( const RVertex& v0 , const RVertex& v1 , const RVertex& v2 , PixelShader& shader )
	{
		RVertex rvVert0 = v0 , rvVert1 = v1 , rvVert2 = v2;
		
		//若三角面完全在屏幕之外
		if( IsOutOfScreen( 
			Math::Vec2( v0.m_v3Pos.x , v0.m_v3Pos.y ) , 
			Math::Vec2( v1.m_v3Pos.x , v1.m_v3Pos.y ) , 
			Math::Vec2( v2.m_v3Pos.x , v2.m_v3Pos.y ) ) )
		{
			return;
		}

		{ //对顶点按y由小到大排序

			if( rvVert0.m_v3Pos.y > rvVert1.m_v3Pos.y )
			{
				Math::SwapT( rvVert0 , rvVert1 ); 
			}

			if( rvVert0.m_v3Pos.y > rvVert2.m_v3Pos.y )
			{
				Math::SwapT( rvVert0 , rvVert2 ); 
			}

			if( rvVert1.m_v3Pos.y > rvVert2.m_v3Pos.y  )
			{
				Math::SwapT( rvVert1 , rvVert2 ); 
			} 
		}//对顶点按y由小到大排序

		//若为平底三角形
		if( REAL_EQUAL( rvVert1.m_v3Pos.y , rvVert2.m_v3Pos.y ) )
		{
			//保持顶点1 在 顶点2 的左边
			if( rvVert1.m_v3Pos.x > rvVert2.m_v3Pos.x )
			{
				Math::SwapT( rvVert1 , rvVert2 );
			}
			DrawTriangle2DFlatBottom( rvVert0 , rvVert1 , rvVert2 , shader );
			return;
		}

		//若为平顶三角形
		if( REAL_EQUAL( rvVert0.m_v3Pos.y , rvVert1.m_v3Pos.y ) )
		{
			//保持顶点0 在 顶点1 的左边
			if( rvVert0.m_v3Pos.x > rvVert1.m_v3Pos.x )
			{
				Math::SwapT( rvVert0 , rvVert1 );
			}
			DrawTriangle2DFlatTop( rvVert0 , rvVert1 , rvVert2 , shader );
			return;
		}

		Real fFactor = (rvVert1.m_v3Pos.y - rvVert0.m_v3Pos.y) / ( rvVert2.m_v3Pos.y - rvVert0.m_v3Pos.y );
		RVertex rvMidVert;

		rvMidVert = rvVert0 + ( rvVert2 - rvVert0 )*fFactor;
		  
		DrawTriangle2DFlatBottom( rvVert0 , rvVert1 , rvMidVert , shader );
		DrawTriangle2DFlatTop( rvVert1 , rvMidVert , rvVert2 , shader );
	}

	void RasterProcessor::DrawTriangle2DFlatBottom( 
		const RVertex& v0 , const RVertex& v1 , const RVertex& v2 , PixelShader& shader )
	{ 
		RVertex rvVert0 = v0 , rvVert1 = v1 , rvVert2 = v2; 

		if( rvVert1.m_v3Pos.x > rvVert2.m_v3Pos.x )
		{
			Math::SwapT( rvVert1 , rvVert2 ); 
		}
		 
		Real fInvDet = 1.0f / ( rvVert1.m_v3Pos.y - rvVert0.m_v3Pos.y );

		RVertex rvDVertLeft = ( rvVert1 - rvVert0 ) * fInvDet;
		RVertex rvDVertRight = ( rvVert2 - rvVert0 ) * fInvDet;
		 
		RVertex rvVertStart = rvVert0;
		RVertex rvVertEnd = rvVert0; 

		Real fYStart =  Math::MathUtil::Ceil( rvVert0.m_v3Pos.y );
		Real fYEnd   =  Math::MathUtil::Ceil( rvVert1.m_v3Pos.y ) - 1;

		Real fErrFactor = ( fYStart - rvVert0.m_v3Pos.y );
		
		RVertex rvLeftErr = rvDVertLeft * fErrFactor;
		RVertex rvRightErr = rvDVertRight * fErrFactor;
		Real fLeftErr = rvLeftErr.m_v3Pos.x;
		Real fRightErr = rvRightErr.m_v3Pos.x;
		 
		for( int y = static_cast<int>(fYStart) ; y <= static_cast<int>(fYEnd) ; y++ )
		{
			Real fXStart = Math::MathUtil::Ceil( rvVertStart.m_v3Pos.x + fLeftErr);
			Real fXEnd	= Math::MathUtil::Ceil( rvVertEnd.m_v3Pos.x  + fRightErr ) - 1;
			 
			DrawScanLine(  y , static_cast<int>( fXStart ) , static_cast<int>( fXEnd ) , 
				rvVertStart + rvLeftErr , rvVertEnd + rvRightErr , shader );

			rvVertStart += rvDVertLeft;
			rvVertEnd += rvDVertRight; 
		}
	}

	void RasterProcessor::DrawTriangle2DFlatTop( 
		const RVertex& v0 , const RVertex& v1 , const RVertex& v2 , PixelShader& shader )
	{
		RVertex rvVert0 = v0 , rvVert1 = v1 , rvVert2 = v2; 

		if(   rvVert0.m_v3Pos.x > rvVert1.m_v3Pos.x )
		{
			Math::SwapT( rvVert0 , rvVert1 ); 
		}

		Real fInvDet = 1.0f / ( rvVert2.m_v3Pos.y - rvVert0.m_v3Pos.y ); 

		RVertex rvDVertLeft = ( rvVert2 - rvVert0 ) * fInvDet;
		RVertex rvDVertRight = ( rvVert2 - rvVert1 ) * fInvDet; 

		RVertex rvVertStart = rvVert0;
		RVertex rvVertEnd = rvVert1; 

		Real fYStart =  Math::MathUtil::Ceil( rvVert0.m_v3Pos.y );
		Real fYEnd   =  Math::MathUtil::Ceil( rvVert2.m_v3Pos.y ) - 1;

		Real fErrFactor = ( fYStart - rvVert0.m_v3Pos.y ); 
		RVertex rvLeftErr = rvDVertLeft * fErrFactor;
		RVertex rvRightErr = rvDVertRight * fErrFactor;
		Real fLeftErr = rvLeftErr.m_v3Pos.x;
		Real fRightErr = rvRightErr.m_v3Pos.x;

		for( int y = static_cast<int>( fYStart ) ; y <= static_cast<int>( fYEnd ) ; y++ )
		{
			Real fXStart = Math::MathUtil::Ceil( rvVertStart.m_v3Pos.x + fLeftErr );
			Real fXEnd	= Math::MathUtil::Ceil( rvVertEnd.m_v3Pos.x  + fRightErr ) - 1;

			DrawScanLine(  y , static_cast<int>( fXStart ) , static_cast<int>( fXEnd ) , 
				rvVertStart + rvLeftErr , rvVertEnd + rvRightErr, shader );

			rvVertStart += rvDVertLeft;
			rvVertEnd += rvDVertRight; 
		}
	}

	bool RasterProcessor::IsOutOfScreen( const Math::Vec2& p0 , const Math::Vec2& p1 , const Math::Vec2& p2 )
	{
		if( p0.x < 0.0f && p1.x < 0.0f && p2.x < 0.0f )
		{
			return true;
		}

		if( p0.x > static_cast<Real>( BufferWidth()-1 ) &&
			p1.x > static_cast<Real>( BufferWidth()-1 ) &&
			p2.x > static_cast<Real>( BufferWidth()-1 ) )
		{
			return true;
		}

		if( p0.y < 0.0f && p1.y < 0.0f && p2.y < 0.0f )
		{
			return true;
		}

		if( p0.y > static_cast<Real>( BufferHeight()-1 ) &&
			p1.y > static_cast<Real>( BufferHeight()-1 ) &&
			p2.y > static_cast<Real>( BufferHeight()-1 ) )
		{
			return true;
		}
		return false;
	}

	 


}//namespace Render