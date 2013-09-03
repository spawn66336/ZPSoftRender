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
		* @brief ������Ⱦ��ˮ��������Ⱦ�����Ķ���
		* @param pRenderContext ָ����Ⱦ�����ĵ�ָ��
		*/
		inline void SetRenderContext( RenderContext* pRenderContext ){ m_pRenderContext = pRenderContext; }

		/**
		* @brief ��ȡ��Ⱦ������
		*/
		inline RenderContext* GetRenderContext( void ) const { return m_pRenderContext; }

		/**
		* @brief ���Ƽ���ͼԪ
		*/
		virtual void DrawElements( RenderPrimitive& renderPrimitive );

	protected:

		/**
		* @brief �任�׶�
		* @remark �˽׶λὫ����任��������ռ䣬Ȼ�����
		*				  ����������������׶�޳�����
		*/
		virtual void RunTransformStage( void );

		/**
		* @brief ���ս׶�
		* @remark �˽׶λ�ִ��������йصĲ���
		*				 ��ΪFlat��ɫ�˽׶λ�������Ӱ��������ɫ
		*				 ��ΪGouraud��ɫ�˽׶λ�������Ӱ���µĶ�����ɫ
		*				 ��ΪPhong��ɫ�˽׶λ����ÿ����Դ�����������λ�ò�����Դ��������
		*				 Shader���ڹ�դ���׶δ���
		*/
		virtual void RunLightingStage( void );

		/**
		* @brief ����ս׶�
		* @remark �˽׶λὫ������ռ��е��������͸��ͶӰ
		*				  �ٽ�ͶӰ�������任����Ļ����ռ�
		*/
		virtual void RunPostLightingStage( void );

		/**
		* @brief ��դ���׶�
		* @remark �˽׶ζԲ�ͬ��ɫ���ߵ�ʵ�ֲ�ͬ�����߿���ɫ��ˮ��
		*				  ���ƶ�������ߣ���Flat��ɫ��ˮ�߰��������ɫ������
		*				  ��ɫ���й�դ������Gouraud��ɫ��ˮ�߶ΰ����ն�����ɫ
		*				  ��������ɫ���й�դ������Phong��NormalMap��ɫ��ˮ��
		*				  ��ÿ�����ߡ�Tangent��Binormal �����ؼ���в�ֵ������
		*				  ���ع�����ɫ��������ɫ���й�դ����
		*/
		virtual void RunRasterizationStage( void ); 

	protected:

		/**
		* @brief ��ʼ����Ⱦ�����б�
		* @remark �˺����������ѡ��ɫģ������ʼ��ÿ������
		*/
		void InitRVerts( void );

		/**
		* @brief ����Ⱦ�б��е����ж���ִ�оֲ����������ϵת��
		*/
		void TransformFromLocalSpaceToCameraSpace( void ); 

		/**
		* @brief ������ռ����޳����б�����
		*/
		void RemoveBackFaceInCameraSpace( void );

		/**
		* @brief ������ռ��ж����б������׶�����
		*/
		void ClipFacesInCameraSpace( void );

		/**
		* @brief ������ռ��м����������
		*/
		void CalcPerFaceLightingInCameraSpace( void );

		/**
		* @brief ������ռ��м����𶥵�
		*/
		void CalcPerVertexLightingInCameraSpace( void );

		/**
		* @brief ������ռ���Ϊ�����ع���׼���ƹ��б�
		*/
		void PrepareForPerPixelLightingInCameraSpace( void );

		/**
		* @brief ����Ⱦ�б��е����ж���ִ�������ͶӰ����ϵ�任
		*/
		void TransformFromCameraSpaceToProjectionSpace( void );

		/**
		* @brief ����Ⱦ�б��е����ж���ִ��ͶӰ����ϵ����Ļ����ϵ�任
		*/
		void TransformFromProjectionSpaceToScreenSpace( void );

		/**
		* @brief ͸�ӽ���
		*/
		void PerspectiveCorrection(void);

		/**
		* @brief ��������߿�ģ�͵�֡������
		*/
		void DrawFacesWireFrameToFrameBuffer( void );

		/**
		* @brief �������ʵɫ������ģ�͵�֡������
		*/
		void DrawFacesSolidTrianglesToFrameBuffer( void );

		/**
		* @brief ����������Ƶ�֡������
		*/
		void DrawTrianglesToFrameBuffer( void );
		 

		RenderContext* m_pRenderContext;
		RenderList		  m_renderList;
		RasterProcessor  m_rasterProcessor;
		PixelShader		   m_pixelShader;
	};

}//namespace Render

#endif// ZP_RENDERPIPELINE