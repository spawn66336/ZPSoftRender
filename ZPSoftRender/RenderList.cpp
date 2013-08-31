#include "RenderList.h"
#include "FrameStackAllocator.h"
#include "RenderPrimitive.h"
#include "Vertex.h"
#include "FrameBuffer.h"

namespace Render
{

	RVertex::RVertex():
	m_invZ(1.0f),
	m_uiState(0),
	m_uiAttri( RVERT_ATTRI_SHADE_WIREFRAME),
	m_v4Color(0.0f,0.0f,0.0f,0.0f)
	{
	}

	RVertex::RVertex( const Vertex& v ):
	m_invZ(1.0f),
	m_uiState(0),
	m_uiAttri( RVERT_ATTRI_SHADE_WIREFRAME),
	m_v4Color(0.0f,0.0f,0.0f,0.0f)
	{
		this->CopyFromVertex( v );
	}

	RVertex::RVertex( const RVertex& rv ):
	m_invZ(1.0f),
	m_uiState(0),
	m_uiAttri( RVERT_ATTRI_SHADE_WIREFRAME),
	m_v4Color(0.0f,0.0f,0.0f,0.0f)
	{
		this->operator=( rv );
	}

	RVertex::~RVertex()
	{

	}

	void RVertex::CopyFromVertex( const Vertex& v )
	{
		m_v3Pos = v.m_pos;
		m_v3Normal = v.m_norm;
		m_v2Texcoord = v.m_texcoord;
		m_v3Tangent = v.m_tangent;
		m_v3Binormal = v.m_binormal;
		ClearStateBits();
	}

	RVertex& RVertex::operator=( const RVertex& rhs )
	{
		m_v3Pos = rhs.m_v3Pos;
		m_invZ = rhs.m_invZ;
		m_v3PosInCam = rhs.m_v3PosInCam;
		m_v3Normal = rhs.m_v3Normal;
		m_v2Texcoord = rhs.m_v2Texcoord;
		m_v3Tangent = rhs.m_v3Tangent;
		m_v3Binormal = rhs.m_v3Binormal;
		m_v4Color = rhs.m_v4Color;
		m_uiState = rhs.m_uiState;
		m_uiAttri = rhs.m_uiAttri;
		return *this;
	}

	bool RVertex::TestStateBit( const unsigned int bit )
	{
		return ( 0 != ( m_uiState & bit ) );
	}

 
	Render::RVertex RVertex::operator+( const RVertex& rhs ) const
	{
		RVertex newVert;
		newVert = *this;
		newVert += rhs;
		return newVert; 
	}

	Render::RVertex RVertex::operator-( const RVertex& rhs ) const
	{
		RVertex newVert;
		newVert = *this;
		newVert -= rhs;
		return newVert; 
	}

	Render::RVertex RVertex::operator*( const Real rhs ) const
	{
		RVertex newVert;
		newVert = *this;
		newVert *= rhs;
		return newVert; 
	}

	Render::RVertex RVertex::operator/( const Real rhs ) const
	{
		RVertex newVert;
		newVert = *this;
		newVert /= rhs;
		return newVert; 
	}

	RVertex& RVertex::operator+=( const RVertex& rhs )
	{
		m_v3Pos += rhs.m_v3Pos;
		m_invZ += rhs.m_invZ;
		 
		if( TestAttriBit( RVERT_ATTRI_SHADE_WITH_TEXTURE ) )
		{
			m_v2Texcoord += rhs.m_v2Texcoord;
		} 

		if( TestAttriBit( RVERT_ATTRI_SHADE_GOURAUD ) )
		{ 
			m_v4Color += rhs.m_v4Color;
		}

		if( TestAttriBit( RVERT_ATTRI_SHADE_PHONG ) ||
			TestAttriBit( RVERT_ATTRI_SHADE_NORMMAP ) )
		{
			m_v3Normal += rhs.m_v3Normal;
			m_v3PosInCam += rhs.m_v3PosInCam;
		}
		if( TestAttriBit( RVERT_ATTRI_SHADE_NORMMAP ) )
		{
			m_v3Tangent += rhs.m_v3Tangent;
			m_v3Binormal += rhs.m_v3Binormal;
		}

		return *this;
	}

