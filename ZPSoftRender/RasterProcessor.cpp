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

		Math::BGRA8888_t uiPixel = Math::MathUtil::ColorVecToRGBA8888( color );

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

	void RasterProcessor::DrawScanLine( const RVertex& v0 , const RVertex& v1 , PixelShader& shader )
	{
		int y = Math::MathUtil::Ceil( v0.m_v3Pos.y );
		if( y < 0 || y >= static_cast<int>( BufferHeight() ) )
		{
			return;
		}

		RVertex rvVert0 = v0 , rvVert1 = v1;
		 
		if( rvVert0.m_v3Pos.x > rvVert1.m_v3Pos.x )
		{
			Math::SwapT( rvVert0 , rvVert1 ); 
		}

		int iXStart = Math::MathUtil::Ceil( rvVert0.m_v3Pos.x );
		int iXEnd = Math::MathUtil::Ceil( rvVert1.m_v3Pos.x ); 


		Real dt = 1.0f / ( rvVert1.m_v3Pos.x - rvVert0.m_v3Pos.x  );
		Real t = 0.0f;

		for( int x = iXStart ; x <= iXEnd ; x++ )
		{
			RVertex rvIVert = rvVert0*( 1.0f - t ) + rvVert1*t; 
			Math::Vec4 v4FinalColor = shader.Run( rvIVert );
			Math::BGRA8888_t uiPixel = Math::MathUtil::ColorVecToRGBA8888( v4FinalColor );
			WritePixel( x , y , uiPixel );
			t += dt;
		}

	}

	void RasterProcessor::DrawTriangle2D( const RVertex& v0 , const RVertex& v1 , const RVertex& v2 , PixelShader& shader )
	{
		RVertex rvVert0 = v0 , rvVert1 = v1 , rvVert2 = v2;


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

		int iYStart =  Math::MathUtil::Ceil( rvVert0.m_v3Pos.y );
		int iYEnd   =  Math::MathUtil::Ceil( rvVert1.m_v3Pos.y );

		for( int y = iYStart ; y <= iYEnd ; y++ )
		{ 
			DrawScanLine( rvVertStart , rvVertEnd , shader );
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

		Real fInvDetLeft = 1.0f / ( rvVert2.m_v3Pos.y - rvVert0.m_v3Pos.y );
		Real fInvDetRight = 1.0f / ( rvVert2.m_v3Pos.y - rvVert1.m_v3Pos.y );

		RVertex rvDVertLeft = ( rvVert2 - rvVert0 ) * fInvDetLeft;
		RVertex rvDVertRight = ( rvVert2 - rvVert1 ) * fInvDetRight; 

		RVertex rvVertStart = rvVert0;
		RVertex rvVertEnd = rvVert1; 

		int iYStart =  Math::MathUtil::Ceil( rvVert0.m_v3Pos.y );
		int iYEnd   =  Math::MathUtil::Ceil( rvVert2.m_v3Pos.y );

		for( int y = iYStart ; y <= iYEnd ; y++ )
		{
			DrawScanLine( rvVertStart , rvVertEnd , shader );
			rvVertStart += rvDVertLeft;
			rvVertEnd += rvDVertRight; 
		}
	}

	 


}//namespace Render