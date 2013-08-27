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

		unsigned int m_uiIndices[3];  ///>顶点索引
		unsigned int m_uiState;		 ///>渲染面状态
		RFace*			m_pNext;			 ///>下一个渲染面
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
		* @brief 对渲染列表中的所有顶点执行局部到相机坐标系转换
		*/
		void TransformFromLocalSpaceToCameraSpace( const Math::Matrix4& localToCam ); 

		/**
		* @brief 在相机空间中剔除所有背向面
		*/
		void RemoveBackFaceInCameraSpace( void );

		/**
		* @brief 对渲染列表中的所有顶点执行相机到投影坐标系变换
		*/
		void TransformFromCameraSpaceToProjectionSpace( const Math::Matrix4& camToProj );

		/**
		* @brief 对渲染列表中的所有顶点执行投影坐标系到屏幕坐标系变换
		*/
		void TransformFromProjectionSpaceToScreenSpace( const Math::Matrix4& projToScreen );

		/**
		* @brief 绘制面的线框模型到帧缓冲区
		*/
		void DrawFacesWireFrameToFrameBuffer(  FrameBuffer& buf );

		/**
		* @brief 绘制面的实色三角面模型到帧缓冲区
		*/
		void DrawFacesSolidTrianglesToFrameBuffer( FrameBuffer& buf );

	protected:
		unsigned int m_uiVertCount; ///>顶点数
		unsigned int m_uiFaceCount; ///>面数

		RVertex* m_pRVerts;				  ///>顶点列表
		RVertex* m_pRTransVerts;      ///>变换后的顶点列表
		RFace*    m_pFace;				  ///>面链表头指针
	};

}//namespace Render

#endif //ZP_RENDERLIST