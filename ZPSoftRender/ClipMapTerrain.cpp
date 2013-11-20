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
m_uiLevelNum(0)
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

	//决定绘制哪边的修补环
	unsigned int uiTopLevel = 0;
	for( unsigned int iLevel = uiTopLevel+1 ; iLevel < m_uiLevelNum; iLevel++ )
	{
		ClipMapGridPos prevLevelCenter = m_pLevels[iLevel-1].GetCenter();
		ClipMapGridPos currLevelCenter = m_pLevels[iLevel].GetCenter();
		int diffx = currLevelCenter.x - prevLevelCenter.x;
		int diffz = currLevelCenter.z - prevLevelCenter.z;
		
		if( diffx > 0 )
		{//在右边
			if( diffz > 0)
			{//在上面
				m_pLevels[iLevel].SetShowLTileFlag( SHOW_RIGHT_TOP_L_TILE );
			}else{//在下面
				m_pLevels[iLevel].SetShowLTileFlag( SHOW_RIGHT_BOTTOM_L_TILE );
			}
		}else{//在左边
			if( diffz > 0 )
			{//在上面
				m_pLevels[iLevel].SetShowLTileFlag( SHOW_LEFT_TOP_L_TILE );
			}else{//在下面
				m_pLevels[iLevel].SetShowLTileFlag( SHOW_LEFT_BOTTOM_L_TILE );
			}
		}
	}

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


}//namespace Terrain