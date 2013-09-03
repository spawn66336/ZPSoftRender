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
		* @brief 为光栅器指定颜色缓冲区
		*/
		inline void SetColorBuffer( FrameBuffer* pBuf ){ m_pColorBuffer = pBuf; }

		/**
		* @brief 为光栅器指定深度缓冲区
		*/
		inline void SetDepthBuffer( FrameBuffer* pBuf ){ m_pDepthBuffer = pBuf; }

		/**
		* @brief 缓冲区宽度
		*/
		inline unsigned int BufferWidth( void )  const
		{
			ZP_ASSERT( NULL != m_pColorBuffer );
			return m_pColorBuffer->Width();
		}

		/**
		* @brief 缓冲区高度
		*/
		inline unsigned int BufferHeight( void ) const	{
			ZP_ASSERT( NULL != m_pColorBuffer );
			return m_pColorBuffer->Height();
		}

		/**
		* @brief 对指定位置处的像素进行深度测试
		* @param x 像素位置横坐标
		* @param y 像素位置纵坐标
		* @param depth 深度值
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
		* @brief 将颜色值color 写入颜色缓冲区
		* @param x 写入的像素位置横坐标
		* @param y 写入的像素位置纵坐标
		* @param color 要写入的像素颜色
		*/
		inline void WritePixel( const int x , const int y ,  Math::BGRA8888_t color )
		{
			m_pColorBuffer->WritePixel( x , y , (&color) ); 
		}

		unsigned int Calc2DPointOutCode( const int x , const int y ); 

		/**
		* @brief 判断由p0,p1,p2顶点组成的三角面是否在屏幕外
		*/
		bool IsOutOfScreen( const Math::Vec2& p0 , const Math::Vec2& p1 , const Math::Vec2& p2 );

		/**
		* @brief 用中点画线算法画线
		* @param p0 线段的起始端点
		* @param p1 线段的终止端点
		* @param color 线段颜色
		*/
		void DrawLineMidPoint( const Math::Vec2& p0 , const Math::Vec2& p1 , const Math::Vec4& color  );

		/**
		* @brief 绘制垂线段
		* @param x 垂线段的横坐标
		* @param y0 垂线的起始纵坐标
		* @param y1 垂线的终止纵坐标
		* @param color 线段颜色
		*/
		void DrawVLine( const int x , const int y0 , const int y1 ,  Math::BGRA8888_t color  );

		/**
		* @brief 绘制横线段
		* @param y 横线段的纵坐标
		* @param x0 横线段的起始横坐标
		* @param x1 横线段的终止横坐标
		* @param color 线段颜色
		*/
		void DrawHLine( const int y , const int x0 , const int x1 ,  Math::BGRA8888_t  color  ); 

		/**
		* @brief 绘制扫描线
		* @param y 扫描线的纵坐标
		* @param xs 扫描线的起始横坐标
		* @param xe 扫描线的终止横坐标
		* @param v0 扫描线起始端点的插值顶点信息
		* @param v1 扫描线终止端点的插值顶点信息
		* @param shader 像素着色器
		*/
		void DrawScanLine( const int y , const int xs , const int xe , const RVertex& v0 , const RVertex& v1 , PixelShader& shader );

		/**
		* @brief 绘制任意2D三角面
		* @param v0 屏幕空间顶点0
		* @param v1 屏幕空间顶点1
		* @param v2 屏幕空间顶点2
		* @param shader 像素着色器
		* @remark 此函数会将2D三角面分割为平底和平顶三角形分别进行
		*				  光栅化
		*/
		void DrawTriangle2D( const RVertex& v0 , const RVertex& v1 , const RVertex& v2 , PixelShader& shader );

		/**
		* @brief 绘制平底2D三角面
		* @param v0 平底三角面顶部顶点
		* @param v1 平底三角面左侧顶点
		* @param v2 平底三角面右侧顶点
		* @param shader 像素着色器
		*/
		void DrawTriangle2DFlatBottom( const RVertex& v0 , const RVertex& v1 , const RVertex& v2 , PixelShader& shader );

		/**
		* @brief 绘制平顶2D三角面
		* @param v0 平顶2D三角面顶部左侧顶点
		* @param v1 平顶2D三角面顶部右侧顶点
		* @param v2 平顶2D三角面底部顶点
		* @param shader 像素着色器
		*/
		void DrawTriangle2DFlatTop( const RVertex& v0 , const RVertex& v1 , const RVertex& v2 , PixelShader& shader );


	protected:
		FrameBuffer* m_pColorBuffer;		///>颜色缓冲区
		FrameBuffer* m_pDepthBuffer;		///>深度缓冲区
	};

}//namespace Render

#endif //ZP_RASTERPROCESSOR
