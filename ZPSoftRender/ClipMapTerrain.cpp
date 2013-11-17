#include "ClipMapTerrain.h"
#include "ClipMapLevel.h"
#include "ClipMapReader.h"

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

	m_strMapName = strMapName;
	m_uiClipMapPow = uiClipMapPow;
	m_v3TerrainOrigin = v3TerrainOrigin;
	m_fGridWidth = fGridWidth;

	if( m_uiClipMapPow <  7 )
	{
		m_uiClipMapPow = 7;
	} 
	m_uiClipMapSize = ( 1<<uiClipMapPow )-1;

	ClipMapReader::CreateInstance();
	ClipMapReader::GetInstance()->Init( m_strMapName );

	m_uiLevelNum = 5;
	m_pLevels = new ClipMapLevel[m_uiLevelNum];
	
	for( unsigned int i = 0 ; i < m_uiLevelNum ; i++ )
	{
		m_pLevels[i].Init( i , m_uiClipMapSize , m_fGridWidth );
	}

	//初始化网格区域
	Update( Math::Vec3() ); 
}

void ClipMapTerrain::Destroy( void )
{
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


}

void ClipMapTerrain::Draw( Render::IRender* pRender )
{

}

Terrain::ClipMapGridPos ClipMapTerrain::_WorldPos2GridPos( const Math::Vec3& v3Pos  )
{
	Math::Vec3 v3TerrainCoord = v3Pos - m_v3TerrainOrigin;
	v3TerrainCoord /= m_fGridWidth;
	return ClipMapGridPos(  (int)floor( v3Pos.x ) ,  (int)floor( v3Pos.z ) ); 
}


}//namespace Terrain