	RVertex& RVertex::operator-=( const RVertex& rhs )
	{
		m_v3Pos -= rhs.m_v3Pos;
		m_invZ -= rhs.m_invZ;

		if( TestAttriBit( RVERT_ATTRI_SHADE_WITH_TEXTURE ) )
		{
			m_v2Texcoord -= rhs.m_v2Texcoord;
		}

		if( TestAttriBit( RVERT_ATTRI_SHADE_GOURAUD ) )
		{ 
			m_v4Color -= rhs.m_v4Color;
		}

		if( TestAttriBit( RVERT_ATTRI_SHADE_PHONG ) ||
			TestAttriBit( RVERT_ATTRI_SHADE_NORMMAP ) )
		{
			m_v3Normal -= rhs.m_v3Normal;
			m_v3PosInCam -= rhs.m_v3PosInCam;
		}
		if( TestAttriBit( RVERT_ATTRI_SHADE_NORMMAP ) )
		{
			m_v3Tangent -= rhs.m_v3Tangent;
			m_v3Binormal -= rhs.m_v3Binormal;
		}
		return *this;
	}

	RVertex& RVertex::operator*=( const Real rhs )
	{
		m_v3Pos *= rhs;
		m_invZ *= rhs;

		if( TestAttriBit( RVERT_ATTRI_SHADE_WITH_TEXTURE ) )
		{
			m_v2Texcoord *= rhs;
		}

		if( TestAttriBit( RVERT_ATTRI_SHADE_GOURAUD ) )
		{ 
			m_v4Color *= rhs;
		}

		if( TestAttriBit( RVERT_ATTRI_SHADE_PHONG ) ||
			TestAttriBit( RVERT_ATTRI_SHADE_NORMMAP ) )
		{
			m_v3Normal *= rhs;
			m_v3PosInCam *= rhs;
		}
		if( TestAttriBit( RVERT_ATTRI_SHADE_NORMMAP ) )
		{
			m_v3Tangent *= rhs;
			m_v3Binormal *= rhs;
		}
		return *this;
	}

	RVertex& RVertex::operator/=( const Real rhs )
	{
		m_v3Pos /= rhs;
		m_invZ /= rhs;

		if( TestAttriBit( RVERT_ATTRI_SHADE_WITH_TEXTURE ) )
		{
			m_v2Texcoord /= rhs;
		}

		if( TestAttriBit( RVERT_ATTRI_SHADE_GOURAUD ) )
		{ 
			m_v4Color /= rhs;
		}

		if( TestAttriBit( RVERT_ATTRI_SHADE_PHONG ) ||
			TestAttriBit( RVERT_ATTRI_SHADE_NORMMAP ) )
		{
			m_v3Normal /= rhs;
			m_v3PosInCam /= rhs;
		}
		if( TestAttriBit( RVERT_ATTRI_SHADE_NORMMAP ) )
		{
			m_v3Tangent /= rhs;
			m_v3Binormal /= rhs;
		}
		return *this;
	}


	Render::RVertex operator*( const Real lhs , const RVertex& rhs )
	{
		RVertex newVert = rhs;
		newVert *= lhs; 
		return newVert;
	}


	RFace::RFace(): 
	m_uiState(0),
	m_pNext(NULL)
	{
		m_v4Color.Set( 0.0f , 0.0f , 0.0f , 0.0f );
		memset( m_uiIndices , 0 , sizeof(m_uiIndices[0])*3 );
	}

	RFace::RFace( const RFace& face )
	{
		this->operator=( face );
	}

	RFace::~RFace()
	{

	}

	RFace& RFace::operator=( const RFace& rhs )
	{
		for( int iIndx = 0 ; iIndx < 3 ; iIndx++ )
		{
			m_uiIndices[iIndx] = rhs.m_uiIndices[iIndx];
		}
		m_uiState = rhs.m_uiState;
		m_v3Normal = rhs.m_v3Normal;
		m_v4Color = rhs.m_v4Color;
		return *this;
	}
	 


	void RFace::SetStateBit( const unsigned int bit )
	{
		m_uiState |= bit;
	}

