#ifndef ZP_FRAMEBUFFER
#define ZP_FRAMEBUFFER

#include "ZPDependency.h"
#include "ZPMathDependency.h" 

namespace Render
{

	/**

		��Ļ����ϵ 		
		(0,0)	-------------------> X
		|
		|
		|
		|
		V
		Y
	*/

	enum POINT2D_OUTCODE
	{
		OCODE_2D_LEFT = 1,
		OCODE_2D_RIGHT = 2,
		OCODE_2D_TOP = 4,
		OCODE_2D_BOTTOM = 8
	};

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

		unsigned int Calc2DPointOutCode( const int x , const int y );

		/**
		* @brief DDA�㷨��ֱ��
		*/
		void DrawLineDDA( const Math::Vec2& p0 , const Math::Vec2& p1 , void* pPixel );

		void DrawLineMidPoint( const Math::Vec2& p0 , const Math::Vec2& p1 , void* pPixel );

		/**
		* @brief ����ʵ��ƽ��������
		* @ param x0 y0 ƽ�������εĵ�һ������ λ���Ϸ�
		* @ param x1 y1 ƽ�������εĵڶ������� λ������
		* @ param x2 y2 ƽ�������εĵ��������� λ������
		*/
		void DrawTriangle2DFlatBaseSolid( 
			const int x0 , const int y0 , 
			const int x1 , const int y1 , 
			const int x2 , const int y2 , 
			void* pPixel );

		/**
		* @brief ����ʵ��ƽ��������
		* @ param x0 y0 ƽ�������εĵ�һ������ λ������
		* @ param x1 y1 ƽ�������εĵڶ������� λ������
		* @ param x2 y2 ƽ�������εĵ��������� λ���·�
		*/
		void DrawTriangle2DFlatTopSolid( 
			const int x0 , const int y0 , 
			const int x1 , const int y1 , 
			const int x2 , const int y2 , 
			void* pPixel );

		void DrawTriangle2DSolid( 
			const int x0 , const int y0 , 
			const int x1 , const int y1 , 
			const int x2 , const int y2 , 
			void* pPixel );

		/**
		* @brief ����ʵ��������
		*/
		void DrawTriangle2DSolid( const Math::Vec2& p0 , const Math::Vec2& p1 , const Math::Vec2& p2 , void* pPixel );


		void DrawTriangle2DFlatBase( 			
			const int x0 , const int y0 , 
			const int x1 , const int y1 , 
			const int x2 , const int y2 ,  
			const Math::Vec4& color0 ,
			const Math::Vec4& color1 ,
			const Math::Vec4& color2 );

		void DrawTriangle2DFlatTop( 			
				const int x0 , const int y0 , 
				const int x1 , const int y1 , 
				const int x2 , const int y2 ,
				const Math::Vec4& color0 ,
				const Math::Vec4& color1 ,
				const Math::Vec4& color2  
			); 

		 void DrawTriangle2D( const RVertex& v0 , const RVertex& v1 , const RVertex& v2 );

		void DrawVLine( const int x , const int y0 , const int y1 , void* pPixel );

		void DrawHLine( const int y , const int x0 , const int x1 , void* pPixel ); 

		void DrawHLine( const int y , const int x0 , const int x1 , const Math::Vec4& colorStart , const Math::Vec4& colorEnd );

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