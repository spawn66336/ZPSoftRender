#include "ClipMapLevel.h"

namespace Terrain
{


	ClipMapLevel::ClipMapLevel(void):
	m_uiLevel(0),
	m_uiClipMapSize(0),
	m_uiGridSize(0),
	m_fCenterHeight(0),
	m_fGridWidth(0.0f),
	m_pVerts(NULL),
	m_pFixedRingVerts(NULL),
	m_ppTilesIndices(NULL),
	m_pGapTileIndices(NULL),
	m_pCenterTileIndices(NULL),
	m_pLeftTopLTileIndices(NULL),
	m_pLeftBottomLTileIndices(NULL),
	m_pRightTopTileIndices(NULL),
	m_pRightBottomTileIndices(NULL),
	m_pFixedRingIndices(NULL),
	m_pNextLevel(NULL),
	m_uiFlag( SHOW_OUTER_TILES|
					SHOW_GAP_TILES| 
				    SHOW_FIXED_UP_RING )
{

}
 
ClipMapLevel::~ClipMapLevel(void)
{
	//释放顶点
	ZP_SAFE_DELETE_BUFFER( m_pVerts );
	//释放修补环顶点
	ZP_SAFE_DELETE_BUFFER( m_pFixedRingVerts );

	if( m_ppTilesIndices )
	{
		//释放索引
		for( int i = 0 ; i < 12 ; i++ )
		{
			ZP_SAFE_DELETE_BUFFER( m_ppTilesIndices[i] );
		}
		ZP_SAFE_DELETE_BUFFER( m_ppTilesIndices );
	}

	ZP_SAFE_DELETE_BUFFER( m_pGapTileIndices );
	ZP_SAFE_DELETE_BUFFER( m_pCenterTileIndices );

	ZP_SAFE_DELETE_BUFFER( m_pLeftTopLTileIndices );
	ZP_SAFE_DELETE_BUFFER( m_pLeftBottomLTileIndices );
	ZP_SAFE_DELETE_BUFFER( m_pRightTopTileIndices );
	ZP_SAFE_DELETE_BUFFER( m_pRightBottomTileIndices );

	ZP_SAFE_DELETE_BUFFER( m_pFixedRingIndices );
}
 
void ClipMapLevel::Init( const unsigned int uiLevel , const unsigned int uiClipMapSize ,  const float fGridWidth )
{
 

	m_uiLevel = uiLevel;
	m_uiClipMapSize = uiClipMapSize;
	m_uiGridSize = 1<<uiLevel;
	m_fGridWidth = fGridWidth*( (float)m_uiGridSize );
    m_heightMapBlock.Init( uiLevel , uiClipMapSize );
	//初始化顶点
	_InitVerts();
	//初始化修补环顶点
	_InitFixedRingVerts();
	//初始化12个子块索引
	_InitTilesIndices();
	//初始化上下左右4个修补块索引
	_InitGapTilesIndices();
	//构建中心块的索引
	_InitCenterTileIndices();
	//初始化L型修补块索引
	_InitLFixedTileIndices();

	_InitFixedRingIndices();

}

void ClipMapLevel::_InitVerts( void )
{
	m_pVerts = new TerrainVertex[m_uiClipMapSize*m_uiClipMapSize];
	//使当前块顶点的中心位置对准原点
	Math::Vec3 v3Origin( -m_fGridWidth*(m_uiClipMapSize>>1) , 0.0f , m_fGridWidth*(m_uiClipMapSize>>1) );
	Math::Vec3 v3Dx(m_fGridWidth,0.0f,0.0f);
	Math::Vec3 v3Dz( 0.0f,0.0f,-m_fGridWidth ); 
	for( unsigned int z = 0 ; z < m_uiClipMapSize ; z++ )
	{
		for( unsigned int x = 0 ; x < m_uiClipMapSize ; x++ )
		{
			unsigned int offset = z*m_uiClipMapSize+x;
			m_pVerts[offset].m_pos = v3Origin + v3Dx*(float)x + v3Dz*(float)z;
			m_pVerts[offset].m_norm = Math::Vec3( 0.0f,1.0f,0.0f);
			m_pVerts[offset].m_color = 0xffffffff;
		}
	}
}


void ClipMapLevel::_InitFixedRingVerts( void )
{
	unsigned int uiVertNum = m_uiClipMapSize*4 //内圈
											  +( ((m_uiClipMapSize-1)>>1) + 1)*4; //外圈粗糙层
	m_pFixedRingVerts = new TerrainVertex[uiVertNum];

	Math::Vec3 v3Origin( -m_fGridWidth*(m_uiClipMapSize>>1) , 0.0f , m_fGridWidth*(m_uiClipMapSize>>1) );
	Math::Vec3 v3Dx(m_fGridWidth,0.0f,0.0f);
	Math::Vec3 v3Dz( 0.0f,0.0f,-m_fGridWidth ); 

	unsigned int j = 0;
	//内环上面 （从左到右）
	for( unsigned int x = 0 ; x < m_uiClipMapSize ; x++ )
	{
		m_pFixedRingVerts[j].m_pos = v3Origin + v3Dx*((float)x);
		m_pFixedRingVerts[j].m_norm = Math::Vec3( 0.0f , 1.0f , 0.0f );
		m_pFixedRingVerts[j].m_color = 0xffffffff;
		j++;
	}

	//内环右侧 (从上到下)
	for( unsigned int z = 0 ; z < m_uiClipMapSize ; z++ )
	{
		m_pFixedRingVerts[j].m_pos = v3Origin + v3Dx*((float)(m_uiClipMapSize-1)) + v3Dz*((float)z);
		m_pFixedRingVerts[j].m_norm = Math::Vec3( 0.0f , 1.0f , 0.0f );
		m_pFixedRingVerts[j].m_color = 0xffffffff;
		j++;
	}

	//内环下面（从左到右）
	for( unsigned int x = 0 ; x < m_uiClipMapSize ; x++ )
	{
		m_pFixedRingVerts[j].m_pos = v3Origin + v3Dx*((float)x) + v3Dz*((float)(m_uiClipMapSize-1));
		m_pFixedRingVerts[j].m_norm = Math::Vec3( 0.0f , 1.0f , 0.0f );
		m_pFixedRingVerts[j].m_color = 0xffffffff;
		j++;
	}

	//内环左侧（从上到下）
	for( unsigned int z = 0 ; z < m_uiClipMapSize ; z++ )
	{
		m_pFixedRingVerts[j].m_pos = v3Origin + v3Dz*((float)z);
		m_pFixedRingVerts[j].m_norm = Math::Vec3( 0.0f , 1.0f , 0.0f );
		m_pFixedRingVerts[j].m_color = 0xffffffff;
		j++;
	}

	//外环上面（从左到右）
	for( unsigned int x = 0 ; x <  ((m_uiClipMapSize-1)>>1)+1 ; x++ )
	{
		m_pFixedRingVerts[j].m_pos = v3Origin + v3Dx*((float)x*2);
		m_pFixedRingVerts[j].m_norm = Math::Vec3( 0.0f , 1.0f , 0.0f );
		m_pFixedRingVerts[j].m_color = 0xffffffff;
		j++;
	}

	//外环右面（从上到下）
	for( unsigned int z = 0 ; z <  ((m_uiClipMapSize-1)>>1)+1 ; z++ )
	{
		m_pFixedRingVerts[j].m_pos = v3Origin + v3Dx*((float)(m_uiClipMapSize-1)) + v3Dz*((float)z*2);
		m_pFixedRingVerts[j].m_norm = Math::Vec3( 0.0f , 1.0f , 0.0f );
		m_pFixedRingVerts[j].m_color = 0xffffffff;
		j++;
	}

	//外环下面（从左到右）
	for( unsigned int x = 0 ; x < ((m_uiClipMapSize-1)>>1)+1 ; x++ )
	{
		m_pFixedRingVerts[j].m_pos = v3Origin + v3Dx*((float)x*2) + v3Dz*((float)(m_uiClipMapSize-1));
		m_pFixedRingVerts[j].m_norm = Math::Vec3( 0.0f , 1.0f , 0.0f );
		m_pFixedRingVerts[j].m_color = 0xffffffff;
		j++;
	}

	//外环左侧（从上到下）
	for( unsigned int z = 0 ; z <  ((m_uiClipMapSize-1)>>1)+1 ; z++ )
	{
		m_pFixedRingVerts[j].m_pos = v3Origin +  v3Dz*((float)z*2);
		m_pFixedRingVerts[j].m_norm = Math::Vec3( 0.0f , 1.0f , 0.0f );
		m_pFixedRingVerts[j].m_color = 0xffffffff;
		j++;
	}

	ZP_ASSERT( j == uiVertNum );
}


void ClipMapLevel::Update( const ClipMapGridPos& center )
{
	m_centerPos = center; 
	//使用更新过的中心坐标重新计算当前区域
	m_currArea.minPos.x = m_centerPos.x - ( m_uiClipMapSize>>1 ) * m_uiGridSize ;
	m_currArea.minPos.z = m_centerPos.z - ( m_uiClipMapSize>>1 ) * m_uiGridSize ;
	m_currArea.maxPos.x = m_currArea.minPos.x + ( m_uiClipMapSize - 1 )*m_uiGridSize;
	m_currArea.maxPos.z = m_currArea.minPos.z + ( m_uiClipMapSize - 1 )*m_uiGridSize;
	
	//更新当前层的高程图
	bool bChange = 
		m_heightMapBlock.Update( m_currArea );

	//若有变化更新顶点
	if( bChange )
	{
		_UpdateVerts();
		_UpdateFixedRingVerts();
		SetFlag( VERTS_CHANGE , true );
	}else{
		SetFlag( VERTS_CHANGE , false );
	}
}


void ClipMapLevel::_UpdateFixedRingVerts( void )
{
	//更新内圈顶点高度
	int stride = 1<<m_uiLevel;

	//更新内圈上方顶点高度 
	for( unsigned int x = 0 ; x < m_uiClipMapSize ; x++ )
	{
		int localz = m_currArea.maxPos.z;
		int localx = m_currArea.minPos.x + stride*x;
		m_pFixedRingVerts[x].m_pos.y = m_heightMapBlock.Sample( localx>>m_uiLevel , localz>>m_uiLevel );
		m_pFixedRingVerts[x].m_norm = m_heightMapBlock.SampleNormal( localx>>m_uiLevel , localz>>m_uiLevel );
	}
	//更新内圈右方顶点高度
	for( unsigned int z = 0 ; z < m_uiClipMapSize ; z++ )
	{
		int localx = m_currArea.minPos.x + stride*(m_uiClipMapSize-1);
		int localz = m_currArea.maxPos.z - stride*z;
		m_pFixedRingVerts[m_uiClipMapSize+z].m_pos.y = m_heightMapBlock.Sample( localx>>m_uiLevel , localz>>m_uiLevel );
		m_pFixedRingVerts[m_uiClipMapSize+z].m_norm = m_heightMapBlock.SampleNormal( localx>>m_uiLevel , localz>>m_uiLevel );
	}
	//更新内圈下方顶点高度
	for( unsigned int x = 0 ; x < m_uiClipMapSize ; x++ )
	{
		int localx = m_currArea.minPos.x + stride*x;
		int localz = m_currArea.minPos.z;
		m_pFixedRingVerts[2*m_uiClipMapSize+x].m_pos.y = m_heightMapBlock.Sample( localx>>m_uiLevel , localz>>m_uiLevel );
		m_pFixedRingVerts[2*m_uiClipMapSize+x].m_norm = m_heightMapBlock.SampleNormal( localx>>m_uiLevel , localz>>m_uiLevel );
	}
	//更新内圈左侧顶点高度
	for( unsigned int z = 0 ; z < m_uiClipMapSize ; z++ )
	{
		int localx = m_currArea.minPos.x;
		int localz = m_currArea.maxPos.z - stride*z;
		m_pFixedRingVerts[3*m_uiClipMapSize+z].m_pos.y = m_heightMapBlock.Sample( localx>>m_uiLevel , localz>>m_uiLevel );
		m_pFixedRingVerts[3*m_uiClipMapSize+z].m_norm = m_heightMapBlock.SampleNormal( localx>>m_uiLevel , localz>>m_uiLevel );
	}

	//若没有下一粗糙级别则不更新外圈
	if( !m_pNextLevel )
	{
		return;
	}

	unsigned int uiOutterVertsPerLine = ((m_uiClipMapSize-1)>>1) + 1;
	unsigned int uiOffset2OuterRing = m_uiClipMapSize*4; 
	unsigned int uiNextLevelStride = stride<<1;
	ClipMapHeightMapBlock& nextLevelHMBlockRef = m_pNextLevel->GetHeightMapBlock();
	
	//更新外环上面
	for( unsigned int x = 0 ; x < uiOutterVertsPerLine ; x++ )
	{
		int localx = m_currArea.minPos.x + uiNextLevelStride*x;
		int localz = m_currArea.maxPos.z;
		m_pFixedRingVerts[uiOffset2OuterRing+x].m_pos.y = 
			nextLevelHMBlockRef.Sample( localx>>(m_uiLevel+1) , localz>>(m_uiLevel+1) );
		m_pFixedRingVerts[uiOffset2OuterRing+x].m_norm = m_heightMapBlock.SampleNormal( localx>>(m_uiLevel+1) , localz>>(m_uiLevel+1) );
	}

	//更新外环右侧
	for( unsigned int z = 0 ; z < uiOutterVertsPerLine ; z++ )
	{
		int localx = m_currArea.maxPos.x;
		int localz = m_currArea.maxPos.z - uiNextLevelStride*z;
		m_pFixedRingVerts[uiOffset2OuterRing+uiOutterVertsPerLine+z].m_pos.y = 
			nextLevelHMBlockRef.Sample( localx>>(m_uiLevel+1) , localz>>(m_uiLevel+1) );
		m_pFixedRingVerts[uiOffset2OuterRing+uiOutterVertsPerLine+z].m_norm = 
			m_heightMapBlock.SampleNormal( localx>>(m_uiLevel+1) , localz>>(m_uiLevel+1) );
	}

	//更新外环下面
	for( unsigned int x = 0 ; x < uiOutterVertsPerLine ; x++ )
	{
		int localx = m_currArea.minPos.x + uiNextLevelStride*x;
		int localz = m_currArea.minPos.z;
		m_pFixedRingVerts[uiOffset2OuterRing+2*uiOutterVertsPerLine+x].m_pos.y = 
			nextLevelHMBlockRef.Sample( localx>>(m_uiLevel+1) , localz>>(m_uiLevel+1) );
		m_pFixedRingVerts[uiOffset2OuterRing+2*uiOutterVertsPerLine+x].m_norm = 
			m_heightMapBlock.SampleNormal( localx>>(m_uiLevel+1) , localz>>(m_uiLevel+1) );
	}

	//更新外环左侧
	for( unsigned int z = 0 ; z < uiOutterVertsPerLine ; z++ )
	{
		int localx = m_currArea.minPos.x;
		int localz = m_currArea.maxPos.z - uiNextLevelStride*z;
		m_pFixedRingVerts[uiOffset2OuterRing+3*uiOutterVertsPerLine+z].m_pos.y = 
			nextLevelHMBlockRef.Sample( localx>>(m_uiLevel+1) , localz>>(m_uiLevel+1) );
		m_pFixedRingVerts[uiOffset2OuterRing+3*uiOutterVertsPerLine+z].m_norm = 
			m_heightMapBlock.SampleNormal( localx>>(m_uiLevel+1) , localz>>(m_uiLevel+1) );
	}
}


void ClipMapLevel::_UpdateVerts( void )
{
	//使用高程更新网格点y值
	int stride = 1<<m_uiLevel;
	for( unsigned int z = 0 ; z < m_uiClipMapSize ; z++ )
	{
		int localz = m_currArea.maxPos.z - stride*z;
		for(  unsigned int x = 0 ; x < m_uiClipMapSize ; x++ )
		{
			unsigned int offset = z*m_uiClipMapSize+x;

			int localx = m_currArea.minPos.x + stride*x;		
			m_pVerts[offset].m_pos.y = m_heightMapBlock.Sample( localx>>m_uiLevel , localz>>m_uiLevel ); 
			m_pVerts[offset].m_norm = m_heightMapBlock.SampleNormal( localx>>m_uiLevel , localz>>m_uiLevel );
		}
	}

	//获得中心网格点高度
	m_fCenterHeight = m_heightMapBlock.Sample( m_centerPos.x>>m_uiLevel , m_centerPos.z>>m_uiLevel );
 
}

void ClipMapLevel::_InitTilesIndices( void )
{
	m_ppTilesIndices = new unsigned short*[12];
	unsigned int m = ( m_uiClipMapSize+1 )/4;
	//有m-1个条带，每个条带有2m+2个索引
	unsigned int indicesNum = (m-1)*(2*m+2);
	
	//为12个子块分配空间
	for( int i = 0 ; i < 12 ; i++ )
	{
		m_ppTilesIndices[i] = new unsigned short[indicesNum];
	}

	unsigned int usTileStartIndices[12] = {};
	usTileStartIndices[0] = 0;
	usTileStartIndices[1] = m-1;
	usTileStartIndices[2] = 2*(m-1)+2;
	usTileStartIndices[3] = 3*(m-1)+2;

	usTileStartIndices[4] = (m-1)*m_uiClipMapSize;
	usTileStartIndices[5] = (m-1)*(m_uiClipMapSize)+3*(m-1)+2;

	usTileStartIndices[6] = (2*(m-1)+2)*(m_uiClipMapSize);
	usTileStartIndices[7] = (2*(m-1)+2)*(m_uiClipMapSize)+3*(m-1)+2;

	usTileStartIndices[8] = (3*(m-1)+2)*m_uiClipMapSize;
	usTileStartIndices[9] = (3*(m-1)+2)*m_uiClipMapSize+m-1;
	usTileStartIndices[10] = (3*(m-1)+2)*m_uiClipMapSize+2*(m-1)+2;
	usTileStartIndices[11] = (3*(m-1)+2)*m_uiClipMapSize+3*(m-1)+2;

	//计算12个子块索引
	for( int i = 0 ; i < 12 ; i++ )
	{
		unsigned int j = 0;
		unsigned int uiCurrTileStartIndx = usTileStartIndices[i];
		unsigned short* pCurrTileIndices = m_ppTilesIndices[i];

		for( unsigned int iStrip = 0 ; iStrip < m-1 ; iStrip++ )
		{  
			pCurrTileIndices[j++] = uiCurrTileStartIndx+(iStrip+1)*m_uiClipMapSize;
			for( unsigned int iVert = 0 ; iVert <= m-1 ; iVert++ )
			{
				pCurrTileIndices[j++] = uiCurrTileStartIndx+(iStrip+1)*m_uiClipMapSize+iVert;
				pCurrTileIndices[j++] = uiCurrTileStartIndx+(iStrip)*m_uiClipMapSize+iVert;
			}
			pCurrTileIndices[j++] = uiCurrTileStartIndx+(iStrip)*m_uiClipMapSize+m-1;
		} 

		ZP_ASSERT( j == indicesNum );
	}//for( int i = 0 ; i < 12 ; i++ )
}


void ClipMapLevel::_InitGapTilesIndices( void )
{
	unsigned int m = ( m_uiClipMapSize+1 )/4;
	unsigned int indicesNum = ( ( m*2 ) + 2 )*2 * 4;
	m_pGapTileIndices = new unsigned short[indicesNum ];
	int j = 0;

	//上方的修补条
	unsigned int iStartIndex = 2*( m-1 );
	for( unsigned int iStrip = 0 ; iStrip < 2  ; iStrip++ )
	{
		unsigned int iStripStartIndex = iStartIndex + iStrip;
		m_pGapTileIndices[j++] = iStripStartIndex;
		for( unsigned int i = 0 ; i < m ; i++ )
		{
			m_pGapTileIndices[j++] = iStripStartIndex + i * m_uiClipMapSize;
			m_pGapTileIndices[j++] = iStripStartIndex + i * m_uiClipMapSize + 1;
		}
		m_pGapTileIndices[j++] = iStripStartIndex + ( m - 1 ) * m_uiClipMapSize + 1;
	}


	//下方的修补条
	iStartIndex = 2*( m-1 ) + ( (m-1)*3+2 )*m_uiClipMapSize;
	for( unsigned int iStrip = 0 ; iStrip < 2 ; iStrip++ )
	{
		unsigned int iStripStartIndex = iStartIndex + iStrip;
		m_pGapTileIndices[j++] = iStripStartIndex;
		for( unsigned int i = 0 ; i < m ; i++ )
		{
			m_pGapTileIndices[j++] = iStripStartIndex + i * m_uiClipMapSize;
			m_pGapTileIndices[j++] = iStripStartIndex + i * m_uiClipMapSize + 1;
		}
		m_pGapTileIndices[j++] = iStripStartIndex + ( m - 1 ) * m_uiClipMapSize + 1;
	}

	//左方的修补条
	iStartIndex = ( (m-1)*2+1 )*m_uiClipMapSize;
	for( unsigned int iStrip = 0 ; iStrip < 2 ; iStrip++ )
	{
		unsigned int iStripStartIndex = iStartIndex + iStrip*m_uiClipMapSize;
		m_pGapTileIndices[j++] = iStripStartIndex;
		for( unsigned int i = 0 ; i < m ; i++ )
		{
			m_pGapTileIndices[j++] = iStripStartIndex + i;
			m_pGapTileIndices[j++] = iStripStartIndex + i - m_uiClipMapSize;
		}
		m_pGapTileIndices[j++] = iStripStartIndex + m - 1 - m_uiClipMapSize;
	}

	//右方的修补条
	iStartIndex = ( (m-1)*2+1 )*m_uiClipMapSize + 3*(m-1)+2;
	for( unsigned int iStrip = 0 ; iStrip < 2 ; iStrip++ )
	{
		unsigned int iStripStartIndex = iStartIndex + iStrip*m_uiClipMapSize;
		m_pGapTileIndices[j++] = iStripStartIndex;
		for( unsigned int i = 0 ; i < m ; i++ )
		{
			m_pGapTileIndices[j++] = iStripStartIndex + i;
			m_pGapTileIndices[j++] = iStripStartIndex + i - m_uiClipMapSize;
		}
		m_pGapTileIndices[j++] = iStripStartIndex + m - 1 - m_uiClipMapSize;
	}

	ZP_ASSERT( indicesNum == j );
}


void ClipMapLevel::_InitCenterTileIndices( void )
{
	unsigned int m = ( m_uiClipMapSize+1 )/4;
	unsigned int indicesNum = ( ( 2*m+1 )*2+2 ) * //每个条带的索引数 
												( ( m - 1) *2 + 2); //条带数

	m_pCenterTileIndices = new unsigned short[indicesNum];

	int j = 0;
	unsigned int iStartIndex = m*m_uiClipMapSize + m-1;

	for( unsigned int iStrip = 0 ; iStrip < ( ( m - 1) *2 + 2 ) ; iStrip++ )
	{
		unsigned int iStripStartIndex = iStartIndex + iStrip*m_uiClipMapSize;

		m_pCenterTileIndices[j++] = iStripStartIndex;
		for( unsigned int i = 0 ; i < 2*m+1 ; i++ )
		{
			m_pCenterTileIndices[j++] = iStripStartIndex + i;
			m_pCenterTileIndices[j++] = iStripStartIndex + i - m_uiClipMapSize;
		}
		m_pCenterTileIndices[j++] = iStripStartIndex + 2*(m-1) + 2 - m_uiClipMapSize;
	}
	ZP_ASSERT( j == indicesNum );
}

void ClipMapLevel::_InitLFixedTileIndices( void )
{
	unsigned int m = ( m_uiClipMapSize+1 )/4;
	unsigned int indicesPerLTile = (2*m+1)*2+2 + 2*m*2+2;

	m_pLeftTopLTileIndices = new unsigned short[indicesPerLTile];
	m_pLeftBottomLTileIndices = new unsigned short[indicesPerLTile];
	m_pRightTopTileIndices = new unsigned short[indicesPerLTile];
	m_pRightBottomTileIndices = new unsigned short[indicesPerLTile];

	//构建左上 
	unsigned int j = 0;
	unsigned int iStripStartIndex = m*m_uiClipMapSize+m-1;
	m_pLeftTopLTileIndices[j++] = iStripStartIndex;
	for( unsigned int i = 0 ; i < 2*m+1 ; i++ )
	{
		m_pLeftTopLTileIndices[j++] = iStripStartIndex + i;
		m_pLeftTopLTileIndices[j++] = iStripStartIndex + i - m_uiClipMapSize;
	}
	m_pLeftTopLTileIndices[j++] = iStripStartIndex + 2*m - m_uiClipMapSize;

	iStripStartIndex = m*m_uiClipMapSize+m-1;
	m_pLeftTopLTileIndices[j++] = iStripStartIndex;
	for( unsigned int i = 0 ; i < 2*m ; i++ )
	{
		m_pLeftTopLTileIndices[j++] = iStripStartIndex+i*m_uiClipMapSize;
		m_pLeftTopLTileIndices[j++] = iStripStartIndex+i*m_uiClipMapSize+1;
	}
	m_pLeftTopLTileIndices[j++] = iStripStartIndex+(2*m-1)*m_uiClipMapSize+1;
	ZP_ASSERT( j == indicesPerLTile );

	//构建左下
	j = 0;
	iStripStartIndex = (3*(m-1)+2)*m_uiClipMapSize+m-1;
	m_pLeftBottomLTileIndices[j++] = iStripStartIndex;
	for( unsigned int i = 0 ; i < 2*m+1 ; i++ )
	{
		m_pLeftBottomLTileIndices[j++] = iStripStartIndex+i;
		m_pLeftBottomLTileIndices[j++] = iStripStartIndex+i-m_uiClipMapSize;
	}
	m_pLeftBottomLTileIndices[j++] = iStripStartIndex+2*m-m_uiClipMapSize;

	iStripStartIndex = (m-1)*m_uiClipMapSize+m-1;
	m_pLeftBottomLTileIndices[j++] = iStripStartIndex;
	for( unsigned int i = 0 ; i < 2*m ; i++ )
	{
		m_pLeftBottomLTileIndices[j++] = iStripStartIndex+i*m_uiClipMapSize;
		m_pLeftBottomLTileIndices[j++] = iStripStartIndex+i*m_uiClipMapSize+1;
	}
	m_pLeftBottomLTileIndices[j++] = iStripStartIndex+(2*m-1)*m_uiClipMapSize+1;
	ZP_ASSERT( j == indicesPerLTile );

	//构建右上
	j = 0;
	iStripStartIndex = m*m_uiClipMapSize+m-1;
	m_pRightTopTileIndices[j++] = iStripStartIndex;
	for( unsigned int i = 0 ; i < 2*m+1 ; i++ )
	{
		m_pRightTopTileIndices[j++] = iStripStartIndex + i;
		m_pRightTopTileIndices[j++] = iStripStartIndex + i - m_uiClipMapSize;
	}
	m_pRightTopTileIndices[j++] = iStripStartIndex + 2*m - m_uiClipMapSize;

	iStripStartIndex = m*m_uiClipMapSize+3*m-2; 
	m_pRightTopTileIndices[j++] = iStripStartIndex;
	for( unsigned int i = 0 ; i < 2*m ; i++ )
	{
		m_pRightTopTileIndices[j++] = iStripStartIndex+i*m_uiClipMapSize;
		m_pRightTopTileIndices[j++] = iStripStartIndex+i*m_uiClipMapSize+1;
	}
	m_pRightTopTileIndices[j++] = iStripStartIndex+(2*m-1)*m_uiClipMapSize+1;
	ZP_ASSERT( j == indicesPerLTile );

	//构建右下
	j = 0;
	iStripStartIndex = (3*(m-1)+2)*m_uiClipMapSize+m-1;
	m_pRightBottomTileIndices[j++] = iStripStartIndex;
	for( unsigned int i = 0 ; i < 2*m+1 ; i++ )
	{
		m_pRightBottomTileIndices[j++] = iStripStartIndex+i;
		m_pRightBottomTileIndices[j++] = iStripStartIndex+i-m_uiClipMapSize;
	}
	m_pRightBottomTileIndices[j++] = iStripStartIndex+2*m-m_uiClipMapSize;

	iStripStartIndex = (m-1)*m_uiClipMapSize+3*m-2;
	m_pRightBottomTileIndices[j++] = iStripStartIndex;
	for( unsigned int i = 0 ; i < 2*m ; i++ )
	{
		m_pRightBottomTileIndices[j++] = iStripStartIndex+i*m_uiClipMapSize;
		m_pRightBottomTileIndices[j++] = iStripStartIndex+i*m_uiClipMapSize+1;
	}
	m_pRightBottomTileIndices[j++] =  iStripStartIndex+(2*m-1)*m_uiClipMapSize+1;
	ZP_ASSERT( j == indicesPerLTile );
}
 
void ClipMapLevel::_InitFixedRingIndices( void )
{
	 unsigned int uiBlockNum = (m_uiClipMapSize-1)>>1;

	 unsigned int uiOffset2OuterRing = m_uiClipMapSize*4; 
	 unsigned int uiInnerRingOffset = m_uiClipMapSize;
	 unsigned int uiOutterRingOffset = ((m_uiClipMapSize-1)>>1) + 1; 

	 unsigned int uiIndicesNum = 4*uiBlockNum*9;
	 m_pFixedRingIndices = new unsigned short[uiIndicesNum];
	 memset( m_pFixedRingIndices , 0 , sizeof(unsigned short)*uiIndicesNum );
	 unsigned int j = 0 ;

	 //在组装前两个三角带时需要反转索引，目前不清楚为什么
	for( unsigned int iStrip = 0 ; iStrip < 2 ; iStrip++ )
	{
		 for( unsigned int i = 0 ; i < uiBlockNum ; i++ )
		 { 
			 m_pFixedRingIndices[j++] = uiOffset2OuterRing + iStrip*uiOutterRingOffset + i;
			 m_pFixedRingIndices[j++] = iStrip*uiInnerRingOffset +2*i;
			 m_pFixedRingIndices[j++] = iStrip*uiInnerRingOffset +2*i+1;


			 m_pFixedRingIndices[j++] = uiOffset2OuterRing+ iStrip*uiOutterRingOffset + i+1;
			 m_pFixedRingIndices[j++] = uiOffset2OuterRing+ iStrip*uiOutterRingOffset + i;
			 m_pFixedRingIndices[j++] = iStrip*uiInnerRingOffset + 2*i+1;


			 m_pFixedRingIndices[j++] = uiOffset2OuterRing+ iStrip*uiOutterRingOffset +i+1;
			 m_pFixedRingIndices[j++] = iStrip*uiInnerRingOffset +2*i+1;
			 m_pFixedRingIndices[j++] = iStrip*uiInnerRingOffset +2*i+2; 
		 }
	}

	//组装后两个三角带
	for( unsigned int iStrip = 2 ; iStrip < 4 ; iStrip++ )
	{
		for( unsigned int i = 0 ; i < uiBlockNum ; i++ )
		{
			m_pFixedRingIndices[j++] = iStrip*uiInnerRingOffset +2*i;
			m_pFixedRingIndices[j++] = uiOffset2OuterRing + iStrip*uiOutterRingOffset + i;
			m_pFixedRingIndices[j++] = iStrip*uiInnerRingOffset +2*i+1;

			m_pFixedRingIndices[j++] = uiOffset2OuterRing+ iStrip*uiOutterRingOffset + i;
			m_pFixedRingIndices[j++] = uiOffset2OuterRing+ iStrip*uiOutterRingOffset + i+1;
			m_pFixedRingIndices[j++] = iStrip*uiInnerRingOffset + 2*i+1;

			m_pFixedRingIndices[j++] = iStrip*uiInnerRingOffset +2*i+1;
			m_pFixedRingIndices[j++] = uiOffset2OuterRing+ iStrip*uiOutterRingOffset +i+1;
			m_pFixedRingIndices[j++] = iStrip*uiInnerRingOffset +2*i+2; 
		}
	}
 
	 //ZP_ASSERT( j == uiIndicesNum );
}

Math::Vec3 ClipMapLevel::GetLocalPos( void ) const
{
	return Math::Vec3( ((float)(m_centerPos.x>>m_uiLevel)) * m_fGridWidth , 0.0f , 
									((float)(m_centerPos.z>>m_uiLevel)) * m_fGridWidth );
}

bool ClipMapLevel::TestFlag( unsigned int bit )
{
	return (bool)( m_uiFlag&bit );
}

void ClipMapLevel::SetFlag( unsigned int bit , bool b )
{
	if( b )
	{
		m_uiFlag |= bit;
	}else{
		m_uiFlag &= ~bit;
	}
}


unsigned int ClipMapLevel::GetVertsNum( void ) const
{
	return m_uiClipMapSize*m_uiClipMapSize;
}

unsigned int ClipMapLevel::GetIndicesNumPerTile( void ) const
{
		unsigned int m = ( m_uiClipMapSize+1 )>>2;
		return (m-1)*(2*m+2);
}

unsigned int ClipMapLevel::GetPrimtiveNumPerTile( void ) const
{
	unsigned int m = ( m_uiClipMapSize+1 )>>2;
	return (m-2)*4 //条带间退化三角形
			   +2			//首尾各一个退化三角形
			   +(m-1)*(m-1)*2; 
 
}

unsigned int ClipMapLevel::GetGapTileIndicesNum( void ) const
{
	unsigned int m = ( m_uiClipMapSize+1 )/4;
	return  ( ( m*2 ) + 2 )*2 * 4;
}

unsigned int ClipMapLevel::GetGapTilePrimitiveNum( void ) const
{
	unsigned int m = ( m_uiClipMapSize+1 )/4;
	return ( m - 1 )*4*4 + 2 + 4*7;
}

unsigned int ClipMapLevel::GetCenterTileIndicesNum( void ) const
{
	unsigned int m = ( m_uiClipMapSize+1 )/4;
	return  ( ( 2*m+1 )*2+2 ) * //每个条带的索引数 
		( ( m - 1) *2 + 2); //条带数
}

unsigned int ClipMapLevel::GetCenterTilePrimitiveNum( void ) const
{
	unsigned int m = ( m_uiClipMapSize+1 )/4;
	return (2*(m-1)+2)*2*(2*(m-1)+2)+ //正常三角形数
				(2*(m-1)+1)*4 + 2;  //退化三角形数
}

unsigned int ClipMapLevel::GetIndicesNumPerLTile( void ) const
{
	unsigned int m = ( m_uiClipMapSize+1 )/4;
	return  (2*m+1)*2+2 + 2*m*2+2;
}

unsigned int ClipMapLevel::GetPrimitiveNumPerLTile( void ) const
{
	unsigned int m = ( m_uiClipMapSize+1 )/4;
	return 4+2+(2*(m-1)+2)*2+(2*m-1)*2;
}

unsigned int ClipMapLevel::GetFixedRingIndicesNum( void ) const
{
	unsigned int uiBlockNum = (m_uiClipMapSize-1)>>1; 
	return 4*uiBlockNum*9;
}

unsigned int ClipMapLevel::GetFixedRingPrimitiveNum( void ) const
{
	unsigned int uiBlockNum = (m_uiClipMapSize-1)>>1; 
	return 4*uiBlockNum*3;
}


void ClipMapLevel::SetShowLTileFlag( CLIPMAPLEVEL_FLAG bit )
{ 
	unsigned int m = 
		SHOW_LEFT_TOP_L_TILE|
		SHOW_LEFT_BOTTOM_L_TILE|
		SHOW_RIGHT_TOP_L_TILE|
		SHOW_RIGHT_BOTTOM_L_TILE;
	m_uiFlag &= ~m;
	m_uiFlag |= bit;
}

unsigned int ClipMapLevel::GetFixedRingVertNum( void ) const
{
		return m_uiClipMapSize*4 //内圈
		+( ((m_uiClipMapSize-1)>>1) + 1)*4; //外圈粗糙层
}

void ClipMapLevel::SetNextLevel( ClipMapLevel* pLevel )
{
	m_pNextLevel = pLevel;
}

void ClipMapLevel::ClearShowFlags( void )
{
	unsigned int m = 
		SHOW_OUTER_TILES|
		SHOW_CENTER_TILE|
		SHOW_GAP_TILES|
		SHOW_LEFT_TOP_L_TILE|
		SHOW_LEFT_BOTTOM_L_TILE|
		SHOW_RIGHT_TOP_L_TILE|
		SHOW_RIGHT_BOTTOM_L_TILE|
		SHOW_FIXED_UP_RING; 
	m_uiFlag &= ~m;
}






}//namespace Terrain