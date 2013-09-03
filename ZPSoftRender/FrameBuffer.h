#ifndef ZP_FRAMEBUFFER
#define ZP_FRAMEBUFFER

#include "ZPDependency.h"
#include "ZPMathDependency.h" 

namespace Render
{

	/**

		屏幕坐标系 		
		(0,0)	-------------------> X
		|
		|
		|
		|
		V
		Y
	*/ 
 
	class RVertex;

	class FrameBuffer
	{
	protected:
		FrameBuffer();
	public:
		FrameBuffer( const unsigned int uiWidth , const unsigned int uiHeight , const unsigned int uiBytesPerPixel );
		virtual ~FrameBuffer(void); 

		/**
		* @brief 帧缓冲区宽度
		*/
		inline unsigned int Width( void ) const { return m_uiWidth; }

		/**
		* @brief 帧缓冲区高度
		*/
		inline unsigned int Height( void ) const { return m_uiHeight; }

		/**
		* @brief 每个像素的字节数
		*/
		inline unsigned int BytesPerPixel( void ) const { return m_uiBytesPerPixel; }

		/**
		* @brief 帧缓冲区的总容量
		*/
		inline unsigned int Capacity( void ) const{ return m_uiCapacity; }

		/**
		* @brief 调整帧缓冲区大小
		* @param uiWidth 调整宽度（以像素计）
		* @param uiHeight 调整高度（以像素计）
		*/
		void Resize( const int uiWidth , const int uiHeight );

		/**
		* @brief 根据缓冲区的宽高信息调整缓冲区内存
		*/
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

		/**
		* @brief 根据缓冲区的属性填充BITMAPINFO结构以供GDI显示使用
		*/
		void FillBitmapInfo( BITMAPINFO& bitmapInfo );

	protected: 
		unsigned int m_uiWidth;			///>帧缓冲区宽度（以像素计）
		unsigned int m_uiHeight;			///>帧缓冲区高度（以像素计）
		unsigned int m_uiBytesPerPixel;///>每个像素的字节宽度
		unsigned int m_uiCapacity;		///>缓冲区容量（以字节计）
		unsigned char* m_pPixels;		///>指向缓冲区指针
	};

}//namespace Render

#endif//ZP_FRAMEBUFFER