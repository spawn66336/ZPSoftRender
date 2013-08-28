#include "RenderList.h"
#include "FrameStackAllocator.h"
#include "RenderPrimitive.h"
#include "Vertex.h"
#include "FrameBuffer.h"

namespace Render
{

	RVertex::RVertex():
	m_uiState(0),
	m_v4Color(0.0f,0.0f,0.0f,0.0f)
	{
	}

	RVertex::RVertex( const Vertex& v ):
	m_uiState(0),
	m_v4Color(0.0f,0.0f,0.0f,0.0f)
	{
		this->CopyFromVertex( v );
	}

	RVertex::RVertex( const RVertex& rv ):
	m_uiState(0),
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
		m_v3Normal = rhs.m_v3Normal;
		m_v2Texcoord = rhs.m_v2Texcoord;
		m_v3Tangent = rhs.m_v3Tangent;
		m_v3Binormal = rhs.m_v3Binormal;
		m_v4Color = rhs.m_v4Color;
		m_uiState = rhs.m_uiState;
		return *this;
	}

	bool RVertex::TestStateBit( const unsigned int bit )
	{
		return ( 0 != ( m_uiState & bit ) );
	}

	void RVertex::SetStateBit( const unsigned int bit )
	{
		m_uiState |= bit;
	}

	Render::RVertex RVertex::operator+( const RVertex& rhs ) const
	{
		RVertex newVert;
		newVert.m_v3Pos = m_v3Pos + rhs.m_v3Pos;
		newVert.m_v3Normal = m_v3Normal + rhs.m_v3Normal;
		newVert.m_v2Texcoord = m_v2Texcoord + rhs.m_v2Texcoord;
		newVert.m_v3Tangent = m_v3Tangent + rhs.m_v3Tangent;
		newVert.m_v3Binormal = m_v3Binormal + rhs.m_v3Binormal;
		newVert.m_v4Color = m_v4Color + rhs.m_v4Color;
		return newVert;
	}

	Render::RVertex RVertex::operator-( const RVertex& rhs ) const
	{
		RVertex newVert;
		newVert.m_v3Pos = m_v3Pos - rhs.m_v3Pos;
		newVert.m_v3Normal = m_v3Normal - rhs.m_v3Normal;
		newVert.m_v2Texcoord = m_v2Texcoord - rhs.m_v2Texcoord;
		newVert.m_v3Tangent = m_v3Tangent - rhs.m_v3Tangent;
		newVert.m_v3Binormal = m_v3Binormal - rhs.m_v3Binormal;
		newVert.m_v4Color = m_v4Color - rhs.m_v4Color;
		return newVert;
	}

	Render::RVertex RVertex::operator*( const Real rhs ) const
	{
		RVertex newVert;
		newVert.m_v3Pos = m_v3Pos * rhs;
		newVert.m_v3Normal = m_v3Normal * rhs;
		newVert.m_v2Texcoord = m_v2Texcoord * rhs;
		newVert.m_v3Tangent = m_v3Tangent * rhs;
		newVert.m_v3Binormal = m_v3Binormal * rhs;
		newVert.m_v4Color = m_v4Color * rhs;
		return newVert;
	}

	Render::RVertex RVertex::operator/( const Real rhs ) const
	{
		RVertex newVert;
		newVert.m_v3Pos = m_v3Pos / rhs;
		newVert.m_v3Normal = m_v3Normal / rhs;
		newVert.m_v2Texcoord = m_v2Texcoord  / rhs;
		newVert.m_v3Tangent = m_v3Tangent / rhs;
		newVert.m_v3Binormal = m_v3Binormal / rhs;
		newVert.m_v4Color = m_v4Color / rhs;
		return newVert;
	}

	RVertex& RVertex::operator+=( const RVertex& rhs )
	{
		m_v3Pos += rhs.m_v3Pos;
		m_v3Normal += rhs.m_v3Normal;
		m_v2Texcoord += rhs.m_v2Texcoord;
		m_v3Tangent += rhs.m_v3Tangent;
		m_v3Binormal += rhs.m_v3Binormal;
		m_v4Color += rhs.m_v4Color;
		return *this;
	}

	RVertex& RVertex::operator-=( const RVertex& rhs )
	{
		m_v3Pos -= rhs.m_v3Pos;
		m_v3Normal -= rhs.m_v3Normal;
		m_v2Texcoord  -= rhs.m_v2Texcoord;
		m_v3Tangent -= rhs.m_v3Tangent;
		m_v3Binormal -= rhs.m_v3Binormal;
		m_v4Color -= rhs.m_v4Color;
		return *this;
	}

	RVertex& RVertex::operator*=( const Real rhs )
	{
		m_v3Pos *= rhs;
		m_v3Normal *= rhs;
		m_v2Texcoord  *= rhs;
		m_v3Tangent *= rhs;
		m_v3Binormal *= rhs;
		m_v4Color *= rhs;
		return *this;
	}

	RVertex& RVertex::operator/=( const Real rhs )
	{
		m_v3Pos /= rhs;
		m_v3Normal /= rhs;
		m_v2Texcoord  /= rhs;
		m_v3Tangent /= rhs;
		m_v3Binormal /= rhs;
		m_v4Color /= rhs;
		return *this;
	}


	Render::RVertex operator*( const Real lhs , const RVertex& rhs )
	{
		RVertex newVert;
		newVert.m_v3Pos = rhs.m_v3Pos * lhs;
		newVert.m_v3Normal = rhs.m_v3Normal * lhs;
		newVert.m_v2Texcoord = rhs.m_v2Texcoord * lhs;
		newVert.m_v3Tangent = rhs.m_v3Tangent * lhs;
		newVert.m_v3Binormal = rhs.m_v3Binormal * lhs;
		newVert.m_v4Color = rhs.m_v4Color * lhs;
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

 
	bool RFace::TestStateBit( const unsigned int bit )
	{
		return ( 0 != ( m_uiState&bit) );
	}

	void RFace::SetStateBit( const unsigned int bit )
	{
		m_uiState |= bit;
	}

	RenderList::RenderList(void):
	m_uiVertCount(0),
	m_uiFaceCount(0),
	m_pRVerts(NULL),
	m_pRTransVerts(NULL),
	m_pFace(NULL)
	{
	}
	 
	RenderList::~RenderList(void)
	{
	} 

	void RenderList::Clear( void )
	{
		//由于所有的分配都是帧分配器分配所以不用实际释放内存
		m_uiVertCount = 0;
		m_uiFaceCount = 0;
		m_pRVerts = NULL;
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
		m_uiVertCount = vertexBufRef.Count();

		//在帧栈分配器上分配 无需释放内存
		void* pVertBuf = FrameStackAllocator::GetInstance()->Alloc( sizeof(RVertex)*m_uiVertCount );
		void* pTransVertBuf = FrameStackAllocator::GetInstance()->Alloc( sizeof(RVertex)*m_uiVertCount );

		ZP_ASSERT( NULL != pVertBuf );
		ZP_ASSERT( NULL != pTransVertBuf );

		m_pRVerts = (RVertex*)pVertBuf; 
		//对所有顶点调用 placement new
		for( unsigned int uiVert = 0 ; uiVert < m_uiVertCount ; uiVert++ )
		{
			 new ( (void*)(&m_pRVerts[uiVert] ) ) RVertex( vertexBufRef.Pointer()[uiVert] );
		}

		m_pRTransVerts = (RVertex*)pTransVertBuf;
		for( unsigned int uiVert = 0 ; uiVert < m_uiVertCount ; uiVert++ )
		{
			new ( (void*)(&m_pRTransVerts[uiVert] ) ) RVertex( vertexBufRef.Pointer()[uiVert] );
		}


		//通过索引数计算顶点
		unsigned int uiFaceCount = primitive.IndicesCount() / 3;
		for( unsigned int uiFace = 0 ; uiFace < uiFaceCount ; uiFace++ )
		{
			RFace newFace;
			unsigned int uiOffset = uiFace*3;
			newFace.m_uiIndices[0] = primitive.Indices()[ uiOffset ];
			newFace.m_uiIndices[1] = primitive.Indices()[ uiOffset + 1 ];
			newFace.m_uiIndices[2] = primitive.Indices()[ uiOffset + 2 ];

			//加入面列表
			this->AddFace( newFace );
		}

	}


}//namespace Render