	RenderList::RenderList(void):
	m_uiVertCount(0),
	m_uiFaceCount(0),
	m_pRVerts(NULL),
	m_uiTransVertCount(0),
	m_uiTransVertsCapacity(0),
	m_pRTransVerts(NULL),
	m_pFace(NULL)
	{
	}
	 
	RenderList::~RenderList(void)
	{
	} 

	void RenderList::Clear( void )
	{
		//�������еķ��䶼��֡�������������Բ���ʵ���ͷ��ڴ�
		m_uiVertCount = 0;
		m_uiFaceCount = 0;
		m_pRVerts = NULL;
		m_uiTransVertCount = 0;
		m_uiTransVertsCapacity = 0;
		m_pRTransVerts = NULL;
		m_pFace = NULL;
	}

	void RenderList::AddFace( const RFace& face )
	{
		void* pBuf = FrameStackAllocator::GetInstance()->Alloc( sizeof(RFace) );
		ZP_ASSERT( NULL != pBuf );
		RFace* pNewFace = new (pBuf) RFace( face );
		pNewFace->m_pNext = m_pFace;
		m_pFace = pNewFace;
		m_uiFaceCount++;
	}

	void RenderList::CopyFromRenderPrimitive( RenderPrimitive& primitive )
	{
		this->Clear();

		VertexBuffer& vertexBufRef = primitive.VertexBuf();
		//ͨ����������������
		unsigned int uiFaceCount = primitive.IndicesCount() / 3;
		m_uiVertCount = vertexBufRef.Count();
		m_uiTransVertCount = m_uiVertCount;

		//Ϊ�����������ⶥ��ռ䣨����ÿ���涼�ᱻƽ���ȡ���������µĶ��㣩
		m_uiTransVertsCapacity = m_uiVertCount + uiFaceCount*2;

		//��֡ջ�������Ϸ��� �����ͷ��ڴ�
		void* pVertBuf = FrameStackAllocator::GetInstance()->Alloc( sizeof(RVertex)*m_uiVertCount );
		void* pTransVertBuf = FrameStackAllocator::GetInstance()->Alloc( sizeof(RVertex)*m_uiTransVertsCapacity );

		ZP_ASSERT( NULL != pVertBuf );
		ZP_ASSERT( NULL != pTransVertBuf );

		m_pRVerts = (RVertex*)pVertBuf; 
		//�����ж������ placement new
		//����ԭʼ����
		for( unsigned int uiVert = 0 ; uiVert < m_uiVertCount ; uiVert++ )
		{
			 new ( (void*)(&m_pRVerts[uiVert] ) ) RVertex( vertexBufRef.Pointer()[uiVert] );
		}

		
		m_pRTransVerts = (RVertex*)pTransVertBuf;
		for( unsigned int uiVert = 0 ; uiVert < m_uiVertCount ; uiVert++ )
		{
			new ( (void*)(&m_pRTransVerts[uiVert] ) ) RVertex( vertexBufRef.Pointer()[uiVert] );
		}

		//��ʼ�����������ĺ󱸿ռ�
		for( unsigned int uiVert = m_uiVertCount ; uiVert < m_uiTransVertsCapacity - m_uiVertCount ; uiVert++ )
		{
			new ( (void*)(&m_pRTransVerts[uiVert] ) ) RVertex();
		}

		for( unsigned int uiFace = 0 ; uiFace < uiFaceCount ; uiFace++ )
		{
			RFace newFace;
			unsigned int uiOffset = uiFace*3;
			newFace.m_uiIndices[0] = primitive.Indices()[ uiOffset ];
			newFace.m_uiIndices[1] = primitive.Indices()[ uiOffset + 1 ];
			newFace.m_uiIndices[2] = primitive.Indices()[ uiOffset + 2 ];

			//�������б�
			this->AddFace( newFace );
		}

	}

	unsigned int RenderList::AddTransVert( const RVertex& vert )
	{
		ZP_ASSERT( m_uiTransVertCount < m_uiTransVertsCapacity );
		m_pRTransVerts[ m_uiTransVertCount ] = vert;
		return m_uiTransVertCount++;
	}


}//namespace Render