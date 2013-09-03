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
 
	class RVertex;

	class FrameBuffer
	{
	protected:
		FrameBuffer();
	public:
		FrameBuffer( const unsigned int uiWidth , const unsigned int uiHeight , const unsigned int uiBytesPerPixel );
		virtual ~FrameBuffer(void); 

		/**
		* @brief ֡���������
		*/
		inline unsigned int Width( void ) const { return m_uiWidth; }

		/**
		* @brief ֡�������߶�
		*/
		inline unsigned int Height( void ) const { return m_uiHeight; }

		/**
		* @brief ÿ�����ص��ֽ���
		*/
		inline unsigned int BytesPerPixel( void ) const { return m_uiBytesPerPixel; }

		/**
		* @brief ֡��������������
		*/
		inline unsigned int Capacity( void ) const{ return m_uiCapacity; }

		/**
		* @brief ����֡��������С
		* @param uiWidth ������ȣ������ؼƣ�
		* @param uiHeight �����߶ȣ������ؼƣ�
		*/
		void Resize( const int uiWidth , const int uiHeight );

		/**
		* @brief ���ݻ������Ŀ����Ϣ�����������ڴ�
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
		* @brief ���ݻ��������������BITMAPINFO�ṹ�Թ�GDI��ʾʹ��
		*/
		void FillBitmapInfo( BITMAPINFO& bitmapInfo );

	protected: 
		unsigned int m_uiWidth;			///>֡��������ȣ������ؼƣ�
		unsigned int m_uiHeight;			///>֡�������߶ȣ������ؼƣ�
		unsigned int m_uiBytesPerPixel;///>ÿ�����ص��ֽڿ��
		unsigned int m_uiCapacity;		///>���������������ֽڼƣ�
		unsigned char* m_pPixels;		///>ָ�򻺳���ָ��
	};

}//namespace Render

#endif//ZP_FRAMEBUFFER