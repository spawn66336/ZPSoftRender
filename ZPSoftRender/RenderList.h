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

	enum RVERTEX_ATTRIBUTE
	{
		RVERT_ATTRI_SHADE_FLAT = 1,
		RVERT_ATTRI_SHADE_GOURAUD = 2,
		RVERT_ATTRI_SHADE_PHONG = 4,
		RVERT_ATTRI_SHADE_NORMMAP = 8 ,
		RVERT_ATTRI_SHADE_WIREFRAME = 16 ,
		RVERT_ATTRI_SHADE_WITH_TEXTURE = 32
	};

	class RVertex
	{
	public:
		RVertex();
		RVertex( const Vertex& v );
		RVertex( const RVertex& rv );
		~RVertex();

		bool TestStateBit( const unsigned int bit );
		inline void SetStateBit( const unsigned int bit ){ m_uiState|= bit; }
		inline void ClearStateBits( void ){ m_uiState = 0; }

		inline bool TestAttriBit( const unsigned int bit ) const { return ( 0 != ( m_uiAttri&bit ) ); }
		inline void SetAttriBit( const unsigned int bit ){ m_uiAttri |= bit; }
		inline void ClearAttriBits( void ) { m_uiAttri = 0; }

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

		Math::Vec3 m_v3Pos;				///>����λ��
		Real			   m_invZ;					///>Zֵ����
		Math::Vec3 m_v3PosInCam;     ///>������ռ��еĶ���λ��
		Math::Vec3 m_v3Normal;			///>���㷨��
		Math::Vec2 m_v2Texcoord;		///>������������
		Math::Vec3 m_v3Tangent;		///>����Tangent����
		Math::Vec3 m_v3Binormal;		///>����Binormal����
		Math::Vec4 m_v4Color;				///>������ɫ
		unsigned int m_uiState;			
		unsigned int m_uiAttri;				///>��Ⱦ��������
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

		inline bool TestStateBit( const unsigned int bit ) const	
		{
			return ( 0 != ( m_uiState&bit) );
		}
		void SetStateBit( const unsigned int bit );
		void ClearStateBits( void ){ m_uiState = 0; }
		inline bool IsActive( void ) const 
		{
			return ( 
						  !TestStateBit( RFACE_STATE_CLIPED ) && 
						  !TestStateBit( RFACE_STATE_BACKFACE ) 
					  );
		}

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

		inline unsigned int RTransVertCount( void ) const { return m_uiTransVertCount; }

		inline unsigned int RTransVertCapacity( void ) const { return m_uiTransVertsCapacity; }

		/**
		* @brief ��任���Ķ����б��м����¶���
		* @param vert �����붥��
		* @return �����¼��붥�������
		*/
		unsigned int AddTransVert( const RVertex& vert );

		inline RVertex* GetRVerts( void ) const { return m_pRVerts; }

		inline RVertex* GetRTransVerts( void ) const { return m_pRTransVerts; }

		inline RFace*    GetFaceList( void ) const { return m_pFace; }
		 
	protected:
		unsigned int m_uiFaceCount; ///>����

		unsigned int m_uiVertCount;   ///>ԭʼ������
		RVertex* m_pRVerts;				  ///>ԭʼ�����б�

		unsigned int m_uiTransVertCount;  ///>�任��Ķ����б���ʵ�ʵĶ�����
		unsigned int m_uiTransVertsCapacity; ///>�任��Ķ����б��������ɵĶ�����
		RVertex* m_pRTransVerts;      ///>�任��Ķ����б�
		RFace*    m_pFace;				  ///>������ͷָ��
	};

}//namespace Render

#endif //ZP_RENDERLIST