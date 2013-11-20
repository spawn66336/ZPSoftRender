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
	m_uiLevelCurrFlag(0),
	m_pVBDecl(0),
	m_pVB(0),
	m_pGapTileIB(0),
	m_pCenterTileIB(0),
	m_pLeftTopLTileIB(0),		 
	m_pLeftBottomLTileIB(0),		 
	m_pRightTopLTileIB(0),
	m_pRightBottomLTileIB(0),		 
	m_pGapTileOp(0),
	m_pCenterTileOp(0),
	m_pLeftTopLTileOp(0),
	m_pLeftBottomLTileOp(0),
	m_pRightTopLTileOp(0),
	m_pRightBottomLTileOp(0),
	m_uiVertNum(0),
	m_uiIndiceNumPerTile(0),
	m_uiPrimtivePerTile(0),
	m_uiGapTileIndiceNum(0),
	m_uiGapTilePrimitiveNum(0),
	m_uiCenterTileIndiceNum(0),
	m_uiCenterTilePrimitiveNum(0),
	m_uiIndiceNumPerLTile(0),
	m_uiPrimitivePerLTile(0),
	m_pFixedRingVB(0),
	m_pFixedRingIB(0),
	m_pFixedRingOp(0),
	m_uiFixedRingVertNum(0),
	m_uiFixedRingIndicesNum(0),
	m_uiFixedRingPrimitiveNum(0)
	{
		ZP_ASSERT(0);
		memset( m_ppTileIB , 0 , sizeof(m_ppTileIB[0])*12 );
	}

	TerrainLevelRenderData::TerrainLevelRenderData( LPDIRECT3DDEVICE9 pDevice ):
	m_pDevice(pDevice),
	m_uiLevelCurrFlag(0),
	m_pVBDecl(0),
	m_pVB(0),
	m_pGapTileIB(0),
	m_pCenterTileIB(0),
	m_pLeftTopLTileIB(0),		 
	m_pLeftBottomLTileIB(0),		 
	m_pRightTopLTileIB(0),
	m_pRightBottomLTileIB(0),		 
	m_pGapTileOp(0),
	m_pCenterTileOp(0),
	m_pLeftTopLTileOp(0),
	m_pLeftBottomLTileOp(0),
	m_pRightTopLTileOp(0),
	m_pRightBottomLTileOp(0),
	m_uiVertNum(0),
	m_uiIndiceNumPerTile(0),
	m_uiPrimtivePerTile(0),
	m_uiGapTileIndiceNum(0),
	m_uiGapTilePrimitiveNum(0),
	m_uiCenterTileIndiceNum(0),
	m_uiCenterTilePrimitiveNum(0),
	m_uiIndiceNumPerLTile(0),
	m_uiPrimitivePerLTile(0),
	m_pFixedRingVB(0),
	m_pFixedRingIB(0),
	m_pFixedRingOp(0),
	m_uiFixedRingVertNum(0),
	m_uiFixedRingIndicesNum(0),
	m_uiFixedRingPrimitiveNum(0)
	{
		memset( m_ppTileIB , 0 , sizeof(m_ppTileIB[0])*12 );

		//分配12个子地形块的RenderOp
		for( int i = 0 ; i < 12 ; i++ )
		{
			m_tileRenderOps[i] = new Render::D3DRenderOperation;
		}

		m_pGapTileOp = new Render::D3DRenderOperation;
		m_pCenterTileOp = new Render::D3DRenderOperation;

		m_pLeftTopLTileOp = new Render::D3DRenderOperation;
		m_pLeftBottomLTileOp = new Render::D3DRenderOperation;
		m_pRightTopLTileOp = new Render::D3DRenderOperation;
		m_pRightBottomLTileOp = new Render::D3DRenderOperation;

		m_pFixedRingOp = new Render::D3DRenderOperation;
	} 

	TerrainLevelRenderData::~TerrainLevelRenderData()
	{ 
		ZP_SAFE_RELEASE( m_pVBDecl );
		ZP_SAFE_RELEASE( m_pVB );

		//释放12个Tile的索引缓冲区与渲染操作
		for( int i = 0 ; i < 12 ; i++ )
		{
			ZP_SAFE_RELEASE( m_ppTileIB[i] );
		} 
		for( int i = 0 ; i < 12 ; i++ )
		{
			ZP_SAFE_DELETE( m_tileRenderOps[i] );
		}

		//释放上下左右四个修补条的索引缓冲区与渲染操作
		ZP_SAFE_RELEASE( m_pGapTileIB );
		ZP_SAFE_DELETE( m_pGapTileOp );

		//释放中心区块的索引缓冲区与渲染操作
		ZP_SAFE_RELEASE( m_pCenterTileIB );
		ZP_SAFE_DELETE( m_pCenterTileOp );

		ZP_SAFE_RELEASE( m_pLeftTopLTileIB);		 
		ZP_SAFE_RELEASE( m_pLeftBottomLTileIB );	 
		ZP_SAFE_RELEASE( m_pRightTopLTileIB );
		ZP_SAFE_RELEASE( m_pRightBottomLTileIB );		 

		ZP_SAFE_DELETE( m_pLeftTopLTileOp );
		ZP_SAFE_DELETE(m_pLeftBottomLTileOp);
		ZP_SAFE_DELETE(m_pRightTopLTileOp);
		ZP_SAFE_DELETE(m_pRightBottomLTileOp);

		ZP_SAFE_RELEASE( m_pFixedRingVB );
		ZP_SAFE_RELEASE( m_pFixedRingIB );
		ZP_SAFE_DELETE( m_pFixedRingOp );
	}

	void TerrainLevelRenderData::Update( ClipMapLevel* pLevel )
	{ 
		m_v3LocalPos = pLevel->GetLocalPos();
		m_uiLevelCurrFlag = pLevel->GetFlag();

		if( NULL == m_pVBDecl )
		{
			D3DVERTEXELEMENT9 declElems[] ={
				{ 0 , 0 , D3DDECLTYPE_FLOAT3 , D3DDECLMETHOD_DEFAULT , D3DDECLUSAGE_POSITION , 0 },
				{ 0 , sizeof(float)*3 , D3DDECLTYPE_FLOAT3 , D3DDECLMETHOD_DEFAULT , D3DDECLUSAGE_NORMAL , 0 },
				{ 0 , sizeof(float)*6 , D3DDECLTYPE_D3DCOLOR  , D3DDECLMETHOD_DEFAULT , D3DDECLUSAGE_COLOR , 0 }, 
				D3DDECL_END()
			}; 
			m_pDevice->CreateVertexDeclaration( declElems , &m_pVBDecl );

			for( int i = 0 ; i < 12 ; i++ )
			{
				m_pVBDecl->AddRef();
				ZP_SAFE_RELEASE( m_tileRenderOps[i]->m_pVertexDecl );
				m_tileRenderOps[i]->m_pVertexDecl = m_pVBDecl;
			}

			m_pVBDecl->AddRef();
			ZP_SAFE_RELEASE( m_pGapTileOp->m_pVertexDecl );
			m_pGapTileOp->m_pVertexDecl = m_pVBDecl;

			m_pVBDecl->AddRef();
			ZP_SAFE_RELEASE( m_pCenterTileOp->m_pVertexDecl );
			m_pCenterTileOp->m_pVertexDecl = m_pVBDecl;

			m_pVBDecl->AddRef();
			m_pVBDecl->AddRef();
			m_pVBDecl->AddRef();
			m_pVBDecl->AddRef();

			ZP_SAFE_RELEASE( m_pLeftTopLTileOp->m_pVertexDecl );
			ZP_SAFE_RELEASE( m_pLeftBottomLTileOp->m_pVertexDecl );
			ZP_SAFE_RELEASE( m_pRightTopLTileOp->m_pVertexDecl );
			ZP_SAFE_RELEASE( m_pRightBottomLTileOp->m_pVertexDecl );

			m_pLeftTopLTileOp->m_pVertexDecl = m_pVBDecl;
			m_pLeftBottomLTileOp->m_pVertexDecl = m_pVBDecl;
			m_pRightTopLTileOp->m_pVertexDecl = m_pVBDecl;
			m_pRightBottomLTileOp->m_pVertexDecl = m_pVBDecl;

			m_pVBDecl->AddRef();
			ZP_SAFE_RELEASE( m_pFixedRingOp->m_pVertexDecl );
			m_pFixedRingOp->m_pVertexDecl = m_pVBDecl;
		}

		 //检查VB是否需要更新
		bool bVBNeedUpdate = false; 
		if( pLevel->GetVertsNum() != m_uiVertNum )
		{
			bVBNeedUpdate = true;
			m_uiVertNum = pLevel->GetVertsNum();
			ZP_SAFE_RELEASE( m_pVB );
			m_pDevice->CreateVertexBuffer( m_uiVertNum*sizeof(TerrainVertex) , D3DUSAGE_WRITEONLY , 0 , D3DPOOL_DEFAULT , &m_pVB , NULL );
			ZP_ASSERT( NULL != m_pVB );

			//更新12个地形块的渲染操作的顶点缓冲区
			for( int i = 0 ; i < 12 ; i++ )
			{
				m_pVB->AddRef();
				ZP_SAFE_RELEASE( m_tileRenderOps[i]->m_pVB );
				m_tileRenderOps[i]->m_pVB = m_pVB;
			}

			m_pVB->AddRef();
			ZP_SAFE_RELEASE( m_pGapTileOp->m_pVB );
			m_pGapTileOp->m_pVB = m_pVB;

			m_pVB->AddRef();
			ZP_SAFE_RELEASE( m_pCenterTileOp->m_pVB );
			m_pCenterTileOp->m_pVB = m_pVB;

			m_pVB->AddRef();
			m_pVB->AddRef();
			m_pVB->AddRef();
			m_pVB->AddRef();

			ZP_SAFE_RELEASE( m_pLeftTopLTileOp->m_pVB );
			ZP_SAFE_RELEASE( m_pLeftBottomLTileOp->m_pVB );
			ZP_SAFE_RELEASE( m_pRightTopLTileOp->m_pVB );
			ZP_SAFE_RELEASE( m_pRightBottomLTileOp->m_pVB );

			m_pLeftTopLTileOp->m_pVB = m_pVB;
			m_pLeftBottomLTileOp->m_pVB = m_pVB;
			m_pRightTopLTileOp->m_pVB = m_pVB;
			m_pRightBottomLTileOp->m_pVB = m_pVB;

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
			m_pVB->Lock( 0 , uiLockSize , &pBuf , 0 );
				memcpy_s( pBuf , uiLockSize , pLevel->GetVerts() , uiLockSize );
			m_pVB->Unlock();
			pBuf = NULL;
		}

		//检查IB是否需要更新
		bool bTilesIBNeedUpdate = false;
		if( pLevel->GetIndicesNumPerTile() != m_uiIndiceNumPerTile )
		{
			bTilesIBNeedUpdate = true;
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

		if( bTilesIBNeedUpdate )
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

		//检查上下左右的四个修补条索引是否需要更新
		bool bGapTileIBNeedUpdate = false;
		if( pLevel->GetGapTileIndicesNum() != m_uiGapTileIndiceNum )
		{
			bGapTileIBNeedUpdate = true;
			m_uiGapTileIndiceNum = pLevel->GetGapTileIndicesNum();
			ZP_SAFE_RELEASE( m_pGapTileIB );
			m_pDevice->CreateIndexBuffer( 
				sizeof(unsigned short)*m_uiGapTileIndiceNum , D3DUSAGE_WRITEONLY , D3DFMT_INDEX16 , D3DPOOL_DEFAULT , &m_pGapTileIB , NULL );
			ZP_ASSERT( NULL != m_pGapTileIB );
			m_pGapTileIB->AddRef();
			ZP_SAFE_RELEASE( m_pGapTileOp->m_pIB );
			m_pGapTileOp->m_pIB = m_pGapTileIB;
		}

		if( bGapTileIBNeedUpdate )
		{
			unsigned short* pGapTileIndices = pLevel->GetGapTileIndices();
			void* pBuf = NULL;
			unsigned int uiLockSize = sizeof(unsigned short)*m_uiGapTileIndiceNum ;
			m_pGapTileIB->Lock( 0 ,uiLockSize, &pBuf , 0 );
			memcpy_s( pBuf ,uiLockSize, pGapTileIndices ,uiLockSize );
			m_pGapTileIB->Unlock();
		}
		m_uiGapTilePrimitiveNum = pLevel->GetGapTilePrimitiveNum();


		bool bCenterTileIBNeedUpdate = false;
		if( pLevel->GetCenterTileIndicesNum() != m_uiCenterTileIndiceNum )
		{
			bCenterTileIBNeedUpdate = true;
			m_uiCenterTileIndiceNum = pLevel->GetCenterTileIndicesNum();
			ZP_SAFE_RELEASE( m_pCenterTileIB );
			m_pDevice->CreateIndexBuffer( 
				sizeof(unsigned short)*m_uiCenterTileIndiceNum , D3DUSAGE_WRITEONLY , D3DFMT_INDEX16 , D3DPOOL_DEFAULT , &m_pCenterTileIB , NULL );
			ZP_ASSERT( NULL != m_pCenterTileIB );
			m_pCenterTileIB->AddRef();
			ZP_SAFE_RELEASE( m_pCenterTileOp->m_pIB );
			m_pCenterTileOp->m_pIB = m_pCenterTileIB;
		}

		if( bCenterTileIBNeedUpdate )
		{
			unsigned short* pCenterTileIndices = pLevel->GetCenterTileIndices();
			void* pBuf = NULL;
			unsigned int uiLockSize = sizeof(unsigned short)*m_uiCenterTileIndiceNum;
			m_pCenterTileIB->Lock( 0 ,uiLockSize, &pBuf , 0 );
			memcpy_s( pBuf ,uiLockSize, pCenterTileIndices ,uiLockSize );
			m_pCenterTileIB->Unlock();
		}
		m_uiCenterTilePrimitiveNum = pLevel->GetCenterTilePrimitiveNum();
		 
		bool bLTileIBNeedUpdate = false;
		if( pLevel->GetIndicesNumPerLTile() != m_uiIndiceNumPerLTile )
		{
			bLTileIBNeedUpdate = true;
			m_uiIndiceNumPerLTile = pLevel->GetIndicesNumPerLTile();
			ZP_SAFE_RELEASE( m_pLeftTopLTileIB );
			ZP_SAFE_RELEASE( m_pLeftBottomLTileIB );
			ZP_SAFE_RELEASE( m_pRightTopLTileIB  );
			ZP_SAFE_RELEASE( m_pRightBottomLTileIB );

			m_pDevice->CreateIndexBuffer( 
				sizeof(unsigned short)*m_uiIndiceNumPerLTile , D3DUSAGE_WRITEONLY , D3DFMT_INDEX16 , D3DPOOL_DEFAULT , &m_pLeftTopLTileIB , NULL );
			ZP_ASSERT( m_pLeftTopLTileIB );
			m_pDevice->CreateIndexBuffer( 
				sizeof(unsigned short)*m_uiIndiceNumPerLTile , D3DUSAGE_WRITEONLY , D3DFMT_INDEX16 , D3DPOOL_DEFAULT , &m_pLeftBottomLTileIB , NULL );
			ZP_ASSERT( m_pLeftBottomLTileIB );
			m_pDevice->CreateIndexBuffer( 
				sizeof(unsigned short)*m_uiIndiceNumPerLTile , D3DUSAGE_WRITEONLY , D3DFMT_INDEX16 , D3DPOOL_DEFAULT , &m_pRightTopLTileIB , NULL );
			ZP_ASSERT( m_pRightTopLTileIB );
			m_pDevice->CreateIndexBuffer( 
				sizeof(unsigned short)*m_uiIndiceNumPerLTile , D3DUSAGE_WRITEONLY , D3DFMT_INDEX16 , D3DPOOL_DEFAULT , &m_pRightBottomLTileIB , NULL );
			ZP_ASSERT( m_pRightBottomLTileIB );
			
			m_pLeftTopLTileIB->AddRef();
			m_pLeftBottomLTileIB->AddRef();
			m_pRightTopLTileIB->AddRef();
			m_pRightBottomLTileIB->AddRef();

			ZP_SAFE_RELEASE( m_pLeftTopLTileOp->m_pIB ); 
			ZP_SAFE_RELEASE( m_pLeftBottomLTileOp->m_pIB ); 
			ZP_SAFE_RELEASE( m_pRightTopLTileOp->m_pIB ); 
			ZP_SAFE_RELEASE( m_pRightBottomLTileOp->m_pIB ); 

			 m_pLeftTopLTileOp->m_pIB = m_pLeftTopLTileIB;
			 m_pLeftBottomLTileOp->m_pIB = m_pLeftBottomLTileIB;
			 m_pRightTopLTileOp->m_pIB = m_pRightTopLTileIB;
			 m_pRightBottomLTileOp->m_pIB = m_pRightBottomLTileIB;
		}

		if( bLTileIBNeedUpdate )
		{
			unsigned short* pLeftTopLTileIndices = pLevel->GetLeftTopLTileIndices();
			void* pBuf = NULL;
			unsigned int uiLockSize = sizeof(unsigned short)*m_uiIndiceNumPerLTile;
			m_pLeftTopLTileIB->Lock( 0 ,uiLockSize, &pBuf , 0 );
			memcpy_s( pBuf ,uiLockSize, pLeftTopLTileIndices ,uiLockSize );
			m_pLeftTopLTileIB->Unlock();

			unsigned short* pLeftBottomLTileIndices = pLevel->GetLeftBottomLTileIndices();
			pBuf = NULL;
			uiLockSize = sizeof(unsigned short)*m_uiIndiceNumPerLTile;
			m_pLeftBottomLTileIB->Lock( 0 ,uiLockSize, &pBuf , 0 );
			memcpy_s( pBuf ,uiLockSize, pLeftBottomLTileIndices ,uiLockSize );
			m_pLeftBottomLTileIB->Unlock();

			unsigned short* pRightTopLTileIndices = pLevel->GetRightTopLTileIndices();
			pBuf = NULL;
			uiLockSize = sizeof(unsigned short)*m_uiIndiceNumPerLTile;
			m_pRightTopLTileIB->Lock( 0 ,uiLockSize, &pBuf , 0 );
			memcpy_s( pBuf ,uiLockSize, pRightTopLTileIndices ,uiLockSize );
			m_pRightTopLTileIB->Unlock();

			unsigned short* pRightBottomLTileIndices = pLevel->GetRightBottomLTileIndices();
			pBuf = NULL;
			uiLockSize = sizeof(unsigned short)*m_uiIndiceNumPerLTile;
			m_pRightBottomLTileIB->Lock( 0 ,uiLockSize, &pBuf , 0 );
			memcpy_s( pBuf ,uiLockSize, pRightBottomLTileIndices ,uiLockSize );
			m_pRightBottomLTileIB->Unlock();
		}
		m_uiPrimitivePerLTile = pLevel->GetPrimitiveNumPerLTile();


		bool bFixedRingVBNeedUpdate = false;
		if( pLevel->GetFixedRingVertNum() != m_uiFixedRingVertNum )
		{
			bFixedRingVBNeedUpdate = true;
			m_uiFixedRingVertNum = pLevel->GetFixedRingVertNum();
			ZP_SAFE_RELEASE( m_pFixedRingVB );
			m_pDevice->
				CreateVertexBuffer( m_uiFixedRingVertNum*sizeof(TerrainVertex) , D3DUSAGE_WRITEONLY , 0 , D3DPOOL_DEFAULT , &m_pFixedRingVB , NULL );
			ZP_ASSERT( NULL != m_pFixedRingVB );

			m_pFixedRingVB->AddRef();
			ZP_SAFE_RELEASE( m_pFixedRingOp->m_pVB );
			m_pFixedRingOp->m_pVB = m_pFixedRingVB;

		}else{
			if( pLevel->TestFlag( VERTS_CHANGE ) )
			{
				bFixedRingVBNeedUpdate = true;
			}
		}

		if( bFixedRingVBNeedUpdate )
		{
			void* pBuf = NULL;
			unsigned int uiLockSize = m_uiFixedRingVertNum*sizeof(TerrainVertex) ;
			m_pFixedRingVB->Lock( 0 , uiLockSize , &pBuf , 0 );
			memcpy_s( pBuf , uiLockSize , pLevel->GetFixedRingVerts() , uiLockSize );
			m_pFixedRingVB->Unlock();
			pBuf = NULL;
		}

		bool bFixedRingIBNeedUpdate = false;
		if( pLevel->GetFixedRingIndicesNum() != m_uiFixedRingIndicesNum )
		{
			bFixedRingIBNeedUpdate = true;
			m_uiFixedRingIndicesNum = pLevel->GetFixedRingIndicesNum();
			ZP_SAFE_RELEASE( m_pFixedRingIB );
			m_pDevice->CreateIndexBuffer( 
				sizeof(unsigned short)*m_uiFixedRingIndicesNum , D3DUSAGE_WRITEONLY , D3DFMT_INDEX16 , D3DPOOL_DEFAULT , &m_pFixedRingIB , NULL );
			ZP_ASSERT( NULL != m_pFixedRingIB );
			m_pFixedRingIB->AddRef();
			ZP_SAFE_RELEASE( m_pFixedRingOp->m_pIB );
			m_pFixedRingOp->m_pIB = m_pFixedRingIB;
		}

		if( bFixedRingIBNeedUpdate )
		{
			unsigned short* pFixedRingIndices = pLevel->GetFixedRingIndices();
			void* pBuf = NULL;
			unsigned int uiLockSize = sizeof(unsigned short)*m_uiFixedRingIndicesNum;
			m_pFixedRingIB->Lock( 0 ,uiLockSize, &pBuf , 0 );
			memcpy_s( pBuf ,uiLockSize, pFixedRingIndices ,uiLockSize );
			m_pFixedRingIB->Unlock();
		}

		m_uiFixedRingPrimitiveNum = pLevel->GetFixedRingPrimitiveNum();

	}

	void TerrainLevelRenderData::GetRenderOps( std::vector<Render::D3DRenderOperation*>& opList )
	{
		Math::Matrix4 localMat = Math::Matrix4::MakeTranslationMatrix( m_v3LocalPos );
		 
		if( m_uiLevelCurrFlag & SHOW_OUTER_TILES )
		{ 
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

		if( m_uiLevelCurrFlag & SHOW_GAP_TILES )
		{ 
			m_pGapTileOp->m_worldMat = localMat;
			m_pGapTileOp->m_vertexCount = m_uiVertNum;
			m_pGapTileOp->m_stride = sizeof( TerrainVertex );
			m_pGapTileOp->m_primitiveType = D3DPT_TRIANGLESTRIP;
			m_pGapTileOp->m_primCount = m_uiGapTilePrimitiveNum;
			m_pGapTileOp->m_streamIndex = 0;
			opList.push_back( m_pGapTileOp );
		}

		if( m_uiLevelCurrFlag & SHOW_CENTER_TILE )
		{
			m_pCenterTileOp->m_worldMat = localMat;
			m_pCenterTileOp->m_vertexCount = m_uiVertNum;
			m_pCenterTileOp->m_stride = sizeof( TerrainVertex );
			m_pCenterTileOp->m_primitiveType = D3DPT_TRIANGLESTRIP;
			m_pCenterTileOp->m_primCount = m_uiCenterTilePrimitiveNum;
			m_pCenterTileOp->m_streamIndex = 0;
			opList.push_back( m_pCenterTileOp );
		}

		if( m_uiLevelCurrFlag & SHOW_LEFT_TOP_L_TILE )
		{
			m_pLeftTopLTileOp->m_worldMat = localMat;
			m_pLeftTopLTileOp->m_vertexCount = m_uiVertNum;
			m_pLeftTopLTileOp->m_stride = sizeof( TerrainVertex );
			m_pLeftTopLTileOp->m_primitiveType = D3DPT_TRIANGLESTRIP;
			m_pLeftTopLTileOp->m_primCount = m_uiPrimitivePerLTile;
			m_pLeftTopLTileOp->m_streamIndex = 0;
			opList.push_back( m_pLeftTopLTileOp );
		}else if( m_uiLevelCurrFlag & SHOW_LEFT_BOTTOM_L_TILE )
		{
			m_pLeftBottomLTileOp->m_worldMat = localMat;
			m_pLeftBottomLTileOp->m_vertexCount = m_uiVertNum;
			m_pLeftBottomLTileOp->m_stride = sizeof( TerrainVertex );
			m_pLeftBottomLTileOp->m_primitiveType = D3DPT_TRIANGLESTRIP;
			m_pLeftBottomLTileOp->m_primCount = m_uiPrimitivePerLTile;
			m_pLeftBottomLTileOp->m_streamIndex = 0;
			opList.push_back( m_pLeftBottomLTileOp );
		}else if( m_uiLevelCurrFlag & SHOW_RIGHT_TOP_L_TILE )
		{
			m_pRightTopLTileOp->m_worldMat = localMat;
			m_pRightTopLTileOp->m_vertexCount = m_uiVertNum;
			m_pRightTopLTileOp->m_stride = sizeof( TerrainVertex );
			m_pRightTopLTileOp->m_primitiveType = D3DPT_TRIANGLESTRIP;
			m_pRightTopLTileOp->m_primCount = m_uiPrimitivePerLTile;
			m_pRightTopLTileOp->m_streamIndex = 0;
			opList.push_back( m_pRightTopLTileOp );
		}else if( m_uiLevelCurrFlag & SHOW_RIGHT_BOTTOM_L_TILE )
		{
			m_pRightBottomLTileOp->m_worldMat = localMat;
			m_pRightBottomLTileOp->m_vertexCount = m_uiVertNum;
			m_pRightBottomLTileOp->m_stride = sizeof( TerrainVertex );
			m_pRightBottomLTileOp->m_primitiveType = D3DPT_TRIANGLESTRIP;
			m_pRightBottomLTileOp->m_primCount = m_uiPrimitivePerLTile;
			m_pRightBottomLTileOp->m_streamIndex = 0;
			opList.push_back( m_pRightBottomLTileOp );
		} 
		
		if( m_uiLevelCurrFlag & SHOW_FIXED_UP_RING )
		{
			m_pFixedRingOp->m_worldMat = localMat;
			m_pFixedRingOp->m_vertexCount = m_uiFixedRingVertNum;
			m_pFixedRingOp->m_stride = sizeof( TerrainVertex );
			m_pFixedRingOp->m_primitiveType = D3DPT_TRIANGLELIST;
			m_pFixedRingOp->m_primCount = m_uiFixedRingPrimitiveNum;
			m_pFixedRingOp->m_streamIndex = 0;
			opList.push_back( m_pFixedRingOp );
		}
	}

}