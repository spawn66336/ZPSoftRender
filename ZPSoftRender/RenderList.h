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

		Math::Vec3 m_v3Pos;				///>����λ��
		Math::Vec3 m_v3Normal;			///>���㷨��
		Math::Vec2 m_v2Texcoord;		///>������������
		Math::Vec3 m_v3Tangent;		///>����Tangent����
		Math::Vec3 m_v3Binormal;		///>����Binormal����
		Math::Vec4 m_v4Color;				///>������ɫ
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

		Math::Vec3  m_v3Normal;     ///>�淨�ߣ��ڱ����޳��׶����ɣ�
		Math::Vec4  m_v4Color;		 ///>����ɫ
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

		inline unsigned int VertCount(void) const { return m_uiVertCount; }

		inline unsigned int FaceCount(void) const { return m_uiFaceCount; }

		inline RVertex* GetRVerts( void ) const { return m_pRVerts; }

		inline RVertex* GetRTransVerts( void ) const { return m_pRTransVerts; }

		inline RFace*    GetFaceList( void ) const { return m_pFace; }
		 
	protected:
		unsigned int m_uiVertCount; ///>������
		unsigned int m_uiFaceCount; ///>����

		RVertex* m_pRVerts;				  ///>�����б�
		RVertex* m_pRTransVerts;      ///>�任��Ķ����б�
		RFace*    m_pFace;				  ///>������ͷָ��
	};

}//namespace Render

#endif //ZP_RENDERLIST