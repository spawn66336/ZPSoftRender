#include "ClipMapLevel.h"

namespace Terrain
{


ClipMapLevel::ClipMapLevel(void):
m_uiLevel(0),
m_uiClipMapSize(0),
m_uiGridSize(0),
m_fGridWidth(0.0f),
m_pVerts(NULL),
m_uiFlag(0)
{

}
 
ClipMapLevel::~ClipMapLevel(void)
{
	ZP_SAFE_DELETE_BUFFER( m_pVerts );
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
}

void ClipMapLevel::_InitVerts( void )
{
	m_pVerts = new TerrainVertex[m_uiClipMapSize*m_uiClipMapSize];
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
			m_pVerts[offset].m_color = Math::Vec4(1.0f,1.0f,1.0f,1.0f);
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
	
	bool bChange = 
		m_heightMapBlock.Update( m_currArea );

	//若有变化更新顶点
	if( bChange )
	{
		_UpdateVerts();
	}
}

void ClipMapLevel::_UpdateVerts( void )
{

}
 
}//namespace Terrain