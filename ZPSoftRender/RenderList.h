#ifndef ZP_RENDERLIST
#define ZP_RENDERLIST

#include "ZPDependency.h"
#include "ZPMathDependency.h"


namespace Render
{ 
	class Vertex;
	class RenderPrimitive;
	class FrameBuffer;

	enum RVERTEX_STATE
	{ 
		RVERT_STATE_ACTIVE = 1
	};

	class RVertex
	{
	public:
		RVertex();
		RVertex( const Vertex& v );
		RVertex( const RVertex& rv );
		~RVertex();

		bool TestStateBit( const unsigned int bit );
		void SetStateBit( const unsigned int bit );
		void ClearStateBits( void ){ m_uiState = 0; }

		void CopyFromVertex( const Vertex& v );
		RVertex& operator=( const RVertex& rhs );

		Math::Vec3 m_v3Pos;
		Math::Vec3 m_v3Normal;
		Math::Vec2 m_v2Texcoord;
		Math::Vec3 m_v3Tangent;
		Math::Vec3 m_v3Binormal;

		unsigned int m_uiState;
	};


	enum RENDERFACE_STATE
	{
		RFACE_STATE_CLIPED = 1,
		RFACE_STATE_BACKFACE = 2
	};

	class RFace
	{
	public:
		RFace();
		RFace( const RFace& face );
		~RFace();

		bool TestStateBit( const unsigned int bit );
		void SetStateBit( const unsigned int bit );
		void ClearStateBits( void ){ m_uiState = 0; }

		RFace& operator=( const RFace& rhs );

		unsigned int m_uiIndices[3];  ///>��������
		unsigned int m_uiState;		 ///>��Ⱦ��״̬
		RFace*			m_pNext;			 ///>��һ����Ⱦ��
	};

	class RenderList
	{
	public:
		RenderList(void);
		~RenderList(void);

		void CopyFromRenderPrimitive( RenderPrimitive& primitive );

		void AddFace( const RFace& face );

		void Clear( void );

		/**
		* @brief ����Ⱦ�б��е����ж���ִ�оֲ����������ϵת��
		*/
		void TransformFromLocalSpaceToCameraSpace( const Math::Matrix4& localToCam ); 

		/**
		* @brief ������ռ����޳����б�����
		*/
		void RemoveBackFaceInCameraSpace( void );

		/**
		* @brief ����Ⱦ�б��е����ж���ִ�������ͶӰ����ϵ�任
		*/
		void TransformFromCameraSpaceToProjectionSpace( const Math::Matrix4& camToProj );

		/**
		* @brief ����Ⱦ�б��е����ж���ִ��ͶӰ����ϵ����Ļ����ϵ�任
		*/
		void TransformFromProjectionSpaceToScreenSpace( const Math::Matrix4& projToScreen );

		/**
		* @brief ��������߿�ģ�͵�֡������
		*/
		void DrawFacesWireFrameToFrameBuffer(  FrameBuffer& buf );

		/**
		* @brief �������ʵɫ������ģ�͵�֡������
		*/
		void DrawFacesSolidTrianglesToFrameBuffer( FrameBuffer& buf );

	protected:
		unsigned int m_uiVertCount; ///>������
		unsigned int m_uiFaceCount; ///>����

		RVertex* m_pRVerts;				  ///>�����б�
		RVertex* m_pRTransVerts;      ///>�任��Ķ����б�
		RFace*    m_pFace;				  ///>������ͷָ��
	};

}//namespace Render

#endif //ZP_RENDERLIST