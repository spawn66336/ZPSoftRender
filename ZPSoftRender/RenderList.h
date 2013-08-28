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

		RVertex operator+( const RVertex& rhs ) const;
		RVertex operator-( const RVertex& rhs ) const;
		RVertex operator*( const Real rhs ) const;
		RVertex operator/( const Real rhs ) const;

		RVertex& operator+=( const RVertex& rhs );
		RVertex& operator-=( const RVertex& rhs );
		RVertex& operator*=( const Real rhs );
		RVertex& operator/=( const Real rhs );

		Math::Vec3 m_v3Pos;				///>顶点位置
		Math::Vec3 m_v3Normal;			///>顶点法线
		Math::Vec2 m_v2Texcoord;		///>顶点纹理坐标
		Math::Vec3 m_v3Tangent;		///>顶点Tangent向量
		Math::Vec3 m_v3Binormal;		///>顶点Binormal向量
		Math::Vec4 m_v4Color;				///>顶点颜色
		unsigned int m_uiState;
	};

	RVertex operator*( const Real lhs , const RVertex& rhs );


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

		Math::Vec3  m_v3Normal;     ///>面法线（在背面剔除阶段生成）
		Math::Vec4  m_v4Color;		 ///>面颜色
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

		inline unsigned int VertCount(void) const { return m_uiVertCount; }

		inline unsigned int FaceCount(void) const { return m_uiFaceCount; }

		inline RVertex* GetRVerts( void ) const { return m_pRVerts; }

		inline RVertex* GetRTransVerts( void ) const { return m_pRTransVerts; }

		inline RFace*    GetFaceList( void ) const { return m_pFace; }
		 
	protected:
		unsigned int m_uiVertCount; ///>顶点数
		unsigned int m_uiFaceCount; ///>面数

		RVertex* m_pRVerts;				  ///>顶点列表
		RVertex* m_pRTransVerts;      ///>变换后的顶点列表
		RFace*    m_pFace;				  ///>面链表头指针
	};

}//namespace Render

#endif //ZP_RENDERLIST