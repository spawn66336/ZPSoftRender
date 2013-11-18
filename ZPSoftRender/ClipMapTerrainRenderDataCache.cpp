#include "ClipMapTerrainRenderDataCache.h"
#include "ClipMapLevel.h"
#include "D3DRenderOperation.h"

namespace Terrain
{


	ClipMapTerrainRenderDataCache::ClipMapTerrainRenderDataCache(void):
	m_pDevice(NULL)
	{
	}


	ClipMapTerrainRenderDataCache::~ClipMapTerrainRenderDataCache(void)
	{
	}

	void ClipMapTerrainRenderDataCache::OnLostDevice( void )
	{
		Destroy();
	}

	void ClipMapTerrainRenderDataCache::OnResetDevice( void )
	{ 
	}

	void ClipMapTerrainRenderDataCache::UpdateTerrainLevelRenderData( ClipMapLevel* pLevel )
	{
		TerrainLevelRenderData* pRenderData = NULL;
		auto itFind = m_renderDataTab.find( pLevel );
		if( itFind == m_renderDataTab.end() )
		{
			pRenderData = new TerrainLevelRenderData( m_pDevice );
			m_renderDataTab.insert( std::make_pair( pLevel , pRenderData) );
		}else{
			pRenderData = itFind->second;
		}
		pRenderData->Update( pLevel );
	}

	void ClipMapTerrainRenderDataCache::Destroy( void )
	{
		auto itRenderData = m_renderDataTab.begin();
		while( itRenderData != m_renderDataTab.end() )
		{
			ZP_SAFE_DELETE( itRenderData->second );
			++itRenderData;
		}
		m_renderDataTab.clear();
	}

	void ClipMapTerrainRenderDataCache::Init( LPDIRECT3DDEVICE9 pDevice )
	{
		m_pDevice = pDevice;
	}

	void ClipMapTerrainRenderDataCache::GetRenderOps( std::vector<Render::D3DRenderOperation*>& opList )
	{
		auto itRenderData = m_renderDataTab.begin();
		while( itRenderData != m_renderDataTab.end() )
		{
			itRenderData->second->GetRenderOps( opList );
			++itRenderData;
		}
	}


	TerrainLevelRenderData::TerrainLevelRenderData():
	m_pDevice(0),
	m_pVBDecl(0),
	m_pVB(0),
	m_uiVertNum(0),
	m_uiIndiceNumPerTile(0),
	m_uiPrimtivePerTile(0)
	{
		ZP_ASSERT(0);
		memset( m_ppTileIB , 0 , sizeof(m_ppTileIB[0])*12 );
	}

	TerrainLevelRenderData::TerrainLevelRenderData( LPDIRECT3DDEVICE9 pDevice ):
	m_pDevice(pDevice),
	m_pVBDecl(0),
	m_pVB(0),
	m_uiVertNum(0),
	m_uiIndiceNumPerTile(0),
	m_uiPrimtivePerTile(0)
	{
		memset( m_ppTileIB , 0 , sizeof(m_ppTileIB[0])*12 );

		//分配12个子地形块的RenderOp
		for( int i = 0 ; i < 12 ; i++ )
		{
			m_tileRenderOps[i] = new Render::D3DRenderOperation;
		}
	} 

	TerrainLevelRenderData::~TerrainLevelRenderData()
	{ 
		ZP_SAFE_RELEASE( m_pVBDecl );
		ZP_SAFE_RELEASE( m_pVB );
		for( int i = 0 ; i < 12 ; i++ )
		{
			ZP_SAFE_RELEASE( m_ppTileIB[i] );
		}

		for( int i = 0 ; i < 12 ; i++ )
		{
			ZP_SAFE_DELETE( m_tileRenderOps[i] );
		}
	}

