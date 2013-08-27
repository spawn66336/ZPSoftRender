#include "RenderList.h"
#include "FrameStackAllocator.h"
#include "RenderPrimitive.h"
#include "Vertex.h"
#include "FrameBuffer.h"

namespace Render
{

	RVertex::RVertex():
	m_uiState(0)
	{

	}

	RVertex::RVertex( const Vertex& v )
	{
		this->CopyFromVertex( v );
	}

	RVertex::RVertex( const RVertex& rv )
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

	RFace::RFace(): 
	m_uiState(0),
	m_pNext(NULL)
	{
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
			new ( (void*)(&m_pRTransVerts[uiVert] ) ) RVertex();
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

	void RenderList::TransformFromLocalSpaceToCameraSpace( const Math::Matrix4& localToCam )
	{
		for( unsigned int uiVert = 0 ; uiVert < m_uiVertCount ; uiVert++ )
		{
			Math::Vec4 v4Pos( m_pRVerts[uiVert].m_v3Pos );
			v4Pos = v4Pos * localToCam;
			m_pRTransVerts[uiVert].m_v3Pos = Math::Vec3( v4Pos.x , v4Pos.y , v4Pos.z );
		}
	}

	void RenderList::RemoveBackFaceInCameraSpace( void )
	{
		//遍历面列表剔除背向面
		RFace* pFace = m_pFace;
		while( NULL != pFace )
		{
			unsigned int uiVertIndx0 = pFace->m_uiIndices[0];
			unsigned int uiVertIndx1 = pFace->m_uiIndices[1];
			unsigned int uiVertIndx2 = pFace->m_uiIndices[2];

			Math::Vec3 v3U =m_pRTransVerts[uiVertIndx2].m_v3Pos - m_pRTransVerts[uiVertIndx0].m_v3Pos;
			Math::Vec3 v3V = m_pRTransVerts[uiVertIndx1].m_v3Pos - m_pRTransVerts[uiVertIndx0].m_v3Pos;

			//求出面法线
			Math::Vec3 v3N = v3U.CrossProduct( v3V );
			
			//若为背面
			if( v3N.DotProduct( -m_pRTransVerts[uiVertIndx1].m_v3Pos ) <= 0.0f  )
			{
				pFace->SetStateBit( RFACE_STATE_BACKFACE );
			}else{
				m_pRTransVerts[uiVertIndx0].SetStateBit( RVERT_STATE_ACTIVE );
				m_pRTransVerts[uiVertIndx1].SetStateBit( RVERT_STATE_ACTIVE );
				m_pRTransVerts[uiVertIndx2].SetStateBit( RVERT_STATE_ACTIVE );
			}

			pFace = pFace->m_pNext;
		}
	}

	void RenderList::TransformFromCameraSpaceToProjectionSpace( const Math::Matrix4& camToProj )
	{
		for( unsigned int uiVert = 0 ; uiVert < m_uiVertCount ; uiVert++ )
		{
			//若当前顶点已被剔除则变换下一个顶点
			if( !m_pRTransVerts[uiVert].TestStateBit( RVERT_STATE_ACTIVE ) )
			{
				continue;
			}

			Math::Vec4 v4Pos( m_pRTransVerts[uiVert].m_v3Pos );
			v4Pos = v4Pos * camToProj;
			m_pRTransVerts[uiVert].m_v3Pos = Math::Vec3( v4Pos.x , v4Pos.y , v4Pos.z );

			Real invW = 1.0f / v4Pos.w;

			m_pRTransVerts[uiVert].m_v3Pos.x *= invW;
			m_pRTransVerts[uiVert].m_v3Pos.y *= invW;
		}
	}

	void RenderList::TransformFromProjectionSpaceToScreenSpace( const Math::Matrix4& projToScreen )
	{
		for( unsigned int uiVert = 0 ; uiVert < m_uiVertCount ; uiVert++ )
		{
			//若当前顶点已被剔除则变换下一个顶点
			if( !m_pRTransVerts[uiVert].TestStateBit( RVERT_STATE_ACTIVE ) )
			{
				continue;
			}

			Math::Vec4 v4Pos( m_pRTransVerts[uiVert].m_v3Pos );
			v4Pos = v4Pos * projToScreen;
			m_pRTransVerts[uiVert].m_v3Pos = Math::Vec3( v4Pos.x , v4Pos.y , v4Pos.z ); 
		}
	}

	void RenderList::DrawFacesWireFrameToFrameBuffer(  FrameBuffer& buf )
	{
		RFace* pFace = m_pFace;
		Math::BGRA8888_t lineColor = 
			Math::MathUtil::ColorVecToRGBA8888( Math::Vec4( 1.0f , 1.0f , 1.0f , 1.0f ) );

		while( NULL != pFace )
		{
			if( pFace->TestStateBit( RFACE_STATE_BACKFACE ) )
			{
				pFace = pFace->m_pNext;
				continue;
			}

			unsigned int uiVertIndices[3];  
			uiVertIndices[0] = pFace->m_uiIndices[0];
			uiVertIndices[1] = pFace->m_uiIndices[1];
			uiVertIndices[2] = pFace->m_uiIndices[2];

			for( unsigned int uiLine = 0 ; uiLine < 3 ; uiLine++ )
			{
				unsigned int uiStartIndx = uiVertIndices[ uiLine ];
				unsigned int uiEndIndx = uiVertIndices[ (uiLine+1)%3 ];
				 Math::Vec2 v2P0( m_pRTransVerts[uiStartIndx].m_v3Pos.x , m_pRTransVerts[uiStartIndx].m_v3Pos.y );
				 Math::Vec2 v2P1( m_pRTransVerts[uiEndIndx].m_v3Pos.x , m_pRTransVerts[uiEndIndx].m_v3Pos.y );
				 buf.DrawLineMidPoint( v2P0 , v2P1 , static_cast<void*>( &lineColor ) );
			} 
			pFace = pFace->m_pNext;
		}
	} 

	void RenderList::DrawFacesSolidTrianglesToFrameBuffer( FrameBuffer& buf )
	{
		RFace* pFace = m_pFace;
		Math::BGRA8888_t faceColor = 
			Math::MathUtil::ColorVecToRGBA8888( Math::Vec4( 0.75f , 0.75f , 0.75f , 1.0f ) );

		while( NULL != pFace )
		{
			if( pFace->TestStateBit( RFACE_STATE_BACKFACE ) )
			{
				pFace = pFace->m_pNext;
				continue;
			}

			unsigned int uiVertIndices[3];  
			uiVertIndices[0] = pFace->m_uiIndices[0];
			uiVertIndices[1] = pFace->m_uiIndices[1];
			uiVertIndices[2] = pFace->m_uiIndices[2];
			  
			Math::Vec2 v2P0( m_pRTransVerts[uiVertIndices[0]].m_v3Pos.x , m_pRTransVerts[uiVertIndices[0]].m_v3Pos.y );
			Math::Vec2 v2P1( m_pRTransVerts[uiVertIndices[1]].m_v3Pos.x , m_pRTransVerts[uiVertIndices[1]].m_v3Pos.y );
			Math::Vec2 v2P2( m_pRTransVerts[uiVertIndices[2]].m_v3Pos.x , m_pRTransVerts[uiVertIndices[2]].m_v3Pos.y ); 
			buf.DrawTriangle2DSolid( v2P0 , v2P1 , v2P2 ,  static_cast<void*>( &faceColor ) );

			pFace = pFace->m_pNext;
		}
	}




}//namespace Render