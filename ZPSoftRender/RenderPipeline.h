#ifndef ZP_RENDERPIPELINE
#define ZP_RENDERPIPELINE
#include "RenderList.h"
#include "RasterProcessor.h"
#include "PixelShader.h"

namespace Render
{
	class RenderContext;
	class RenderPrimitive;

	class RenderPipeline
	{
	public:
		RenderPipeline(void);
		virtual ~RenderPipeline(void); 

		/**
		* @brief 设置渲染流水线所需渲染上下文对象
		* @param pRenderContext 指向渲染上下文的指针
		*/
		inline void SetRenderContext( RenderContext* pRenderContext ){ m_pRenderContext = pRenderContext; }

		/**
		* @brief 获取渲染上下文
		*/
		inline RenderContext* GetRenderContext( void ) const { return m_pRenderContext; }

		/**
		* @brief 绘制几何图元
		*/
		virtual void DrawElements( RenderPrimitive& renderPrimitive );

	protected:

		/**
		* @brief 变换阶段
		* @remark 此阶段会将顶点变换到摄像机空间，然后进行
		*				  背面消除最后进行视锥剔除剪裁
		*/
		virtual void RunTransformStage( void );

		/**
		* @brief 光照阶段
		* @remark 此阶段会执行与光照有关的操作
		*				 若为Flat着色此阶段会计算光照影响下面颜色
		*				 若为Gouraud着色此阶段会计算光照影响下的顶点颜色
		*				 若为Phong着色此阶段会计算每个光源的摄像机坐标位置并将光源传给像素
		*				 Shader供在光栅化阶段处理
		*/
		virtual void RunLightingStage( void );

		/**
		* @brief 后光照阶段
		* @remark 此阶段会将摄像机空间中的坐标进行透视投影
		*				  再将投影后的坐标变换到屏幕坐标空间
		*/
		virtual void RunPostLightingStage( void );

		/**
		* @brief 光栅化阶段
		* @remark 此阶段对不同着色管线的实现不同，在线框着色流水线
		*				  绘制顶点间连线，在Flat着色流水线按面光照颜色与纹理
		*				  颜色进行光栅化，在Gouraud着色流水线段按光照顶点颜色
		*				  与纹理颜色进行光栅化，在Phong或NormalMap着色流水线
		*				  对每个法线、Tangent、Binormal 在像素间进行插值计算逐
		*				  像素光照颜色与纹理颜色进行光栅化。
		*/
		virtual void RunRasterizationStage( void ); 

	protected:

		/**
		* @brief 初始化渲染顶点列表
		* @remark 此函数会根据所选着色模型来初始化每个顶点
		*/
		void InitRVerts( void );

		/**
		* @brief 对渲染列表中的所有顶点执行局部到相机坐标系转换
		*/
		void TransformFromLocalSpaceToCameraSpace( void ); 

		/**
		* @brief 在相机空间中剔除所有背向面
		*/
		void RemoveBackFaceInCameraSpace( void );

		/**
		* @brief 在相机空间中对面列表进行视锥体剪裁
		*/
		void ClipFacesInCameraSpace( void );

		/**
		* @brief 在相机空间中计算逐面光照
		*/
		void CalcPerFaceLightingInCameraSpace( void );

		/**
		* @brief 在相机空间中计算逐顶点
		*/
		void CalcPerVertexLightingInCameraSpace( void );

		/**
		* @brief 在相机空间中为逐像素光照准备灯光列表
		*/
		void PrepareForPerPixelLightingInCameraSpace( void );

		/**
		* @brief 对渲染列表中的所有顶点执行相机到投影坐标系变换
		*/
		void TransformFromCameraSpaceToProjectionSpace( void );

		/**
		* @brief 对渲染列表中的所有顶点执行投影坐标系到屏幕坐标系变换
		*/
		void TransformFromProjectionSpaceToScreenSpace( void );

		/**
		* @brief 透视矫正
		*/
		void PerspectiveCorrection(void);

		/**
		* @brief 绘制面的线框模型到帧缓冲区
		*/
		void DrawFacesWireFrameToFrameBuffer( void );

		/**
		* @brief 绘制面的实色三角面模型到帧缓冲区
		*/
		void DrawFacesSolidTrianglesToFrameBuffer( void );

		/**
		* @brief 将三角面绘制到帧缓冲区
		*/
		void DrawTrianglesToFrameBuffer( void );
		 

		RenderContext* m_pRenderContext;
		RenderList		  m_renderList;
		RasterProcessor  m_rasterProcessor;
		PixelShader		   m_pixelShader;
	};

}//namespace Render

#endif// ZP_RENDERPIPELINE