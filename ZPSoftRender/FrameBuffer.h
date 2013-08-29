#ifndef ZP_FRAMEBUFFER
#define ZP_FRAMEBUFFER

#include "ZPDependency.h"
#include "ZPMathDependency.h" 

namespace Render
{

	/**

		ÆÁÄ»×ø±êÏµ 		
		(0,0)	-------------------> X
		|
		|
		|
		|
		V
		Y
	*/

	//enum POINT2D_OUTCODE
	//{
	//	OCODE_2D_LEFT = 1,
	//	OCODE_2D_RIGHT = 2,
	//	OCODE_2D_TOP = 4,
	//	OCODE_2D_BOTTOM = 8
	//};

	class RVertex;

	class FrameBuffer
	{
	protected:
		FrameBuffer();
	public:
		FrameBuffer( const unsigned int uiWidth , const unsigned int uiHeight , const unsigned int uiBytesPerPixel );
		virtual ~FrameBuffer(void); 

		inline unsigned int Width( void ) const { return m_uiWidth; }
		inline unsigned int Height( void ) const { return m_uiHeight; }
		inline unsigned int BytesPerPixel( void ) const { return m_uiBytesPerPixel; }
		inline unsigned int Capacity( void ) const{ return m_uiCapacity; }

		void Resize( const int uiWidth , const int uiHeight );
		void Assign( void );

		inline void WritePixel( const int x , const int y , void* pPixel )
		{
			ZP_ASSERT( NULL != pPixel );
			if( x < 0 || x >= (int)Width() ) return;
			if( y < 0 || y >= (int)Height() ) return;

			const unsigned int uiLineOffset = y*BytesPerPixel()*Width();
			const unsigned int uiPixelOffset = x*BytesPerPixel();
			memcpy( &m_pPixels[uiLineOffset + uiPixelOffset] , pPixel , BytesPerPixel() ); 
		}

		inline void ReadPixel( const int x , const int y , void* pPixel )
		{
			ZP_ASSERT( NULL != pPixel );
			if( x < 0 || x >= (int)Width() ) return;
			if( y < 0 || y >= (int)Height() ) return;

			const unsigned int uiLineOffset = y*BytesPerPixel()*Width();
			const unsigned int uiPixelOffset = x*BytesPerPixel();
			memcpy( pPixel , &m_pPixels[uiLineOffset + uiPixelOffset] ,  BytesPerPixel() ); 
		}

		inline void Clear( void* pPixel )
		{
			ZP_ASSERT( NULL != pPixel );

			unsigned int uiTotalPixelCount = Width()*Height();
			for( unsigned int uiPixel = 0 ; uiPixel < uiTotalPixelCount ; uiPixel++ )
			{
				memcpy( &m_pPixels[uiPixel*BytesPerPixel()] , pPixel , BytesPerPixel() );
			}
		}

		inline void* Pixels( void ) const { return m_pPixels; }

		void FillBitmapInfo( BITMAPINFO& bitmapInfo );

	protected: 
		unsigned int m_uiWidth;			
		unsigned int m_uiHeight; 
		unsigned int m_uiBytesPerPixel;
		unsigned int m_uiCapacity;
		unsigned char* m_pPixels;
	};

}//namespace Render

#endif//ZP_FRAMEBUFFER