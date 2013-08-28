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

		virtual void SetRenderContext( RenderContext* pRenderContext ){ m_pRenderContext = pRenderContext; }

		virtual RenderContext* GetRenderContext( void ) const { return m_pRenderContext; }

		virtual void DrawElements( RenderPrimitive& renderPrimitive );

	protected:

		virtual void RunVertexShaderStage( void );

		virtual void RunRasterizationStage( void );

		virtual void RunEarlyZPassStage( void );

		virtual void RunFragmentShaderStage( void );

		virtual void RunMergeStage( void );

	protected:


		/**
		* @brief 对渲染列表中的所有顶点执行局部到相机坐标系转换
		*/
		void TransformFromLocalSpaceToCameraSpace( void ); 

		/**
		* @brief 在相机空间中剔除所有背向面
		*/
		void RemoveBackFaceInCameraSpace( void );

		void CalcLightInCameraSpace( void );

		void CalcPerVertexLightInCameraSpace( void );

		/**
		* @brief 对渲染列表中的所有顶点执行相机到投影坐标系变换
		*/
		void TransformFromCameraSpaceToProjectionSpace( void );

		/**
		* @brief 对渲染列表中的所有顶点执行投影坐标系到屏幕坐标系变换
		*/
		void TransformFromProjectionSpaceToScreenSpace( void );

		/**
		* @brief 绘制面的线框模型到帧缓冲区
		*/
		void DrawFacesWireFrameToFrameBuffer( void );

		/**
		* @brief 绘制面的实色三角面模型到帧缓冲区
		*/
		void DrawFacesSolidTrianglesToFrameBuffer( void );

		void DrawGouraudShadingTrianglesToFrameBuffer( void );
		 

		RenderContext* m_pRenderContext;
		RenderList		  m_renderList;
		RasterProcessor  m_rasterProcessor;
		PixelShader			m_pixelShader;
	};

}//namespace Render

#endif// ZP_RENDERPIPELINE