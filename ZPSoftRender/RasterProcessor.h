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

		/**
		* @brief Ϊ��դ��ָ����ɫ������
		*/
		inline void SetColorBuffer( FrameBuffer* pBuf ){ m_pColorBuffer = pBuf; }

		/**
		* @brief Ϊ��դ��ָ����Ȼ�����
		*/
		inline void SetDepthBuffer( FrameBuffer* pBuf ){ m_pDepthBuffer = pBuf; }

		/**
		* @brief ���������
		*/
		inline unsigned int BufferWidth( void )  const
		{
			ZP_ASSERT( NULL != m_pColorBuffer );
			return m_pColorBuffer->Width();
		}

		/**
		* @brief �������߶�
		*/
		inline unsigned int BufferHeight( void ) const	{
			ZP_ASSERT( NULL != m_pColorBuffer );
			return m_pColorBuffer->Height();
		}

		/**
		* @brief ��ָ��λ�ô������ؽ�����Ȳ���
		* @param x ����λ�ú�����
		* @param y ����λ��������
		* @param depth ���ֵ
		*/
		inline bool DepthTest( const int x , const int y , Real depth )
		{ 
			Real fDepth = 0;
			m_pDepthBuffer->ReadPixel( x , y , &fDepth ); 

			if( depth > fDepth )
			{
				m_pDepthBuffer->WritePixel( x , y , &depth );
				return true;
			}
			return false;
		}

		/**
		* @brief ����ɫֵcolor д����ɫ������
		* @param x д�������λ�ú�����
		* @param y д�������λ��������
		* @param color Ҫд���������ɫ
		*/
		inline void WritePixel( const int x , const int y ,  Math::BGRA8888_t color )
		{
			m_pColorBuffer->WritePixel( x , y , (&color) ); 
		}

		unsigned int Calc2DPointOutCode( const int x , const int y ); 

		/**
		* @brief �ж���p0,p1,p2������ɵ��������Ƿ�����Ļ��
		*/
		bool IsOutOfScreen( const Math::Vec2& p0 , const Math::Vec2& p1 , const Math::Vec2& p2 );

		/**
		* @brief ���е㻭���㷨����
		* @param p0 �߶ε���ʼ�˵�
		* @param p1 �߶ε���ֹ�˵�
		* @param color �߶���ɫ
		*/
		void DrawLineMidPoint( const Math::Vec2& p0 , const Math::Vec2& p1 , const Math::Vec4& color  );

		/**
		* @brief ���ƴ��߶�
		* @param x ���߶εĺ�����
		* @param y0 ���ߵ���ʼ������
		* @param y1 ���ߵ���ֹ������
		* @param color �߶���ɫ
		*/
		void DrawVLine( const int x , const int y0 , const int y1 ,  Math::BGRA8888_t color  );

		/**
		* @brief ���ƺ��߶�
		* @param y ���߶ε�������
		* @param x0 ���߶ε���ʼ������
		* @param x1 ���߶ε���ֹ������
		* @param color �߶���ɫ
		*/
		void DrawHLine( const int y , const int x0 , const int x1 ,  Math::BGRA8888_t  color  ); 

		/**
		* @brief ����ɨ����
		* @param y ɨ���ߵ�������
		* @param xs ɨ���ߵ���ʼ������
		* @param xe ɨ���ߵ���ֹ������
		* @param v0 ɨ������ʼ�˵�Ĳ�ֵ������Ϣ
		* @param v1 ɨ������ֹ�˵�Ĳ�ֵ������Ϣ
		* @param shader ������ɫ��
		*/
		void DrawScanLine( const int y , const int xs , const int xe , const RVertex& v0 , const RVertex& v1 , PixelShader& shader );

		/**
		* @brief ��������2D������
		* @param v0 ��Ļ�ռ䶥��0
		* @param v1 ��Ļ�ռ䶥��1
		* @param v2 ��Ļ�ռ䶥��2
		* @param shader ������ɫ��
		* @remark �˺����Ὣ2D������ָ�Ϊƽ�׺�ƽ�������ηֱ����
		*				  ��դ��
		*/
		void DrawTriangle2D( const RVertex& v0 , const RVertex& v1 , const RVertex& v2 , PixelShader& shader );

		/**
		* @brief ����ƽ��2D������
		* @param v0 ƽ�������涥������
		* @param v1 ƽ����������ඥ��
		* @param v2 ƽ���������Ҳඥ��
		* @param shader ������ɫ��
		*/
		void DrawTriangle2DFlatBottom( const RVertex& v0 , const RVertex& v1 , const RVertex& v2 , PixelShader& shader );

		/**
		* @brief ����ƽ��2D������
		* @param v0 ƽ��2D�����涥����ඥ��
		* @param v1 ƽ��2D�����涥���Ҳඥ��
		* @param v2 ƽ��2D������ײ�����
		* @param shader ������ɫ��
		*/
		void DrawTriangle2DFlatTop( const RVertex& v0 , const RVertex& v1 , const RVertex& v2 , PixelShader& shader );


	protected:
		FrameBuffer* m_pColorBuffer;		///>��ɫ������
		FrameBuffer* m_pDepthBuffer;		///>��Ȼ�����
	};

}//namespace Render

#endif //ZP_RASTERPROCESSOR
