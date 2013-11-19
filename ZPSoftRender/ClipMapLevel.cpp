#include "ClipMapLevel.h"

namespace Terrain
{


	ClipMapLevel::ClipMapLevel(void):
	m_uiLevel(0),
	m_uiClipMapSize(0),
	m_uiGridSize(0),
	m_fGridWidth(0.0f),
	m_pVerts(NULL),
	m_ppTilesIndices(NULL),
	m_pGapTileIndices(NULL),
	m_pCenterTileIndices(NULL),
	m_uiFlag( SHOW_GAP_TILES| 
				    SHOW_FIXED_UP_RING )
{

}
 
ClipMapLevel::~ClipMapLevel(void)
{
	//释放顶点
	ZP_SAFE_DELETE_BUFFER( m_pVerts );

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
}
 
void ClipMapLevel::Init( const unsigned int uiLevel ,  const unsigned int uiClipMapSize ,  const float fGridWidth )
{
	m_uiLevel = uiLevel;
	m_uiClipMapSize = uiClipMapSize;
	m_uiGridSize = 1<<uiLevel;
	m_fGridWidth = fGridWidth*( (float)m_uiGridSize );
    m_heightMapBlock.Init( uiLevel , uiClipMapSize );
	//初始化顶点
	_InitVerts();
	//初始化12个子块索引
	_InitTilesIndices();
	//初始化上下左右4个修补块索引
	_InitGapTilesIndices();

	_InitCenterTileIndices();
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
		SetFlag( VERTS_CHANGE , true );
	}else{
		SetFlag( VERTS_CHANGE , false );
	}
}

void ClipMapLevel::_UpdateVerts( void )
{
	//使用高程更新网格点y值
	int stride = 1<<m_uiLevel;
	//for( unsigned int z = 0 ; z < m_uiClipMapSize ; z++ )
	//{
	//		int localz = m_currArea.minPos.z + stride*z;
	//		for( unsigned int x = 0 ; x < m_uiClipMapSize ; x++ )
	//		{
	//			unsigned int offset = z*m_uiClipMapSize+x;
	//			int localx = m_currArea.minPos.x + stride*x;		
	//			m_pVerts[offset].m_pos.y = m_heightMapBlock.Sample( localx>>m_uiLevel , localz>>m_uiLevel );
	//		
	//		}
	//}

	for( unsigned int z = 0 ; z < m_uiClipMapSize ; z++ )
	{ 
		for( unsigned int x = 0 ; x < m_uiClipMapSize ; x++ )
		{
			unsigned int offset = z*m_uiClipMapSize+x;
			m_pVerts[offset].m_pos.y = m_heightMapBlock.Sample( x , m_uiClipMapSize-1-z );
		}
	}
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


Math::Vec3 ClipMapLevel::GetLocalPos( void ) const
{
	return Math::Vec3( ((float)(m_centerPos.x>>m_uiLevel)) * m_fGridWidth , 0.0f , 
									((float)(m_centerPos.z>>m_uiLevel)) * m_fGridWidth );
}

bool ClipMapLevel::TestFlag( CLIPMAPLEVEL_FLAG bit )
{
	return (bool)( m_uiFlag&bit );
}

void ClipMapLevel::SetFlag( CLIPMAPLEVEL_FLAG bit , bool b )
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
 

}//namespace Terrain