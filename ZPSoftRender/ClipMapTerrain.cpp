#include "ClipMapTerrain.h"
#include "ClipMapLevel.h"
#include "ClipMapReader.h"
#include "D3DRenderOperation.h"
#include "ZPMathDependency.h"

namespace Terrain
{

	int Map2Odd( int x )
	{
		return ( x%2==0 ) ? (x-1):x;
	}

	//求余数
	int Mod( int x , int y )
	{
		return ( x>0 ) ? x%y : -x%y;
	}

ClipMapTerrain::ClipMapTerrain(void):
m_pDevice(0),
m_uiClipMapSize(0),
m_uiClipMapPow(0),
m_fGridWidth(1.0f),
m_pLevels(NULL),
m_uiLevelNum(0),
m_uiShowMask(TERRAIN_MASK_SHOW_ALL)
{
}


ClipMapTerrain::~ClipMapTerrain(void)
{
}

void ClipMapTerrain::Init(  
	LPDIRECT3DDEVICE9 pDevice ,
	const String& strMapName , 
	const unsigned int uiClipMapPow , 
	const Math::Vec3& v3TerrainOrigin , 
	const float fGridWidth )
{
	//若已经初始化则退出
	if( m_pLevels )
	{
		return;
	}

	m_pDevice = pDevice;
	m_strMapName = strMapName;
	m_uiClipMapPow = uiClipMapPow;
	m_v3TerrainOrigin = v3TerrainOrigin;
	m_fGridWidth = fGridWidth;

	if( m_uiClipMapPow <  7 )
	{
		m_uiClipMapPow = 7;
	} 
	m_uiClipMapSize = ( 1<<m_uiClipMapPow )-1;

	ClipMapReader::CreateInstance();
	ClipMapReader::GetInstance()->Init( m_strMapName );

	m_renderDataCache.Init( pDevice );

	m_uiLevelNum = 5;
	m_pLevels = new ClipMapLevel[m_uiLevelNum];
	
	//使最顶层 渲染中心区块
	m_pLevels[0].SetFlag( SHOW_CENTER_TILE , true );

	for( unsigned int i = 0 ; i < m_uiLevelNum ; i++ )
	{
		m_pLevels[i].Init( i , m_uiClipMapSize , m_fGridWidth );
	}
	//链接所有层
	for( unsigned int i = 0 ; i < m_uiLevelNum-1 ; i++ )
	{
		m_pLevels[i].SetNextLevel( &m_pLevels[i+1] );
	}

	m_pLevels[m_uiLevelNum-1].SetFlag( SHOW_FIXED_UP_RING , false );

	//初始化网格区域
	Update( Math::Vec3() ); 
}

void ClipMapTerrain::Destroy( void )
{
	m_renderDataCache.Destroy();
	m_uiLevelNum = 0;
	ZP_SAFE_DELETE_BUFFER( m_pLevels ); 
	ClipMapReader::GetInstance()->Destroy();
	ClipMapReader::DestroyInstance();
}

 
void ClipMapTerrain::Update(  const Math::Vec3 v3CamPos  )
{
	m_v3CamPos = v3CamPos;
	//将相机的世界坐标位置转换为地形格点坐标位置
	m_CamPos = _WorldPos2GridPos( m_v3CamPos );
	
	unsigned int uiCurrLevelGirdSize = 1;
	int iPrevLevelCenterX = m_CamPos.x;
	int iPrevLevelCenterZ = m_CamPos.z;
	for( unsigned int iLevel = 0 ; iLevel < m_uiLevelNum ; iLevel++ )
	{ 
		int iCurrLevelCenterX = Map2Odd( iPrevLevelCenterX );
		int iCurrLevelCenterZ = Map2Odd( iPrevLevelCenterZ );
		ClipMapGridPos currLevelCenter( iCurrLevelCenterX*uiCurrLevelGirdSize , iCurrLevelCenterZ*uiCurrLevelGirdSize );

		m_pLevels[iLevel].Update( currLevelCenter ); 

		unsigned int uiNextLevelGridSize = uiCurrLevelGirdSize<<1;
		iPrevLevelCenterX = ( currLevelCenter.x + Mod( currLevelCenter.x , uiNextLevelGridSize ) ) / uiNextLevelGridSize;
		iPrevLevelCenterZ = ( currLevelCenter.z + Mod( currLevelCenter.z , uiNextLevelGridSize ) ) / uiNextLevelGridSize;
		uiCurrLevelGirdSize = uiNextLevelGridSize;
	} 

	//清空所有显示标志
	for( unsigned int iLevel = 0 ; iLevel < m_uiLevelNum ; iLevel++ )
	{
		m_pLevels[iLevel].ClearShowFlags(); 
	}

	unsigned int uiTopLevel = 0;
	float fLevelWidth = m_fGridWidth*((float)m_uiClipMapSize);
	for( unsigned int iLevel = 0 ; iLevel < m_uiLevelNum ; iLevel++ )
	{
		if( 2.5f*( v3CamPos.y - m_pLevels[iLevel].GetCenterHeight() ) >
			fLevelWidth*((float)(1<<iLevel)) )
		{ 
			uiTopLevel++;
		}else{
			break;
		}
	}

	if( uiTopLevel >= m_uiLevelNum )
	{
		uiTopLevel = m_uiLevelNum-1;
	}

	unsigned int uiTopLevelShowFlags = SHOW_OUTER_TILES|
																SHOW_CENTER_TILE |
																SHOW_GAP_TILES|
																SHOW_FIXED_UP_RING ;

		m_pLevels[uiTopLevel].SetFlag( uiTopLevelShowFlags&m_uiShowMask , true );
	 

	//决定绘制哪边的修补环
	for( unsigned int iLevel = uiTopLevel+1 ; iLevel < m_uiLevelNum; iLevel++ )
	{
		m_pLevels[iLevel].SetFlag( (SHOW_OUTER_TILES|SHOW_GAP_TILES|SHOW_FIXED_UP_RING)&m_uiShowMask , true );

		ClipMapGridPos prevLevelCenter = m_pLevels[iLevel-1].GetCenter();
		ClipMapGridPos currLevelCenter = m_pLevels[iLevel].GetCenter();
		int diffx = currLevelCenter.x - prevLevelCenter.x;
		int diffz = currLevelCenter.z - prevLevelCenter.z;
		
		if( diffx > 0 )
		{//在右边
			if( diffz > 0)
			{//在上面
				m_pLevels[iLevel].SetShowLTileFlag( SHOW_RIGHT_TOP_L_TILE&m_uiShowMask );
			}else{//在下面
				m_pLevels[iLevel].SetShowLTileFlag( SHOW_RIGHT_BOTTOM_L_TILE&m_uiShowMask );
			}
		}else{//在左边
			if( diffz > 0 )
			{//在上面
				m_pLevels[iLevel].SetShowLTileFlag( SHOW_LEFT_TOP_L_TILE&m_uiShowMask );
			}else{//在下面
				m_pLevels[iLevel].SetShowLTileFlag( SHOW_LEFT_BOTTOM_L_TILE&m_uiShowMask );
			}
		}
	}
	 

	//更新地形渲染数据
	for( unsigned int iLevel = 0 ; iLevel < m_uiLevelNum ; iLevel++ )
	{
		m_renderDataCache.UpdateTerrainLevelRenderData( &m_pLevels[iLevel] );
	}
} 
 
Terrain::ClipMapGridPos ClipMapTerrain::_WorldPos2GridPos( const Math::Vec3& v3Pos  )
{
	Math::Vec3 v3TerrainCoord = v3Pos - m_v3TerrainOrigin;
	v3TerrainCoord /= m_fGridWidth;
	return ClipMapGridPos(  (int)floor( v3TerrainCoord.x ) ,  (int)floor( v3TerrainCoord.z ) ); 
}

void ClipMapTerrain::OnLostDevice( void )
{
	m_renderDataCache.OnLostDevice();
}

void ClipMapTerrain::OnResetDevice( void )
{
	m_renderDataCache.OnResetDevice();
}

void ClipMapTerrain::GetRenderOps( std::vector<Render::D3DRenderOperation*>& opList )
{
	m_renderDataCache.GetRenderOps( m_tmpOpList ); 
	auto itOp = m_tmpOpList.begin();
	while( itOp != m_tmpOpList.end() )
	{
		(*itOp)->m_worldMat = 
			Math::Matrix4::MakeTranslationMatrix( m_v3TerrainOrigin )*(*itOp)->m_worldMat;
		opList.push_back( *itOp );
		++itOp;
	}
	m_tmpOpList.clear();
}

void ClipMapTerrain::CameraCollision( const Math::Vec3& v3CamPos , Math::Vec3& v3FinalPos )
{
	ClipMapGridPos camLBGridPos = _WorldPos2GridPos( m_v3CamPos );
	ClipMapGridPos camLTGridPos( camLBGridPos.x , camLBGridPos.z+1 );
	ClipMapGridPos camRTGridPos( camLBGridPos.x+1 , camLBGridPos.z+1 );
	ClipMapGridPos camRBGridPos( camLBGridPos.x+1 , camLBGridPos.z );

	Math::Vec3 v3TerrainCoord = v3CamPos - m_v3TerrainOrigin; 
	v3TerrainCoord.x /= m_fGridWidth;
	v3TerrainCoord.z /= m_fGridWidth;

	float fHeightLB = 0.0f , fHeightLT = 0.0f , fHeightRT = 0.0f , fHeightRB = 0.0f;
	ClipMapReader::GetInstance()->Sample( camLBGridPos.x , camLBGridPos.z , fHeightLB );
	ClipMapReader::GetInstance()->Sample( camLTGridPos.x , camLTGridPos.z , fHeightLT );
	ClipMapReader::GetInstance()->Sample( camRTGridPos.x , camRTGridPos.z , fHeightRT );
	ClipMapReader::GetInstance()->Sample( camRBGridPos.x , camRBGridPos.z , fHeightRB );

	float fU = v3TerrainCoord.x - (float)camLBGridPos.x ;
	float fV = v3TerrainCoord.z - (float)camLBGridPos.z;
	float fOneMiusU = 1.0f - fU;
	float fOneMiusV = 1.0f - fV;
	
	float fFinalHeight = 
		fOneMiusU*fOneMiusV*fHeightLB + 
		fU*fOneMiusV*fHeightRB+
		fU*fV*fHeightRT+
		fOneMiusU*fV*fHeightLT
		;
	float fOffset = 10.0f;
	if( v3CamPos.y < fFinalHeight+fOffset )
	{
		v3FinalPos = Math::Vec3( v3CamPos.x , fFinalHeight+fOffset , v3CamPos.z ); 
	}else{
		v3FinalPos = v3CamPos;
	}
}
 

}//namespace Terrain