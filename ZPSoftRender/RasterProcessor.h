#ifndef ZP_RASTERPROCESSOR
#define ZP_RASTERPROCESSOR

#include "ZPDependency.h"
#include "ZPMathDependency.h"
#include "FrameBuffer.h"

namespace Render
{
	class PixelShader;
	class FrameBuffer;
	class RVertex;

	enum POINT2D_OUTCODE
	{
		OCODE_2D_LEFT = 1,
		OCODE_2D_RIGHT = 2,
		OCODE_2D_TOP = 4,
		OCODE_2D_BOTTOM = 8
	};

	class RasterProcessor
	{
	public:
		RasterProcessor(void);
		~RasterProcessor(void);

		inline void SetColorBuffer( FrameBuffer* pBuf ){ m_pColorBuffer = pBuf; }

		inline void SetDepthBuffer( FrameBuffer* pBuf ){ m_pDepthBuffer = pBuf; }

		inline unsigned int BufferWidth( void )  const
		{
			ZP_ASSERT( NULL != m_pColorBuffer );
			return m_pColorBuffer->Width();
		}

		inline unsigned int BufferHeight( void ) const	{
			ZP_ASSERT( NULL != m_pColorBuffer );
			return m_pColorBuffer->Height();
		}

		inline bool ZTest( const int x , const int y , unsigned char depth ){ return true; }

		inline void WritePixel( const int x , const int y ,  Math::BGRA8888_t color ,  unsigned char depth = 0 )
		{
			m_pColorBuffer->WritePixel( x , y , (&color) );
			m_pDepthBuffer->WritePixel( x , y , &depth );
		}

		unsigned int Calc2DPointOutCode( const int x , const int y ); 

		void DrawLineMidPoint( const Math::Vec2& p0 , const Math::Vec2& p1 , const Math::Vec4& color  );

		void DrawVLine( const int x , const int y0 , const int y1 ,  Math::BGRA8888_t color  );

		void DrawHLine( const int y , const int x0 , const int x1 ,  Math::BGRA8888_t  color  ); 

		void DrawScanLine( const RVertex& v0 , const RVertex& v1 , PixelShader& shader );

		void DrawTriangle2D( const RVertex& v0 , const RVertex& v1 , const RVertex& v2 , PixelShader& shader );

		void DrawTriangle2DFlatBottom( const RVertex& v0 , const RVertex& v1 , const RVertex& v2 , PixelShader& shader );

		void DrawTriangle2DFlatTop( const RVertex& v0 , const RVertex& v1 , const RVertex& v2 , PixelShader& shader );


	protected:
		FrameBuffer* m_pColorBuffer;
		FrameBuffer* m_pDepthBuffer;
	};

}//namespace Render

#endif //ZP_RASTERPROCESSOR