	void TerrainLevelRenderData::Update( ClipMapLevel* pLevel )
	{ 
		if( NULL == m_pVBDecl )
		{
			D3DVERTEXELEMENT9 declElems[] ={
				{ 0 , 0 , D3DDECLTYPE_FLOAT3 , D3DDECLMETHOD_DEFAULT , D3DDECLUSAGE_POSITION , 0 },
				{ 0 , sizeof(float)*3 , D3DDECLTYPE_FLOAT3 , D3DDECLMETHOD_DEFAULT , D3DDECLUSAGE_NORMAL , 0 },
				{ 0 , sizeof(float)*6 , D3DDECLTYPE_FLOAT4  , D3DDECLMETHOD_DEFAULT , D3DDECLUSAGE_COLOR , 0 }, 
				D3DDECL_END()
			}; 
			m_pDevice->CreateVertexDeclaration( declElems , &m_pVBDecl );

			for( int i = 0 ; i < 12 ; i++ )
			{
				m_pVBDecl->AddRef();
				ZP_SAFE_RELEASE( m_tileRenderOps[i]->m_pVertexDecl );
				m_tileRenderOps[i]->m_pVertexDecl = m_pVBDecl;
			}
		}

		 //检查VB是否需要更新
		bool bVBNeedUpdate = false; 
		if( pLevel->GetVertsNum() != m_uiVertNum )
		{
			bVBNeedUpdate = true;
			m_uiVertNum = pLevel->GetVertsNum();
			ZP_SAFE_RELEASE( m_pVB );
			m_pDevice->CreateVertexBuffer( m_uiVertNum*sizeof(TerrainVertex) , D3DUSAGE_DYNAMIC , 0 , D3DPOOL_DEFAULT , &m_pVB , NULL );
			ZP_ASSERT( NULL != m_pVB );

			//更新12个地形块的渲染操作的顶点缓冲区
			for( int i = 0 ; i < 12 ; i++ )
			{
				m_pVB->AddRef();
				ZP_SAFE_RELEASE( m_tileRenderOps[i]->m_pVB );
				m_tileRenderOps[i]->m_pVB = m_pVB;
			}

		}else{
			if( pLevel->TestFlag( VERTS_CHANGE ) )
			{
				bVBNeedUpdate = true;
			}
		}
 
		if( bVBNeedUpdate )
		{
			void* pBuf = NULL;
			unsigned int uiLockSize = m_uiVertNum*sizeof(TerrainVertex) ;
			m_pVB->Lock( 0 , uiLockSize , &pBuf , D3DLOCK_DISCARD );
				memcpy_s( pBuf , uiLockSize , pLevel->GetVerts() , uiLockSize );
			m_pVB->Unlock();
			pBuf = NULL;
		}

		//检查IB是否需要更新
		bool bIBNeedUpdate = false;
		if( pLevel->GetIndicesNumPerTile() != m_uiIndiceNumPerTile )
		{
			bIBNeedUpdate = true;
			m_uiIndiceNumPerTile = pLevel->GetIndicesNumPerTile();
			for( int i = 0 ; i < 12 ; i++ )
			{
				ZP_SAFE_RELEASE( m_ppTileIB[i] );

				m_pDevice->CreateIndexBuffer( 
					sizeof(unsigned short)*m_uiIndiceNumPerTile , D3DUSAGE_WRITEONLY , D3DFMT_INDEX16 , D3DPOOL_DEFAULT , &m_ppTileIB[i] , NULL );
				ZP_ASSERT( NULL != m_ppTileIB[i] );

				//更新12个地形块的渲染操作的索引缓冲区
				m_ppTileIB[i]->AddRef();
				ZP_SAFE_RELEASE( m_tileRenderOps[i]->m_pIB );
				m_tileRenderOps[i]->m_pIB = m_ppTileIB[i];
			}
		}

		if( bIBNeedUpdate )
		{
			unsigned short** ppIndices = pLevel->GetTilesIndices();
			for( int i = 0 ; i < 12 ; i++ )
			{
				void* pBuf = NULL;
				unsigned int uiLockSize = sizeof(unsigned short)*m_uiIndiceNumPerTile ;
				m_ppTileIB[i]->Lock( 0 ,uiLockSize, &pBuf , 0 );
				memcpy_s( pBuf ,uiLockSize, ppIndices[i] ,uiLockSize );
				m_ppTileIB[i]->Unlock();
			}
		}

		m_uiPrimtivePerTile = pLevel->GetPrimtiveNumPerTile();
	}

	void TerrainLevelRenderData::GetRenderOps( std::vector<Render::D3DRenderOperation*>& opList )
	{
		Math::Matrix4 localMat = Math::Matrix4::MakeTranslationMatrix( m_v3LocalPos );
		for( int i = 0 ; i < 12 ; i++ )
		{
			m_tileRenderOps[i]->m_worldMat = localMat;
			m_tileRenderOps[i]->m_vertexCount = m_uiVertNum;
			m_tileRenderOps[i]->m_stride = sizeof( TerrainVertex );
			m_tileRenderOps[i]->m_primitiveType = D3DPT_TRIANGLESTRIP;
			m_tileRenderOps[i]->m_primCount = m_uiPrimtivePerTile;
			m_tileRenderOps[i]->m_streamIndex = 0;
			opList.push_back( m_tileRenderOps[i] );
		}
	}

}